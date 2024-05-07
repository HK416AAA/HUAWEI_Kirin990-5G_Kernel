/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * 1 Include HeadFile
 */
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <securec.h>
#include "diag_fs_proc.h"
#include "vos.h"
#include "msp_errno.h"
#include "diag_common.h"
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID MSP_FILE_ID_DIAG_FS_PROC_C

/*
 * 2 Declare the Global Variable
 */
const VOS_CHAR g_acDiagRootPath[] = "/";
const VOS_CHAR g_acDiagDumpDir[] = MODEM_LOG_ROOT;
const VOS_CHAR g_acDiagLogDir[] = MODEM_LOG_ROOT;

DIAG_FILE_INFO_STRU g_stDiagFileInfo = { DIAG_FILE_NULL, DIAG_FS_FOLDER_BUTT, VOS_NULL_PTR };

/*
 * 3 Function
 */
/*lint -e501 -esym(501,*)*/

VOS_VOID diag_FsClose(VOS_VOID)
{
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    if (g_stDiagFileInfo.lFile != DIAG_FILE_NULL) {
        DIAG_FS_CLOSE(g_stDiagFileInfo.lFile);
        g_stDiagFileInfo.lFile = DIAG_FILE_NULL;
    }

    (VOS_VOID)DIAG_FS_FILE_SYNC(g_stDiagFileInfo.lFile);
    set_fs(old_fs);
}


VOS_UINT32 diag_FsGetDirInfo(VOS_CHAR *pDirName, VOS_UINT32 *pulTotalNum)
{
    VOS_INT32 dir_handle = 0;
    unsigned long nRead = 0;
    VOS_INT32 nCount = 0;
    VOS_UINT32 i;
    VOS_UINT8 *buf = NULL;

    struct linux_dirent64 *pstTmpDirent = NULL;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    /* 打开目录 */
    if ((dir_handle = DIAG_FS_OPENDIR((VOS_CHAR *)pDirName, DIAG_FS_RDONLY | DIAG_FS_DIRECTORY, 0)) < 0) {
        set_fs(old_fs);
        diag_error("DIAG_FS_OPENDIR error!\n");
        return VOS_ERR;
    }

    buf = (VOS_UINT8 *)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT, MSP_DF_DIR_BUF_LEN);
    if (buf == NULL) {
        diag_error("Alloc mem error\n");
        DIAG_FS_CLOSEDIR(dir_handle);
        set_fs(old_fs);
        return VOS_ERR;
    }

    nRead = sys_getdents64(dir_handle, (struct linux_dirent64 __user *)buf, MSP_DF_DIR_BUF_LEN);
    if (nRead == -1) {
        diag_error("dents error,nRead=%d\n", (VOS_INT)nRead);
        *pulTotalNum = 0;
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);
        DIAG_FS_CLOSEDIR(dir_handle);
        set_fs(old_fs);
        return VOS_ERR;
    }

    if (nRead == 0) {
        diag_error("dents zero!\n");
        *pulTotalNum = 0;
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);
        DIAG_FS_CLOSEDIR(dir_handle);
        set_fs(old_fs);
        return VOS_OK;
    }

    for (i = 0; i < (VOS_UINT32)nRead;) {
        pstTmpDirent = (struct linux_dirent64 *)(buf + i);
        i += pstTmpDirent->d_reclen;

        if ((VOS_StrCmp((char *)pstTmpDirent->d_name, ".") == 0) ||
            (VOS_StrCmp((char *)pstTmpDirent->d_name, "..") == 0)) {
            diag_error("diag_FsGetDirInfo:d_name=%s\n", pstTmpDirent->d_name);
            continue;
        }
        nCount++;
    }

    *pulTotalNum = nCount;

    /* 关闭目录 */
    if (DIAG_FS_CLOSEDIR(dir_handle) == DIAG_FS_ERROR) {
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);
        diag_error("DIAG_FS_CLOSEDIR zero!\n");
        set_fs(old_fs);
        return VOS_ERR;
    }

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);

    set_fs(old_fs);
    return VOS_OK;
}

/*
 * Function Name: diag_FsGetItemInfo
 * Description: 获取文件或文件夹信息
 * VOS_CHAR *pDirName
 * Output: None
 * Return: VOS_UINT32
 */
VOS_UINT32 diag_FsGetItemInfo(VOS_CHAR *pDirectory, DIAG_DIR_FILE_INFO_STRU *pstDirFileInfo)
{
    struct linux_dirent64 *pstTmpDirent = VOS_NULL;
    DIAG_STAT_STRU stStat;
    VOS_UINT8 ucDirName[MSP_DF_DIR_MAX_LEN];
    VOS_INT32 dir_handle = 0;
    unsigned long nRead = 0;
    VOS_INT32 i;
    VOS_UINT32 len;
    VOS_UINT8 *buf = NULL;
    mm_segment_t old_fs;
    errno_t err;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    /* 再次打开目录 */
    if ((dir_handle = DIAG_FS_OPENDIR(pDirectory, DIAG_FS_RDONLY | DIAG_FS_DIRECTORY, 0)) < 0) {
        diag_error("DIAG_FS_OPENDIR error!");
        set_fs(old_fs);
        return VOS_ERR;
    }

    buf = (VOS_UINT8 *)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT, MSP_DF_DIR_BUF_LEN);
    if (buf == NULL) {
        diag_error("Alloc mem error!");
        DIAG_FS_CLOSEDIR(dir_handle);
        set_fs(old_fs);
        return VOS_ERR;
    }

    nRead = sys_getdents64(dir_handle, (struct linux_dirent64 __user *)buf, MSP_DF_DIR_BUF_LEN);
    if ((nRead == -1) || (nRead == 0)) {
        diag_error("dents error,nRead=%d!\n", (VOS_INT)nRead);
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);
        DIAG_FS_CLOSEDIR(dir_handle);
        set_fs(old_fs);
        return VOS_ERR;
    }

    /* 轮询文件夹将所有文件名保存至全局变量 */
    for (i = 0; i < nRead;) {
        pstTmpDirent = (struct linux_dirent64 *)(buf + i);
        i += pstTmpDirent->d_reclen;

        if ((VOS_StrCmp((char *)pstTmpDirent->d_name, ".") == 0) ||
            (VOS_StrCmp((char *)pstTmpDirent->d_name, "..") == 0)) {
            diag_error("d_name=%s!\n", pstTmpDirent->d_name);
            continue;
        }

        err = memset_s(ucDirName, MSP_DF_DIR_MAX_LEN, 0, MSP_DF_DIR_MAX_LEN);
        if (err != EOK) {
            diag_error("memset fail:%x\n", err);
        }
        err = memcpy_s(ucDirName, sizeof(ucDirName), pDirectory, VOS_StrNLen(pDirectory, (MSP_DF_DIR_MAX_LEN - 1)));
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }

        len = VOS_StrLen(pDirectory);
        if (len >= (MSP_DF_DIR_MAX_LEN - 1)) {
            continue;
        }
        ucDirName[len] = '/';
        len++;

        if (len >= (MSP_DF_DIR_MAX_LEN - 1)) {
            continue;
        }

        err = memcpy_s((ucDirName + len), sizeof(ucDirName) - len, pstTmpDirent->d_name,
            VOS_StrNLen(pstTmpDirent->d_name, sizeof(ucDirName) - len));
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }

        /* 通过stat获取文件或文件夹的信息 */
        if (DIAG_FS_STAT((VOS_CHAR *)ucDirName, &stStat) == DIAG_FS_ERROR) {
            diag_error("DIAG_FS_STAT: error!\n");
            pstDirFileInfo->ulItemType = DIAG_FS_ITEM_FILE;
            pstDirFileInfo->st_size = 0;
            pstDirFileInfo->st_mode = 0;
            pstDirFileInfo->st_atime = 0;
            pstDirFileInfo->st_mtime = 0;
            pstDirFileInfo->st_ctime = 0;
        } else {
            /* 目录 */
            if ((DIAG_IF_DIR & stStat.mode) != 0) {
                pstDirFileInfo->ulItemType = DIAG_FS_ITEM_FOLDER;
            } else { /* 文件 */
                pstDirFileInfo->ulItemType = DIAG_FS_ITEM_FILE;
            }

            pstDirFileInfo->st_size = (stStat.size & 0xFFFFFFFF); /* 目前文件大小不会超过32位大小 */
            pstDirFileInfo->st_atime = stStat.atime.tv_sec;
            pstDirFileInfo->st_mtime = stStat.mtime.tv_sec;
            pstDirFileInfo->st_ctime = stStat.ctime.tv_sec;
            pstDirFileInfo->st_mode = stStat.mode;
        }

        /* 文件或文件夹名 */
        err = memset_s(pstDirFileInfo->aucDirName, sizeof(pstDirFileInfo->aucDirName), 0,
            sizeof(pstDirFileInfo->aucDirName));
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }
        err = memcpy_s(pstDirFileInfo->aucDirName, sizeof(pstDirFileInfo->aucDirName) - 1, pstTmpDirent->d_name,
            VOS_StrNLen(pstTmpDirent->d_name, sizeof(pstDirFileInfo->aucDirName) - 1));
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }

        pstDirFileInfo++;
    }

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, buf);
    DIAG_FS_CLOSEDIR(dir_handle);
    set_fs(old_fs);

    return VOS_OK;
}

#define DIAG_FS_MODE 0755
VOS_INT32 diag_FsOpen(const VOS_CHAR *pcFileName, VOS_INT lFlag)
{
    VOS_INT32 fp;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    fp = DIAG_FS_OPEN(pcFileName, lFlag, DIAG_FS_MODE);
    diag_crit("open %s\n", pcFileName);

    set_fs(old_fs);

    return (VOS_INT32)fp;
}


VOS_UINT32 diag_FsQueryProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_QUERY_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_QUERY_CNF stFsCnf = { 0 };
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };
    errno_t err;

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_QUERY_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_QUERY_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_QUERY_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    if (pstFsReq->ulFolderType == DIAG_FS_ROOT_FOLDER) {
        stFsCnf.ulLength = VOS_StrLen((VOS_CHAR *)g_acDiagRootPath);
        err = memcpy_s(stFsCnf.aucDirPath, sizeof(stFsCnf.aucDirPath), g_acDiagRootPath, stFsCnf.ulLength);
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }

        g_stDiagFileInfo.ulFileType = DIAG_FS_ROOT_FOLDER;
    } else if (pstFsReq->ulFolderType == DIAG_FS_LOG_FOLDER) {
        stFsCnf.ulLength = VOS_StrLen((VOS_CHAR *)g_acDiagLogDir);
        err = memcpy_s(stFsCnf.aucDirPath, sizeof(stFsCnf.aucDirPath), g_acDiagLogDir, stFsCnf.ulLength);
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }
        g_stDiagFileInfo.ulFileType = DIAG_FS_LOG_FOLDER;
    } else if (pstFsReq->ulFolderType == DIAG_FS_DUMP_FOLDER) { /* 临终遗言文件所在的路径 */
        stFsCnf.ulLength = VOS_StrLen((VOS_CHAR *)g_acDiagDumpDir);
        err = memcpy_s(stFsCnf.aucDirPath, sizeof(stFsCnf.aucDirPath), g_acDiagDumpDir, stFsCnf.ulLength);
        if (err != EOK) {
            diag_error("memcpy fail:%x\n", err);
        }
        g_stDiagFileInfo.ulFileType = DIAG_FS_DUMP_FOLDER;
    } else {
        ret = ERR_MSP_FAILURE;
    }
    stFsCnf.ulRet = ret;

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    ret = DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));

    return ret;
}


VOS_UINT32 diag_FsScanProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_SCAN_DIR_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_SCAN_DIR_CNF *pstFsCnf = NULL;
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT32 ulTotalNum;
    VOS_UINT32 ulTotalSize;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_SCAN_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_SCAN_DIR_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_SCAN_DIR_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    /* 得到目录内，文件和文件夹的总个数、总的名字长度 */
    if (diag_FsGetDirInfo(pstFsReq->szDirectory, &ulTotalNum) != VOS_OK) {
        diag_error("diag_FsGetDirInfo fail\n");
        return VOS_ERR;
    }

    /* 计算返回给工具侧消息包的长度 */
    ulTotalSize = (ulTotalNum * sizeof(DIAG_DIR_FILE_INFO_STRU) + sizeof(DIAG_CMD_FS_SCAN_DIR_CNF));
    if (ulTotalSize > DIAG_MEM_ALLOC_LEN_MAX) {
        diag_error("ERROR: ulTotalNum=%d\n", ulTotalNum);
        return VOS_ERR;
    }

    pstFsCnf = (DIAG_CMD_FS_SCAN_DIR_CNF *)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT, ulTotalSize);
    if (pstFsCnf == VOS_NULL_PTR) {
        diag_error("malloc fail\n");
        return VOS_ERR;
    }

    /* 获取每个文件和文件夹的信息 */
    if (ulTotalNum > 0) {
        ret = diag_FsGetItemInfo(pstFsReq->szDirectory, pstFsCnf->stDirInfo);
    }

    pstFsCnf->ulRet = ret;
    pstFsCnf->ulDirNum = ulTotalNum;
    pstFsCnf->ulDirInfoLen = ulTotalNum * sizeof(DIAG_DIR_FILE_INFO_STRU);

    DIAG_MSG_COMMON_PROC(stDiagInfo, (*pstFsCnf), pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    ret = DIAG_MsgReport(&stDiagInfo, pstFsCnf, ulTotalSize);

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pstFsCnf);

    return ret;
}


VOS_UINT32 diag_FsMkdirProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_MAKE_DIR_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_MAKE_DIR_CNF stFsCnf = { 0 };
    VOS_UINT32 ret;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    mm_segment_t old_fs;

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_MAKE_DIR_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_MAKE_DIR_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_MAKE_DIR_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    ret = (VOS_UINT32)DIAG_FS_MKDIR((VOS_CHAR *)pstFsReq->szDirectory, DIAG_FS_MODE);

    stFsCnf.ulRet = ret;

    set_fs(old_fs);

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
}


VOS_UINT32 diag_FsOpenProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_OPEN_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_OPEN_CNF stFsCnf = { 0 };
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_OPEN_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_OPEN_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_OPEN_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    /* 不支持操作文件的重入操作 */
    if (g_stDiagFileInfo.lFile != DIAG_FILE_NULL) {
        /* 文件导出过程中可能文件size还在增加，导致上一个文件没读完工具就开始新的文件导出 */
        diag_FsClose();
    }
    diag_crit("start to open %s\n", pstFsReq->szDirectory);
    g_stDiagFileInfo.lFile = diag_FsOpen(pstFsReq->szDirectory, (VOS_INT)(pstFsReq->ulMode));

    if (g_stDiagFileInfo.lFile < 0) {
        g_stDiagFileInfo.lFile = DIAG_FILE_NULL;
        ret = VOS_ERR;
    } else {
        diag_crit("open %s success\n", pstFsReq->szDirectory);
    }
    stFsCnf.ulRet = ret;

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    diag_crit("open %s finish\n", pstFsReq->szDirectory);

    return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
}


VOS_UINT32 diag_FsImportProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_IMPORT_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_IMPORT_CNF stFsCnf = { 0 };
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_INT32 lResult;
    mm_segment_t old_fs;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_IMPORT_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_IMPORT_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_IMPORT_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    if (g_stDiagFileInfo.lFile == DIAG_FILE_NULL) {
        diag_error("lFile is null\n");
        return ERR_MSP_FAILURE;
    }
    diag_crit("start to import file\n");

    /* 写文件完毕，关闭文件 */
    if (pstFsReq->ulSize == 0) {
        diag_crit("pstFsReq->ulSize == 0\n");
        diag_FsClose();
        /* 打包回复给FW */
        stFsCnf.ulRet = ERR_MSP_SUCCESS;

        DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

        stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

        return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    lResult = DIAG_FS_WRITE(g_stDiagFileInfo.lFile, pstFsReq->acContent, pstFsReq->ulSize);

    set_fs(old_fs);

    /* 写文件操作失败或者写入长度不正确 */
    if ((lResult == DIAG_FS_ERROR) || (lResult != (VOS_INT32)(pstFsReq->ulSize))) {
        diag_error("lResult=%d\n", lResult);
        diag_FsClose();
        return VOS_ERR;
    }

    stFsCnf.ulRet = ret;

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
}


VOS_UINT32 diag_FsExportProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_EXPORT_CNF *pstFsCnf = NULL;
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT32 ulReadSize;
    mm_segment_t old_fs;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_EXOPORT_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (g_stDiagFileInfo.lFile == DIAG_FILE_NULL) {
        diag_error("lFile is NULL\n");
        return ERR_MSP_FAILURE;
    }

    pstFsCnf = (DIAG_CMD_FS_EXPORT_CNF *)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,
        sizeof(DIAG_CMD_FS_EXPORT_CNF) + DIAG_TRANSFER_BLOCK_MAX_SIZE);
    if (pstFsCnf == VOS_NULL_PTR) {
        diag_error("malloc fail\n");
        diag_FsClose();
        return ERR_MSP_FAILURE;
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    ulReadSize = (VOS_UINT32)DIAG_FS_READ(g_stDiagFileInfo.lFile, (VOS_CHAR *)pstFsCnf + sizeof(DIAG_CMD_FS_EXPORT_CNF),
        DIAG_TRANSFER_BLOCK_MAX_SIZE);

    set_fs(old_fs);

    /* 读取文件出现错误 */
    if ((VOS_INT32)ulReadSize == DIAG_FS_ERROR) {
        diag_error("read file error\n");
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pstFsCnf);
        diag_FsClose();
        return ERR_MSP_FAILURE;
    }

    /* 表明已经没有内容可以读取，文件内容全部读完 */
    if (ulReadSize == 0) {
        diag_crit("read finish\n");
        diag_FsClose();
    } else {
        diag_crit("has data read continue\n");
    }

    pstFsCnf->ulRet = ret;
    pstFsCnf->ulSize = ulReadSize;

    DIAG_MSG_COMMON_PROC(stDiagInfo, (*pstFsCnf), pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    ret = DIAG_MsgReport(&stDiagInfo, pstFsCnf, (sizeof(DIAG_CMD_FS_EXPORT_CNF) + ulReadSize));

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pstFsCnf);
    diag_crit("export finish\n");
    return ret;
}


VOS_UINT32 diag_FsDeleteProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_DELETE_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_DELETE_CNF stFsCnf = { 0 };
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    mm_segment_t old_fs;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_DELETE_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_DELETE_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_DELETE_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    if (pstFsReq->ulItemType == DIAG_FS_ITEM_FOLDER) {
        ret = (VOS_UINT32)DIAG_FS_RMDIR(pstFsReq->szDirectory);
    } else if (pstFsReq->ulItemType == DIAG_FS_ITEM_FILE) {
        ret = (VOS_UINT32)DIAG_FS_RMFILE(pstFsReq->szDirectory);
    }

    set_fs(old_fs);

    stFsCnf.ulRet = ret;

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
}


VOS_UINT32 diag_FsSpaceProc(VOS_UINT8 *pstReq)
{
    DIAG_CMD_FS_SPACE_REQ *pstFsReq = NULL;
    DIAG_CMD_FS_SPACE_CNF stFsCnf = { 0 };
    VOS_UINT32 ret;
    mm_segment_t old_fs;
    msp_diag_frame_info_s *pstDiagHead = NULL;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };

    pstDiagHead = (msp_diag_frame_info_s *)(pstReq);
    mdrv_diag_ptr(EN_DIAG_PTR_FS_SPACE_PROC, 1, pstDiagHead->ulCmdId, 0);

    if (pstDiagHead->ulMsgLen < sizeof(msp_diag_data_head_s) + sizeof(DIAG_CMD_FS_SPACE_REQ)) {
        diag_error("rev tool datalen error, 0x%x\n", pstDiagHead->ulMsgLen);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    pstFsReq = (DIAG_CMD_FS_SPACE_REQ *)(pstDiagHead->aucData + sizeof(msp_diag_data_head_s));

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    ret = (VOS_UINT32)DRV_FILE_GET_DISKSPACE((VOS_CHAR *)(pstFsReq->szDirectory), (VOS_UINT *)&stFsCnf.ulDiskSpace,
        (VOS_UINT *)&stFsCnf.ulUsedSpace, (VOS_UINT *)&stFsCnf.ulValidSpace);

    stFsCnf.ulRet = ret;
    set_fs(old_fs);

    DIAG_MSG_COMMON_PROC(stDiagInfo, stFsCnf, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_BSP;

    return DIAG_MsgReport(&stDiagInfo, &stFsCnf, sizeof(stFsCnf));
}
/*lint -e501 +esym(501,*)*/
VOS_VOID diag_fsInit(VOS_VOID)
{
    mdrv_hds_cmd_register(DIAG_CMD_FS_QUERY_DIR, (bsp_hds_func)diag_FsQueryProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_SCAN_DIR, (bsp_hds_func)diag_FsScanProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_MAKE_DIR, (bsp_hds_func)diag_FsMkdirProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_OPEN, (bsp_hds_func)diag_FsOpenProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_IMPORT, (bsp_hds_func)diag_FsImportProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_EXPORT, (bsp_hds_func)diag_FsExportProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_DELETE, (bsp_hds_func)diag_FsDeleteProc);
    mdrv_hds_cmd_register(DIAG_CMD_FS_SPACE, (bsp_hds_func)diag_FsSpaceProc);
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
