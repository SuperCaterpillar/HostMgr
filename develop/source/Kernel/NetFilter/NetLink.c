#include "NetLink.h"


static int user_cmd_test(struct sk_buff *skb, struct genl_info *info)
{
    //listener_nlportid = info->snd_portid;
    printk("user_cmd nlpid %u\n", info->snd_portid);
    return 0;
}

static const struct genl_ops hostmgr_nl_ops[] = {
{
    .cmd	= HOSTMGR_GENL_TEST_CMD_EVENT,
    .doit	= user_cmd_test,
},
};


static struct genl_family family = {
    .name		= HOSTMGR_GENL_FAMILY_NAME,
    .version            = HOSTMGR_GENL_VERSION,
    .ops		= hostmgr_nl_ops,
    .n_ops		= ARRAY_SIZE(hostmgr_nl_ops),
    .module		= THIS_MODULE,
    .maxattr            = HOSTMGR_GENL_ATTR_MAX,
};


int __init hostmgr_netlink_init(void)
{

    return genl_register_family(&family);
}

void hostmgr_netlink_exit(void)
{
    genl_unregister_family(&family);
}
