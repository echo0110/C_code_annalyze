/******************************************************************************
 *   FILE NAME    : oam_snmp_main.c
 *
 *   DESCRIPTION  : SNMP Agent Initialization & Multithreading
 *
 *      DATE            AUTHOR          REFERENCE       REASON
 *   25 July 2013   Purshottam Gupta     ---------       Initial
 *   23 Aug  2013   Vivek K   Gupta      ---------       Added trap code
 *
 *   Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

/*******************************************************************************
 *                        Header Files
 *******************************************************************************/
#include <stacklayer.h>
#include <oam_snmp_main.h>
#include <oam_snmp_interface.h>

/*******************************************************************************
 *                        GLOBEL VARIBLES
 *******************************************************************************/
int wakeup_flag = SNMP_OAM_IF_WAKEUP_TIMEDOUT; /*timeout */
int expected_trans_id = 0;
int expected_api_id = 0;
void *channel;
typedef QTIMER oam_timer_t;
extern const QSYSOP* os;
extern const QWAIT* waitstruct;
extern const QSHELL* operations;
trans_cntrl_ptr p_cntrl_ptr;
prov_request_t prov_req;

/****************************************************************************
 * Function Name  : Alloc
 * Inputs         : module & size
 * Returns        : NONE
 ****************************************************************************/
static void *Alloc(QMODULE module, U32 attrsize)
{
    /* SPR 17777 fix */
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
            " unused parameter %p", module);
    /* Coverity FIX */
    return qvMsgAllocBySize( attrsize );
}

/****************************************************************************
 * Function Name  : Pack
 * Inputs         : module & size
 * Returns        : NONE
 ****************************************************************************/
static void *Pack(QMODULE module, void *buffer, U32 *size)
{
    /* SPR 17777 fix */
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
            " unused parameter %p and %p", module, size);

   /* Coverity FIX 39099 */
    return buffer;
}
/****************************************************************************
 * Function Name  : get_word_from_header
 * Inputs         : p_header
 * Outputs        : none
 * Returns        : U16 value
 * Description    : This function finds the value of 2 bytes
 ****************************************************************************/
U16 get_word_from_header
(
 U8 *p_header
 )
{
    U16 value = 0;

    value = p_header[0];
    value = value << ONE_BYTE_SHIFT;
    value = value | p_header[1];

    return value;
}

/******************************************************************************
 * Function Name    : oam_intf_process_timer
 * DESCRIPTION      : It is a callback function for timer expiry
 * INPUT            : p_timer_buf :Pointer to timer buffer
 *                    p_context :Pointer to context
 * OUTPUT           : None                       
 * RETURNS          : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et oam_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 )
{
    OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,"Timer expired: Timer [%p] p_context [%p]"
            " p_timer_buf [%p]", 
            timer_id, p_context, p_timer_buf);

    return OAM_SUCCESS;
}

/******************************************************************************
 * Function Name    : snmp_intf_process_timer
 * DESCRIPTION      : It is a callback function for timer expiry
 * INPUT            : p_timer_buf :Pointer to timer buffer
 *                    p_context :Pointer to context
 * OUTPUT           : None                       
 * RETURNS          : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et snmp_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 )
{
    int ret_val=0;
    init_oam_snmp_agent();

    OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,"Timer expired: Timer [%p] p_context [%p]"
            " p_timer_buf [%p]", 
            timer_id, p_context, p_timer_buf);

    while(1) {
        ret_val = agent_check_and_process(BLOCKING_CALL);
        if (-1 == ret_val) {
            continue;
        }
    }
    return OAM_SUCCESS;
}

/******************************************************************************
 * Function Name  : snmp_intf_process_msg
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This is the message handling function of OAM.
 ******************************************************************************/
oam_return_et snmp_intf_process_msg
(
 void *p_api_buf
 )
{
    /* SPR 17777 fix */
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
            " unused parameter %p", p_api_buf);

    return OAM_SUCCESS; 
}

/******************************************************************************
 * Function Name  : oam_intf_process_msg
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This is the message handling function of OAM.
 ******************************************************************************/
oam_return_et
oam_intf_process_msg(void *p_api_buf)
{

    UInt16         api_id   = OAM_ZERO;
    UInt16         msg_size = OAM_ZERO;
    UInt16 trans_id = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    trans_id = get_word_from_header(p_api_buf);
    api_id   = get_word_from_header((UInt8*)(p_api_buf) + SNMP_OAM_IF_API_ID_OFFSET);
    msg_size = get_word_from_header((UInt8*)(p_api_buf) + SNMP_OAM_IF_MSG_LEN_OFFSET);

    /* Store the received buffer in a global pointer 
     * after storing the buffer, signal the other thread to handle the received message*/
    if(trans_id == expected_trans_id && api_id == expected_api_id) {
        oam_mgmt_memset(p_cntrl_ptr.msg_ptr, 0, OAM_MGMT_IF_MAX_MSG_LEN);
        /* KLOCWORK warning fix start */
        if(msg_size <= OAM_MGMT_IF_MAX_MSG_LEN) {
            snmp_memcpy((p_cntrl_ptr.msg_ptr), p_api_buf, msg_size);
            OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                         "Received message from OAM, calling wakeup");
            expected_trans_id = 0;
            expected_api_id   = 0;
            wakeup_flag = SNMP_OAM_IF_MESSAGE_RECEIVED; /* message recieved */
            waitstruct->wakeup(channel);
        }
        else {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                      "Buffer overflow !!! Recieved message is too large");
            retVal = OAM_FAILURE;
        }
        /* KLOCWORK warning fix end */
    }
    else if(OAM_NOTIFICATION_IND == api_id) {    
        OAM_SNMP_LOG (SNMP, OAM_SNMP_DETAILED, "OAM alarm received");

        if(SNMP_OAM_IF_ERR_NOERR != snmp_oam_handle_oam_notif(p_api_buf)) {
            retVal = OAM_FAILURE;
        }
    }
    else {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                     "Message with unexpected transaction id[%u]. "
                     "Expected tran_id[%u]",
                     trans_id, expected_trans_id);
        retVal = OAM_FAILURE;
    }
    return retVal;
}

/*******************************************************************************
 *                        MANIFESTS & DESTINATION LISTS
 *******************************************************************************/
static const unsigned long SNMPDestList[] =
{
    OAM_MODULE_ID,
    0
};

const   QMANIFEST  SNMPEntity =
{
    "SNMP_MODULE_ID",
    SNMP_MODULE_ID,
    { 0, 0 },
    {(S32 (*)(void *, void *))snmp_intf_process_msg,(void (*)(QTIMER, void *, void *))snmp_intf_process_timer},
    {(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
    SNMPDestList
};
/* coverity 41270 41271 41274 41275 */

static const unsigned long OAMDestList[] =
{
    OAM_MODULE_ID,
    0
};

const   QMANIFEST  OAMEntity =
{
    "INTERFACE_MODULE_ID",
    INTERFACE_MODULE_ID,
    { 0, 0 },
    {(S32 (*)(void *, void *))(oam_intf_process_msg),(void (*)(QTIMER, void *, void *))(oam_intf_process_timer)},
    {(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
    OAMDestList
};
/* coverity 41270 41271 41272 41273 */

/******************************************************************************
 *   FUNCTION NAME   : snmp_intf_thread_func
 *   DESCRIPTION     : It is a callback function of pthread_create for "MGMT interface"
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * snmp_intf_thread_func(void * thread_arg)
{

    thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
    qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
    return 0;
}

/******************************************************************************
 *   FUNCTION NAME   : snmp_mgmt_thread_func
 *   DESCRIPTION     : It is a callback function of pthread_create for "OAM interface"
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * snmp_mgt_thread_func(void * thread_arg)
{

    thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
    qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
    return 0;
}

/******************************************************************************
 *   FUNCTION NAME   : main
 *   DESCRIPTION     : This function is main function
 *   Inputs          : argc :no of command line arguements
 *                   : arg[] :array of command line arguements   
 *   Outputs         : NONE
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
/* SPR 17777 fix */
int main(void)
{
    QDRIVER snmp_intf_driver,snmp_mgt_driver;
    thread_arg_t snmp_intf_thd_arg,snmp_mgt_thd_arg;
    pthread_t snmp_intf_tid,snmp_mgt_tid;

    void * listener;

    qvInit(0, os);
    listener = qvOpen(operations,NULL);
    /*SPR 22554 Fix Start*/
    Char8  threadName[20] = "oam_snmp_mgmt_intf";
    /*SPR 22554 Fix End*/
    if(!listener) {
        perror("qvOpen");
        oam_snmp_exit(1);
    }

    /* INFO FOR LOGGING */
    /* SPR 15198 Fixed Start */
    prov_req.log_level = OAM_SNMP_DETAILED;
    /*SPR 15198 Fixed End*/
    prov_req.p_out = fopen ("oam_snmp_logs.log", "w+");

    if(!prov_req.p_out) {
        oam_snmp_fprintf (stderr,
                "\n UNABLE TO OPEN 'oam_snmp_logs.log' For OAM_SNMP LOGS");
        oam_snmp_exit(1);
    }

    snmp_intf_driver = qvNewDriver();
    snmp_intf_thd_arg.driver_id = snmp_intf_driver;
    snmp_intf_thd_arg.p_qwait = (QWAIT*)waitstruct;

    snmp_mgt_driver = qvNewDriver();
    snmp_mgt_thd_arg.driver_id = snmp_mgt_driver;
    snmp_mgt_thd_arg.p_qwait = (QWAIT*)waitstruct;

    qvRegister(snmp_intf_driver, "SNMP_MODULE_ID",SNMP_MODULE_ID,
            &SNMPEntity,(void *)&snmp_intf_thd_arg);

    qvRegister(snmp_mgt_driver, "INTERFACE_MODULE_ID", INTERFACE_MODULE_ID,
            &OAMEntity,(void *)&snmp_mgt_thd_arg);
    qvSetDestination(OAM_MODULE_ID, operations, listener,0);
    qvSetImmediate(OAM_MODULE_ID, QVERROR_NONE);

    if(0 != pthread_create(&snmp_intf_tid, 0, snmp_intf_thread_func,
                (void*)&snmp_intf_thd_arg))
    {
        fprintf(stdout,"failed to create thread [%d] \n",__LINE__);
    }

    /*SPR 22554 Fix Start*/

    /* Set name of the main thread */
    if (0 != pthread_setname_np(snmp_intf_tid, threadName))
    {
        fprintf(stdout, "Failed to set name of the snmp management interface thread \n");
    }

    /*SPR 22554 Fix End*/
    if(0 != pthread_create(&snmp_mgt_tid, 0, snmp_mgt_thread_func,
                (void*)&snmp_mgt_thd_arg))
    {
        fprintf(stdout,"failed to create thread [%d] \n",__LINE__);
    }

    /*SPR 22554 Fix Start*/

    /* Set name of the main thread */
    snprintf(threadName, 20, "oam_snmp_intf");
    if (0 != pthread_setname_np(snmp_mgt_tid, threadName))
    {
        fprintf(stdout, "Failed to set name of the snmp OAM interface thread \n");
    }

    /*SPR 22554 Fix End*/
    QTIME timer_timeout;
    QMODULE module;
    waitstruct->walltime(&timer_timeout);

    timer_timeout.s += SNMP_MASTER_AGENT_POLLING_TIME_S;
    timer_timeout.us = SNMP_MASTER_AGENT_POLLING_TIME_US;
    module=qvGetService(SNMP_MODULE_ID);

    /* Timer for initial msg polling from Master agent */
    qvAbsoluteTimerStart(module,&timer_timeout,"TIMER",0,NULL);

    channel= waitstruct->newchannel();


    qvInputLoop(operations,listener);
    return 0;
}
