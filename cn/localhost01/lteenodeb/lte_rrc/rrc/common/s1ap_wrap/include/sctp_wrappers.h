/****************************************************************************
*
*  ARICENT -
*
*  Copyright (c) 2009 Aricent.
*
****************************************************************************
* File Details
* ------------
*  $Id: sctp_wrappers.h,v 1.4 2010/10/13 07:02:19 gur18569 Exp $
****************************************************************************
*
*  File Description : 
*
****************************************************************************
*
* Revision Details
* ----------------
*
* $Log: sctp_wrappers.h,v $
* Revision 1.4  2010/10/13 07:02:19  gur18569
* Merge from LTE_L3_Rel_2_0_2 tag tag
*
* Revision 1.3.4.2  2010/09/28 15:55:14  gur18569
* increased recv buff size
*
* Revision 1.3.4.1  2010/06/24 12:43:56  gur22047
* internal source and s1ap code
*
* Revision 1.3  2010/03/24 09:50:40  gur18569
* Merged files from Rel 1.0 support branch
*
* Revision 1.2.2.5  2010/03/05 08:57:29  gur21006
* char changed to S8, int changed to S32
*
* Revision 1.2.2.4  2010/03/05 05:53:45  gur21006
* warnings removed
*
* Revision 1.2.2.3  2010/02/02 13:18:15  ukr15916
* tabs->spaces expanded
*
* Revision 1.2.2.2  2010/01/28 12:20:03  gur21006
* lksctp support added
*
* Revision 1.2.2.1  2010/01/28 10:52:49  gur20470
* Removed Compilation Warnings
*
* Revision 1.2  2010/01/04 16:10:05  ukr15916
* no message
*
* Revision 1.1.2.11  2009/12/22 14:35:21  gur18569
* fixed 164
*
* Revision 1.1.2.10  2009/12/19 06:33:48  gur21006
* Buffer length modified
*
* Revision 1.1.2.9  2009/12/11 04:49:14  gur21006
* Reduced count for Init retry to 3
*
* Revision 1.1.2.8  2009/12/02 07:15:05  gur20470
* Included the prototype of l3_init_sctp_stack
*
* Revision 1.1.2.7  2009/11/25 14:03:24  gur18569
* added S1AP_UT_TESTING related changes
*
* Revision 1.1.2.6  2009/11/23 07:25:28  gur21006
* Function defination of l3_sctp_close_association changed again
*
* Revision 1.1.2.5  2009/11/23 06:45:04  gur21006
* Function defination of l3_sctp_close_association changed
*
* Revision 1.1.2.4  2009/11/20 07:15:11  gur21006
* function prototype of l3_open_sctp_association modified
*
* Revision 1.1.2.3  2009/11/11 09:55:19  gur21006
* l3_sctp_close_socket Function Prortotype added
*
* Revision 1.1.2.2  2009/11/04 11:10:53  gur21006
* Compilation errors removed
*
* Revision 1.1.2.1  2009/10/23 16:16:38  gur18569
* Initial version
*
*
*
****************************************************************************/




#ifndef _SCTP_WRAPPERS_H
#define _SCTP_WRAPPERS_H


/****************************************************************************
* Project Includes
****************************************************************************/

#include "stacklayer.h"
#include "s1ap_global_ctx.h"
#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "m2ap_api.h"
#endif


/****************************************************************************
* Exported Includes
****************************************************************************/
#if 0
    #ifdef ARICENT_SCTP_IPR
        #include <s_cogbl.h>
        #include <s_cotyps.h>
        #include <s_osmem.h>
        #include <s_uisuif.h>
        #include "s_uisusk.h"
        #include "s_cspl.h"
        #include "s_cotyps.h"
        #include "sctp.h"
        #include "s_select.h"
        #include "sig_ipc_intr.h"
        #include "sig_typs.h"
        #include "sig_cspl_wrap.h"
    #endif
#endif	

    #ifdef LKSCTP
        //#include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/sctp.h>

        #define SCTP_ADDRESS_UNREACHABLE           SCTP_ADDR_UNREACHABLE
        #define SCTP_ADDRESS_AVAILABLE             SCTP_ADDR_AVAILABLE
        #define SCTP_COMMUNICATION_UP              SCTP_COMM_UP
        #define SCTP_COMMUNICATION_LOST            SCTP_COMM_LOST
        #define SCTP_SHUTDOWN_COMPLETE             SCTP_SHUTDOWN_COMP
        #define SCTP_CANT_START_ASSOC              SCTP_CANT_STR_ASSOC

        #define SCTP_PARTIAL_DELIVERY              SCTP_PARTIAL_DELIVERY_EVENT
        #define SCTP_ADAPTION_EVENT                SCTP_ADAPTATION_INDICATION

#endif




/****************************************************************************
* Exported Definitions
****************************************************************************/
/* L3-X2AP Integration Activity - Start */


/* Bug-4680 FIX START */
extern S32 g_sctp_sd_list[MAX_PEER_ENB+1];
/* Bug-4680 FIX END */

extern U8 g_sctp_sd_count;
extern S32 g_m2ap_sctp_sd;
extern S32 g_m2ap_sctp_sd_count;

/* L3-X2AP Integration Activity - End */


/****************************************************************************
* Exported Types
****************************************************************************/

/****************************************************************************
* Exported Constants
****************************************************************************/

/****************************************************************************
* Exported Variables
****************************************************************************/
/* All the Active SCTP Sock.desc are set in this GLOBAL variable */
#ifdef LKSCTP
/* SPR 20633 changes start */
extern S32 g_sctp_server_fd_ipv4;
extern S32 g_sctp_server_fd_ipv6;
/* SPR 20633 changes end */
extern fd_set sctp_fd;
#endif

/****************************************************************************
* Exported Functions
****************************************************************************/

#define SCTP_PAYLOAD_PROTOCOL_ID_S1AP 18
#define MAX_IP_ADDR            10
#define MAX_SCTP_INIT_ATTEMPT  3
#define MAX_BUFFER_SIZE             30000

#define S1AP_SCTP_ERROR            (-1)
#define SCTP_SOCKET_ERROR            (-1)
#define INVALID_SCTP_SOCKET        (-1)
#define SCTP_SD_OCTET_SIZE          4
#define SCTP_STREAM_ID_SIZE          2
#define SCTP_COMMON_PROC_STREAMS               0

#define SA_FAMILY_T sa_family_t


rrc_bool_et is_sctp_stack_init(void);
#if 0
#ifdef ARICENT_SCTP_IPR

/* SPR 20633 changes start */
extern S32 g_sctp_server_fd;
/* SPR 20633 changes end */
U32 l3_sctp_init_stack(
            U8 cspl_already_intialised,
            enb_comm_info_t *p_enb_comm_info);

#endif
#endif	

void* l3_sctp_receive(
                      QMODULE *sender,
                      QMODULE *receiver,
                      S8 *priority);


/*SPR 15570 Fix Start*/
S32 l3_retry_connect(s1ap_gb_context_t *p_s1ap_gb_ctx ,
		     mme_context_t *p_mme_ctx);
/*SPR 15570 Fix Stop*/
S32 l3_open_sctp_association(s1ap_gb_context_t *p_s1ap_gb_ctx,
                             mme_context_t *p_mme_ctx,
                             enb_comm_info_t *p_enb_comm_info);

#if 0
    #ifdef ARICENT_SCTP_IPR
    S32 l3_sctp_send(
                 sctp_sd_t                    socket,
                 const void            *p_message,
                 sctp_socklen_t         msg_len,
                 U32                    stream_no);
    #endif
#endif	

    #ifdef LKSCTP
    S32 l3_sctp_send(
                 sctp_sd_t                    socket,
                 const void            *p_message,
                 socklen_t              msg_len,
                 U32                    stream_no);
   /* Coverity_86875 Fix Start */
     S32 l3_sctp_association_setup(
            s1ap_gb_context_t *p_s1ap_gb_ctx,
            mme_context_t *p_mme_ctx,
            enb_comm_info_t *p_enb_comm_info);
    #endif

rrc_return_et l3_sctp_close_association(sctp_sd_t  socket);

rrc_return_et l3_sctp_close_socket(sctp_sd_t  socket);

/* L3-X2AP Integration Activity - Start */

typedef enum
{
   SCTP_FD_NOT_SET=0,
   S1AP_SCTP_FD_SET,
#ifdef LTE_EMBMS_SUPPORTED
   X2AP_SCTP_FD_SET,
   M2AP_SCTP_FD_SET
#else
   X2AP_SCTP_FD_SET
#endif
} s1ap_x2ap_sctp_fd;

/* L3-X2AP Integration Activity - End */


#endif
