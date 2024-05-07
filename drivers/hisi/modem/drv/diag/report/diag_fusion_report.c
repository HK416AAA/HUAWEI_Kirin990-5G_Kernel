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
#include <securec.h>
#include "mdrv_diag.h"
#include "mdrv_timer.h"
#include "msg_id.h"
#include "bsp_diag_cmdid.h"
#include "bsp_print.h"
#include "diag_fusion_debug.h"
#include "diag_fusion_report.h"
#include "diag_fusion_cfg.h"
#include "diag_service.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

diag_report_info_s g_diag_report_info;

void diag_report_init(void)
{
    u32 i;

    for (i = 0; i < DIAG_LOG_BUTT; i++) {
        g_diag_report_info.diag_log_num[i].num = 0;
        spin_lock_init(&g_diag_report_info.diag_log_num[i].lock);
    }
    bsp_log_string_pull_hook((bsp_log_string_pull_func)diag_report_drv_log);
    return;
}

void diag_report_reset(void)
{
    unsigned long lock;
    u32 i;

    for (i = 0; i < DIAG_LOG_BUTT; i++) {
        spin_lock_irqsave(&g_diag_report_info.diag_log_num[i].lock, lock);
        g_diag_report_info.diag_log_num[i].num = 0;
        spin_unlock_irqrestore(&g_diag_report_info.diag_log_num[i].lock, lock);
    }

    g_diag_report_info.ind_ps_trans_id = 0;
}

s32 diag_send_data(diag_msg_report_head_s *data, diag_log_type_e log_type)
{
    diag_srv_log_head_s *srv_head = (diag_srv_log_head_s *)data->header;
    unsigned long lock;
    u32 *num = NULL;
    u32 ret;

    spin_lock_irqsave(&g_diag_report_info.diag_log_num[log_type].lock, lock);
    switch (log_type) {
        case DIAG_LOG_PRINT:
            num = &(((diag_srv_print_head_s *)data->header)->print_head.msg_num);
            break;
        case DIAG_LOG_AIR:
            num = &(((diag_srv_air_head_s *)data->header)->air_header.msg_num);
            break;
        case DIAG_LOG_VoLTE:
            num = &(((diag_srv_volte_head_s *)data->header)->volte_header.msg_num);
            break;
        case DIAG_LOG_TRACE:
            num = &(((diag_srv_layer_head_s *)data->header)->trace_header.msg_num);
            break;
        case DIAG_LOG_TRANS:
            num = &(((diag_srv_trans_head_s *)data->header)->trans_header.msg_num);
            break;
        case DIAG_LOG_USER:
            num = &(((diag_srv_user_plane_head_s *)data->header)->user_header.msg_num);
            break;
        case DIAG_LOG_EVENT:
            num = &(((diag_srv_event_head_s *)data->header)->event_header.msg_num);
            break;
        case DIAG_LOG_DT:
            num = &(((diag_srv_dt_head_s *)data->header)->dt_header.msg_num);
            break;
        default:
            break;
    }
    if (num != NULL) {
        *num = g_diag_report_info.diag_log_num[log_type].num++;
    }

    srv_head->frame_header.srv_head.timestamp[0] = mdrv_timer_get_normal_timestamp();

    ret = diag_srv_pack_ind_data(data);
    spin_unlock_irqrestore(&g_diag_report_info.diag_log_num[log_type].lock, lock);

    return ret;
}

/*
 * 函 数 名: mdrv_diag_log_report
 * 功能描述: 打印上报接口
 * 输入参数: ulModuleId( 31-24:modemid,23-16:modeid,15-12:level )
 * pcFileName(上报时会把文件路径删除，只保留文件名)
 * ulLineNum(行号)
 * pszFmt(可变参数)
 * 注意事项: 由于此接口会被协议栈频繁调用，为提高处理效率，本接口内部会使用1K的局部变量保存上报的字符串信息，
 * 从而此接口对协议栈有两点限制，一是调用此接口的任务栈中的内存要至少为此接口预留1K空间；
 * 二是调用此接口输出的log不要超过1K（超出部分会自动丢弃）
 */
u32 mdrv_diag_log_report(u32 module_id, u32 mid, char *cFileName, u32 ulLineNum, char *pszFmt, ...)
{
    diag_print_ind_head_s *report_info = NULL;
    diag_srv_log_head_s log_head;
    diag_msg_report_head_s diag_head;
    char *offset_name = NULL;
    va_list arg;
    s32 text_len = 0;
    u32 msg_len;
    s32 ret;
    char text[DIAG_PS_PRINT_MAX_LEN + 1]; /* 所有打印文本内容，可能包括文件和行号,以'\0'结尾 */

    ret = diag_get_log_switch(module_id, mid);
    if (ret) {
        diag_report_exc_record(DIAG_EXC_LOG_REPORT, mid, 0, ret);
        return ret;
    }

    if (cFileName == NULL) {
        offset_name = " ";
    } else {
        /* 文件名截短 */
        offset_name = diag_get_file_name_from_path(cFileName);
    }

    va_start(arg, pszFmt);

    /* 给HSO的打印字符串形式如下:pszFileName[line_num]data。HSO根据中括号[]去截取相应的信息 */
    ret = snprintf_s(text, DIAG_PS_PRINT_MAX_LEN, (DIAG_PS_PRINT_MAX_LEN - 1), "%s[%d]", offset_name, ulLineNum);
    if (ret < 0) {
        diag_error("snprintf_s fail,ulParamLen:0x%x\n", text_len);
        va_end(arg);
        return ERR_MSP_FAILURE;
    } else if (ret > DIAG_PS_PRINT_MAX_LEN) {
        /* 内存越界，主动复位 */
        system_error(DRV_ERRNO_DIAG_OVERFLOW, __LINE__, (u32)text_len, 0, 0);
        va_end(arg);
        return ERR_MSP_FAILURE;
    }
    text_len = ret;

    ret = vsnprintf_s((text + text_len), (u32)(DIAG_PS_PRINT_MAX_LEN - text_len),
        (u32)(DIAG_PS_PRINT_MAX_LEN - text_len - 1), (const char *)pszFmt, arg);
    if (ret < 0) {
        diag_error("vsnprintf fail, ret:0x%x\n", ret);
        va_end(arg);
        return ERR_MSP_FAILURE;
    }
    text_len += ret;

    text[DIAG_PS_PRINT_MAX_LEN - 1] = '\0';
    msg_len = strnlen(text, DIAG_PS_PRINT_MAX_LEN) + 1;

    report_info = &log_head.print_head;

    /* 组装DIAG命令参数 */
    report_info->module = mid;
    /* 1:error, 2:warning, 3:normal, 4:info */
    /* (0|ERROR|WARNING|NORMAL|INFO|0|0|0) */
    report_info->level = (0x80000000 >> DIAG_GET_PRINT_LEVEL(module_id));

    /* 字符串的长度加上信息的长度 */
    msg_len += sizeof(diag_print_ind_head_s);

    va_end(arg);

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&log_head);
    DIAG_SRV_SET_MODEM_ID(&log_head.frame_header, DIAG_GET_MODEM_ID(module_id));
    DIAG_SRV_SET_TRANS_ID(&log_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&log_head.frame_header, DIAG_FRAME_MSG_TYPE_PS, DIAG_GET_MODE_ID(module_id),
        DIAG_FRAME_MSG_PRINT, 0);
    DIAG_SRV_SET_MSG_LEN(&log_head.frame_header, msg_len);

    diag_head.header_size = sizeof(log_head);
    diag_head.header = &log_head;

    diag_head.data_size = (u32)text_len;
    diag_head.data = (void *)&text;

    ret = diag_send_data(&diag_head, DIAG_LOG_PRINT);
    diag_mntn_log_record(ret);
    return ret;
}

s32 diag_report_drv_log(u32 module_id, u32 mid, const char *fmt, va_list arg)
{
    diag_print_ind_head_s *report_info = NULL;
    diag_srv_log_head_s log_head;
    diag_msg_report_head_s diag_head;
    s32 text_len;
    u32 msg_len;
    s32 ret;
    char text[DIAG_DRV_PRINT_MAX_LEN + 1] = DIAG_HDS_NAME; /* 所有打印文本内容，可能包括文件和行号,以'\0'结尾 */

    ret = diag_get_drv_print_switch(module_id);
    if (ret) {
        diag_report_exc_record(DIAG_EXC_LOG_REPORT, mid, 1, ret);
        return ret;
    }

    /* 由于底软打印中会增加[], HIDS识别时会认为底软中的[] 为行号，因此此处默认填写[] */
    ret = strcat_s(text, DIAG_DRV_PRINT_MAX_LEN, fmt);
    if (ret < 0) {
        diag_error("vsnprintf_s return error ret:0x%x\n", ret);
        return ERR_MSP_INALID_LEN_ERROR;
    }
    text_len = strnlen(text, DIAG_DRV_PRINT_MAX_LEN);

    report_info = &log_head.print_head;

    /* 组装DIAG命令参数 */
    report_info->module = mid;
    /* 1:error, 2:warning, 3:normal, 4:info */
    /* (0|ERROR|WARNING|NORMAL|INFO|0|0|0) */
    report_info->level = (0x80000000 >> DIAG_GET_PRINT_LEVEL(module_id));

    /* 字符串的长度加上信息的长度 */
    msg_len = (u32)text_len + sizeof(diag_print_ind_head_s);

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&log_head);
    DIAG_SRV_SET_MODEM_ID(&log_head.frame_header, DIAG_GET_MODEM_ID(module_id));
    DIAG_SRV_SET_TRANS_ID(&log_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&log_head.frame_header, DIAG_FRAME_MSG_TYPE_BSP, DIAG_GET_MODE_ID(module_id),
        DIAG_FRAME_MSG_PRINT, 0);
    DIAG_SRV_SET_MSG_LEN(&log_head.frame_header, msg_len);

    diag_head.header_size = sizeof(log_head);
    diag_head.header = &log_head;

    diag_head.data_size = (u32)text_len;
    diag_head.data = (void *)text;

    return diag_send_data(&diag_head, DIAG_LOG_PRINT);
}

char *diag_get_file_name_from_path(char *file_name)
{
    char *path_pos1 = NULL;
    char *path_pos2 = NULL;
    char *path_pos = NULL;

    /* 操作系统可能使用'\'来查找路径 */
    path_pos1 = (char *)strrchr(file_name, '\\');
    if (path_pos1 == NULL) {
        path_pos1 = file_name;
    }

    /* 操作系统可能使用'/'来查找路径 */
    path_pos2 = (char *)strrchr(file_name, '/');
    if (path_pos2 == NULL) {
        path_pos2 = file_name;
    }

    path_pos = (path_pos1 > path_pos2) ? path_pos1 : path_pos2;

    /* 如果没找到'\'或'/'则使用原来的字符串，否则使用截断后的字符串 */
    if (file_name != path_pos) {
        path_pos++;
    }

    return path_pos;
}

/*
 * 函 数 名: mdrv_diag_trans_report
 * 功能描述: 结构化数据上报接口
 * 输入参数: mdrv_diag_trans_ind_s->ulModule( 31-24:modemid,23-16:modeid )
 * mdrv_diag_trans_ind_s->ulMsgId(透传命令ID)
 * mdrv_diag_trans_ind_s->ulLength(透传信息的长度)
 * mdrv_diag_trans_ind_s->pData(透传信息)
 */
u32 mdrv_diag_trans_report(mdrv_diag_trans_ind_s *trans_msg)
{
    s32 ret = ERR_MSP_INVALID_PARAMETER;

    /* 检查参数合法性 */
    if (unlikely(trans_msg == NULL)) {
        diag_error("trans_msg invalid\n");
        diag_report_exc_record(DIAG_EXC_TRANS_REPORT, (uintptr_t)trans_msg, 2, ret);
        return ret;
    }

    ret = diag_get_trans_switch();
    if (ret) {
        diag_report_exc_record(DIAG_EXC_TRANS_REPORT, trans_msg->ulPid, 0, ret);
        return ret;
    }

    trans_msg->ulModule = MDRV_DIAG_GEN_MODULE_EX(DIAG_GET_MODEM_ID(trans_msg->ulModule),
        DIAG_GET_MODE_ID(trans_msg->ulModule), DIAG_MSG_TYPE_PS);

    ret = diag_report_trans(trans_msg);
    if (ret) {
        diag_report_exc_record(DIAG_EXC_TRANS_REPORT, trans_msg->ulPid, 1, ret);
    }

    diag_mntn_trans_record(ret);
    return ret;
}

s32 bsp_diag_trans_report(void *trans_msg)
{
    s32 ret = ERR_MSP_INVALID_PARAMETER;
    mdrv_diag_trans_ind_s *trans_data = (mdrv_diag_trans_ind_s *)trans_msg;

    if (trans_data != NULL) {
        ret = diag_get_trans_switch();
        if (ret) {
            diag_report_exc_record(DIAG_EXC_TRANS_REPORT, trans_data->ulPid, 0, ret);
            return (u32)ret;
        }

        ret = diag_report_trans(trans_data);
        diag_mntn_trans_record(ret);
        return ret;
    } else {
        diag_error("parameter invalid\n");
        diag_report_exc_record(DIAG_EXC_TRANS_REPORT, (u32)(uintptr_t)trans_data, 2, ret);
    }

    return ret;
}

s32 diag_report_trans(mdrv_diag_trans_ind_s *trans_msg)
{
    diag_srv_trans_head_s trans_head;
    diag_msg_report_head_s diag_head;

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&trans_head);
    DIAG_SRV_SET_MODEM_ID(&trans_head.frame_header, DIAG_GET_MODEM_ID(trans_msg->ulModule));
    DIAG_SRV_SET_TRANS_ID(&trans_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&trans_head.frame_header, DIAG_GET_GROUP_ID(trans_msg->ulModule),
        DIAG_GET_MODE_ID(trans_msg->ulModule), DIAG_FRAME_MSG_STRUCT, ((trans_msg->ulMsgId) & 0x7ffff));
    DIAG_SRV_SET_MSG_LEN(&trans_head.frame_header, sizeof(trans_head.trans_header) + trans_msg->ulLength);

    trans_head.trans_header.module = trans_msg->ulPid;
    trans_head.trans_header.msg_id = trans_msg->ulMsgId;

    diag_head.header_size = sizeof(trans_head);
    diag_head.header = &trans_head;
    diag_head.data_size = DIAG_SRV_GET_REAL_LEN(trans_msg->ulLength);
    diag_head.data = trans_msg->pData;

    return diag_send_data(&diag_head, DIAG_LOG_TRANS);
}

/*
 * 函 数 名: mdrv_diag_trace_report
 * 功能描述: 层间消息上报接口，给PS使用(替换原来的DIAG_ReportLayerMessageLog)
 * 输入参数: pMsg(标准的VOS消息体，源模块、目的模块信息从消息体中获取)
 */
u32 mdrv_diag_trace_report(void *trace_data)
{
    s32 ret = ERR_MSP_INVALID_PARAMETER;
    diag_trace_msg_s *trace_msg = (diag_trace_msg_s *)trace_data;

    if (unlikely(trace_data == NULL)) {
        diag_report_exc_record(DIAG_EXC_TRACE_REPORT, (u32)(uintptr_t)trace_data, 2, ret);
        return ret;
    }

    ret = diag_get_trace_switch(trace_msg);
    if (ret) {
        diag_report_exc_record(DIAG_EXC_TRACE_REPORT, trace_msg->src_id, trace_msg->dst_id, ret);
        return ret;
    }

    ret = diag_report_trace(trace_msg, 0); /* acore 不支持通过pid获取modem id，默认填0 */
    diag_mntn_trace_record(ret);
    return ret;
}

/*
 * 函 数 名: diag_report_trace
 * 功能描述: 层间消息上报接口，给PS使用(替换原来的DIAG_ReportLayerMessageLog)
 * 输入参数: pMsg(标准的VOS消息体，源模块、目的模块信息从消息体中获取)
 */
s32 diag_report_trace(diag_trace_msg_s *trace_msg, u32 modem_id)
{
    diag_srv_layer_head_s trace_head;
    diag_msg_report_head_s diag_head;
    u32 msg_len;

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&trace_head);
    DIAG_SRV_SET_MODEM_ID(&trace_head.frame_header, modem_id);
    DIAG_SRV_SET_TRANS_ID(&trace_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&trace_head.frame_header, DIAG_FRAME_MSG_TYPE_PS, DIAG_FRAME_MODE_COMM,
        DIAG_FRAME_MSG_LAYER, 0);
    msg_len = MDRV_MSG_HEAD_LEN + trace_msg->data_len;
    DIAG_SRV_SET_MSG_LEN(&trace_head.frame_header, sizeof(trace_head.trace_header) + msg_len);

    trace_head.trace_header.module = trace_msg->src_id;
    trace_head.trace_header.dest_module = trace_msg->dst_id;
    trace_head.trace_header.msg_id = trace_msg->msg_id;

    diag_head.header_size = sizeof(trace_head);
    diag_head.header = &trace_head;
    diag_head.data_size = DIAG_SRV_GET_REAL_LEN(msg_len);
    diag_head.data = trace_msg;

    return diag_send_data(&diag_head, DIAG_LOG_TRACE);
}

/*
 * 函 数 名: mdrv_diag_event_report
 * 功能描述: 事件上报接口，给PS使用(替换原来的DIAG_ReportEventLog)
 * 输入参数: mdrv_diag_event_ind_s->ulModule( 31-24:modemid,23-16:modeid,15-12:level,11-0:pid )
 * mdrv_diag_event_ind_s->ulEventId(event ID)
 * mdrv_diag_event_ind_s->ulLength(event的长度)
 * mdrv_diag_event_ind_s->pData(event信息)
 */
u32 mdrv_diag_event_report(mdrv_diag_event_ind_s *event_data)
{
    u32 ret = ERR_MSP_INVALID_PARAMETER;

    if (unlikely(event_data == NULL)) {
        diag_error("parameter invalid\n");
        diag_report_exc_record(DIAG_EXC_EVENT_REPORT, (u32)(uintptr_t)event_data, 2, (s32)ret);
        return ret;
    }

    ret = (u32)diag_get_event_switch(event_data);
    if (ret) {
        diag_report_exc_record(DIAG_EXC_EVENT_REPORT, event_data->ulPid, 0, (s32)ret);
        return ret;
    }

    ret = diag_report_event(event_data);
    diag_mntn_event_record(ret);
    return ret;
}

s32 diag_report_event(mdrv_diag_event_ind_s *event_msg)
{
    diag_srv_event_head_s event_head;
    diag_msg_report_head_s diag_head;

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&event_head);
    DIAG_SRV_SET_MODEM_ID(&event_head.frame_header, DIAG_GET_MODEM_ID(event_msg->ulModule));
    DIAG_SRV_SET_TRANS_ID(&event_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&event_head.frame_header, DIAG_FRAME_MSG_TYPE_PS, DIAG_GET_MODE_ID(event_msg->ulModule),
        DIAG_FRAME_MSG_EVENT, 0);
    DIAG_SRV_SET_MSG_LEN(&event_head.frame_header, sizeof(event_head.event_header) + event_msg->ulLength);

    event_head.event_header.event_id = event_msg->ulEventId;
    event_head.event_header.module = event_msg->ulPid;

    diag_head.header_size = sizeof(event_head);
    diag_head.header = &event_head;
    diag_head.data_size = DIAG_SRV_GET_REAL_LEN(event_msg->ulLength);
    diag_head.data = event_msg->pData;

    return diag_send_data(&diag_head, DIAG_LOG_EVENT);
}

/*
 * 函 数 名: mdrv_diag_air_report
 * 功能描述: 空口消息上报接口，给PS使用(替换原来的DIAG_ReportAirMessageLog)
 * 输入参数: mdrv_diag_air_ind_s->ulModule( 31-24:modemid,23-16:modeid,15-12:level,11-0:pid )
 * mdrv_diag_air_ind_s->ulMsgId(空口消息ID)
 * mdrv_diag_air_ind_s->ulDirection(空口消息的方向)
 * mdrv_diag_air_ind_s->ulLength(空口消息的长度)
 * mdrv_diag_air_ind_s->pData(空口消息信息)
 */
u32 mdrv_diag_air_report(mdrv_diag_air_ind_s *air_data)
{
    u32 ret = ERR_MSP_INVALID_PARAMETER;

    /* 检查参数合法性 */
    if (unlikely((air_data == NULL) || (air_data->pData == NULL) || (air_data->ulLength == 0))) {
        diag_error("parameter invalid\n");
        diag_report_exc_record(DIAG_EXC_AIR_REPORT, (u32)(uintptr_t)air_data, 2, (s32)ret);
        return ret;
    }

    ret = mdrv_diag_allow_air_msg_report();
    if (ret) {
        diag_report_exc_record(DIAG_EXC_AIR_REPORT, air_data->ulPid, 0, (s32)ret);
        return ret;
    }

    ret = diag_report_air(air_data);
    diag_mntn_air_record(ret);
    return ret;
}

s32 diag_report_air(mdrv_diag_air_ind_s *air_msg)
{
    diag_msg_report_head_s diag_head;
    diag_srv_air_head_s air_head;

    /* 填充数据头 */
    diag_srv_fill_head((diag_srv_head_s *)&air_head);
    DIAG_SRV_SET_MODEM_ID(&air_head.frame_header, DIAG_GET_MODEM_ID(air_msg->ulModule));
    DIAG_SRV_SET_TRANS_ID(&air_head.frame_header, g_diag_report_info.ind_ps_trans_id++);
    DIAG_SRV_SET_COMMAND_ID(&air_head.frame_header, DIAG_FRAME_MSG_TYPE_PS, DIAG_GET_MODE_ID(air_msg->ulModule),
        DIAG_FRAME_MSG_AIR, 0);
    DIAG_SRV_SET_MSG_LEN(&air_head.frame_header, sizeof(air_head.air_header) + air_msg->ulLength);

    air_head.air_header.module = air_msg->ulPid;
    air_head.air_header.msg_id = air_msg->ulMsgId;
    air_head.air_header.direction = air_msg->ulDirection;

    diag_head.header_size = sizeof(air_head);
    diag_head.header = &air_head;
    diag_head.data_size = DIAG_SRV_GET_REAL_LEN(air_msg->ulLength);
    diag_head.data = air_msg->pData;

    return diag_send_data(&diag_head, DIAG_LOG_AIR);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
