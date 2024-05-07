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

#include "taf_cdma_plus_translate.h"
#include "ps_common_def.h"
#include "taf_type_def.h"
/* 此处主要是A核的打印功能申明，下移C核后不放开 */
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#include "at_mntn.h"
#else
#include "taf_log.h"
#endif
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_TAF_CDMA_PLUS_TRANSLATE_C

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

/* 添加电信物联网号码10649 */
static const TAF_COUNTRY_DIAL_CODE_INFO g_tafCountryList[] = {
    { 13568, 14335, 86, 460, "00", "13", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "13", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "14", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "14", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "15", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "15", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "16", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "16", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "17", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "17", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "18", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "18", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "19", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "19", "0", VOS_FALSE, "" }, /* China 25600 */

    { 13568, 14335, 86, 460, "00", "10649", "0", VOS_FALSE, "" }, /* China 13568 */

    { 25600, 26111, 86, 460, "00", "10649", "0", VOS_FALSE, "" }, /* China 25600 */

    { 12288, 13311, 81, 440, "010", "", "0", VOS_FALSE, "" }, /* Japan 12288 */

    { 12288, 13311, 81, 441, "010", "", "0", VOS_FALSE, "" }, /* Japan */

    { 2176, 2303, 82, 450, "00700", "", "0", VOS_FALSE, "" }, /* Korea (South) */

    { 11296, 11311, 853, 455, "00", "", "", VOS_FALSE, "" }, /* Macao/Macau : 11296 */
};

const TAF_COUNTRY_DIAL_CODE_INFO* TAF_GetCountryListAddr(VOS_VOID)
{
    return (g_tafCountryList);
}

VOS_INT32 TAF_ConvertHomeMccToSidRange(VOS_UINT16 homeMcc)
{
    VOS_INT32                           sid = TAF_INVALID_SID;
    VOS_UINT32                          i;
    const TAF_COUNTRY_DIAL_CODE_INFO   *countryDailCodeInfo = VOS_NULL_PTR;

    countryDailCodeInfo = TAF_GetCountryListAddr();

    for (i = 0; i < TAF_COUNTRY_LIST_LEN; i++) {
        if (homeMcc == countryDailCodeInfo[i].mcc) {
            sid = (VOS_INT32)countryDailCodeInfo[i].sidRangeStart;

            return sid;
        }
    }

    return sid;
}

VOS_UINT16 TAF_GetCcOfCountryList(VOS_UINT16 indexNum)
{
    return (g_tafCountryList[indexNum].cc);
}

VOS_UINT8* TAF_GetIpcOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8 *)(g_tafCountryList[indexNum].interPrefixCode);
}

VOS_UINT8* TAF_GetDpcOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8 *)(g_tafCountryList[indexNum].dialPrefixCode);
}

VOS_UINT8* TAF_GetNehOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8 *)(g_tafCountryList[indexNum].numEntityHeader);
}

VOS_UINT8 TAF_GetNehLenOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8)VOS_StrNLen((VOS_CHAR *)g_tafCountryList[indexNum].numEntityHeader,
                                  TAF_CODE_ENTITY_HRADER_MAX_LEN);
}

VOS_UINT8 TAF_GetIpcLenOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8)VOS_StrNLen((VOS_CHAR *)g_tafCountryList[indexNum].interPrefixCode,
                                  TAF_INTER_PREFIX_CODE_MAX_LEN);
}

VOS_UINT8 TAF_GetDpcLenOfCountryList(VOS_UINT16 indexNum)
{
    return (VOS_UINT8)VOS_StrNLen((VOS_CHAR *)g_tafCountryList[indexNum].dialPrefixCode, TAF_DIAL_PREFIX_CODE_MAX_LEN);
}

VOS_UINT8 TAF_GetCcLenOfCountryList(VOS_UINT16 indexNum)
{
    VOS_INT32  returnValue;
    VOS_UINT8  ccLen;
    VOS_UINT16 countryListCc;
    VOS_INT8   acCountryListCc[TAF_COUNTRY_CODE_MAX_LEN];

    countryListCc = TAF_GetCcOfCountryList(indexNum);

    (VOS_VOID)memset_s(acCountryListCc, sizeof(acCountryListCc), 0x00, sizeof(acCountryListCc));
    returnValue = snprintf_s((VOS_CHAR *)acCountryListCc, sizeof(acCountryListCc), sizeof(acCountryListCc) - 1, "%d",
                             countryListCc);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(returnValue, sizeof(acCountryListCc), sizeof(acCountryListCc) - 1);
    ccLen = (VOS_UINT8)VOS_StrNLen((VOS_CHAR *)acCountryListCc, TAF_COUNTRY_CODE_MAX_LEN);

    return ccLen;
}

VOS_VOID TAF_GetInfoInCountryList(
    VOS_INT32 sid,
    VOS_UINT16 *mccFirstMatchEntryIndex, VOS_UINT16 *mccMatchEntryNum)
{
    VOS_UINT16                          i;
    VOS_UINT16                          firstMatchEntryIndex;
    VOS_UINT16                          matchEntryNum;
    const TAF_COUNTRY_DIAL_CODE_INFO   *countryDailCodeInfo = VOS_NULL_PTR;

    firstMatchEntryIndex = 0;
    matchEntryNum        = 0;
    countryDailCodeInfo  = TAF_GetCountryListAddr();

    for (i = 0; i < TAF_COUNTRY_LIST_LEN; i++) {
        if ((sid >= (VOS_INT32)countryDailCodeInfo[i].sidRangeStart) &&
            (sid <= (VOS_INT32)countryDailCodeInfo[i].sidRangeEnd)) {
            if (matchEntryNum == 0) {
                firstMatchEntryIndex = i;
            }

            matchEntryNum = i - firstMatchEntryIndex + 1;
        }
    }

    if (matchEntryNum != 0) {
        *mccFirstMatchEntryIndex = firstMatchEntryIndex;
        *mccMatchEntryNum        = matchEntryNum;
    }
}

VOS_UINT32 TAF_ConvertASCIINumToUint(VOS_UINT8 ucstartDigit, VOS_UINT8 digitsLen, VOS_UINT8 *asciiNum)
{
    VOS_UINT32  i;
    VOS_UINT32 value = 0;
    VOS_UINT8  digit = 0;

    for (i = ucstartDigit; i < digitsLen; i++) {
        digit = 0;
        if (asciiNum[i] >= '0') {
            digit = (VOS_UINT8)(asciiNum[i] - '0');
        } else {
            TAF_ERROR_LOG1(WUEPS_PID_TAF, "TAF_ConvertASCIINumToUint: pucAsciiNum[i] is less than '0'!", i);
        }

        value = value * 10 + digit;
    }

    return value;
}

VOS_VOID TAF_InsertDigitsInNumHeader(const VOS_UINT8 *insertDigits, VOS_UINT8 insertDigitsLen, VOS_UINT8 *asciiNum,
                                     VOS_UINT8 *numLen)
{
    errno_t   memResult;
    VOS_UINT8 numLenTemp;

    numLenTemp = *numLen;

    if ((insertDigitsLen == 0) || ((insertDigitsLen + numLenTemp) > (TAF_MO_MAX_ASCII_NUM_LEN + 1))) {
        return;
    }

    /* 号码后移 */
    if (numLenTemp > 0) {
        memResult = memmove_s(&(asciiNum[insertDigitsLen]),
                              ((TAF_MO_MAX_ASCII_NUM_LEN + 1 - insertDigitsLen) * sizeof(VOS_UINT8)), &(asciiNum[0]),
                              (numLenTemp * sizeof(VOS_UINT8)));
        TAF_MEM_CHK_RTN_VAL(memResult, ((TAF_MO_MAX_ASCII_NUM_LEN + 1 - insertDigitsLen) * sizeof(VOS_UINT8)),
                            (numLenTemp * sizeof(VOS_UINT8)));
    }

    /* 插入号码 */
    memResult = memcpy_s(asciiNum, (TAF_MO_MAX_ASCII_NUM_LEN + 1), insertDigits, insertDigitsLen);
    TAF_MEM_CHK_RTN_VAL(memResult, (TAF_MO_MAX_ASCII_NUM_LEN + 1), insertDigitsLen);

    /* 更新号码长度 */
    numLenTemp += insertDigitsLen;

    *numLen = numLenTemp;
}

VOS_VOID TAF_SkipDigitsOfNumHeader(VOS_UINT8 *asciiNum, VOS_UINT8 *numLen, VOS_UINT8 skipDigitsLen)
{
    errno_t   memResult;
    VOS_UINT8 numLenTemp;

    numLenTemp = *numLen;

    if (numLenTemp >= skipDigitsLen) {
        if ((numLenTemp > 0) && (numLenTemp - skipDigitsLen > 0)) {
            memResult = memmove_s(&(asciiNum[0]), numLenTemp * sizeof(VOS_UINT8), &(asciiNum[skipDigitsLen]),
                                  (numLenTemp - skipDigitsLen) * sizeof(VOS_UINT8));
            TAF_MEM_CHK_RTN_VAL(memResult, numLenTemp * sizeof(VOS_UINT8),
                                (numLenTemp - skipDigitsLen) * sizeof(VOS_UINT8));
        }

        /* 检查memset的保护长度不能为0 */
        if (skipDigitsLen != 0) {
            /* 剩余位清零 */
            (VOS_VOID)memset_s(&(asciiNum[numLenTemp - skipDigitsLen]), skipDigitsLen, 0x00, skipDigitsLen);
        }

        /* 更新号码长度 */
        numLenTemp -= skipDigitsLen;

        *numLen = numLenTemp;
    }
}

VOS_UINT8 TAF_IsNumEntityHeaderInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                             VOS_UINT8 *asciiNum, VOS_UINT8 maxASCIINum)
{
    VOS_UINT16 i;
    VOS_UINT32 calledNumEntityHeader;
    VOS_UINT32 countryListNumEntityHeader;
    VOS_UINT8  numEntityHeaderLen;
    VOS_UINT8 *numEntityHeader = VOS_NULL_PTR;

    /* 在country list 中，一个mcc只会对应一个 ipc 和 dpc ，但可能对应多个号码实体头  */

    /* 检测 号码实体头部是否在country list 中 */
    for (i = mccFirstMatchEntryIndex; i < (mccFirstMatchEntryIndex + mccMatchEntryNum); i++) {
        /* 获取 号码实体头部 所应占据的长度 */
        numEntityHeader = TAF_GetNehOfCountryList(i);

        numEntityHeaderLen = TAF_GetNehLenOfCountryList(i);

        if (numEntityHeaderLen == 0) {
            return VOS_FALSE;
        }

        /* 获取country list中，匹配MCC的记录中， 号码实体头部 的值 */
        countryListNumEntityHeader = TAF_ConvertASCIINumToUint(0, numEntityHeaderLen, (VOS_UINT8 *)numEntityHeader);

        /* 长度超过最大值 */
        if (numEntityHeaderLen > maxASCIINum) {
            return VOS_FALSE;
        }

        /* 依据号码实体头部的位数，求出呼叫号码中该值 */
        calledNumEntityHeader = TAF_ConvertASCIINumToUint(0, numEntityHeaderLen, asciiNum);

        if (countryListNumEntityHeader == calledNumEntityHeader) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_UINT8 TAF_IsIpcAndCcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *asciiNum, VOS_UINT8 maxASCIINum)
{
    VOS_UINT32 numIpc;
    VOS_UINT16 numCc;
    VOS_UINT32 countryListIpc;
    VOS_UINT8  ipcLen;
    VOS_UINT16 countryListCc;
    VOS_UINT8  ccLen;

    VOS_UINT8 *countryListIpcInfo = VOS_NULL_PTR;

    /*
     * 如果匹配，则呼叫号码的构成为: ipc + cc + 剩余号码 ,
     * country list中，一个mcc 只会对应一组 ipp + cc
     */

    /* 获取country list中， cc 的值 */
    countryListCc = TAF_GetCcOfCountryList(mccFirstMatchEntryIndex);

    /*
     * 依据country list中，ipc 和 cc 的值的长度，判断呼叫号码中，
     * 如果号码开头也由ipc + cc 组成，ipc 和 cc 应该占据的位数
     */
    ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

    ccLen = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

    countryListIpcInfo = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);

    /* 获取country list中，ipc 的值 */
    countryListIpc = TAF_ConvertASCIINumToUint(0, ipcLen, countryListIpcInfo);

    if (ccLen + ipcLen > maxASCIINum) {
        return VOS_FALSE;
    }

    /* 依据ipc、cc占据的位数，求出呼叫号码中该值 */
    numIpc = TAF_ConvertASCIINumToUint(0, ipcLen, asciiNum);
    numCc  = (VOS_UINT16)TAF_ConvertASCIINumToUint(ipcLen, (ccLen + ipcLen), asciiNum);

    /* ipc + cc 匹配， */
    if ((numIpc == countryListIpc) && (numCc == countryListCc)) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

VOS_UINT8 TAF_IsCcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *asciiNum, VOS_UINT8 maxASCIINum)
{
    VOS_UINT16 numCc;
    VOS_UINT16 countryListCc;
    VOS_UINT8  ccLen;

    countryListCc = TAF_GetCcOfCountryList(mccFirstMatchEntryIndex);

    ccLen = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

    /* 长度超过最大值 */
    if (ccLen > maxASCIINum) {
        return VOS_FALSE;
    }

    numCc = (VOS_UINT16)TAF_ConvertASCIINumToUint(0, ccLen, asciiNum);

    if (countryListCc == numCc) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

VOS_UINT8 TAF_TranslatePlusToNationalNum(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                         VOS_UINT8 *curASCIINum, VOS_UINT8 maxASCIINum, VOS_UINT8 *curNumLen,
                                         VOS_UINT8 isServingChinaMainland)
{
    VOS_UINT8  ipcLen;
    VOS_UINT8  ccLen;
    VOS_UINT8 *numDpc = VOS_NULL_PTR;
    VOS_UINT8  dpcLen;

    /* 检测开头的号码是否是IPC+CC,如果是，直接跳过IPC + CC，返回真，如果不是，退出，号码不做修改 */
    if (TAF_IsIpcAndCcInCountryList(mccFirstMatchEntryIndex, curASCIINum, maxASCIINum) == VOS_TRUE) {
        /* 驻留在国内网络，不需要再检查呼叫号码头,也不需要跳过ipc +cc */
        if (isServingChinaMainland == VOS_TRUE) {
            return VOS_TRUE;
        } else {
            /* 跳过 ipc + cc 位 */
            ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);
            ccLen  = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_SkipDigitsOfNumHeader(curASCIINum, curNumLen, (VOS_UINT8)(ipcLen + ccLen));
        }
    } else {
        return VOS_FALSE;
    }

    /* 检测号码实体的头部是否在国家列表中 */
    if (TAF_IsNumEntityHeaderInCountryList(mccFirstMatchEntryIndex, mccMatchEntryNum, curASCIINum, maxASCIINum) ==
        VOS_FALSE) {
        /* 号码实体头部不在country list 中，将dpc插入到号码头部 */
        numDpc = TAF_GetDpcOfCountryList(mccFirstMatchEntryIndex);
        dpcLen = TAF_GetDpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_InsertDigitsInNumHeader(numDpc, dpcLen, curASCIINum, curNumLen);
    }

    return VOS_TRUE;
}

VOS_VOID TAF_TranslatePlusToInternationalNum(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT16 mccMatchEntryNum,
                                             VOS_UINT8 *curASCIINum, VOS_UINT8 maxASCIINum, VOS_UINT8 *curNumLen,
                                             TAF_PlusTranslateDialTypeUint8 dialType,
                                             VOS_UINT8 isServingChinaMainland)
{
    VOS_UINT8  ccLen;
    VOS_UINT8 *numIpc = VOS_NULL_PTR;
    VOS_UINT8  ipcLen;
    VOS_UINT8 *numDpc = VOS_NULL_PTR;
    VOS_UINT8  dpcLen;

    if ((TAF_IsCcInCountryList(mccFirstMatchEntryIndex, curASCIINum, maxASCIINum) == VOS_TRUE) &&
        (dialType != TAF_PLUS_TRANSLATE_DIAL_TYPE_SMS)) {
        /*
         * 中国网络，不需要检查呼叫号码头,在呼叫号码前面加上ipc，比如原来的呼叫号码是8619912345678，转换后的呼叫号码就是
         * 008619912345678，这样就可以避免表里没有维护199号码，就会造成发给网侧的号码是019912345678，网侧不能识别
         */
        if (isServingChinaMainland == VOS_TRUE) {
            /* 相等, 带加号的国内呼叫 ，处于中国大陆，加上IPC，最终号码格式即 ipc + cc + 号码实体 */
            numIpc = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);
            ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_InsertDigitsInNumHeader(numIpc, ipcLen, curASCIINum, curNumLen);
            return;
        } else {
            /* 相等, 带加号的国内呼叫，跳过国家码 */
            ccLen = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_SkipDigitsOfNumHeader(curASCIINum, curNumLen, ccLen);
        }

        /* 继续按国内呼叫流程执行 */
        if (TAF_IsNumEntityHeaderInCountryList(mccFirstMatchEntryIndex, mccMatchEntryNum, curASCIINum, maxASCIINum) ==
            VOS_FALSE) {
            /* 号码实体头部不在country list 中，将dpc插入到号码头部 */
            numDpc = TAF_GetDpcOfCountryList(mccFirstMatchEntryIndex);
            dpcLen = TAF_GetDpcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_InsertDigitsInNumHeader(numDpc, dpcLen, curASCIINum, curNumLen);
        }
    } else {
        /* 不同，带加号的国际呼 ，加上IPC，最终号码格式即 ipc + cc + 号码实体 */
        numIpc = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);
        ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_InsertDigitsInNumHeader(numIpc, ipcLen, curASCIINum, curNumLen);
    }
}

VOS_VOID TAF_TranslatePlusToNum(
    VOS_INT32 sid,
    TAF_PlusTranslateDialTypeUint8 dialType, TAF_PlusTranslateNumTypeUint8 *numType, VOS_UINT8 *asciiNum,
    VOS_UINT8 *numLen, VOS_UINT8 maxAsciiNum,
    VOS_UINT8 isServingChinaMainland)
{
    /*
     * 1、主叫过程中，无论是语音业务还是短信业务，在前一阶段的处理过程中，如果带加号，都将加号去掉，
     * 通过将号码类型改为国际类型来标示之前号码带有加号；
     * 2、国际呼叫类型处理:
     * 2.1、如果呼叫号码中的Cc与国际列表中，Mcc对应的CC相同，即带加号的国内呼叫,去掉CC，再根据neh
     * 是否与国家列表中Mcc对应的相匹配，如果存在不匹配，需要加上npc
     * (exp +8613888888888 转换为 13888888888；+86 21 888888888 转换为 021 888888888)
     * 2.2、如果呼叫号码中的Cc与国际列表中，Mcc对应的CC相同，即国际呼叫，最终呼叫号码结构:"ipc" + "cc" + "neh" + "剩余号码"
     * (exp 国内用户呼叫日本用户 +81xxxxxxx 转换为 0081xxxxxxx)
     * 3、国内呼叫类型处理
     * 3.1、如果呼叫类型为"ipc" + "cc" + "neh" + "剩余号码"，转换为"ipc/npc(判断同2.1)"+"neh" + "剩余号码"
     * (exp 008613888888888 转换为 0013888888888；008614888888888 转换为 014888888888)
     * 3.2、其他号码不做改变
     * 4、如果呼叫是在中国(大陆)发起，呼叫号码前面不用再加dpc，呼叫号码按照转换结果直接使用
     */

    errno_t                       memResult;
    VOS_UINT16                    mccFirstMatchEntryIndex;
    VOS_UINT16                    mccMatchEntryNum;
    VOS_UINT8                     asciiNumInfo[TAF_MO_MAX_ASCII_NUM_LEN + 1];
    VOS_UINT8                     numLenTemp;
    TAF_PlusTranslateNumTypeUint8 numTypeValue;

    /* 检测mcc 是否在 countrylist,返回匹配的记录数量，以及第一条记录的索引 */
    mccFirstMatchEntryIndex = 0;
    mccMatchEntryNum        = 0;

    /* 获取匹配的Sid信息 */
    TAF_GetInfoInCountryList(sid, &mccFirstMatchEntryIndex, &mccMatchEntryNum);

    /* 没有匹配记录 */
    if (mccMatchEntryNum == 0) {
        return;
    }

    /* 获取呼叫号码长度 */
    /* 如果长度过大取截断的长度 */
    numLenTemp = TAF_MIN(*numLen, (TAF_MO_MAX_ASCII_NUM_LEN + 1));

    /* 获取号码类型 */
    numTypeValue = *numType;

    /* 获取呼叫号码 */
    (VOS_VOID)memset_s(asciiNumInfo, sizeof(asciiNumInfo), 0x00, sizeof(asciiNumInfo));
    if (numLenTemp > 0) {
        memResult = memcpy_s(asciiNumInfo, sizeof(asciiNumInfo), asciiNum, numLenTemp);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNumInfo), numLenTemp);
    }

    if (numTypeValue == TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL) {
        /* 国际呼叫处理 */
        TAF_TranslatePlusToInternationalNum(mccFirstMatchEntryIndex, mccMatchEntryNum, asciiNumInfo,
                                            TAF_MO_MAX_ASCII_NUM_LEN, &numLenTemp, dialType, isServingChinaMainland);
    } else {
        if (TAF_TranslatePlusToNationalNum(mccFirstMatchEntryIndex, mccMatchEntryNum, asciiNumInfo,
                                           TAF_MO_MAX_ASCII_NUM_LEN, &numLenTemp,
                                           isServingChinaMainland) == VOS_FALSE) {
            return;
        }
    }

    /* 如果完成加号转换，即加号已经被去掉，所有呼叫类型更新为国内呼叫类型 */
    numTypeValue = TAF_PLUS_TRANSLATE_NUM_NATIONAL;

    /* 将转换后的号码信息写回,使用新的长度 */

    /* 函数TAF_TranslatePlusToNationalNum中会修改ucNumLen值，可能超过32，需要与目标空间取小 */
    numLenTemp = TAF_MIN(numLenTemp, maxAsciiNum);

    (VOS_VOID)memset_s(asciiNum, maxAsciiNum, 0x00, maxAsciiNum);
    if (numLenTemp > 0) {
        memResult = memcpy_s(asciiNum, maxAsciiNum, asciiNumInfo, numLenTemp);
        TAF_MEM_CHK_RTN_VAL(memResult, maxAsciiNum, numLenTemp);
    }
    *numLen  = numLenTemp;
    *numType = numTypeValue;
}

VOS_UINT8 TAF_IsIpcInCountryList(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *asciiNum)
{
    VOS_UINT32 numIpc;
    VOS_UINT32 countryListIpc;
    VOS_UINT8  ipcLen;
    VOS_UINT8 *countryListIpcInfo = VOS_NULL_PTR;

    countryListIpcInfo = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);

    ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

    countryListIpc = TAF_ConvertASCIINumToUint(0, ipcLen, countryListIpcInfo);
    if (ipcLen > TAF_MT_MAX_ASCII_NUM_LEN) {
        return VOS_FALSE;
    }

    numIpc = TAF_ConvertASCIINumToUint(0, ipcLen, asciiNum);

    if (countryListIpc == numIpc) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

VOS_UINT8 TAF_TranslateInternationalNumToPlus(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *asciiNum,
                                              VOS_UINT8 *numLen, TAF_PlusTranslateDialTypeUint8 dialType)
{
    VOS_UINT8 ipcLen;
    VOS_UINT8 numLenTemp;
    errno_t   returnValue;

    /* 匹配号码开头的ipc是否在国家列表中 */
    if (TAF_IsIpcInCountryList(mccFirstMatchEntryIndex, asciiNum) == VOS_TRUE) {
        /* ipc 匹配，跳过ipc位 */
        ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_SkipDigitsOfNumHeader(asciiNum, numLen, ipcLen);

        /* 短信呼叫 */
        if (dialType == TAF_PLUS_TRANSLATE_DIAL_TYPE_SMS) {
            /* 插入加号在号码头部 */
            if ((*numLen) >= (TAF_MT_MAX_ASCII_NUM_LEN + 1)) {
                return VOS_FALSE;
            }

            numLenTemp = *numLen;

            /* 号码后移 */
            if (numLenTemp > 0) {
                returnValue = memmove_s(&(asciiNum[1]), TAF_MT_MAX_ASCII_NUM_LEN * sizeof(VOS_UINT8), &(asciiNum[0]),
                                        (numLenTemp * sizeof(VOS_UINT8)));
                TAF_MEM_CHK_RTN_VAL(returnValue, TAF_MT_MAX_ASCII_NUM_LEN * sizeof(VOS_UINT8),
                                    numLenTemp * sizeof(VOS_UINT8));
            }

            asciiNum[0] = '+';

            numLenTemp++;

            *numLen = numLenTemp;
        }

        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

VOS_UINT8 TAF_TranslateNationalNumToPlus(VOS_UINT16 mccFirstMatchEntryIndex, VOS_UINT8 *asciiNum, VOS_UINT8 *numLen,
                                         TAF_PlusTranslateNumTypeUint8 *numType)
{
    VOS_UINT8 ipcLen;

    if (TAF_IsIpcInCountryList(mccFirstMatchEntryIndex, asciiNum) == VOS_TRUE) {
        /* ipc 匹配，跳过ipc位 */
        ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_SkipDigitsOfNumHeader(asciiNum, numLen, ipcLen);

        /* 更新号码类型为国际呼叫 */
        *numType = TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL;

        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

VOS_VOID TAF_TranslateNumToPlus(VOS_INT32 sid, TAF_PlusTranslateDialTypeUint8 dialType,
                                TAF_PlusTranslateNumTypeUint8 *numType, VOS_UINT8 *asciiNum, VOS_UINT8 *numLen)
{
    /*
     * 1、国际呼叫类型处理:
     * 1.1、"ipc" + "cc" +号码,语音业务，转换为"cc" +号码
     * 1.2、"ipc" + "cc" +号码,短信业务，转换为"+" + "cc" + 号码
     * 2、国内呼叫类型处理:
     * 任何业务，"ipc" + "cc" +号码,语音业务，转换为"cc" +号码
     * 3、其他号码不做修改
     */

    errno_t                       memResult;
    VOS_UINT16                    mccFirstMatchEntryIndex = 0;
    VOS_UINT16                    mccMatchEntryNum = 0;
    VOS_UINT8                     asciiNumInfo[TAF_MT_MAX_ASCII_NUM_LEN + 1];
    VOS_UINT8                     numLenTemp;
    TAF_PlusTranslateNumTypeUint8 numTypeValue;

    /* 检测mcc 是否在 countrylist,返回匹配的记录数量，以及第一条记录的索引 */

    /* 获取匹配的MCC信息 */
    TAF_GetInfoInCountryList(sid, &mccFirstMatchEntryIndex, &mccMatchEntryNum);

    /* 没有匹配记录 */
    if (mccMatchEntryNum == 0) {
        return;
    }

    /* 获取呼叫号码长度 */
    /* 对长度值取保护 */
    numLenTemp = TAF_MIN(*numLen, (TAF_MT_MAX_ASCII_NUM_LEN + 1));

    /* 获取号码类型 */
    numTypeValue = *numType;

    /* 获取呼叫号码 */
    (VOS_VOID)memset_s(asciiNumInfo, sizeof(asciiNumInfo), 0x00, sizeof(asciiNumInfo));
    memResult = memcpy_s(asciiNumInfo, sizeof(asciiNumInfo), asciiNum, sizeof(asciiNumInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNumInfo), sizeof(asciiNumInfo));

    if (numTypeValue == TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL) {
        /* 国际业务 */
        if (TAF_TranslateInternationalNumToPlus(mccFirstMatchEntryIndex, asciiNumInfo, &numLenTemp, dialType) ==
            VOS_FALSE) {
            return;
        }
    } else {
        /* 国内业务 */
        if (TAF_TranslateNationalNumToPlus(mccFirstMatchEntryIndex, asciiNumInfo, &numLenTemp, &numTypeValue) ==
            VOS_FALSE) {
            return;
        }
    }

    /* 将转换后的号码信息写回 */
    memResult = memcpy_s(asciiNum, sizeof(asciiNumInfo), asciiNumInfo, sizeof(asciiNumInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNumInfo), sizeof(asciiNumInfo));
    *numLen  = numLenTemp;
    *numType = numTypeValue;
}

VOS_UINT32 TAF_QueryMccWithSid(VOS_INT32 sid)
{
    VOS_UINT32 i;

    for (i = 0; i < TAF_COUNTRY_LIST_LEN; i++) {
        if ((sid >= g_tafCountryList[i].sidRangeStart) && (sid <= g_tafCountryList[i].sidRangeEnd)) {
            return (VOS_UINT32)g_tafCountryList[i].mcc;
        }
    }

    return TAF_MCORE_INVALID_MCC;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
