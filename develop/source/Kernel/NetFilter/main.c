#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/types.h>

#include "../Log/logger.h"

#include "NetFilter.h"
#include "NetLink.h"

//内核等待
#ifndef SLEEP_SEC
#define SLEEP_SEC(nMilliSec)\
    do { \
    long timeout = (nMilliSec) * HZ / 1000; \
    while(timeout > 0) \
{\
    timeout = schedule_timeout(timeout); \
    } \
    }while(0);
#endif


static int __init NetFilter_init(void)
{
    int err;

    err = hostmgr_netlink_init();
    if(err)
    {
        panic("Register netfilter module: hostmgr_netlink_init: error %d\n", err);
    }
    LOG_INFO ("Register hostmgr_netlink_init module.\n");

    err = register_pernet_subsys(&my_net_ops);
    if (err)
        panic("Register netfilter module: register_pernet_subsys: error %d\n", err);
    LOG_INFO ("Register register_pernet_subsys module.\n");

    return 0;
}

static void __exit NetFilter_exit(void)
{

    hostmgr_netlink_exit();
    unregister_pernet_subsys(&my_net_ops);
    LOG_INFO("NetFilter_exit:  Unregistering netfilter hooks\n");
}


module_init(NetFilter_init);
module_exit(NetFilter_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("XXXXX");
MODULE_DESCRIPTION("Netfilter");
MODULE_VERSION("1.0.0");
MODULE_ALIAS("Net control");
