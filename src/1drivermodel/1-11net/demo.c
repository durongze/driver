
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>	
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>

#include <linux/in6.h>
#include <asm/checksum.h>

#include "demo.h"

#define IRQ_NET_CHIP  200//��Ҫ����Ӳ��ȷ��

MODULE_AUTHOR("fgj");
MODULE_LICENSE("Dual BSD/GPL");

static char  netbuffer[100];
struct net_device *simnetdevs;
/*
 * simnetrx,recieves a network packet and put the packet into TCP/IP up
 * layer,netif_rx() is the kernel API to do such thing. The recieving
 * procedure must alloc the sk_buff structure to store the data,
 * and the sk_buff will be freed in the up layer.
 */

void simnetrx(struct net_device *dev, int len, unsigned char *buf)
{
    struct sk_buff *skb;
    //struct simnetpriv *priv = (struct simnetpriv *) dev->priv;
    struct simnetpriv *priv = (struct simnetpriv *)netdev_priv(dev);

    printk("[%s:%d] => netif_rx.\n", __FUNCTION__, __LINE__);
    skb = dev_alloc_skb(len+2);
    if (!skb) {
        printk("simnetrx can not allocate more memory to store the packet. drop the packet\n");
        priv->stats.rx_dropped++;
        return;
    }
    skb_reserve(skb, 2);
    memcpy(skb_put(skb, len), buf, len);

    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);
    /* We need not check the checksum */
    skb->ip_summed = CHECKSUM_UNNECESSARY; 
    priv->stats.rx_packets++;
                              
    netif_rx(skb);
    return;
}

static irqreturn_t simnet_interrupt (int irq, void *dev_id)
{
	struct net_device *dev;
	dev = (struct net_device *) dev_id;

    //get data from hardware register
    printk("[%s:%d]irq:%d => simnetrx.\n", __FUNCTION__, __LINE__, irq);
	simnetrx(dev,100,netbuffer);
	
	return IRQ_HANDLED;
}

int simnetopen(struct net_device *dev)
{
	int ret=0;
	printk("[%s:%d] request_irq:%d => netif_start_queue.\n",__FUNCTION__, __LINE__, IRQ_NET_CHIP);
    ret = request_irq(IRQ_NET_CHIP, simnet_interrupt, IRQF_SHARED,
			  dev->name, dev);
	if (ret) return ret;
	netif_start_queue(dev);
    return 0;
}

int simnetrelease(struct net_device *dev)
{
	printk("[%s:%d] => netif_stop_queue.\n",__FUNCTION__, __LINE__);
    netif_stop_queue(dev);          
    return 0;
}

/*
 * pseudo network hareware transmit,it just put the data into the 
 * ed_tx device.
 */

void simnethw_tx(char *buf, int len, struct net_device *dev)
{
    struct simnetpriv *priv;
   
	printk("[%s:%d] \n",__FUNCTION__, __LINE__);
    /* check the ip packet length,it must more then 34 octets */
    if (len < sizeof(struct ethhdr) + sizeof(struct iphdr))
	{
        printk("Bad packet! It's size is less then 34!\n");
        return;
    }
    /* record the transmitted packet status */
    //priv = (struct simnetpriv *) dev->priv;
    priv = (struct simnetpriv *) netdev_priv(dev);
    priv->stats.tx_packets++;
    priv->stats.rx_bytes += len;
    
    /* remember to free the sk_buffer allocated in upper layer. */
    dev_kfree_skb(priv->skb);
}

/*
 * Transmit the packet,called by the kernel when there is an
 * application wants to transmit a packet.
 */

int simnettx(struct sk_buff *skb, struct net_device *dev)
{
    int len;
    char *data;
    struct netdev_queue *txq = NULL;
    //struct simnetpriv *priv = (struct simnetpriv *) dev->priv;
    struct simnetpriv *priv = (struct simnetpriv *) netdev_priv(dev);

	printk("[%s:%d] => simnethw_tx\n",__FUNCTION__, __LINE__);
    len = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;
    data = skb->data;
    /* stamp the time stamp */
    //dev->trans_start = jiffies;
    txq = netdev_get_tx_queue(dev,0);
    txq->trans_start = jiffies;

    /* remember the skb and free it in simnethw_tx */
    priv->skb = skb;
    
    /* pseudo transmit the packet,hehe */
    simnethw_tx(data, len, dev);

    return 0; 
}

/*
 * Deal with a transmit timeout.
 */
void simnettx_timeout (struct net_device *dev)
{
    //struct simnetpriv *priv = (struct simnetpriv *) dev->priv;
    struct simnetpriv *priv = (struct simnetpriv *) netdev_priv(dev);
	
    printk("[%s:%d] => netif_wake_queue.\n",__FUNCTION__, __LINE__);
    priv->stats.tx_errors++;
    netif_wake_queue(dev);

    return;
}



/*
 * When we need some ioctls.
 */
int simnetioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	printk("[%s:%d] \n",__FUNCTION__, __LINE__);
    return 0;
}

/*
 * ifconfig to get the packet transmitting status.
 */

struct net_device_stats *simnetstats(struct net_device *dev)
{
    //struct simnetpriv *priv = (struct simnetpriv *) dev->priv;
    struct simnetpriv *priv = (struct simnetpriv *) netdev_priv(dev);
	printk("[%s:%d] \n",__FUNCTION__, __LINE__);
    return &priv->stats;
}

/*
 * TCP/IP handshake will call this function, if it need.
 */
int simnetchange_mtu(struct net_device *dev, int new_mtu)
{
    unsigned long flags;
    //spinlock_t *lock = &((struct simnetpriv *) dev->priv)->lock;
    spinlock_t *lock = &((struct simnetpriv *) netdev_priv(dev))->lock;
	printk("[%s:%d] \n",__FUNCTION__, __LINE__);
    
    /* en, the mtu CANNOT LESS THEN 68 OR MORE THEN 1500. */
    if (new_mtu < 68)
        return -EINVAL;
   
    spin_lock_irqsave(lock, flags);
    dev->mtu = new_mtu;
    spin_unlock_irqrestore(lock, flags);

    return 0; 
}

struct net_device_ops simnet_ops = 
{
    .ndo_open = simnetopen,
    .ndo_stop = simnetrelease,
    .ndo_start_xmit = simnettx,
    .ndo_do_ioctl = simnetioctl,
    .ndo_get_stats = simnetstats,
    .ndo_change_mtu = simnetchange_mtu,
    .ndo_tx_timeout = simnettx_timeout,
};
void simnetinit(struct net_device *dev)
{
	struct simnetpriv *priv;
	ether_setup(dev); /* assign some of the fields */
	printk("[%s:%d] \n",__FUNCTION__, __LINE__);
#if 0
	dev->open            = simnetopen;
	dev->stop            = simnetrelease;
	dev->hard_start_xmit = simnettx;
	dev->do_ioctl        = simnetioctl;
	dev->get_stats       = simnetstats;
	dev->change_mtu      = simnetchange_mtu;  
	dev->tx_timeout      = simnettx_timeout;
#else
    dev->netdev_ops = &simnet_ops;
#endif

	dev->dev_addr[0] = 0x18;//(0x01 & addr[0])Ϊmulticast
	dev->dev_addr[1] = 0x02;
	dev->dev_addr[2] = 0x03;
	dev->dev_addr[3] = 0x04;
	dev->dev_addr[4] = 0x05;
	dev->dev_addr[5] = 0x06;

	/* keep the default flags, just add NOARP */
	dev->flags           |= IFF_NOARP;
	dev->features        |= NETIF_F_NO_CSUM;
	/*
	 * Then, initialize the priv field. This encloses the statistics
	 * and a few private fields.
	 */
	priv = netdev_priv(dev);
	memset(priv, 0, sizeof(struct simnetpriv));
	spin_lock_init(&priv->lock);
}

void simnetcleanup(void)
{
	if (simnetdevs) 
	{
		unregister_netdev(simnetdevs);
		free_netdev(simnetdevs);
	}
	return;
}

int simnetinit_module(void)
{
	int result,ret = -ENOMEM;

	/* Allocate the devices */
	/*simnetdevs=alloc_netdev(sizeof(struct simnetpriv), "eth%d",
			simnetinit);*/
	simnetdevs=alloc_netdev(sizeof(struct simnetpriv), "eth%d", NET_NAME_UNKNOWN, simnetinit);
	if (simnetdevs == NULL)
		goto out;

	ret = -ENODEV;
	if ((result = register_netdev(simnetdevs)))
		printk("demo: error %i registering device \"%s\"\n",result, simnetdevs->name);
	else
		ret = 0;
   out:
	if (ret) 
		simnetcleanup();
	return ret;
}

module_init(simnetinit_module);
module_exit(simnetcleanup);
