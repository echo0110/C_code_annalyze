/****************************************************************************
 * *
 * **  FILE NAME     :  glue_main.c
 * **
 * **  DESCRIPTION   :  This file contains the various functions of glue layer.
 * **
 * **
 * **  DATE        AUTHOR      REF             REASON
 * **  ----        ------      ---             ----------
 * **  Revision Details
 * **  ----------------
 * ** Jan 30, 2013 Anant K Sharma Added a function ipr_cwmp_get_ip
 * ** Feb 28, 2013 Anant K Sharma Added a check to skip sending crypto_prof params to OAM in create_oam_msg
 * ** Feb 28, 2013 Anant K Sharma Moved ipr_cwmp_get_ip function to tr069 lib strfun.c
 * ** Mar 19, 2013 Anant K Sharma Added compilation flag HAVE_IPSEC for IPSec related code.
 * ** Apr 08, 2013 Anant K Sharma Added check for connectivity of GWs passed from OAM.
 * ** Jun 19, 2014 Vinod                         Fix for SPR 12059  
 * ** Jun 19, 2014 Vinod                         Fix for SPR 11921
 * ** Jul 23, 2014 Avinash                       Fix for SPR 8235
 * ** Copyright (c) 2013, Aricent Inc. All Rights Reserved  
 * **
 * **
 * ******************************************************************************/

#include <stdlib.h>
#include <cspl.h>
#include <tr69_driver.h>
#include <string.h>
#include <stdio.h>
#include <map_glue.h>
#include <glue_layer_def.h>
/*CSR 57877 Fix START */
#include <oam_mgmt_intf_api.h>
/*CSR 57877 Fix End*/
#include <logger.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <ipr_cwmp_enb_wrapper.h>
//IPSEC SSI code starts
#include <ipr_cwmp_read_config.h>
#include <lteTypes.h>
/* Periodic Inform Changes Start */
#include <ManagementServerModel.h>
/* Periodic Inform Changes End */
#include <common_def.h>

#ifdef HAVE_IPSEC
/* Spr 16751 changes Start */
/* Code Removed */
/* Spr 16751 changes End */
#include <ipr_cwmp_ping_util.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <ipr_cwmp_policy_handler.h>
#include <ipr_cwmp_state_handler.h>
#include <ipr_cwmp_sig_handler.h>
#include <strfun.h>

//this global varibale is used to save IPs of SecGWs, eNB and HeMS
config_res_t *g_ipr_cwmp_config_res = NULL;
//this global variable is used to save if there is a tunnel configured or not
UInt8 have_tunnel = 0;
//this global variable is used to check if user wants to use ipsec functionality for security or not
UInt8 have_ipsec = 0;
UInt8 ipsec_net_prefix = 0;
UInt8 ipsec_enc_key[IPSEC_ENC_KEY_LEN + 1] = {0};
UInt8 ipsec_auth_key[IPSEC_AUTH_KEY_LEN + 1] = {0};
struct itimerval g_tout_val = {0};
struct sigaction g_act = {0};
/* Global variable for cryptoprofile table params */
ipr_cwmp_device_fap_tunnel_crypto_profile_t g_crypto_profile = {0};
/* Global pointer variable for IPSec related params */
ipr_cwmp_crypto_params_t*                                         g_ipr_cwmp_crypto_params = PNULL;

#endif
	/*SPR 19749 FIXED START*/
#define SPV_BUFFER_MAX_SIZE 255

	/*SPR 19749 FIXED END*/
/* SPR 22092 FIXED Start */
/* Code Remove */
/* SPR 22092 FIXED Start */
/* SPR 21588 FIXED START  */
#define REMOVE_FRM_LIST ylPopTail  
/* SPR 21588 FIXED END  */

/* SPR 17880 START */
Uint16 decode_set_parameter_indication(void *p_buffer);
/* SPR 17880 END */
//IPSEC SSI code ends
/* SPR 16568 FIX START */
extern struct Device_ManagementServer g_ipr_cwmp_S_Device_Man_Ser;
UInt8 g_flag_to_notify_connection_req_url = 0;
/* SPR 16568 FIX END */

/*TLS CURL CHANGES*/
UInt8 g_hems_fqdn[128] ="";
UInt32 g_is_fqdn;

/* Spr 16990 Fix Start */
static UInt16 gpn_trans_id = 0;
/* Spr 16990 Fix End */
/* SPR 16327 Fixed Start  */
extern UInt8 MaxSessionRetryCount;
	      /* SPR 20704 FIXED START */
extern int send_curl_err_oam_g ;
	      /* SPR 20704 FIXED END */
/* SPR 16327 Fixed End  */
/* SPR 21588 FIXED START  */
#define REMOVE_FRM_LIST ylPopTail
/* SPR 21588 FIXED END  */

/* SPR 18104 FIXED START*/
UInt8 g_invoke_in_perform = 0;
UInt8 g_invoke_in_perform_periodic_inform = 0;
/* SPR 18104 FIXED END*/

static void *tr069_malloc (unsigned int size)
{
	return malloc (size);
	//return calloc (1,size);
}
static void tr069_free (void *m)
{
	free (m);
}
//extern const QSYSOP my_os;
const QSYSOP my_os = {
	tr069_malloc,
	tr069_free,
	0 ,
	0 ,
	0 ,
	0 ,
	0  ,
	0 ,
	0 ,
	0 ,
	0 ,
	0
};
//const QSYSOP *os = &my_os;


/* SPR 16645 START */
UInt32 g_Download_time_out_thres;
/* SPR 16645 END */
/* Coverity 114339/114338/114337/114336 Fixed Start*/
char *g_notif_buffer = NULL;
/* Coverity 114339/114338/114337/114336 Fixed End*/
/*SPR 17625 FIXED START */
#define PARANT_NAME_GPV 512
#define PARANT_NAME_GPA 512
/*SPR 17625 FIXED END*/
#define API_HEADER_SIZE 12
#define TRANS_RANGE 32767
/* GPN Change Start */
#define MAX_RECV_BUFF 65507 
/* GPN Change End */
/*download upload change starts*/
/* SPR 17268 FIXED START */
#define CPE_FILE_DIR_NAME  "/cpe_files/"
/* SPR 17268 FIXED END */
/*download upload change ends*/
#define BUFFER_LENGTH 276
#define NAME_VAL_GPV 400
/*SPR 16453 Fixed Start*/
#define PARANT_NAME_GPV 512
/*SPR 16453 Fixed End*/

//OAM SSI code starts
UInt32 recv_port = 0;
UInt32 send_port = 0;
UInt8  password[CONFIG_PARAM_LEN] = {0}, username[CONFIG_PARAM_LEN] = {0};
/* Periodic Inform Changes Start */
UInt8  PeriodicInformEnable = 0;
/* Periodic Inform Changes End */
UInt32  PeriodicInformInterval = 0;
UInt8  ConnectionRequestPassword[CONFIG_PARAM_LEN] = {0};
UInt8  ConnectionRequestUsername[CONFIG_PARAM_LEN] = {0};
UInt8  ConnectionRequestURL[CONFIG_PARAM_LEN] = {0};
UInt8  Manufacturer[CONFIG_PARAM_LEN] = {0};
UInt8  OUI[CONFIG_PARAM_LEN] = {0};
UInt8  ProductClass[CONFIG_PARAM_LEN] = {0};
UInt8  SerialNumber[CONFIG_PARAM_LEN] = {0};
UInt8 OAM_IP[17] = {0};
UInt8 TR069_IP[17] = {0};
/*SPR 17610 Fixed Start*/
/* SPR 16568 FIX START */
UInt8 HeNB_SELF_IP[40] = {0};
/*SPR 17610 Fixed End*/

UInt32  g_sleep_ind = 0;
UInt8  check_duplicates_spv_flag = 0;


/*download upload change starts*/
UInt32 queue_upload_max_size = 0;
UInt32 queue_download_max_size = 0;
/*download upload change ends*/

/* SPR 17268 fixed start */
extern UInt32 ABS_len;
/*  coverity 96672 fix start */
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/*  coverity 96672 fix end */
/* SPR 17268 fixed end */


//OAM SSI code ends

UInt8 * buf_spv = NULL;
Uint16 tmp_spv_len = 0;
UInt8  iHems_url[URL_SIZE] = {0};
UInt8 shems_Session = 0;
UInt8 iHems_session = 0;
UInt8 g_send_init_config = 0; 
UInt8 IsLte_ScanTimeExists = 0;
UInt8 g_Lte_LastScanTime[CONFIG_PARAM_LEN] = {0};

/* used in Get Parameter to keep track of index in Name-Value Pair */
Uint16 g_Index = 0 ;      

/* Used for totalno. of bytes to be send in case of GetParameter */
Uint16 g_get_buff_size = 0;  

/* SPR 17596 fixed start */
// Used for totalno of bytes to be send in case of GetRPC_tMethod
UInt16 rpc_get_buff_size = 1024;
/* SPR 17596 fixed end */

/* Used for totalno. of bytes to be send in case of Downlad RPC*/
Uint16 g_dwn_buffer = 0;   

/* SPR 17631 fixed start*/
/* Used for totalno. of bytes to be send in case of Downlad RPC*/
Uint16 g_upld_buffer = 0;
/* SPR 17631 fixed end */

/* SPR 17717 fixed start */
UInt16 g_alarm_buffer = 0; 
/* SPR 17717 fixed end */
static Uint16 g_trans_id  = 1;
static Uint16 g_trans_init_id= 0 ;

//SInt32 g_sleep_ind = 1;
U8 g_first_file_ind = 0; 
U8 g_Is_inform_rec = 0;

Uint16 g_trans_id_sec_param = 0;
U8 g_Is_Security_param = 0;
//U8 g_Is_sec_url = 0;
U8 g_Is_config_res_send = 0;
//UInt8 tmp_sec_buffer[1024] = {0};
U8 *g_tmp_url_buffer = NULL;
U8 g_parent_URL = 0; 
Uint16 g_url_trans = 0;
U8 g_Is_new_url = 0;
U8 g_Reboot_recv = 0;
/* Spr 16890 Fix Start */
/* Code Deleted */
/* Spr 16890 Fix End */
extern U8 g_send_notif_fail;
/*socket to send data */
struct sockaddr_in g_serverAddr;

time_t g_file_ind_start;
time_t g_file_ind_end;

pthread_cond_t count_threshold_cv;
pthread_mutex_t count_mutex;
U8 thread_unlock = 0;

U8 g_exped_wait_inform = 0;
U8 g_queued_wait_inform = 0;
extern U8 g_expedited_inform;
static Uint16 g_notif_transid = 0;


extern struct ParameterValueList *g_queued_paramList;
extern U8 g_queued_paramList_size;

Tr69Parameters_t   tr69Parameters = {'\0'};
/*
   extern struct ParameterValueList *g_queued_paramList;
   extern U8 g_queued_paramList_size;
   */
#ifdef VENDOR_SPECIFIC 
extern ParameterValueList g_forcedParamList;
#endif


extern void * remove_node_from_Set (YLIST *yList , transData_t *p_transD);
/* SPR 21563 FIX START */
extern void free_gpv_resp_buffer (struct ParameterValueList *p_get_stat_t);
/* SPR 21563 FIX END  */
extern void addNodeinSet ( YLIST *yList , transData_t *p_transD);
extern void initiate_TR069_Inform_session (char * eventCode, ParameterValueList *ParamValList); 
extern void init_periodic_inform(void);
extern int connManagerUpload (Upload *arg_upload_t, char * arg_file_path);
extern void initialize_TR069_Client (sDeviceInfo              sDeviceIdInfo,
		ParameterValueList     sParamValList,
		Tr69Parameters_t        tr69Parameters,
		sCallbackFunList_t      sCallbackFunList,
		char *                  HeMS_URL,
		enum eFlag   			Init_flag); 
static void * handle_get_param(void *msg , int eType);
/*SPR 17625 FIXED START*/
/* SPR 17596 fixed start */
static void * create_oam_msg_for_get_rpc();
static void * handle_get_rpc(int eType);
struct MethodList *  decode_OAM_get_rpc(void * p_buffer );
/* SPR 17596 fixed end */
static void * handle_get_param_attr(void *msg);
/*SPR 17625 FIXED END*/
static void * buffer_OAM_Get_Parameter_Val (char * parent, YLIST *Names, Uint16 num_param);
static Uint16  decode_OAM_get(void * p_res, ParameterValueList *cpeParamList_t);
static void * create_oam_msg_for_get(map_map * m);
static void  decode_OAM_Download_Res(void *p_res,  DownloadStatus * );
static void * handle_oam_file_download_ind(void  *msg , int eType);
/* SPR 17717 fixed start */
static void * handle_oam_alarm_ind(void);
/*SPR 17717 fixed end */
static  void * handle_oam_download(void *msg);

/* SPR 17631 fixed start */
static void decode_OAM_Upload_Resp(void *, UpldResp*);
void * recv_msg_oam_Upload(YLIST *yList);
static void * handle_oam_upload (void *msg);
static void * handle_oam_file_upload_ind  (void  *msg);

/* SPR 17631 fixed end */
extern void map_free(map_map* in);

static void * encode_OAM_Config_Res(void *p_buffer, sDeviceInfo *devidstruct, Tr69Parameters_t *);
static Uint16  buffer_OAM_Config_Resp(U8 status, void **p_buffer, Uint16 error_code , U8 *err_str);;
#ifdef USE_HANDLE_SET
static void handle_set_url(void * p_URL);
#endif
void decode_notification (void * p_buffer);
void * handle_add_object (void * msg);
void * decode_oam_cntrl_cmd(void * p_arg_buffer);
UInt8 upload_all_in_Dir(Upload_Dir *upl_dir_t);
void *handle_deleteobject_req(void *msg);
void *handle_reboot_req(void);
void *handle_factoryreset_req(void);
Uint16  buffer_OAM_notif_Resp  (U8 status, void **p_buffer);
void * buffer_OAM_Control_Cmd_Req (U8 num_param  ,
		char *arg1_Name_Val,
		UInt32 *buffer_size ,
		UInt8 *contrl_cmd);
void * recv_msg_oam_Download(YLIST *yList);
/*SPR 17627 FIXED START*/
int send_heartbeat_resp(void);
/*SPR 17627 FIXED END*/

void oam_sighand(void);

/* SPR 21294 FIXED START */
Uint16 checkDuplicateNotificationParams(cwmp__ParameterValueStruct **cpeParamArr,cwmp__ParameterValueStruct *p_cpeParamVal_t,Uint16 g_queued_paramList_size);
/* SPR 21294 FIXED END */

static void *msgHandle = NULL;
//typedef struct  nParameterValueStruct ParameterValueStruct;
/* SPR 21294 FIXED START */
Uint16 checkDuplicateNotificationParams(cwmp__ParameterValueStruct **cpeParamArr,cwmp__ParameterValueStruct *p_cpeParamVal_t,Uint16 g_queued_paramList_size);
/* SPR 21294 FIXED END */
/*SPR 17625 FIXED START*/
typedef struct  nSetParameterAttributesStruct ParameterAttributesStruct; /*-------- Ruby --------*/
typedef struct  nAccessList sAccessList;  /*-------- Ruby -------*/
/*SPR 17625 FIXED END*/

/* SPR 21588 FIXED START  */
YLIST g_notification_Q;
/* SPR 21588 FIXED END */
map_map* module_map_ptr= NULL;

/* cspl fix start */
/* function for getting size of memory */
unsigned int get_size(void *buffer)
{
	unsigned int size = 0;
        unsigned char   *c = (unsigned char *)buffer;
        qbuf    *q;
        qpool   *pool;

        c -= YALIGN(sizeof(qbuf),8);
        q = (qbuf *)(void *)c;
	pool = q->u.pool;
	qvLock( pool->lock );
	size = pool->size;
	qvUnlock( pool->lock );
	return size;
}
/* function to allocate memory */
void *deallocate(void * ptr)
{
	if (NULL != ptr)
	{
		qvFree(ptr);
		ptr = NULL;

	}

	return (void *)NULL;
} 

#ifdef CSPL_LEAK_DEBUG_LITE
void *tr_Allocate(unsigned int size,const char *func, unsigned int line) 
#else
void *tr_allocate(unsigned int size) 
#endif
{
    void *ptr; 

#ifdef CSPL_LEAK_DEBUG_LITE
    int  err = 0;
    ptr = qvAllocEX(size, PNULL,&err,func,line);
#else
    ptr = (void *)qvAlloc(size, NULL);
#endif
    if (NULL == ptr)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "qvalloc memory allocation failed size= %d", size);
        return NULL;
    }
    memset(ptr, 0, size);
    return ptr;
}
/* function to reallocate  memory */
void *tr_realloc_fun(void *ptr, unsigned int size)
{
	unsigned int size_p = 0;
	if (NULL != ptr)
    {
		size_p = get_size(ptr);
	
        if (size <= size_p)
            return ptr;
    }

	void *temp = ALLOCATE( size);
	if (NULL == temp)
	{

		TRACE(LOG_CRITICAL , GLUE_LAYER, "TR_REALLOC memory allocation failed");
		return NULL;
	}
	if (ptr == NULL)
		return temp;

	ENB_MEMCPY(temp, ptr, size_p);
	DEALLOCATE(ptr);
	/* coverity 111425 fix start */
   /*code remove */
   /* coverity 11425 fix end */

    return temp;
}
/* cspl fix end */
/****************************************************************************
 ** ** Function Name  : get_fd
 ** ** Inputs         : none
 ** ** Outputs        : none
 ** ** Returns        : UInt32 value
 ** ** Description    : This function return the recving socket descriptor 
 ** *****************************************************************************/
UInt32 get_fd(void)
{
	MsgHandle *p_handle  = (MsgHandle *)msgHandle;
	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s,  Value of FD : %d", __FUNCTION__, p_handle->recv_sockfd);
	return p_handle->recv_sockfd;
}

/****************************************************************************
 * Function Name  : glue_layer_init
 * Inputs         : none
 * Outputs        : none
 * Returns        : UInt32 value
 * Description    : This function initializes glue layer 
 * ** *****************************************************************************/
UInt32 glue_layer_init(void)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering in %s",__FUNCTION__);

	MsgHandle *p_handle;
	SInt32 ret_bind, port = 0;

	struct sockaddr_in serverAddr;

	p_handle = (MsgHandle *)ALLOCATE(sizeof(MsgHandle) );
	if(p_handle == NULL)
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, could not allocate storage form MsgHandle_FD", __FUNCTION__);
		return GLUE_FAILURE;
	}

	port = send_port;

	if((p_handle->send_sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, could not create socket", __FUNCTION__);
		exit(1);
	}

	memset((char *)&g_serverAddr, 0, sizeof(g_serverAddr));
	g_serverAddr.sin_family = AF_INET;
	g_serverAddr.sin_port = htons(port);
	if (inet_aton((const char *)OAM_IP, &g_serverAddr.sin_addr) == 0)
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Wrong UDP_TX_IP ");
		exit (1);
	}

	port = recv_port;

	if((p_handle->recv_sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, could not create socket", __FUNCTION__);
		exit(1);
	}
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	if (inet_aton((const char *)TR069_IP , &serverAddr.sin_addr)==0)
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Wrong UDP_RX_IP");
		exit (1);
	}
    /* SPR 22541 Start */
    UInt32 option = 1;
    if( -1 == ENB_SETSOCKOPT(p_handle->recv_sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(UInt32)))
    {
        perror ("Socket set Fails\n");
		exit(1);
    }
    /* SPR 22541 End */
	ret_bind = bind(p_handle->recv_sockfd , (struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if (-1 == ret_bind)
	{
		perror("Bind Fails rec_msg_oam\n");
		exit(1);
	}
	msgHandle = (void *)p_handle;
	return 0;
}


/****************************************************************************
 * ** Function Name  : get_word_from_header
 * ** Inputs         : p_header
 * ** Outputs        : none
 * ** Returns        : UInt16 value
 * ** Description    : This function finds the value of 2 bytes
 * *****************************************************************************/

	UInt16 get_word_from_header
(
 UInt8 *p_header
 )
{
	UInt16 value = 0;
	value = p_header[0];
	value = value << 8;
	value = value | p_header[1];
	return value;
}

/******************************************************************************
  FUNCTION NAME  : recv_msg_oam
Inputs         : 
Outputs        : none
RETURNS        : none
DESCRIPTION    : This function will create socket and recev msg to oam
 *******************************************************************************/
void * recv_msg_oam(YLIST *yList,Uint16 etype)
{
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering  %s",__FUNCTION__);
    UInt32 sockfd = 0 ; 
    SInt32 result = 0,byte_read = 0 ;
    struct timeval      timeval;
    fd_set set;
    UInt8 *buffer = NULL;
    /* Spr 16990 Fix Start */
    UInt8 end_mark = 0;
    UInt16 error_code = 0;
    GetParameterNamesStatus_t * p_gpn_res_t = NULL ;
    GetParameterNamesStatus_t * p_gpn_fault_res_t = NULL ;
    /* Spr 16990 Fix End */
	buffer =  (UInt8 *)ALLOCATE(MAX_RECV_BUFF );
    if(NULL == buffer) 
    {
        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
        exit(0);
    }

    memset(buffer , '\0', MAX_RECV_BUFF );

    UInt32 rc = 0;
    sDeleteObjectStatus_t *p_delete_obj_res_t = NULL;
    /*SPR 17625 FIXED START*/
    sSetParamAttrRes_t *p_set_attr_resp_t = NULL;
    /*SPR 17625 FIXED END*/
    sControl_CmdStatus_t *p_cntrl_cmd_res_t = NULL;

    Uint16 l_rec_trans_id = 0;
    Uint16 l_ret_code = 0 ; 
    GetStatus *p_get_stat_t = NULL;
    /* SPR 17596 fixed start */
    GetRPC_t *p_rpc_stat_t  =NULL;
    /* SPR 17596 fixed end */
    /*SPR 17625 FIXED START*/
    GetParamAttrRes *p_get_stat_attr_t =NULL;

    /* coverity 96642 fixed /code removed */
    UInt8 param_notification = 11;
    /*SPR 17625 FIXED END*/
    sAddObjectStatus_t *p_add_obj_res_t = NULL;
    UInt16  api_idr = 0;
    /* SPR 22314 Start */
    /* Code Removed */
    /* SPR 22314 End */
    transData_t g_transD_t;
    transData_t *p_retback_t = NULL;
    ParameterValueList *cpeParamList_t = NULL;
    /* coverity 61971 fixed / code removed */
    StStatus *p_set_stat_t = NULL;
    /* SPR 18130 FIXED START */
    UInt16 node_name_length = 0;
    UInt8 *node_name = NULL;
    /* SPR 18130 FIXED END */
    UInt8 status_resp = '0';
    UInt8 error_str_len = '0';
    UInt16 recv_trans_id = 0;
    /* GPA Change Start */
    //Uint32 l_offset = 0 ;
    int l_offset = 0; 
    /* GPA Change End */
    Uint16 l_f_ind = 0 ;
    U8 temp_l_len = 0 ;
	/* SPR 19083 FIXED START*/
	struct timeval start_t;
	struct timeval end_t;
	/* SPR 19083 FIXED END */ 

    struct _cwmp__Fault *stFault = NULL;
    struct _cwmp__Fault_SetParameterValuesFault *stTmpArr = NULL;

    char * ParameterName = NULL;
    char * FaultCode = NULL;
    char * FaultString = NULL;

    struct sockaddr_in client;
    UInt32 len = sizeof(client);

    /* SPA,SPR 17625 FIXED START*/
    UInt16 num_param = 0;
    UInt16 l_count = 0;
    /* coverity 96649 fixed /code removed */
    UInt16 l_len = 0;
    UInt16 param_len = 0;
    ParameterAttributeStruct **cpeParamAttrbArr = NULL;
    ParameterAttributeStruct *p_cwmpParamAttr_t = NULL;
    ParamAccessList *p_accessList = NULL;
    sParameterList *cpeParamListA_t = NULL;
    /*SPA,SPR 17625 FIXED END*/
    /* SPR 17880 START */
    UInt16  counter_loop = 0;
    /* SPR 17880 END */
    sockfd = get_fd();	
    rc = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    /* SPA,SPR 17625 FIXED START*/
    UInt16 accesslist_len;
    /* SPA,SPR 17625 FIXED END*/

	/* SPR 19083 FIXED START*/
	time_t ret_start = 0;
	time_t  ret_end = 0;
	tr069_gettimeofday_wrapper(&start_t);
	ret_start = (time_t)start_t.tv_sec;
	tr069_gettimeofday_wrapper(&end_t);
	ret_end = (time_t)end_t.tv_sec;

	/* SPR 19083 FIXED END */ 
    if (rc != 0)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s, fcntl error  : %d", __FUNCTION__, rc);
        //return NULL;
    }
    if (etype == GLUE_INIT)
    {

        /*SPR 16294 Code Remove*/
        byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len );
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s,  Bytes Recieved : %d", __FUNCTION__, byte_read);
        if (byte_read <= 0)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s, recv ()  fail", __FUNCTION__);
			DEALLOCATE(buffer);
            return NULL;
        }		
        if (byte_read > 0)
        {
            /* Coverity 56617 + */
            buffer[MAX_RECV_BUFF - 1] = '\0';
            /* Coverity 56617 - */
            /* SPR 22314 Start */
			api_idr = get_word_from_header((U8 *) (buffer) + 6);
            /* SPR 22314 End */
            if (api_idr == TR069_OAM_CONFIG_REQ)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_CONFIG_REQ"); 
                return buffer;
            }	

            /*SPR 17627 FIXED START*/
            else if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ"); 
                /*send hearbeat response to sysctrl */
                if(1 != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response"); 
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully"); 
                }
				DEALLOCATE(buffer);
                return NULL;
            }
            /*SPR 17627 FIXED END*/
        }	
    }

    /* SPR 17596 fixed start */
    if(etype == RPC_GETRPCMETHOD)
    {
		p_rpc_stat_t = (GetRPC_t *)ALLOCATE(sizeof (GetRPC_t) );
        if (NULL == p_rpc_stat_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(buffer);
            return NULL;
        }

		p_rpc_stat_t->resp = (struct _cwmp__GetRPCMethodsResponse *) ALLOCATE (sizeof(struct _cwmp__GetRPCMethodsResponse) );
        if(NULL == p_rpc_stat_t->resp)
        {
			TRACE (LOG_CRITICAL , GLUE_LAYER, "p_rpc_stat_t->resp Exiting %s ALLOCATE failed",__FUNCTION__);
			DEALLOCATE(p_rpc_stat_t);
            p_rpc_stat_t = NULL;
            return NULL;
        }
		/* p_rpc_stat_t->resp->MethodList = (struct MethodList *) ALLOCATE(sizeof(struct MethodList) );
        if(NULL == p_rpc_stat_t->resp->MethodList)
        {
		   TRACE (LOG_CRITICAL , GLUE_LAYER, "p_rpc_stat_t->resp Exiting %s ALLOCATE failed",__FUNCTION__);
		   DEALLOCATE(p_rpc_stat_t->resp);
                p_rpc_stat_t->resp = NULL;
                return NULL;
        }*/

        struct _cwmp__GetRPCMethodsResponse *response = {0};
        /* SPR 20636 Changes Start*/
        response = (struct _cwmp__GetRPCMethodsResponse *)(ADDR)ALLOCATE(sizeof(struct _cwmp__GetRPCMethodsResponse));
        /* SPR 20636 Changes End*/
        if (NULL == response)
        {
			TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting %s ALLOCATE failed",__FUNCTION__);
            return NULL;
        }

        TRACE(LOG_CRITICAL , GLUE_LAYER , "...In recv_msg_oam etype is : %d",etype);
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s,  type = RPC_GETRPCMETHOD",__FUNCTION__);


		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;

		TRACE(LOG_CRITICAL , GLUE_LAYER, " Start Time for GET RPC PARAMETER: %ld", ret_start);
		/* SPR 19083 FIXED END */ 


        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;
        while (!(ylEmpty(yList)))
        {

            TRACE(LOG_CRITICAL , GLUE_LAYER ,"...in getrpc......in while");
            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");
            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec; 
				if (ret_end - ret_start > WAIT_LIMIT)
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", ret_end);
					/* SPR 19083 FIXED END */ 
                    p_rpc_stat_t->eStatus = RPC_FAULT;
                    p_rpc_stat_t->fault_code = INTERNAL_FAILURE;
					p_rpc_stat_t->fault_string = (char *)ALLOCATE( 80);
                    if(p_rpc_stat_t->fault_string)
                    {
                         ENB_STRNCPY(p_rpc_stat_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
                    }
					DEALLOCATE(buffer);
                    return p_rpc_stat_t;
                }
                else
                {
                    TR069_USLEEP(500);
                    continue;
                }
            }
    
            api_idr = get_word_from_header((U8 *) (buffer) + 6);
            TRACE(LOG_CRITICAL , GLUE_LAYER, "api id for get rpc : %d ",api_idr);

            recv_trans_id = get_word_from_header(buffer);
            TRACE(LOG_CRITICAL , GLUE_LAYER, "recv trans id for get rpc : %d ",recv_trans_id);
            if(0 != recv_trans_id)
            {
                g_transD_t.trans_id = recv_trans_id;
            }
            else
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
            }

            p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);

            if (p_retback_t != NULL)
            {
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&start_t);
				ret_start = (time_t)start_t.tv_sec;
				/* SPR 19083 FIXED END */ 
                REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
				DEALLOCATE (p_retback_t);
                p_retback_t = NULL;

                response->MethodList = decode_OAM_get_rpc(buffer);
                if(NULL == response->MethodList)
                {
                    p_rpc_stat_t->eStatus = RPC_FAULT;
                    p_rpc_stat_t->fault_code = INTERNAL_FAILURE;
					p_rpc_stat_t->fault_string = (char *)ALLOCATE( 80);
                    if(p_rpc_stat_t->fault_string)
                    {
                          ENB_STRNCPY(p_rpc_stat_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
                    }

					DEALLOCATE(buffer);
                    return p_rpc_stat_t;

                }

            }

            else
				/* SPR 19083 FIXED START */ 
				tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;
			if (ret_end - ret_start > WAIT_LIMIT)
				/* SPR 19083 FIXED END */ 
            {
                p_rpc_stat_t->eStatus = RPC_FAULT;
                p_rpc_stat_t->fault_code = INTERNAL_FAILURE;
				p_rpc_stat_t->fault_string = (char *)ALLOCATE( 80);
                if(p_rpc_stat_t->fault_string)
                {
                      ENB_STRNCPY(p_rpc_stat_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
                }

				DEALLOCATE(buffer);
                return p_rpc_stat_t;
            }


        }
        p_rpc_stat_t->resp->MethodList = response->MethodList;
        p_rpc_stat_t->eStatus  = RPC_SUCCESS;
		DEALLOCATE(response);
		DEALLOCATE(buffer);
        return p_rpc_stat_t;

    }
    /* SPR 17596 fixed end */

    if (etype == RPC_GETPARAMETERVALUES)
    {
        //coverity fix
		p_get_stat_t = (GetStatus *)ALLOCATE(sizeof (GetStatus) );
        if (NULL == p_get_stat_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
            DEALLOCATE(buffer);
            return NULL;
        }

        memset(p_get_stat_t , '\0' , sizeof (GetStatus));

		cpeParamList_t = (ParameterValueList *)ALLOCATE(sizeof(ParameterValueList) );
        if (NULL == cpeParamList_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(p_get_stat_t);
            DEALLOCATE(buffer);
            return NULL;
        }

        memset(cpeParamList_t  , '\0' , sizeof (ParameterValueList ));



		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		TRACE(LOG_CRITICAL , GLUE_LAYER, " Start Time for GET PARAMETER: %ld", ret_start);
		/* SPR 19083 FIXED END */ 
        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        /* SPR 18135 changes start */
        g_Index = 0;

        while (!(ylEmpty(yList)))
            /* SPR 18135 changes end */
        {
            /* Spr 17463 Fix End */ 
            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");
            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {

                /* GPV PARTIAL PATH Change Start */		

				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT_GPN_RESP)
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", ret_end); 
					/* SPR 19083 FIXED END */ 
                    /* GPV PARTIAL PATH Change End */		
                    p_get_stat_t->eStatus = RPC_FAULT;
                    p_get_stat_t->fault_code = INTERNAL_FAILURE;
                    p_get_stat_t->parameterList = NULL;
					DEALLOCATE(buffer);
                    return p_get_stat_t;
                }
                else
                {
                    TR069_USLEEP(500);
                    continue;
                }
            }
            //	if(buffer[0]){
            // Code added to support Notification in between RPC's
            api_idr = get_word_from_header((U8 *) (buffer) + 6);
            TRACE(LOG_CRITICAL , GLUE_LAYER, "api id for get %d ",api_idr);

            /*SPR 17627 FIXED START*/
            if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                /*send hearbeat response to sysctrl */
                if(1 != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                }

            }
            /*SPR 17627 FIXED END*/

            /*CSR 57877 Fix START */
            if (api_idr == OAM_NOTIFICATION_IND)
                /*CSR 57877 Fix End*/
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
		/* SPR 21588 FIXED START  */
		/* Code removed */
		/* SPR 21588 FIXED END  */
				g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF );
                if (g_notif_buffer)
		{
                   memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
		   TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
		   /* SPR 21588 FIXED START  */
		   TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
		   ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
		   g_notif_buffer = NULL;
		   /* SPR 21588 FIXED END */

		}
            }
            /* Spr 17463 Fix Start */
            else if(OAM_GET_PARAMETER_VALUES_RESP == api_idr)
            {
                /* Spr 17463 Fix End */ 
                l_rec_trans_id = get_word_from_header(buffer);	
                /* Spr 17463 Fix Start */
                /* SPR 18135 changes start */
                if(0 != l_rec_trans_id)
                    /* SPR 18135 changes end */
                {
                    g_transD_t.trans_id = l_rec_trans_id;
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid trans ID Received");
                    p_get_stat_t->eStatus = RPC_FAULT;
                    p_get_stat_t->fault_code = INTERNAL_FAILURE;
					/*SPR 18331 FIXED START */
					//DEALLOCATE(p_get_stat_t->parameterList);
					//p_get_stat_t->parameterList = NULL;
					/*SPR 18331 FIXED END */

                    /* Free the resources here */
					DEALLOCATE(buffer);
                    return p_get_stat_t;
                }
                /* Spr 17463 Fix End */ 
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&start_t);
				ret_start = (time_t)start_t.tv_sec;
				/* SPR 19083 FIXED END */ 
                /* Spr 17463 Fix Start */
                /* Code Removed */
                /* Spr 17463 Fix End */ 

                /* SPR 18135 changes start */
                /* Spr 17463 ZOMMBI Fix Start */
                //g_Index = 0;
                /* Spr 17463 ZOMMBI Fix Start */
                /* SPR 18135 changes end */

                /* SPR 18135 changes start */
                p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                if (p_retback_t != NULL)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "p_retback_t is not null\n");
                    REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Nodes in list = %d",(ylCount(yList)));
                    /* SPR 18135 changes end */
                    /*Decode the buffer now */
                    l_ret_code = decode_OAM_get(buffer , cpeParamList_t);
                    /* SPR 21563 FIXED START */
                    if (g_Index > 0)
                    {
                       cpeParamList_t->__size  = g_Index; 
                    }    
                    /* SPR 21563 FIXED END */

                    if (l_ret_code != 0 && l_ret_code != 1)
                    {
               /* SPR 20974 FIXED START */
                  free_gpv_resp_buffer (cpeParamList_t); 
               /* SPR 21785 FIXED START */
                  /*SPR 21884 FIXED START */
                /* Code removed */
                  /*SPR 21884 FIXED END */
               /* SPR 21785 FIXED END */
               /* SPR 20974 FIXED END */
                        p_get_stat_t->eStatus = RPC_FAULT;
                        p_get_stat_t->fault_code = l_ret_code;
                        p_get_stat_t->parameterList = NULL;
                        /* Free the resources here */
						DEALLOCATE(buffer);
                        return p_get_stat_t;
                    }
                    /* SPR 18135 changes start */
                }   
                /* Spr 17463 Fix Start */
                if ( (g_Index > 0) && (ylEmpty(yList)))
                    /* SPR 18135 changes end */
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "pankaj_debug List is empy returning\n");
                    cpeParamList_t->__size  = g_Index;
                    p_get_stat_t->parameterList = (struct ParameterValueList *)cpeParamList_t;
                    p_get_stat_t->eStatus  = RPC_SUCCESS;
					DEALLOCATE(buffer);
                    return p_get_stat_t;
                }
            }
            else if(OAM_GET_PARAMETER_VALUES_RESP_ALL == api_idr) 
            {
                p_get_stat_t->parameterList = (struct ParameterValueList *)cpeParamList_t;
                l_rec_trans_id = get_word_from_header(buffer);

                /* coverity 96647 fixed /code removed */
                l_offset = 0 ;
                status_resp =0;
                l_offset  += 12;
                memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));
                /* SPR 18135 changes start */
                if(0 != l_rec_trans_id)
                    /* SPR 18135 changes end */
                {
                    g_transD_t.trans_id = l_rec_trans_id;
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid trans ID Received");
  /* MEMORY LEAK 31 DEC FIXED START */
  /* SPR 21785 FIXED START */
                   free_gpv_resp_buffer (p_get_stat_t->parameterList); //V9D
  /* SPR 21785 FIXED END */                 
  /* MEMORY LEAK 31 DEC FIXED END */
  
                    p_get_stat_t->eStatus = RPC_FAULT;
                    p_get_stat_t->fault_code = INTERNAL_FAILURE;
               /* SPR 20974 FIXED START */
                /* code removed */
               /* SPR 20974 FIXED END */
                    /* Free the resources here */
					DEALLOCATE(buffer);
               /* SPR 21785 FIXED START */
               /* Code Removed */
               /* SPR 21785 FIXED END */
                    return p_get_stat_t;
                }
                if (status_resp == 0)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "GPV Status = %d",status_resp);
                    /* SPR 18135 changes start */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Nodes in list = %d",(ylCount(yList)));
                    /* Success Received Case */
                    if (GLUE_SUCCESS ==  decode_gpv_response(buffer ,p_get_stat_t, &end_mark))
                    {
                        if (end_mark == 1)
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = FALSE, expected more GPV responses ");
                            continue;
                        }
                        else if ( (0 == end_mark) && (!(ylEmpty(yList))) )
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "end_mark TRUE (0), list not empty");
                            p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                            if (p_retback_t != NULL)
                            {
                                REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                            /* SPR 21563 FIXED START */
                            DEALLOCATE(p_retback_t);
                            /* SPR 21563 FIXED END */
                            }

                            if(ylEmpty(yList))
                            {
                                TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = TRUE, LIST EMPTY");
                                gpn_trans_id = 0;  /*resetting the value */
                                /* SPR 21563 FIXED START */
                                DEALLOCATE(buffer);
                                /* SPR 21563 FIXED END */
                                return p_get_stat_t;
                            } 
                            else
                            {
                                TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = TRUE, LIST NOT EMPTY =  %d, continue",(ylCount(yList)));
                                continue;
                            } 
                        }           
                        else if ((0 == end_mark) && (ylEmpty(yList)))
                            /* SPR 18135 changes end */
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = TRUE, all GPV response received");
                            gpn_trans_id = 0;  /*resetting the value */
							/* SPR 21563 FIXED START */
							DEALLOCATE(buffer);
							/* SPR 21563 FIXED END */
                            return p_get_stat_t;
                        }
                    }
                    else
                    {
						TRACE(LOG_CRITICAL, GLUE_LAYER, "Failure in decoding the GPV Response");
               /* SPR 20974 FIXED START */
                  /* SPR 21785 FIXED START */
						free_gpv_resp_buffer (p_get_stat_t->parameterList);
                  /* SPR 21785 FIXED END */
               /* SPR 20974 FIXED END */
                        p_get_stat_t->eStatus = RPC_FAULT;
                        p_get_stat_t->fault_code = INTERNAL_FAILURE;
                        p_get_stat_t->parameterList = NULL;
                        /* Free the resources here */
               /* SPR 20974 FIXED START */
                /* code removed */
               /* SPR 20974 FIXED END */
						DEALLOCATE(buffer);
                        /* SPR 21785 FIXED START */
                   /*Code Remove */     
                  p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                  if (p_retback_t != NULL)
                  {
                      REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                      DEALLOCATE(p_retback_t);
                        free_gpv_resp_buffer (p_get_stat_t->parameterList);
                        /* SPR 21785 FIXED END */
						DEALLOCATE(buffer);
						/* SPR 21563 FIXED START */
						DEALLOCATE(cpeParamList_t);
						/* SPR 21563 FIXED END */
                        return p_get_stat_t;
                   }
                    }

                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Failure GPV RESP Received");
                    /* Failure Case */
                    l_offset += 1; /* 1 Status Resp */
                    error_code = error_code | (UInt16) ((buffer)[l_offset] << 8);
                    error_code = error_code | (UInt16) ((buffer)[l_offset+1]);
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Error Code received in GPV RESP: %d",error_code);
                    p_get_stat_t->eStatus = RPC_FAULT;
                    p_get_stat_t->fault_code = error_code;
                    /* SPR 21785 FIXED START */
                    free_gpv_resp_buffer (p_get_stat_t->parameterList);
                    /* SPR 21785 FIXED END */
                    /* SPR 22314 Start */
                    p_get_stat_t->parameterList = NULL;
                    /* SPR 22314 End */
                    /* Free the resources here */
                    DEALLOCATE(buffer);
                    /* SPR 21785 FIXED START */
                    /* Code Remove*/
                    /* SPR 21785 FIXED START */
                    p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                    if (p_retback_t != NULL)
                    {
                        REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                        DEALLOCATE(p_retback_t);
                    }
                    /* SPR 21785 FIXED END */



                    return p_get_stat_t;

                }  
                /* Spr 17463 Fix End */ 
                }
                else
                    /* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;
            /* GPV PARTIAL PATH Change Start */		
			if (ret_end - ret_start > WAIT_LIMIT_GPN_RESP) //Spr 17463 - Timer increased during GPV partial path
                /* GPV PARTIAL PATH Change End */		
				/* SPR 19083 FIXED END */ 
            {
				TRACE(LOG_INFO , GLUE_LAYER, "Operation Timed Out");
            //leak fix
               /* SPR 20974 FIXED START */
                  /* SPR 21785 FIXED START */
				free_gpv_resp_buffer (p_get_stat_t->parameterList); //V9D
                  /* SPR 21785 FIXED END */
               /* SPR 20974 FIXED END */
                p_get_stat_t->eStatus = RPC_FAULT;
                p_get_stat_t->fault_code = INTERNAL_FAILURE;
                p_get_stat_t->parameterList = NULL;
                 /* MEMORY LEAK 31 DEC FIXED START */
                 /* SPR 21785 FIXED START */
            /* Code Remove */
                /* SPR 21785 FIXED END */
                /* SPR 21785 FIXED START */
                p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                if (p_retback_t != NULL)
                {
                    REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                    DEALLOCATE(p_retback_t);
                }
                 /* SPR 21785 FIXED END */

				DEALLOCATE(buffer);
                return p_get_stat_t;
            }
            //}
        }

        if (g_Index > 0)
        {
            cpeParamList_t->__size  = g_Index;
            p_get_stat_t->parameterList = (struct ParameterValueList *)cpeParamList_t;
            p_get_stat_t->eStatus  = RPC_SUCCESS;
			DEALLOCATE(buffer);
            /* SPR 21785 FIXED START */
            p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
            if (p_retback_t != NULL)
            {
                REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                DEALLOCATE(p_retback_t);
            }    
            /* SPR 21785 FIXED END */
			
            return p_get_stat_t;
        }
        /* Spr 17463 Fix Start */
        /* Code Removed */
        /* Spr 17463 Fix End */ 
    }
    /* GPA Change Start */
    /* SPR 17625 FIXED START */
    if (etype == RPC_GETPARAMETERATTRIBUTE)
    {
        UInt8 gpa_end_mark = 0;
        //coverity fix
		p_get_stat_attr_t = (GetParamAttrRes *)ALLOCATE(sizeof (GetParamAttrRes) );
        if (NULL == p_get_stat_attr_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(buffer);
            return NULL;
        }

        memset(p_get_stat_attr_t , '\0' , sizeof(GetParamAttrRes));


		cpeParamListA_t = (sParameterList *)ALLOCATE(sizeof(sParameterList) );
        if (NULL == cpeParamListA_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(p_get_stat_attr_t);
			p_get_stat_attr_t = NULL;
			DEALLOCATE(buffer);
            return NULL;
        }

        memset(cpeParamListA_t  , '\0' , sizeof (sParameterList));

		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;

		TRACE(LOG_CRITICAL , GLUE_LAYER, " Start Time for GET PARAMETER ATTRIBUTE: %ld", ret_start);
		/* SPR 19083 FIXED END */ 
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        timeval.tv_usec = 500000;
        while (1)
        {
            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);

            memset(buffer , '\0', MAX_RECV_BUFF );

            if (result < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");
            }
            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
                /* sg wait_limit change start */
				if (ret_end - ret_start > 240)
                    /* sg wait_limit change end */
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", ret_end);
					/* SPR 19083 FIXED END */ 
                    p_get_stat_attr_t->eStatus = RPC_FAULT;
                    p_get_stat_attr_t->fault_code = INTERNAL_FAILURE;
                    p_get_stat_attr_t->parameterList = NULL;
					DEALLOCATE(buffer);
                    return p_get_stat_attr_t;
                }
                else
                {
                    TR069_USLEEP(500);
                    continue;
                }
            }
            /* Coverity 56617 + */
            buffer[MAX_RECV_BUFF - 1] = '\0';
            /* Coverity 56617 - */
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Total Byte Received:%d",byte_read);
            byte_read = 0;  
            api_idr = get_word_from_header((U8 *) (buffer) + 6);
            TRACE(LOG_CRITICAL , GLUE_LAYER, "api id for get parameter attribute %d ",api_idr);
            if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                /*send hearbeat response to sysctrl */
                if(TRUE != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                }

            }

            if (api_idr == OAM_NOTIFICATION_IND)
	    {
		    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
		    /* SPR 21588 FIXED START  */
		    /* Code removed */
		    /* SPR 21588 FIXED END  */
				g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF );
		    if (g_notif_buffer)
		    {
			    memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
			    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
			    /* SPR 21588 FIXED START  */
			    TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
			    ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
			    g_notif_buffer = NULL;
			    /* SPR 21588 FIXED END */

		    }
	    }

            l_rec_trans_id = get_word_from_header(buffer);
            if(0 != l_rec_trans_id)
            {
                g_transD_t.trans_id = l_rec_trans_id;
            }
            else
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
            }

            if (api_idr == OAM_GET_PARAMETER_ATTRIBUTES_RESP)
            {
                /* coverity 96629 fixed /code removed */
                UInt16 temp=0,temp_t=0;
                //Decode Set parameter attribute Response
                l_offset = 0 ;
                status_resp =0;
                param_notification = 0;
                if (cpeParamListA_t == NULL)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Internal Error in OAM_GET_PARAMETER_ATTRIBUTES_RESP");
                    exit(0);
                }

                //cpeParamAttrbArr = cpeParamListA_t->__ptrParameterAttributeStruct;

                //Skip 12  byte API header
                l_offset  += 12;

                //Status
                memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));
                l_offset += 1;

                if (status_resp == 1)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"OAM_GET_PARAMETER_ATTRIBUTES_RESP Failure");
                    /*Failure*/
                    //l_offset += sizeof(UInt8);
                    p_get_stat_attr_t->eStatus = RPC_FAULT;
                    /*
                       memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                       l_err_code = temp_l_len & 0x00FF;
                       l_err_code = l_err_code << 8;
                       memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                       l_err_code = l_err_code | (temp_l_len & 0x00FF);
                       */

                    memcpy(&temp, buffer + l_offset , sizeof(UInt16));
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"ERROR CODE:%x & %x", temp, 9005);
                    temp_t = ntohs(temp);
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"ERROR CODE 1:%d", temp_t);

                    p_get_stat_attr_t->fault_code = temp_t;
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"OAM_GET_PARAMETER_ATTRIBUTES_RESP Failure with ERROR CODE:%d", p_get_stat_attr_t->fault_code);
                    l_offset += 2; // error code

                    //l_offset += 4; //Reserved Bytes

                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));

                    TRACE(LOG_CRITICAL , GLUE_LAYER,"OAM_GET_PARAMETER_ATTRIBUTES_RESP Failure with ERR STR LEN:%d",temp_l_len);
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"OAM_GET_PARAMETER_ATTRIBUTES_RESP Failure with ERR STR :%s",buffer + l_offset+1);

                    l_offset += temp_l_len + 1;
                    p_get_stat_attr_t->parameterList = NULL;

					DEALLOCATE(buffer);
                    return p_get_stat_attr_t;
                }
                else
                    /*Success*/
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"OAM_GET_PARAMETER_ATTRIBUTES_RESP Success");
                    p_get_stat_attr_t->eStatus = RPC_SUCCESS;

                    /* Skip for Error code */
                    l_offset += sizeof(UInt16);

                    /* Skip Error length in GPA response */
                    //memcpy ( &l_len_err,  buffer + l_offset , sizeof(UInt8));
                    l_offset +=sizeof(UInt8);

                    /* Skip the error string len, in case of successful GPA response length will be 0 */
                    //l_offset += l_len_err;

                    /* Total number of parameters in GPA response */
                    memcpy ( &l_len,  buffer + l_offset , sizeof(UInt16));
                    l_offset +=sizeof(UInt16);

                    /*Completion Flag*/
                    gpa_end_mark = buffer[l_offset];
                    TRACE (LOG_CRITICAL , GLUE_LAYER, "gpa_end_mark =%d ",gpa_end_mark);
                    l_offset +=1; /* 1 End Marking */
                    l_offset +=1; /* 1 reserved */

                    num_param = ntohs(l_len);

                    //cpeParamListA_t->__size = num_param;

                    TRACE(LOG_CRITICAL , GLUE_LAYER,"No of parameter : %d",num_param);
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"cpeParamListA_t->__size value: %d",cpeParamListA_t->__size);

					/*cpeParamAttrbArr  = (ParameterAttributeStruct **)ALLOCATE(
                      sizeof(ParameterAttributeStruct*) * (num_param) , NULL);*/


					cpeParamAttrbArr  = (ParameterAttributeStruct **)TR_REALLOC(cpeParamAttrbArr,
                            (sizeof(ParameterAttributeStruct*) * (num_param + cpeParamListA_t->__size)));
                    if(cpeParamAttrbArr == NULL)
                    {
                        TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, could not allocate storage form MsgHandle_FD", __FUNCTION__);

                        /* SPR 22314 Start */
                        DEALLOCATE(buffer);
                        /* SPR 22314 End */
                        return NULL;
                    }
                    cpeParamListA_t->__size += num_param;
                    TRACE(LOG_CRITICAL , GLUE_LAYER,"No of parameter : %d",cpeParamListA_t->__size);
                    cpeParamListA_t->__ptrParameterAttributeStruct = cpeParamAttrbArr;
                    /* TR069 Crash Fix */
                    g_Index = 0;
                    /* TR069 Crash Fix End*/

                    TRACE(LOG_CRITICAL , GLUE_LAYER,"l_count value: %d :: l_offset::%d",l_count,l_offset);

                    while(g_Index < num_param)
                    {
                        /* Length of parameter in GPA response */
                        memcpy ( &param_len,  buffer + l_offset , sizeof(UInt16));
                        TRACE (LOG_CRITICAL , GLUE_LAYER, "In, Start l_offset::%d",l_offset);
                        l_offset +=sizeof(UInt16);

                        param_len = ntohs(param_len);
                        TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Recieved Length of parameter :%d", __FUNCTION__ , param_len);

                        /* Name od parameter of parameter in GPA response */
						p_cwmpParamAttr_t = (ParameterAttributeStruct *) ALLOCATE( sizeof(ParameterAttributeStruct));
                        if (NULL == p_cwmpParamAttr_t)
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                            exit(0);
                        }
                        if (NULL != p_cwmpParamAttr_t)
                        {
                            /*Parameter Name*/
							p_cwmpParamAttr_t->Name = (char *) ALLOCATE( PARANT_NAME_GPA);

                            if (NULL == p_cwmpParamAttr_t->Name)
                            {
                                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                                exit(0);
                            }

                            /*Access List*/
							p_accessList = (ParamAccessList *) ALLOCATE(sizeof(ParamAccessList));
                            if(NULL == p_accessList)
                            {
                                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                                exit(0);
                            }
                            else
                            {
								p_accessList->__ptrstring = (enum AccessListValueType *) ALLOCATE(sizeof(enum AccessListValueType));
                                if(NULL == p_accessList->__ptrstring)
                                {
                                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                                    exit(0);
                                }
                            }

                            memcpy (p_cwmpParamAttr_t->Name ,  buffer + l_offset , param_len);
                            TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Recieved %s", __FUNCTION__, p_cwmpParamAttr_t->Name);
                            remove_brackets((UInt8 *)p_cwmpParamAttr_t->Name);
                            l_offset += 256;

                            memcpy (&param_notification, buffer + l_offset , sizeof(UInt8)) ;


                            //TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification1= %c", __FUNCTION__, *(buffer + l_offset-1));
                            //TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification2= %c", __FUNCTION__, *(buffer + l_offset-2));

                            TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification= %d :: %c", __FUNCTION__, param_notification,*(buffer + l_offset));

                            //TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification3= %c :: %d", __FUNCTION__, *(buffer + l_offset+1),*(buffer + l_offset+1));
                            //TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification4= %c", __FUNCTION__, *(buffer + l_offset+2));
                            //TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification5= %c", __FUNCTION__, *(buffer + l_offset+3));

                            switch(param_notification)
                            {
                                case ParameterAttributeNotificationValueType__0:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__0;
                                break;

                                case ParameterAttributeNotificationValueType__1:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__1;
                                break;
                                case ParameterAttributeNotificationValueType__2:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__2;
                                break;
                                case ParameterAttributeNotificationValueType__3:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__3;
                                break;
                                case ParameterAttributeNotificationValueType__4:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__4;
                                break;
                                case ParameterAttributeNotificationValueType__5:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__5;
                                break;
                                case ParameterAttributeNotificationValueType__6:
                                p_cwmpParamAttr_t->Notification = ParameterAttributeNotificationValueType__6;
                                break;
                                default:
                                TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Invalid Notification= %d",__FUNCTION__, param_notification);
                            }
                            /* Notification [1]*/
                            l_offset += sizeof(UInt8);
                            TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Notification= %d", __FUNCTION__, p_cwmpParamAttr_t->Notification);
                            
							/* SPR 21563 FIXED START */
                            *p_accessList->__ptrstring = AccessListValueType__Subscriber;
							/* SPR 21563 FIXED END */
							TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Access Type:%d",__FUNCTION__,p_accessList->__ptrstring);
                            p_accessList->__size = 1;




                        }
                        memcpy (&accesslist_len, buffer + l_offset , 2) ;
                        /* Access List Len [2]*/
                        l_offset += sizeof(UInt16);

                        accesslist_len = ntohs(accesslist_len);
                        TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , accesslist_len= %d",__FUNCTION__, accesslist_len);
                        /* Access List [64] */
                        l_offset += 64;


                        p_cwmpParamAttr_t->AccessList = p_accessList;

                        cpeParamAttrbArr [l_count] = p_cwmpParamAttr_t;
                        g_Index++;
                        l_count++;
                    }
                    TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , outside while",__FUNCTION__);
                }

            }
            else
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;

			if (ret_end - ret_start > WAIT_LIMIT)
				/* SPR 19083 FIXED END */ 
            {
                p_get_stat_attr_t->eStatus = RPC_FAULT;
                p_get_stat_attr_t->fault_code = INTERNAL_FAILURE;
                p_get_stat_attr_t->parameterList = NULL;
				DEALLOCATE(buffer);
                return p_get_stat_attr_t;
            }

            /* GPA Fix Start */
            if ((gpa_end_mark == 0) && (l_count > 0))
                /* GPA Fix End */
            {
                TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , g_Index %d",__FUNCTION__, g_Index);
                TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = FALSE, All GPA responses received");
                cpeParamListA_t->__size  = l_count;

                TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Final No. of PArameter %d",__FUNCTION__,cpeParamListA_t->__size);
                p_get_stat_attr_t->parameterList = (struct ParameterAttributeList *)cpeParamListA_t;


                //TRACE (LOG_CRITICAL , GLUE_LAYER, "In , Name:%s",*((p_get_stat_attr_t->parameterList)->__ptrParameterAttributeStruct[0])->Name);
                //TRACE (LOG_CRITICAL , GLUE_LAYER, "In , Notification: %d",*((p_get_stat_attr_t->parameterList)->__ptrParameterAttributeStruct[0])->Notification);


                p_get_stat_attr_t->eStatus  = RPC_SUCCESS;
                TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Step2",__FUNCTION__);
				DEALLOCATE(buffer);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Step3",__FUNCTION__);
                return p_get_stat_attr_t;
            }
			/* SPR 19264 FIXED START */
			else if (TR069_ZERO == num_param) 
			{
			        TRACE (LOG_CRITICAL , GLUE_LAYER, "ZERO PARAMETER RECIVED IN GPA RESPONSE: \n");
				p_get_stat_attr_t->eStatus = RPC_SUCCESS;
				/* SPR 21563 FIXED START */
				Free_GPA_Parameters((sParameterList *)p_get_stat_attr_t->parameterList);
				/* SPR 21563 FIXED END */
				p_get_stat_attr_t->parameterList = NULL;
				DEALLOCATE(buffer);
				return p_get_stat_attr_t;
         		}

			/* SPR 19264 FIXED END */
            else 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = TRUE, expected more GPA responses ");
                continue;
            }    
        }
    }
    /* SPR 17625 FIXED END */
    /* GPA Change End */


    if (etype == RPC_SETPARAMETERVALUES)
    {
        //coverity fix
		p_set_stat_t = (StStatus *)ALLOCATE(sizeof(StStatus) );
        if(NULL == p_set_stat_t)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(buffer);
            return NULL;
        }
		/* SPR 19750 FIXED START */
		p_set_stat_t->eStatus  = RPC_SUCCESS;
		/* SPR 19750 FIXED END */


		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Start Time for SET PARAMETER: %ld", ret_start);
		/* SPR 19083 FIXED END */ 
        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        /* SPR 17880 START */
        while (!(ylEmpty(yList)) || (counter_loop < 2))
            //        while (!(ylEmpty(yList)))
            /* SPR 17880 END */
        {

            l_offset = 0 ;

            FD_ZERO(&set); 
            FD_SET(sockfd, &set);
            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if(result < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " Select Error");
            }

            if ( (byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len )) < 0)
            {
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT_GPN_RESP)//if (end_t - start_t > WAIT_LIMIT)   /*SPR 458 StartSPV failure correction */
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait over: end Time : %ld", ret_end);
					/* SPR 19083 FIXED END */ 
                    p_set_stat_t->eStatus = RPC_FAULT;
					stFault = (struct _cwmp__Fault *) ALLOCATE (sizeof(struct _cwmp__Fault) );
                    if (NULL == stFault)
                    {
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                        DEALLOCATE(buffer);
                        return NULL;
                    }
					FaultCode =  (char *) ALLOCATE ( FAULT_CODE_SIZE ) ;
                    if (NULL == FaultCode)
                    {
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                        DEALLOCATE (buffer);
                        return NULL;
                    }
					FaultString = (char *) ALLOCATE (FAULT_STRING_SIZE );
                    if (NULL == FaultString)
                    {
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                        DEALLOCATE (buffer);
                        return NULL;
                    }

                    /* SPR 22448 Start */
                    /* Code Deleted */
                    /* SPR 22448 End */
                    snprintf (FaultCode , INTERNAL_FAILURE_LEN , "%d",INTERNAL_FAILURE);
                    ENB_STRNCPY ((char *)FaultString , (const char *)INTERNAL_FAILURE_STR, FAULT_STRING_SIZE);
                    stFault->FaultCode = FaultCode;
                    stFault->FaultString = FaultString;
                    /* SPR 22448 Start */
                    /* Code Deleted */
                    /* SPR 22448 End */
                    stFault->SetParameterValuesFault = NULL;
                    stFault->__sizeSetParameterValuesFault = 0;
                    p_set_stat_t->stFault = stFault;
					DEALLOCATE (buffer);
                    return p_set_stat_t;
                }
                else
                {
                    /* SPR 17880 START */
                    // counter_loop++;
                    /* SPR 17880 END */
                    TR069_USLEEP(500);
                    continue;
                }
            }

            //if(buffer[0])
            {
                api_idr = get_word_from_header((U8 *) (buffer) + 6);
                /* SPR 17880 START */
                TRACE(LOG_CRITICAL , GLUE_LAYER, "api_addr%d", api_idr);
                if (api_idr == OAM_TR069_SET_PARAMETER_VALUE_INDICATION)
                {
                    if (decode_set_parameter_indication(buffer) != SUCCESS)
                    {
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved damaged set parameter indication");
                    }
                    counter_loop++;
                    continue;
                }
                /* SPR 17880 END */
                /*SPR 17627 FIXED START*/
                if (api_idr == TR069_OAM_HEARTBEAT_REQ)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                    /*send hearbeat response to sysctrl */
                    if(1 != send_heartbeat_resp())
                    {
                        /*log message response not sent*/
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                    }
                    else
                    {
                        /*log message response sent successfully */
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                    }

                }
                /*SPR 17627 FIXED END*/

                /*CSR 57877 Fix START */
                if (api_idr == OAM_NOTIFICATION_IND)
                    /*CSR 57877 Fix End*/
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
			/* SPR 21588 FIXED START  */
			/* Code removed */
			/* SPR 21588 FIXED END  */

			g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
			if (g_notif_buffer)
			{
				memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
				/* SPR 21588 FIXED START  */
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
				TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
				ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
				g_notif_buffer = NULL;
				/* SPR 21588 FIXED END */

			}
		}

                /*SPR #14479 Fixed Start */
                /*SPR #17674 Fixed Start */
                if(OAM_SET_PARAMETER_VALUES_RESP == api_idr){
                    /* SPR 17880 START */
                    counter_loop++;
                    /* SPR 17880 START */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "OAM_SET_PARAMETER_VALUES_RESP");

                    recv_trans_id = get_word_from_header(buffer);
                    g_transD_t.trans_id = recv_trans_id;
                    /*SPR #14479 Fixed End */

                    p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
                    if (p_retback_t != NULL)
                    {
						/* SPR 19083 FIXED START*/
						tr069_gettimeofday_wrapper(&start_t);
						ret_start = (time_t)start_t.tv_sec;
						/* SPR 19083 FIXED END */ 

                        REM_FROM_LIST( yList , (YLNODE *)p_retback_t);
						DEALLOCATE (p_retback_t);
                        p_retback_t = NULL;
                        l_offset += 12;

                        memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));
                        l_offset += sizeof(UInt8);
                        if (status_resp == 0)
                        {
                            ENB_GL_PRINTF ("\n\n\n   Recieved Trans id : %d And actual is : %d", recv_trans_id , g_url_trans);
                            if (g_parent_URL && g_url_trans == recv_trans_id && g_tmp_url_buffer)
                            {
                                //handle_set_url (g_tmp_url_buffer);
                                g_parent_URL = 0;
                                g_url_trans = 0;
                                g_Is_new_url = 1;

                        /* SPR 18262 FIXED START*/
                        UInt8 ABSPath4[CONFIG_PARAM_LEN] = {'\0'};
                        strcpy((char*)ABSPath4,(char *)ABSPath);

                        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath before concat of BOOTSTRAP_FILE_NAME %s ",ABSPath4 );
                        strncat((char *)ABSPath4 , (const char *)BOOTSTRAP_FILE_NAME, strlen(BOOTSTRAP_FILE_NAME));
                        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath after concat of BOOTSTRAP_FILE_NAME  %s ",ABSPath4 );

                                /* SPR 21629 FIXED START */    
                                /* Code Remove */
                                /* SPR 21629 FIXED END */    
                        /* SPR 18262 FIXED END*/ 

                            }
                            /* Raveen - Check if actual_status is set to committed but not applied,
                             * Send 1 as status code in RPC response */
                            UInt8    actual_status = 0;

                            l_offset += sizeof(UInt16);
                            memcpy(&actual_status,buffer+l_offset, sizeof(UInt8));
							/* SPR 19750 FIXED START */
							if(1 == actual_status || p_set_stat_t->eStatus == RPC_PARTIAL)
							/* SPR 19750 FIXED END */
                            {
                                p_set_stat_t->eStatus  = RPC_PARTIAL;
                            }
                            else
                            {
                                p_set_stat_t->eStatus  = RPC_SUCCESS;
                            }
                            p_set_stat_t->stFault = NULL;
                    /*SPR 18292 FIXED START*/
                    /* MEMORY LEAK 31 DEC FIXED START*/
							//DEALLOCATE(buffer);
                    //buffer = NULL;

                    /* MEMORY LEAK 31 DEC FIXED START*/

                   //	return p_set_stat_t;
                    
                    /*SPR 18292 FIXED END*/
                    /*multi spv change start */

                    if((ylEmpty(yList)))
                    {

                        /*SPR 18294 FIXED START*/
								DEALLOCATE(buffer);

                        /*SPR 18294 FIXED END*/
                        return p_set_stat_t;
                    }
                    /*multi spv change  end */ 
                }
                
                        if (status_resp == 1)
                        {
							stTmpArr = ( struct _cwmp__Fault_SetParameterValuesFault *)ALLOCATE(sizeof(struct _cwmp__Fault_SetParameterValuesFault) );
                            if(NULL == stTmpArr) //coverity fix
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                exit(0);
                            }
							stFault = (struct _cwmp__Fault *) ALLOCATE (sizeof(struct _cwmp__Fault) );
                            if (NULL == stFault)
                            {
                                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
								DEALLOCATE (buffer);
                                return NULL;
                            }
                            l_f_ind = 0;
                            p_set_stat_t->eStatus  =  RPC_FAULT;
                            memcpy ( &temp_l_len, buffer+l_offset , sizeof(U8));
                            l_ret_code = temp_l_len & 0x00FF;
                            l_ret_code = l_ret_code<<8;
                            memcpy ( &temp_l_len, buffer+l_offset+1, sizeof(U8));
                            l_ret_code = l_ret_code | (temp_l_len & 0x00FF);

							ParameterName = ( char *) ALLOCATE ( MAX_PARAM_NAME_SIZE ) ;
                            if(NULL == ParameterName) //coverity fix
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                exit(0);
                            }
							FaultCode =  (char *) ALLOCATE ( FAULT_CODE_SIZE ) ;
                            if(NULL == FaultCode) //coverity fix
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                exit(0);
                            }
							FaultString = (char *) ALLOCATE (FAULT_STRING_SIZE );
                            /* SPR 18130 FIXED START */
                            memset(FaultString, 0, FAULT_STRING_SIZE);
                            /* SPR 18130 FIXED END */
                            if(NULL == FaultString) //coverity fix
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                exit(0);
                            }

                            if ( ParameterName!= NULL && FaultCode != NULL && FaultString != NULL)
                            {

                                snprintf(FaultCode, FAULT_CODE_SIZE, "%d", l_ret_code);

                                l_offset += 6;
                                memcpy(&error_str_len, buffer+l_offset, sizeof(UInt8));
                                l_offset += 1;

                                memcpy(FaultString, buffer+l_offset, error_str_len);
                                /* SPR 18130 FIXED START */
                                TRACE (LOG_CRITICAL, GLUE_LAYER, "fault string  %s faultstring length is%d", FaultString, error_str_len);
                                l_offset += (3+error_str_len);
                                memcpy(&node_name_length, buffer+l_offset,sizeof(UInt8));
                                l_offset +=1;
								//node_name = (UInt8 *)ALLOCATE(sizeof(char)*(node_name_length));
                                memcpy(ParameterName, buffer+l_offset, node_name_length);
                                //p_set_stat_t->node_name_length = node_name_length;
                                //p_set_stat_t->node_name = node_name;
                                TRACE (LOG_CRITICAL, GLUE_LAYER, "node name  %s and node length is %d\n", node_name, node_name_length);
                                //ENB_STRNCPY ((char *)ParameterName,(const char *)node_name , node_name_length);
                                /* SPR 18130 FIXED END */
                                stTmpArr[l_f_ind].ParameterName = ParameterName;
                                stTmpArr[l_f_ind].FaultCode= FaultCode;
                                stTmpArr[l_f_ind].FaultString = FaultString;
                                l_f_ind++;
                                stFault->SetParameterValuesFault = stTmpArr;
                                stFault->__sizeSetParameterValuesFault = l_f_ind;

								FaultCode =  (char *) ALLOCATE ( FAULT_CODE_SIZE ) ;
                                if(NULL == FaultCode) //coverity fix
                                {
                                    TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                    exit(0);
                                }
								FaultString = (char *) ALLOCATE (FAULT_STRING_SIZE );
                                if(NULL == FaultString) //coverity fix
                                {
                                    TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                                    exit(0);
                                }

                                if (FaultCode && FaultString)
                                {
                                    snprintf (FaultCode , DEFAULT_FAULT_CODE_LEN , "%d",DEFAULT_FAULT_CODE);
                                    ENB_STRNCPY ((char *)FaultString , (const char *)DEFAULT_FAULT_STR, FAULT_STRING_SIZE);
                                    stFault->FaultCode = FaultCode;
                                    stFault->FaultString = FaultString;
                                }


                                p_set_stat_t->stFault = stFault;
								        DEALLOCATE (buffer);
                                return p_set_stat_t;

                            }
                        }

                    }

                }
                /*SPR #17674 Fixed End */	
                else
					/* SPR 19083 FIXED START*/
					tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;


				if ( ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    p_set_stat_t->eStatus = RPC_FAULT;
					stFault = (struct _cwmp__Fault *) ALLOCATE (sizeof(struct _cwmp__Fault) );
                    if(NULL == stFault) //coverity fix
                    {
                        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                        exit(0);
                    }
					FaultCode =  (char *) ALLOCATE ( FAULT_CODE_SIZE ) ;
                    if(NULL == FaultCode) //coverity fix
                    {
                        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                        exit(0);
                    }
					FaultString = (char *) ALLOCATE (FAULT_STRING_SIZE );
                    if(NULL == FaultString) //coverity fix
                    {
                        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                        exit(0);
                    }

                    if ((NULL == stFault) )
                    {
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
						DEALLOCATE (buffer);
                        return NULL;
                    }

                    if (FaultCode && FaultString)
                    {
                        snprintf (FaultCode , INTERNAL_FAILURE_LEN , "%d",INTERNAL_FAILURE);
                        ENB_STRNCPY ((char *)FaultString ,(const char *) INTERNAL_FAILURE_STR, FAULT_STRING_SIZE);
                        stFault->FaultCode = FaultCode;
                        stFault->FaultString = FaultString;
                    }
                    /* Memory leak 21 jan fixed start */
                    DEALLOCATE(p_set_stat_t);
                    /* Memory leak 21 jan fixed end */
                    stFault->SetParameterValuesFault = NULL;
                    stFault->__sizeSetParameterValuesFault = 0;
                    p_set_stat_t->stFault = stFault;
					DEALLOCATE (buffer);
                    return p_set_stat_t;
                }

            }

            /*joshi change start */
			//DEALLOCATE (buffer);
            //return p_set_stat_t;
            /*joshi change end */
        }
    }
    /*SPR 17625 FIXED START*/
    if (etype == RPC_SETPARAMATTRIBUTE)
    {

		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		/* SPR 19083 FIXED END */ 
		p_set_attr_resp_t = ALLOCATE(sizeof(sSetParamAttrRes_t) );
        if (NULL == p_set_attr_resp_t)
        {
            //coverity fix
			DEALLOCATE(buffer);
            return NULL;
        }
        /* SPR 22448 Start */
        memset(p_set_attr_resp_t, 0, sizeof(sSetParamAttrRes_t));
        /* SPR 22448 End */
        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        while(1)
        {

            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                //Select error
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");

            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Waiting for Set parameter attribute Response");
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start> WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //Internal Failure
                    p_set_attr_resp_t->eStatus = RPC_FAULT;
                    p_set_attr_resp_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_set_attr_resp_t;
                }
                else
                {
                    TR069_USLEEP(500);
                    continue;
                }
            }

            api_idr = get_word_from_header((U8 *) (buffer) + 6);
            /*joshi change start */

            /*CSR 57877 Fix START */
            if (api_idr == OAM_NOTIFICATION_IND)
                /*CSR 57877 Fix End*/
	    {
		    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
		    /* SPR 21588 FIXED START  */
		    /* Code removed */
		    /* SPR 21588 FIXED END  */

		    g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
		    if (g_notif_buffer)
		    {
			    memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
			    /* SPR 21588 FIXED START  */
			    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
			    TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
			    ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
			    g_notif_buffer = NULL;
			    /* SPR 21588 FIXED END */

		    }
	    }
           

            /* SPR 19152 FIXED START */
            if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_DETAIL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                /*send hearbeat response to sysctrl */
                if(TR069_ONE != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_DETAIL , GLUE_LAYER, "Failed to send hearbeat response");
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_DETAIL , GLUE_LAYER, "Hearbeat response sent successfully");
                }

            }
            /* SPR 19152 FIXED END */

            if (api_idr == OAM_SET_PARAMETER_ATTRIBUTE_RESP)
            {
                Uint16 l_err_code = 0;
                //Decode Set parameter attribute Response
                l_offset = 0 ;
                status_resp =0;

                //Skip 12  byte API header
                l_offset  += 12;

                //Status
                memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));

                if (status_resp == 0)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Set parameter attribute Response received with success");
                    p_set_attr_resp_t->eStatus = RPC_SUCCESS;
                    p_set_attr_resp_t->fault_string = NULL;
					DEALLOCATE(buffer);
                    return p_set_attr_resp_t;
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Set parameter attribute Response received with fault code");
                    l_offset += sizeof(UInt8);
                    p_set_attr_resp_t->eStatus = RPC_FAULT;
                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                    l_err_code = temp_l_len & 0x00FF;
                    l_err_code = l_err_code << 8;
                    memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                    l_err_code = l_err_code | (temp_l_len & 0x00FF);

                    p_set_attr_resp_t->fault_code = l_err_code;

                    l_offset += 2; // error code

                    l_offset += 4; //Reserved Bytes

                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));

                    l_offset += temp_l_len + 1;

					DEALLOCATE(buffer);
                    return p_set_attr_resp_t;

                }
            }
            else
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;

			if (ret_end - ret_start > WAIT_LIMIT)
				/* SPR 19083 FIXED END */ 
            {
                //  Internal Failure;
                p_set_attr_resp_t->eStatus = RPC_FAULT;
                p_set_attr_resp_t->fault_code = INTERNAL_FAILURE;
				DEALLOCATE(buffer);
                return p_set_attr_resp_t;
            }
        }

    }
    /*SPR 17625 FIXED END*/


    if (etype == RPC_DELETEOBJECT)
    {

		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		/* SPR 19083 FIXED END */ 

		p_delete_obj_res_t = ALLOCATE(sizeof(sDeleteObjectStatus_t) );
        if (NULL == p_delete_obj_res_t)
        {
            //coverity fix
			DEALLOCATE(buffer);        
            return NULL;
        }
        /* SPR 22448 Start */
        memset(p_delete_obj_res_t, 0, sizeof(sDeleteObjectStatus_t));
        /* SPR 22448 End */

        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        while(1)
        {

            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                //Select error
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");

            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Waiting for delete obj Response");
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //Internal Failure
                    p_delete_obj_res_t->eStatus = RPC_FAULT;
                    p_delete_obj_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_delete_obj_res_t;
                }
                else
                {
                    TR069_USLEEP(500);
                    continue;
                }
            }
            //	if(buffer[0])
            {
                api_idr = get_word_from_header((U8 *) (buffer) + 6);
                /*SPR 17627 FIXED START*/
                if (api_idr == TR069_OAM_HEARTBEAT_REQ)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                    /*send hearbeat response to sysctrl */
                    if(1 != send_heartbeat_resp())
                    {
                        /*log message response not sent*/
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                    }
                    else
                    {
                        /*log message response sent successfully */
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                    }

                }
                /*SPR 17627 FIXED END*/
                /*joshi change start */

                /*CSR 57877 Fix START */
                if (api_idr == OAM_NOTIFICATION_IND)
                    /*CSR 57877 Fix End*/
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
			/* SPR 21588 FIXED START  */
			/* Code removed */
			/* SPR 21588 FIXED END  */

			g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
			if (g_notif_buffer)
			{
				memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
				/* SPR 21588 FIXED START  */
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
				TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
				ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
				g_notif_buffer = NULL;
				/* SPR 21588 FIXED END */

			}
		}
               
                /*joshi change end */

                if (api_idr == OAM_DELETE_OBJECT_RESP)
                {
                    Uint16 l_err_code = 0;
                    //Decode Add object Response
                    l_offset = 0 ;
                    status_resp =0;

                    //Skip 12  byte API header
                    l_offset  += 12;

                    //Status
                    memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));

                    if (status_resp == 0)
                    {
                        p_delete_obj_res_t->eStatus = RPC_SUCCESS;
						DEALLOCATE(buffer);
                        return p_delete_obj_res_t;
                    }
                    else
                    {
                        l_offset += sizeof(UInt8);
                        p_delete_obj_res_t->eStatus = RPC_FAULT;
                        memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                        l_err_code = temp_l_len & 0x00FF;
                        l_err_code = l_err_code << 8;
                        memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                        l_err_code = l_err_code | (temp_l_len & 0x00FF);

                        p_delete_obj_res_t->fault_code = l_err_code;

                        l_offset += 2; // error code

                        l_offset += 4; //Reserved Bytes

                        memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));

                        l_offset += temp_l_len + 1;

						DEALLOCATE(buffer);
                        return p_delete_obj_res_t;

                    }
                }
                else
					/* SPR 19083 FIXED START*/
					tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //  Internal Failure;
                    p_delete_obj_res_t->eStatus = RPC_FAULT;
                    p_delete_obj_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_delete_obj_res_t;
                }
            }
        }
    }
    if (etype == RPC_CONTROL_CMD)
    {
        /*SPR 16294 Code Remove*/		
		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		/* SPR 19083 FIXED END */ 
		p_cntrl_cmd_res_t = ALLOCATE(sizeof(sControl_CmdStatus_t) );

        if (NULL == p_cntrl_cmd_res_t)
        {
            //coverity fix
			DEALLOCATE(buffer);	
            return NULL;
        }
        /* SPR 22448 Start */
        memset(p_cntrl_cmd_res_t, 0, sizeof(sControl_CmdStatus_t));
        /* SPR 22448 End */
        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        while(1)
        {

            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                //Select error
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");

            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Waiting for RPC_CONTROL_CMD response");
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end- ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //Internal Failure
                    p_cntrl_cmd_res_t->eStatus = RPC_FAULT;
                    p_cntrl_cmd_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_cntrl_cmd_res_t;
                }
                else
                {
                    TR069_USLEEP (5000);
                    continue;
                }
            }
            //	if(buffer[0])
            {
                api_idr = get_word_from_header((U8 *) (buffer) + 6);
                /*SPR 17627 FIXED START*/
                if (api_idr == TR069_OAM_HEARTBEAT_REQ)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                    /*send hearbeat response to sysctrl */
                    if(1 != send_heartbeat_resp())
                    {
                        /*log message response not sent*/
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                    }
                    else
                    {
                        /*log message response sent successfully */
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                    }

                }
                /*joshi change start */

                /*CSR 57877 Fix START */
                if (api_idr == OAM_NOTIFICATION_IND)
                    /*CSR 57877 Fix End*/
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
			/* SPR 21588 FIXED START  */
			/* Code removed */
			/* SPR 21588 FIXED END  */

			g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
			if (g_notif_buffer)
			{
				memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
				/* SPR 21588 FIXED START  */
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
				TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
				ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
				g_notif_buffer = NULL;
				/* SPR 21588 FIXED END */

			}
		}
               

                /*joshi change end */

                /*SPR 17627 FIXED END*/

                /* SPR 11921 Fix Start*/
                if (api_idr == OAM_CONTROL_COMMAND_RESP)
                    /* SPR 11921 Fix End*/
                {
                    Uint16 l_err_code = 0;
                    //Decode Add object Response
                    l_offset = 0 ;
                    status_resp =0;

                    //Skip 12  byte API header
                    l_offset  += 12;

                    //Status
                    memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));

                    if (status_resp == 0)
                    {
                        p_cntrl_cmd_res_t->eStatus = RPC_SUCCESS;
						DEALLOCATE(buffer);
                        return p_cntrl_cmd_res_t;
                    }
                    else
                    {
                        l_offset += sizeof(UInt8);
                        p_cntrl_cmd_res_t->eStatus = RPC_FAULT;
                        memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                        l_err_code = temp_l_len & 0x00FF;
                        l_err_code = l_err_code << 8;
                        memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                        l_err_code = l_err_code | (temp_l_len & 0x00FF);

                        p_cntrl_cmd_res_t->fault_code = l_err_code;

                        l_offset += 2; // error code

                        l_offset += 4; //Reserved Bytes

                        memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));

                        l_offset += temp_l_len + 1;

                        /* Spr 16890 Fix Start */
						FaultString = (char *) ALLOCATE (FAULT_STRING_SIZE );
                        memcpy(FaultString, buffer+l_offset, temp_l_len);
                        p_cntrl_cmd_res_t->fault_string = FaultString;
                        /* Spr 16890 Fix End */  
						DEALLOCATE(buffer);
                        return p_cntrl_cmd_res_t;

                    }
                }
                else
					/* SPR 19083 FIXED START*/
					tr069_gettimeofday_wrapper(&end_t);
				ret_end  = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //  Internal Failure;
                    p_cntrl_cmd_res_t->eStatus = RPC_FAULT;
                    p_cntrl_cmd_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_cntrl_cmd_res_t;
                }
            }
        }
    }




    if (etype == RPC_ADDOBJECT)
    {

		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		/* SPR 19083 FIXED END */ 

		p_add_obj_res_t = ALLOCATE( sizeof(sAddObjectStatus_t) );

        if (NULL == p_add_obj_res_t)
        {
			DEALLOCATE(buffer);        
            return NULL;
        }
        /* SPR 22448 Start */
        memset(p_add_obj_res_t, 0, sizeof(sAddObjectStatus_t));
        /* SPR 22448 End */
        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

        while(1)
        {

            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                //Select error
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");

            }

            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Waiting for  Add object Response");
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				if (ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //Internal Failure 
                    p_add_obj_res_t->eStatus = RPC_FAULT;   
                    p_add_obj_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_add_obj_res_t;
                }
                else    
                {
                    TR069_USLEEP(5000);
                    //continue; 
                }
            }
            //if(buffer[0])
            {

                api_idr = get_word_from_header((U8 *) (buffer) + 6);
                /*SPR 17627 FIXED START*/
                if (api_idr == TR069_OAM_HEARTBEAT_REQ)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                    /*send hearbeat response to sysctrl */
                    if(1 != send_heartbeat_resp())
                    {
                        /*log message response not sent*/
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                    }
                    else
                    {
                        /*log message response sent successfully */
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                    }

                }
                /*SPR 17627 FIXED END*/

                // Code added to support Notification in between RPC's
                /*CSR 57877 Fix START */
                if (api_idr == OAM_NOTIFICATION_IND)
                    /*CSR 57877 Fix End*/
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
			/* SPR 21588 FIXED START  */
			/* Code removed */
			/* SPR 21588 FIXED END  */

			g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
			if (g_notif_buffer)
			{
				memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
				/* SPR 21588 FIXED START  */
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
				TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
				ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
				g_notif_buffer = NULL;
				/* SPR 21588 FIXED END */

			}
		}

                if (api_idr == OAM_ADD_OBJECT_RESP)
                {
                    Uint16 l_err_code = 0;
                    //	U8 temp_l_code = 0;
                    //Decode Add object Response
                    l_offset = 0 ;
                    status_resp =0;

                    //Skip 12  byte API header 
                    l_offset  += 12;

                    //Status 
                    memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));

                    if (status_resp == 0)
                    {
                        p_add_obj_res_t->eStatus = RPC_SUCCESS;
                    }
                    else
                    {
                        p_add_obj_res_t->eStatus = RPC_FAULT;
                    }
                    l_offset += sizeof(UInt8);
                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                    l_err_code = temp_l_len & 0x00FF;
                    l_err_code = l_err_code << 8;
                    memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                    l_err_code = l_err_code | (temp_l_len & 0x00FF);

                    p_add_obj_res_t->fault_code = l_err_code;

                    l_offset += 2; // error code 

                    l_offset += 4; //Reserved Bytes

                    //temp_l_code = 0;

                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));

                    l_offset += temp_l_len + 1;

                    //temp_l_code = 0;      

                    memcpy(&temp_l_len, buffer + l_offset , sizeof(UInt8));
                    p_add_obj_res_t->instance_number = temp_l_len & 0x00FF;
                    p_add_obj_res_t->instance_number = p_add_obj_res_t->instance_number << 8;
                    memcpy(&temp_l_len, buffer+l_offset+1, sizeof(UInt8));
                    p_add_obj_res_t->instance_number = p_add_obj_res_t->instance_number | (temp_l_len & 0x00FF);

					DEALLOCATE(buffer);
                    return p_add_obj_res_t; 
                }
                else
					/* SPR 19083 FIXED START*/
					tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;

				if (ret_end - ret_start > WAIT_LIMIT)
					/* SPR 19083 FIXED END */ 
                {
                    //  Internal Failure;
                    p_add_obj_res_t->eStatus = RPC_FAULT;
                    p_add_obj_res_t->fault_code = INTERNAL_FAILURE;
					DEALLOCATE(buffer);
                    return p_add_obj_res_t;
                }
            }
        }

    }

    /* Spr 16990 Fix Start */
    if (etype == RPC_GETPARAMETERNAME)
    {

        /*  coverity 96630/96650/96679/96643/96641 fixed start*/
		if((p_gpn_res_t = ALLOCATE(sizeof(GetParameterNamesStatus_t))) == NULL)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			DEALLOCATE(buffer);
            return NULL;
        }
        else
        {
			if( (p_gpn_res_t->parameterList = ALLOCATE(sizeof(struct ParameterInfoList))) == NULL)
            {
				DEALLOCATE(p_gpn_res_t);
                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
				DEALLOCATE(buffer);
                return NULL;
            }
            else
            {
				if( (p_gpn_fault_res_t = ALLOCATE(sizeof(GetParameterNamesStatus_t))) == NULL)
                {
					DEALLOCATE(p_gpn_res_t->parameterList);
					DEALLOCATE(p_gpn_res_t);
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
					DEALLOCATE(buffer);
                    return NULL;
                }
            }

        }

        /*  coverity 96630/96650/96679 fixed end */

        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        timeval.tv_sec = 0;
        timeval.tv_usec = 500000;

		/* SPR 19083 FIXED START*/
		tr069_gettimeofday_wrapper(&start_t);
		ret_start = (time_t)start_t.tv_sec;
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Start Time for GET PARAMETER  NAME: %ld", ret_start);
		/* SPR 19083 FIXED END */ 

        while(1)
        {
            memset(buffer,'\0',MAX_RECV_BUFF);
            result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
            if (result < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");

            }
            if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Waiting for  Get Parameter Name Response");
				/* SPR 19083 FIXED START*/
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
                /* GPN Change Start */
				if (ret_end - ret_start > WAIT_LIMIT_GPN_RESP)
					/* SPR 19083 FIXED END */ 
                    /* GPN Change End */		
                {
                    //Internal Failure 
                    free_gpn_resp_buffer(p_gpn_res_t);
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", end_t); 
                    p_gpn_fault_res_t->eStatus = RPC_FAULT;
                    p_gpn_fault_res_t->fault_code = INTERNAL_FAILURE;
					FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE);
                    strncpy(FaultString, "Internal Failure"  ,FAULT_STRING_SIZE);
                    /* SPR 21785 FIXED START */
                    p_gpn_fault_res_t->fault_string = FaultString;
                    /* SPR 21785 FIXED END */
					DEALLOCATE(buffer);
                    return p_gpn_fault_res_t;
                }
                else
                {
                    TR069_USLEEP(5000);
                    continue;
                }
            }
            api_idr = get_word_from_header((U8 *) (buffer) + 6);
            TRACE(LOG_CRITICAL , GLUE_LAYER, "api id for recieved while waiting GPN %d ",api_idr);

            if (api_idr == OAM_NOTIFICATION_IND)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Received Notification Request");
		/* SPR 21588 FIXED START  */
		/* code removed */
		/* SPR 21588 FIXED END */

		g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);

                /* SPR 21588 FIXED START  */
		if (g_notif_buffer)
		{
		    TRACE(LOG_INFO , GLUE_LAYER, "Adding notification into List GPV");
                    memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
		    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
                    ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
		    g_notif_buffer = NULL;

		}
                /* SPR 21588 FIXED END */
            }

            /* SPR 19152 FIXED START */
            if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_DETAIL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                /*send hearbeat response to sysctrl */
                if(TR069_ONE != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_DETAIL , GLUE_LAYER, "Failed to send hearbeat response");
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_DETAIL , GLUE_LAYER, "Hearbeat response sent successfully");
                }

            }
            /* SPR 19152 FIXED END */

            if (api_idr == OAM_GET_PARAMETER_NAME_RESP)
            {
                l_rec_trans_id = get_word_from_header(buffer);

                /* coverity 96651 fixed /code removed */
                l_offset = 0 ;
                status_resp =0;

                l_offset  += 12;

                memcpy(&status_resp,buffer+l_offset, sizeof(UInt8));

                if(gpn_trans_id == l_rec_trans_id)
                {
                    g_transD_t.trans_id = l_rec_trans_id;
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid trans ID Received");
                    /* Failure Case */
                    free_gpn_resp_buffer(p_gpn_res_t);
                    p_gpn_fault_res_t->eStatus = RPC_FAULT;
                    p_gpn_fault_res_t->fault_code = INTERNAL_FAILURE;
					FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE);
                    strncpy(FaultString, "Internal Failure"  ,FAULT_STRING_SIZE);
                    p_gpn_fault_res_t->fault_string = FaultString;
					DEALLOCATE(buffer);
                    gpn_trans_id = 0;  /*resetting the value */
                    return p_gpn_fault_res_t;
                }

                if (status_resp == 0)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "GPN Status = %d",status_resp);
                    /* Success Received Case */
                    if (GLUE_SUCCESS ==  decode_gpn_response(buffer ,(GetParameterNamesStatus_t *)p_gpn_res_t, &end_mark))
                    {
                        if (end_mark == 1)
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = TRUE, expected more GPN responses ");
                            continue; 
                        }
                        else
                        {
                            TRACE(LOG_CRITICAL , GLUE_LAYER, "end mark = FALSE ,all GPN response received");
                            gpn_trans_id = 0;  /*resetting the value */
							DEALLOCATE(p_gpn_fault_res_t);
							/* SPR 21563 FIXED START */
							DEALLOCATE(buffer);
							/* SPR 21563 FIXED END */
                            return p_gpn_res_t;
                        }
                    }
                    else
                    {
                        //  Internal Failure;
                        TRACE(LOG_CRITICAL , GLUE_LAYER, "Failure in decoding the GPN Response");
                        free_gpn_resp_buffer(p_gpn_res_t);
                        p_gpn_fault_res_t->eStatus = RPC_FAULT;
                        p_gpn_fault_res_t->fault_code = INTERNAL_FAILURE;
						FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE);
                        strncpy(FaultString, "Internal error"  ,FAULT_STRING_SIZE);
                        p_gpn_fault_res_t->fault_string = FaultString;
						DEALLOCATE(buffer);
                        gpn_trans_id = 0;  /*resetting the value */
                        return p_gpn_fault_res_t;

                    }
                } 
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "GPN Status = %d",status_resp);
                    /* Failure Case */
                    l_offset += 1; /* 1 Status Resp */

                    error_code = error_code | (UInt16) ((buffer)[l_offset] << 8);
                    error_code = error_code | (UInt16) ((buffer)[l_offset+1]);
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Error Code received in GPN RESP: %d",error_code);

                    l_offset +=2; /* error code */
                    l_offset +=4; /* reserved */
                    error_str_len = buffer[l_offset];
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Error String Len received in GPN RESP: %d",error_str_len);
                    l_offset += 1; /* 1 Error Str Len */ 
                    p_gpn_fault_res_t->eStatus = RPC_FAULT;
                    p_gpn_fault_res_t->fault_code = error_code; 
					FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE );
                    /*  coverity 96687 fixed start */
                    strncpy(FaultString, (char *)(buffer+l_offset), error_str_len);
                    /*  coverity 96687 fixed end */
                    p_gpn_fault_res_t->fault_string = FaultString;
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Error String Len received in GPN RESP: %s :: %s",FaultString,(p_gpn_fault_res_t->fault_string));
                    free_gpn_resp_buffer(p_gpn_res_t);
					DEALLOCATE(buffer);
                    gpn_trans_id = 0;  /*resetting the value */
                    return p_gpn_fault_res_t;
                }

            }
            /* GPN Change Start */
			/* SPR 19083 FIXED START */ 
			if (ret_end - ret_start > WAIT_LIMIT_GPN_RESP)
				/* SPR 19083 FIXED END */ 
                /* GPN Change End */		
            {
                //  Internal Failure;
                free_gpn_resp_buffer(p_gpn_res_t);
                p_gpn_fault_res_t->eStatus = RPC_FAULT;
                p_gpn_fault_res_t->fault_code = INTERNAL_FAILURE;
				FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE);
                strncpy(FaultString, "Internal error"  ,FAULT_STRING_SIZE);
                p_gpn_fault_res_t->fault_string = FaultString;
				DEALLOCATE(buffer);
                gpn_trans_id = 0;  /*resetting the value */
                return p_gpn_fault_res_t;
            }


        }

    }
    /* Spr 16990 Fix End */

    TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting  %s",__FUNCTION__); 
	DEALLOCATE (buffer);
	//DEALLOCATE (p_get_stat_t);
	//DEALLOCATE (cpeParamList_t);
    return NULL;
}


/******************************************************************************
 *   FUNCTION NAME  : construct_interface_api_header
 *   Inputs         : p_buffer,transaction_id,src_module_id,dst_module_id,api_id,api_buf_size
 *   Outputs        : none
 *   RETURNS        : none
 *   DESCRIPTION    : This function will construct api_header ,this header will appear at the 
 beginning of each meassage
 *******************************************************************************/
void
	construct_interface_api_header
(
 UInt8                  *p_header,      /*  interface header */
 UInt16                 transaction_id, /* Interface transaction identifier */
 UInt16                 src_module_id,  /* Source module identifier */
 UInt16                 dst_module_id,  /* Destination module identifier */
 UInt16                 api_id,         /* API identifier */
 UInt16                 api_buf_size    /* API buffer size */
 )
{


	/* transaction identifier (HI) */
	p_header[0] = (UInt8)((transaction_id & 0xFF00) >> 8);

	/* transaction identifier (LOW) */
	p_header[1] = (UInt8)(transaction_id & 0x00FF);

	/* from (HI) */
	p_header[2] = (UInt8)((src_module_id & 0xFF00) >> 8);

	/* from (LOW) */
	p_header[3] = (UInt8)(src_module_id & 0x00FF);

	/* to (HI) */
	p_header[4] = (UInt8)((dst_module_id & 0xFF00) >> 8);

	/* to (LOW) */
	p_header[5] = (UInt8)(dst_module_id & 0x00FF);

	/* api id (HI) */
	p_header[6] = (UInt8)((api_id & 0xFF00) >> 8);

	/* api id (LOW) */
	p_header[7] = (UInt8)(api_id & 0x00FF);

	/* api size (HI) */
	p_header[8] = (UInt8)((api_buf_size & 0xFF00) >> 8);

	/* api size (LOW) */
	p_header[9] = (UInt8)(api_buf_size & 0x00FF);

	/* spare */
	p_header[10]    = 0x01;
	p_header[11]    = 0x00;


}


/******************************************************************************
 *   FUNCTION NAME  : send_msg_oam
 *   Inputs         : p_buffer
 *   Outputs        : none
 *   RETURNS        : none
 *   DESCRIPTION    : This function will create socket and send msg to oam
 *******************************************************************************/
int send_msg_oam (UInt8 *msg, int sockfd , Uint16 size)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering in %s",__FUNCTION__);

	UInt32 ret_val = GLUE_SUCCESS;
	if(NULL == msg)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\n  msg is null");
		ret_val =  GLUE_FAILURE;
    }
    else
	{
		if((TR069_SENDTO(sockfd, msg, size, 0, (void *)&g_serverAddr, sizeof(g_serverAddr))) == -1)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "\n could not send message");
			ret_val =  GLUE_FAILURE;
		}
	}
	return ret_val;

}

/******************************************************************************
 *   FUNCTION NAME  : create_oam_msg
 *   Inputs         : p_buffer
 *   Outputs        : none
 *   RETURNS        : none
 *   DESCRIPTION    : This function creates buffer in a format expected by oam and send buffer to oam
 *******************************************************************************/
void * create_oam_msg (map_map* in)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);

	map_map* m;
	void *p_res = NULL;
	SInt32 count = 0,  param_count = 0;
	UInt32 msg_api_length = 0 , msg_length = 0;
	void * p_enodeb_data = NULL;
	Uint16 l_offset = 0;
	UInt16 total_info_len = 0, parent_len = 0, child_len = 0;
	UInt32 child_info_len = 0 , trans_id = 0 ;
	UInt8 total_no_of_parameter = 0, fd = 0;
	int index = 0;
	/*SPR 19749 FIXED START*/
	static UInt8 multiSpvTrxId = 1;/*MultiParent SPV RollBack implementation ++START*/
	UInt8 resp = 1;

	/*SPR 19749 FIXED END*/

	UInt8 **tmp_buffer = NULL;
    /* SPR 22314 Start */
	tmp_buffer = (UInt8 **)ALLOCATE((PARAMETER_BUFFER_SIZE * sizeof(UInt8 *))  )  ;
    /* SPR 22314 End */
	if (tmp_buffer == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
		return NULL;
	}
	/*SPR 16151 Fixed Start*/
	for( index = 0 ; index < PARAMETER_BUFFER_SIZE ; index++ )
	{
        /* SPR 22314 Start */
		tmp_buffer[index] = (UInt8 *)ALLOCATE(DEVICE_INFO_NAME_VALUE_MAX_LEN) ;
        /* SPR 22314 End */
	/*SPR 16151 Fixed Start*/

		if (tmp_buffer[index] == NULL)
		{
			int index_1 = 0;
			if(index > 0)
			{    
                /* SPR 22314 Start */
				while(index_1 < index)
				{     
                    /* SPR 16448 Fix Start */   
					DEALLOCATE(tmp_buffer[index_1]); 
                    /* SPR 22314 End */
                    /* SPR 16448 Fix End */ 
					index_1++;
				}
			}
            /* SPR 16448 Fix Start */   
			DEALLOCATE(tmp_buffer); 
            /* SPR 16448 Fix End */ 
			TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
			return NULL;
		}
	}
	U8 msb = 0x00;
	U8 lsb = 0x00;
	U8 l_Is_parent_URL = 0;

	YLIST yList_set;
	transData_t *transD_t = NULL;

	m = (map_map *) in;

	fd = get_fd();
	/* SPR 21926 Start */
	YLINIT(&yList_set);
	/* SPR 21926 End */

	while (count < m->size)
	{
		trans_id =  count + 1;
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%d trans id", trans_id);

		p_enodeb_data = ALLOCATE(((sizeof(char *) * ( SET_PARAM_PAYLOAD_SIZE))) );
		if (p_enodeb_data == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
            /* SPR 16448 Fix Start */
            index = 0;
            while(index < PARAMETER_BUFFER_SIZE)
            {
                if(tmp_buffer[index] != NULL)
					DEALLOCATE(tmp_buffer[index]); 

                index++;
            }
            /* SPR 16448 Fix End */
			DEALLOCATE(tmp_buffer);	
			return NULL;
		}

		l_offset += 12;

		param_count = 0;

		YLIST *plist = &m->data[count].mylist;
		data_t *node  = NULL;
        /* SPR 17880 START */
        if (m->data[count].child_key == NULL)
			TRACE(LOG_CRITICAL , GLUE_LAYER, " m->data[count].child_key");

		if ((ENB_STRNCMP((const char *)m->data[count].parent_key , URL_PARENT, URL_PARENT_LEN) == 0) &&
                (m->data[count].child_key != NULL) &&
                (ENB_STRNCMP((const char *)m->data[count].child_key, "URL", strlen("URL")) == 0))
        /* SPR 17880 END */
		{
			l_Is_parent_URL = 1;
		}

        /* SPR 22314 Start */
        node = (data_t *)ylLast(plist);
        while (node != NULL)
            /* SPR 22314 End */
		{
			if (1 == l_Is_parent_URL && g_tmp_url_buffer == NULL)
			{
			
				/*SPR 16151 Fixed Start*/
				g_tmp_url_buffer = (U8 *)ALLOCATE(PARAMETER_BUFFER_SIZE );

				/*SPR 16151 Fixed End*/
				if (g_tmp_url_buffer == NULL)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
					DEALLOCATE(p_enodeb_data);	
            /* SPR 16448 Fix Start */
                    index = 0;
                    while(index < PARAMETER_BUFFER_SIZE)
                    {
                        if(tmp_buffer[index] != NULL)
							DEALLOCATE(tmp_buffer[index]); 

                        index++;
                    }
            /* SPR 16448 Fix End */
					DEALLOCATE(tmp_buffer);	
					return NULL;
				}
				ENB_STRNCPY((char *)g_tmp_url_buffer ,(const char *) node->data , 256);
				//l_save_trans = 1;
			}

			snprintf((char *)tmp_buffer[param_count], MAX_BUFF_SIZE,"%s=%s", node->child_key,node->data);
			child_info_len += strlen((const char *)tmp_buffer[param_count]) + 5;	
			total_no_of_parameter += 1;

			param_count++;
            /* SPR 22314 Start */
            node = (data_t *)ylPrev(&node->anchor);
            /* SPR 22314 End */
			//	DEALLOCATE (node);
		}

		total_info_len = child_info_len + strlen((const char *)m->data[count].parent_key) + 9;

		// Total length of parent information
		msg_length += sizeof(UInt16);

		msb = total_info_len/256 ;
		lsb = total_info_len%256;

		memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
		memcpy(p_enodeb_data+l_offset + 1, &lsb , sizeof(U8));


		l_offset += sizeof(UInt16);

		// Number of parameters
		msg_length += sizeof(UInt8);
		memcpy(p_enodeb_data+l_offset,&total_no_of_parameter,sizeof(UInt8) );
		l_offset +=  sizeof(UInt8);

		/*SPR 19749 FIXED START*/
		if(count == (m->size - 1))
		{
			//end mark = TRUE(1), all SPV msg sent
		msg_length += sizeof(UInt8);
			memcpy(p_enodeb_data+l_offset, &resp , sizeof(U8));

		}
		else
		{
			//end mark = FALSE (0), expected more SPV msg
			msg_length += sizeof(UInt8); //This is also StartIdx in GPV
		memset(p_enodeb_data+l_offset, 0 , sizeof(UInt8) );
		}
		/*SPR 19749 FIXED END*/


		l_offset +=  sizeof(UInt8);

		/*SPR 19749 FIXED START*/
		msg_length += sizeof(UInt8);
		if(m->size > 1){

			//This will act as a marker to denote multiparent SPV , also its TrxIId , but in GPV its reservedBit
			memcpy(p_enodeb_data+l_offset, &multiSpvTrxId , sizeof(UInt8) );
			TRACE(LOG_DETAIL , GLUE_LAYER, "MultiParentSPV TrxId: %d",multiSpvTrxId);
		}else{
			memset(p_enodeb_data+l_offset, 0 , sizeof(UInt8) );    //If only 1 TRname as parent then set 0
		}
		l_offset += sizeof(UInt8);
		//Reserved
		msg_length += sizeof(UInt8);

		memset(p_enodeb_data+l_offset, 0 , sizeof(UInt8) ); //This is also EndIdx in GPV, ZERO value also denotes LTE_TR069 as Source

		l_offset += sizeof(UInt8);

		/*SPR 19749 FIXED END*/


		//Length of Parent Node Name
		parent_len = strlen((const char *)m->data[count].parent_key) + 1;
		msg_length += sizeof(UInt16);

		msb = parent_len/256;
		lsb = parent_len%256;

		memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
		memcpy(p_enodeb_data+l_offset+1, &lsb , sizeof(U8));

		l_offset += sizeof(UInt16);

		//Parent Node Name
		msg_length += strlen((const char *)m->data[count].parent_key);

		memcpy(p_enodeb_data+l_offset, m->data[count].parent_key, strlen((const char *)m->data[count].parent_key) +1);
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Parent Node Name %s",(char *)p_enodeb_data+l_offset);
		l_offset += strlen((const char *)m->data[count].parent_key) + 1;

		param_count = 0;
		while(total_no_of_parameter > 0)
		{
			//      Length of Parameter Info
			child_len = strlen((const char *)tmp_buffer[param_count])+ 5;
			msg_length += sizeof(UInt16);

			
			/*SPR 16151 Fixed Start*/
			msb = child_len/PARAMETER_BUFFER_SIZE;
			lsb = child_len%PARAMETER_BUFFER_SIZE;
			/*SPR 16151 Fixed End*/

			memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
			memcpy(p_enodeb_data+l_offset+1, &lsb , sizeof(U8));

			l_offset += sizeof(UInt16);

			//Reserved
			msg_length += sizeof(UInt16);
			memset(p_enodeb_data+l_offset,0, sizeof(UInt16) );
			l_offset += sizeof(UInt16);

			//Name value par
			msg_length += strlen((const char *)tmp_buffer[param_count]);
			memcpy(p_enodeb_data+l_offset, (const char *)tmp_buffer[param_count], strlen((const char *)tmp_buffer[param_count]) + 1);
			TRACE (LOG_CRITICAL , GLUE_LAYER, "Received child Name  %s",(char *)p_enodeb_data+l_offset);

			l_offset += strlen((const char *)tmp_buffer[param_count])+1;


			total_no_of_parameter--;
			param_count++;
			child_len = 0;
		}

		//total_info_len = 0;
		child_info_len = 0;
		total_no_of_parameter = 0;
		count++;

		msg_api_length = total_info_len + API_HEADER_SIZE;
		total_info_len = 0;

		/* Fill API_HEADER */
		g_trans_id++;
		if(g_trans_id >TRANS_RANGE)
			g_trans_id = 1;
		construct_interface_api_header(p_enodeb_data, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_SET_PARAMETER_VALUES, msg_api_length);

		/* Send message to oam */
		send_msg_oam(p_enodeb_data , fd , msg_api_length);
		DEALLOCATE(p_enodeb_data);
		p_enodeb_data = NULL;

		/* adding transaction_id in the list */
		transD_t = ( transData_t *) ALLOCATE (sizeof(transData_t) );
		if (transD_t == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
            /* SPR 16448 Fix Start */
            index = 0;
            while(index < PARAMETER_BUFFER_SIZE)
            {
                if(tmp_buffer[index] != NULL)
					DEALLOCATE(tmp_buffer[index]); 

                index++;
            }
            /* SPR 16448 Fix End */
			DEALLOCATE(tmp_buffer);
			return NULL;
		}
		transD_t->trans_id = g_trans_id;
		addNodeinSet (&yList_set ,transD_t);
		msg_length = 0;
		l_offset = 0;

		/* Call handle set url if Parent URL parameter is recieved */

		if (1 == l_Is_parent_URL)
		{
			//handle_set_url(tmp_url_buffer);
			g_parent_URL = 1;
			g_url_trans = g_trans_id;
			l_Is_parent_URL = 0;
		}
	}


	/*SPR 19749 FIXED START*/
	if(multiSpvTrxId==SPV_BUFFER_MAX_SIZE)
		multiSpvTrxId=1; //Resetting the Value to One,else keep on increasing
	else

		multiSpvTrxId++;
	/*SPR 19749 FIXED END*/
	p_res = recv_msg_oam(&yList_set, RPC_SETPARAMETERVALUES);

	if (p_res)
	{
		/* Coverity 114335 Fixed Start*/
		/* Code Remove */
		/* Coverity 114335 Fixed End*/
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
		/* SPR 21588 FIXED START  */
       while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
		{
			decode_notification(g_notif_buffer);
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
			qvFree (g_notif_buffer);
			g_notif_buffer = NULL;
		}
		/* SPR 21588 FIXED END */
		/* SPR 21588 FIXED START  */
		/* code removed */
		/* SPR 21588 FIXED END */
		/* MEMORY LEAK FIXED END */
	}
    /* SPR 16448 Fix Start */
    index = 0; 
    while(index < PARAMETER_BUFFER_SIZE)
    {       
        if(tmp_buffer[index] != NULL)
			DEALLOCATE(tmp_buffer[index]);

        index++;
    }       
	DEALLOCATE(tmp_buffer); 
   /*defunct 29/9 fix Start */ 
	//DEALLOCATE_glue_map(m);
   /*defunct 29/9 fix End */ 
    /* SPR 16448 Fix End */
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
	return p_res;
}

/******************************************************************************
 *   FUNCTION NAME  : handle_set_param
 *   Inputs         : p_buffer, eType
 *   Outputs        : none
 *   RETURNS        : none
 *   DESCRIPTION    : This function handles set parameter RPC request received from ACS.
 *******************************************************************************/
void * handle_set_param (void *msg)
{
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering %s  ",__FUNCTION__);
	UInt32  size  = 0, count = 0 ;
	UInt8 *paramName = NULL, *paramValue = NULL ;

	cwmp__ParameterValueStruct **tmpparamVal;
	UInt8 *child_param = NULL, *parent_param = NULL;
	UInt32 len = 0;
	void *p_set_resp = NULL;


	ParameterValueList *list = (ParameterValueList *)msg;
	cwmp__ParameterValueStruct **paramVal = (cwmp__ParameterValueStruct **)list->__ptrParameterValueStruct;

	size = list->__size;

	tmpparamVal = paramVal;

	while(count < size)
	{
		paramName = (UInt8 *) paramVal[count]->Name;
		paramValue =(UInt8 *) paramVal[count]->Value;
		add_brackets((UInt8 *)paramVal[count]->Name);
		TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Parameter Name [%d] = %s", __FUNCTION__ , count , paramName);
		TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Parameter value [%d] = %s", __FUNCTION__ , count , paramValue);
		count++;
	}

	count = 0;
	module_map_ptr = map_new_value();

	//map Initialization
	if ( module_map_ptr == NULL )
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s Map not initialized successfully  ",__FUNCTION__);
		return 0;
	}
	while(count < size)
	{

		child_param = (UInt8 *)strrchr((const char *)tmpparamVal[count]->Name,'.');
		if (child_param  == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "%s strrchr returned null ",__FUNCTION__);
            /*Memory Leak 15 jan fixed start */
			DEALLOCATE(module_map_ptr);  
            /*Memory Leak 15 jan fixed end */
			return NULL;
		}
		len = strlen((const char *)tmpparamVal[count]->Name) - strlen((const char *)child_param);

		parent_param = ALLOCATE( MAX_BUFF_SIZE);
		if (parent_param == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
            /*Memory Leak 15 jan fixed start */
			DEALLOCATE(module_map_ptr);  
            /*Memory Leak 15 jan fixed end */
			return NULL;
		}
		ENB_STRNCPY((char *)parent_param, (const char *)tmpparamVal[count]->Name, len );
		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s Map added for %s ",__FUNCTION__ ,(const char *)tmpparamVal[count]->Name);
		map_put_value(module_map_ptr,(Uint8 *) parent_param, (Uint8 *)(child_param +1), (Uint8 *)tmpparamVal[count]->Value);
		DEALLOCATE(parent_param);
		parent_param = NULL;
		count++;
	}


	p_set_resp = create_oam_msg(module_map_ptr);
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting %s  ",__FUNCTION__);
    /*Memory Leak 15 jan fixed start */
	/* Memory Leak 18 Jan change start */
	//map_free(module_map_ptr); 
	map_free_1(module_map_ptr);
	/* Memory Leak 18 Jan change end */
    /*Memory Leak 15 jan fixed end */
	return p_set_resp;

}
/*SPR 17625 FIXED START */
/******************************************************************************
 *   FUNCTION NAME  : handle_set_attribute
 *   Inputs         : p_buffer, eType
 *   Outputs        : none
 *   RETURNS        : none
 *   DESCRIPTION    : This function handles set parameter Attribute RPC request received from ACS.
 *******************************************************************************/
void * handle_set_attribute (void *msg)
{
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering %s  ",__FUNCTION__);
        UInt32  size  = 0, count = 0;
        /* coverity 96681 fixed start code removed */
        /*  coverity 96666/96656/96627 fix start */
        UInt8 *paramName = NULL;
        /*  coverity 96666/96656/96627 fix end */

        /* coverity 96665/96633 fixed   / code removed */
        /*  coverty 96669/96682/96675/96661/96668 fixed code removed*/
        UInt16 total_no_of_parameter = 0, fd = 0, parent_len = 0;
        UInt32 msg_api_length = 0;
        
        void *p_res = NULL;

        void * p_enodeb_data = NULL;
        Uint16 l_offset = 0;

        /*  coverity 96626 fix start */
        /*  coverity 96626 fix end */
        /*  coverity 96658/96657/96959  fixed code removed */
        //sSetParamAttrRes_t *res = NULL;

        sParameterAttributesList *list = (sParameterAttributesList *)msg;
        ParameterAttributesStruct **paramAttr = (ParameterAttributesStruct **)list->__ptrSetParameterAttributesStruct;
        //sAccessList *accList = (sAccessList *)&((paramAttr)->AccessList);

        /*  coverity 96959 fix code removed */

        U8 msb = 0x00;
        U8 lsb = 0x00;

        fd = get_fd();

        /*Allocated 1024 bytes for buffer(to be sent to SOM)*/
	p_enodeb_data = ALLOCATE( SET_PARAM_PAYLOAD_SIZE);
        if (p_enodeb_data == NULL)
        {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
                return NULL;
        }
        
        /*Skip the first 12 bytes for API Header*/
        l_offset += API_HEADER_SIZE;
        TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);


        size = list->__size;
        total_no_of_parameter = size;
        TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Total number of parameters = %d", __FUNCTION__ , total_no_of_parameter);

        //accSize = accList->__size;

        //tempaccList = accList;

        //toatl number of parameters
        msb = total_no_of_parameter/256 ;
        lsb = total_no_of_parameter%256;

        /*Filling total number of parameters in buffer*/
        memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
        memcpy(p_enodeb_data+l_offset + 1, &lsb , sizeof(U8));

        l_offset += sizeof(UInt16);
        TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);


        while (count < total_no_of_parameter)
        {
                paramName = (UInt8 *) paramAttr[count]->Name;
                /*while(count1 < accSize)
                  {
                  paramAccessList = (UInt8 *)accList[count1]->__ptrstring;
                  TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - AccessList Name [%d] = %s", __FUNCTION__ , count1 , paramAccessList);
                  count1++;
                  } */
                TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Parameter Name [%d] = %s", __FUNCTION__ , count , paramName);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Notification Change [%d] = %d", __FUNCTION__ , count ,paramAttr[count]->NotificationChange );
                TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - notification [%d] = %d", __FUNCTION__ , count , paramAttr[count]->Notification);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - AccessList Change [%d] = %d", __FUNCTION__ , count , paramAttr[count]->AccessListChange);

                //Node Name Length
                parent_len = strlen((const char *)paramName) + 1;
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Parent length calculate %d",parent_len);

                msb = parent_len/256;
                lsb = parent_len%256;

                memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
                memcpy(p_enodeb_data+l_offset+1, &lsb , sizeof(U8));

                l_offset += sizeof(UInt16);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

                //Parent Node Name
                memcpy(p_enodeb_data+l_offset, (UInt8 *)paramAttr[count]->Name, parent_len);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received  Node Name %s",(char *)p_enodeb_data+l_offset);
                l_offset += parent_len; //strlen(p_enodeb_data+l_offset) + 1;
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);


                //NotificationChange
                memcpy((p_enodeb_data+l_offset), (UInt8 *)(&(paramAttr[count]->NotificationChange)), sizeof(UInt8));
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Notification change value  %d",*((UInt8*)(p_enodeb_data+l_offset)));

                l_offset += sizeof(UInt8);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

                //Notification Value
                memcpy(p_enodeb_data+l_offset, (UInt8 *)(&(paramAttr[count]->Notification)), sizeof(UInt8));
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Notification  value  %d",*((UInt8*)(p_enodeb_data+l_offset)));

                l_offset += sizeof(UInt8);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

                //AccessListChange
                memcpy(p_enodeb_data+l_offset, (UInt8 *)(&(paramAttr[count]->AccessListChange)), sizeof(UInt8));
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Access List value  %d",*((UInt8*)(p_enodeb_data+l_offset)));

                l_offset += sizeof(UInt8);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

                //AccessListLength
                *((UInt8*)p_enodeb_data+l_offset) = 1;
                //memcpy(p_enodeb_data+l_offset, (UInt8 *)(&(paramAttr[count]->AccessListChange)), sizeof(UInt8));
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Access List Length is  %d",*((UInt8*)(p_enodeb_data+l_offset)));

                //access_len = *((UInt8*)p_enodeb_data+l_offset);
                l_offset += sizeof(UInt8);
                TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

                //AccessListValue
                *((UInt8*)p_enodeb_data+l_offset) = '\0';
                TRACE (LOG_CRITICAL , GLUE_LAYER, "Received Access List Length value  %d",*((UInt8*)(p_enodeb_data+l_offset)));
                l_offset += sizeof(UInt8);

                //memcpy(p_enodeb_data+l_offset, (UInt8 *)), access_len);
               //total_no_of_parameter--;
                 count++;
        }
        msg_api_length = l_offset;

        /* Fill API_HEADER */
        g_trans_id++;
        if(g_trans_id >TRANS_RANGE)
                g_trans_id = 1;
        construct_interface_api_header(p_enodeb_data, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_SET_PARAMETER_ATTRIBUTE, msg_api_length);

        /* Send message to oam */
        send_msg_oam(p_enodeb_data , fd , l_offset);
        TRACE (LOG_CRITICAL , GLUE_LAYER, "After send msg oam");

	DEALLOCATE(p_enodeb_data);
        p_enodeb_data = NULL;
        if(p_enodeb_data == NULL)
        {
            TRACE (LOG_CRITICAL , GLUE_LAYER, "p_enodeb is free");
        }




        p_res = recv_msg_oam(NULL, RPC_SETPARAMATTRIBUTE);



        if (p_res)
	{
		/* SPR 21588 FIXED START  */
		/* code removed */
		/* SPR 21588 FIXED END */
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
		/* SPR 21588 FIXED START  */
       while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
		{
			/* SPR 21588 FIXED END */
			decode_notification(g_notif_buffer);
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
			qvFree (g_notif_buffer);
			g_notif_buffer = NULL;
			TR069_USLEEP(500);
		}
	}
       
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
        return p_res;


}
/* SPR 17625 FIXED END */


/******************************************************************************
 *   FUNCTION NAME  : glue_sessionInitCallback
 *   Inputs         : sessionId, URL
 *   Outputs        : none
 *   RETURNS        : Void 
 *   DESCRIPTION    : This function handles glue_sessionInitCallback  request.
 *******************************************************************************/
void glue_sessionInitCallback (char *sessionId, char* URL )
{
	UInt32 len = 0;
	UInt32 fd = 0; 
	void *p_buffer = NULL;
	void *p_buffer1 = NULL;
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s",__FUNCTION__);
	TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , sessionInitCallback sessionId = %s \n", __FUNCTION__ , sessionId);
    if(NULL == URL) {
	TRACE (LOG_CRITICAL , GLUE_LAYER, "URL is NULL\n"); }
	fd = get_fd();

/* SPR 20429 FIXED START */
/* Code remove */
	
/* SPR 20429 FIXED END */
	if (0 == g_Is_inform_rec) //g_Is_sec_url != 1)
	{
		len = buffer_OAM_Config_Resp('0', &p_buffer , 0 , NULL);
		if (p_buffer)
		{
			construct_interface_api_header(p_buffer, g_trans_init_id, TR069_MODULE_ID, 
					OAM_MGMT_INTF_MODULE_ID, TR069_OAM_CONFIG_RESP, len);
			send_msg_oam(p_buffer , fd,  len);
			DEALLOCATE(p_buffer);
			p_buffer = NULL;
		}
		g_Is_inform_rec = 1;
		g_Is_config_res_send  = 1;

		if (buf_spv != NULL)
		{
			send_msg_oam ( buf_spv , fd, tmp_spv_len);
			DEALLOCATE(buf_spv);	
			buf_spv = NULL;

		}

	}
	time(&g_file_ind_start);
	g_Is_inform_rec = 1;

	if (1 == g_exped_wait_inform)
	{
		len = buffer_OAM_notif_Resp('0' , &p_buffer1);
		if (p_buffer1)
		{
			construct_interface_api_header(p_buffer1 , g_notif_transid , TR069_MODULE_ID , OAM_MGMT_INTF_MODULE_ID , 
					OAM_NOTIFICATION_IND_RESP , len);
			send_msg_oam(p_buffer1 , fd,  len);
			DEALLOCATE(p_buffer1 );	
			p_buffer1= NULL;
		}
		g_exped_wait_inform = 0;
		g_expedited_inform = 0;
	}

    /* SPR 18033 FIXED START */
	if (g_queued_paramList && g_queued_wait_inform == 0)
    /* SPR 18033 FIXED END */
	{
		/* spr 18338 fix start */
		/*for (i = cpeParamList_t->__size - 1 ; i > 0 ; i--)
		  {
		  DEALLOCATE(paramList[i]->Name);
		  DEALLOCATE(paramList[i]->Value); 
		  DEALLOCATE(paramList[i]);
		}*/
		/* spr 18338 fix end */
		/* SPR 15346 Fix Start */
		len = buffer_OAM_notif_Resp('0' , &p_buffer1);
		if(p_buffer1){
			construct_interface_api_header(p_buffer1 , g_notif_transid , TR069_MODULE_ID , OAM_MGMT_INTF_MODULE_ID , OAM_NOTIFICATION_IND_RESP , len);
			send_msg_oam(p_buffer1 , fd,  len);
            /*SPR 17796 START*/
			/*g_queued_paramList = NULL;*/
            /*SPR 17796 END*/
			DEALLOCATE(p_buffer1 );
		}

        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
	}

	/* SPR 15346 Fix End*/

	if (shems_Session == 1)
	{
		shems_Session = 2;  // Session Is now initialized 
		TRACE(LOG_CRITICAL , GLUE_LAYER, "SHems session initialized");
	}

	if (iHems_session)
	{
		TRACE (LOG_CRITICAL , GLUE_LAYER," iHEMS  session initialized");
	}


	TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting from  %s",__FUNCTION__);
}

/******************************************************************************
 * *   FUNCTION NAME  : glue_sessionFailedCallback
 * *   Inputs         : sessionId
 * *   Outputs        : none
 * *   RETURNS        : Void
 * *   DESCRIPTION    : This function handles glue_sessionFailedCallback  request.
 ********************************************************************************/
/* SPR 16547 Fix Start */
void glue_sessionFailedCallback (char *sessionId,int curl_err_code,char *curl_err_str)
	/* SPR 16547 Fix End */

{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering in %s", __FUNCTION__);
	UInt32 len = 0 ;
	UInt32 fd = 0;
	void *p_buffer = NULL;
	void *p_buffer1 = NULL;
	//coverity
	      /* SPR 20704 FIXED START */
	      Uint16 err_code = 0;
			U8 error_str[64] = {0};
			U8 error_code_str[8] = {0};
			U8 *exc_start = NULL;
	      /* SPR 20704 FIXED END */

	if (NULL != sessionId)
	{

		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , sessionInitCallback sessionId = %s \n", __FUNCTION__ , sessionId);
		fd = get_fd();

		if (0 == g_Is_config_res_send)
		{
	      /* SPR 20704 FIXED START */
         /* code removed */
	      /* SPR 20704 FIXED END */
			//coverity fix
			if ((exc_start =(U8 *)ENB_STRCHR(sessionId, ':')) != NULL)
			{
				tr_strncpy((char *)error_str ,(const char *) (exc_start + 1));
				tr_strncpy((char *)error_code_str , sessionId );
				err_code =  atoi((const char *)error_code_str);
			}

			/* SPR 16547 Fix Start */
			if(curl_err_code)
			{
				/* SPR 19124 FIXED START  */
				TRACE(LOG_DETAIL , GLUE_LAYER,"curl_err_code = %d",curl_err_code); 
                /*  coverity 96676 fix start */
				/* COVERITY 108700 FIX START */
				char *temp = NULL;
				temp = (char *) ALLOCATE( strlen(curl_err_str)+1);
				if(NULL == temp)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER ,"Memory allocation failed for 'temp' in Function %s",__FUNCTION__);
					exit(0);
                }
                ENB_STRCPY(temp , curl_err_str);
				TRACE(LOG_CRITICAL , GLUE_LAYER," temp  = %s", (char*)temp);
				len = buffer_OAM_Config_Resp  ('1', &p_buffer , curl_err_code , (U8*)temp);
				/* COVERITY 108700 FIX END */
				TRACE(LOG_DETAIL , GLUE_LAYER,"buffer length is = %d", len);
				/* SPR 19124 FIXED END */
				DEALLOCATE(temp);
                /*  coverity 96676 fix end */
			}
			else
			{
				len = buffer_OAM_Config_Resp  ('1', &p_buffer , err_code , error_str);
			}
			/* SPR 16547 Fix End */


			if (p_buffer)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s, Sending COnfig Response to OAM ",__FUNCTION__);
				construct_interface_api_header( p_buffer, g_trans_init_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, TR069_OAM_CONFIG_RESP, len);
				send_msg_oam (p_buffer , fd,  len);
				DEALLOCATE(p_buffer );	
				p_buffer= NULL;
				g_Is_config_res_send = 1;
			}

		}

	      /* SPR 20704 FIXED START */
      if (send_curl_err_oam_g == 1 && g_Is_config_res_send != 0)
      {
          if ((exc_start =(U8 *)ENB_STRCHR(sessionId, ':')) != NULL)
          {
              tr_strncpy((char *)error_str ,(const char *) (exc_start + 1));
              tr_strncpy((char *)error_code_str , sessionId );
              err_code =  atoi((const char *)error_code_str);
          }

          /* SPR 16547 Fix Start */
          if(curl_err_code)
          {
              /* SPR 19124 FIXED START  */
              TRACE(LOG_DETAIL , GLUE_LAYER,"curl_err_code = %d",curl_err_code); 
              /*  coverity 96676 fix start */
              /* COVERITY 108700 FIX START */
              char *temp = NULL;
              temp = (char *) ALLOCATE (strlen(curl_err_str)+1);
              if(NULL == temp)
              {
                  TRACE(LOG_CRITICAL , GLUE_LAYER ,"Memory allocation failed for 'temp' in Function %s",__FUNCTION__);
                  exit(0);
              }
              ENB_STRCPY(temp , curl_err_str);
              TRACE(LOG_CRITICAL , GLUE_LAYER," temp  = %s", (char*)temp);
              len = buffer_OAM_Config_Resp  ('1', &p_buffer , curl_err_code , (U8*)temp);
              /* COVERITY 108700 FIX END */
              TRACE(LOG_DETAIL , GLUE_LAYER,"buffer length is = %d", len);
              /* SPR 19124 FIXED END */
              DEALLOCATE(temp);
              /*  coverity 96676 fix end */
          }
          else
          {
              len = buffer_OAM_Config_Resp  ('1', &p_buffer , err_code , error_str);
          }
          /* SPR 16547 Fix End */
          /* COV 57571/113184 FIXED START */
          if (p_buffer)
          {
              TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s, Sending COnfig Response to OAM ",__FUNCTION__);
              construct_interface_api_header( p_buffer, g_trans_init_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, TR069_OAM_CONFIG_RESP, len);
              send_msg_oam (p_buffer , fd,  len);
              DEALLOCATE(p_buffer );	
              p_buffer= NULL;
              g_Is_config_res_send = 1;
          }

          /* COV 57571/113184 FIXED END */

         send_curl_err_oam_g = 0;
      }
	      /* SPR 20704 FIXED END */
		if (1 == g_exped_wait_inform)
		{
			len = buffer_OAM_notif_Resp('1' , &p_buffer1);
			if (p_buffer1)
			{
				construct_interface_api_header(p_buffer1 , g_notif_transid , TR069_MODULE_ID , OAM_MGMT_INTF_MODULE_ID ,
						OAM_NOTIFICATION_IND_RESP , len);
				send_msg_oam(p_buffer1 , fd,  len);
				DEALLOCATE(p_buffer1);	
				p_buffer1 = NULL;
			}
			g_exped_wait_inform = 0;
			g_expedited_inform  = 0;
		}

		if (iHems_session)
			iHems_session = 0;
	}
	else
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Session Is NULL " );

	}
}
/******************************************************************************
 *   FUNCTION NAME  : glue_sessionClosedCallback
 *   Inputs         : sessionId
 *   Outputs        : none
 *   RETURNS        : Void
 *   DESCRIPTION    : This function handles glue_sessionClosedCallback  request.
 *******************************************************************************/
void glue_sessionClosedCallback(char *sessionId )
{
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s",__FUNCTION__);
    if(NULL == sessionId) {
	 TRACE(LOG_CRITICAL , GLUE_LAYER, "sessionId is NULL\n"); }
	/*Coverity 111660/111591 Fix Start*/
	/*Code removed*/
	/*Coverity 111660/111591 Fix End*/
    /* Spr 16890 Fix Start */
    UInt32 l_buffer_len = 0;
    void *l_p_buffer = NULL;
    UInt32 fd = 0;
    /* Spr 16890 Fix End */  

	if (g_tmp_url_buffer && g_Is_new_url)
	{
/* SPR 19774 FIXED START */
    /* CODE REMOVE */
/* SPR 19774 FIXED END */

		g_Is_new_url = 0;
		g_url_trans = 0;
	}

	if (g_Reboot_recv)
	{ 
		sleep(1);
		handle_reboot_req(); // Send reboot Request to  OAM 
		g_Reboot_recv = 0;
	}

    /* Spr 16890 Fix Start */
    /* Code Deleted */
    /* Spr 16890 Fix End */

	if (shems_Session == 2 && !g_send_init_config)   // If session is initialized  successfully.
	{
		// Send Initial Configuration Control Command  
		TRACE(LOG_CRITICAL , GLUE_LAYER, " Value of shems_Session now  : %d", shems_Session); 
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Sending Initial Configuration Now");
        /* Spr 16890 Fix Start */
        /* Code Deleted */
        /* Spr 16890 Fix End */
		UInt8 *contrl_cmd = (UInt8 *)INIT_CONFIG_CMD;
		fd = get_fd();
		l_p_buffer = buffer_OAM_Control_Cmd_Req(0 , NULL, &l_buffer_len, contrl_cmd);
		if (l_p_buffer)
		{
			g_trans_id++;
			if (g_trans_id > TRANS_RANGE)
				g_trans_id = 1;
			construct_interface_api_header(l_p_buffer , g_trans_id, TR069_MODULE_ID,
/* SPR 11921 Fix Start*/
					OAM_MGMT_INTF_MODULE_ID, OAM_CONTROL_COMMAND_REQ, l_buffer_len);
/* SPR 11921 Fix End*/
			send_msg_oam(l_p_buffer , fd, l_buffer_len);
			DEALLOCATE(l_p_buffer);	
			l_p_buffer= NULL;
		}       
		shems_Session = 0;
		g_send_init_config = 1;
	}
	else
		shems_Session = 0;

	if (iHems_session)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, " First iHEMS session closed");
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Now Sending Restart control command to OAM");
		handle_reboot_req (); // Send reboot Request to  OAM after First Ihems session.
		iHems_session = 0;
	}


    /* Spr 16890 Fix Start */
    /* sending Session Close Indication To OAM */
    /* Allocating buffer 12 byte header + 2 byte reserved*/
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Sending Session Close Ind to OAM");
    fd = get_fd();
    l_buffer_len = API_HEADER_SIZE + 2; 
	l_p_buffer  =  (void *) ALLOCATE( l_buffer_len);
    /* coverity 96660 fixed start */
    if(l_p_buffer)
    {
    /* coverity 96660 fixed end */
    g_trans_id++;
    if (g_trans_id > TRANS_RANGE)
        g_trans_id = 1;
    construct_interface_api_header(l_p_buffer , g_trans_id, TR069_MODULE_ID,
            OAM_MGMT_INTF_MODULE_ID, TR069_HEMS_SESSION_CLOSE_IND, l_buffer_len);
    send_msg_oam(l_p_buffer , fd, l_buffer_len);
		DEALLOCATE(l_p_buffer);	
    /* coverity 96660 fixed start */
    }
    /* coverity 96660 fixed end */
    /* coverity 86575 fixed  /code removed  */
 
    /* Spr 16890 Fix End */
}

/******************************************************************************
 *   FUNCTION NAME  : handle_deleteobject_req
 *   Inputs         : p_buffer, eType
 *   Outputs        : none
 *   RETURNS        : Void
 *   DESCRIPTION    : This function handles  Delete object request received from  tr069 client library.
 *******************************************************************************/
void *handle_deleteobject_req( void *msg)
{
	Uint16 l_offset = 0;
	void * p_enodeb_data = NULL;
	UInt32 len = 0;
	U8 msb = 0x00;
	U8 lsb = 0x00;
	UInt32 msg_api_length = 0;
	UInt32 fd = 0;
	void *p_res = NULL;
	UInt8 *paramName =  NULL;

	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);

	p_enodeb_data = ALLOCATE( BUFFER_LENGTH);
	if (p_enodeb_data == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
		return NULL;
	}

	//paramName = (UInt8 *)msg;

	paramName = ALLOCATE( MAX_PARAM_NAME_SIZE);
	if(NULL == paramName)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		//coverity fix
		DEALLOCATE(p_enodeb_data);
		return NULL;
	}		
	ENB_STRNCPY((char *)paramName , (const char *)(UInt8 *)msg , MAX_PARAM_NAME_SIZE);
	add_brackets(paramName);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Received req  %s",paramName);
	fd = get_fd();

	//API HEADER
	l_offset += 12;

	// Reserved
	memset(p_enodeb_data+l_offset, 0,sizeof(UInt32));
	l_offset +=4;

	//Length
	len = strlen((const char *)paramName) + 1 + 2 + 2;

	msb = len/256 ;
	lsb = len%256;

	memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
	memcpy(p_enodeb_data+l_offset + 1, &lsb , sizeof(U8));
	l_offset +=2;

	//Reserved

	memset(p_enodeb_data+l_offset, 0,sizeof(UInt16));
	l_offset +=2;

	memcpy(p_enodeb_data+l_offset, paramName,255);
	l_offset += strlen((const char *)paramName) + 1;

	msg_api_length = l_offset;


	/* Fill API_HEADER */
	g_trans_id++;
	if(g_trans_id >TRANS_RANGE)
		g_trans_id = 1;
	construct_interface_api_header(p_enodeb_data, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_DELETE_OBJECT_REQ, msg_api_length);

	/* Send message to oam */
	send_msg_oam(p_enodeb_data , fd , msg_api_length);
	DEALLOCATE(p_enodeb_data);	
    /* coverity 86657 fixed code removed */

	p_res = recv_msg_oam (NULL, RPC_DELETEOBJECT);

		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
       /* SPR 21588 FIXED START  */
       /* code removed */
       /* SPR 21588 FIXED END */
            /* SPR 21588 FIXED START  */
         while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
    {
		decode_notification(g_notif_buffer);
        /* SPR  12059  Fix Start */
		DEALLOCATE(g_notif_buffer);
        /* SPR 12059  Fix End*/
		g_notif_buffer = NULL;
	}
	 /* SPR 21588 FIXED END */

	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
	DEALLOCATE(paramName);
	return p_res;
}



/******************************************************************************
 *   FUNCTION NAME  : invokeCallbackStub
 *   Inputs         : msg, eType, tmp
 *   Outputs        : none
 *   RETURNS        : Void
 *   DESCRIPTION    : This function handles invokeCallbackStub  request.
 *******************************************************************************/
void * invokeCallbackStub (void *msg , int eType , char *tmp)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering in %s",__FUNCTION__);
    if(NULL == tmp) {
	 TRACE(LOG_CRITICAL , GLUE_LAYER, "tmp is NULL\n"); }
	void  *p_status = NULL;

    /* SPR 18104 FIXED START*/
    g_invoke_in_perform = 1 ;

    /* SPR 18104 FIXED END*/
    /* coverity 96686 fixed code removed */
    /* SPR 19268 FIXED START*/
    UpldResp *ret = NULL;
    /* SPR 19268 FIXED END*/
    /* coverity 55865 fixed  / code removed */
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Received etype %d",eType);

	switch(eType)
	{
        /* SPR 17596 fixed start */
        case RPC_GETRPCMETHOD:
            TRACE (LOG_CRITICAL , GLUE_LAYER , "In %s, Received RPC_GETRPCMETHOD" , __FUNCTION__);
            p_status = handle_get_rpc(RPC_GETRPCMETHOD );
            TRACE(LOG_CRITICAL , GLUE_LAYER ," ......p_status = %p" , p_status);
            break;
        /* SPR 17596 fixed end */
		case RPC_DELETEOBJECT:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received RPC_DELETEOBJECT", __FUNCTION__);
			p_status = handle_deleteobject_req((void *)msg);
			break;
		case RPC_SETPARAMETERVALUES:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received RPC_SETPARAMETERVALUES", __FUNCTION__);
			p_status = handle_set_param((void *)msg);
			break;
		case RPC_GETPARAMETERVALUES:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_GETPARAMETERVALUES", __FUNCTION__);
			p_status = handle_get_param((void *)msg, RPC_GETPARAMETERVALUES);
			break;
		case  RPC_PREDOWNLOAD:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_PREDOWNLOAD",  __FUNCTION__);
			p_status = handle_oam_file_download_ind(msg , eType);
			if (shems_Session >= 2)
			{
				shems_Session++;
				TRACE(LOG_CRITICAL , GLUE_LAYER,"value of shems_Session after downlaod: %d",shems_Session);
			}
			break;
		case  RPC_DOWNLOAD:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_DOWNLOAD", __FUNCTION__);
			g_first_file_ind = 1;
			p_status =  handle_oam_file_download_ind(msg , eType);
			break;
        /* SPR 17717 fixed start */
		case RPC_RAISE_ALARM:
		      TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_RAISE_ALARM", __FUNCTION__);
		      p_status =  handle_oam_alarm_ind();
                      break;
        
        /* SPR 17717 fixed end */
		case RPC_SET_URL:
		case RPC_SETPARAMATTRIBUTE:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_SETPARAMATTRIBUTE or set url --> %d ",__FUNCTION__ ,eType);
		     /* SPR 17625 FIXED START */
            p_status = handle_set_attribute((void *)msg);
            break;
            /* SPR 17625 FIXED END */
            /* SPR 17625 FIXED START */
        case RPC_GETPARAMETERATTRIBUTE:
            TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_GETPARAMATTRIBUTE or set url --> %d ",__FUNCTION__ ,eType);
            p_status = handle_get_param_attr((void *)msg);
            break;
            /* SPR 17625 FIXED END */

        case RPC_ADDOBJECT:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s , Received RPC_ADDOBJECT", __FUNCTION__);
			p_status = (void *)handle_add_object(msg);
			break;
		case RPC_REBOOT:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received RPC_REBOOT", __FUNCTION__);
            /* Spr 16890 Fix Start */
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Sending Reboot command to OAM");
            p_status = handle_reboot_req(); // Send reboot Request to  OAM 
            /* Spr 16890 Fix End */
			break;
		case RPC_FACTORYRESET:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received RPC_FACTORYRESET", __FUNCTION__);
            /* Spr 16890 Fix Start */
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Sending Factory Reboot command to OAM");
            p_status = handle_factoryreset_req();  //Send Factory_Reset request to OAM.
            /* Spr 16890 Fix End */
			break;

            /*SPR 17631 fixed start*/
        case RPC_PREUPLOAD:
            TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received  PREUPLOAD ", __FUNCTION__);

    /* SPR 19268 FIXED START*/
            /*code removed */
    /* SPR 19268 FIXED END*/
            ret = handle_oam_file_upload_ind(msg);
            /* coverity 96652 fixed start */
            if(ret)
            {
            /* coverity 96652 fixed end */
            if(ret->eStatus == RPC_SUCCESS)
            {
					TRACE(LOG_DETAIL , GLUE_LAYER,"RPC UPLOAD SUCCESS");
    /* SPR 19268 FIXED START*/
					TRACE(LOG_DETAIL , GLUE_LAYER,"File Path : %s",(const char*)ret->filepath);
    /* SPR 19268 FIXED END*/
            }
            else
            {
					/* SPR 19268 FIXED START*/
					TRACE(LOG_DETAIL , GLUE_LAYER,"RPC UPLOAD FAILURE");
					/* SPR 19268 FIXED END*/
            }
            /*  coverity 96652 fixed start */
            }
            /*  coverity 96652 fixed end */
           
			/* SPR 19268 FIXED START*/
			return ret;
			/* SPR 19268 FIXED END*/

		/*case RPC_PREUPLOAD:
			TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received  PREUPLOAD ", __FUNCTION__);
			file_path  = ALLOCATE(64);
			if (file_path == NULL)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
				return NULL;
			}
			return file_path;*/

            /*SPR 17631 fixed end*/
        /* Spr 16990 Fix Start  */
            break;
        case RPC_GETPARAMETERNAME:
            TRACE(LOG_CRITICAL , GLUE_LAYER,"In %s, Received  RPC_GETPARAMETERNAME"
                                            " Sending GPN Request to OAM", __FUNCTION__);
            p_status = (void *)handle_get_parameter_name_req(msg);  //Send GPN request to OAM.
            break;
         /* Spr 16990 Fix End */
  
		default:
			TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Invalid message Received from Client");
	}
    /* SPR 18104 FIXED START*/
    g_invoke_in_perform =0 ;
    if(g_invoke_in_perform_periodic_inform == 1)
    {
        g_invoke_in_perform_periodic_inform = 0;
            
        init_periodic_inform();

     }
    /* SPR 18104 FIXED END*/
 
     TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
	return p_status;
}

/******************************************************************************
  FUNCTION NAME  : thread_msg_oam 
Inputs         : void * 
Outputs        : none
RETURNS        : none.
DESCRIPTION    : Thread function for sending oam Init ind in a configured 
interval till config request recieved.
 ******************************************************************************/

void * thread_msg_oam (void *arg)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering in %s", __FUNCTION__);
    if(NULL == arg)
    {
	   TRACE(LOG_CRITICAL , GLUE_LAYER, "Arg NULL\n");
    }    
	void  *p_ret = NULL;
	time_t t1;
	time_t t2;
	SInt32 fd = 0; 
    /*Coverity 110669 Fix Start*/
	SInt32 ret = 0;  
    /*Coverity 110669 Fix End*/
	time (&t1);
	time (&t2);
	fd = get_fd();
	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,  fd used : %d", __FUNCTION__, fd);

	while (1)
	{
		p_ret =  ALLOCATE( 12);
		if (p_ret)
		{
			construct_interface_api_header(p_ret, g_trans_init_id, TR069_MODULE_ID,
					OAM_MGMT_INTF_MODULE_ID, TR069_OAM_INIT_IND, 12);
			send_msg_oam(p_ret,fd, API_HEADER_SIZE);
			DEALLOCATE(p_ret);	
			p_ret= NULL;
		}
        /* SPR 21671 FIXED START */
        TRACE(LOG_CRITICAL , GLUE_LAYER, "oam thead 1");
        sleep(g_sleep_ind);
        /* SPR 21671 FIXED END */
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Slept for : %d", __FUNCTION__, g_sleep_ind);
        /*Coverity 110669 Fix Start*/
        ret = pthread_cond_wait(&count_threshold_cv,&count_mutex);
		if(TR069_ZERO != ret)
        {
            TRACE(LOG_MAJOR,GLUE_LAYER,"error in pthread_cond_wait, error number %d: %s",ret,strerror(ret));
        }
        /*Coverity 110669 Fix End*/
        /* SPR 21671 FIXED START */
		if(thread_unlock)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "oam thead 2");
			break;
        }
        /* SPR 21671 FIXED END */
        /* SPR 21671 FIXED START */
        TRACE(LOG_CRITICAL , GLUE_LAYER, "CSR 131421 thread_msg_oam function");
        /* SPR 21671 FIXED END */

	}

	thread_unlock = 0;
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Existing from %s", __FUNCTION__);
	return NULL;
}

/* Signla handler */
void oam_sighand (void){}

/******************************************************************************
  FUNCTION NAME  : main 
Inputs         : none 
Outputs        : none
RETURNS        : none 
DESCRIPTION    : Main function .
 ******************************************************************************/
int main (void)
{
//	qvInit(0, os);
	qvSimpleInit (&my_os);
	/* SPR 21588 FIXED START  */
	YLINIT((YLIST *)&g_notification_Q);	
	/* SPR 21588 FIXED END */

       /* Coverity 86672 + */
	SInt32  byte_read = 0;
    /* Coverity 86672 - */
	Uint16 trans_id = 0xFFFF; 
	g_trans_init_id = 0xFFFF;
	void *p_buffer = NULL; 
	void *p_buffer1 = NULL;
	UInt32 fd = 0; 

	UInt32 sockfd = 0;
	struct timeval      timeval;

	struct sockaddr_in client;
	/* SPR 18746 FIX START */
	UInt8 *buffer = NULL;
	buffer = ALLOCATE(MAX_RECV_BUFF);
	if(NULL == buffer) //coverity fix
	{
		TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
		exit(0);
	}
	/* SPR 18746 FIX END */

	UInt16  api_idr = 0;
	pthread_t thread_Oam;
	struct sigaction        actions;
	memset(&actions, 0, sizeof(actions));
    /* Coverity 97261 + */
	actions.sa_handler = (__sighandler_t)oam_sighand;
    /* Coverity 97261 - */
	actions.sa_flags = 0;

	time_t t2;

	void *p_ret = NULL;


	UInt8  *URL_IP = NULL;

#ifdef HAVE_IPSEC
	FILE *url_file = NULL;
#endif

	read_glue_config();

	logger_setLogLevel (1);
	logger_init();
    /* SPR 17880 START / Trace Removed / SPR 17880 END*/
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nenable_duplicate_param_check = %d", check_duplicates_spv_flag);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nglue_port = %u", send_port);
    /* SPR 17880 START / Trace Removed /SPR 17880 END*/
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\ninit_ind_wait_interval = %u", g_sleep_ind);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nmanufacturer = %s", Manufacturer);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\noui_env = %s", OUI);
    /* SPR 17880 START / Trace Removed / SPR 17880 END*/
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nproductclass = %s", ProductClass);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nrecv_port = %u", recv_port);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nserialno = %s", SerialNumber);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nusername = %s\n", username);
    /* SPR 17880 START / Trace Removed / SPR 17880 END*/
#ifdef HAVE_IPSEC
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nhave_ipsec = %d\n", have_ipsec);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nipsec_net_prefix = %d\n", ipsec_net_prefix);
#endif

	/*download upload change starts*/
    TRACE(LOG_CRITICAL , GLUE_LAYER, "\ndwn_queue_max = %u", queue_download_max_size);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "\nupl_queue_max = %u", queue_upload_max_size);
	/*download upload change ends*/

	glue_layer_init();
	pthread_cond_init (&count_threshold_cv, NULL);
	pthread_mutex_init(&count_mutex, NULL);
	sigaction(SIGALRM,&actions,NULL);
	UInt32 len = 0 ; 
	Uint16 l_begin = 0; 
	//Tr69Parameters_t   tr69Parameters = {'\0'};
	ParameterValueList   sParamValList = {'\0'};
	sDeviceInfo devidstruct ;
	sCallbackFunList_t      sCallbackFunList = {NULL,NULL,NULL,NULL};

	//IPSEC SSI code starts
#ifdef HAVE_IPSEC

	UInt8 isTunnelError = 0;
	//IPSec pahse2 code starts    
	//populate the crypto params global var to use if there is a file for this
	FILE *file = PNULL;
	g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
	if(NULL == g_ipr_cwmp_crypto_params) //coverity fix
	{
		TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
		exit(0);
	}
	ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));

	//Check if the crypto params file is there.
	file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "a");
	if(file == PNULL)
	{
		("\nFailed to open/create a file to read/store cryptoprofile table params.\n");
        /* SPR_19838: Coverity_12932 Fix Start */
		/* code removed */
        /* SPR_19838: Coverity_12932 Fix Stop */
		return FAILED;
	}
	else
		fclose(file);

	file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
	//open the data model file and read the params
	if(file == NULL)
	{
		TRACE ( 1 , GLUE_LAYER, "Error in opening file to save crypto param ike_rekey_lifetime");
		DEALLOCATE(g_ipr_cwmp_crypto_params);
		/* SPR 18746 FIX START */
		/* code removed */
        /* SPR_19838: Coverity_12932 Fix Stop */
		return FAILED;
	}
	else
	{
		ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
		ENB_FCLOSE(file);
	}
	g_crypto_profile = g_ipr_cwmp_crypto_params->crypto_prof;
	DEALLOCATE(g_ipr_cwmp_crypto_params);

	ENB_MEMSET(&g_act, '\0', sizeof(g_act));
	g_act.sa_sigaction = &sig_hdl;
	g_act.sa_flags = SA_SIGINFO;
	g_tout_val.it_interval.tv_sec = 0;
	g_tout_val.it_interval.tv_usec = 0;
	g_tout_val.it_value.tv_sec = (g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime)?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime;
	g_tout_val.it_value.tv_usec = 0;
	//IPSec pahse2 code ends
	g_ipr_cwmp_config_res = (config_res_t*) ALLOCATE( sizeof(config_res_t)); 
	if(NULL == g_ipr_cwmp_config_res) //coverity fix
	{
		TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
		exit(0);
	}
	memset(g_ipr_cwmp_config_res, 0, sizeof(config_res_t));
	url_file = fopen(TRO69_IP_SAVE, "r");
	if(url_file == NULL)
	{
		TRACE(LOG_CRITICAL, GLUE_LAYER, "Error opening config_res file to restore saved IPs ");
	}
	else
	{
		fread(g_ipr_cwmp_config_res, sizeof(config_res_t), 1, url_file);
		fclose(url_file);
	}

#endif
	//IPSEC SSI code ends
	if (0 == g_sleep_ind)
		g_sleep_ind = 1;


	// 40554 coverity fixes
    /* SPR 17880 START/END  Code Removed */
	devidstruct.Manufacturer = (char *) ALLOCATE (CONFIG_PARAM_LEN);
	if (NULL == devidstruct.Manufacturer)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		exit(0);
	}
	devidstruct.OUI =  (char *) ALLOCATE(CONFIG_PARAM_LEN);
	if (NULL == devidstruct.OUI)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		exit(0);
	}
	devidstruct.ProductClass = (char *) ALLOCATE(CONFIG_PARAM_LEN);
	if (NULL == devidstruct.ProductClass )
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		exit(0);
	}
	devidstruct.SerialNumber = (char *) ALLOCATE(CONFIG_PARAM_LEN);
	if (NULL == devidstruct.SerialNumber)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		exit(0);
	}

	if((Manufacturer[0] != 0) && (OUI[0] != 0) && (ProductClass[0] != 0) && (SerialNumber[0] != 0))
	{

		len = strlen ((const char *)Manufacturer);
		ENB_STRNCPY ((char *)devidstruct.Manufacturer ,(const char *) Manufacturer , len) ;


		len = strlen ((const char *)OUI);
		ENB_STRNCPY ((char *)devidstruct.OUI ,(const char *) OUI , len) ;


		len = strlen ((const char *)ProductClass);
		ENB_STRNCPY ((char *)devidstruct.ProductClass ,(const char *) ProductClass , len) ;


		len = strlen ((const char *)SerialNumber);
		ENB_STRNCPY ((char *)devidstruct.SerialNumber ,(const char *) SerialNumber , len) ;

	}

	sCallbackFunList.invokeCallback = invokeCallbackStub;
	sCallbackFunList.sessionInitCallback = glue_sessionInitCallback;
	sCallbackFunList.sessionFailedCallback = glue_sessionFailedCallback;
	sCallbackFunList.sessionClosedCallback = glue_sessionClosedCallback;

	fd = get_fd();


	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, (PTHREAD_STACK_SIZE/2));
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
/*SPR 20636 Changes Start*/
	p_ret = (void *)(ADDR)pthread_create(&thread_Oam, &attr, thread_msg_oam, (void *)p_buffer);
/*SPR 20636 Changes End*/
	if(p_ret)
		TRACE(LOG_CRITICAL, GLUE_LAYER, "failed to create thread \n");
	pthread_attr_destroy(&attr);


	time (&t2);

	while (1)
	{
		pthread_cond_signal(&count_threshold_cv);
		p_ret = recv_msg_oam (NULL, GLUE_INIT);
		if (p_ret)
		{
			URL_IP = encode_OAM_Config_Res (p_ret, &devidstruct, &tr69Parameters);
	
			if(p_ret)
			g_trans_init_id = get_word_from_header(p_ret);
			if(0 == g_trans_init_id)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
			}
			/* SPR 18746 FIX START */
            /* code removed */
			DEALLOCATE(p_ret);
			/* SPR 18746 FIX END */
			if (URL_IP)
			{
				l_begin = 1;
			}
			else
				len = buffer_OAM_Config_Resp  ('1', &p_buffer1, 0, NULL);
			if (p_buffer1)
			{
				construct_interface_api_header( p_buffer1, trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, TR069_OAM_CONFIG_RESP, len);
				send_msg_oam (p_buffer1 , fd,  len);
				DEALLOCATE(p_buffer1);	
				p_buffer1= NULL;

			}
			thread_unlock = 1;
			break;
		}
		sleep(1);
	}

	 pthread_join(thread_Oam, NULL);
	/*if(0 !=  pthread_join(thread_Oam, NULL))
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "error in func %s thread  joining", __FUNCTION__);
		return 1;
	}
*/
	//IPSEC SSI code starts
#ifdef HAVE_IPSEC
	if(!have_tunnel && have_ipsec && strlen((const char *)g_ipr_cwmp_config_res->HeNBIPAddr) != 0 && (strlen((const char *)g_ipr_cwmp_config_res->SecGWServer1) != 0 || 
				strlen((const char *)g_ipr_cwmp_config_res->SecGWServer2) != 0 || strlen((const char *)g_ipr_cwmp_config_res->SecGWServer3) != 0))
	{
		if(strlen((const char *)g_ipr_cwmp_config_res->SecGWServer1) != 0)
		{
			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, 
						(UInt8*)g_ipr_cwmp_config_res->SecGWServer1, 
						g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
						PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", 
						g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer1);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", 
						g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer1);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->SecGWServer1, 
						(UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr,
						g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, 
						PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", 
						g_ipr_cwmp_config_res->SecGWServer1, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", g_ipr_cwmp_config_res->SecGWServer1, g_ipr_cwmp_config_res->HeNBIPAddr);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer1, g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add in policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else if(!isTunnelError)
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added in policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

			if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->SecGWServer1, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add out policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				isTunnelError = 1;
			}
			else if(!isTunnelError)
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added out policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				if(!ping(g_ipr_cwmp_config_res->HeMSIPAddr))
				{
					TRACE(LOG_CRITICAL, GLUE_LAYER, "Tunnel is esatblished and HeMS %s is pingable", g_ipr_cwmp_config_res->HeMSIPAddr );
				}
				else
				{
					//delete the security assiciations
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer1);
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr);

					//call del policy for HeMS
					if (ipr_cwmp_policy_out_del((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del out policy failed for src = %s and dest = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted out policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					if (ipr_cwmp_policy_in_del((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del in policy failed for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted in policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

					isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
				}
			}
		}//end of SecGW1
		else
			isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
		//changed this due to we need to try next gw in case of failure
		if(isTunnelError && strlen((const char *)g_ipr_cwmp_config_res->SecGWServer2) != 0)
		{
			isTunnelError = 0;//reset this before trying second GW
			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->SecGWServer2, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer2);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer2);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				//signal(SIGALRM, sig_hdl); 
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->SecGWServer2, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", g_ipr_cwmp_config_res->SecGWServer2, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", g_ipr_cwmp_config_res->SecGWServer2, g_ipr_cwmp_config_res->HeNBIPAddr);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer2, g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add in policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else if(!isTunnelError)
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added in policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

			if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->SecGWServer2, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add out policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				isTunnelError = 1;
			}
			else if(!isTunnelError)
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added out policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				if(!ping(g_ipr_cwmp_config_res->HeMSIPAddr))
				{
					TRACE(LOG_CRITICAL, GLUE_LAYER, "Tunnel is esatblished and HeMS %s is pingable", g_ipr_cwmp_config_res->HeMSIPAddr );
				}
				else
				{
					// delete the config and set the tunnel error flag

					//delete the security assiciations
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer2);
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr);

					//call del policy for HeMS
					if (ipr_cwmp_policy_out_del((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del out policy failed for src = %s and dest = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted out policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					if (ipr_cwmp_policy_in_del((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del in policy failed for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted in policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

					isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
				}
			}
		}//end of SecGW2
		if(isTunnelError && strlen((const char *)g_ipr_cwmp_config_res->SecGWServer3) != 0)
		{
			isTunnelError = 0;//reset this before trying second GW
			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->SecGWServer3, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer3);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer3);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (ipr_cwmp_add_state_3des((UInt8*)g_ipr_cwmp_config_res->SecGWServer3, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0, PNULL, ipsec_auth_key, PNULL, ipsec_enc_key))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add state src = %s and  dst = %s failed", g_ipr_cwmp_config_res->SecGWServer3, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added state src = %s and  dst = %s ", g_ipr_cwmp_config_res->SecGWServer3, g_ipr_cwmp_config_res->HeNBIPAddr);
				g_tout_val.it_value.tv_sec = g_crypto_profile.enable?(g_crypto_profile.ipsec_rekey_lifetime_time <= g_crypto_profile.ike_rekey_lifetime?g_crypto_profile.ipsec_rekey_lifetime_time:g_crypto_profile.ike_rekey_lifetime):0; 
				setitimer(ITIMER_VIRTUAL, &g_tout_val,0);
				if (sigaction(SIGALRM, &g_act, NULL) < 0) 
					TRACE(LOG_CRITICAL, GLUE_LAYER, "sigaction failed");
			}

			if (!isTunnelError && ipr_cwmp_policy_add_in((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->SecGWServer3, g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add in policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
				isTunnelError = 1;
			}
			else if(!isTunnelError)
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added in policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

			if (!isTunnelError && ipr_cwmp_policy_add_out((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->SecGWServer3, ipsec_net_prefix, ipsec_net_prefix))
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "add out policy src = %s and  dst = %s failed", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				isTunnelError = 1;
			}
			else if(!isTunnelError)
			{
				TRACE(LOG_CRITICAL, GLUE_LAYER, "added out policy src = %s and  dst = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr );
				if(!ping(g_ipr_cwmp_config_res->HeMSIPAddr))
				{
					TRACE(LOG_CRITICAL, GLUE_LAYER, "Tunnel is esatblished and HeMS %s is pingable", g_ipr_cwmp_config_res->HeMSIPAddr );
				}
				else
				{
					//delete the security assiciations
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer3);
					ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr);

					//call del policy for HeMS
					if (ipr_cwmp_policy_out_del((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del out policy failed for src = %s and dest = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted out policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
					if (ipr_cwmp_policy_in_del((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
						TRACE (LOG_CRITICAL , GLUE_LAYER,"del in policy failed for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
					else
						TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted in policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);

					isTunnelError = 1;//set this to pass the condition in rest two Sec GWs
				}
			}
		}//end of SecGW3

		if (isTunnelError)
		{
			TRACE(LOG_CRITICAL, GLUE_LAYER, "All 3 given SecGWs are either NULL string or tunnel could not be established with any of them");
			send_tunnel_error_oam();


		}
		else
		{
			have_tunnel = 1;
		}

	}
#endif
	//IPSEC SSI code ends

	if (l_begin)
	{
        /* SPR 17268 FIXED START */
		UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
		strcpy((char *)ABSPath1,(char *)ABSPath);
		strncat((char *)ABSPath1 , UPD_DWN_URL , strlen(UPD_DWN_URL));


        /* coverity 96628 fixed start */    
		FILE *fp = ENB_FOPEN((char *)ABSPath1, "rb");
        /* coverity 96628 fixed end */
        /* SPR 17268 FIXED END */
		if (fp != NULL)
		{
			memset(URL_IP , 0 , 128);
			if((0 || EOF) == ENB_FSCANF(fp , "%128s", URL_IP))
				TRACE (LOG_CRITICAL , GLUE_LAYER,"  Error in Fscanf \n ");

			fclose(fp);

			if(0 !=  remove(UPD_DWN_URL))
				TRACE (LOG_CRITICAL , GLUE_LAYER,"  Error in Remove \n ");

			buf_spv = (UInt8 *) ALLOCATE( 500); 
			if (NULL == buf_spv)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
				exit(0);
			}

			unsigned char buf[100]={0x00,0x64,0x00,0x14,0x00,0x0d,0x00,0xb9,0x00,0x69,0x01,0x00,0x00,0x5d,0x01,0x00\
				,0x00,0x00,0x00,0x27,0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,0x47,0x61,0x74,0x65\
					,0x77,0x61,0x79,0x44,0x65,0x76,0x69,0x63,0x65,0x2e,0x4d,0x61,0x6e,0x61,0x67,0x65,0x6d\
					,0x65,0x6e,0x74,0x53,0x65,0x72,0x76,0x65,0x72,0x00,0x00,0x2e,0x00,0x00,0x55\
					,0x52,0x4c,0X3d};

			memcpy(buf_spv,buf,67);

			Uint16 tot_len  = 67;

			//	Uint16 buff_len = 0;

			Uint16 param_val = strlen ((const char *)URL_IP) + 1;

			UInt8 msb= 0 ;
			UInt8 lsb = 0 ;

			//	Uint16 l_len_param = 0 ;

			memcpy (buf_spv + 67, URL_IP , param_val -1); 

			tot_len += param_val;

			construct_interface_api_header(buf_spv, 50, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_SET_PARAMETER_VALUES, tot_len);

			tmp_spv_len = tot_len;
			tot_len -= 12;


			msb = tot_len/256;
			lsb = tot_len%256;

			memcpy (buf_spv+12 , &msb , 1);
			memcpy (buf_spv+13 , &lsb , 1);


			msb = (param_val+8) /256;
			lsb = (param_val+8) % 256;

			memcpy (buf_spv+59 , &msb , 1);
			memcpy (buf_spv+60 , &lsb , 1);

			TRACE (LOG_CRITICAL , GLUE_LAYER,"  Buffer created for spv  ");
		}
        /* SPR_19838:Coverity_110543 Fix Start */
		initialize_TR069_Client (devidstruct , sParamValList , tr69Parameters , sCallbackFunList ,(char * )URL_IP , E_TRUE);
        /* SPR_19838:Coverity_110543 Fix Stop */

        /* SPR_19838: Coverity_12932 Fix Start */
		DEALLOCATE(URL_IP);
        /* SPR_19838: Coverity_12932 Fix Stop */
	}
	//IPSEC SSI code starts
#ifdef HAVE_IPSEC

	if(have_ipsec && have_tunnel)
	{
		//calling del state for all three Sec GWs and HeNB
		ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer1);
		ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer2);
		ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->SecGWServer3);

		if (ipr_cwmp_del_state((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr))
			TRACE (LOG_CRITICAL , GLUE_LAYER,"del state dst =  %s failed", g_ipr_cwmp_config_res->SecGWServer1);
		else
			TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted state dst =  %s ", g_ipr_cwmp_config_res->HeNBIPAddr);
		//call del policy for HeMS
		if (ipr_cwmp_policy_out_del((UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, ipsec_net_prefix, ipsec_net_prefix))
			TRACE (LOG_CRITICAL , GLUE_LAYER,"del out policy failed for src = %s and dest = %s ", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
		else
			TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted out policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeNBIPAddr, g_ipr_cwmp_config_res->HeMSIPAddr);
		if (ipr_cwmp_policy_in_del((UInt8*)g_ipr_cwmp_config_res->HeMSIPAddr, (UInt8*)g_ipr_cwmp_config_res->HeNBIPAddr, ipsec_net_prefix, ipsec_net_prefix))
			TRACE (LOG_CRITICAL , GLUE_LAYER,"del in policy failed for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
		else
			TRACE (LOG_CRITICAL , GLUE_LAYER,"deleted in policy for src = %s and dest = %s", g_ipr_cwmp_config_res->HeMSIPAddr, g_ipr_cwmp_config_res->HeNBIPAddr);
		ipr_cwmp_add_ips(g_ipr_cwmp_config_res);
		have_tunnel = 0;
	}//end of have_ipsec
#endif
	//IPSEC SSI code ends

	sockfd = get_fd();
	timeval.tv_sec = 2;
	timeval.tv_usec = 0;


	len = sizeof(client);

	TRACE(LOG_CRITICAL , GLUE_LAYER,"Indicator for file ind response: %d", g_first_file_ind);
	TRACE(LOG_CRITICAL , GLUE_LAYER,"Is inform Recieved : %d ", g_Is_inform_rec);

	g_Is_inform_rec = 0;

	/* Periodic Inform Changes Start */
	/* SPR 19083 FIXED START*/
	struct timeval start_t;
	struct timeval end_t;
	time_t ret_start = 0;
	time_t ret_end = 0;
	tr069_gettimeofday_wrapper(&start_t);
	ret_start = (time_t)start_t.tv_sec;
	/* SPR 19083 FIXED END */

	/* Periodic Inform Changes End */
	while (1)
	{
		TR069_USLEEP (500000);
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeval,sizeof(timeval)) < 0) {
			perror("Error while set socket option");
		}

		memset(buffer,'\0',MAX_RECV_BUFF);
		byte_read = recvfrom(sockfd,buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len);

        /* Periodic Inform Changes Start */
		/* SPR 19083 FIXED START */
		tr069_gettimeofday_wrapper(&end_t);
		ret_end = (time_t)end_t.tv_sec;
		/* SPR 19083 FIXED END */
        if(TRUE == g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformEnable)
        {
			/* SPR 19083 FIXED START */
			if((ret_end - ret_start) >= (g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformInterval))
            {    
				tr069_gettimeofday_wrapper(&start_t);    
				ret_start = (time_t)start_t.tv_sec;
				/* SPR 19083 FIXED END */
                /* SPR 18104 FIXED START*/
                if(g_invoke_in_perform == 1)
                {
               
                   g_invoke_in_perform_periodic_inform = 1;     
  
                }   
                else{
                
                   TRACE(LOG_CRITICAL , GLUE_LAYER, "Perform init_periodic_inform");
                   init_periodic_inform();
                
                }
               
               /* SPR 18104 FIXED END*/
            }    
        }
        /* Periodic Inform Changes End */

      /*SPR 20202 FIXED START*/
      /*Coverity 109693 Fix Start*/
      if (!byte_read ){		
      /*Coverity 109693 Fix End*/

        continue ;
		}
      /*SPR 20202 FIXED END*/

		else 
		{

//if(buffer[0])
//{
			api_idr = get_word_from_header((U8 *) (buffer) + 6);

/*SPR 17627 FIXED START*/
            if (api_idr == TR069_OAM_HEARTBEAT_REQ)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
                /*send hearbeat response to sysctrl */
                if(1 != send_heartbeat_resp())
                {
                    /*log message response not sent*/
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
                }
                else
                {
                    /*log message response sent successfully */
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
                }

            }
/*SPR 17627 FIXED END*/

/*CSR 57877 Fix START */
			if (api_idr == OAM_NOTIFICATION_IND)
/*CSR 57877 Fix End*/
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
				g_notif_transid = get_word_from_header((U8 *)(buffer));
				if(0 != g_notif_transid)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved  transid");
				}
				else
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
				}
				decode_notification (buffer);
			}
			if (api_idr == TR069_CONTROL_COMMAND_REQ)
			{
				Upload_Dir *upl_dir_t = {0};
				UInt8 ret = 0;
				void * p_buffer_res = NULL;
				UInt16 l_offset = 0;
				U8 msb = 0x00;
				U8 lsb = 0x00;
				Char8 *err_str = FAULT_CODE_9011_STR;

				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Control Cmd Upload ");
				upl_dir_t = decode_oam_cntrl_cmd(buffer);
				if (!upl_dir_t)
					continue;
				/*Coverity fix */
				trans_id = get_word_from_header(buffer);
				if(0 != trans_id)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved  transid");
				}
				else
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
				}
				ret = upload_all_in_Dir(upl_dir_t);
				if (ret != SUCCESS) // We have to send Failure to Oam
				{
					p_buffer_res = (void *)ALLOCATE( UPL_FAIL_BUFFER);
					if (!p_buffer_res)
					{
						DEALLOCATE(upl_dir_t );
						continue;
					}
					l_offset += 12;
					ret = FAILURE;
					memcpy(p_buffer_res + l_offset ,  &ret , sizeof(UInt8)); 
					l_offset += sizeof(UInt8);

					msb = FAULT_CODE_9011/256;
					lsb = FAULT_CODE_9011%256;

					memcpy(p_buffer_res + l_offset , &msb , sizeof(UInt8));
					memcpy(p_buffer_res + l_offset + 1, &lsb , sizeof(UInt8));

					l_offset += sizeof(UInt16);

					l_offset += 4; // Reserved 

					ret = FAULT_CODE_9011_STR_LEN + 1;

					memcpy(p_buffer_res + l_offset ,  &ret , sizeof(UInt8));
					l_offset += sizeof(UInt8);

					memcpy(p_buffer_res + l_offset , err_str , ret - 1); 
					l_offset += ret;

					construct_interface_api_header(p_buffer_res, trans_id, TR069_MODULE_ID,
							OAM_MGMT_INTF_MODULE_ID, TR069_CONTROL_COMMAND_RESP, l_offset);
					send_msg_oam(p_buffer_res , fd, l_offset);
					DEALLOCATE(p_buffer_res);	
				}
				else    
				{
					p_buffer_res = (void *)ALLOCATE( UPL_FAIL_BUFFER);
					if (!p_buffer_res)
					{
						DEALLOCATE(upl_dir_t );
						continue;
					};

					l_offset += 12;
					ret = SUCCESS;

					memcpy(p_buffer_res + l_offset ,  &ret , sizeof(UInt8));
					l_offset += sizeof(UInt8);

					l_offset += 2; // Error code

					l_offset += 4; // Reserved

					l_offset += 1; // length Of error string 

					construct_interface_api_header(p_buffer_res, trans_id, TR069_MODULE_ID,
							OAM_MGMT_INTF_MODULE_ID, TR069_CONTROL_COMMAND_RESP, l_offset);
					send_msg_oam(p_buffer_res , fd, l_offset);
					DEALLOCATE(p_buffer_res);	
				}
				DEALLOCATE(upl_dir_t );
			}
		//}
		// Code for Notif ends here
}
	}

}

/* SPR 17596 fixed start */
/******************************************************************************
 *   FUNCTION NAME  : handle_get_rpc
 *   Inputs         : eType
 *   Outputs        : None 
 *   RETURNS        : void *  - returns p_res as a response. 
 *   DESCRIPTION    : This function handles get RPC request received from ACS.
 *******************************************************************************/
void * handle_get_rpc(int eType)
{
    void *p_res = NULL;

    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s  ",__FUNCTION__);
    if(eType == RPC_GETRPCMETHOD)
    {
        p_res = create_oam_msg_for_get_rpc();
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
        return p_res;
    }
    return p_res;
}

/******************************************************************************
FUNCTION NAME  :    decode_OAM_get_rpc
Inputs         :    void *  ( Input Buffer)  
Outputs        :    MethodList * ( Pointer to RPC Parameter List)   
RETURNS        :    MethodList  - returns MethodList  as response to as success.
DESCRIPTION    :    This function decodes the input buffer which is a response
                    from ACS.  and fills paramterlist in output List..
********************************************************************************/
struct MethodList * decode_OAM_get_rpc(void *p_buffer)
{
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering  %s", __FUNCTION__);

    U8 l_count = 0;

  /* Cov 99902 +*/ 
    int i = 0;    
    struct MethodList* Temp1 = NULL;
  /* Cov 99902 -*/ 
    Uint16 l_offset = 0;
    Uint16 total_params = 0;
    Uint16 rpc_len =0;
    U8 temp_l_len = 0 ;
    l_offset += 12;      //num of parameter


     memcpy ( &temp_l_len  , p_buffer + l_offset , 1);
     total_params = temp_l_len & 0x00FF;
     total_params =  total_params  << 8;
     memcpy (&temp_l_len, p_buffer + l_offset + 1 , 1);
     total_params = total_params |  temp_l_len & 0x00FF;


    TRACE(LOG_CRITICAL , GLUE_LAYER, " ...Total Parameter = %d ", total_params);

    /* GET RPC FIX START */
    //l_offset +=1 ; 
    l_offset +=2 ; 
    l_offset +=2 ;  //reserved 
    /* GET RPC FIX END */

    struct _cwmp__GetRPCMethodsResponse *response = {0};
/*SPR 20636 Changes Start*/
    response = (struct _cwmp__GetRPCMethodsResponse *)(ADDR)ALLOCATE(sizeof(struct _cwmp__GetRPCMethodsResponse));
/*SPR 20636 Changes End*/
    if (NULL == response)
    {
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting %s ALLOCATE failed",__FUNCTION__);
        return NULL;
    }
/* SPR 20636 Changes Start*/
    response->MethodList= (struct MethodList*)(ADDR)ALLOCATE(sizeof(struct MethodList));
/* SPR 20636 Changes End*/
    if(NULL == response->MethodList)
    {
        TRACE(LOG_CRITICAL ,GLUE_LAYER ,"malloc failure");
        /*Cov 99902 + */
		DEALLOCATE(response);
        return NULL;
        /*Cov 99902 - */

    }
    response->MethodList->__size = total_params;
    TRACE(LOG_CRITICAL , GLUE_LAYER ,"......size of list is = %d" , response->MethodList->__size);
/* SPR 20636 Changes Start*/
    response->MethodList->__ptrstring = (char **) (ADDR)ALLOCATE((sizeof (char *)) * total_params);
/* SPR 20636 Changes End*/
    if(NULL == response->MethodList->__ptrstring)
    {
        TRACE(LOG_CRITICAL ,GLUE_LAYER ,"malloc failure");

        /*Cov 99902 + */
		DEALLOCATE(response->MethodList);
		DEALLOCATE(response);
        /*Cov 99902 - */
        return NULL;
    }
    memset(response->MethodList->__ptrstring, '\0' , ((sizeof (char *)) * total_params));
    while(l_count < total_params)
    {
        //rpc_len =0;
        /* GET RPC FIX START */
        //l_offset += 1;
        /* GET RPC FIX END */

        memcpy ( &temp_l_len  , p_buffer + l_offset , 1);
        rpc_len = temp_l_len & 0x00FF;
        rpc_len =  rpc_len << 8;
        memcpy (&temp_l_len, p_buffer + l_offset + 1 , 1);
        rpc_len = rpc_len |  (temp_l_len & 0x00FF);

        TRACE(LOG_CRITICAL , GLUE_LAYER ,"...rpc_len = %d",rpc_len);
        l_offset += 2;

/* SPR 20636 Changes Start*/
        response->MethodList->__ptrstring[l_count] = (char *)(ADDR)ALLOCATE((sizeof(char) * rpc_len));
/* SPR 20636 Changes End*/
        if(NULL == response->MethodList->__ptrstring[l_count])
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER ,"malloc faliure");

        /*Cov 99902 + */
                
            for(i= 0 ;i< l_count; i++)
            {
				DEALLOCATE(response->MethodList->__ptrstring[i]);
            }
			DEALLOCATE(response->MethodList->__ptrstring);
			DEALLOCATE(response->MethodList);
			DEALLOCATE(response);

        /*Cov 99902 - */

            return NULL;
        }

        /* GET RPC FIX START */
        TRACE(LOG_CRITICAL , GLUE_LAYER ,"Parameter Name = %s",(char *)p_buffer + l_offset);
        memset(response->MethodList->__ptrstring[l_count] ,'\0' ,( sizeof(char) * (rpc_len) ));
        strncpy(response->MethodList->__ptrstring[l_count] ,(char *)p_buffer + l_offset , rpc_len);
        /* GET RPC FIX END */
        l_offset += rpc_len ;
        l_count++;
    }
    
  /* Cov 99902 +*/ 
   Temp1 = response->MethodList;
	DEALLOCATE(response); 
   
  return Temp1;

  /* Cov 99902 +*/ 
}

/****************************************************************************
 * Function Name  : create_oam_msg_for_get_rpc
 * Inputs         : none
 * Outputs        : none
 * Returns        : void pointer
 * Description    : This function  creates get parameter request  and send to oam  
 * ** *****************************************************************************/
void * create_oam_msg_for_get_rpc()
{
    /*
    struct _cwmp__GetRPCMethodsResponse *response = {0};
	   response = (struct _cwmp__GetRPCMethodsResponse *) ALLOCATE( sizeof(struct _cwmp__GetRPCMethodsResponse));
    if (NULL == response)
    {
	   TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting %s ALLOCATE failed",__FUNCTION__);
        return NULL;
		}
     */

    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering from %s",__FUNCTION__);
    UInt32 fd = 0;
    void *p_res = NULL;
    transData_t *transD_t = NULL;
    char * buffer_for_oam = NULL;
    fd = get_fd();
    YLIST yList_get_rpc;
	yList_get_rpc.count = 0;
    init_list (&yList_get_rpc);
	buffer_for_oam =  ALLOCATE( sizeof(char ) *  1024);
    if (buffer_for_oam == NULL)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
        return NULL;
    }

    memset(buffer_for_oam , '\0',1024);
    /* Fill API_HEADER */
    g_trans_id++;
    if(g_trans_id >TRANS_RANGE)
    {
        g_trans_id = 1;
    }
    construct_interface_api_header((UInt8 * )buffer_for_oam, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_GET_RPC_METHOD, rpc_get_buff_size);

    /* send msg to oam */
    send_msg_oam((UInt8 *)buffer_for_oam , fd, rpc_get_buff_size);
	DEALLOCATE(buffer_for_oam);

    /* adding transaction_id in the list */
	transD_t = ( transData_t *) ALLOCATE (sizeof(transData_t) );
    if (transD_t == NULL)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
        return NULL;
	}
    transD_t->trans_id = g_trans_id;
    addNodeinSet (&yList_get_rpc ,transD_t);
    p_res = recv_msg_oam (&yList_get_rpc, RPC_GETRPCMETHOD);
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);

    return p_res;
}
/* SPR 17596 fixed end */


/******************************************************************************
 *   FUNCTION NAME  : handle_get_param
 *   Inputs         : msg, eType
 *   Outputs        : None 
 *   RETURNS        : void *  - returns GetStatus as a response. 
 *   DESCRIPTION    : This function handles get parameter RPC request received from ACS.
 *******************************************************************************/
void *  handle_get_param (void *msg, int eType)
{
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s  ",__FUNCTION__);
	char **arrArgNames = NULL;
	Uint16 iCount  = 0 ;
	Uint16 len = 0 ;
	map_map* get_map_ptr = NULL;
	Uint16 size = 0 ;
	char *child_param = NULL;
	char *parent_param = NULL;
	void *p_res = NULL;

	if (eType == RPC_GETPARAMETERVALUES)
	{
		struct sParameterNames *pNames = ( struct sParameterNames *) msg ;
		if ( pNames != NULL && pNames->__ptrstring != NULL)
		{
			arrArgNames = (char **)pNames->__ptrstring;
			size = pNames->__size;
		}
		else
		{
			TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s : Memory unavailable", __FUNCTION__); 
			return NULL;
		}

		get_map_ptr = map_new_value();
		//map Initialization
		if (get_map_ptr  == NULL )
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "\n \n Map not initialized successfully");
			TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s : Map not initialized successfully", __FUNCTION__);
			return NULL;
		}
		while(iCount < size)
		{
			add_brackets((UInt8 *)arrArgNames[iCount]);
            /*SPR_17987_START*/
			parent_param = (char *)ALLOCATE( MAX_BUFF_SIZE);
            if (!parent_param)
            {
				DEALLOCATE(get_map_ptr );
                return NULL;
            }
			child_param = strrchr ((const char *)arrArgNames[iCount], '.');
			if (!child_param)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "\n \n child param null ");
                map_put_value(get_map_ptr,(Uint8 *) parent_param, NULL,(Uint8 *)parent_param );
			}
            else
            {
                len = strlen((const char *)arrArgNames[iCount]);
                if (*(arrArgNames[iCount]+len-1)!='.')
                    len = len - strlen ((const char *)child_param);
                                ENB_STRNCPY((char *)parent_param,(const char *) arrArgNames[iCount] , len );
                                map_put_value(get_map_ptr,(Uint8 *) parent_param, (Uint8 *)(child_param +1),(Uint8 *)parent_param );
            }
            /*SPR_17987_END*/
			iCount++;
			DEALLOCATE(parent_param);
			parent_param = NULL;
		}


		p_res = create_oam_msg_for_get (get_map_ptr);
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
         /* MEMORY LEAK 31 DEC FIXED START */
        if (get_map_ptr) //V9d
			/* Memory Leak 18 Jan change start */
			//map_free(get_map_ptr);
			map_free_1(get_map_ptr);
	     	/* Memory Leak 18 Jan change end */
             /* MEMORY LEAK 31 DEC FIXED END */
		return p_res;
	}
	return p_res; 
}


/******************************************************************************
  FUNCTION NAME  : 	decode_OAM_get
Inputs         :	void *  ( Input Buffer)  
Outputs        :	ParameterValueList * ( Pointer to Parameter List)	 
RETURNS        : 	Uint16  - returns error code  as response or 0 as success.
DESCRIPTION    : 	This function decodes the input buffer which is a response
from ACS.  and fills paramterlist in output variable..
 *******************************************************************************/
Uint16  decode_OAM_get ( void * p_res, ParameterValueList *cpeParamList_t)
{
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering from %s",__FUNCTION__);
	UINT8 l_status = '0';
	unsigned char num_param = 0;
	Uint16 l_count = 0;
	Uint16 l_offset = 0 ;
	Uint16 l_len_param = 0 ;
	Uint16 l_name_len = 0 ;
	//Uint16 l_val_len = 0;
	Uint16 l_fault_code = 0 ;
	U8 l_len_err = 0 ; 
	U8 temp_l_len = 0 ;

	char *p_Name_Val = NULL;
	char *p_EQ = NULL;
	char *p_par_Name = NULL;

	cwmp__ParameterValueStruct *p_cpeParamVal_t = NULL;
	cwmp__ParameterValueStruct **cpeParamArr = NULL;



	if (cpeParamList_t == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\n Internal Error");
		return FAILURE;
	}

	cpeParamArr = cpeParamList_t->__ptrParameterValueStruct;

	l_offset += 12;   /* Skip header Reading  */

	memcpy (&l_status , p_res + l_offset , 1) ;

	TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Received status :%d",__FUNCTION__, l_status);

	l_offset += 1;

	if (l_status == 0)
	{
		/* Skip for Error code parsing */
		l_offset += 6; 		

		memcpy ( &l_len_err,  p_res + l_offset , 1);
		l_offset +=1; 
		
		l_offset += l_len_err;

		/* Retrieve the num of parameters */
		l_offset += 2;

		memcpy (&num_param, p_res + l_offset , 1) ;

		/* Skip  Parsing Parent Node info */
		l_offset += 4;


		memcpy ( &temp_l_len, p_res + l_offset , 1);
		l_len_param = temp_l_len & 0x00FF;
		l_len_param = l_len_param << 8;
		memcpy ( &temp_l_len, p_res + l_offset +1 , 1);
		l_len_param = l_len_param | (temp_l_len & 0x00FF);

		//memcpy (&l_par_len ,  p_res + l_offset  , sizeof(Uint16));
        /*SPR 16453 Fix Start */ 
		p_par_Name = (char *) ALLOCATE( PARANT_NAME_GPV);
        /*SPR 16453 Fix End */ 
		if (!p_par_Name)
			return 0;

		l_offset += 2;

		memcpy ( p_par_Name , p_res + l_offset , l_len_param);

		if (l_len_param > 2)
		{
			if (*(p_par_Name+l_len_param-2)=='.')
				*(p_par_Name+l_len_param-2)=0;
		}

		l_offset +=  l_len_param;  

		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Number of parameters in response : %d", __FUNCTION__ , num_param); 

		TRACE (LOG_CRITICAL , GLUE_LAYER, "Index of parameter list  : %d", g_Index);

        /*Dynamic memory allocation for the parameter recieved for gpv response */
        /* Spr 17463 Fix Start */
			cpeParamArr = (cwmp__ParameterValueStruct **)TR_REALLOC(cpeParamArr ,
						sizeof(cwmp__ParameterValueStruct *) * (num_param + g_Index));
           /* SPR 18135 changes end */
        /* Spr 17463 Fix End */ 
            if(cpeParamArr == NULL)
            {
            TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, could not allocate storage form MsgHandle_FD", __FUNCTION__);
			DEALLOCATE (p_par_Name);
            return GLUE_FAILURE;
            }

            cpeParamList_t->__ptrParameterValueStruct = cpeParamArr;


		while (l_count < num_param)
		{
			l_len_param = 0;
			memcpy ( &temp_l_len, p_res + l_offset , 1);
			l_len_param = temp_l_len & 0x00FF;
			l_len_param = l_len_param << 8;
			memcpy ( &temp_l_len, p_res + l_offset+1 , 1);
			l_len_param = l_len_param | (temp_l_len & 0x00FF);

			//memcpy (&l_len_param , p_res + l_offset ,  sizeof(Uint16));
			l_offset += 4;

			TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Recieved Length of parameter :%d", __FUNCTION__ , l_len_param);

			p_Name_Val  = ( char *) ALLOCATE( NAME_VAL_GPV);
			if (!p_Name_Val)
			{
				DEALLOCATE (p_par_Name);
				return GLUE_FAILURE;
			}
			memcpy (p_Name_Val ,  p_res + l_offset , l_len_param-2);

			TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Recieved Name=Value %s", __FUNCTION__, p_Name_Val);


			p_EQ = strchr ((const char *)p_Name_Val, '=');

			if (p_EQ != NULL)
			{
				l_name_len = p_EQ - p_Name_Val;
				//l_val_len =  strlen ((const char *)(p_EQ+1));
				p_cpeParamVal_t = (cwmp__ParameterValueStruct *) ALLOCATE (sizeof(cwmp__ParameterValueStruct));
				if (NULL == p_cpeParamVal_t)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
					exit(0);
				}

				if (NULL != p_cpeParamVal_t)
				{

                    /*SPR 16453 Fix Start*/ 
					p_cpeParamVal_t->Name = (char *) ALLOCATE( PARANT_NAME_GPV);
                    /*SPR 16453 Fix End */ 
					if (NULL == p_cpeParamVal_t->Name)
					{
						TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
						exit(0);
					}
					p_cpeParamVal_t->Value = (char *) ALLOCATE( 512);
					if (NULL == p_cpeParamVal_t->Value)
					{
						TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
						exit(0);
					}
					p_cpeParamVal_t->Name[0]='\0';
   /* SPR 13658 START */
					*((char *)p_cpeParamVal_t->Value)='\0';
   /* SPR 13658 END */
					/*SPR 20007:CID 57532 Fix Start*/
					/*Code Deleted*/
					/*SPR 20007:CID 57532 Fix Start*/
   /* SPR 13658 START */
                        /* code removed */
   /* SPR 13658 END */
						tr_strncpy((char *) p_cpeParamVal_t->Name ,(const char *) p_par_Name);
						strncat ( p_cpeParamVal_t->Name , ".", 1);
						strncat (p_cpeParamVal_t->Name , p_Name_Val , l_name_len);
                       /*SPR 16453 Fix Start */ 
                        /* Coverity 96640 + */
						p_cpeParamVal_t->Name[PARANT_NAME_GPV - 1] = '\0';
                        /* Coverity 96640 - */
                       /*SPR 16453 Fix End */ 
                        /* SPR 13658 START */
                        if (!(p_cpeParamVal_t->__type = ipr_cwmp_find_parameter_type((char *) p_cpeParamVal_t->Name)))
                        {
                          p_cpeParamVal_t->__type = PAR_TYPE_STRING; 
                        }
                        if(p_cpeParamVal_t->__type == PAR_TYPE_UINT || p_cpeParamVal_t->__type == PAR_TYPE_SINT)
                            (*(int *)(p_cpeParamVal_t->Value)) = atoi((const char *) p_EQ+1);
                        else if(p_cpeParamVal_t->__type == PAR_TYPE_BOOLEAN)
                        {
                            if(0 == strcmp((const char *) p_EQ+1,"1"))
                                (*(int *)(p_cpeParamVal_t->Value)) = 1;
                            else
                                 (*(int *)(p_cpeParamVal_t->Value)) = 0;
                        }
                        else
                            tr_strncpy((char *)p_cpeParamVal_t->Value ,(const char *) p_EQ+1);
                        /* SPR 13658 END */
						remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
						cpeParamArr [g_Index] = p_cpeParamVal_t;
						g_Index++;
					/*SPR 20007:CID 109690 Fix Start*/
					/*Code Deleted*/
					/*SPR 20007:CID 109690 Fix End*/


				}
				else
				{
					l_fault_code = FAILURE;
					TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
					TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s with value  :%d",__FUNCTION__, l_fault_code);
					DEALLOCATE (p_par_Name);
					//coverity fix
					DEALLOCATE(p_Name_Val);
                    /* coverity 86608 fixed code removed */
					return l_fault_code;
				}

			}
			l_offset += l_len_param-4;
			l_count++;
			DEALLOCATE (p_Name_Val);
			p_Name_Val = NULL;
			l_len_param = 0;
		}
		DEALLOCATE (p_par_Name);
	}
	else
	{
		/* Parse the Error String */
		memcpy ( &temp_l_len, p_res + l_offset , 1);
		l_fault_code = temp_l_len & 0x00FF;
		l_fault_code = l_fault_code << 8; 
		memcpy ( &temp_l_len, p_res + l_offset+1, 1);
		l_fault_code = l_fault_code | (temp_l_len & 0x00FF);
		TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s with value  :%d",__FUNCTION__, l_fault_code);
		return l_fault_code;
	}


	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s with value  :%d",__FUNCTION__, l_fault_code);
	return l_fault_code;
}

/****************************************************************************
 * ** ** Function Name  : create_oam_msg_for_get
 * ** ** Inputs         : m, size
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function  creates get parameter request  and send to oam  
 * ** *****************************************************************************/
void * create_oam_msg_for_get (map_map * m)
{

	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering from %s",__FUNCTION__);

	void *p_res = NULL;
	Uint16 count = 0 ;
	Uint16 num_list = 0 ;
	YLIST *p_list = NULL;
	void * p_buffer_for_OAM = NULL;
	//UInt32 trans_id = 0;
	UInt32 fd = 0 ;
	YLIST yList_get;
    /* Coverity 58734 + */
    yList_get.count = 0;
    /* Coverity 58734 - */
	transData_t *transD_t = NULL;  


	fd = get_fd();

	init_list (&yList_get);

	TRACE (LOG_CRITICAL , GLUE_LAYER, "Size of Map for get : %d", m->size);

	for(count =0;count<m->size;count++)
	{
		if (m->data[count].parent_key)
		{
			p_list = &m->data[count].mylist;
			if ( p_list == NULL)
				continue;
			num_list  = ylCount(p_list);
			if( num_list == 0 ) {
				TRACE (LOG_CRITICAL , GLUE_LAYER, " fault in num list : %d", num_list );
				return NULL;    
			}

			p_buffer_for_OAM = buffer_OAM_Get_Parameter_Val (m->data[count].parent_key ,p_list , num_list) ;
			//DEALLOCATE(m->data[count].parent_key);
			DEALLOCATE(m->data[count].parent_key);
            /* SPR 22314 Start */
            DEALLOCATE(m->data[count].child_key);
            /* SPR 22314 End */

			if (p_buffer_for_OAM)
			{
				/* Send Buffer to OAM */
				g_trans_id++;
				if(g_trans_id >TRANS_RANGE)
					g_trans_id = 1;

				construct_interface_api_header(p_buffer_for_OAM , g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_GET_PARAMETER_VALUES , g_get_buff_size);
				send_msg_oam(p_buffer_for_OAM , fd, g_get_buff_size);
				DEALLOCATE(p_buffer_for_OAM);	
				/* coverity 96680 fix start */
				//p_buffer_for_OAM= NULL;
				/* coverity 96680 fix end */
				g_get_buff_size = 0;
				transD_t = ( transData_t *) ALLOCATE(sizeof(transData_t) );	 
				if (NULL == transD_t)
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
					return NULL;
				}
                /* SPR 18135 changes start */
				/* Spr 17463 Fix Start */
				transD_t->trans_id = g_trans_id;	
				addNodeinSet (&yList_get ,transD_t);
				//gpn_trans_id = g_trans_id;
				/* Spr 17463 Fix End */ 
                /* SPR 18135 changes end */
			}
		}
	}
	p_res = recv_msg_oam (&yList_get, RPC_GETPARAMETERVALUES);
	g_Index = 0 ;

	//free_glue_map(m);
	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
	/* SPR 21588 FIXED START  */
   while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
	{
		decode_notification(g_notif_buffer);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
		/* SPR  12059  Fix Start */
		qvFree(g_notif_buffer);
		/* SPR 12059  Fix End*/
		g_notif_buffer = NULL;
		TR069_USLEEP (500);
	}

/* SPR 21588 FIXED END */
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
   /* Memory leak 20 jan fixed start */ 
	/* SPR 21563 FIXED START */
	/* code removed */
    /* SPR 21563 FIXED START */
   /* Memory leak 20 jan fixed end */ 
	return p_res;
}
/* coverity 96673 fix start */
/*SPR 17625 FIXED START/END  Code Removed */

/* coverity 96673 fix end */
/****************************************************************************
 * ** ** Function Name  : buffer_OAM_Get_Parameter_Val
 * ** ** Inputs         : parent, Names, num_param
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function handles get  parameter value request 
 * ** *****************************************************************************/
void * buffer_OAM_Get_Parameter_Val (char * parent, YLIST *Names, Uint16 num_param)
{
	void *buffer_new = NULL;

	Uint16 len = 0;

	U8 msb = 0x00;
	U8 lsb = 0x00;


	UInt32 offset = 0 ;
	Uint16 len_parent_Node = 0 ;
	Uint16 l_total = 0; 
	U8 param_count = '\0';
	Uint16 l_lpn_offset = 0;


	if (num_param > 254)
	{
		param_count = 254;
	}

	else
		param_count = num_param;


	buffer_new =  (void *) ALLOCATE( (276 + 68 * (num_param)) );
	if (NULL == buffer_new)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		return NULL;
	}

	/* API header */

	offset += 12;

	l_lpn_offset = offset;

	/* Total length of parent information*/
	if ( parent != NULL)
		l_total = 8 +  strlen ((const char *)parent) + 1;    // 3 reserved , 2 length , 1 num of param
	else
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nError:NULL in  Parent ");
		DEALLOCATE(buffer_new);
		return NULL;   
	}
	//memcpy (buffer_new+offset , &len_parent_Node , sizeof(Uint16));
	offset += 2;

	/*Number of additional parameters */

	memcpy (buffer_new+offset ,&param_count ,1);
	offset += 1;

	/* Reserved  */
	memset (buffer_new+offset ,0, 1);
	offset += 1;


	/* Reserved  */
	memset (buffer_new+offset ,0, 2);
	offset += 2;


	/* Length of Parent Node Name */
	//coverity fix
	if(0 != strlen((const char *)parent))
		len_parent_Node  = strlen ((const char *)parent) + 1;
	else
		len_parent_Node  = 0;

	msb = len_parent_Node/256;
	lsb = len_parent_Node%256;

	memcpy (buffer_new+offset , &msb , sizeof(U8));
	memcpy (buffer_new+offset+1 , &lsb , sizeof(U8));

	offset += 2;

	//l_total += len_parent_Node;


	/* Parent Node Name */
	memcpy (buffer_new+offset , parent ,  strlen((const char *)parent)+1);
	offset += len_parent_Node;

	if (Names != NULL)
	{
		data_t *node  = NULL;

		while((node = (data_t *)ylPopTail(Names))!=NULL)
		{
			//TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s ,In list :Child %s ,Data %s ", __FUNCTION__ , node->child_key,node->data);
            /*SPR_17987_START*/
            if(node->child_key != NULL)
                len = 4 + strlen ((const char *)node->child_key) + 1;
            else
                len = 5;
            /*SPR_17987_END*/
			l_total += len;

			msb = len/256;
			lsb = len%256;

			memcpy ( buffer_new+offset, &msb,  sizeof(U8));
			memcpy ( buffer_new+offset+1, &lsb , sizeof(U8));

			offset += 2;

			/* Resreved */
			memset ( buffer_new+offset , 0, 2);
			offset +=  2;	

            /*SPR_17987_START*/
            if(node->child_key != NULL)
                memcpy ( buffer_new+offset , node->child_key  , len - 4);
            /*SPR_17987_END*/
			offset += len-4;
			//DEALLOCATE (node);
			DEALLOCATE(node);
			node = NULL;
		}
	}

	msb = l_total/256;
	lsb = l_total%256;
	memcpy (buffer_new + l_lpn_offset , &msb , sizeof(U8));
	memcpy (buffer_new + l_lpn_offset+1 , &lsb,  sizeof(U8));

	g_get_buff_size = offset; 
	return  buffer_new ;
}


/****************************************************************************
 * ** ** Function Name  : handle_oam_download
 * ** ** Inputs         : msg, eType
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function handles oam download request
 * ** *****************************************************************************/
void * handle_oam_download (void *msg)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Enetring in %s", __FUNCTION__);
	U8 msb = 0x00;
	U8 lsb = 0x00;
	U8 byte_1 = 0x00;
	U8 byte_2 = 0x00;
	U8  byte_3 = 0x00;
	U8 byte_4 = 0x00;

	void *buffer_new =  (void *) ALLOCATE( 354);
	if (buffer_new == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		return NULL;

	}

	struct sDownload *ptr_D = (struct sDownload *) msg;

	if (ptr_D == NULL)
	{
		return buffer_new;
	}


	U8 fileType = '\0';
	UInt32 fileSize = 0;
	Uint16 fileLen = 0 ;
	char fileName[256] = {'\0'};
	U8 fd_Len= 0 ;
	Uint16 l_tmp_len = 0;
	Uint16 l_slash = 0;
	char fileDesc[64] = {'\0'};
	UInt32 offset = 0 ;
	U8 *file_path = NULL;

	file_path = (U8 *)ALLOCATE( 256);
	if(NULL == file_path)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nError: Not Enough Memory");
		//coverity fix    
		DEALLOCATE(buffer_new ); 
		return NULL;
	}


	fileSize = ptr_D->FileSize;

	if (ptr_D->TargetFileName && strlen((const char *)ptr_D->TargetFileName) != 0)
	{
		tr_strncpy ((char *)fileName , ptr_D->TargetFileName);
		fileLen  = 4 + strlen ((const char *)fileName);
	}
	else if (ptr_D->URL)
	{
		l_tmp_len = strlen ((const char *)ptr_D->URL);

		if (l_tmp_len == 0)
			l_slash = 1;
		else
			l_slash = l_tmp_len;

		while( ptr_D->URL[l_slash-1]!= '/' && l_slash > 0)
		{
			l_slash--;
		}
		tr_strncpy ((char *)fileName , ptr_D->URL + l_slash); 
		fileLen  = 4 + l_tmp_len - l_slash;
		fd_Len = 0; 
	}
	if (file_path && fileLen > 0)
	{
        /* SPR 17268 FIXED START */
		//getcwd((char *)file_path , 256); 
		//tr_strcat (file_path , "/");

        UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath1,(char*)ABSPath);

        TRACE(LOG_CRITICAL , GLUE_LAYER, "\n ABS file_path before concat = %s" ,ABSPath1 );
        TRACE(LOG_CRITICAL , GLUE_LAYER, "\n file_path before concat of ABSPath1 = %s" ,file_path );

 
        strncat((char *)file_path,(char *) ABSPath1 , strlen((char *)ABSPath1));
      TRACE(LOG_CRITICAL , GLUE_LAYER, "\n file_path after concat = %s", file_path);
  
		tr_strcat (file_path , CPE_FILE_DIR_NAME);
        tr_strcat (file_path , "/");
      /* SPR 17268 FIXED END */
		fileLen = fileLen + strlen((const char *)file_path) + 1;
		strncat((char *)file_path , fileName , 255);
	}
	else
		fileLen = 4;

	if (ptr_D->FileType)
	{
		fileType  = *(ptr_D->FileType) - 48;
	}

	/* API header */
	offset += 12;

	/* General */
	memset ((buffer_new+offset) , 0, 4);
	offset += 4;


	/* File Type */
	memcpy ((buffer_new+offset) , &fileType,  sizeof(U8));
	offset += 1;

	/* Reserved */
	memset (buffer_new+offset , 0, 3);
	offset += 3;

	/* File Size */

	byte_1 = fileSize>>24 ;
	byte_2 = fileSize>>16;
	byte_3 = fileSize>>8;
	byte_4 = fileSize;

	memcpy ((buffer_new+offset) , &byte_1 , sizeof(U8)); 
	memcpy ((buffer_new+offset)+1, &byte_2 , sizeof(U8));
	memcpy ((buffer_new+offset)+2 , &byte_3 , sizeof(U8));
	memcpy ((buffer_new+offset)+3 , &byte_4, sizeof(U8));

	//memcpy ((buffer_new+offset) ,  &fileSize , sizeof(UInt32));
	offset += 4;
	/* File Name Info -> Length */

	msb = fileLen/256;
	lsb  = fileLen%256;
	memcpy (buffer_new+offset ,  &msb , sizeof(U8));
	memcpy (buffer_new+offset +1, &lsb , sizeof(U8));
	//memcpy (buffer_new+offset ,  &fileLen , sizeof(Uint16));
	offset += 2;

	/* Reserved */
	memset (buffer_new+offset , 0, 2);
	offset += 2;

	/* File name */
	memcpy ( buffer_new+offset , file_path , fileLen-4);
	offset += fileLen-4;

	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s, File path send : %s , File Length: %d", __FUNCTION__, file_path, fileLen);
	DEALLOCATE(file_path);

	tr_strncpy ((char *)fileDesc , "LTE eNodeB based File");

	fileDesc[0] = 0;
	fd_Len = 4 + strlen ((const char *)fileDesc) + 1;

	/*File Description ==> Length of File Description */
	memcpy (buffer_new+offset,  &fd_Len , sizeof(U8));
	offset += 1;

	/* Reserved */
	memset (buffer_new+offset , 0, 3);
	offset += 3;

	/* File Description */

	memcpy (buffer_new+offset , fileDesc , fd_Len-4);
	offset += fd_Len-4;

	g_dwn_buffer = offset;

	return buffer_new;
}

/* SPR 17717 fixed start */
/****************************************************************************
 * ** ** Function Name  : handle_oam_alarm_ind
 * ** ** Inputs         : msg, eType
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function handles oam alarm ind request
 * ** ****************************************************************************/
void * handle_oam_alarm_ind  (void)
{
	UInt32 fd = 0;
	void * p_buffer_for_alarm= NULL;
    /* coverity 96635 fixed code removed*/
	SInt32 *l_ret_alarm = NULL;
	
	l_ret_alarm = (SInt32 *) ALLOCATE( sizeof(SInt32));
        if (NULL == l_ret_alarm)
        {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                return NULL;
        }

	p_buffer_for_alarm =  (void *) ALLOCATE( 354);
        if (p_buffer_for_alarm == NULL)
        {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                /*  coverity 96663 fix start */
		DEALLOCATE(l_ret_alarm);
                /*  coverity 96663 fix end */
                return NULL;

        }

	 fd = get_fd();
	g_trans_id++;
        if(g_trans_id >TRANS_RANGE)
        g_trans_id = 1;
	g_alarm_buffer = 12;
        construct_interface_api_header(p_buffer_for_alarm , g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_ALARM_IND, g_alarm_buffer);
        send_msg_oam(p_buffer_for_alarm , fd, g_alarm_buffer);
	DEALLOCATE(p_buffer_for_alarm);
        /* coverity 96646 fixed code removed */ 
	
	 *l_ret_alarm = 1;
         return l_ret_alarm;
}
/* SPR 17717 fixed end */
/* Coverity 114334 Fixed Start */
/****************************************************************************
 * ** ** Function Name  : handle_oam_file_download_ind
 * ** ** Inputs         : msg, eType
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function handles oam file download ind request 
 * ** *****************************************************************************/
void * handle_oam_file_download_ind  (void  *msg , int eType)
{
    SInt32 *l_ret_pre = NULL;
    void * p_buffer_for_down = NULL;
    UInt32 fd = 0;
    transData_t *transD_t = NULL;
    void *p_res = NULL;

    fd = get_fd();

    l_ret_pre = (SInt32 *) ALLOCATE( sizeof(SInt32));
    if (NULL == l_ret_pre)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
        return NULL;
    }

    YLIST yList_Down;

    init_list (&yList_Down);
    /* Coverity 58735 + */
    yList_Down.count = 0;
    /* Coverity 58735 - */



    if (eType == RPC_PREDOWNLOAD)
    {
        *l_ret_pre = 1;
        return l_ret_pre;
    }

    if (eType == RPC_DOWNLOAD)
    {
        /*Review Comments incorporation */
        struct sDownload *ptr_D = (struct sDownload *) msg;
        if (ptr_D == NULL)
        {
            //coverity fix
            DEALLOCATE(l_ret_pre);
            return NULL;
        };

        if (*(ptr_D->FileType) != FIRM_UPGRADE  &&  *(ptr_D->FileType) != VENDOR_CONFIG)
        {
            DownloadStatus *p_dwn_t = NULL;
            p_dwn_t = (DownloadStatus *)ALLOCATE( sizeof(DownloadStatus));
            if ( NULL == p_dwn_t ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                DEALLOCATE(l_ret_pre);
                return 0;
            }
            p_dwn_t->eStatus = PASS ; 
            DEALLOCATE(l_ret_pre);    
            return p_dwn_t;
        }

		p_buffer_for_down = handle_oam_download (msg);
		if (p_buffer_for_down)
		{
			g_trans_id++;
			if(g_trans_id >TRANS_RANGE)
				g_trans_id = 1;
			construct_interface_api_header(p_buffer_for_down , g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_FILE_DOWNLOAD_IND, g_dwn_buffer);
			send_msg_oam(p_buffer_for_down , fd, g_dwn_buffer);
			ENB_FREE(p_buffer_for_down);	
			g_dwn_buffer = 0;
			transD_t = ( transData_t *) ALLOCATE ( sizeof(transData_t));
			if ( NULL == transD_t ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
				ENB_FREE(l_ret_pre);
				return 0;
			}
			transD_t->trans_id = g_trans_id;
			addNodeinSet (&yList_Down , transD_t);
			p_res = recv_msg_oam_Download(&yList_Down);
		}
	}
	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
            /* SPR 21588 FIXED START  */
    while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
      {
		decode_notification(g_notif_buffer);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
		qvFree(g_notif_buffer);
		g_notif_buffer = NULL;
	}
/* SPR 21588 FIXED END */



    DEALLOCATE(l_ret_pre);
    /*  coverity 58732 fix start */
    DEALLOCATE(transD_t);
    /*  coverity 58732 fix end */
    return p_res;
}
/* Coverity 114334 Fixed End */

/****************************************************************************
 * ** ** Function Name  : decode_OAM_Download_Res
 * ** ** Inputs         : p_res, p_dwn_t
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function decodes oam_download_resp
 * ** *****************************************************************************/
void decode_OAM_Download_Res (void *p_res, DownloadStatus * p_dwn_t)
{

	Uint16 l_offset = 0 ;
	U8 l_status = 0 ;
	Uint16 l_err_code = 0 ;
	U8 temp_l_len = 0 ;

	/* Skid header Reading */
	l_offset += 12;

	memcpy (&l_status , p_res+l_offset , 1) ;

	l_offset += 1;

	if ( l_status == 0)
	{

		p_dwn_t->eStatus = PASS;
	}
	else
	{
		/* Parse the Error code and String */
		p_dwn_t->eStatus = FAULT;
		memcpy ( &temp_l_len, p_res + l_offset , 1);
		l_err_code = temp_l_len & 0x00FF;
		l_err_code = l_err_code << 8;
		memcpy ( &temp_l_len, p_res + l_offset+1, 1);
		l_err_code = ((l_err_code) | (temp_l_len & 0x00FF));
		l_offset += 2;

		/* Reserved */
		l_offset += 4;

		p_dwn_t->fault_code = l_err_code;

		temp_l_len = 0 ;

		memcpy (&temp_l_len ,  p_res + l_offset , 1);
		l_offset += 1;

		p_dwn_t->fault_string = (char *) ALLOCATE( 80);	
		if (p_dwn_t->fault_string)
			memcpy (p_dwn_t->fault_string ,  p_res + l_offset , temp_l_len); 	

	}
}

/****************************************************************************
 * ** ** Function Name  : encode_OAM_Config_Res
 * ** ** Inputs         : p_buffer
 * ** ** Outputs        : none
 * ** ** Returns        : void pointer
 * ** ** Description    : This function encodes oam_config_rqe
 * ** *****************************************************************************/
void * encode_OAM_Config_Res (void *p_buffer, sDeviceInfo *devidstruct, Tr69Parameters_t *tr69Parameters) 
{

	Uint16 l_offset = 0 ;
	U8 l_num_param = '\0' ;
	U8 l_count = '\0';
	char *p_EQ = NULL;
	char *p_Name_Val = NULL;	
	Uint16 l_len = 0 ; 
	Uint16 l_str_len = 0;
	U8 temp_l_len = 0 ; 
	void *p_ret_buffer = NULL;
	UInt8* url_ip = NULL;//IPSEC SSI code, needed without HAVE_IPSEC flag also
    /* SPR 17880 START */
    time_t tmpTime = 0 ;
    Uint8 zone[32] = {0};
    struct tm Periodic_Time = {0};
    Sint32 conv = 0;
    /* SPR 17880 END */
#ifdef HAVE_IPSEC
	UInt8* get_ip_addr = NULL;//IPSEC phase2 code, needed without HAVE_IPSEC flag also
#endif
	/* Skid header Reading */

	l_offset += 12;

	memcpy (&l_num_param , p_buffer+l_offset, 1);

	/* Reserved Bytes  */

	l_offset += 4;


	while (l_count < l_num_param)
	{
		memcpy (&temp_l_len, p_buffer+l_offset , 1);
		l_len = temp_l_len & 0x00FF;
		l_len =   l_len  << 8;
		memcpy (&temp_l_len, p_buffer+l_offset+1 , 1);
		l_len = ((l_len) |  (temp_l_len & 0x00FF));

		p_Name_Val  = (char *)ALLOCATE( MAX_BUFF_SIZE);
		if (NULL == p_Name_Val)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			/*Review comments incorporated */
			//coverity fix
			if(NULL != p_ret_buffer)
				DEALLOCATE(p_ret_buffer);
			return NULL;
		}

		memset(p_Name_Val , '\0',MAX_BUFF_SIZE );

		if (l_len > 4) 
			memcpy(p_Name_Val , p_buffer+l_offset+4 , l_len-4);

		p_EQ = strchr((const char *)p_Name_Val , '=');

		if (p_EQ != NULL && p_EQ > p_Name_Val)
		{
            /* SPR 17880 START */
			if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_SHEMS_PASSWORD, p_EQ - p_Name_Val) == 0 ) 
			{
				tr69Parameters->Password = (char *) ALLOCATE( strlen((const char *)p_EQ));
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_SHEMS_PASSWORD= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                if (NULL == tr69Parameters->Password)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                    exit(0);
                }
			    ENB_STRNCPY (tr69Parameters->Password ,(const char *) (p_EQ + 1), strlen((const char *)(p_EQ + 1)));
            }
			if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_SHEMS_USERNAME, p_EQ - p_Name_Val) == 0 ) 
			{
				tr69Parameters->Username = (char *) ALLOCATE( strlen((const char *)p_EQ));
				TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_SHEMS_USERNAME= %s  value = %d", p_EQ, strlen(p_EQ));
                if (NULL == tr69Parameters->Username)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
					DEALLOCATE(tr69Parameters->Password);
                    exit(0);
                }
			    ENB_STRNCPY (tr69Parameters->Username ,(const char *) (p_EQ + 1), strlen((const char *)(p_EQ + 1)));
            }
			if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_ENABLE_CWMP, p_EQ - p_Name_Val) == 0 ) 
			{
                TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_ENABLE_CWMP= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                /* Periodic Inform Enable Changes Start */
                if ( (*(p_EQ + 1) == 48) || (0 == strcmp((const char *) p_EQ+1,"FALSE")) || (0 == strcmp((const char *) p_EQ+1,"false")) )
                /* Periodic Inform Enable Changes End */
                {
                    ipr_cwmp_setEnableCWMP(false);
                }
                /* Periodic Inform Enable Changes Start */
                else if ( (*(p_EQ + 1) == 49) || (0 == strcmp((const char *) p_EQ+1,"TRUE")) || (0 == strcmp((const char *) p_EQ+1,"true")) )
                /* Periodic Inform Enable Changes End */
                {
                    ipr_cwmp_setEnableCWMP(true);
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, " Invalid value %s of EnableCWMP ", p_EQ);
                }
            }

            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORMTIME, p_EQ - p_Name_Val) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PERIODIC_INFORMTIME= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                *zone = '\0';
                memset((void*)&Periodic_Time, 0, sizeof(Periodic_Time));

                if(0 == sscanf((const char *)(p_EQ + 1),"%4d-%2d-%2dT%2d:%2d:%2d%31s", &Periodic_Time.tm_year, &Periodic_Time.tm_mon, &Periodic_Time.tm_mday, &Periodic_Time.tm_hour, &Periodic_Time.tm_min, &Periodic_Time.tm_sec, zone))
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value %s for PeriodicInformTime", p_EQ + 1);
                    perror("sscanf error");
                }
                tmpTime = ipr_cwmp_setPeriodicInformTime(Periodic_Time.tm_mon , Periodic_Time.tm_mday , Periodic_Time.tm_hour , Periodic_Time.tm_min , Periodic_Time.tm_sec,0);

                if (tmpTime !=  0)
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value %s for PeriodicInformTime", p_EQ);
                }

            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PARAMETER_KEY, strlen(p_EQ)) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PARAMETER_KEY= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                ipr_cwmp_setParameterKey((Char8 *)(p_EQ + 1));
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CWMP_RETRY_MINIMUM_WAIT_INTERVAL, p_EQ - p_Name_Val) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PARAMETER_KEY= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                ipr_cwmp_setCWMPRetryMinimumWaitInterval(atoi(p_EQ + 1));
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CWMP_RETRY_INTERVAL_MULTIPLIER, p_EQ - p_Name_Val) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PARAMETER_KEY= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                conv  = strtoul((const char *)(p_EQ + 1), NULL , 0);
                if (conv < 1000)
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value %s for CwmpRetryIntervalMultiplier", p_EQ);
                }
                else
                {
                    ipr_cwmp_setCWMPRetryIntervalMultiplier(conv);
                }

            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CONNECTION_REQUEST_USERNAME, p_EQ - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_CONNECTION_REQUEST_USERNAME= %s lenght of value = %d", p_EQ, strlen(p_EQ));
				tr69Parameters->ConnectionRequestUsername  =  (char *) ALLOCATE( strlen(p_EQ));
                if (NULL == tr69Parameters->ConnectionRequestUsername)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                    exit(0);
                }

                strncpy(tr69Parameters->ConnectionRequestUsername, (p_EQ + 1), strlen(p_EQ + 1));
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CONNECTION_REQUEST_PASSWORD, p_EQ - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_CONNECTION_REQUEST_PASSWORD= %s lenght of value = %d", p_EQ, strlen(p_EQ));
				tr69Parameters->ConnectionRequestPassword  =  (char *) ALLOCATE( strlen(p_EQ));
                if (NULL == tr69Parameters->ConnectionRequestPassword)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
                    exit(0);
                }

                strncpy(tr69Parameters->ConnectionRequestPassword, (p_EQ + 1), strlen(p_EQ + 1));
            }

            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORM_INTERVAL, p_EQ - p_Name_Val) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PERIODIC_INFORM_INTERVAL= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                conv = strtoul((const char *)(p_EQ + 1), NULL , 10);
                //Setter method
                ipr_cwmp_setPeriodicInformInterval (conv);

            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORM_ENABLE, p_EQ - p_Name_Val) == 0 ) 
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, " value of MANAGEMENT_SERVER_PERIODIC_INFORM_ENABLE= %s lenght of value = %d", p_EQ, strlen(p_EQ));
                /* Periodic Inform Enable Changes Start */
                if ( (*(p_EQ + 1) == 48) || (0 == strcmp((const char *) p_EQ+1,"FALSE")) || (0 == strcmp((const char *) p_EQ+1,"false")) )
                /* Periodic Inform Enable Changes End */
                {
                    //ipr_cwmp_setPeriodicInformEnable(false);
                    tr69Parameters->PeriodicInformEnable = false;
                    g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformEnable = FALSE;
                }
                /* Periodic Inform Enable Changes Start */
                else if ( (*(p_EQ + 1) == 49) || (0 == strcmp((const char *) p_EQ+1,"TRUE")) || (0 == strcmp((const char *) p_EQ+1,"true")) )
                /* Periodic Inform Enable Changes End */
                {
                    //ipr_cwmp_setPeriodicInformEnable(true);
                    tr69Parameters->PeriodicInformEnable = true;
                    g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformEnable = TRUE;
                }
                else
                {
					TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value of PeriodicInformEnable received");
                }
            }
            /* SPR 17880 END */

			if (ENB_STRNCMP((const char *)p_Name_Val , SECGWS1 , p_EQ - p_Name_Val) == 0 ) 
			{
				if(*(p_EQ + 1) < '1' || *(p_EQ + 1) > '9')
				{
#ifdef HAVE_IPSEC
					get_ip_addr = ipr_cwmp_get_ip(p_EQ+1);
					if(get_ip_addr)
					{
						ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer1, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer1));
						ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer1, (const char *)get_ip_addr, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
						DEALLOCATE(get_ip_addr);
					}
					else
						TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP returned from DNS/Given IP is not pingable");
#endif
				}
				else
#ifdef HAVE_IPSEC
					ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer1, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer1));
				ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer1,(const char *) p_EQ+1, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
#endif
				TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP passed as SecGw1");
				//config_res_t.SecGWServer1   copy 

			}		
			if (ENB_STRNCMP((const char *)p_Name_Val , SECGWS2 , p_EQ - p_Name_Val) == 0 ) 
			{
				if(*(p_EQ + 1) < '1' || *(p_EQ + 1) > '9')
				{
#ifdef HAVE_IPSEC
					get_ip_addr = ipr_cwmp_get_ip(p_EQ+1);
					if(get_ip_addr)
					{ 
						ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer2, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer2));
						ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer2, (const char *)get_ip_addr, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
						DEALLOCATE(get_ip_addr);
					}
					else
						TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP returned from DNS");
#endif
				}
				else
#ifdef HAVE_IPSEC
					if(p_EQ+1)
						ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer2, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer2));
				ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer2, (const char *)p_EQ+1, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
#endif
				TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP passed as SecGw2");

				//config_res_t.SecGWServer2   copy 

			}		
			if (ENB_STRNCMP((const char *)p_Name_Val , SECGWS3 , p_EQ - p_Name_Val) == 0 ) 
			{
				if(*(p_EQ + 1) < '1' || *(p_EQ + 1) > '9')
				{
#ifdef HAVE_IPSEC
					get_ip_addr = ipr_cwmp_get_ip(p_EQ+1);
					if(get_ip_addr)
					{
						ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer3, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer3));
						ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer3, (const char *)get_ip_addr, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
						DEALLOCATE(get_ip_addr);
					}
					else
						TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP returned from DNS");
#endif
				}
				else
#ifdef HAVE_IPSEC
					if(p_EQ+1)
						ENB_MEMSET(&g_ipr_cwmp_config_res->SecGWServer3, 0, strlen((const char *)g_ipr_cwmp_config_res->SecGWServer3));
				ENB_STRNCPY((char *)g_ipr_cwmp_config_res->SecGWServer3,(const char *) p_EQ+1, strlen((const char *)(p_EQ+1)));   //IPSEC SSI code, needed without HAVE_IPSEC flag also
#endif
				TRACE(LOG_CRITICAL , GLUE_LAYER, "no IP passed as SecGw3");

				//config_res_t.SecGWServer1   copy 

			}		
			if (ENB_STRNCMP((const char *)p_Name_Val , HEMSIPADDRESS , p_EQ - p_Name_Val) == 0) 
			{
				// IPSEC SSI temp code to be decided url or ip
				if(*(p_EQ+1) == 'h')
				{
					//coverity fix
					url_ip = (UInt8*)ALLOCATE(64);
					if(NULL == url_ip)
					{
						TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
						/*Review comments incorporated */
						DEALLOCATE(p_Name_Val);
                        /*  coverity 86669 fix code removed */
						return NULL;
					}

					tr_strncpy((char *)url_ip, p_EQ+1);
					ipr_cwmp_urltoip(&url_ip);
					if(url_ip)
					{
#ifdef HAVE_IPSEC
						ENB_MEMSET(&g_ipr_cwmp_config_res->HeMSIPAddr, 0, strlen((const char *)g_ipr_cwmp_config_res->HeMSIPAddr));
						tr_strncpy((char *)g_ipr_cwmp_config_res->HeMSIPAddr, url_ip);   //copy
#endif
						DEALLOCATE (url_ip);
					}
				}
				else
				{
#ifdef HAVE_IPSEC
					ENB_MEMSET(&g_ipr_cwmp_config_res->HeMSIPAddr, 0, strlen((const char *)g_ipr_cwmp_config_res->HeMSIPAddr));
					tr_strncpy((char *)g_ipr_cwmp_config_res->HeMSIPAddr, p_EQ+1);   //copy
#endif
				}

				if (strlen((const char *)(p_EQ + 1)) >= NULL_IP_ADD_LEN && ENB_STRNCMP((const char *)(p_EQ + 1) , NULL_IP_ADD , NULL_IP_ADD_LEN) != 0)
				{                
					//config_res_t.HEMSIPADDRESS   copy 
					TRACE(LOG_CRITICAL , GLUE_LAYER, "URL Is not  empty, Binding to SHems");
					p_ret_buffer =  (U8 *)ALLOCATE( 128);								
					if (!p_ret_buffer)
					{
						/*Review comments incorporated */
						DEALLOCATE(p_Name_Val);
                        /*  coverity 86669 fix code removed */
						return NULL;	
					}
					//snprintf(p_ret_buffer , 128 , "http://%s:%s/ACSServlet",p_EQ+1 ,DEFAULT_PORT);
					ENB_STRNCPY ((char *)p_ret_buffer ,(const char *) p_EQ+1 , 128);
					shems_Session = 1;
				}
				else
				{
					TRACE(LOG_CRITICAL , GLUE_LAYER, "URL Is empty in Config Response , copying Ihems address"); 
					iHems_session = 1;
					//Copy iHems address 
				}

				//commented this code to execute flollwing IPSEC SSI code                return p_ret_buffer;	
			}		
			if (ENB_STRNCMP((const char *)p_Name_Val , HENBSELF , p_EQ - p_Name_Val) == 0) 
			{
/*SPR 17610 Fixed Start*/
/* SPR 16568 FIX START */
				ENB_MEMSET(HeNB_SELF_IP, 0, 40);
				ENB_STRNCPY((char *)HeNB_SELF_IP, p_EQ+1,40);   
				TRACE(LOG_CRITICAL , GLUE_LAYER, "HeNB Self IP Address = %s",HeNB_SELF_IP); 
/* SPR 16568 FIX END */
/*SPR 17610 Fixed End*/
#ifdef HAVE_IPSEC
				ENB_MEMSET(&g_ipr_cwmp_config_res->HeNBIPAddr, 0, strlen((const char *)g_ipr_cwmp_config_res->HeNBIPAddr));
				tr_strncpy((char *)g_ipr_cwmp_config_res->HeNBIPAddr, p_EQ+1);   //IPSEC SSI code, needed without HAVE_IPSEC flag also
#endif
				//config_res_t.HEMSIPADDRESS   copy 
			}		
			if (ENB_STRNCMP((const char *)p_Name_Val , DEVICEINFO_MANU , p_EQ - p_Name_Val) == 0)
			{
				if (devidstruct->Manufacturer)
				{
					l_str_len = strlen((const char *)devidstruct->Manufacturer);
					if (l_str_len)
						memset(devidstruct->Manufacturer , 0 , l_str_len);
					ENB_STRNCPY((char *)devidstruct->Manufacturer ,(const char *) p_EQ+1 , CONFIG_PARAM_LEN);             
				}
			}
			if (ENB_STRNCMP((const char *)p_Name_Val , DEVICEINFO_OUI , p_EQ - p_Name_Val) == 0)
			{
				if (devidstruct->OUI)
				{
					l_str_len = strlen((const char *)devidstruct->OUI);
					if (l_str_len)
						memset(devidstruct->OUI , 0 , l_str_len);
					ENB_STRNCPY((char *)devidstruct->OUI , (const char *)p_EQ+1 , CONFIG_PARAM_LEN);
				}
			}
			if (ENB_STRNCMP((const char *)p_Name_Val , DEVICEINFO_PROD_CLASS , p_EQ - p_Name_Val) == 0)
			{
				if (devidstruct->ProductClass)
				{
					l_str_len = strlen((const char *)devidstruct->ProductClass);
					if (l_str_len)
						memset(devidstruct->ProductClass , 0 , l_str_len);
					ENB_STRNCPY((char *)devidstruct->ProductClass ,(const char *) p_EQ+1 , CONFIG_PARAM_LEN);
				}
			}
			if (ENB_STRNCMP((const char *)p_Name_Val , DEVICEINFO_SRN_NO , p_EQ - p_Name_Val) == 0)
			{
				if (devidstruct->SerialNumber)
				{
					l_str_len = strlen((const char *)devidstruct->SerialNumber);
					if (l_str_len)
						memset(devidstruct->SerialNumber,  0 , l_str_len);
					ENB_STRNCPY((char *)devidstruct->SerialNumber ,(const char *) p_EQ+1 , CONFIG_PARAM_LEN);
				}
			}
#ifdef VENDOR_SPECIFIC 
			if (ENB_STRNCMP((const char *)p_Name_Val , DEVICEINFO_SF_VERS , p_EQ - p_Name_Val) == 0)
			{
				tr69Parameters->SoftwareVers   =  ALLOCATE(CONFIG_PARAM_LEN);
				if (tr69Parameters->SoftwareVers)
				{
					ENB_STRNCPY((char *)tr69Parameters->SoftwareVers ,(const char *) p_EQ+1 , CONFIG_PARAM_LEN);
				}
			}
#endif
			/*TLS CURL CHANGES*/
	            	if (ENB_STRNCMP((const char *)p_Name_Val,HEMS_FQDN, p_EQ - p_Name_Val) == 0)
            		{
						/*TR ZOMBIE FIX*/
						Uint16 url_len = strlen((const char *)(p_EQ+1));
						if (url_len >= NULL_IP_ADD_LEN )
						{
                			ENB_STRNCPY((char *)g_hems_fqdn , (const char *)p_EQ + 1 , 128);
						}
            		}
            

			if (ENB_STRNCMP((const char *)p_Name_Val , IHEMS_URL_NAME , p_EQ - p_Name_Val) == 0)
			{
				Uint16 url_len = strlen((const char *)(p_EQ+1));

				if (url_len >= NULL_IP_ADD_LEN && ENB_STRNCMP((const char *)(p_EQ+1) , NULL_IP_ADD , NULL_IP_ADD_LEN) != 0)
				{
					if (NULL == p_ret_buffer) 
					{
						p_ret_buffer =  (U8 *)ALLOCATE( 128);
						if (!p_ret_buffer)
						{ 
							/*Review comments incorporated */
							DEALLOCATE(p_Name_Val);
                            /*  coverity 86669 fix code removed */
							return NULL;
						}
						ENB_STRNCPY ((char *)p_ret_buffer ,(const char *) p_EQ+1 , 128);
					}
					// Copy ihems URL buffer
					ENB_MEMSET (iHems_url , 0 , 128);
					ENB_STRNCPY((char *)iHems_url, (const char *)p_EQ + 1 , 128);
				}
			}
			if (ENB_STRNCMP((const char *)p_Name_Val , LTE_SCANTIME , p_EQ - p_Name_Val) == 0)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Scan time : %s", p_EQ + 1); 
				IsLte_ScanTimeExists = 1;
				ENB_STRNCPY((char *)g_Lte_LastScanTime , (const char *)p_EQ + 1 , 128);
/* SPR 16568 FIX START */
			} 
			if (ENB_STRNCMP((const char *)p_Name_Val , CONNECTION_REQ_URL , p_EQ - p_Name_Val) == 0)
			{
				g_flag_to_notify_connection_req_url = 1;
                /* SPR_19838: Coverity_12932 Fix Start */
				/*if (NULL != tr69Parameters->ConnectionRequestURL)
				  {
				  DEALLOCATE(tr69Parameters->ConnectionRequestURL);
				  tr69Parameters->ConnectionRequestURL = NULL;
				  }*/
                /* SPR_19838: Coverity_12932 Fix Stop */
				tr69Parameters->ConnectionRequestURL   =  ALLOCATE(CONFIG_PARAM_LEN);
				if (tr69Parameters->ConnectionRequestURL)
				{
					ENB_STRNCPY((char *)tr69Parameters->ConnectionRequestURL ,(const char *) p_EQ+1 , CONFIG_PARAM_LEN);
				}
/* SPR 16568 FIX END */
			}
            /*TLS CURL CHANGES*/
		}
		DEALLOCATE(p_Name_Val);
		p_Name_Val = NULL;                                                                                               		
		l_offset += l_len;
		l_count++;
	}
	//IPSEC SSI code starts
#ifdef HAVE_IPSEC
	//g_ipr_cwmp_config_res->isBootstrap = 1;
	ipr_cwmp_add_ips(g_ipr_cwmp_config_res);
	//DEALLOCATE(url_ip);
#endif
	//IPSEC SSI code ends
	/*TLS CURL CHANGES*/
    ENB_FPRINTF(stderr, "Error in socket open:%s %s\n", iHems_url,g_hems_fqdn);
	g_is_fqdn = 0;
	if(ENB_STRNCMP((const char *)iHems_url,(const char *)g_hems_fqdn,128) == 0)
	{
    ENB_FPRINTF(stderr, "Error in socket open:%s %s\n", iHems_url,g_hems_fqdn);
		g_is_fqdn = 1;
	    if(1 == iHems_session)
		{
    ENB_FPRINTF(stderr, "Error in socket open:%s %s\n", iHems_url,g_hems_fqdn);
			/*TR ZOMBIE FIX*/
            /* Coverity 97093 + */
			if( (p_ret_buffer) && (strlen((const char *)iHems_url) >0))
            /* Coverity 97093 - */
			{
				ENB_STRNCPY ((char *)p_ret_buffer ,(const char *)iHems_url,128);
			}
		 }
	}
    
	return p_ret_buffer;
}

/* SPR 17880 START */
Uint16 decode_set_parameter_indication(void *p_buffer) 
{

	Uint16 buffer_offset = 0 ;
	U8 parameter_count = '\0', loop_counter = '\0', *p_value = NULL, *p_Name_Val = NULL;	
	Uint16 length = 0 ; 
	U8 temp_l_len = 0 ; 
    time_t timer_val = 0 ;
    Uint8 zone[32] = {0};
    struct tm Periodic_Time = {0};
    Sint32 conv = 0;
    Uint16  ret_val = SUCCESS;
	/* Skid header Reading */

	buffer_offset += 12;

	memcpy (&parameter_count, p_buffer + buffer_offset, 1);

	/* Reserved Bytes  */

	buffer_offset += 4;

	while (loop_counter < parameter_count)
	{
		memcpy (&temp_l_len, p_buffer + buffer_offset, 1);
		length = temp_l_len & 0x00FF;
		length =   length << 8;
		memcpy (&temp_l_len, p_buffer + buffer_offset + 1 , 1);
		length = ((length) |  (temp_l_len & 0x00FF));

		p_Name_Val  = (U8 *)ALLOCATE(MAX_BUFF_SIZE);
		if (NULL == p_Name_Val)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
            /* Coverity 97479 + */
            ret_val = FAILURE;
            break;
            /* Coverity 97479 - */
		}

		memset(p_Name_Val , '\0',MAX_BUFF_SIZE );

		if (length > 4) 
			memcpy(p_Name_Val , p_buffer + buffer_offset + 4 , length -4);

		p_value = (U8 *)strchr((const char *)p_Name_Val , '=');

		if (p_value != NULL && p_value > p_Name_Val)
        {
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_SHEMS_PASSWORD, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_SHEMS_PASSWORD = %s value = %d", p_value + 1, strlen((const char *)p_value));
                if (ipr_cwmp_setPassword((Char8*) (p_value + 1)) != SUCCESS)
                {
                    TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of Password received %s",(char *)p_value + 1);
                    ret_val = FAILURE;
                }
            }
            /* SPR 17880 18JAN FIX START */
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_SHEMS_USERNAME, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_SHEMS_USERNAME = %s value = %d", p_value + 1, strlen((const char *)p_value));
            /* SPR 17880 18JAN FIX END */
                if (ipr_cwmp_setUsername((Char8* ) (p_value + 1)) != SUCCESS)
                {
                    TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of Username received %s",(char *)p_value + 1);
                    ret_val = FAILURE;
                }
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_ENABLE_CWMP, p_value- p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_ENABLE_CWMP= %s value = %d", p_value + 1, strlen((const char *)p_value));
                /* Periodic Inform Enable Changes Start */
                if ( (*(p_value + 1) == 48) || (0 == strcmp((const char *) p_value+1,"FALSE")) || (0 == strcmp((const char *) p_value+1,"false")) )
                /* Periodic Inform Enable Changes End */
                {
                    ipr_cwmp_setEnableCWMP(false);
                }
                /* Periodic Inform Enable Changes Start */
                else if ( (*(p_value + 1) == 49) || (0 == strcmp((const char *) p_value+1,"TRUE")) || (0 == strcmp((const char *) p_value+1,"true")) )
                /* Periodic Inform Enable Changes End */
                {
                    ipr_cwmp_setEnableCWMP(true);
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, " Invalid value %s of EnableCWMP ", p_value + 1);
                    ret_val = FAILURE;
                }
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORMTIME, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_PERIODIC_INFORMTIME= %s value = %d", p_value, strlen((const char *)p_value));
                *zone = '\0';
                memset((void*)&Periodic_Time, 0, sizeof(Periodic_Time));

                if(0 == sscanf((const char *)(p_value + 1),"%4d-%2d-%2dT%2d:%2d:%2d%31s", &Periodic_Time.tm_year, &Periodic_Time.tm_mon, &Periodic_Time.tm_mday, &Periodic_Time.tm_hour, &Periodic_Time.tm_min, &Periodic_Time.tm_sec, zone))
                {
                    perror("sscanf error");
                    ret_val = FAILURE;
                }
                timer_val = ipr_cwmp_setPeriodicInformTime(Periodic_Time.tm_mon , Periodic_Time.tm_mday , Periodic_Time.tm_hour , Periodic_Time.tm_min , Periodic_Time.tm_sec,0);

                if (timer_val !=  0)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value %s for PeriodicInformTime", p_value + 1);
                    ret_val = FAILURE;
                }
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PARAMETER_KEY, strlen((const char *)p_value)) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_PARAMETER_KEY= %s value = %d", p_value + 1, strlen((const char *)p_value + 1));
                ipr_cwmp_setParameterKey((Char8 *)(p_value + 1));
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CWMP_RETRY_MINIMUM_WAIT_INTERVAL, p_value- p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "MANAGEMENT_SERVER_CWMP_RETRY_MINIMUM_WAIT_INTERVAL= %s value = %d",
                p_value + 1, strlen((const char *)p_value + 1));
                ipr_cwmp_setCWMPRetryMinimumWaitInterval(atoi((const char *)p_value + 1));
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CWMP_RETRY_INTERVAL_MULTIPLIER, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "MANAGEMENT_SERVER_CWMP_RETRY_INTERVAL_MULTIPLIER = %s value = %d", p_value + 1, strlen((const char *)p_value + 1));
                conv  = strtoul((const char *)(p_value + 1), NULL , 0);
                if (conv < 1000)
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value %s for CwmpRetryIntervalMultiplier", p_value + 1);
                    ret_val = FAILURE;
                }
                else
                {
                    ipr_cwmp_setCWMPRetryIntervalMultiplier(conv);
                }

            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CONNECTION_REQUEST_USERNAME, p_value- p_Name_Val) == 0 ) 
            {
               TRACE(LOG_CRITICAL , GLUE_LAYER, "MANAGEMENT_SERVER_CONNECTION_REQUEST_USERNAME= %s value = %d", p_value, strlen((const char *)p_value));
                if (ipr_cwmp_setConnectionRequestUsername((Char8 *)p_value + 1) != SUCCESS)
                {
                    TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of ConnectionRequestUsername received %s",(char *)p_value + 1);
                    ret_val = FAILURE;
                }
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_CONNECTION_REQUEST_PASSWORD, p_value - p_Name_Val) == 0 ) 
            {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "MANAGEMENT_SERVER_CONNECTION_REQUEST_PASSWORD= %s value = %d",(char *)p_value + 1, strlen((const char *)(p_value + 1)));
                if (ipr_cwmp_setConnectionRequestPassword((Char8 *)p_value + 1) != SUCCESS)
                {
                    TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of ConnectionRequestPassword received %s",(char *)p_value + 1);
                    ret_val = FAILURE;
                }
            }

            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORM_INTERVAL, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_PERIODIC_INFORM_INTERVAL= %s value = %d", p_value + 1, strlen((const char *)p_value + 1));
                conv = strtoul((const char *)(p_value + 1), NULL , 10);
                ipr_cwmp_setPeriodicInformInterval (conv);

            }
            if (ENB_STRNCMP((const char *)p_Name_Val , MANAGEMENT_SERVER_PERIODIC_INFORM_ENABLE, p_value - p_Name_Val) == 0 ) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, " MANAGEMENT_SERVER_PERIODIC_INFORM_ENABLE= %s value = %d", p_value + 1, strlen((const char *)p_value + 1));
                /* Periodic Inform Enable Changes Start */
                if ( (*(p_value + 1) == 48) || (0 == strcmp((const char *) p_value+1,"FALSE")) || (0 == strcmp((const char *) p_value+1,"false")) )
                /* Periodic Inform Enable Changes End */
                {
	                tr69Parameters.PeriodicInformEnable = false;
                    ipr_cwmp_setPeriodicInformEnable(false);
                }
                /* Periodic Inform Enable Changes Start */
                else if ( (*(p_value + 1) == 49) || (0 == strcmp((const char *) p_value+1,"TRUE")) || (0 == strcmp((const char *) p_value+1,"true")) )
                /* Periodic Inform Enable Changes End */
                {
	                tr69Parameters.PeriodicInformEnable = true;
                    ipr_cwmp_setPeriodicInformEnable(true);
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "Invalid value of PeriodicInformEnable received");
                    ret_val = FAILURE;
                }
            }
            if (ENB_STRNCMP((const char *)p_Name_Val , HEMSIPADDRESS , p_value - p_Name_Val) == 0) 
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "HEMSIPADDRESS = %s value = %d", p_value + 1, strlen((const char *)p_value + 1));
                if (strlen((const char *)(p_value + 1 )) >= NULL_IP_ADD_LEN && ENB_STRNCMP((const char *)(p_value + 1) , NULL_IP_ADD , NULL_IP_ADD_LEN) != 0)
                {                
                    ipr_cwmp_setACSURL((Char8 *)p_value);
                }
                else
                {
                    TRACE(LOG_CRITICAL , GLUE_LAYER, "URL Is empty in Config Response , copying Ihems address"); 
                    ret_val = FAILURE;
                }
            }		
            if (ENB_STRNCMP((const char *)p_Name_Val , CONNECTION_REQ_URL , p_value - p_Name_Val) == 0)
            {
                if (ipr_cwmp_setConnectionRequestURL((Char8 *)(p_value + 1)) != SUCCESS)
                {
                    TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value received for ConnectionRequestURL %s",(char *)p_value + 1);
                    ret_val = FAILURE;
                }
            }
        }
		DEALLOCATE(p_Name_Val);
		p_Name_Val = NULL;                                                                                               		
		buffer_offset += length;
		loop_counter++;
	}

	return ret_val;
}
/* SPR 17880 END */


/******************************************************************************
 *  *   FUNCTION NAME  : buffer_OAM_Config_Resp
 *  *   Inputs         : status,p_buffer
 *  *   Outputs        : none
 *  *   RETURNS        : Uint16
 *  *   DESCRIPTION    : This function will construct OAM_Config_Resp ,and send to oam 
 * *******************************************************************************/
Uint16  buffer_OAM_Config_Resp  (U8 status, void **p_buffer , Uint16 error_code , U8 *err_str)
{

	/* SPR 19124 FIXED START */
	TRACE(LOG_DETAIL , GLUE_LAYER,"Entering In function %s",__FUNCTION__);
	/* SPR 19124 FIXED END */
	U8 len = '\0'; 
	//char *err_string = ERR_STR;//IPSEC SSI code
	Uint16 l_offset = 0;
	static U8 hit = 0;

	U8 msb = 0 ;
	U8 lsb = 0;

	if (err_str)
		len = strlen((const char *)err_str) + 1;


	/* API header */

	l_offset += 12;    	

	if (status == '1')
	{
		if (hit == 1)
		{
			*p_buffer = NULL;
			return 0;
		}

		hit = 1;

		*p_buffer = ALLOCATE(((sizeof(char *) * (l_offset+8+len))));//coverity fix 
		if ( NULL == *p_buffer)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			return 0;
		}
		/*Status */
		memset (*p_buffer + l_offset , 1, 1);
		l_offset += 1;

		/*  Error Code  */
		msb = error_code/256;
		lsb = error_code%256;
		memcpy (*p_buffer + l_offset , &msb , 1);
		memcpy (*p_buffer + l_offset + 1 , &lsb , 1);
		l_offset += 2;

		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 1);
		l_offset += 1;

		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 3);
		l_offset += 3;


		/* Length of Error String */
		//IPSEC SSI code len += 1;

		memcpy (*p_buffer + l_offset , &len, 1);
		l_offset += 1;


		/* Error String */
		if (len > 1)
			memcpy (*p_buffer + l_offset , err_str, len);
		l_offset += len;

	}
	else
	{              
		if (hit == 1)
		{
			*p_buffer = NULL;
			return 0;
		}
		hit = 1;

		*p_buffer = ALLOCATE(((sizeof(char *) * ( l_offset + 8))));
		if (*p_buffer == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			exit(0);
		}

		/*Status */
		memset (*p_buffer + l_offset , 0, 1);
		l_offset += 1;

		/*  Error Code  */
		error_code = 0;
		memcpy (*p_buffer + l_offset , &error_code, 2);
		l_offset += 2;


		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 1);
		l_offset += 1;

		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 3);
		l_offset += 3;

		/* Length of Error String */
		len = '\0';

		memcpy (*p_buffer + l_offset , &len, 1);
		l_offset += 1;

	}    
	return l_offset;
}

//IPSEC SSI code starts
//this function is needed without HAVE_IPSEC flag also so leaving this beyond flag

/******************************************************************************
 * *  *   FUNCTION NAME  : ipr_cwmp_urltoip
 * *  *   Inputs         : UInt8** url
 * *  *   Outputs        : none
 * *  *   RETURNS        : none
 * *  *   DESCRIPTION    : This function will convert url to ip
 * * *******************************************************************************/
void ipr_cwmp_urltoip(UInt8** url)
{
	if(*url == NULL)
		return ;
	//coverity
	char* QWord1 = NULL ;
	char* QWord4 = NULL;

	QWord1 = strrchr((const char *)*url, '/');
	if (QWord1  == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s strrchr returned null ",__FUNCTION__);
		exit(0);        
	}
	*QWord1 = '\0';
	QWord1 = strrchr((const char *)*url, '/');
	if (QWord1  == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s strrchr returned null ",__FUNCTION__);
		exit(0);        
	}
	QWord1 += 1;
	QWord4 = strrchr((const char *)*url, ':');
	if (QWord4  == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s strrchr returned null ",__FUNCTION__);
		exit(0);        
	}
	*QWord4 = '\0';
	/* Valgrind fix for source dest overlap*/
	memmove((void *)*url, (const void *)QWord1, strlen((const char *)*url));
	TRACE(LOG_CRITICAL , GLUE_LAYER, "returning url is %s", *url);

}

#ifdef HAVE_IPSEC

/******************************************************************************
 * * *  *   FUNCTION NAME  : ipr_cwmp_add_ips
 * * *  *   Inputs         : config_res_t *ips
 * * *  *   Outputs        : none
 * * *  *   RETURNS        : none
 * * *  *   DESCRIPTION    : This function will save ip 
 * * * *******************************************************************************/
void ipr_cwmp_add_ips (config_res_t *ips)
{
	FILE *file = NULL;

	if (ips)
	{
		file = fopen(TRO69_IP_SAVE, "w");
		if(file == NULL)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Error opening file to save IPs ");
			return ;
		}

		fwrite(ips, sizeof(config_res_t), 1, file);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Saved IPs \n");

		fclose(file);
		return ;
	}
	else
	{
		return ;
	}

}

#endif
//IPSEC SSI code ends

/******************************************************************************
 * * * *  *   FUNCTION NAME  : buffer_OAM_Control_Cmd_Req
 * * * *  *   Inputs         : U8 num_param,
 *                             char *arg1_Name_Val,
 *                             char *arg2_Name_Val ,
 *                             char *arg3_Name_val ,
 *                             char *arg4_Name_Val ,
 *                             UInt32 *buffer_size ,
 *                             UInt8 *contrl_cmd  
 * * * *  *   Outputs        : none
 * * * *  *   RETURNS        : none
 * * * *  *   DESCRIPTION    : This function creates buffer for control command request
 * * * * *******************************************************************************/
void * buffer_OAM_Control_Cmd_Req (U8 num_param  ,
		char *arg1_Name_Val, 
		UInt32 *buffer_size ,
		UInt8 *contrl_cmd)
{
	void *buffer_new = NULL;

	UInt32 offset = 0 ;
	Uint16 len_admin = 0 ;
	Uint16 l_len_cmd = 0 ; 
	char Cmd[255] = {'\0'};
	Uint16  param_len = 0 ;

	/* SPR 19021 fix start */
	Uint8 is_internal_reboot = TR069_ONE;
	/* SPR 19021 fix end */


	

	U8 msb = 0x00;
	U8 lsb = 0x00;
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);

	tr_strncpy ((char *)Cmd ,(const char *) contrl_cmd);
	l_len_cmd = strlen((const char *)Cmd) + 1;

	if (num_param > 4) 
		num_param = 4;

	buffer_new =  (void *) ALLOCATE( (280 + 132 * (num_param)) );
	if (NULL == buffer_new)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		return NULL;
	}

	/* API header */

	offset += 12;

	/* Length of Admin Control Command Specific information*/
	len_admin = 12 + l_len_cmd;      /* 13 (2 + 3  + 4  ) is for Reserved */

	msb = len_admin/256;
	lsb = len_admin%256;

	memcpy(buffer_new+offset , &msb , sizeof(U8));
	memcpy (buffer_new+offset+1 , &lsb , sizeof(U8));
	offset += 2;

	/* Reserved  */
	memset (buffer_new+offset ,0, 2);
	offset += 2;


	/*.AdminControlCommand=shutdown. */

	memcpy (buffer_new+offset , Cmd, l_len_cmd);
	offset += l_len_cmd;

	/*Number of additional parameters */

	memcpy (buffer_new+offset ,&num_param , 1);
	offset += 1;

	/* ReserVed */

	/* SPR 19021 fix start */
	/* ReserVed */
	if(g_Reboot_recv)
	{
		TRACE(LOG_DETAIL , GLUE_LAYER, "Internal Reboot: g_Reboot_recv =  %u" , g_Reboot_recv);
        /* SPR 22092 FIXED Start */
        /* Code Remove */
        /* SPR 22092 FIXED End */

		*((Uint8*)(buffer_new +offset)) = is_internal_reboot;
		offset += TR069_ONE;
		ENB_MEMSET(buffer_new+offset ,TR069_ZERO, TR069_TWO);
		offset += TR069_TWO;
	}
	else
	{
		TRACE(LOG_DETAIL , GLUE_LAYER, "External Reboot: g_Reboot_recv =  %u" , g_Reboot_recv);

		ENB_MEMSET(buffer_new+offset ,TR069_ZERO,TR069_THREE);
		offset += TR069_THREE;
	}
	/* ReserVed */
	/* SPR 19021 fix end*/

	/* ReserVed */
	memset (buffer_new+offset ,0, 4);
	offset += 4;

	if (num_param > 0)
	{
		/*   Additional parameter 1  */
		if ( arg1_Name_Val != NULL)
		{

			/* Length of Additional Parameter */
			param_len = strlen((const char *)arg1_Name_Val) + 1 + 2 + 2 ;
			msb = param_len/256;
			lsb = param_len%256;

			memcpy(buffer_new+offset , &msb , sizeof(U8));
			memcpy (buffer_new+offset+1 , &lsb , sizeof(U8));
			offset += 2;

			/* Reserved  */
			memset (buffer_new+offset ,0, 2);
			offset += 2;


			/*.AdminControlCommand=shutdown. */

			memcpy (buffer_new+offset , arg1_Name_Val, strlen((const char *)arg1_Name_Val));
			offset +=  strlen((const char *)arg1_Name_Val) + 1 ;

		}
	}

	if (buffer_size)
	{
		*buffer_size = offset;
	}
	return (buffer_new);
}

/******************************************************************************
 * * *  *   FUNCTION NAME  : handle_set_url
 * * *  *   Inputs         : void * p_URL
 * * *  *   Outputs        : none
 * * *  *   RETURNS        : none
 * * *  *   DESCRIPTION    : This function handles set url request
 * * * *******************************************************************************/
#ifdef USE_HANDLE_SET
void handle_set_url (void * p_URL)
{
	void *l_p_buffer = NULL;
	UInt32 l_buffer_len = 0;
	UInt32 sockfd, ret_bind, port = 0 ;
	SInt32 result = 0,byte_read = 0 ;
	struct sockaddr_in serverAddr;
	struct timeval      timeval;
	fd_set set;
	UInt8 *buffer = NULL;
	UInt32 fd = 0 ;
	fd = get_fd();
	FILE *url_file = NULL;
	transData_t g_transD_t;
	transData_t *p_retback_t = NULL;
	transData_t *transD_t = NULL;
	/* SPR 19083 FIXED START*/
	struct timeval start_t;
	struct timeval end_t;
	time_t ret_start = 0;
	time_t ret_end = 0;
	tr069_gettimeofday_wrapper(&start_t);
	ret_start = (time_t)start_t.tv_sec;

	ret_end = tr069_gettimeofday_wrapper(&end_t);
	ret_end = (time_t)end_t.tv_sec;
	/* SPR 19083 FIXED END */

	struct sockaddr_in client;
	UInt32 len = sizeof(client);
	UInt16 l_rec_trans_id = 0;
	UInt32 l_offset = 0;
	U8 l_stat = 0;
	UInt8 *contrl_cmd = "AdminControlCommand=SHUTDOWN";

	sockfd = get_fd();
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	YLIST cntrl_cmd_list;

	buffer = (char *)ALLOCATE( MAX_RECV_BUFF); 
	if (NULL == buffer)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		return NULL;
	}

	l_p_buffer = buffer_OAM_Control_Cmd_Req(0 , NULL, &l_buffer_len,contrl_cmd);

	if (l_p_buffer)
	{
		g_trans_id++;
		if (g_trans_id > TRANS_RANGE)
			g_trans_id = 1;
		construct_interface_api_header(l_p_buffer , g_trans_id, TR069_MODULE_ID, 
/* SPR 11921 Fix Start*/
				OAM_MGMT_INTF_MODULE_ID, OAM_CONTROL_COMMAND_REQ, l_buffer_len); 
/* SPR 11921 Fix End*/
		send_msg_oam(l_p_buffer , fd, l_buffer_len);
		DEALLOCATE(l_p_buffer);	
		l_p_buffer= NULL;
		init_list (&cntrl_cmd_list);
		transD_t = (transData_t *) ALLOCATE( sizeof(transData_t));
		if (NULL ==  transD_t)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			return NULL;
		}
		transD_t->trans_id = g_trans_id;
		addNodeinSet (&cntrl_cmd_list , transD_t);

		//if (p_URL)
		remove (BOOTSTRAP_FILE_NAME);

		TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, Waiting for control_cmd_response",__FUNCTION__);

		while (!(ylEmpty(&cntrl_cmd_list)))
		{
			FD_ZERO( &set );
			FD_SET( sockfd, &set );
			timeval.tv_sec = 0;
			timeval.tv_usec = 0;


			/* SPR 19083 FIXED START*/
			/* sg wait_limit change start */
			if (ret_end - ret_start > 240)
				/* sg wait_limit change end */
			{

				/* SPR 19083 FIXED END */
				TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s,  Waiting for response over",__FUNCTION__);              
				//remove (SECURITY_GATEWAY_FILE);
				//remove (URL_FILE_NAME);
				url_file = NULL;
     /* SPR 17268 FIXED START */
       
         UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
         strcpy((char*)ABSPath1,(char *)ABSPath);
 
        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath before concat of BOOTSTRAP_FILE_NAME %s ",ABSPath1 );
        strncat(ABSPath1 , BOOTSTRAP_FILE_NAME, strlen(BOOTSTRAP_FILE_NAME));
        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath after concat of BOOTSTRAP_FILE_NAME  %s ",ABSPath );
        url_file = fopen(ABSPath1 ,"w");
        /* SPR 17268 FIXED END */

		//		url_file = fopen(BOOTSTRAP_FILE_NAME, "w");
				if (url_file)
					fclose (url_file);
				break;
			}
			result = select(FD_SETSIZE, &set, NULL, NULL, &timeval);
			if (result < 0)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "\n select Error");
				exit(1);
			}
			if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
			{
				DEALLOCATE (buffer);
				buffer = NULL;

				/* SPR 19083 FIXED START */
				tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				/* SPR 19083 FIXED END */
				continue;
			}

			//if(buffer[0])
			{
			l_rec_trans_id =  get_word_from_header(buffer);
			if(0 != l_rec_trans_id)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved  transid");
				g_transD_t.trans_id = l_rec_trans_id;
			}
			else
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
			}
			p_retback_t = (transData_t *)remove_node_from_Set  (&cntrl_cmd_list, &g_transD_t);
			if (p_retback_t != NULL)
			{
					/* SPR 19083 FIXED START*/
					tr069_gettimeofday_wrapper(&start_t);
					ret_start = (time_t)start_t.tv_sec;


					/* SPR 19083 FIXED END */
				REM_FROM_LIST (&cntrl_cmd_list , (YLNODE *)p_retback_t);
				/* Decode The buffer */ 
				l_offset += 12;
				memcpy (&l_stat, buffer + l_offset , sizeof(U8));
				TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s,  recieved cntrl cmd response with status : %c",__FUNCTION__, l_stat);
				if (l_stat == 1)
				{
					//remove (SECURITY_GATEWAY_FILE);
					//remove (URL_FILE_NAME);
					url_file = NULL;


        /* SPR 17268 FIXED START */
        UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath2,(char *)ABSPath);

        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath before concat of BOOTSTRAP_FILE_NAME  %s ",ABSPath2 );
        strncat(ABSPath2 , BOOTSTRAP_FILE_NAME, strlen(BOOTSTRAP_FILE_NAME));
        TRACE ( LOG_CRITICAL , GLUE_LAYER , "ABSPath after concat of BOOTSTRAP_FILE_NAME  %s ",ABSPath2 );
        url_file = fopen(ABSPath2 ,"w");
        /* SPR 17268 FIXED END */

					//url_file = fopen(BOOTSTRAP_FILE_NAME, "w");
					if (url_file)
						fclose (url_file); 
					break;
				}
			}
			else
					/* SPR 19083 FIXED START*/					
					tr069_gettimeofday_wrapper(&end_t);
				ret_end = (time_t)end_t.tv_sec;
				/* SPR 19083 FIXED END*/

		}
	}
	}
	return;
}
#endif // FUnction not in use

/******************************************************************************
 * * *  *   FUNCTION NAME  : read_glue_config
 * * *  *   Inputs         : none
 * * *  *   Outputs        : none
 * * *  *   RETURNS        : none
 * * *  *   DESCRIPTION    : This function reads client config file
 * * * *******************************************************************************/
void read_glue_config(void)
{
	FILE *fp = NULL;
	UInt8 config_buffer[CONFIG_BUFFER_SIZE] = {0};

	TRACE(LOG_CRITICAL , GLUE_LAYER, "\n In function %s Reading CWMP Client config File: %s... \n", __FUNCTION__, CONFIG_FILE);

	fp = fopen(CONFIG_FILE, "r");
	if (fp == NULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nERROR: Could not open CWMP Client config file %s\n", __FUNCTION__);
		exit(0);
	}

	if(0 != fseek(fp, 0, SEEK_END))
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nERROR: Could not seek file%s\n",CONFIG_FILE);
		exit(0);
	}
	signed long len = (unsigned long)ftell(fp);
	if( -1 == len)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nERROR:  ftell failure%s\n",CONFIG_FILE);
		exit(0);
	}

	if(0 != fseek(fp, 0, SEEK_SET))
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "\nERROR: Could not seek file%s\n",CONFIG_FILE);
		exit(0);
	}

	while(fgets((char *)config_buffer, len, fp) != NULL)
	{
        /* SPR 17880 START */
        if (ENB_STRNCMP((const char *)config_buffer, "ENABLE_DUPLICATE_PARAM_CHECK=", 29)==0 )
            /* SPR 17880 END */
			check_duplicates_spv_flag = getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "GLUE_PORT=", 10)==0 )
			send_port = getVal(config_buffer);
        /* SPR 17880 START/END  Code Removed */
		else if (ENB_STRNCMP((const char *)config_buffer, "INIT_IND_WAIT_INTERVAL=", 23)==0 )
			g_sleep_ind = getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "MANUFACTURER=", 13)==0 )
			getValStr(config_buffer, Manufacturer);
		else if (ENB_STRNCMP((const char *)config_buffer, "OUI_ENV=", 8)==0 )
			getValStr(config_buffer, OUI);
        /* SPR 17880 START/END  Code Removed */
		else if (ENB_STRNCMP((const char *)config_buffer, "PRODUCTCLASS=", 13)==0 )
			getValStr(config_buffer, ProductClass);
		else if (ENB_STRNCMP((const char *)config_buffer, "RECV_PORT=", 10)==0 )
			recv_port=getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "SERIALNO=", 9)==0 )
			getValStr(config_buffer, SerialNumber);
		else if (ENB_STRNCMP((const char *)config_buffer, "USERNAME=", 9)==0 )
			getValStr(config_buffer, username);
        /* SPR 17880 START/END Code Removed */
#ifdef HAVE_IPSEC
		else if (ENB_STRNCMP((const char *)config_buffer, "HAVE_IPSEC=", 11)==0 )
			have_ipsec=getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "IPSEC_NET_PREFIX=", 16)==0 )
			ipsec_net_prefix=getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "IPSEC_ENC_KEY=", 14)==0 )
			getValStr(config_buffer, ipsec_enc_key);
		else if (ENB_STRNCMP((const char *)config_buffer, "IPSEC_AUTH_KEY=", 15)==0 )
			getValStr(config_buffer, ipsec_auth_key);
#endif
		else if (ENB_STRNCMP((const char *)config_buffer, "UDP_TX_IP=", 10)==0)
			getValStr(config_buffer, OAM_IP);
		else if (ENB_STRNCMP((const char *)config_buffer, "UDP_RX_IP=", 10)==0)
			getValStr(config_buffer, TR069_IP);
		/* SPR 16327 Fixed Start  */
		else if (strncmp((const char *)config_buffer, "MAX_SESSION_RETRY_COUNT=", 24)==0)
			MaxSessionRetryCount = getVal(config_buffer);
		/* SPR 16327 Fixed End  */
		/* SPR 16645 START */
		else if (strncmp((const char *)config_buffer, "DOWNLOAD_TIME_OUT=", 18)==0)
			g_Download_time_out_thres = getVal(config_buffer);
		/* SPR 16645 END */



		/*download upload change starts*/
        else if (ENB_STRNCMP((const char *)config_buffer, "DWN_QUEUE_MAX=", 14)==0 )
		    queue_download_max_size = getVal(config_buffer);
		else if (ENB_STRNCMP((const char *)config_buffer, "UPL_QUEUE_MAX=", 14)==0 )
		    queue_upload_max_size = getVal(config_buffer);
		/*download upload change ends*/
          /* SPR 17268 FIXED START */
        else if (strncmp((const char *)config_buffer, "ABS_PATH=", 9)==0)
        {
            /*  coverity 96638 fix start */
            getValStr(config_buffer ,  ABSPath);
            /*  coverity 96638 fix end */
            
            ABS_len = strlen ((char *)ABSPath);
            TRACE (1 , "READ_CONFIG" , "ABSPATH length is  %d", ABS_len);
}
        /* SPR 17268 FIXED END */

	}
	fclose(fp);
}

/******************************************************************************
  FUNCTION NAME  : decode_notification 
Inputs         : Void * :  Buffer.
Outputs        : .
RETURNS        : Void * : 
DESCRIPTION    : This function  handles request recieved from OAM , i.e Nitofication
request. (Notification event , Alarm management (Queued  and 
Expedited Events). 
 *******************************************************************************/
void decode_notification (void * p_buffer)
{
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering  %s", __FUNCTION__);
	Uint16 l_offset = 0;
   /* SPR 21294 FIXED START */
   Uint16 ret_val = 0;
   /* SPR 21294 FIXED END */
	U8 l_tmp_8 = 0;
	U8 l_tot_par_8 = 0;
	U8 l_num_param = 0; 
	ParameterValueList *cpeParamList_t = NULL;
	cwmp__ParameterValueStruct **cpeParamArr = NULL;
	cwmp__ParameterValueStruct *p_cpeParamVal_t = NULL; 
	//ParameterValueList tmpForced = {0};
	cwmp__ParameterValueStruct **forcedArr = NULL;
	U8 arr_offset = 0;
	U8 *p_EQ = NULL;
	U8 *p_Name_Val = NULL;
	U8 l_count = 0; 
	U8 l_inner_count = 0;
	U8 *parent_name = 0; 
	Uint16 parent_len = 0;
	Uint16 l_tot_params = 0; 
	U8 IsqueuedEvent = 0;
	//U8 IsFapControl  =  0;
	U8 Isexpedited = 0;
	//U8 IsValChanged = 0;
    /*SPR 17625 fIXED START*/
    U8 Isfaplte = 0;
    U8 IsForcedInform = 0;
    /*SPR 17625 fIXED END*/



    /* Coverity 86567 + */
	//tmpForced = (ParameterValueList) g_forcedParamList;
	//coverity fix
	//tmpForced.__ptrParameterValueStruct = (ParameterValueList) g_forcedParamList.__ptrParameterValueStruct
        //forcedArr = tmpForced.__ptrParameterValueStruct;
    forcedArr = (cwmp__ParameterValueStruct**) g_forcedParamList.__ptrParameterValueStruct;
    /* Coverity 86567 - */
	if(NULL ==  forcedArr) 
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "ERROR: IN  %s , forcesarr value is NULL  ", __FUNCTION__);
		exit(0);
	}

	l_offset += 12;
    /*  coverity 87083 fix start */
	Uint16 l_tmp_16; 
    /*  coverity 87083 fix end */

	U8 event_code[256] = {'\0'};
	U8 event_code_inform[256] = {'\0'};

	memcpy ( &l_tmp_8, p_buffer + l_offset , 1);
    /*  coverity 87083 fix code removed */
	memcpy ( &l_tmp_8, p_buffer + l_offset +1 , 1);
    /* coverity 86623 fixed code removed */


	l_offset += sizeof(Uint16);
	l_offset += 6;  //Resreved 

	l_tmp_8 = 0;

	memcpy (&l_tmp_8, p_buffer + l_offset , 1);

	l_offset += sizeof(U8);
	l_offset += 3;

	if (l_tmp_8 > 7)
	{
		ENB_STRNCPY((char *)event_code,(const char *) p_buffer + l_offset , l_tmp_8);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , event code is : %s ", __FUNCTION__ ,event_code);
	}
	l_offset += l_tmp_8;
	l_offset += 4; //reserved 
	l_offset += 2;

	memcpy (&l_tot_par_8, p_buffer + l_offset , sizeof(U8));

	TRACE(LOG_CRITICAL , GLUE_LAYER, " Total Parameter ; %d, ", l_tot_par_8);

	l_offset += sizeof(U8);
	l_offset += 5;  


	/* SPR 19420 FIXED START */
	if (ENB_STRCMP((char *)event_code, "8 DIAGNOSTICS COMPLETE") == 0)
	{

		TRACE(LOG_CRITICAL , GLUE_LAYER, "8 DIAGNOSTICS COMPLETE event code received\n");
		tr_strncpy ((char *)event_code_inform , "8 DIAGNOSTICS COMPLETE");
		/*Coverity 111661 Fix Start*/
		initiate_TR069_Inform_session ((char*)event_code_inform , 0);
		/*Coverity 111661 Fix End*/

	}
	else
	{
	//skip reading total length

	while (l_count < l_tot_par_8)
	{
		parent_len = 0;
		parent_name = NULL;

		l_offset += 2 ;//skip size of all para,m    
		memcpy (&l_num_param , p_buffer + l_offset , sizeof(U8));

		TRACE(LOG_CRITICAL , GLUE_LAYER, "Number of parameter :%d", l_num_param); 

		l_offset += sizeof(U8);

		l_offset += 3;     //Reserved 

		l_tmp_8  =  0;
		l_tmp_16 = 0;

		memcpy(&l_tmp_8, p_buffer + l_offset , 1);
		parent_len = l_tmp_8 & 0x00FF;
		parent_len = parent_len << 8;
		memcpy ( &l_tmp_8, p_buffer + l_offset + 1 , 1);
		parent_len = ((parent_len) | (l_tmp_8 & 0x00FF));      

        /* SPR 17625 FIXED START */
        //memcpy(parent_name, ( p_buffer + l_offset + sizeof(Uint16)), parent_len);
        parent_name = p_buffer + l_offset + sizeof(Uint16);
        parent_name[parent_len] = '\0';
        TRACE (LOG_CRITICAL, GLUE_LAYER, " Parent Length = %d name: %s",parent_len,parent_name);
        /* SPR 17625 FIXED END */

        /* Coverity 97604 + */ 
        if( (Isexpedited) && ((parent_len >= FAULT_MGMT_QUEUED_EVT_LEN
                    && ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_QUEUED_EVT , FAULT_MGMT_QUEUED_EVT_LEN) == 0) ||
                (ENB_STRSTR((const char *)parent_name , "PassiveNotifications"))) )
        {
            TRACE (LOG_CRITICAL,GLUE_LAYER,"Queued/Passive Notification received after expedited, Isexpedited %d, Parent Name = %s,  Parent Len = %d",
                    Isexpedited,parent_name,parent_len);
            return;
        }    
        else  if( (IsqueuedEvent) &&  (((parent_len >= FAULT_MGMT_EXPEDITED_STR_LEN
                        && ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_EXPEDITED_STR, FAULT_MGMT_EXPEDITED_STR_LEN) == 0)) ||
                (ENB_STRSTR((const char *)parent_name , "ActiveNotifications"))) )
        {
            TRACE (LOG_CRITICAL,GLUE_LAYER,"Expedited/Active Notification received after queued, IsqueuedEvent %d, Parent Name = %s,  Parent Len = %d",
                    IsqueuedEvent,parent_name,parent_len);
            return;
        }    

		IsqueuedEvent = 0;
		Isexpedited = 0;
        /* Coverity 97604 - */ 

		l_offset += sizeof(Uint16);
		l_offset += parent_len;
		l_tot_params += l_num_param;

		arr_offset = 0;

	//	if (parent_len >= FAULT_MGMT_QUEUED_EVT_LEN
	//			&& ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_QUEUED_EVT , FAULT_MGMT_QUEUED_EVT_LEN) == 0)
        /* SPR 17625 FIXED START */
        if ((parent_len >= FAULT_MGMT_QUEUED_EVT_LEN
                    && ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_QUEUED_EVT , FAULT_MGMT_QUEUED_EVT_LEN) == 0) ||
                (ENB_STRSTR((const char *)parent_name , "PassiveNotifications")))
        /* SPR 17625 FIXED END */

		{
			IsqueuedEvent = 1;
			TR069_USLEEP(1000);            
			if (g_queued_paramList != NULL && g_queued_paramList_size > 0)
			{
				cpeParamList_t = (ParameterValueList *)g_queued_paramList;
				cpeParamArr = cpeParamList_t->__ptrParameterValueStruct;
				cpeParamArr  = (cwmp__ParameterValueStruct **)TR_REALLOC(cpeParamArr ,
						sizeof(cwmp__ParameterValueStruct *) * (g_queued_paramList_size + l_num_param));
				if(cpeParamArr == NULL)
				{
					TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s, TR_REALLOC failure ", __FUNCTION__);
					exit(1);
				}
				cpeParamList_t->__ptrParameterValueStruct = cpeParamArr;
			}
			else
			{
				/*SPR 17419 Fixed Start*/ 
				//cpeParamList_t = (sParameterValueList  *) ALLOCATE (1 , sizeof(sParameterValueList) );
				/*SPR 17419 Fixed End*/ 
				/* SPR 18746 FIX START */
				cpeParamList_t = ALLOCATE(sizeof(ParameterValueList) );
				/* SPR 18746 FIX END */
				if(NULL == cpeParamList_t) //coverity fix
				{
					TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
					exit(0);
				}
		

                /*SPR 17419 Fixed Start*/ 
				cpeParamArr = (ParameterValueStruct **) ALLOCATE( ((sizeof(ParameterValueStruct *) * (l_num_param))));
                /*SPR 17419 Fixed End*/ 
				//cpeParamArr =  ALLOCATE((sizeof(ParameterValueStruct *) * l_num_param));
				if(NULL == cpeParamArr) //coverity fix
				{
					TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
					exit(0);
				}

				cpeParamList_t->__ptrParameterValueStruct = cpeParamArr;
				g_queued_paramList = (struct ParameterValueList *)cpeParamList_t;
				g_queued_paramList_size = 0;
			}
		}

		//else  if ((parent_len >= FAULT_MGMT_EXPEDITED_STR_LEN
		//			&& ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_EXPEDITED_STR, FAULT_MGMT_EXPEDITED_STR_LEN) == 0))
        /* SPR 17625 FIXED START*/
        else  if (((parent_len >= FAULT_MGMT_EXPEDITED_STR_LEN
                        && ENB_STRNCMP((const char *)parent_name , FAULT_MGMT_EXPEDITED_STR, FAULT_MGMT_EXPEDITED_STR_LEN) == 0)) ||
                (ENB_STRSTR((const char *)parent_name , "ActiveNotifications")))
            /* SPR 17625 FIXED END */

		{
			Isexpedited = 1;

            /*SPR 17419 Fixed Start*/ 
            /* SPR_19838: Coverity_12932 Fix Start */
			/* code removed */
			cpeParamList_t = (ParameterValueList * ) ALLOCATE(sizeof(ParameterValueList));
            /* SPR_19838: Coverity_12932 Fix Stop */
            /*SPR 17419 Fixed End*/ 
			//cpeParamList_t = ALLOCATE(sizeof(sParameterValueList) );
			if(NULL == cpeParamList_t) //coverity fix
			{
				TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
				exit(0);
			}
            /*SPR 17419 Fixed Start*/ 
            cpeParamArr = (cwmp__ParameterValueStruct **) ALLOCATE (((sizeof(cwmp__ParameterValueStruct *) * (l_num_param))));
            /*SPR 17419 Fixed End*/ 
			if(NULL == cpeParamArr) //coverity fix
			{
				TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
				exit(0);
			}
		}


		l_inner_count = 0;

		while (l_inner_count < l_num_param)
		{
			l_tmp_8  =  0;
			l_tmp_16 = 0;
			memcpy(&l_tmp_8, p_buffer + l_offset , 1);
			l_tmp_16 = l_tmp_8 & 0x00FF;
			l_tmp_16 = l_tmp_16 << 8;
			memcpy (&l_tmp_8, p_buffer + l_offset + 1 , 1);
			l_tmp_16 = ((l_tmp_16) | (l_tmp_8 & 0x00FF));

			p_Name_Val = p_buffer + l_offset + 4;

			if (((l_tmp_16 > 4)  &&  (parent_len >= NOTIF_PARENT_FAPCONTROL_LEN) 
						&& (ENB_STRNCMP((const char *)parent_name , NOTIF_PARENT_FAPCONTROL ,NOTIF_PARENT_FAPCONTROL_LEN)) == 0)
					||((l_tmp_16 > 4) &&  (parent_len >= LTE_SCAN_TIME_LEN) 
						&& (ENB_STRNCMP((const char *)parent_name ,  LTE_SCANTIME_PARENT , LTE_SCAN_TIME_LEN))== 0)
					||((l_tmp_16 > 4) &&  (parent_len >= LTE_RAN_PARENT_LEN) 
						&& (ENB_STRNCMP((const char *)parent_name ,  LTE_RAN_PARENT, LTE_RAN_PARENT_LEN)== 0) && (forcedArr)))
			{

				//IsFapControl = 1;
				p_EQ = (U8 *)strchr ((const char *)p_Name_Val , '=');

				if (ENB_STRNCMP((const char *)p_Name_Val , OPSTATE , OPSTATE_LEN) == 0)
				{
					if (ENB_STRCMP((const char *)(p_EQ + 1) , forcedArr[OPSTATE_INDX]->Value) != 0)
					{

						// Value changed
                        /* SPR 13658 START */
                        /* SPR 21413 FIXED START */
                        if(atoi((const char *) p_EQ + 1) || (strcmp("true", (const char *)(p_EQ + 1))==0))
                        {
                            *((int *)forcedArr[OPSTATE_INDX]->Value) = 1;
                        }
                        else
                        { 
                            *((int *)forcedArr[OPSTATE_INDX]->Value) = 0;
                        }   
                        /* SPR 21413 FIXED END */
						//ENB_STRNCPY ((char *)forcedArr[OPSTATE_INDX]->Value ,(const char *) p_EQ + 1 , 512);
                        /* SPR 13658 END */
						//IsValChanged = 1;
					}
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , RFXSTATUS , RFXSTATUS_LEN) == 0)
				{
					if (ENB_STRCMP((const char *)(p_EQ + 1 ), forcedArr[RFTXSTATUS_INDX]->Value) != 0)
					{

						// Value changed
                        /* SPR 13658 START */
                        /* SPR 21413 FIXED START */
                        if(atoi((const char *) p_EQ + 1) || (strcmp("true", (const char *) p_EQ + 1) == 0))
                        {
                            *((int *)forcedArr[RFTXSTATUS_INDX]->Value) = 1;
                        }
                        else
                            *((int *)forcedArr[RFTXSTATUS_INDX]->Value) = 0;
                        /* SPR 21413 FIXED END */
						//ENB_STRNCPY ((char *)forcedArr[RFTXSTATUS_INDX]->Value , (const char *)p_EQ + 1 , 512);
                        /* SPR 13658 END */
					//	IsValChanged = 1;
					}
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , LASTSCANTIME_STR , LASTSCANTIME_STR_LEN) == 0)
				{
					if (ENB_STRCMP((const char *)(p_EQ + 1 ), forcedArr[LASTSCANTIME_INDX]->Value) != 0)
					{

						// Value changed
						ENB_STRNCPY ((char *)forcedArr[LASTSCANTIME_INDX]->Value ,(const char *) p_EQ + 1 , 512);
					//	IsValChanged = 1;
					}
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , NRT_UPDATETIME_STR , NRT_UPDATETIME_STR_LEN) == 0)
				{
					if (ENB_STRCMP((const char *)(p_EQ + 1 ), forcedArr[UPDATETIME_INDX]->Value) != 0)
					{

						// Value changed
						ENB_STRNCPY ((char *)forcedArr[UPDATETIME_INDX]->Value , (const char *)p_EQ + 1 , 512);
						//IsValChanged = 1;
					}
				}

			}
			/* SPR 17625 FIXED START */  
			if (Isexpedited)
			{
                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory Isexpedited is TRUE",__FUNCTION__);
				p_cpeParamVal_t = (cwmp__ParameterValueStruct *) ALLOCATE (sizeof(cwmp__ParameterValueStruct));
				if(NULL == p_cpeParamVal_t) //coverity fix
				{
					TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
					exit(0);
				}
				p_EQ = (U8 *)strchr ((const char *)p_Name_Val , '=');

                if(p_EQ == NULL)
                {
                    TRACE (LOG_CRITICAL, GLUE_LAYER, "SETU: p_EQ IS NULL");
                }

				if (NULL != p_cpeParamVal_t)
				{
					p_cpeParamVal_t->Name = (char *) ALLOCATE( SMALL_BUFFER_LEN);
					if(NULL == p_cpeParamVal_t->Name) //coverity fix
					{
						TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
						exit(0);
					}
					p_cpeParamVal_t->Value = (char *) ALLOCATE( MEDIUM_BUFFER_LEN);
					if(NULL == p_cpeParamVal_t->Value) //coverity fix
					{
						TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
						exit(0);
					}
                    /*  coverity 96964 fix start */
					memset(p_cpeParamVal_t->Value,0,sizeof(MEDIUM_BUFFER_LEN));
                    /*  coverity 96964 fix end */
					p_cpeParamVal_t->Name[0]='\0';
                    /* SPR 13658 START */
                    *((char *)p_cpeParamVal_t->Value)='\0';
                    /* SPR 13658 END */

					/*SPR 20007:CID 57532 Fix Start*/
					/*Code Deleted*/
					/*SPR 20007:CID 57532 Fix End*/
						if (p_EQ)
						{
                            if((ENB_STRSTR((const char *)parent_name , "ActiveNotifications")))
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s:: In Case Active Notification",__FUNCTION__);
                                tr_strncpy((char *)p_cpeParamVal_t->Value , (const char *)p_EQ + 1);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Value: %s",__FUNCTION__,(char *)p_cpeParamVal_t->Value);
                                strncpy(p_cpeParamVal_t->Name ,(const char *)p_Name_Val ,  p_EQ - p_Name_Val);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Edited TRName : %s",__FUNCTION__,p_cpeParamVal_t->Name);
                                remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
                            }
                            else
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s:: In Case EXPEDITED",__FUNCTION__);
                                tr_strncpy((char *)p_cpeParamVal_t->Value , (const char *)p_EQ + 1);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Value: %s",__FUNCTION__,(char *)p_cpeParamVal_t->Value);
                                tr_strncpy((char *)p_cpeParamVal_t->Name ,(const char *)parent_name);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, TRName : %s",__FUNCTION__,p_cpeParamVal_t->Name);
                                tr_strcat(p_cpeParamVal_t->Name , ".");
                                strncat(p_cpeParamVal_t->Name ,(const char *)p_Name_Val ,  p_EQ - p_Name_Val);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Edited TRName : %s",__FUNCTION__,p_cpeParamVal_t->Name);
                                remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
                            }
						}
						cpeParamArr[arr_offset] = p_cpeParamVal_t;
						cpeParamArr[arr_offset]->__type = PAR_TYPE_STRING; 
						arr_offset++;
					/*SPR 20007:CID 57532 Fix Start*/
					/*Code Deleted*/
					/*SPR 20007:CID 57532 Fix End*/
				}
			}
			/* SPR 17625 FIXED END */
            /* SPR 17625 FIXED START */
            if (Isfaplte && !(IsForcedInform))
            {
                p_cpeParamVal_t = (cwmp__ParameterValueStruct *) ALLOCATE (sizeof(cwmp__ParameterValueStruct));
                if(NULL == p_cpeParamVal_t) //coverity fix
                {
                    TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                    exit(0);
                }
                p_EQ = (U8 *)strchr ((const char *)p_Name_Val , '=');

                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s:: IN FAP LTE TRUE",__FUNCTION__);

                if (NULL != p_cpeParamVal_t)
                {
					p_cpeParamVal_t->Name = (char *) ALLOCATE( SMALL_BUFFER_LEN);
                    if(NULL == p_cpeParamVal_t->Name) //coverity fix
                    {
                        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                        exit(0);
                    }
					p_cpeParamVal_t->Value = (char *) ALLOCATE( MEDIUM_BUFFER_LEN);
                    if(NULL == p_cpeParamVal_t->Value) //coverity fix
                    {
                        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
                        exit(0);
                    }
                    p_cpeParamVal_t->Name[0]='\0';
                    *((char *)p_cpeParamVal_t->Value)='\0'; 
               /*SPR 20007:CID 57532 Fix START */
               /*Code Deleted*/
               /*SPR 20007:CID 57532 Fix START */
                        if (p_EQ)
                        {
                            /* SPR 13658 START */
                            p_cpeParamVal_t->__type = SOAP_TYPE_string;
                            /* SPR 13658 END */
                            tr_strncpy((char *)p_cpeParamVal_t->Value , (const char *)p_EQ + 1);
                            tr_strncpy((char *)p_cpeParamVal_t->Name ,(const char *)parent_name);
                            tr_strcat(p_cpeParamVal_t->Name , ".");
                            strncat(p_cpeParamVal_t->Name ,(const char *)p_Name_Val ,  p_EQ - p_Name_Val);
                            remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
                            TRACE (LOG_CRITICAL, GLUE_LAYER, " In %s :: p_cpeParamVal_t->Name",__FUNCTION__);
                        }
                        cpeParamArr[arr_offset] = p_cpeParamVal_t;
 			/*SPR 17836 FIXED START */
 		
                        cpeParamArr[arr_offset]->__type = PAR_TYPE_STRING;
 			/*SPR 17836 FIXED END */
                        arr_offset++;
               /*SPR 20007:CID 57532 Fix START */
               /*Code Deleted*/
               /*SPR 20007:CID 57532 Fix End */
                }
            }
			/* SPR 17625 FIXED END */

            //if (l_tmp_16 > 4 && IsqueuedEvent)
            /* SPR 17625 FIXED START*/
            if (IsqueuedEvent)
            /* SPR 17625 FIXED END */
 
			{
				p_cpeParamVal_t = (cwmp__ParameterValueStruct *) ALLOCATE (sizeof(cwmp__ParameterValueStruct));
				if(NULL == p_cpeParamVal_t) //coverity fix
				{
					TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
					exit(0);
				}
				p_EQ = (U8 *)strchr ((const char *)p_Name_Val , '=');

				if (NULL != p_cpeParamVal_t)
				{
					p_cpeParamVal_t->Name = (char *) ALLOCATE( SMALL_BUFFER_LEN);
					if(NULL == p_cpeParamVal_t->Name) //coverity fix
					{
						TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
						exit(0);
					}
					p_cpeParamVal_t->Value = (char *) ALLOCATE( MEDIUM_BUFFER_LEN);
					if(NULL == p_cpeParamVal_t->Value) //coverity fix
					{
						TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
						exit(0);
					}

					p_cpeParamVal_t->Name[0]='\0';
                    /* SPR 13658 START */
                    *((char *)p_cpeParamVal_t->Value)='\0';
                    /* SPR 13658 END */
					/* SPR 17625 FIXED START */
                                        /*SPR 20007:CID 57532 Fix START */
                                       /*Code Deleted*/ 
                                        /*SPR 20007:CID 57532 Fix End */
						if (p_EQ)
						{
                        /* SPR 13658 START */
                            p_cpeParamVal_t->__type = SOAP_TYPE_string;
                        /* SPR 13658 END */
                            if((ENB_STRSTR((const char *)parent_name , "PassiveNotifications")))
                            {
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s:: In Case Passive Notifications",__FUNCTION__);
                                tr_strncpy((char *)p_cpeParamVal_t->Value , (const char *)p_EQ + 1);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Value: %s",__FUNCTION__,(char *)p_cpeParamVal_t->Value);
                                strncpy(p_cpeParamVal_t->Name ,(const char *)p_Name_Val ,  p_EQ - p_Name_Val);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Edited TRName : %s",__FUNCTION__,p_cpeParamVal_t->Name);
                                remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
                            }
                            else
                            {

							    tr_strncpy((char *)p_cpeParamVal_t->Value ,(const char *) p_EQ + 1);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Value: %s",__FUNCTION__,(char *)p_cpeParamVal_t->Value);
							    tr_strncpy((char *)p_cpeParamVal_t->Name ,(const char *)parent_name);
							    tr_strcat(p_cpeParamVal_t->Name , ".");
							    strncat(p_cpeParamVal_t->Name ,(const char *)p_Name_Val ,  p_EQ - p_Name_Val);
							    remove_brackets((UInt8 *)p_cpeParamVal_t->Name);
                                TRACE (LOG_CRITICAL, GLUE_LAYER, " %s, Edited TRName : %s",__FUNCTION__,p_cpeParamVal_t->Name);
                            }
						}

						/* SPR 21294 FIXED START */
						ret_val = checkDuplicateNotificationParams(cpeParamArr, p_cpeParamVal_t, g_queued_paramList_size);
						if (!ret_val)
						{
							cpeParamArr[g_queued_paramList_size] = p_cpeParamVal_t;
							cpeParamArr[arr_offset]->__type = PAR_TYPE_STRING; 
							g_queued_paramList_size++;
						}
						else
						{
							TRACE (LOG_DETAIL, GLUE_LAYER, "Duplicate notification received");
						}
						/* SPR 21294 FIXED END */
						 /*SPR 20007:CID 57532 Fix START */
                                       /*Code Deleted*/ 
                                        /*SPR 20007:CID 57532 Fix End */
					/* SPR 17625 FIXED END*/

				}
			}
			l_inner_count++;                
			l_offset += l_tmp_16;               
		}// Inner while
                /*SPR #13788 Fix Start*/
/* Coverity 40695_40696 + */
	/* Code Removed */
		if ((arr_offset > 0) && Isexpedited)
/* Coverity 40695_40696 - */
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, " Total Parent : %d,  count : %d", l_tot_par_8 , l_count);
			cpeParamList_t->__ptrParameterValueStruct = cpeParamArr;
			cpeParamList_t->__size = arr_offset;
			TRACE(LOG_CRITICAL , GLUE_LAYER, " First parent : Total size : %d", cpeParamList_t->__size);
			if (strstr((char *)event_code, "Value Change") != NULL)
			{
				//memset (event_code , 0 , 256);
				tr_strncpy ((char *)event_code_inform , "4 VALUE CHANGE");
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Calling for initiate Inform Session For Expedited notif");
				if (Isexpedited)
				{
					g_exped_wait_inform = 1;
					g_expedited_inform = 1;
				}
				initiate_TR069_Inform_session ((char *)event_code_inform , cpeParamList_t);
			}
        /* Coverity 40695_40696 + */
            else
            {
                if(cpeParamList_t) {
                DEALLOCATE(cpeParamList_t);
                }
                if(cpeParamArr) {
                    DEALLOCATE(cpeParamArr); }
            }    
		}
        else if((0 == arr_offset) && Isexpedited)
        {
           if(cpeParamList_t) {
           DEALLOCATE(cpeParamList_t); }
           if(cpeParamArr) {
           DEALLOCATE(cpeParamArr); }
        /* Coverity 40695_40696 - */
        }    

		if (IsqueuedEvent)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Queued Events ");
			cpeParamList_t->__size = g_queued_paramList_size;     
			g_queued_wait_inform = 1;
		}
		l_count++;
	}
    }
 /* SPR 19420 FIXED END */
} 
#ifdef HAVE_IPSEC
/******************************************************************************
 * * *  *   FUNCTION NAME  : send_tunnel_error_oam
 * * *  *   Inputs         : void 
 * * *  *   Outputs        : none
 * * *  *   RETURNS        : none
 * * *  *   DESCRIPTION    : This function sends message to oam 
 * * * *******************************************************************************/
void send_tunnel_error_oam(void)
{
	UInt32 len = 0;
	UInt32 fd = 0; 
	void* p_buffer1 = PNULL;
	p_buffer1 = ALLOCATE( SET_PARAM_PAYLOAD_SIZE);
	if (p_buffer1 == PNULL)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);system("sleep 10");
		return;
	}
	//Uint16 trans_id = 0xFFFF; 
	g_trans_id++;
	if(g_trans_id >TRANS_RANGE)
		g_trans_id = 1;
	len = buffer_OAM_Config_Resp  ('1', &p_buffer1, ERR_STR , htons(ERROR_CODE_CONFIG));
	fd = get_fd();//anant
	construct_interface_api_header( p_buffer1, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, TR069_OAM_CONFIG_RESP, len);
	send_msg_oam (p_buffer1 , fd,  len);
	DEALLOCATE(p_buffer1);	
	p_buffer1= NULL;
}
#endif

/* Coverty 114333 Fixed Start*/
/******************************************************************************
  FUNCTION NAME  : handle_add_object 
Inputs         : Void * :  Buffer.
Outputs        : .
RETURNS        : Void * : AddObject reponse.
DESCRIPTION    : This function  handles Add object RPC' request recieved from ACS
as it first send Add Object request to OAM and receives response from
Oam is passed to ACS.
 *******************************************************************************/
void * handle_add_object (void * msg)
{
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering  %s",__FUNCTION__);
    AddObject *arg_obj_t = NULL;
    void * p_buffer_for_OAM = NULL;
    UInt16 l_tot_len = 0;
    void * p_res = NULL;
    UInt16  l_offset = 0;
    UInt8  msb = 0x00,
           lsb = 0x00;
    UInt8 ret = 0;

    arg_obj_t = (AddObject *)msg;
    UInt8 ObjectName[300] = {0};
    UInt32 fd = 0 ;

    fd = get_fd();

    p_buffer_for_OAM = ALLOCATE( sizeof(char *) *  ADD_OBJECT_BUFFER_LEN);

    if(NULL == p_buffer_for_OAM)
    {
        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
        return NULL;
    }


    /* API HEADER*/
    l_offset += 12; 

    /* RESERVED */
    l_offset += 4;

    if (arg_obj_t->ObjectName)
    {
        ENB_STRNCPY((char *)ObjectName , (const char *)arg_obj_t->ObjectName, 256);
        add_brackets(ObjectName);
        l_tot_len = strlen((const char *) ObjectName) + 1;
    }

    l_tot_len += 2; /* 2 for reserved */

    msb = (l_tot_len + sizeof(UInt16))/256;
    lsb = (l_tot_len + sizeof(UInt16))%256; 

    memcpy(p_buffer_for_OAM + l_offset,&msb,sizeof(UInt8));
    memcpy(p_buffer_for_OAM + l_offset+1,&lsb,sizeof(UInt8));
    l_offset += 2; /* Length of Object Info */

    l_offset += 2; /* 2 for reserved */

    /* Object Name */
    memcpy(p_buffer_for_OAM + l_offset,ObjectName, l_tot_len - 2);    


    l_offset += l_tot_len - 2; 

    g_trans_id++;
    if(g_trans_id >TRANS_RANGE)
        g_trans_id = 1;

    construct_interface_api_header(p_buffer_for_OAM, g_trans_id, TR069_MODULE_ID,
            OAM_MGMT_INTF_MODULE_ID, OAM_ADD_OBJECT_REQ, l_offset);
    /* coverity 86510 fix start */
    /*code remove */
    /* coverity 86510 fix end */

    ret = send_msg_oam(p_buffer_for_OAM , fd , l_offset);
    DEALLOCATE(p_buffer_for_OAM);	

	//send_msg_oam(p_buffer_for_OAM , fd , l_tot_len);
	/*Review comments incorporated*/    
	if (ret == GLUE_SUCCESS)
	{
		p_res = recv_msg_oam(NULL, RPC_ADDOBJECT); 
	}
    /* SPR 21588 FIXED START  */
	TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
    /* CID: 114338 Start */
      while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
    /* SPR 114338 End */
/* SPR 21588 FIXED END */

	{
		decode_notification(g_notif_buffer);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
		qvFree(g_notif_buffer);
		g_notif_buffer = NULL;
	}

    /* Coverity 114333 Fixed End */
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting  %s",__FUNCTION__);
    return p_res;

}

/* Coverty 114333 Fixed End*/
/******************************************************************************
  FUNCTION NAME  : decode_oam_cntrl_cmd 
Inputs         : Void * :  Buffer. 
Outputs        : .
RETURNS        : Void *  
DESCRIPTION    : This function parse buffer received from Oam and perform Upload
for PM Xml files.
 *******************************************************************************/
void * decode_oam_cntrl_cmd (void * p_arg_buffer)
{
	Uint16 l_offset = 0;
	Uint16 l_admin_len = 0 ; 
	U8 *p_Name_Val = NULL;
	U8 *p_Eq = NULL;
	SInt32  l_ret_stat = 0;
	U8 num_of_param = 0;
	U8 l_count = 0;
	Uint16 l_param_len = 0;
	Upload_Dir *upl_dir_t = {0};

	l_offset += 12; /* Skip header parsing */
	U8 l_tmp_8 = 0;
	memcpy(&l_tmp_8, p_arg_buffer + l_offset , 1);
	l_admin_len = l_tmp_8 & 0x00FF;
	l_admin_len = l_admin_len << 8;
	memcpy(&l_tmp_8, p_arg_buffer + l_offset +1 , 1);
	l_admin_len = ((l_admin_len) | (l_tmp_8 & 0x00FF));


	if (l_admin_len > 0) 
	{
		p_Name_Val = p_arg_buffer + l_offset + 4;
		p_Eq = (U8 *)strchr((const char *)p_Name_Val , '=');
		if (p_Eq != NULL)
		{
			l_ret_stat  = ENB_STRNCMP((const char *)(p_Eq +1) , "FileUpload", 10);
		}
		//coverity fix
		if (l_ret_stat)
			return NULL;

		memcpy(&num_of_param , p_arg_buffer + l_offset + (l_admin_len - 8) , sizeof(U8));

		l_offset += l_admin_len;

		upl_dir_t = ALLOCATE (sizeof(Upload_Dir) );
		if (!upl_dir_t)
			return NULL;

		memset(upl_dir_t , '\0', sizeof(Upload_Dir) );

		while (l_count < num_of_param)
		{
			l_tmp_8 = 0;
			l_param_len = 0;
			memcpy(&l_tmp_8, p_arg_buffer + l_offset , 1);
			l_param_len = l_tmp_8 & 0x00FF;
			l_param_len = l_param_len << 8;
			memcpy(&l_tmp_8, p_arg_buffer + l_offset +1 , 1);
			l_param_len = ((l_param_len) | (l_tmp_8 & 0x00FF));

			p_Name_Val = p_arg_buffer + l_offset + 4;
			p_Eq = (U8 *)strchr((const char *)p_Name_Val , '=');
			if (p_Eq != NULL)
			{
				if (ENB_STRNCMP((const char *)p_Name_Val , "URL", 3) == 0)
				{
					upl_dir_t->url = ALLOCATE( UPLOAD_STR );
					if (upl_dir_t->url)
						ENB_STRNCPY((char *)upl_dir_t->url,(const char *) p_Eq + 1 ,UPLOAD_STR);      
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , "Username", 8) == 0)
				{
					upl_dir_t->user = ALLOCATE(UPLOAD_STR );
					if (upl_dir_t->user)
						ENB_STRNCPY((char *)upl_dir_t->user,(const char *) p_Eq + 1 ,UPLOAD_STR);     
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , "Password", 8) == 0)
				{
					upl_dir_t->pwd = ALLOCATE( UPLOAD_STR );
					if (upl_dir_t->pwd)
						ENB_STRNCPY((char *)upl_dir_t->pwd,(const char *) p_Eq + 1 ,UPLOAD_STR);      
				}
				if (ENB_STRNCMP((const char *)p_Name_Val , "DirectoryPath", 13) == 0)
				{
					upl_dir_t->dir_path = ALLOCATE(UPLOAD_STR );
					if (upl_dir_t->dir_path)
						ENB_STRNCPY((char *)upl_dir_t->dir_path,(const char *) p_Eq + 1 ,UPLOAD_STR);     
				}               
			}

			l_offset += l_param_len;
			l_count += 1;
		}

	}

	return upl_dir_t;

}



/******************************************************************************
  FUNCTION NAME  : upload_all_in_Dir 
Inputs         : Upload_Dir * :  pointer to Upload Directory struct (Dir Path) 
Outputs        : .
RETURNS        : UInt8 : Status. (0 as Success)
DESCRIPTION    : This function will upload all files present in dir path.
 *******************************************************************************/
UInt8 upload_all_in_Dir(Upload_Dir *upl_dir_t)
{
	struct dirent *entry;
	DIR *dp;
	sUpload *l_upload = NULL;
	Int res = 0; 

	char file_path[SMALL_BUFFER_LEN] = {0};
	char file_path_new[SMALL_BUFFER_LEN] = {0};

	if(upl_dir_t->dir_path)
	{
	dp = opendir( (const char *)upl_dir_t->dir_path);
	if (dp == NULL)
	{
		perror("opendir: Path does not exist or could not be read Dir provided in Upload");
		return 1;
	}
	}else
	{
		perror("opendir: Path does not exist or could not be read Dir provided in Upload");
		return 1;
	}
	while ((entry = readdir(dp)))
	{
		if (ENB_STRNCMP((const char *)(entry->d_name) , ".", 1) == 0  ||  ENB_STRNCMP((const char *)entry->d_name , "..", 2) == 0
				|| strstr((char *)entry->d_name , "_done") != NULL) 
		{
			continue;
		}
		else
		{
			l_upload = ALLOCATE( sizeof(sUpload)); 
			if (!l_upload)
				continue;

			l_upload->Username = (Char8 *) upl_dir_t->user;
			l_upload->Password = (Char8 *)upl_dir_t->pwd;
			l_upload->URL = (Char8 *)upl_dir_t->url;
			memset(file_path , 0, SMALL_BUFFER_LEN);

			tr_strncpy(file_path , (const char *)upl_dir_t->dir_path);

			tr_strcat(file_path , entry->d_name);

			TRACE (LOG_CRITICAL , GLUE_LAYER, "Uploading file ==> %s", file_path);

			res = connManagerUpload((Upload *)l_upload, file_path);
			TRACE (LOG_CRITICAL , GLUE_LAYER, "response is : %d", res);

			if (res == 0)
			{
				tr_strncpy((char *)file_path_new, file_path);
				tr_strcat(file_path_new , "_done");
				if(-1 ==  rename(file_path , file_path_new))
				{
					TRACE (LOG_CRITICAL , GLUE_LAYER, "ERROR: in rename function ");
				}

			}
			else
            {
                /* Coverity 113217 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir \n ");
                }
                /* Coverity 113217 fix end */

                DEALLOCATE(l_upload);    
                return res; 
            }
        }
	}
    /* Coverity 113217 fix start */
    if(TR069_ZERO != closedir(dp))
    {
        TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir \n ");
    }
    /* Coverity 113217 fix end */

    /* Coverity 113258 fix start */
    /* Code Remove */
    /* Coverity 113258 fix end */

    DEALLOCATE(l_upload);
    return res;
} 



/******************************************************************************
 * *   FUNCTION NAME  : handle_reboot_req
 * *   Inputs         : p_buffer, eType
 * *   Outputs        : none
 * *   RETURNS        : Void
 * *   DESCRIPTION    : This function handles  Reboot Request received from  tr069 client library.
 * *******************************************************************************/
void *handle_reboot_req(void)
{
	void *l_p_buffer = NULL;
	void *p_res = NULL;
	UInt32 fd = 0;
	UInt32 l_buffer_len = 0;
	UInt8 *contrl_cmd = "AdminControlCommand=Restart";
	/* COVERITY 108699/108704 FIX START */
	/* code removed */
	/* COVERITY 108699/108704 FIX END */

	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);
	fd = get_fd();

	l_p_buffer = buffer_OAM_Control_Cmd_Req(0 , NULL, &l_buffer_len, contrl_cmd);

	if (l_p_buffer)
	{
		g_trans_id++;
		if (g_trans_id > TRANS_RANGE)
			g_trans_id = 1;
		construct_interface_api_header(l_p_buffer , g_trans_id, TR069_MODULE_ID,

/* SPR 11921 Fix Start*/
				OAM_MGMT_INTF_MODULE_ID, OAM_CONTROL_COMMAND_REQ, l_buffer_len);
/* SPR 11921 Fix End*/
		send_msg_oam(l_p_buffer , fd, l_buffer_len);
		DEALLOCATE(l_p_buffer);	
        /* coverity 86570 fixed code removed */
	}
	p_res = recv_msg_oam (NULL, RPC_CONTROL_CMD);
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
	return p_res;
}
/******************************************************************************
 * *   FUNCTION NAME  : handle_factoryreset_req
 * *   Inputs         : p_buffer, eType
 * *   Outputs        : none
 * *   RETURNS        : Void
 * *   DESCRIPTION    : This function handles  Factory Reset Request received from  tr069 client library.
 * *******************************************************************************/
void *handle_factoryreset_req(void)
{
	void *l_p_buffer = NULL;
	void *p_res = NULL;
	UInt32 fd = 0;
	UInt32 l_buffer_len = 0;
	UInt8 *contrl_cmd = (UInt8 *)"AdminControlCommand=FactoryReset";


	/* COVERITY 108699/108704 FIX START */
	/* code removed */
	/* COVERITY 108699/108704 FIX END */

	TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);
	fd = get_fd();

	l_p_buffer = buffer_OAM_Control_Cmd_Req(1 , "PlatformrRestart=False", &l_buffer_len, contrl_cmd);

	if (l_p_buffer)
	{
		g_trans_id++;
		if (g_trans_id > TRANS_RANGE)
			g_trans_id = 1;
		construct_interface_api_header(l_p_buffer , g_trans_id, TR069_MODULE_ID,
/* SPR 11921 Fix Start*/
				OAM_MGMT_INTF_MODULE_ID, OAM_CONTROL_COMMAND_REQ, l_buffer_len);
/* SPR 11921 Fix End*/
		send_msg_oam(l_p_buffer , fd, l_buffer_len);
		DEALLOCATE(l_p_buffer);	
	/* coverity 86510 fix start */
   /*code remove */
   /* coverity 86510 fix end */
    }
	p_res = recv_msg_oam (NULL, RPC_CONTROL_CMD);
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
	return p_res;
}


/******************************************************************************
  FUNCTION NAME  : buffer_OAM_notif_Resp 
Inputs         : status :0 - Success , 1 as Failure
Outputs        : . 
RETURNS        : Uint16 : No. of bytes written. 
DESCRIPTION    : This function will create Response  buffer for Notification request 
 *******************************************************************************/
Uint16  buffer_OAM_notif_Resp (U8 status, void **p_buffer)
{
	U8 len = '\0';
	U8 msb = 0 ;
	U8 lsb = 0;
	char err_string[] = "Request denied";
	Uint16 error_code = 9001;
	Uint16 l_offset = 0;

	len  = sizeof(err_string);

	/* API header */

	l_offset += 12;

	if (status == '1')
	{

		*p_buffer = ALLOCATE((sizeof(char *) * (l_offset+8+len +1)));
		//coverity fix
		if ( NULL == *p_buffer)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			return 0;
		}
		/*Status */
		memset (*p_buffer + l_offset , 1, 1);
		l_offset += 1;

		/*  Error Code  */

		msb = error_code / 256;
		lsb = error_code % 256;
		memcpy(*p_buffer + l_offset , &msb , sizeof(U8));
		memcpy (*p_buffer + l_offset , &lsb , sizeof(U8));
		l_offset += 2;

		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 1);
		l_offset += 1;

		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 3);
		l_offset += 3;


		/* Length of Error String */

		memcpy (*p_buffer + l_offset , &len, 1);
		l_offset += 1;


		/* Error String */
		memcpy (*p_buffer + l_offset , err_string, len);
		l_offset += len + 1;

	}
	else
	{

		*p_buffer = ALLOCATE((sizeof(char *) * ( l_offset + 8)));
		if(NULL == *p_buffer) //coverity fix
		{
			TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
			exit(0);
		}
		/*Status */
		memset (*p_buffer + l_offset , 0, 1);
		l_offset += 1;

		/*  Error Code  */
		error_code = 0;
		memcpy (*p_buffer + l_offset , &error_code, sizeof(Uint16));
		l_offset += 2;


		/* Reserved  */
		memset (*p_buffer + l_offset , 0 , 4);
		l_offset += 4;


		/* Length of Error String */
		len = 0;

		memcpy (*p_buffer + l_offset , &len, 1);
		l_offset += 1;

	}
	return l_offset;
}



void * recv_msg_oam_Download(YLIST *yList)
{
	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering  %s",__FUNCTION__);
	UInt32 sockfd = 0;
	SInt32 byte_read = 0 ;
	struct timeval      timeval;
	fd_set set;
	UInt8 *buffer;

	buffer = (UInt8  *)ALLOCATE( MAX_RECV_BUFF);
	if (NULL == buffer)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		return NULL;
	}

	DownloadStatus *p_dwn_t = NULL;
	UInt16  api_idr = 0;
	transData_t g_transD_t;
	transData_t *p_retback_t = NULL;


	UInt16 recv_trans_id = 0;


	/* SPR 19083 FIXED START*/
	struct timeval start_t;
	struct timeval end_t;
	/* SPR 19083 FIXED END */


	struct sockaddr_in client;
	UInt32 len = sizeof(client);

	sockfd = get_fd();

	/* SPR 19083 FIXED START*/
	time_t ret_start = 0;
	time_t ret_end = 0;
	tr069_gettimeofday_wrapper(&start_t);
	ret_start = (time_t)start_t.tv_sec;

	tr069_gettimeofday_wrapper(&end_t);
	ret_end = (time_t)end_t.tv_sec;

	/* SPR 19083 FIXED END */ 


	p_dwn_t = (DownloadStatus *)ALLOCATE( sizeof(DownloadStatus));

	if (NULL == p_dwn_t)
	{
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
		//coverity fix
		DEALLOCATE(buffer);
		return NULL;
	}
	/* COVERITY 108705 FIX START */
	/*code 108705 removed */
	/* COVERITY 108705 FIX END */

	TRACE(LOG_CRITICAL , GLUE_LAYER, "Start Time : %ld", ret_start);

	TRACE(LOG_CRITICAL , GLUE_LAYER, "Start Time : %ld", start_t);

	FD_ZERO(&set);
	FD_SET(sockfd, &set);

	timeval.tv_sec = 0;
	timeval.tv_usec = 500000;

	while (!(ylEmpty(yList)))
	{
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeval,sizeof(timeval)) < 0) {
			perror("Error");
		}

		if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
		{
		TRACE(LOG_NONE , GLUE_LAYER, " bytes read  : %d",byte_read );

			/* SPR 19083 FIXED START*/
			tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;
			/* sg wait_limit change start */
			if (ret_end - ret_start > WAIT_LIMIT_DOWNLOAD)
				/* sg wait_limit change end */
			{

				TRACE(LOG_CRITICAL , GLUE_LAYER, "END time nOW : %ld", ret_end);
				/* SPR 19083 FIXED END */


				p_dwn_t->eStatus = FAULT;
				p_dwn_t->fault_code = INTERNAL_FAILURE;
				p_dwn_t->fault_string = (char *)ALLOCATE( 80);
				if (p_dwn_t->fault_string)
					ENB_STRNCPY(p_dwn_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
				DEALLOCATE(buffer);
				return p_dwn_t;
			}
			else
			{
				TR069_USLEEP(5000);
				continue;
			}
		}
		// Code added to support Notification in between RPC's
		api_idr = get_word_from_header((U8 *) (buffer) + 6);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved api_idr = %d", api_idr);


/*SPR 17627 FIXED START*/
		if (api_idr == TR069_OAM_HEARTBEAT_REQ)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
			/*send hearbeat response to sysctrl */
			if(1 != send_heartbeat_resp())
			{
				/*log message response not sent*/
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
			}
			else
			{
				/*log message response sent successfully */
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
			}

		}
/*SPR 17627 FIXED END*/

/*CSR 57877 Fix START */
		if (api_idr == OAM_NOTIFICATION_IND)
/*CSR 57877 Fix End*/
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
			/* SPR 21588 FIXED START  */
			/* Code removed */
			/* SPR 21588 FIXED END  */

			g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
			if (g_notif_buffer)
            {
				/*SPR 21673 FIXED START */
                memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
                ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
                /*SPR 21673 FIXED START */
            }
            /* SPR 21588 FIXED END */
			continue;
		}
		if (api_idr == OAM_FILE_DOWNLOAD_IND_RESP)
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved OAM_FILE_DOWNLOAD_IND_RESP");
			recv_trans_id = get_word_from_header(buffer);
			if(0 != recv_trans_id)
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved  transid");
				g_transD_t.trans_id = recv_trans_id;
			}
			else
			{
				TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
			}

			p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);
			if (p_retback_t != NULL)
			{
				/* COVERITY 108705 FIX START */
				/* code removed */
				/* COVERITY 108705 FIX END */
				REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
				decode_OAM_Download_Res(buffer, p_dwn_t);
				DEALLOCATE(buffer);
				return p_dwn_t;
			}
		}
		else
			/* SPR 19083 FIXED START*/
			tr069_gettimeofday_wrapper(&end_t);
		ret_end = (time_t)end_t.tv_sec;
        /* sg wait_limit change start */
		if (ret_end - ret_start > WAIT_LIMIT_DOWNLOAD)
        /* sg wait_limit change end */
		{
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", ret_end);
			/* SPR 19083 FIXED END */ 

			TRACE(LOG_CRITICAL , GLUE_LAYER, "WAIT LIMIT OVER... ");
			p_dwn_t->eStatus = FAULT;
			p_dwn_t->fault_code = INTERNAL_FAILURE;
			p_dwn_t->fault_string = (char *)ALLOCATE( 80);
			if (p_dwn_t->fault_string)
				ENB_STRNCPY(p_dwn_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
			DEALLOCATE(buffer);
			return p_dwn_t;
		}

}	

	DEALLOCATE(buffer);
	return  p_dwn_t;
}
/* SPR 13658 START */
parameter_type_mapping_table_t *g_parameter_map_element = NULL;

/****************************************************************************
 * Function Name  : ipr_cwmp_find_parameter_type 
 * Inputs         : param_name
 * Outputs        : type
 * Returns        : UInt32 value
 * Description    : This function returns the type value of the parameter 
 * *******************************************************************************/
int ipr_cwmp_find_parameter_type(char *param_name)
{
    UInt32 loop_index = 0;
    UInt32 sizeof_table = 0;

    if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE"))
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
		TRACE(LOG_CRITICAL , GLUE_LAYER, "MAP TABLE FOUND ");
    }
    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc")) 
    {
        g_parameter_map_element = &g_parameter_lte_l3_Assoc_list[0];
        sizeof_table = (sizeof(g_parameter_lte_l3_Assoc_list)/sizeof(g_parameter_lte_l3_Assoc_list[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP"))
    {
        g_parameter_map_element = &g_parameter_lte_l3_sctp_list[0];
        sizeof_table = (sizeof(g_parameter_lte_l3_sctp_list)/sizeof(g_parameter_lte_l3_sctp_list[0])); 
    }

    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.QoSNum"))
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.FaultMgmt.CurrentAlarm."))
    {
        g_parameter_map_element = g_parameter_multi_instanse_FM_CURR_GET_SET;
        sizeof_table = (sizeof(g_parameter_multi_instanse_FM_CURR_GET_SET)/sizeof(g_parameter_multi_instanse_FM_CURR_GET_SET[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.FaultMgmt.QueuedEvent"))
    {
        g_parameter_map_element = g_parameter_multi_instanse_FM_QUED_GET_SET;
        sizeof_table = (sizeof(g_parameter_multi_instanse_FM_QUED_GET_SET)/sizeof(g_parameter_multi_instanse_FM_QUED_GET_SET[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.FaultMgmt.ExpeditedEvent."))
    {
        g_parameter_map_element = g_parameter_multi_instanse_FM_EXPD_GET_SET;
        sizeof_table = (sizeof(g_parameter_multi_instanse_FM_EXPD_GET_SET)/sizeof(g_parameter_multi_instanse_FM_EXPD_GET_SET[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.FaultMgmt.HistoryEvent."))
    {
        g_parameter_map_element = g_parameter_multi_instanse_FM_HIST_GET_SET;
        sizeof_table = (sizeof(g_parameter_multi_instanse_FM_HIST_GET_SET)/sizeof(g_parameter_multi_instanse_FM_HIST_GET_SET[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.FaultMgmt.SupportedAlarm."))
    {
        g_parameter_map_element = g_parameter_multi_instanse_FM_SUPP_GET_SET;
        sizeof_table = (sizeof(g_parameter_multi_instanse_FM_SUPP_GET_SET)/sizeof(g_parameter_multi_instanse_FM_SUPP_GET_SET[0])); 
    }
    else if( strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.AntennaInfo"))
    {
        g_parameter_map_element = g_parameter_TR_element;
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.InServiceHandling"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }

    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CarrierMeasNumberOfEntries"))
    {
        g_parameter_map_element =  g_parameter_son_nw_scan_status;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_status)/sizeof(g_parameter_son_nw_scan_status[0])); 
    }

    else if(strstr((char *)param_name,"InternetGatewayDevice.Services.FAPService.[1].Capabilities"))
    {

        g_parameter_map_element = g_parameter_TR_element;
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }

    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanOnBoot"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanPeriodically"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.PeriodicInterval"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.PeriodicTime"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.EUTRACarrierARFCNDLList"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanTimeout"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.REMBandList"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name,"InternetGatewayDevice.Services.FAPService.[1].REM.LTE.REMPLMNList"))
    {
        g_parameter_map_element = g_parameter_son_nw_scan_req;
        sizeof_table = (sizeof(g_parameter_son_nw_scan_req)/sizeof(g_parameter_son_nw_scan_req[0])); 
    }
    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell"))
    {
        if(strstr(param_name, "RF")){
            g_parameter_map_element = &g_parameter_son_nw_scan_results_rf[0];
            sizeof_table = (sizeof(g_parameter_son_nw_scan_results_rf)/sizeof(g_parameter_son_nw_scan_results_rf[0])); 
        }
        else if(strstr(param_name, "BCCH")){
            g_parameter_map_element = &g_parameter_son_nw_scan_results_bcch[0];
            sizeof_table = (sizeof(g_parameter_son_nw_scan_results_bcch)/sizeof(g_parameter_son_nw_scan_results_bcch[0])); 
        }
    }
    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].BCCH.PLMNList"))
    {
        g_parameter_map_element = &g_parameter_son_nw_scan_results_bcch_plmnlist[0];
        sizeof_table = (sizeof(g_parameter_son_nw_scan_results_bcch_plmnlist)/sizeof(g_parameter_son_nw_scan_results_bcch_plmnlist[0])); 
    }

    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].REM.LTE"))
    {

        if(strstr((char *)param_name, "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CarrierMeas"))
        {
            g_parameter_map_element = &(g_parameter_son_rem_lte_carrierMeas[0]);
            sizeof_table = (sizeof(g_parameter_son_rem_lte_carrierMeas)/sizeof(g_parameter_son_rem_lte_carrierMeas[0])); 

        }
        else{
            g_parameter_map_element = &g_parameter_son_nw_scan_status[0]; 
            sizeof_table = (sizeof(g_parameter_son_nw_scan_status)/sizeof(g_parameter_son_nw_scan_status[0])); 
        }
    }
    else if(strstr(param_name, "LoadReporting")) 
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }

    else if(strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.QoS"))
    {
        g_parameter_map_element = &g_parameter_TR_QOS_element[0];
        sizeof_table = (sizeof(g_parameter_TR_QOS_element)/sizeof(g_parameter_TR_QOS_element[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNList"))
    {
        g_parameter_map_element = &g_parameter_TR_PLMNList_element[0];
        sizeof_table = (sizeof(g_parameter_TR_PLMNList_element)/sizeof(g_parameter_TR_PLMNList_element[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig"
                ".LTE.RAN.Mobility.IdleMode.InterFreq.Carrier.")) {
        g_parameter_map_element =
            g_parameter_TR_InterFreq_Carrier_element;
        sizeof_table = (sizeof(g_parameter_TR_InterFreq_Carrier_element)/sizeof(g_parameter_TR_InterFreq_Carrier_element[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig"
                ".LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.")){
        g_parameter_map_element =
            g_parameter_TR_UTRA_FDD_element;
        sizeof_table = (sizeof(g_parameter_TR_UTRA_FDD_element)/sizeof(g_parameter_TR_UTRA_FDD_element[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA."
                "IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS.")) {
        g_parameter_map_element = g_parameter_UTRA_TDD_element;
        sizeof_table = (sizeof(g_parameter_UTRA_TDD_element)/sizeof(g_parameter_UTRA_TDD_element[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA.")) {
        g_parameter_map_element =
            g_parameter_TR_UTRA_element;
        sizeof_table = (sizeof(g_parameter_TR_UTRA_element)/sizeof(g_parameter_TR_UTRA_element[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig"
                ".LTE.RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.")){
        g_parameter_map_element =
            g_parameter_son_anr_geranFDDFreq_params;
        sizeof_table = (sizeof(g_parameter_son_anr_geranFDDFreq_params)/sizeof(g_parameter_son_anr_geranFDDFreq_params[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.GERAN.")) {
        g_parameter_map_element =
            g_parameter_son_anr_geran_params;
        sizeof_table = (sizeof(g_parameter_son_anr_geran_params)/sizeof(g_parameter_son_anr_geran_params[0])); 
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.NeighborList.")) {
        if(strstr(param_name, "InterRATCell.UMTS.")) {
            g_parameter_map_element =
                g_parameter_lte_inter_rat_neighbor_list;
        sizeof_table = (sizeof(g_parameter_lte_inter_rat_neighbor_list)/sizeof(g_parameter_lte_inter_rat_neighbor_list[0])); 
        }
        else if(strstr(param_name, "LTECell.")) {
            g_parameter_map_element =
                g_parameter_lte_intra_rat_neighbor_list;
        sizeof_table = (sizeof(g_parameter_lte_intra_rat_neighbor_list)/sizeof(g_parameter_lte_intra_rat_neighbor_list[0])); 
        }
    }
    else if(strstr(param_name,
                "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.NeighborListInUse.")) 
    {

        if(strstr(param_name, "InterRATCell.UMTS.")) 
        {
            g_parameter_map_element =
                g_param_inter_rat_neigh_list_in_use;
        sizeof_table = (sizeof(g_param_inter_rat_neigh_list_in_use)/sizeof(g_param_inter_rat_neigh_list_in_use[0])); 
        }
        else if(strstr(param_name, "LTECell.")) 
        {
            g_parameter_map_element =
                g_param_intra_rat_neigh_list_in_use;
        sizeof_table = (sizeof(g_param_intra_rat_neigh_list_in_use)/sizeof(g_param_intra_rat_neigh_list_in_use[0])); 
        }
    }
    else if (strstr(param_name, "eNodeBParams"))
    {
        g_parameter_map_element = &g_parameter_NONTR_element[0];
        sizeof_table = (sizeof(g_parameter_NONTR_element)/sizeof(g_parameter_NONTR_element[0])); 
    }
    else if (strstr(param_name, "Show.Version"))
    {
        g_parameter_map_element = &g_parameter_NONTR_element[0]; 
        sizeof_table = (sizeof(g_parameter_NONTR_element)/sizeof(g_parameter_NONTR_element[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.WANDevice.[1].WANEthernetInterfaceConfig"))
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.FAP.Tunnel.VirtualInterfaceNum"))
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.FAP.Tunnel.VirtualInterface.[1]"))
    {
        g_parameter_map_element = &g_parameter_tunnel_virtualInterface[0];
        sizeof_table = (sizeof(g_parameter_tunnel_virtualInterface)/sizeof(g_parameter_tunnel_virtualInterface[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1]"))
    {
        g_parameter_map_element = &g_parameter_tunnel_crypto_profile[0];
        sizeof_table = (sizeof(g_parameter_tunnel_crypto_profile)/sizeof(g_parameter_tunnel_crypto_profile[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.FAP.Tunnel.IKESA.[1]"))
    {
        g_parameter_map_element = &g_parameter_tunnel_ikesa[0];
        sizeof_table = (sizeof(g_parameter_tunnel_ikesa)/sizeof(g_parameter_tunnel_ikesa[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice.FAP.PerfMgmt"))
    {
        g_parameter_map_element = &g_parameter_pm_gpv_spv_param[0];
        sizeof_table = (sizeof(g_parameter_pm_gpv_spv_param)/sizeof(g_parameter_pm_gpv_spv_param[0])); 
    }
    else if (strstr(param_name, "InternetGatewayDevice"))
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
        sizeof_table = (sizeof(g_parameter_TR_element)/sizeof(g_parameter_TR_element[0])); 
    }
    else
    {
        g_parameter_map_element = &g_parameter_TR_element[0];
    }


    if(g_parameter_map_element != NULL)
    {
        for(loop_index = 0; loop_index < sizeof_table; loop_index++)
        {
            if((strstr(g_parameter_map_element[loop_index].parameter_name,param_name)))
            {
			    TRACE(LOG_CRITICAL , GLUE_LAYER, "TYPE FOUND %d",g_parameter_map_element[loop_index].parameter_type);
                return (g_parameter_map_element[loop_index].parameter_type);
            }
            else
                continue;
        }
    }
    return 0;
}
/* SPR 13658 END */


/*SPR 17631 fixed start*/


void * handle_oam_file_upload_ind  (void  *msg)
{
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);
    void * p_buffer_for_upload = NULL;
    UInt32 fd = 0;
    transData_t *transD_t = NULL;
    void *p_res = NULL;
    /* SPR 22048 FIXED START */
    sUpload *ptr_U = (sUpload *) msg;
    /* SPR 22048 FIXED END */
    fd = get_fd();


    /*Coverity 97265 Fix Start*/
	YLIST yList_Upload = {0};
    /*Coverity 97265 Fix End*/

    init_list (&yList_Upload);

    p_buffer_for_upload = handle_oam_upload (msg);
    if (p_buffer_for_upload)
    {
        g_trans_id++;
        if(g_trans_id >TRANS_RANGE)
            g_trans_id = 1;

        construct_interface_api_header(p_buffer_for_upload , g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_FILE_UPLOAD_IND, g_upld_buffer);
        /* SPR 22048 FIXED START */
        TRACE(LOG_CRITICAL , GLUE_LAYER, "sleeping for before sending OAM upload ind %d\n",ptr_U->DelaySeconds);
        sleep(ptr_U->DelaySeconds);
        /* SPR 22048 FIXED END */
        send_msg_oam(p_buffer_for_upload, fd, g_upld_buffer);

        DEALLOCATE(p_buffer_for_upload);
        g_upld_buffer = 0;
		transD_t = ( transData_t *) ALLOCATE( sizeof(transData_t));
        if ( NULL == transD_t )
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
			//DEALLOCATE(l_ret_pre);
            return 0;
        }
        transD_t->trans_id = g_trans_id;
        addNodeinSet (&yList_Upload , transD_t);
        p_res = recv_msg_oam_Upload(&yList_Upload);
    }

    if (g_notif_buffer != NULL)
    {
       TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
       /* SPR 21588 FIXED START  */
       /* CID: 114336 Start */
       while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
       {
	       decode_notification(g_notif_buffer);
	       TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
	       qvFree(g_notif_buffer);
	       g_notif_buffer = NULL;
       }
       /* CID: 114336 End */
    }
       /* SPR 21588 FIXED END  */

       /* SPR 21588 FIXED END  */


	//DEALLOCATE(l_ret_pre);
    /*  coverity 96639 fix start */
	DEALLOCATE(transD_t);
    /*  coverity 96639 fix end */
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting %s",__FUNCTION__);
    return p_res;
}

void * handle_oam_upload (void *msg)
{
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Enetring in %s", __FUNCTION__);

	void *buffer_new =  (void *) ALLOCATE( 20);
    UInt32 offset = 0 ;

    if (buffer_new == NULL)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
        return NULL;

    }

    sUpload *ptr_U = (sUpload *) msg;

    if (ptr_U == NULL)
    {
        return buffer_new;
    }


    U8 fileType = '\0';

    if (ptr_U->FileType)
    {
        fileType  = (*(ptr_U->FileType) - 48);
    }

    /* API header */
    offset += 12;

    /* General */
    memset ((buffer_new+offset) , 0, 4);
    offset += 4;


    /* File Type */
    memcpy ((buffer_new+offset) , &fileType,  sizeof(U8));
    offset += 1;

    /* Reserved */
    memset (buffer_new+offset , 0, 3);
    offset += 3;

    g_upld_buffer = offset;

    TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting %s",__FUNCTION__);
    return buffer_new;
}
/*SPR 17631 fixed end*/



/*SPR 17631 fixed start*/
void * recv_msg_oam_Upload(YLIST *yList)
{
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering  %s",__FUNCTION__);
    UInt32 sockfd = 0;
    SInt32 byte_read = 0 ;
    struct timeval      timeval;
    fd_set set;
    UInt8 *buffer;
    UpldResp *p_upld_t = NULL;

	buffer = (UInt8  *)ALLOCATE( MAX_RECV_BUFF);
    if (NULL == buffer)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
        return NULL;
    }
    
    UInt16  api_idr = 0;
    transData_t g_transD_t;
    transData_t *p_retback_t = NULL;


    UInt16 recv_trans_id = 0;


	/* SPR 19083 FIXED START*/
	struct timeval start_t;
	struct timeval end_t;
	/* SPR 19083 FIXED END */ 



    struct sockaddr_in client;
    UInt32 len = sizeof(client);

    sockfd = get_fd();


	/* SPR 19083 FIXED START*/
	time_t ret_start = 0;
	time_t ret_end = 0;
	tr069_gettimeofday_wrapper(&start_t);
	ret_start = (time_t)start_t.tv_sec;

	tr069_gettimeofday_wrapper(&end_t);
	ret_end = (time_t)end_t.tv_sec;

	/* SPR 19083 FIXED END */ 

	p_upld_t = (UpldResp *)ALLOCATE( sizeof(UpldResp));


	if (NULL == p_upld_t)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Not Enough memory available",__FUNCTION__);
        //coverity fix
		DEALLOCATE(buffer);
        return NULL;
    }

	/* COVERITY 108702 FIX START */
	/*code removed */
	/* COVERITY 108702 FIX END */
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Start Time : %ld", ret_start);


    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    timeval.tv_sec = 0;
    timeval.tv_usec = 500000;

    while (!(ylEmpty(yList)))
    {
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeval,sizeof(timeval)) < 0)
        {
            TRACE(LOG_NONE , GLUE_LAYER, "Error in creating socket");
            perror("Error");
        }

        if ((byte_read = recvfrom(sockfd, buffer, MAX_RECV_BUFF, 0 , (struct sockaddr*)&client, &len)) < 0)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, " bytes read  : %d",byte_read );

			/* SPR 19083 FIXED START*/
			tr069_gettimeofday_wrapper(&end_t);
			ret_end = (time_t)end_t.tv_sec;
			/* sg wait_limit change start */
			if (ret_end - ret_start > 6000)
				/* sg wait_limit change end */
            {
				TRACE(LOG_CRITICAL , GLUE_LAYER, "END time nOW : %ld", ret_end);
				/* SPR 19083 FIXED END */ 
                /*
                p_dwn_t->eStatus = FAULT;
                p_dwn_t->fault_code = INTERNAL_FAILURE;
				   p_dwn_t->fault_string = (char *)ALLOCATE( 80);
                if (p_dwn_t->fault_string)
                    ENB_STRNCPY(p_dwn_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
                */

                p_upld_t->eStatus = RPC_FAULT;


                p_upld_t->fault_code = INTERNAL_FAILURE;
				p_upld_t->fault_string = (char *)ALLOCATE( 80);
                if (p_upld_t->fault_string)
                    ENB_STRNCPY(p_upld_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
				DEALLOCATE(buffer);
                return p_upld_t;
            }
            else
            {
                TR069_USLEEP(1000);
                continue;
            }
        }
        // Code added to support Notification in between RPC's
        api_idr = get_word_from_header((U8 *) (buffer) + 6);
        TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved api_idr = %d", api_idr);


       /* if (api_idr == TR069_OAM_HEARTBEAT_REQ)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved TR069_OAM_HEARTBEAT_REQ");
         */   /*send hearbeat response to sysctrl */
           /* if(GLUE_SUCCESS != send_heartbeat_resp())
            {
                //log message response not sent
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Failed to send hearbeat response");
            }
            else
            {
                //log message response sent successfully 
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Hearbeat response sent successfully");
            }

        }*/

/*CSR 57877 Fix START */
        if (api_idr == OAM_NOTIFICATION_IND)
/*CSR 57877 Fix End*/
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved Notification Request");
            g_notif_buffer  = ALLOCATE(MAX_RECV_BUFF);
	/* SPR 21588 FIXED START  */	
            if (g_notif_buffer){
                memcpy(g_notif_buffer , buffer , MAX_RECV_BUFF);
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
		ADD_IN_LIST(&g_notification_Q, (YLNODE *)g_notif_buffer);
		g_notif_buffer = NULL;

		}
	    /* SPR 21588 FIXED END  */
            continue;
        }

        if (api_idr == OAM_FILE_UPLOAD_IND_RESP)
        {
            TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved OAM_FILE_UPLOAD_IND_RESP");
            recv_trans_id = get_word_from_header(buffer);
            if(0 != recv_trans_id)
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved  transid:%d",recv_trans_id);
                g_transD_t.trans_id = recv_trans_id;
            }
            else
            {
                TRACE(LOG_CRITICAL , GLUE_LAYER, "Recieved ZERO in transid");
            }

            p_retback_t = (transData_t *)remove_node_from_Set(yList , &g_transD_t);

            if (p_retback_t != NULL)
            {
				/* COVERITY 108702 FIX START */
				/* code removed */
				/* COVERITY 108702 FIX END */
                REM_FROM_LIST(yList , (YLNODE *)p_retback_t);
                decode_OAM_Upload_Resp(buffer, p_upld_t);
				DEALLOCATE(buffer);
                return p_upld_t;
            }
        }
        else
			/* SPR 19083 FIXED START*/
			tr069_gettimeofday_wrapper(&end_t);
		ret_end = (time_t)end_t.tv_sec;
		/* sg wait_limit change start */
		if (ret_end - ret_start > 6000)
			/* sg wait_limit change end */
        {
			TRACE(LOG_CRITICAL , GLUE_LAYER, "Wait time: End time :%ld", ret_end);
			/* SPR 19083 FIXED END */ 

            TRACE(LOG_CRITICAL , GLUE_LAYER, "WAIT LIMIT OVER... ");
            p_upld_t->eStatus = RPC_FAULT;
            p_upld_t->fault_code = INTERNAL_FAILURE;
			p_upld_t->fault_string = (char *)ALLOCATE( 80);
            if (p_upld_t->fault_string)
                ENB_STRNCPY(p_upld_t->fault_string , (const char *)INTERNAL_FAILURE_STR , 80);
			DEALLOCATE(buffer);
            return p_upld_t;
        }

    }
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting %s",__FUNCTION__);
	DEALLOCATE(buffer);
    return  p_upld_t;
}

void decode_OAM_Upload_Resp (void *p_res, UpldResp * p_upld_t)
{
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);
    Uint16 l_offset = 0 ;
    U8 l_status = 0 ;
    Uint16 l_err_code = 0 ;
    U8 temp_l_len = 0 ;

    /* Skid header Reading */
    l_offset += 12;

    memcpy (&l_status , p_res+l_offset , 1) ;

    l_offset += 1;
     TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , l_status = %d",__FUNCTION__ ,l_status);

    if ( l_status == 0)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "RECEIVE STATUS SUCCESS");
        p_upld_t->eStatus = RPC_SUCCESS;
        memcpy ( &temp_l_len, p_res + l_offset , 1);
        l_err_code = temp_l_len & 0x00FF;
        l_err_code = l_err_code << 8;
        memcpy ( &temp_l_len, p_res + l_offset+1, 1);
        l_err_code = ((l_err_code) | (temp_l_len & 0x00FF));
        l_offset += 2;



		/* Reserved */
        l_offset += 4;

        p_upld_t->fault_code = l_err_code;

        temp_l_len = 0 ;
        memcpy (&temp_l_len ,  p_res + l_offset , 1);
        l_offset += 1;
		p_upld_t->fault_string = (char *) ALLOCATE( 80);
        if (p_upld_t->fault_string)
            memcpy (p_upld_t->fault_string ,  p_res + l_offset , temp_l_len);

        temp_l_len = 0 ;
        memcpy (&temp_l_len ,  p_res + l_offset , 1);
        l_offset += 1;
        /* SPR_19838: Coverity_12917_12905 Fix Start */
        p_upld_t->filepath = (char *) ALLOCATE (256);
        /* SPR_19838: Coverity_12917_12905 Fix Stop */
    /* SPR 19268 FIXED START*/
        /* COVERITY 109038/109036 FIXED START */
		if(p_upld_t->filepath)
        /* COVERITY 109038/109036 FIXED END */
        {
           memcpy(p_upld_t->filepath ,  p_res + l_offset, temp_l_len);

        TRACE(LOG_CRITICAL , GLUE_LAYER, "Upload file path:%s",p_upld_t->filepath);
    }
    /* SPR 19268 FIXED END*/
	}
    else
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "RECEIVE STATUS FAILURE");
        /* Parse the Error code and String */
        p_upld_t->eStatus = RPC_FAULT;
        memcpy ( &temp_l_len, p_res + l_offset , 1);
        l_err_code = temp_l_len & 0x00FF;
        l_err_code = l_err_code << 8;
        memcpy ( &temp_l_len, p_res + l_offset+1, 1);
        l_err_code = ((l_err_code) | (temp_l_len & 0x00FF));
        l_offset += 2;


 /* Reserved */
        l_offset += 4;

        p_upld_t->fault_code = l_err_code;

        temp_l_len = 0 ;

        memcpy (&temp_l_len ,  p_res + l_offset , 1);
        l_offset += 1;
		p_upld_t->fault_string = (char *) ALLOCATE( 80);
        if (p_upld_t->fault_string)
            memcpy (p_upld_t->fault_string ,  p_res + l_offset , temp_l_len);

    }
	TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting %s",__FUNCTION__);
}

/*SPR 17631 fixed end*/






/* Spr 16990 Fix Start  */
/****************************************************************************
 * Function Name  : handle_get_parameter_name_req 
 * Inputs         : msg
 * Outputs        : NONE 
 * Returns        : Void p_resp
 * Description    : This function handles the GPN get Request from HEMS 
 * ***************************************************************************/
void* handle_get_parameter_name_req(void * msg)
{

  TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s  ",__FUNCTION__);

  GetParameterNames * p_req = NULL;
  void               * p_res = NULL;
  void               * p_oam_msg = NULL;
  UInt16               msg_size = 0;
  /*  coverity 96689 fixed start */
  UInt32               retVal;
  /* coverity 96689 fixed end */
  UInt32               fd = 0;
   
  p_req = (GetParameterNames *)msg;

  if(p_req == NULL)
  {
      TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s :"
                            " Input Message is NULL", __FUNCTION__); 
      /*  coverity 96623 fixed start */
      return NULL;          
      /*coverity 96623 fixed end */
  }

  fd = get_fd();

  p_oam_msg = create_msg_to_oam_for_gpn(p_req, &msg_size);

  retVal = send_msg_oam (p_oam_msg , fd,  msg_size);
	DEALLOCATE(p_oam_msg);

  if(retVal == GLUE_SUCCESS)
  {    
      p_res = recv_msg_oam(NULL,RPC_GETPARAMETERNAME);  
  }
    /* SPR 17894 fixed start */

        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for Decoding Notification Request",__FUNCTION__);
	/* SPR 21588 FIXED START  */
   /* CID 114339 Start */
   while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
   /* CID 114339 End */
   {
        decode_notification(g_notif_buffer);
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
        qvFree(g_notif_buffer);
        g_notif_buffer = NULL;
   }

/* SPR 21588 FIXED END */
  
    /* SPR 17894 fixed end */

 
  TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
  return p_res;
}


/****************************************************************************
 * Function Name  : create_msg_to_oam_for_gpn 
 * Inputs         : p_req msg_size 
 * Outputs        : NONE 
 * Returns        : OAM message 
 * Description    : This function creates the GPN get Request For OAM 
 * ***************************************************************************/
void * create_msg_to_oam_for_gpn(void * msg ,UInt16 *msg_size)
{
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s  ",__FUNCTION__);

    UInt8 *p_oam_buffer = NULL, *p_temp_buf = NULL;
    UInt32 counter = 0;
    UInt16 parameter_path_length = 0;
    GetParameterNames * p_req = (GetParameterNames *)msg;

    parameter_path_length = strlen(p_req->ParameterPath);
 
    g_trans_id++;
    if(g_trans_id >TRANS_RANGE)
        g_trans_id = 1;

    /* storing Trans ID for the validation */
    gpn_trans_id = g_trans_id; 

    /* Calculating the msg Size */
    *msg_size += API_HEADER_SIZE; /* Size of API Header */ 
    *msg_size += sizeof(UInt16); /* Size of Parent length field */ 
    *msg_size += sizeof(UInt8); /* Size of Reserved field */
    *msg_size += parameter_path_length; /* Size of parameter path */
    *msg_size += sizeof(UInt8); /* Size of Next Level field */ 
    
	p_oam_buffer =  (void *) ALLOCATE( *msg_size);
    if (NULL == p_oam_buffer)
    {
        TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,"
                     " Not Enough memory available",__FUNCTION__);
        return NULL;
    }
    p_temp_buf =  p_oam_buffer; 

    construct_interface_api_header(p_temp_buf,gpn_trans_id, 
                                   TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID,
                                   OAM_GET_PARAMETER_NAME_REQ, *msg_size);

    p_temp_buf += API_HEADER_SIZE;

    p_temp_buf[counter] = (UInt8)(parameter_path_length >> 8); 
    counter+=sizeof(UInt8);
    p_temp_buf[counter] = (UInt8)parameter_path_length;
    counter+=sizeof(UInt8);
 
    /* Incrementing Counter For Reserved Field */
    counter+=sizeof(UInt8);
    
    /* Populating Parameter path Info */    
    strncpy((char *)&p_temp_buf[counter],
            (char *)p_req->ParameterPath, parameter_path_length);

    counter+=parameter_path_length;
   
    p_temp_buf[counter]= (UInt8) p_req->NextLevel; 

    TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting From %s ",__FUNCTION__);
    return p_oam_buffer; 
}


/****************************************************************************
 * Function Name  : decode_gpn_response 
 * Inputs         : buffer,p_gpn_res_t,end_mark 
 * Outputs        : resp from OAM 
 * Returns        : NONE 
 * Description    : This function decode the GPN response from OAM 
 * ***************************************************************************/
UInt32 decode_gpn_response(UInt8 *buffer,
                         GetParameterNamesStatus_t* p_gpn_res_t,
                         UInt8 * end_mark)
{
  int l_offset = 12; /* Skipping Header */
  int count = 0;
  U8 l_len_err = 0 ;
  U16 num_param_count = 0;
  U16 param_length = 0;
  UInt8 *p_res = NULL;
  p_res = buffer;
  int parameter_index = 0;
  UInt32 retVal = GLUE_SUCCESS;
 
  TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering From %s ",__FUNCTION__);
  TRACE (LOG_CRITICAL , GLUE_LAYER, "buffer received %s ",buffer);
 
  struct cwmp__ParameterInfoStruct ***parameter_list = 
           &p_gpn_res_t->parameterList->__ptrParameterInfoStruct;
 
  p_gpn_res_t->eStatus = RPC_SUCCESS; 
  
  /* Error Code Parsing */
  l_offset += 7; /* 1 Status + 2 Error Code + 4 Reserved */   
   
  memcpy ( &l_len_err,  p_res + l_offset , 1);

  l_offset += 1; /*1 error len info */  

  l_offset += l_len_err; /* error str len */ 

  num_param_count = num_param_count | (UInt16) ((p_res)[l_offset] << 8);
  num_param_count = num_param_count | (UInt16) ((p_res)[l_offset+1]); 

  TRACE (LOG_CRITICAL , GLUE_LAYER, "num_param_count =%d ",num_param_count);
  l_offset += 2; /*2 num parameter */ 

  *end_mark = p_res[l_offset];
  TRACE (LOG_CRITICAL , GLUE_LAYER, "end_mark =%d ",*end_mark);

  l_offset +=1; /* 1 End Marking */
  l_offset +=1; /* 1 reserved */

  /* Coverity 96659 + */
  *parameter_list =  TR_REALLOC((void *)*parameter_list ,
             (num_param_count +  p_gpn_res_t->parameterList->__size) * (sizeof(ParameterInfoStruct *)));
  /* Coverity 96659 - */

  if(NULL ==  *parameter_list) 
  {
    TRACE (LOG_CRITICAL , GLUE_LAYER, "Realloc Failed Return from Fun %s  ",__FUNCTION__);
    retVal  = GLUE_FAILURE ;  
    return retVal;
  }
  
  if(p_gpn_res_t->parameterList->__size > 0)
  parameter_index = p_gpn_res_t->parameterList->__size;

  TRACE (LOG_CRITICAL , GLUE_LAYER, "parameter_index =%d ",parameter_index);
/* SPR 20232 FIXED START */
  while (count< num_param_count)
  {
	  param_length = param_length | (UInt16) ((p_res)[l_offset] << 8);
	  param_length = param_length | (UInt16) ((p_res)[l_offset+1]);

	  l_offset+=2; /* param_length */


	  (*parameter_list)[parameter_index] = ALLOCATE((sizeof(ParameterInfoStruct)));
	  if(p_gpn_res_t->parameterList->__ptrParameterInfoStruct[parameter_index]==NULL)
		  TRACE (LOG_CRITICAL , GLUE_LAYER, "base pointer is NULL");

/* SPR 20455 FIXED START */
/* code Remove */
/* SPR 20455 FIXED START */

		(*parameter_list)[parameter_index]->Name =(char *)ALLOCATE(param_length);

	  memcpy((*parameter_list)[parameter_index]->Name, p_res+l_offset, param_length);

	  l_offset += param_length;
	  l_offset += 1; /* Reserved Byte */


        /*  coverity 96685 fix start */
            if(p_res[l_offset] == 0)
            {
            (*parameter_list)[parameter_index]->Writable = xsd__boolean__false_;
            }
            if(p_res[l_offset]  ==1)
            {
                (*parameter_list)[parameter_index]->Writable = xsd__boolean__true_;
            }
        /*coverity 96685 fixed end */
	  l_offset +=1; /* writable field */

	  param_length = 0;
	  parameter_index++;
	  count++;
  } 
/* SPR 20232 FIXED END */
 
  p_gpn_res_t->parameterList->__size += num_param_count;
  TRACE (LOG_CRITICAL , GLUE_LAYER, "total parameter received %d ",
                                          p_gpn_res_t->parameterList->__size);
  TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting From %s ",__FUNCTION__);
  return retVal;
}
/****************************************************************************
 * Function Name  : free_gpn_resp_buffer 
 * Inputs         : p_gpn_res_t 
 * Outputs        : NONE 
 * Returns        : NONE 
 * Description    : This function free dynamic allocated GPN response  
 * ***************************************************************************/
void free_gpn_resp_buffer (GetParameterNamesStatus_t *p_gpn_res_t)
{
  TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering From %s ",__FUNCTION__);
  int index = 0;

    if(p_gpn_res_t->parameterList->__size)
    {
        for(index = 0; index <p_gpn_res_t->parameterList->__size; index++)
        {
			DEALLOCATE(p_gpn_res_t->parameterList->__ptrParameterInfoStruct[index]->Name);
            p_gpn_res_t->parameterList->__ptrParameterInfoStruct[index]->Name = NULL;
			DEALLOCATE(p_gpn_res_t->parameterList->__ptrParameterInfoStruct[index]); 
        }
		DEALLOCATE(p_gpn_res_t->parameterList->__ptrParameterInfoStruct);
        p_gpn_res_t->parameterList->__ptrParameterInfoStruct = NULL;
    } 
	DEALLOCATE(p_gpn_res_t->parameterList);
    p_gpn_res_t->parameterList = NULL; 
	DEALLOCATE(p_gpn_res_t);
    /* coverity 96688 fixed code removed */
  TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting From %s ",__FUNCTION__);
}
/* Spr 16990 Fix End  */
/* Spr 17463 Fix Start */
/****************************************************************************
 * Function Name  : decode_gpv_response 
 * Inputs         : buffer,p_get_stat_t,end_mark 
 * Outputs        : resp from OAM 
 * Returns        : NONE 
 * Description    : This function decode the GPV response from OAM 
 * ***************************************************************************/
UInt32 decode_gpv_response(UInt8 *buffer,
                         GetStatus *p_get_stat_t,
                         UInt8 * end_mark)
{
    int l_offset = 12; /* Skipping Header */
    int count = 0;
    U8 l_len_err = 0 ;
    U16 num_param_count = 0;
    U16 param_length = 0;
    U16 Value_length = 0;
    UInt8 * p_res = buffer;
    int parameter_index = 0;
    char Value[256] = {0};
    UInt32 retVal = GLUE_SUCCESS;

    TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering From %s ",__FUNCTION__);

    struct cwmp__ParameterValueStruct ***parameter_list = 
        &p_get_stat_t->parameterList->__ptrParameterValueStruct;

    p_get_stat_t->eStatus = RPC_SUCCESS; 

    /* Error Code Parsing */
    l_offset += 7; /* 1 Status + 2 Error Code + 4 Reserved */   

    memcpy ( &l_len_err,  p_res + l_offset , 1);

    l_offset += 1; /*1 error len info */  

    l_offset += l_len_err; /* error str len */ 

    num_param_count = num_param_count | (UInt16) ((p_res)[l_offset] << 8);
    num_param_count = num_param_count | (UInt16) ((p_res)[l_offset+1]); 

    TRACE (LOG_CRITICAL , GLUE_LAYER, "num_param_count =%d ",num_param_count);
    l_offset += 2; /*2 num parameter */ 

    *end_mark = p_res[l_offset];
    TRACE (LOG_CRITICAL , GLUE_LAYER, "end_mark =%d ",*end_mark);

    l_offset +=1; /* 1 End Marking */
    l_offset +=1; /* 1 reserved */

    /* Coverity 96692 + */
    *parameter_list =  TR_REALLOC((void *)*parameter_list ,
            (num_param_count +  p_get_stat_t->parameterList->__size) * (sizeof(struct cwmp__ParameterValueStruct *)));
    /* Coverity 96692 - */

    if(NULL ==  *parameter_list) 
    {
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Realloc Failed Existing from Fun %s  ",__FUNCTION__);
        retVal  = GLUE_FAILURE ;  
        return retVal;
    }

    if(p_get_stat_t->parameterList->__size > 0)
        parameter_index = p_get_stat_t->parameterList->__size;

    TRACE (LOG_CRITICAL , GLUE_LAYER, "parameter_index =%d ",parameter_index);

    while (count< num_param_count)
    {
        param_length = param_length | (UInt16) ((p_res)[l_offset] << 8);
        param_length = param_length | (UInt16) ((p_res)[l_offset+1]); 

        l_offset+=2; /* param_length */ 

		(*parameter_list)[parameter_index] = ALLOCATE((sizeof(struct cwmp__ParameterValueStruct)));

		(*parameter_list)[parameter_index]->Name =(char *)ALLOCATE(param_length);

        memcpy((*parameter_list)[parameter_index]->Name, p_res+l_offset, param_length);     

        l_offset += param_length; /* strlen of parameter name */ 

        Value_length = Value_length | (UInt16) ((p_res)[l_offset] << 8);
        Value_length = Value_length  | (UInt16) ((p_res)[l_offset+1]);

        l_offset+=2; /* Value_length */

        memcpy(Value, p_res+l_offset, Value_length);   
 
		(*parameter_list)[parameter_index]->Value =(char *)ALLOCATE(Value_length);

        if (!((*parameter_list)[parameter_index]->__type = 
                    ipr_cwmp_find_parameter_type((char *)(*parameter_list)[parameter_index]->Name)))
        {
            (*parameter_list)[parameter_index]->__type = PAR_TYPE_STRING;
        }
		/* SPR 18816 FIXED START*/

		if((*parameter_list)[parameter_index]->__type == PAR_TYPE_UINT || 
           (*parameter_list)[parameter_index]->__type == PAR_TYPE_SINT)
        {
            (*(int *)((*parameter_list)[parameter_index]->Value)) = atoi((const char *)Value);
		}


		/*SPR 17793 Start*/

		else if((*parameter_list)[parameter_index]->__type == PAR_TYPE_BOOLEAN)
			/* SPR 18816 FIXED END*/
        {
            if((0 == strcmp((const char *) Value,"1"))|| (0 == strcmp((const char *) Value,"true")))
              (*(int *)((*parameter_list)[parameter_index]->Value)) = 1;
            else
              (*(int *)((*parameter_list)[parameter_index]->Value)) = 0;
        }
        /*SPR 17793 END*/
        else
        { 
            memcpy((*parameter_list)[parameter_index]->Value, Value, strlen(Value)+1);     
        }

        l_offset += Value_length; /* strlen of parameter Value */

        param_length = 0;
        Value_length = 0;
        parameter_index++; 
        count++;
    }
    p_get_stat_t->parameterList->__size += num_param_count;

    TRACE (LOG_CRITICAL , GLUE_LAYER, "total parameter received %d ",
            p_get_stat_t->parameterList->__size);

    TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting From %s ",__FUNCTION__);
    return retVal;
}
/* Spr 17463 Fix End */

/* SPR 17625 FIXED START */

/******************************************************************************
  *   FUNCTION NAME  : handle_get_param_attr
  *   Inputs         : msg, eType
  *   Outputs        : None
  *   RETURNS        : void *  - returns GetParamAttribute as a response.
  *   DESCRIPTION    : This function handles get parameter attribute RPC request received from ACS.
  *******************************************************************************/
 void *  handle_get_param_attr (void *msg)
 {
     TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s  ",__FUNCTION__);
     char **arrArgNames = NULL;
     UInt32 count  = 0 ;
     /*  coverity 96662/96667/96690 fixed  / code removed */ 
     UInt8 *paramName = NULL;
     //char *l_child_param = NULL;
     //char *l_parent_param = NULL;
      UInt16 total_no_of_parameter = 0, fd = 0, parent_len = 0;
     void *p_res = NULL;
     UInt16 l_offset = 0;
     void *p_enodeb_data = NULL;
     //GetParamAttrRes* resp;
    UInt32 msg_api_length;

     U8 msb = 0x00;
     U8 lsb = 0x00;

      fd = get_fd();
      
      struct sParameterAttr *arg_pNames_t = ( struct sParameterAttr *) msg ;
     if ( arg_pNames_t != NULL && arg_pNames_t->__ptrstring != NULL)
     {
         arrArgNames = (char **)arg_pNames_t->__ptrstring;
     }
     else
     {
         TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s : Memory unavailable", __FUNCTION__);
         return NULL;
     }

     /*Allocated 60000 bytes for buffer(to be sent to SOM)*/
	p_enodeb_data = ALLOCATE( GET_PARAM_ATTRIBUTE_PAYLOAD_SIZE);
     if (p_enodeb_data == NULL)
     {
         TRACE(LOG_CRITICAL , GLUE_LAYER, "%s Unable to allocate memory",__FUNCTION__);
         return NULL;
     }

     l_offset+=12;
     TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

    total_no_of_parameter = arg_pNames_t->__size;
     TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Total number of parameters = %d", __FUNCTION__ , total_no_of_parameter);

     //toatl number of parameters
     msb = total_no_of_parameter/256 ;
     lsb = total_no_of_parameter%256;

     /*Filling total number of parameters in buffer*/
     memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
     memcpy(p_enodeb_data+l_offset + 1, &lsb , sizeof(U8));

     l_offset += sizeof(UInt16);
    
    TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

     while (count < total_no_of_parameter)
     {

           // ENB_STRNCPY((char *)paramName , (const char *)(UInt8 *)msg , MAX_PARAM_NAME_SIZE);
         //paramName = (UInt8 *) arrArgNames[count]->Name;
         paramName = (UInt8 *) arrArgNames[count];
          TRACE (LOG_CRITICAL , GLUE_LAYER, "%s - Parameter Name [%d] = %s", __FUNCTION__ , count , paramName);
           //Node Name Length
          parent_len = strlen((const char *)paramName) + 1;
          TRACE (LOG_CRITICAL , GLUE_LAYER, "Parent length calculate %d",parent_len);

          msb = parent_len/256;
          lsb = parent_len%256;

         memcpy(p_enodeb_data+l_offset, &msb , sizeof(U8));
         memcpy(p_enodeb_data+l_offset+1, &lsb , sizeof(U8));

        l_offset += sizeof(UInt16);
        TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);

        //Parent Node Name

        //memcpy(p_enodeb_data+l_offset, (UInt8 *)arrArgNames[count]->Name, parent_len);
        memcpy(p_enodeb_data+l_offset, (UInt8 *)arrArgNames[count], parent_len);
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Received  Node Name %s",(char *)p_enodeb_data+l_offset);

        l_offset += parent_len; //strlen(p_enodeb_data+l_offset) + 1;

        TRACE (LOG_CRITICAL , GLUE_LAYER, "l_offset %d",l_offset);
        count++;

     }
      msg_api_length = l_offset;

    /* Fill API_HEADER */
      g_trans_id++;
      if(g_trans_id >TRANS_RANGE)
       g_trans_id = 1;
      construct_interface_api_header(p_enodeb_data, g_trans_id, TR069_MODULE_ID, OAM_MGMT_INTF_MODULE_ID, OAM_GET_PARAMETER_ATTRIBUTES, msg_api_length);
       /* Send message to oam */
             send_msg_oam(p_enodeb_data , fd , l_offset);
             TRACE (LOG_CRITICAL , GLUE_LAYER, "After send msg oam");
	DEALLOCATE(p_enodeb_data);
             TRACE (LOG_CRITICAL , GLUE_LAYER, "Free Successful"); 
	//DEALLOCATE(p_enodeb_data);
             p_enodeb_data = NULL;
             if(p_enodeb_data == NULL)
             {
		TRACE (LOG_CRITICAL , GLUE_LAYER, "p_enodeb is DEALLOCATE");
              }

       p_res = recv_msg_oam(NULL, RPC_GETPARAMETERATTRIBUTE);
        TRACE (LOG_CRITICAL , GLUE_LAYER, "In %s , Step4",__FUNCTION__);
        if (p_res)
        {
       /* SPR 21588 FIXED START */
		TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s ,Calling for  Decoding Notification Request",__FUNCTION__);
        /* CID 114750 Start */
		while (g_notif_buffer = (char *)REMOVE_FRM_LIST(&g_notification_Q))
        /* CID:114750 End */

		{
			decode_notification(g_notif_buffer);
			TRACE(LOG_CRITICAL , GLUE_LAYER, "In %s , Buffer recieved : %s ", __FUNCTION__ , g_notif_buffer);
			qvFree (g_notif_buffer);
			g_notif_buffer = NULL;
			TR069_USLEEP(500);
		}

       /* SPR 21588 FIXED END */
        }
        TRACE (LOG_CRITICAL , GLUE_LAYER, "Exiting from %s",__FUNCTION__);
        return p_res;
}
/* SPR 17625 FIXED END */
/*SPR 17627 FIXED START*/
int send_heartbeat_resp(void)
{
    void *msg_p = NULL;
    UInt32 fd = 0;
    /*coverity 96683 fixed code removed */
    UInt16 trans_id = 0;
    /*allocate memory from message pool */
	msg_p = ALLOCATE(API_HEADER_SIZE);
    
    TRACE(LOG_CRITICAL , GLUE_LAYER, "Entering %s",__FUNCTION__);
    fd = get_fd();

    if(NULL != msg_p)
    {
        /*get trans id*/
        g_trans_id++;
        if( g_trans_id >TRANS_RANGE)
        {
            g_trans_id = 1;
        }
        trans_id = g_trans_id;
        construct_interface_api_header(msg_p, trans_id, TR069_MODULE_ID,
                OAM_MGMT_INTF_MODULE_ID, TR069_OAM_HEARTBEAT_RESP, API_HEADER_SIZE);
        /* SPR 11921 Fix End*/
        send_msg_oam(msg_p, fd, API_HEADER_SIZE);
        TRACE(LOG_CRITICAL , GLUE_LAYER, "Heartbeat resp sent successfully");
    /*coverity 96683 fixed code removed */
		DEALLOCATE(msg_p);
        /* coverity 96636 fixed code removed */
    }
    else
    {
        /*log message Memory allocation failure */
        TRACE (LOG_CRITICAL, GLUE_LAYER, " %s unable to allocate memory",__FUNCTION__);
    }

    TRACE(LOG_CRITICAL , GLUE_LAYER, "Exiting %s",__FUNCTION__);
    return 1;
}
/*SPR 17627 FIXED END*/

/* SPR 21294 FIXED START */
/****************************************************************************
 ** ** Function Name  : checkDuplicateNotificationParams
 ** ** Inputs         : cpeParamArr, p_cpeParamVal_t
 ** ** Outputs        : Uint16 
 ** ** Returns        : UInt16: TRUE/FALSE
 ** ** Description    : This function return the recving socket descriptor 
 ** *****************************************************************************/
Uint16 checkDuplicateNotificationParams(cwmp__ParameterValueStruct **cpeParamArr, cwmp__ParameterValueStruct *p_cpeParamVal_t, Uint16 g_queued_paramList_size)
{
    Uint16 index = 0;
    for (index = 0; index < g_queued_paramList_size; index++)
    {
        if (!ENB_STRCMP((char *)cpeParamArr[index]->Name,(char *)p_cpeParamVal_t->Name))
        {
            ENB_STRCPY((char *)cpeParamArr[index]->Value, (char *)p_cpeParamVal_t->Value);
            TRACE (LOG_DETAIL, GLUE_LAYER, "Duplicate notification reveived for Parameter: %s", cpeParamArr[index]->Name);
            qvFree(p_cpeParamVal_t->Name);
            qvFree(p_cpeParamVal_t->Value);
            qvFree(p_cpeParamVal_t);
            return TR069_ONE;
        }

    }
    return TR069_ZERO;
}
/* SPR 21294 FIXED END */


/* SPR 21294 FIXED END */
