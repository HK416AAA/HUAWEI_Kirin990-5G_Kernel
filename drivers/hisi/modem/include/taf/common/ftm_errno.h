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

#ifndef __FTM_ERRNO_H__
#define __FTM_ERRNO_H__

#define ERR_FTM_SUCCESS (0)
#define ERR_FTM_WAIT_ASYNC (1)
#define ERR_FTM_INVALID_ID (2)
#define ERR_FTM_NO_INITILIZATION (3)
#define ERR_FTM_NOT_FOUND (4)
#define ERR_FTM_FULL (7)
#define ERR_FTM_INSUFFICIENT_BUFFER (8)
#define ERR_FTM_CONSUMED (9)
#define ERR_FTM_CONTINUE (10)
#define ERR_FTM_TOO_SMALL_BUFFER (11)
#define ERR_FTM_NOT_FREEE_SPACE (12)
#define ERR_FTM_NO_MORE_DATA (13)
#define ERR_FTM_MALLOC_FAILUE (15)
#define ERR_FTM_NOT_ENOUGH_MEMORY (16)
#define ERR_FTM_INVALID_HEAP_ADDR (17)
#define ERR_FTM_SHUTDOWN (18)    /* δ�ܹػ� */
#define ERR_FTM_NOT_CONNECT (19) /* δ��ȷ����UE */
#define ERR_FTM_BUSY (20)        /* ϵͳæ����ʹ�ó�ͻ */

#define ERR_FTM_UNAVAILABLE (50)
#define ERR_FTM_TIMEOUT (51)
#define ERR_FTM_INIT_FAILURE (52)
#define ERR_FTM_IO_REGISTER_FAILURE (53)
#define ERR_FTM_NO_REGISTER (54)

#define ERR_FTM_FLASH_ERASE_ERROR (100)
#define ERR_FTM_FLASH_WRITE_ERROR (101)
#define ERR_FTM_FLASH_ALIGN_ERROR (102) /* sector aligned addresses */
#define ERR_FTM_FLASH_PROTECT_ERROR (103)
#define ERR_FTM_FLASH_BIN_LOAD_NEED_FIRST_BLOCK (104) /* while bin file load,req need first block */
#define ERR_FTM_DIAG_CMD_SIZE_INVALID (106)

#define ERR_FTM_CFG_RAM_SUCCESS (201)
#define ERR_FTM_CFG_PART_SUCCESS (202)
#define ERR_FTM_CFG_LOG_NOT_ALLOW (203)
#define ERR_FTM_DIAG_CYC_CMD_NOT_START (204)
#define ERR_FTM_DIAG_MSG_CFG_NOT_SET (205)
#define ERR_FTM_QUEUE_FULL (300)
#define ERR_FTM_QUEUE_NOT_EXIST (301)
#define ERR_FTM_INVALID_TASK_ID (302)
#define ERR_FTM_INVALID_TASK_QUEUE (303)

#define ERR_FTM_VCOM_WRITE_ERROR (401)

/* AT�����룬���Ժ��ַ�����Ӧ */
/* �ô������FTM�ڲ�������ͳһ���� */
#define ERR_FTM_NOT_EXIST (500) /* �ַ�����"δʵ��"   ��Undone�� */
#define ERR_FTM_UNKNOWN (501)   /* �ַ�����"δ֪����" ��unknown error�� */
/* �ַ�����"��������Ƿ����������ںϷ���Χ�ȴ���" "invalid parameters" */
#define ERR_FTM_INVALID_PARAMETER (502)
#define ERR_FTM_NV_NOT_SUPPORT_ID (520)    /* �ַ�����"NV������""NVIM Not Exist" */
#define ERR_FTM_NV_ERROR_READ (521)        /* �ַ�����"NV��ȡʧ��" "Read NVIM Failure" */
#define ERR_FTM_NV_NOT_SUPPORT_LENTH (522) /* �ַ�����"NVдʧ�ܣ����ȹ���""Write Error for Length Overflow" */
#define ERR_FTM_NV_BAD_BLOCK (523)         /* �ַ�����"NVдʧ�ܣ�Flash����" "Write Error for Flash Bad Block" */
#define ERR_FTM_NV_ERROR_WRITE (524)       /* �ַ�����"NVдʧ�ܣ���������" "Write Error for Unknown Reason" */
#define ERR_FTM_VCTCXO_OVER_HIGH (525)     /* �ַ�����"����VCTCXO��ѹ����" "Higher Voltage"	 */
#define ERR_FTM_UE_MODE_ERR (526)          /* �ַ�����"����ģʽ����(��������ģʽ)"	"UE Mode Error"	 */
#define ERR_FTM_NOT_SET_CHAN (527)         /* �ַ�����"û����������ŵ�""Not Set Appointed Channel"	 */
#define ERR_FTM_OPEN_RFTX_ERROR (528)      /* �ַ�����"�򿪷����ʧ��""Open TX Transmitter Failure"	 */
#define ERR_FTM_OPEN_RFRX_ERROR (529)      /* �ַ�����"�򿪽��ջ�ʧ��""Open RX Transmitter Failure"	 */
#define ERR_FTM_OPEN_CHAN_ERROR (530)      /* �ַ�����"ͨ����ʧ��""Open Channel Failure"	 */
#define ERR_FTM_CLOSE_CHAN_ERROR (531)     /* �ַ�����"ͨ���ر�ʧ��""Close Channel Failure"	 */
/* �ַ�����"��֧�֣����ͨ��2�ķ���ͨ����DD800��Ŀ����1T2Rģʽ��"	"Not Support" */
#define ERR_FTM_NOT_SUPPORT (532)
#define ERR_FTM_INVALID_OP (533)           /* �ַ�����"�Ƿ������������ȡ����NV��""Invalid Operation"	 */
#define ERR_FTM_ENTER_BAND_ERROR (534)     /* �ַ�����"����Ƶ����Ϣ�޷���Ӧ" "Band No Match"	 */
#define ERR_FTM_SET_CHAN_INFOR_ERROR (535) /* �ַ�����"�����ŵ���Ϣʧ��" "Set Channel Information Failure"	 */
/* �ַ�����"����Ƶ�κ��ŵ���Ϣ�޷����""Band And Channel Not Combinated"	 */
#define ERR_FTM_FRE_CHANI_NOTMATCH (536)
#define ERR_FTM_SET_TX_POWER_FAILUE (537)    /* �ַ�����"����TX�ķ��书��ʧ��""Set TX Transmitter Power Error"	 */
#define ERR_FTM_SET_PA_LEVEL_FAILUE (538)    /* �ַ�����"����PA��ʧ��""Set PA Level Failure"	 */
#define ERR_FTM_NOT_SET_CURRENT_CHAN (539)   /* �ַ�����"δ���õ�ǰ�ŵ�"	"Not Set Current Channel" */
#define ERR_FTM_NOT_ABLETO_RD_APC (540)      /* �ַ�����"�޷���ȡ��ǰ���õ�APCֵ"	"APC Value Can't Be Read" */
#define ERR_FTM_SET_APC_ERROR (541)          /* �ַ�����"����APC����ʧ��"	"Write APC Failure" */
#define ERR_FTM_RD_APC_ERROR (542)           /* �ַ�����"��ȡAPC����ʧ��"	"Read APC Failure" */
#define ERR_FTM_SET_LINA_ERROR (543)         /* �ַ�����"����LNAʧ��"	"Set AAGC Failure" */
#define ERR_FTM_NOT_OPEN_RXRX (544)          /* �ַ�����"û�д򿪽��ջ�"	"RX Transmitter Not Open" */
#define ERR_FTM_NOT_OPEN_RXTX (545)          /* �ַ�����"û�д򿪷����"	"TX Transmitter Not Open" */
#define ERR_FTM_NO_SIGNAL (546)              /* �ַ�����"���ź�������ⲻ���ź�" "No Signal"	 */
#define ERR_FTM_PHYNUM_LENGTH_ERROR (547)    /* �ַ�����"����ų��ȴ���"	"PHY Number Length Error" */
#define ERR_FTM_PHYNUM_INVALID (548)         /* �ַ�����"����Ų��Ϸ�" "Invalid PHY Number"	 */
#define ERR_FTM_PHYNUM_TYPE_ERROR (549)      /* �ַ�����"��������ʹ���""Invalid PHY Type" */
#define ERR_FTM_PLATINFOR_NOTABLE_TORD (550) /* �ַ�����"�޷���ȡƽ̨��Ϣ""Platform Information Can't Be Read"	 */
/* �ַ�����"��ʾд����ʱ�����ݱ���δ����, �޷�������������"	"Not unlock, write error" */
#define ERR_FTM_WRT_NOT_UNLOCK (551)
#define ERR_FTM_CALLNUM_OVER20 (552) /* �ַ�����"����ĺŶζ���20��"	"Card Number Morethan 20" */
/* �ַ�����"<mnc-digital-num>���볬����Χ" "<mnc-digital-num> Enter Error"	 */
#define ERR_FTM_MNC_ENTER_OVERFLOW (553)
#define ERR_FTM_RD_VER_FAILUE (554)              /* �ַ�����"�汾��Ϣ��ȡʧ��""Read VERSION Failure"	 */
#define ERR_FTM_OPERTION_ERROR (555)             /* �ַ�����"����ʧ��""Operation Failure" */
#define ERR_FTM_PWD_ERR (556)                    /* �ַ���: "�����������""Password Error" */
#define ERR_FTM_TIME_OUT (557)                   /* �ַ���: "��ʱ����" "Overtime" */
#define ERR_FTM_NO_MORE_MEMORY (558)             /* �ڴ治�� "No Memory" */
#define ERR_FTM_SIMM_LOCK (559)                  /* ������SIMLOCK����"Sim Lock Active" */
#define ERR_FTM_CLOSE_RFTX_ERROR (560)           /* �ַ�����"�رշ����ʧ��""Close TX Transmitter Failure"	 */
#define ERR_FTM_CLOSE_RFRX_ERROR (561)           /* �ַ�����"�رս��ջ�ʧ��""Close RX Transmitter Failure" */
#define ERR_FTM_NV_DATA_INVALID (562)            /* �ַ�����"��ȡ��NV�����ݷǷ�""NV DATA INVALID" */
#define ERR_FTM_AT_CHANNEL_CLOSE (563)           /* DIAG/MUX��ATͨ���ر� */
#define ERR_FTM_AT_CHANNEL_BUSY (564)            /* DIAG/MUX��ATͨ��BUSY */
#define ERR_FTM_OS_MSG_BODY_NULL (565)           /* ���յ�����Ϣ��ΪNULL(��Ӧ��ΪNULL) */
#define ERR_FTM_AT_ACK_URC_INVALID (566)         /* AT ACK or URC invalid, such as size is zero */
#define ERR_FTM_PS_START_FAILURE (567)           /* START PS FAILURE */
#define ERR_FTM_PS_STOP_FAILURE (568)            /* STOP PS FAILURE */
#define ERR_FTM_PS_SET_APN_FAILURE (569)         /* SET APN FAILURE */
#define ERR_FTM_PS_SET_AUTH_FAILURE (570)        /* SET AUTH FAILURE */
#define ERR_FTM_PS_SET_BEARER_TYPE_FAILURE (571) /* SET BEARER TYPE */

#define ERR_FTM_BNADSW_ERROR (572)        /* �ַ���: "BAND�л�����" */
#define ERR_FTM_TXWAVE_ERROR (573)        /* �ַ���: "���䲨�����ʹ���" */
#define ERR_FTM_AT_FW_TABLEID_EXIST (574) /* client id �Ѵ��� */
#define ERR_FTM_START_TIMER_FAIL (575)

/* add simm error number (���޿�/67193/2009-08-14) */
/* 600 ~ 699 should be used by simm */
#define ERR_FTM_SIMM_NOT_INIT (601)        /* SIMMδ��ʼ�� */
#define ERR_FTM_SIMM_WRONG_PARA (602)      /* ��Ϊ�������󣬺���ִ��ʧ�� */
#define ERR_FTM_SIMM_NOAPP (603)           /* ��Ϊû�п���Ӧ�ã�����ִ��ʧ�� */
#define ERR_FTM_SIMM_NO_MEM (604)          /* ��Ϊ��ʼ��û����ɣ�����ִ��ʧ�� */
#define ERR_FTM_SIMM_ALLOCMEM_FAILED (605) /* ��Ϊ�ڴ����ʧ�ܣ�����ִ��ʧ�� */
#define ERR_FTM_SIMM_SENDMSG_FAILED (606)  /* ��Ϊ��Ϣ����ʧ�ܣ�����ִ��ʧ�� */
#define ERR_FTM_SIMM_TIMEOUT (607)         /* ��ʱ,����ִ��ʧ�� */
#define ERR_FTM_SIMM_NO_CARD (608)         /* ������λʧ�� */
#define ERR_FTM_SIMM_SEM (609)             /* ��������ź������� */

#define ERR_FTM_SIMM_QUEUE_FULL (621)    /* �������� */
#define ERR_FTM_SIMM_QUEUE_EMPTY (622)   /* ����Ϊ�� */
#define ERR_FTM_SIMM_CMD_EXCEPTION (623) /* �����ʧ�� */
#define ERR_FTM_SIMM_CMD_TIMEOUT (624)   /* �����ʱ */

#define ERR_FTM_SIMM_DF_FCP_FAILED (630) /* DF�ļ�����ʧ�� */
#define ERR_FTM_SIMM_PIDO_FAILED (631)   /* PIDO����ʧ�� */
#define ERR_FTM_SIMM_FCP_FAILED (632)    /* EF�ļ�����ʧ�� */
#define ERR_FTM_SIMM_EFDIR_FAILED (632)  /* EF�ļ�����ʧ�� */

#define ERR_FTM_SIMM_NEED_PIN (640)  /* ��ҪPIN�� */
#define ERR_FTM_SIMM_NEED_PIN2 (641) /* ��ҪPIN2�� */
#define ERR_FTM_SIMM_NEED_PUK (642)  /* ��ҪPUK�� */
#define ERR_FTM_SIMM_NEED_PUK2 (643) /* ��ҪPUK2�� */

#define ERR_FTM_SIMM_AUTH_MAC_FAILURE (645)   /* ��ȨMAC���� */
#define ERR_FTM_SIMM_AUTH_SYNC_FAILURE (646)  /* ��ȨSYNC���� */
#define ERR_FTM_SIMM_AUTH_OTHER_FAILURE (647) /* ������Ȩ���� */
#define ERR_FTM_SIMM_OTHER_FAILURE (648)      /* ������Ȩ���� */

#define ERR_FTM_QUEUE_NOT_OPEN (1000)
#define ERR_FTM_QUEUE_UNAVAILABLE (1001)
#define ERR_FTM_QUEUE_SHUTDOWN (1002)

#define ERR_FTM_NV_ITEM_IS_EMPTY (1100)
#define ERR_FTM_NV_FLASH_FULL (1103)

/* ��ӦBSP������ */
#define ERR_FTM_NV_GOOD_BLOCK (1105)
#define ERR_FTM_NV_ERROR_INIT (1106)
#define ERR_FTM_NV_ERROR_ARGS (1107)
#define ERR_FTM_NV_ERROR_MALLOC (1108)
#define ERR_FTM_NV_ERROR_ERASE (1111)

/* ���������� */
#define ERR_FTM_SUM_ONLY_NVCOPY_OK (1200)
#define ERR_FTM_SUM_BACK_OK (1201)         /* ���˳ɹ� */
#define ERR_FTM_SUM_BACK_FAIL (1202)       /* ����ʧ�� */
#define ERR_FTM_SUM_BACK_NO_VERSION (1203) /* û�а汾�ɻ��� */
#define ERR_FTM_SUM_UPDATE_RESET_FAILUE (1204)

#define ERR_FTM_NVIM_BACKUP_FILE_FAIL (2001)
#define ERR_FTM_NVIM_BACKUP_DATA_FAIL (2002)
#define ERR_FTM_NVIM_WRITE_FILE_FAIL (2003)
#define ERR_FTM_NVIM_UPDATA_DATA_FAIL (2004)
#define ERR_FTM_NVIM_UPDATA_FILE_FAIL (2005)
#define ERR_FTM_NVIM_SECTION_NOT_SUPPORT_UPDATA 2006
#define ERR_FTM_NVIM_NOT_SUPPORT_WRITE 2007
#define ERR_FTM_NVIM_NEED_BACK_ERR 2008

#define FTM_SD_OPEN_FILE_ERROR (2200)
#define FTM_SD_WRITE_FILE_ERROR (2201)
#define FTM_SD_SEEK_FILE_ERROR (2202)
#define FTM_SD_GET_STATE_ERROR (2203)
#define FTM_SD_CREAT_DIR_ERROR (2204)

#define ERR_FTM_CDM_BEGIN (0x3000) /* CDM ERR NO. START */

#define ERR_FTM_CDM_ACPU_CODER_SRC_FAIL (ERR_FTM_CDM_BEGIN + 1)

#define ERR_FTM_CDM_END (0x3100) /* CDM ERR NO. END */

/* DIAG����������ظ��Ĵ����� 0x4000-0x4fff */
#define ERR_FTM_DIAG_ERROR_BEGIN (0x4000)
#define ERR_FTM_DIAG_ACORE_ERROR (ERR_FTM_DIAG_ERROR_BEGIN + 1) /* A�˴���ʧ�� */
#define ERR_FTM_DIAG_CCORE_ERROR (ERR_FTM_DIAG_ERROR_BEGIN + 2) /* C�˴���ʧ�� */
#define ERR_FTM_DIAG_INVALID_CMD (ERR_FTM_DIAG_ERROR_BEGIN + 3) /* ����ʶ���������� */

#define ERR_FTM_FAILURE (0XFFFFFFFFU)
#define ERR_FTM_SYSTEM (0XFFFFFFFFU)

/* The definition of error number which result in reboot */
enum {
    FTM_REBOOT_FTM_ERR = 0xb0000000,
    FTM_REBOOT_ID_BUTT = 0xbfffffff
};

#endif /* __FTM_ERRNO_H__ */
