/********************************************************************************
*
*   FILE NAME   : ipr_cwmp_state_handler.c
*
*   DESCRIPTION: This file contains netlink functions to add and delete state in SADB
*
*   REVISION HISTORY:
*
*   NAME              DATE                REFERENCE              REASON
*
*   Anant K Sharma    Nov 17, 2012                              Initial Creation
*   Anant K Sharma    JAN 25, 2013                              changed ipr_cwmp_add_state_3des and ipr_cwmp_mod_state to support
*                                                               lifetime cfg and made enc key and auth key args of this function
*
*
*   All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <ipr_cwmp_state_handler.h>


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_add_state_3des
*
*  DESCRIPTION: This function adds a state in SADB and uses security algo 3des
*
*  RETURNS: if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_add_state_3des(UInt8* src, UInt8* dst, UInt64 ipsec_rekey_lifetime_time, UInt8* auth_algo, UInt8* authkey, UInt8* enc_algo, UInt8* enckey) 
{
	if (src == PNULL || dst == PNULL || authkey == PNULL ||  enckey == PNULL)
		return IPR_CWMP_FAILURE;

    rtnetlink_handle_t rth = {0};
    add_mod_stat_req_t req;

    ENB_MEMSET( &req, 0, sizeof(req) );

    // Netlink Header:
    req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xsinfo) );
    req.n.nlmsg_flags = NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL;
    req.n.nlmsg_type = XFRM_MSG_NEWSA;


    req.xsinfo.family = AF_INET;       
	if (src)
    req.xsinfo.saddr.a4 = ENB_INET_ADDR((const char*)src);
     req.xsinfo.mode = (__u8) 1;         

    // ID:
	if (dst)
    req.xsinfo.id.daddr.a4 = ENB_INET_ADDR((const char*)dst);
    req.xsinfo.id.proto = IPPROTO_ESP; 
    req.xsinfo.id.spi = (__u32) 0x0010001; 

    // LFT:
    req.xsinfo.lft.soft_byte_limit = XFRM_INF;
    req.xsinfo.lft.hard_byte_limit = XFRM_INF;
    req.xsinfo.lft.soft_packet_limit = XFRM_INF;
    req.xsinfo.lft.hard_packet_limit = XFRM_INF;
    //IPSEC phase 2 code starts
    if(ipsec_rekey_lifetime_time)
        req.xsinfo.lft.soft_add_expires_seconds = (__u64)ipsec_rekey_lifetime_time;
    //IPSEC phase 2 code ends

    // XFRMA_ALG_CRYPT:
    alg_t des;
    ENB_MEMSET(&des, 0, sizeof( alg_t ));
    ENB_STRNCPY((char *)(des.alg.alg_name),(const char *) "des3_ede", sizeof(des.alg.alg_name));

    // key = 0x79b5d6e36dda9da4982f51293767d6108649ced573c8349e
    //key code starts
    if(strlen((const char *)enckey) != 50)
    {
	    ENB_PRINTF ("\n Size of encryption key is not correct.\n ");
	    ENB_PRINTF ("\n Size of encryption key is %u.\n ", (UInt32)(strlen((const char *)enckey)));
	    return IPR_CWMP_FAILURE;
    }
    UInt8* enc_key = (UInt8*)ALLOCATE(52);
    if(NULL == enc_key)
    {
	    ENB_PRINTF ("\n Malloc returned failure\n ");
	    exit(0);    
    }
    strncpy((char *)enc_key,(const char *)enckey, 52);

    UInt8 key_byte1[5];
    UInt8 key_byte2[3];
    UInt8 key_byte3[3];
    UInt8 key_byte4[3];
    UInt8 key_byte5[3];
    UInt8 key_byte6[3];
    UInt8 key_byte7[3];
    UInt8 key_byte8[3];
    UInt8 key_byte9[3];
    UInt8 key_byte10[3];
    UInt8 key_byte11[3];
    UInt8 key_byte12[3];
    UInt8 key_byte13[3];
    UInt8 key_byte14[3];
    UInt8 key_byte15[3];
    UInt8 key_byte16[3];
    UInt8 key_byte17[3];
    UInt8 key_byte18[3];
    UInt8 key_byte19[3];
    UInt8 key_byte20[3];
    UInt8 key_byte21[3];
    UInt8 key_byte22[3];
    UInt8 key_byte23[3];
    UInt8 key_byte24[3];

    memset(key_byte1, 0, sizeof(key_byte1));
    memset(key_byte2, 0, sizeof(key_byte2));
    memset(key_byte3, 0, sizeof(key_byte2));
    memset(key_byte4, 0, sizeof(key_byte2));
    memset(key_byte5, 0, sizeof(key_byte2));
    memset(key_byte6, 0, sizeof(key_byte2));
    memset(key_byte7, 0, sizeof(key_byte2));
    memset(key_byte8, 0, sizeof(key_byte2));
    memset(key_byte9, 0, sizeof(key_byte2));
    memset(key_byte10, 0, sizeof(key_byte2));
    memset(key_byte11, 0, sizeof(key_byte2));
    memset(key_byte12, 0, sizeof(key_byte2));
    memset(key_byte13, 0, sizeof(key_byte2));
    memset(key_byte14, 0, sizeof(key_byte2));
    memset(key_byte15, 0, sizeof(key_byte2));
    memset(key_byte16, 0, sizeof(key_byte2));
    memset(key_byte17, 0, sizeof(key_byte2));
    memset(key_byte18, 0, sizeof(key_byte2));
    memset(key_byte19, 0, sizeof(key_byte2));
    memset(key_byte20, 0, sizeof(key_byte2));
    memset(key_byte21, 0, sizeof(key_byte2));
    memset(key_byte22, 0, sizeof(key_byte2));
    memset(key_byte23, 0, sizeof(key_byte2));
    memset(key_byte24, 0, sizeof(key_byte2));

    strncpy((char *)key_byte1, (const char *)enc_key, 4);
    strncpy((char *)key_byte2,(const char *) (enc_key + 4), 2);
    strncpy((char *)key_byte3,(const char *) enc_key + 6, 2);
    strncpy((char *)key_byte4, (const char *)enc_key + 8, 2);
    strncpy((char *)key_byte5, (const char *)enc_key + 10, 2);
    strncpy((char *)key_byte6,(const char *) enc_key + 12, 2);
    strncpy((char *)key_byte7, (const char *)enc_key + 14, 2);
    strncpy((char *)key_byte8,(const char *) enc_key + 16, 2);
    strncpy((char *)key_byte9,(const char *) enc_key + 18, 2);
    strncpy((char *)key_byte10,(const char *) enc_key + 20, 2);
    strncpy((char *)key_byte11,(const char *) enc_key + 22, 2);
    strncpy((char *)key_byte12, (const char *)enc_key + 24, 2);
    strncpy((char *)key_byte13, (const char *)enc_key + 26, 2);
    strncpy((char *)key_byte14,(const char *) enc_key + 28, 2);
    strncpy((char *)key_byte15, (const char *)enc_key + 30, 2);
    strncpy((char *)key_byte16, (const char *)enc_key + 32, 2);
    strncpy((char *)key_byte17, (const char *)enc_key + 34, 2);
    strncpy((char *)key_byte18, (const char *)enc_key + 36, 2);
    strncpy((char *)key_byte19, (const char *)enc_key + 38, 2);
    strncpy((char *)key_byte20, (const char *)enc_key + 40, 2);
    strncpy((char *)key_byte21, (const char *)enc_key + 42, 2);
    strncpy((char *)key_byte22,(const char *) enc_key + 44, 2);
    strncpy((char *)key_byte23,(const char *) enc_key + 46, 2);
    strncpy((char *)key_byte24,(const char *) enc_key + 48, 2);

    des.alg.alg_key_len = 192;
    des.alg.alg_key[0] = strtol((const char *)key_byte1, NULL, 16);
    des.alg.alg_key[1] = strtol((const char *)key_byte2, NULL, 16);
    des.alg.alg_key[2] = strtol((const char *)key_byte3, NULL, 16);
    des.alg.alg_key[3] = strtol((const char *)key_byte4, NULL, 16);
    des.alg.alg_key[4] = strtol((const char *)key_byte5, NULL, 16);
    des.alg.alg_key[5] = strtol((const char *)key_byte6, NULL, 16);
    des.alg.alg_key[6] = strtol((const char *)key_byte7, NULL, 16);
    des.alg.alg_key[7] = strtol((const char *)key_byte8, NULL, 16);
    des.alg.alg_key[8] = strtol((const char *)key_byte9, NULL, 16);
    des.alg.alg_key[9] = strtol((const char *)key_byte10, NULL, 16);
    des.alg.alg_key[10] = strtol((const char *)key_byte11, NULL, 16);
    des.alg.alg_key[11] = strtol((const char *)key_byte12, NULL, 16);
    des.alg.alg_key[12] = strtol((const char *)key_byte13, NULL, 16);
    des.alg.alg_key[13] = strtol((const char *)key_byte14, NULL, 16);
    des.alg.alg_key[14] = strtol((const char *)key_byte15, NULL, 16);
    des.alg.alg_key[15] = strtol((const char *)key_byte16, NULL, 16);
    des.alg.alg_key[16] = strtol((const char *)key_byte17, NULL, 16);
    des.alg.alg_key[17] = strtol((const char *)key_byte18, NULL, 16);
    des.alg.alg_key[18] = strtol((const char *)key_byte19, NULL, 16);
    des.alg.alg_key[19] = strtol((const char *)key_byte20, NULL, 16);
    des.alg.alg_key[20] = strtol((const char *)key_byte21, NULL, 16);
    des.alg.alg_key[21] = strtol((const char *)key_byte22, NULL, 16);
    des.alg.alg_key[22] = strtol((const char *)key_byte23, NULL, 16);
    des.alg.alg_key[23] = strtol((const char *)key_byte24, NULL, 16);

    UInt32 len = sizeof(struct xfrm_algo) + des.alg.alg_key_len;
    ipr_cwmp_addattr_l(&req.n, sizeof(req.buf), XFRMA_ALG_CRYPT, (void *)&des, len);

    // =- =- =- =- MD5 -= -= -= -=
    alg_t md5;
    ENB_MEMSET(&md5, 0, sizeof(alg_t));
    ENB_STRNCPY((char *)md5.alg.alg_name,(const char *) "md5", sizeof(md5.alg.alg_name));

    // auth md5 0x982f51293767d610

    UInt8* auth_key = (UInt8*)ALLOCATE( 20);
    if(NULL == auth_key)
    {
	    ENB_PRINTF ("\n Malloc returned failure\n ");
	    exit(0);    
    }
    strncpy((char *)auth_key,(const char *) authkey, 20);

    if(strlen((const char *)authkey) != 18)
    {
        ENB_PRINTF ("\n Size of auth key is not correct.\n ");
        ENB_PRINTF ("\n Size of auth key is %u.\n ", (UInt32)(strlen((const char *)authkey)));
        //coverity fix
	DEALLOCATE(auth_key);
	DEALLOCATE(enc_key);
	return IPR_CWMP_FAILURE;
    }
    memset(key_byte1, 0, sizeof(key_byte1));
    memset(key_byte2, 0, sizeof(key_byte2));
    memset(key_byte3, 0, sizeof(key_byte2));
    memset(key_byte4, 0, sizeof(key_byte2));
    memset(key_byte5, 0, sizeof(key_byte2));
    memset(key_byte6, 0, sizeof(key_byte2));
    memset(key_byte7, 0, sizeof(key_byte2));

    strncpy((char *)key_byte1, (const char *)auth_key, 4);
    strncpy((char *)key_byte2, (const char *)(auth_key + 4), 2);
    strncpy((char *)key_byte3, (const char *)auth_key + 6, 2);
    strncpy((char *)key_byte4, (const char *)auth_key + 8, 2);
    strncpy((char *)key_byte5, (const char *)auth_key + 10, 2);
    strncpy((char *)key_byte6, (const char *)auth_key + 12, 2);
    strncpy((char *)key_byte7,(const char *) auth_key + 14, 2);

    md5.alg.alg_key_len = 64;
    md5.alg.alg_key[0] = strtol((const char *)key_byte1, NULL, 16);
    md5.alg.alg_key[1] = strtol((const char *)key_byte2, NULL, 16);
    md5.alg.alg_key[2] = strtol((const char *)key_byte3, NULL, 16);
    md5.alg.alg_key[3] = strtol((const char *)key_byte4, NULL, 16);
    md5.alg.alg_key[4] = strtol((const char *)key_byte5, NULL, 16);
    md5.alg.alg_key[5] = strtol((const char *)key_byte6, NULL, 16);
    md5.alg.alg_key[6] = strtol((const char *)key_byte7, NULL, 16);
    md5.alg.alg_key[7] = strtol((const char *)key_byte8, NULL, 16);

    
    len = sizeof(struct xfrm_algo) + md5.alg.alg_key_len;
    ipr_cwmp_addattr_l(&req.n, sizeof(req.buf), XFRMA_ALG_AUTH, (void *)&md5, len);

    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    {
	    DEALLOCATE(enc_key);    
	    DEALLOCATE(auth_key);    
	    close(rth.fd);
	    return IPR_CWMP_FAILURE;
    }
    if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, PNULL) < 0)
    {
	    DEALLOCATE(enc_key);    
	    DEALLOCATE(auth_key);    
	    close(rth.fd);
	    return IPR_CWMP_FAILURE;
    }
    ipr_cwmp_rtnl_close(&rth);
    DEALLOCATE(enc_key);
    DEALLOCATE(auth_key);

    return IPR_CWMP_SUCCESS;
}


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_del_state
*
*  DESCRIPTION: This function deletes a state from SADB 
*
*  RETURNS: SInt8 value 0 if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_del_state( UInt8* dst) 
{
	if (dst == PNULL)
		return IPR_CWMP_FAILURE;

    rtnetlink_handle_t rth = {0};
    del_stat_req_t req;
    UInt32 preferred_family = AF_INET;

    req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xsid) );
    req.n.nlmsg_flags = NLM_F_REQUEST;
    req.n.nlmsg_type = XFRM_MSG_DELSA;
    req.xsid.family = preferred_family;

	if (dst)
    req.xsid.daddr.a4 = ENB_INET_ADDR((const char*)dst);
    req.xsid.spi = (__u32) 0x0010001;

    req.xsid.proto = IPPROTO_ESP;

    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    {
	    close(rth.fd);
	    return IPR_CWMP_FAILURE;
    }

    if ( ipr_cwmp_rtnl_talk( &rth, &req.n, 0, 0, PNULL ) < 0 )
    { 
	    close(rth.fd);
	    return IPR_CWMP_FAILURE;
    }

    ipr_cwmp_rtnl_close( &rth );

    return IPR_CWMP_SUCCESS;
}


/**************************************************************************************
*
*  FUNCTION NAME: ipr_cwmp_mod_state
*
*  DESCRIPTION: This function updates a state in SADB to use auth algo md5
*
*  RETURNS: SInt8 value 0 if success IPR_CWMP_FAILURE if failure
*
**************************************************************************************/


ipr_cwmp_ret_t ipr_cwmp_mod_state(UInt8* src, UInt8* dst, UInt64 ipsec_rekey_lifetime_time, UInt8* auth_algo, UInt8* auth_key) 
{

	if(src == PNULL && dst == PNULL)
		return IPR_CWMP_FAILURE;

    rtnetlink_handle_t rth = {0};
    add_mod_stat_req_t req;

    ENB_MEMSET( &req, 0, sizeof(req) );
	
    // Netlink Header:
    req.n.nlmsg_len = NLMSG_LENGTH( sizeof(req.xsinfo) );
    req.n.nlmsg_flags = NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL;
    req.n.nlmsg_type = XFRM_MSG_UPDSA;


    // primitive xsinfo fields:
    req.xsinfo.family = AF_INET; 
	if(src)
    req.xsinfo.saddr.a4 = ENB_INET_ADDR((const char*) src); 
    req.xsinfo.mode = (__u8) 1;        

    // ID:
	if(dst)
    req.xsinfo.id.daddr.a4 = ENB_INET_ADDR((const char*) dst);
    req.xsinfo.id.proto = IPPROTO_ESP;  
    req.xsinfo.id.spi = (__u32) 0x7; 

    // LFT:
    req.xsinfo.lft.soft_byte_limit = XFRM_INF;
    req.xsinfo.lft.hard_byte_limit = XFRM_INF;
    req.xsinfo.lft.soft_packet_limit = XFRM_INF;
    req.xsinfo.lft.hard_packet_limit = XFRM_INF;
    //IPSEC phase 2 code starts
    if(ipsec_rekey_lifetime_time)
        req.xsinfo.lft.soft_add_expires_seconds = (__u64)ipsec_rekey_lifetime_time;
    //IPSEC phase 2 code ends


    // XFRMA_ALG_CRYPT:
    alg_t des;
    ENB_MEMSET(&des, 0, sizeof( alg_t ));
    ENB_STRNCPY((char *)des.alg.alg_name,(const char *) "des", sizeof(des.alg.alg_name));

    // key = 0x79b5d6e36dda9da4

    des.alg.alg_key_len = 64;
    des.alg.alg_key[0] = 0x79;
    des.alg.alg_key[1] = 0xb5;
    des.alg.alg_key[2] = 0xd6;
    des.alg.alg_key[3] = 0xe3;
    des.alg.alg_key[4] = 0x6d;
    des.alg.alg_key[5] = 0xda;
    des.alg.alg_key[6] = 0x9d;
    des.alg.alg_key[7] = 0xa4;

    UInt32 len = sizeof(struct xfrm_algo) + des.alg.alg_key_len;
    ipr_cwmp_addattr_l(&req.n, sizeof(req.buf), XFRMA_ALG_CRYPT, (void *)&des, len);


    // =- =- =- =- MD5 -= -= -= -=
    alg_t md5;
    ENB_MEMSET(&md5, 0, sizeof(alg_t));
    ENB_STRNCPY((char *)md5.alg.alg_name,(const char *) "md5", sizeof(md5.alg.alg_name));

    // auth md5 0x982f51293767d610

    md5.alg.alg_key_len = 64;
    md5.alg.alg_key[0] = 0x98;
    md5.alg.alg_key[1] = 0x2f;
    md5.alg.alg_key[2] = 0x51;
    md5.alg.alg_key[3] = 0x29;
    md5.alg.alg_key[4] = 0x37;
    md5.alg.alg_key[5] = 0x67;
    md5.alg.alg_key[6] = 0xd6;
    md5.alg.alg_key[7] = 0x10;

    len = sizeof(struct xfrm_algo) + md5.alg.alg_key_len;
    ipr_cwmp_addattr_l(&req.n, sizeof(req.buf), XFRMA_ALG_AUTH, (void *)&md5, len);


    if (ipr_cwmp_rtnl_open_byproto(&rth, 0, NETLINK_XFRM) < 0)
    {
	    close(rth.fd); 
	    return IPR_CWMP_FAILURE;
    }

    if (ipr_cwmp_rtnl_talk(&rth, &req.n, 0, 0, PNULL) < 0)
    {
	    close(rth.fd); 
	    return IPR_CWMP_FAILURE;
    }

    ipr_cwmp_rtnl_close(&rth);

    return IPR_CWMP_SUCCESS;
}
