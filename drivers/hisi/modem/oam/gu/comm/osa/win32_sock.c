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
 * 版权所有（c）华为技术有限公司 2012-2019
 * 功能描述: OSA win32操作系统上socket功能实现
 * 生成日期: 2006年10月3日
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "stdio.h"
#include "v_typdef.h"
#include "v_sock.h"

#if (VOS_WIN32 == VOS_OS_VER)

#define THIS_FILE_ID PS_FILE_ID_V_WIN32_SOCK_C

#ifdef LLT_OS_WIN

/*
 * Description: creates a socket that is bound to a specific service provider      :
 */
VOS_SOCKET VOS_Socket(VOS_INT32 af, VOS_INT32 type, VOS_INT32 protocol)
{
    VOS_SOCKET vsRet;
    SOCKET     soRet;

    soRet = socket((VOS_INT)af, (VOS_INT)type, (VOS_INT)protocol);
    if (INVALID_SOCKET == soRet) {
        vsRet = VOS_NULL_LONG;
    } else {
        vsRet = (VOS_SOCKET)soRet;
    }

    return vsRet;
}

/*
 * Description: associates a local address with a socket
 */
VOS_UINT32 VOS_Bind(VOS_SOCKET s, struct sockaddr *paddr, VOS_INT32 addrlen)
{
    if (0 == bind((SOCKET)s, paddr, (VOS_INT)addrlen)) {
        return VOS_OK;
    } else {
        return VOS_NULL_LONG;
    }
}

/*
 * Description: determines the status of one or more sockets, waiting if necessary,
 * to perform synchronous I/O.
 */
VOS_UINT32 VOS_Select(VOS_INT32 width, struct fd_set *preadfds, struct fd_set *pwritefds, struct fd_set *pexceptfds,
                      struct timeval *ptimeout)
{
    int returnValue;

    returnValue = select((VOS_INT)width, preadfds, pwritefds, pexceptfds, ptimeout);
    if (SOCKET_ERROR == returnValue) {
        return VOS_NULL_LONG;
    } else {
        return (VOS_UINT32)returnValue;
    }
}

/*
 * Description: accepts an incoming connection attempt on a socket
 */
VOS_SOCKET VOS_Accept(VOS_SOCKET s, struct sockaddr *paddr, VOS_INT32 *paddrlen)
{
    SOCKET soRet;

    soRet = accept((SOCKET)(s), paddr, (VOS_INT *)paddrlen);
    if (INVALID_SOCKET == soRet) {
        return VOS_NULL_LONG;
    } else {
        return (VOS_SOCKET)soRet;
    }
}

/*
 * Description: receives data from a connected socket Calls
 */
VOS_UINT32 VOS_Recv(VOS_SOCKET s, VOS_CHAR *buf, VOS_INT32 len, VOS_INT32 flags)
{
    int returnValue;

    returnValue = recv((SOCKET)s, buf, (VOS_INT)len, (VOS_INT)flags);
    if (SOCKET_ERROR == returnValue) {
        return VOS_NULL_LONG;
    } else {
        return (VOS_UINT32)returnValue;
    }
}

/*
 * Description: closes an existing socket
 */
VOS_UINT32 VOS_CloseSocket(VOS_SOCKET s)
{
    if (0 == closesocket((SOCKET)s)) {
        return VOS_OK;
    } else {
        return VOS_NULL_LONG;
    }
}

/*
 * Description: sends data on a connected socket
 */
VOS_UINT32 VOS_Send(VOS_SOCKET s, VOS_CHAR *buf, VOS_INT32 len, VOS_INT32 flags)
{
    int returnValue;

    returnValue = send((SOCKET)s, buf, (VOS_INT)len, (VOS_INT)flags);
    if (SOCKET_ERROR == returnValue) {
        return VOS_NULL_LONG;
    } else {
        return (VOS_UINT32)returnValue;
    }
}

/*
 * Description: places a socket a state where it is listening for an incoming connection
 */
VOS_UINT32 VOS_Listen(VOS_SOCKET s, VOS_INT32 backlog)
{
    if (0 == listen((SOCKET)s, (VOS_INT)backlog)) {
        return VOS_OK;
    } else {
        return VOS_NULL_LONG;
    }
}

/*
 *  Description: controls the I/O mode of a socket
 */
VOS_UINT32 VOS_IoctlSocket(VOS_SOCKET s, VOS_INT32 cmd, VOS_INT32 *parg)
{
    if (0 == ioctlsocket((SOCKET)s, (VOS_INT32)cmd, (VOS_UINT32 *)parg)) {
        return VOS_OK;
    } else {
        return VOS_NULL_LONG;
    }
}

/*
 *  Description: Initialization for later use
 */
VOS_UINT32 VOS_SocketInit(VOS_VOID)
{
    return VOS_OK;
}

#endif /* end  #ifdef LLT_OS_WIN */
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
