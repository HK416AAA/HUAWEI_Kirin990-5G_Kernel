/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef _CSHELL_LOGGER_H_
#define _CSHELL_LOGGER_H_

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>

struct logger_entry {
    __u16 len;   /* length of the payload */
    __u16 __pad; /* no matter what, we get 2 bytes of padding */
    __s32 pid;   /* generating process's pid */
    __s32 tid;   /* generating process's tid */
    __s32 sec;   /* seconds since Epoch */
    __s32 nsec;  /* nanoseconds */
    char msg[0]; /* the entry's payload */
};

struct logger_log_tag {
    unsigned char *tag_save_buff;
    struct mutex mutex;
};
struct logger_log {
    unsigned char *buffer;    /* the ring buffer itself */
    struct miscdevice misc;   /* misc device representing the log */
    wait_queue_head_t wq;     /* wait queue for readers */
    struct list_head readers; /* this log's readers */
    struct mutex mutex;       /* mutex protecting buffer */
    size_t w_off;             /* current write head offset */
    size_t head;              /* new readers start here */
    size_t size;              /* size of the log */
};
/*
 * struct logger_reader - a logging device open for reading
 * This object lives from open to release, so we don't need additional
 * reference counting. The structure is protected by log->mutex.
 */
struct logger_reader {
    struct logger_log *log; /* associated log */
    struct list_head list;  /* entry in logger_log's list */
    size_t r_off;           /* current read head offset */
};

#define LOGGER_ENTRY_MAX_LEN (512 + sizeof(struct logger_entry)) /* (4*1024) */
#define LOGGER_ENTRY_MAX_PAYLOAD (LOGGER_ENTRY_MAX_LEN - sizeof(struct logger_entry))

int a_cshell_write(int log_left, void *in_buff);
#endif
