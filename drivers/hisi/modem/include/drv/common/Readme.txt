һ������ԭ��
��1���ϲ����includeͳһʹ��ͷ�ļ�mdrv.h����������ͷ�ļ�������ͬʱ��mdrv.h��������á�
��2���ϲ���������ʱ����Ҫ������������ͷ�ļ�Ŀ¼����ǰ�˶�����Ŀ¼�͹���Ŀ¼������
    acore��������������Ŀ¼:  -Iacore/ -Icommon/ 
    ccore��������������Ŀ¼:  -Iccore/ -Icommon/
��3��mdrv_stub.h���֮ǰ��׮��������ɾ������ֹ���� 

�����ļ�·��
��1��ͷ�ļ���ǰ��Ŀ¼�ṹ����modem\include\drv�£����˵����ṩ������ͷ�ļ������õĺ��������ݽṹ����commĿ¼�¡�A�ˡ�C�˶����������ⲿ���벻��������comm Ŀ¼�µ�ͷ�ļ���      
    acore/      ��Դ��a ����ص�ͷ�ļ���ÿ��ͷ�ļ���Ӧ��һ��ģ��
    ccore/      ����Դ��c ����ص�ͷ�ļ���ÿ��ͷ�ļ���Ӧ��һ��ģ��
    common/     ��Դ��a, c �˹����ͷ�ļ�
    crslibs/    ����Դ��cre�����ṩ��ͷ�ļ�
    dsp/        ����Դ��2/3/4G DSP�ṩ��ͷ�ļ�
    hac/        ����Դ��l2hac��ͷ�ļ�
    hifi/       ����Դ��hifi��ͷ�ļ�
    hl1c/       ����Դ��hl1c��ͷ�ļ�
    ll1c/       ����Դ��ll1c��ͷ�ļ�
	
������������
ͷ�ļ����� ��mdrv_xxx.h
ͷ�ļ����壬       
             #ifndef __MDRV_XXX_H__
             #define __MDRV_XXX_H__
	... ...
	#endif
	������
	�ṹ��
	ö��
	�� 
��ipfͷ�ļ�Ϊ���������������£�
    A�ˣ� acore/mdrv_ipf.h
       #ifndef __MDRV_IPF_H__
       #define __MDRV_IPF_H__
       #include "mdrv_ipf_common.h"
       ....
    C�ˣ� ccore/mdrv_ipf.h
       #ifndef __MDRV_IPF_H__
       #define __MDRV_IPF_H__
       #include "mdrv_ipf_common.h"
       ....
    comm: common/mdrv_ipf_comm.h
       #ifndef __MDRV_IPF_COMMON_H__
       #define __MDRV_IPF_COMMOM_H__
	   
�ġ��ļ�ͷ��ʽ
��1����Դ��Դ�����ļ�ͷ����˫license������GPL&Huawei����
��Դͷ�ļ�Ŀ¼��
modem\include\drv\acore
modem\include\drv\common
�ļ�ͷ��ʽ���£�
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
��2���ǿ�Դ��Դ�����ļ�ͷ�����Բ���˫License������Ҳ���Բ������¼򵥸�ʽ��
�ǿ�Դͷ�ļ�Ŀ¼��
modem\include\drv\ccore
modem\include\drv\crslibs
modem\include\drv\dsp
modem\include\drv\hac
modem\include\drv\hifi
modem\include\drv\hl1c
modem\include\drv\ll1c
�ļ�ͷ��ʽ���£�
/*
*
* Copyright (C), 2012-2018, Huawei Tech. Co., Ltd.
*
* File name: xxx.x
*
* Description:
*     xxx
*
* Version: v0.x
*
* History:
*    xxxx
*
*/

�塢������������ͳһ
��1������ӿ�ͳһʹ��ԭʼ�������ͣ���int/unsigned int�ȡ�
��2�������ֹʹ��bool���͡�

������ȫ����
��1���ļ��У���ֹʹ�ù��š����������ⵥ�ŵȡ�

��ע��
1��modem/include/drv������ͷ�ļ����޸ı�����DE��review���ܺ��롣	
2������ͷ�ļ���ֹʹ�����Ժ�/�汾�������֡�
3�����Ϲ����������飬������������ST����þ���
