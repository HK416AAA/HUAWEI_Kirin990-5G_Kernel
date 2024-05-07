

/* 头文件包含 */
#include "frw_event_main.h"

#include "oam_ext_if.h"
#include "frw_task.h"
#include "frw_main.h"
#include "frw_event_sched.h"
#include "hal_ext_if.h"
#include "oal_kernel_file.h"
#include "securec.h"
#include "frw_timer.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_MAIN_C

/* STRUCT定义 */
/*
 * 结构名  : frw_event_cfg_stru
 * 结构说明: 事件队列配置信息结构体
 */
typedef struct {
    oal_uint8 uc_weight;                   /* 队列权重 */
    oal_uint8 uc_max_events;               /* 队列所能容纳的最大事件个数 */
    frw_sched_policy_enum_uint8 en_policy; /* 队列所属调度策略(高优先级、普通优先级) */
    oal_uint8 auc_resv;
} frw_event_cfg_stru;

/*
 * 结构名  : frw_event_ipc_register_stru
 * 结构说明: IPC模块注册结构体
 */
typedef struct {
    oal_uint32 (*p_frw_event_deploy_pipeline_func)(frw_event_mem_stru *pst_event_mem, oal_uint8 *puc_deploy_result);
    oal_uint32 (*p_frw_ipc_event_queue_full_func)(oal_void);
    oal_uint32 (*p_frw_ipc_event_queue_empty_func)(oal_void);
} frw_event_ipc_register_stru;

/* 事件队列配置信息全局变量 */
OAL_STATIC frw_event_cfg_stru g_event_queue_cfg_table[] = WLAN_FRW_EVENT_CFG_TABLE;

/* 事件管理实体 */
frw_event_mgmt_stru g_event_manager[WLAN_FRW_MAX_NUM_CORES];

/* 事件表全局变量 */
frw_event_table_item_stru g_event_table[FRW_EVENT_TABLE_MAX_ITEMS];

/* IPC注册管理实体 */
OAL_STATIC frw_event_ipc_register_stru g_ipc_register;

#if defined(_PRE_DEBUG_MODE)
#if defined(_PRE_EVENT_TIME_TRACK_DEBUG)
OAL_STATIC frw_event_track_time_stru g_event_time_track[FRW_RX_EVENT_TRACK_NUM];
OAL_STATIC oal_uint32 g_rx_event_idx = 0;
#endif
oal_bool_enum_uint8 g_en_event_track_switch = OAL_TRUE;
oal_uint32 g_schedule_idx = 0;
oal_uint32 g_schedule_time[FRW_RX_EVENT_TRACK_NUM] = {0};
oal_uint32 g_mac_process_event;  // for debug, adjust the event cnt per frw task scheldued
#endif

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
/* smp os use the task lock to protect the event process */
oal_task_lock_stru g_frw_event_task_lock_entity;
oal_module_symbol(g_frw_event_task_lock_entity);
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
OAL_STATIC oal_int32 frw_trace_print_event_item(frw_event_trace_item_stru *pst_event_trace,
                                                char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_ulong rem_nsec;
    oal_uint64 timestamp = pst_event_trace->timestamp;

    rem_nsec = do_div(timestamp, NSEC_PER_SEC);
    ret = snprintf_s(buf, buf_len, buf_len - 1, "%u,%u,%u,%u,%5lu.%06lu\n",
                     pst_event_trace->st_event_seg.uc_vap_id,
                     pst_event_trace->st_event_seg.en_pipeline,
                     pst_event_trace->st_event_seg.en_type,
                     pst_event_trace->st_event_seg.uc_sub_type,
                     (oal_ulong)timestamp,
                     rem_nsec / MSEC_PER_SEC);
    if (ret < 0) {
        OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
    }
    return ret;
}
#endif

OAL_STATIC oal_int32 frw_print_cpu_count_stat(char *buf, oal_int32 buf_len)
{
#if defined(_PRE_FRW_TIMER_BIND_CPU) && defined(CONFIG_NR_CPUS)
    do {
        oal_uint32 cpu_id;
        oal_int32 count = 0;
        oal_int32 ret;
        for (cpu_id = 0; cpu_id < CONFIG_NR_CPUS; cpu_id++) {
            if (g_frw_timer_cpu_count[cpu_id]) {
                ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "[cpu:%u]count:%u\n",
                                 cpu_id, g_frw_timer_cpu_count[cpu_id]);
                if (ret < 0) {
                    OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                    return count;
                }
                count += ret;
            }
        }
        return count;
    } while (0);
#else
    return 0;
#endif
}

OAL_STATIC oal_int32 frw_print_panic_stat(oal_void *data, char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_int32 count = 0;
    oal_uint32 ul_core_id;
    oal_uint32 i;

    oal_reference(data);
#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    if (g_frw_event_task_lock_entity.claimer) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw task lock claimer:%s\n",
                         g_frw_event_task_lock_entity.claimer->comm);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
    }
#endif
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "last pc:%s,line:%d\n",
                         g_event_manager[ul_core_id].pst_frw_trace->pst_func_name,
                         g_event_manager[ul_core_id].pst_frw_trace->line_num);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1,
                         "task thread total cnt:%u,event cnt:%u,empty max count:%u\n",
                         g_event_task[ul_core_id].ul_total_loop_cnt,
                         g_event_task[ul_core_id].ul_total_event_cnt,
                         g_event_task[ul_core_id].ul_max_empty_count);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
#endif

        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw event trace buff:\n");
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

        count += frw_print_cpu_count_stat(buf + count, buf_len - count);

        if (g_event_manager[ul_core_id].pst_frw_trace->ul_over_flag == 1) {
            /* overturn */
            for (i = g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos;
                 i < CONFIG_FRW_MAX_TRACE_EVENT_NUMS; i++) {
                ret = frw_trace_print_event_item(&g_event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                                 buf + count, buf_len - count);
                if (ret < 0) {
                    OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                    return count;
                }
                count += ret;
            }
        }

        i = 0;
        for (i = 0; i < g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos; i++) {
            ret = frw_trace_print_event_item(&g_event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                             buf + count, buf_len - count);
            if (ret < 0) {
                OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                return count;
            }
            count += ret;
        }
    }
#else
    oal_reference(i);
    oal_reference(ul_core_id);
    oal_reference(ret);
    oal_reference(count);
#endif
    return count;
}
OAL_STATIC declare_wifi_panic_stru(frw_panic_stat, frw_print_panic_stat);
#endif

/*
 * 函 数 名  : frw_event_init_event_queue
 * 功能描述  : 初始化事件队列
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
OAL_STATIC oal_uint32 frw_event_init_event_queue(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* 循环初始化事件队列 */
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            ul_ret = frw_event_queue_init(&g_event_manager[ul_core_id].st_event_queue[us_qid],
                                          g_event_queue_cfg_table[us_qid].uc_weight,
                                          g_event_queue_cfg_table[us_qid].en_policy,
                                          FRW_EVENT_QUEUE_STATE_INACTIVE,
                                          g_event_queue_cfg_table[us_qid].uc_max_events);
            if (oal_unlikely(ul_ret != OAL_SUCC)) {
                OAM_WARNING_LOG1(0, OAM_SF_FRW,
                                 "{frw_event_init_event_queue, frw_event_queue_init return != OAL_SUCC!%d}",
                                 ul_ret);
                return ul_ret;
            }
        }
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : frw_event_destroy_event_queue
 * 功能描述  : 销毁事件队列
 * 输入参数  : ul_core_id: 核号
 */
OAL_STATIC oal_void frw_event_destroy_event_queue(oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;

    /* 循环销毁事件队列 */
    for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
        frw_event_queue_destroy(&g_event_manager[ul_core_id].st_event_queue[us_qid]);
    }
}

/*
 * 函 数 名  : frw_event_init_sched
 * 功能描述  : 初始化调度器
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
OAL_STATIC oal_uint32 frw_event_init_sched(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* 循环初始化调度器 */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            ul_ret = frw_event_sched_init(&g_event_manager[ul_core_id].st_sched_queue[us_qid]);
            if (oal_unlikely(ul_ret != OAL_SUCC)) {
                OAM_WARNING_LOG1(0, OAM_SF_FRW,
                                 "{frw_event_init_sched, frw_event_sched_init return != OAL_SUCC!%d}",
                                 ul_ret);
                return ul_ret;
            }
        }
    }

    return OAL_SUCC;
}

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG

OAL_STATIC oal_uint32 frw_event_trace_init(oal_void)
{
    oal_uint32 ul_core_id;
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        g_event_manager[ul_core_id].pst_frw_trace = (frw_event_trace_stru *)vmalloc(OAL_SIZEOF(frw_event_trace_stru));
        if (g_event_manager[ul_core_id].pst_frw_trace == NULL) {
            OAL_IO_PRINT("frw_event_init_sched coreid:%u, alloc frw event trace %u bytes failed! \n",
                         ul_core_id,
                         (oal_uint32)OAL_SIZEOF(frw_event_trace_stru));
            return OAL_ERR_CODE_PTR_NULL;
        }
        memset_s((oal_void *)g_event_manager[ul_core_id].pst_frw_trace,
                 OAL_SIZEOF(frw_event_trace_stru), 0, OAL_SIZEOF(frw_event_trace_stru));
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void frw_event_trace_exit(oal_void)
{
    oal_uint32 ul_core_id;
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        if (g_event_manager[ul_core_id].pst_frw_trace != NULL) {
            vfree(g_event_manager[ul_core_id].pst_frw_trace);
            g_event_manager[ul_core_id].pst_frw_trace = NULL;
        }
    }
}
#endif

/*
 * 函 数 名  : frw_event_dispatch_event
 * 功能描述  : 事件分发接口(分发事件至核间通讯、事件队列、或者查表寻找相应事件处理函数)
 * 输入参数  : pst_event_mem: 指向事件内存块的指针
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_dispatch_event(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    frw_event_deploy_enum_uint8 en_deploy;
    oal_uint32 ul_ret;
#endif

#ifdef _PRE_DEBUG_MODE
    oal_uint32 ul_dog_tag;
#endif

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_event_dispatch_event: pst_event_mem is null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 异常: 内存写越界 */
#ifdef _PRE_DEBUG_MODE
    ul_dog_tag = (*((oal_uint32 *)(pst_event_mem->puc_origin_data + pst_event_mem->us_len - OAL_DOG_TAG_SIZE)));
    if (ul_dog_tag != OAL_DOG_TAG) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "frw_event_dispatch_event, ul_dog_tag changed is[%d]changed", ul_dog_tag);
        return OAL_ERR_CODE_OAL_MEM_DOG_TAG;
    }
    frw_event_report(pst_event_mem);
#endif

#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    /* 如果没有开启核间通信，则根据事件分段号处理事件(入队或者执行相应的处理函数) */
    if (st_ipc_register.p_frw_event_deploy_pipeline_func == OAL_PTR_NULL) {
        return frw_event_process(pst_event_mem);
    }

    ul_ret = st_ipc_register.p_frw_event_deploy_pipeline_func(pst_event_mem, &en_deploy);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_event_dispatch_event, p_frw_event_deploy_pipeline_func return != OAL_SUCC!%d}",
                         ul_ret);
        return ul_ret;
    }

    /* 如果为核间通信，则直接返回成功。否则，根据事件分段号处理事件 */
    if (en_deploy == FRW_EVENT_DEPLOY_IPC) {
        return OAL_SUCC;
    }
#endif

    return frw_event_process(pst_event_mem);
}

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
OAL_STATIC ssize_t frw_get_event_trace(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        OAL_IO_PRINT("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        OAL_IO_PRINT("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        OAL_IO_PRINT("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += frw_print_panic_stat(NULL, buf, PAGE_SIZE - ret);
    return ret;
}
OAL_STATIC struct kobj_attribute g_dev_attr_event_trace =
    __ATTR(event_trace, S_IRUGO, frw_get_event_trace, NULL);

OAL_STATIC struct attribute *g_frw_sysfs_entries[] = {
    &g_dev_attr_event_trace.attr,
    NULL
};

OAL_STATIC struct attribute_group g_frw_attribute_group = {
    .name = "frw",
    .attrs = g_frw_sysfs_entries,
};

OAL_STATIC oal_int32 frw_sysfs_entry_init(oal_void)
{
    oal_int32 ret;
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::get sysfs root object failed!}");
        return -OAL_EFAIL;
    }

    ret = oal_debug_sysfs_create_group(pst_root_object, &g_frw_attribute_group);
    if (ret) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::sysfs create group failed!}");
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void frw_sysfs_entry_exit(oal_void)
{
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object != NULL) {
        oal_debug_sysfs_remove_group(pst_root_object, &g_frw_attribute_group);
    }
}
#endif

/*
 * 函 数 名  : frw_event_init
 * 功能描述  : 事件管理模块初始化总入口
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_init(oal_void)
{
    oal_uint32 ul_ret;

    memset_s(&g_ipc_register, OAL_SIZEOF(g_ipc_register), 0, OAL_SIZEOF(g_ipc_register));
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)

    memset_s(&g_event_time_track, FRW_RX_EVENT_TRACK_NUM * OAL_SIZEOF(frw_event_track_time_stru),
             0, FRW_RX_EVENT_TRACK_NUM * OAL_SIZEOF(frw_event_track_time_stru));
#endif

#ifdef _PRE_DEBUG_MODE
    g_mac_process_event = 0;
#endif

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    oal_smp_task_lock_init(&g_frw_event_task_lock_entity);
#endif

    /* 初始化事件队列 */
    ul_ret = frw_event_init_event_queue();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_event_init, frw_event_init_event_queue != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    /* 初始化调度器 */
    ul_ret = frw_event_init_sched();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "frw_event_init, frw_event_init_sched != OAL_SUCC!%d", ul_ret);
        return ul_ret;
    }

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    ul_ret = frw_event_trace_init();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "frw_event_init, frw_event_trace_init != OAL_SUCC!%d", ul_ret);
        return ul_ret;
    }
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_register(&frw_panic_stat, NULL);
#endif

    frw_task_event_handler_register(frw_event_process_all_event);

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
    frw_sysfs_entry_init();
#endif

    return OAL_SUCC;
}

/*
 * 函 数 名  : frw_event_exit
 * 功能描述  : 事件管理模块卸载接口
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_exit(oal_void)
{
    oal_uint32 ul_core_id;
#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
    frw_sysfs_entry_exit();
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    frw_event_trace_exit();
#endif

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* 销毁事件队列 */
        frw_event_destroy_event_queue(ul_core_id);
    }

    return OAL_SUCC;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
oal_void frw_event_sub_rx_adapt_table_init(frw_event_sub_table_item_stru *pst_sub_table, oal_uint32 ul_table_nums,
                                           frw_event_mem_stru *(*p_rx_adapt_func)(frw_event_mem_stru *))
{
    oal_uint32 i;
    frw_event_sub_table_item_stru *pst_curr_table = NULL;
    for (i = 0; i < ul_table_nums; i++) {
        pst_curr_table = pst_sub_table + i;
        pst_curr_table->p_rx_adapt_func = p_rx_adapt_func;
    }
}
#endif

/*
 * 函 数 名  : frw_event_queue_enqueue
 * 功能描述  : 将事件内存放入相应的事件队列
 * 输入参数  : pst_event_mem: 指向事件内存块的指针
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_queue_enqueue(frw_event_queue_stru *pst_event_queue, frw_event_mem_stru *pst_event_mem)
{
    oal_uint32 ul_ret;
    oal_uint ul_irq_flag;

    if (oal_unlikely((pst_event_queue == NULL) || (pst_event_mem == NULL))) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);
    ul_ret = oal_queue_enqueue(&pst_event_queue->st_queue, (void *)pst_event_mem);
    oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
    return ul_ret;
}

/*
 * 函 数 名  : frw_event_queue_dequeue
 * 功能描述  : 事件内存出队
 * 输入参数  : pst_event_queue: 事件队列
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
frw_event_mem_stru *frw_event_queue_dequeue(frw_event_queue_stru *pst_event_queue)
{
    frw_event_mem_stru *pst_event_mem = NULL;
    oal_uint ul_irq_flag;

    if (oal_unlikely(pst_event_queue == NULL)) {
        oal_warn_on(1);
        return NULL;
    }

    oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);
    pst_event_mem = (frw_event_mem_stru *)oal_queue_dequeue(&pst_event_queue->st_queue);
    oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
    return pst_event_mem;
}

/*
 * 函 数 名  : frw_event_post_event
 * 功能描述  : 将事件内存放入相应的事件队列
 * 输入参数  : pst_event_mem: 指向事件内存块的指针
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_post_event(frw_event_mem_stru *pst_event_mem, oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    oal_uint32 ul_ret;
    frw_event_hdr_stru *pst_event_hdr = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    /* 获取事件队列ID */
    ul_ret = frw_event_to_qid(pst_event_mem, &us_qid);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, frw_event_to_qid return != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* 根据核号 + 队列ID，找到相应的事件队列 */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    /* 检查policy */
    if (oal_unlikely(pst_event_queue->en_policy >= FRW_SCHED_POLICY_BUTT)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", pst_event_queue->en_policy);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* 获取调度队列 */
    pst_sched_queue = &pst_event_mgmt->st_sched_queue[pst_event_queue->en_policy];

    /* 事件内存引用计数加1 */
#ifdef _PRE_DEBUG_MODE
    /* 异常: 该内存块上的共享用户数已为最大值 */
    if (oal_unlikely((oal_uint16)(pst_event_mem->uc_user_cnt + 1) > WLAN_MEM_MAX_USERS_NUM)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{pst_event_mem->uc_user_cnt is too large.%d}", pst_event_mem->uc_user_cnt);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }
#endif
    /* 先取得引用，防止enqueue与取得引用之间被释放 */
    pst_event_mem->uc_user_cnt++;

    /* 事件入队 */
    ul_ret = frw_event_queue_enqueue(pst_event_queue, pst_event_mem);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        pst_event_hdr = (frw_event_hdr_stru *)(frw_get_event_data(pst_event_mem));
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        oam_warning_log4(0, OAM_SF_FRW,
                         "frw_event_post_event:: enqueue fail. core %d, type %d, sub type %d, pipeline %d ",
                         ul_core_id,
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline);

        oam_warning_log4(0, OAM_SF_FRW, "event info: type: %d, sub type: %d, pipeline: %d,max num:%d",
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline,
                         pst_event_queue->st_queue.uc_max_elements);
#else
        /* 因CPU动态调频造成跑流个别事件入队失败，暂时修改为warning */
        oam_warning_log4(0, OAM_SF_FRW,
                         "frw_event_post_event:: enqueue fail. core %d, type %d, sub type %d, pipeline %d ",
                         ul_core_id,
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline);

        oam_warning_log4(0, OAM_SF_FRW, "event info: type: %d, sub type: %d, pipeline: %d,max num:%d",
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline,
                         pst_event_queue->st_queue.uc_max_elements);

        /* 添加针对mac error错误的维测讯息，看是什么错误导致队列溢出 */
        if ((pst_event_hdr->en_type == FRW_EVENT_TYPE_HIGH_PRIO)
            && (pst_event_hdr->uc_sub_type == HAL_EVENT_ERROR_IRQ_MAC_ERROR)
            && (pst_event_hdr->en_pipeline == FRW_EVENT_PIPELINE_STAGE_0)) {
            frw_event_stru *pst_event;
            hal_error_irq_event_stru *pst_error_irq_event;
            oal_uint32 ul_error1_irq_state = 0;
            pst_event = frw_get_event_stru(pst_event_mem);
            pst_error_irq_event = (hal_error_irq_event_stru *)(pst_event->auc_event_data);
            ul_error1_irq_state = pst_error_irq_event->st_error_state.ul_error1_val;
            OAM_ERROR_LOG1(0, OAM_SF_FRW, "mac error event enqueue fail: error status:0x%08x", ul_error1_irq_state);
        }

#endif
        /* 释放事件内存引用 */
        frw_event_free_m(pst_event_mem);

        return ul_ret;
    }

    /* 根据所属调度策略，将事件队列加入可调度队列 */
    ul_ret = frw_event_sched_activate_queue(pst_sched_queue, pst_event_queue);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW,
                       "{frw_event_post_event, frw_event_sched_activate_queue return != OAL_SUCC! %d}",
                       ul_ret);
        return ul_ret;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_DEBUG_MODE
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_TASKLET)
    if (g_en_event_track_switch == OAL_TRUE) {
        if (!oal_task_is_scheduled(&g_event_task[ul_core_id].st_event_tasklet)) {
            g_schedule_time[g_schedule_idx] = oal_5115timer_get_10ns();
        }
    }
#endif
#endif
#endif

    frw_task_sched(ul_core_id);

    return OAL_SUCC;
}

/*
 * 函 数 名  : frw_event_table_register
 * 功能描述  : 注册相应事件对应的事件处理函数
 * 输入参数  : en_type:       事件类型
 *             en_pipeline:   事件分段号
 *             pst_sub_table: 事件子表指针
 */
oal_void frw_event_table_register(frw_event_type_enum_uint8 en_type,
                                  frw_event_pipeline_enum en_pipeline,
                                  frw_event_sub_table_item_stru *pst_sub_table)
{
    oal_uint8 uc_index;

    if (oal_unlikely(pst_sub_table == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_event_table_register: pst_sub_table is null ptr!}");
        return;
    }

    /* 根据事件类型及分段号计算事件表索引 */
    uc_index = (((oal_uint8)en_type << 1) | ((oal_uint8)en_pipeline & 0x01));

    if (oal_unlikely(uc_index >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_table_register, array overflow! %d}", uc_index);
        return;
    }

    g_event_table[uc_index].pst_sub_table = pst_sub_table;
}

/*
 * 函 数 名  : frw_event_deploy_register
 * 功能描述  : 供event deploy模块注册事件部署接口
 * 输入参数  : p_func: 事件部署接口
 */
oal_void frw_event_deploy_register(oal_uint32 (*p_func)(frw_event_mem_stru *pst_event_mem,
                                                        frw_event_deploy_enum_uint8 *pen_deploy_result))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_event_deploy_pipeline_func = p_func;
}

/*
 * 函 数 名  : frw_event_ipc_event_queue_full_register
 * 功能描述  : 供IPC模块注册核间中断频度管理接口
 * 输入参数  : p_func: 核间中断频度管理接口
 */
oal_void frw_event_ipc_event_queue_full_register(oal_uint32 (*p_func)(oal_void))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_ipc_event_queue_full_func = p_func;
}

/*
 * 函 数 名  : frw_event_ipc_event_queue_empty_register
 * 功能描述  : 供IPC模块注册核间中断频度管理接口
 * 输入参数  : p_func: 核间中断频度管理接口
 */
oal_void frw_event_ipc_event_queue_empty_register(oal_uint32 (*p_func)(oal_void))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_ipc_event_queue_empty_func = p_func;
}

/*
 * 函 数 名  : frw_event_process_all_event
 * 功能描述  : 处理事件队列中的所有事件
 */
oal_void frw_event_process_all_event(oal_uint ui_data)
{
    oal_uint32 ul_core_id;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 ul_mac_process_event = FRW_PROCESS_MAX_EVENT;
    const oal_uint32 ul_frw_max_event = 500;
#ifdef _PRE_FEATURE_WAVEAPP_CLASSIFY
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    frw_event_stru *pst_event = NULL;
    hal_wlan_rx_event_stru *pst_wlan_rx_event = NULL;
    hal_to_dmac_device_stru *pst_device = NULL;
#endif
#endif

#ifdef _PRE_DEBUG_MODE
#if defined(_PRE_EVENT_TIME_TRACK_DEBUG)
    oal_uint32 ul_timestamp_start;
    oal_uint32 ul_timestamp_end;
    oal_uint32 ul_event_index;
#endif
    if (g_en_event_track_switch == OAL_TRUE) {
        g_schedule_time[g_schedule_idx] = g_schedule_time[g_schedule_idx] - oal_5115timer_get_10ns();
        oal_incr(g_schedule_idx, FRW_RX_EVENT_TRACK_NUM);
    }
#endif

    /* 获取核号 */
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow! %d}", ul_core_id);
        return;
    }

#ifdef _PRE_DEBUG_MODE
    if ((g_mac_process_event) && (g_mac_process_event <= ul_frw_max_event)) {
        ul_mac_process_event = g_mac_process_event;
    }
#endif

    pst_sched_queue = g_event_manager[ul_core_id].st_sched_queue;

    /* 调用事件调度模块，选择一个事件 */
    pst_event_mem = (frw_event_mem_stru *)frw_event_schedule(pst_sched_queue);

#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)

    if (g_en_event_track_switch == OAL_TRUE) {
        oal_incr(g_rx_event_idx, FRW_RX_EVENT_TRACK_NUM);
        g_event_time_track[g_rx_event_idx].ul_event_cnt = 0;
    }

#endif

    while (pst_event_mem != OAL_PTR_NULL) {
        /* 获取事件头结构 */
        pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
        ul_timestamp_start = oal_5115timer_get_10ns();
#endif

#ifdef _PRE_FEATURE_WAVEAPP_CLASSIFY
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
        if (pst_event_hrd->en_type == FRW_EVENT_TYPE_WLAN_DRX) {
            /* 获取事件头和事件结构体指针 */
            pst_event = frw_get_event_stru(pst_event_mem);
            pst_wlan_rx_event = (hal_wlan_rx_event_stru *)(pst_event->auc_event_data);
            pst_device = pst_wlan_rx_event->pst_hal_device;

            if (oal_unlikely(pst_device == OAL_PTR_NULL)) {
                OAM_ERROR_LOG0(0, OAM_SF_RX, "{frw_event_process_all_event::pst_device null.}");
                break;
            }

            if (pst_device->en_test_is_on_waveapp_flag == OAL_TRUE) {
                ul_mac_process_event = 1;
            }
        }
#endif
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
        /* trace the event serial */
        frw_event_trace(pst_event_mem, ul_core_id);
#endif
        /* 根据事件找到对应的事件处理函数 */
        frw_event_task_lock();
        frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
        frw_event_task_unlock();

#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
        if (g_en_event_track_switch == OAL_TRUE) {
            ul_timestamp_end = oal_5115timer_get_10ns();
            ul_event_index = g_event_time_track[g_rx_event_idx].ul_event_cnt;
            if (ul_event_index < FRW_EVENT_TRACK_NUM) {
                g_event_time_track[g_rx_event_idx].aul_event_time[ul_event_index] = ul_timestamp_start -
                                                                                           ul_timestamp_end;
                g_event_time_track[g_rx_event_idx].us_event_type[ul_event_index] = pst_event_hrd->en_type;
                g_event_time_track[g_rx_event_idx].us_event_sub_type[ul_event_index] = pst_event_hrd->uc_sub_type;
            }
            g_event_time_track[g_rx_event_idx].ul_event_cnt++;
        }

#endif
        /* 释放事件内存 */
        frw_event_free_m(pst_event_mem);
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        if (oal_likely(ul_core_id < WLAN_FRW_MAX_NUM_CORES)) {
            g_event_task[ul_core_id].ul_total_event_cnt++;
        }
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
        frw_event_last_pc_trace(__FUNCTION__, __LINE__, ul_core_id);
#endif

        if (--ul_mac_process_event) {
            /* 调用事件调度模块，选择一个事件 */
            pst_event_mem = (frw_event_mem_stru *)frw_event_schedule(pst_sched_queue);
        } else {
            frw_task_sched(ul_core_id);
            break;
        }
    }

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    frw_event_last_pc_trace(__FUNCTION__, __LINE__, ul_core_id);
#endif
}

/*
 * 函 数 名  : frw_event_flush_event_queue
 * 功能描述  : 清空某个事件队列中的所有事件
 * 返 回 值  : OAL_SUCC 或其它错误码
 */
oal_uint32 frw_event_flush_event_queue(frw_event_type_enum_uint8 uc_event_type)
{
    oal_uint32 core_id;
    oal_uint16 qid;
    oal_uint8 vap_id;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 event_succ = 0;
    ;

    /* 遍历每个核的每个vap对应的事件队列 */
    for (core_id = 0; core_id < WLAN_FRW_MAX_NUM_CORES; core_id++) {
        for (vap_id = 0; vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; vap_id++) {
            qid = vap_id * FRW_EVENT_TYPE_BUTT + uc_event_type;

            /* 根据核号 + 队列ID，找到相应的事件队列 */
            pst_event_mgmt = &g_event_manager[core_id];
            pst_event_queue = &pst_event_mgmt->st_event_queue[qid];

            /* flush所有的event */
            while (pst_event_queue->st_queue.uc_element_cnt != 0) {
                pst_event_mem = (frw_event_mem_stru *)frw_event_queue_dequeue(pst_event_queue);
                if (pst_event_mem == OAL_PTR_NULL) {
                    continue;
                }

                /* 获取事件头结构 */
                pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);

                /* 根据事件找到对应的事件处理函数 */
                frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);

                /* 释放事件内存 */
                frw_event_free_m(pst_event_mem);

                event_succ++;
            }
#if 1
            /* 如果事件队列变空，需要将其从调度队列上删除，并将事件队列状态置为不活跃(不可被调度) */
            if (pst_event_queue->st_queue.uc_element_cnt == 0) {
                frw_event_sched_deactivate_queue(&g_event_manager[core_id].st_sched_queue[pst_event_queue->en_policy],
                                                 pst_event_queue);
            }
#endif
        }
    }

    return event_succ;
}

/*
 * 函 数 名  : frw_event_dump_event
 * 功能描述  : 打印事件
 * 输入参数  : puc_event: 事件结构体首地址
 */
oal_void frw_event_dump_event(oal_uint8 *puc_event)
{
    frw_event_stru *pst_event = (frw_event_stru *)puc_event;
    frw_event_hdr_stru *pst_event_hdr = &pst_event->st_event_hdr;
    oal_uint8 *puc_payload = pst_event->auc_event_data;
    oal_uint32 ul_event_length = pst_event_hdr->us_length - FRW_EVENT_HDR_LEN;
    oal_uint32 ul_loop;

    OAL_IO_PRINT("==================event==================\n");
    OAL_IO_PRINT("type     : [%02X]\n", pst_event_hdr->en_type);
    OAL_IO_PRINT("sub type : [%02X]\n", pst_event_hdr->uc_sub_type);
    OAL_IO_PRINT("length   : [%X]\n", pst_event_hdr->us_length);
    OAL_IO_PRINT("pipeline : [%02X]\n", pst_event_hdr->en_pipeline);
    OAL_IO_PRINT("chip id  : [%02X]\n", pst_event_hdr->uc_chip_id);
    OAL_IO_PRINT("device id: [%02X]\n", pst_event_hdr->uc_device_id);
    OAL_IO_PRINT("vap id   : [%02X]\n", pst_event_hdr->uc_vap_id);

    OAL_IO_PRINT("payload: \n");

    for (ul_loop = 0; ul_loop < ul_event_length; ul_loop += sizeof(oal_uint32)) { /* 每次偏移4字节 */
        OAL_IO_PRINT("%02X %02X %02X %02X\n", puc_payload[ul_loop], puc_payload[ul_loop + 1],
                     puc_payload[ul_loop + 2], puc_payload[ul_loop + 3]);
    }
}

/*
 * 函 数 名  : frw_event_get_info_from_event_queue
 * 功能描述  : 从事件队列中获取每一个事件的事件头信息
 * 输入参数  : pst_event_queue: 事件队列
 */
oal_void frw_event_get_info_from_event_queue(frw_event_queue_stru *pst_event_queue)
{
    frw_event_stru *pst_event = NULL;
    oal_queue_stru *pst_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    oal_uint8 uc_loop;

    pst_queue = &pst_event_queue->st_queue;

    for (uc_loop = 0; uc_loop < pst_queue->uc_element_cnt; uc_loop++) {
        pst_event_mem = (frw_event_mem_stru *)pst_queue->pul_buf[uc_loop];
        pst_event = frw_get_event_stru(pst_event_mem);

#ifdef _PRE_DEBUG_MODE
        OAL_IO_PRINT("frw event info:vap %d user_cnt: %u, pool_id: %u, subpool_id: %u, len: %u, "
                     "alloc_core_id = %u, alloc_file_id: %u, alloc_line_num: %u, alloc_time_stamp: %u, "
                     "trace_file_id: %u, trace_line_num: %u, trace_time_stamp: %u.\n",
                     pst_event->st_event_hdr.uc_vap_id,
                     pst_event_mem->uc_user_cnt,
                     pst_event_mem->en_pool_id,
                     pst_event_mem->uc_subpool_id,
                     pst_event_mem->us_len,
                     pst_event_mem->ul_alloc_core_id[0],
                     pst_event_mem->ul_alloc_file_id[0],
                     pst_event_mem->ul_alloc_line_num[0],
                     pst_event_mem->ul_alloc_time_stamp[0],
                     pst_event_mem->ul_trace_file_id,
                     pst_event_mem->ul_trace_line_num,
                     pst_event_mem->ul_trace_time_stamp);
#else
        OAL_IO_PRINT("frw event info:vap %d,type = %d,subtype=%d,pipe=%d, user_cnt: %u, pool_id: %u, subpool_id: %u, len: %u.\n",
                     pst_event->st_event_hdr.uc_vap_id,
                     pst_event->st_event_hdr.en_type,
                     pst_event->st_event_hdr.uc_sub_type,
                     pst_event->st_event_hdr.en_pipeline,
                     pst_event_mem->uc_user_cnt,
                     pst_event_mem->en_pool_id,
                     pst_event_mem->uc_subpool_id,
                     pst_event_mem->us_len);
#endif
    }
}

/*
 * 函 数 名  : frw_event_queue_info
 * 功能描述  : 将事件队列中的事件个数以及每个事件的事件头信息上报.
 *             从调度队列找到每一个存在事件的事件队列，然后获取事件个数并得到每一个
 *             事件的事件头信息
 * 返 回 值  : 成功或者失败码
 */
oal_uint32 frw_event_queue_info(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    oal_dlist_head_stru *pst_dlist = NULL;
    oal_uint ul_irq_flag;

    /* 获取核号 */
    ul_core_id = oal_get_core_id();
    OAL_IO_PRINT("frw_event_queue_info get core id is %d.\n", ul_core_id);

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        OAL_IO_PRINT("-------------frw_event_queue_info core id is %d--------------.\n", ul_core_id);
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            pst_event_queue = &g_event_manager[ul_core_id].st_event_queue[us_qid];
            oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);

            if (pst_event_queue->st_queue.uc_element_cnt != 0) {
                OAL_IO_PRINT("qid %d,state %d, event num %d, max event num %d, weigt_cnt %d,head idx %d,tail idx %d,prev=0x%p,next=0x%p\n",
                             us_qid, pst_event_queue->en_state,
                             pst_event_queue->st_queue.uc_element_cnt,
                             pst_event_queue->st_queue.uc_max_elements,
                             pst_event_queue->uc_weight,
                             pst_event_queue->st_queue.uc_head_index,
                             pst_event_queue->st_queue.uc_tail_index,
                             pst_event_queue->st_list.pst_prev,
                             pst_event_queue->st_list.pst_next);
                frw_event_get_info_from_event_queue(pst_event_queue);
            }
            oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
        }
        /* 根据核号，找到相应的事件管理结构体 */
        pst_event_mgmt = &g_event_manager[ul_core_id];

        /* 遍历获取调度队列 */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            /* 获取事件管理结构体中的调度队列 */
            pst_sched_queue = &pst_event_mgmt->st_sched_queue[us_qid];
            oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &ul_irq_flag);
            /* 获取调度队列中每个事件队列的每个事件的信息 */
            if (!oal_dlist_is_empty(&pst_sched_queue->st_head)) {
                /* 获取调度队列中的每一个事件队列 */
                oal_dlist_search_for_each(pst_dlist, &pst_sched_queue->st_head)
                {
                    pst_event_queue = oal_dlist_get_entry(pst_dlist, frw_event_queue_stru, st_list);

                    /* 获取队列中每一个事件的事件头信息 */
                    oal_spin_lock(&pst_event_queue->st_lock);
                    frw_event_get_info_from_event_queue(pst_event_queue);
                    oal_spin_unlock(&pst_event_queue->st_lock);
                }
            } else {
                OAL_IO_PRINT("Schedule queue %d empty\n", us_qid);
            }
            oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &ul_irq_flag);
        }
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : frw_event_vap_pause_event
 * 功能描述  : 设置特定VAP的所有事件队列的VAP状态为暂停，停止调度，允许继续入队
 * 输入参数  : uc_vap_id: VAP ID值
 * 返 回 值  : OAL_SUCC或者OAL_FAIL
 */
oal_void frw_event_vap_pause_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* 根据核号，找到相应的事件管理 */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    /* 根据队列ID，找到相应的VAP的第一个事件队列 */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* 如果事件队列已经被pause的话，直接返回，不然循环中调度队列总权重会重复减去事件队列的权重 */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* 根据队列ID，找到相应的事件队列 */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

        frw_event_sched_pause_queue(pst_sched_queue, pst_event_queue);
    }
}

/*
 * 函 数 名  : frw_event_vap_resume_event
 * 功能描述  : 设置特定VAP的所有事件队列的VAP状态为恢复，可调度
 * 输入参数  : uc_vap_id: VAP ID值
 */
oal_void frw_event_vap_resume_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* 根据核号，找到相应的事件管理 */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    /* 根据队列ID，找到相应的VAP的第一个事件队列 */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* 如果事件队列已经被resume的话，直接返回，不然循环中调度队列总权重会重复减去事件队列的权重 */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_RESUME) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* 根据队列ID，找到相应的事件队列 */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

        frw_event_sched_resume_queue(pst_sched_queue, pst_event_queue);
    }

    /* 唤醒线程 */
    frw_task_sched(ul_core_id);
}

/*
 * 函 数 名  : frw_event_vap_flush_event
 * 功能描述  : 冲刷指定VAP、指定事件类型的所有事件，同时可以指定是丢弃这些事件还是全部处理
 * 输入参数  : uc_vap_id:     VAP ID值
 *             en_event_type: 事件类型
 *             en_drop:       事件丢弃(1)或者处理(0)
 * 返 回 值  : OAL_SUCC、OAL_FAIL或其他错误码
 */
oal_uint32 frw_event_vap_flush_event(oal_uint8 uc_vap_id,
                                     frw_event_type_enum_uint8 en_event_type,
                                     oal_bool_enum_uint8 en_drop)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;

    /* 获取核号 */
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_vap_flush_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    if (en_event_type == FRW_EVENT_TYPE_WLAN_TX_COMP) {
        uc_vap_id = 0;
    }

    us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + en_event_type;

    /* 根据核号 + 队列ID，找到相应的事件队列 */
    pst_event_mgmt = &g_event_manager[ul_core_id];
    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    /* 如果事件队列本身为空，没有事件，不在调度队列，返回错误 */
    if (pst_event_queue->st_queue.uc_element_cnt == 0) {
        return OAL_FAIL;
    }

    /* flush所有的event */
    while (pst_event_queue->st_queue.uc_element_cnt != 0) {
        pst_event_mem = (frw_event_mem_stru *)frw_event_queue_dequeue(pst_event_queue);
        if (pst_event_mem == OAL_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* 处理事件，否则直接释放事件内存而丢弃事件 */
        if (en_drop == 0) {
            /* 获取事件头结构 */
            pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);

            /* 根据事件找到对应的事件处理函数 */
            frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
        }

        /* 释放事件内存 */
        frw_event_free_m(pst_event_mem);
    }

    /* 若事件队列已经变空，需要将其从调度队列上删除，并将事件队列状态置为不活跃(不可被调度) */
    if (pst_event_queue->st_queue.uc_element_cnt == 0) {
        frw_event_sched_deactivate_queue(&g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy],
                                         pst_event_queue);
    } else {
        OAM_ERROR_LOG1(uc_vap_id, OAM_SF_FRW, "{flush vap event failed, left!=0: type=%d}", en_event_type);
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : frw_event_get_sched_queue
 * 功能描述  : 用于VAP暂停、恢复和冲刷函数进行IT测试，获取frw_event_main.c中定义的全局事件的调度队列
 * 输入参数  : ul_core_id: 核号
 *             en_policy:  调度策略
 * 返 回 值  : 调度队列指针
 */
frw_event_sched_queue_stru *frw_event_get_sched_queue(oal_uint32 ul_core_id, frw_sched_policy_enum_uint8 en_policy)
{
    if (oal_unlikely((ul_core_id >= WLAN_FRW_MAX_NUM_CORES) || (en_policy >= FRW_SCHED_POLICY_BUTT))) {
        return OAL_PTR_NULL;
    }

    return &(g_event_manager[ul_core_id].st_sched_queue[en_policy]);
}

/*
 * 函 数 名  : frw_is_event_queue_empty
 * 功能描述  : 判断所有VAP对应的事件队列是否为空
 * 输入参数  : uc_event_type: 事件类型
 * 返 回 值  : OAL_TRUE:  空
 */
oal_bool_enum_uint8 frw_is_event_queue_empty(frw_event_type_enum_uint8 uc_event_type)
{
    oal_uint32 ul_core_id;
    oal_uint8 uc_vap_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    /* 获取核号 */
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, core id = %d overflow!}", ul_core_id);

        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    pst_event_mgmt = &g_event_manager[ul_core_id];

    /* 遍历该核上每个VAP对应的事件队列， */
    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + uc_event_type;

        /* 根据核号 + 队列ID，找到相应的事件队列 */
        pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

        if (pst_event_queue->st_queue.uc_element_cnt != 0) {
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}

/*
 * 函 数 名  : frw_is_vap_event_queue_empty
 * 功能描述  : 根据核id和事件类型，判断vap事件队列是否空
 * 输入参数  : ul_core_id: 核id; event_type:  事件ID;
 * 返 回 值  : OAL_TRUE 或其它错误码
 */
oal_bool_enum_uint8 frw_is_vap_event_queue_empty(oal_uint32 ul_core_id, oal_uint8 uc_vap_id, oal_uint8 event_type)
{
    frw_event_mgmt_stru *pst_event_mgmt;
    frw_event_queue_stru *pst_event_queue;
    oal_uint16 us_qid;

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION)
    us_qid = (oal_uint16)event_type;
#else
    us_qid = (oal_uint16)(uc_vap_id * FRW_EVENT_TYPE_BUTT + event_type);
#endif

    /* 根据核号 + 队列ID，找到相应的事件队列 */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    if (pst_event_queue->st_queue.uc_element_cnt != 0) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/*
 * 函 数 名  : frw_task_thread_condition_check
 * 功能描述  : 判断是否有事件需要调度
 * 输入参数  : 核id
 * 返 回 值  : OAL_TRUE
 */
oal_uint8 frw_task_thread_condition_check(oal_uint32 ul_core_id)
{
    /* 返回OAL_TRUE
     * 1.调度队列非空
     *  2.调度队列里有非pause的队列
     */
    oal_uint8 sched_policy;
    oal_uint ul_irq_flag = 0;
    oal_dlist_head_stru *pst_list = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    pst_sched_queue = g_event_manager[ul_core_id].st_sched_queue;

    for (sched_policy = 0; sched_policy < FRW_SCHED_POLICY_BUTT; sched_policy++) {
        oal_spin_lock_irq_save(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
        /* 遍历整个调度链表 */
        oal_dlist_search_for_each(pst_list, &pst_sched_queue[sched_policy].st_head)
        {
            pst_event_queue = oal_dlist_get_entry(pst_list, frw_event_queue_stru, st_list);
            if (pst_event_queue->st_queue.uc_element_cnt == 0) {
                continue;
            }

            /* 如果事件队列的vap_state为暂停，则跳过，继续挑选下一个事件队列 */
            if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
                continue;
            }
            /* 找到事件队列非空 */
            oal_spin_unlock_irq_restore(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
            return OAL_TRUE;
        }
        oal_spin_unlock_irq_restore(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
    }
    /* 空返回OAL_FALSE */
    return OAL_FALSE;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(frw_event_dispatch_event);
oal_module_symbol(frw_event_post_event);
oal_module_symbol(frw_event_table_register);
oal_module_symbol(frw_event_dump_event);
oal_module_symbol(frw_event_process_all_event);
oal_module_symbol(frw_event_flush_event_queue);
oal_module_symbol(frw_event_queue_info);
oal_module_symbol(frw_event_get_info_from_event_queue);
oal_module_symbol(frw_event_vap_pause_event);
oal_module_symbol(frw_event_vap_resume_event);
oal_module_symbol(frw_event_vap_flush_event);
oal_module_symbol(frw_event_get_sched_queue);

oal_module_symbol(frw_is_event_queue_empty);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
oal_module_symbol(frw_event_sub_rx_adapt_table_init);
#endif
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
oal_module_symbol(g_event_time_track);
oal_module_symbol(g_rx_event_idx);
#endif
#ifdef _PRE_DEBUG_MODE
oal_module_symbol(g_en_event_track_switch);
oal_module_symbol(g_schedule_idx);
oal_module_symbol(g_schedule_time);
oal_module_symbol(g_mac_process_event);
#endif
