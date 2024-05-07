一、基本原则
（1）上层软件include统一使用头文件mdrv.h，所有新增头文件，必须同时在mdrv.h里添加引用。
（2）上层软件编译的时候需要包含两个底软头文件目录，当前核独立的目录和公共目录，例如
    acore软件编译包含底软目录:  -Iacore/ -Icommon/ 
    ccore软件编译包含底软目录:  -Iccore/ -Icommon/
（3）mdrv_stub.h存放之前的桩函数，待删除，禁止增加 

二、文件路径
（1）头文件当前的目录结构，在modem\include\drv下，各核单独提供独立的头文件，共用的函数和数据结构放在comm目录下。A核、C核独立包含，外部代码不允许引用comm 目录下的头文件。      
    acore/      开源：a 核相关的头文件，每个头文件对应于一个模块
    ccore/      不开源：c 核相关的头文件，每个头文件对应于一个模块
    common/     开源：a, c 核共享的头文件
    crslibs/    不开源：cre对外提供的头文件
    dsp/        不开源：2/3/4G DSP提供的头文件
    hac/        不开源：l2hac的头文件
    hifi/       不开源：hifi的头文件
    hl1c/       不开源：hl1c的头文件
    ll1c/       不开源：ll1c的头文件
	
三、命名规则
头文件名称 ，mdrv_xxx.h
头文件定义，       
             #ifndef __MDRV_XXX_H__
             #define __MDRV_XXX_H__
	... ...
	#endif
	函数名
	结构体
	枚举
	宏 
以ipf头文件为例，命名规则如下：
    A核： acore/mdrv_ipf.h
       #ifndef __MDRV_IPF_H__
       #define __MDRV_IPF_H__
       #include "mdrv_ipf_common.h"
       ....
    C核： ccore/mdrv_ipf.h
       #ifndef __MDRV_IPF_H__
       #define __MDRV_IPF_H__
       #include "mdrv_ipf_common.h"
       ....
    comm: common/mdrv_ipf_comm.h
       #ifndef __MDRV_IPF_COMMON_H__
       #define __MDRV_IPF_COMMOM_H__
	   
四、文件头格式
（1）开源的源代码文件头采用双license声明（GPL&Huawei）。
开源头文件目录：
modem\include\drv\acore
modem\include\drv\common
文件头格式如下：
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
（2）非开源的源代码文件头部可以采用双License声明，也可以采用如下简单格式。
非开源头文件目录：
modem\include\drv\ccore
modem\include\drv\crslibs
modem\include\drv\dsp
modem\include\drv\hac
modem\include\drv\hifi
modem\include\drv\hl1c
modem\include\drv\ll1c
文件头格式如下：
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

五、基础数据类型统一
（1）对外接口统一使用原始数据类型，如int/unsigned int等。
（2）对外禁止使用bool类型。

六、安全红线
（1）文件中，禁止使用工号、姓名、问题单号等。

备注：
1）modem/include/drv下所有头文件的修改必须有DE组review才能合入。	
2）对外头文件禁止使用特性宏/版本宏来区分。
3）如上规则，若有异议，请上升至底软ST例会裁决。
