/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_policy_handler.h
*
*   DESCRIPTION: This file contains functions' declaration and macros to add policy in SPD
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

#ifndef __IPR_POLICYADD_H
#define __IPR_POLICYADD_H

#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <linux/xfrm.h>

#include <lteTypes.h>
#include <ipr_cwmp_netlink.h>

#define XFRM_TMPLS_BUF_SIZE 1024

typedef struct {
        struct nlmsghdr                 n;
        struct xfrm_userpolicy_info     xpinfo;
        UInt8                            buf[ BUF_SIZE ];
    } add_policy_req_t;

typedef struct {
        struct nlmsghdr         n;
        struct xfrm_userpolicy_id   xpid;
    } del_policy_req_t;


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_in
*
*  DESCRIPTION: This function adds an In policy in SPD.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_add_in(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) ;
/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_fwd
*
*  DESCRIPTION: This function adds an fwd policy in SPD.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_add_fwd(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) ;



/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_add_out
*
*  DESCRIPTION: This function adds an Out policy in SPD.
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_policy_add_out(UInt8* src, UInt8* dst, UInt8* tmpl_src, UInt8* tmpl_dst, UInt8 src_prefix, UInt8 dst_prefix) ;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_in_delete
*
*  DESCRIPTION: This function deletes an in policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_in_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) ;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_fwd_delete
*
*  DESCRIPTION: This function deletes an fwd policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_fwd_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) ;



/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_policy_out_delete
*
*  DESCRIPTION: This function deletes an out policy from PD (or fails if policy is not present in PD).
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_policy_out_del(UInt8* src, UInt8* dst, UInt8 src_prefix, UInt8 dst_prefix) ;

#endif
