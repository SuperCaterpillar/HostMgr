#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef struct net_connect_info
{
    unsigned long	ulDestStartIP;
    unsigned long	ulDestEndIP;
    unsigned short	uDestStartPort;
    unsigned short	uDestEndPort;
    unsigned long	ulSourceStartIP;
    unsigned long	ulSourceEndIP;
    unsigned short	uSourceStartPort;
    unsigned short	uSourceEndPort;
    unsigned short  uProtocol;      //IPPROTO_UDP
    unsigned short  uIn;             // 1 进入本机 0从本机流出
}net_connect_info;

typedef struct net_ipport_info
{
    unsigned short	nSourcePort;
    unsigned short	nDestPort;
    unsigned long	nSourceIP;
    unsigned long	nDestIP;
    unsigned int	dataLen;        //数据包长度
    unsigned short  uIn;            // 1 进入本机 0从本机流出
    unsigned short  uProtocol;      //IPPROTO_UDP = 17   IPPROTO_TCP = 6,	IPPROTO_ICMP = 1,
}net_ipport_info;
#endif // DATASTRUCTURE_H
