/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#ifndef __NV_COMM_H__
#define __NV_COMM_H__

#include <linux/slab.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <osl_thread.h>
#include <bsp_nvim.h>
#include "nv_partition.h"
#include "nv_msg.h"
#include "nv_debug.h"

#define NV_MBN_SIZE (128 * 1024)

#define nv_malloc(a) kmalloc(a, GFP_KERNEL)
#define nv_free(p) kfree(p)

enum nv_init_state {
    NVM_INIT_IDLE = 0,
    NVM_INIT_START = 1,
    NVM_INIT_READ = 2,
    NVM_INIT_SUCCESS = 3,
    NVM_INIT_FAILED = 4,
};

#define NV_SNFLAG_HD    (1 << NV_SECTION_HD)
#define NV_SNFLAG_RS    (1 << NV_SECTION_RS)
#define NV_SNFLAG_RO    (1 << NV_SECTION_RO)
#define NV_SNFLAG_RW    (1 << NV_SECTION_RW)
#define NV_SNFLAG_TAG   (1 << NV_SECTION_TAG)

#define NV_SNFLAG_HD_UNMASK    (~NV_SNFLAG_HD)
#define NV_SNFLAG_RS_UNMASK    (~NV_SNFLAG_RS)
#define NV_SNFLAG_RO_UNMASK    (~NV_SNFLAG_RO)
#define NV_SNFLAG_RW_UNMASK    (~NV_SNFLAG_RW)
#define NV_SNFLAG_TAG_UNMASK   (~NV_SNFLAG_TAG)

struct rwbuf_info {
    u64 rwaddr;
    u64 rwsize;
    osl_sem_id sem;
    u8 rsv[0x4];
};

#define NV_LOAD_BUF_SIZE (15 * 1024 * 1024)
#define NV_GLOBAL_CTRL_SIZE (0x400)

struct nvm_info {
    int nvm_init_state;
    u64 dtaddr;   /* nv data base addr */
    u64 dtsize;   /* nv data mem */
    u64 mbnaddr;
    u64 mbnsize;
    u64 ldaddr;
    u64 ldsize;
    u32 flush_time;
    osl_sem_id task_sem;
    OSL_TASK_ID task_id;
    struct wakeup_source wakelock;
    struct nvm_fs nvmfs;
    struct nvm_msg_info msg_info;
    struct rwbuf_info rwbuf;
    struct nvm_debug_stru debug_info;
};

struct nvm_info *nv_get_nvminfo(void);

u64 nv_get_mem_addr(void);

u64 nv_get_mbnaddr(void);

struct nv_ctrl_info *nv_get_ctrl_info(void);

struct section_info *nv_get_sninfo(void);

struct item_info *nv_get_item_info(void);

u32 nv_flush_data(u32 msg);

u32 nv_flush_rwbuf(u32 buf);

u32 nv_reload_flush(void);

u32 nv_update_factory(void);

u32 nv_update_backup(void);

u32 nv_factory_reset(void);

u32 nv_set_upgrade_flag(void);

u32 nv_upgrade_flush(void);
#endif
