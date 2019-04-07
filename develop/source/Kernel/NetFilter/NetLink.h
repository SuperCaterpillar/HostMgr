#ifndef _NETLINK_H_
#define _NETLINK_H_

#include <linux/genetlink.h>
#include <linux/netlink.h>
#include <net/genetlink.h>

#include "../../PublicFun/NetLinkKernel.h"

#include "../Log/logger.h"

int __init hostmgr_netlink_init(void);

void hostmgr_netlink_exit(void);


#endif
