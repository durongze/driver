#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/netlink.h>

#define NETLINK_SAMPLE 22//ÀàÐÍ
#define MAX_PAYLOAD 1200

struct sock *nl_sk = NULL;

bool skb_get_data(struct sk_buff *__skb, char *data, int *data_len, unsigned int *pid)
{
	struct nlmsghdr *nlh = NULL;
	struct sk_buff *skb = skb_get(__skb);
	if (skb->len >= NLMSG_SPACE(0)) 
	{
		nlh = nlmsg_hdr(skb);
		*pid  = nlh->nlmsg_pid;
		if (nlh->nlmsg_len <= *data_len) {
			memset(data, 0, *data_len);
			memcpy(data, NLMSG_DATA(nlh), nlh->nlmsg_len);
			*data_len = nlh->nlmsg_len; 
		}
		printk("net_link: recv (%d) '%s' from process %d.\n", *data_len, data, *pid);
		kfree_skb(skb);
		return true;
	}
	return false;
}

struct sk_buff *skb_create(int data_len, char *data)
{
	struct nlmsghdr *nlh = NULL;
	int len = NLMSG_SPACE(MAX_PAYLOAD);
	struct sk_buff *skb = alloc_skb(len, GFP_ATOMIC);
	if (!skb) {
		return skb;
	}
	nlh = nlmsg_put(skb, 0, 0, 0, MAX_PAYLOAD, 0);
	nlh->nlmsg_len = data_len;
	// NETLINK_CB(skb).pid = 0;
	NETLINK_CB(skb).creds.pid = 0;
	memcpy(NLMSG_DATA(nlh), data, strlen(data));
	return skb;
}

void sample_input (struct sk_buff *__skb)
{
    /* #define NLMSG_ALIGNTO 4U
     * #define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
     * #define NLMSG_HDRLEN  ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
     * #define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)
     * #define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
     * */
	int rc;
 	bool ret;
	unsigned int pid;
	struct sk_buff *skb;
	char data[100] = {"kernel running..."};
	int data_len = sizeof(data);

    ret = skb_get_data(__skb, data, &data_len, &pid);
	if (!ret) {
		return;
	}

    skb = skb_create(data_len, data);
	if (skb == NULL) {
		return;
	}

	rc = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
	if (rc < 0) 
	{
		printk("net_link: unicast skb error\n");
	} else {
		printk("net_link: send (%d) '%s' to process %d ok.\n", data_len, data, pid);
	}

	return;
}

struct netlink_kernel_cfg cfg = {
    .input = sample_input,
};

static int init_netlink(void) 
{
	// nl_sk = netlink_kernel_create(&init_net, NETLINK_SAMPLE, 0, sample_input, NULL, THIS_MODULE);
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
