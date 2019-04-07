#include "NetFilter.h"
#include "NetConnect.h"
#include "../../PublicFun/datastructure.h"


bool g_net_check = true;
/*
 * 检查tcp网络链接
 */
static unsigned int check_tcp_connect(bool in, struct iphdr   *iph,
                                      struct sk_buff *pskb, struct tcphdr  *tcph)
{
    if (!tcph)
        return NF_ACCEPT;

    unsigned int uRet = NF_ACCEPT;
    net_ipport_info info;
    memset(&info, -1, sizeof (info));
    info.nSourcePort = htons(tcph->source);
    info.nDestPort   = htons(tcph->dest);
    info.nSourceIP   = iph->saddr;
    info.nDestIP     = iph->daddr;
    info.dataLen     = pskb->len;
    info.uIn       = in ? 1 : 0;
    info.uProtocol = IPPROTO_TCP;

    if(NF_DROP == check_net_connect(&info))
        return NF_DROP;

    return uRet;
}

/*
 * 检查udp网络链接
 */
static unsigned int check_udp_connect(bool in, struct iphdr   *iph,
                                      struct sk_buff *pskb, struct udphdr  *udph)
{
    if (!udph)
        return NF_ACCEPT;

    unsigned int uRet = NF_ACCEPT;
    net_ipport_info info;
    memset(&info, -1, sizeof (info));
    info.nSourcePort = htons(udph->source);
    info.nDestPort   = htons(udph->dest);
    info.nSourceIP   = iph->saddr;
    info.nDestIP     = iph->daddr;
    info.dataLen     = pskb->len;
    info.uIn       = in ? 1 : 0;
    info.uProtocol = IPPROTO_UDP;
    if(NF_DROP == check_net_connect(&info))
        return NF_DROP;

    return uRet;
}

/*
 * 检查icmp网络链接
 */
static unsigned int check_icmp_connect(bool in, struct icmphdr *icmph)
{
    if (!icmph)
        return NF_ACCEPT;
//    net_ipport_info info;
//    memset(&info, -1, sizeof (info));
//    info.nSourcePort = htons(icmph->source);
//    info.nDestPort   = htons(icmph->dest);
//    info.nSourceIP   = iph->saddr;
//    info.nDestIP     = iph->daddr;
//    info.dataLen     = pskb->len;
//    info.inOut       = in ? 1 : 0;
//    info.uProtocol = IPPROTO_ICMP;
    return NF_ACCEPT;
}


/*
 *
 * 所有的网络数据在该函数进行分流
 */
static unsigned int check_net_flow(bool in, struct sk_buff *pskb, u16 family)
{
    unsigned int   nRet		= NF_ACCEPT;
    struct icmphdr *icmph	= NULL;
    struct udphdr  *udph	= NULL;
    struct tcphdr  *tcph	= NULL;
    struct iphdr   *iph		= NULL;

    if (NULL == pskb){
        return nRet;
    }

    iph = ip_hdr(pskb);  // 获取ip头
    if (iph->daddr == iph->saddr){
        return NF_ACCEPT;
    }
    //    if ((iph->daddr == g_nServerIP)||(iph->saddr == g_nServerIP))
    //    {
    //        //服务器通信放行
    //        nRet = NF_ACCEPT;
    //        break;
    //    }
    switch(iph->protocol)
    {
    case IPPROTO_UDP:
        udph = udp_hdr(pskb);
        nRet = check_udp_connect(in, iph, pskb, udph);
        //        if(nRet == NF_DROP)
        //            return nRet;
        break;
    case IPPROTO_TCP:
        tcph = tcp_hdr(pskb);
        nRet = check_tcp_connect(in, iph, pskb, tcph);
        //        if(nRet == NF_DROP)
        //            return nRet;
        break;
    case IPPROTO_ICMP:
        icmph = icmp_hdr(pskb);
        nRet = check_icmp_connect(in, icmph);
        //        if(nRet == NF_DROP)
        //            return nRet;
        break;
    default:
        break;
    }

    return nRet;
}

/*
 * 注册netfilter hook函数
 */
static unsigned int my_ipv4_output(void *priv,
                                   struct sk_buff *skb,
                                   const struct nf_hook_state *state)
{
    if(g_net_check)
    {
        return check_net_flow(false, skb, PF_INET);
    }
    else
    {
        return NF_ACCEPT;
    }
}

/*
 * 注册netfilter hook函数
 */
static unsigned int my_ipv4_input(void *priv,
                                  struct sk_buff *skb,
                                  const struct nf_hook_state *state)
{
    if(g_net_check)
    {
        return check_net_flow(true, skb, PF_INET);
    }
    else
    {
        return NF_ACCEPT;
    }
}

/*
 * 注册netfilter hook函数
 */
static const struct nf_hook_ops my_nf_ops[] = {
{
    .hook =		my_ipv4_input,
    .pf =		NFPROTO_IPV4,
    .hooknum =	NF_INET_LOCAL_IN,
    .priority =	NF_IP_PRI_SELINUX_FIRST,
},
{
    .hook =		my_ipv4_output,
    .pf =		NFPROTO_IPV4,
    .hooknum =	NF_INET_LOCAL_OUT,
    .priority =	NF_IP_PRI_SELINUX_FIRST,
},
};


static int __net_init my_nf_register(struct net *net)
{
    return nf_register_net_hooks(net, my_nf_ops,
                                 ARRAY_SIZE(my_nf_ops));
}

static void __net_exit my_nf_unregister(struct net *net)
{
    nf_unregister_net_hooks(net, my_nf_ops,
                            ARRAY_SIZE(my_nf_ops));
}

struct pernet_operations my_net_ops = {
    .init = my_nf_register,
    .exit = my_nf_unregister,
};

//static LIST_HEAD(check_call_list);
//static DEFINE_MUTEX(check_call_mutex);

//void reg_check_call(check_call call)
//{
//    check_call_struct *this;

//    mutex_lock(&check_call_mutex);
//    this = kmalloc(sizeof(check_call_struct), GFP_KERNEL);
//    if (this) {
//        this->call = call;
//        list_add(&this->list, &check_call_list);
//    }
//    mutex_unlock(&check_call_mutex);
//}
