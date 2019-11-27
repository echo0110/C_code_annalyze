/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Porting Code -- For Unix Systems                                **/
/**                                                                        **/
/****************************************************************************/
#ifndef	__SAMPLE_PORT__
#include "mempool.h"

extern const QSYSOP my_os;

#define SRVNAME_GTPU             9
#define SRVNAME_UDP              2
#define SRVNAME_RELAY            4
#define SRVNAME_CP               3
#define SRVNAME_LM               1
#define SRVNAME_TS               6
#define SRVNAME_UDP6             7
#define MAX_SOURCE_IP_ADDS       10
#define PORT_TMR_POOL_MSG_SIZE   32
#define PORT_TMR_MAX             128
#define PORT_MSG_POOL_MSG_SIZE   8192
/*SPR 17888 +-*/
#define PORT_MSG_MAX             3800
/* SPR 21001 changes end */
/*SPR 17888 +-*/
#endif

