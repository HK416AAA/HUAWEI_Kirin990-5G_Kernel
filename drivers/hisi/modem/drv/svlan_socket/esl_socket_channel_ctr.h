#ifndef _ESL_SOCKET_CHANNEL_CTR_H
#define _ESL_SOCKET_CHANNEL_CTR_H

#include "esl_socket_os_interface.h"

#define SIMSOCK_VERSION 0x0
#define SIMSOCK_HWPARAMS 0x4
#define SIMSOCK_CHINTSTATS 0x8

#define SIMSOCK_OP(ch) (0x10 + (ch)*60 + 0x0)
#define SIMSOCK_RESULT(ch) (0x10 + (ch)*60 + 0x4)
#define SIMSOCK_PARAM0(ch) (0x10 + (ch)*60 + 0x8)
#define SIMSOCK_PARAM1(ch) (0x10 + (ch)*60 + 0xc)
#define SIMSOCK_PARAM2(ch) (0x10 + (ch)*60 + 0x10)
#define SIMSOCK_PARAM3(ch) (0x10 + (ch)*60 + 0x14)
#define SIMSOCK_PARAM4(ch) (0x10 + (ch)*60 + 0x18)
#define SIMSOCK_PARAM5(ch) (0x10 + (ch)*60 + 0x1c)
#define SIMSOCK_PARAM6(ch) (0x10 + (ch)*60 + 0x20)
#define SIMSOCK_PARAM7(ch) (0x10 + (ch)*60 + 0x24)
#define SIMSOCK_PARAM8(ch) (0x10 + (ch)*60 + 0x28)
#define SIMSOCK_PARAM9(ch) (0x10 + (ch)*60 + 0x2C)
#define SIMSOCK_INTRAW(ch) (0x10 + (ch)*60 + 0x30)
#define SIMSOCK_INTMASK(ch) (0x10 + (ch)*60 + 0x34)
#define SIMSOCK_INTMSTATS(ch) (0x10 + (ch)*60 + 0x38)

#define INTMASKENABLE 0xffffffff

struct simsock_chan {
    sim_atomic_t cnt;
    int sockfd;
    int opt;
    osl_sem_id sock_wait;
    void *buf;
    sim_dma_addr_t buf_phy;
    unsigned long size;
    unsigned long len;
};

int simsock_chan_alloc(int sockfd, int opt);
int simsock_chan_free(int ch);

int simsock_chan_ctrl_init(void);
void simsock_chan_ctrl_exit(void);

#endif
