#ifndef GENETLINKSOCKET_H
#define GENETLINKSOCKET_H

#include "../PublicFun/Publicdefine.h"
#include <netlink/handlers.h>
#include <netlink/netlink.h>

#define KERNEL_CONTROL_EXPORT

#ifdef KERNEL_CONTROL_EXPORT

#define KERNEL_CONTROL_API __declspec(dllexport)

#else

#define KERNEL_CONTROL_API __declspec(dllimport)

#endif

class KERNEL_CONTROL_EXPORT GenetlinkSocket
{
public:
    GenetlinkSocket();

    ~GenetlinkSocket();

    bool Init(const std::string &family_name, const std::string &group_name = "");

    bool Join(int group = 0) const;

    bool Register(nl_recvmsg_msg_cb_t func, void *arg) const;

    bool Send(int command, int attrtype, const void *data, int datalen) const;

    bool Receive() const;

private:
    /* nocopyable */
    GenetlinkSocket(const GenetlinkSocket &);
    GenetlinkSocket &operator=(const GenetlinkSocket &);

private:
    nl_sock *sock_;
    bool initialized_;
    int family_id_;
    int group_id_;
};

#endif // GENETLINKSOCKET_H
