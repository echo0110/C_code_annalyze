/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 *************************************************************************/

#ifndef _RRC_VALIDATION_H_
#define _RRC_VALIDATION_H_


/*****************************************************************************
**               MACRO DEFINATIONS                                           *
******************************************************************************/
#define RRC_FSCANF      fscanf
#define RRC_FEOF        feof
#define exit_wrapper        exit 
#define RRC_SOCKET_ERR  -1
#define MAX_MAC_ADD     6
#define MAC_ADD_LEN     12
/****************************************************************************/
S32 rrc_mac_addr_sys ( U8 *addr);
S32 rrc_validate_mac_address(S8 *, S8 *);
S32 rrc_validate_duration(FILE  *fptr_limitsfile, S8 *, U32 *p_left_duration);
S32 rrc_authentication(FILE *fptr_limitsfile, U32 *p_exp_duration);

#endif
