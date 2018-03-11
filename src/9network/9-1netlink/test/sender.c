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
        int sock_fd;

    //    struct msghdr {
    //        void        *msg_name;  /* ptr to socket address structure */
    //        int     msg_namelen;    /* size of socket address structure */
    //        struct iov_iter msg_iter;   /* data */
    //        void        *msg_control;   /* ancillary data */
    //        __kernel_size_t msg_controllen; /* ancillary data buffer length */
    //        unsigned int    msg_flags;  /* flags on received message */
    //        struct kiocb    *msg_iocb;  /* ptr to iocb for async requests */
    //    }; 
        struct msghdr msg;

    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_SAMPLE);
    memset(&msg, 0, sizeof(msg));
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); 
    src_addr.nl_groups = 0;
    bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;
    dest_addr.nl_groups = 0;

    nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));

    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh), "Hello kernel!");

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    sendmsg(sock_fd, &msg, 0);
        printf("Send message payload: %s\n",NLMSG_DATA(nlh));

    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    recvmsg(sock_fd, &msg, 0);
    printf("Received message payload: %s\n",NLMSG_DATA(nlh));
    close(sock_fd);
}
