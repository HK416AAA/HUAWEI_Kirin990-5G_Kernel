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

#ifndef TAF_CDMA_PLUS_TRANSLATE_H
#define TAF_CDMA_PLUS_TRANSLATE_H

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define TAF_INTER_PREFIX_CODE_MAX_LEN 8

#define TAF_CODE_ENTITY_HRADER_MAX_LEN 8

#define TAF_DIAL_PREFIX_CODE_MAX_LEN 4
#define TAF_COUNTRY_CODE_MAX_LEN 8
#define TAF_PLUS_DIGIT_LEN 1
#define TAF_MT_MAX_BCD_NUM_LEN 20
#define TAF_MT_MAX_ASCII_NUM_LEN 40
#define TAF_MO_MAX_BCD_NUM_LEN 32
#define TAF_MO_MAX_ASCII_NUM_LEN 64
#define TAF_COUNTRY_LIST_LEN (sizeof(g_tafCountryList) / sizeof(TAF_COUNTRY_DIAL_CODE_INFO))

#define TAF_INVALID_SID (-1)

#define TAF_MCORE_INVALID_MCC 0xFFFFFFFF /* MCC的无效值 */

/* 此处主要是A核的打印功能申明，下移C核后不放开 */
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#include "at_mntn.h"
#define TAF_INFO_LOG(Mod, String) TAF_LOG((Mod), SUBMOD_NULL, PS_LOG_LEVEL_INFO, String)
#define TAF_INFO_LOG1(Mod, String, Para1) TAF_LOG1((Mod), SUBMOD_NULL, PS_LOG_LEVEL_INFO, String, (VOS_INT32)(Para1))
#define TAF_INFO_LOG2(Mod, String, Para1, Para2) \
    TAF_LOG2((Mod), SUBMOD_NULL, PS_LOG_LEVEL_INFO, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2))
#define TAF_INFO_LOG3(Mod, String, Para1, Para2, Para3)                                               \
    TAF_LOG3((Mod), SUBMOD_NULL, PS_LOG_LEVEL_INFO, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3))
#define TAF_INFO_LOG4(Mod, String, Para1, Para2, Para3, Para4)                                        \
    TAF_LOG4((Mod), SUBMOD_NULL, PS_LOG_LEVEL_INFO, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3), (VOS_INT32)(Para4))

#define TAF_NORMAL_LOG(Mod, String) TAF_LOG((Mod), SUBMOD_NULL, PS_LOG_LEVEL_NORMAL, String)
#define TAF_NORMAL_LOG1(Mod, String, Para1) \
    TAF_LOG1((Mod), SUBMOD_NULL, PS_LOG_LEVEL_NORMAL, String, (VOS_INT32)(Para1))
#define TAF_NORMAL_LOG2(Mod, String, Para1, Para2) \
    TAF_LOG2((Mod), SUBMOD_NULL, PS_LOG_LEVEL_NORMAL, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2))
#define TAF_NORMAL_LOG3(Mod, String, Para1, Para2, Para3)                                               \
    TAF_LOG3((Mod), SUBMOD_NULL, PS_LOG_LEVEL_NORMAL, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3))
#define TAF_NORMAL_LOG4(Mod, String, Para1, Para2, Para3, Para4)                                        \
    TAF_LOG4((Mod), SUBMOD_NULL, PS_LOG_LEVEL_NORMAL, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3), (VOS_INT32)(Para4))

#define TAF_WARNING_LOG(Mod, String) TAF_LOG((Mod), SUBMOD_NULL, PS_LOG_LEVEL_WARNING, String)
#define TAF_WARNING_LOG1(Mod, String, Para1) \
    TAF_LOG1((Mod), SUBMOD_NULL, PS_LOG_LEVEL_WARNING, String, (VOS_INT32)(Para1))
#define TAF_WARNING_LOG2(Mod, String, Para1, Para2) \
    TAF_LOG2((Mod), SUBMOD_NULL, PS_LOG_LEVEL_WARNING, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2))
#define TAF_WARNING_LOG3(Mod, String, Para1, Para2, Para3)                                               \
    TAF_LOG3((Mod), SUBMOD_NULL, PS_LOG_LEVEL_WARNING, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3))
#define TAF_WARNING_LOG4(Mod, String, Para1, Para2, Para3, Para4)                                        \
    TAF_LOG4((Mod), SUBMOD_NULL, PS_LOG_LEVEL_WARNING, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3), (VOS_INT32)(Para4))

#define TAF_ERROR_LOG(Mod, String) TAF_LOG((Mod), SUBMOD_NULL, PS_LOG_LEVEL_ERROR, String)
#define TAF_ERROR_LOG1(Mod, String, Para1) \
    TAF_LOG1((Mod), SUBMOD_NULL, PS_LOG_LEVEL_ERROR, String, (VOS_INT32)(Para1))
#define TAF_ERROR_LOG2(Mod, String, Para1, Para2) \
    TAF_LOG2((Mod), SUBMOD_NULL, PS_LOG_LEVEL_ERROR, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2))
#define TAF_ERROR_LOG3(Mod, String, Para1, Para2, Para3)                                               \
    TAF_LOG3((Mod), SUBMOD_NULL, PS_LOG_LEVEL_ERROR, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3))
#define TAF_ERROR_LOG4(Mod, String, Para1, Para2, Para3, Para4)                                        \
    TAF_LOG4((Mod), SUBMOD_NULL, PS_LOG_LEVEL_ERROR, String, (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
             (VOS_INT32)(Para3), (VOS_INT32)(Para4))

#endif


enum TAF_PlusTranslateDialType {
    TAF_PLUS_TRANSLATE_DIAL_TYPE_VOICE,
    TAF_PLUS_TRANSLATE_DIAL_TYPE_SMS,
    TAF_PLUS_TRANSLATE_DIAL_TYPE_BUTT
};
typedef VOS_UINT8 TAF_PlusTranslateDialTypeUint8;

enum TAF_PlusTranslateNumType {
    TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL = 0x91,
    TAF_PLUS_TRANSLATE_NUM_NATIONAL      = 0xa1,
};
typedef VOS_UINT8 TAF_PlusTranslateNumTypeUint8;


typedef struct {
    VOS_UINT16 sidRangeStart; /* SID 范围起始值 */
    VOS_UINT16 sidRangeEnd;   /* SID 范围结束值 */
    VOS_UINT16 cc;            /* country code */
    VOS_UINT16 mcc;           /* mobile country code */
    VOS_INT8   interPrefixCode[TAF_INTER_PREFIX_CODE_MAX_LEN];
    /* 通常呼叫号码的头部 exp: 13... ,15..., */
    VOS_INT8 numEntityHeader[TAF_CODE_ENTITY_HRADER_MAX_LEN];
    VOS_INT8 dialPrefixCode[TAF_DIAL_PREFIX_CODE_MAX_LEN];
    /* 当前网络是否支持"+"号传输 */
    VOS_UINT8 networkSupportPlusFlg;
    VOS_INT8  reserve[3];
} TAF_COUNTRY_DIAL_CODE_INFO;

const TAF_COUNTRY_DIAL_CODE_INFO* TAF_GetCountryListAddr(VOS_VOID);

VOS_INT32 TAF_ConvertHomeMccToSidRange(VOS_UINT16 homeMcc);

VOS_UINT16 TAF_GetCcOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8* TAF_GetIpcOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8* TAF_GetDpcOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8* TAF_GetNehOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8 TAF_GetNehLenOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8 TAF_GetIpcLenOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8 TAF_GetDpcLenOfCountryList(VOS_UINT16 indexNum);

VOS_UINT8 TAF_GetCcLenOfCountryList(VOS_UINT16 indexNum);

VOS_VOID TAF_GetInfoInCountryList(
    VOS_INT32 sid,
    VOS_UINT16 *mccFirstMatchEntryIndex, VOS_UINT16 *mccMatchEntryNum);

VOS_UINT32 TAF_ConvertASCIINumToUint(VOS_UINT8 ucstartDigit, VOS_UINT8 digitsLen, VOS_UINT8 *aSCIINum);

VOS_VOID TAF_InsertDigitsInNumHeader(const VOS_UINT8 *insertDigits, VOS_UINT8 insertDigitsLen, VOS_UINT8 *aSCIINum,
                                     VOS_UINT8 *numLen);

VOS_VOID TAF_SkipDigitsOfNumHeader(VOS_UINT8 *aSCIINum, VOS_UINT8 *numLen, VOS_UINT8 skipDigitsLen);

VOS_UINT8 TAF_IsNumEntityHeaderInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                             VOS_UINT8 *aSCIINum, VOS_UINT8 maxASCIINum);

VOS_UINT8 TAF_IsIpcAndCcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *aSCIINum, VOS_UINT8 maxASCIINum);

VOS_UINT8 TAF_IsCcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *aSCIINum, VOS_UINT8 maxASCIINum);

VOS_UINT8 TAF_TranslatePlusToNationalNum(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                         VOS_UINT8 *curASCIINum, VOS_UINT8 maxASCIINum, VOS_UINT8 *curNumLen,
                                         VOS_UINT8 isServingChinaMainland);

VOS_VOID TAF_TranslatePlusToInternationalNum(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                             VOS_UINT8 *curASCIINum, VOS_UINT8 maxASCIINum, VOS_UINT8 *curNumLen,
                                             TAF_PlusTranslateDialTypeUint8 dialType,
                                             VOS_UINT8 isServingChinaMainland);

VOS_VOID TAF_TranslatePlusToNum(
    VOS_INT32 sid,
    TAF_PlusTranslateDialTypeUint8 dialType, TAF_PlusTranslateNumTypeUint8 *numType, VOS_UINT8 *aSCIINum,
    VOS_UINT8 *numLen, VOS_UINT8 maxAsciiNum,
    VOS_UINT8 isServingChinaMainland);

VOS_UINT8 TAF_IsIpcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *aSCIINum);

VOS_UINT8 TAF_TranslateInternationalNumToPlus(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *aSCIINum,
                                              VOS_UINT8 *numLen, TAF_PlusTranslateDialTypeUint8 dialType);

VOS_UINT8 TAF_TranslateNationalNumToPlus(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *aSCIINum, VOS_UINT8 *numLen,
                                         TAF_PlusTranslateNumTypeUint8 *numType);

VOS_VOID TAF_TranslateNumToPlus(VOS_INT32 sid, TAF_PlusTranslateDialTypeUint8 dialType,
                                TAF_PlusTranslateNumTypeUint8 *numType, VOS_UINT8 *aSCIINum, VOS_UINT8 *numLen);

VOS_UINT32 TAF_QueryMccWithSid(VOS_INT32 sid);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* TAF_CDMA_PLUS_TRANSLATE_H */
