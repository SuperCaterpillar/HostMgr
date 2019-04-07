#ifndef NETLINKKERNEL_H
#define NETLINKKERNEL_H

/* attributes of acpi_genl_family */
enum {
    HOSTMGR_GENL_ATTR_UNSPEC,
    HOSTMGR_GENL_TEST_ATTR_EVENT,	/* ACPI event info needed by user space */
    __HOSTMGR_GENL_ATTR_MAX,
};
#define HOSTMGR_GENL_ATTR_MAX (__HOSTMGR_GENL_ATTR_MAX - 1)

/* commands supported by the acpi_genl_family */
enum {
    HOSTMGR_GENL_CMD_UNSPEC,
    HOSTMGR_GENL_TEST_CMD_EVENT,	/* kernel->user notifications for ACPI events */
    __HOSTMGR_GENL_CMD_MAX,
};
#define HOSTMGR_GENL_CMD_MAX (__HOSTMGR_GENL_CMD_MAX - 1)

#define HOSTMGR_GENL_FAMILY_NAME		"hostMgr_event"
#define HOSTMGR_GENL_VERSION		0x01
#define HOSTMGR_GENL_MCAST_GROUP_NAME 	"hostMgr_mc_group"


#endif // NETLINKKERNEL_H
