#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>

#define NETLINK_SAMPLE 21
#define MAX_PAYLOAD 1024

int init_sockaddr_nl(struct sockaddr_nl *addr, int pid)
{
    memset(addr, 0, sizeof(struct sockaddr_nl));
    addr->nl_family = AF_NETLINK;
    addr->nl_pid = pid;
    addr->nl_groups = 0;
	return 0;
}

struct nlmsghdr *create_nlmsghdr(const char *msg, int msg_len)
{
	struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strncpy(NLMSG_DATA(nlh), msg, msg_len);
	return nlh;
}

struct msghdr *create_msghdr(struct sockaddr_nl *addr, struct iovec *iov)
{
	struct msghdr *msg = (struct msghdr*)malloc(sizeof(struct msghdr));
    memset(msg, 0, sizeof(msg));
    msg->msg_name = (void *)addr;
    msg->msg_namelen = sizeof(struct sockaddr_nl);
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
	return msg;
}

int main(int argc, char* argv[])
{
    //struct sockaddr_nl {
    //    __kernel_sa_family_t    nl_family;  /* AF_NETLINK   */
    //    unsigned short  nl_pad;     /* zero     */
    //    __u32       nl_pid;     /* port ID  */
    //    __u32       nl_groups;  /* multicast groups mask */
    //};
    struct sockaddr_nl src_addr, dest_addr;

    //    struct nlmsghdr {
    //        __u32       nlmsg_len;  /* Length of message including header */
    //        __u16       nlmsg_type; /* Message content */
    //        __u16       nlmsg_flags;    /* Additional flags */
    //        __u32       nlmsg_seq;  /* Sequence number */
    //        __u32       nlmsg_pid;  /* Sending process port ID */
    //    };
    struct nlmsghdr *nlh = NULL;

    //struct iovec
    //{
    //        void __user *iov_base;  /* BSD uses caddr_t (1003.1g requires void *) */
    //        __kernel_size_t iov_len; /* Must be size_t (1003.1g) */
    //};
    struct iovec iov;

    //    struct msghdr {
    //        void        *msg_name;  /* ptr to socket address structure */
    //        int     msg_namelen;    /* size of socket address structure */
    //        struct iov_iter msg_iter;   /* data */
    //        void        *msg_control;   /* ancillary data */
    //        __kernel_size_t msg_controllen; /* ancillary data buffer length */
    //        unsigned int    msg_flags;  /* flags on received message */
    //        struct kiocb    *msg_iocb;  /* ptr to iocb for async requests */
    //    }; 
    struct msghdr *msg;

    int sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_SAMPLE);
    
	init_sockaddr_nl(&src_addr, getpid());
    bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
   
    init_sockaddr_nl(&dest_addr, 0);
    
	const char *hello = "hello kernel!";
	nlh = create_nlmsghdr(hello, strlen(hello));
    
	iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    
	msg = create_msghdr(&dest_addr, &iov);
 
    sendmsg(sock_fd, msg, 0);
    printf("Send message payload: %s\n", (char*)NLMSG_DATA(nlh));

    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    recvmsg(sock_fd, msg, 0);
    printf("Received message payload: %s\n", (char*)NLMSG_DATA(nlh));
	
	free((void*)nlh);
	free((void*)msg);
    close(sock_fd);
	return 0;
}
