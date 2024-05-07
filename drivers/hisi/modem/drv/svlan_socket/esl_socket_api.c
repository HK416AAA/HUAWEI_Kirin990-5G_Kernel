
#include "product_config.h"

#ifdef FEATURE_SVLSOCKET

/*
 * 1 头文件包含
 */
#include "esl_socket_channel_ctr.h"
#include "esl_socket_api.h"
#include "securec.h"
#include <bsp_print.h>
#include <osl_types.h>
#include <bsp_version.h>
#include <osl_malloc.h>

/*
 * 2 宏定义
 */
#define HIGH32(a) ((unsigned long)(((a) >> 32) & 0xffffffff))
#define LOW32(a) ((unsigned long)((a)&0xffffffff))

#define SIMSOCK_OP_START (1 << 31)
#define INT_TIMEOVER 1
#define THIS_MODU mod_hisocket
#define SOCKET_DBG(format, arg...) do { \
    if (DISABLE_SOCKET_DEBUG == simsock_debug)     \
        bsp_info(format, ##arg);                   \
    else if (ENABLE_SOCKET_DEBUG == simsock_debug) \
        bsp_err(format, ##arg);                    \
} while (0)

enum copy_mode {
    map_single_mode = 0,
    alloc_mode,
    dirc_trans_mode
};

/*
 * 3 结构体
 */
enum simsock_cmd {
    simsock_cmd_socket = 0,
    simsock_cmd_select,
    simsock_cmd_getpeername,
    simsock_cmd_setsockopt,
    simsock_cmd_bind,
    simsock_cmd_listen,
    simsock_cmd_accept,
    simsock_cmd_connect,
    simsock_cmd_send,
    simsock_cmd_recv,
    simsock_cmd_sendto,
    simsock_cmd_recvfrom,
    simsock_cmd_shutdown,
    simsock_cmd_close,
    simsock_cmd_max
};

int simsock_debug = DISABLE_SOCKET_DEBUG;
struct simsock_dbg_table {
    // interface func count
    atomic_t sock;
    atomic_t bind;
    atomic_t listen;
    atomic_t accept_in;
    atomic_t accept_out;
    atomic_t select_in;
    atomic_t select_out;
    atomic_t send;
    atomic_t recv_in;
    atomic_t recv_out;
    atomic_t sendto;
    atomic_t getpeername;
    atomic_t shutdown;
    atomic_t close;
    // core func count
    atomic_t sockregfd;
    atomic_t sockunregfd;
    atomic_t sockgetfd;
    atomic_t sockputfd;
};
struct simsock_dbg_table simsockdbg;

struct simsock_ctx g_simsock_ctx;

#define hi_get_sockfd(fd) g_simsock_ctx.socks[fd].sockfd

static unsigned long long simsock_dma_mask = 0xffffffffULL;

struct g_simsock_dbg_t {
    int svlan_module_init_begin;
    int platform_ret;
    int svlan_module_init_end;
    int svlan_probe_begin;
    int svlan_probe_end;
};
struct g_simsock_dbg_t g_simsock_dbg = {0};
/*
 * 4 函数实现
 */
irqreturn_t sock_interrupt(int irq, void *dev)
{
    unsigned int status = 0;
    unsigned int value = 0;
    int i;

    status = simsock_readl(g_simsock_ctx.simsock_iqrstatus);

    for (i = 0; i < SIMSOCK_CHAN_MAX; i++) {
        if ((status & (1 << i))) {
            osl_sem_up(&g_simsock_ctx.chan[i].sock_wait);

            value = simsock_readl((void *)(SIMSOCK_INTRAW(i) + g_simsock_ctx.simsock_regs));
            simsock_writel(value, (void *)(SIMSOCK_INTRAW(i) + g_simsock_ctx.simsock_regs));
        }
    }

    return SIM_IRQ_HANDLED;
}

inline int simsock_chan_complete(unsigned int ch)
{
    unsigned int value;
    int timeout = 1000;

    do {
        value = simsock_readl(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs);
        if (0 == (value & (1 << 31))) {
            break;
        }

        sim_sleep(1);
    } while (timeout--);

    if (timeout <= 0) {
        return SIMSOCK_FAIL;
    }

    return SIMSOCK_OK;
}

inline int simsock_chan_interrupt_wait(unsigned int ch)
{
    int irq_test = 1;
    unsigned int ret;

    do {
        osl_sem_downtimeout(&g_simsock_ctx.chan[ch].sock_wait, INT_TIMEOVER);
        ret = simsock_readl((void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));
        if (!(ret >> 31)) {
            irq_test = 0;
        }
    } while (irq_test);
    return SIMSOCK_OK;
}

int simsock_create(int family, int type, int protocol)
{
    int ret = 0;
    int ch = 0;
    struct simsock_chan *chan = NULL;

    ret = simsock_chan_alloc(0, simsock_cmd_socket);
    if (ret < 0)
        return ret;

    ch = ret;
    chan = &g_simsock_ctx.chan[ch];

    simsock_writel(family, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(type, (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(protocol, (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_socket | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    ret = simsock_chan_complete(ch);
    if (ret) {
        return ret;
    }

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    chan->sockfd = ret;

    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_bind(int sockfd, struct sockaddr *addr, int addrlen)
{
    int ret = -1;
    int ch = -1;
    sim_dma_addr_t dma_phy;

    if (addr == NULL) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_bind);
    if (ret < 0)
        return ret;

    ch = ret;

    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    dma_phy = sim_dma_map_single(g_simsock_ctx.dev, addr, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(addrlen, (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));

    simsock_writel(simsock_cmd_bind | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    ret = simsock_chan_complete(ch);

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_phy, sizeof(struct sockaddr), SIM_DMA_FROM_DEVICE);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));

    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_listen(int sockfd, int backlog)
{
    int ret = 0;
    int ch = 0;

    ret = simsock_chan_alloc(sockfd, simsock_cmd_listen);
    if (ret < 0)
        return ret;
    ch = ret;
    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(backlog, (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_listen | SIMSOCK_OP_START, SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs);
    ret = simsock_chan_complete(ch);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_accept(int sockfd, struct sockaddr *upeer_sockaddr, int *upeer_addrlen)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_upaddr_phy;
    sim_dma_addr_t dma_addrlen_phy;

    if ((upeer_sockaddr == NULL) || (upeer_addrlen == NULL)) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_accept);
    if (ret < 0)
        return ret;
    ch = ret;

    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    dma_upaddr_phy = sim_dma_map_single(g_simsock_ctx.dev, upeer_sockaddr, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_upaddr_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_upaddr_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));
    dma_addrlen_phy = sim_dma_map_single(g_simsock_ctx.dev, upeer_addrlen, sizeof(int), SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_addrlen_phy), (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_addrlen_phy), (void *)(SIMSOCK_PARAM4(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_accept | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    simsock_chan_interrupt_wait(ch);

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_upaddr_phy, sizeof(struct sockaddr), SIM_DMA_FROM_DEVICE);
    sim_dma_unmap_single(g_simsock_ctx.dev, dma_addrlen_phy, sizeof(struct sockaddr), SIM_DMA_FROM_DEVICE);
    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_recv(int sockfd, void *ubuf, size_t size, int flags)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_ubuf_phy;

    if (ubuf == NULL) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_recv);
    if (ret < 0)
        return ret;
    ch = ret;

    dma_ubuf_phy = sim_dma_map_single(g_simsock_ctx.dev, ubuf, size, SIM_DMA_TO_DEVICE);
    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(LOW32(dma_ubuf_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_ubuf_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(size, (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(flags, (void *)(SIMSOCK_PARAM4(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_recv | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    simsock_chan_interrupt_wait(ch);

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_ubuf_phy, size, SIM_DMA_FROM_DEVICE);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_send(int sockfd, void *buff, size_t size, int flags)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_buff_phy = {0};

    if (buff == NULL) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_send);
    if (ret < 0) {
        return ret;
    }

    ch = ret;

    switch (g_simsock_ctx.copy_mode) {
        case map_single_mode: {
            dma_buff_phy = sim_dma_map_single(g_simsock_ctx.dev, buff, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
            break;
        }
        case alloc_mode: {
            ret = memcpy_s(g_simsock_ctx.chan[ch].buf, g_simsock_ctx.chan[ch].size, buff, size);
            if (ret != 0) {
                return SIMSOCK_FAIL;
            }
            g_simsock_ctx.chan[ch].len = size;

            dma_buff_phy = g_simsock_ctx.chan[ch].buf_phy;
            break;
        }
        case dirc_trans_mode: {
            dma_buff_phy = (uintptr_t)buff;
            break;
        }
        default:
            break;
    }

    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(LOW32(dma_buff_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_buff_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(size, (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(flags, (void *)(SIMSOCK_PARAM4(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_send | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    simsock_chan_interrupt_wait(ch);

    if (!g_simsock_ctx.copy_mode) {
        sim_dma_unmap_single(g_simsock_ctx.dev, dma_buff_phy, size, SIM_DMA_FROM_DEVICE);
    }

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));

    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_sendto(int sockfd, void *buff, size_t size, int flags, struct sockaddr *addr, int addrlen)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_buff_phy = {0};
    sim_dma_addr_t dma_addr_phy = {0};

    if ((buff == NULL) || (addr == NULL)) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_sendto);
    if (ret < 0) {
        return ret;
    }

    ch = ret;

    switch (g_simsock_ctx.copy_mode) {
        case map_single_mode: {
            dma_buff_phy = sim_dma_map_single(g_simsock_ctx.dev, buff, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
            break;
        }
        case alloc_mode: {
            ret = memcpy_s(g_simsock_ctx.chan[ch].buf, g_simsock_ctx.chan[ch].size, buff, size);
            if (ret != 0) {
                return SIMSOCK_FAIL;
            }
            g_simsock_ctx.chan[ch].len = size;

            dma_buff_phy = g_simsock_ctx.chan[ch].buf_phy;
            break;
        }
        case dirc_trans_mode: {
            dma_buff_phy = (uintptr_t)buff;
            break;
        }
        default:
            break;
    }

    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(LOW32(dma_buff_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_buff_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));

    simsock_writel(size, (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(flags, (void *)(SIMSOCK_PARAM4(ch) + g_simsock_ctx.simsock_regs));

    dma_addr_phy = sim_dma_map_single(g_simsock_ctx.dev, addr, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_addr_phy), (void *)(SIMSOCK_PARAM5(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_addr_phy), (void *)(SIMSOCK_PARAM6(ch) + g_simsock_ctx.simsock_regs));

    simsock_writel(addrlen, (void *)(SIMSOCK_PARAM7(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(addrlen, (void *)(SIMSOCK_PARAM8(ch) + g_simsock_ctx.simsock_regs));

    simsock_writel(simsock_cmd_sendto | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    simsock_chan_interrupt_wait(ch);

    if (!g_simsock_ctx.copy_mode) {
        sim_dma_unmap_single(g_simsock_ctx.dev, dma_buff_phy, size, SIM_DMA_FROM_DEVICE);
    }

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_addr_phy, sizeof(struct sockaddr), SIM_DMA_FROM_DEVICE);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_getpeername(int sockfd, struct sockaddr *addr, int *addrlen)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_addr_phy;
    sim_dma_addr_t dma_addrlen_phy;

    if ((addr == NULL) || (addrlen == NULL)) {
        return SIMSOCK_FAIL;
    }

    ret = simsock_chan_alloc(sockfd, simsock_cmd_getpeername);
    if (ret < 0)
        return ret;
    ch = ret;
    dma_addr_phy = sim_dma_map_single(g_simsock_ctx.dev, addr, sizeof(struct sockaddr), SIM_DMA_TO_DEVICE);
    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(LOW32(dma_addr_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_addr_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));

    dma_addrlen_phy = sim_dma_map_single(g_simsock_ctx.dev, addrlen, sizeof(int), SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_addrlen_phy), (void *)(SIMSOCK_PARAM3(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_addrlen_phy), (void *)(SIMSOCK_PARAM4(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_getpeername | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    ret = simsock_chan_complete(ch);
    if (ret) {
        return ret;
    }

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_addr_phy, sizeof(struct sockaddr), SIM_DMA_FROM_DEVICE);
    sim_dma_unmap_single(g_simsock_ctx.dev, dma_addrlen_phy, sizeof(int), SIM_DMA_FROM_DEVICE);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_shutdown(int sockfd, enum sock_shutdown_cmd how)
{
    int ret = 0;
    int ch = 0;

    ret = simsock_chan_alloc(sockfd, simsock_cmd_shutdown);
    if (ret < 0)
        return ret;
    ch = ret;
    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(how, (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_shutdown | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));
    ret = simsock_chan_complete(ch);
    if (ret) {
        return ret;
    }

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_release(int sockfd)
{
    int ret = 0;
    int ch = 0;

    ret = simsock_chan_alloc(sockfd, simsock_cmd_close);
    if (ret < 0)
        return ret;
    ch = ret;
    simsock_writel(sockfd, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(simsock_cmd_close | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));
    ret = simsock_chan_complete(ch);
    if (ret) {
        return ret;
    }
    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

int simsock_select(int nfds, int *readfds, int *writefds, int *exceptfds)
{
    int ret = 0;
    int ch = 0;
    sim_dma_addr_t dma_rd_phy;

    ret = simsock_chan_alloc(0, simsock_cmd_sendto);
    if (ret < 0)
        return ret;
    ch = ret;

    simsock_writel(nfds, (void *)(SIMSOCK_PARAM0(ch) + g_simsock_ctx.simsock_regs));
    dma_rd_phy = sim_dma_map_single(g_simsock_ctx.dev, readfds, SIMSOCK_FDS_SIZE, SIM_DMA_TO_DEVICE);
    simsock_writel(LOW32(dma_rd_phy), (void *)(SIMSOCK_PARAM1(ch) + g_simsock_ctx.simsock_regs));
    simsock_writel(HIGH32(dma_rd_phy), (void *)(SIMSOCK_PARAM2(ch) + g_simsock_ctx.simsock_regs));

    simsock_writel(simsock_cmd_select | SIMSOCK_OP_START, (void *)(SIMSOCK_OP(ch) + g_simsock_ctx.simsock_regs));

    simsock_chan_interrupt_wait(ch);

    sim_dma_unmap_single(g_simsock_ctx.dev, dma_rd_phy, SIMSOCK_FDS_SIZE, SIM_DMA_FROM_DEVICE);

    ret = simsock_readl((void *)(SIMSOCK_RESULT(ch) + g_simsock_ctx.simsock_regs));
    (void)simsock_chan_free(ch);

    return ret;
}

/* register a socket to table,set reference count to 1 */
static int hi_sock_reg_fd(int sockfd)
{
    int i = -1;

    sim_mutex_lock(&g_simsock_ctx.simfd_mutex);

    if (SIMSOCK_MAX <= g_simsock_ctx.used) {
        bsp_err("sock_reg fail, sock used full\n");
        goto fail;
    }
    for (i = 0; i < SIMSOCK_MAX; i++)
        if (!sim_atomic_read(&g_simsock_ctx.socks[i].cnt))
            break;
    if (SIMSOCK_MAX <= i) {
        i = -1;
        goto fail;
    }

    g_simsock_ctx.socks[i].sockfd = sockfd;
    sim_atomic_set(&g_simsock_ctx.socks[i].cnt, 1);

    g_simsock_ctx.used++;

    atomic_inc(&simsockdbg.sockregfd);

fail:
    sim_mutex_unlock(&g_simsock_ctx.simfd_mutex);

    return i;
}

static int hi_sock_unreg_fd(int fd)
{
    if (fd > SIMSOCK_MAX - 1 || fd < 0) {
        bsp_err("sock_unreg fail, sock used full\n");
        return -1;
    }

    sim_mutex_lock(&g_simsock_ctx.simfd_mutex);

    g_simsock_ctx.used--;

    sim_atomic_set(&g_simsock_ctx.socks[fd].cnt, 0);

    g_simsock_ctx.socks[fd].sockfd = 0;

    atomic_inc(&simsockdbg.sockunregfd);

    sim_mutex_unlock(&g_simsock_ctx.simfd_mutex);

    return 1;
}

static int hi_sock_get_fd(int fd)
{
    int err = -1;

    if (fd > SIMSOCK_MAX - 1 || fd < 0) {
        bsp_err("sock_getfd fail, fd = %d\n", fd);
        return err;
    }

    if (likely(sim_atomic_inc_not_zero(&g_simsock_ctx.socks[fd].cnt)))
        err = 0;
    atomic_inc(&simsockdbg.sockgetfd);

    return err;
}

static int hi_sock_put_fd(int fd)
{
    int sockfd;

    int err = -1;

    if (fd > SIMSOCK_MAX - 1 || fd < 0) {
        bsp_err("sock_putfd fail, fd = %d\n", fd);
        return err;
    }

    /* no use */
    if (unlikely(sim_atomic_dec_and_test(&g_simsock_ctx.socks[fd].cnt))) {
        sockfd = g_simsock_ctx.socks[fd].sockfd;

        err = hi_sock_unreg_fd(fd);
		bsp_err("close fd = %d\n", sockfd);
        simsock_release(sockfd);
    }
    atomic_inc(&simsockdbg.sockputfd);

    return 1;
}

int mdrv_socket(int family, int type, int protocol)
{
    int sockfd = 0;
    int retval = -1;

    atomic_inc(&simsockdbg.sock);

    retval = simsock_create(family, type, protocol);
    if (retval < 0) {
        bsp_err("create sock fail!\n");
        goto out;
    }
    sockfd = retval;
    retval = hi_sock_reg_fd(sockfd);
    if (retval < 0)
        goto out_release;

out:
    return retval;

out_release:
    simsock_release(sockfd);
    return retval;
}

int mdrv_bind(int fd, struct sockaddr *addr, int addrlen)
{
    int sockfd;
    int retval = -1;

    atomic_inc(&simsockdbg.bind);

    if (hi_sock_get_fd(fd) < 0) {
        return retval;
    }

    sockfd = hi_get_sockfd(fd);

    retval = simsock_bind(sockfd, addr, addrlen);
    if (retval < 0) {
        bsp_err("bind fail!\n");
    }

    hi_sock_put_fd(fd);

    return retval;
}

int mdrv_listen(int fd, int backlog)
{
    int sockfd;
    int retval = -1;

    atomic_inc(&simsockdbg.listen);

    if (hi_sock_get_fd(fd) < 0) {
        return retval;
    }

    sockfd = hi_get_sockfd(fd);

    retval = simsock_listen(sockfd, backlog);
    if (retval < 0) {
        bsp_err("listen fail!\n");
    }

    hi_sock_put_fd(fd);

    return retval;
}

int mdrv_accept(int fd, struct sockaddr *upeer_sockaddr, int *upeer_addrlen)
{
    int sockfd;
    int newsockfd;
    int err = -1;

    atomic_inc(&simsockdbg.accept_in);

    if (hi_sock_get_fd(fd) < 0) {
        atomic_inc(&simsockdbg.accept_out);
        return -1;
    }

    sockfd = hi_get_sockfd(fd);

    err = simsock_accept(sockfd, upeer_sockaddr, upeer_addrlen);
    if (err < 0)
        goto done;

    newsockfd = err;
    err = hi_sock_reg_fd(newsockfd);
    if (err < 0)
        simsock_release(newsockfd);

done:
    hi_sock_put_fd(fd);

    atomic_inc(&simsockdbg.accept_out);

    return err;
}

int mdrv_recv(int fd, void *ubuf, size_t size, int flags)
{
    int sockfd;
    int err = -1;

    atomic_inc(&simsockdbg.recv_in);

    if (hi_sock_get_fd(fd) < 0)
        goto out;

    sockfd = hi_get_sockfd(fd);

    if (ubuf == NULL) {
        bsp_err("recv buff is NULL \n");
        atomic_inc(&simsockdbg.recv_out);
        return -1;
    }

    err = simsock_recv(sockfd, ubuf, size, flags);

    hi_sock_put_fd(fd);

out:
    atomic_inc(&simsockdbg.recv_out);
    return err;
}

int mdrv_send(int fd, void *buff, size_t len, int flags)
{
    int sockfd;
    int err = -1;

    atomic_inc(&simsockdbg.send);

    if (hi_sock_get_fd(fd) < 0)
        goto out;

    sockfd = hi_get_sockfd(fd);

    if (buff == NULL) {
        bsp_err("send buff is NULL \n");
        return -1;
    }

    err = simsock_send(sockfd, buff, len, flags);

    hi_sock_put_fd(fd);

out:
    return err;
}

int mdrv_sendto(int fd, void *buff, size_t len, unsigned flags, struct sockaddr *addr, int addr_len)
{
    int sockfd;
    int err = -1;

    atomic_inc(&simsockdbg.sendto);

    if (hi_sock_get_fd(fd) < 0)
        goto out;

    sockfd = hi_get_sockfd(fd);

    err = simsock_sendto(sockfd, buff, len, flags, addr, addr_len);

    hi_sock_put_fd(fd);
out:
    return err;
}

int mdrv_getpeername(int fd, struct sockaddr *addr, int *addrlen)
{
    int sockfd;
    int err = -1;

    atomic_inc(&simsockdbg.getpeername);

    if (hi_sock_get_fd(fd) < 0)
        goto out;

    sockfd = hi_get_sockfd(fd);

    err = simsock_getpeername(sockfd, addr, addrlen);

    hi_sock_put_fd(fd);
out:
    return err;
}

int mdrv_shutdown(int fd, enum sock_shutdown_cmd how)
{
    int sockfd;
    int err = -1;

    atomic_inc(&simsockdbg.shutdown);

    if (hi_sock_get_fd(fd) < 0)
        goto out;

    sockfd = hi_get_sockfd(fd);
	bsp_err("shutdown sockfd: %d\n", sockfd);
    err = simsock_shutdown(sockfd, how);
    if (err < 0) {
        bsp_err("simsock shutdown fail\n");
    }

    hi_sock_put_fd(fd);
out:
    return err;
}

int mdrv_close(int fd)
{
    int ret = -1;

    atomic_inc(&simsockdbg.close);

    ret = hi_sock_put_fd(fd);

    return ret;
}

int mdrv_select(int n, hi_fd_set *inp, hi_fd_set *outp, hi_fd_set *exp_fdset, struct timeval *tvp)
{
    int i;
    int ret;
    int *readfds = NULL;
    int nfds = 0;
    int err = -1;

    atomic_inc(&simsockdbg.select_in);
    if ((SIMSOCK_MAX_READFDS < n) || (inp == NULL)) {
        bsp_err("select err \n");
        atomic_inc(&simsockdbg.select_out);
        return SIMSOCK_FAIL;
    }

	readfds = (int *)osl_malloc(sizeof(int) * SIMSOCK_MAX_READFDS);
	if (readfds == NULL) {
		return -1;
	}
	
    err = memset_s((char *)readfds, (sizeof(int) * SIMSOCK_MAX_READFDS), 0, (sizeof(int) * SIMSOCK_MAX_READFDS));
    if (err) {
        bsp_err("memset failed\n");
    }

    for (i = 0; i < SIMSOCK_MAX_READFDS; i++) {
        if (inp->fds_bits & (1 << i)) {
            readfds[i] = hi_get_sockfd(i);
            if (nfds < readfds[i])
                nfds = readfds[i];
        }
    }

    ret = simsock_select((nfds + 1), readfds, NULL, NULL);
    if (ret < 0) {
        bsp_err("simsock select err \n");
        atomic_inc(&simsockdbg.select_out);
		osl_free(readfds);
        return ret;
    }

    inp->fds_bits = 0;
    for (i = 0; i < SIMSOCK_MAX_READFDS; i++) {
		if (readfds[i] != 0) {
            inp->fds_bits |= (1 << i);				
		}
    }

    atomic_inc(&simsockdbg.select_out);
	osl_free(readfds);
    return ret;
}

void mdrv_sock_fd_zero(hi_fd_set* set)
{
    int ret;

    ret = memset_s((void*)set, sizeof(hi_fd_set), 0, sizeof(*set));
    if (ret) {
        bsp_err("memset_s failed\n");
    }
}

void svlan_show(void)
{
    bsp_err("sock:  %u", atomic_read(&simsockdbg.sock));
    bsp_err("bind:  %u", atomic_read(&simsockdbg.bind));
    bsp_err("listen:  %u", atomic_read(&simsockdbg.listen));
    bsp_err("select_in:  %u", atomic_read(&simsockdbg.select_in));
    bsp_err("select_out:  %u", atomic_read(&simsockdbg.select_out));
    bsp_err("accept_in:  %u", atomic_read(&simsockdbg.accept_in));
    bsp_err("accept_out:  %u", atomic_read(&simsockdbg.accept_out));
    bsp_err("send:  %u", atomic_read(&simsockdbg.send));
    bsp_err("recv_in:  %u", atomic_read(&simsockdbg.recv_in));
    bsp_err("recv_out:  %u", atomic_read(&simsockdbg.recv_out));
    bsp_err("sendto:  %u", atomic_read(&simsockdbg.sendto));
    bsp_err("getpeername:  %u", atomic_read(&simsockdbg.getpeername));
    bsp_err("shutdown:  %u", atomic_read(&simsockdbg.shutdown));
    bsp_err("close:  %u", atomic_read(&simsockdbg.close));
    bsp_err("sockregfd:  %u", atomic_read(&simsockdbg.sockregfd));
    bsp_err("sockunregfd:  %u", atomic_read(&simsockdbg.sockunregfd));
    bsp_err("sockgetfd:  %u", atomic_read(&simsockdbg.sockgetfd));
    bsp_err("sockputfd:  %u", atomic_read(&simsockdbg.sockputfd));
}

int simsock_set_copymode(int copy_mode)
{
    g_simsock_ctx.copy_mode = copy_mode;
    return g_simsock_ctx.copy_mode;
}
void set_simsock_debug(int level)
{
    simsock_debug = level;
}

static int sock_probe(sim_platform_device *pdev)
{
    int i;
    int ret;
    struct resource *regs = NULL;
    unsigned int irq;
    g_simsock_dbg.svlan_probe_begin = 1;
    bsp_err("[init]start \n");

    g_simsock_ctx.dev = &pdev->dev;

    g_simsock_ctx.copy_mode = alloc_mode;

    regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (regs == NULL) {
        return -ENXIO;
    }

    g_simsock_ctx.simsock_regs = devm_ioremap_resource(&pdev->dev, regs);
    if (IS_ERR(g_simsock_ctx.simsock_regs)) {
        return PTR_ERR(g_simsock_ctx.simsock_regs);
    }

    irq = platform_get_irq(pdev, 0);
    if (unlikely(irq == 0)) {
        return -ENXIO;
    }

    g_simsock_ctx.irq = irq;

    g_simsock_ctx.dev->dma_mask = &simsock_dma_mask;

    g_simsock_ctx.used = 0;
    g_simsock_ctx.chan_used = 0;
    g_simsock_ctx.chan_num = SIMSOCK_CHAN_MAX;
    g_simsock_ctx.chan_map = 0xffffffff;

    sim_mutex_init(&g_simsock_ctx.simfd_mutex);
    sim_mutex_init(&g_simsock_ctx.simch_mutex);

    for (i = 0; i < SIMSOCK_MAX; i++) {
        sim_atomic_set(&g_simsock_ctx.socks[i].cnt, 0);
        g_simsock_ctx.socks[i].sockfd = 0;
    }

    ret = simsock_chan_ctrl_init();
    if (unlikely(ret)) {
        bsp_err("[init]chan_init fail \n");
    }

    ret = sim_request_irq(irq, sock_interrupt, SIM_IRQF_SHARED, "sim_sock_driver", g_simsock_ctx.dev);
    if (ret < 0) {
        bsp_err("[init]request irq fail \n");
        return -ENXIO;
    }

    bsp_err("[init] ok\n");
    g_simsock_dbg.svlan_probe_end = 1;
    return 0;
}

int sock_remove(sim_platform_device *pdev)
{
    return 0;
}

static const struct of_device_id sock_match[] = {
    { .compatible = "hisilicon,sim_sock_drv" },
    {},
};

static struct platform_driver sim_sock_driver = {
    .probe = sock_probe,
    .remove = sock_remove,
    .driver = {
        .name = "sim_sock_driver",
        .of_match_table = sock_match,
    },
};

static int __init svlan_socket_init(void)
{
    int ret;
    bsp_version_info_s *version_info = NULL;

    g_simsock_dbg.svlan_module_init_begin = 1;
    bsp_err("svlan_socket init start!\n");
    pr_err("[pr_err]svlan_socket init start!\n");
    version_info = (bsp_version_info_s *)bsp_get_version_info();
    if (version_info->plat_type == PLAT_ESL || version_info->plat_type == PLAT_HYBRID) {
        ret = platform_driver_register(&sim_sock_driver);
    } else {
        ret = 0;
    }
    g_simsock_dbg.platform_ret = ret;
    g_simsock_dbg.svlan_module_init_end = 1;
    return ret;
}
module_init(svlan_socket_init);
#endif
