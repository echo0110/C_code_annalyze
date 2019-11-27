/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: strfun.h $
 *
 *******************************************************************************/

/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lteTypes.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>


/*********************************************************************************
 *                FUNCTION PROTOTYPES
 *
 ********************************************************************************/
void itoa(UInt32, Char8 *);
char *sput_i(int, char *);
char *sput_ip1(int, char *);
UInt8 *time_stamp (void);
Char8* ipr_cwmp_hex_to_str(UInt8* src);
void ipr_cwmp_hex_2_str(UInt8*, UInt8*);

//function to get IP from DNS by passing the fqdn as arg
UInt8* ipr_cwmp_get_ip(UInt8* fqdn);

void add_brackets(UInt8* buf);
void remove_brackets(UInt8* buf);

