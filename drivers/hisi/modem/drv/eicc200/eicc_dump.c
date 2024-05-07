/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include "eicc_platform.h"
#include "eicc_dts.h"
#include "eicc_device.h"
#include "eicc_driver.h"
#include "eicc_core.h"
#include "eicc_pmsr.h"
#include "eicc_dump.h"

#if EICC_FEATURE_DUMP_SUPPORT
/* 本来想用EICC，没有I，用1代替 */
#define EICC_DUMP_MAGIC 0x0000E1CC
#define EICC_DUMP_VERSION 0x1
struct eicc_dump_head {
    u32 magic;
    u32 version;
    u32 reserved;
    u32 len;
};

#define EICC_DUMP_PMSR 0x0001
#define EICC_DUMP_PMSR_VER 0x0001
#define EICC_DUMP_HWREG 0x0002
#define EICC_DUMP_HWREG_VER 0x0001
#define EICC_DUMP_CHNS 0x0003
#define EICC_DUMP_CHNS_VER 0x0001
struct eicc_dump_tlv {
    u32 modid; /* this is type[tlv:t] */
    u32 version;
    u32 reserved;
    u32 len; /* this is len[tlv:l] */
};           /* [tlv:v] is after this struct */

struct eicc_dump_devinfo {
    u32 devid;
    u32 cpu_cnt;
    u32 pipepair_cnt;
};

static u32 eicc_device_dump(struct eicc_device *pdev, u8 *buf, u32 len)
{
    struct eicc_dump_devinfo devinfo;
    struct eicc_dump_devinfo *pdevinfo = &devinfo;

    u32 used_len = sizeof(devinfo);
    if (len < used_len) {
        return 0;
    }
    pdevinfo->devid = pdev->id;
    pdevinfo->cpu_cnt = EICC_CPU_ID_MAX;
    pdevinfo->pipepair_cnt = pdev->pipepair_cnt;
    if (memcpy_s(buf, len, &devinfo, sizeof(devinfo))) {
        return 0;
    }
    return used_len;
}

struct eicc_dump_cpuregsinfo {
    u32 cpuid;
    u32 omask0;
    u32 omask1;
    u32 imask0;
    u32 imask1;
    u32 ostat0;
    u32 ostat1;
    u32 istat0;
    u32 istat1;
};

static u32 eicc_cpu_regs_dump(struct eicc_device *pdev, u32 cpuid, u8 *buf, u32 len)
{
    struct eicc_dump_cpuregsinfo cpu_regs;
    struct eicc_dump_cpuregsinfo *pcpu_regs = &cpu_regs;
    char *base_addr = (char *)pdev->base_va;

    u32 used_len = sizeof(cpu_regs);
    if (len < used_len) {
        return 0;
    }
    pcpu_regs->cpuid = cpuid;
    pcpu_regs->omask0 = readl((void *)(base_addr + EICC_CORE_OPIPE_INT_MASK0(cpuid)));
    pcpu_regs->omask1 = readl((void *)(base_addr + EICC_CORE_OPIPE_INT_MASK1(cpuid)));
    pcpu_regs->imask0 = readl((void *)(base_addr + EICC_CORE_IPIPE_INT_MASK0(cpuid)));
    pcpu_regs->imask1 = readl((void *)(base_addr + EICC_CORE_IPIPE_INT_MASK1(cpuid)));

    pcpu_regs->ostat0 = readl((void *)(base_addr + EICC_CORE_OPIPE_INT_STAT0(cpuid)));
    pcpu_regs->ostat1 = readl((void *)(base_addr + EICC_CORE_OPIPE_INT_STAT1(cpuid)));
    pcpu_regs->istat0 = readl((void *)(base_addr + EICC_CORE_IPIPE_INT_STAT0(cpuid)));
    pcpu_regs->istat1 = readl((void *)(base_addr + EICC_CORE_IPIPE_INT_STAT1(cpuid)));

    if (memcpy_s(buf, len, &cpu_regs, sizeof(cpu_regs))) {
        return 0;
    }
    return used_len;
}

struct eicc_dump_piperegsinfo {
    u32 pipeid;
    u32 owptr; /* 'OPIPE_WPTR */
    u32 orptr; /* 'OPIPE_RPTR */
    u32 omask; /* 'OPIPE_INT_MASK */
    u32 oraw;  /* 'OPIPE_INT_RAW */
    u32 octrl; /* 'OPIPE_CTRL */
    u32 iwptr; /* 'IPIPE_WPTR */
    u32 irptr; /* 'IPIPE_RPTR */
    u32 irmte; /* 'IPIPE_REMOTE_CTRL */
    u32 imask; /* 'IPIPE_INT_MASK */
    u32 iraw;  /* 'IPIPE_INT_RAW */
    u32 ictrl; /* 'IPIPE_CTRL */
};

static u32 eicc_pipe_regs_dump(struct eicc_device *pdev, u32 pipeid, u8 *buf, u32 len)
{
    struct eicc_dump_piperegsinfo pipe_regs;
    struct eicc_dump_piperegsinfo *ppipe_regs = &pipe_regs;
    char *base_addr = (char *)pdev->base_va;

    u32 used_len = sizeof(pipe_regs);
    if (len < used_len) {
        return 0;
    }
    ppipe_regs->pipeid = pipeid;
    ppipe_regs->owptr = readl((void *)(base_addr + EICC_OPIPE_WPTR(pipeid)));
    ppipe_regs->orptr = readl((void *)(base_addr + EICC_OPIPE_RPTR(pipeid)));
    ppipe_regs->omask = readl((void *)(base_addr + EICC_OPIPE_INT_MASK(pipeid)));
    ppipe_regs->oraw = readl((void *)(base_addr + EICC_OPIPE_INT_RAW(pipeid)));
    ppipe_regs->octrl = readl((void *)(base_addr + EICC_OPIPE_CTRL(pipeid)));

    ppipe_regs->iwptr = readl((void *)(base_addr + EICC_IPIPE_WPTR(pipeid)));
    ppipe_regs->irptr = readl((void *)(base_addr + EICC_IPIPE_RPTR(pipeid)));
    ppipe_regs->irmte = readl((void *)(base_addr + EICC_IPIPE_REMOTE_CTRL(pipeid)));
    ppipe_regs->imask = readl((void *)(base_addr + EICC_IPIPE_INT_MASK(pipeid)));
    ppipe_regs->iraw = readl((void *)(base_addr + EICC_IPIPE_INT_RAW(pipeid)));
    ppipe_regs->ictrl = readl((void *)(base_addr + EICC_IPIPE_CTRL(pipeid)));

    if (memcpy_s(buf, len, &pipe_regs, sizeof(pipe_regs))) {
        return 0;
    }
    return used_len;
}

u32 eicc_hwregs_dump_save(u8 *buf, u32 len)
{
    u32 devid, cpuid, pipeid;
    u32 ret_len, used_len;
    struct eicc_device *pdev = NULL;

    used_len = 0;
    for (devid = 0; devid < EICC_DEVICE_NUM_MAX; devid++) {
        pdev = eicc_device_get_fast(devid);
        if (pdev == NULL) {
            continue;
        }
        if (pdev->ctrl_level != EICC_DEV_CONTROL_LEVEL_HOST && pdev->ctrl_level != EICC_DEV_CONTROL_LEVEL_GUEST) {
            continue;
        }
        if (len < used_len) {
            return used_len;
        }
        ret_len = eicc_device_dump(pdev, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
            return used_len;
        }
        used_len += ret_len;
        for (cpuid = 0; cpuid < EICC_CPU_ID_MAX; cpuid++) {
            if (len < used_len) {
                return used_len;
            }
            ret_len = eicc_cpu_regs_dump(pdev, cpuid, buf + used_len, len - used_len);
            if (ret_len == 0) {
                /* dump空间不够 没记录完 */
                eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
                return used_len;
            }
            used_len += ret_len;
        }

        for (pipeid = 0; pipeid < pdev->pipepair_cnt; pipeid++) {
            if (len < used_len) {
                return used_len;
            }
            ret_len = eicc_pipe_regs_dump(pdev, pipeid, buf + used_len, len - used_len);
            if (ret_len == 0) {
                /* dump空间不够 没记录完 */
                eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
                return used_len;
            }
            used_len += ret_len;
        }
    }

    return used_len;
}

struct eicc_dump_chnsinfo {
    u32 ldrvchn_id;
    u32 type;
    u32 wptr; /* 对于ipipe，这个dump vptr指向的位置 */
    u32 rptr; /* 对于opipe，这个dump vptr指向的位置 */
    u32 dym_flags;
    u32 const_flags;
};

static u32 eicc_chn_dump(struct eicc_device *pdev, struct eicc_channel *pchannel, u8 *buf, u32 len)
{
    struct eicc_dump_chnsinfo chninfo;
    struct eicc_dump_chnsinfo *pchninfo = &chninfo;

    u32 used_len = sizeof(chninfo);
    if (len < used_len) {
        return 0;
    }
    pchninfo->ldrvchn_id = pchannel->ldrvchn_id;
    pchninfo->type = pchannel->type;
    if (pchannel->state == (u32)EICC_CHN_INIT) {
        pchninfo->wptr = 0xDEADBEEF;
        pchninfo->rptr = 0xDEADBEEF;
    } else {
        if (isMsgSendChn(pchannel)) {
            pchninfo->wptr = pchannel->opipe_cfg->wptr;
            pchninfo->rptr = *(pchannel->opipe_cfg->ptr_vaddr);
        } else {
            pchninfo->wptr = *(pchannel->ipipe_cfg->ptr_vaddr);
            pchninfo->rptr = pchannel->ipipe_cfg->rptr;
        }
    }

    pchninfo->dym_flags = pchannel->dym_flags;
    pchninfo->const_flags = pchannel->const_flags;
    if (memcpy_s(buf, len, &chninfo, sizeof(chninfo))) {
        return 0;
    }
    return used_len;
}

u32 eicc_chns_dump_save(u8 *buf, u32 len)
{
    u32 idx;
    u32 ret_len, used_len;
    struct eicc_device *pdev = NULL;
    struct eicc_channel *pchannel = NULL;

    idx = 0;
    used_len = 0;
    while (eicc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (len < used_len) {
            return used_len;
        }
        /* 没有打开的通道没必要记录 */
        if (pchannel->state == (u32)EICC_CHN_INIT) {
            continue;
        }
        ret_len = eicc_chn_dump(pdev, pchannel, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
            return used_len;
        }
        used_len += ret_len;
    };

    return used_len;
}

static u32 eicc_do_pmsr_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct eicc_dump_tlv *tlv = (struct eicc_dump_tlv *)buf;
    u32 used_len = sizeof(struct eicc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = EICC_DUMP_PMSR;
    tlv->version = EICC_DUMP_PMSR_VER;
    ret_len = eicc_pmsr_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

static u32 eicc_do_hwregs_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct eicc_dump_tlv *tlv = (struct eicc_dump_tlv *)buf;
    u32 used_len = sizeof(struct eicc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = EICC_DUMP_HWREG;
    tlv->version = EICC_DUMP_HWREG_VER;
    ret_len = eicc_hwregs_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

static u32 eicc_do_chns_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct eicc_dump_tlv *tlv = (struct eicc_dump_tlv *)buf;
    u32 used_len = sizeof(struct eicc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = EICC_DUMP_CHNS;
    tlv->version = EICC_DUMP_CHNS_VER;
    ret_len = eicc_chns_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        eicc_print_error("eicc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

/* eicc dump使用tlv结构，便于脚本解析 */
void eicc_do_dump(u8 *buf, u32 len)
{
    u32 used_len;
    struct eicc_dump_head *dump_hdr = (struct eicc_dump_head *)buf;
    /* 写入magic num 和 版本号 长度， 占用16字节，方便对齐分析 */
    if (len < sizeof(struct eicc_dump_head)) {
        return;
    }

    dump_hdr->magic = EICC_DUMP_MAGIC;
    dump_hdr->version = EICC_DUMP_VERSION;
    used_len = 0;
    dump_hdr->len = used_len;
    len -= sizeof(struct eicc_dump_head);
    buf += sizeof(struct eicc_dump_head);

    if (len - used_len > 0) {
        used_len += eicc_do_pmsr_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }
    if (len - used_len > 0) {
        used_len += eicc_do_hwregs_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }

    if (len - used_len > 0) {
        used_len += eicc_do_chns_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }
    return;
}

#else
void eicc_do_dump(u8 *buf, u32 len)
{
    return;
}
#endif
