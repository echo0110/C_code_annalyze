/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_enb_wrapper.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains common macros for C APIs 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Anant K Sharma    Nov 17, 2012   TR-069 Amendment 4   Initial Creation
 * 
******************************************************************************/

#ifndef __ENB_WRAPPER_H__
#define __ENB_WRAPPER_H__

//tmp inclusion for SIT
#include <lteTypes.h>

#define ENB_STRNCPY strncpy
#define ENB_STRCPY strcpy
#define ENB_STRCSPN strcspn
#define ENB_STRCHR strchr
#define ENB_STRNCMP strncmp
#define ENB_STRCMP strcmp
#define ENB_STRFTIME strftime
#define ENB_STRLEN strlen
#define ENB_STRNCAT strncat
#define ENB_STRCAT strcat
#define ENB_STRSTR strstr
#define ENB_STRTOK strtok
#define ENB_SNPRINTF snprintf
#define ENB_FPRINTF fprintf
#define ENB_PRINTF printf
#define ENB_MALLOC malloc
#define ENB_CALLOC calloc
#define ENB_FREE free
#define ENB_FREAD fread
#define ENB_FWRITE fwrite
#define ENB_MEMSET memset
#define ENB_MEMCMP memcmp
#define ENB_MEMCPY memcpy
#define ENB_CLOSE close
#define ENB_FCLOSE fclose
#define ENB_FOPEN fopen
#define ENB_SOCKET socket
#define ENB_INET_ADDR inet_addr
#define TR069_SENDTO sendto
#define TR069_USLEEP usleep
#define ENB_SENDMSG sendmsg
#define ENB_RECVMSG recvmsg
#define ENB_RECVFROM recvfrom
#define ENB_SETSOCKOPT setsockopt
#define ENB_BIND bind
#define ENB_LISTEN listen
#define ENB_READ read
#define ENB_WRITE write
#define ENB_ACCEPT accept
#define ENB_INET_NTOP inet_ntop
#define ENB_GETADDRINFO getaddrinfo
/* SPR 22314 Start */
#define TR069_FREE_ADDRINFO(ptr) do\
    {\
        freeaddrinfo(ptr); \
        ptr = NULL;\
    }while(TR069_ZERO)
/* SPR 22314 End */
#define ENB_HTONS htons
#define ENB_HTONL htonl
/*SPR 16545 Fixed Start*/
#define ENB_GETTIMEOFDAY tr069_gettimeofday_wrapper
/*SPR 16545 Fixed End*/
#define ENB_ITOA itoa
#define ENB_IOCTL ioctl
#define ENB_SETSOCKOPT setsockopt
#define ENB_POLL poll
#define ENB_ENB_STRCSPN strcspn
#define ENB_FSCANF fscanf
#define ENB_SSCANF sscanf
/* SPR 21926 Start */
#define YLINIT ylInit
/* SPR 21926 End */

/*SPR 19393 FIXED START */
#define ENB_INET_PTON inet_pton
/*SPR 19393 FIXED END */
#define ALLOCATE(size) tr_allocate(size)
#ifdef CSPL_LEAK_DEBUG_LITE
#define tr_allocate(size) tr_Allocate(size,__func__,__LINE__)

void*
tr_Allocate
(
    unsigned int size,
    const char *func, 
    unsigned int line
);

#else
void *tr_allocate(unsigned int size);
#endif

void *deallocate(void * ptr);
/* SPR 22314 Start */
#define DEALLOCATE(ptr) do\
    {\
        deallocate(ptr);\
        ptr = NULL; \
    }while(TR069_ZERO)
/* SPR 22314 End */
void *tr_realloc_fun(void *ptr, unsigned int size);
#define TR_REALLOC tr_realloc_fun
/* SPR 19162 FIXED START */
/*SPR 19393 FIXED START */
#define ENB_INET_PTON inet_pton
/*SPR 19393 FIXED END */


#define ENB_STRRCHR strrchr
#define TR069_ZERO 0
#define TR069_ONE 1
#define TR069_TWO 2
#define TR069_THREE 3
/* SPR 19162 FIXED END */
typedef enum {
	IPR_CWMP_FAILURE              = -1,     /**<Failure. */
	IPR_CWMP_SUCCESS              = 0,     /**<Success. */
} ipr_cwmp_ret_t;

// temp struct for glue layer
typedef struct 
{
	UInt8	HeNBIPAddr[64];//increased the size from 16 to 64 as hostname could come instead of URL
	UInt8	HeMSIPAddr[64];//increased the size from 16 to 64 as hostname could come instead of URL
	UInt8	SecGWServer1[64];//increased the size from 16 to 64 as hostname could come instead of URL
	UInt8	SecGWServer2[64];//increased the size from 16 to 64 as hostname could come instead of URL
	UInt8	SecGWServer3[64];//increased the size from 16 to 64 as hostname could come instead of URL
} config_res_t;

extern config_res_t* g_ipr_cwmp_config_res;

#endif /* __ENB__WRAPPER_H__ */
