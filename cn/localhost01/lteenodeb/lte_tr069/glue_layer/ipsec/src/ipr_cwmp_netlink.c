/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_netlink.c
*
*   DESCRIPTION: This file contains netlink socket functions
*
*   REVISION HISTORY:
*
*   NAME              DATE                REFERENCE              REASON
*
*   Anant K Sharma    Nov 17, 2012                              Initial Creation
*
*
*   All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/uio.h>

#include <ipr_cwmp_netlink.h>
#include <lteTypes.h>
#include <ipr_cwmp_enb_wrapper.h>

typedef int SInt32;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_rtnl_close
*
*  DESCRIPTION: This function is a wrapper to close API of linux.
*
*  RETURNS: Success IPR_CWMP_SUCCESS; Failure IPR_CWMP_FAILURE
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_rtnl_close(rtnetlink_handle_t *rth)
{
	if(-1 == ENB_CLOSE(rth->fd))
	{
		return IPR_CWMP_FAILURE; 
	}
	else
	{
		return IPR_CWMP_SUCCESS;
	}
}


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_rtnl_open_byproto
*
*  DESCRIPTION: This function is for opening a netlink socket.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_rtnl_open_byproto(rtnetlink_handle_t *rth, UInt32 subscriptions, UInt32 protocol)
{
	UInt32 addr_len;
	UInt32 sndbuf = 32768;
	UInt32 rcvbuf = 32768;
	SInt32 fd = 0;

	ENB_MEMSET(rth, 0, sizeof(rtnetlink_handle_t));

	//coverity fix
	fd = ENB_SOCKET(AF_NETLINK, SOCK_RAW, protocol);
	if (fd  < 0)
	{
		perror ("socket");
		return IPR_CWMP_FAILURE;  
	}

	rth->fd = fd;
	if (ENB_SETSOCKOPT(rth->fd,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) {
		return IPR_CWMP_FAILURE;
	}

	if (ENB_SETSOCKOPT(rth->fd,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {
		return IPR_CWMP_FAILURE;
	}

	ENB_MEMSET(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = subscriptions;

	if (ENB_BIND(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0) {
		return IPR_CWMP_FAILURE;
	}
	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local, &addr_len) < 0) {
		return IPR_CWMP_FAILURE;
	}
	if (addr_len != sizeof(rth->local)) {
		return IPR_CWMP_FAILURE;
	}
	if (rth->local.nl_family != AF_NETLINK) {
		return IPR_CWMP_FAILURE;
	}
	rth->seq = time(PNULL);
	return IPR_CWMP_SUCCESS;
}


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_rtnl_talk
*
*  DESCRIPTION: This function is for sending message to kernel via netlink sockets.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_rtnl_talk(rtnetlink_handle_t *rtnl, struct nlmsghdr *n, pid_t peer, UInt32 groups, struct nlmsghdr *answer)
{
	SInt32 status;
	UInt32 seq;
	struct nlmsghdr *h;
	struct sockaddr_nl nladdr;
	struct iovec iov = { (void*)n, n->nlmsg_len };
	UInt8   *buf = NULL;

	struct msghdr msg = {
		(void*)&nladdr, 
		sizeof(nladdr),
		&iov,
		1,
		PNULL,	
		0,
		0
	};
	//cov
	buf = (UInt8 *)ALLOCATE( 16384);
	if(NULL == buf)
	{
		DEALLOCATE(iov.iov_base);
		return IPR_CWMP_FAILURE;
	}

	ENB_MEMSET(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = peer;
	nladdr.nl_groups = groups;

	n->nlmsg_seq = seq = ++rtnl->seq;

	if (answer == PNULL)
		n->nlmsg_flags |= NLM_F_ACK;

	status = ENB_SENDMSG(rtnl->fd, &msg, 0);

	if (status < 0) {
		DEALLOCATE(buf);	
		return IPR_CWMP_FAILURE;
	}

	ENB_MEMSET(buf,0,sizeof(16384));
	iov.iov_base = buf;

	while (1) {
		iov.iov_len = sizeof(buf);
		status = ENB_RECVMSG(rtnl->fd, &msg, 0);

		if (status < 0) {
		
                /*SPR 20007 :CID 109689 Fix START*/ 	
                /*Code Deleted*/
                /*SPR 20007 :CID 109689 Fix END*/ 	
			continue;
		}
		if (status == 0) {
			DEALLOCATE(iov.iov_base);
			return IPR_CWMP_FAILURE;
		}
		if (msg.msg_namelen != sizeof(nladdr)) {

			DEALLOCATE(iov.iov_base);
			return IPR_CWMP_FAILURE;
		}
		for (h = (struct nlmsghdr*)buf; status >= sizeof(*h); ) {
			//	SInt32 err;
			SInt32 len = h->nlmsg_len;
			SInt32 l = len - sizeof(*h);

            if (l<0 || len>status) {
                /* SPR_19838 :Coverity_12913 Fix Start */
                /* Code removed */
                /* SPR_19838 :Coverity_12913 Fix Start */
                DEALLOCATE(iov.iov_base);
                return IPR_CWMP_FAILURE;
            }

			if (nladdr.nl_pid != peer ||
					h->nlmsg_pid != rtnl->local.nl_pid ||
					h->nlmsg_seq != seq) {
				continue;
			}

			if (h->nlmsg_type == NLMSG_ERROR) {
				struct nlmsgerr *err = (struct nlmsgerr*)NLMSG_DATA(h);
				if (l < sizeof(struct nlmsgerr)) {
				} else {
					errno = -err->error;
					if (errno == 0) {
						if (answer)
						{
							ENB_MEMCPY(answer, h, h->nlmsg_len);
							DEALLOCATE(buf);
							return IPR_CWMP_SUCCESS;
						}
					}
				}
				DEALLOCATE(iov.iov_base);
				return IPR_CWMP_FAILURE;
			}
			if (answer) {
				ENB_MEMCPY(answer, h, h->nlmsg_len);
				return IPR_CWMP_SUCCESS;
			}


			status -= NLMSG_ALIGN(len);
			h = (struct nlmsghdr*)((UInt8*)h + NLMSG_ALIGN(len));
		}
		if (msg.msg_flags & MSG_TRUNC) {
			continue;
		}
		if (status) {
			DEALLOCATE(iov.iov_base);
			return IPR_CWMP_FAILURE;
		}
	}
	DEALLOCATE(buf);
}



/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_addattr_l
*
*  DESCRIPTION: This function is for adding netlink message attribute.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_addattr_l(struct nlmsghdr *n, UInt32 maxlen, UInt32 type, const void *data, 
	      UInt32 alen)
{
	UInt32 len = RTA_LENGTH(alen);
	struct rtattr *rta;

	if (NLMSG_ALIGN(n->nlmsg_len) + len > maxlen) {
		return IPR_CWMP_FAILURE;
	}
	rta = (struct rtattr*)(((UInt8*)n) + NLMSG_ALIGN(n->nlmsg_len));
	rta->rta_type = type;
	rta->rta_len = len;
	ENB_MEMCPY(RTA_DATA(rta), data, alen);
	n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + len;
	return IPR_CWMP_SUCCESS;
}
