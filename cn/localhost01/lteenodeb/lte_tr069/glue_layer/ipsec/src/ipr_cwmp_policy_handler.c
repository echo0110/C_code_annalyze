/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_policy_handler.c
*
*   DESCRIPTION: This file contains functions to add and delete policy in SPD
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
#include <string.h>
#include <stdlib.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/xfrm.h>

#include <ipr_cwmp_netlink.h>
#include <lteTypes.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <ipr_cwmp_policy_handler.h>


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_in
*
*  DESCRIPTION: This function adds an In policy in SPD.
*
*  RETURNS: SInt8 value 0 if success -1 if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_add_in(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) 
{
    UInt32 preferred_family = AF_INET;

    rtnetlink_handle_t rth;
    add_policy_req_t req;

    UInt8 tmpls_buf[XFRM_TMPLS_BUF_SIZE];
    UInt32 tmpls_len = 0;
    UInt32 idx;

    ENB_MEMSET(&req, 0, sizeof(req));
    ENB_MEMSET(&tmpls_buf, 0, sizeof(tmpls_buf));

    req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xpinfo));
    req.n.nlmsg_type = XFRM_MSG_NEWPOLICY;
    req.n.nlmsg_flags = NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL;

    req.xpinfo.sel.family = preferred_family;
    req.xpinfo.lft.soft_byte_limit = XFRM_INF;
    req.xpinfo.lft.hard_byte_limit = XFRM_INF;
    req.xpinfo.lft.soft_packet_limit = XFRM_INF;
    req.xpinfo.lft.hard_packet_limit = XFRM_INF;

    req.xpinfo.dir = XFRM_POLICY_IN;

    struct xfrm_selector *sel = &req.xpinfo.sel;

	if(src)
    sel->saddr.a4 = ENB_INET_ADDR((const char*)src); 
    sel->family = preferred_family;
    sel->prefixlen_s = src_prefix;
	if(dst)
    sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
    sel->prefixlen_d = dst_prefix;

    struct xfrm_user_tmpl *tmpl;

    for ( idx = 0; idx < 1; idx++ ) {
        // adding to buffer:
        if ( tmpls_len + sizeof(*tmpl) > sizeof( tmpls_buf ) )
        { 
			return IPR_CWMP_FAILURE;
		}
        tmpl = (struct xfrm_user_tmpl *)((UInt8 *)tmpls_buf + tmpls_len);

        // template properties:
        tmpl->family = preferred_family;
        tmpl->mode = 1;
        tmpl->reqid = 0x0;
        tmpl->optional = 0; 
		if(tmpl_src)
        tmpl->saddr.a4 = ENB_INET_ADDR((const char*)tmpl_src); 
		if(tmpl_dst)
        tmpl->id.daddr.a4 = ENB_INET_ADDR((const char*)tmpl_dst);
        tmpl->id.proto = IPPROTO_ESP;

        // set all algos to infinity:
        tmpl->aalgos = (~(__u32)0);
        tmpl->ealgos = (~(__u32)0);
        tmpl->calgos = (~(__u32)0);

        tmpls_len += sizeof( *tmpl );
    }
    // copy and allign template(s) to req
    if (tmpls_len > 0)
        { ipr_cwmp_addattr_l(&req.n, sizeof(req), XFRMA_TMPL, (void *)tmpls_buf, tmpls_len); }

    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

    if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, PNULL) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

    ipr_cwmp_rtnl_close(&rth);

    return IPR_CWMP_SUCCESS;
}



/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_out
*
*  DESCRIPTION: This function adds an Out policy in SPD.
*
*  RETURNS: SInt8 value 0 if success -1 if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_policy_add_out(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) 
{
	if(src == PNULL && dst == PNULL)
		return IPR_CWMP_FAILURE;
    UInt32 preferred_family = AF_INET;

    rtnetlink_handle_t rth;
    add_policy_req_t req;

    UInt8 tmpls_buf[XFRM_TMPLS_BUF_SIZE];
    UInt32 tmpls_len = 0;
    UInt32 idx;

    ENB_MEMSET(&req, 0, sizeof(req));
    ENB_MEMSET(&tmpls_buf, 0, sizeof(tmpls_buf));

    req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xpinfo));
    req.n.nlmsg_flags = NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL;
    req.n.nlmsg_type = XFRM_MSG_NEWPOLICY;
    req.xpinfo.sel.family = preferred_family;

    req.xpinfo.lft.soft_byte_limit = XFRM_INF;
    req.xpinfo.lft.hard_byte_limit = XFRM_INF;
    req.xpinfo.lft.soft_packet_limit = XFRM_INF;
    req.xpinfo.lft.hard_packet_limit = XFRM_INF;

    req.xpinfo.dir = XFRM_POLICY_OUT;

    struct xfrm_selector *sel = &req.xpinfo.sel;

	if(src)
   		sel->saddr.a4 = ENB_INET_ADDR((const char*)src); 
    sel->family = preferred_family;
    sel->prefixlen_s = src_prefix;
	if(dst)
    	sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
    sel->prefixlen_d = dst_prefix;

    struct xfrm_user_tmpl *tmpl;

    for ( idx = 0; idx < 1; idx++ ) {
        // adding to buffer:
        if ( tmpls_len + sizeof(*tmpl) > sizeof( tmpls_buf ) )
        { 
			return IPR_CWMP_FAILURE;
		}
        tmpl = (struct xfrm_user_tmpl *)((UInt8 *)tmpls_buf + tmpls_len);

        // template properties:
        tmpl->family = preferred_family;
        tmpl->mode = 1;
        tmpl->reqid = 0x0;
        tmpl->optional = 0;
		if(tmpl_src)
        tmpl->saddr.a4 = ENB_INET_ADDR((const char*)tmpl_src);
		if(tmpl_dst)
        tmpl->id.daddr.a4 = ENB_INET_ADDR((const char*)tmpl_dst);
        tmpl->id.proto = IPPROTO_ESP;

        // set all algos to infinity:
        tmpl->aalgos = (~(__u32)0);
        tmpl->ealgos = (~(__u32)0);
        tmpl->calgos = (~(__u32)0);

        tmpls_len += sizeof( *tmpl );
    }
    // copy and allign template(s) to req
    if (tmpls_len > 0)
        { ipr_cwmp_addattr_l(&req.n, sizeof(req), XFRMA_TMPL, (void *)tmpls_buf, tmpls_len); }

    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	 }

    if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, PNULL) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

    ipr_cwmp_rtnl_close(&rth);

    return IPR_CWMP_SUCCESS;
}


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_in_del
*
*  DESCRIPTION: This function deletes an in policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: 0 if success -1 if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_in_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) 
{
	if(src == PNULL && dst == PNULL)
		return IPR_CWMP_FAILURE;

	UInt32 preferred_family = AF_INET;

	rtnetlink_handle_t rth = {0};
	del_policy_req_t req;

	ENB_MEMSET(&req, 0, sizeof(req));

	req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xpid) );
	req.n.nlmsg_flags = NLM_F_REQUEST;
	req.n.nlmsg_type = XFRM_MSG_DELPOLICY;

	if ( !req.xpid.index ) {
		struct xfrm_selector *sel = &req.xpid.sel;

		sel->family = preferred_family;

		if (src)
			sel->saddr.a4 = ENB_INET_ADDR((const char*)src);
		sel->family = preferred_family;
		sel->prefixlen_s = src_prefix;

		if (dst)
			sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
		sel->prefixlen_d = dst_prefix;
	}

	if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}



	void *res_nlbuf = PNULL;

	if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, res_nlbuf) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}


	ipr_cwmp_rtnl_close(&rth);

	return IPR_CWMP_SUCCESS;
}

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_out_del
*
*  DESCRIPTION: This function deletes an out policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: 0 if success -1 if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_out_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) 
{

	if(src == PNULL && dst == PNULL)
		return IPR_CWMP_FAILURE;

	UInt32 preferred_family = AF_INET;

	rtnetlink_handle_t rth = {0};
	del_policy_req_t req;

	ENB_MEMSET(&req, 0, sizeof(req));

	req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xpid) );
	req.n.nlmsg_flags = NLM_F_REQUEST;
	req.n.nlmsg_type = XFRM_MSG_DELPOLICY;

	req.xpid.dir = XFRM_POLICY_OUT; 

	if ( !req.xpid.index ) {
		struct xfrm_selector *sel = &req.xpid.sel;

		sel->family = preferred_family;

		if (src)	
			sel->saddr.a4 = ENB_INET_ADDR((const char*)src);
		sel->family = preferred_family;
		sel->prefixlen_s = src_prefix;

		if (dst)
			sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
		sel->prefixlen_d = dst_prefix;
	}

	if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}


	void *res_nlbuf = PNULL;

	if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, res_nlbuf) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

	ipr_cwmp_rtnl_close(&rth);

	return IPR_CWMP_SUCCESS;
}
/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_fwd
*
*  DESCRIPTION: This function adds an fwd policy in SPD.
*
*  RETURNS: SInt8 value 0 if success -1 if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_add_fwd(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) 
{
    UInt32 preferred_family = AF_INET;

    rtnetlink_handle_t rth;
    add_policy_req_t req;

    UInt8 tmpls_buf[XFRM_TMPLS_BUF_SIZE];
    UInt32 tmpls_len = 0;
    UInt32 idx;

    ENB_MEMSET(&req, 0, sizeof(req));
    ENB_MEMSET(&tmpls_buf, 0, sizeof(tmpls_buf));

    req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xpinfo));
    req.n.nlmsg_type = XFRM_MSG_NEWPOLICY;
    req.n.nlmsg_flags = NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL;

    req.xpinfo.sel.family = preferred_family;
    req.xpinfo.lft.soft_byte_limit = XFRM_INF;
    req.xpinfo.lft.hard_byte_limit = XFRM_INF;
    req.xpinfo.lft.soft_packet_limit = XFRM_INF;
    req.xpinfo.lft.hard_packet_limit = XFRM_INF;

    req.xpinfo.dir = XFRM_POLICY_FWD;

    struct xfrm_selector *sel = &req.xpinfo.sel;

	if(src)
    sel->saddr.a4 = ENB_INET_ADDR((const char*)src); 
    sel->family = preferred_family;
    sel->prefixlen_s = src_prefix;
	if(dst)
    sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
    sel->prefixlen_d = dst_prefix;

    struct xfrm_user_tmpl *tmpl;

    for ( idx = 0; idx < 1; idx++ ) {
        // adding to buffer:
        if ( tmpls_len + sizeof(*tmpl) > sizeof( tmpls_buf ) )
        { 
			return IPR_CWMP_FAILURE;
		}
        tmpl = (struct xfrm_user_tmpl *)((UInt8 *)tmpls_buf + tmpls_len);

        // template properties:
        tmpl->family = preferred_family;
        tmpl->mode = 1;
        tmpl->reqid = 0x0;
        tmpl->optional = 0; 
		if(tmpl_src)
        tmpl->saddr.a4 = ENB_INET_ADDR((const char*)tmpl_src); 
		if(tmpl_dst)
        tmpl->id.daddr.a4 = ENB_INET_ADDR((const char*)tmpl_dst);
        tmpl->id.proto = IPPROTO_ESP;

        // set all algos to infinity:
        tmpl->aalgos = (~(__u32)0);
        tmpl->ealgos = (~(__u32)0);
        tmpl->calgos = (~(__u32)0);

        tmpls_len += sizeof( *tmpl );
    }
    // copy and allign template(s) to req
    if (tmpls_len > 0)
        { ipr_cwmp_addattr_l(&req.n, sizeof(req), XFRMA_TMPL, (void *)tmpls_buf, tmpls_len); }

    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

    if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, PNULL) < 0)
    { 
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}

    ipr_cwmp_rtnl_close(&rth);

    return IPR_CWMP_SUCCESS;
}
/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_fwd_del
*
*  DESCRIPTION: This function deletes an fwd policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: 0 if success -1 if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_fwd_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) 
{

	if(src == PNULL && dst == PNULL)
		return IPR_CWMP_FAILURE;

	UInt32 preferred_family = AF_INET;

	rtnetlink_handle_t rth = {0};
	del_policy_req_t req;

	ENB_MEMSET(&req, 0, sizeof(req));

	req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xpid) );
	req.n.nlmsg_flags = NLM_F_REQUEST;
	req.n.nlmsg_type = XFRM_MSG_DELPOLICY;

	req.xpid.dir = XFRM_POLICY_FWD; 

	if ( !req.xpid.index ) {
		struct xfrm_selector *sel = &req.xpid.sel;

		sel->family = preferred_family;

		if (src)	
			sel->saddr.a4 = ENB_INET_ADDR((const char*)src);
		sel->family = preferred_family;
		sel->prefixlen_s = src_prefix;

		if (dst)
			sel->daddr.a4 = ENB_INET_ADDR((const char*)dst); 
		sel->prefixlen_d = dst_prefix;
	}

	if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}



	void *res_nlbuf = PNULL;

	if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, res_nlbuf) < 0)
	{    
		ENB_CLOSE(rth.fd); 
		return IPR_CWMP_FAILURE;
	}


	ipr_cwmp_rtnl_close(&rth);

	return IPR_CWMP_SUCCESS;
}
