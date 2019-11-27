/********************************************************************************
*
*      FILE NAME   : http.c
*
*      DESCRIPTION : This file contains the function for sending http
*           	      message
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Aniruth Oblah    Aug 1, 2012   TR-069 Amendment 4   Initial Creation
*       Anant K Sharma   Mar 18,2013   Coding guideline changes and removed warnings
*       Anant K Sharma   Apr 08,2013   Added check for connectivity after tunnel establishment.
*       Vinod Vyas       Oct 2013      Ipv6 Related Changes
*       Avinash          Jul 2014      Fix for SPR 8235
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/
	
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/lhash.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
	
#include "http.h"

//IPSec SSI code starts	
#ifdef HAVE_IPSEC
#include<ipr_cwmp_ping_util.h>
#include<ipr_cwmp_read_config.h>
#include<ipr_cwmp_enb_wrapper.h>
#include<ManagementServerModel.h>
#include<ipr_cwmp_sig_handler.h>
#endif
//IPSec SSI code ends

//config file changes moved from tr69_driver.c for lib delivery
#include<ipr_cwmp_read_config.h>
/*MUTUAL AUTHENTICATION CHANGES*/
#define TLS_CERT		"ruckusCert.pem"
#define TLS_KEY			"privkey.pem"
/*SPR 18835 fix start */
UInt8 cert_file_path_shems[CONFIG_PARAM_LEN] = {0};
UInt8 cert_file_path_ihems[CONFIG_PARAM_LEN] = {0};
/*SPR 18835 fix end */
/*SPR 19088 FIXED START*/
/* code removed */
/*SPR 19088 FIXED END */
UInt8 tlscertpath[CONFIG_PARAM_LEN] = {'\0'};
/* SPR 22314 Start*/
void free_tokens(char **tokens , int c);
/* SPR 22314 End*/
/*TLS CURL CHANGES*/
extern UInt8 iHems_session;
/*SPR 18835 fix start */
UInt8 certificate_path[CONFIG_PARAM_LEN] = {0};
extern UInt8 shems_Session;
/* SPR 18835 fix end */
extern UInt8 g_hems_fqdn[128];
extern UInt8  iHems_url[256];
extern UInt32 g_is_fqdn;
void create_tls_certpath(void);
void  create_resolve_command(UInt8*,UInt8*,UInt8**);
/*global variable for ssl pass pharse*/
static UInt8 g_ipr_cwmp_pPassphrase[64] = {0};
	
/*global variable for ssl key type*/
static UInt8 g_ipr_cwmp_pKeyType[64] = {0};
	
/*global variable for ssl key name*/
static UInt8 g_ipr_cwmp_pKeyName[64] = {0};
	
/*global variable for ssl ca certificate file*/
static UInt8 g_ipr_cwmp_pCACertFile [64] = {0};
static UInt8 g_ipr_cwmp_pClientKeyFile[256] = {0};
static UInt8 g_ipr_cwmp_pClientCertFile[256] = {0};

/* SPR 20618 FIXED START */
char * tr069_cooki(char *string);
int check_dupid(char **tokens, char token[], int i, int num_tok);
char last_cooki[1024] = {0};
/* SPR 20618 FIXED END */

/*SPR 19088 FIXED START*/
extern UInt8 HeNB_SELF_IP[40];

/*SPR 19088 FIXED END*/

/*This function is to get ssl extensions from ca certificate
static UInt8* ipr_cwmp_my_get_ext(X509 * cert, const UInt32 type, UInt32 extensiontype);
commented this function as its not being used as of now*/
/*This function is to set ssl context*/
static CURLcode ipr_cwmp_sslctxfun(void * sslctx, void * parm) ;
/*SPR 17929 change start*/	
struct MemoryStruct header_chunk = {0};
/*SPR 17929 change end*/	
/********************************************************************************
*	Function Name : ipr_cwmp_WriteMemoryCallback 
*
*	Description   : 
*		
*
*	Returns       : 
*
*******************************************************************************/
	
size_t ipr_cwmp_WriteMemoryCallback(void *contents, 
	size_t size, 
	size_t nmemb, 
	void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	mem->memory = TR_REALLOC(mem->memory, mem->size + realsize + 1 );


	TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Entering in %s ",__FUNCTION__);
	if (mem->memory == PNULL) 
	{
	    /* out of memory! */ 
    	TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
    	exit(EXIT_FAILURE);
	}
	ENB_MEMCPY(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
	return realsize;
}
	
/********************************************************************************
*	Function Name : ipr_cwmp_send_http_msg
*
*	Description   : This function is used to send the SOAP message to the 
*			ACS and receive the response from ACS.
*
*	Returns       : status 
*
******************************************************************************/

    CURL *curl = PNULL;
    struct curl_slist *slist = NULL;
	
UInt32 ipr_cwmp_send_http_msg(UInt8 *msg,  // Message that has to be sent
	UInt8 *URL, // URL the message has to be sent to
	UInt8 **response_message, // Message received from ACS
	UInt8 *username, //username
	UInt8 *password) //password
{
    /*SPR 16521 Fix Start */
    static char pCACertFile[256] = {'\0'};
    //    const char *pKeyType;
    /* SPR 20618 FIXED START */
    char *cooki = NULL;
    /* SPR 20618 FIXED END */
    char pKeyName[256] = {'\0'};
    strncpy((char*)pCACertFile,(char*)tlscertpath,256);
    strcat(pCACertFile,TLS_CERT);
    pCACertFile[255] = '\0';
#ifdef USE_ENGINE
    strncpy((char*)pKeyName,tlscertpath,256);
    strcat(pKeyName,TLS_KEY);
    pKeyName[255]='\0';
#else
    strncpy((char*)pKeyName,(char*)tlscertpath,256);
    strcat(pKeyName,TLS_KEY);
    pKeyName[255]='\0';
#endif
    /*SPR 16521 Fix End*/

    UInt8 *eurl = PNULL;
    struct MemoryStruct chunk = {0}; 
    /*SPR 17929 changes start*/
    char arr[512];
    memset(arr, '\0', 512);
    /*SPR 17929 changes end*/

    chunk.memory = PNULL;  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this poUInt32 */ 
    TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Entering in %s \n\n Sending MSG => %s",__FUNCTION__, msg);

    //IPSEC SSI code starts
#ifdef HAVE_IPSEC

    config_res_t    *p_config_res = NULL;
    UInt32          isTunnelError = 0;
    FILE*           file = NULL;

    p_config_res = (config_res_t*)ALLOCATE(sizeof(config_res_t));
    file = fopen(TRO69_IP_SAVE, "r");
    if(file == NULL)
    {
        TRACE(LOG_CRITICAL , HTTP_CONNECTION, "Error opening file to restore saved IPs \n");
        DEALLOCATE(p_config_res);
        return;
    }
    else
    {
        if(fread(p_config_res, sizeof(config_res_t), 1, file) > 0 )
            fclose(file);
    }
    TRACE (LOG_CRITICAL , HTTP_CONNECTION, "\n >>>>>>>>>>have_ipsec = %d \n", have_ipsec);
    TRACE (LOG_CRITICAL , HTTP_CONNECTION, "\n >>>>>>>>>>have_tunnel = %d \n", have_tunnel);
    if(!have_tunnel && have_ipsec && strlen(p_config_res->HeNBIPAddr) != 0 && (strlen(p_config_res->SecGWServer1) != 0 || strlen(p_config_res->SecGWServer2) != 0 || strlen(p_config_res->SecGWServer3) != 0))
    {
        if(strlen(p_config_res->SecGWServer1) != 0)
        {
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer1, (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "failed to add state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer1, (UInt8*)p_config_res->HeNBIPAddr, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "failed to add state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer1);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->SecGWServer1, p_config_res->HeNBIPAddr);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");

            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr, p_config_res->SecGWServer1, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "failed to add in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer1, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "failed to add out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer1);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                }
            }

        }//end of SECGW1
        else
            isTunnelError = 1;//set this to try other 2 GWs if any
        if(isTunnelError && strlen(p_config_res->SecGWServer2) != 0 )
        {
            isTunnelError = 0;//reset this before trying other GW
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer2, (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "failed to add state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer2, (UInt8*)p_config_res->HeNBIPAddr, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer2);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");

            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr, p_config_res->SecGWServer2, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
            /*if (ipr_cwmp_policy_add_fwd((UInt8*)p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr, p_config_res->SecGWServer2, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
              TRACE(LOG_CRITICAL , HTTP_CONNECTION, "add fwd policy src = %s and  dst = %s failed", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
              else
              TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added fwd policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
              */
            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer2, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer2);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                    isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
                }
            }

        }//end of SECGW2
        if(isTunnelError && strlen(p_config_res->SecGWServer3) != 0 )
        {
            isTunnelError = 0;//reset this before trying other GW
            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer3, (g_crypto_profile.enable)?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");
            }

            if (ipr_cwmp_add_state_3des((UInt8*)p_config_res->SecGWServer3, (UInt8*)p_config_res->HeNBIPAddr, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                isTunnelError = 1;
            }
            else
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added state src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->SecGWServer3);
                g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0;
                setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
                //signal(SIGALRM, sig_hdl);
                if (sigaction(SIGALRM, &g_act, NULL) < 0)
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "sigaction failed");

            }

            if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr, p_config_res->SecGWServer3, p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                isTunnelError = 1;
            }
            else if(!isTunnelError)
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added in policy src = %s and  dst = %s ", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
            if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->SecGWServer3, ipsec_net_prefix, ipsec_net_prefix))
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                isTunnelError = 1;
            }
            else if(!isTunnelError)
            {
                TRACE(LOG_CRITICAL, HTTP_CONNECTION, "added out policy src = %s and  dst = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr );
                if(!ping(p_config_res->HeMSIPAddr))
                {
                    TRACE(LOG_CRITICAL, HTTP_CONNECTION, "Tunnel is esatblished and HeMS %s is pingable", p_config_res->HeMSIPAddr );
                }
                else
                {
                    // delete the config and set the tunnel error flag

                    //delete the security assiciations
                    ipr_cwmp_del_state((UInt8*)p_config_res->SecGWServer3);
                    ipr_cwmp_del_state((UInt8*)p_config_res->HeNBIPAddr);

                    //call del policy for HeMS
                    if (ipr_cwmp_policy_out_del((UInt8*)p_config_res->HeNBIPAddr, (UInt8*)p_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del out policy failed for src = %s and dest = %s ", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted out policy for src = %s and dest = %s", p_config_res->HeNBIPAddr, p_config_res->HeMSIPAddr);
                    if (ipr_cwmp_policy_in_del((UInt8*)p_config_res->HeMSIPAddr, (UInt8*)p_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"del in policy failed for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);
                    else
                        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"deleted in policy for src = %s and dest = %s", p_config_res->HeMSIPAddr, p_config_res->HeNBIPAddr);

                }
            }

        }//end of SECGW3
    }//end of have_tunnel

    //send tunnel error message to OAM if any
    if (isTunnelError)

        TRACE (LOG_CRITICAL , HTTP_CONNECTION, "skipping sending tunnel error in http.c");
    else
        have_tunnel = 1;

    DEALLOCATE(p_config_res); 
#endif
    //IPSEC SSI code ends
    /*TLS CURL CHANGES*/
    struct curl_slist *host = NULL;
    if(g_is_fqdn == 0 && iHems_session == 1)
    {
        /* BUG 537 fixed Start*/
        TRACE (LOG_CRITICAL , HTTP_CONNECTION, "SECURITY DEBUG:global fqdn not null\n");
        UInt8* resolve_command = NULL;
        resolve_command = (UInt8*)ALLOCATE(256);
        if(resolve_command == NULL)
        {    
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Malloc faliure in function  %s",__FUNCTION__);

            return 0;
        }

        /*coverity 96624 fixed start*/
        TRACE (LOG_CRITICAL , HTTP_CONNECTION, "SECURITY DEBUG: ip address passed is:[%s] and fqdn passed to resolve command [%s]\n",URL,g_hems_fqdn);

        if(strlen((char*)g_hems_fqdn) > 1)
        {
            /* coverity 106976 fixed start*/
            if(0 != (ENB_STRNCMP((char*)URL,(char*)g_hems_fqdn,strlen((char *)URL))))
                /* coverity 106976 fixed end*/
                create_resolve_command(URL,g_hems_fqdn,&resolve_command);  
            host = curl_slist_append(NULL,(char *)resolve_command); 
        }

        /*coverity 96624 fixed end*/
        /*coverity 97092 fixed start*/ 
        DEALLOCATE(resolve_command); 
        /*coverity 97092 fixed end*/ 
    }
    /* BUG 537 fixed End*/

    if (!curl)
        curl = curl_easy_init();

    if(curl) 
    {
        /*TLS CURL CHANGES*/
        curl_easy_setopt(curl,CURLOPT_RESOLVE, host);
        TRACE (LOG_CRITICAL , HTTP_CONNECTION, "SECURITY DEBUG:CURLOPT_RESOLVE set\n");
        TRACE(LOG_CRITICAL , HTTP_CONNECTION,"CURL DEBUG: url before :%s\n",URL); 
        curl_easy_setopt(curl, CURLOPT_URL,(char *)URL);
        //UInt8 secure_check = '\0';
        //secure_check = URL[4];
        /*SPR 19088 FIXED START*/

        if(ENB_STRLEN((char*)HeNB_SELF_IP)>0)
        { 
            TRACE(LOG_DETAIL , HTTP_CONNECTION,"INTERFACE IP FOR LOGGING OUTGOING ACS TRAFFIC = %s",(char*)HeNB_SELF_IP);
            /* COVERITY 108697/108698 FIX START */	
            curl_easy_setopt(curl, CURLOPT_INTERFACE, HeNB_SELF_IP);
            /* COVERITY 108697/108698 FIX END */	

        }
        /*SPR 19088 FIXED END*/

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        UInt8 secure_check = '\0';
        secure_check = URL[4];

        /*SPR 16521 Fix Start */
        if('s' == secure_check)
        {
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "IN IPR FIX SPR 16521\n");
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "MUTUAL AUTHENTICATION DEBUG:clientkey = %s & clientcert = %s\n",g_ipr_cwmp_pClientKeyFile,g_ipr_cwmp_pClientCertFile);
            /* if we use a key stored in a crypto engine, we must set the key type to "ENG" */
            /* set the private key (file or ID in engine) */
            curl_easy_setopt(curl, CURLOPT_SSLKEY, g_ipr_cwmp_pClientKeyFile);
            /* set the file with the certs vaildating the server */
            curl_easy_setopt(curl, CURLOPT_SSLCERT, g_ipr_cwmp_pClientCertFile);
            /* disconnect if we can't validate server's cert */
            curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,1L);
            /*SPR 16521 Fix End*/
        }
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        /* set the file with the certs vaildating the server */
        curl_easy_setopt(curl,CURLOPT_CAINFO,g_ipr_cwmp_pCACertFile);
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5L);
        curl_easy_setopt(curl,CURLOPT_POSTREDIR,CURL_REDIR_POST_ALL);
        if( PNULL == msg ||
                0 == ENB_STRLEN((char*)msg))
        {
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
            return ipr_cwmp_send_empty_http_msg(msg, URL, response_message, username, password);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_POST, 1);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS,(char *)msg);
            /* SPR 21563_p1 FIXED START */
            /* Code removed */
            /* SPR 21563_p1 FIXED END */
          
            slist = curl_slist_append(PNULL, "Content-Type: text/xml; UInt8set=utf-8\nSOAPAction:");
            /*SPR 17929 changes start*/
            if (header_chunk.size)
            {

                /*SPR 19601 FIXED START */
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "debug %s", header_chunk.memory);
                TRACE(LOG_CRITICAL, "HTTP_CONNECTION", "cooki= %s",cooki);
                /* SPR 20618 FIXED START */
                if (NULL != header_chunk.memory)
                {
                    cooki = tr069_cooki((char *)header_chunk.memory);
                    /*Coverity 111580 Fix Start*/
                    if (NULL != cooki && (BUFF_MAX_LEN >= ENB_STRLEN(cooki)))
                        ENB_STRNCPY(last_cooki, cooki ,ENB_STRLEN(cooki));
                    /*Coverity 111580 Fix End*/

                }
                //str = tr069_strrstr_fun((char *)header_chunk.memory,(char *) "JSESSIONID=", header_chunk.size);
                /*SPR 19601 FIXED END */
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Extracted JSESSIONID cooki = %s", cooki);
            }

            if ((header_chunk.size) && (cooki != NULL))
            {
                //token = strtok(str + 11, ";");
                memset(arr,0x00,512);
                sprintf(arr, "%s",cooki);
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Token has a value, %s", arr);
                slist = curl_slist_append(slist, arr);
                //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, " Added httpHeader\n");
            }
            //sending last cookie if corrent is NULL
            if (cooki == NULL && last_cooki[0] != '\0')
            {
                slist = curl_slist_append(slist, last_cooki);
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, "last cooki %s\n", last_cooki);
            }
            /* SPR 20618 FIXED END */
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, " Added httpHeader\n");
            /* send all data to this function  */
            /*SPR 17929 changes end*/
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ipr_cwmp_WriteMemoryCallback);
            /* we pass our 'chunk' struct to the callback function */
            //coverity fix
            curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&chunk);
            /*SPR 17929 change start*/
            if(0 == header_chunk.size)
            {

                /*SPR 19601 FIXED START */
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, ipr_cwmp_WriteMemoryCallback);

                /* SPR 19684 FIXED START*/ 
                /* SPR 20618 FIXED START */
                //curl_easy_setopt(curl, CURLOPT_COOKIE,(void *)&header_chunk);
                //curl_easy_setopt(curl, CURLOPT_COOKIELIST,(void *)&header_chunk);
                /* SPR 20618 FIXED END */
                /* SPR 19684 FIXED END */ 

                curl_easy_setopt(curl, CURLOPT_HEADERDATA,(void *)&header_chunk);
                /*SPR 19601 FIXED END */

            }
            /*SPR 17929 change end*/
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST);	
            if( (PNULL != username) &&
                    (PNULL != password) )
            {
                //coverity fix
                curl_easy_setopt (curl, CURLOPT_USERNAME ,(char *)username);
                curl_easy_setopt (curl,  CURLOPT_PASSWORD ,(char *)password);
            }
            curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, ipr_cwmp_sslctxfun)  ;
            /*SPR 18830 FIXED START */ 

            /* URL hanged state Fixed START */
            curl_easy_setopt(curl,CURLOPT_TIMEOUT,120);
            /* URL hanged state Fixed END */
            /*SPR 18830 FIXED END */ 

            CURLcode  code = curl_easy_perform(curl); 
            /* SPR 16451 Fix Start */
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "CURLcode is %d\n",code);
            /* SPR 16451 Fix End */
            curl_easy_getinfo (curl, CURLINFO_EFFECTIVE_URL ,&eurl);
            UInt8 domain_or_ip;
            UInt8 dummy_url1[257] = {'\0'};
            UInt8 dummy_url2[257] = {'\0'};
            /*SPR 22314 Start */
            curl_slist_free_all(slist);     
            curl_slist_free_all(host);    
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);
            curl_easy_setopt(curl,CURLOPT_RESOLVE, NULL);
            DEALLOCATE(cooki);
            /*SPR 22314 End */
            UInt8 *temp = (UInt8 *)ipr_cwmp_find_host(URL, &domain_or_ip, dummy_url1, dummy_url2);
            if(temp == NULL)
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in ipr cwmp find host \n");
                return 0;
            }
            UInt8 * iptemp = (UInt8 *)ALLOCATE(257);
            if(iptemp == NULL)
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in Memmory \n");

                return 0;
            }

            ENB_STRNCPY((char*)iptemp,(char*)temp, URL_LEN -1);
            UInt8 *tempeurl = (UInt8 *)ipr_cwmp_find_host(eurl, &domain_or_ip, dummy_url1, dummy_url2);
            if(tempeurl == NULL)
            {
                TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in ipr cwmp find host \n");
                DEALLOCATE(iptemp);
                DEALLOCATE(temp);

                return 0;
            }
            UInt32 redir=0;
            if(ENB_STRCMP((char*)iptemp , (char*)tempeurl) != 0)
            {
                ipr_cwmp_set_session_acs_host_name(tempeurl);
                redir =1;
            }
            DEALLOCATE(iptemp);
            ENB_STRNCPY((char*)URL, (char*)eurl, URL_LEN);

            /* Freeing the link list after its operation are done*/
            /* SPR 22314 Start */
            /* Code Removed */
            /* SPR 22314 End */
            /* MEMORY LEAK 5 JAN FIXED START*/
            DEALLOCATE(tempeurl);
            DEALLOCATE(temp);

            /* MEMORY LEAK 5 JAN FIXED END*/

            /* always cleanup */ 

            if(code!=0)
            {
                /* SPR 16451 Fix Start */
                switch(code)
                {
                    case CURLE_SSL_CACERT :
                        {
                            /* SPR 19124 FIXED START*/
                            TRACE (LOG_DETAIL , HTTP_CONNECTION, "CURLcode is %d\n",code);
                            TRACE (LOG_DETAIL , HTTP_CONNECTION, "Error : %s ", curl_easy_strerror(code));
                            /* SPR 19124 FIXED END*/
                            *response_message = PNULL;
                            /* SPR 16547 Fix Start */
                            return code;
                            /* SPR 16547 Fix End */
                        }
                        /* SPR 16580 Fix Start */
                        /* SPR 20048 FIXED START */
                    case CURLE_HTTP_RETURNED_ERROR :
                    case CURLE_SSL_CACERT_BADFILE :
                        {
                            /* SPR 20048 FIXED END */
                            /*SPR 19124 FIXED START*/
                            TRACE (LOG_DETAIL , HTTP_CONNECTION, "CURLcode is %d\n",code);
                            TRACE (LOG_DETAIL , HTTP_CONNECTION, "Error : %s ", curl_easy_strerror(code));
                            /*SPR 19124 FIXED END*/
                            *response_message = PNULL;
                            return code;
                        }
                        /* SPR 16580 Fix End */

               /* SPR 21547 FIXED START*/     
               case CURLE_SEND_ERROR :
                  {
                      TRACE (LOG_DETAIL , HTTP_CONNECTION, "CURLcode is %d\n",code);
                      TRACE (LOG_DETAIL , HTTP_CONNECTION, "Error : %s ", curl_easy_strerror(code));
                      *response_message = PNULL;
                      return code;
                  }
               /* SPR 21547 FIXED END*/     
                    default :
                        {
                            if(redir != 0)
                            {
                                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Dinesh 10 ipr_cwmp_send_http_msg\n");
                                return  ipr_cwmp_send_http_msg(msg, URL, response_message,username, password);
                            }
                            UInt8 * new_url = (UInt8 *)ipr_cwmp_acs_discovery('u');
                            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Got new URL %s ", new_url);
                            if ( PNULL == new_url )
                            {
                                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                                /* SPR 20703 FIXED START */
                                return code;
                                /* SPR 20703 FIXED END */
                            }
                            else
                            {
                                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                                ENB_STRNCPY((char*)URL, (char*)new_url, URL_LEN);
                                /* SPR 16327 Fixed Start */
                                /* SPR 20703 FIXED START */
                                return code;
                                /* SPR 20703 FIXED END */
                                /* SPR 16327 Fixed End */

                            }
                        }
                }
                /* SPR 16451 Fix End */
            }
            if(chunk.size == 0)
            {
                *response_message = PNULL;
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);

                return EMPTY;
            }
            *response_message = chunk.memory;
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
        }
    }
    return SUCCESS;
}
    	
/********************************************************************************
*       Function Name : ipr_cwmp_send_empty_http_msg
*
*       Description   : This function is used to send the empty message to the
*                       ACS and receive the response from ACS.
*
*       Returns       : status
*
******************************************************************************/
	
UInt32 ipr_cwmp_send_empty_http_msg(UInt8 *msg, // message that si to be sent
	UInt8 *URL, // Destination URL
	UInt8 **response_message, // Response message will be stored here
	UInt8 *username, //username to connect to ACS
	UInt8 *password) //password to connect to ACS
{
    //CURL *curl = PNULL;
    CURLcode res = CURLE_OK;
    /* SPR 20618 FIXED START */
    char *cooki = NULL;
    /* SPR 20618 FIXED END */
    UInt8 *eurl = PNULL;
    struct MemoryStruct chunk = {0}; 
    //curl = curl_easy_init();
    chunk.memory = PNULL;
    chunk.size = 0;    /* no data at this poUInt32 */ 
    /*SPR 17929 changes start*/
    char arr[512];
    memset(arr, '\0', 512);
    /*SPR 17929 changes end*/

    TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Entering in %s ",__FUNCTION__);
    if(curl) 
    {

        //curl_slist_free_all(slist);
        struct curl_httppost *formpost=PNULL;
        //coverity fix 
        //url **
        curl_easy_setopt(curl, CURLOPT_URL, (char *)URL);
        /*SPR 19088 FIXED START*/
        if(ENB_STRLEN((char*)HeNB_SELF_IP)>0)
        { 
            TRACE(LOG_DETAIL , HTTP_CONNECTION,"INTERFACE IP FOR LOGGING OUTGOING ACS TRAFFIC = %s",(char*)HeNB_SELF_IP);
            /* COVERITY 108697/108698 FIX START */	
            curl_easy_setopt(curl, CURLOPT_INTERFACE ,HeNB_SELF_IP);
            /* COVERITY 108697/108698 FIX END */	

        }
        /*SPR 19088 FIXED END*/

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);
        /*SPR 17929 changes start*/
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");
        if (header_chunk.size)
        {
            /* SPR 21563_p1 FIXED START */
            /* Code removed */
            /* SPR 21563_p1 FIXED END */
            slist = curl_slist_append(PNULL, "Content-Type: text/xml; UInt8set=utf-8\nSOAPAction:");

            /*SPR 19601 FIXED START */
            /* SPR 20618 FIXED START */
            if (NULL != header_chunk.memory)
            {
                cooki = tr069_cooki((char *)header_chunk.memory);
                /*Coverity 111578 Fix Start*/
                if (NULL != cooki &&  (1024>=ENB_STRLEN(cooki)))
                    ENB_STRNCPY(last_cooki, cooki ,ENB_STRLEN(cooki));
                /*Coverity 111578 Fix End*/
            }
            //str = tr069_strrstr_fun((char *)header_chunk.memory, (char *)"JSESSIONID=", header_chunk.size);

            /*SPR 19601 FIXED END */
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Extracted JSESSIONIDcooki=%s", cooki);
        }
        //TRACE(LOG_CRITICAL , HTTP_CONNECTION, " Session id = %s", str + 8);/* Ruckus */

        if ((header_chunk.size) && (cooki != NULL))
        {
            //token = strtok(str + 11, ";");
            memset(arr,0x00,512);
            sprintf(arr, "%s",cooki);
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Token has a value, %s", arr);
            strcpy(last_cooki, arr);
            slist = curl_slist_append(slist, arr);
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, " debug 2 Added httpHeader\n");
        }
        if (cooki == NULL && last_cooki[0] != '\0')
        {
            slist = curl_slist_append(slist, last_cooki);
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, "last cooki %s\n", last_cooki);

        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        /* SPR 20618 FIXED END */
        /* send all data to this function  */
        /*SPR 17929 changes end*/
        curl_easy_setopt (curl,CURLOPT_USERNAME, "");	
        //UInt8 secure_check;
        //secure_check = URL[4];
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#ifdef SKIP_PEER_HOST_VERIFICATION
        UInt8 secure_check;
        secure_check = URL[4];
        if('s' == secure_check)
        {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        }
#endif
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl,CURLOPT_SSLKEYTYPE,g_ipr_cwmp_pKeyType);

        /* set the private key (file or ID in engine) */
        curl_easy_setopt(curl,CURLOPT_SSLKEY,g_ipr_cwmp_pKeyName);

        /* set the file with the certs vaildating the server */
        curl_easy_setopt(curl,CURLOPT_CAINFO,g_ipr_cwmp_pCACertFile);
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5L);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        /* send all data to this function  */

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ipr_cwmp_WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        //coverity fix
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&chunk);
        /*SPR 17929 changes start*/
        if(0 == header_chunk.size)
        {
            /*SPR 19601 FIXED START */
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, ipr_cwmp_WriteMemoryCallback);
            /* SPR 19684 FIXED START*/ 
            /* SPR 20618 FIXED START */
            //curl_easy_setopt(curl, CURLOPT_COOKIE,(void *)&header_chunk);
            //curl_easy_setopt(curl, CURLOPT_COOKIELIST,(void *)&header_chunk);
            /* SPR 20618 FIXED END */
            /* SPR 19684 FIXED END */ 
            curl_easy_setopt(curl, CURLOPT_HEADERDATA,(void *)&header_chunk);
        }
        /*SPR 17929 changes end*/

        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST);

        if( (PNULL != username) &&
                (PNULL != password) )
        {
            curl_easy_setopt (curl, CURLOPT_USERNAME , (char *)username);
            curl_easy_setopt (curl,  CURLOPT_PASSWORD , (char *)password);
        }
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, ipr_cwmp_sslctxfun)  ;

        /*SPR 18830 FIXED START */ 
        /* URL hanged state Fixed START */
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,120);
        /* URL hanged state Fixed END */
        /*SPR 18830 FIXED END */ 

        res = curl_easy_perform(curl);
        /* SPR 22314 Start */
        DEALLOCATE(cooki);
        /* SPR 22314 End */
        curl_easy_getinfo (curl, CURLINFO_EFFECTIVE_URL ,&eurl);
        UInt8 domain_or_ip = '\0';
        UInt8 dummy_url1[257] = {'\0'};
        UInt8 dummy_url2[257] = {'\0'};
        UInt8 *temp = (UInt8 *)ipr_cwmp_find_host(URL, &domain_or_ip, dummy_url1, dummy_url2);
        if(temp == NULL)
        {
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in ipr cwmp find host \n");

            return 0;
        }
        UInt8 * iptemp = (UInt8 *)ALLOCATE(257);
        if(iptemp == NULL)
        {
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in Memmory \n");
            return 0;
        }
        ENB_STRNCPY((char*)iptemp, (char*)temp, URL_LEN - 1);
        UInt8 *tempeurl = (UInt8 *)ipr_cwmp_find_host(eurl, &domain_or_ip, dummy_url1, dummy_url2);
        if(tempeurl == NULL)
        {
            TRACE(LOG_CRITICAL , HTTP_CONNECTION, " HTTP : error in ipr cwmp find host \n");
            DEALLOCATE(iptemp);
            DEALLOCATE(temp);

            return 0;
        }
        UInt32 redir=0;
        if(ENB_STRCMP((char*)temp , (char*)tempeurl) != 0)
        {
            ipr_cwmp_set_session_acs_host_name(tempeurl);
            redir =1;
        }
        DEALLOCATE(iptemp);
        ENB_STRNCPY((char*)URL, (char*)eurl, URL_LEN);

        /* MEMORY LEAK 31 DEC FIXED START*/
        DEALLOCATE(tempeurl);
        DEALLOCATE(temp);

        /* MEMORY LEAK 31 DEC FIXED END*/
        /* always cleanup */ 

		/* SPR 21563_p1 FIXED START */
        /* SPR 22314 Start */
        curl_slist_free_all(slist);     
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);
        /* SPR 22314 End */
		/* SPR 21563_p1 FIXED END */

        if( 0 != res)
        {
            /* MEMORY LEAK 31 DEC FIXED START*/
            if(redir != 0)
            {
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                /*SPR 22314 Start */ 
                DEALLOCATE(cooki);
                /*SPR 22314 End */ 
                return  ipr_cwmp_send_empty_http_msg(msg, URL, response_message,username, password);
            }
            UInt8 * new_url = (UInt8 *)ipr_cwmp_acs_discovery('u');
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Got new URL %s ", new_url);
            if ( PNULL == new_url )
            {
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                return (res + 2);
            }
            else
            {
                TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
                ENB_STRNCPY((char*)URL, (char*)new_url, URL_LEN);
                return  ipr_cwmp_send_empty_http_msg(msg, URL, response_message,username, password);
            }

            /* MEMORY LEAK 31 DEC FIXED END*/
        }
        *response_message = chunk.memory;
        /*SPR 17929 changes start*/
		/* SPR 21563_p1 FIXED START */
		/* Code removed */
        /* SPR 21563_p1 FIXED END */
        /*Coverity 111582 Fix Start*/
        /*code removed*/
        /*Coverity 111582 Fix End*/
        /* SPR 20618 FIXED END */
        /*SPR 17929 changes end*/
        if ( (chunk.size == 0) ) 
        {
            *response_message = PNULL;
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
        }
        else
        {
            TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Exiting in %s ",__FUNCTION__);
        }
    }
    return SUCCESS;
}

/*******************************************************************************************************************
*   Function Name : ipr_cwmp_Init_SSL_param
*
*   Description   : This function is for initializing parameters required by ssl lib
*
*   Returns       : nothing
*
*********************************************************************************************************************/
	
void ipr_cwmp_Init_SSL_param(void)
{
	//OAM SI code changes UInt8 * cert_file_path = PNULL;
	FILE *fd = PNULL;
	UInt8 *str = PNULL;
	UInt8 *str1 = PNULL;
	UInt8 tmpBuffer[2048] = {'\0'};
	memset( tmpBuffer , 0 , 2048);
	int bytesRead = 0;
	int flag= 0;

	//OAM SI code change cert_file_path = (UInt8 *)getenv(CERT_PATH);
    /* SPR 18835 fix start */



    if(shems_Session)
    {
        TRACE(LOG_INFO , HTTP_CONNECTION ,"connection with shems is established ");
        ENB_STRNCPY((char *)certificate_path ,(char *)cert_file_path_shems , strlen((char *)cert_file_path_shems));    
    }
    else if(iHems_session)
    {
        TRACE(LOG_INFO , HTTP_CONNECTION ,"connection with ihems is established ");
        ENB_STRNCPY((char *)certificate_path ,(char *)cert_file_path_ihems , strlen((char *)cert_file_path_ihems));    
    }

    TRACE(LOG_DETAIL, HTTP_CONNECTION ," length of certificate_path = %d" , strlen((char *)certificate_path));
    TRACE(LOG_DETAIL, HTTP_CONNECTION ," certificate_path = %s" , (char *)certificate_path);
	if (ENB_STRLEN((char*)(certificate_path)) > 0)
	{
		/*MUTUAL AUTHENTICATION CHANGES*/
		create_tls_certpath();	
		fd = ENB_FOPEN((char*)(certificate_path) , "r");
		if (fd)
		{
			TRACE (LOG_DETAIL , HTTP_CONNECTION, "In fun %s  certificate file %s opened ", __FUNCTION__ , (char *)certificate_path);
	       /* SPR 18835 fix end */
			bytesRead =  ENB_FREAD((void *)tmpBuffer,1024,2, fd);
			TRACE (LOG_CRITICAL , HTTP_CONNECTION, "in :%s  bytes read %d from certpath file  ", __FUNCTION__ , bytesRead);
			if(feof(fd))		
			{
				TRACE (LOG_CRITICAL , HTTP_CONNECTION, "EOF Reched: in feof  In  :%s", __FUNCTION__);
				tmpBuffer[bytesRead - 1] = '\0';
                flag = 1;
            } 
            else if(0 == bytesRead)
			{
				TRACE (LOG_CRITICAL , HTTP_CONNECTION, "ERROR : in fread  in  :%s", __FUNCTION__);
				ENB_FCLOSE(fd);
				return;  
			}
            else
			{
				TRACE (LOG_CRITICAL , HTTP_CONNECTION, "In  :%s  , fread success", __FUNCTION__);
				tmpBuffer[bytesRead - 1] = '\0';
		        flag = 1;
			}


		}
        else
        {
    	/* SPR 18835 fix start */
			TRACE (LOG_CRITICAL , HTTP_CONNECTION, "ERROR : In function : %s, error in certificate file opening on path :%s , errno = %d", __FUNCTION__ , (char *)certificate_path , errno);
  	    /* SPR 18835 fix end */
            return;
		}
	}
	str = (UInt8*)ENB_STRTOK((char*)tmpBuffer , "\n");
	while (str != (UInt8*)PNULL)
	{
		str1 = (UInt8*) ENB_STRCHR((char*)str, '=');
		if (str1 != PNULL)
		{
			if ( ENB_STRSTR((char*)str , "Passphrase") != PNULL)
			{
				str1 = str1+1;
				if (PNULL != str1)
				{
					ENB_STRNCPY((char*)g_ipr_cwmp_pPassphrase, (char*)str1, sizeof(*str1));
				}
			}
			else if ( ENB_STRSTR((char*)str , "KeyType") != PNULL)
			{
				str1 = str1+1;
				if (PNULL != str1)
				{
					ENB_STRNCPY((char*)g_ipr_cwmp_pKeyType, (char*)str1, sizeof(*str1));
				}
			}
			else if ( ENB_STRSTR((char*)str , "KeyName") != PNULL)
			{
				str1 = str1+1;
				if (PNULL != str1)
				{
					tr_strncpy((char*)g_ipr_cwmp_pKeyName , (char*)str1);
				}
			}
			else if ( ENB_STRSTR((char*)str , "CACertFile") != PNULL)
			{
				str1 = str1+1;
				if (PNULL != str1)
				{
					tr_strncpy((char*)g_ipr_cwmp_pCACertFile , (char*)str1);
				}
			}
			/*MUTUAL AUTHENTICATION CHANGES*/
			else if ( ENB_STRSTR((char*)str , "ClientKey") != PNULL)
			{
				TRACE (LOG_CRITICAL , HTTP_CONNECTION,"MUTUAL AUTHENTICATION DEBUG:fill ClientKeyName\n");
				str1 = str1+1;
				if (PNULL != str1)
				{
					tr_strncpy((char*)g_ipr_cwmp_pClientKeyFile , (char*)str1);
				}
			}
			else if ( ENB_STRSTR((char*)str , "ClientCertName") != PNULL)
            {
				TRACE (LOG_CRITICAL , HTTP_CONNECTION,"MUTUAL AUTHENTICATION DEBUG:fill ClientCertName\n");
                str1 = str1+1;
                if (PNULL != str1)
                {
                    tr_strncpy((char*)g_ipr_cwmp_pClientCertFile , (char*)str1);
                }
            }
		}
		str = (UInt8*)ENB_STRTOK((char*)PNULL , "\n");
	}
    if(flag)
	ENB_FCLOSE(fd);
}


/*************************************************************************************************
*   Function Name : ipr_cwmp_ssl_app_verify_callback
*
*   Description   : This function is a call back for X509_verify_cert that attempts to discover 
*                   and validate a certificate chain based on parameters in ctxto get ssl extensions 
*                   from ca certificate
*   Returns       : on success 1; on failure 0
*
***************************************************************************************************/
	
//static UInt32 ipr_cwmp_ssl_app_verify_callback(X509_STORE_CTX *ctx, void *arg)
    //changed this to supress the warning for ssl prototype of this function
static int ssl_app_verify_callback(X509_STORE_CTX *ctx, void *arg)
{
	UInt32 ok = 0;
	UInt8 buf[257] = {'\0'};
    if (NULL == arg)
    {    
		TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Arg NULL");
    }    
	X509_NAME_get_text_by_NID(X509_get_subject_name( ctx->cert), NID_commonName, (char*)buf, URL_LEN);
	UInt8 *acs_host_name = (UInt8*)PNULL;
	UInt8 *session_acs_host_name = PNULL;
	acs_host_name = (UInt8 *)ipr_cwmp_get_acs_host_name();
	session_acs_host_name = (UInt8 *)ipr_cwmp_get_session_acs_host_name();

    /*SPR 20088 FIXED START */
    TRACE (LOG_CRITICAL , HTTP_CONNECTION,"acs_host_name and  session_acs_host_name format\n %s %s",acs_host_name , session_acs_host_name);
    /*SPR 20088 FIXED END */

    /* SPR 19847 Fixed Start */
    char * buf_temp = NULL;
    char * acs_host_name_temp = NULL;
    char * session_acs_host_name_temp = NULL;

    if(buf[0]== '*')
    {
        buf_temp = strstr((char *)buf, ".");
        acs_host_name_temp = strstr((char *)acs_host_name, ".");

        /* COVERITY 110590/92 FIXED START */
        /*SPR 20088 FIXED START */ 

        if (NULL == buf_temp)
        {
            TRACE (LOG_CRITICAL , HTTP_CONNECTION,"buf_temp is null\n");
            /*Coverity 111415 Fix Start*/
            return 0;
            /*Coverity 111415 Fix End*/
        }

        if (NULL == acs_host_name_temp)
        {
            acs_host_name_temp = (char *)acs_host_name; 
            TRACE (LOG_CRITICAL , HTTP_CONNECTION,"acs Host name is not in format\n");
        }

        if (NULL != session_acs_host_name)
            session_acs_host_name_temp = strstr((char *)session_acs_host_name, ".");
        if (NULL == session_acs_host_name_temp)
        {
            session_acs_host_name_temp = (char *)session_acs_host_name; 
            TRACE (LOG_CRITICAL , HTTP_CONNECTION,"session Host name is not in format\n");
        }
        /* COVERITY 110590/92 FIXED END */
        /*SPR 20088 FIXED END */ 

        if(0 == ENB_STRNCMP((char*)acs_host_name_temp, (char*)buf_temp, strlen((char *)buf_temp)))
        {
            return X509_verify_cert(ctx);
        }
        else 
        {
            if(session_acs_host_name!=PNULL && ENB_STRLEN((char*)session_acs_host_name)>0 && 0 == ENB_STRNCMP((char*)session_acs_host_name_temp, (char*)buf_temp, strlen((char *)buf_temp)))
            {
                return X509_verify_cert(ctx);
            }
            else
                return 0;
        }
    }
    else
    {
        if(0 == ENB_STRNCMP((char*)acs_host_name, (char*)buf, strlen((char *)buf)))
	{
	    return X509_verify_cert(ctx);
	}
	else 
	{
            if(session_acs_host_name!=PNULL && ENB_STRLEN((char*)session_acs_host_name)>0 && 0 == ENB_STRNCMP((char*)session_acs_host_name, (char*)buf, strlen((char * )buf)))
	    {
        	return X509_verify_cert(ctx);
	    }
	    else
        	return 0;
	}



    }
	return(ok);

    /* SPR 19847 Fixed End */



}

/*************************************************************************************************
*   Function Name : ipr_cwmp_my_get_ext
*
*   Description   : This function is to get ssl extensions from ca certificate
*   Returns       : extension string
*
***************************************************************************************************/
	
/*static  UInt8 *ipr_cwmp_my_get_ext(X509 * cert, const UInt32 type, UInt32 extensiontype) 
{
	UInt32 i = 0;
	STACK_OF(ACCESS_DESCRIPTION) * accessinfo;
	accessinfo =  X509_get_ext_d2i(cert, extensiontype, PNULL, PNULL) ;
	
	if (!sk_ACCESS_DESCRIPTION_num(accessinfo))
    	return PNULL;
	for (i = 0; i < sk_ACCESS_DESCRIPTION_num(accessinfo); i++) 
    {
	    ACCESS_DESCRIPTION * ad = sk_ACCESS_DESCRIPTION_value(accessinfo, i);
    	if (OBJ_obj2nid(ad->method) == type) 
        {
    	    if (ad->location->type == GEN_URI) 
            {
        	    return ipr_cwmp_i2s_ASN1_IA5STRING(ad->location->d.ia5);
	        }
    	    return PNULL;
	    }
    }
	return PNULL;
}
commented this function as this is not being used as of now*/
	
/*************************************************************************************************
*   Function Name : ipr_cwmp_sslctxfun
*
*   Description   : This is an example of an curl SSL initialisation call back. The callback sets:
*                   a private key and certificate
*                   a trusted ca certificate
*                   a preferred cipherlist
*                   an application verification callback (the function above)
*   Returns       : CURLcode
*
***************************************************************************************************/

static CURLcode ipr_cwmp_sslctxfun(void* sslctx, void* parm)
{
	SSL_CTX* ctx = (SSL_CTX *) sslctx ;
	SSL_CTX_set_cert_verify_callback(ctx, ssl_app_verify_callback, parm);
	return CURLE_OK ;
}


void ipr_clean_http_session(void)
{
    TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Entering in  :%s", __FUNCTION__); 
    if (curl)
    {
        TRACE (LOG_CRITICAL , HTTP_CONNECTION, "Closing tcp connection"); 
        curl_easy_cleanup(curl);
        curl = NULL;
    }
    slist = NULL;        

}
/*SPR 17929 changes end*/
void initialize_cookie(void)
{
    /* SPR_19838 :Coverity_12932 Fix Start*/
    if(header_chunk.memory != NULL)
        DEALLOCATE(header_chunk.memory);

    /* SPR_19838 :Coverity_12932 Fix Stop*/
	header_chunk.memory = NULL;  
	header_chunk.size = 0;

}
/*SPR 17929 changes end*/

/*TLS CURL CHANGES*/
void create_resolve_command(UInt8* fqdnip,UInt8* Url,UInt8** resolveCmd)
{
	TRACE (LOG_CRITICAL , HTTP_CONNECTION, "SECURITY DEBUG:create_resolve_command called\n");
    UInt8 *fqdnStr = NULL;
    UInt8 *url = NULL;
    UInt32 i = 0;
    UInt32 j = 0;
    UInt8 s[2]= ":";
    Char8 *token_fqdn = NULL;
    //UInt8* token1_fqdn ;
    Char8* token2_fqdn = NULL;
    Char8 *token =NULL;
    Char8 *token1 = NULL;
    Char8 *token2 =NULL;
    UInt8 port[64]= {'\0'};
    UInt8 ip[64]={'\0'};
    UInt8 fqdn[64]={'\0'};

    
    
	fqdnStr  = ALLOCATE(200  );
    if (fqdnStr  == NULL)
    {
    exit(0); 
    }
	url  = ALLOCATE(200 );
    if (fqdnStr  == NULL)
    {
    exit(0); 
    }

    UInt8 resolve_command[256] = {'\0'};
    strncpy((char *)fqdnStr,(char *)Url,256);
    strncpy((char *)url,(char *)fqdnip,256);
	TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:url=%s\n",url);
	TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:fqdnStr=%s\n",fqdnStr);
    token = strtok((char *)fqdnStr,(char *)s); 
    while(token !=NULL)
    {
            token1 = token;
            token=strtok((char *)NULL,(const char *)s);
            if(j==0)
                token2=token;
            j++;
    }
    strncpy((char *)fqdn,(char *)token2+2,64);
    /* Coverity 97091 + */
    fqdn[63] = '\0';
    /* Coverity 97091 - */
    while(1)
    {
            if(token1[i] == '/')
                break;
            else
                port[i] = token1[i];
            i++;
    }
    port[i]='\0';
    token_fqdn = strtok((char *)url,(char *)s);
    j=0;
    while(token_fqdn !=NULL)
    {
            token_fqdn=strtok(NULL,(char *)s);
            if(j==0)
                token2_fqdn = token_fqdn;
            j++;
    }
	strncpy((char *)ip,(char *)token2_fqdn+2,64);
    /* Coverity 97091 + */
	ip[63]='\0';
    /* Coverity 97091 - */
	TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:ip is %s\n",ip);
TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:FQDN is %s\n",fqdn);
TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:PORT is %s\n",port);
    strncpy((char*)resolve_command,(char*)fqdn,64);
    strncat((char*)resolve_command,":",2);
    strncat((char*)resolve_command,(char*)port,64);
    strncat((char*)resolve_command,":",2);
    strncat((char*)resolve_command,(char*)ip,64);
    strncpy((char*)*resolveCmd,(char*)resolve_command,256);
TRACE (LOG_CRITICAL , HTTP_CONNECTION,"SECURITY DEBUG:RESOLVE command is %s\n",*resolveCmd);
}
/*MUTUAL AUTHENTICATION CHANGES*/

void create_tls_certpath(void)
{
	UInt8 str[CONFIG_PARAM_LEN] = {'\0'};
    /*SPR 18835 fix start */
    ENB_STRNCPY((char *)str,(char *)certificate_path,CONFIG_PARAM_LEN);
	str[CONFIG_PARAM_LEN -1] ='\0';
    TRACE(LOG_DETAIL, HTTP_CONNECTION ," str = %s" , (char *)str);
    /*SPR 18835 fix end */
	Char8* token =NULL;
	Char8* token1=NULL;
	UInt8 s[2] = {"/"};
	strncpy((char *)tlscertpath,(char *)s,2);
	 token = strtok((char *)str,(char *)s);
	while(token!=NULL)
	{   
		token1=token;
    	printf("token:%s\n",token);
    	token = strtok(NULL,(char *)s);
    	if(token !=NULL) 
    	{
            /* Coverity 97090 + */
        	strncat((char *)tlscertpath,(char *)token1,(CONFIG_PARAM_LEN - 1));
            /* Coverity 97090 - */
    		strcat((char *)tlscertpath,(char *)s);
    	}
	}
	TRACE (LOG_CRITICAL , HTTP_CONNECTION,"MUTUAL AUTHENTICATION DEBUG:tlscertpath=%s\n",tlscertpath);
}
/*SPR 19601 FIXED END */
/* SPR 20618 FIXED START */
char * tr069_cooki(char *string)
{

    char *token = NULL;
    int flag = 0;
    int emptycoki = 0;
    int add_colon = 0;
    char *temp = NULL;
    char *temp1 = NULL;
    char *cookie = NULL;
    char *cooki = NULL;
    cooki = ALLOCATE(BUFF_MAX_LEN);
    if (NULL == cooki)
    { 
        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"Memory allocation Failed\n");
        return NULL;
    }
    char **tokens = ALLOCATE(strlen(string)+1);
    if (NULL == tokens)
    {
        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"Memory allocation Failed\n");
        /*Coverity 111576 Fix Start*/
        DEALLOCATE(cooki);
        DEALLOCATE(tokens);
        /*Coverity 111576 Fix End*/

        return NULL;
    }

    char *header_chunk_string = NULL;
    header_chunk_string =  ALLOCATE(strlen(string)+1);
    if (NULL == header_chunk_string)
    {
        TRACE (LOG_CRITICAL , HTTP_CONNECTION,"Memory allocation Failed\n");
        /*Coverity 111576/111572 Fix Start*/
        DEALLOCATE(cooki);
        DEALLOCATE(tokens);
        /*Coverity 111576/111572 Fix End*/
        return NULL;
    }
    strcpy(header_chunk_string,string);
    int c = 0;
    int i = 0;
    token = strtok(header_chunk_string , ";");
    while (NULL != token)
    {
        tokens[c] = ALLOCATE(strlen(token)+1);
        if (NULL == tokens[c])
        {
            TRACE (LOG_CRITICAL , HTTP_CONNECTION,"Memory allocation Failed\n");
            /*Coverity 111576/111575/111572 Fix Start*/
            DEALLOCATE(header_chunk_string);
            DEALLOCATE(cooki);
            /* SPR 22314 Start */
            free_tokens(tokens, c);
            /* SPR 22314 End */
            /*Coverity 111576/111575/111572 Fix End*/
            return NULL;
        }
        strcpy(tokens[c], token);
        c++;
        token = strtok(NULL, ";");
    }

    for (i = 0; i < c; i++)
    {
        if (NULL != tokens[i])
        {
            temp = strstr(tokens[i], "Set-Cookie:");
            temp1 = strstr(tokens[i], "request_method");
        }
        if (NULL != temp && temp1 == NULL)
        {
            cookie = ALLOCATE( strlen(temp)+1);
            /*Coverity 111581/111571 Fix Start*/
            if(NULL == cookie)
            {
                TRACE(LOG_CRITICAL ,HTTP_CONNECTION ,"'Memory allocation failure");
                /*Coverity 111576/111575/111572 Fix Start*/
                DEALLOCATE(header_chunk_string);
                DEALLOCATE(cooki);
                /* SPR 22314 Start */
                free_tokens(tokens, c);
                /* SPR 22314 End */

                /*Coverity 111576/111575/111572 Fix End*/

                return NULL;
            }	
            /*Coverity 111581/111571 Fix End*/
            strcpy(cookie, temp);
            flag = check_dupid(tokens, tokens[i],i, c);
            if (flag == 0)
            {
                if(add_colon == 1)
                    strcat(cooki, "; ");
                else
                    strcat(cooki, "Cookie: ");
                strcat(cooki, (cookie+12));
                add_colon = 1;
                emptycoki = 1;
            }

            if (cookie)
                DEALLOCATE(cookie);
        }
    }
    //freeing memory
    for (i = 0; i < c; i++)
    {
        DEALLOCATE(tokens[i]);
    }
    TRACE(LOG_CRITICAL, HTTP_CONNECTION ,"cooki is = %s" , cooki);
    if(tokens)
    {
        TRACE(LOG_CRITICAL, HTTP_CONNECTION ,"tokens not null");
        /* SPR 22314 Start*/
        free_tokens(tokens ,c );
        /* SPR 22314 End*/
        /*coverity 111579 Fix Start*/
        /*code removed*/
        /*coverity 111579 Fix End*/
    }
    if(header_chunk_string)
    {
        TRACE(LOG_CRITICAL, HTTP_CONNECTION ,"header_chunk_string not null");
        DEALLOCATE(header_chunk_string);
    }
    if (emptycoki == 0)
        /*Coverity 111576 Fix Start*/
    {
        DEALLOCATE(cooki);
        return NULL;
    }
    /*Coverity 111576 Fix End*/
    return cooki;

}
int check_dupid(char **tokens, char token[], int i, int num_tok)
{
    char *token_id = NULL;
    int j = 0;
    char *temp = NULL;
    token = strstr(token, "Set-Cookie");
    if (NULL != token)
        token_id = strtok(token , "=");
    /*Coverity 111570/111569 Fix Start*/
    if(NULL != token_id)
    {
        for (j = i+1; j < num_tok; j++)
        {
            temp = strstr(token_id, "Set-Cookie");
            if (NULL != temp)
            {
                temp = temp + 12;
                if (strstr(tokens[j], temp) != NULL)
                {
                    return 1;
                }
            }
        }
    }
    /*Coverity 111570/111569 Fix End*/

    return 0;
}


/* SPR 20618 FIXED END */
/* SPR 22314 Start*/
void free_tokens(char **tokens , int c)
{
   int i = 0;
    for (i = 0; i < c; i++)
    {
        DEALLOCATE(tokens[i]);
    }
    DEALLOCATE(tokens);
}

/* SPR 22314 End*/
