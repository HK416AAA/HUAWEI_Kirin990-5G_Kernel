/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
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

#ifndef __APPRRCINTERFACE_H__
#define __APPRRCINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
#include  "lps_common.h"
#include  "app_rrc_interface.h"


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/




/*****************************************************************************
  3 Massage Declare
*****************************************************************************/
/*****************************************************************************
 ö����    : APP_RRC_MSG_ID_ENUM
 Э�����  :
 ASN.1���� :
 ö��˵��  : RRC <-> APP �ӿ���ϢID
*****************************************************************************/

typedef VOS_UINT32    APP_RRC_MSG_ID_ENUM_UINT32;

/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : DT_CMD_ENUM
 �ṹ˵��  : DT����ö��
*****************************************************************************/

typedef VOS_UINT8 DT_CMD_ENUM_UINT8;

/*****************************************************************************
 �ṹ��    : DT_RESULT_ENUM
 �ṹ˵��  : DT�������ö��
*****************************************************************************/

typedef VOS_UINT32 DT_RESULT_ENUM_UINT32;

typedef DT_CMD_ENUM_UINT8       APP_CMD_ENUM_UINT8;
typedef DT_RESULT_ENUM_UINT32   APP_RESULT_ENUM_UINT32;
/*****************************************************************************
 ö����    : APP_RRC_CELL_RPT_PERIOD_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 APP_RRC_CELL_RPT_PERIOD_ENUM_UINT8;

/*****************************************************************************
 ö����    : APP_LATENCY_TYPE_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 APP_LATENCY_TYPE_ENUM_UINT8;

/*****************************************************************************
 ö����    : APP_RRC_LOCK_WORK_INFO_TYPE_ENUM
 ö��˵��  : ����������
*****************************************************************************/

typedef VOS_UINT8 APP_RRC_LOCK_WORK_INFO_TYPE_ENUM_UINT8;
/*****************************************************************************
 ö����    : APP_DELAY_TYPE_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 APP_DELAY_TYPE_ENUM_UINT8;
/*****************************************************************************
 ö����    : APP_CAMPED_FLAG_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 APP_CAMPED_FLAG_ENUM_UINT8;
/*****************************************************************************
 ö����    : APP_DELAY_TYPE_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 APP_STATE_FLAG_ENUM_UINT8;


/*****************************************************************************
 ö����    : LRRC_CA_CC_NUM_ENUM
 ö��˵��  :
*****************************************************************************/

typedef VOS_UINT8 LRRC_CA_CC_NUM_ENUM_UINT8;


/*****************************************************************************
   5 STRUCT
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : APP_OM_Msg
 �ṹ˵��  : APP(��̨����)��OM��������Ϣ��
*****************************************************************************/
typedef struct
{
     VOS_MSG_HEADER                     /*VOSͷ
 */
     VOS_UINT32          msgId;
     APP_MSG_HEADER                     /*APPͷ
 */
     VOS_UINT8           para[4];    /* �������� */
}APP_OM_MSG_STRU;

/*****************************************************************************
 �ṹ��    : APP_PLMN_ID_STRU
 �ṹ˵��  :
    MCC, Mobile country code (aucPlmnId[0], aucPlmnId[1] bits 1 to 4)
    MNC, Mobile network code (aucPlmnId[2], aucPlmnId[1] bits 5 to 8).

    The coding of this field is the responsibility of each administration but BCD
    coding shall be used. The MNC shall consist of 2 or 3 digits. For PCS 1900 for NA,
    Federal regulation mandates that a 3-digit MNC shall be used. However a network
    operator may decide to use only two digits in the MNC over the radio interface.
    In this case, bits 5 to 8 of octet 4 shall be coded as "1111". Mobile equipment
    shall accept MNC coded in such a way.

    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2            |           MCC digit 1
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3            |           MCC digit 3
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2            |           MNC digit 1
    ---------------------------------------------------------------------------

    AT���
    at+cops=1,2,"mcc digit 1, mcc digit 2, mcc digit 3, mnc digit 1, mnc digit 2, mnc

digit 3",2 :

    e.g.
    at+cops=1,2,"789456",2 :
    --------------------------------------------------------------------------------
    (mcc digit 1)|(mcc digit 2)|(mcc digit 3)|(mnc digit 1)|(mnc digit 2)|(mnc digit 3)
    --------------------------------------------------------------------------------
       7         |     8       |      9      |     4       |      5      |     6
    --------------------------------------------------------------------------------

    ��aucPlmnId[3]�еĴ�Ÿ�ʽ:
    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2 = 8        |           MCC digit 1 = 7
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3 = 6        |           MCC digit 3 = 9
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2 = 5        |           MNC digit 1 = 4
    ---------------------------------------------------------------------------

*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucPlmnId[3];
    VOS_UINT8                           ucRsv;
} APP_PLMN_ID_STRU;

/*****************************************************************************
 �ṹ��    : APP_RRC_CELL_MEAS_RPT_REQ_STRU
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;
    APP_RRC_CELL_RPT_PERIOD_ENUM_UINT8  enPeriod;
    VOS_UINT8                           aucReserved[2];
}APP_RRC_CELL_MEAS_RPT_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_APP_CELL_MEAS_RPT_CNF_STRU
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_CELL_MEAS_RPT_CNF_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_CELL_MEAS_RSLT_STRU
 �ṹ˵��  :С���������
*****************************************************************************/
typedef struct
{
    VOS_UINT16 usCellId;
    VOS_INT16  sRSRP;
    VOS_INT16  sRSRQ;
    VOS_INT16  sRSSI;
}RRC_APP_CELL_MEAS_RSLT_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_CELL_MEAS_REPORT_IND_STRU
 �ṹ˵��  :RRCͨ����ԭ�������ϱ�С�����������������
            ��ǰ����ͬƵ����ʱ��RRC�ϱ�С���Ĳ�������������ڣ����ϱ����ݣ�
            �����ǰ������ͬƵ���������������֮������ͬƵ������RRC����֮ǰ��OM���ý����ϱ�С�����������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT8                           ucCellCnt;                              /* С������ */
    VOS_UINT8                           aucReserved[3];
    RRC_APP_CELL_MEAS_RSLT_STRU         astCellMeasInd[RRC_APP_CELL_MAX_COUNT];
}RRC_APP_CELL_MEAS_REPORT_IND_STRU;

/*****************************************************************************
 �ṹ��    : APP_RRC_TIME_DELAY_RPT_REQ_STRU
 �ṹ˵��  : Ҫ���ϱ�������ʱ�ӡ�������ʱ�ӣ�����RB�����Ŀ���ʱ�䣬���������ӽ�������RB1�����ɹ���ʱ�䡣
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;
    APP_DELAY_TYPE_ENUM_UINT8           enDelayType;
    VOS_UINT8                           aucReserved[2];
}APP_RRC_TIME_DELAY_RPT_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_TIME_DELAY_RPT_CNF_STRU
 �ṹ˵��  :��ԭ��APP_RRC_TIME_DELAY_RPT_REQ_STRU�Ļظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_DELAY_TYPE_ENUM_UINT8           enDelayType;
    VOS_UINT8                           aucReserved[3];
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_TIME_DELAY_RPT_CNF_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU
 �ṹ˵��  :��ȡ��ǰפ��С����ID��Ƶ�����Ϣ�Ĳ�ѯ���󣬻���ֹͣ�ϱ�פ��С����Ϣ������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;
    VOS_UINT8                           aucReserved[3];
}APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_INQ_CAMP_CELL_INFO_CNF_STRU
 �ṹ˵��  :��ԭ��APP_RRC_INQ_CAMP_CELL_INFO_REQ_STRU�Ļظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_INQ_CAMP_CELL_INFO_CNF_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_INQ_CAMP_CELL_INFO_IND_STRU
 �ṹ˵��  :�ϱ���ǰפ��С����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    PS_BOOL_ENUM_UINT8                  enValueFlag;
    VOS_UINT8                           aucReserved[3];
    VOS_UINT16                          usCellId;
    VOS_UINT16                          usFreqInfo;
}RRC_APP_INQ_CAMP_CELL_INFO_IND_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_FREQ_BAND_STRU
 �ṹ˵��  :Ƶ����Χ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usLowBand;                              /* Ƶ������ */
    VOS_UINT16                          usHighBand;                             /* Ƶ������ */
}APP_RRC_FREQ_BAND_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_CELL_INFO_STRU
 �ṹ˵��  :С����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usCellId;                               /* С��ID */
    VOS_UINT16                          usFreqPoint;                            /* Ƶ����Ϣ */
}APP_RRC_CELL_INFO_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_LOCK_INFO_STRU
 �ṹ˵��  :�����������Ϣ
*****************************************************************************/
typedef struct
{
    APP_RRC_LOCK_WORK_INFO_TYPE_ENUM_UINT8  enLockType;                         /* �������� */
    VOS_UINT8                               ucBandInd;
    union
    {
        VOS_UINT16                      usFreqPoint;                            /* ������Ƶ�� */
        APP_RRC_CELL_INFO_STRU          stFreqAndCell;                          /* ������С��ID��Ƶ�� */
        APP_RRC_FREQ_BAND_STRU          stFreqBand;                             /* ����Ƶ���������� */
    }u;
}APP_RRC_LOCK_INFO_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_LOCK_WORK_INFO_REQ_STRU
 �ṹ˵��  :·���·�����������Ľṹ��
*****************************************************************************/
/*lint -save -e959*/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;                              /* �������ǽ��� */
    VOS_UINT8                           aucReserved[3];                         /* ����, �˴���2, ����Ϊ��һ���ǵ��ֽ� */
    APP_RRC_LOCK_INFO_STRU              stLockInfo;
}APP_RRC_LOCK_WORK_INFO_REQ_STRU;
/*lint -restore*/

/*****************************************************************************
 �ṹ��    :RRC_APP_LOCK_WORK_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_LOCK_WORK_INFO_REQ���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_LOCK_WORK_INFO_CNF_STRU;


/*****************************************************************************
 �ṹ��    :APP_LRRC_GET_UE_CAP_INFO_REQ_STRU
 �ṹ˵��  :HIDS�·��Ļ�ȡ�ؼ�UE������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT8                           aucReserved[128];                         /* ���� */
}APP_LRRC_GET_UE_CAP_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UE_CAP_INFO_STRU
 �ṹ˵��  : LRRC��APP֮���ϱ���UE�����ؼ���Ԫ
*****************************************************************************/
typedef struct
{
    /* ulFGI01To32: APP������bitλ����: short DRX:FGI4, C-DRX:FGI5, L-W,PSHO:FGI8,
       L-G,PSHO:FGI9, CSFB w/measurements(REL9):FGI22, TTI Bundling:FGI28, SPS:FGI29 */
    VOS_UINT32                          ulFGI01To32;                            /* fgi bitλ�����λ�����λ��ӦFGI1��FGI32 */
    VOS_UINT32                          ulFGI33To64;                            /* fgi bitλ�����λ�����λ��ӦFGI33��FGI64 */
    VOS_UINT32                          ulFGI101To132;                          /* fgi bitλ�����λ�����λ��ӦFGI101��FGI132 */
    VOS_UINT32                          ulFGIReserved;                          /* fgi bit����λ������32bit�Ա���Э����չ */

    /* �����bit��ʼ������Ϊprofile0x0001,�Դ����ƣ����ֵΪ1����ʾ֧�֣�0��ʾ
       ��֧��;�ֱ��Ӧprofile0x0001,profile0x0002,profile0x0003,profile0x0004,
       profile0x0006,profile0x0101,profile0x0103,profile0x0104 */
    VOS_UINT16                          usROHC;

    VOS_UINT8                           ucCategory;
    LRRC_CA_CC_NUM_ENUM_UINT8           enCaCCNum;                              /* ���֧�ֵ�CA CC�� */


    VOS_UINT32                          ulBandNum;                                   /* usBandNum ȡֵ��Χ[1,256] */
    VOS_UINT32                          aulBandInd[LRRC_APP_MAX_BAND_IND_ARRAY_NUM]; /* aulBandInd,ÿ��BIT��1:����֧�֣�0������֧�֣�
                                                                                        ulBandInd[0]�е�һ��BIT����BAND1,�Դ�����; */
    VOS_UINT32                          aul256Qam[LRRC_APP_MAX_BAND_IND_ARRAY_NUM]; /* aul256Qam,ÿ��BIT��1:����֧�֣�0������֧�֣�
                                                                                        aul256Qam[0]�е�һ��BIT���Դ�����; */
    PS_BOOL_ENUM_UINT8                  enUlMimo4Layer;                           /* �Ƿ�֧������4�� */
    PS_BOOL_ENUM_UINT8                  enDlMimo4Layer;                           /* �Ƿ�֧������4�� */
    VOS_UINT8                           aucReserved[2];
}LRRC_APP_UE_CAP_INFO_STRU;

/*****************************************************************************
 �ṹ��    :LRRC_APP_GET_UE_CAP_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_LRRC_GET_UE_CAP_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
    LRRC_APP_UE_CAP_INFO_STRU           stUeCapInfo;

    VOS_UINT8                           aucReserved[32];                       /* ����ָʾ�����Ƿ���Indָʾ
                                                                                   ��Ŀǰ��Ҫ������չCA��ϣ���Ϊ
                                                                                   CA���ռ�ÿռ�ܴ� */
}LRRC_APP_GET_UE_CAP_INFO_CNF_STRU;


/*****************************************************************************
 �ṹ��    : APP_RRC_TRANSPARENT_CMD_REQ
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :͸������ͷ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgId;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT16 usTransCmdNum;
    VOS_UINT16 usParaSize;
    VOS_UINT8  aucPara[4];
}APP_RRC_TRANSPARENT_CMD_REQ_STRU;


/*****************************************************************************
 �ṹ��    : RRC_APP_TRANSPARENT_CMD_CNF_STRU
 �ṹ˵��  : PS->OMT��͸������ִ�н�����ݽṹ(
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32          ulMsgId;                            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT8           aucTransCmdCnf[4];                 /*͸���������������ɱ�*/
}RRC_APP_TRANSPARENT_CMD_CNF_STRU;

/*****************************************************************************
 �ṹ��    :PS_APP_HO_LATENCY_DETAIL_STRU
 �ṹ˵��  :�л�ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          T4_ulRrcRcvHoRecfg;
    VOS_UINT32                          ulRrcRcvNasRabmRsp;
    VOS_UINT32                          ulRrcRcvCmmHoCnf;
    VOS_UINT32                          ulRrcRcvStopPdcpCnf;
    VOS_UINT32                          T5_ulRrcSndDspHoReq;
    VOS_UINT32                          T6_ulRrcRcvDspHoCnf;
    VOS_UINT32                          T7_ulRrcSndMacRaReq;
    VOS_UINT32                          T12_ulMacSndPhyAccessReq;
    VOS_UINT32                          T13_ulMacRcvRar;
    VOS_UINT32                          T8_ulRrcRcvMacRaCnf;
    VOS_UINT32                          ulRrcSndSmcSecuCfg;
    VOS_UINT32                          T9_ulRrcSndRecfgCmp;
    VOS_UINT32                          T10_ulRrcRcvAmDataCnf;
    VOS_UINT32                          ulRrcRcvCqiSrsCnf;
    VOS_UINT32                          ulRrcRcvPdcpContineCnf;
    VOS_UINT32                          T11_ulRrcRcvRabmStatusRsp;
}PS_APP_HO_LATENCY_DETAIL_STRU;
/*****************************************************************************
 �ṹ��    :PS_APP_HO_COMP_DETAIL_STRU
 �ṹ˵��  :�л�ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRealTestT6T5;
    VOS_UINT32                          ulPtlT6T5;          /* 1ms */
    VOS_UINT32                          ulRealTestT8T7;
    VOS_UINT32                          ulPtlT8T7;          /* 2.5 + 1+ 7.5ms */
    VOS_UINT32                          ulRealTestT10T9;
    VOS_UINT32                          ulPtlT10T9;         /* 13ms */
    VOS_UINT32                          ulRealTestT7T4;
    VOS_UINT32                          ulPtlT7T4;          /* 50ms */
    VOS_UINT32                          ulRealTestT11T4;
    VOS_UINT32                          ulPtlT11T4;         /* 100ms */
}PS_APP_HO_COMP_DETAIL_STRU;
/*****************************************************************************
 �ṹ��    :PS_APP_EST_LATENCY_DETAIL_STRU
 �ṹ˵��  :����ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          T1_ulRrcSndMacRaReq;
    VOS_UINT32                          T2_ulMacSndPhyAccessReq;
    VOS_UINT32                          T4_ulMacRcvRar;
    VOS_UINT32                          ulRrcRcvMacRaCnf;
    VOS_UINT32                          T5_ulMacSndMsg3;
    VOS_UINT32                          T6_ulRrcRcvSetUp;
    VOS_UINT32                          T7_ulRrcSndEstCmp;
    VOS_UINT32                          ulRrcRcvSecCmd;
    VOS_UINT32                          ulRrcSndSecCmdCmp;
    VOS_UINT32                          ulRrcRcvCapEnq;
    VOS_UINT32                          ulRrcSndCapInfo;
    VOS_UINT32                          ulRrcRcvRecfg;
    VOS_UINT32                          T13_ulRrcSndRecfgCmp;
}PS_APP_EST_LATENCY_DETAIL_STRU;
/*****************************************************************************
 �ṹ��    :PS_APP_EST_COMP_STRU
 �ṹ˵��  :����ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRealTestT4T1;
    VOS_UINT32                          ulPtlT4T1;              /* 5s + 6ms */
    VOS_UINT32                          ulRealTestT5T4;
    VOS_UINT32                          ulPtlT5T4;              /* 2.5ms */
    VOS_UINT32                          ulTestRealT6T5;
    VOS_UINT32                          ulPtlT6T5;              /* 28.5ms+2*Ts1c */
    VOS_UINT32                          ulRealTestT7T6;
    VOS_UINT32                          ulPtlT7T6;              /* 3ms */
    VOS_UINT32                          ulReaTestlT7T1;
    VOS_UINT32                          ulPtlT7T1;              /* 47.5ms+2*Ts1c */
    VOS_UINT32                          ulReaTestlT13T1;
    VOS_UINT32                          ulPtlT13T1;             /* 61ms+2*Ts1c+Ts1u */
}PS_APP_EST_COMP_DETAIL_STRU;
/*****************************************************************************
 �ṹ��    :PS_APP_REEST_LATENCY_DETAIL_STRU
 �ṹ˵��  :�ؽ�ʱ�ӹؼ�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          T2_ulRrcRcvReestInd;
    VOS_UINT32                          T3_ulRrcSndCellSearchReq;
    VOS_UINT32                          T4_ulRrcRcvCellSearchInd;
    VOS_UINT32                          T5_ulRrcRcvSi;
    VOS_UINT32                          T6_ulRrcSndCampReq;
    VOS_UINT32                          T7_ulRrcRcvCampCnf;
    VOS_UINT32                          T10_ulRrcSndMacRaReq;
    VOS_UINT32                          ulMacSndPhyAccessReq;
    VOS_UINT32                          ulMacRcvRar;
    VOS_UINT32                          ulRrcRcvMacRaCnf;
    VOS_UINT32                          ulMacSndMsg3;
    VOS_UINT32                          ulRrcRcvReest;
    VOS_UINT32                          ulRrcSndReestCmpl;
}PS_APP_REEST_LATENCY_DETAIL_STRU;

/*****************************************************************************
 �ṹ��    :PS_APP_REEST_COMP_DETAIL_STRU
 �ṹ˵��  :�ؽ�ʱ����Э��Ա�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRealTestT4T3;
    VOS_UINT32                          ulPtlT4T3;              /* 100ms */
    VOS_UINT32                          ulRealTestT5T4;
    VOS_UINT32                          ulPtlT5T4;              /* 1280ms */
    VOS_UINT32                          ulRealTestT10T2;
    VOS_UINT32                          ulPtlT10T2;             /* 1500ms */

}PS_APP_REEST_COMP_DETAIL_STRU;
/*****************************************************************************
 �ṹ��    :RRC_APP_TIME_LATENCY_RPT_IND_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  T7T4_ulHoCPlaneRealLatency;
    VOS_UINT32                                  ulHoCPlanePtlLatency;
    VOS_UINT32                                  T11T4_ulHoUPlaneRealLatency;
    VOS_UINT32                                  ulHoUPlanePtlLatency;
    PS_APP_HO_LATENCY_DETAIL_STRU               stHoLatencyDetail;
    PS_APP_HO_COMP_DETAIL_STRU                  stHoCompDetail;
}PS_APP_HO_LATENCY_STRU;

/*****************************************************************************
 �ṹ��    :PS_APP_REEST_LATENCY_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  T10T2_ulReestRealLatency;
    VOS_UINT32                                  ulReestPtlLatency;
    PS_APP_REEST_LATENCY_DETAIL_STRU            stReestLatencyDetail;
    PS_APP_REEST_COMP_DETAIL_STRU               stReestCompDetail;
}PS_APP_REEST_LATENCY_STRU;

/*****************************************************************************
 �ṹ��    :PS_APP_EST_LATENCY_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  T13T1_ulEstUPlaneRealLatency;
    VOS_UINT32                                  ulEstUPlanePtlLatency;
    VOS_UINT32                                  T7T1_ulEstCPlaneRealLatency;
    VOS_UINT32                                  ulEstCPlanePtlLatency;
    PS_APP_EST_LATENCY_DETAIL_STRU              stEstLatencyDetail;
    PS_APP_EST_COMP_DETAIL_STRU                 stEstCompDetail;
}PS_APP_EST_LATENCY_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_TIME_LATENCY_RPT_IND_STRU
 �ṹ˵��  :ʱ��ʱ����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    APP_LATENCY_TYPE_ENUM_UINT8         enLatencyType;
    VOS_UINT8                           aucReserved[3];
union
    {
        PS_APP_HO_LATENCY_STRU             stHoElapse;
        PS_APP_REEST_LATENCY_STRU          stReestElapse;
        PS_APP_EST_LATENCY_STRU            stEstElapse;
    }u;
} RRC_APP_TIME_DELAY_RPT_IND_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_CSQ_REQ_STRU
 �ṹ˵��  :CSQ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT16                          usSwt;              /* 0: ֹͣ�����ϱ� 1: ��ѯRSSI��������IND�ϱ�  2: ���������ϱ�RSSI */
    VOS_UINT16                          usPeriod;
    VOS_UINT16                          usNdb;              /* ��Χ 0-5 dbm*/
    VOS_UINT16                          usMs;               /* ��Χ 1-20 s*/
}APP_RRC_CSQ_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_CNF_STRU
 �ṹ˵��  :CSQ��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usRI;                /*RIֵ*/
    VOS_UINT16                          ausCQI[2];           /* CQI�������� */
    VOS_UINT8                           aucRes[2];

}APP_RRC_CQI_INFO_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_CNF_STRU
 �ṹ˵��  :CSQ��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
    VOS_INT16                           sRsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           sRsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_INT16                           sRssi;              /* RSSI����ֵ */
    VOS_UINT16                          usBer;              /* ������ */
    VOS_INT32                           lSINR;              /* SINR  RS_SNR */
    APP_RRC_CQI_INFO_STRU               stCQI;              /* CQI�������� */
}RRC_APP_CSQ_CNF_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_CSQ_IND_STRU
 �ṹ˵��  :RRC�ϱ���APP������CSQ��ѯָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_INT16                           sRsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           sRsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_INT16                           sRssi;              /* RSSI����ֵ */
    VOS_UINT16                          usBer;              /* ������ */
    VOS_INT32                           lSINR;              /* SINR  RS_SNR */
    APP_RRC_CQI_INFO_STRU               stCQI;              /* CQI�������� */
}RRC_APP_CSQ_IND_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_PTL_STATE_QUERY_REQ_STRU
 �ṹ˵��  :Э��״̬��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;
    APP_RRC_CELL_RPT_PERIOD_ENUM_UINT8  enPeriod;
    VOS_UINT8                           aucReserved[2];

}APP_RRC_PTL_STATE_QUERY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_QUERY_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_PTL_STATE_QUERY_CNF_STRU;


/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_CHANGE_IND_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT32                          ulCurrentState;     /* RRCЭ��״̬, 0:��ʾIDLE̬ 1:��ʾCONNECTED 2:��ʾЭ��״̬��Ч */
}RRC_APP_PTL_STATE_CHANGE_IND_STRU;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_CHANGE_IND_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : RRC�ϱ���Э��״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCurrentState;     /* RRCЭ��״̬, 0:��ʾIDLE̬ 1:��ʾCONNECTED 2:��ʾЭ��״̬��Ч */
}RRC_APP_DT_PTL_STATE_IND_STRU;



/*****************************************************************************
 �ṹ��    :APP_RRC_PTL_STATE_QUERY_REQ_STRU
 �ṹ˵��  :С��������Ϣ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
}APP_RRC_CELL_INFO_QUERY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_APP_PTL_STATE_QUERY_CNF_STRU
 �ṹ˵��  : RRC�ظ���С��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT16                          usFreq;             /* 0xFFFFΪ��Ч,��λ��100KHz */
    VOS_UINT16                          usPci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_UINT8                           ucDlBandWidth;      /* 0xffΪ��Ч,��Χ:(0,5): (0 : 1.4M , 1 : 3M , 2 : 5M ,3 : 10M , 4 : 15M, 5 : 20M) */
    VOS_UINT8                           aucReserved[3];
}RRC_APP_CELL_INFO_QUERY_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_CMD_ENUM_UINT8                  enCommand;
    APP_RRC_CELL_RPT_PERIOD_ENUM_UINT8  enPeriod;
    VOS_UINT8                           aucReserved[2];
}APP_RRC_INQ_CMD_REQ_STRU;

typedef APP_RRC_INQ_CMD_REQ_STRU APP_RRC_INQ_SERVING_CELL_INFO_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}APP_RRC_INQ_CMD_CNF_HEADER_STRU;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
}APP_RRC_INQ_CMD_IND_HEADER_STRU;


typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_FORCE_HOANDCSEL_CNF_STRU;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    DT_CMD_ENUM_UINT8                   enCmd;              /* FORCE HO/CSEL FLAG */
    VOS_UINT8                           ucRsv[3];
    VOS_UINT32                          ulPci;              /* PHY Cell ID */
}APP_RRC_FORCE_HOANDCSEL_REQ_STRU;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_BARCELL_ACCESS_CNF_STRU;
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgID;                                     /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    DT_CMD_ENUM_UINT8                   enCmd;              /* Bar Cell Access FLAG */
    VOS_UINT8                           ucRsv[3];
}APP_RRC_BARCELL_ACCESS_REQ_STRU;



/*****************************************************************************
 �ṹ��    : APP_RRC_MSG_DATA
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : APP_RRC_MSG_DATA���ݽṹ,����ΪRRCר��
*****************************************************************************/
typedef struct
{
    APP_RRC_MSG_ID_ENUM_UINT32          enMsgID;        /*_H2ASN_MsgChoice_Export APP_RRC_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          APP_RRC_MSG_ID_ENUM_UINT32
    ****************************************************************************/
}APP_RRC_MSG_DATA;

/*_H2ASN_Length UINT32*/

/*****************************************************************************
 �ṹ��    : App_Rrc_InterfaceMsg
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : AppRrcInterface_MSG���ݽṹ,����ΪRRCר��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    APP_RRC_MSG_DATA                    stMsgData;
}AppRrcInterface_MSG;
/*********************************************************
 ö����    : LRRC_LPHY_LTE_BAND_WIDTH_ENUM
 Э�����  :
 ASN.1���� :
 ö��˵��  :
**********************************************************/

typedef VOS_UINT16 RRC_APP_BAND_WIDTH_ENUM_UINT16;

/*****************************************************************************
 �ṹ��    : APP_RRC_LWCLASH_REQ_STRU
 �ṹ˵��  :Э��״̬��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
} APP_RRC_LWCLASH_REQ_STRU;

/*********************************************************
 ö����    : APP_RRC_ANTENNA_MAX_LAYERS_MIMO_ENUM
 Э�����  :
 ASN.1���� :
 ö��˵��  :
**********************************************************/

typedef VOS_UINT8    APP_RRC_ANTENNA_MAX_LAYERS_MIMO_ENUM_UINT8;

/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_PARA_STRU
�ṹ˵��  : RRC�ϱ�����Ϣ
*****************************************************************************/
typedef struct
{

    VOS_UINT32                              ulUlFreq;          /*��������Ƶ�� ��λ:100Khz*/
    VOS_UINT32                              ulDlFreq;          /*��������Ƶ�� ��λ:100Khz*/

    RRC_APP_BAND_WIDTH_ENUM_UINT16          usUlBandwidth;     /*���д��� */
    RRC_APP_BAND_WIDTH_ENUM_UINT16          usDlBandwidth;     /*���д��� */
    APP_CAMPED_FLAG_ENUM_UINT8              enCamped;          /*�Ƿ�פ�� */
    APP_STATE_FLAG_ENUM_UINT8               enState;           /*�Ƿ�Ϊ��ͻ״̬ */
    VOS_UINT8                               usBand;            /*Ƶ��ָʾ */
    APP_RRC_ANTENNA_MAX_LAYERS_MIMO_ENUM_UINT8   enDlMimo;
} RRC_APP_LWCLASH_PARA_STRU;


/*****************************************************************************
 �ṹ��    : RRC_APP_SCELL_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT32                              ulUlFreq;           /*��������Ƶ�� ��λ:100Khz*/
    VOS_UINT32                              ulDlFreq;           /*��������Ƶ�� ��λ:100Khz*/
    RRC_APP_BAND_WIDTH_ENUM_UINT16          usUlBandwidth;      /*���д��� */
    RRC_APP_BAND_WIDTH_ENUM_UINT16          usDlBandwidth;      /*���д��� */
    APP_RRC_ANTENNA_MAX_LAYERS_MIMO_ENUM_UINT8   enDlMimo;
    VOS_UINT8                               aucResv[3];
}RRC_APP_SCELL_INFO_STRU;



/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;         /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    RRC_APP_LWCLASH_PARA_STRU           stLWClashPara;
    VOS_UINT32                          ulScellNum;
    RRC_APP_SCELL_INFO_STRU             stScellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
    PS_BOOL_ENUM_UINT8                  dl256QamFlag;
    VOS_UINT8                           resv[3];
} RRC_APP_LWCLASH_CNF_STRU;
/*****************************************************************************
 �ṹ��    : RRC_APP_LWCLASH_IND_STRU
�ṹ˵��  : RRC�ϱ�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    RRC_APP_LWCLASH_PARA_STRU           stLWClashPara;
    VOS_UINT32                          ulScellNum;
    RRC_APP_SCELL_INFO_STRU             stScellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
    PS_BOOL_ENUM_UINT8                  dl256QamFlag;
    VOS_UINT8                           resv[3];
} RRC_APP_LWCLASH_IND_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_NMR_REQ_STRU
 �ṹ˵��  :NMR��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
}APP_RRC_NMR_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usMncNum;           /*ָʾ MNC ����*/
    VOS_UINT16                          usMcc;
    VOS_UINT16                          usMnc;
    VOS_UINT8                           aucRes[2];
} RRC_APP_PLMN_ID_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    RRC_APP_PLMN_ID_STRU                stPlmnId;
    VOS_UINT32                          sCellId;            /* ��Χ��(0,503) */
} RRC_APP_CELL_GLOBAL_ID_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CGI_STRU
 �ṹ˵��  :NMR�ṹ�嶨��
*****************************************************************************/
typedef struct
{
    RRC_APP_CELL_GLOBAL_ID_STRU         stCellGloId;        /* CellGlobalId */
    VOS_UINT16                          usTAC;
    VOS_UINT16                          usPci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_INT16                           sRsrp;              /* RSRP����ֵ��Χ��(-141,-44) */
    VOS_INT16                           sRsrq;              /* RSRQ����ֵ��Χ��(-40, -6) */
    VOS_UINT16                          usTa;               /* TAֵ*/
    VOS_UINT8                           aucRes[2];
} RRC_APP_LTE_CELL_INFO_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_NMR_CNF_STRU
 �ṹ˵��  :NMR��ѯ������Ӧ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
    RRC_APP_LTE_CELL_INFO_STRU          stLteCelInfo;
}RRC_APP_NMR_CNF_STRU;

/*****************************************************************************
 �ṹ��    :APP_RRC_CELLID_REQ_STRU
 �ṹ˵��  :С��������Ϣ��ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
}APP_RRC_CELLID_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_APP_CELLID_CNF_STRU
 �ṹ˵��  : RRC�ظ���С��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
    APP_PLMN_ID_STRU                    stPlmnId;
    VOS_UINT32                          ulCi;               /* 0xFFFFΪ��Ч, */
    VOS_UINT16                          usPci;              /* 0xFFFFΪ��Ч,��Χ��(0,503) */
    VOS_UINT16                          usTAC;             /* TAC */
}RRC_APP_CELLID_CNF_STRU;

/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : APP_LPS_MSG_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulMsgId;      /*��ϢID*/
    VOS_UINT32                          ulSize;       /*��Ϣ��ĳ���*/
    VOS_UINT8                           aucValue[4];  /*��Ϣ����Ч���ݵ�ָ��*/
}APP_LPS_MSG_STRU;


/*****************************************************************************
 �ṹ��    : APP_LRRC_RESEL_OFFSET_CFG_NTF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER

    VOS_UINT32             ulOpId;
    VOS_UINT32             ulFlag;
}APP_LRRC_RESEL_OFFSET_CFG_NTF_STRU;

/*****************************************************************************
 �ṹ��    : APP_LRRC_CON_TO_IDLE_NTF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32              ulOpId;
    VOS_INT32              ulReserv;
}APP_LRRC_CON_TO_IDLE_NTF_STRU;


/*****************************************************************************
 ö����    : LRRC_LPDCP_FAST_DORMANCY_CMD_ENUM
 Э�����  :
 ASN.1���� :
 ö��˵��  : ָʾPDCP������ֹͣFAST_DORMANCY
*****************************************************************************/
typedef VOS_UINT32 APP_LRRC_FAST_DORM_ENUM_UINT32;

/*****************************************************************************
 �ṹ��    : APP_LRRC_FAST_DORM_CFG_NTF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    APP_LRRC_FAST_DORM_ENUM_UINT32      ulFlag;
    VOS_UINT32            ulTimerLen;   /* unite: s */
}APP_LRRC_FAST_DORM_CFG_NTF_STRU;



/*****************************************************************************
 �ṹ��    : ID_APP_LRRC_SET_UE_REL_VERSION_REQ
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32             ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32              ulMode;     /* 2:LTE , 1:TDS */
    VOS_UINT32              ulVersion;  /* 9 - 14 */
}APP_LRRC_SET_UE_REL_VERSION_REQ_STRU;

/*****************************************************************************
 �ṹ��    : ID_LRRC_APP_SET_UE_REL_VERSION_CNF
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;         /* MSPֱ�ӽ���ID��ΪCmdID����Prob */
    APP_RESULT_ENUM_UINT32              enResult;
} LRRC_APP_SET_UE_REL_VERSION_CNF_STRU;


/*****************************************************************************
 �ṹ��    :APP_RRC_SCELL_INFO_REQ_STRU
 �ṹ˵��  :AT�·��Ĳ�ѯSCELL��Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT8                           aucReserved[4];                         /* ���� */
}APP_RRC_SCELL_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_SCELL_INFO_STRU
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT32  ulPhyCellId;          /* scell������С��id */
    VOS_UINT32  ulScellUlFreq;        /* scell����Ƶ�� */
    VOS_UINT32  ulScellDlFreq;        /* scell����Ƶ�� */
    VOS_UINT32  ulScellUlFreqPoint;     /* scell����Ƶ��*/
    VOS_UINT32  ulScellDlFreqPoint;     /* scell����Ƶ��*/
    VOS_UINT8    usScellUlBandWidth;   /* scell���д���*/
    VOS_UINT8    usScellDlBandWidth;   /* scell���д���*/
    VOS_UINT8    ucFreqBandIndicator;   /* scell Ƶ��*/
    VOS_UINT8    ucRsv;   /* ���� */
    VOS_INT16    sRssi;
    VOS_INT16    sRsrp;
    VOS_INT16    sRsrq;
    VOS_UINT16  usScellIndex;
}RRC_SCELL_INFO_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                            ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                            ulOpId;
    APP_RESULT_ENUM_UINT32      enResult;
    VOS_UINT32                            ulSCellCnt;
    RRC_SCELL_INFO_STRU           astSCellInfo[LRRC_SCELL_MAX_NUM];
}RRC_APP_SCELL_INFO_CNF_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_DAM_BAR_LIST_ITEM_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usFreqInfo;
    VOS_UINT16                          usCellId;
    VOS_UINT8                           ucBandInd;
    VOS_UINT8                           aucReserved[3];
    VOS_UINT32                          ulTimeOutTimeInMs;
    VOS_UINT32                          ulRemainTimeOutInMs;
}LRRC_DAM_BAR_LIST_ITEM_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_DAM_BAR_LIST_DBG_INFO_IND_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT16                          usItemCount;        /* Bar List����Ŀ���� */
    VOS_UINT16                          usRev;              /* ���� */
    LRRC_DAM_BAR_LIST_ITEM_STRU         astDamBarListItem[RRC_APP_MAX_LIMITED_ITEM_COUNT];
}LRRC_APP_DAM_BAR_LIST_DBG_INFO_IND_STRU;
/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_SERVE_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          enCurrReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                          enBandInd;           /* Ƶ��ָʾ */
    VOS_UINT16                          usFreqInfo;          /* ����С��Ƶ�� */
    VOS_UINT16                          usCellId;            /* ����С��ID */
    VOS_INT16                           sRsrp;               /* RSRP����ֵ */
    VOS_INT16                           sRsrq;               /* RSRQ����ֵ */
    VOS_INT16                           sRssi;               /* RSSI����ֵ */
    VOS_INT16                           sSValue;             /*����õ���SPֵ*/
    VOS_INT16                           sSqual;              /*����õ���SQֵ*/
    VOS_INT16                           sQRxLevMin;          /*׼��RSRP��������*/
    VOS_INT16                           sPCompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                           sQqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                           sSIntraSearchP;      /* ����ͬƵ������rsrp��ֵ���� */
    VOS_INT16                           sSIntraSearchQ;      /* ����ͬƵ������rsrq��ֵ���� */
    VOS_INT16                           sSNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                           sSNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                           sThreshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                           sThreshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
} LRRC_APP_IDLE_SERVE_INFO_STRU;


/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_INTRA_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usCellId;
    VOS_INT16                           sSValue;            /*����õ���SPֵ*/
    VOS_INT16                           sSqual;             /*����õ���SQֵ*/
    VOS_INT16                           sRsrp;              /* RSRP����ֵ */
    VOS_INT16                           sRsrq;              /* RSRQ����ֵ */
    VOS_INT16                           sRssi;              /* RSSI����ֵ */
}LRRC_APP_IDLE_CELL_MEAS_RESULT_STRU;

typedef struct
{
    VOS_UINT16                          enCurrReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                          enBandInd;           /* Ƶ��ָʾ */
    VOS_UINT16                          usFreqInfo;          /*Ƶ��*/
    VOS_UINT16                          usTotalCellNum;      /*С������*/
    VOS_UINT16                          usDetectedCellNum;   /*��⵽��С����Ŀ*/
    VOS_INT16                           sQRxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                           sPCompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                           sQqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                           sSIntraSearchP;      /* ����ͬƵ������rsrp��ֵ���� */
    VOS_INT16                           sSIntraSearchQ;      /* ����ͬƵ������rsrq��ֵ���� */
    LRRC_APP_IDLE_CELL_MEAS_RESULT_STRU astIntraCellMesRslt[LRRC_LCSEL_INTRA_CELL_REPORT_NUM];
}LRRC_APP_IDLE_INTRA_INFO_STRU;



/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_INTER_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              enCurrReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              usFreqInfo;
    VOS_UINT16                              enBandInd;           /* Ƶ��ָʾ */
    VOS_INT16                               sThreshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               sThreshLowQ;         /* ��ѡ����ʱʹ��R9���� */
    VOS_INT16                               sThreshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               sThreshHighQ;        /* ��ѡ����ʱʹ��R9���� */
    VOS_UINT16                              usTotalCellNum;      /*С������*/
    VOS_UINT16                              usDetectedCellNum;   /*��⵽��С����Ŀ*/
    LRRC_APP_IDLE_CELL_MEAS_RESULT_STRU     astInterCellMesRslt[LRRC_LCSEL_INTER_CELL_REPORT_NUM];/* SCELL���ڵ�һ����λ�� */
}LRRC_APP_IDLE_SINGLE_FREQ_MEAS_RESULT_STRU;

typedef struct
{
    VOS_UINT16                                   usInterFreqNum;
    VOS_INT16                                    sSNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                    sSNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                    sServValue;          /*����õ���SPֵ*/
    VOS_INT16                                    sServqual;           /*����õ���SQֵ*/
    VOS_INT16                                    sThreshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                    sThreshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
    LRRC_APP_IDLE_SINGLE_FREQ_MEAS_RESULT_STRU   astInterFreqMesRslt[LRRC_LCSEL_INTER_FREQ_REPORT_NUM];
}LRRC_APP_IDLE_INTER_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_UTRAN_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              usPrimaryCode;
    VOS_INT16                               sRscp;               /* ����1/8 */
    VOS_INT16                               sEcN0;               /* ����1/8 */
    VOS_INT16                               sSValue;             /*����õ���SPֵ*/
    VOS_INT16                               sSqual;              /*����õ���SQֵ*/
}LRRC_APP_UTRAN_CELL_MEAS_RESULT_STRU;

typedef struct
{
    VOS_UINT16                              enCurrReselPrior;    /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              usArfcn;
    VOS_INT16                               sRssi;               /* ����1/8 */
    VOS_INT16                               sQRxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                               sPCompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                               sQqualMin;           /* S׼��RSRQ�������� */
    VOS_INT16                               sThreshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               sThreshLowQ;         /* ��ѡ����ʱʹ��R9���� */
    VOS_INT16                               sThreshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               sThreshHighQ;        /* ��ѡ����ʱʹ��R9���� */
    VOS_UINT16                              usCellNum;
    LRRC_APP_UTRAN_CELL_MEAS_RESULT_STRU    astUtranCellMesRslt[LRRC_LCSEL_UTRAN_CELL_REPORT_NUM];
}LRRC_APP_UTRAN_SIGNLE_FREQ_MEAS_RESULT_STRU;

typedef struct
{
    VOS_UINT16                                      usArfcnNum;
    VOS_INT16                                       sSNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                       sSNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                       sServValue;          /*����õ���SPֵ*/
    VOS_INT16                                       sServqual;           /*����õ���SQֵ*/
    VOS_INT16                                       sThreshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                       sThreshServingLowQ;  /* ��ѡ����ʱ�S���R9���� */
    LRRC_APP_UTRAN_SIGNLE_FREQ_MEAS_RESULT_STRU     astUtranFreqMesRslt[LRRC_TRRC_PHY_MAX_SUPPORT_CARRIER_NUM];
}LRRC_APP_IDLE_UTRAN_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_IDLE_UTRAN_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              enCurrReselPrior;   /* ��ǰʹ�õ���ѡ���ȼ����ƶ���ת����ʱ�� */
    VOS_UINT16                              usArfcn;
    VOS_UINT16                              enBandInd;          /* Ƶ��ָʾ */
    VOS_INT16                               sRssi;              /* ����1/8 */
    VOS_UINT16                              usNcc;
    VOS_UINT16                              usBcc;
    VOS_INT16                               sSValue;             /*����õ���SPֵ*/
    VOS_INT16                               sQRxLevMin;          /*����SPֵ�õ�����ͽ����ƽ*/
    VOS_INT16                               sPCompensation;      /* ���� P-Max �� UE Max Tx power ��������� PCompensation */
    VOS_INT16                               sThreshLowP;         /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                               sThreshHighP;        /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
}LRRC_APP_GERAN_SIGNLE_FREQ_MEAS_RESULT_STRU;

typedef struct
{
    VOS_UINT16                                    usArfcnNum;
    VOS_INT16                                     sSNonIntraSearchP;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrp��ֵ���� */
    VOS_INT16                                     sSNonIntraSearchQ;   /* ����ͬ���ȼ��͵����ȼ���Ƶ������rsrq��ֵ���� */
    VOS_INT16                                     sServValue;          /*����õ���SPֵ*/
    VOS_INT16                                     sServqual;           /*����õ���SQֵ*/
    VOS_INT16                                     sThreshServingLowP;  /* ��Ե����ȼ�С����ѡ�ķ���С���������� */
    VOS_INT16                                     sThreshServingLowQ;  /* ��ѡ����ʱʹ��R9���� */
    LRRC_APP_GERAN_SIGNLE_FREQ_MEAS_RESULT_STRU   astGeranFreqMesRslt[LRRC_GURRC_GERAN_ARFCN_MAX_NUM];
}LRRC_APP_IDLE_GERAN_INFO_STRU;


/*****************************************************************************
 �ṹ��    : LRRC_APP_CELL_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usCellId;
    VOS_UINT16                          usFreqInfo;
    VOS_INT16                           sRsrp;              /* RSRP����ֵ */
    VOS_INT16                           sRsrq;              /* RSRQ����ֵ */
    VOS_INT16                           sRssi;              /* RSSI����ֵ */
} LRRC_APP_CELL_MEAS_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UTRAN_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usArfcn;
    VOS_UINT16                          usPrimaryCode;
    VOS_INT16                           sRscp;               /* ����1/8 */
    VOS_INT16                           sEcN0;               /* ����1/8 */
    VOS_INT16                           sRssi;               /* ����1/8 */
    VOS_UINT16                          UtranType;
} LRRC_APP_UTRAN_CELL_RSLT_STRU;
/*****************************************************************************
 ö����     :LRRC_GRR_BANDINDICATOR_ENUM
 Э�����  :
 ASN.1����   :
 ö��˵�� : 2GС��Ƶ��ָʾ
*****************************************************************************/

typedef VOS_UINT16    LRRC_APP_GERAN_BANDIND_ENUM_UINT16;

/*****************************************************************************
 ö����    : RRC_APP_PROTOCOL_STATE
 Э�����  :
 ASN.1���� :
 ö��˵��  : RRCЭ��״̬����
*****************************************************************************/

typedef VOS_UINT16 RRC_APP_PROTOCOL_STATE_ENUM_UINT16;

/*****************************************************************************
 ö����     :LRRC_GRR_BANDINDICATOR_ENUM
 Э�����  :
 ASN.1����   :
 ö��˵�� : 2GС��Ƶ��ָʾ
*****************************************************************************/

typedef VOS_UINT16    LRRC_GRR_BANDINDICATOR_ENUM_UINT16;

/*****************************************************************************
 �ṹ��    : LRRC_APP_UTRAN_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usArfcn;
    LRRC_APP_GERAN_BANDIND_ENUM_UINT16  enBandInd;          /* Ƶ��ָʾ */
    VOS_INT16                           sRssi;              /* ����1/8 */
} LRRC_APP_GERAN_CELL_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_SERV_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    RRC_APP_PROTOCOL_STATE_ENUM_UINT16 enState;             /* ��ǰЭ��״̬ */
    LRRC_APP_CELL_MEAS_RSLT_STRU       stServCellRslt;      /* ����С���ϱ���� */
} LRRC_APP_SERV_MEAS_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_SCELL_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CELL_MEAS_RSLT_STRU       astSCellRslt[LRRC_SCELL_MAX_NUM];          /* SCell�ϱ���� */
} LRRC_APP_SCELL_MEAS_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_INTRA_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CELL_MEAS_RSLT_STRU       astIntraCellRslt[LRRC_INTRA_CELL_MAX_NUM]; /* ͬƵС���ϱ���� */
} LRRC_APP_INTRA_MEAS_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_CELL_MEAS_RSLT_STRU       astInterCellRslt[LRRC_INTER_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_INTER_MEAS_RSLT_STRU;
/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_UTRAN_CELL_RSLT_STRU       astUtranCellRslt[LRRC_UTRAN_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_UTRAN_MEAS_RSLT_STRU;
/*****************************************************************************
 �ṹ��    : LRRC_APP_INTER_MEAS_RSLT_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : Э��ջ��APP��Ľӿ���Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    LRRC_APP_GERAN_CELL_RSLT_STRU       astGeranCellRslt[LRRC_GERAN_CELL_MAX_NUM]; /* ��ƵС���ϱ���� */
} LRRC_APP_GERAN_MEAS_RSLT_STRU;

/* add for Conn Meas Filter Optimize begin */
/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_INFO_IND_TYPE_ENUM
 �ṹ˵��  : LRRC�ϱ��˲����������
*****************************************************************************/

typedef VOS_UINT32 LRRC_CONN_MEAS_FILTER_INFO_IND_TYPE_ENUM_UNIT32;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_EUTRA_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : LRRC EUTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16    usCellId;    /* С��id */
    VOS_INT16     sRsrp;       /* �˲����RSRP����ֵ */
    VOS_INT16     sRsrq;       /* �˲����RSRQ����ֵ */
    VOS_INT16     sRssi;       /* �˲����RSSI����ֵ */
    VOS_UINT32    ulMeasTimeInterval;    /* ���β����ϱ���� */
}LRRC_CONN_MEAS_FILTER_EUTRA_INFO_STRU;


/*****************************************************************************
 �ṹ��    :LRRC_GRR_BSIC_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :CELL BAIC Info��Э��36331 6.3.4
            usNcc(GSM Network Colour Code)  ������Χ:(0..7), 8 ��ʾ��Чֵ
            usBcc(GSM Base Station Colour Code)  ������Χ:(0..7) , 8 ��ʾ��Чֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                              usNcc;
    VOS_UINT16                                              usBcc;
}LRRC_GRR_BSIC_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_GERA_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : LRRC GERAN�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    PS_BOOL_ENUM_UINT8        enBsicVaild;    /* BSIC��Ч��־ */
    VOS_UINT8                 ucResv;         /* ����λ */
    VOS_INT16                 sRssi;          /* �˲����Rssi */
    VOS_UINT16                usArfcn;            /* Ƶ�� */
    LRRC_GRR_BANDINDICATOR_ENUM_UINT16     enBandInd;          /* Ƶ��ָʾ */
    LRRC_GRR_BSIC_INFO_STRU   stBsic;         /* BSIC��־��Чʱ��BSIC��Ϣ */
    VOS_UINT32                ulMeasTimeInterval;    /* ���β����ϱ���� */
}LRRC_CONN_MEAS_FILTER_GERA_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_UTRA_TYPE_ENUM
 �ṹ˵��  : LRRC�ϱ��˲����������
*****************************************************************************/

typedef VOS_UINT8 LRRC_CONN_MEAS_UTRA_TYPE_ENUM_UINT8;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_UTRA_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : LRRC FDD UTRA�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                usCellId;       /* С��id */
    LRRC_CONN_MEAS_UTRA_TYPE_ENUM_UINT8    enUtraType;    /* Utra������ */
    VOS_UINT8                 ucResv;     /* ����λ */
    VOS_INT16                 sRscp;          /* �˲����Rscp */
    VOS_INT16                 sEcN0;          /* �˲����EcN0 */
    VOS_UINT32                ulMeasTimeInterval;    /* ���β����ϱ���� */
}LRRC_CONN_MEAS_FILTER_UTRA_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_CONN_MEAS_FILTER_INFO_IND_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : LRRC�˲������Ϣ�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    LRRC_CONN_MEAS_FILTER_INFO_IND_TYPE_ENUM_UNIT32       enUnionStructSelChoice;/*_H2ASN_Skip*/
    union  /* _H2ASN_Skip */
    {      /* _H2ASN_Skip */
        LRRC_CONN_MEAS_FILTER_EUTRA_INFO_STRU        stEutraConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MEAS_FILTER_GERA_INFO_STRU        stGeraConnMeasFilterInfo;   /* _H2ASN_Skip */
        LRRC_CONN_MEAS_FILTER_UTRA_INFO_STRU        stUtraConnMeasFilterInfo;   /* _H2ASN_Skip */
    }u;  /* _H2ASN_Skip */
}LRRC_CONN_MEAS_FILTER_INFO_IND_STRU;
/* add for Conn Meas Filter Optimize end */

/*****************************************************************************
 �ṹ��    : APP_OM_MSG_REDF_STRU
 �ṹ˵��  : APP(��̨����)��OM��������Ϣ��(��APP_OM_MSG_STRU��ͬ)
*****************************************************************************/
typedef struct
{
     VOS_MSG_HEADER                     /*VOSͷ*/
     VOS_UINT32          ulMsgId;
     APP_MSG_HEADER                     /*APPͷ*/
     VOS_UINT8           aucPara[4];    /* �������� */
}APP_OM_MSG_REDF_STRU;



/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_SET_3HOUR_REQ_STRU
 �ṹ˵��  :��׮3Сʱ��ʱ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      ulMsgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      ulOpId;
    VOS_UINT32                      ulTimerLength;         /* 3Сʱ��ʱ��ʱ�� */
}APP_RRC_DEBUG_STUB_SET_3HOUR_REQ_STRU;
/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_HO_FAIL_REQ_STRU
 �ṹ˵��  :��׮�ؽ���
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      ulMsgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      ulOpId;
    VOS_UINT32                      ulHoFailFlag;         /* 1Ϊ��׮�ؽ�����0Ϊ����׮ */
}APP_RRC_DEBUG_STUB_HO_FAIL_REQ_STRU;
/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_REL_ALL_REQ_STRU
 �ṹ˵��  :��׮REL ALL
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      ulMsgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      ulOpId;
    VOS_UINT32                      ulRelAllCmd;            /* 1Ϊ��׮REL_ALL��0Ϊ����׮ */
}APP_RRC_DEBUG_STUB_REL_ALL_REQ_STRU;
/*****************************************************************************
 �ṹ��    :RRC_APP_DEBUG_STUB_CMD_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    APP_RESULT_ENUM_UINT32              enResult;
}RRC_APP_DEBUG_STUB_CMD_CNF_STRU;



/*****************************************************************************
 �ṹ��    :APP_RRC_DEBUG_STUB_ADD_BAD_CELL_REQ_STRU
 �ṹ˵��  :��׮��С���б�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                      ulMsgId;                /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                      ulOpId;
    VOS_UINT32                      ulAddBadCellFlag;         /* 1�ӻ�С����0Ϊɾ����С�� */
    VOS_UINT16                      usBand;
    VOS_UINT16                      usFreq;
    VOS_UINT16                      usPhyCellId;
    VOS_UINT16                      usCause;
    VOS_UINT32                      enExitFLow;
    VOS_UINT32                      ulRsv;
    VOS_UINT8                       ucRsv0;
    VOS_UINT8                       ucRsv1;
    VOS_UINT8                       ucRsv2;
    VOS_UINT8                       ucRsv3;
    VOS_INT16                       sRsrp;
    VOS_INT16                       sRsrq;    /* ��С������ʱ������ */
    VOS_UINT16                      usRsv2;
    VOS_UINT16                      usRsv3;
}APP_RRC_DEBUG_STUB_ADD_BAD_CELL_REQ_STRU;




/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :����С����ͬƵ��Ƶ������Ϣ
****************************************************************************/
typedef struct
{
    VOS_UINT32    ulMcc;         //��Чֵ0xFFFFFFFF
    VOS_UINT32    ulMnc;         //MCC-MNC����Чֵ0xFFFFFFFF
    VOS_UINT16    usArfcn;       //Ƶ��
    VOS_UINT16    usBand;        //Ƶ��
    VOS_UINT32    ulPCI;         //PCI
    VOS_UINT32    ulCGI;         //CGI����Чֵ0xFFFFFFFF
    VOS_UINT16    usTAC;         //TAC����Чֵ0xFFFF
    VOS_UINT16    usBW;          //BW����Чֵ0xFFFF
    VOS_INT16     sRsrp;
    VOS_INT16     sRssi;
    VOS_INT16     sRsrq;
    VOS_UINT8     ucReserved[6];
} PS_LTE_CELL_INFO_STRU;

/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ǿС����Ϣ����������С������������Ϣ�����֧��8������
****************************************************************************/
typedef struct
{
    VOS_UINT32    ulFlag;           //Ϊ��ǰ̨�������ݣ���ǰĬ��Ϊ1
    VOS_UINT16    usLength;         //���ȣ����ֽ�֮�����ݳ��ȣ����������ֽڣ�
    VOS_UINT8     ucUeState;        /*״̬*,��ЧֵΪ0xFF*/
    VOS_UINT8     ucReserved[5];    //Ԥ��
    VOS_UINT32    ulActiveCount;    //����С������
    VOS_UINT32    ulNbrCount;       //��������
    PS_LTE_CELL_INFO_STRU     astCellInfo[LRRC_ECELL_QRY_CELL_REPORT_NUM];
} PS_LTE_ECELL_INFO_STRU;

/*****************************************************************************
 �ṹ��    :CA_STATE_INFO
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :Scell�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32    ulCellId;    //С��ID
    VOS_UINT32    ulArfcn;     //���ȷ���Ƶ�㡢��Ƶ�ʣ�
} CA_STATE_INFO;

/*****************************************************************************
 �ṹ��    :LTE_CA_STATE_INFO_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :CA�����Ϣ
****************************************************************************/
typedef struct
{
    VOS_UINT32        ulFlag;        //Ĭ��Ϊ1
    VOS_UINT16        usLength;      //����
    VOS_UINT8         ucReserved[2];
    VOS_UINT32        ulCount;       //CA��Ϣ�ĸ���
    CA_STATE_INFO     stCAInfo[LRRC_ECELL_QRY_CELL_REPORT_NUM];      //CA������Ϣ
} LTE_CA_STATE_INFO_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ѯ��ǿС����Ϣ����������С������������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32         ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32         ulOpId;
    VOS_UINT8          aucReserved[4];     /* ���� */
}APP_RRC_QUERY_ECELL_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                         /*_H2ASN_Skip*/
    VOS_UINT32         ulMsgID;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32         ulOpId;
    VOS_UINT8          aucReserved[4];     /* ���� */
}APP_RRC_QUERY_SCELL_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_ECELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                               /*_H2ASN_Skip*/
    VOS_UINT32                 ulMsgId;          /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                 ulOpId;
    PS_LTE_ECELL_INFO_STRU     stECellInfo;   /*��ǿ��С��������С��������*/
}RRC_APP_QUERY_ECELL_INFO_CNF_STRU;

/*****************************************************************************
 �ṹ��    :RRC_APP_SCELL_INFO_CNF_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  :��ԭ��APP_RRC_SCELL_INFO_REQ_STRU���лظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                               /*_H2ASN_Skip*/
    VOS_UINT32                 ulMsgId;          /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                 ulOpId;
    LTE_CA_STATE_INFO_STRU     stSCellInfo;      /*CAС����Ϣ*/
}RRC_APP_QUERY_SCELL_INFO_CNF_STRU;






/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/










#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of AppRrcInterface.h */