/*******************************************************************************
 * File Name   : oam_qcli_port.c
 *
 * Description : This file contains Qcli and socket based programming functions
 *
 * Revision History:
 *
 * Date             Author               Reference             Comments
 * -----            ------               ---------             --------
 * June,2012        Vivek Kumar Dixit    OAM Design Document   Initial Version
 *
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *******************************************************************************/
/*******************************************************************************
 * Includes
 *** ***************************************************************************/
#include<oam_qcli_proto.h>
#include <qcli.h>
#include <qutil.h>
#include <qrl.h>
#include <qval.h>
#include <qmacro.h>
#include <cspl.h>
#include<oam_defines.h>
#include<oam_proto.h>
#include<oam_port_utils.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/

extern UInt16 qcli_trans_id;
extern UInt8 multi_parent_flag;
/* Qcli variable */
extern edit_t *M;
extern interpret_t *L;
/*******************************************************************************
  Global Declaration
 *******************************************************************************/
oam_sockaddr_t g_oam_address, g_qcli_address;
extern const struct command_t commands[];
/* SPR 1251 FIX START */
extern void * cli_listener;
static void oam_close (void *data); 
/* + SPR 17439 */
void oam_cli_abort(void);
/* - SPR 17439 */
/* SPR 1251 FIX END */
fd_set F;
char flag_prompt_gb;
extern mess_send_t message_send;
char wait_gb;
QTIMER timer_id;
void oam_get_cli_communication_info(oam_cli_comm_info_t * comm_info);
#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)
int process_signal_flag=0;
#endif
/*SPR:9224_CHANGE_START*/
int timer_flag=0;
/*SPR:9224_CHANGE_END*/
/*******************************************************************************
  Private Definitions
 *******************************************************************************/
static Char8 *___completion (void *Q, const Char8 * s, UInt32 n, Int * p)
{
	return qclComplete ((interpret_t *) Q, s, n, p);
}

static Int ___choices (void *Q, const Char8 * s, UInt32 n, Int p)
{
	return qclChoices ((interpret_t *) Q, s, n, p);
}

static Int ___permission (void *Q, const Char8 * s, Int n)
{
    /* SPR 17777 fix */
    qcliHandleUnusedParameter(Q);
    qcliHandleUnusedParameter(n);
	return !qclQuoting (s);
}
	static void *
oam_newkey (void)
{
	pthread_key_t *key = (pthread_key_t *) oam_malloc (sizeof (pthread_key_t));
	if(NULL==key)
		return NULL;

	oam_memset( key, 0, sizeof (pthread_key_t) ); 
	pthread_key_create (key, 0);
	return (void *) key;
}
	static void *
oam_getkey (void *key)
{
	return pthread_getspecific (*((pthread_key_t *) key));
}

	static void
oam_setkey (void *key, void *value)
{
	pthread_setspecific (*((pthread_key_t *) key), value);
}

	static void *
oam_newlock (void)
{
	pthread_mutex_t *lock =
		(pthread_mutex_t *) oam_malloc (sizeof (pthread_mutex_t));
	if(NULL != lock){
		oam_memset( lock, 0, sizeof (pthread_mutex_t) ); 
		pthread_mutex_init (lock, 0);
	}
	return (void *) lock;
}

	static void
oam_lock (void *lock)
{
	pthread_mutex_lock ((pthread_mutex_t *) lock);
}

	static void
oam_unlock (void *lock)
{
	pthread_mutex_unlock ((pthread_mutex_t *) lock);
}

	static void
oam_destroylock (void *lock)
{
	pthread_mutex_destroy ((pthread_mutex_t *) lock);
	oam_free (lock);
}

	static Int
oam_vprintf (const Char8 * format, va_list A)
{
	return vfprintf (stderr, format, A);
}

typedef struct
{
	SInt16 rx_sd;			/*Receiver discriptor */
	SInt16 tx_sd;			/*Transmiter discriptor */
} LISTENER;			/*The porting function for UNIX paltform.
				  Structure QSYSOP is initialized with these functions */
LISTENER *u;

const QSYSOP my_os = {
	oam_malloc,
	oam_free,
	oam_newkey,
	oam_getkey,
	oam_setkey,
	oam_newlock,
	oam_destroylock,
	oam_lock,
	oam_unlock,
	oam_vprintf,
	oam_vprintf,
	/* SPR 1251 FIX START */
	oam_cli_abort
		/* SPR 1251 FIX END */
};

	static void
oam_walltime (QTIME * now)
{
	oam_timeval T;
	OAM_GETTIMEOFDAY (&T, OAM_ZERO);
	now->s = T.tv_sec;
	now->us = T.tv_usec;
}


typedef struct __waitdata
{
	pthread_cond_t condition;
	pthread_mutex_t lock;
	int value;
} __waitdata;


	static void *
oam_newchannel (void)
{
	__waitdata *w = (__waitdata *) oam_malloc (sizeof (__waitdata));
	if(w != NULL)
	{
		oam_memset(w,0,sizeof (__waitdata)); 
		pthread_mutex_init (&(w->lock), 0);
		w->value = 0;
	}
	return (void *) w;
}

	static void
oam_sleep (void *channel, const QTIME * timeout)
{
	__waitdata *w = (__waitdata *) channel;
	struct timespec T;
	pthread_mutex_lock (&(w->lock));
	if (!w->value)
	{
		if (!timeout)
		{
            /*SPR_20000 :Coverity_110656 Fix Start*/
            if(OAM_ZERO == pthread_cond_wait( &(w->condition), &(w->lock) ))
            {
            #ifdef OAM_DEBUG
                OAM_LOG(OAM_LOG,OAM_DETAILED,"conditional wait Success");
            #endif
            }

            /*SPR_20000 :Coverity_110656 Fix End*/

		}
		else if (timeout->s || timeout->us)
		{
			struct timeval _T;
			OAM_GETTIMEOFDAY (&_T, 0);

			_T.tv_sec += timeout->s;
			_T.tv_usec += timeout->us;
			while (_T.tv_usec >= 1000000L)
			{
				_T.tv_usec -= 1000000L;
				_T.tv_sec++;
			}


			T.tv_sec = _T.tv_sec;
			T.tv_nsec = 1000 * _T.tv_usec;
            /*SPR_20000 :Coverity_110650 Fix Start*/
            if(OAM_ZERO == pthread_cond_timedwait( &(w->condition), &(w->lock), &T ))
            {
#ifdef OAM_DEBUG
                OAM_LOG(OAM_LOG,OAM_INFO,"cond timedwait Success ");
#endif
            }
            /*SPR_20000 :Coverity_110650 Fix End*/
		}
		else
		{
			/**Nothing -- zero	timeout was specified**/
		}
	}

	w->value = 0;
	pthread_mutex_unlock (&(w->lock));
}


	static void
oam_wakeup (void *channel)
{
	__waitdata *w = (__waitdata *) channel;
	pthread_mutex_lock (&(w->lock));
	w->value = 1;
	pthread_cond_signal (&(w->condition));
	pthread_mutex_unlock (&(w->lock));
}


const QWAIT my_waitstruct = {
	oam_newchannel,
	oam_sleep,
	oam_wakeup,
	oam_walltime
};


	UInt16
oam_get_word_from_header (UChar8 * p_header)
{
	UInt16 value = OAM_ZERO;
	value = p_header[OAM_BIT_ZERO];
	value = value << OAM_BYTESIZE;
	value = value | p_header[OAM_BIT_ONE];
	return value;
}

	UChar8
oam_get_byte_from_header (UChar8 * p_header)
{
	return *p_header;
}



/****************************************************************************
 * Function Name  : oam_errorabort
 * Inputs         : error string
 * Outputs        : prints the error string
 * Returns        : none
 * Description    : Encaps function for errorabort
 ****************************************************************************/
static void oam_errorabort (const Char8 * s)	/*This function print the error and aborts the process */
{
    /* SPR 17777 fix */
    qcliHandleUnusedParameter(s);

	OAM_ABORT ();
}

/* SPR 1251 FIX START */
/****************************************************************************
 * Function Name  : oam_cli_abort
 * Inputs         : None
 * Outputs        : None
 * Returns        : void
 * Description    : contains the handling for aborting the CLI.
 ****************************************************************************/
/* + SPR 17439 */
void oam_cli_abort(void)
/* - SPR 17439 */
{
	oam_close(cli_listener);
	OAM_EXIT(0);
}

/* SPR 1251 FIX END */
/****************************************************************************
 * Function Name  : oam_makeserver_udp
 * Inputs         : pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket and bind it.
 ****************************************************************************/
static SInt16 oam_makeserver_udp (const oam_sockaddr_t * self)
{
	Int sd;			/*socket descriptor */

	if ((sd = socket (AF_INET, SOCK_DGRAM, OAM_ZERO)) < OAM_ZERO)
	{
		oam_errorabort ("socket");
	}

	if (OAM_BIND (sd, (struct sockaddr *) self, sizeof (*self)) < OAM_ZERO)
	{
		perror ("OAM_LTE:");
		OAM_EXIT (0);
	}

	return sd;
}

/****************************************************************************
 * Function Name  : oam_makeclient_udp
 * Inputs         : pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket.
 ****************************************************************************/
static SInt16 oam_makeclient_udp (void)
{
	long int sd;	/*cov_fix*/		/*socket descriptor */

	if ((sd = socket (AF_INET, SOCK_DGRAM, OAM_ZERO)) < OAM_ZERO)
	{
		oam_errorabort ("socket");
	}

	return sd;
}

	int
show_prompt ()
{
	char *prompt = OAM_NULL;
	const Char8 *t;
	/* review commt 1 */
	char *line = OAM_NULL;
	char *s;

	if ((t = qclGetObject (L, OAM_ZERO)) != OAM_ZERO)
	{
		prompt = qasprintf (OAM_ZERO, "[%s] %s: ", t, qclGetMode (L));
	}
	else
	{
		prompt = qasprintf (OAM_ZERO, "OAM_LTE %s#>", qclGetMode (L));
	}
	if(prompt == OAM_NULL)
	{
		return -1;
	}
	else
	{
		line = qrlGetLine (M, prompt, ___completion, ___choices, ___permission, L);
		oam_free (prompt);
        /* OAM BCOM Code Comment Changes Start */
        prompt = OAM_NULL;
        /* OAM BCOM Code Comment Changes End */
	}
	if (line == OAM_NULL)
	{
		return -1;
	}
	else
	{
		if (qclExecute (L, line) == OAM_ZERO)
		{
			oam_free (line);
			OAM_CLOSE (1);
			OAM_CLOSE (2);
			OAM_EXIT (0);
		}
	}
	/* Coverity Fix 55194 */
	oam_free (line);
    /* OAM BCOM Code Comment Changes Start */
    line = OAM_NULL;
    /* OAM BCOM Code Comment Changes End */
	if ((s = qclResult (L)) != OAM_ZERO)
	{
		oam_fprintf (stderr, "[%s]\n", s);
		oam_free (s);
	}
	if (message_send.message_send == 0 )
	{
		show_prompt ();
	}
	return OAM_ZERO;
}

/****************************************************************************
 * Function Name  : oam_receive
 * Inputs         : sender, receiver, priority
 * Outputs        : None
 * Returns        : void *
 * Description    : Encpas function for receive
 ****************************************************************************/
static void *oam_receive (void *S, QMODULE * sender, QMODULE * receiver, signed char *priority)	/*This function receive a single incoming message in to a
												  message buffer and returns it. This function determines the intended
												  receiver, the sender and the priority of the message */
{
	UChar8 *receive_buffer = OAM_NULL;
	oam_error_code_et err_code = NO_ERROR;
	SInt64 n;
	long int                 bytes_read;/*cov_fix*/
	UChar8 *msg_buf = NULL;
	UInt16 src_id = QCLI_MODULE_ID;

	UInt16 dst_id = OAM_ZERO;
	UInt16 api_id = OAM_ZERO;
	UInt16 recv_trans_id = OAM_ZERO;
	struct timeval zero;
	LISTENER *u = (LISTENER *) S;

	FD_ZERO (&F);
	FD_SET (u->rx_sd, &F);

    /* SPR 17777 fix */
    qcliHandleUnusedParameter(priority);

	zero.tv_sec = 1;
	zero.tv_usec = 0;

	n = select (FD_SETSIZE - 1, &F, 0, 0, &zero);

	if (n < 0)
	{
		return OAM_NULL;
	}
	if(OAM_FAILURE == oam_mem_alloc(MAX_RX_DATA_BUFF_SIZE, (void**)&receive_buffer,
				&err_code)) {
        oam_fprintf(stderr, "\nMemory allocation failed with error code %d\n",
				err_code);
		return OAM_NULL;
	}

	if (FD_ISSET (u->rx_sd, &F))
	{
		if (message_send.message_send == 1)
		{
			do
			{

				if ((bytes_read = OAM_RECV_FROM (u->rx_sd, (char *) receive_buffer,
								MAX_RX_DATA_BUFF_SIZE, OAM_ZERO,
								OAM_ZERO, OAM_ZERO)) < OAM_ZERO)
				{
					oam_errorabort ("read:attr");
				}
				else if (!bytes_read)
				{
					/* Coverity ID : 72221 Fix Start*/
					oam_mem_free(receive_buffer,&err_code);
					/* Coverity ID : 72221 Fix End*/
					return OAM_NULL;
				}
				else if (bytes_read < OAM_MGMT_INTF_HEADER_SIZE)
				{
					qvLogDump (OAM_ZERO, (unsigned char *) receive_buffer,
							bytes_read, OAM_ZERO);
				}
				else
				{
					/* Parse header */
					recv_trans_id = get_word_from_header (receive_buffer + 0);
					src_id = get_word_from_header (receive_buffer + 2);
					dst_id = get_word_from_header (receive_buffer + 4);
					api_id = get_word_from_header (receive_buffer + 6);
					if ((qcli_trans_id != recv_trans_id)&& (dst_id != OAM_QCLI_TRAP_MODULE_ID))
					{
						/* Coverity ID : 72221 Fix Start*/
						oam_mem_free(receive_buffer,&err_code);
						/* Coverity ID : 72221 Fix End*/
						return OAM_NULL;
					}    

					*sender = qvGetService (src_id);
					*receiver = qvGetService (dst_id);

					if ((api_id == message_send.apiid_expected)
							|| (api_id == OAM_NOTIFICATION_IND))
					{
						msg_buf =
							qvMsgAlloc (NULL, OAM_ZERO, OAM_ZERO, bytes_read);
						if (msg_buf == PNULL)
						{
							oam_fprintf (stderr, "Memory allocation to msg_buf"
									" failed\n");
							/* Not enough memory */
						}
						else
						{
#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)   
							process_signal_flag=0;
#else
							QTIME timeout;
							/*SPR:9224_CHANGE_START*/
							if(qvTimerRunning(timer_id)==1)
							{
								timer_flag=0; 
								qvModuleTimerStop (timer_id, &timeout, NULL);
							}
							/*SPR:9224_CHANGE_END*/
#endif
							oam_memcpy (msg_buf, receive_buffer, bytes_read);
							break;
						}
					}
				}
			}
			while ((api_id != message_send.apiid_expected)
					|| (api_id != OAM_NOTIFICATION_IND));
		}
		else
		{
			if ((bytes_read = OAM_RECV_FROM (u->rx_sd, (char *) receive_buffer,
							MAX_RX_DATA_BUFF_SIZE, OAM_ZERO,
							OAM_ZERO, OAM_ZERO)) < OAM_ZERO)
			{
				oam_errorabort ("read:attr");
			}
			else if (!bytes_read)
			{

				/* Coverity ID : 72221 Fix Start*/
				oam_mem_free(receive_buffer,&err_code);
				/* Coverity ID : 72221 Fix End*/
				return OAM_NULL;
			}
			else
			{
				/* Parse header */
				recv_trans_id = get_word_from_header (receive_buffer + 0);
				src_id = get_word_from_header (receive_buffer + 2);
				dst_id = get_word_from_header (receive_buffer + 4);
				/*Coverity 79037 Fix Start*/
				/*Coode deleted*/
				/*Coverity 79037 Fix End*/
                                if ((qcli_trans_id != recv_trans_id)&& (dst_id != OAM_QCLI_TRAP_MODULE_ID))
				{
					/* Coverity ID : 72221 Fix Start*/
					oam_mem_free(receive_buffer,&err_code);
					/* Coverity ID : 72221 Fix End*/
					return OAM_NULL;
				}    


				*sender = qvGetService (src_id);
				*receiver = qvGetService (dst_id);

				msg_buf = qvMsgAlloc (NULL, OAM_ZERO, OAM_ZERO, bytes_read);
				if (msg_buf == PNULL)
				{
					oam_fprintf (stderr, "Memory allocation to msg_buf"
							" failed\n");
					/* Not enough memory */
				}
				else
				{
					oam_memcpy (msg_buf, receive_buffer, bytes_read);
				}

			}
		}

	}

	if ((!flag_prompt_gb) && message_send.message_send == 0)
	{
		flag_prompt_gb = 1;

		if ((M = qrlOpen (OAM_ZERO, OAM_ZERO, OAM_ZERO, OAM_ZERO)) == OAM_ZERO)
		{
	            oam_fprintf (stderr, "Please check your `TERM'\n");

	            /* Coverity ID : 72221 Fix Start*/
		    oam_mem_free(receive_buffer,&err_code);
		   /* Coverity ID : 72221 Fix End*/
	            return " ";
		}
		L = qclOpen (commands, OAM_ZERO, OAM_ZERO);



		*sender = qvGetService (src_id);
		*receiver = qvGetService (10);
		receive_buffer[0] = 0x00;


		msg_buf = qvMsgAlloc (NULL, OAM_ZERO, OAM_ZERO, 1);
		if(NULL != msg_buf)
		{
			oam_memcpy (msg_buf, receive_buffer, 1);
		}
		else
		{
			oam_fprintf (stderr, "Memory allocation to msg_buf"
					" failed\n");
		}
	}

        /* Coverity ID : 72221 Fix Start*/
        oam_mem_free(receive_buffer,&err_code);
        /* Coverity ID : 72221 Fix End*/
	return msg_buf;
}

/****************************************************************************
 * Function Name  : oam_open
 * Inputs         : pointer to data
 * Outputs        : None
 * Returns        : void *
 * Description    : Opens sockets for communication and xml parsing
 ****************************************************************************/
static void *oam_open (void *data)
{
	u = (LISTENER *) qvSysMalloc (sizeof (LISTENER));	/*This function
								  creates a new listner data structure, accepts a connection and
								  returns a pointer to updated listner structure */
	/* Read IP,Port of Lower Layers from enb_address_config.cfg */
	/* Receive Socket of OAM */
	oam_cli_comm_info_t comm_info;
	oam_memset (&comm_info, OAM_ZERO, sizeof (oam_cli_comm_info_t));

    /* SPR 17777 fix */
    qcliHandleUnusedParameter(data);

	oam_get_cli_communication_info(&comm_info);
	oam_memset (&g_qcli_address, OAM_ZERO, sizeof (g_qcli_address));
	g_qcli_address.sin_family = AF_INET;
	g_qcli_address.sin_addr.s_addr = OAM_INET_ADDR (comm_info.cli_ip_addr);
	g_qcli_address.sin_port = oam_htons (comm_info.cli_port);
	u->rx_sd = oam_makeserver_udp (&g_qcli_address);
	if (OAM_NEG_ONE == u->rx_sd)
	{
		qvPanic ("ERROR: Failed to create UDP Server\n");
		/* SPR 1251 FIX START */
		oam_cli_abort ();
		/* SPR 1251 FIX END */
	}
	/* Send Socket of OAM */
	oam_memset (&g_oam_address, OAM_ZERO, sizeof (g_oam_address));
	g_oam_address.sin_family = AF_INET;
	g_oam_address.sin_addr.s_addr = OAM_INET_ADDR (comm_info.oam_ip_addr);
	g_oam_address.sin_port = oam_htons (comm_info.oam_port);

	u->tx_sd = oam_makeclient_udp ();
	if (OAM_NEG_ONE == u->tx_sd)
	{
		qvPanic ("ERROR: Failed to create UDP client\n");
		/* SPR 1251 FIX START */
		oam_cli_abort ();
		/* SPR 1251 FIX START */
	}
	return u;
}

/****************************************************************************
 * Function Name  : oam_close
 * Inputs         : pointer to data
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for close
 ****************************************************************************/
static void oam_close (void *data)	/*This function cleans up the IPC listner structure */
{
	LISTENER *Lis = (LISTENER *) data;

	close (Lis->tx_sd);
	close (Lis->rx_sd);
	qvSysFree (data);
	qclClose (L);
	qrlClose (M);
}

/****************************************************************************
 * Function Name  : oam_timedwait
 * Inputs         : S, QTIME *T
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for timedwait
 ****************************************************************************/
static void oam_timedwait (void *S, const QTIME * T)	/*This function waits until either the listner the object is ready for read,
							  or until the specified timed out has occured */
{
    /* SPR 17777 fix */
    qcliHandleUnusedParameter(S);
    qcliHandleUnusedParameter(T);

	/*   LISTENER        *u = (LISTENER *)S;
	     struct timeval  timeout;
	     SInt16     n;
	     fd_set  F;
	     FD_ZERO( &F );
	     FD_SET( u->rx_sd, &F );

	     if( T )
	     {
	     timeout.tv_sec = 10;//T->s;
	     timeout.tv_usec = 0;// T->us;
	     }
	     n = select(  FD_SETSIZE - 1, &F, OAM_ZERO, OAM_ZERO, &timeout );

	     if( n < OAM_ZERO )
	     {
	// oam_errorabort("select");
	} */
}

/****************************************************************************
 * Function Name  : oam_send
 * Inputs         : message, sender, receiver, data, size
 * Outputs        : None
 * Returns        : void
 * Description    : ENCAPS functionality to send message out of OAM
 ****************************************************************************/

	static void
oam_send(void *S, const void *message,
		QMODULE sender, QMODULE receiver,
		void *data, UInt32 size)	
{
	LISTENER *u = (LISTENER *) S;
	U16 api_id = 0;
	U16 api_size = 0;
	SInt32 retVal = OAM_ZERO;
	QMODULE module = OAM_NULL;

	message += OAM_CSPL_HDR_SIZE;
	api_id = get_word_from_header ((U8 *) message + 6);
	api_size = get_word_from_header ((U8 *) message + 8);

    /* SPR 17777 fix */
    qcliHandleUnusedParameter(sender);
    qcliHandleUnusedParameter(receiver);
    qcliHandleUnusedParameter(data);
    qcliHandleUnusedParameter(size);

#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)
	process_signal_flag=1;
#endif
#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)
	alarm(15);
#else
	if(0 == multi_parent_flag)
	{
		QTIME timer_timeout;
		timer_timeout.s = OAM_USER_INTF_RESP_TIMEOUT_S;
		timer_timeout.us = OAM_ZERO;

		/* Coverity CID: 55189 fix */
		module = qvGetService(QCLI_MODULE_ID);
		if(!module) {
            oam_fprintf(stderr, "Failed to fetch the module-id");
			return;
		}
		/*SPR:9224_CHANGE_START*/
		timer_flag=1;
		/*SPR:9224_CHANGE_END*/

		timer_id = qvModuleTimerStart(module, &timer_timeout,
				OAM_NULL, OAM_ZERO, OAM_NULL);
	}
	else {
		/* Reset flag */
		multi_parent_flag = 0;
	}
#endif

	retVal = OAM_SEND_TO (u->tx_sd, (SInt8 *) message,
			api_size, OAM_ZERO,
			(struct sockaddr *) &g_oam_address,
			sizeof (struct sockaddr_in));
	if(retVal < OAM_ZERO) {
        oam_fprintf(stderr, "\nError: Sending failed permanently, "
				"the message will be dropped\n");
		return;
	}

	if (api_id == OAM_SHUTDOWN_REQ)
		OAM_EXIT (1);
}

const QSHELL my_shell = {
	oam_timedwait,
	oam_send,
	oam_receive,
	oam_open,
	oam_close,
	OAM_ZERO
};
