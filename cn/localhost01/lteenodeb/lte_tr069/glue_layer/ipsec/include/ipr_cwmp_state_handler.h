/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_state_handler.h
*
*   DESCRIPTION: This file contains functions' declaration and macros to add state in SADB
*
*   REVISION HISTORY:
*
*   NAME         DATE                REFERENCE              REASON
*
*   Anant K Sharma    Nov 17, 2012   TR-069 Amendment 4   Initial Creation
*   Anant K Sharma    JAN 25, 2013                              changed ipr_cwmp_add_state_3des and ipr_cwmp_mod_state to support
*                                                               lifetime cfg
*
*
*   All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/

#ifndef __IPR_STATEADD_H
#define __IPR_STATEADD_H

#include <lteTypes.h>
#include <linux/xfrm.h>
#include <linux/netlink.h>


#define XFRM_ALGO_KEY_BUF_SIZE 512

typedef struct {
        struct nlmsghdr     n;
        struct xfrm_usersa_info xsinfo;
        UInt8            buf[BUF_SIZE];
    } add_mod_stat_req_t;

typedef struct {
        struct nlmsghdr     n;
        struct xfrm_usersa_id   xsid;
    } del_stat_req_t;

typedef struct {
        struct xfrm_algo alg;
        UInt8 buf[XFRM_ALGO_KEY_BUF_SIZE];
    } alg_t;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_add_state_3des
*
*  DESCRIPTION: This function adds a state in SADB and uses security algo 3des
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_add_state_3des(UInt8* src, UInt8* dst, UInt64 ipsec_rekey_lifetime_time, UInt8* auth_algo, UInt8* auth_key, UInt8* enc_algo, UInt8* enc_key) ;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_del_state
*
*  DESCRIPTION: This function deletes a state from SADB
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_del_state( UInt8* dst) ;

/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_mod_state
*
*  DESCRIPTION: This function updates a state in SADB
*
*  RETURNS: IPR_CWMP_SUCCESS if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_mod_state(UInt8* src, UInt8* dst, UInt64 ipsec_rekey_lifetime_time, UInt8* auth_algo, UInt8* auth_key) ;

#endif
