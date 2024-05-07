#include "esl_socket_api.h"
#include <osl_types.h>
#include <osl_sem.h>
#include <linux/in.h>
#include "securec.h"
#include <bsp_print.h>
#include <linux/kthread.h>
#include <osl_malloc.h>

#define SERV_PORT 4506
#define LISTENQ 12
#define THIS_MODU mod_hisocket
#define CHSIZE 16
#define SENDBUFSIZE 1024 * 8
#define RECVBUFSIZE 1024 * 8
#define SENDTOBUFSIZE 200

struct clientdata {
    int clientfd;
    struct sockaddr_in cli_addr;
    int cli_len;
    int recv_len;
    int send_len;
    char recv_buf[RECVBUFSIZE];
    char send_buf[SENDBUFSIZE];
    int used;
    struct task_struct *clthread;
};

struct test_socket {
    struct mutex lock;
    int sockfd;
    struct sockaddr_in serv_addr;
    int udpfd;
    struct clientdata client_sock[CHSIZE];
    char sendto_buf[200];
    unsigned int sendto_len;
};

struct test_socket* test_sock = NULL;
unsigned int g_svlan_init_ok = 0;
char g_svlan_data[200] = {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};

extern int simsock_debug;

#define __HI_NFDBITS (8 * sizeof(unsigned int))
#define __HI_FDMASK(d) (1UL << ((d) % __HI_NFDBITS))

#define HI_FD_ISSET(d, set) ((set)->fds_bits & __HI_FDMASK(d))
#define HI_FD_SET(d, set) ((set)->fds_bits |= __HI_FDMASK(d))
#define HI_FD_CLR(d, set) ((set)->fds_bits &= ~__HI_FDMASK(d))

#define SOCKET_DBG(format, arg...) do { \
    bsp_err(format, ##arg);    \
} while (0)
#define SOCK_NULL (0)

void svlan_test_init(void)
{
    int ret;

    test_sock = osl_malloc(sizeof(struct test_socket));
    if (test_sock == NULL) {
        bsp_err("osl_malloc failed\n");
        return;
    }
    
    ret = memset_s(test_sock, sizeof(*test_sock), 0, sizeof(struct test_socket));
    if (ret != 0) {
        bsp_err("memset_s failed\n");
        return;
    }

    g_svlan_init_ok = 1;
    return;
}

void sim_test_creat(int type)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    ret = mdrv_socket(AF_INET, type, 0);
    if (ret < 0) {
        bsp_err("creat err\n");
        return;
    }
    test_sock->sockfd = ret;
    SOCKET_DBG("creat ok, sockfd:%d\n", ret);
    return;
}

void sim_test_bind(unsigned int sim_port)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }
    
    test_sock->serv_addr.sin_family = AF_INET;
    test_sock->serv_addr.sin_port = htons(sim_port);
    test_sock->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = mdrv_bind(test_sock->sockfd, (void *)&test_sock->serv_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        bsp_err("bind err\n");
        return;
    }
    SOCKET_DBG("bind ok\n");
    return;
}

void sim_test_listen(void)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    ret = mdrv_listen(test_sock->sockfd, LISTENQ);
    if (ret < 0) {
        bsp_err("listen err\n");
        return;
    }
    SOCKET_DBG("listen ok\n");
    return;
}

int sim_test_accept(int index)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return -1;
    }

    ret = mdrv_accept(test_sock->sockfd, (void *)&test_sock->client_sock[index].cli_addr,
                    &test_sock->client_sock[index].cli_len);
    if (ret < 0) {
        bsp_err("accept err\n");
        return -1;
    }
    test_sock->client_sock[index].clientfd = ret;
    SOCKET_DBG("accept ok,client sockfd:%d\n", ret);
    return 0;
}

void sim_test_recv(int index)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    ret = mdrv_recv(test_sock->client_sock[index].clientfd, test_sock->client_sock[index].recv_buf,
                  sizeof(test_sock->client_sock[index].recv_buf), 0);
    if (ret < 0) {
        bsp_err("recv err!\n");
        return;
    }

    test_sock->client_sock[index].recv_len = ret;
    SOCKET_DBG("recv %d bytes\n", ret);

    return;
}

void sim_test_send(int index, int send_mode, int sim_copy_mode)
{
    int ret;
    int clientfd = test_sock->client_sock[index].clientfd;
    char *sendbuf = test_sock->client_sock[index].send_buf;
    char *recvbuf = test_sock->client_sock[index].recv_buf;
    int recv_len = test_sock->client_sock[index].recv_len;
    int send_len;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    (void)simsock_set_copymode(sim_copy_mode);

    if (send_mode) {
        send_len = strlen("hello,this is sim_sock_serv");
        strcpy_s(sendbuf, send_len, "hello,this is sim_sock_serv");  //lint !e421
    } else {
        send_len = recv_len;
        ret = memcpy_s((void *)sendbuf, sizeof(char) * SENDBUFSIZE, (void *)recvbuf, recv_len);
        if (ret) {
            bsp_err("memcpy_s failed\n");
        }
    }
    ret = mdrv_send(clientfd, sendbuf, send_len, 0);
    if (ret < 0) {
        bsp_err("send err\n");
        return;
    }

    SOCKET_DBG("send ok\n");
    return;
}

void sim_test_shutdown(int index)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    ret = mdrv_shutdown(test_sock->client_sock[index].clientfd, 2);
    if (ret < 0) {
        bsp_err("shutdown err\n");
        return;
    }
    SOCKET_DBG("shutdown ok\n");
    return;
}

void sim_test_close(int sockfd)
{
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    ret = mdrv_close(sockfd);

    if (ret < 0) {
        bsp_err("close err\n");
        return;
    }
    SOCKET_DBG("close ok\n");
    return;
}

void sim_test_sendrecv(int type, int serv_port, unsigned int sendrecv_times)
{
    unsigned int forever = (0 == sendrecv_times) ? 1 : 0;
    int index = 0;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    (void)sim_test_creat(type);
    (void)sim_test_bind(serv_port);
    (void)sim_test_listen();
    (void)sim_test_accept(index);

    do {
        (void)sim_test_recv(index);
        (void)sim_test_send(index, 0, 1);
        sendrecv_times--;
    } while (sendrecv_times || forever);

    (void)sim_test_close(test_sock->client_sock[index].clientfd);
    (void)sim_test_close(test_sock->sockfd);

    SOCKET_DBG("test_sendrecv finished!\n");
}

void svlan_test_sendto(unsigned long long ipv4, int serv_port, unsigned int times, unsigned int data_len)
{
    unsigned int forever = (0 == times) ? 1 : 0;
    int ret;

    if (g_svlan_init_ok != 1) {
        bsp_err("please run svlan_test_init first\n");
        return;
    }

    if (ipv4 == 0) {
        bsp_err("please input ipv4\n");
        return;
    }

    (void)sim_test_creat(SOCK_DGRAM);

    test_sock->serv_addr.sin_family = AF_INET;
    test_sock->serv_addr.sin_port = htons(serv_port);
    test_sock->serv_addr.sin_addr.s_addr = ipv4;

    test_sock->sendto_len = data_len;
    ret = memcpy_s(test_sock->sendto_buf, sizeof(test_sock->sendto_buf), g_svlan_data, test_sock->sendto_len);
    if (ret != 0) {
        return;
    }

    do {
        ret = mdrv_sendto(test_sock->sockfd, test_sock->sendto_buf, test_sock->sendto_len, 0,
                        (void *)&test_sock->serv_addr, sizeof(test_sock->serv_addr));
        if (ret < 0) {
            bsp_err("sendto err\r\n");
            return;
        }
        SOCKET_DBG("sendto ok, %d bytes\r\n", ret);

        times--;

    } while (times || forever);

    (void)sim_test_close(test_sock->sockfd);

    SOCKET_DBG("test_sendto finished!");
}
