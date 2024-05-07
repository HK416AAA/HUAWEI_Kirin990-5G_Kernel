/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */
#ifndef __NV_DEBUG_H__
#define __NV_DEBUG_H__

#include <bsp_print.h>

#define THIS_MODU mod_nv

#define CONFIG_NV_DEBUG 0
#define nv_dbg_printf(fmt, ...)          \
    do {                                 \
        if (CONFIG_NV_DEBUG) {           \
            bsp_err(fmt, ##__VA_ARGS__); \
        }                                \
    } while (0);

enum {
    NV_CRIT,
    NV_ERR,
    NV_WARN,
    NV_INFO,
    NV_DBG,
    NV_DEFT,
};

#define CONFIG_NV_DEBUG_LEVEL 3
#define nv_debug_printf(level, fmt, ...)     \
    do {                                     \
        if (level < CONFIG_NV_DEBUG_LEVEL) { \
            bsp_err(fmt, ##__VA_ARGS__);     \
        }                                    \
    } while (0);

#define nv_printf(fmt, ...) bsp_err(fmt, ##__VA_ARGS__)

#define NV_DUMP_SIZE (4 * 1024)

enum {
    NV_API_READ = 0,
    NV_FUN_READ_EX = 1,
    NV_API_AUTHREAD = 2,
    NV_API_DIRECTREAD = 3,
    NV_API_WRITE = 4,
    NV_FUN_WRITE_EX = 5,
    NV_API_AUTHWRITE = 6,
    NV_API_DIRECTWRITE = 7,
    NV_API_GETLEN = 8,
    NV_API_AUTHGETLEN = 9,
    NV_API_FLUSH = 10,
    NV_API_REVERT = 11,
    NV_API_BACKUP = 12,
    NV_API_REVERT_MANF = 13,
    NV_API_REVERT_USER = 14,
    NV_API_IMPORT = 15,
    NV_API_EXPORT = 16,
    NV_FUN_MEM_INIT = 17,
    NV_FUN_UPGRADE_PROC = 18,
    NV_FUN_KERNEL_INIT = 19,
    NV_FUN_REMAIN_INIT = 20,
    NV_FUN_NVM_INIT = 21,
    NV_FUN_XML_DECODE = 22,
    NV_FUN_SEARCH_NV = 23,
    NV_FUN_REVERT_DATA = 24,
    NV_FUN_UPDATE_DEFAULT = 25,
    NV_FUN_REVERT_DEFAULT = 26,
    NV_FUN_ERR_RECOVER = 27,
    NV_FUN_KEY_CHECK = 28,
    NV_FUN_FLUSH_SYS = 29,
    NV_API_FLUSH_LIST = 32,
    NV_CRC32_MAKE_DDR = 33,
    NV_CRC32_DATA_CHECK = 34,
    NV_CRC32_GET_FILE_LEN = 35,
    NV_CRC32_MAKE_DDR_CRC = 36,
    NV_CRC32_DLOAD_FILE_CHECK = 37,
    NV_CRC32_DDR_CRC_CHECK = 38,
    NV_CRC32_DDR_RESUME_IMG = 39,
    NV_FUN_READ_FILE = 40,
    NV_FUN_RESUME_ITEM = 41,
    NV_FUN_RESUME_IMG_ITEM = 42,
    NV_FUN_RESUME_BAK_ITEM = 43,
    NV_FUN_MAX_ID,
};

typedef struct debug_point {
    u32 callnum;
    u32 rsv1;
    u32 rsv2;
    u32 rsv3;
    u32 rsv4;
} debug_point;

enum nv_debug_state {
    NV_DEBUG_WRITEEX_START = 1,
    NV_DEBUG_WRITEEX_GET_IPC_START,
    NV_DEBUG_WRITEEX_GET_IPC_END,
    NV_DEBUG_WRITEEX_GIVE_IPC,
    NV_DEBUG_WRITEEX_MEM_START,
    NV_DEBUG_WRITEEX_FILE_START,
    NV_DEBUG_FLUSH_START,
    NV_DEBUG_FLUSH_END,
    NV_DEBUG_REQ_FLUSH_START,
    NV_DEBUG_REQ_FLUSH_END,
    NV_DEBUG_FLUSHEX_START,
    NV_DEBUG_FLUSHEX_OPEN_START,
    NV_DEBUG_FLUSHEX_OPEN_END,
    NV_DEBUG_FLUSHEX_GET_PROTECT_SEM_START,
    NV_DEBUG_FLUSHEX_GET_PROTECT_SEM_END,
    NV_DEBUG_FLUSHEX_GIVE_IPC,
    NV_DEBUG_FLUSHEX_GIVE_SEM,
    NV_DEBUG_FLUSHEX_WRITE_FILE_START,
    NV_DEBUG_FLUSHEX_WRITE_FILE_END,
    NV_DEBUG_WRITEEX_END,
    NV_DEBUG_SEND_MSG_START,
    NV_DEBUG_SEND_ICC,
    NV_DEBUG_SEND_MSG_TIMEOUT,
    NV_DEBUG_RECEIVE_ICC,
    NV_DEBUG_READ_ICC,
    NV_DEBUG_SEND_MSG_END,
    NV_DEBUG_REQ_ASYNC_WRITE,
    NV_DEBUG_REQ_REMOTE_WRITE,
    NV_DEBUG_BUTT
};

typedef struct debug_element {
    u32 state;
    u32 slice;
} debug_element;

typedef struct {
    u32 maxnum;
    u32 head;
    u32 tail;
    u32 num;
    debug_element data[0];
} debug_queue;

typedef struct {
    u32 state;
    char *info;
} debug_table;

struct nvm_debug_stru {
    u32 print;
    debug_point point[NV_FUN_MAX_ID];
    debug_queue *dump_queue;
    debug_table *table;
};

u32 nv_debug_init(void);

void nv_debug_record(u32 state);

void nv_debug(u32 type, u32 rsv1, u32 rsv2, u32 rsv3, u32 rsv4);

void nv_help(u32 type);

void nv_show_ddr_info(void);

void nv_show_ref_info(u32 startid, u32 endid);

u32 nv_show_item_info(u32 itemid);

#endif
