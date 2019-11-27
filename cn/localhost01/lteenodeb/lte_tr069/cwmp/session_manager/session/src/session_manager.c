/********************************************************************************
 *
 *      FILE NAME   : session_manager.c
 *
 *      DESCRIPTION : This file is used for maintaing the session
 *
 *      REVISION HISTORY:
 *
 *      NAME             DATE            REFERENCE               REASON
 *_______________________________________________________________________________
 *
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 29,2013                          Added HAVE_IPSEC
 *   Anant K Sharma    Apr 08,2013                          Added check for connectivity of servers after tunnel establishment
 *
 *
 *       All Rights Reserved. Copyright @ Aricent Group, 2012.
 *
 ********************************************************************************/


/******************************************************************************
 *
 *                               HEADER FILES
 *
 *******************************************************************************/
#include 	<session_manager.h>
#include        <session_retry.h>
#include        <logger.h>
#include        <stub.h>
#include        <stdlib.h>
#include 	<unistd.h>
#include 	<signal.h>
#include 	<cspl.h>
/* SPR 13359 Fix Start */
#include 	<map_glue.h>
/* SPR 13359 Fix Start */

/******************************************************************************
  Function Declarations
 ******************************************************************************/
/* SPR 16568 FIX START */
extern struct Device_ManagementServer g_ipr_cwmp_S_Device_Man_Ser; 
extern UInt8 g_flag_to_notify_connection_req_url;
/* SPR 16568 FIX END */
/*SPR 17929 change start*/	
void initialize_cookie(void);
/*SPR 17929 change end*/

/*SPR 18033 FIXED START */
extern U8 g_queued_wait_inform;
/*SPR 18033 FIXED END */

inline void ipr_cwmp_free_stat_status(StStatus *);
inline void ipr_cwmp_free_get_param(ParameterNames *);
void ipr_cwmp_free_FileTypeArg(struct FileTypeArg *);
struct SOAP_ENV__Fault *ipr_cwmp_construct_soap_fault (UINT16 code);
extern Uint8 *ipr_cwmp_time_stamp (void);
void yeMd5Digest( const unsigned char *buffer, unsigned int length, unsigned char result[16] );
extern UInt32 ipr_cwmp_send_http_msg(Uint8 *, Uint8 *, Uint8 **, Uint8 *, Uint8*);
extern UInt32 ipr_cwmp_send_empty_http_msg(Uint8 *, Uint8 *, Uint8 **, Uint8 *, Uint8*);
extern void ipr_clean_http_session(void);

void stub_print_acs_rpc_list(struct MethodList *List);
extern void ipr_cwmp_acs_discovery(Uint8);
/*SPR 16672 Fixed Start */
extern U8 g_inform_set_to_value ;

/*SPR 16672 Fixed End */

/* Periodic Inform Changes Start */
extern U8 g_periodic_inform_set_to_value;
/* Periodic Inform Changes End */

/* Spr 16990 Fix Start */
extern void free_gpn_resp_buffer (GetParameterNamesStatus_t *p_gpn_res_t);
/* Spr 16990 Fix End */

/* Spr 17463 Fix Start */
/* SPR 21563_p1_FIX */
void free_gpv_resp_buffer (struct ParameterValueList *p_get_stat_t);
/* SPR 21563_p1_FIX */
/* Spr 17463 Fix End */

/* SPR 20703 FIXED START */
#define EMPTY 2
int send_curl_err_oam_g = 0;
extern int MaxSessionRetryCount;
/* SPR 20703 FIXED END */

/* SPR 17268 FIXED START */
#define CONFIG_PARAM_LEN 256
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */

/* SPR 17596 fixed start */
GetRPC_t * ipr_cwmp_GetRPCMethods_rpc();
/* SPR 17596 fixed end */

#define SESSION_TERMINATED_UNSUCCESSFULLY 8005

/* GET RPC Method Change gur29108 Start */
#define MAX_NUMBER_RPC_SUPPORTED 17
/* GET RPC Method Change gur29108 End */


//IPSEC SIT code starts
#ifdef HAVE_IPSEC
#include <ipr_cwmp_read_config.h>
#include <ipr_cwmp_enb_wrapper.h>
#include  <ipr_cwmp_ping_util.h>

#ifdef HAVE_IPSEC
#include <ipr_cwmp_policy_handler.h>
#include <ipr_cwmp_state_handler.h>
#endif

#include <ManagementServerModel.h>
#include <ipr_cwmp_sig_handler.h>
//#include <glue_layer_def.h>
#define TRO69_IP_SAVE "./tr069_ip_save" 
#endif
//IPSEC SSI code ends

/******************************************************************************
  Global Variables
 ******************************************************************************/
/* Array for error Strings */
Uint8 g_session_manager_error_string_array[33][100] = {
    {"Method not supported"},
    {"Request denied no reason specified"},
    {"Internal error"},
    {"Invalid arguments"},
    {"Resources exceeded"},
    {"Retry request"},
    {"Vendor defined fault codes"}
};

/* Global vaiable to indicate if session is opened */
SInt32 g_session_manager_session_open = 0 ; 

/* Variable used in connection manager for number of connection requests */
extern SInt32 g_ipr_cwmp_con_req;

struct ParameterValueList *g_queued_paramList = NULL; 
U8 g_queued_paramList_size = 0;


#ifdef VENDOR_SPECIFIC
struct ParameterValueList g_forcedParamList = {0};
#endif
//U8 g_IsValueChanged = 0;

//U8 g_send_notif_fail = 0;
U8 g_expedited_inform = 0;


extern U8  iHems_url[URL_SIZE];

#ifdef VENDOR_SPECIFIC
extern U8 shems_Session;
extern U8 iHems_session;
extern U8 g_Lte_LastScanTime[256];
#endif


static char g_cpeError_str_arr[33][100] = {
    {"Method not supported"},
    {"Request denied no reason specified"},
    {"Internal error"},
    {"Invalid arguments"},
    {"Resources exceeded"},
    {"Invalid parameter name"},
    {""},
    {""},
    {""},
    {"Notification request rejected"}
};


/* SPR 16448 Fix START */
void freeSTFaultParameters(STFault *pstFault)
{
    if(pstFault->FaultCode)
		DEALLOCATE (pstFault->FaultCode);
    if (pstFault->FaultString)
		DEALLOCATE(pstFault->FaultString);

    /*  coverity 94829 fix start */
    if( (pstFault->__sizeSetParameterValuesFault) &&
            (pstFault->SetParameterValuesFault) )
        /*  coverity 94829 fix end */
    {
        if (pstFault->SetParameterValuesFault->ParameterName)
			DEALLOCATE(pstFault->SetParameterValuesFault->ParameterName);
        if (pstFault->SetParameterValuesFault->FaultCode)
			DEALLOCATE(pstFault->SetParameterValuesFault->FaultCode);
        if (pstFault->SetParameterValuesFault->FaultString)
			DEALLOCATE(pstFault->SetParameterValuesFault->FaultString);

        /*  coverity 94829 fix code removed */
		DEALLOCATE(pstFault->SetParameterValuesFault);
    }
    return;
}

void freeSoapFaultParameters(struct SOAP_ENV__Fault *soap_fault)
{
    if (soap_fault)
    {
         if(soap_fault->faultstring)
			DEALLOCATE (soap_fault->faultstring);

         if(soap_fault->faultcode)
			DEALLOCATE (soap_fault->faultcode);
         if (soap_fault->detail)
         {
             if(soap_fault->detail->__any)
                 freeSTFaultParameters(soap_fault->detail->__any);
			DEALLOCATE (soap_fault->detail);
         }
		DEALLOCATE (soap_fault);
    }
    return;
}
/* SPR 16448 Fix End */


CHAR8 * ipr_cwmp_cpeErr_to_string (UINT16 errorno)
{
    if (errorno >= 9000 && 9009 >= errorno)
    {
        return g_cpeError_str_arr[errorno - 9000];
    }
    else
        return "Not defined";

}



/******************************************************************************
  Function Definitions
 ******************************************************************************/
inline void free_transferList (AllTransferList *a);

/*********************************************************************************
 *
 *       Function Name : err_to_string
 *
 *       Description   : This function returns error string for error code sended by ACS
 *
 *       Returns       : Uint8 * ( error string )
 *
 *********************************************************************************/

Uint8 * err_to_string (SInt32 errorno)
{
    if (errorno >= 8800 && 8806 >= errorno)
    {
        return g_session_manager_error_string_array[errorno - 8800];
    }
    if (errorno >= 8807 && 8899 >= errorno)
    {
        return g_session_manager_error_string_array[6];
    }
    return NULL;
}


/* SPR 22314 Start */
void  free_informReq_buffer(sInform *informReq)
{
    int param_count = 0;
    if(NULL == informReq)
    {
        return;
    }
    if (NULL != informReq->Event)
    {

        if (NULL != informReq->Event->__ptrEventStruct)
        {
            for (param_count = 0; param_count < informReq->Event->__size; param_count++)
            {
                if(NULL !=informReq->Event->__ptrEventStruct[param_count])
                {
                    if (NULL != informReq->Event->__ptrEventStruct[param_count]->EventCode)
                    {
                        DEALLOCATE(informReq->Event->__ptrEventStruct[param_count]->EventCode);
                    }

                    if (NULL != informReq->Event->__ptrEventStruct[param_count]->CommandKey)
                    {
                        DEALLOCATE(informReq->Event->__ptrEventStruct[param_count]->CommandKey);
                    }
                    DEALLOCATE(informReq->Event->__ptrEventStruct[param_count]);
                }
            }

            DEALLOCATE(informReq->Event->__ptrEventStruct);
        }

        DEALLOCATE(informReq->Event);
    }

    if (NULL != informReq->ParameterList)
    {
        if(NULL != informReq->ParameterList->__ptrParameterValueStruct)
        {
                for (param_count = 0; param_count < informReq->ParameterList->__size; param_count++)
            {
                if(NULL !=informReq->ParameterList->__ptrParameterValueStruct[param_count])
                {
                    if(NULL != informReq->ParameterList->__ptrParameterValueStruct[param_count]->Name)
                    {
                        DEALLOCATE(informReq->ParameterList->__ptrParameterValueStruct[param_count]->Name);
                    }
                    if(NULL != informReq->ParameterList->__ptrParameterValueStruct[param_count]->Value)
                    {
                        DEALLOCATE(informReq->ParameterList->__ptrParameterValueStruct[param_count]->Value);
                    }

                    DEALLOCATE (informReq->ParameterList->__ptrParameterValueStruct[param_count]);
                }
            }
            DEALLOCATE(informReq->ParameterList->__ptrParameterValueStruct);
        }
        DEALLOCATE(informReq->ParameterList);
    }
    DEALLOCATE(informReq);
}

/* SPR 22314 End */
UInt8 *ipr_cwmp_create_sessionId(void)
{
	UInt8 *session_id = NULL;
	session_id = (UInt8 *) ALLOCATE(33 );
	if (NULL == session_id)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		exit(0);
	}
	Uint8 *time_str = NULL;
	UInt8 temp_hex[33] = {'\0'};
	time_str = ipr_cwmp_time_stamp();
	if (NULL == time_str)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s  As time str fail",__FUNCTION__);
		exit(0);
	}
	ipr_cwmp_hex_2_str(temp_hex, session_id);
	yeMd5Digest((unsigned char *)time_str, strlen(time_str), temp_hex);
	DEALLOCATE(time_str);
	ipr_cwmp_hex_2_str(temp_hex, session_id);
	return session_id;
}


static void hdl (SInt32 sig, siginfo_t *siginfo, void *context)
{
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "sig %d",sig);
    if((NULL != siginfo) && (NULL != context))
    {    
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "siginfo %p",siginfo);
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "context %p",context);
    }
    ipr_cwmp_set_retry_count(SUCCESS);
}


/*********************************************************************************
 *
 *       Function Name : ipr_cwmp_isSessionTerminated
 *
 *       Description   : This function returns the TRUE 
 *                       	If session is needed to be terminate
 *       		This function returns the FALSE 
 *                       	If session is not needed to be terminate
 *
 *       Returns       : TRUE / FALSE
 *
 *********************************************************************************/

UINT4 ipr_cwmp_isSessionTerminated (Uint8 * resp_xml, 
        UINT4 flag)
{
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    if ((NULL == resp_xml || (0 == strncmp(resp_xml, "empty", 5))) && (flag == TRUE))
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with status TRUE ",__FUNCTION__);
        return TRUE;
    }
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s  with status FALSE ",__FUNCTION__);
    return FALSE;
}




/********************************************************************************
 * 
 *       Function Name : SessionInit
 *
 *       Description   : This function is used to initiate the session for 
 *       			new in coming events.
 *
 *       Returns       : status ( SUCCESS / FAILED )
 *
 *********************************************************************************/

SInt32 ipr_cwmp_SessionInit(DeviceInfo *devidstruct, 
        Uint8 *url, 
        struct ParameterValueList *paramList,
        Uint8 *username,
        Uint8 *password)
{

        /* SPR 20703 FIXED START */
   UInt32 status_http = 0;
	int curl_err_code = 0;
    const char *curl_err_string = NULL;
    char curl_err_str[FAULT_STRING_SIZE]={0};
        /* SPR 20703 FIXED END */
    SInt32 status;
    Uint8 *msg = "";
    Uint8 *resp_xml = NULL;
    sHeader *header = NULL;
    sHeader rheader ={NULL,NULL};
    Uint8 URL[MAX_URL_LEN + 1] = {'\0'};
    SInt32 type  = 0 ;
    UINT4 acsPendReq  = FALSE;
    UINT4 emptyrequest_with_hold_req_false  = FALSE;
    UINT4 cpe_req_count = 0;
    /* SPR 16327 Fixed Start  */
    /*code remove */
    /* SPR 16327 Fixed End  */
    void *resp = NULL;
    void *resp_rpc = NULL;
    Uint8 *session_id = NULL;
    /* SPR 21002 FIXED START */
    UInt8 ABSPath3[CONFIG_LEN] = {'\0'};
    strcpy((char *)ABSPath3,(char *)ABSPath);
    strncat((char *)ABSPath3 ,"/reset" , strlen("/reset"));
		UInt8 ABSPath4[CONFIG_LEN] = {'\0'};
		strcpy((char *)ABSPath4,(char *)ABSPath);
		strncat((char *)ABSPath4 ,"/retry" , strlen("/retry"));
    /* SPR 21002 FIXED END */

    //IPSec phase 2 code starts 
#ifdef HAVE_IPSEC
    UInt8 isTunnelError = 0;
#endif
    //IPSec phase 2 code ends
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    /* Use the sa_sigaction field because the handles has two additional parameters */
    act.sa_sigaction = &hdl;

    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR2, &act, NULL) < 0) {
        perror ("sigaction");
        return 1;
    }

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
#ifdef HAVE_IPSEC
	config_res_t *p_config_res = (config_res_t*)ALLOCATE(sizeof(config_res_t));
    if (NULL == p_config_res)
    {
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
        exit(0);
    }
    FILE* file = fopen(TRO69_IP_SAVE, "r");
    if(file == NULL)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Error opening file to restore saved IPs");
        return -1;
    }
    fread(p_config_res, sizeof(config_res_t), 1, file);
    fclose(file);
    if(!have_tunnel && have_ipsec && strlen(p_config_res->HeNBIPAddr) != 0 &&
            (strlen(p_config_res->SecGWServer1) != 0 ||
             strlen(p_config_res->SecGWServer2) != 0 || strlen(p_config_res->SecGWServer3) != 0))
    {
        if(strlen(p_config_res->SecGWServer1) != 0)
        {
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                        (UInt8*)p_config_res->SecGWServer1, 
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",
                        p_config_res->HeNBIPAddr,p_config_res->SecGWServer1);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ",
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer1, 
                        (UInt8*)p_config_res->HeNBIPAddr,
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",
                        p_config_res->SecGWServer1,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ",
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer1, p_config_res->HeNBIPAddr,
                        ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add in policy src = %s and dst = %s failed ",
                        p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            } else if(!isTunnelError)
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and dst = %s",
                        p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);

            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                        p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr,
                        (UInt8*)p_config_res->SecGWServer1, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add out policy src = %s and dst = %s failed ",
                        p_config_res->HeNBIPAddr,p_config_res->HeMSIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer1);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                }
            }
        }//end of SECGW1
        else
            isTunnelError = 1;//set this to try other 2 GWs if any
        if(isTunnelError && strlen(p_config_res->SecGWServer2) != 0)
        {
            isTunnelError = 0;
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                        (UInt8*)p_config_res->SecGWServer2, 
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",
                        p_config_res->HeNBIPAddr,p_config_res->SecGWServer2);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ",
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer2, 
                        (UInt8*)p_config_res->HeNBIPAddr, 
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",p_config_res->SecGWServer2,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->SecGWServer2, p_config_res->HeNBIPAddr);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer2, p_config_res->HeNBIPAddr,
                        ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add in policy src = %s and dst = %s failed ",p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and dst = %s",p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);
            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                        p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr,
                        (UInt8*)p_config_res->SecGWServer2, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add out policy src = %s and dst = %s failed ",p_config_res->HeNBIPAddr,p_config_res->HeMSIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer2);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                }
            }
        }//end of SECGW2
        if(isTunnelError && strlen(p_config_res->SecGWServer3) != 0)
        {
            isTunnelError = 0;
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                        (UInt8*)p_config_res->SecGWServer3, 
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",p_config_res->HeNBIPAddr,p_config_res->SecGWServer3);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer3, 
                        (UInt8*)p_config_res->HeNBIPAddr, 
                        (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                        PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add sate src = %s and dst = %s failed ",p_config_res->SecGWServer3,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");
            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                        p_config_res->HeNBIPAddr, p_config_res->SecGWServer3,
                        p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add in policy src = %s and dst = %s failed ",p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and dst = %s",p_config_res->HeMSIPAddr,p_config_res->HeNBIPAddr);
            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                        p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr,
                        (UInt8*)p_config_res->SecGWServer3, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "add out policy src = %s and dst = %s failed",p_config_res->HeNBIPAddr,p_config_res->HeMSIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer3);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                }
            }
        }//end of SECGW3
    }
    //Notify OAM of tunnel establishment error, if any
    if (isTunnelError)
    {

        TRACE (LOG_CRITICAL , SESSION_MANAGER, "skipping sending tunnel error in session manager.c");
    }
    else
    {
        have_tunnel = 1;
    }

#endif
    // end of IPSEC SSI code


    if (NULL == url)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL URL",__FUNCTION__);
        return FAILED;
    }

    if (NULL == devidstruct)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL devidstruct",__FUNCTION__);
        return FAILED;
    }
    ENB_STRNCPY (URL, url, MAX_URL_LEN);
    URL[MAX_URL_LEN] = '\0';
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "URL => %s \n\n",URL);

    sInformResponse *informResp = NULL;
    sInform *informReq = {0};

    cpe_req_count = g_clientRequestQ.count;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Parameter List start" );
    stub_print_param_list(paramList);

    informReq = ipr_cwmp_createInformReq(devidstruct, paramList, NULL);
    if (NULL == informReq)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL informReq",__FUNCTION__);
        return FAILED;
    }
    informResp = ipr_cwmp_Inform(informReq, URL,  &status, &header, username, password, &session_id);
    /*spr 17966 fix start*/
    if (informResp != NULL)
    {
        ENB_FREE (informResp );
    }
    /*spr 17966 fix end*/
    informResp = NULL;
    ipr_cwmp_set_retry_count(status);
    if (status != SUCCESS)
    {		
        /* SPR 16451 Fix Start */
        if(status == CRITICAL_ERROR) 
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with status %d ",__FUNCTION__, status);
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the eventlist added into queue %s ",__FUNCTION__);
            /*SPR 22452 FIXED START*/
            DEALLOCATE(session_id);
            /* SPR 22314 Start */
            TR069_FREE_SINFORM(informReq);
            /* SPR 22314 End */
            /*SPR 22452 FIXED END*/

            /*SPR 18317 FIXED START */
            /* sg changes */
            /*SPR 16672 Fixed Start */
            //g_inform_set_to_value = 0;
            /*SPR 16672 Fixed Start */
            /* Periodic Inform Changes Start */
            // g_periodic_inform_set_to_value = 0;
            /* Periodic Inform Changes End */
            /* sg changes */
            /*SPR 18317 FIXED END */
            return FAILED;
        }
        /* SPR 16451 Fix End */

        informResp = ipr_cwmp_Session_Retry(informReq, URL, header, username, password, &session_id);
        ipr_cwmp_set_retry_count(SUCCESS);
        if ( NULL == informResp )
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with status %d ",__FUNCTION__, status);
            /* SPR 18228 FIXED START */
            DEALLOCATE(session_id);
            /* SPR 18228 FIXED END */
            /*SPR 22452 FIXED START*/ 
            /* SPR 22314 Start */
            TR069_FREE_SINFORM(informReq);
            /* SPR 22314 End */
            /*SPR 22452 FIXED END*/

            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the eventlist added into queue %s ",__FUNCTION__);
            return FAILED;
        }
        else
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the eventlist added into queue %s ",__FUNCTION__);
            ENB_FREE (informResp );
            /* SPR 16327 Fixed Start  */
            /* code remove */
            /* SPR 16327 Fixed Start  */
        }
    }/* End of if (status != SUCCESS) */
    else
    {
        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */

        /*SPR 22452 FIXED START*/
        /* CODE REMOVE */
        /*SPR 22452 FIXED END*/                         
    
       
          
		/* SPR 21563 FIXED START*/
        /* code removed */
        /* SPR 21563 FIXED END*/
        /* SPR 20703 FIXED START */
        //informReq = NULL;
        /* spr 18338 fix start */

        /* SPR 19441 FIXED END*/            
        /* SPR 21002 FIXED START */
        if (-1 == remove((char *)ABSPath3))
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "file removal fail");

        }
        if (-1 == remove((char *)ABSPath4))
        {

            TRACE (LOG_CRITICAL , SESSION_MANAGER, "file removal fail");

        } 
        /* SPR 21002 FIXED END */
    }

    /*
     * Check that client request is Pending or not
     */
    while(1)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "cpe_req_count = %d", cpe_req_count);
        if (cpe_req_count > 0 )
        {
            if (header && header->__HoldRequests &&
                    (header->__HoldRequests->__item == TRUE))
            {
                TRACE (LOG_CRITICAL, SESSION_MANAGER, "Hold Request from ACS = %d",header->__HoldRequests->__item);
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "acsPendReq = %d",acsPendReq);
                if(acsPendReq == TRUE)
                {
        /* SPR 20703 FIXED START */
					status_http = ipr_cwmp_send_http_msg(resp_rpc, URL, &resp_xml, username, password);
        /* SPR 20703 FIXED END */
                    DEALLOCATE(resp_rpc);
                }/* End if(acsPendReq == TRUE) */
                else 
                {
                    ipr_cwmp_send_empty_http_msg(msg, URL, &resp_xml, username, password);
                }/* End else of if(acsPendReq == TRUE) */

            }/*End if (header && header->__HoldRequests && (header->__HoldRequests->__item == TRUE)) */
            else
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "acsPendReq = %d",acsPendReq);
                if(acsPendReq == TRUE)
                {
        /* SPR 20703 FIXED START */
					status_http = ipr_cwmp_send_http_msg(resp_rpc, URL, &resp_xml, username, password);
        /* SPR 20703 FIXED END */
                    DEALLOCATE(resp_rpc);
                }/* if(acsPendReq == TRUE) */
                else 
                {
                    ClientRequest *fetch_node = NULL;
                    if (g_clientRequestQ.count > 0)
                    {
                    }
                    fetch_node = (ClientRequest *) getClientReq (&g_clientRequestQ);
                    if(NULL != fetch_node)
                    {
                        /* Send Autonomous Complete Transfer */
                        switch (fetch_node->eReqtype)
                        {
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"Type = %d",fetch_node->eReqtype);
                            case  TRANS_COMPLETE_REQ :
                            {
                                if (NULL != fetch_node->pstTxComplete)
                                {
                                    ipr_cwmp_TransferComplete(fetch_node->pstTxComplete, URL, &status ,username ,password);
                                }
                                else
                                {
                                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "%s with NULL Tx complete Req",__FUNCTION__);
                                }
                                break ;
                            }

                            case  AUTONOMOUS_COMPLETE_REQ :
                            {

                                if (NULL != fetch_node->pstAutoTxComplete)
                                {
                                    ipr_cwmp_AutoTransCompReq(fetch_node->pstAutoTxComplete, URL, &status ,username ,password);	
                                }
                                else
                                {
                                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL Auto Tx complete Req",__FUNCTION__);		
                                }
                                break ;
                            }

                            case  REQUEST_DOWNLOAD_REQ :
                            {
                                TRACE (LOG_CRITICAL , SESSION_MANAGER," Received Type = %d",fetch_node->eReqtype);
                                if (NULL != fetch_node->pstReqDownload)
                                {
                                    ipr_cwmp_RequestDownload(fetch_node->pstReqDownload, URL, &status, username, password);
                                }
                                else
                                {
                                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL Request download Req",__FUNCTION__);
                                }


                                break ;
                            }
                            case GET_RPC_METHOD :
                            {
                                struct _cwmp__GetRPCMethodsResponse *response = NULL;
                                response = ipr_cwmp_GetRPCMethods_from_acs(URL, &status ,username, password);
                                TRACE (LOG_CRITICAL , SESSION_MANAGER, "ACS RPC List START");
                                if (response != NULL)
                                {
                                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "ACS RPC List END");
                                    stub_print_acs_rpc_list(response->MethodList);
                                }
                                break ;
                            }
                            case AUTONOMOUS_DU_STATE_CHANGE_COMPLETE_REQ :
                            {
                                TRACE (LOG_CRITICAL , SESSION_MANAGER," Received Type = %d",fetch_node->eReqtype);
                                if (NULL != fetch_node->pstAutoDUStateChangeComplete)
                                {
                                    ipr_cwmp_AutoDUStateChangeCompleteReq(fetch_node->pstAutoDUStateChangeComplete, URL, &status, username, password);
                                }
                                else
                                {
                                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL Request download Req",__FUNCTION__);
                                }


                                break ;
                            }


                        }/* Ens  switch (fetch_node->eReqtype) */
						/*DEALLOCATE (fetch_node->EventCode);
                          fetch_node->EventCode = NULL;
						  DEALLOCATE (fetch_node);
                          fetch_node = NULL;*/
                        acsPendReq = FALSE;
                        cpe_req_count-- ;
                        continue ;
                    }

                }/* else of if(acsPendReq == TRUE) */

            }/* else of if(header && header->__HoldRequests && (header->__HoldRequests->__item == TRUE))*/

        }/* End if(cpe_req_count > 0 ) */
        else
        {
            if(acsPendReq == TRUE)
            {
        /* SPR 20703 FIXED START */
              status_http = ipr_cwmp_send_http_msg(resp_rpc, URL, &resp_xml, username, password);
        /* SPR 20703 FIXED END */
                DEALLOCATE(resp_rpc);
            }/* End  if(acsPendReq == TRUE) */
            else 
            {
                ipr_cwmp_send_empty_http_msg("", URL, &resp_xml, username, password);
                emptyrequest_with_hold_req_false = TRUE;
            }/* End  else od if(acsPendReq == TRUE) */

        }/* End else of i (cpe_req_count > 0 ) */

        /*
         * decode response XML and break the while loop
         */
        if (ipr_cwmp_isSessionTerminated(resp_xml, emptyrequest_with_hold_req_false) == TRUE )
        {
            if (NULL != stCallbackFunList.sessionClosedCallback)
            {
                stCallbackFunList.sessionClosedCallback(session_id);
                g_session_manager_session_open = 0;
                ipr_clean_http_session();
            }
            g_session_manager_session_open = 0;
        /* SPR 20703 FIXED START */
        /* code removed */
        /* SPR 20703 FIXED END */
            
            /* MEMORY LEAK 31 DEC FIXED START*/
            /* MEMORY LEAK 5 JAN FIXED START*/
            if(resp_xml)
            {
				DEALLOCATE (resp_xml);
            }
            /* MEMORY LEAK 5 JAN FIXED END*/
            /* MEMORY LEAK 31 DEC FIXED END*/   
        /* SPR 20703 FIXED START */
         if (status_http != 0 && status_http != EMPTY) 
         { 
             TRACE (LOG_CRITICAL , SESSION_MANAGER, "status_http is %d\n", status_http);
             if(status_http == CRITICAL_ERROR)
             {
                 stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
             }
             switch (status_http)
             {
                 case CURLE_SSL_CACERT :
                     {
                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SSL_CACERT = %d", status_http);

                         curl_err_code = CURLE_SSL_CACERT;
                         curl_err_string = curl_easy_strerror(CURLE_SSL_CACERT);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                            if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount)
                            {
                            send_curl_err_oam_g = 1; 
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);

                            }
                         break;

                     }
                 case CURLE_SSL_CACERT_BADFILE :
                     {
                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SSL_CACERT_BADFILE = %d" , status_http);

                         curl_err_code = CURLE_SSL_CACERT_BADFILE;
                         curl_err_string = curl_easy_strerror(CURLE_SSL_CACERT_BADFILE);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);

                         }
                         break;

                     }
                 case CURLE_HTTP_RETURNED_ERROR :
                     {


                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_HTTP_RETURNED_ERROR = %d" , status_http);

                         curl_err_code = CURLE_HTTP_RETURNED_ERROR;
                         curl_err_string = curl_easy_strerror(CURLE_HTTP_RETURNED_ERROR);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                         }
                         break;

                     }

                 case CURLE_COULDNT_CONNECT :
                     {


                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_COULDNT_CONNECT = %d" , status_http);

                         curl_err_code = CURLE_COULDNT_CONNECT;
                         curl_err_string = curl_easy_strerror(CURLE_COULDNT_CONNECT);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                         }
                         break;

                     }

                 case CURLE_OPERATION_TIMEDOUT :
                     {


                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_OPERATION_TIMEDOUT = %d" , status_http);

                         curl_err_code = CURLE_OPERATION_TIMEDOUT;
                         curl_err_string = curl_easy_strerror(CURLE_OPERATION_TIMEDOUT);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                         }

                         break;

                     }

                 case CURLE_GOT_NOTHING :
                     {
                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_GOT_NOTHING = %d" , status);

                         curl_err_code = CURLE_GOT_NOTHING;
                         curl_err_string = curl_easy_strerror(CURLE_GOT_NOTHING);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                         }
                         break;



                     }
                 case CURLE_SEND_ERROR :
                     {
                         TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SEND_ERROR = %d" , status);

                         curl_err_code = CURLE_SEND_ERROR;
                         curl_err_string = curl_easy_strerror(CURLE_SEND_ERROR);
                         if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                             ENB_STRCPY(curl_err_str , curl_err_string);
                         TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                         if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                         {
                             send_curl_err_oam_g = 1;
                             stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                         }
                         break;



                     }


                 default :
                     {
                        /*SPR 21547 FIXED START*/
                        TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SEND_ERROR = %d" , status_http);
                        curl_err_code = status_http;
                        /* Coverity 114340 Fixed Start */
                        curl_err_string = curl_easy_strerror(CURLE_SEND_ERROR);
                        /* Coverity 114340 Fixed End */
                        /* Coverity 114693 Fixed Start */
                        /* Code Removed */
                        /* Coverity 114693 Fixed Start */
                        if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                            ENB_STRCPY(curl_err_str , curl_err_string);
                        TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                        if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                        {
                            send_curl_err_oam_g = 1;
                            stCallbackFunList.sessionFailedCallback(session_id,curl_err_code,curl_err_str);
                        }
                        /*SPR 21547 FIXED END*/

                        /*SPR 21547 FIXED START*/

                        /* Code remove */
                        /*SPR 21547 FIXED END*/
                       
                         break;
                     }
             }   
                ipr_cwmp_set_retry_count(status_http);
                TRACE(LOG_DETAIL , SESSION_MANAGER ,"debug after set ipr_cwmp_get_retry_count%d", ipr_cwmp_get_retry_count());
             /* SPR 22045 FIXED START */
             informReq->Event = NULL;
             /* SPR 22045 FIXED END */
            informResp = ipr_cwmp_Session_Retry(informReq, URL, header, username, password, &session_id);
                       
             if ( NULL == informResp )
             {
                 TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with status %d ",__FUNCTION__, status);
                 TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the eventlist added into queue %s ",__FUNCTION__);
				 /* SPR 21563 FIXED START*/
				 DEALLOCATE(session_id);
				 /* SPR 21563 FIXED END*/
             }
            /*
             else
             {
                 TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the eventlist added into queue %s ",__FUNCTION__);
                 ENB_FREE (informResp );
                 informResp = NULL;
                 }
                 */
         }
         /* SPR 20703 FIXED END */
         /* SPR 22314 Start */
         /* Code Deleted */
         /* SPR 22314 End */
         break;
        }
        if ( NULL == resp_xml )
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with status Failed ",__FUNCTION__);
			//DEALLOCATE(session_id);
            /* SPR 18228 FIXED START */	
			DEALLOCATE(session_id);
            /* SPR 18228 FIXED END */
            /*SPR 22452 FIXED START*/ 
            /* SPR 22314 Start */
            TR069_FREE_SINFORM(informReq);
            /* SPR 22314 End */
            /*SPR 22452 FIXED END*/
            return FAILED;
        }

        if (header)
        {
            if (header->__HoldRequests)
            {
                if (header->__HoldRequests->SOAP_ENV__mustUnderstand )
                {
                    ENB_FREE(header->__HoldRequests->SOAP_ENV__mustUnderstand);
                    header->__HoldRequests->SOAP_ENV__mustUnderstand = NULL;
                }
                ENB_FREE( header->__HoldRequests);
                header->__HoldRequests = NULL;
            }
            if (header->__ID)
            {
                if (header->__ID->SOAP_ENV__mustUnderstand )
                {
                    ENB_FREE(header->__ID->SOAP_ENV__mustUnderstand);
                    header->__ID->SOAP_ENV__mustUnderstand = NULL;
                }
                if (header->__ID->__item)
                {
                    ENB_FREE(header->__ID->__item);
                    header->__ID->__item = NULL;
                }
                ENB_FREE( header->__ID);
                header->__ID = NULL;
            }
            ENB_FREE (header);
            header = NULL;
        }

        status = soap_decode(resp_xml, &type, &resp, &header);
        if (header != NULL)
            rheader.__ID = header->__ID;
        else
            rheader.__ID = NULL;
		DEALLOCATE (resp_xml);
        resp_xml = NULL;
      /*SPR 17795 START/END  Code Removed */
        /*spr 17966 fix start*/
        /*SPR 17795 END*/

        if (NULL != resp && type!= SOAP_TYPE_cwmp__ACSFaultCodeType)
        {
            resp_rpc = ipr_cwmp_SendRespToDriver(resp, type ,rheader);
        }

        /* SPR 18295 FIXED START */
        if (NULL == resp || NULL == resp_rpc )
        {

            /*spr 17966 fix end*/


            if(type == SOAP_TYPE_cwmp__ACSFaultCodeType)
            {

                if (NULL != stCallbackFunList.sessionClosedCallback)
                {
                    stCallbackFunList.sessionClosedCallback(session_id);
                    g_session_manager_session_open = 0;
                    ipr_clean_http_session();
                }
                g_session_manager_session_open = 0;
				DEALLOCATE(session_id);
                break;

            }    
            else{
                struct SOAP_ENV__Fault *soap_fault = NULL;
                soap_fault = ipr_cwmp_construct_soap_fault(9000);
                if (NULL != soap_fault )
                {

                    resp_rpc = soap_encode(soap_fault,SOAP_TYPE_SOAP_ENV__Fault,&rheader);
                    acsPendReq = TRUE;

                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "In %s resp NULL",__FUNCTION__);
                    if (soap_fault->detail)
                    {
						DEALLOCATE(soap_fault->detail->__any->FaultString);
						DEALLOCATE(soap_fault->detail->__any->FaultCode);
						DEALLOCATE (soap_fault->detail->__any);
						DEALLOCATE (soap_fault->detail);
					}
					DEALLOCATE(soap_fault->faultstring);
					DEALLOCATE(soap_fault->faultcode);
					DEALLOCATE(soap_fault);

                    continue;
                }        
            }
        }
        /* SPR 18295 FIXED END */

        acsPendReq = TRUE;
    }/* End while(1) */
    if (header)
    {
        if (header->__HoldRequests)
        {
            if (header->__HoldRequests->SOAP_ENV__mustUnderstand )
            {
                ENB_FREE(header->__HoldRequests->SOAP_ENV__mustUnderstand);
                header->__HoldRequests->SOAP_ENV__mustUnderstand = NULL;
            }
            ENB_FREE( header->__HoldRequests);
            header->__HoldRequests = NULL;
        }
        if (header->__ID)
        {
            if (header->__ID->SOAP_ENV__mustUnderstand )
            {
                ENB_FREE(header->__ID->SOAP_ENV__mustUnderstand);
                header->__ID->SOAP_ENV__mustUnderstand = NULL;
            }
            if (header->__ID->__item)
            {
                ENB_FREE(header->__ID->__item);
                header->__ID->__item = NULL;
            }
            ENB_FREE( header->__ID);
            header->__ID = NULL;
        }
        ENB_FREE (header);
        header = NULL;
    }
    // IPSEC SSI code starts
#ifdef HAVE_IPSEC
    if(have_ipsec && have_tunnel)
    {
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer1);
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer2);
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer3);

        if (ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr))
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "del state dst = %s failed",p_config_res->HeNBIPAddr);
        else
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "deleted state  dst = %s",p_config_res->HeNBIPAddr);

        if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr,
                    ipsec_net_prefix, ipsec_net_prefix))
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "del out policy failed for  src = %s and dst = %s",
                    p_config_res->HeNBIPAddr,p_config_res->HeMSIPAddr);
        else
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "deleted out policy for  src = %s and dst = %s",
                    p_config_res->HeNBIPAddr,p_config_res->HeMSIPAddr);
        if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr,
                    ipsec_net_prefix, ipsec_net_prefix))
            TRACE(LOG_CRITICAL, SESSION_MANAGER, "\ndel in policy failed for src = %s and dest = %s",
                    p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
        else
            TRACE(LOG_CRITICAL, SESSION_MANAGER, "\ndeleted in policy for src = %s and dest = %s",
                    p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

        have_tunnel = 0;
    }
	DEALLOCATE(p_config_res);
#endif
    // IPSEC SSI code ends
    /*SPR 22452 FIXED START*/ 
    /* SPR 22314 Start */
    TR069_FREE_SINFORM(informReq);
    DEALLOCATE(session_id);
    /* SPR 22314 End */
    /*SPR 22452 FIXED END*/
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with status %d ",__FUNCTION__, status);
    return status;
}/* End of function SInt32 SessionInit (sDeviceInfo *, char *, UINT4 , struct ParameterValueList *) */





/********************************************************************************
 *
 *       Function Name : get_event_list
 *
 *       Description   : This function is used to DEQUEUE the nodes form event queue 
 *       		 and to create the event list. 
 *
 *       Returns       : Returns event List 
 *       		 If no event in EVENT QUEUE then returns NULL
 *
 *********************************************************************************/

sEventList *ipr_cwmp_get_event_list(YSQUEUE *ieventQ)
{
    SInt32 icount = 0 ;
    /*  coverity 86590 fix start */
    SInt32 i;
    /*  coverity 86590 fix end */
    sEventList *ret_eventList;
    sEventNode *fetch_node= NULL;

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    pthread_mutex_lock(&g_event_hnd_mutex);
    icount = ieventQ->count;
    if (0 == icount)
    {
        pthread_mutex_unlock(&g_event_hnd_mutex);
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL",__FUNCTION__);
        return NULL;
    }

	ret_eventList = (sEventList *) ALLOCATE( sizeof(sEventList));
    if (NULL == ret_eventList)
    {
        pthread_mutex_unlock(&g_event_hnd_mutex);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
        return NULL;
    }

	ret_eventList->__ptrEventStruct = (EventStruct **)ALLOCATE  (icount*sizeof(EventStruct *));
    if (NULL == ret_eventList->__ptrEventStruct)
    {
		DEALLOCATE(ret_eventList);
        pthread_mutex_unlock(&g_event_hnd_mutex);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
        return NULL;
    }

    for ( i = 0 ;(i < icount) && (i < MAX_SIZE_OUT); i++ )
    {
	    fetch_node = (sEventNode*)DEQUEUE (ieventQ);
	    if(NULL != fetch_node)
	    {
		    ret_eventList->__ptrEventStruct[i] = fetch_node->sEvent;
			DEALLOCATE (fetch_node);
	    }
    }

    ret_eventList->__size = icount;
    pthread_mutex_unlock(&g_event_hnd_mutex);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with eventList ",__FUNCTION__);
    return ret_eventList;
} /* End of function sEventList *session_manager_get_event_list (YSQUEUE *ieventQ) */


/********************************************************************************
 *
 *       Function Name : get_event_param_list
 *
 *       Description   : This function is used to DEQUEUE the nodes form event queue
 *                        and to create the event list.
 *
 *       Returns       : Returns event List
 *                        If no event in EVENT QUEUE then returns NULL
 *
 **********************************************************************************/


sEventList *ipr_cwmp_get_event_param_list(YSQUEUE *ieventQ, ParameterValueList *ret_paramList)
{
	SInt32 icount = 0 ;
    /*  coverity 86628 fix start */
	SInt32 i;
    /*  coverity 86628 fix end */
	SInt32 new_size = 0;
	SInt32 param_count = 0;
	sEventList *ret_eventList;
	sEventNode *fetch_node= NULL;

	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	pthread_mutex_lock(&g_event_hnd_mutex);
	icount = ieventQ->count;
	if (0 == icount)
	{
        /*SPR 18317 FIXED START */ 
        g_inform_set_to_value = 0;
        g_periodic_inform_set_to_value = 0;
        /*SPR 18317 FIXED END */ 
		pthread_mutex_unlock(&g_event_hnd_mutex);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL",__FUNCTION__);
		return NULL;
	}

	//ret_eventList = (sEventList *)ALLOCATEEx(sizeof(sEventList),&err);
	
	ret_eventList = (sEventList *) ALLOCATE( sizeof(sEventList));
	if (NULL == ret_eventList)
	{
        /*SPR 18317 FIXED START */ 
        g_inform_set_to_value = 0;
        g_periodic_inform_set_to_value = 0;
        /*SPR 18317 FIXED END */ 

		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		pthread_mutex_unlock(&g_event_hnd_mutex);
		return NULL;
	}


	memset(ret_eventList,'\0',sizeof(sEventList));
	//ret_eventList->__ptrEventStruct = (EventStruct *)ALLOCATEEx(( icount * sizeof(EventStruct *) ),&err);

	ret_eventList->__ptrEventStruct = (EventStruct **)ALLOCATE  (icount*sizeof(EventStruct *));
	if (NULL == ret_eventList->__ptrEventStruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		DEALLOCATE(ret_eventList);
        /*SPR 18317 FIXED START */ 
        g_inform_set_to_value = 0;
        g_periodic_inform_set_to_value = 0;
        /*SPR 18317 FIXED END */ 

		pthread_mutex_unlock(&g_event_hnd_mutex);
		return NULL;
	}

	for ( i = 0 ;(i < icount) && (i < MAX_SIZE_OUT); i++ )
	{
		fetch_node = (sEventNode*)DEQUEUE (ieventQ);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, " Node fetched in fun %s ",__FUNCTION__);

		if(NULL != fetch_node )
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, " Node fetched in fun %s is not null ",__FUNCTION__);
            /* Periodic Inform Changes Start */
            if(NULL != fetch_node->sEvent)
            {    
            /* Periodic Inform Changes End */
			ret_eventList->__ptrEventStruct[i] = fetch_node->sEvent;
			if (fetch_node->sEvent && fetch_node->sEvent->EventCode)
			{
				if ( strcmp( fetch_node->sEvent->EventCode , "6 CONNECTION REQUEST") == 0)
					g_ipr_cwmp_con_req--;

			}
            /* Periodic Inform Changes Start */
            }
            /* Periodic Inform Changes End */

			if (NULL != fetch_node->sParamValList )
			{
			
	                 /* SPR 19441 FIXED START */
				if (fetch_node->sParamValList->__size > 64)
				{
					new_size = new_size + 64;
					fetch_node->sParamValList->__size = 64;

				}

                                else
				new_size = new_size + fetch_node->sParamValList->__size;
				TRACE (LOG_CRITICAL , SESSION_MANAGER, " size of recvd node %d in fun %s",new_size , __FUNCTION__);
				TRACE (LOG_CRITICAL , SESSION_MANAGER, " in fun %s  step 1",__FUNCTION__);
	                  /* SPR 19441 FIXED END */
				if(new_size)
				{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, " in fun %s  step 2",__FUNCTION__);
					ret_paramList->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **) TR_REALLOC(
							ret_paramList->__ptrParameterValueStruct, new_size * sizeof(struct cwmp__ParameterValueStruct *));
					if (NULL == ret_paramList->__ptrParameterValueStruct)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s  qw realloc failed",__FUNCTION__);
						exit(0);
					}
					for (param_count = 0; 
							((fetch_node->sParamValList->__size <= 64) && (param_count < fetch_node->sParamValList->__size) ); 
							param_count++)
					{
                        /*SPR 17563 Fixed Start */ 
						ret_paramList->__ptrParameterValueStruct[ new_size - fetch_node->sParamValList->__size  + param_count] = fetch_node->sParamValList->__ptrParameterValueStruct[param_count];
                        /*SPR 17563 Fixed End */
					}
				}
			}
		}
	}
	ret_paramList->__size = new_size;
	ret_eventList->__size = icount;
	pthread_mutex_unlock(&g_event_hnd_mutex);
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with eventList ",__FUNCTION__);
    /*SPR 18317 FIXED START */ 
    g_inform_set_to_value = 0;
    g_periodic_inform_set_to_value = 0;
	pthread_mutex_unlock(&g_event_hnd_mutex);
    /*SPR 18317 FIXED END */ 
	//code removed
	/* coverity 99420 fixed*/
	/* SPR 19441 FIXED START*/
	/* SPR 19441 FIXED END*/

	/*Memory Leak 18 Jan change start */ 
	DEALLOCATE(fetch_node);
	/*Memory Leak 18 Jan change end */ 
	return ret_eventList;

} /* End of function sEventList *session_manager_get_event_param_list (YSQUEUE *ieventQ) */



/********************************************************************************
 *
 *       Function Name : Inform
 *
 *       Description   : This function is used to create XML stream for 
 *       			Inform structure and sent the XML stream to ACS.
 *       		 	Receive the XML stream and fill the inform Response
 *
 *       Returns       : Returns NULL If response xml is NULL or not valid.
 *       		Returns response  of inform.
 *
 *********************************************************************************/

sInformResponse * ipr_cwmp_Inform(sInform *informReq,	// Inform Structure for which XML will generated
        Uint8 *URL, 		// URL of ACS
        SInt32 *status, 		// Status need to filled for SUCCESS/FAILURE
        sHeader **in_header,	// Header for hold request and ID
        Uint8 *username,
        Uint8 *password,
        Uint8 **session_id)

{
    void *informResp = NULL;
    SInt32 type = 0;
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header = NULL;

    /* SPR 18885 FIXED START */
    struct SOAP_ENV__Header rheader;

    /* SPR 18885 FIXED END */


    /* SPR 16547 Fix Start */
    SInt32 curl_err_code = 0;
    /* COVERITY 108703 FIX START */
    char curl_err_str[FAULT_STRING_SIZE]={0};
    const char *curl_err_string = NULL;
    /* COVERITY 108703 FIX END */


    /* SPR 16547 Fix End */
#ifdef HAVE_IPSEC
    UInt32 isTunnelError = 0;
    UInt8 is_retry = 0;
    config_res_t *p_config_res = NULL;
    FILE* file = NULL;
#endif

    *session_id = (char *)ipr_cwmp_create_sessionId();
    /* SPR 18885 FIXED START */
    rheader.__ID = (struct _cwmp__ID*)ALLOCATE(sizeof(struct _cwmp__ID));
    /*coverity 106593 fixed start */
    if(NULL != rheader.__ID)
    {
        rheader.__ID->__item = (char*)ALLOCATE(5);
        rheader.__ID->SOAP_ENV__mustUnderstand = (char*)ALLOCATE(5);
        if(NULL !=rheader.__ID->__item)
            strcpy(rheader.__ID->__item,"1");
        if(NULL !=rheader.__ID->SOAP_ENV__mustUnderstand)
            strcpy( rheader.__ID->SOAP_ENV__mustUnderstand,"1");
    }
    /*coverity 106593 fixed end */


    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(informReq, SOAP_TYPE__cwmp__Inform,&rheader);

    printf("inform message content :%s\n",req_xml);
    /* SPR 18885 FIXED END */

    //IPSEC SSI code starts
#ifdef HAVE_IPSEC
    if(have_ipsec && !have_tunnel && ipr_cwmp_get_retry_count())
    {
        p_config_res = (config_res_t*)ALLOCATE(sizeof(config_res_t));
        if (NULL == p_config_res)
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
            exit(0);
        }
        is_retry = 1;
        file = fopen(TRO69_IP_SAVE, "r");
        if(file == NULL)
        {
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "Error opening file to restore saved IPs \n");
            DEALLOCATE(p_config_res);
            return;
        }
        else
        {
            fread(p_config_res, sizeof(config_res_t), 1, file);
            fclose(file);
        }
        if(strlen(p_config_res->HeNBIPAddr) != 0 && (strlen(p_config_res->SecGWServer1) != 0 || strlen(p_config_res->SecGWServer2) != 0 || strlen(p_config_res->SecGWServer3) != 0))
        {
            if(strlen(p_config_res->SecGWServer1) != 0)
            {
                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                            (UInt8*)p_config_res->SecGWServer1, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                    isTunnelError = 1;
                }
                else
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer1, 
                            (UInt8*)p_config_res->HeNBIPAddr, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                    isTunnelError = 1;
                }
                else
                {

                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                            p_config_res->HeNBIPAddr, p_config_res->SecGWServer1, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    isTunnelError = 1;
                }
                else
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                            p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer1, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    isTunnelError = 1;
                }
                else if(!isTunnelError)
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    if(!ping(p_config_res->HeMSIPAddr))
                    {
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                    }
                    else
                    {
                        // delete the config and set the tunnel error flag

                        //delete the security assiciations
                        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer1);
                        ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                        //call del policy for HeMS
                        if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                        isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                    }
                }

            }//end of SecGW1
            else
                isTunnelError = 1;//set this to try other 2 GWs if any
            if(isTunnelError && strlen(p_config_res->SecGWServer2) != 0)
            {
                isTunnelError = 0;//reset this flag before trying to establish tunnel with other GW
                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                            (UInt8*)p_config_res->SecGWServer2, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                    isTunnelError = 1;
                }
                else
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer2, 
                            (UInt8*)p_config_res->HeNBIPAddr, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                    isTunnelError = 1;
                }
                else
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                            p_config_res->HeNBIPAddr, p_config_res->SecGWServer2, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add in policy src = %s and  dst = %s failed", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    isTunnelError = 1;
                }
                else
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                            p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer2, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add out policy src = %s and  dst = %s failed", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    isTunnelError = 1;
                }
                else if(!isTunnelError)
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    if(!ping(p_config_res->HeMSIPAddr))
                    {
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                    }
                    else
                    {
                        // delete the config and set the tunnel error flag

                        //delete the security assiciations
                        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer2);
                        ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                        //call del policy for HeMS
                        if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                        isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                    }
                }
            }//end of SecGW2
            if(isTunnelError && strlen(p_config_res->SecGWServer3) != 0)
            {
                isTunnelError = 0;
                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, 
                            (UInt8*)p_config_res->SecGWServer3, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add state src = %s and  dst = %s failed", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                    isTunnelError = 1;
                }
                else
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer3, 
                            (UInt8*)p_config_res->HeNBIPAddr, 
                            (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
                            PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add state src = %s and  dst = %s failed", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                    isTunnelError = 1;
                }
                else
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                    g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                    setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                    //signal(SIGALRM, sig_hdl);
                    if (sigaction(SIGALRM, &g_act, NULL) < 0)
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "sigaction failed");

                }

                if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, 
                            p_config_res->HeNBIPAddr, p_config_res->SecGWServer3, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add in policy src = %s and  dst = %s failed", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    isTunnelError = 1;
                }
                else
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, 
                            p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer3, ipsec_net_prefix, ipsec_net_prefix))
                {
                    TRACE(LOG_CRITICAL , SESSION_MANAGER, "add out policy src = %s and  dst = %s failed", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    isTunnelError = 1;
                }
                else if(!isTunnelError)
                {
                    TRACE(LOG_CRITICAL, SESSION_MANAGER, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                    if(!ping(p_config_res->HeMSIPAddr))
                    {
                        TRACE(LOG_CRITICAL, SESSION_MANAGER, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                    }
                    else
                    {
                        // delete the config and set the tunnel error flag

                        //delete the security assiciations
                        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer3);
                        ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                        //call del policy for HeMS
                        if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                        if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                        else
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    }
                }
            }//end of SecGW3
            if(isTunnelError)

                TRACE (LOG_CRITICAL , SESSION_MANAGER, "skipping sending tunnel error in session mamager.c");
            else
                have_tunnel = 1;
        }
    }
#endif
    /* SPR 18276 FIXED START/END  Code Removed  */

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "resp_xml = %s  ",resp_xml);
    /*SPR #14384 Fixed Start */
    DEALLOCATE (req_xml);
    /*  coverity 86603 fix code removed */
    /*SPR #14384 Fixed End */
    /*SPR 18885 FIXED START */    
    if (NULL != rheader.__ID)
    {
        if (NULL != rheader.__ID->__item)
        {
            DEALLOCATE(rheader.__ID->__item);
        }
        if (NULL != rheader.__ID->SOAP_ENV__mustUnderstand)
        {
            DEALLOCATE(rheader.__ID->SOAP_ENV__mustUnderstand);
        }
        DEALLOCATE(rheader.__ID);
    }

    /*SPR 18885 FIXED END */    
    if ((*status != SUCCESS) || (NULL == resp_xml))
    {
        /* SPR 16547 Fix Start */
        /* SPR 16451 Fix Start */
        if(*status == CRITICAL_ERROR)
        {
            stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
        }
        /*SPR 19124 FIXED START*/
        TRACE(LOG_DETAIL , SESSION_MANAGER,"status is = %d", *status);
        /* code removed*/
        /*SPR 19124 FIXED END*/
        /* SPR 16451 Fix End */
        switch (*status)
        {
            case CURLE_SSL_CACERT :
                {
                    /*SPR 19124 FIXED START*/
                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SSL_CACERT = %d", *status);

                    curl_err_code = CURLE_SSL_CACERT;
                    /* COVERITY 108703/108701 FIX START */
                    curl_err_string = curl_easy_strerror(CURLE_SSL_CACERT);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    /* COVERITY 108703/108701 FIX END */
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    /*SPR 19124 FIXED END */
                    /* SPR 20703 FIXED START */
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount)
                    { 
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    /* SPR 20703 FIXED END */
                    break;

                }
                /* SPR 16580 Fix Start */
            case CURLE_SSL_CACERT_BADFILE :
                {
                    /*SPR 19124 FIXED START*/
                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SSL_CACERT_BADFILE = %d" , *status);

                    curl_err_code = CURLE_SSL_CACERT_BADFILE;
                    /* COVERITY 108703/108701 FIX START */
                    curl_err_string = curl_easy_strerror(CURLE_SSL_CACERT_BADFILE);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    /* COVERITY 108703/108701 FIX END */
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    /* code removed */
                    /*SPR 19124 FIXED END*/
                    /* SPR 20703 FIXED START */
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) { 
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    break;

                }
            case CURLE_HTTP_RETURNED_ERROR :
                {


                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_HTTP_RETURNED_ERROR = %d" , *status);

                    curl_err_code = CURLE_HTTP_RETURNED_ERROR;
                    curl_err_string = curl_easy_strerror(CURLE_HTTP_RETURNED_ERROR);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                    {
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }             
                    break;

                }

            case CURLE_COULDNT_CONNECT :
                {


                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_COULDNT_CONNECT = %d" , *status);

                    curl_err_code = CURLE_COULDNT_CONNECT;
                    curl_err_string = curl_easy_strerror(CURLE_COULDNT_CONNECT);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                    {
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    break;

                }

            case CURLE_OPERATION_TIMEDOUT :
                {


                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_OPERATION_TIMEDOUT = %d" , *status);

                    curl_err_code = CURLE_OPERATION_TIMEDOUT;
                    curl_err_string = curl_easy_strerror(CURLE_OPERATION_TIMEDOUT);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount)  
                    {
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    break;

                }
            case CURLE_GOT_NOTHING :
                {
                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_GOT_NOTHING = %d" , *status);

                    curl_err_code = CURLE_GOT_NOTHING;
                    curl_err_string = curl_easy_strerror(CURLE_GOT_NOTHING);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                    {
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    break;



                }
            case CURLE_SEND_ERROR :
                {
                    TRACE(LOG_DETAIL , SESSION_MANAGER ,"Status Case is CURLE_SEND_ERROR = %d" , *status);

                    curl_err_code = CURLE_SEND_ERROR;
                    curl_err_string = curl_easy_strerror(CURLE_SEND_ERROR);
                    if (ENB_STRLEN(curl_err_string) < FAULT_STRING_SIZE)
                        ENB_STRCPY(curl_err_str , curl_err_string);
                    TRACE(LOG_DETAIL , SESSION_MANAGER ," curl_err_str = %s", curl_err_str);
                    if (ipr_cwmp_get_retry_count() == MaxSessionRetryCount) 
                    {
                        send_curl_err_oam_g = 1;
                        stCallbackFunList.sessionFailedCallback(*session_id,curl_err_code,curl_err_str);
                    }
                    break;



                }

                /* SPR 20703 FIXED END */


            default :
                {
                    *status = FAILED;
                    break;
                }
        }

        /* SPR 16547 Fix End */


        DEALLOCATE (resp_xml);
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL",__FUNCTION__);
        return NULL;
    }
    *status = soap_decode(resp_xml, &type, (void**)&informResp, &header);

    if (header && header->__HoldRequests)
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Hold Request from ACS = %u",header->__HoldRequests->__item);

    *in_header = header;

    DEALLOCATE (resp_xml);
    if (SOAP_TYPE__cwmp__InformResponse == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Inform Response ",__FUNCTION__);
        if (NULL != stCallbackFunList.sessionInitCallback)
        {
            stCallbackFunList.sessionInitCallback(*session_id, URL);
        }
        g_session_manager_session_open = 1;
        *status = SUCCESS;
        return (sInformResponse *)informResp;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        struct _cwmp__Fault * t = NULL;
        t = ((struct _cwmp__Fault *)informResp);
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Fault Response: %s" , __FUNCTION__, t->FaultString);
        /* Coverity 86603 + */
        Uint8 *new_session_id = NULL;
        new_session_id = ALLOCATE(100);
        /* Coverity 86603 - */
        if (NULL == new_session_id)
        {
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
            exit(0);
        }

        ENB_STRNCAT(new_session_id , t->FaultCode , 8);
        ENB_STRNCAT(new_session_id, ":" , 1);
        ENB_STRNCAT(new_session_id , t->FaultString , 64);


        if (NULL != stCallbackFunList.sessionFailedCallback)
        {
            stCallbackFunList.sessionFailedCallback(new_session_id,curl_err_code,curl_err_str);
        }
        g_session_manager_session_open = 0;

        /* SPR 20703 FIXED START */
        //*session_id = NULL;
        /* SPR 20703 FIXED END */
        DEALLOCATE(new_session_id);
        /*spr 17966 fix start*/
        if(!strcmp(t->FaultCode,"8005"))
        {
            *status = FAILED;
        }
        else
        {
            *status = CRITICAL_ERROR;
        }
        /*spr 17966 fix end*/
        /* SPR 21671 FIXED START */
        ENB_FREE(t->FaultCode);
        ENB_FREE(t->FaultString);
        ENB_FREE(t);
        /* SPR 21671 FIXED END */
        return NULL;
    }


    *status = FAILED;
    // IPSEC SSI code starts
#ifdef HAVE_IPSEC
    if(have_ipsec && have_tunnel && is_retry)

    {
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer1);
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer2);
        ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer3);

        if (ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr))
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "del state dst =  %s failed", p_config_res->HeNBIPAddr);
        else
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "deleted state dst =  %s ", p_config_res->HeNBIPAddr);

        if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
        else
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
        if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
        else
            TRACE(LOG_CRITICAL , SESSION_MANAGER, "deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
        have_tunnel = 0;
    }
    DEALLOCATE(p_config_res);
#endif
    // IPSEC SSI code ends
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with FAILED ",__FUNCTION__);
    return NULL;
}/* End of function sInformResponse * Inform(sInform *, char *, SInt32 *, sHeader *) */





/********************************************************************************
 *
 *       Function Name : createInformReq
 *
 *       Description   : Creats the Inform structure.
 *
 *       Returns       : Pointer to Inform Structure 
 *
 *********************************************************************************/

sInform * ipr_cwmp_createInformReq(DeviceInfo *devidstruct,		// sDeviceInfo will contain info of device
        struct ParameterValueList *paramList, 	// parameter list for event
        struct EventList *eventList)		// List of event for which Infor will be created
{
	sInform *informReq = {0};
	sEventList *new_eventList = {0};
	sEventNode *fetch_node= {0};
	sEventStruct *newEvent = {0};
	sEventNode *newNode = {0};
	sEventStruct *newEvent1 = {0};
	sEventNode *newNode1 = {0};
	SInt32 count = 0;
	UINT4 new_size = 0;
	SInt32 param_count = 0;
	UINT4 new_param_size = 0;
	SInt32 event_count = 0;
	struct ParameterValueList *tmp_list = {0};
	ParameterValueStruct **forced_paramArr = {0};
	ParameterValueStruct ** l_forced_paramArr = {0};
	ParameterValueStruct *l_tmp_param = {0};
	struct ParameterValueList *l_forcedParamList = {0};
   /* SPR 13658 START */
    UINT4 temp_val = 0;
   /* SPR 13658 END */

    /* sg change */
    //code removed
    /*sg change */
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	informReq = (sInform *) ALLOCATE (sizeof (sInform) );
	if (NULL == informReq)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE failed",__FUNCTION__);
		return NULL;
	}
	
	memset(informReq,'\0',sizeof (sInform));
	informReq->DeviceId = devidstruct;
	informReq->RetryCount = ipr_cwmp_get_retry_count();
	informReq->MaxEnvelopes = 1;
	informReq->ParameterList = NULL;
	time(&(informReq->CurrentTime));
	/*SPR 17929 change start*/
	initialize_cookie();
   
	/*SPR 17929 change end*/

	newEvent = (sEventStruct *)ALLOCATE(sizeof(sEventStruct) ) ;
	if (NULL == newEvent)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		exit(0);	
	}

	newNode = (sEventNode *)ALLOCATE(sizeof(sEventNode) );
	if (NULL == newNode)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		exit(0);	
	}


	count = informReq->RetryCount;  //Added  for checking Retry count for Queued And Expedited event

#ifdef VENDOR_SPECIFIC
	// Initialized Forced Notification parameters to go in Every Inform

	if (NULL == g_forcedParamList.__ptrParameterValueStruct)
	{

		forced_paramArr = (ParameterValueStruct **)ALLOCATE (( sizeof(ParameterValueStruct *) * NUM_FORCED_PARAM) );

		if (forced_paramArr)
		{
			g_forcedParamList.__ptrParameterValueStruct = forced_paramArr;
		}
		else
		{
			DEALLOCATE(newNode );
			DEALLOCATE(informReq);
			DEALLOCATE(newEvent);	
			return NULL;
		} 
		UINT4 indx = 0;

		while (indx < NUM_FORCED_PARAM)
		{
            /* spr 18338 fix start */
			//l_tmp_param = (ParameterValueStruct *) ALLOCATE (sizeof(ParameterValueStruct) );
			l_tmp_param = (ParameterValueStruct *) ALLOCATE( sizeof(ParameterValueStruct));
            /* spr 18338 fix end */
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Value of Index is:%d",indx);
			if (NULL == l_tmp_param)
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
				exit(0);	
			}

            /* spr 18338 fix start */
			//l_tmp_param->Name = (char *) ALLOCATE(256 );
			l_tmp_param->Name = (char *) ALLOCATE( 256 );
            /* spr 18338 fix end */
			if (NULL == l_tmp_param->Name )
			{
		        /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param);
        		/* Memory Leak 15 jan fixed end */
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
				exit(0);	
			}

   /* SPR 13658 START */
            /* spr 18338 fix start */
			//l_tmp_param->Value =  ALLOCATE(512 );
			l_tmp_param->Value =  ALLOCATE(512);
            /* spr 18338 fix end */
   /* SPR 13658 END */

			if (NULL == l_tmp_param->Value )
			{
		        /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param->Name);
				DEALLOCATE (l_tmp_param);
        		/* Memory Leak 15 jan fixed end */
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
				exit(0);	
			}

			if (indx == OPSTATE_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					strncpy(l_tmp_param->Name , FAPCONTROL_OPSTATE , FAPCONTROL_OPSTATE_LEN);
   /* SPR 13658 START */
					l_tmp_param->__type = SOAP_TYPE_xsd__boolean;
					//strncpy(l_tmp_param->Value ,"0", 2);
                    *((SInt32 *)l_tmp_param->Value) = 0;
   /* SPR 13658 END */
					forced_paramArr[OPSTATE_INDX] = l_tmp_param;
				}
			}
			if (indx == RFTXSTATUS_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					strncpy(l_tmp_param->Name , FAPCONTROL_RFTXSTATUS,  FAPCONTROL_RFTXSTATUS_LEN);
   /* SPR 13658 START */
					l_tmp_param->__type = SOAP_TYPE_xsd__boolean;
					//strncpy(l_tmp_param->Value ,"0", 2);
                    *((SInt32 *)l_tmp_param->Value) = 0;
   /* SPR 13658 END */
					forced_paramArr[RFTXSTATUS_INDX] = l_tmp_param;
				}
			}
			if (indx == LASTSCANTIME_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					if (strlen((const char *)g_Lte_LastScanTime) != 0)
					{

						strncpy(l_tmp_param->Value ,(const char *)g_Lte_LastScanTime , 512);
					}
					else
						strncpy(l_tmp_param->Value ,"0001-01-01T00:00:00Z", 21);
					strncpy(l_tmp_param->Name , LTE_LASTSCANTIME , LTE_LASTSCANTIME_LEN);
					forced_paramArr[LASTSCANTIME_INDX] = l_tmp_param;
				}
			}
/* SPR 16568 FIX START */
            if(g_flag_to_notify_connection_req_url == 1)
            { 
                if (indx == CONNECTION_REQ_URL_INDX)
                {
                    if (l_tmp_param->Name && l_tmp_param->Value)
                    {
                        strncpy(l_tmp_param->Name , CONNECTION_REQ_URL , CONNECTION_REQ_URL_LEN);
                        strncpy(l_tmp_param->Value ,(char *)g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL,  
                                strlen((char *)g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL));

                        forced_paramArr[CONNECTION_REQ_URL_INDX] = l_tmp_param;
                    }
                }
            }
/* SPR 16568 FIX END */
			if (indx == UPDATETIME_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					strncpy(l_tmp_param->Name , LTE_NRT_UPDATETIME , LTE_NRT_UPDATETIME_LEN);
					strncpy(l_tmp_param->Value ,"0001-01-01T00:00:00Z", 21);
					forced_paramArr[UPDATETIME_INDX] = l_tmp_param;
				}
			}

			indx += 1;

		}
/* SPR 16568 FIX START */
        if(g_flag_to_notify_connection_req_url == 1)
        {
            g_forcedParamList.__size = NUM_FORCED_PARAM;
        }
        else
        { 
            g_forcedParamList.__size = NUM_FORCED_PARAM -1;
        }
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Value of size is:%d",g_forcedParamList.__size);
/* SPR 16568 FIX END */
	}

	if (NULL == l_forcedParamList) 
	{

		l_forcedParamList =  ALLOCATE(sizeof(ParameterValueList) );
		if (NULL == l_forcedParamList)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with qvAlloc FAILED",__FUNCTION__);
			exit(0);	
		}

		//l_forced_paramArr = (ParameterValueStruct **)ALLOCATE ( (sizeof(ParameterValueStruct *) * NUM_FORCED_PARAM ) );
		
        /* Memory Leak 15 jan fixed start */
		l_forced_paramArr = (ParameterValueStruct **)ALLOCATE ( NUM_FORCED_PARAM*sizeof(ParameterValueStruct *) ); 
        /* Memory Leak 15 jan fixed end */
		if (NULL == l_forced_paramArr)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
			exit(0);	
		}

		l_forcedParamList->__ptrParameterValueStruct = l_forced_paramArr;
		UINT4 indx = 0;

		while (indx < NUM_FORCED_PARAM)
		{
            /* spr 18338 fix start */
			//l_tmp_param = (ParameterValueStruct *) ALLOCATE ( sizeof(ParameterValueStruct) );
			l_tmp_param = (ParameterValueStruct *) ALLOCATE( sizeof(ParameterValueStruct)) ;
            /* spr 18338 fix end */
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Value of Index in forced is:%d",indx);
			if (NULL == l_tmp_param)
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				exit(0);
			}
            /* spr 18338 fix start */
			//l_tmp_param->Name = (char *) ALLOCATE(256 );
			l_tmp_param->Name = (char *) ALLOCATE(256 );
            /* spr 18338 fix end */
			if (NULL == l_tmp_param->Name)
			{
		        /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param);
        		/* Memory Leak 15 jan fixed end */
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				exit(0);
			}
   /* SPR 13658 START */
            /* spr 18338 fix start */
			//l_tmp_param->Value =  ALLOCATE(512 );
			l_tmp_param->Value =  ALLOCATE(512);
            /* spr 18338 fix end */
   /* SPR 13658 END */
			if (NULL == l_tmp_param->Value)
			{
		        /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param->Name);
				DEALLOCATE (l_tmp_param);
        		/* Memory Leak 15 jan fixed end */
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				exit(0);
			}

			if (indx == OPSTATE_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					strncpy(l_tmp_param->Name , FAPCONTROL_OPSTATE , FAPCONTROL_OPSTATE_LEN);
   /* SPR 13658 START */
					l_tmp_param->__type = SOAP_TYPE_xsd__boolean;
					//strncpy(l_tmp_param->Value , g_forcedParamList.__ptrParameterValueStruct[OPSTATE_INDX]->Value, 512);
                    temp_val = *((SInt32 *)g_forcedParamList.__ptrParameterValueStruct[OPSTATE_INDX]->Value);
                    if(temp_val)
                        *((SInt32 *)l_tmp_param->Value) = 1;
                    else
                        *((SInt32 *)l_tmp_param->Value) = 0;
                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "temp_val = %d, Value : %d ",temp_val,*((SInt32 *)l_tmp_param->Value));
   /* SPR 13658 END */
					l_forced_paramArr[OPSTATE_INDX] = l_tmp_param;
				}
			}
			if (indx == RFTXSTATUS_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					//coverity fix
					tr_strncpy(l_tmp_param->Name , FAPCONTROL_RFTXSTATUS);
   /* SPR 13658 START */
					//strncpy(l_tmp_param->Value ,g_forcedParamList.__ptrParameterValueStruct[RFTXSTATUS_INDX]->Value, 512);
					l_tmp_param->__type = SOAP_TYPE_xsd__boolean;
                    temp_val = *((SInt32 *)g_forcedParamList.__ptrParameterValueStruct[RFTXSTATUS_INDX]->Value);
                    if(temp_val)
                        *((SInt32 *)l_tmp_param->Value) = 1;
                    else
                        *((SInt32 *)l_tmp_param->Value) = 0;
                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "temp_val = %d, Value : %d ",temp_val,*((SInt32 *)l_tmp_param->Value));
   /* SPR 13658 END */
					l_forced_paramArr[RFTXSTATUS_INDX] = l_tmp_param;
				}
			}
			if (indx == LASTSCANTIME_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{

					strncpy(l_tmp_param->Value ,g_forcedParamList.__ptrParameterValueStruct[LASTSCANTIME_INDX]->Value, 512);
					strncpy(l_tmp_param->Name , LTE_LASTSCANTIME , LTE_LASTSCANTIME_LEN);
   /* SPR 13658 START */
					l_tmp_param->__type = SOAP_TYPE_string;
   /* SPR 13658 END */
					l_forced_paramArr[LASTSCANTIME_INDX] = l_tmp_param;
				}
			}
			if (indx == UPDATETIME_INDX)
			{
				if (l_tmp_param->Name && l_tmp_param->Value)
				{
					strncpy(l_tmp_param->Name , LTE_NRT_UPDATETIME , LTE_NRT_UPDATETIME_LEN);
					strncpy(l_tmp_param->Value , g_forcedParamList.__ptrParameterValueStruct[UPDATETIME_INDX]->Value, 512);
   /* SPR 13658 START */
					l_tmp_param->__type = SOAP_TYPE_string;
   /* SPR 13658 END */
					l_forced_paramArr[UPDATETIME_INDX] = l_tmp_param;
				}
			}
/* SPR 16568 FIX START */
            if(g_flag_to_notify_connection_req_url == 1)
            {
                if (indx == CONNECTION_REQ_URL_INDX)
                {
                    if (l_tmp_param->Name && l_tmp_param->Value)
                    {
                        strncpy(l_tmp_param->Name , CONNECTION_REQ_URL , CONNECTION_REQ_URL_LEN);
                        strncpy(l_tmp_param->Value , g_forcedParamList.__ptrParameterValueStruct[CONNECTION_REQ_URL_INDX]->Value, 512);
                        l_tmp_param->__type = SOAP_TYPE_string;
                        l_forced_paramArr[CONNECTION_REQ_URL_INDX] = l_tmp_param;
                    }
                }
            }
/* SPR 16568 FIX END */
			indx += 1;
		}
/* SPR 16568 FIX START */
        if(g_flag_to_notify_connection_req_url == 1)
        {
            l_forcedParamList->__size = NUM_FORCED_PARAM;
            g_flag_to_notify_connection_req_url = 0;
        }
        else
        {
            l_forcedParamList->__size = NUM_FORCED_PARAM -1;

        }
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Value of size in forced is:%d",l_forcedParamList->__size);
/* SPR 16568 FIX END */

	}
#endif 

	if (newEvent != NULL && g_queued_paramList != NULL && count == 0)
	{
		newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE );

		if (NULL != newEvent->EventCode)
		{
			ENB_STRNCPY(newEvent->EventCode ,"4 VALUE CHANGE",EVENT_CODE_SIZE);
			newEvent->CommandKey = ALLOCATE (COMMAND_KEY_SIZE );
			if (newEvent->CommandKey)
			{
				ENB_STRNCPY(newEvent->CommandKey, "", 2);
			}
			newNode->sEvent = newEvent;
			newNode->sParamValList = g_queued_paramList;
            /*SPR 18033 FIXED START */  
			/* SPR 22314 Start */
            g_queued_paramList = NULL;
            g_queued_paramList_size = 0;
			/* SPR 22314 End */
			g_queued_wait_inform = 0;
            
            /*SPR 18033 FIXED END */  
			PUSH(&g_event_hnd_eventQueue, (YSNODE *)newNode);
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "In %s : ,Pushed new Node into queue g_event_hnd_eventQueue with evntcode %s ",__FUNCTION__,newNode->sEvent->EventCode);

		}
	}
	/* SPR 21563 FIXED START*/
	else
	{
		TRACE (LOG_INFO , SESSION_MANAGER, "Freeing enent node");
		DEALLOCATE(newEvent);
		DEALLOCATE(newNode);
	}
	/* SPR 21563 FIXED END*/
	if (g_expedited_inform && count > 2)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Session Failed  After 3 retries ");
		if (NULL != stCallbackFunList.sessionFailedCallback)
                        /* SPR 16547 Fix Start */
			stCallbackFunList.sessionFailedCallback(NULL,0,NULL);
                        /* SPR 16547 Fix End */
	}

#ifdef VENDOR_SPECIFIC
	if (shems_Session && count > 2)
	{

		newNode1 = (sEventNode *)ALLOCATE(sizeof(sEventNode) );
		if (NULL == newNode1)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
			exit(0);	
		}

		newEvent1 = (sEventStruct *)ALLOCATE(sizeof(sEventStruct) );
		if (NULL == newEvent1)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
			exit(0);	
		}

		newEvent1->EventCode = ALLOCATE(EVENT_CODE_SIZE );
		if (NULL != newEvent1->EventCode)
		{
			ENB_STRNCPY(newEvent1->EventCode ,"0 BOOTSTRAP",EVENT_CODE_SIZE);
			newEvent1->CommandKey = ALLOCATE (COMMAND_KEY_SIZE );
			if (newEvent1->CommandKey)
			{
				ENB_STRNCPY(newEvent1->CommandKey, "", 2);
			}
			newNode1->sEvent = newEvent1;
			newNode1->sParamValList = NULL;
			PUSH(&g_event_hnd_eventQueue, (YSNODE *)newNode1);

		}

	}
#endif 

	count = 0;
	tmp_list = (struct ParameterValueList *) ALLOCATE (sizeof(struct ParameterValueList) );
	if (NULL == tmp_list)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
        /* Memory Leak 15 jan fixed start */
		DEALLOCATE (l_tmp_param->Name);  
		DEALLOCATE (l_tmp_param->Value);  
		DEALLOCATE (l_tmp_param);  
        /* Memory Leak 15 jan fixed end */
		DEALLOCATE(informReq);   
		DEALLOCATE(l_forced_paramArr);   
		DEALLOCATE(l_forcedParamList);
		return NULL;
	}
	tmp_list->__ptrParameterValueStruct = NULL;
	
	if (NULL == eventList)
	{
		informReq->Event =ipr_cwmp_get_event_param_list(&g_event_hnd_eventQueue, tmp_list);
		if (0 != tmp_list->__size)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "in %s size not zero size:%d ",__FUNCTION__, tmp_list->__size);
			//  Adding forced  parameters
            /*SPR 17794 start*/
			tmp_list->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **) TR_REALLOC (
					tmp_list->__ptrParameterValueStruct,
				sizeof(struct cwmp__ParameterValueStruct *) * (tmp_list->__size + l_forcedParamList->__size));
			if (NULL == tmp_list->__ptrParameterValueStruct)
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s  qww TR_REALLOC failed",__FUNCTION__);
				exit(0);
			}
			UINT4 force_offset = 0;
			SInt32 indx = 0;
			force_offset = tmp_list->__size;
			for (; indx < l_forcedParamList->__size ; indx++)
			{
				tmp_list->__ptrParameterValueStruct[force_offset] = l_forcedParamList->__ptrParameterValueStruct[indx]; 
				force_offset++; 
			}
            /*SPR 17794 END*/
        /* Memory Leak 15 jan fixed start */
			DEALLOCATE (l_forcedParamList->__ptrParameterValueStruct); 
			DEALLOCATE(l_forcedParamList); 
        /* Memory Leak 15 jan fixed end */

			tmp_list->__size = force_offset;
			informReq->ParameterList = tmp_list;
		}
		else 
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "in %s  temp size 0 ",__FUNCTION__);
			DEALLOCATE (tmp_list);
			tmp_list = l_forcedParamList;
			informReq->ParameterList = tmp_list;
		}
	}
	else
	{
		if (0 == g_event_hnd_eventQueue.count)
		{
			new_eventList = (sEventList *) ALLOCATE (sizeof(sEventList) );
			if (NULL == new_eventList)
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				exit(0);
			}
			new_eventList->__ptrEventStruct = (sEventStruct**)ALLOCATE ( (eventList->__size * sizeof(sEventStruct *)) );
			if (NULL ==  new_eventList->__ptrEventStruct)
            {
				DEALLOCATE(new_eventList);
				DEALLOCATE (tmp_list);
				DEALLOCATE(informReq) ;
                /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param->Name);  
				DEALLOCATE (l_tmp_param->Value);  
				DEALLOCATE (l_tmp_param);             
                /* Memory Leak 15 jan fixed end */
				DEALLOCATE(l_forced_paramArr);   
				DEALLOCATE(l_forcedParamList);            
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                return NULL;
            }

			for ( count = 0; count < eventList->__size; count++)
			{
				new_eventList->__ptrEventStruct[count] = eventList->__ptrEventStruct[count];
			}

			new_eventList->__size = eventList->__size;
			informReq->Event = new_eventList;
			if (NULL != paramList)
			{
				/* SPR 16327 Fixed Start*/
				tmp_list->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **)ALLOCATE((
							paramList->__size * sizeof (struct cwmp__ParameterValueStruct *)));
				/* SPR 16327 Fixed End*/

				if (NULL ==  tmp_list->__ptrParameterValueStruct )
                {
					DEALLOCATE(tmp_list);
					DEALLOCATE(informReq);
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                    /* Memory Leak 15 jan fixed start */
					DEALLOCATE (l_tmp_param->Name);  
					DEALLOCATE (l_tmp_param->Value);  
					DEALLOCATE (l_tmp_param);            
                    /* Memory Leak 15 jan fixed end */
					DEALLOCATE(l_forced_paramArr);   
					DEALLOCATE(l_forcedParamList);            
					DEALLOCATE(new_eventList);            
                    return NULL;
                }

				for ( count = 0; count < paramList->__size; count++)
				{
					tmp_list->__ptrParameterValueStruct[count] = paramList->__ptrParameterValueStruct[count];
				}
				tmp_list->__size = paramList->__size;
				informReq->ParameterList = tmp_list;
			}
			else
			{
				DEALLOCATE (tmp_list);
				informReq->ParameterList = NULL;
			}
		}
		else
		{
			event_count = g_event_hnd_eventQueue.count ;
			new_size = eventList->__size + g_event_hnd_eventQueue.count ;

			new_eventList = (sEventList *) ALLOCATE (sizeof(sEventList) );
			if (NULL ==  new_eventList)
            {
				DEALLOCATE (tmp_list);
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param->Name);  
				DEALLOCATE (l_tmp_param->Value);  
				DEALLOCATE (l_tmp_param);             
                /* Memory Leak 15 jan fixed end */
				DEALLOCATE(informReq);            
				DEALLOCATE(l_forced_paramArr);   
				DEALLOCATE(l_forcedParamList);            
                return NULL;
            }

			new_eventList->__ptrEventStruct = (sEventStruct **)ALLOCATE ((new_size * sizeof(sEventStruct *)) );
			if (NULL ==  new_eventList->__ptrEventStruct)
            {
				DEALLOCATE(new_eventList);
				DEALLOCATE (tmp_list);
				DEALLOCATE(informReq);
            /* coverity 86659 fix start */
            /*code remove */
            /* coverity 86659 fix end */
            TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                /* Memory Leak 15 jan fixed start */
				DEALLOCATE (l_tmp_param->Name);  
				DEALLOCATE (l_tmp_param->Value);  
				DEALLOCATE (l_tmp_param);            
                /* Memory Leak 15 jan fixed end */
				DEALLOCATE(l_forced_paramArr);   
				DEALLOCATE(l_forcedParamList);            
                return NULL;
            }

			for ( count = 0; count < eventList->__size; count++)
			{
				new_eventList->__ptrEventStruct[count] = eventList->__ptrEventStruct[count];
			}
			if (NULL != paramList)
			{
				/* SPR 16327 Fixed Start */
				tmp_list->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **) ALLOCATE (
						(paramList->__size * sizeof (struct cwmp__ParameterValueStruct *)));
				/* SPR 16327 Fixed End */

				if (NULL ==  tmp_list->__ptrParameterValueStruct )
                {
					DEALLOCATE(tmp_list);
					DEALLOCATE(informReq);
					DEALLOCATE(new_eventList->__ptrEventStruct);
					DEALLOCATE(new_eventList);
                    /*  coverity 86659 fix start */
                    /* code removed */
                    /*  coverity 86659 fix end */
                    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s qvAlloc failed",__FUNCTION__);
                    /* Memory Leak 15 jan fixed start */
					DEALLOCATE (l_tmp_param->Name);  
					DEALLOCATE (l_tmp_param->Value);  
					DEALLOCATE (l_tmp_param);            
                    /* Memory Leak 15 jan fixed end */
					DEALLOCATE(l_forced_paramArr);            
					DEALLOCATE(l_forcedParamList);		    
                    return NULL;
                }

				for ( count = 0; count < paramList->__size; count++)
				{
					tmp_list->__ptrParameterValueStruct[count] = paramList->__ptrParameterValueStruct[count];
				}
				new_param_size = paramList->__size;
			}
            /* Coverity 99011 start*/ 
            /*SPR 18317 FIXED START */
            pthread_mutex_lock(&g_event_hnd_mutex);
            /*SPR 18317 FIXED END */
            /* Coverity 99011 end*/ 

			for ( count = 0; count < event_count; count++)
			{
				fetch_node = (sEventNode*)DEQUEUE (&g_event_hnd_eventQueue);
				if (NULL == fetch_node)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s DEQUEUE failed",__FUNCTION__);
                    /*SPR 18317 FIXED START */
                    g_inform_set_to_value = 0;
                    g_periodic_inform_set_to_value = 0;

                    pthread_mutex_unlock(&g_event_hnd_mutex);

                    /*SPR 18317 FIXED END */

					exit(0);
				}

				if (fetch_node->sEvent && fetch_node->sEvent->EventCode )
				{		
					if ( strcmp( fetch_node->sEvent->EventCode , "6 CONNECTION REQUEST") == 0)
						g_ipr_cwmp_con_req--;

				}	

				new_eventList->__ptrEventStruct[count + eventList->__size] = fetch_node->sEvent;
				if (NULL != fetch_node->sParamValList )
				{
					/* SPR 19441 FIXED START */
					if (fetch_node->sParamValList->__size > 64)
					{
						new_param_size = new_param_size + 64;
						fetch_node->sParamValList->__size = 64;

					}
					else		

					/* SPR 19441 FIXED START */
					new_param_size = new_param_size + fetch_node->sParamValList->__size;
					tmp_list->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **) TR_REALLOC ( tmp_list->__ptrParameterValueStruct, new_param_size * sizeof(struct cwmp__ParameterValueStruct *));
					if (NULL == tmp_list->__ptrParameterValueStruct)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s iiTR_REALLOC failed",__FUNCTION__);
                        /*SPR 18317 FIXED START */
                        g_inform_set_to_value = 0;
                        g_periodic_inform_set_to_value = 0;

                        pthread_mutex_unlock(&g_event_hnd_mutex);
                        /*SPR 18317 FIXED END*/

						exit(0);
					}
					for (param_count = 0; 
							((fetch_node->sParamValList->__size <=64) && 
							 (param_count < fetch_node->sParamValList->__size));
							param_count++)
					{
                        /*SPR 17563 Fixed Start */
						tmp_list->__ptrParameterValueStruct[ new_param_size - fetch_node->sParamValList->__size + param_count] = fetch_node->sParamValList->__ptrParameterValueStruct [ param_count ];
                        /*SPR 17563 Fixed End */
					}
				}

				//	DEALLOCATE (fetch_node);
			}

			tmp_list->__size = new_param_size;
			if (0 == new_param_size)
			{
				DEALLOCATE(tmp_list);
			}
			informReq->ParameterList = tmp_list;
			new_eventList->__size = new_size;
			informReq->Event = new_eventList;
		}
	}
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting  %s with Inform Request ",__FUNCTION__);
    /*SPR 18317 FIXED START */
    g_inform_set_to_value = 0;
    g_periodic_inform_set_to_value = 0;

    pthread_mutex_unlock(&g_event_hnd_mutex);
    /*SPR 18317 FIXED END */
  
	return informReq;
}/* End of function sInform * createInformReq (sDeviceInfo *, struct ParameterValueList *, struct EventList *) */





/********************************************************************************
 *
 *       Function Name : TransferComplete
 *
 *       Description   : This function is used to create xml for Transfer Complete 
 *       		response and send the xml to ACS
 *
 *       Returns       :  Transfer Complete Response if response comes
 *       		 NULL if response is not OK
 *
 *********************************************************************************/

sTransferCompleteResponse * ipr_cwmp_TransferComplete(sTransferComplete *transCompReq, 	//Structure of Transfer complete Request
        Uint8 *URL,	//URL of ACS
        SInt32 *status,	// Status need to filled for SUCCESS/FAILURE
        Uint8 *username,
        Uint8 *password)	
{
    /*Coverity 111408 Fix Start*/
    void *transCompResp = NULL;
    /*Coverity 111408 Fix End*/
    /*Coverity 111405 Fix Start*/
    SInt32 type = 0;
    /*Coverity 111405 Fix End*/
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header;

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(transCompReq, SOAP_TYPE__cwmp__TransferComplete,NULL);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "req_xml = %s  ",req_xml);

    //if(transCompReq->CommandKey)
	//  DEALLOCATE (transCompReq->CommandKey);

/*    if (NULL != transCompReq->FaultStruct)
    {
	      DEALLOCATE(transCompReq->FaultStruct->FaultCode);
	      DEALLOCATE(transCompReq->FaultStruct->FaultString);
	      DEALLOCATE(transCompReq->FaultStruct);
    }
*/
    if (NULL == req_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL req_xml",__FUNCTION__);
        return NULL;
    }

	//DEALLOCATE (transCompReq);
    //transCompReq = NULL;

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
    DEALLOCATE(req_xml);
    /*  coverity 86632 fix start */
    /* code removed */
    /*  coverity 86632 fix end */
    if (*status != SUCCESS)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return NULL;
    }
    if (NULL == resp_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s resp_xml is NULL",__FUNCTION__);
        return NULL;
    }

    *status = soap_decode(resp_xml, &type, &transCompResp, &header);
	DEALLOCATE (resp_xml);
    resp_xml = NULL;

    if (SOAP_TYPE__cwmp__TransferCompleteResponse == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return (sTransferCompleteResponse *)transCompResp;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Response %s"
                ,__FUNCTION__,err_to_string(*((unsigned int *)transCompResp)));
        *status = SUCCESS;
        return NULL;
    }

    *status = FAILED;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
    return NULL;
}/* End of function TransferComplete * TransferComplete(sTransferComplete *, char *, SInt32 *) */


/********************************************************************************
 *
 *       Function Name : ipr_cwmp_RequestDownload
 *
 *       Description   : This function is used to create  download request 
 *
 *       Returns       :  List of RPC Methods
 *                        NULL if response is not OK
 *
 *
 *********************************************************************************/

struct _cwmp__RequestDownloadResponse* ipr_cwmp_RequestDownload(struct _cwmp__RequestDownload *reqdwReq,   //Structure of Transfer complete Request
        Uint8 *URL,        //URL of ACS
        SInt32 *status,      // Status need to filled for SUCCESS/FAILURE
        Uint8 *username,
        Uint8 *password)
{   /*Coverity 111407 Fix Start*/
    void *reqdwResp = NULL;
   /*Coverity 111407 Fix End*/
   /*Coverity 111411 Fix Start */
    SInt32 type = 0;
   /*Coverity 111411 Fix End */
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header;

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(reqdwReq, SOAP_TYPE__cwmp__RequestDownload,NULL);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "req_xml = %s  ",req_xml);

    ipr_cwmp_free_FileTypeArg(reqdwReq->FileTypeArg);
	DEALLOCATE(reqdwReq->FileType);
	DEALLOCATE (reqdwReq);
	/*  coverity 86614 fix code removed */

    if (NULL == req_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL req_xml",__FUNCTION__);
        return NULL;
    }

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
	DEALLOCATE (req_xml);
   /* coverity 86614 fix start */
   /*code remove */
   /* coverity 86614 fix end */


    if (*status != SUCCESS)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return NULL;
    }
    if (NULL == resp_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s resp_xml is NULL",__FUNCTION__);
        return NULL;
    }

    *status = soap_decode(resp_xml, &type, &reqdwResp, &header);
	DEALLOCATE (resp_xml);
    resp_xml = NULL;

    if (SOAP_TYPE__cwmp__RequestDownloadResponse == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return (struct _cwmp__RequestDownloadResponse*)reqdwResp;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Response %s"
                ,__FUNCTION__,err_to_string(*((unsigned int *)reqdwResp)));
        *status = SUCCESS;
        return NULL;
    }

    *status = FAILED;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
    return NULL;
}/* End of function  RequestDownload */




/********************************************************************************
 *
 *       Function Name : ipr_cwmp_GetRPCMethods_from_acs
 *
 *       Description   : This function is used to create List of RPC which CPE can call on ACS 
 *
 *       Returns       :  List of RPC Methods
 *                        NULL if response is not OK
 *
 *
 *********************************************************************************/


struct _cwmp__GetRPCMethodsResponse *ipr_cwmp_GetRPCMethods_from_acs(
        Uint8 *URL,        //URL of ACS
        SInt32 *status,      // Status need to filled for SUCCESS/FAILURE
        Uint8 *username,
        Uint8 *password)

{
    void *response = NULL;
    SInt32 type = 0;
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header;
    struct _cwmp__GetRPCMethods * getRPCReq = NULL;
	getRPCReq = (struct _cwmp__GetRPCMethods *) ALLOCATE( sizeof(struct _cwmp__GetRPCMethods));
    if (NULL == getRPCReq)
    {
	    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL req_xml",__FUNCTION__);
	    return NULL;
    }

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(getRPCReq, SOAP_TYPE__cwmp__GetRPCMethods, NULL);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "req_xml = %s  ",req_xml);

	DEALLOCATE (getRPCReq);
   /* coverity 86670 fix start */
   /*code remove */
   /* coverity 86670 fix end */
 
    if (NULL == req_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL req_xml",__FUNCTION__);
        return NULL;
    }

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
	DEALLOCATE (req_xml);
    /* coverity 86670 fix start */
   /*code remove */
   /* coverity 86670 fix end */
    
    if (*status != SUCCESS)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return NULL;
    }
    if (NULL == resp_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s resp_xml is NULL",__FUNCTION__);
        return NULL;
    }

    *status = soap_decode(resp_xml, &type, &response, &header);
	DEALLOCATE (resp_xml);
    /* coverity 86670 fix start */
   /*code remove */
   /* coverity 86670 fix end */
 

    if (SOAP_TYPE__cwmp__GetRPCMethodsResponse == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return (struct _cwmp__GetRPCMethodsResponse *)response;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Response %s"
                ,__FUNCTION__,err_to_string(*((unsigned int *)response)));
        *status = SUCCESS;
        return NULL;
    }

    *status = FAILED;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
    return NULL;
}/* End of function GetRPCMethods_from_acs TransferComplete(sTransferComplete *, char *, int *) */


/********************************************************************************
 *
 *       Function Name : ipr_cwmp_AutoTransCompReq
 *
 *       Description   : This function is used to create xml for Auto Transfer 
 *       		Complete response and send the xml to ACS
 *
 *       Returns       :  Auto Transfer Complete Response if response comes
 *                        NULL if response is not OK
 *
 *
 *********************************************************************************/

sAutoTransCompResp * ipr_cwmp_AutoTransCompReq(sAutoTranComplete *autoTranComeReq,	//Structure of Auto Transfer complete Request
        Uint8 *URL, 	//URL of ACS
        SInt32 *status,
        Uint8 *username, 
        Uint8 *password)	// Status need to filled for SUCCESS/FAILURE
{
    /*coverity 111409 Fix Start*/
    void *autoTranComeResp = NULL;
    /*coverity 111409 Fix End*/
   /*Coverity 111406 Fix Start*/
    SInt32 type = 0;
   /*Coverity 111406 Fix End*/
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header;

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(autoTranComeReq, SOAP_TYPE__cwmp__AutonomousTransferComplete,NULL);

	DEALLOCATE (autoTranComeReq->AnnounceURL);
	DEALLOCATE (autoTranComeReq->TransferURL);
	DEALLOCATE (autoTranComeReq->FileType);
	DEALLOCATE (autoTranComeReq->TargetFileName);

    if (autoTranComeReq->FaultStruct)
    {
		DEALLOCATE (autoTranComeReq->FaultStruct->FaultCode);
		DEALLOCATE (autoTranComeReq->FaultStruct->FaultString);
    }

	DEALLOCATE (autoTranComeReq);

    /*  coverity 86495 fix start */
    /* code removed */
    /*  coverity 86495 fix end */

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
	DEALLOCATE (req_xml);
    if (*status != SUCCESS)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return NULL;
    }
    if (NULL == resp_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s resp_xml is NULL",__FUNCTION__);
        return NULL;
    }
    *status = soap_decode(resp_xml, &type, &autoTranComeResp, &header);
    DEALLOCATE (resp_xml);
    resp_xml = NULL;
    if (SOAP_TYPE__cwmp__AutonomousTransferCompleteResponse == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return (sAutoTransCompResp *)autoTranComeResp;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Response %s"
                ,__FUNCTION__,err_to_string(*((unsigned int *)autoTranComeResp)));
        *status = SUCCESS;
        return NULL;
    }


    *status = FAILED;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
    return NULL;
}/* End of function sAutoTransCompResp * AutoTransCompReq(sAutoTranComplete *, char *, int *) */





/********************************************************************************
 *
 *       Function Name : ipr_cwmp_SendRespToDriver
 *
 *       Description   : Sende the ACS request to RPC Handler and recevies thr
 *       		 response.
 *
 *       Returns       : Returns the xml of response which comes from RPC Handler
 *
 *********************************************************************************/

Uint8 *ipr_cwmp_SendRespToDriver(void *resp,	// Response which will come from ACS to request to CPE for RPC
		SInt32 type,sHeader header)	// Type of Response
{
	Uint8 *req_xml = NULL;
    /*defunct 29/9 fix Start */
	void *fault;
        /*defunct 29/9 fix End */
	/*GPA Change Start*/
    SInt32 index = 0;
	/*GPA Change End*/
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	switch(type)
	{
		case SOAP_TYPE__cwmp__SetParameterValues :
			{
				StStatus  *status = {0};
				struct _cwmp__SetParameterValuesResponse *response = NULL ;
				sSetParameterValues *paramVal = (sSetParameterValues *)resp;
				status = ipr_cwmp_setParameterValues(paramVal->ParameterList);
				if(NULL != 	status)
				{
                    /* SPR 22314 Start */
					TR069_FREE_SSETPARAMETERVALUES(paramVal);
                    /* SPR 22314 End */
					if (status->eStatus == RPC_FAULT)
					{
						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s calloc failed",__FUNCTION__);
							exit(0);
						}
						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->detail = detail;
                        /*defunct 29/9 fix Start */
						fault = (void *)(status->stFault);
						//req_xml = soap_encode(fault, SOAP_TYPE__cwmp__Fault);

						/*SPR 16554 Fix start*/
                         detail->__any = fault;    
                        /*defunct 29/9 fix End */ 
						/*SPR 392 FIXED START */
						ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
						ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						/*SPR 392 FIXED  END*/
 
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						/*SPR 16554 Fix End*/

/* SPR 16448 Fix Start */
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "freeing the allocated soap fault");
						freeSoapFaultParameters(soap_fault);
/* SPR 16448 Fix End */

						/* SPR 18746 FIX START */
						DEALLOCATE (status);
						/* SPR 18746 FIX END */

					}/* End of if (status->eStatus == RPC_FAULT) */
					else
					{
						response = (struct _cwmp__SetParameterValuesResponse *) ALLOCATE(sizeof(struct _cwmp__SetParameterValuesResponse) );
						if (NULL == response)
						{
							ipr_cwmp_free_stat_status(status);
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}

						if(RPC_PARTIAL ==  status->eStatus)
							response->Status = _cwmp__SetParameterValuesResponse_Status__1 ;
						else if((RPC_SUCCESS == status->eStatus))
							response->Status = _cwmp__SetParameterValuesResponse_Status__0 ;

						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__SetParameterValuesResponse,&header);
						DEALLOCATE(response);
						DEALLOCATE(status);
                        /* SPR_19838:Coverity_12932 Fix Stop */

					}/* End of else of if (status->eStatus == RPC_FAULT) */
                   /*SPR 20007:CID 96677 Fix Start */
                   /*Code Delete*/
                   /*SPR 20007:CID 96677 Fix Start */
					//ipr_cwmp_free_stat_status(status);
				}

				break;
			} /* End of case SOAP_TYPE__cwmp__SetParameterValues : */

		case SOAP_TYPE__cwmp__GetParameterValues :
			{
				GetStatus  *status = {0};
				struct _cwmp__Fault *stFault = {0};
				struct _cwmp__GetParameterValuesResponse *response = {0};
				response = (struct _cwmp__GetParameterValuesResponse *) ALLOCATE( sizeof(struct _cwmp__GetParameterValuesResponse));
				if (NULL == response)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					return NULL;
				}

				sGetParameterValues *paramVal = (sGetParameterValues *)resp;
				status = ipr_cwmp_getParameterValues(paramVal->ParameterNames);
				if (NULL == status)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					DEALLOCATE (response);
					return NULL;
				}
				ipr_cwmp_free_get_param(paramVal->ParameterNames);
				ENB_FREE(paramVal);

				if (RPC_FAULT  == status->eStatus)
				{
					stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
					if (NULL == stFault)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					stFault->FaultCode = ALLOCATE( 8);
					if (NULL == stFault->FaultCode )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					stFault->FaultString = ALLOCATE( 1000);
					if (NULL == stFault->FaultString)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					if (status->fault_code == 9001 )
					{
						ENB_STRNCPY(stFault->FaultCode, "9001", 5);
						ENB_STRNCPY(stFault->FaultString, "Request denied (no reason specified)", 999);
					}
					else if (status->fault_code == 9002 )
					{
						ENB_STRNCPY(stFault->FaultCode, "9002", 5);
						ENB_STRNCPY(stFault->FaultString, "Internal error", 999);
					}
					else if (status->fault_code == 9003 )
					{
						ENB_STRNCPY(stFault->FaultCode, "9003", 5);
						ENB_STRNCPY(stFault->FaultString, "Invalid arguments", 999);
					}
					else if (status->fault_code == 9004 )
					{
						ENB_STRNCPY(stFault->FaultCode, "9004", 5);
						ENB_STRNCPY(stFault->FaultString, "Resources exceeded", 999);
					}
					else if (status->fault_code == 9005 )
					{
						ENB_STRNCPY(stFault->FaultCode, "9005", 5);
						ENB_STRNCPY(stFault->FaultString, "Invalid Parameter name", 999);
					}
					//req_xml = soap_encode(stFault, SOAP_TYPE__cwmp__Fault);
					struct SOAP_ENV__Fault *soap_fault = {0};
					struct SOAP_ENV__Detail *detail =  NULL;
					detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
					if (NULL == detail )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
					if (NULL == soap_fault )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					soap_fault->faultcode = ALLOCATE( 8);
					if (NULL == soap_fault->faultcode )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					soap_fault->faultstring = ALLOCATE( 100);
					if (NULL == soap_fault->faultstring  )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					soap_fault->detail = detail;


					detail->__any = stFault;
					ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
					ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
					req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

					DEALLOCATE(soap_fault->faultstring);
					DEALLOCATE(detail);
					DEALLOCATE(soap_fault->faultcode);
					DEALLOCATE(soap_fault);
					DEALLOCATE (stFault->FaultString);
					DEALLOCATE (stFault->FaultCode);
					DEALLOCATE(stFault);
					DEALLOCATE (response);
					//DEALLOCATE (status);
                    /* MEMORY LEAK 31 DEC FIXED START */
                    /* SPR 22314 Start */
					free_gpv_resp_buffer(status->parameterList); 
                    /* SPR 22314 End */
					DEALLOCATE(status);//V9D
                    /* MEMORY LEAK 31 DEC FIXED END */
				}
				else
				{
					response->ParameterList = status->parameterList;

					req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__GetParameterValuesResponse,&header);
/* Spr 17463 Fix Start */
					/* SPR 21563_p1_FIX */
					free_gpv_resp_buffer(status->parameterList); 
					/* SPR 21563_p1_FIX */
					DEALLOCATE(status);
               /* coverity 86670 fix start */
               /*code remove */
               /* coverity 86670 fix end */

              /* Spr 17463 Fix End */
					//ipr_cwmp_free_paramlist(response->ParameterList);
					//DEALLOCATE(response->ParameterList);
					DEALLOCATE (response);
					//DEALLOCATE (status);
					//status = NULL;
				}
				break;
			}/* End of case SOAP_TYPE__cwmp__GetParameterValues : */

        /* SPR 17596 fixed start*/
		case SOAP_TYPE__cwmp__GetRPCMethods :
			{
                GetRPC_t *status = {0};
                int count=0;
                struct _cwmp__Fault *stFault = {0};
				struct _cwmp__GetRPCMethodsResponse *response = {0};
				response = (struct _cwmp__GetRPCMethodsResponse *) ALLOCATE( sizeof(struct _cwmp__GetRPCMethodsResponse));
				if (NULL == response)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					return NULL;
				}
				/*response->MethodList = (struct MethodList *) ALLOCATE( sizeof(struct MethodList));
                if(NULL == response->MethodList)
                {   
				  TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                    return NULL;

                }*/	
		     	status =  ipr_cwmp_GetRPCMethods_rpc();
                if(status == NULL){

					status  = (GetRPC_t *) ALLOCATE( sizeof(GetRPC_t ));
                    if (NULL == status)
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						DEALLOCATE (response);
                        return NULL;
                    }

                    status->eStatus = RPC_FAULT;    
                    status->fault_code = 9002;    
                }

                if (RPC_FAULT  == status->eStatus)
                {
					stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
                    if (NULL == stFault)
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }

					stFault->FaultCode = ALLOCATE( 8);
                    if (NULL == stFault->FaultCode )
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
					stFault->FaultString = ALLOCATE( 1000);
                    if (NULL == stFault->FaultString)
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
                    if (status->fault_code == 9001 )
                    {
                        ENB_STRNCPY(stFault->FaultCode, "9001", 5);
                        ENB_STRNCPY(stFault->FaultString, "Request denied (no reason specified)", 999);
                    }
                    else if (status->fault_code == 9002 )
                    {
                        ENB_STRNCPY(stFault->FaultCode, "9002", 5);
                        ENB_STRNCPY(stFault->FaultString, "Internal error", 999);
                    }
                    struct SOAP_ENV__Fault *soap_fault = {0};
                    struct SOAP_ENV__Detail *detail =  NULL;

					detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
                    if (NULL == detail )
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
					soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
                    if (NULL == soap_fault )
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
					soap_fault->faultcode = ALLOCATE( 8);
                    if (NULL == soap_fault->faultcode )
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
					soap_fault->faultstring = ALLOCATE( 100);
                    if (NULL == soap_fault->faultstring  )
                    {
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
                        exit(0);
                    }
                    soap_fault->detail = detail;

                    detail->__any = stFault;
                    ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
                    ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
                
                    req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);  


					DEALLOCATE(soap_fault->faultstring);
					DEALLOCATE(detail);
					DEALLOCATE(status);
					DEALLOCATE(soap_fault->faultcode);
					DEALLOCATE(soap_fault);
					DEALLOCATE (stFault->FaultString);
					DEALLOCATE (stFault->FaultCode);
					DEALLOCATE(stFault);
					DEALLOCATE (response);
                }
                else
                {
                    response->MethodList = status->resp->MethodList;
				req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__GetRPCMethodsResponse,&header);
                    for (count=0;count<response->MethodList->__size;count++)
                    {
						DEALLOCATE (response->MethodList->__ptrstring[count]);
                    } 
					DEALLOCATE (response->MethodList->__ptrstring);
					DEALLOCATE (response->MethodList);
					DEALLOCATE (response);
					DEALLOCATE(status->resp);
					DEALLOCATE(status);
               /* coverity 86510 fix start */
               /*code remove */
               /* coverity 86510 fix end */

                }
                /* SPR 17596 fixed end */
				break;
			}/* End of case SOAP_TYPE__cwmp__GetRPCMethods : */

		case SOAP_TYPE__cwmp__Download :
			{
				DownloadResponse* dw_resp = {0};
				STDownload *tmp_resp = (STDownload *)resp;
                /*  coverity 86520 fix start */
				UINT16 out_rpc_res;
                /*  coverity 86520 fix end */
				dw_resp = ALLOCATE( sizeof(DownloadResponse));
				if (NULL == dw_resp)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}
				//soap_fault->detail = detail;
				out_rpc_res = ipr_cwmp_Download(tmp_resp);
				if (out_rpc_res <= 1)
				{
					if(1 == out_rpc_res)
						dw_resp->Status = _cwmp__DownloadResponse_Status__1;
					else if(0 == out_rpc_res)
						dw_resp->Status = _cwmp__DownloadResponse_Status__0;

                    /* spr 17961 fix start */
					dw_resp->StartTime = 0;
					dw_resp->CompleteTime = 0;
                    /* spr 17961 fix end */
					req_xml = soap_encode((void *)dw_resp, SOAP_TYPE__cwmp__DownloadResponse,&header);
                    /* SPR_19838:Coverity_12927 Fix Start */
                    /* Code Removed */ 
                    /* SPR_19838:Coverity_12927 Fix Stop */
					ENB_FREE (resp);
				} 
				else
				{
					struct SOAP_ENV__Fault *soap_fault = {0};
					soap_fault = ipr_cwmp_construct_soap_fault(out_rpc_res);
					if (NULL != soap_fault )
					{
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
						if (soap_fault->detail)
						{
							DEALLOCATE(soap_fault->detail->__any->FaultString);
							DEALLOCATE (soap_fault->detail->__any->FaultCode);
							DEALLOCATE (soap_fault->detail->__any);
							DEALLOCATE (soap_fault->detail);
						}
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE (soap_fault);
						/*download upload change starts*/
                        /* SPR_19838:Coverity_12927 Fix Start */
                        /* Code Removed */ 
                        /* SPR_19838:Coverity_12927 Fix Stop */
						/*download upload change ends*/
					}
				}
                /* SPR_19838:Coverity_12927 Fix Start */
                DEALLOCATE(dw_resp ); 
                /* SPR_19838:Coverity_12927 Fix Stop */

				break;
			}/*End of case SOAP_TYPE__cwmp__Download : */
		case SOAP_TYPE__cwmp__ScheduleDownload :
			{
				struct _cwmp__ScheduleDownloadResponse *resp_t = {0};
				ScheduleDownload *inp_t = {0};
				inp_t = (ScheduleDownload *)resp;
                /*  coverity 86520 fix start */
				UINT16 out_rpc_res;
                /*  coverity 86520 fix end */

				out_rpc_res = ipr_cwmp_startScheduleDownload(inp_t);

				resp_t = (struct _cwmp__ScheduleDownloadResponse *)ALLOCATE( 
						sizeof(struct _cwmp__ScheduleDownloadResponse));

				if (resp_t)
				{

					if (out_rpc_res > 1)
					{
						struct SOAP_ENV__Fault *soap_fault = {0};
						soap_fault = ipr_cwmp_construct_soap_fault(out_rpc_res);
						if (NULL != soap_fault )
						{
							req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
							if (soap_fault->detail)
							{
								DEALLOCATE(soap_fault->detail->__any->FaultString);
								DEALLOCATE (soap_fault->detail->__any->FaultCode);
								DEALLOCATE (soap_fault->detail->__any);
								DEALLOCATE (soap_fault->detail);
							}
							DEALLOCATE(soap_fault->faultstring);
							DEALLOCATE(soap_fault->faultcode);
							DEALLOCATE (soap_fault);
							DEALLOCATE(resp_t);
						}

						/* Coverity 40629 start */
						DEALLOCATE(resp_t);
						/* Coverity 40629 end */

					}
					else
					{
						req_xml = soap_encode((void *)resp_t , SOAP_TYPE__cwmp__ScheduleDownloadResponse , &header);
						DEALLOCATE(resp_t);
					}
				}
				break;    
			}

		case SOAP_TYPE__cwmp__GetParameterNames :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received GetParameterNames Request in %s ",__FUNCTION__);
				GetParameterNamesStatus_t *status = {0};
				struct  _cwmp__GetParameterNamesResponse *response = {0};
				struct _cwmp__Fault *stFault = {0};
				Uint8 *fault_string = NULL;

				sGetParameterNames *paramVal = (sGetParameterNames *)resp; // typedef _cwmp__GetParameterNames sGetParameterNames

				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Parameter Path Name %s",paramVal->ParameterPath);
				status = ipr_cwmp_getparametername(paramVal);
				/* SPR 21563 FIXED START*/
				if (NULL != paramVal->ParameterPath)
				{	
					ENB_FREE(paramVal->ParameterPath);
                } 
				/* SPR 21563 FIXED END*/
				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Fault response " );
						fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);

						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if(stFault)
						{
							stFault->FaultCode = ALLOCATE( 8);
							if (NULL == stFault->FaultCode  )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
							stFault->FaultString = ALLOCATE( 1000);
							if (NULL == stFault->FaultString )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
						}else
							return NULL;

						if(stFault->FaultCode &&stFault->FaultString)
						{
							snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
							ENB_STRNCPY (stFault->FaultString , fault_string, 64);
						}

						struct SOAP_ENV__Fault *soap_fault = NULL ;
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->detail = detail;
						detail->__any = stFault;

						ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
						ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(detail);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
                        /* Spr 16990 Fix Start */

                        /*  coverity 96677 fix start */
                        /* SPR 21563_p1 FIXED START */
						/* Code removed */
						/* SPR 21563_p1 FIXED END */
                        /* SPR 21563_p1 FIXED START */
							DEALLOCATE(status->fault_string);
							DEALLOCATE(status);
                        /* SPR 21563_p1 FIXED END */
                        /*  coverity 96677 fix end */

                        /* Spr 16990 Fix End */
					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Succss response " );
						response =  (struct  _cwmp__GetParameterNamesResponse *) ALLOCATE( sizeof(struct  _cwmp__GetParameterNamesResponse));
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}
						response->ParameterList = status->parameterList;
						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__GetParameterNamesResponse,&header);

                        /* Spr 16990 Fix Start */
                        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Freeing the GPN response buffer");
                        free_gpn_resp_buffer(status);  
                        /* Spr 16990 Fix End */
					}
				}

                /*  coverity 96653 fix start */
                if(response)
					DEALLOCATE (response);
                /*  coverity 96653 fix end */

				break;
			}

		case SOAP_TYPE__cwmp__SetParameterAttributes :
			{
				SetParamAttrRes *res = NULL;
				struct _cwmp__SetParameterAttributes *Setparamattr_t = NULL;
				/*SPR 17625 FIXED START*/
                struct _cwmp__SetParameterAttributesResponse *response = NULL;
				/*SPR 17625 FIXED END*/
				SetParameterAttributesList *paramlist_t = NULL;
				struct _cwmp__Fault *stFault = NULL;
				char *fault_string = NULL;
				Setparamattr_t = (struct _cwmp__SetParameterAttributes *)resp;
				if (Setparamattr_t && Setparamattr_t->ParameterList)
					paramlist_t = Setparamattr_t->ParameterList;
				res = (SetParamAttrRes *)ipr_cwmp_setParameterAttribute(paramlist_t);
				if (res && res->eStatus == RPC_FAULT)
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "in function %s ..recieved fault ",__FUNCTION__);
					struct SOAP_ENV__Fault *soap_fault = {0};
					fault_string = ipr_cwmp_cpeErr_to_string(res->fault_code);
					struct SOAP_ENV__Detail *detail =  {0};
					detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
					if (NULL == detail )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
					soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
					if (soap_fault)
					{
						soap_fault->faultcode = (char *)ALLOCATE( 8);
						if (NULL == soap_fault->faultcode  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = (char *)ALLOCATE( 64);
						if (NULL == soap_fault->faultstring )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						if (soap_fault->faultcode && soap_fault->faultstring)
						{
							ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 64);
							ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						}
					}else
					{
						DEALLOCATE(detail);
						return NULL;
					}
					soap_fault->detail = detail;
					stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
					if (stFault)
					{
						stFault->FaultCode = ALLOCATE( 8);
						if (NULL == stFault->FaultCode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultString = ALLOCATE( 64);
						if (NULL == stFault->FaultString  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						if (stFault->FaultCode && stFault->FaultString)
						{
							snprintf (stFault->FaultCode , 8, "%d", res->fault_code);
							ENB_STRNCPY (stFault->FaultString , fault_string, 64);
						}
						detail->__any = stFault;
					}
					req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

					if (soap_fault)
					{
						DEALLOCATE(soap_fault->detail);
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(soap_fault);
					}
					if (stFault)
					{
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
					}
				}
				else
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "in function %s ..recieved success Resp ",__FUNCTION__);
					/* SPR 17625 FIXED START */
					response = (struct _cwmp__SetParameterAttributesResponse *)ALLOCATE(
							sizeof(struct _cwmp__SetParameterAttributesResponse));
					if (NULL == response )
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
						exit(0);
					}
                response->Status = _cwmp__SetParameterValuesResponse_Status__0 ;
                req_xml = soap_encode((void *) response , SOAP_TYPE__cwmp__SetParameterAttributesResponse ,&header);
					DEALLOCATE(response);
				/* SPR 17625 FIXED END*/
				}
				/* SPR 21563_p1 FIXED START */
				DEALLOCATE(res);
				/* SPR 21563_p1 FIXED END */
				Free_SPA_Request(Setparamattr_t);
				/* SPR 21563 FIXED END*/
				break;
			}
/*GPA Change Start*/
		case SOAP_TYPE__cwmp__GetParameterAttributes :
			{
				GetParamAttrRes *res = {0};
				struct _cwmp__GetParameterAttributes *l_getparamAttr_t = {0};

				l_getparamAttr_t = (struct _cwmp__GetParameterAttributes *)resp;

				if (l_getparamAttr_t && l_getparamAttr_t->ParameterNames)
				{
					res = (GetParamAttrRes *)ipr_cwmp_GetParameterAttribute(l_getparamAttr_t->ParameterNames);
					if (res && res->eStatus == RPC_FAULT)
					{
						struct SOAP_ENV__Fault *soap_fault = {0};
						soap_fault = ipr_cwmp_construct_soap_fault(res->fault_code);
						if (NULL != soap_fault )
						{
							req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
							if (soap_fault->detail)
							{
								DEALLOCATE(soap_fault->detail->__any->FaultString);
								DEALLOCATE (soap_fault->detail->__any->FaultCode);
								DEALLOCATE (soap_fault->detail->__any);
								DEALLOCATE (soap_fault->detail);
							}
							DEALLOCATE(soap_fault->faultstring);
							DEALLOCATE(soap_fault->faultcode);
							DEALLOCATE (soap_fault);
						}
					}
                    /*  coverity 96648/96617 fix start */
					else if(res && res->eStatus == RPC_SUCCESS)
                    /*  coverity 96648/96617 fix end */
					{
						struct _cwmp__GetParameterAttributesResponse * resp_t = NULL;
						resp_t = (struct _cwmp__GetParameterAttributesResponse *)ALLOCATE(
								sizeof(struct _cwmp__GetParameterAttributesResponse));
						if (NULL == resp_t )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						/* SPR 19264 FIXED START */
						if (res && resp_t)
						/* SPR 19264 FIXED END */
                            resp_t->ParameterList = res->parameterList;
						req_xml = soap_encode((void *) resp_t , 
								SOAP_TYPE__cwmp__GetParameterAttributesResponse ,&header);
                       
                        /*  coverity 96648 fix start */
                        if(res && res->parameterList != NULL)
                        /*  coverity 96648 fix end */
                        {
                            TRACE (LOG_CRITICAL , SESSION_MANAGER,"Free Allocated Memory");
                            for(index = 0;index < res->parameterList->__size; index++)
                            {
								DEALLOCATE (res->parameterList->__ptrParameterAttributeStruct[index]->AccessList->__ptrstring);
                                res->parameterList->__ptrParameterAttributeStruct[index]->AccessList->__ptrstring = NULL;
								DEALLOCATE (res->parameterList->__ptrParameterAttributeStruct[index]->AccessList);
                                res->parameterList->__ptrParameterAttributeStruct[index]->AccessList = NULL;
								DEALLOCATE (res->parameterList->__ptrParameterAttributeStruct[index]->Name);
                                res->parameterList->__ptrParameterAttributeStruct[index]->Name = NULL;
								DEALLOCATE (res->parameterList->__ptrParameterAttributeStruct[index]);
                            }
							DEALLOCATE (res->parameterList->__ptrParameterAttributeStruct);
                            res->parameterList->__ptrParameterAttributeStruct = NULL;
							DEALLOCATE(res->parameterList);
                            res->parameterList = NULL;
                        }
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"Successfully freed the memory");
						DEALLOCATE(res);
                  /* coverity 86670 fix start */
                  /*code remove */
                  /* coverity 86670 fix end */	

						DEALLOCATE(resp_t);
                  /* coverity 86670 fix start */
                  /*code remove */
                  /* coverity 86670 fix end */	

               }
                    else 
                    {
                        TRACE (LOG_CRITICAL , SESSION_MANAGER,"res is NULL, send Internal Error");
                        struct SOAP_ENV__Fault *soap_fault = {0};
                        soap_fault = ipr_cwmp_construct_soap_fault(9003);
                        if (NULL != soap_fault )
                        {
                            req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
                            if (soap_fault->detail)
                            {
								DEALLOCATE(soap_fault->detail->__any->FaultString);
								DEALLOCATE (soap_fault->detail->__any->FaultCode);
								DEALLOCATE (soap_fault->detail->__any);
								DEALLOCATE (soap_fault->detail);
							}
							DEALLOCATE(soap_fault->faultstring);
							DEALLOCATE(soap_fault->faultcode);
							DEALLOCATE (soap_fault);
				}
                    }
				}
                else 
                {
                    TRACE (LOG_CRITICAL , SESSION_MANAGER,"Wrong SOAP messeage format");
                    struct SOAP_ENV__Fault *soap_fault = {0};

                    /*Send Method Not Supported */
                    soap_fault = ipr_cwmp_construct_soap_fault(9000);
                    if (NULL != soap_fault )
                    {
                        req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
                        if (soap_fault->detail)
                        {
							DEALLOCATE(soap_fault->detail->__any->FaultString);
							DEALLOCATE (soap_fault->detail->__any->FaultCode);
							DEALLOCATE (soap_fault->detail->__any);
							DEALLOCATE (soap_fault->detail);
						}
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE (soap_fault);
                    }
                }
                 /* SPR 21563_p1 FIXED START*/
				 Free_GPA_Request(l_getparamAttr_t);
				 /* SPR 21563_p1 FIXED END*/
				break;
			}
/* GPA Change End */
		case SOAP_TYPE__cwmp__AddObject :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Add Object Request in %s ",__FUNCTION__);
				AddObjectStatus_t *status = {0};
				struct _cwmp__AddObjectResponse *response = {0};
				struct _cwmp__Fault *stFault = {0};
				Uint8 *fault_string = NULL;

				sAddObject *paramVal = (sAddObject *)resp;
				status = ipr_cwmp_AddObject(paramVal);
				/* spr 18907 valgrind fix start */
                if(NULL != paramVal)
                {
                    if(NULL != paramVal->ObjectName)
                    {
				ENB_FREE(paramVal->ObjectName);
                    }
                    if(NULL != paramVal->ParameterKey)
                    {
				ENB_FREE(paramVal->ParameterKey);
                    }
				ENB_FREE(paramVal);
                }
				/* spr 18907 valgrind fix end */


				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Fault response " );
						fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);

						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if(stFault)
						{
							stFault->FaultCode = ALLOCATE( 8);
							if (NULL == stFault->FaultCode  )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
							stFault->FaultString = ALLOCATE( 1000);
							if (NULL == stFault->FaultString )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
						}else
							return NULL;

						if(stFault->FaultCode &&stFault->FaultString)
						{
							snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
							ENB_STRNCPY (stFault->FaultString , fault_string, 64);
						}

						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->detail = detail;
						detail->__any = stFault;

						ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
						ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(detail);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Succss response " );
						response =  (struct _cwmp__AddObjectResponse *) ALLOCATE( sizeof(struct _cwmp__AddObjectResponse));
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}
						response->InstanceNumber = status->instance_number;
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received instance no %d ",status->instance_number);

						if(RPC_PARTIAL ==  status->eStatus)
							response->Status = _cwmp__AddObjectResponse_Status__1 ;
						else if((RPC_SUCCESS == status->eStatus))
							response->Status = _cwmp__AddObjectResponse_Status__0 ;

						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__AddObjectResponse,&header);

						DEALLOCATE(response);

					}
					/* SPR 21563_p1 FIXED START */
					DEALLOCATE(status);
					/* SPR 21563_p1 FIXED END */
				}


				break;
			}
		case SOAP_TYPE__cwmp__ScheduleInform:
			{
				struct _cwmp__ScheduleInformResponse *res_t = {0};
				struct _cwmp__ScheduleInform *l_inp = (struct _cwmp__ScheduleInform *)resp; 
				UINT32 out_rpc_res = 0;

				out_rpc_res = ipr_cwmp_ScheduleInform(l_inp);
				if (out_rpc_res == TRUE)
				{
					res_t = ALLOCATE( sizeof(struct _cwmp__ScheduleInformResponse));
					if (res_t)
					{
						req_xml = soap_encode((void *)res_t , SOAP_TYPE__cwmp__ScheduleInformResponse ,&header);
						DEALLOCATE (res_t);
					}
				}
				else
				{
					struct SOAP_ENV__Fault *soap_fault = {0};
					soap_fault = ipr_cwmp_construct_soap_fault(out_rpc_res);
					if (NULL != soap_fault )
					{
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
						if (soap_fault->detail)
						{
							DEALLOCATE(soap_fault->detail->__any->FaultString);
							DEALLOCATE (soap_fault->detail->__any->FaultCode);
							DEALLOCATE (soap_fault->detail->__any);
							DEALLOCATE (soap_fault->detail);
						}
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(soap_fault);
					}
				}        
				break;
			}
		case SOAP_TYPE__cwmp__GetAllQueuedTransfers:
			{

				AllTransferList *res_list = {0};
				struct _cwmp__GetAllQueuedTransfersResponse * resp_t = NULL;


				res_list = (AllTransferList *)ipr_cwmp_GetAllQueuedTransfers();
				if (NULL == res_list)
				{
					struct SOAP_ENV__Fault *soap_fault = {0};
					soap_fault = ipr_cwmp_construct_soap_fault(9002);
					if (NULL != soap_fault )
					{
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
						if (soap_fault->detail)
						{
							DEALLOCATE(soap_fault->detail->__any->FaultString);
							DEALLOCATE (soap_fault->detail->__any->FaultCode);
							DEALLOCATE (soap_fault->detail->__any);
							DEALLOCATE (soap_fault->detail);
						}
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(soap_fault);
						//DEALLOCATE(res_list);
					}
				}
				else
				{   
					resp_t = (struct _cwmp__GetAllQueuedTransfersResponse *)ALLOCATE( 
							sizeof(struct _cwmp__GetAllQueuedTransfersResponse));

					if (NULL !=  resp_t)
					{
						resp_t->TransferList = res_list;
						req_xml = soap_encode((void *)resp_t , SOAP_TYPE__cwmp__GetAllQueuedTransfersResponse , &header);
						free_transferList(res_list);
						DEALLOCATE(resp_t);
					}else
					{
						perror("Memory allocatio fail");
						exit(1);
						//free_transferList(res_list);
						//DEALLOCATE(resp_t);

					}

				}
				break;

			}

		case SOAP_TYPE__cwmp__DeleteObject :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Delete Object Request in %s ",__FUNCTION__);
				DeleteObjectStatus_t *status = {0};
				struct _cwmp__DeleteObjectResponse *response = {0};
				struct _cwmp__Fault *stFault = {0};

				sDeleteObject *paramVal = (sDeleteObject *)resp;  //typedef _cwmp__DeleteObject sDeleteObject;

				status = ipr_cwmp_deleteobject(paramVal);
				/* spr 18907 valgrind fix start */
                if (NULL != paramVal)
                {
                    if (NULL != paramVal->ObjectName)
                    {
				ENB_FREE(paramVal->ObjectName);
                    }
                    if (NULL != paramVal->ParameterKey)
                    {
				ENB_FREE(paramVal->ParameterKey);
                    }
				ENB_FREE(paramVal);
                }
                /* spr 18907 valgrind fix end */

				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Fault response " );
						Uint8 *fault_string = NULL;
						fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);
						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if(stFault)
						{
							stFault->FaultCode = ALLOCATE( 8);
							if (NULL == stFault->FaultCode  )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
							stFault->FaultString = ALLOCATE( 1000);
							if (NULL == stFault->FaultString )
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
								exit(0);
							}
						}else
						{
							DEALLOCATE(stFault);
							return NULL;
						}
						if(stFault->FaultCode &&stFault->FaultString)
						{
							snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
							ENB_STRNCPY (stFault->FaultString , fault_string, 64);
						}

						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->detail = detail;
						detail->__any = stFault;

						ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
						ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(detail);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
						//DEALLOCATE (status);
						//status = NULL;
					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received SUCCESS response " );
						response =  (struct _cwmp__DeleteObjectResponse *) ALLOCATE(sizeof(struct _cwmp__DeleteObjectResponse ) );
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}
						// fill value of  _cwmp__DeleteObjectResponse Structt
						if(RPC_PARTIAL ==  status->eStatus)
							response->Status = _cwmp__DeleteObjectResponse_Status__1;
						else if((RPC_SUCCESS == status->eStatus))
							response->Status = _cwmp__DeleteObjectResponse_Status__0;
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "DEBBUG: In func %s response status %d  status->estatus %d ",__FUNCTION__ , response->Status ,status->eStatus);
						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__DeleteObjectResponse,&header);
						DEALLOCATE(response);

					}
					/* SPR 21563_p1 FIXED START */
					DEALLOCATE(status);
					/* SPR 21563_p1 FIXED END */
				}

				break;
			}

		case SOAP_TYPE__cwmp__Reboot :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Reboot Request in %s ",__FUNCTION__);
				RebootStatus_t *status = {0};
				struct _cwmp__Fault *stFault = {0};
				struct _cwmp__RebootResponse *response = {0};

				sReboot *paramVal = (sReboot *)resp; //typedef struct _cwmp__Reboot sReboot; 

				status = ipr_cwmp_reboot();
				Uint8 *fault_string = NULL;
				Uint8 *event_code = NULL;
				Uint8 *cmd_key = NULL;
				FILE *file = NULL;
				FILE *cmd_key_file = NULL;

				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if (NULL == stFault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultCode = ALLOCATE( 8);
						if (NULL == stFault->FaultCode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultString = ALLOCATE( 100);
						if (NULL == stFault->FaultString  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);

						snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
						ENB_STRNCPY (stFault->FaultString , fault_string, 64);

						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->detail = detail;
						detail->__any = stFault;

						if(soap_fault->faultcode && soap_fault->faultstring)
						{
							ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
							ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						}
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						DEALLOCATE(soap_fault->detail);
						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);


					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received SUCCESS response " );
						response =  (struct _cwmp__RebootResponse *)ALLOCATE( sizeof(struct _cwmp__RebootResponse));
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}
						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__RebootResponse,&header);
						DEALLOCATE(response);
                        /* SPR 17268 FIXED START */
                        UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
                         strcpy((char *)ABSPath1,(char *)ABSPath);

                       TRACE (LOG_CRITICAL , SESSION_MANAGER, "path before concat, ABSPath1 %s" , ABSPath1);
                        strncat((char *)ABSPath1 , EVENT_CODE_FILE , strlen(EVENT_CODE_FILE));

                        TRACE (LOG_CRITICAL , SESSION_MANAGER, "after concat, ABSPath1 %s" ,ABSPath1);
                        file = fopen((char *)ABSPath1, "w+");
			/* SPR 17268 FIXED END */

						//file = fopen(EVENT_CODE_FILE, "w+");
						if (NULL == file)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Error in opening file");
						}
						else
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "even code & command key for reboot");

							event_code = (Uint8 *) ALLOCATE( EVENT_CODE_SIZE);
							if(event_code)
							{
								tr_strncpy(event_code, REBOOT_CODE);
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "copied str %s", event_code);
								fwrite(event_code, strlen(REBOOT_CODE), 1, file); 
								fclose(file);
								DEALLOCATE(event_code);
							}else 
								fclose(file);
							if(paramVal->CommandKey == NULL)
							{
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "command key not received");
							}
							else
							{
								if(strlen(paramVal->CommandKey) != 0)
								{
									TRACE (LOG_CRITICAL , SESSION_MANAGER, "received command key %s",paramVal->CommandKey);
									TRACE (LOG_CRITICAL , SESSION_MANAGER, "received command key length %d",strlen(paramVal->CommandKey));
									


                                 /* SPR 17268 FIXED START */
                                     UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
                                     strcpy((char *)ABSPath2,(char *)ABSPath);

                                    strncat((char *)ABSPath2 , REBOOT_CMD_KEY, strlen(REBOOT_CMD_KEY));

                                    cmd_key_file = fopen((char *)ABSPath2, "w+");
                                    /* SPR 17268 FIXED END */

                                //    cmd_key_file = fopen(REBOOT_CMD_KEY, "w+");
									if (NULL != cmd_key_file)
									{
										cmd_key = (Uint8 *) ALLOCATE(COMMAND_KEY_SIZE);
										if(cmd_key != NULL)
										{
											ENB_STRNCPY(cmd_key, paramVal->CommandKey, strlen(paramVal->CommandKey));
											fwrite(cmd_key, strlen(cmd_key), 1, cmd_key_file);
											TRACE (LOG_CRITICAL , SESSION_MANAGER, "cmd key written into file");
											fclose(cmd_key_file);
											DEALLOCATE(cmd_key);
										}else
										{
											fclose(cmd_key_file);
											return NULL;
										}
									}
									else
									{
										TRACE (LOG_CRITICAL , SESSION_MANAGER, "Error in opening file for command key received in reboot req");
									}

								}
							}
						}
					}
				}
				break;
			}
		case SOAP_TYPE__cwmp__Upload :
			{
				struct _cwmp__UploadResponse *upl_res_t = {0};
				struct _cwmp__Upload *l_inp_t = (struct _cwmp__Upload *)resp;
				//SInt32 out_rpc_res = 1;
                /*  coverity 86520 fix start */
				UINT16 out_rpc_res;
                /*  coverity 86520 fix end */

				out_rpc_res = ipr_cwmp_Upload(l_inp_t);
				upl_res_t = ALLOCATE( sizeof(struct _cwmp__UploadResponse));
				if (upl_res_t)
				{
					if (out_rpc_res > 1)
					{
						struct SOAP_ENV__Fault *soap_fault = {0};
						soap_fault = ipr_cwmp_construct_soap_fault(out_rpc_res);
						if (NULL != soap_fault )
						{
							req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
							if (soap_fault->detail)
							{
								DEALLOCATE(soap_fault->detail->__any->FaultString);
								DEALLOCATE (soap_fault->detail->__any->FaultCode);
								DEALLOCATE (soap_fault->detail->__any);
								DEALLOCATE (soap_fault->detail);
							}
							DEALLOCATE(soap_fault->faultstring);
							DEALLOCATE(soap_fault->faultcode);
							DEALLOCATE(soap_fault);
							 /*download upload change starts*/
                             /* SPR_19838:Coverity_12925 Fix Start */
                            /* Code Removed */
                            /* SPR_19838:Coverity_12925 Fix Stop */
							/*download upload change ends*/
						}        
					}
					else 
					{
						upl_res_t->Status = _cwmp__UploadResponse_Status__1;
                        /* spr 17961 fix start */
						upl_res_t->StartTime = 0; 
						upl_res_t->CompleteTime = 0;
                        /* spr 17961 fix end */ 
						req_xml = soap_encode((void *)upl_res_t , SOAP_TYPE__cwmp__UploadResponse ,&header);
                         /* SPR_19838:Coverity_12925 Fix Start */
                        /* Code Removed */
                        /* SPR_19838:Coverity_12925 Fix Stop */
                        /*  coverity 86490 fix code removed */
					}
                    /* SPR_19838:Coverity_12925 Fix Start */
                    DEALLOCATE(upl_res_t);
                    /* SPR_19838:Coverity_12925 Fix Stop */

				} 
				/* SPR 21563_p1 FIXED START */
				Free_Upload_Req(l_inp_t);
				/* SPR 21563_p1 FIXED END */
				break;
			}
		case SOAP_TYPE__cwmp__ChangeDUState:
			{
				//  struct _cwmp__ChangeDUState * inp = (struct _cwmp__ChangeDUState *) resp;


				break;

			}

		case SOAP_TYPE__cwmp__FactoryReset :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Factory Reset Request in %s ",__FUNCTION__);
				FactoryResetStatus_t  *status = {0};
				struct _cwmp__Fault *stFault = {0};
				Uint8 *fault_string = NULL;
				struct _cwmp__FactoryResetResponse *response = {0};

				status = ipr_cwmp_factoryreset();

				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if (NULL == stFault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						stFault->FaultCode = ALLOCATE( 8);
						if (NULL == stFault->FaultCode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultString = ALLOCATE( 100);
						if (NULL == stFault->FaultString  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);

						snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
						ENB_STRNCPY (stFault->FaultString , fault_string, 64);

						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->detail = detail;
						detail->__any = stFault;

						if(soap_fault->faultcode && soap_fault->faultstring)
						{
							ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
							ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						}
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(detail);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received SUCCESS response " );
						response =  (struct _cwmp__FactoryResetResponse *)ALLOCATE( sizeof(struct _cwmp__FactoryResetResponse));
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							return NULL;
						}
						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__FactoryResetResponse,&header);
						DEALLOCATE(response);


                         /* SPR 17268 FIXED START */
                        UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
                         strcpy((char *)ABSPath1,(char *)ABSPath);

                        strncat((char *)ABSPath1, BOOTSTRAP_FILE_NAME, strlen(BOOTSTRAP_FILE_NAME));

                        FILE* file1 = fopen((char *)ABSPath1, "r");

                        /* SPR 17268 FIXED END */

						//FILE* file1 = fopen(BOOTSTRAP_FILE_NAME, "r");
						if(file1 == NULL)
						{
                            /* SPR 17268 fixed start */
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Error opening file to restore saved IPs");
							/* SPR 20429 FIXED START */
							/*code removed*/
							/* SPR 20429 FIXED END */

						}else	
						{
							if(-1 ==  remove((char*)ABSPath1))
                            /* SPR 17268 fixed end */
								TRACE (LOG_CRITICAL , SESSION_MANAGER, "ERROR in remove  in  %s ",__FUNCTION__);
							fclose(file1);
						}
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "System is  Reset to its default state in  %s ",__FUNCTION__);
					}
				}

				break;
			}
		case SOAP_TYPE__cwmp__CancelTransfer :
			{
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received CancelTransfer Request in %s ",__FUNCTION__);
				CancelTransferStatus_t *status = {0};
				struct _cwmp__CancelTransferResponse *response = {0};
				struct _cwmp__Fault *stFault = {0};
				//Uint8 *fault_string = NULL;


				sCancelTransfer *paramVal = (sCancelTransfer *)resp;

				status = ipr_cwmp_canceltransfer(paramVal);

				ENB_FREE(paramVal);

				if(status){
					if(RPC_FAULT == status->eStatus)
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Fault response " );
						//fault_string = ipr_cwmp_cpeErr_to_string(status->fault_code);

						stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
						if (NULL == stFault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultCode = ALLOCATE( 8);
						if (NULL == stFault->FaultCode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						stFault->FaultString= ALLOCATE( 100);
						if (NULL == stFault->FaultString )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						snprintf (stFault->FaultCode , 8, "%d", status->fault_code);
						ENB_STRNCPY (stFault->FaultString , status->fault_string, 64);

						struct SOAP_ENV__Fault *soap_fault = {0};
						struct SOAP_ENV__Detail *detail =  NULL;

						soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
						if (NULL == soap_fault )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
						if (NULL == detail )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}

						soap_fault->faultcode = ALLOCATE( 8);
						if (NULL == soap_fault->faultcode )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->faultstring = ALLOCATE( 100);
						if (NULL == soap_fault->faultstring  )
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							exit(0);
						}
						soap_fault->detail = detail;
						detail->__any = stFault;

						ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
						ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
						req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);

						DEALLOCATE(soap_fault->faultstring);
						DEALLOCATE(soap_fault->faultcode);
						DEALLOCATE(detail);
						DEALLOCATE(soap_fault);
						DEALLOCATE (stFault->FaultString);
						DEALLOCATE (stFault->FaultCode);
						DEALLOCATE(stFault);
					}
					else
					{
						TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received Succss response " );
						response =  (struct _cwmp__CancelTransferResponse *) ALLOCATE( sizeof(struct _cwmp__CancelTransferResponse));
						if (NULL == response)
						{
							TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
							DEALLOCATE (status);
							status = NULL;
							return NULL;
						}
						req_xml = soap_encode((void *)response, SOAP_TYPE__cwmp__CancelTransferResponse,&header);

						DEALLOCATE(response);

					}
				}
				//cob
				if(status)
					DEALLOCATE(status);
				break;
			}
		default :
			{
				struct _cwmp__Fault *stFault = NULL;
				TRACE (LOG_CRITICAL , SESSION_MANAGER, "Received UnSupported Request" );
				Uint8 *fault_string = NULL;
				fault_string = ipr_cwmp_cpeErr_to_string(9001);
				stFault = (struct _cwmp__Fault *) ALLOCATE( sizeof(struct _cwmp__Fault ));
				if (NULL == stFault )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}
				stFault->FaultCode = ALLOCATE( 8);
				if (NULL == stFault->FaultCode )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}
				stFault->FaultString = ALLOCATE( 1000);
				if (NULL == stFault->FaultString  )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}

				snprintf (stFault->FaultCode , 8, "%d", 9001);
				ENB_STRNCPY (stFault->FaultString , fault_string, 64);

				struct SOAP_ENV__Fault *soap_fault = {0};
				struct SOAP_ENV__Detail *detail =  NULL;

				soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE( sizeof(struct SOAP_ENV__Fault));
				if (NULL == soap_fault )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}
				detail = (struct SOAP_ENV__Detail *) ALLOCATE( sizeof(struct SOAP_ENV__Detail));
				if (NULL == detail )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}

				soap_fault->faultcode = ALLOCATE( 8);
				if (NULL == soap_fault->faultcode )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}
				soap_fault->faultstring = ALLOCATE( 100);
				if (NULL == soap_fault->faultstring  )
				{
					TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
					exit(0);
				}

				soap_fault->detail = detail;
				detail->__any = stFault;

				ENB_STRNCPY (soap_fault->faultstring, "CWMP fault", 100);
				ENB_STRNCPY (soap_fault->faultcode, "Client", 8);
				req_xml = soap_encode(soap_fault, SOAP_TYPE_SOAP_ENV__Fault,&header);
				DEALLOCATE(soap_fault->faultstring);
				DEALLOCATE(soap_fault->faultcode);
				DEALLOCATE(detail);
				DEALLOCATE(soap_fault);
				DEALLOCATE (stFault->FaultString);
				DEALLOCATE (stFault->FaultCode);
				DEALLOCATE(stFault);

				break;
			}

	}/* End of switch(type) */
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with XML ",__FUNCTION__);
	return req_xml;
}/* End of function char *SendRespToDriver (void *resp, int type) */



/*SPR 17596 fixed start */
//code removed
/* SPR 17596 fixed end */



/***********************************************************************************
 *
 *       Function Name : ipr_cwmp_free_paramlist
 *
 *       Description   : This function will free memory allocated for Name,
 *                       Value and __ptrParameterValueStruct.
 *
 *       Returns       : NONE
 *
 **************************************************************************************/
/* SPR 22314 Start */
void tr069_free_parameterValueList(struct ParameterValueList *ptr)
{
    SInt32 i = 0;
    if (NULL == ptr)
    {
        return;
    }
    for (i = 0; i < ptr->__size; i++)
    {

        /*SPR 18923 FIXED START */
        if(NULL != ptr->__ptrParameterValueStruct[i]) {

        if(NULL != ptr->__ptrParameterValueStruct[i]->Name)
        {    
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Name[%d] = %s\t  ",i,ptr->__ptrParameterValueStruct[i]->Name);
        ENB_FREE(ptr->__ptrParameterValueStruct[i]->Name);
        ptr->__ptrParameterValueStruct[i]->Name = NULL;
        }
        if(NULL != ptr->__ptrParameterValueStruct[i]->Value)
        {    
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Value[%d] = %d\t  ",i,ptr->__ptrParameterValueStruct[i]->Value);
        ENB_FREE(ptr->__ptrParameterValueStruct[i]->Value);
        ptr->__ptrParameterValueStruct[i]->Value = NULL;    

        }
        
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "INDEX[%d] = %s\t  ",i,ptr->__ptrParameterValueStruct[i]);
        ENB_FREE(ptr->__ptrParameterValueStruct[i]);
        ptr->__ptrParameterValueStruct[i] = NULL;

        }
        /*SPR 18923 FIXED END */

    }
    ENB_FREE(ptr->__ptrParameterValueStruct);
    ENB_FREE(ptr);
} /* End of function void session_manager_free_paramlist(struct ParameterValueList *ptr) */

/* SPR 22314 End */


/***********************************************************************************
 *
 *       Function Name : free_eventlist
 *
 *       Description   : This function will free memory allocated for EventCode, i
 *       		CommandKey and __ptrEventStruct.
 *
 *       Returns       : NONE
 *
 **************************************************************************************/

void ipr_cwmp_free_eventlist(struct EventList *ptr)
{
    SInt32 i = 0;
    if (NULL == ptr)
    {
        return;
    }
    for (i = 0; i < ptr->__size; i++)
    {
		DEALLOCATE(ptr->__ptrEventStruct[i]->EventCode);
		DEALLOCATE(ptr->__ptrEventStruct[i]->CommandKey);
		DEALLOCATE(ptr->__ptrEventStruct[i]);
    }
	DEALLOCATE(ptr->__ptrEventStruct);
}/* End of function void free_eventlist(struct EventList *ptr) */





/***********************************************************************************
 *
 *       Function Name : session_manager_free_stat_status
 *
 *       Description   : This function will free the memory that is allocated
 *                       in driver for RPC
 *
 *       Returns       : NONE
 *
 **************************************************************************************/

inline void ipr_cwmp_free_stat_status(StStatus *a)
{
    SInt32 s = 0 ;
    struct _cwmp__Fault *f = NULL;
    struct _cwmp__Fault_SetParameterValuesFault *sf = NULL;

    if (a != NULL)
    {
        f = a->stFault;
        if ( f != NULL)
        {
            s = f->__sizeSetParameterValuesFault;
            s--;
            if (s > 0)
            {
                sf = f->SetParameterValuesFault;
                while ( s >= 0)
                {
					DEALLOCATE (sf[s].ParameterName);
                    sf[s].ParameterName  = NULL;
					DEALLOCATE (sf[s].FaultCode);
                    sf[s].FaultCode = NULL;
					DEALLOCATE (sf[s].FaultString);
                    sf[s].FaultString = NULL;
                    s--; 
                }
				DEALLOCATE (sf);
				/*  coverity 86494 fix code removed */
            }
			DEALLOCATE (f->FaultCode);
			DEALLOCATE (f->FaultString);
			DEALLOCATE (f);

        }
		DEALLOCATE (a);
    }
}




/***********************************************************************************
 *
 *       Function Name : ipr_cwmp_free_get_param 
 *
 *       Description   : This function will free the memory that is allocated
 *                       in driver for RPC
 *
 *       Returns       : NONE
 *
 **************************************************************************************/

inline void ipr_cwmp_free_get_param(ParameterNames *pNames)
{

    SInt32 s = 0 ;
    Uint8 **ptr = NULL;

    if (NULL == pNames)
        return;

    s = pNames->__size ;
    ptr = pNames->__ptrstring;
    s -= 1;

    while ( s >= 0)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "ptr[%d] = %s\t  ",s,ptr[s]);
        ENB_FREE (ptr[s]);
        ptr[s] = NULL;
        s--;
    }
    ENB_FREE(ptr);
    ENB_FREE(pNames);
    /*  coverity 86661 fix start */
    /* code removed */
    /*  coverity 86661 fix end */
}

/********************************************************************************
 *
 * Function Name : ipr_cwmp_free_FileTypeArg
 *
 * Description   : This function is used to free the pointers 
 *                  
 *
 * Returns       : NULL
 *
 ***********************************************************************************/
void ipr_cwmp_free_FileTypeArg(struct FileTypeArg *ptr)
{
    SInt32 i = 0;
    if (NULL == ptr)
    {
        return;
    }
    for (i = 0; i < ptr->__size; i++)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Name[%d] = %s\tValue[%d] = %s  ", i, ptr->__ptrArgStruct[i]->Name, i, ptr->__ptrArgStruct[i]->Value);
		DEALLOCATE(ptr->__ptrArgStruct[i]->Name);
		DEALLOCATE(ptr->__ptrArgStruct[i]->Value);
		DEALLOCATE(ptr->__ptrArgStruct[i]);
        ptr->__ptrArgStruct[i] = NULL;
    }
	DEALLOCATE(ptr->__ptrArgStruct);
} /* End of function void (struct FileTypeArg *ptr) */

/********************************************************************************
 *
 * Function Name : ipr_cwmp_construct_soap_fault
 *
 * Description   : This function contructs soap fault message  
 *                  
 *
 * Returns       : A structure of SOAP_ENV_Fault type.
 *
 ***********************************************************************************/
struct SOAP_ENV__Fault *ipr_cwmp_construct_soap_fault (UINT16 code)
{

	struct SOAP_ENV__Fault *soap_fault = NULL;
	CHAR8 *fault_string = NULL;
	struct _cwmp__Fault *stFault = NULL;
	struct SOAP_ENV__Detail *detail =  NULL;
	fault_string = ipr_cwmp_cpeErr_to_string(code);
	soap_fault = (struct SOAP_ENV__Fault *) ALLOCATE (sizeof(struct SOAP_ENV__Fault) );
	if (NULL == soap_fault )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		exit(0);
	}

	memset(soap_fault , '\0', sizeof(struct SOAP_ENV__Fault));

	detail = (struct SOAP_ENV__Detail *) ALLOCATE ( sizeof(struct SOAP_ENV__Detail)   );
	if (NULL == detail )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		exit(0);
	}

	memset(detail , '\0', sizeof(struct SOAP_ENV__Detail));
	soap_fault->faultcode = ALLOCATE ( 8  );
	if (NULL == soap_fault->faultcode )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		exit(0);
	}
	memset(soap_fault->faultcode  , '\0', 8);
	soap_fault->faultstring = ALLOCATE (100  );
	if (NULL == soap_fault->faultstring  )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
		exit(0);
	}
	memset(soap_fault->faultstring, '\0', 100);
	if (soap_fault->faultcode && soap_fault->faultstring)
	{
		ENB_STRNCPY(soap_fault->faultstring, "CWMP fault", 64);
		ENB_STRNCPY(soap_fault->faultcode, "Client", 8);
	}
	soap_fault->detail = detail;
	stFault = (struct _cwmp__Fault *)ALLOCATE(sizeof(struct _cwmp__Fault)  );
	if (stFault)
	{
			memset(stFault,'\0', sizeof(struct _cwmp__Fault ));
		stFault->FaultCode = ALLOCATE ( 8  );
			    if (NULL ==  stFault->FaultCode  )
			    {
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				    exit(0);
			    }
			memset(stFault->FaultCode,'\0', 8 );
		stFault->FaultString = ALLOCATE ( 64  );
			    if (NULL == stFault->FaultString  )
			    {
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ALLOCATE failed",__FUNCTION__);
				    exit(0);
			    }
			memset(stFault->FaultString ,'\0',64 );
		if (stFault->FaultCode && stFault->FaultString)
		{
			snprintf(stFault->FaultCode , 8, "%d", code);
			ENB_STRNCPY(stFault->FaultString , fault_string, 64);
		}
		detail->__any = stFault;
	}
	return soap_fault;
}

//Phase 2
/********************************************************************************
 * *
 * * Function Name : ipr_cwmp_AutoDUStateChangeCompleteReq
 * *
 * * Description   : This function is used to create xml for Auto DU State Change Complete 
 *                   response and send the xml to ACS
 * *
 * * Returns       : Auto DU State Change Complete Response if response comes
 * *                 NULL if response is not OK
 * *
 * * *********************************************************************************/
sAutoDUStateChangeCompResp * ipr_cwmp_AutoDUStateChangeCompleteReq(sAutoDUStateChangeComplete *autoDUChangeCompReq,  
        Uint8 *URL,     //URL of ACS
        SInt32 *status,
        Uint8 *username,
        Uint8 *password)    // Status need to filled for SUCCESS/FAILURE
{
    /*Coverity 111404 Fix Start*/
    void *autoDUChangeStateComeResp = NULL;
    /*Coverity 111404 Fix End*/
    Uint8 *req_xml = NULL;
    Uint8 *resp_xml = NULL;
    sHeader *header;
   /*Coverity 111411 Fix Start */
    SInt32 type = 0;
   /*Coverity 111411 Fix End */
    struct cwmp__AutonOpResultStruct *cpeList = NULL;

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    req_xml = (Uint8 *)soap_encode(autoDUChangeCompReq, SOAP_TYPE__cwmp__AutonomousDUStateChangeComplete,NULL);

    cpeList = autoDUChangeCompReq->Results;

    if(cpeList == NULL)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL  cpeList in req_xml",__FUNCTION__);
	//coverity fix
		DEALLOCATE(req_xml);
	return NULL;
    }

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "UUID in req_xml = %s  ",cpeList->UUID);

    if (NULL == req_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL req_xml",__FUNCTION__);
        return NULL;
    }
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "req_xml = %s  ",req_xml);

    *status = ipr_cwmp_send_http_msg(req_xml, URL, &resp_xml, username, password);
	DEALLOCATE (req_xml);
	/*  coverity 86566 fix code removed */
    if (*status != SUCCESS)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, " Failure Received Exiting in %s ",__FUNCTION__);
        return NULL;
    }
    if (NULL == resp_xml)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s resp_xml is NULL",__FUNCTION__);
        return NULL;
    }

    *status = soap_decode(resp_xml, &type, &autoDUChangeStateComeResp, &header);
	DEALLOCATE (resp_xml);
	/* coverity 86670 fix start */
   /*code remove */
   /* coverity 86670 fix end */
   

    if (SOAP_TYPE__cwmp__AutonomousDUStateChangeCompleteResponse == type)
    {
        *status = SUCCESS;
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return (sAutoDUStateChangeCompResp *)autoDUChangeStateComeResp;
    }

    if (SOAP_TYPE_cwmp__ACSFaultCodeType == type)
    {
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with Response %s"
                ,__FUNCTION__,err_to_string(*((unsigned int *)autoDUChangeStateComeResp)));
        *status = FAILED;
        return NULL;
    }

    *status = FAILED;
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
    return NULL;
}



/********************************************************************************
 *
 * Function Name : free_transferList
 *
 * Description   : This function is used to free the memory for parameter
 *                  List Struct.
 *
 * Returns       : NULL
 *                 
 *
 **********************************************************************************/


inline void free_transferList (AllTransferList *a)
{
    SInt32 i = 0 ;
    SInt32 s = 0 ;

    if (NULL == a)
        return;

    struct cwmp__AllQueuedTransferStruct ** b = NULL;
    b = a->__ptrAllQueuedTransferStruct;

    s = a->__size ;
    s--;

    if (NULL == b)
    {
		DEALLOCATE(a);
        return;
    }

    while (s >= i)
    {
		DEALLOCATE (b[s]->CommandKey);
		DEALLOCATE (b[s]->FileType);
		DEALLOCATE (b[s]->TargetFileName);
		DEALLOCATE (b[s]);
        s--;
    }
	DEALLOCATE(b);
	DEALLOCATE (a);
	/*  coverity 86595 fix code removed */
}
/* Spr 17463 Fix Start */
/****************************************************************************
 * Function Name  : free_gpv_resp_buffer 
 * Inputs         : p_gpn_res_t 
 * Outputs        : NONE 
 * Returns        : NONE 
 * Description    : This function frees dynamic allocated GPV response  
 * ***************************************************************************/
 /* SPR 21563_p1 FIXED START */
void free_gpv_resp_buffer (struct ParameterValueList *p_get_stat_t)
{

    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering From %s ",__FUNCTION__);
    SInt32 index = 0; 
    /* SPR 17882 fixed start */
    if (p_get_stat_t ==NULL)
    {

        TRACE (LOG_CRITICAL , SESSION_MANAGER, "p_get_stat_t is NULL");
        return ;
    }  
    else
    { 
        TRACE(LOG_CRITICAL , SESSION_MANAGER," ..... p_get_stat_t = %p " ,p_get_stat_t);
    }
    /* SPR 21563_p1 FIXED START */
	/* Code Removed */
	/* SPR 21563_p1 FIXED END */
	
    /* SPR 17882 fixed end */
	 /* SPR 21563_p1 FIXED START */
    if(p_get_stat_t->__size)
    {                           
        for(index = 0; index <p_get_stat_t->__size; index++)
        {
            /* SPR 17882 fixed start */
		  if (NULL != p_get_stat_t->__ptrParameterValueStruct)
		  {  
				
            if (NULL != (p_get_stat_t->__ptrParameterValueStruct[index]->Name))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER ,"... NAME =  %p " , p_get_stat_t->__ptrParameterValueStruct[index]->Name);
				DEALLOCATE (p_get_stat_t->__ptrParameterValueStruct[index]->Name);
                
            }
            if(NULL != (p_get_stat_t->__ptrParameterValueStruct[index]->Value))
            {
                TRACE(LOG_CRITICAL ,SESSION_MANAGER ,"VALUE = %p" , p_get_stat_t->__ptrParameterValueStruct[index]->Value);
				DEALLOCATE (p_get_stat_t->__ptrParameterValueStruct[index]->Value);
                
            }
            if(NULL != (p_get_stat_t->__ptrParameterValueStruct[index]))
            {
                TRACE (LOG_CRITICAL , SESSION_MANAGER , "INDEX = %p",p_get_stat_t->__ptrParameterValueStruct[index]);
				DEALLOCATE (p_get_stat_t->__ptrParameterValueStruct[index]);
                
            }
            /* SPR 17882 fixed end */
		  }
        }
		DEALLOCATE (p_get_stat_t->__ptrParameterValueStruct);
        DEALLOCATE(p_get_stat_t);
		/* SPR 21563_p1 FIXED END */
    }
	
    
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting From %s ",__FUNCTION__);
}
/* Spr 17463 Fix End */

/* SPR 21563 FIXED START*/
void Free_GPA_Request(struct _cwmp__GetParameterAttributes *l_getparamAttr_t)
{
    struct ParameterNames *ParameterNames;
    int index = 0;
    if (NULL != l_getparamAttr_t)
    {
        ParameterNames =  l_getparamAttr_t->ParameterNames;
        if (NULL == ParameterNames)
        {
            ENB_FREE(l_getparamAttr_t);
            return ;
        }
        if (ParameterNames->__size > 0)
        {
            if (NULL != ParameterNames->__ptrstring) 
            {
                for (index = 0; index < ParameterNames->__size; index++)
                {
                    if (NULL != ParameterNames->__ptrstring[index])
                    {
                        ENB_FREE(ParameterNames->__ptrstring[index]);
                        ParameterNames->__ptrstring[index] = NULL;
                    }
                }
                ENB_FREE(ParameterNames->__ptrstring);
                ParameterNames->__ptrstring = NULL;
            }
            ENB_FREE(ParameterNames);
            ParameterNames = NULL;
        }
        ENB_FREE(l_getparamAttr_t);
        /* Coverity 114571 Fixed Start */
        /* Code Removed */
        /* Coverity 114571 Fixed End */
    } 
}
void Free_SPA_Request(struct _cwmp__SetParameterAttributes *Setparamattr_t)
{
    int index = 0;
    struct SetParameterAttributesList *ParameterList = NULL;
    if (NULL != Setparamattr_t)
    {
        ParameterList = Setparamattr_t->ParameterList;
        /* Coverity 114572/114573 Fixed Start */
        if (ParameterList !=  NULL)
        {
            if (NULL != ParameterList->__ptrSetParameterAttributesStruct)
            {
                for (index = 0; index < ParameterList->__size; index++)
                {
                    if (NULL != ParameterList->__ptrSetParameterAttributesStruct[index])
                    {
                        if (NULL != ParameterList->__ptrSetParameterAttributesStruct[index]->Name)
                        {
                            ENB_FREE(ParameterList->__ptrSetParameterAttributesStruct[index]->Name);
                            ParameterList->__ptrSetParameterAttributesStruct[index]->Name = NULL;

                        } 

                        ENB_FREE(ParameterList->__ptrSetParameterAttributesStruct[index]);
                        /* SPR 21563_p1 FIXED START */
                        ParameterList->__ptrSetParameterAttributesStruct[index] = NULL;
                        /* SPR 21563_p1 FIXED END */
                    }

                }
                /* SPR 21563_p1 FIXED START */
                if (NULL != ParameterList->__ptrSetParameterAttributesStruct)
                {
                    ENB_FREE(ParameterList->__ptrSetParameterAttributesStruct);
                    ParameterList->__ptrSetParameterAttributesStruct = NULL;

                }


            }

            ENB_FREE(ParameterList);

        }
        ENB_FREE(Setparamattr_t);
        /* Code Removed */
        /* Coverity 114572/114573 Fixed End */
        /* SPR 21563_p1 FIXED END */
    } 
}
void Free_GPA_Parameters(sParameterList *p_get_stat_attr_t)
{
    int index = 0;
    if (NULL == p_get_stat_attr_t)
        return ;
    if (p_get_stat_attr_t->__size > 0)
    {
        for (index = 0; index < p_get_stat_attr_t->__size; index++)
        {
            if (NULL != p_get_stat_attr_t->__ptrParameterAttributeStruct)
            {
                if (NULL != p_get_stat_attr_t->__ptrParameterAttributeStruct[index])
                {
                    DEALLOCATE(p_get_stat_attr_t->__ptrParameterAttributeStruct[index]->Name);
                    DEALLOCATE(p_get_stat_attr_t->__ptrParameterAttributeStruct[index]->AccessList->__ptrstring);
                    DEALLOCATE(p_get_stat_attr_t->__ptrParameterAttributeStruct[index]->AccessList);
                    DEALLOCATE(p_get_stat_attr_t->__ptrParameterAttributeStruct[index]); 
                }
            }
        }
        DEALLOCATE(p_get_stat_attr_t->__ptrParameterAttributeStruct);
        DEALLOCATE(p_get_stat_attr_t);
    }
}
void Free_GetParameterAttribute(ParameterNames *newParamName)
{
    int index = 0;
    CHAR8 **arrGlueNames;
    arrGlueNames = newParamName->__ptrstring;
    if (NULL == arrGlueNames)
        return ;
    if (newParamName->__size > 0)
    {
        if (NULL != arrGlueNames)
        {
            for (index = 0; index < newParamName->__size; index++)
            {
                if (NULL != arrGlueNames[index])
                    DEALLOCATE(arrGlueNames[index]);
            }
            DEALLOCATE(arrGlueNames);
        }  
    }     
}
/* SPR 21563 FIXED END */
/* SPR 21563_p1 FIXED START */
void Free_Upload_Req(struct _cwmp__Upload *l_inp_t)
{
   if (NULL != l_inp_t)
   {
        if (NULL != l_inp_t->CommandKey)
        {
            ENB_FREE(l_inp_t->CommandKey);
            l_inp_t->CommandKey = NULL;
        }
        if (NULL != l_inp_t->FileType)
        {
            ENB_FREE(l_inp_t->FileType);
            l_inp_t->FileType = NULL;
        }
        if (NULL != l_inp_t->URL)
        {
            ENB_FREE(l_inp_t->URL);
            l_inp_t->URL = NULL;
        }
        if (NULL != l_inp_t->Username)
        {
            ENB_FREE(l_inp_t->Username);
            l_inp_t->Username = NULL;
        }
        if (NULL != l_inp_t->Password)
        {
            ENB_FREE(l_inp_t->Password);
            l_inp_t->Password = NULL;
        }

    } 
}

/* SPR 21563_p1 FIXED END  */
/* SPR 22314 Start */
void tr069_free_ssetparametervalues(sSetParameterValues *ptr)
{
   if(NULL == ptr)
   {
       return;
   }
   if(NULL != ptr->ParameterKey)
   {
       ENB_FREE(ptr->ParameterKey);
   }
   if(NULL != ptr->ParameterList)
   {
       TR069_FREE_PARAMMETERVALUELIST(ptr->ParameterList);
   }
   ENB_FREE(ptr);
   return;
}
/* SPR 22314 End */
