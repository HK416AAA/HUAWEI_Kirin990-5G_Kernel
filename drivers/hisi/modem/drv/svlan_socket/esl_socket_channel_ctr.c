
/*
 * 1 头文件包含
 */

#include "esl_socket_api.h"
#include "esl_socket_os_interface.h"
#include "esl_socket_channel_ctr.h"
/*
 * 2 宏定义
 */
/*
 * 3 结构体
 */
extern struct simsock_ctx g_simsock_ctx;

/*
 * 4 函数实现
 */

int simsock_chan_alloc(int sockfd, int opt)
{
    int ret = 0;
    unsigned int ch = 0;
    struct simsock_chan *chan = NULL;

    sim_mutex_lock(&g_simsock_ctx.simch_mutex);
    if (SIMSOCK_CHAN_MAX <= g_simsock_ctx.chan_used) {
        ret = SIMSOCK_FAIL;
        goto exit0;
    }

    ch = find_first_bit(&g_simsock_ctx.chan_map, g_simsock_ctx.chan_num);
    if (SIMSOCK_CHAN_MAX <= ch) {
        ret = SIMSOCK_FAIL;
        goto exit0;
    }

    g_simsock_ctx.chan_used++;
    g_simsock_ctx.chan_map &= ~(1 << ch);

    chan = &g_simsock_ctx.chan[ch];
    chan->opt = opt;
    chan->sockfd = sockfd;

    ret = ch;

exit0:
    sim_mutex_unlock(&g_simsock_ctx.simch_mutex);

    return ret;
}

int simsock_chan_free(int ch)
{
    struct simsock_chan *chan = NULL;

    if (ch > SIMSOCK_CHAN_MAX || ch < 0) {
        return SIMSOCK_FAIL;
    }

    chan = &g_simsock_ctx.chan[ch];

    sim_mutex_lock(&g_simsock_ctx.simch_mutex);
    g_simsock_ctx.chan_map |= 1 << ch;
    g_simsock_ctx.chan_used--;
    chan->opt = 0;
    chan->sockfd = 0;
    sim_mutex_unlock(&g_simsock_ctx.simch_mutex);

    return 0;
}

int simsock_chan_ctrl_init(void)
{
    unsigned int i;
    unsigned int value;

    for (i = 0; i < SIMSOCK_CHAN_MAX; i++) {
        sim_atomic_set(&g_simsock_ctx.chan[i].cnt, 0);
        osl_sem_init(0, &g_simsock_ctx.chan[i].sock_wait);
        g_simsock_ctx.chan[i].opt = 0;
        g_simsock_ctx.chan[i].sockfd = 0;

        value = simsock_readl((void *)(SIMSOCK_INTRAW(i) + g_simsock_ctx.simsock_regs));
        simsock_writel(value, (void *)(SIMSOCK_INTRAW(i) + g_simsock_ctx.simsock_regs));

        if (g_simsock_ctx.copy_mode) {
            g_simsock_ctx.chan[i].size = 64 * 1024;
            g_simsock_ctx.chan[i].buf = (void *)dma_alloc_coherent(g_simsock_ctx.dev, g_simsock_ctx.chan[i].size,
                                                                   &g_simsock_ctx.chan[i].buf_phy, GFP_KERNEL);
            if (!g_simsock_ctx.chan[i].buf) {
                return -ENOMEM;
            }
        }

        simsock_writel(INTMASKENABLE, (void *)(SIMSOCK_INTMASK(i) + g_simsock_ctx.simsock_regs));
    }

    g_simsock_ctx.simsock_iqrstatus = SIMSOCK_CHINTSTATS + g_simsock_ctx.simsock_regs;

    return 0;
}

void simsock_chan_ctrl_exit(void)
{
    return;
}
