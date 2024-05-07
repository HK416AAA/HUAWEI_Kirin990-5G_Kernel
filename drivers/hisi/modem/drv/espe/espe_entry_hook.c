/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/switchdev.h>
#include "espe.h"
#include "espe_ip_entry.h"
#include "espe_mac_entry.h"
#include "espe_port.h"
#include "espe_dbg.h"

#ifdef CONFIG_BALONG_ESPE_FW
#include <net/netfilter/br_netfilter.h>
#endif

#ifdef CONFIG_BALONG_ESPE_FW
struct spe_hook_ops g_spe_hook = {0};

int espe_skb_is_updateonly(void *skb)
{
    struct sk_buff *espe_skb = NULL;

    espe_skb = (struct sk_buff *)skb;

    if (espe_skb->update_only) {
        g_espe.cpuport.cpu_updonly_comp++;
        if (unlikely(g_espe.msg_level & SPE_MSG_TUPLE)) {
            SPE_ERR("update only complete! \n");
        }
    }
    return (int)(espe_skb->update_only);
}

unsigned int espe_ip_hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct nf_conn *ct = NULL;
    enum ip_conntrack_info ctinfo;

    ct = nf_ct_get(skb, &ctinfo);
    if (ct != NULL) {
        ct->status |= IPS_SPE_FORWARD;
    } else {
        SPE_ERR("espe_ip_hook_func ct is null.\n");
    }

    return NF_ACCEPT;
}

const struct nf_hook_ops g_espe_nf_ops[] = {
    {
        .hook =     espe_ip_hook_func,
        .pf =       NFPROTO_IPV4,
        .hooknum =  NF_INET_FORWARD,
        .priority = NF_IP_PRI_FIRST,
    },
    {
        .hook =     espe_ip_hook_func,
        .pf =       NFPROTO_IPV6,
        .hooknum =  NF_INET_FORWARD,
        .priority = NF_IP_PRI_FIRST,
    },
};

int __net_init espe_hook_init(void)
{
    int ret = 0;

    ret = nf_register_net_hooks(&init_net, g_espe_nf_ops, ARRAY_SIZE(g_espe_nf_ops));
    if (ret) {
        SPE_ERR("registering netfilter hooks fail.\n");
    }
    return ret;
}

void __net_exit espe_hook_exit(void)
{
    nf_unregister_net_hooks(&init_net, g_espe_nf_ops, ARRAY_SIZE(g_espe_nf_ops));
    return;
}

int espe_switchdev_event(struct notifier_block *nb, unsigned long event, void *ptr)
{
    struct net_device *dev = switchdev_notifier_info_to_dev(ptr);
    struct switchdev_notifier_fdb_info *fdb_info = ptr;
    struct net_bridge_port *p = br_port_get_rcu(dev);
    struct net_bridge *br = NULL;

    if (p == NULL) {
        SPE_ERR(" dev correspond port of br is null, please check.\n");
        return NOTIFY_DONE;
    }

    br = p->br;
    switch (event) {
        case SWITCHDEV_FDB_ADD_TO_DEVICE:
            if (fdb_info->is_local) {
                if (memcmp(fdb_info->addr, br->dev->dev_addr, ETH_ALEN) < 0) {
                    espe_mac_fw_del(br->dev->dev_addr, 0, br->dev->spe_portno, br->dev->spe_portno);
                    espe_mac_fw_add(fdb_info->addr, 0, br->dev->spe_portno, br->dev->spe_portno, 1);
                }
                espe_port_attach_brg_modify(fdb_info->info.dev->spe_portno, spe_attach_brg_brif);
            } else {
                espe_mac_fw_add((char *)fdb_info->addr,
                                is_vlan_dev(fdb_info->info.dev) ? vlan_dev_vlan_id(fdb_info->info.dev) : 0,
                                br->dev->spe_portno, fdb_info->info.dev->spe_portno, 0);
            }
            break;
        case SWITCHDEV_FDB_DEL_TO_DEVICE:
            if (fdb_info->is_local) {
                espe_port_attach_brg_modify(fdb_info->info.dev->spe_portno, spe_attach_brg_normal);
            } else {
                espe_mac_fw_del((char *)fdb_info->addr,
                                is_vlan_dev(fdb_info->info.dev) ? vlan_dev_vlan_id(fdb_info->info.dev) : 0,
                                br->dev->spe_portno, fdb_info->info.dev->spe_portno);
            }
            break;
        default:
            return NOTIFY_DONE;
    }
    return NOTIFY_DONE;
}

struct notifier_block g_espe_switchdev_notifier = {
    .notifier_call = espe_switchdev_event,
};

/*
 * Handle changes in state of network devices enslaved to a bridge.
 * Note: don't care about up/down if bridge itself is down, because
 *     port state is checked when bridge is brought up.
 */
int espe_br_device_event(struct notifier_block *unused, unsigned long event, void *ptr)
{
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);
    struct net_bridge_port *p = NULL;
    struct net_bridge *br = NULL;
    int ret;
    int portno;
    struct netdev_notifier_changeupper_info *changeupper_info = ptr;

    /* register of bridge completed, add entries */
    if ((dev->priv_flags & IFF_EBRIDGE) && event == NETDEV_REGISTER) {
            portno = espe_alloc_br_port();
            if (!SPE_BR_PORT(portno)) {
                SPE_ERR("br alloc port failed,portno %d\n", portno);
            } else {
                dev->spe_portno = portno;
                espe_mac_fw_add(dev->dev_addr, 0, dev->spe_portno, dev->spe_portno, 1);
            }
            return NOTIFY_DONE;
    }
    /* unregister of bridge completed, del entries */
    if ((dev->priv_flags & IFF_EBRIDGE) && event == NETDEV_UNREGISTER) {
            portno = dev->spe_portno;
            espe_mac_fw_del(dev->dev_addr, 0, dev->spe_portno, dev->spe_portno);
            ret = espe_free_br_port(portno);
            if (ret) {
                SPE_INFO("br free port failed, portno %d\n", portno);
            }
            return NOTIFY_DONE;
    }

    /* not a port of a bridge */
    p = br_port_get_rtnl(dev);
    if (p == NULL || dev->spe_alloced == 0) {
        return NOTIFY_DONE;
    }

    br = p->br;
    switch (event) {
        case NETDEV_UP: // 先执行brctl addif br00 usb0，再执行ifconfig usb0 up时，触发该事件
            espe_port_attach_brg_modify(dev->spe_portno, spe_attach_brg_brif);
            break;
        case NETDEV_DOWN: // 先执行brctl addif br00 usb0，ifconfig usb0 up，再执行ifconfig usb0 down时，触发该事件
            espe_port_attach_brg_modify(dev->spe_portno, spe_attach_brg_normal);
            break;
        case NETDEV_PRECHANGEUPPER:  // brctl addif/delif br00 usb0, 都触发该envent, 依据changeupper_info->linking的状态
                                    // 区分addif or delif
            if (changeupper_info->linking == false) {
                espe_port_attach_brg_modify(dev->spe_portno, spe_attach_brg_normal);
            }
            break;
        default:
            return NOTIFY_DONE;
    }
    return NOTIFY_DONE;
}

static struct notifier_block g_espe_br_device_notifier = {
    .notifier_call = espe_br_device_event
};

int espe_macfw_hook_init(void)
{
    int err;

    err = register_netdevice_notifier(&g_espe_br_device_notifier);
    if (err) {
        SPE_ERR("Failed to register netdevice notifier\n");
        return err;
    }

    err = register_switchdev_notifier(&g_espe_switchdev_notifier);
    if (err) {
        SPE_ERR("Failed to register switchdev notifier\n");
        return err;
    }

    return 0;
}
void espe_macfw_hook_exit(void)
{
    unregister_switchdev_notifier(&g_espe_switchdev_notifier);
    return;
}

void spe_hook_register(void)
{
    int ret;

    g_spe_hook.ipfw_entry_add = espe_ip_fw_add;
    g_spe_hook.ipfw_entry_del = espe_ip_fw_del;
    g_spe_hook.is_updateonly = espe_skb_is_updateonly;

    ret = espe_hook_init();
    if (ret) {
        SPE_ERR("espe hook init fail. \n");
    }
    ret = espe_macfw_hook_init();
    if (ret) {
        SPE_ERR("espe macfw hook init fail.\n");
    }
    return;
}

void spe_hook_unregister(void)
{
    g_spe_hook.ipfw_entry_add = NULL;
    g_spe_hook.ipfw_entry_del = NULL;
    g_spe_hook.is_updateonly = NULL;

    espe_hook_exit();
    espe_macfw_hook_exit();
}
#endif

MODULE_ALIAS("hisilicon network hardware accelerator driver");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("smart packet engine(spe) driver");
