

#ifndef __PLAT_FIRMWARE_H__
#define __PLAT_FIRMWARE_H__

/* 头文件包含 */
#include "plat_type.h"
#include "oal_net.h"
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_exception_rst.h"
#endif
/* 宏定义 */
/* string */
#define os_mem_cmp(dst, src, size)  memcmp(dst, src, size)
#define os_str_cmp(dst, src)        strcmp(dst, src)
#define os_str_len(s)               strlen(s)
#define os_str_chr(str, chr)        strchr(str, chr)
#define os_str_str(dst, src)        strstr(dst, src)

/* memory */
#define os_mem_kfree(p)      kfree(p)
#define os_kmalloc_gfp(size) kmalloc(size, (GFP_KERNEL | __GFP_NOWARN))
#define os_kzalloc_gfp(size) kzalloc(size, (GFP_KERNEL | __GFP_NOWARN))
#define os_vmalloc_gfp(size) vmalloc(size)
#define os_mem_vfree(p)      vfree(p)

#define READ_MEG_TIMEOUT      2000  /* 200ms */
#define READ_MEG_JUMP_TIMEOUT 15000 /* 15s */

#define FILE_CMD_WAIT_TIME_MIN 5000 /* 5000us */
#define FILE_CMD_WAIT_TIME_MAX 5100 /* 5100us */

#define SEND_BUF_LEN 520
#define RECV_BUF_LEN 512
#define VERSION_LEN  64

#define READ_CFG_BUF_LEN 2048

#define DOWNLOAD_CMD_LEN      32
#define DOWNLOAD_CMD_PARA_LEN 800

#define HOST_DEV_TIMEOUT 3
#define INT32_STR_LEN    32

#define SHUTDOWN_TX_CMD_LEN 64

#define CMD_JUMP_EXEC_RESULT_SUCC 0
#define CMD_JUMP_EXEC_RESULT_FAIL 1

#define WIFI_MODE_DISABLE 0
#define WIFI_MODE_2G      1
#define WIFI_MODE_5G      2
#define WIFI_MODE_2G_5G   3

#define SOFT_WCPU_EN_ADDR  "0x50000c00"
#define SOFT_BCPU_EN_ADDR  "0x50000c04"
#define BCPU_DE_RESET_ADDR "0x50000094"

/* 发往device命令在cfg文件中的第一个字符  例: 2,0x50002a00,0x0 */
#define HOST_TO_DEVICE_CMD_HEAD  2

/* 以下是发往device命令的关键字 */
#define VER_CMD_KEYWORD          "VERSION"
#define JUMP_CMD_KEYWORD         "JUMP"
#define FILES_CMD_KEYWORD        "FILES"
#define SETPM_CMD_KEYWORD        "SETPM"
#define SETBUCK_CMD_KEYWORD      "SETBUCK"
#define SETSYSLDO_CMD_KEYWORD    "SETSYSLDO"
#define SETNFCRETLDO_CMD_KEYWORD "SETNFCRETLDO"
#define SETPD_CMD_KEYWORD        "SETPD"
#define SETNFCCRG_CMD_KEYWORD    "SETNFCCRG"
#define SETABB_CMD_KEYWORD       "SETABB"
#define SETTCXODIV_CMD_KEYWORD   "SETTCXODIV"
#define RMEM_CMD_KEYWORD         "READM"
#define WMEM_CMD_KEYWORD         "WRITEM"
#define QUIT_CMD_KEYWORD         "QUIT"

/* 以下命令字不会发往device，用于控制校准流程，但是会体现在cfg文件中 */
#define CALI_COUNT_CMD_KEYWORD     "CALI_COUNT"
#define CALI_BFGX_DATA_CMD_KEYWORD "CALI_BFGX_DATA"
#define CALI_DCXO_DATA_CMD_KEYWORD "CALI_DCXO_DATA"
#define SHUTDOWN_WIFI_CMD_KEYWORD  "SHUTDOWN_WIFI"
#define SHUTDOWN_BFGX_CMD_KEYWORD  "SHUTDOWN_BFGX"

/* 以下是device对命令执行成功返回的关键字，host收到一下关键字则命令执行成功 */
#define MSG_FROM_DEV_WRITEM_OK "WRITEM OK"
#define MSG_FROM_DEV_READM_OK  ""
#define MSG_FROM_DEV_FILES_OK  "FILES OK"
#define MSG_FROM_DEV_READY_OK  "READY"
#define MSG_FROM_DEV_JUMP_OK   "JUMP OK"
#define MSG_FROM_DEV_SET_OK    "SET OK"
#define MSG_FROM_DEV_QUIT_OK   ""

/* 以下是cfg文件配置命令的参数头，一条合法的cfg命令格式为:参数头+命令关键字(QUIT命令除外) */
#define FILE_TYPE_CMD_KEY "ADDR_FILE_"
#define NUM_TYPE_CMD_KEY  "PARA_"

#define COMPART_KEYWORD ' '
#define CMD_LINE_SIGN   ';'

#define CFG_INFO_RESERVE_LEN 8

#define HI1103_ASIC_MPW2  0
#define HI1103_ASIC_PILOT 1

#define BFGX_AND_WIFI_CFG_PATH "/vendor/firmware/bfgx_and_wifi_cfg"
#define WIFI_CFG_PATH          "/vendor/firmware/wifi_cfg"
#define BFGX_CFG_PATH          "/vendor/firmware/bfgx_cfg"
#define RAM_CHECK_CFG_PATH     "/vendor/firmware/ram_reg_test_cfg"

#define BFGX_AND_WIFI_CFG_HI1103_MPW2_PATH "/vendor/firmware/hi1103/mpw2/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1103_MPW2_PATH          "/vendor/firmware/hi1103/mpw2/wifi_cfg"
#define BFGX_CFG_HI1103_MPW2_PATH          "/vendor/firmware/hi1103/mpw2/bfgx_cfg"
#define RAM_CHECK_CFG_HI1103_MPW2_PATH     "/vendor/firmware/hi1103/mpw2/ram_reg_test_cfg"

#define BFGX_AND_WIFI_CFG_HI1103_PILOT_PATH  "/vendor/firmware/hi1103/pilot/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1103_PILOT_PATH           "/vendor/firmware/hi1103/pilot/wifi_cfg"
#define BFGX_CFG_HI1103_PILOT_PATH           "/vendor/firmware/hi1103/pilot/bfgx_cfg"
#define RAM_CHECK_CFG_HI1103_PILOT_PATH      "/vendor/firmware/hi1103/pilot/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1103_PILOT_PATH "/vendor/firmware/hi1103/pilot/reg_bcpu_mem_test_cfg"

#define HI1103_MPW2_BOOTLOADER_VERSION  "Hi1103V100R001C01B060 Feb 10 2017"
#define HI1103_PILOT_BOOTLOADER_VERSION "Hi1103V100R001C01B083 Dec 16 2017"

#define FILE_COUNT_PER_SEND          1
#define MIN_FIRMWARE_FILE_TX_BUF_LEN 4096
#define MAX_FIRMWARE_FILE_TX_BUF_LEN (512 * 1024)
#define DEV_MEMDUMP_BUFF_LEN         100
#define QUIT_CMD_BUF_LEN             8
/* dump the device cpu reg mem when panic,
  24B mem header + 24*4 reg info */
#define CPU_PANIC_MEMDUMP_HEAD_SIZE  24
#define CPU_PANIC_MEMDUMP_INFO_SIZE  (24 * 4)
#define CPU_PANIC_MEMDUMP_SIZE       (CPU_PANIC_MEMDUMP_HEAD_SIZE + CPU_PANIC_MEMDUMP_INFO_SIZE)

/* 枚举定义 */
enum return_type {
    SUCC = 0,
    EFAIL,
};

enum FIRMWARE_CFG_CMD_ENUM {
    ERROR_TYPE_CMD = 0,     /* 错误的命令 */
    FILE_TYPE_CMD,          /* 下载文件的命令 */
    NUM_TYPE_CMD,           /* 下载配置参数的命令 */
    QUIT_TYPE_CMD,          /* 退出命令 */
    SHUTDOWN_WIFI_TYPE_CMD, /* SHUTDOWN WCPU命令 */
    SHUTDOWN_BFGX_TYPE_CMD  /* SHUTDOWN BCPU命令 */
};

/*
 * 1.首次开机时，使用BFGN_AND_WIFI_CFG，完成首次开机校准，host保存校准数据，完成后device下电
 * 2.deivce下电状态，开wifi，使用BFGN_AND_WIFI_CFG
 * 3.deivce下电状态，开bt，使用BFGX_CFG
 * 4.bt开启，再开wifi，使用WIFI_CFG
 * 5.wifi开启，再开bt，通过sdio解复位BCPU
 */
enum FIRMWARE_CFG_FILE_ENUM {
    BFGX_AND_WIFI_CFG = 0, /* 加载BFGIN和wifi fimware的命令数组index，执行独立校准 */
    WIFI_CFG,              /* 只加载wifi firmware的命令数组索引，执行独立校准 */
    BFGX_CFG,              /* 只加载bfgx firmware的命令数组索引，不执行独立校准 */
    RAM_REG_TEST_CFG,      /* 产线测试wcpu mem reg遍历使用 */
    RAM_BCPU_REG_TEST_CFG, /* 产线测试bcpu mem reg遍历使用 */
    BFGX_AND_HITALK_CFG,   /* 加载BFGIN和hitalk fimware的命令数组index，执行独立校准 */
    HITALK_CFG,            /* 只加载hitalk firmware的命令数组索引，执行独立校准 */

    CFG_FILE_TOTAL
};

enum FIRMWARE_SUBSYS_ENUM {
    DEV_WCPU = 0,
    DEV_BCPU,
    DEV_CPU_BUTT,
};

/* 全局变量定义 */
extern uint8 **g_cfg_path;
extern uint8 *g_pilot_cfg_patch_in_vendor[CFG_FILE_TOTAL];

/* STRUCT定义 */
typedef struct cmd_type_st {
    int32 cmd_type;
    uint8 cmd_name[DOWNLOAD_CMD_LEN];
    uint8 cmd_para[DOWNLOAD_CMD_PARA_LEN];
} cmd_type_struct;

typedef struct firmware_globals_st {
    int32 count[CFG_FILE_TOTAL];            /* 存储每个cfg文件解析后有效的命令个数 */
    cmd_type_struct *apst_cmd[CFG_FILE_TOTAL]; /* 存储每个cfg文件的有效命令 */
    uint8 cfg_version[VERSION_LEN];         /* 存储cfg文件中配置的版本号信息 */
    uint8 dev_version[VERSION_LEN];         /* 存储加载时device侧上报的版本号信息 */
} firmware_globals_struct;

/* OTHERS定义 */
typedef struct file os_kernel_file_stru;
extern firmware_globals_struct g_cfg_info;

/* 函数声明 */
extern void set_hi1103_asic_type(uint32 ul_asic_type);
extern uint32 get_hi1103_asic_type(void);
extern int32 firmware_download(uint32 ul_index);
extern int32 firmware_cfg_init(void);
extern int32 firmware_get_cfg(uint8 *cfg_patch, uint32 ul_index);
extern void firmware_cfg_clear(void);
extern int32 wifi_device_mem_dump(struct st_wifi_dump_mem_info *pst_mem_dump_info, uint32 count);
extern int32 read_device_reg16(uint32 address, uint16 *value);
extern int32 write_device_reg16(uint32 address, uint16 value);
extern int32 is_device_mem_test_succ(void);
extern int32 get_device_test_mem(uint8 *file_name);
#endif /* end of plat_firmware.h */
