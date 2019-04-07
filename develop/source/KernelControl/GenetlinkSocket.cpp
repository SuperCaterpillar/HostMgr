#include "GenetlinkSocket.h"

#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

GenetlinkSocket::GenetlinkSocket()
    : sock_(nullptr), initialized_(false), family_id_(0), group_id_(0)
{}

GenetlinkSocket::~GenetlinkSocket()
{
    if (initialized_) {
        nl_close(sock_);
        nl_socket_free(sock_);
    }
}

bool GenetlinkSocket::Init(const std::string &family_name, const std::string &group_name)
{
    if (initialized_) {
        return false;
    }

    sock_ = nl_socket_alloc();
    if (!sock_) {
        return false;
    }

    if (genl_connect(sock_) < 0) {
        nl_socket_free(sock_);
        return false;
    }

    family_id_ = genl_ctrl_resolve(sock_, family_name.c_str());
    if (family_id_ < 0) {
        nl_socket_free(sock_);
        return false;
    }

    if (!group_name.empty()) {
        group_id_ = genl_ctrl_resolve_grp(sock_, family_name.c_str(), group_name.c_str());
        if (group_id_ < 0) {
            nl_socket_free(sock_);
            return false;
        }
    }

    initialized_ = true;

    return true;
}

bool GenetlinkSocket::Join(int group) const
{
    if (!initialized_) {
        return false;
    }

    if (group == 0) {
        return (nl_socket_add_membership(sock_, group_id_) == 0);
    } else {
        return (nl_socket_add_membership(sock_, group) == 0);
    }
}

bool GenetlinkSocket::Send(int command, int attrtype, const void *data, int datalen) const
{
    if (!initialized_) {
        return false;
    }

    nl_msg *msg = nlmsg_alloc();
    if (!msg) {
        return false;
    }

    if (!genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id_, 0, NLM_F_REQUEST, command, 1)) {
        nlmsg_free(msg);
        return false;
    }

    if (nla_put(msg, attrtype, datalen, data) < 0) {
        nlmsg_free(msg);
        return false;
    }

    int result = nl_send_auto(sock_, msg);
    nlmsg_free(msg);

    return (result >= 0);
}

bool GenetlinkSocket::Register(nl_recvmsg_msg_cb_t func, void *arg) const
{
    if (!initialized_) {
        return false;
    }

    return (nl_socket_modify_cb(sock_, NL_CB_MSG_IN, NL_CB_CUSTOM, func, arg) == 0);
}

bool GenetlinkSocket::Receive() const
{
    if (!initialized_) {
        return false;
    }

    return (nl_recvmsgs_default(sock_) == 0);
}
