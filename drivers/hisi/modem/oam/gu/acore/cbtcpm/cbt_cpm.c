/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2001-2011
 * ��������: CBT������˿ں��߼�ͨ��ӳ�����ģ��
 * �޸���ʷ:
 * 1.��    ��: 2014��5��31��
 *   �޸�����: V8R1 OM_Optimize��Ŀ����
 */

#include "cbt_cpm.h"
#include "cbt_scm_soft_decode.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_CBT_CPM_C

#if defined(PAM_CT_ENABLED) || defined(PAM_BT_ENABLED) || defined(CBT_ENABLED)

CBTCPM_RCV_FUNC  g_cbtRcvFunc = VOS_NULL_PTR;
CBTCPM_SEND_FUNC g_cbtSndFunc = VOS_NULL_PTR;

/*
 * ��������: ��CBTͨ��ע����պ���
 * �޸���ʷ:
 * 2014.5.31 V8R1 OM_Optimize��Ŀ����
 */
VOS_VOID CBTCPM_PortRcvReg(CBTCPM_RCV_FUNC rcvFunc)
{
    g_cbtRcvFunc = rcvFunc;

    return;
}

/*
 * ��������: ��CBTͨ��ע�ᷢ�ͺ���
 * �޸���ʷ:
 * 2014.5.31 V8R1 OM_Optimize��Ŀ����
 */
VOS_VOID CBTCPM_PortSndReg(CBTCPM_SEND_FUNC sndFunc)
{
    g_cbtSndFunc = sndFunc;

    return;
}

/*
 * ��������: ��ȡCBTͨ�����պ�����ַ
 * �޸���ʷ:
 * 2014.5.31 V8R1 OM_Optimize��Ŀ����
 */
CBTCPM_RCV_FUNC CBTCPM_GetRcvFunc(VOS_VOID)
{
    return g_cbtRcvFunc;
}

/*
 * ��������: ��ȡCBTͨ�����ͺ�����ַ
 * �޸���ʷ:
 * 2014.5.31 V8R1 OM_Optimize��Ŀ����
 */
CBTCPM_SEND_FUNC CBTCPM_GetSndFunc(VOS_VOID)
{
    return g_cbtSndFunc;
}

/*
 * ��������: AT���CBT�����ݽ��պͷ��ͺ���
 * �޸���ʷ:
 * 2014.5.31 V8R1 OM_Optimize��Ŀ����
 */
VOS_UINT32 CBTCPM_NotifyChangePort(AT_PHY_PortUint32 phyPort)
{
    /* NAS��UART����У׼ */
    if (phyPort < CPM_IND_PORT)  {
#if (VOS_WIN32 == VOS_OS_VER)
        /* ��AT��÷������ݵĺ���ָ�� */
        CBTCPM_PortSndReg(AT_QuerySndFunc(phyPort));

        /* ��AT�Ķ˿���У׼������USB��VCOM�������� */
        CBTCPM_PortRcvReg(VOS_NULL_PTR);

        /* ��У׼ͨ���Ľ��պ�����ATģ�� */
        AT_RcvFuncReg(phyPort, CBTSCM_SoftDecodeDataRcv);
#endif
        return VOS_OK;
    }

    return VOS_ERR;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
