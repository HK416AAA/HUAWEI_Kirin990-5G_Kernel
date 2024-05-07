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
/* �˴���Ҫ��A�˵Ĵ�ӡ��������������C�˺󲻷ſ� */
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

/* ��ӵ�������������10649 */
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

    /* ������� */
    if (numLenTemp > 0) {
        memResult = memmove_s(&(asciiNum[insertDigitsLen]),
                              ((TAF_MO_MAX_ASCII_NUM_LEN + 1 - insertDigitsLen) * sizeof(VOS_UINT8)), &(asciiNum[0]),
                              (numLenTemp * sizeof(VOS_UINT8)));
        TAF_MEM_CHK_RTN_VAL(memResult, ((TAF_MO_MAX_ASCII_NUM_LEN + 1 - insertDigitsLen) * sizeof(VOS_UINT8)),
                            (numLenTemp * sizeof(VOS_UINT8)));
    }

    /* ������� */
    memResult = memcpy_s(asciiNum, (TAF_MO_MAX_ASCII_NUM_LEN + 1), insertDigits, insertDigitsLen);
    TAF_MEM_CHK_RTN_VAL(memResult, (TAF_MO_MAX_ASCII_NUM_LEN + 1), insertDigitsLen);

    /* ���º��볤�� */
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

        /* ���memset�ı������Ȳ���Ϊ0 */
        if (skipDigitsLen != 0) {
            /* ʣ��λ���� */
            (VOS_VOID)memset_s(&(asciiNum[numLenTemp - skipDigitsLen]), skipDigitsLen, 0x00, skipDigitsLen);
        }

        /* ���º��볤�� */
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

    /* ��country list �У�һ��mccֻ���Ӧһ�� ipc �� dpc �������ܶ�Ӧ�������ʵ��ͷ  */

    /* ��� ����ʵ��ͷ���Ƿ���country list �� */
    for (i = mccFirstMatchEntryIndex; i < (mccFirstMatchEntryIndex + mccMatchEntryNum); i++) {
        /* ��ȡ ����ʵ��ͷ�� ��Ӧռ�ݵĳ��� */
        numEntityHeader = TAF_GetNehOfCountryList(i);

        numEntityHeaderLen = TAF_GetNehLenOfCountryList(i);

        if (numEntityHeaderLen == 0) {
            return VOS_FALSE;
        }

        /* ��ȡcountry list�У�ƥ��MCC�ļ�¼�У� ����ʵ��ͷ�� ��ֵ */
        countryListNumEntityHeader = TAF_ConvertASCIINumToUint(0, numEntityHeaderLen, (VOS_UINT8 *)numEntityHeader);

        /* ���ȳ������ֵ */
        if (numEntityHeaderLen > maxASCIINum) {
            return VOS_FALSE;
        }

        /* ���ݺ���ʵ��ͷ����λ����������к����и�ֵ */
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
     * ���ƥ�䣬����к���Ĺ���Ϊ: ipc + cc + ʣ����� ,
     * country list�У�һ��mcc ֻ���Ӧһ�� ipp + cc
     */

    /* ��ȡcountry list�У� cc ��ֵ */
    countryListCc = TAF_GetCcOfCountryList(mccFirstMatchEntryIndex);

    /*
     * ����country list�У�ipc �� cc ��ֵ�ĳ��ȣ��жϺ��к����У�
     * ������뿪ͷҲ��ipc + cc ��ɣ�ipc �� cc Ӧ��ռ�ݵ�λ��
     */
    ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

    ccLen = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

    countryListIpcInfo = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);

    /* ��ȡcountry list�У�ipc ��ֵ */
    countryListIpc = TAF_ConvertASCIINumToUint(0, ipcLen, countryListIpcInfo);

    if (ccLen + ipcLen > maxASCIINum) {
        return VOS_FALSE;
    }

    /* ����ipc��ccռ�ݵ�λ����������к����и�ֵ */
    numIpc = TAF_ConvertASCIINumToUint(0, ipcLen, asciiNum);
    numCc  = (VOS_UINT16)TAF_ConvertASCIINumToUint(ipcLen, (ccLen + ipcLen), asciiNum);

    /* ipc + cc ƥ�䣬 */
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

    /* ���ȳ������ֵ */
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

    /* ��⿪ͷ�ĺ����Ƿ���IPC+CC,����ǣ�ֱ������IPC + CC�������棬������ǣ��˳������벻���޸� */
    if (TAF_IsIpcAndCcInCountryList(mccFirstMatchEntryIndex, curASCIINum, maxASCIINum) == VOS_TRUE) {
        /* פ���ڹ������磬����Ҫ�ټ����к���ͷ,Ҳ����Ҫ����ipc +cc */
        if (isServingChinaMainland == VOS_TRUE) {
            return VOS_TRUE;
        } else {
            /* ���� ipc + cc λ */
            ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);
            ccLen  = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_SkipDigitsOfNumHeader(curASCIINum, curNumLen, (VOS_UINT8)(ipcLen + ccLen));
        }
    } else {
        return VOS_FALSE;
    }

    /* ������ʵ���ͷ���Ƿ��ڹ����б��� */
    if (TAF_IsNumEntityHeaderInCountryList(mccFirstMatchEntryIndex, mccMatchEntryNum, curASCIINum, maxASCIINum) ==
        VOS_FALSE) {
        /* ����ʵ��ͷ������country list �У���dpc���뵽����ͷ�� */
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
         * �й����磬����Ҫ�����к���ͷ,�ں��к���ǰ�����ipc������ԭ���ĺ��к�����8619912345678��ת����ĺ��к������
         * 008619912345678�������Ϳ��Ա������û��ά��199���룬�ͻ���ɷ�������ĺ�����019912345678�����಻��ʶ��
         */
        if (isServingChinaMainland == VOS_TRUE) {
            /* ���, ���ӺŵĹ��ں��� �������й���½������IPC�����պ����ʽ�� ipc + cc + ����ʵ�� */
            numIpc = TAF_GetIpcOfCountryList(mccFirstMatchEntryIndex);
            ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_InsertDigitsInNumHeader(numIpc, ipcLen, curASCIINum, curNumLen);
            return;
        } else {
            /* ���, ���ӺŵĹ��ں��У����������� */
            ccLen = TAF_GetCcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_SkipDigitsOfNumHeader(curASCIINum, curNumLen, ccLen);
        }

        /* ���������ں�������ִ�� */
        if (TAF_IsNumEntityHeaderInCountryList(mccFirstMatchEntryIndex, mccMatchEntryNum, curASCIINum, maxASCIINum) ==
            VOS_FALSE) {
            /* ����ʵ��ͷ������country list �У���dpc���뵽����ͷ�� */
            numDpc = TAF_GetDpcOfCountryList(mccFirstMatchEntryIndex);
            dpcLen = TAF_GetDpcLenOfCountryList(mccFirstMatchEntryIndex);

            TAF_InsertDigitsInNumHeader(numDpc, dpcLen, curASCIINum, curNumLen);
        }
    } else {
        /* ��ͬ�����ӺŵĹ��ʺ� ������IPC�����պ����ʽ�� ipc + cc + ����ʵ�� */
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
     * 1�����й����У�����������ҵ���Ƕ���ҵ����ǰһ�׶εĴ�������У�������Ӻţ������Ӻ�ȥ����
     * ͨ�����������͸�Ϊ������������ʾ֮ǰ������мӺţ�
     * 2�����ʺ������ʹ���:
     * 2.1��������к����е�Cc������б��У�Mcc��Ӧ��CC��ͬ�������ӺŵĹ��ں���,ȥ��CC���ٸ���neh
     * �Ƿ�������б���Mcc��Ӧ����ƥ�䣬������ڲ�ƥ�䣬��Ҫ����npc
     * (exp +8613888888888 ת��Ϊ 13888888888��+86 21 888888888 ת��Ϊ 021 888888888)
     * 2.2��������к����е�Cc������б��У�Mcc��Ӧ��CC��ͬ�������ʺ��У����պ��к���ṹ:"ipc" + "cc" + "neh" + "ʣ�����"
     * (exp �����û������ձ��û� +81xxxxxxx ת��Ϊ 0081xxxxxxx)
     * 3�����ں������ʹ���
     * 3.1�������������Ϊ"ipc" + "cc" + "neh" + "ʣ�����"��ת��Ϊ"ipc/npc(�ж�ͬ2.1)"+"neh" + "ʣ�����"
     * (exp 008613888888888 ת��Ϊ 0013888888888��008614888888888 ת��Ϊ 014888888888)
     * 3.2���������벻���ı�
     * 4��������������й�(��½)���𣬺��к���ǰ�治���ټ�dpc�����к��밴��ת�����ֱ��ʹ��
     */

    errno_t                       memResult;
    VOS_UINT16                    mccFirstMatchEntryIndex;
    VOS_UINT16                    mccMatchEntryNum;
    VOS_UINT8                     asciiNumInfo[TAF_MO_MAX_ASCII_NUM_LEN + 1];
    VOS_UINT8                     numLenTemp;
    TAF_PlusTranslateNumTypeUint8 numTypeValue;

    /* ���mcc �Ƿ��� countrylist,����ƥ��ļ�¼�������Լ���һ����¼������ */
    mccFirstMatchEntryIndex = 0;
    mccMatchEntryNum        = 0;

    /* ��ȡƥ���Sid��Ϣ */
    TAF_GetInfoInCountryList(sid, &mccFirstMatchEntryIndex, &mccMatchEntryNum);

    /* û��ƥ���¼ */
    if (mccMatchEntryNum == 0) {
        return;
    }

    /* ��ȡ���к��볤�� */
    /* ������ȹ���ȡ�ضϵĳ��� */
    numLenTemp = TAF_MIN(*numLen, (TAF_MO_MAX_ASCII_NUM_LEN + 1));

    /* ��ȡ�������� */
    numTypeValue = *numType;

    /* ��ȡ���к��� */
    (VOS_VOID)memset_s(asciiNumInfo, sizeof(asciiNumInfo), 0x00, sizeof(asciiNumInfo));
    if (numLenTemp > 0) {
        memResult = memcpy_s(asciiNumInfo, sizeof(asciiNumInfo), asciiNum, numLenTemp);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNumInfo), numLenTemp);
    }

    if (numTypeValue == TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL) {
        /* ���ʺ��д��� */
        TAF_TranslatePlusToInternationalNum(mccFirstMatchEntryIndex, mccMatchEntryNum, asciiNumInfo,
                                            TAF_MO_MAX_ASCII_NUM_LEN, &numLenTemp, dialType, isServingChinaMainland);
    } else {
        if (TAF_TranslatePlusToNationalNum(mccFirstMatchEntryIndex, mccMatchEntryNum, asciiNumInfo,
                                           TAF_MO_MAX_ASCII_NUM_LEN, &numLenTemp,
                                           isServingChinaMainland) == VOS_FALSE) {
            return;
        }
    }

    /* �����ɼӺ�ת�������Ӻ��Ѿ���ȥ�������к������͸���Ϊ���ں������� */
    numTypeValue = TAF_PLUS_TRANSLATE_NUM_NATIONAL;

    /* ��ת����ĺ�����Ϣд��,ʹ���µĳ��� */

    /* ����TAF_TranslatePlusToNationalNum�л��޸�ucNumLenֵ�����ܳ���32����Ҫ��Ŀ��ռ�ȡС */
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

    /* ƥ����뿪ͷ��ipc�Ƿ��ڹ����б��� */
    if (TAF_IsIpcInCountryList(mccFirstMatchEntryIndex, asciiNum) == VOS_TRUE) {
        /* ipc ƥ�䣬����ipcλ */
        ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_SkipDigitsOfNumHeader(asciiNum, numLen, ipcLen);

        /* ���ź��� */
        if (dialType == TAF_PLUS_TRANSLATE_DIAL_TYPE_SMS) {
            /* ����Ӻ��ں���ͷ�� */
            if ((*numLen) >= (TAF_MT_MAX_ASCII_NUM_LEN + 1)) {
                return VOS_FALSE;
            }

            numLenTemp = *numLen;

            /* ������� */
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
        /* ipc ƥ�䣬����ipcλ */
        ipcLen = TAF_GetIpcLenOfCountryList(mccFirstMatchEntryIndex);

        TAF_SkipDigitsOfNumHeader(asciiNum, numLen, ipcLen);

        /* ���º�������Ϊ���ʺ��� */
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
     * 1�����ʺ������ʹ���:
     * 1.1��"ipc" + "cc" +����,����ҵ��ת��Ϊ"cc" +����
     * 1.2��"ipc" + "cc" +����,����ҵ��ת��Ϊ"+" + "cc" + ����
     * 2�����ں������ʹ���:
     * �κ�ҵ��"ipc" + "cc" +����,����ҵ��ת��Ϊ"cc" +����
     * 3���������벻���޸�
     */

    errno_t                       memResult;
    VOS_UINT16                    mccFirstMatchEntryIndex = 0;
    VOS_UINT16                    mccMatchEntryNum = 0;
    VOS_UINT8                     asciiNumInfo[TAF_MT_MAX_ASCII_NUM_LEN + 1];
    VOS_UINT8                     numLenTemp;
    TAF_PlusTranslateNumTypeUint8 numTypeValue;

    /* ���mcc �Ƿ��� countrylist,����ƥ��ļ�¼�������Լ���һ����¼������ */

    /* ��ȡƥ���MCC��Ϣ */
    TAF_GetInfoInCountryList(sid, &mccFirstMatchEntryIndex, &mccMatchEntryNum);

    /* û��ƥ���¼ */
    if (mccMatchEntryNum == 0) {
        return;
    }

    /* ��ȡ���к��볤�� */
    /* �Գ���ֵȡ���� */
    numLenTemp = TAF_MIN(*numLen, (TAF_MT_MAX_ASCII_NUM_LEN + 1));

    /* ��ȡ�������� */
    numTypeValue = *numType;

    /* ��ȡ���к��� */
    (VOS_VOID)memset_s(asciiNumInfo, sizeof(asciiNumInfo), 0x00, sizeof(asciiNumInfo));
    memResult = memcpy_s(asciiNumInfo, sizeof(asciiNumInfo), asciiNum, sizeof(asciiNumInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNumInfo), sizeof(asciiNumInfo));

    if (numTypeValue == TAF_PLUS_TRANSLATE_NUM_INTERNATIONAL) {
        /* ����ҵ�� */
        if (TAF_TranslateInternationalNumToPlus(mccFirstMatchEntryIndex, asciiNumInfo, &numLenTemp, dialType) ==
            VOS_FALSE) {
            return;
        }
    } else {
        /* ����ҵ�� */
        if (TAF_TranslateNationalNumToPlus(mccFirstMatchEntryIndex, asciiNumInfo, &numLenTemp, &numTypeValue) ==
            VOS_FALSE) {
            return;
        }
    }

    /* ��ת����ĺ�����Ϣд�� */
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
