/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  File Description : This file contains #defines for the packet type  and bit
 *                     strings to resolve the packet for profile IP/TCP.
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_DEF_P6_H_
#define _ROHC_CPRM_DEF_P6_H_

#include "rohc_c_typ.h"
#include "rohc_com_def.h"

/* for computing delta bit map */



/* TCP */

#define ROHC_CPRM_CH_ACK_FLAG       0x00000001          
#define ROHC_CPRM_CH_UPTR           0x00000002          
#define ROHC_CPRM_CH_IP_DSCP        0x00000004          
#define ROHC_CPRM_CH_ACK_STRIDE     0x00000008          
#define ROHC_CPRM_CH_IP_DF          0x00000010          
#define ROHC_CPRM_CH_TCP_OPT_LIST   0x00000020          
#define ROHC_CPRM_CH_RSF_FLAG       0x00000040          
#define ROHC_CPRM_CH_IP_TTL_HL      0x00000080  
#define ROHC_CPRM_CH_ECN_USED       0x00000100  
#define ROHC_CPRM_CH_INNER_IPID_BEH  0x00000200  
#define ROHC_CPRM_CH_TCP_WINDOW      0x00000400  
#define ROHC_CPRM_CH_TCP_SN          0x00000800  
#define ROHC_CPRM_CH_TCP_ACK_NO      0x00001000  
#define ROHC_CPRM_CH_TCP_SN_STRIDE   0x00002000  
#define ROHC_CPRM_CH_TCP_SN_RESIDUE  0x00004000  
#define ROHC_CPRM_CH_TCP_ACK_RESIDUE 0x00008000  



#define ROHC_CPRM_CH_BOTH_ACKNO_ACK_STRIDE    0x00001008 

#define ROHC_UNKNOWN_PKT 0xFF
    

#define ROHC_PRM_CRC_BITS0 0x00
#define ROHC_PRM_CRC_BITS3 0x03
#define ROHC_PRM_CRC_BITS7 0x07

#define ROHC_P6_IPID_BITS 16


#define ROHC_P6_SN_BITS  32
#define ROHC_P6_MSN_BITS 16

#endif  /* _ROHC_CPRM_DEF_P6_H_ */
