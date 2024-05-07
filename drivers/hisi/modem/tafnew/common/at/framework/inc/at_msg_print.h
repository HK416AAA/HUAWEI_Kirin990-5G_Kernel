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

#ifndef _AT_MSG_PRINT_H_
#define _AT_MSG_PRINT_H_

#include "vos.h"
#include "at_ctx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)



enum AT_MSG_PARSE_STATUS {
    AT_MSG_PARSE_STATUS_START,
    AT_MSG_PARSE_STATUS_LOWERDATA,
    AT_MSG_PARSE_STATUS_WAITUPPER,
    AT_MSG_PARSE_STATUS_UPPERDATA,
    AT_MSG_PARSE_STATUS_END,
    AT_MSG_PARSE_STATUS_BUTT
};
typedef VOS_UINT32 AT_MSG_PARSE_STATUS_UINT32;

typedef VOS_UINT32 (*AT_MSG_STATUS_CHK_FUNC)(TAF_UINT8);

/*
 * 3���Ͷ���
 */


#define AT_MAX_TIMEZONE_VALUE 48

#define At_GetNumTypeFromAddrType(numType, addrType) ((numType) = (((addrType) >> 4) & 0x07))
#define At_GetNumPlanFromAddrType(numPlan, addrType) ((numPlan) = ((addrType) & 0x0f))

/*
 * Description: �Ƚϡ�ƥ�������
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
extern VOS_UINT32 At_CheckEnd(VOS_UINT8 Char);

/*
 * Description: �Ƚϡ�ƥ�����ӷ�
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */

VOS_UINT32 AT_AsciiNumberToBcd(const VOS_CHAR *pcAsciiNumber, VOS_UINT8 *bcdNumber, VOS_UINT8 *bcdLen);

VOS_UINT32 AT_BcdNumberToAscii(const VOS_UINT8 *bcdNumber, VOS_UINT8 bcdLen, VOS_CHAR *pcAsciiNumber);

VOS_VOID AT_JudgeIsPlusSignInDialString(const VOS_CHAR *pcAsciiNumber, VOS_UINT16 len, VOS_UINT8 *isExistPlusSign,
                                        VOS_UINT8 *plusSignLocation);
TAF_UINT32 At_CheckNumLen(TAF_UINT16 max, TAF_UINT16 len);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _AT_MSG_PRINT_H_ */