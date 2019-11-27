/* vim: ts=4:et:ai:ci:sw=4
*/
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_shell.c,v 1.1.2.1.4.3 2010/10/15 13:45:24 gur20491 Exp $
 ****************************************************************************
 *
 *  File Description : This file defines the encapsulating shell functions
 *                     like socket open, close, receive & send data.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rrm_shell.c,v $
 * Revision 1.1.2.1.4.3  2010/10/15 13:45:24  gur20491
 * Changes for RRC 2.0.1 Compliance
 *
 * Revision 1.1.2.2  2010/08/09 11:47:31  gur20491
 * Adding log for Bind error
 * Revision 1.2.0	gur29184, gur26515
 *Adding S1AP interface
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <errno.h>
#include    <string.h>
#include    <fcntl.h>
#include    "rrc_ext_api.h"
#include    "x2ap_api.h"
#include    "s1ap_api.h"
#include    <mqueue.h>
#include    <pthread.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include    <cspl.h>
#include    <stacklayer.h>
#include    <shell.h>
#include    "rrm_l2_intf.h"
#include    "ltePdcpRRMInterface.h" 
#include    "rrm_defines.h"
#include    "rrm_utils.h"
#include    "rrm_event_defines.h"
/*SPR 20712 Start*/
#include    "lteMessageQIntf.h"
/*SPR 20712 End*/

/*S_global variable used in RRM Shell*/
S8                         *p_rrm_encaps_facility_name = (S8 *)"RRM_SHELL"; 
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* extern rrm_global_context_t     *p_rrm_glb_ctxt;*/


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/* static const int  yes = 1, no = 0; Kshitij, Coverity CID 16310 and 16311 */
static       fd_set rrm_fd_set;
#ifdef RRM_UT_FRAMEWORK
#include "rrm_ut_stub_db.h"
static int gb_self_rrm_port;
#endif
extern U8* p_config_file;

rrm_ext_communication_data comm_data;
/*SPR 20908 Start*/
rrm_oam_l2_comm_info_t l2_comm_data;
/*SPR 20908 End*/


#if (RRM_ADV_LOGGER == LOGGER_TYPE)
extern AdvL_ContextIndex g_main_advl_ctxt;
#endif

/****************************************************************************
 * Function Name  : make_int_to_str
 * Inputs         : value
 * Outputs        : address as string
 * Returns        : void
 * Description    : converts address array into string form.
 ****************************************************************************/
void make_int_to_str(U32 value,S8* addr_str)
{

    S8    index           = 0;
    S8    index2          = 0;
    U8    remain          = 0;
    S8    tmp_str[10];

    do
    {
        remain = value % 10;
        value = value / 10;
        switch(remain)
        {
            case 0:
                tmp_str[index] = '0';
                break;
            case 1:
                tmp_str[index] = '1';
                break;
            case 2:
                tmp_str[index] = '2';
                break;
            case 3:
                tmp_str[index] = '3';
                break;
            case 4:
                tmp_str[index] = '4';
                break;
            case 5:
                tmp_str[index] = '5';
                break;
            case 6:
                tmp_str[index] = '6';
                break;
            case 7:
                tmp_str[index] = '7';
                break;
            case 8:
                tmp_str[index] = '8';
                break;
            case 9:
                tmp_str[index] = '9';
                break;
        };
        index++;
    }while(value != 0);

    index--;

    while (index >= 0)
    {
        addr_str[index2] =  tmp_str[index];
        index--;
        index2++;
    }
    addr_str[index2] = '\0';
}


/****************************************************************************
 * Function Name  : make_addr_str
 * Inputs         : addr
 * Outputs        : address in string form
 * Returns        : void
 * Description    : converts address array into string form.
 ****************************************************************************/
void make_addr_str(U8* addr,S8* addr_str)
{
    U8 index = 0;
    /* Start fix for Coverity_ID : 83294 */
    S8 tmp_str[MAX_IP_ADD_LEN_STR] = {0};
    /* End fix for Coverity_ID : 83294 */
    for (index = 0; index < 4; index ++)
    {
        // tmp_str = itoa(addr[index]);
        make_int_to_str(addr[index],tmp_str);
        /* Coverity 109026 Fix Start */
        /* SPR 19838 : CID 12911 Fix Start */
		if (index == 0)
		{
			strncpy(((char*)addr_str),((char*)tmp_str), MAX_IP_ADD_LEN_STR);
		}
		else
		{
			/* CID 11198*/
			strncat(((char*)addr_str),((char*)tmp_str),strlen((char*)tmp_str));
		}
		if (index != 3)
			/* CID 11198*/
			strncat(((char *)addr_str),".",RRM_ONE);
        /* SPR 19838 : CID 12911 Fix Start */
        /* Coverity 109026 Fix End */
    }
}

/****************************************************************************
 * Function Name  : change_address_format
 * Inputs         : addr
 * Outputs        : address in desired form
 * Returns        : void
 * Description    : converts string address array into decimal form.
 ****************************************************************************/
void change_address_format(S8* addr_str,U8* addr)
{
    U8 index = 0;
    U8 index_for_decimal = 0;
    U8 value = 0;
    /* Klocwork_fix_start : ipaddr */
    /* Klocwork changes start 22 may */
    for (index = 0; (index < MAX_IP_ADD_LEN_STR) && ((addr_str[index] != '\0') ); index ++)
    /* Klocwork changes end 22 may */
    {
        if (addr_str[index] == '.')
        {
            addr[index_for_decimal] = value;
            value = 0;
            index_for_decimal++ ;
        }
        else
        {
            value = value * 10 + (addr_str[index] - '0' );
        }
    }

/*KLOC FIX */

    if( MAX_IP_ADD_LEN_STR > index_for_decimal ) 
    {   
	    addr[index_for_decimal] = value;
    }

}

rrm_void_t rrm_trim_space(S8 *buf,U8 buf_size)
{
    U8 idx = 0;
    U8 loc = 0;

    while((buf[idx] != '\n') && (idx < buf_size ))
    {
        if(buf[idx] != ' ')
        {
            buf[loc] = buf[idx];
            loc++;
        }
        idx++;
    }
    buf[loc] = '\0';
}

S8* rrm_check_key_string_present( S8* p_string, char* p_key_name)
{
	/* Coverity CID 109027 Fix Start */
	if(PNULL!= p_string && PNULL!= p_key_name)
	{  
		U32 key_len = RRM_STRLEN(p_key_name);
		if ( 0 == RRM_STRNCMP(p_key_name,p_string, key_len))
		{
			if ( '=' == p_string[key_len] )
			{
				/* Returning position for Key Value */
				return p_string + key_len + 1;
			}
		}
	}
	/* Coverity CID 109027 Fix End */
    return RRM_PNULL;
}


/****************************************************************************
 * Function Name Name   : readConfigFile
 * Description          : Reads each module's configuration from the file
 *                        and stores in PortIPData global varaible.
 * Returns              : void
 ****************************************************************************/
static void readConfigFile (U8 *confFileName_p)
{
    S8                 keyStringBuf[MAX_KEY_SIZE];
    FILE               *fp = PNULL;
    S8*                 p_temp_str = RRM_PNULL;

    RRM_ASSERT (PNULL != confFileName_p);
    /*SPR 20908 +-*/
    RRM_ENCAPS_WARNING("RRM_DEBUG : File is = %s", confFileName_p);
    fp = fopen(((char*)p_config_file),"r");
    if (RRM_PNULL == fp)
    {
        RRM_ENCAPS_WARNING("RRM_ERROR : Not able to open cfg file");
        qvPanic("Not able to open cfg file\n");
        return;
    }

/* SPR 12537 start */
   /*SPR 20856 +-*/
/* SPR 12537 end */
	/*SPR 20908 +-*/

    while (RRM_PNULL != fgets (((char*)keyStringBuf), 100, fp))
    {
        /* Coverity Fixes Start */
        rrm_trim_space(keyStringBuf,sizeof(keyStringBuf)); 
	/*SPR 20856 Start*/
	/* Changing ext_comm_info to p_ext_comm_info*/
        if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_RRM_IP_ADDRESS")))
        {
            change_address_format(p_temp_str,
                    comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr);
        }
        /*SPR 21881 Start*/
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_RRM_RX_PORT")))
        /*SPR 21881 End*/
        {
            comm_data.ext_comm_info[RRM_PORT_INDEX].port = atoi(((char*)p_temp_str));
        }
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_L3_IP_ADDRESS")))
        {
            change_address_format(p_temp_str,
                    comm_data.ext_comm_info[RRC_PORT_INDEX].ipaddr);
        }
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_L3_PORT")))
        {
            comm_data.ext_comm_info[RRC_PORT_INDEX].port = atoi(((char*)p_temp_str));
        }
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_OAM_IP_ADDRESS")))
        {
            change_address_format(p_temp_str,
                    comm_data.ext_comm_info[OAM_PORT_INDEX].ipaddr);
        }
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_OAM_RX_PORT")))
        {
            comm_data.ext_comm_info[OAM_PORT_INDEX].port = atoi(((char*)p_temp_str));
        }
		/*SPR 21881 Start*/
        else if(PNULL != (p_temp_str = rrm_check_key_string_present(keyStringBuf,"ENB_SON_IP_ADDRESS")))
        {
		/*SPR 21881 End */
            change_address_format(p_temp_str,
                    comm_data.ext_comm_info[SON_PORT_INDEX].ipaddr);
        }
            /*SPR 21881 Start*/
        else if(PNULL != (p_temp_str= rrm_check_key_string_present(keyStringBuf,"ENB_SON_RRM_RX_PORT")))
            /*SPR 21881 End*/
        {
            comm_data.ext_comm_info[SON_PORT_INDEX].port = atoi(((char*)p_temp_str));
        }
	/*SPR 20856 End*/
	/*SPR 20908 +-*/
}

    fclose(fp);
/* SPR 12537 start */
	/*SPR 20856 +-*/
/* SPR 12537 end */
}
/********************** The `Listener' Structure ****************************/
typedef struct  
{
    int rrm_rx_sd;
    int rrm_tx_sd;
} LISTENER;

/****************************************************************************
 * Function Name  : rrm_malloc
 * Inputs         : unsigned int size
 * Outputs        : None
 * Returns        : void *
 * Description    : Encaps function for malloc
 ****************************************************************************/
void *rrm_malloc(unsigned int size)
{
    return malloc(size);
}


/****************************************************************************
 * Function Name  : rrm_vprintf
 * Inputs         : const char *format, va_list A
 * Outputs        : None
 * Returns        : integer value
 * Description    : Encaps function for vfprintf
 ****************************************************************************/
static int rrm_vprintf(const char *format, va_list A)
{
    return vfprintf( stdout, format, A );
}

/****************************************************************************
 * Function Name  : rrm_walltime
 * Inputs         : QTIME *now
 * Outputs        : None
 * Returns        : void
 * Description    : Encpas function for walltime 
 ****************************************************************************/
static void rrm_walltime( QTIME *now )
{
    struct timeval    T;

    gettimeofday( &T, 0 );
    now->s = T.tv_sec;
    now->us = T.tv_usec;
}

/* SPR:757- Coverity fix start */
/****************************************************************************
 * Function Name  : rrm_timedwait
 * Inputs         : S, QTIME T,
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for timed wait
 ****************************************************************************/
static rrm_void_t rrm_timedwait(
        rrm_void_t   *p_info_listener,
        const QTIME  *p_qtime)
{
    struct timeval *p_timeout = RRM_PNULL;
    rrm_listener_t *p_rrm_listener = RRM_PNULL;
    int n = 0;
    /*SPR 20856 +-*/
    if (RRM_PNULL != p_info_listener)
    {
        p_rrm_listener = (rrm_listener_t *)p_info_listener;

        /* Clearing rrm_fd_set by filling all zeros */
        FD_ZERO(&rrm_fd_set);

        /* Inserting sockets into rrm_fd_set */
        FD_SET(p_rrm_listener->sd_udp_rrm, &rrm_fd_set);

        /* Inserting messageQ into rrm_fd_set */
	/*SPR 20856 +-*/
            /* CID 30652*/

            //    break;

  

        if (RRM_PNULL != p_qtime) 
        { 
            p_timeout = (struct timeval *)rrm_mem_get(sizeof(struct timeval));
            if (RRM_PNULL == p_timeout )
            {
                RRM_PANIC("RRM_ERROR : Memory Allocation Failed");
            }
            else
            {
                p_timeout->tv_sec = p_qtime->s;
                p_timeout->tv_usec = p_qtime->us;

                n = select(FD_SETSIZE - 1, &rrm_fd_set, 0, 0, p_timeout);
                if (n < 0)
                {
                    fprintf(stderr, "Select failed for RRM\n");
                }

                /*Valgrind Memory Leak Fix Start*/
                RRM_MEM_FREE(p_timeout);
            }
            /*Valgring Memory Leak Fix End*/
        }
    }
}	
/* SPR:757- Coverity fix end */


/*SPR 20856 +-*/
/********************************************************************************
 * Function Name  : rrm_handle_message
 * Inputs         : sd_udp            - UDP socket descriptor on which data 
 is to be received
 *                  sender_module_id  - Module identifier from which 
 to receive the data
 *                  p_sender          - QMODULE structure for source module
 *                  p_receiver        - QMODULE structure for destination module
 *                  p_priority        - Priority of the message received.
 index             - Sector id to support Multiple cell support.
 * Outputs        :
 * Returns        : Pointer to the created buffer
 * Description    : It receives the data (external API header and data) at 
 *                  given UDP socket and appends CSPL header to it
 ********************************************************************************/

static U8 * rrm_handle_message(
        rrm_listener_t   *p_rrm_listener,
        QMODULE          *p_sender,
        QMODULE          *p_receiver,
        S8               *p_priority,
        U8                index)
{
    struct sockaddr_in         cli_addr = {0};
    socklen_t                  length = 0;
    U8                         ext_msg_api_hdr[RRM_INTERFACE_API_HEADER_SIZE] = {0};
    U8                         *p_recv_buffer   = RRM_PNULL;
    S32                        retval           = RRM_NULL;
    U16                        buf_len          = RRM_NULL;
    U16                        api_id           = RRM_NULL;
    U16                        recv_module_id   = RRM_NULL;
    U16                        sender_module_id = RRM_NULL;
    /* Coverity:CID 30721*/
    S32                        bytes_read       = RRM_NULL;//Deepika 
    /*SPR 20856 Start*/
    /*SPR 20856 End*/
    /* SPR 12537 start */
    /*SPR 20856 +-*/
    /* SPR 12537 end */

    if( FD_ISSET( p_rrm_listener->sd_udp_rrm, &rrm_fd_set) )
    {
        length = sizeof(struct sockaddr);
        /* Read the external API Header from the socket */
        bytes_read = recvfrom( p_rrm_listener->sd_udp_rrm, 
                ext_msg_api_hdr,
                RRM_INTERFACE_API_HEADER_SIZE,
                MSG_PEEK,
                (struct sockaddr *)&cli_addr,
                &length);

        if (RRM_INTERFACE_API_HEADER_SIZE != bytes_read)
        {
            RRM_ENCAPS_WARNING("RRM_INVALID_HEADER : Invalid API Header[%d] Recevd", bytes_read);
            /*raising error event - IPC_RECV_MSG_FAIL*/
            rrm_raise_error_event_notification(RRM_EE_IPC_RECV_MSG_FAIL);
        }
        else
        {
            /* Parse the EXT API HDR to get the API Id and buffer length*/
            sender_module_id = rrm_get_word_from_header(
                    &ext_msg_api_hdr[EXT_MSG_SRC_MODULE_ID_OFFSET]);

            api_id = rrm_get_word_from_header(
                    &ext_msg_api_hdr[EXT_MSG_API_OFFSET]);

            buf_len = rrm_get_word_from_header(
                    &ext_msg_api_hdr[EXT_MSG_BUF_LEN_OFFSET]);

            *p_priority = RRM_API_PRIORITY;

            *p_sender = qvGetService(sender_module_id);
            if (sender_module_id == RRC_MODULE_ID)
            {
                if(	
					(RRC_RRM_UPDATED_PWS_SI_LIST_CNF == api_id) || 
                    ((S1AP_RRM_PWS_REQ == api_id) ||(S1AP_RRM_PWS_CNF == api_id ) ||  
                     	(S1AP_RRM_KILL_REQ == api_id) || (S1AP_RRM_KILL_CNF == api_id))	||
                    ((X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND == api_id) || 
                    	(X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES == api_id)) ||
                    ((RRC_RRM_CELL_SETUP_REQ <= api_id) && (api_id <=  RRC_RRM_CELL_DELETE_RESP)) ||
                    ((RRC_RRM_CELL_RECONFIGURE_REQ <= api_id) && (api_id <= RRC_RRM_CELL_RECONFIG_RESP)) ||
                    ((RRC_RRM_UPDATED_PWS_SI_LIST_REQ <= api_id) &&(api_id <= RRC_RRM_UPDATED_PWS_SI_LIST_REQ)) ||
                    ((X2AP_RRM_LINK_DOWN_IND <= api_id) &&(api_id <= X2AP_RRM_LINK_UP_IND)) ||
                    ((RRC_RRM_CELL_START_REQ <= api_id)&&(api_id <= RRC_RRM_CELL_STOP_RESP )) ||
                    (S1AP_RRM_MME_CONFIG_TRANSFER == api_id) || 
                    ((X2AP_RRM_RSU_RRM_START_REQ <= api_id) && (api_id <= X2AP_RRM_RSU_ENB_UPDATE_IND)) ||
                    (api_id == S1AP_RRM_RIM_INFO) ||
                    (S1AP_RRM_LPPA_OTDOA_INFO_REQ == api_id) ||
					/* mod by yhliu 20191015 for ENDC config update --start */
#ifdef ENDC_ENABLED

                    ((X2AP_ENDC_CONFIG_UPDATE_REQ <= api_id)&&(api_id <= X2AP_ENDC_CONFIG_UPDATE_WAIT_IND )) ||
#endif
					/* mod by yhliu 20191015 for ENDC config update --end */

					/* SPR 16477 Fix Start */
                    (S1AP_RRM_MME_INFO == api_id)
                    /* SPR 16477 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
                    || (M2AP_RRM_MBMS_SF_INFO_REQ == api_id)
#endif             
                  )
                {
                    *p_receiver = qvGetService(RRM_CM_MODULE_ID);
                    recv_module_id = RRM_CM_MODULE_ID;
                }
                else
                {
                    *p_receiver = qvGetService(RRM_UEM_MODULE_ID);
                    recv_module_id = RRM_UEM_MODULE_ID;
                }
            }
            else if((sender_module_id == RRM_SON_MODULE_ID) ||
                    (sender_module_id == RRM_SON_APPS_MODULE_ID) ||
                    (sender_module_id == RRM_SON_ANR_MODULE_ID) ||
                    (sender_module_id == RRM_SON_ES_MODULE_ID) ||
                    (sender_module_id == RRM_SON_NMM_MODULE_ID) ||
                    (sender_module_id == RRM_SON_MLB_MODULE_ID) ||
                    (sender_module_id == RRM_SON_MRO_MODULE_ID) ||
                    (sender_module_id == RRM_SON_RACH_OPT_MODULE_ID))
            {
                *p_receiver = qvGetService(RRM_MIF_MODULE_ID);
                recv_module_id = RRM_MIF_MODULE_ID;
            }
            /*SPR 20856 Start*/
            else if(sender_module_id == PDCP_MODULE_ID)
            {
                if((api_id == RRM_PDCP_CONFIG_PARAMS_CNF)||
                        (api_id == RRM_PDCP_CONFIGURE_KPI_STATS_CNF)||
                        (api_id == RRM_PDCP_KPI_THP_STATS_IND))
                {
                    *p_receiver = qvGetService(RRM_CM_MODULE_ID);
                    recv_module_id = RRM_CM_MODULE_ID;
                }
                else if ((api_id == RRM_PDCP_UE_DRB_ECN_CE_CNF)||
                        (api_id == RRM_PDCP_UE_DRB_ECN_CE_IND))
                {
                    *p_receiver = qvGetService(RRM_UEM_MODULE_ID);
                    recv_module_id = RRM_UEM_MODULE_ID;
                }
            }
            else if (sender_module_id == MAC_MODULE_ID)
            {
                if((api_id == RRM_MAC_CONFIGURE_KPI_STATS_CNF)||
                        (api_id == RRM_MAC_KPI_STATS_IND) ||
                        (api_id == RRM_MAC_SCHEDULER_RECONFIG_CNF) ||
                        (api_id == RRM_MAC_CELL_CONFIG_CNF) || 
                        (api_id == RRM_MAC_L1_REPORT_IND) ||
                        (api_id == RRM_MAC_ICIC_REPORT_IND) ||
                        (api_id == MAC_CONFIGURE_PERF_STATS_CNF) ||
                        /* bug_14009_start */  
                        (api_id == MAC_CELL_PERF_STATS_IND) ||
                        (api_id == RRM_MAC_EICIC_REPORT_IND))
                    /* bug_14009_end */ 
                {
                    *p_receiver = qvGetService(RRM_CM_MODULE_ID);
                    recv_module_id = RRM_CM_MODULE_ID;
                }
                else /*SNR REPORT*/
                {
                    *p_receiver = qvGetService(RRM_UEM_MODULE_ID);
                    recv_module_id = RRM_UEM_MODULE_ID;
                }
            }
#ifdef RRM_UT_FRAMEWORK
            else if(sender_module_id == RRM_MODULE_ID)
            {
                recv_module_id = rrm_get_word_from_header(
                        &ext_msg_api_hdr[EXT_MSG_DEST_MODULE_ID_OFFSET]);
                switch (recv_module_id)
                {
                    case RRM_SON_MODULE_ID:
                    case RRM_SON_APPS_MODULE_ID:
                    case RRM_SON_ANR_MODULE_ID:
                    case RRM_SON_ES_MODULE_ID:
                    case RRM_SON_NMM_MODULE_ID:
                    case RRM_SON_MLB_MODULE_ID:
                    case RRM_SON_MRO_MODULE_ID:
                    case RRM_SON_RACH_OPT_MODULE_ID:
                        *p_receiver = qvGetService(STUB_RRM_SON_MODULE_ID);
                        break;

                    case RRC_MODULE_ID:
                        *p_receiver = qvGetService(STUB_RRC_MODULE_ID);
                        break;

                    case PDCP_MODULE_ID:
                        *p_receiver = qvGetService(STUB_PDCP_MODULE_ID);
                        break;

                    case MAC_MODULE_ID:
                        *p_receiver = qvGetService(STUB_MAC_MODULE_ID);
                        break;

                    case RRM_OAM_MODULE_ID:
                        *p_receiver = qvGetService(STUB_RRM_OAM_MODULE_ID);
                        break;
                }
            }
#endif
            /*SPR 20856 End*/
            else /*OAM Messages shall go to MIF */
            {
                if (api_id == PLATFORM_RRM_UPDATE_BACKHAUL )
                {
                    *p_receiver = qvGetService(RRM_CM_MODULE_ID);
                    recv_module_id = RRM_CM_MODULE_ID;
                }
                else
                {  
                    /*Both Platform or OAM message will go to MIF*/
                    *p_receiver = qvGetService(RRM_MIF_MODULE_ID);
                    recv_module_id = RRM_MIF_MODULE_ID;
                }
            }
            /*Cov_fix_63703*/

            p_recv_buffer  = (U8 *)rrm_msg_mem_get
                (CV_HDRSIZE + buf_len);

            if (RRM_PNULL == p_recv_buffer)
            {
                RRM_PANIC("RRM_ERROR : Memory Allocation Failed");
            }
            else
	    {
		    retval = recvfrom( p_rrm_listener->sd_udp_rrm, 
				    p_recv_buffer + CV_HDRSIZE,
				    buf_len,
				    0,
				    (struct sockaddr*)&cli_addr,
				    &length);
		    if (retval <= 0)
		    {
			    rrm_msg_mem_free(p_recv_buffer);
			    p_recv_buffer = RRM_PNULL; 

			    /*raising error event - IPC_RECV_MSG_FAIL*/
			    rrm_raise_error_event_notification(RRM_EE_IPC_RECV_MSG_FAIL);
		    }
		    else if (buf_len != retval)
		    {
			    rrm_msg_mem_free(p_recv_buffer);
			    p_recv_buffer = RRM_PNULL; 

			    /*raising error event - IPC_RECV_MSG_FAIL*/
			    rrm_raise_error_event_notification(RRM_EE_IPC_RECV_MSG_FAIL);
		    }
		    else
		    {
			    /* SPR 20007 : CID 94001 Fix Start */
			    /* Code Removed */

			    /*Appending CSPL header at top 16 bytes of this buffer*/
			    RRM_MEMSET(p_recv_buffer, 0, CV_HDRSIZE);
#ifdef RRM_UT_FRAMEWORK
			    if (STUB_FLUSH_API == api_id)
			    {
				    stub_roll_logs();
				    return RRM_NULL;
			    }
#endif
                /*SPR 20856 +-*/
                rrm_construct_cspl_header((U8*)p_recv_buffer,
                        RRM_VERSION_ID,
                        sender_module_id,
                        recv_module_id,
                        api_id,
                        RRM_API_PRIORITY,
                        retval);
                /* SPR 20007 : CID 94001 Fix End */
			    /*SPR 20856 +-*/

		    }
	    }
	}
    }
    /*SPR 20856 +-*/
    if((sender_module_id == PDCP_MODULE_ID) ||
            (sender_module_id == MAC_MODULE_ID) ||
            (sender_module_id == RRM_OAM_MODULE_ID) ||
            (sender_module_id == RRM_SON_MODULE_ID))
    {
        RRM_LOG_HEX_DUMP("RECV", p_recv_buffer, (CV_HDRSIZE + buf_len),
                sender_module_id, api_id, RRM_VERSION_ID);
    }
    return p_recv_buffer;
}



/****************************************************************************
 * Function Name  : rrm_receive
 * Inputs         : S, sender, receiver, priority
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function to receive at RRM
 ****************************************************************************/
static rrm_void_t *rrm_receive(
        rrm_void_t  *p_info_listener,
        QMODULE     *p_sender,
        QMODULE     *p_receiver,
        S8          *p_priority)
{
    rrm_listener_t  *p_rrm_listener = (rrm_listener_t *)p_info_listener;
    U8              *p_recv_buffer  = RRM_PNULL;
    S32              n              = RRM_ZERO;
    U8               l2_instance          = RRM_ZERO;
    U8               is_fd_set      = RRM_ZERO;

    FD_ZERO( &rrm_fd_set );
    FD_SET( p_rrm_listener->sd_udp_rrm, &rrm_fd_set);
    /* Multiple cell support */
/*SPR 20856 +-*/    
/* CID 30651*/
    struct timeval   zero;
    zero.tv_sec  = 0;
    zero.tv_usec = 500;

    n = select( FD_SETSIZE - 1, &rrm_fd_set, 0, 0, &zero );

    if( n < 0 ) {
        rrm_abort();
    }

    if(FD_ISSET( p_rrm_listener->sd_udp_rrm, &rrm_fd_set))
    {
        is_fd_set = RRM_ONE;
    }

/*SPR 20856 +-*/    
/* CID 30651*/


    /* Return if no socket descriptor is set */ 
    if(RRM_ZERO == is_fd_set)
    {
        return RRM_PNULL;
    }

    /* Msg Recevd from RRC/OAM/SON on socket and on MsgQ from MAC */
    if(FD_ISSET(p_rrm_listener->sd_udp_rrm, &rrm_fd_set)
        )
    {
        p_recv_buffer = rrm_handle_message(p_rrm_listener, p_sender,
                p_receiver, p_priority,
                                           l2_instance);
        return p_recv_buffer;
    }

    
    
/*SPR 20856 +-*/
    
    return p_recv_buffer;
}

/*****************************************************************************
 * Function Name  : rrm_make_udp
 * Inputs         : p_self     - Pointer to the address structure to be binded
 to the UDP socket
 * Outputs        : None
 * Returns        : UDP socket descriptor
 * Description    : Creates a UDP socket, assigns an address to udp socket
 *                  and makes it ready for communication.
 ****************************************************************************/
/*Coverity CID 101181 Fix Start */
static S32 rrm_make_udp(struct sockaddr_in* p_self)
{
    S32 sd      = RRM_ZERO;
    U8  i       = RRM_ZERO;
    /*SPR 20856 +-*/
# ifdef RRM_UT_FRAMEWORK
    U8  max_setup_try = RRM_TEN;
#else
    U8  max_setup_try = RRM_ONE;
#endif


    if (RRM_PNULL == p_self)
    {
        RRM_PANIC(" RRM_ERROR : NULL pointer Recvd");
    }

    /* Create UDP socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, RRM_ZERO)) < RRM_ZERO)
    {
        RRM_PANIC("RRM_ERROR : Unable to create UDP socket");
    }
    else
    {

	    /*coverity 16584, rel2.0.1, aditya, rel_val removed since never used*/
	    /* Binding address structures with a socket */
#ifdef RRM_UT_FRAMEWORK
	    gb_self_rrm_port = p_self->sin_port;
#endif

	    for (i = 0; i < max_setup_try; i++)
	    {
		    /*SPR 20856 +-*/
		    if (bind(sd, (struct sockaddr *)p_self,
					    sizeof(struct sockaddr)) == RRM_ZERO)
		    {
			    return sd;
		    }
#ifdef RRM_UT_FRAMEWORK
		    if (EADDRINUSE == errno)
		    {
			    gb_self_rrm_port = p_self->sin_port++;
		    }
		    else
		    {
			    RRM_PANIC( "RRM_ERROR : Binding Error ");
		    }
#endif

		    /*SPR 20856 +-*/
	    }
	    /* Coverity CID 87633 Fix Start */
	    if(-1 == close(sd))
	    {

		    RRM_TRACE(RRM_TRUE, p_rrm_encaps_facility_name, RRM_ERROR,
				    "Error while closing socket");
	    }
	    /* Coverity CID 87633 Fix End */
    }
    RRM_PANIC( "RRM_ERROR : Binding Error ");
    return RRM_ZERO;
}
/*Coverity CID 101181 Fix End */

/****************************************************************************
 * Function Name  : rrm_open
 * Inputs         : data
 * Outputs        : None
 * Returns        : void*
 * Description    : Opens sockets for RRM OAM and RRC
 ****************************************************************************/
static rrm_void_t *rrm_open(rrm_void_t  *p_data)
{
    struct sockaddr_in rrm_self_addr_udp = {0};
    S8                 tmp_addr[16] = {0};
    /* Dual Cell Support */
    /*SPR 20856 +-*/

    rrm_listener_t *p_rrm_listener = (rrm_listener_t *)rrm_sys_mem_get(sizeof(rrm_listener_t));
    /*SPR 17777 +-*/
    RRM_TRACE(RRM_TRUE, p_rrm_encaps_facility_name, RRM_ERROR, "Unused variable p_data=%p",p_data);
    /*SPR 17777 +-*/ 
    if (RRM_PNULL == p_rrm_listener)
    {
        RRM_PANIC("RRM_ERROR : Memory Alocation Failed");
    }
    else
    {
	    if (RRM_PNULL == p_config_file)
	    {
		    p_config_file = ((U8*)RRM_CONFIG_FILE);
	    }

	    readConfigFile(p_config_file);
	    rrm_self_addr_udp.sin_family = AF_INET;
	    /*SPR 20856 Start*/
	    /*Changing ext_comm_info to p_ext_comm_info*/
	    rrm_self_addr_udp.sin_port = htons(comm_data.ext_comm_info[RRM_PORT_INDEX].port);
	    make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
	    /*SPR 20856 End*/
	    rrm_self_addr_udp.sin_addr.s_addr = inet_addr(((char *)tmp_addr));

	    p_rrm_listener->sd_udp_rrm = rrm_make_udp(&rrm_self_addr_udp);
	    /*SPR 20856 +-*/

    /* FOR MAC */
	    /*SPR 20856 Start*/
	    /*SPR 20856 end*/
    }
    return p_rrm_listener;
}

/*****************************************************************************
 * Function Name  : rrm_send
 * Inputs         : p_info_listener - Pointer to the listener object
 *                  p_message - Pointer to the message buffer
 *                  p_sender - Module which sent the message
 *                  p_receiver - Module which should receive the message
 *                  p_data - Pointer to data provided by CSPL
 *                  size - Size of the message
 * Outputs        : None
 * Returns        : None
 * Description    : It is used by CSPL for sending message over sockets.
 ****************************************************************************/
static rrm_void_t rrm_send(
        rrm_void_t        *p_info_listener,
        const rrm_void_t  *p_message,
        QMODULE            p_sender,
        QMODULE            p_receiver,
        rrm_void_t         *p_data,
        unsigned int       size)
{
    rrm_listener_t   *p_rrm_listener = (rrm_listener_t *)p_info_listener;
    struct  sockaddr_in serveraddress;
    S16 server_length = 0;
    S32 sd_udp = 0;
    S32 bytes_sent  = 0;
    U16 msg_len = RRC_NULL;
    STACKAPIHDR   cspl_hdr = {0};
    U16 receiverId = (U16)qvGetServiceName(p_receiver);
    /* Coverity 115236 Fix Start */
    S8 tmp_addr[16] = {0};
    /* Coverity 115236 Fix End */
    /* Multiple Cell Support */
    L2RrmApiHeader           *l2_hdr_buf = RRM_PNULL;
    /* coverity_67316 start */
    if(PNULL == p_message)
    {
        RRM_ENCAPS_WARNING("RRM_ERROR : p_message received is NULL");
        return;
    }
    /* coverity_67316 stop */
    rrm_parse_cspl_header((U8 *)p_message, &cspl_hdr);

    if((receiverId == PDCP_MODULE_ID) || (receiverId == MAC_MODULE_ID))
    {
        RRM_LOG_HEX_DUMP("SEND", p_message, size, receiverId, (cspl_hdr.api),
                (cspl_hdr.version));
    }

    /* Get the message length from EXT API HEADER */

    RRM_MEMSET( &serveraddress, 0, sizeof(serveraddress) );
    sd_udp = p_rrm_listener->sd_udp_rrm;
    /* Get the appropriate socket descriptor based on the dest module */
    if (RRM_OAM_MODULE_ID == receiverId)
    {
        /* Get the message length from EXT API HEADER */
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);

        serveraddress.sin_family = AF_INET;
#ifdef RRM_UT_FRAMEWORK
        serveraddress.sin_port = gb_self_rrm_port;
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
#else
        /*SPR 20856 Start*/
        /*Changing ext_comm_info to p_ext_comm_info*/
        serveraddress.sin_port = htons(comm_data.ext_comm_info[OAM_PORT_INDEX].port);
        /*IP ADDRESS*/
        make_addr_str( comm_data.ext_comm_info[OAM_PORT_INDEX].ipaddr, tmp_addr);
        /*SPR 20856 End*/
#endif
        serveraddress.sin_addr.s_addr = inet_addr(((char*)tmp_addr));
        server_length = sizeof(serveraddress);

        yfSetU16At(((U8 *)p_message + CV_HDRSIZE), 2, RRM_MODULE_ID);
        /*SPR 20856 +-*/
    }
    else if (RRC_MODULE_ID == receiverId)
    {
        /* Get the message length from EXT API HEADER */
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);

        serveraddress.sin_family = AF_INET;

#ifdef RRM_UT_FRAMEWORK
        serveraddress.sin_port = gb_self_rrm_port;
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
#else
        /*PORT NO*/
        /*SPR 20856 Start*/
        /*Changing ext_comm_info to p_ext_comm_info*/
        serveraddress.sin_port = htons(comm_data.ext_comm_info[RRC_PORT_INDEX].port);
        /*IP ADDRESS*/
        make_addr_str( comm_data.ext_comm_info[RRC_PORT_INDEX].ipaddr, tmp_addr);
        /*SPR 20856 End*/
#endif
        serveraddress.sin_addr.s_addr = inet_addr(((char*)tmp_addr));
        server_length = sizeof(serveraddress);

        yfSetU16At(((U8 *)p_message + CV_HDRSIZE), 2, RRM_MODULE_ID);
        /*SPR 20856 +-*/
    }
#ifdef RRM_UT_FRAMEWORK
    else if (RRM_MODULE_ID == receiverId )
    {
        serveraddress.sin_family = AF_INET;
        /*PORT NO*/
        serveraddress.sin_port = gb_self_rrm_port;
        /*IP ADDRESS*/
        //inet_pton(AF_INET, comm_data.ext_comm_info[RRC_IP_ADDR_INDEX], &(serveraddress.sin_addr));
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
        serveraddress.sin_addr.s_addr = inet_addr(((char*)tmp_addr));
        server_length = sizeof(serveraddress);
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);
    }
#endif
    else if ((RRM_SON_MODULE_ID == receiverId) ||
            (receiverId == RRM_SON_APPS_MODULE_ID) ||
            (receiverId == RRM_SON_ANR_MODULE_ID) ||
            (receiverId == RRM_SON_ES_MODULE_ID) ||
            (receiverId == RRM_SON_NMM_MODULE_ID) ||
            (receiverId == RRM_SON_MLB_MODULE_ID) ||
            (receiverId == RRM_SON_MRO_MODULE_ID) ||
            (receiverId == RRM_SON_RACH_OPT_MODULE_ID))
    {
        /* Get the message length from EXT API HEADER */
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);

        serveraddress.sin_family = AF_INET;
        /*PORT NO*/
#ifdef RRM_UT_FRAMEWORK
        serveraddress.sin_port = gb_self_rrm_port;
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
#else
        serveraddress.sin_port = htons(comm_data.ext_comm_info[SON_PORT_INDEX].port);
        make_addr_str( comm_data.ext_comm_info[SON_PORT_INDEX].ipaddr, tmp_addr);
#endif
        /*IP ADDRESS*/
        serveraddress.sin_addr.s_addr = inet_addr((char*)(tmp_addr));
        server_length = sizeof(serveraddress);

        yfSetU16At(((U8 *)p_message + CV_HDRSIZE), 2, RRM_MODULE_ID);
    }
    /*SPR 20856 Start*/
    else if (RRM_L2_MODULE_ID == receiverId)
    {
        /* Get the message length from EXT API HEADER */
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);
        l2_hdr_buf = (L2RrmApiHeader *)((U8 *)p_message + CV_HDRSIZE);
        /*SPR 20856 +-*/
        /*SPR 20908 Start*/
        rrm_map_cell_id_to_ip_port(l2_hdr_buf->cellIndex,&l2_comm_data,receiverId);
        /*SPR 20908 End*/
        /* Get the message length from Interface API HEADER */
        serveraddress.sin_family = AF_INET;
        /*PORT NO*/
        /*SPR 20856 Start*/
        /*SPR 20908 Start*/
#ifdef RRM_UT_FRAMEWORK
        serveraddress.sin_port = gb_self_rrm_port;
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
#else
        serveraddress.sin_port = htons(l2_comm_data.port);
        make_addr_str( l2_comm_data.ip_addr, tmp_addr);
#endif
        /*SPR 20908 End*/
        /*SPR 20856 End*/
        serveraddress.sin_addr.s_addr = inet_addr(((char*)tmp_addr));
        server_length = sizeof(serveraddress);
        yfSetU16At(((U8 *)p_message + CV_HDRSIZE), 2, RRM_MODULE_ID);
        /*SPR 20856 +-*/
        /* SPR 12537 end */

    }
    else if (PDCP_MODULE_ID == receiverId)
    {
        /* Get the message length from EXT API HEADER */
        msg_len = (U16)yfGetU16At(( (U8 *)p_message + CV_HDRSIZE), 8);
        l2_hdr_buf = (L2RrmApiHeader *)((U8 *)p_message + CV_HDRSIZE);
        /*SPR 20856 +-*/
        /* Get the message length from Interface API HEADER */
        /*SPR 20908 Start*/
        rrm_map_cell_id_to_ip_port(l2_hdr_buf->cellIndex,&l2_comm_data,receiverId);
        /*SPR 20908 End*/

        serveraddress.sin_family = AF_INET;
        /*PORT NO*/
        /*SPR 20856 Start*/
        /*Changing ext_comm_info to p_ext_comm_info*/
#ifdef RRM_UT_FRAMEWORK
        serveraddress.sin_port = gb_self_rrm_port;
        make_addr_str( comm_data.ext_comm_info[RRM_PORT_INDEX].ipaddr, tmp_addr);
#else
        /*SPR 20908 Start*/
        serveraddress.sin_port = htons(l2_comm_data.port);
        make_addr_str( l2_comm_data.ip_addr, tmp_addr);
#endif
        /*SPR 20908 End*/
        /*SPR 20856 End*/
        serveraddress.sin_addr.s_addr = inet_addr(((char*)tmp_addr));
        server_length = sizeof(serveraddress);
        yfSetU16At(((U8 *)p_message + CV_HDRSIZE), 2, RRM_MODULE_ID);
        /*SPR 20856 +-*/
        /* SPR 12537 end */

    }
    /*SPR 20856 End*/
    /* SPR 12537 end */
    switch(receiverId)
    {
        case RRM_OAM_MODULE_ID:
        case RRC_MODULE_ID:
        case RRM_SON_MODULE_ID:
        case RRM_SON_APPS_MODULE_ID:
        case RRM_SON_ANR_MODULE_ID:
        case RRM_SON_ES_MODULE_ID:
        case RRM_SON_NMM_MODULE_ID:
        case RRM_SON_MLB_MODULE_ID:
        case RRM_SON_MRO_MODULE_ID:
        case RRM_SON_RACH_OPT_MODULE_ID:
#ifdef RRM_UT_FRAMEWORK
        case RRM_MODULE_ID:
#endif

            {
                /* Send the data on UDP socket */
                bytes_sent = sendto(sd_udp,
                        (S8 *)p_message + CV_HDRSIZE,
                        msg_len, 0, (struct sockaddr *)&serveraddress,
                        server_length);
                if (bytes_sent < 0)
                {
                    RRM_ENCAPS_WARNING("RRM_ERROR : Unable to send data on UDP socket");
                    /*raising error event - IPC_SEND_MSG_FAIL*/
                    rrm_raise_error_event_notification(RRM_EE_IPC_SEND_MSG_FAIL);
                }
                break;
            }
        case RRM_L2_MODULE_ID:
            {
                /*CA STAGE 2*/
                /*SPR 20856 Start*/
                /*SPR 20856 Start*/
                bytes_sent = sendto(sd_udp,
                        (S8 *)p_message + CV_HDRSIZE,
                        msg_len, 0, (struct sockaddr *)&serveraddress,
                        server_length);
                if (bytes_sent < 0)
                {
                    RRM_ENCAPS_WARNING("RRM_ERROR : Unable to send data on UDP socket");
                    /*raising error event - IPC_SEND_MSG_FAIL*/
                    rrm_raise_error_event_notification(RRM_EE_IPC_SEND_MSG_FAIL);
                }
                /*SPR 20856 End*/
                /* SPR 12537 end */

                break;
                /*SPR 20856 +-*/
            }
        case PDCP_MODULE_ID:
            {
                /*SPR 20856 end*/
                /* SPR 5178 Added impl. to send data to PDCP via socket interface */
                /* Send the data on UDP socket */
                bytes_sent = sendto(sd_udp,
                        (S8 *)p_message + CV_HDRSIZE,
                        msg_len, 0, (struct sockaddr *)&serveraddress,
                        server_length);
                if (bytes_sent < 0)
                {
                    fprintf(stderr, " Error in sending msg to PDCP by RRM for KPY errorno = %d\n", errno);
                    RRM_ENCAPS_WARNING("RRM_ERROR : Unable to send data on UDP socket");
                    /*raising error event - IPC_SEND_MSG_FAIL*/
                    rrm_raise_error_event_notification(RRM_EE_IPC_SEND_MSG_FAIL);
                }
                /*SPR 20856 +-*/
                /* SPR 12537 end */
                /*Coverity_fix_start_62527*/
                break;
                /*Coverity_fix_end_62527*/
            }
            /*SPR 20856 +-*/
        default :
            /*SPR 17777 +-*/
            RRM_ENCAPS_WARNING("RRM_ERROR : Incorrect receiver Id %d"
                    "Unused variable p_sender=%p p_receiver=%p p_data=%p",receiverId,p_sender,p_data);
            /*SPR 17777 +-*/
    }
}

/****************************************************************************
 * Function Name  : rrm_close
 * Inputs         : data
 * Outputs        : None
 * Returns        : void
 * Description    : closes the sockets open and frees the memory
 ****************************************************************************/
static void rrm_close( void *data )
{
     /*SPR 20856 Start*/
     rrm_listener_t *p_rrm_listener  = (rrm_listener_t *)data;
    /* Coverity CID 87630 Fix Start */
    if(-1 == close( p_rrm_listener->sd_udp_rrm))
    {

        RRM_TRACE(RRM_TRUE, p_rrm_encaps_facility_name, RRM_ERROR,
                "Error while closing socket");
    }
     /*SPR 20856 End*/
    /* Coverity CID 87630 Fix Start */
    qvSysFree( data );
}


#if defined(RRM_IN_MEM_LOGGER) ||  defined(RRM_UT_FRAMEWORK)
/****************************************************************************
 * Function Name  : rrm_newkey
 * Inputs         : None
 * Outputs        : None
 * Returns        : Newly created key for the thread
 * Description    : Create a new key for a thread
 ****************************************************************************/

static	void	*rrm_newkey( void )
{
    pthread_key_t	*key = (pthread_key_t *)malloc( sizeof(pthread_key_t) );
    if(key != RRM_PNULL)
    {
	    pthread_key_create( key, 0 );
    }
    return (void *)key;
}

/****************************************************************************
 * Function Name  : rrm_getkey
 * Inputs         : Key object
 * Outputs        : None
 * Returns        : Context stored in key
 * Description    : Return context stored for a calling thread
 ****************************************************************************/

static	void	*rrm_getkey( void *key )
{
    return	pthread_getspecific( *((pthread_key_t *)key) );
}

/****************************************************************************
 * Function Name  : rrm_setkey
 * Inputs         : Key, Value
 * Outputs        : None
 * Returns        : None
 * Description    : Function for setting the value corresponding to the key
 ****************************************************************************/

static	void	rrm_setkey( void *key, void *value )
{
    pthread_setspecific( *((pthread_key_t *)key), value );
}

/****************************************************************************
 * Function Name  : rrm_newlock
 * Inputs         : None
 * Outputs        : None
 * Returns        : Lock object
 * Description    : Returns new lock object
 ****************************************************************************/

static	void	*rrm_newlock( void )
{
    pthread_mutex_t	*lock = (pthread_mutex_t *)malloc( sizeof(pthread_mutex_t) );
    if(lock != RRM_PNULL)
    {
	    pthread_mutex_init( lock, 0 );
    }
    return	(void *)lock;
}

/****************************************************************************
 * Function Name  : rrm_lock
 * Inputs         : Lock object
 * Outputs        : None
 * Returns        : None
 * Description    : Function for taking the lock
 ****************************************************************************/

static	void	rrm_lock( void *lock )
{
    pthread_mutex_lock( (pthread_mutex_t *)lock );
}

/****************************************************************************
 * Function Name  : rrm_unlock
 * Inputs         : Lock
 * Outputs        : None
 * Returns        : None
 * Description    : Function to unlock 
 ****************************************************************************/

static	void	rrm_unlock( void *lock )
{
    pthread_mutex_unlock( (pthread_mutex_t *)lock );
}

/****************************************************************************
 * Function Name  : rrm_destroylock
 * Inputs         : Lock object
 * Outputs        : None
 * Returns        : None
 * Description    : Destroy the locking object created for mutex
 ****************************************************************************/

static	void	rrm_destroylock( void *lock )
{
    pthread_mutex_destroy( (pthread_mutex_t *)lock );
    free( lock );
}


typedef	struct __waitdata {
    pthread_cond_t	condition;
    pthread_mutex_t	lock;
    int		value;
} __waitdata;


/****************************************************************************
 * Function Name  : rrm_newchannel
 * Inputs         : None
 * Outputs        : None
 * Returns        : wait data structure
 * Description    : Function to initialize mutex for communication between
 *                  threads
 ****************************************************************************/

static	void	*rrm_newchannel( void )
{
    __waitdata	*w = (__waitdata *)malloc( sizeof(__waitdata) );
    if(w != RRM_PNULL)
    {
	    pthread_cond_init(&(w->condition), 0);
	    pthread_mutex_init( &(w->lock), 0 );
	    w->value = 0;
    }

    return (void *)w;
}

/****************************************************************************
 * Function Name  : rrm_sleep
 * Inputs         : channel, timeout
 * Outputs        : None
 * Returns        : None
 * Description    : Function to sleep a thread when waiting for message
 ****************************************************************************/

static	void	rrm_sleep( void *channel, const QTIME *timeout )
{
    __waitdata	*w = (__waitdata *)channel;
    struct timespec	T;

    pthread_mutex_lock( &(w->lock) );
    if( !w->value ) {
        if( !timeout ) {
            /*coverity_115242 Start*/
            if(pthread_cond_wait( &(w->condition), &(w->lock))) 
                RRM_TRACE(RRM_TRUE, p_rrm_encaps_facility_name, RRM_ERROR,"pthread_cond_wait returned failure");
            /*coverity_115242 Stop*/
        } else if ( timeout->s || timeout->us ) {
            struct timeval	_T;

            gettimeofday( &_T, 0 );

            _T.tv_sec += timeout->s;
            _T.tv_usec += timeout->us;
            while( _T.tv_usec >= 1000000L ) {
                _T.tv_usec -= 1000000L;
                _T.tv_sec++;
            }

            T.tv_sec = _T.tv_sec;
            T.tv_nsec = 1000 * _T.tv_usec;
            /* SPR 21978: CID 115456 Start */
            if(pthread_cond_timedwait( &(w->condition), &(w->lock), &T ))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_encaps_facility_name, RRM_ERROR,"pthread_cond_timedwait returned failure");
            }
            /* SPR 21978: CID 115456 End */
        } else {
            /** Nothing -- zero timeout was specified **/
        }
    }
    w->value = 0;
    pthread_mutex_unlock( &(w->lock) );
}

/****************************************************************************
 * Function Name  : rrm_wakeup
 * Inputs         : channel
 * Outputs        : None
 * Returns        : None
 * Description    : Function to wakeup the sleeping thread
 ****************************************************************************/

static	void	rrm_wakeup( void *channel )
{
    __waitdata	*w = (__waitdata *)channel;

    pthread_mutex_lock( &(w->lock) );
    w->value = 1;
    pthread_cond_signal( &(w->condition) );
    pthread_mutex_unlock( &(w->lock) );
}

/* Structure definitions */
const QSYSOP rrm_os = {
    rrm_malloc,
    rrm_free,
    rrm_newkey,
    rrm_getkey,
    rrm_setkey,
    rrm_newlock,
    rrm_destroylock,
    rrm_lock,
    rrm_unlock,
    rrm_vprintf,
    rrm_vprintf,
    rrm_abort
};

const QWAIT rrm_waitstruct = {
    rrm_newchannel,
    rrm_sleep,
    rrm_wakeup,
    rrm_walltime
};
#else
const QSYSOP rrm_os = {
    rrm_malloc,
    rrm_free,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    rrm_vprintf,
    rrm_vprintf,
    rrm_abort
};

const QWAIT rrm_waitstruct = {
    0,
    0,
    0,
    rrm_walltime
};
#endif

/*SPR 22554 Fix Start*/
/************************************************************************************
 *   FUNCTION NAME : rrm_pthread_setname_np
 *   INPUT         : thread,threadName
 *   OUTPUT        : 
 *   RETURNS       : 
 *   DESCRIPTIOPN  : This is a porting function to set name of thread
 *************************************************************************************/
U16
rrm_pthread_setname_np
(
pthread_t *thread,
U8 *threadName
)
{
    return (pthread_setname_np(*thread, (const char *)threadName));
}
/*SPR 22554 Fix End*/

const QSHELL rrm_shell = {
    rrm_timedwait,
    rrm_send,
    rrm_receive,
    rrm_open,
    rrm_close,
    0
};

#ifdef LINUX_PC_TEST
extern short test_log(U16 usModId, U16 logLevel, char* fmt, ...);
extern U8 qvDeliverForL3Test(QMODULE from, QMODULE to, void *message);
U8 for_adapter_send_data_to_queue(U16 ulSrcModId, U16 ulDstModId, U16 ulMsgId, char *msg, U32 ulMsgLen)
{
    U8  *pucMsgBuf = PNULL;
    U8  ret = 0;
    QMODULE pstSrcModule = qvGetService(ulSrcModId);
    QMODULE pstDstModule = qvGetService(ulDstModId);
    
    test_log(RRC_MODULE_ID, 1, "%s: SRC=%ld[%ld], DST=%ld[%ld], MSG=%ld, LEN=%ld", 
        __FUNCTION__, ulSrcModId, qvGetServiceName(pstSrcModule), 
        ulDstModId, qvGetServiceName(pstDstModule), ulMsgId, ulMsgLen);
        
    pucMsgBuf = rrm_msg_mem_get(ulMsgLen);
    if (pucMsgBuf == PNULL)
    {
        test_log(RRC_MODULE_ID, 2, "%s: rrc_msg_mem_get failure!!!!!");
        return 1;
    }

    memcpy(pucMsgBuf, msg, ulMsgLen);

    ret = qvDeliverForL3Test(pstSrcModule, pstDstModule, pucMsgBuf);
    test_log(RRC_MODULE_ID, 1, "%s: qvDeliverForL3Test RET=%d", __FUNCTION__, ret); 

    return 0;
}
#endif

