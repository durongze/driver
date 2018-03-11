#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/netlink.h>

#define NETLINK_SAMPLE 21//类型

struct sock *nl_sk = NULL;

void sample_input (struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	unsigned int pid;
	int rc;
    /* #define NLMSG_ALIGNTO 4U
     * #define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
     * #define NLMSG_HDRLEN  ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
     * #define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)
     * #define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
     * */
	int len = NLMSG_SPACE(1200);
	char data[100];
	int dlen=0;
	
	skb = skb_get(__skb);
	if (skb->len >= NLMSG_SPACE(0)) 
	{
		nlh = nlmsg_hdr(skb);
		dlen= nlh->nlmsg_len;
		pid = nlh->nlmsg_pid;/*发送进程ID */
		if(dlen>100)dlen=100;
		memset(data,0,100);
		memcpy(data,NLMSG_DATA(nlh),dlen); 
		printk("[%s:%d] net_link: recv '%s' from process %d.\n",__FUNCTION__,__LINE__,data,pid);
		kfree_skb(skb);
		
		skb = alloc_skb(len, GFP_ATOMIC);
		if (!skb)
		{
			printk("net_link: alloc_skb failed.\n");
			return;
		}
		nlh = nlmsg_put(skb,0,0,0,1200,0);
		nlh ->nlmsg_len=dlen;
		//NETLINK_CB(skb).pid = 0;/* 发自内核*/
	    NETLINK_CB(skb).creds.pid = 0;/* 发自内核*/
		memcpy(NLMSG_DATA(nlh), data, strlen(data));

		rc = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
		if (rc < 0) 
		{
			printk("net_link: unicast skb error\n");
		}
		printk("[%s:%d] net_link: send '%s' to process %d ok.\n",__FUNCTION__,__LINE__,data,pid);
	}
	return;
}
struct netlink_kernel_cfg cfg = {
        .input = sample_input,
};
static int init_netlink(void) 
{
	//nl_sk = netlink_kernel_create(&init_net, NETLINK_SAMPLE, 0, sample_input, NULL, THIS_MODULE);
	nl_sk = netlink_kernel_create(&init_net, NETLINK_SAMPLE, &cfg);
	if (!nl_sk) 
	{
		printk("net_link: Cannot create netlink socket.\n");
		return -EIO;
	}
	printk("net_link: create socket ok.\n");
	return 0;
}

int init_module()
{
	init_netlink();
	return 0;
}

void cleanup_module( )
{
	if (nl_sk != NULL)
	{
		sock_release(nl_sk->sk_socket);
	}
	printk("net_link: remove ok.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fgjnew@163.com");
