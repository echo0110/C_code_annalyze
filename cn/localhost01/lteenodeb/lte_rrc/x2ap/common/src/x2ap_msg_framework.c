/*******************************************************************************
*
*  FILE NAME   : x2ap_msg_fw.c
*
*  DESCRIPTION : This file contains definition of functions which will be used 
*                for sending message and receiving message to and from external
*                and internal interface respectively.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_msg_framework.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_msg_mgmt.h"

/******************************************************************************
 *SPR 20172 Fix Start
 * Function Name : x2ap_send_message_ex
 *SPR 20172 Fix End
 * Inputs        : p_msg - pointer to Message that will be passed 
 *                 dst_module - Module id for which message will be passed
 * Outputs       : None
 * Returns       : None
 * Description   : This function is called by stack to Send a message to some 
 *                 external module.
******************************************************************************/
/*SPR 20172 Fix Start*/
void x2ap_send_message_ex(
/*SPR 20172 Fix End*/
    void            *p_msg,     /* Message that will be passed */
    x2ap_module_id_t dst_module) /* Module id for which message will be passed */
{
    S32 error_code = X2AP_NULL;
    QMODULE service = qvGetServiceEx(dst_module, (int *)(&error_code));

    X2AP_ASSERT(X2AP_NULL != p_msg);

    if(QVERROR_NONE != error_code)
    {
        X2AP_TRACE(X2AP_FATAL, "Cannot get service name: %s",
            error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        qvPanic("Cannot get service name: %s",
                error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");
        return;
    }

    X2AP_TRACE(X2AP_BRIEF, "[SEND] [MODULE:%u(0x%X)] [API:%u(0x%X)]",
            dst_module, dst_module, x2ap_get_api_id(p_msg), x2ap_get_api_id
                (p_msg));
    qvSend(service, 0, p_msg);
}

/******************************************************************************
 * Function Name : x2ap_check_cspl_header
 * Inputs        : p_api -  Pointer to input API buffer
 * Outputs       : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function check general CSPL message header integrity.
******************************************************************************/
x2ap_return_et x2ap_check_cspl_header( U8 *p_api) /* Pointer to input API buffer */
{
    STACKAPIHDR     api_header;
    x2ap_return_et   result = X2AP_FAILURE;
    U32             msg_size = X2AP_NULL;

    X2AP_ASSERT(X2AP_NULL != p_api);

    msg_size = cvApiParseHeader(p_api, CV_HDRSIZE, &api_header);

    if (0 != msg_size)
    {
        if ((api_header.from >= RRC_MIN_INT_MODULE_ID) && (api_header.from <=
            RRC_MAX_INT_MODULE_ID))
        {
            result = X2AP_SUCCESS;
        }
        else if (((api_header.from >= RRC_MIN_EXT_MODULE_ID) && 
                    (api_header.from <= RRC_MAX_EXT_MODULE_ID)) ||
                        (api_header.from == RRC_SON_MODULE_ID))
        {
            /* Check whether external header present */
            if (RRC_OAM_MODULE_ID == api_header.from)
            {
                /*
                ** If message is from OAM 16 byte CSPL header + 10 bytes 
                ** Interface specific header is attached to buffer 
                */
                if (msg_size >= RRC_FULL_INTERFACE_HEADERS_SIZE)   
                {
                    result = X2AP_SUCCESS;
                }
            }
            /* 
            ** For other External Modules (Other than OAM) Only 16 byte 
            ** CSPL header is attached
            */
            else if (msg_size >= RRC_API_HEADER_SIZE)
            {
                result = X2AP_SUCCESS;
            }
        }
    }
    
    if (X2AP_SUCCESS == result)
    {
        /* report about the message */
        X2AP_TRACE(X2AP_INFO, "[RECV] [MODULE:%u(0x%X)] [API:%u(0x%X)]" 
            "[SIZE:%u(0x%X)] [VER:%u]",
            api_header.from, api_header.from,
            api_header.api, api_header.api,
            msg_size,msg_size,
            api_header.version );

        {
            /* Binary dump of the message */
            U32     cur             = X2AP_NULL;
            U32     sixteens_align  = (msg_size / 16) * 16;
            U32     remains         = msg_size - sixteens_align;
            S8    format_buff[]   = "%02X %02X %02X %02X %02X %02X %02X %02X"\
                "  %02X %02X %02X %02X %02X %02X %02X %02X";
            for (cur = 0; cur < sixteens_align; cur += 16)
            {
               /* SPR 22304 Fix Start */
                X2AP_TRACE(X2AP_DETAILED, (const char *)format_buff,
                    p_api[cur+0],p_api[cur+1],p_api[cur+2],p_api[cur+3],
                    p_api[cur+4],p_api[cur+5],p_api[cur+6],p_api[cur+7],
                    p_api[cur+8],p_api[cur+9],p_api[cur+10],p_api[cur+11],
                    p_api[cur+12],p_api[cur+13],p_api[cur+14],p_api[cur+15]);
                /* SPR 22304 Fix End */
            }
            if (remains > 0)
            {
                X2AP_ASSERT(cur == sixteens_align);
                /* Output (and access) only remains elements */
                format_buff[remains * 5 - 1 + (remains > 8)] = '\0';

               /* SPR 22304 Fix Start */
                X2AP_TRACE(X2AP_DETAILED, (const char *)format_buff,
                    p_api[cur+0],p_api[cur+1],p_api[cur+2],p_api[cur+3],
                    p_api[cur+4],p_api[cur+5],p_api[cur+6],p_api[cur+7],
                    p_api[cur+8],p_api[cur+9],p_api[cur+10],p_api[cur+11],
                    p_api[cur+12],p_api[cur+13],p_api[cur+14],p_api[cur+15]);
                /* SPR 22304 Fix End */
            }
        }
    }

    return result;
}

