/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: alarm.c 
 *
 *************************************************************************
 *
 * File Description : This file contains the functions that are used for 
 *                    alarm handling
 *
 *************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include<lteMisc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h> 
#include <pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "alarm.h"
#ifdef MAC_AUT_TEST
#include "lteTypes.h"
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
__thread alarm_criticality_et alarmCriticality_g = DEFAULT_ALARM;
alarm_cntxt_t alarm_global_cntxt = {0};
#ifdef MAC_AUT_TEST
UInt8 *  pdcpOamBuff_g = 0;
extern UInt8  macAutFlag ;
extern void    *memCpy( void *dest, const void *src, size_t n );
#endif  

/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define UPDATE_ALM_CNTXT(p_alarm_cntxt, src_module_id,dst_module_id,dst_ip_addr, dst_port)\
{\
    p_alarm_cntxt->src_module_id = src_module_id;\
    p_alarm_cntxt->dst_module_id = dst_module_id;\
    strncpy(p_alarm_cntxt->dst_ip_addr,dst_ip_addr, IP_ADDR_MAX_SIZE - 1);\
    p_alarm_cntxt->dst_ip_addr[IP_ADDR_MAX_SIZE - 1] = '\0';\
    p_alarm_cntxt->dst_port = dst_port;\
}


/****************************************************************************
  Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/*****************************************************************************
 * Function Name  : construct_api_header
 * Inputs         : pMsgBuf,transaction_id, src_module_id,dst_module_id,
 *                  API_ID, msg_len
 * Outputs        : None
 * Returns        : None
 * Description    : construct_api_header
 *****************************************************************************/
static void construct_api_header(char** ppMsgBuf,
                    unsigned short int trans_id, 
                    unsigned short int src_module_id,
                    unsigned short int dst_module_id,
                    unsigned short int api_id, 
                    unsigned int buf_len);

/*****************************************************************************
 * Function Name  : compose_oam_alarm_notify_msg
 * Inputs         : pMsgBuf,p_msg_payload
 * Outputs        : None
 * Returns        : None
 * Description    : compose_oam_alarm_notify_msg
 *****************************************************************************/
static void compose_oam_alarm_notify_msg(
        char** pMsgBuf, 
        oam_alarm_notification_t* pMsg);

/*****************************************************************************
 * Function Name  : send_message
 * Inputs         : pMsgBuf, buf_len
 * Outputs        : None
 * Returns        : None
 * Description    : send_message
 *****************************************************************************/
static void send_message(char* pMsgBuf, unsigned int
);
#ifdef CSPL
/*****************************************************************************
 * Function Name  : send_message_to_cspl
 * Inputs         : pMsgBuf, buf_len
 * Outputs        : None
 * Returns        : None
 * Description    : dummy function
 *****************************************************************************/
static void send_message_to_cspl(char* pMsgBuf, unsigned int buf_len);
#endif

/****************************************************************************
 * Private Constants
 ****************************************************************************/
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/



/*****************************************************************************
 * Function Name  : alarm_init
 * Inputs         : file_name   
 * Outputs        : None
 * Returns        : None
 * Description    : function used to log alarms
 *****************************************************************************/
/* +- SPR 17777 */
void alarm_init(
        unsigned short int dst_module_id,
        char* dst_ip_addr,
        unsigned int dst_port
        )
/* +- SPR 17777 */
{
    alarm_cntxt_t *p_alarm_cntxt = &alarm_global_cntxt;
    unsigned short int src_module_id = 0;

    if((p_alarm_cntxt->sockfd = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket() system call failure\n");
        return;
    }
    p_alarm_cntxt->serv_addr.sin_family = AF_INET;
    p_alarm_cntxt->serv_addr.sin_port = htons_wrapper(dst_port);
    p_alarm_cntxt->serv_addr.sin_addr.s_addr = inet_addr_wrapper((char*)dst_ip_addr);
    UPDATE_ALM_CNTXT(p_alarm_cntxt,src_module_id,dst_module_id,dst_ip_addr, dst_port);  

    /* Set Default Alarm level */    
    alarmCriticality_g = DEFAULT_ALARM;   
}

/*****************************************************************************
 * Function Name  : send_alarm
 * Inputs         : source module id, alarmId, criticality
 * Outputs        : None
 * Returns        : None
 * Description    : function used to log alarms
 *****************************************************************************/
void send_alarm(unsigned short int src_module_id,
                        unsigned int alarmId, 
                        unsigned int criticality)
{
    void* pBuf = NULL;
    char* pMsgBuf = NULL;
    oam_alarm_notification_t oam_alarm_notification;
    unsigned int buf_len = 0;
    unsigned int msg_len = sizeof(oam_alarm_notification_t);
    alarm_cntxt_t *p_alarm_cntxt = &alarm_global_cntxt;
    
    if(criticality < alarmCriticality_g)
    {
        buf_len = OAM_INTERFACE_HEADER_SIZE + msg_len;
        
        if(NULL == (pBuf = malloc_wrapper(buf_len)))
        {
            PERROR("malloc failure");
            return;
        }

        pMsgBuf = (char*)pBuf;

        memSet(pMsgBuf, 0, buf_len);

        construct_api_header(&pMsgBuf,OAM_TRANSACTION_ID, 
                    src_module_id,
                    p_alarm_cntxt->dst_module_id, 
                    OAM_ALARM_NOTIFICATION, buf_len);

        pMsgBuf+= OAM_INTERFACE_HEADER_SIZE;

        oam_alarm_notification.alarmId = alarmId;
        oam_alarm_notification.criticality = criticality;
        strncpy(oam_alarm_notification.almStr, "\0", MAX_STR_SIZE);

        compose_oam_alarm_notify_msg(&pMsgBuf, &oam_alarm_notification);
        send_message(pBuf, buf_len
                );
        free(pBuf);
    }
}

/*****************************************************************************
 * Function Name  : send_fmt_alarm
 * Inputs         : source module id,alarmId, criticality, va_args
 * Outputs        : None
 * Returns        : None
 * Description    : function used to log alarms by creating a formatted message
 *****************************************************************************/
void send_fmt_alarm(unsigned short int src_module_id,
                        unsigned int alarmId, 
                        unsigned int criticality,
                        const char* pStartStr,...)
{
    void* pBuf = NULL;
    char* pMsgBuf = NULL;
    va_list arg_list;
    char pStr[MAX_STR_SIZE] = {0};
    /*unsigned int strSize = 0;*/
    oam_alarm_notification_t oam_alarm_notification;
    unsigned int buf_len = 0;
    unsigned int msg_len = sizeof(oam_alarm_notification_t);
    alarm_cntxt_t *p_alarm_cntxt = &alarm_global_cntxt;
    
    if(criticality < alarmCriticality_g)
    {
        buf_len = OAM_INTERFACE_HEADER_SIZE + msg_len;
        if(NULL == (pBuf = malloc_wrapper(buf_len)))
        {
            PERROR("malloc failure");
            return;
        }

        pMsgBuf = (char*)pBuf;

        memSet(pMsgBuf, 0, buf_len);

        construct_api_header(&pMsgBuf,OAM_TRANSACTION_ID, 
                    src_module_id,
                    p_alarm_cntxt->dst_module_id, 
                    OAM_ALARM_NOTIFICATION, buf_len);

        pMsgBuf+= OAM_INTERFACE_HEADER_SIZE;

        va_start(arg_list, pStartStr);
        /*strSize = vsnprintf(pStr,MAX_STR_SIZE, pStartStr, arg_list);*/
        va_end(arg_list);

        oam_alarm_notification.alarmId = alarmId;
        oam_alarm_notification.criticality = criticality;
	/* Coverity 10002 starts */
        strncpy(oam_alarm_notification.almStr, pStr, MAX_STR_SIZE-1);
	oam_alarm_notification.almStr[MAX_STR_SIZE - 1] = '\0';
	/* Coverity 10002 ends */
	
        compose_oam_alarm_notify_msg(&pMsgBuf, &oam_alarm_notification);
        send_message( pBuf, buf_len
                );
        free(pBuf);
    }
}

/*****************************************************************************
 * Function Name  : construct_api_header
 * Inputs         : pMsgBuf,transaction_id, src_module_id,dst_module_id,
 *                  API_ID, msg_len
 * Outputs        : None
 * Returns        : None
 * Description    : construct_api_header
 *****************************************************************************/
static void construct_api_header(char** ppMsgBuf,
                    unsigned short int trans_id, 
                    unsigned short int src_module_id,
                    unsigned short int dst_module_id,
                    unsigned short int api_id, 
                    unsigned int buf_len)
{
    char* pMsgBuf = (char *)*ppMsgBuf;
    unsigned int pos =0;
    pMsgBuf[pos++] = (trans_id & 0xFF00)>>8;
    pMsgBuf[pos++] = (trans_id & 0x00FF);
    pMsgBuf[pos++] = (src_module_id & 0xFF00)>>8;
    pMsgBuf[pos++] = (src_module_id & 0x00FF);
    pMsgBuf[pos++] = (dst_module_id & 0xFF00)>>8;
    pMsgBuf[pos++] = (dst_module_id & 0x00FF);
    pMsgBuf[pos++] = (api_id & 0xFF00)>>8;
    pMsgBuf[pos++] = (api_id & 0x00FF);
    pMsgBuf[pos++] = (buf_len & 0xFF00)>>8;
    /* CA changes start */
    pMsgBuf[pos++] = (buf_len & 0x00FF);
    /* fill invalid cell id*/
    pMsgBuf[pos] = OAM_API_HEADER_CELL_ID_NIL;
    /* CA changes end */
}
    

/*****************************************************************************
 * Function Name  : compose_oam_alarm_notify_msg
 * Inputs         : pMsgBuf,p_msg_payload
 * Outputs        : None
 * Returns        : None
 * Description    : compose_oam_alarm_notify_msg
 *****************************************************************************/
static void compose_oam_alarm_notify_msg(
        char** ppMsgBuf, oam_alarm_notification_t* pMsg)
{
    char* pMsgBuf = (char *)*ppMsgBuf;
    unsigned int pos =0;
    unsigned int almLen =0;
    pMsgBuf[pos++] = ((pMsg->alarmId) & 0xFF00)>>8;
    pMsgBuf[pos++] = ((pMsg->alarmId) & 0x00FF);
    pMsgBuf[pos++] = ((pMsg->criticality) & 0xFF00)>>8;
    pMsgBuf[pos++] = ((pMsg->criticality) & 0x00FF);
    /*+COVERITY 5.3.0 - 32083*/
    //            MAX_STR_SIZE; 
    almLen = strnlen(pMsg->almStr,MAX_STR_SIZE);
    /*-COVERITY 5.3.0 - 32083*/
    strncpy((pMsgBuf+pos), pMsg->almStr, almLen);
}
    

/*****************************************************************************
 * Function Name  : send_message
 * Inputs         : pMsgBuf, buf_len
 * Outputs        : None
 * Returns        : None
 * Description    : send_message
 *****************************************************************************/
static void send_message(char* pMsgBuf,unsigned int buf_len 
        )
{
#ifdef CSPL
    send_message_to_cspl(pMsgBuf, buf_len);
#else
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
    /** CA Changes Start **/
        memCpy(pdcpOamBuff_g,pMsgBuf,buf_len);
    /** CA Changes End **/
    }
    else
#endif
    {
        alarm_cntxt_t *p_alm_cntxt = &alarm_global_cntxt;

        send_message_to_udp_socket(pMsgBuf,buf_len,
                p_alm_cntxt->sockfd,
                &p_alm_cntxt->serv_addr);
    }
#endif
}
#ifdef CSPL
/*****************************************************************************
 * Function Name  : send_message_to_cspl
 * Inputs         : pMsgBuf, buf_len
 * Outputs        : None
 * Returns        : None
 * Description    : dummy function
 *****************************************************************************/
static void send_message_to_cspl(char* pMsgBuf, unsigned int buf_len)
{
    /* Dummy Function. Would be verified later if needed */
}
#endif
    
        
