//#include <linux/kernel.h>
//#include <linux/module.h>
//#include <linux/init.h>
//#include <linux/types.h>
//#include <linux/string.h>
//#include <linux/netdevice.h>
//#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
//#include <linux/fs.h>
//#include <asm/uaccess.h>
//#include <linux/kthread.h>
//#include <linux/semaphore.h>
//#include <net/genetlink.h>
//#include <net/ip.h>
//#include <linux/skbuff.h>
//#include <linux/netfilter.h>
//#include <linux/fs.h>
//#include <linux/path.h>
//#include <linux/sched.h>
//#include <generated/uapi/linux/version.h>
//#include <net/genetlink.h>

#ifndef _NETFILTER_H_
#define _NETFILTER_H_

#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/netfilter_ipv4.h>

#include <linux/skbuff.h>

#include <linux/udp.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>


extern bool g_net_check;
extern struct pernet_operations my_net_ops;

//typedef unsigned int (*check_call)(bool in, struct sk_buff *pskb);
//typedef struct check_call_struct {
//    check_call call;
//    struct list_head list;
//}check_call_struct;

//void reg_check_call(check_call);


#endif
