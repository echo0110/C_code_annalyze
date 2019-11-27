/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_main.c
 *
 * Description : This file contains main function and global data
 *                description.
 * Revision History:
 *
 *   Date        Author                Reference          Comments
 *   -----       ------                 ---------         --------
 * July,2016  Siddhant Prakash       			  Initial Version
 * 
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "oam_agnt_utils.h"
#include "oam_agnt_port.h"
#include "oam_agnt_main.h"

/*******************************************************************************
 * Exported Variables
 *******************************************************************************/

/*SPR 21889 +- */

/* Array for saving the arguments passed at startup so that next time when
 * restart needs to be done process can automatically starts itself with these
 * arguments
 */
Char8 **g_agent_args;
UInt8 g_agent_args_cnt = OAM_AGNT_ZERO;
extern char **environ;
extern const QSYSOP my_os;
const QMANIFEST agnt_entity;
LOCAL oam_sockaddr_t oam_address, agnt_address;

/*SPR 21889 +- */
/*******************************************************************************
  Private Types
 *******************************************************************************/

static void
oam_walltime (QTIME * now)
{
    oam_agnt_timeval T;
    gettimeofday(&T, OAM_AGNT_ZERO);
    now->s = T.tv_sec;
    now->us = T.tv_usec;
}

LISTENER *u;


const QWAIT my_waitstruct = {
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    oam_walltime
};

/*SPR 21889 +- */

/******************************************************************************
  Global Types
 ******************************************************************************/
/*SPR 21889 +- */
agnt_ext_com_config_t comm_data;

/***************************************************************************r
 * Function Name  : oam_agnt_timedwait
 * Inputs         : S :Pointer to void
 *                  T :Pointer to QTIME 
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for timedwait
 ****************************************************************************/

void oam_agnt_timedwait (void *S, const QTIME * T)	/*This function waits until either the listner the object is ready for read,
                                                          or until the specified timed out has occured */
{
    struct timeval  timeout = {0};
    SInt16     n;
    fd_set  F;
    FD_ZERO( &F );

    timeout.tv_sec = OAM_AGNT_TIMEOUT;
    timeout.tv_usec = OAM_AGNT_ZERO;
    
    n = select(  FD_SETSIZE - OAM_AGNT_ONE, &F, OAM_AGNT_ZERO, OAM_AGNT_ZERO, &timeout );
    if( n < OAM_AGNT_ZERO )
    {
       oam_agnt_perror("select");
    }

}

/****************************************************************************
 * Function Name  : oam_agnt_send
 * Inputs         : S :Pointer to void
 *                  message :Pointer to message
 *                  sender :sender
 *                  receiver :receiver
 *                  data :data str
 *                  size :size
 * Outputs        : None
 * Returns        : void
 * Description    : ENCAPS functionality to send message out of OAM
 ****************************************************************************/

void oam_agnt_send (void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, UInt32 size)	/*This function sends the message to the appropriate destination after
                                                                                                                  filling the Shell header */
{

    LISTENER *u   = (LISTENER *) S;
    U16 api_size  = OAM_AGNT_ZERO;
    UInt8 ret_val = OAM_AGNT_ZERO;

    message += OAM_AGNT_API_HEADER_SIZE;
    api_size = oam_agnt_get_word_from_header ((U8 *) message + OAM_AGNT_MSG_SZ_HDR_IDX);

    ret_val= sendto (u->rx_tx_sd, (SInt8 *) message,
            api_size, OAM_AGNT_ZERO,
            (struct sockaddr *) &oam_address,
            sizeof (struct sockaddr_in));
    
    if(ret_val)
    {
        /* SPR 17777 fix */
        OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"Send successfully , unused parameter %p, %p, %p and %p", sender, receiver, data, size);
    }


}

/****************************************************************************
 * Function Name  : oam_agnt_receive
 * Inputs         : sender :sender string
 *                  receiver :receiver string
 *                  priority :priority str
 * Outputs        : None
 * Returns        : void *
 * Description    : Encpas function for receive
 ****************************************************************************/
void *oam_agnt_receive (void *S, QMODULE * sender, QMODULE * receiver, signed char *priority)	/*This function receive a single incoming message in to a
                                                                                                  message buffer and returns it. This function determines the intended
                                                                                                  receiver, the sender and the priority of the message */

{

    char            receive_buffer[OAM_AGNT_RECV_BUFFER_SIZE] = { OAM_AGNT_ZERO };   
    fd_set          F;
    S32             n;
    U32             bytes_read;
    char            *msg_buf   = OAM_AGNT_NULL;
    U16             src_id     = OAM_AGNT_MODULE_ID;
    U16             dst_id     = OAM_AGNT_ZERO;
    U16             api_id     = OAM_AGNT_ZERO;
    U16             msg_size   = OAM_AGNT_ZERO;
    struct timeval  zero;
    LISTENER        *u         = (LISTENER *)S;


    FD_ZERO( &F );
    FD_SET( u->rx_tx_sd, &F );


    zero.tv_sec  = OAM_AGNT_ZERO;
    zero.tv_usec = OAM_AGNT_ZERO;
    
    /* SPR 17777 fix */

    n = select( FD_SETSIZE - OAM_AGNT_ONE, &F, OAM_AGNT_ZERO, OAM_AGNT_ZERO, &zero );

    if( n < OAM_AGNT_ZERO )
    {
        oam_agnt_perror("select");
    }

    if( FD_ISSET( u->rx_tx_sd, &F) )
    {

        bytes_read = recvfrom(u->rx_tx_sd, (char *)receive_buffer, OAM_AGNT_RECV_BUFFER_SIZE, OAM_AGNT_ZERO, 
                                          OAM_AGNT_ZERO, OAM_AGNT_ZERO);  

        if (!bytes_read)//klocwork fix
        {
            return OAM_AGNT_ZERO;
        }
        else
        {
            /* Parse header */
            src_id      = oam_agnt_get_word_from_header((UInt8 *)(receive_buffer + OAM_AGNT_SRC_ID_HDR_IDX));
            dst_id      = oam_agnt_get_word_from_header((UInt8 *)(receive_buffer + OAM_AGNT_DST_ID_HDR_IDX));
            api_id      = oam_agnt_get_word_from_header((UInt8 *)(receive_buffer + OAM_AGNT_API_ID_HDR_IDX));
            msg_size    = oam_agnt_get_word_from_header((UInt8 *)(receive_buffer + OAM_AGNT_MSG_SZ_HDR_IDX));
            OAM_AGNT_LOG(AGNT,OAM_AGNT_INFO,
                       "Received API[%u] of length[%u] from Module[%u] to Module[%u]", api_id, msg_size, src_id, dst_id);

            *sender     = qvGetService(src_id);
            *receiver   = qvGetService(dst_id);

            if(msg_size > OAM_AGNT_ZERO && msg_size < OAM_AGNT_RECV_BUFFER_SIZE)  
            {
                msg_buf = qvMsgAlloc(OAM_AGNT_NULL, OAM_AGNT_ZERO, OAM_AGNT_ZERO, msg_size);
                if (msg_buf == OAM_AGNT_NULL)
                {
                }
                else
                {
                    oam_agnt_memcpy(msg_buf ,receive_buffer, msg_size );

                }
            }
            else
            {
                OAM_AGNT_LOG(AGNT,OAM_AGNT_WARNING,"Msg size is invalid[%d]. Source Id=[%d], API Id=[%d]",
                            msg_size, src_id, api_id);
            }
        }
    }
    return msg_buf;
}

/****************************************************************************
 * Function Name  : oam_errorabort
 * Inputs         : s :Pointer to error string
 * Outputs        : none
 * Returns        : none
 * Description    : Encaps function for errorabort
 ****************************************************************************/

void oam_errorabort (const Char8 * s)	/*This function print the error and aborts the process */
{
    /* SPR 17777 fix */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"\nunused parameter %p\n", s);
    oam_agnt_abort ();
}

/****************************************************************************
 * Function Name  : oam_makeserver_udp
 * Inputs         : self :Pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP server socket and bind it.
 ****************************************************************************/
SInt16 oam_agnt_makeserver_udp (const oam_sockaddr_t * self)
{
    OAM_AGNT_FUN_ENTRY_TRACE();
    Int sd;	
    if ((sd = socket (AF_INET, SOCK_DGRAM, OAM_AGNT_ZERO)) < OAM_AGNT_ZERO)
    {
        oam_errorabort ("socket");
        OAM_AGNT_FUN_EXIT_TRACE();
    }

    if (bind(sd, (struct sockaddr *) self, sizeof (struct sockaddr)) < OAM_AGNT_ZERO)
    {
        perror ("AGENT_BIND_LTE:");
        OAM_AGNT_FUN_EXIT_TRACE();
        oam_agnt_exit (OAM_AGNT_EXIT_SUCCESS);
    }

    OAM_AGNT_FUN_EXIT_TRACE();
    return sd;
}

/****************************************************************************
 * Function Name  : oam_agnt_open
 * Inputs         : data :data str
 * Outputs        : None
 * Returns        : Listner structure.
 * Description    : Opens sockets for communication and xml parsing
 ****************************************************************************/

void *oam_agnt_open (void *data)
{
    OAM_AGNT_FUN_ENTRY_TRACE();
    UInt16 oam_port = OAM_AGNT_ZERO;
    UInt16 agnt_port = OAM_AGNT_ZERO;
    u = (LISTENER *) qvSysMalloc (sizeof (LISTENER));	/*This function
                                                          creates a new listner data structure, accepts a connection and
                                                          returns a pointer to updated listner structure */
    /* Read IP,Port of Lower Layers from enb_address_config.cfg */
    /* Receive Socket of OAM */
    
    oam_agnt_memset (&agnt_address, OAM_AGNT_ZERO, sizeof (agnt_address));
    agnt_address.sin_family = AF_INET;
    agnt_address.sin_addr.s_addr = inet_addr((const char *) comm_data.agnt_com_params.ipaddr); 
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"AGNT IP: %s \n",comm_data.agnt_com_params.ipaddr);
    agnt_port = atoi((const char *) comm_data.agnt_com_params.port);
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"AGNT Port: %s \n",comm_data.agnt_com_params.port);
    agnt_address.sin_port = oam_agnt_htons (agnt_port);
    u->rx_tx_sd = oam_agnt_makeserver_udp (&agnt_address);
    if (OAM_NEG_ONE == u->rx_tx_sd)
    {
        qvPanic ("ERROR: Failed to create UDP Server\n");
        oam_agnt_abort ();
    }
    /* Send Socket of AGNT */
    oam_agnt_memset (&oam_address, OAM_AGNT_ZERO, sizeof (oam_address));
    oam_address.sin_family = AF_INET;
    oam_address.sin_addr.s_addr = inet_addr((const char *) comm_data.oam_com_params.ipaddr); 
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"OAM IP: %s \n",comm_data.oam_com_params.ipaddr);
    oam_port = atoi((const char *) comm_data.oam_com_params.port);
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED,"OAM Port: %s , unused parameter %p \n",comm_data.oam_com_params.port, data);
    oam_address.sin_port = oam_agnt_htons (oam_port);

    OAM_AGNT_FUN_EXIT_TRACE();
    return u;
}

/****************************************************************************
 * Function Name  : oam_agnt_close
 * Inputs         : data :data str
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for close
 ****************************************************************************/

void oam_agnt_close (void *data)	/*This function cleans up the IPC listner structure */
{
    OAM_AGNT_FUN_ENTRY_TRACE();
    LISTENER *Lis = (LISTENER *) data;

    close (Lis->rx_tx_sd);
    qvSysFree (data);
    OAM_AGNT_FUN_EXIT_TRACE();
}

const QSHELL my_shell = {
    oam_agnt_timedwait,
    oam_agnt_send,
    oam_agnt_receive,
    oam_agnt_open,
    oam_agnt_close,
    OAM_AGNT_ZERO
};

/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_single_thd_init
 *   DESCRIPTION    : This function creates interface thd and also responsible 
 *                    for initializing the cspl library
 *  INPUT : NONE 
 *  RETURNS         : OAM_AGNT_FAILURE/OAM_AGNT_SUCCESS
 ******************************************************************************/

/* + SPR 17439 */
oam_agnt_return_et
oam_agnt_single_thd_init (void)
/* - SPR 17439 */
{

	void *listener = OAM_AGNT_NULL;

	OAM_AGNT_FUN_ENTRY_TRACE ();

	listener = qvOpen (&my_shell, OAM_AGNT_NULL);	/* open communication channel ie socket  */

	if (listener == OAM_AGNT_NULL)
	{
		OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,
				"Unable to create Listener");
		return OAM_AGNT_FAILURE;
	}

	qvSetDestination (OAM_MODULE_ID, &my_shell, listener, OAM_AGNT_ZERO);	/* setting destination */

	qvRegister (OAM_AGNT_ZERO, OAM_AGNT_ZERO, OAM_AGNT_ZERO, &agnt_entity, OAM_AGNT_ZERO);	/* registering module */



	qvRun (&my_shell, listener, &my_waitstruct, OAM_AGNT_ZERO);


	OAM_AGNT_FUN_EXIT_TRACE ();
	return OAM_AGNT_SUCCESS;
}
/*SPR 21889 +- */
/****************************************************************************
 * Function Name  : agnt_get_config_info
 * Description    : This is used to read configuration file 
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 *****************************************************************************/
oam_agnt_return_et
agnt_get_config_info (const Char8 *p_filename)
{
    char read_buf[OAM_AGNT_READ_BUFF_SIZE] = { OAM_AGNT_ZERO };
    FILE *fp_port_info = OAM_AGNT_NULL;
    OAM_AGNT_FUN_ENTRY_TRACE ();
    UInt8 inst_id = OAM_AGNT_ZERO;

    oam_agnt_memset(&comm_data, 0, sizeof(agnt_ext_com_config_t));
    fp_port_info = oam_agnt_fopen(p_filename,"r");
    if (OAM_AGNT_NULL == fp_port_info) 
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,"Not able to open cfg fileShutting down eNodeB");

        OAM_AGNT_FUN_EXIT_TRACE ();
        oam_agnt_exit(OAM_AGNT_EXIT_FAILURE);
    }
    while (oam_agnt_fgets((char *)read_buf,OAM_AGNT_READ_BUFF_SIZE,fp_port_info))
    {
        if(snscanf((const char *)read_buf,sizeof("ENB_OAM_IP_ADDRESS=")+OAM_AGNT_MAX_IP_ADD_LEN_STR,
                    "ENB_OAM_IP_ADDRESS=%s",comm_data.oam_com_params.ipaddr) > OAM_AGNT_ZERO )
        {
            if( !oam_agnt_isValidIp4(comm_data.oam_com_params.ipaddr) )
            {
                OAM_AGNT_LOG(AGNT,OAM_AGNT_ERROR,"LTE_OAM_IP_ADDRESS is INVALID. Shutting down eNodeB\n");
                oam_agnt_abort();
            }
            OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"LTE_OAM_IP_ADDRESS : %s",
                    comm_data.oam_com_params.ipaddr);
        }
        else if (snscanf((const char *)read_buf,sizeof("ENB_OAM_RX_PORT=")+OAM_AGNT_MAX_PORT_LENGTH,
                    "ENB_OAM_RX_PORT=%s",comm_data.oam_com_params.port) > OAM_AGNT_ZERO )
        {
            OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"LTE_OAM_RX_PORT : %s",
                    comm_data.oam_com_params.port);
        }
        else if(snscanf((const char *)read_buf,sizeof("ENB_OAM_AGNT_IP_ADDRESS=") + OAM_AGNT_MAX_IP_ADD_LEN_STR,
                    "ENB_OAM_AGNT_IP_ADDRESS=%s",comm_data.agnt_com_params.ipaddr) > OAM_AGNT_ZERO )
        {
            if( !oam_agnt_isValidIp4(comm_data.agnt_com_params.ipaddr) )
            {
                OAM_AGNT_LOG(AGNT,OAM_AGNT_ERROR,"ENB_OAM_AGNT_IP_ADDRESS is INVALID. Shutting down eNodeB");
                oam_agnt_abort();
            }
            OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"ENB_OAM_AGNT_IP_ADDRESS : %s",
                    comm_data.agnt_com_params.ipaddr);
        }
        else if (snscanf((const char *)read_buf,sizeof("ENB_OAM_AGNT_RX_PORT=")+OAM_AGNT_MAX_PORT_LENGTH,
                    "ENB_OAM_AGNT_RX_PORT=%s",comm_data.agnt_com_params.port) > OAM_AGNT_ZERO )
        {

            OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"ENB_OAM_AGNT_RX_PORT : %s",
                    comm_data.agnt_com_params.port);
        }
        else if (snscanf((const char *)read_buf,sizeof("L2_INSTANCE_ID=")+OAM_AGNT_MAX_PORT_LENGTH,
                    "L2_INSTANCE_ID=%d", &inst_id) > OAM_AGNT_ZERO )
        {

            OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"L2_INSTANCE_ID: %d",
                    inst_id);
            oam_agnt_set_my_id(inst_id);
        }
    }
    oam_agnt_fclose(fp_port_info);

    OAM_AGNT_FUN_EXIT_TRACE ();
    return OAM_AGNT_SUCCESS;
}
/****************************************************************************
 * Function Name  : main
 * Description    : This function is the entry point of OAM_AGNT.
 *                  Interfaces
 * Inputs         : Command line arguments.
 * Outputs        : none
 * Returns        : none
 ******************************************************************************/

int main (int argc, char* arg[])
{
 
    /*SPR 9435 Fix Start*/
    cpu_set_t       cpuSetSch;
    unsigned int    coreNum = OAM_AGNT_ONE;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;
    UInt8 i = OAM_AGNT_ZERO;

    if (argc != OAM_AGNT_FOUR)
    {    
        oam_agnt_fprintf (stderr, "\n USAGE:");
        oam_agnt_fprintf (stderr, "%s <Core binding> <eNodeB_Configuration.cfg> <log level>\n\n",
                arg[OAM_AGNT_ZERO]);
        oam_agnt_fprintf (stderr, "log level: %u ->CRITICAL\nlog level: %u ->ERROR\n"
                "log level: %u ->WARNING\nlog level: %u ->INFO\n"
                "log level: %u ->BRIEF\nlog level: %u ->DETAILED\n"
                "log level: %u ->DETAILEDALL\n", OAM_AGNT_CRITICAL, OAM_AGNT_ERROR,
                OAM_AGNT_WARNING, OAM_AGNT_INFO, OAM_AGNT_BRIEF,
                OAM_AGNT_DETAILED, OAM_AGNT_DETAILEDALL);
        return OAM_AGNT_FAILURE;
    }

    coreNum = oam_agnt_atoi(arg[OAM_AGNT_ONE]);
    if (coreNum >= (OAM_AGNT_EIGHT * sizeof(cpuSetSch)))
    {
    oam_agnt_fprintf(stderr,"Invalid core number[%d]\n", coreNum);
    return 0;
    }
    OAM_AGNT_CPU_ZERO(&cpuSetSch);
    OAM_AGNT_CPU_SET(coreNum ,&cpuSetSch);
    pid_t p = OAM_AGNT_ZERO;
    if (OAM_AGNT_ZERO != oam_agnt_sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch))
    {
        oam_agnt_fprintf(stderr, " oam_agnt_sched_setaffinity Returned Error in AGNT %s Core not Binded", arg[OAM_AGNT_THREE]);
        oam_agnt_exit(OAM_AGNT_EXIT_FAILURE);
    }
    /*SPR 9435 Fix End*/

	if(LogSuccess != init_log(LogTarget_File, "oam_agnt_logs.log", OAM_AGNT_NULL))
	{
		oam_agnt_fprintf(stderr, "Logging initialization failed");
		return 0;
	}
	else
	{
        set_loglevel_limit(LogContext_Default, oam_agnt_convert_log_level((log_priority_et)atoi(arg[OAM_AGNT_THREE])));
		OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Logging initialization successful");
	}

	qvSimpleInit (&my_os);

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(UInt8*) * (argc + OAM_AGNT_ONE), (void*)&g_agent_args, &err))
    {
	    OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR,
                "Memory allocation failed for g_agent_args. Cannot create argument list for agent");
        oam_agnt_exit(OAM_AGNT_EXIT_FAILURE);
    }
    g_agent_args_cnt = argc;
    for (i = OAM_AGNT_ZERO; i < argc ; i++)
    {
        g_agent_args[i] = arg[i];
    }
    g_agent_args[i] = OAM_AGNT_NULL;

    /*SPR 21889 Start*/
    agnt_get_config_info(arg[OAM_AGNT_TWO]);
    /*SPR 21889 End */
    if (OAM_AGNT_FAILURE == oam_agnt_main ())
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR, "AGNT initialization failed");
    }

    return OAM_AGNT_ZERO;

}

/*****************************************************************************
 * Function Name  : oam_agnt_main
 * Description    : Function to handle spawning of OAM process. 
 * Inputs         : TR & Non-TR XML file
 * Outputs        : none
 * Returns        : none
 ******************************************************************************/
/* + SPR 17439 */
oam_agnt_return_et 
oam_agnt_main (void)
/* - SPR 17439 */
{
    OAM_AGNT_FUN_ENTRY_TRACE ();

    oam_agnt_single_thd_init ();

    OAM_AGNT_FUN_EXIT_TRACE ();
    return OAM_AGNT_SUCCESS;
}

