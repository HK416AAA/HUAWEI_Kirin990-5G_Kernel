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
#ifndef __BSP_ECDC_H_
#define __BSP_ECDC_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*service_ops)(void *);

#define ECDC_ERROR (-1)
#define ECDC_OK (0)
#define ECDC_NOSYMBOL         (0xc0)
#define ECDC_TIMEOUT          (0xc1)


#define HISI_SERVICE_TYPE 0X48490000

//取service type后四位
typedef enum {
    ECDC_SERVICE_RFVERIFY = 0x0000,
    /* add registered service here */
    ECDC_SERVICE_LLTSHELL, /* lltshell */

    /* end of registered service */
    ECDC_SERVICE_MAX ,
}ecdc_service_type;

#define ECDC_LRCCPU_PANRPC_ICC_CHN_ID       (ICC_CHN_AC_PANRPC << 16 | ECDC_PANRPC_ID)
#define ECDC_NRCCPU_PANRPC_ICC_CHN_ID       (ICC_CHN_NRIFC << 16 | NRIFC_RECV_FUNC_ECDC)

typedef enum {
    ECDC_PACKET_REQUEST = 0,
    ECDC_PACKET_RESPONSE = 1,
}ecdc_packet_type;

#define ECDC_SERVICE_MAGIC_BITS 16
#define ECDC_SERVICE_TYPE(n) (unsigned int)(HISI_SERVICE_TYPE | (n & ((unsigned int)(1 << ECDC_SERVICE_MAGIC_BITS)-1)))
#define ECDC_SERVICE_ENUM_TYPE(n) (unsigned int)(n & (unsigned int)((1<<ECDC_SERVICE_MAGIC_BITS) - 1))
#define ECDC_PACKET_TYPE(n) (unsigned int)(n)

#define ECDC_OPERATIONID_LEN 24

enum operation_len_seq {
    ECDC_ACORE_OPERATE_LEN = 20,
    ECDC_LRCCPU_OPERATE_LEN,
    ECDC_NRCCPU_OPERATE_LEN,
    ECDC_HAC_OPERATE_LEN,
    ECDC_TSP_OPERATE_LEN
};

enum sendto_type{
    LRCCPU = 0,
    NRCCPU,
    TSPCPU,
};

/* the packet should be word aligned */
#pragma pack(push,4)
#define ECDC_PACKET_PLUS 8
/*lint --e{43}*/
struct ecdc_packet_req {
    unsigned int service_type;
    unsigned int data_len;
    unsigned int packet_type;
    unsigned int operation_id;
    unsigned int para_len;
    unsigned int para_data[0];
};
struct ecdc_packet_resp {
    unsigned int service_type;
    unsigned int data_len;
    unsigned int packet_type;
    unsigned int operation_id;
    int          status;
    unsigned int result_len;
    unsigned int result_data[0];
};
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
