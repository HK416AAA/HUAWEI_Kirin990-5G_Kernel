/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#include "vos.h"
#include "ps_common_def.h"
#include "at_mn_interface.h"
#include "mn_call_api.h"
#include "product_config.h"
#include "mn_error_code.h"
#include "AtParse.h"
#include "at_cmd_proc.h"


#define THIS_FILE_ID PS_FILE_ID_MNCALL_API_C

VOS_UINT32 MN_CALL_CheckUus1ParmValid(MN_CALL_SetUus1TypeUint32 setType, MN_CALL_Uus1Info *uus1Info)
{
    if (uus1Info == VOS_NULL_PTR) {
        return MN_ERR_INVALIDPARM;
    }

    /*  У������ĺϷ���,�Ƿ�ֱ�ӷ��� */
    if ((setType >= MN_CALL_SET_UUS1_BUTT) || (uus1Info->msgType > MN_CALL_UUS1_MSG_RELEASE_COMPLETE)) {
        return MN_ERR_INVALIDPARM;
    }

    /*
     * ����UUIE�ļ�������һ���Ƿ���UUIE,�����ĳ��Ⱥ�PD�����м��,
     * ��Ӧ�ñ�֤,������ڼ���UUS1ʱ��Ҫ���,ȥ������ĸ���
     */
    if ((setType == MN_CALL_SET_UUS1_ACT) && (uus1Info->uuie[MN_CALL_IEI_POS] != MN_CALL_UUS_IEI)) {
        return MN_ERR_INVALIDPARM;
    }

    return MN_ERR_NO_ERROR;
}
