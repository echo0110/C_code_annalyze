/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_netlink.h
*
*   DESCRIPTION: This file contains netlink functions' declaration and macros
*
*   REVISION HISTORY:
*
*   NAME         DATE                REFERENCE              REASON
*
*   Anant K Sharma    Nov 17, 2012   TR-069 Amendment 4   Initial Creation
*
*
*   All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/

#ifndef __IPR_NETLINK_H
#define __IPR_NETLINK_H

#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <lteTypes.h>
#include <ipr_cwmp_enb_wrapper.h>


#define BUF_SIZE 2048

extern UInt32 preferred_family;

#ifndef IPPROTO_ESP
#define IPPROTO_ESP 50
#endif


typedef struct
{
	UInt32			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
} rtnetlink_handle_t;

ipr_cwmp_ret_t ipr_cwmp_rtnl_open_byproto(rtnetlink_handle_t *rth, UInt32 subscriptions, UInt32 protocol);
ipr_cwmp_ret_t ipr_cwmp_rtnl_close(rtnetlink_handle_t *rth);
ipr_cwmp_ret_t ipr_cwmp_rtnl_talk(rtnetlink_handle_t *rtnl, struct nlmsghdr *n, pid_t peer, UInt32 groups, struct nlmsghdr *answer);
ipr_cwmp_ret_t ipr_cwmp_addattr_l(struct nlmsghdr *n, UInt32 maxlen, UInt32 type, const void *data, UInt32 alen);


#endif /* __IPR_NETLINK_H */

