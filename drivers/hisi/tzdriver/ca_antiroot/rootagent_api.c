/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2022. All rights reserved.
 * Description: rootagent API
 * Create: 2016-04-01
 */

#include "rootagent_api.h"

#include <linux/lsm_hooks.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/version.h>

#include <asm/sections.h>
#include <asm/syscall.h>

#include "rootagent.h"
#include "rootagent_common.h"
#include "rootagent_crypto.h"
#include "rootagent_mem.h"
#include "teek_client_api.h"
#include "teek_client_id.h"

static TEEC_Context g_context;
static TEEC_Session g_session;
static int g_inited; /* init flag, 0:uninitialized, 1:initialized */
static int g_count;  /* init count */
static TEEC_UUID g_uuid = UUID_RM_TA; /* TA service ID */
/* for invokecmd params[0].tmpref.buffer */
static TEEC_TempMemoryReference g_mem;
static DEFINE_MUTEX(root_monitor_lock);

/* for cbc crypto buffer: rm_command_in_out */
static TEEC_TempMemoryReference g_swap_mem;
static uint8_t g_chal_req_key[CHALLENGE_REQ_KEY_LENGTH];
static uint8_t g_chal_key[CHALLENGE_KEY_LENGTH];
static uint8_t g_nounce[CHALLENGE_NOUNCE_LENGTH];

static int g_root_flag = REV_NOT_ROOT;
static uint32_t g_tee_scan_status = REV_NOT_ROOT;
static u8 g_package_name[] = "antiroot-ca";
static int g_se_hooks_num;

/* only consider rootscan bits form tee(but rootproc) */
static uint32_t g_tee_valid_bits = ((1 << KERNELCODEBIT) |
				(1 << SYSTEMCALLBIT) |
				(1 << SESTATUSBIT) | (1 << SEHOOKBIT) |
				(1 << SETIDBIT) | (1 << RODATABIT));

uint32_t get_tee_status(void)
{
	return (g_tee_scan_status & g_tee_valid_bits);
}

void root_monitor_finalize(void)
{
	mutex_lock(&root_monitor_lock);
	rm_mem_destroy();
	g_mem.buffer = NULL;
	g_mem.size = 0;
	g_swap_mem.buffer = NULL;
	g_swap_mem.size = 0;
	TEEK_CloseSession(&g_session);
	TEEK_FinalizeContext(&g_context);
	g_inited = 0;
	g_count = 0;
	mutex_unlock(&root_monitor_lock);
}

static int not_init_agent(void)
{
	return ((g_inited == 0) || (g_mem.buffer == NULL));
}

static TEEC_Result rootagent_open_session(void)
{
	uint32_t origin = 0;
	u32 root_id = 0;
	TEEC_Result teec_ret;
	TEEC_Operation operation;

	teec_ret = TEEK_InitializeContext(NULL, &g_context);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("rootagent initialize context failed\n");
		return teec_ret;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	/*
	 * operation.params is a array which saves the message sending to TA,
	 * it's total length is 4, so params[2] or params[3] is third or fourth
	 * params and paramTypes is TEEC_MEMREF_TEMP_INPUT
	 */
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	operation.params[3].tmpref.buffer = (void *)(g_package_name);
	operation.params[3].tmpref.size = strlen(g_package_name) + 1; /* lint !e64 */
	teec_ret = TEEK_OpenSession(&g_context, &g_session, &g_uuid,
			TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("root agent open session failed\n");
		TEEK_FinalizeContext(&g_context);
	}

	return teec_ret;
}

TEEC_Result root_monitor_tee_init(void)
{
	TEEC_Result teec_ret;

	if ((get_ro_secure() != 0) && (g_root_flag != REV_NOT_ROOT)) {
		antiroot_error("device is rooted! ro secure: %d, root flag: %d\n",
			get_ro_secure(), g_root_flag);
		return TEE_ERROR_ANTIROOT_RSP_FAIL; /* lint !e570 */
	}
	mutex_lock(&root_monitor_lock);
	if (g_inited != 0) {
		antiroot_debug("RootAgent has already initialized");
		mutex_unlock(&root_monitor_lock);
		return TEEC_SUCCESS;
	}
	teec_ret = rm_mem_init();
	if (teec_ret != 0) {
		mutex_unlock(&root_monitor_lock);
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	if (initial_rm_shared_mem(&g_mem, &g_swap_mem) != 0) {
		rm_mem_destroy();
		mutex_unlock(&root_monitor_lock);
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	teec_ret = rootagent_open_session();
	if (teec_ret != TEEC_SUCCESS)
		goto rm_mem_free;

	g_inited = 1;
	g_count++;
	antiroot_debug("root_monitor_tee_init ok, initialized count: %d, g_inited: %d\n",
		g_count, g_inited);
	mutex_unlock(&root_monitor_lock);
	return TEEC_SUCCESS;

rm_mem_free:
	rm_mem_destroy();
	mutex_unlock(&root_monitor_lock);
	return teec_ret;
}

static int get_sescan_address(phys_addr_t *phy_addreses, int used_length)
{
	phys_addr_t count = 1;

	if (phy_addreses == NULL) {
		antiroot_error("get sescan address phy_addreses error\n");
		return TEEC_ERROR_GENERIC;
	}

	/*
	 * COLLECT PHY ADDRESS FOR SE_FUNCTIONS
	 * return in do_gen_phy_addr macro has no security risk
	 */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 16, 0)
	#define do_gen_phy_addr(FUNC) \
	do { \
		struct security_hook_list *p = NULL; \
		list_for_each_entry(p, &security_hook_heads.FUNC, list) { \
			phy_addreses[count] = __pa_symbol(&(p->hook.FUNC)); \
			count++; \
			if (((count + used_length) * sizeof(phys_addr_t)) > g_mem.size) \
				return TEEC_ERROR_GENERIC; \
		} \
	} while (0)
#else
	#define do_gen_phy_addr(FUNC) \
	do { \
		struct security_hook_list *p = NULL; \
		hlist_for_each_entry(p, &security_hook_heads.FUNC, list) { \
			phy_addreses[count] = __pa_symbol(&(p->hook.FUNC)); \
			count++; \
			if (((count + used_length) * sizeof(phys_addr_t)) > g_mem.size) \
				return TEEC_ERROR_GENERIC; \
		} \
	} while (0)
#endif

	/*
	 * reference initialization in security_hook_heads
	 * in security/security.c
	 */
	do_gen_phy_addr(binder_set_context_mgr);
	do_gen_phy_addr(binder_transaction);
	do_gen_phy_addr(binder_transfer_binder);
	do_gen_phy_addr(binder_transfer_file);
	do_gen_phy_addr(ptrace_access_check);
	do_gen_phy_addr(ptrace_traceme);
	do_gen_phy_addr(capget);
	do_gen_phy_addr(capset);
	do_gen_phy_addr(capable);
	do_gen_phy_addr(quotactl);
	do_gen_phy_addr(quota_on);
	do_gen_phy_addr(syslog);
	do_gen_phy_addr(settime);
	do_gen_phy_addr(vm_enough_memory);
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
	do_gen_phy_addr(bprm_set_creds);
#else
	do_gen_phy_addr(bprm_creds_for_exec);
	do_gen_phy_addr(bprm_creds_from_file);
	do_gen_phy_addr(inode_init_security_anon);
	do_gen_phy_addr(kernel_post_load_data);
	do_gen_phy_addr(task_fix_setgid);
#if defined(CONFIG_SECURITY) && defined(CONFIG_WATCH_QUEUE)
	do_gen_phy_addr(post_notification);
#endif

#if defined(CONFIG_SECURITY) && defined(CONFIG_KEY_NOTIFICATIONS)
	do_gen_phy_addr(watch_key);
#endif

#ifdef CONFIG_PERF_EVENTS
	do_gen_phy_addr(perf_event_open);
	do_gen_phy_addr(perf_event_alloc);
	do_gen_phy_addr(perf_event_free);
	do_gen_phy_addr(perf_event_read);
	do_gen_phy_addr(perf_event_write);
#endif
#endif // LINUX_VERSION_CODE
	do_gen_phy_addr(bprm_check_security);
	do_gen_phy_addr(bprm_committing_creds);
	do_gen_phy_addr(bprm_committed_creds);
	do_gen_phy_addr(sb_alloc_security);
	do_gen_phy_addr(sb_free_security);
	do_gen_phy_addr(sb_remount);
	do_gen_phy_addr(sb_kern_mount);
	do_gen_phy_addr(sb_show_options);
	do_gen_phy_addr(sb_statfs);
	do_gen_phy_addr(sb_mount);
	do_gen_phy_addr(sb_umount);
	do_gen_phy_addr(sb_pivotroot);
	do_gen_phy_addr(sb_set_mnt_opts);
	do_gen_phy_addr(sb_clone_mnt_opts);
	do_gen_phy_addr(dentry_init_security);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 19, 0)
	do_gen_phy_addr(sb_copy_data);
	do_gen_phy_addr(sb_parse_opts_str);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
	do_gen_phy_addr(fs_context_dup);
	do_gen_phy_addr(fs_context_parse_param);
	do_gen_phy_addr(sb_free_mnt_opts);
	do_gen_phy_addr(sb_eat_lsm_opts);
	do_gen_phy_addr(sb_add_mnt_opt);
	do_gen_phy_addr(move_mount);
	do_gen_phy_addr(dentry_create_files_as);
	do_gen_phy_addr(path_notify);
	do_gen_phy_addr(inode_copy_up);
	do_gen_phy_addr(inode_copy_up_xattr);
	do_gen_phy_addr(kernfs_init_security);
	do_gen_phy_addr(cred_getsecid);
	do_gen_phy_addr(kernel_load_data);
	do_gen_phy_addr(kernel_read_file);
	do_gen_phy_addr(kernel_post_read_file);
	do_gen_phy_addr(kernel_module_request);
	do_gen_phy_addr(inode_invalidate_secctx);
	do_gen_phy_addr(locked_down);
#ifdef CONFIG_SECURITY_NETWORK
	do_gen_phy_addr(socket_socketpair);
	do_gen_phy_addr(sctp_assoc_request);
	do_gen_phy_addr(sctp_bind_connect);
	do_gen_phy_addr(sctp_sk_clone);
#endif
#ifdef CONFIG_BPF_SYSCALL
	do_gen_phy_addr(bpf);
	do_gen_phy_addr(bpf_map);
	do_gen_phy_addr(bpf_prog);
	do_gen_phy_addr(bpf_map_alloc_security);
	do_gen_phy_addr(bpf_map_free_security);
	do_gen_phy_addr(bpf_prog_alloc_security);
	do_gen_phy_addr(bpf_prog_free_security);
#endif
#endif  /* LINUX_VERSION_CODE (5, 4, 0) */
#ifdef CONFIG_SECURITY_PATH
	do_gen_phy_addr(path_unlink);
	do_gen_phy_addr(path_mkdir);
	do_gen_phy_addr(path_rmdir);
	do_gen_phy_addr(path_mknod);
	do_gen_phy_addr(path_truncate);
	do_gen_phy_addr(path_symlink);
	do_gen_phy_addr(path_link);
	do_gen_phy_addr(path_rename);
	do_gen_phy_addr(path_chmod);
	do_gen_phy_addr(path_chown);
	do_gen_phy_addr(path_chroot);
#endif
	do_gen_phy_addr(inode_alloc_security);
	do_gen_phy_addr(inode_free_security);
	do_gen_phy_addr(inode_init_security);
	do_gen_phy_addr(inode_create);
	do_gen_phy_addr(inode_link);
	do_gen_phy_addr(inode_unlink);
	do_gen_phy_addr(inode_symlink);
	do_gen_phy_addr(inode_mkdir);
	do_gen_phy_addr(inode_rmdir);
	do_gen_phy_addr(inode_mknod);
	do_gen_phy_addr(inode_rename);
	do_gen_phy_addr(inode_readlink);
	do_gen_phy_addr(inode_follow_link);
	do_gen_phy_addr(inode_permission);
	do_gen_phy_addr(inode_setattr);
	do_gen_phy_addr(inode_getattr);
	do_gen_phy_addr(inode_setxattr);
	do_gen_phy_addr(inode_post_setxattr);
	do_gen_phy_addr(inode_getxattr);
	do_gen_phy_addr(inode_listxattr);
	do_gen_phy_addr(inode_removexattr);
	do_gen_phy_addr(inode_need_killpriv);
	do_gen_phy_addr(inode_killpriv);
	do_gen_phy_addr(inode_getsecurity);
	do_gen_phy_addr(inode_setsecurity);
	do_gen_phy_addr(inode_listsecurity);
	do_gen_phy_addr(inode_getsecid);
	do_gen_phy_addr(file_permission);
	do_gen_phy_addr(file_alloc_security);
	do_gen_phy_addr(file_free_security);
	do_gen_phy_addr(file_ioctl);
	do_gen_phy_addr(mmap_addr);
	do_gen_phy_addr(mmap_file);
	do_gen_phy_addr(file_mprotect);
	do_gen_phy_addr(file_lock);
	do_gen_phy_addr(file_fcntl);
	do_gen_phy_addr(file_set_fowner);
	do_gen_phy_addr(file_send_sigiotask);
	do_gen_phy_addr(file_receive);
	do_gen_phy_addr(file_open);
	do_gen_phy_addr(task_alloc);
	do_gen_phy_addr(task_free);
	do_gen_phy_addr(cred_alloc_blank);
	do_gen_phy_addr(cred_free);
	do_gen_phy_addr(cred_prepare);
	do_gen_phy_addr(cred_transfer);
	do_gen_phy_addr(kernel_act_as);
	do_gen_phy_addr(kernel_create_files_as);
	do_gen_phy_addr(task_fix_setuid);
	do_gen_phy_addr(task_setpgid);
	do_gen_phy_addr(task_getpgid);
	do_gen_phy_addr(task_getsid);
	do_gen_phy_addr(task_getsecid);
	do_gen_phy_addr(task_setnice);
	do_gen_phy_addr(task_setioprio);
	do_gen_phy_addr(task_getioprio);
	do_gen_phy_addr(task_prlimit);
	do_gen_phy_addr(task_setrlimit);
	do_gen_phy_addr(task_setscheduler);
	do_gen_phy_addr(task_getscheduler);
	do_gen_phy_addr(task_movememory);
	do_gen_phy_addr(task_kill);
	do_gen_phy_addr(task_prctl);
	do_gen_phy_addr(task_to_inode);
	do_gen_phy_addr(ipc_permission);
	do_gen_phy_addr(ipc_getsecid);
	do_gen_phy_addr(msg_msg_alloc_security);
	do_gen_phy_addr(msg_msg_free_security);
	do_gen_phy_addr(msg_queue_alloc_security);
	do_gen_phy_addr(msg_queue_free_security);
	do_gen_phy_addr(msg_queue_associate);
	do_gen_phy_addr(msg_queue_msgctl);
	do_gen_phy_addr(msg_queue_msgsnd);
	do_gen_phy_addr(msg_queue_msgrcv);
	do_gen_phy_addr(shm_alloc_security);
	do_gen_phy_addr(shm_free_security);
	do_gen_phy_addr(shm_associate);
	do_gen_phy_addr(shm_shmctl);
	do_gen_phy_addr(shm_shmat);
	do_gen_phy_addr(sem_alloc_security);
	do_gen_phy_addr(sem_free_security);
	do_gen_phy_addr(sem_associate);
	do_gen_phy_addr(sem_semctl);
	do_gen_phy_addr(sem_semop);
	do_gen_phy_addr(netlink_send);
	do_gen_phy_addr(d_instantiate);
	do_gen_phy_addr(getprocattr);
	do_gen_phy_addr(setprocattr);
	do_gen_phy_addr(ismaclabel);
	do_gen_phy_addr(secid_to_secctx);
	do_gen_phy_addr(secctx_to_secid);
	do_gen_phy_addr(release_secctx);
	do_gen_phy_addr(inode_notifysecctx);
	do_gen_phy_addr(inode_setsecctx);
	do_gen_phy_addr(inode_getsecctx);
#ifdef CONFIG_SECURITY_NETWORK
	do_gen_phy_addr(unix_stream_connect);
	do_gen_phy_addr(unix_may_send);
	do_gen_phy_addr(socket_create);
	do_gen_phy_addr(socket_post_create);
	do_gen_phy_addr(socket_bind);
	do_gen_phy_addr(socket_connect);
	do_gen_phy_addr(socket_listen);
	do_gen_phy_addr(socket_accept);
	do_gen_phy_addr(socket_sendmsg);
	do_gen_phy_addr(socket_recvmsg);
	do_gen_phy_addr(socket_getsockname);
	do_gen_phy_addr(socket_getpeername);
	do_gen_phy_addr(socket_getsockopt);
	do_gen_phy_addr(socket_setsockopt);
	do_gen_phy_addr(socket_shutdown);
	do_gen_phy_addr(socket_sock_rcv_skb);
	do_gen_phy_addr(socket_getpeersec_stream);
	do_gen_phy_addr(socket_getpeersec_dgram);
	do_gen_phy_addr(sk_alloc_security);
	do_gen_phy_addr(sk_free_security);
	do_gen_phy_addr(sk_clone_security);
	do_gen_phy_addr(sk_getsecid);
	do_gen_phy_addr(sock_graft);
	do_gen_phy_addr(inet_conn_request);
	do_gen_phy_addr(inet_csk_clone);
	do_gen_phy_addr(inet_conn_established);
	do_gen_phy_addr(secmark_relabel_packet);
	do_gen_phy_addr(secmark_refcount_inc);
	do_gen_phy_addr(secmark_refcount_dec);
	do_gen_phy_addr(req_classify_flow);
	do_gen_phy_addr(tun_dev_alloc_security);
	do_gen_phy_addr(tun_dev_free_security);
	do_gen_phy_addr(tun_dev_create);
	do_gen_phy_addr(tun_dev_attach_queue);
	do_gen_phy_addr(tun_dev_attach);
	do_gen_phy_addr(tun_dev_open);
#endif /* CONFIG_SECURITY_NETWORK */
#ifdef CONFIG_SECURITY_INFINIBAND
	do_gen_phy_addr(ib_pkey_access);
	do_gen_phy_addr(ib_endport_manage_subnet);
	do_gen_phy_addr(ib_alloc_security);
	do_gen_phy_addr(ib_free_security);
#endif /* CONFIG_SECURITY_INFINIBAND */
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	do_gen_phy_addr(xfrm_policy_alloc_security);
	do_gen_phy_addr(xfrm_policy_clone_security);
	do_gen_phy_addr(xfrm_policy_free_security);
	do_gen_phy_addr(xfrm_policy_delete_security);
	do_gen_phy_addr(xfrm_state_alloc);
	do_gen_phy_addr(xfrm_state_alloc_acquire);
	do_gen_phy_addr(xfrm_state_free_security);
	do_gen_phy_addr(xfrm_state_delete_security);
	do_gen_phy_addr(xfrm_policy_lookup);
	do_gen_phy_addr(xfrm_state_pol_flow_match);
	do_gen_phy_addr(xfrm_decode_session);
#endif /* CONFIG_SECURITY_NETWORK_XFRM */
#ifdef CONFIG_KEYS
	do_gen_phy_addr(key_alloc);
	do_gen_phy_addr(key_free);
	do_gen_phy_addr(key_permission);
	do_gen_phy_addr(key_getsecurity);
#endif /* CONFIG_KEYS */
#ifdef CONFIG_AUDIT
	do_gen_phy_addr(audit_rule_init);
	do_gen_phy_addr(audit_rule_known);
	do_gen_phy_addr(audit_rule_match);
	do_gen_phy_addr(audit_rule_free);
#endif /* CONFIG_AUDIT */

	/* -1 is because first element is reserved for number of SE hooks */
	phy_addreses[0] = count - 1;
	return TEEC_SUCCESS;
}

/* This assumes that session is already open and operation is valid */
static int prepare_physical_addr_to_ta(TEEC_Operation *operation)
{
	int s_ret;
	int result;
	phys_addr_t *ptr = (phys_addr_t *)g_mem.buffer;
	/*
	 * used_length is the number space used of g_mem.buffer except sehooks,
	 * if define CONFIG_HW_ROOT_SCAN_RODATA_MEASUREMENT_API, it will add
	 * two which used for rodata
	 */
#ifdef CONFIG_HW_ROOT_SCAN_RODATA_MEASUREMENT_API
	int used_length = OFFSET_SEHOOKS_ADDR_START + RODATA_OCCUPY_LENGTH +
			REAL_OFFSET_LENGTH;
#else
	int used_length = OFFSET_SEHOOKS_ADDR_START + REAL_OFFSET_LENGTH;
#endif

	if (operation == NULL) {
		antiroot_error("prepare physical addr to ta operation error\n");
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(g_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, g_mem.size);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	ptr[OFFSET_KCODE_ADDR_START] = __pa_symbol(_stext);
	ptr[OFFSET_KCODE_ADDR_END] = __pa_symbol(_etext);
	ptr[OFFSET_SYSCALL_ADDR_START] =
		(phys_addr_t)__pa_symbol(((void *)sys_call_table));
	ptr[OFFSET_SYSCALL_NUM] = NR_syscalls * sizeof(void *);
	result = get_sescan_address(ptr + OFFSET_SEHOOKS_NUM, used_length);
	if (result != TEEC_SUCCESS) {
		antiroot_error("get sescan address error\n");
		return TEEC_ERROR_GENERIC;
	}
	g_se_hooks_num = ptr[OFFSET_SEHOOKS_NUM];
	/*
	 * if defined CONFIG_DEBUG_RODATA in 4.9 or defined
	 * CONFIG_STRICT_KERNEL_RWX in 4.14, we use __init_begin
	 * than __end_data_ro_after_init to cover EXCEPTION_TABLE.
	 */
#ifdef CONFIG_HW_ROOT_SCAN_RODATA_MEASUREMENT_API
	ptr[OFFSET_RODATA_ADDR_START] =
		__pa_symbol((uintptr_t)__start_data_ro_after_init);
#if defined(CONFIG_DEBUG_RODATA) || defined(CONFIG_STRICT_KERNEL_RWX)
	ptr[OFFSET_RODATA_ADDR_END] = __pa_symbol((uintptr_t)__init_begin);
#else
	ptr[OFFSET_RODATA_ADDR_END] =
		__pa_symbol((uintptr_t)__end_data_ro_after_init);
#endif
#endif
	(*operation).started = 1;
	(*operation).paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
		TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	(*operation).params[0].tmpref.buffer = g_mem.buffer;
	(*operation).params[0].tmpref.size = g_mem.size;
	return TEEC_SUCCESS;
}

static int cmd_measurement(enum root_agent_cmd cmd)
{
	int result;
	int s_ret;
	TEEC_Operation operation;

	if (not_init_agent()) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	s_ret = memset_s(g_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, g_mem.size);
	if (s_ret != EOK)
		return s_ret;

	s_ret = memset_s(&operation, sizeof(TEEC_Operation),
			0, sizeof(TEEC_Operation));
	if (s_ret != EOK) {
		antiroot_error("memset_s failed.\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_OUTPUT,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = g_mem.buffer;
	operation.params[0].tmpref.size = g_mem.size;

	result = TEEK_InvokeCommand(&g_session, cmd, &operation, NULL);
	if (result != TEEC_SUCCESS)
		return TEEC_ERROR_GENERIC;

	g_tee_scan_status = operation.params[1].value.b;
	if (operation.params[1].value.a != REV_NOT_ROOT) {
		g_root_flag = operation.params[1].value.a;
		return TEE_ERROR_ANTIROOT_RSP_FAIL;
	}

	return result;
}

/*
 * Send physical address range of kernel, sys_call
 * table and SE hooks to task_antiroot
 */
static TEEC_Result send_phyaddr(TEEC_Operation *operations, uint32_t *origin)
{
	TEEC_Result teec_ret;

	teec_ret = prepare_physical_addr_to_ta(operations);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("prepare physical addr to ta failed\n");
		return teec_ret;
	}

	teec_ret = TEEK_InvokeCommand(&g_session, ROOTAGENT_KERNEL_ADDR_ID,
				operations, origin);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("Set kernel physical addr range failed\n");
		return teec_ret;
	}
	g_tee_scan_status = operations->params[1].value.b;
	if (operations->params[1].value.a != REV_NOT_ROOT) {
		g_root_flag = operations->params[1].value.a;
		antiroot_debug("Set kernel physical addr failed due to Rooted\n");
		return TEE_ERROR_ANTIROOT_RSP_FAIL; /* lint !e570 */
	}

	return teec_ret;
}

/* inform TA to pause measurement */
int pause_measurement(void)
{
	int ret;

	ret = cmd_measurement(ROOTAGENT_PAUSE_ID);
	if (ret != TEEC_SUCCESS)
		antiroot_error("Pause measurement failed, ret = %d\n", ret);

	return ret;
}

/* resend kernel txt physical address to TA */
int resume_measurement(void)
{
	int ret;

	ret = cmd_measurement(ROOTAGENT_RESUME_ID);
	if (ret != TEEC_SUCCESS)
		antiroot_error("Resume measurement failed, ret = %d\n", ret);

	return ret;
}

#ifdef CONFIG_TEE_ANTIROOT_CLIENT_ENG_DEBUG
int tee_status_check(int cmd_item)
{
	int result;
	int s_ret;
	TEEC_Operation operation;
	int *test_cmd = (int *)g_mem.buffer;

	if (not_init_agent()) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	s_ret = memset_s(g_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, g_mem.size);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	s_ret = memset_s(&operation, sizeof(TEEC_Operation), 0,
			sizeof(TEEC_Operation));
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	test_cmd[0] = cmd_item;
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_OUTPUT,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = g_mem.buffer;
	operation.params[0].tmpref.size = g_mem.size;

	result = TEEK_InvokeCommand(&g_session, TEE_STATUS_TEST,
				&operation, NULL);
	if (result != TEEC_SUCCESS) {
		antiroot_error("tee status check failed\n");
		return TEEC_ERROR_GENERIC;
	}
	antiroot_debug("tee status check ok\n");
	return result;
}
#endif

static inline bool right_proc_len(const struct ragent_rootproc *r_proc)
{
	return ((r_proc->length > 0) &&
		(r_proc->length < ROOTAGENT_RPROCS_MAX_LENGTH) &&
		(r_proc->procid != NULL));
}

/*
 * set the aes key cipher of g_chal_req_key and
 * g_chal_key, according to cfg parameter.
 */
static int set_aes_key_cipher(const struct ragent_config *cfg,
			struct ragent_command *rm_cmd)
{
	int s_ret;

	s_ret = memcpy_s(g_chal_req_key, CHALLENGE_REQ_KEY_LENGTH,
			cfg->cipher_key.cha_req_key, CHALLENGE_REQ_KEY_LENGTH);
	if (s_ret != EOK)
		goto end;
	s_ret = memcpy_s(rm_cmd->content.config.cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH, cfg->cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH);
	if (s_ret != EOK)
		goto end;

	s_ret = memcpy_s(g_chal_key, CHALLENGE_KEY_LENGTH,
			cfg->cipher_key.cha_key, CHALLENGE_KEY_LENGTH);
	if (s_ret != EOK)
		goto end;
	s_ret = memcpy_s(rm_cmd->content.config.cipher_key.cha_key,
			CHALLENGE_KEY_LENGTH, cfg->cipher_key.cha_key,
			CHALLENGE_KEY_LENGTH);
	if (s_ret != EOK)
		goto end;

	return s_ret;
end:
	antiroot_error("memcpy_s failed\n");
	return s_ret;
}

static int setting_ragent_config(TEEC_Operation *operation,
				const struct ragent_config *cfg,
				struct ragent_command **rm_cmd,
				const struct ragent_rootproc *r_proc)
{
	int s_ret;

	s_ret = memset_s(g_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, g_mem.size);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return s_ret;
	}
	s_ret = memset_s(operation, sizeof(TEEC_Operation), 0,
			sizeof(TEEC_Operation));
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return s_ret;
	}

	operation->started = 1;
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_OUTPUT, TEEC_NONE,
						TEEC_NONE);
	operation->params[0].tmpref.buffer = g_mem.buffer;
	operation->params[0].tmpref.size = g_mem.size;
	*rm_cmd = (struct ragent_command *)g_mem.buffer;
	(*rm_cmd)->magic = MAGIC;
	(*rm_cmd)->version = VERSION;
	(*rm_cmd)->interface = ROOTAGENT_CONFIG_ID;
	(*rm_cmd)->content.config.white_list.dstatus = cfg->white_list.dstatus;
	(*rm_cmd)->content.config.white_list.selinux = cfg->white_list.selinux;
	(*rm_cmd)->content.config.white_list.proclength = r_proc->length;
	(*rm_cmd)->content.config.white_list.setid = 0;
	s_ret = set_aes_key_cipher(cfg, *rm_cmd);

	return s_ret;
}

static TEEC_Result setting_proc_length(const struct ragent_command *rm_command,
				const struct ragent_rootproc *r_proc)
{
	int s_ret;

	antiroot_debug("setting_config------proclength: %u\n",
			rm_command->content.config.white_list.proclength);
	if (right_proc_len(r_proc)) {
		s_ret = memcpy_s(g_mem.buffer + sizeof(struct ragent_command),
				RM_PRE_ALLOCATE_SIZE - sizeof(struct ragent_command),
				r_proc->procid, r_proc->length);
		if (s_ret != EOK) {
			antiroot_error("memcpy_s failed\n");
			return TEEC_ERROR_GENERIC; /* lint !e570 */
		}
	} else {
		antiroot_error("root_proc is NULL!\n");
		return TEEC_ERROR_OUT_OF_MEMORY; /* lint !e570 */
	}

	return TEEC_SUCCESS;
}

TEEC_Result setting_config(const struct ragent_config *config,
			const struct ragent_rootproc *root_proc)
{
	uint32_t origin;
	TEEC_Result teec_ret;
	TEEC_Operation operation;
	struct ragent_command *rm_command = NULL;
	int s_ret;

	if (not_init_agent()) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	/* judge the input parameter */
	if ((config == NULL) || (root_proc == NULL) ||
			(root_proc->procid == NULL)) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS; /* lint !e570 */
	}

	s_ret = setting_ragent_config(&operation, config,
				&rm_command, root_proc);
	if (s_ret != EOK)
		return TEEC_ERROR_GENERIC; /* lint !e570 */

	teec_ret = setting_proc_length(rm_command, root_proc);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("Setting_config proclength failed\n");
		return teec_ret;
	}
	teec_ret = TEEK_InvokeCommand(&g_session, ROOTAGENT_CONFIG_ID,
				&operation, &origin);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("Setting_config failed\n");
		return teec_ret;
	}
	g_tee_scan_status = operation.params[1].value.b;

	/*
	 * If in user version TA read rootstatus is already rooted,
	 * then TA will NOT setTrustlist, and return TEEC_SUCCESS directly,
	 * with value.a = REV_ROOTED, value is TEE_ERROR_ANTIROOT_RSP_FAIL.
	 * The setting config and tee init will fail.
	 * But if in eng wersion, the value.a will set REV_NOT_ROOT,
	 * so can continue test in fastboot unlock status.
	 */
	if (operation.params[1].value.a != REV_NOT_ROOT) {
		antiroot_debug("Setting_config failed due to Rooted\n");
		return TEE_ERROR_ANTIROOT_RSP_FAIL; /* lint !e570 */
	}

	teec_ret = send_phyaddr(&operation, &origin);
	if (teec_ret != TEEC_SUCCESS)
		antiroot_error("send_phyaddr failed\n");

	return teec_ret;
}

static int setting_operation_config(TEEC_Operation *s_opt)
{
	int s_ret;

	s_ret = memset_s(g_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, g_mem.size);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return s_ret;
	}
	s_ret = memset_s(g_swap_mem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0,
			g_swap_mem.size);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return s_ret;
	}

	s_ret = memset_s(s_opt, sizeof(TEEC_Operation), 0,
			sizeof(TEEC_Operation));
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return s_ret;
	}
	s_opt->started = 1;
	s_opt->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	s_opt->params[0].tmpref.buffer = g_mem.buffer;
	s_opt->params[0].tmpref.size = g_mem.size;

	return s_ret;
}

static TEEC_Result teec_req_challenge(const char *rm_command_out_in,
				char *rm_command_in_out,
				const struct ragent_command *rm_command,
				struct ragent_challenge *ichallenge)
{
	int ret;
	int s_ret;

	ret = do_aes256_cbc((u8 *)rm_command_in_out,
			(const u8 *)rm_command_out_in,
			g_mem.buffer, g_mem.size, g_chal_key,
			CHALLENGE_KEY_LENGTH, ANTIROOT_SRC_LEN, DECRYPT);
	if (ret != 0) {
		antiroot_error("do_aes256_cbc failed, ret = %d\n", ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(g_nounce, CHALLENGE_NOUNCE_LENGTH,
			rm_command->content.challenge.nounce,
			CHALLENGE_NOUNCE_LENGTH);
	if (s_ret != EOK) {
		antiroot_error("memcpy_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	s_ret = memcpy_s(ichallenge->challengeid,
			CHALLENGE_MAX_LENGTH * sizeof(uint32_t),
			rm_command->content.challenge.challengeid,
			CHALLENGE_MAX_LENGTH * sizeof(uint32_t));
	if (s_ret != EOK) {
		antiroot_error("memcpy_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	return TEEC_SUCCESS;
}

static TEEC_Result challenge_proc(char *rm_command_out_in,
				char *rm_command_in_out,
				struct ragent_command *rm_command,
				TEEC_Operation *operation,
				struct ragent_challenge *ichallenge)
{
	int result;
	int s_ret;
	uint32_t origin;
	TEEC_Result teec_ret;

	s_ret = memset_s(g_nounce, CHALLENGE_NOUNCE_LENGTH, 0,
			CHALLENGE_NOUNCE_LENGTH);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	result = do_aes256_cbc((u8 *)rm_command_out_in,
			(u8 *)rm_command_in_out,
			g_mem.buffer, g_mem.size, g_chal_req_key,
			CHALLENGE_REQ_KEY_LENGTH, ANTIROOT_SRC_LEN, ENCRYPT);
	if (result) {
		antiroot_error("do_aes256_cbc failed, result = %d\n", result);
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	teec_ret = TEEK_InvokeCommand(&g_session, ROOTAGENT_CHALLENGE_ID,
				operation, &origin);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("Request_challenge failed!\n");
	} else if (operation->params[1].value.a != REV_NOT_ROOT) {
		antiroot_debug("Request_challenge failed due to Rooted!\n");
		g_root_flag = operation->params[1].value.a;
		g_tee_scan_status = operation->params[1].value.b;
		teec_ret = teec_req_challenge(rm_command_out_in,
				rm_command_in_out, rm_command, ichallenge);
		if (teec_ret == TEEC_SUCCESS)
			antiroot_debug("Request_challenge successful when Rooted\n");
	} else {
		g_tee_scan_status = operation->params[1].value.b;
		teec_ret = teec_req_challenge(rm_command_out_in,
				rm_command_in_out, rm_command, ichallenge);
		if (teec_ret == TEEC_SUCCESS)
			antiroot_debug("Request_challenge successful\n");
	}
	return teec_ret;
}

TEEC_Result request_challenge(struct ragent_challenge *ichallenge)
{
	TEEC_Result teec_ret;
	TEEC_Operation operation;
	char *rm_command_out_in = NULL;
	char *rm_command_in_out = NULL;
	struct ragent_command *rm_command = NULL;
	int s_ret;

	if (not_init_agent()) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	if (ichallenge == NULL) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS; /* lint !e570 */
	}

	s_ret = setting_operation_config(&operation);
	if (s_ret != EOK)
		return TEEC_ERROR_GENERIC; /* lint !e570 */

	get_random_bytes(g_mem.buffer, IV_SIZE);
	rm_command_out_in = (char *)(g_mem.buffer + IV_SIZE);
	rm_command_in_out = (char *)g_swap_mem.buffer;
	if (rm_command_in_out == NULL) {
		antiroot_error("malloc failed!\n");
		return TEEC_ERROR_OUT_OF_MEMORY; /*lint !e570*/
	}
	rm_command = (struct ragent_command *)rm_command_in_out;
	rm_command->magic = MAGIC;
	rm_command->version = VERSION;
	rm_command->interface = ROOTAGENT_CHALLENGE_ID;
	rm_command->content.challenge.cpuload = ichallenge->cpuload;
	rm_command->content.challenge.battery = ichallenge->battery;
	rm_command->content.challenge.charging = ichallenge->charging;
	rm_command->content.challenge.time = ichallenge->time;
	rm_command->content.challenge.timezone = ichallenge->timezone;
	s_ret = memset_s(rm_command->content.challenge.nounce,
			CHALLENGE_NOUNCE_LENGTH, 0, CHALLENGE_NOUNCE_LENGTH);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	teec_ret = challenge_proc(rm_command_out_in, rm_command_in_out,
				rm_command, &operation, ichallenge);

	return teec_ret;
}

static TEEC_Result rootagent_response(const struct ragent_rootproc *root_proc,
				TEEC_Operation *operation,
				struct ragent_command *rm_command,
				u8 *rm_command_out,
				const u8 *rm_command_in)
{
	int size;
	int ret;
	int s_ret;
	uint32_t origin;
	TEEC_Result teec_ret;

	if (root_proc->procid && (root_proc->length > 0) &&
		(root_proc->length < ROOTAGENT_RPROCS_MAX_LENGTH)) {
		size = sizeof(struct ragent_command) + root_proc->length;
		if (size > ANTIROOT_SRC_LEN) {
			antiroot_error("response is oom!\n");
			return TEEC_ERROR_OUT_OF_MEMORY; /* lint !e570 */
		}

		s_ret = memcpy_s((char *)rm_command + sizeof(struct ragent_command),
				RM_PRE_ALLOCATE_SIZE - sizeof(struct ragent_command),
				root_proc->procid, root_proc->length);
		if (s_ret != EOK) {
			antiroot_error("memcpy_s failed\n");
			return TEEC_ERROR_GENERIC; /* lint !e570 */
		}
	}
	ret = do_aes256_cbc(rm_command_out, rm_command_in,
			g_mem.buffer, g_mem.size,
			g_nounce, CHALLENGE_NOUNCE_LENGTH,
			ANTIROOT_SRC_LEN, ENCRYPT);
	if (ret) {
		antiroot_error("do_aes256_cbc failed, ret = %d\n", ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(g_nounce, CHALLENGE_NOUNCE_LENGTH, 0,
			CHALLENGE_NOUNCE_LENGTH);
	if (s_ret != EOK) {
		antiroot_error("memset_s failed\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}
	teec_ret = TEEK_InvokeCommand(&g_session,
				ROOTAGENT_RESPONSE_ID, operation, &origin);
	if (teec_ret != TEEC_SUCCESS) {
		antiroot_error("Feedback_response failed result = 0x%x\n",
				teec_ret);
	} else if (operation->params[1].value.a != REV_NOT_ROOT) {
		antiroot_debug("feedback_response failed due to Rooted!\n");
		g_root_flag = operation->params[1].value.a;
		g_tee_scan_status = operation->params[1].value.b;
	} else {
		g_tee_scan_status = operation->params[1].value.b;
	}

	return teec_ret;
}

TEEC_Result feedback_response(const struct ragent_response *response,
			const struct ragent_rootproc *root_proc)
{
	TEEC_Result teec_ret;
	TEEC_Operation operation;
	void *rm_command_out = NULL;
	void *rm_command_in = NULL;
	struct ragent_command *rm_command = NULL;
	struct ragent_response *rsp = NULL;
	int s_ret;

	if (not_init_agent()) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC; /* lint !e570 */
	}

	if ((response == NULL) || (root_proc == NULL)) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS; /* lint !e570 */
	}

	s_ret = setting_operation_config(&operation);
	if (s_ret != EOK)
		return TEEC_ERROR_GENERIC; /* lint !e570 */

	get_random_bytes(g_mem.buffer, IV_SIZE);
	rm_command_out = (void *)(g_mem.buffer + IV_SIZE);
	rm_command_in = (void *)g_swap_mem.buffer;
	if (rm_command_in == NULL) {
		antiroot_error("response kmalloc failed!\n");
		return TEEC_ERROR_OUT_OF_MEMORY; /* lint !e570 */
	}
	rm_command = rm_command_in;
	rsp = &(rm_command->content.response);
	rm_command->magic = MAGIC;
	rm_command->version = VERSION;
	rm_command->interface = ROOTAGENT_RESPONSE_ID;
	rsp->proc_integrated = response->proc_integrated;
	rsp->noop = response->noop;
	rsp->runtime_white_list.selinux = response->runtime_white_list.selinux;
	rsp->runtime_white_list.proclength = root_proc->length;
	rsp->runtime_white_list.setid = response->runtime_white_list.setid;

	teec_ret = rootagent_response(root_proc, &operation, rm_command,
				(u8 *)rm_command_out, (u8 *)rm_command_in);
	return teec_ret;
}

#ifdef CONFIG_HW_ROOT_SCAN_RODATA_MEASUREMENT_API
void rodata_stp_upload(void)
{
	int s_ret;
	struct stp_item item;

	antiroot_info("tee_scan_status is 0x%x", g_tee_scan_status);
	item.status = (g_tee_scan_status >> RODATABIT) & 0x1;
	if (item.status == 0) {
		antiroot_info("rodata is normal, no need to upload");
		return;
	}

	item.id = STP_ID_RODATA;
	item.credible = STP_REFERENCE;
	item.version = 0;

	if (strlen(STP_NAME_RODATA) >= STP_ITEM_NAME_LEN) {
		antiroot_error("the name of rodata exceeded the max allowed value");
		return;
	}
	s_ret = strcpy_s(item.name, STP_ITEM_NAME_LEN, STP_NAME_RODATA);
	if (s_ret != EOK) {
		antiroot_error("rodata_stp_upload strcpy_s error, s_ret = %d",
			s_ret);
		return;
	}

	s_ret = kernel_stp_upload(item, NULL);
	if (s_ret != 0)
		antiroot_debug("rodata stp upload failed");
}
#endif
