/********************************************************************************
 * File Name    : oam_port_main.c
 * Description  : This file contains procedures to detect the ethernet adaptor ,
 *               and creates the virtual interfaces for OAM.

 * Revision History :
 * Date            Author              Reference             Comments
 * June,2012       Purshottam Gupta    OAM Design Document   Release 0.9 Change
 * January,2013    Vinod Vyas          OAM Design Doc        Changes for SDM
 * June, 2014       Swetank Srivastava     SPR 11314 Fix
 Release 2.0 
  * May 2014        Shalu Sagar         Klockwork Fix                                    
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for log 
 *                                                      level enhancements
 * 
  * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 *********************************************************************************/

#include <cspl.h>
#include<oam_sim.h>
#include <signal.h>
#include <oam_defines.h>
#include <oam_types.h>
#include <oam_proto.h>
#include <oam_sim_utils.h>
#include <oam_fault_mgmt.h>
#include <alarm.h>
#include<oam_fm_alarm_defs.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/
extern prov_request_t prov_req;
extern oam_prov_req_t oam_prov_req;

extern oam_bool_et oam_is_sdm_alive;
extern oam_sim_table_t g_oam_sim_t; 
extern oam_bool_et shutdown_flag;
extern UInt16 flag_shutdown_for_soft_upgrade;
oam_ext_com_config_t comm_data;
/* SPR 20908 Start */
lower_layer_init_info_t l2_comm_info[OAM_MAX_NUM_L2_INSTANCE];
/* SPR 20908 End */
/*Coverity_CID_58308_FIX_START*/
Char8    g_receive_buffer[OAM_MAX_RECV_BUF_SIZE];
/*Coverity_CID_58308_FIX_END*/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
UInt32 get_config_info(void);
/* - SPR 17439 */
void oam_make_int_to_str(ULong32 value,SInt8* addr_str);
void make_int_to_str(ULong32 value,SInt8* addr_str);
void oam_port_process_shutdown(void);

U8 *test_buf;

LOCAL oam_sockaddr_ipfv4 rrm_address;
LOCAL oam_sockaddr_ipfv4 l3_address;
/* SPR 20908 Start */
/* code removed */
/* SPR 20908 End */
/*SPR 21889 Start +- */


LOCAL oam_sockaddr_ipfv4 qcli_address;

#ifdef OAM_SON_ENABLE
LOCAL oam_sockaddr_ipfv4 son_address;
#endif
LOCAL oam_sockaddr_ipfv4 tr069_address;
LOCAL oam_sockaddr_ipfv4 sdm_address; 
LOCAL oam_sockaddr_ipfv4 snmp_address; 

void set_cell_index(U8 cell_index) 
{
    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILEDALL,"unused parameter %d",cell_index);
}

/******************************************************************************
 *   FUNCTION NAME   : s_malloc
 *   DESCRIPTION     : This function will allocate the memory 
 *   Inputs          : size: Size
 *   Outputs         : NONE
 *   RETURNS         : Allocates the memory.
 ******************************************************************************/

static	void	*s_malloc( 
		unsigned int size 
		)
{
	return malloc( size );
}


/******************************************************************************
 *   FUNCTION NAME   : s_free
 *   DESCRIPTION     : This function will free the memory
 *   Inputs          : m :Which is to be freed
 *   Outputs         : NONE
 *   RETURNS         : Frees the memory.
 ******************************************************************************/


static	void	s_free( 
		void *m 
		)
{
	free(m);
}
/*SPR 21096 Start*/
/****************************************************************************
 * Function Name  : make_addr_str
 * Inputs         : addr : Pointer to Addr                  
 * Outputs        : addr_str :address in string form
 * Returns        : void
 * Description    : converts address array into string form.
 ****************************************************************************/
void make_addr_str(
		UChar8* addr,
		SInt8* addr_str
		)
{
	UChar8 index = OAM_ZERO;
	SInt8 tmp_str[MAX_IP_STR_SIZE];
	for (index = OAM_ZERO; index < OAM_MAX_IPV4_DIGITS; index ++)
	{

        snprintf((char *)tmp_str,MAX_IP_STR_SIZE , "%c",addr[index]); 
		make_int_to_str(addr[index],tmp_str);

		if (index == OAM_ZERO)
		{
			oam_strcpy((char *)addr_str,(const char *)tmp_str);

		}
		else
		{
			strncat((char *)addr_str,(const char *)tmp_str, oam_strlen((char*)tmp_str) );
		}
		if (index != OAM_THREE)
			strncat((char *)addr_str,".", sizeof("."));
	}
}
/******************************************************************************
 *   FUNCTION NAME   : IP_Port_address_update
 *   DESCRIPTION     : This function will convert the serveraddress passed form the oam_send 
 *   Inputs          : m :serveraddress to be populated and entity info
 *   Outputs         : NONE
 *   RETURNS         : The ip address and port populated in serveraddress passed form parent function.
 ******************************************************************************/
static void IP_Port_address_update(struct sockaddr_in *p_serveraddress,rrm_oam_l2_comm_info_t *p_l2_entity_info)
{
    /* CID Fix 115503 +*/
	S8 tmp_addr[16] = {OAM_ZERO};
    /* CID Fix 115503 -*/   
	p_serveraddress->sin_family = AF_INET;
	p_serveraddress->sin_port = htons(p_l2_entity_info->port);
	make_addr_str(p_l2_entity_info->ip_addr, tmp_addr);
	p_serveraddress->sin_addr.s_addr = inet_addr(((char*)tmp_addr));
}
/*SPR 21096 End*/


/******************************************************************************
 *   FUNCTION NAME   : s_newkey
 *   DESCRIPTION     : This function will give new key
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : Gives new key
 ******************************************************************************/


static  void    *s_newkey( 
		void 
		)
{

	pthread_key_t   *key = (pthread_key_t *)s_malloc( sizeof(pthread_key_t) );
	if(OAM_NULL == key)
	{
           OAM_LOG(OAM,OAM_ERROR,"Memory allocation to key failed");
	}
	else{
		oam_memset( key, OAM_ZERO, sizeof(pthread_key_t) );
		pthread_key_create( key, OAM_NULL );
	}
	return (void *)key;
}


/******************************************************************************
 *   FUNCTION NAME   : s_getkey
 *   DESCRIPTION     : This function will give key
 *   Inputs          : key : Pointer to key
 *   Outputs         : NONE
 *   RETURNS         : Gives key
 ******************************************************************************/


static  void    *s_getkey( 
		void *key 
		)
{
	return  pthread_getspecific( *((pthread_key_t *)key) );
}


/******************************************************************************
 *   FUNCTION NAME   : s_setkey
 *   DESCRIPTION     : This function will set key
 *   Inputs          : key : Pointer to key
 *                     value : Pointer to value
 *   Outputs         : NONE
 *   RETURNS         : sets key
 ******************************************************************************/


static  void    s_setkey( 
		void *key, 
		void *value 
		)
{
	pthread_setspecific( *((pthread_key_t *)key), value );
}


/******************************************************************************
 *   FUNCTION NAME   : s_newlock
 *   DESCRIPTION     : This function will give new lock
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : Gives new lock
 ******************************************************************************/

static  void    *s_newlock( 
		void 
		)
{
	pthread_mutex_t *lock = (pthread_mutex_t *)s_malloc( sizeof(pthread_mutex_t) );
	int value;
	if(lock == OAM_NULL)
	{
        OAM_LOG(OAM,OAM_ERROR,"Memory allocation to lock failed");
        }
	else
	{
		oam_memset( lock, OAM_ZERO, sizeof(pthread_mutex_t) );
		value = pthread_mutex_init( lock, OAM_NULL );
		if(value != OAM_ZERO)
            OAM_LOG(OAM,OAM_DETAILED," mutex intialized successfuly:%d",value);
	}
	return  (void *)lock;
}


/******************************************************************************
 *   FUNCTION NAME   : s_lock
 *   DESCRIPTION     : This function is used to lock
 *   Inputs          : lock : Pointer to lock
 *   Outputs         : NONE
 *   RETURNS         : Locks.
 ******************************************************************************/


static  void    s_lock( 
		void *lock 
		)
{
	if(OAM_ZERO == pthread_mutex_lock( (pthread_mutex_t *)lock ))
	{ 
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," s_lock fails ");
	}
}


/******************************************************************************
 *   FUNCTION NAME   : s_unlock
 *   DESCRIPTION     : This function is used to unlock
 *   Inputs          : lock : Pointer to lock
 *   Outputs         : NONE
 *   RETURNS         : Unlocks.
 ******************************************************************************/


static  void    s_unlock( 
		void *lock 
		)
{
	if(OAM_ZERO == pthread_mutex_unlock( (pthread_mutex_t *)lock ))
	{
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," s_unlock Failure");
	}
}


/******************************************************************************
 *   FUNCTION NAME   : s_destroylock
 *   DESCRIPTION     : This function is used to destroy lock
 *   Inputs          : lock : Pointer to lock
 *   Outputs         : NONE
 *   RETURNS         : Destroys lock
 ******************************************************************************/


static  void    s_destroylock( 
		void *lock 
		)
{
	if(OAM_ZERO == pthread_mutex_destroy( (pthread_mutex_t *)lock ))
	{
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," s_destroylock Fails");
	}
	s_free( lock );
}


/******************************************************************************
 *   FUNCTION NAME   : s_vprintf
 *   DESCRIPTION     : This function is used to print
 *   Inputs          : format : Pointer to format
 *                     A : Pointer to A
 *   Outputs         : NONE
 *   RETURNS         : Prints. 
 ******************************************************************************/


static	int	s_vprintf( 
		const char *format, 
		va_list A 
		)
{
	return vfprintf( stderr, format, A );
}


/******************************************************************************
 *   FUNCTION NAME   : s_abort
 *   DESCRIPTION     : This function is used to abort.
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

static	void	s_abort(
		void
		)
{
	abort();
}



/********************** The `Listener' Structure ****************************/
typedef struct
{
	int rx_sd;   /*Receiver discriptor*/
	int tx_sd;   /*Transmiter discriptor*/
} LISTENER;      /*The porting function for UNIX paltform. Structure QSYSOP is initialized with these functions*/


extern const QSYSOP	__os;
const QSYSOP	__os = {
	s_malloc,
	s_free,
	s_newkey,
	s_getkey,
	s_setkey,
	s_newlock,
	s_destroylock,
	s_lock,
	s_unlock,
	s_vprintf,
	s_vprintf,
	s_abort
};

const QSYSOP *os = &__os;

typedef struct __waitdata {
	pthread_cond_t  condition;
	pthread_mutex_t lock;
	int             value;
} __waitdata;


/******************************************************************************
 *   FUNCTION NAME   : s_walltime
 *   DESCRIPTION     : This function is used to get walltime
 *   Inputs          : now : Pointer to now
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

static  void    s_walltime( 
		QTIME *now 
		)
{
	struct timeval  T;
#ifdef OAM_DEBUG
     /* SPR_17377_Warning_fix + */
    OAM_LOG(OAM,OAM_INFO,"Thread Id :[%lu]",pthread_self());
    /* SPR_17377_Warning_fix - */
#endif

	OAM_GETTIMEOFDAY( &T, OAM_NULL );
	now->s = T.tv_sec;
	now->us = T.tv_usec;
}



/******************************************************************************
 *   FUNCTION NAME   : s_newchannel
 *   DESCRIPTION     : This function is used to get new channel
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

static  void    *s_newchannel( 
		void 
		)
{
	__waitdata      *w = (__waitdata *)s_malloc( sizeof(__waitdata) );
	int value = OAM_ZERO;
	if(w == OAM_NULL)
	{
		OAM_LOG(OAM,OAM_ERROR,"Memory allocation to waitdata failed");
	}
	else
	{
		oam_memset( w, OAM_ZERO, sizeof(__waitdata) );
		value = pthread_mutex_init( &(w->lock), OAM_NULL );
		if(value != OAM_ZERO){
			OAM_LOG(OAM,OAM_INFO,"Mutex initialized successfuly:%d",value);
		}
		w->value = OAM_ZERO;
		value = pthread_cond_init(&(w->condition), OAM_NULL);
		if(value != OAM_ZERO){
			OAM_LOG(OAM,OAM_INFO,"condition initialized successfuly:%d",value);
		}
	}

	return (void *)w;
}


/******************************************************************************
 *   FUNCTION NAME   : s_sleep
 *   DESCRIPTION     : This function is used for sleep
 *   Inputs          : channel : Pointer to channel
 *                     timeout : Pointer to timeout
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/


static  void    s_sleep( 
		void *channel, 
		const QTIME *timeout 
		)
{
	struct timespec T;
	__waitdata      *w = (__waitdata *)channel;

#ifdef OAM_DEBUG
	OAM_LOG(OAM,OAM_INFO,"Thread Id :[%u]",pthread_self());
#endif

	if(OAM_ZERO == pthread_mutex_lock( &(w->lock) ))
	{
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_DETAILED,"Successfully get mutex for s_sleep");
#endif
	}
	if( !w->value ) {
		if( !timeout ) {
			if(OAM_ZERO == pthread_cond_wait( &(w->condition), &(w->lock) ))
			{
#ifdef OAM_DEBUG
                OAM_LOG(OAM,OAM_DETAILED,"conditional wait Success");
#endif
			}
		} else if ( timeout->s || timeout->us ) {
			struct timeval  _T;

			OAM_GETTIMEOFDAY( &_T, OAM_NULL );

			_T.tv_sec += timeout->s;
			_T.tv_usec += timeout->us;
			while( _T.tv_usec >= 1000000L ) {
				_T.tv_usec -= 1000000L;
				_T.tv_sec++;
			}

			T.tv_sec = _T.tv_sec;
			T.tv_nsec = OAM_MSEC_IN_ONE_SEC * _T.tv_usec;
			if(OAM_ZERO == pthread_cond_timedwait( &(w->condition), &(w->lock), &T ))
			{
#ifdef OAM_DEBUG
                OAM_LOG(OAM,OAM_INFO,"cond timedwait Success ");
#endif
			}
		} else {
			/** Nothing -- zero timeout was specified **/
		}
	}
	w->value = OAM_ZERO;
	if(OAM_ZERO == pthread_mutex_unlock( &(w->lock) ))
	{
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_DETAILED,"mutex unlock Success");
#endif
	}
}


/******************************************************************************
 *   FUNCTION NAME   : s_wakeup
 *   DESCRIPTION     : This function is used for wakeup
 *   Inputs          : channel : Pointer to channel
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

static  void    s_wakeup( 
		void *channel 
		)
{
	__waitdata      *w = (__waitdata *)channel;

	if(OAM_ZERO == pthread_mutex_lock( &(w->lock) ))
	{
#ifdef OAM_DEBUG
#endif
	}
	w->value = OAM_ONE;
	pthread_cond_signal( &(w->condition) );
	if(OAM_ZERO == pthread_mutex_unlock( &(w->lock) ))
	{
#ifdef OAM_DEBUG
#endif
	}

}

const QWAIT __waitstruct = {
	s_newchannel,
	s_sleep,
	s_wakeup,
	s_walltime
};


const QWAIT *waitstruct = &__waitstruct;


/****************************************************************************
 * Function Name  : oam_makeserver_udp
 * Inputs         : self : Pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket and bind it.
 ****************************************************************************/
static int makeserver_udp( 
		const struct sockaddr_in *self 
		)
{
	int sd; /*socket descriptor*/
	if( (sd = socket(AF_INET, SOCK_DGRAM, 0 )) < OAM_ZERO )
	{
		oam_perror("socket");
       s_abort();  
	}

	if( OAM_BIND (sd, (struct sockaddr *)self, sizeof(*self) ) < OAM_ZERO )
	{
       s_abort();
	}

	return sd;
}

/****************************************************************************
 * Function Name  : oam_makeclient_udp
 * Inputs         : NONE
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket.
 ****************************************************************************/
static int makeclient_udp(
		void
		)
{
	int sd; /*socket descriptor*/

	if( (sd = socket(AF_INET, SOCK_DGRAM, 0 )) < OAM_ZERO )
	{
		oam_perror("socket");
		s_abort();

	}

	return sd;
}


/****************************************************************************
 * Function Name  : oam_timedwait
 * Inputs         : S : Pointer to S
 *                  T : Pointer to QTIME
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function waits until either the listner the object is 
 ready for read, or until the specified    timed out has 
 occured.
 ** ****************************************************************************/

static  void    oam_timedwait( void *S, const QTIME *T ) /*This function waits until either the listner the object is ready for read, or until the specified    timed out has occured */
{
	LISTENER        *u = (LISTENER *)S;
	struct timeval  timeout;
	int     n;
	fd_set  F;

	FD_ZERO( &F );
	FD_SET( u->rx_sd, &F );

	if( T )
	{
		timeout.tv_sec = T->s;
		timeout.tv_usec = T->us;
	}
	n = select(  FD_SETSIZE - OAM_ONE, &F, OAM_ZERO, OAM_ZERO, T?&timeout:OAM_ZERO );

	if( n < OAM_ZERO ) {
		oam_perror("select");
	}
}


/****************************************************************************
 * Function Name  : oam_open
 * Inputs         : data : Pointer to data
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is used to open port.
 ******************************************************************************/
static void *oam_open( 
		void *data 
		)
{
	UInt16 oam_port    = OAM_ZERO;
	UInt16 sdm_port    = OAM_ZERO; 
	oam_cli_comm_info_t comm_info;
	oam_sockaddr_ipfv4 oam_address;
        /* Spr 12880 Fix Start*/
        UInt8 CellId = OAM_INVALID_CELL_ID;
        /* Spr 12880 Fix End*/
	LISTENER *u = (LISTENER *)qvSysMalloc(sizeof(LISTENER)); 
	if(u == OAM_NULL)
	{
		OAM_LOG(OAM, OAM_ERROR," qvSysMalloc fail in oam_open");
		oam_reset_reason_code(OAM_INTERNAL_FAILURE);
		oam_abort();
		return OAM_NULL;
	}
	oam_memset(&comm_info, 0, sizeof(oam_cli_comm_info_t));
	oam_get_cli_communication_info(&comm_info);

	get_config_info();
	oam_update_enodeb_product_info();
	/* Receive Socket of OAM */
	oam_memset( &oam_address, OAM_ZERO, sizeof(oam_address) );
	oam_address.sin_family      = AF_INET;

    /* SPR 17777 fix */
	OAM_LOG(OAM, OAM_DETAILED,"OAM IP: %s , unused parameter %p ",comm_data.oam_com_params.ipaddr, data);

	oam_address.sin_addr.s_addr = OAM_INET_ADDR(
			(const char *) comm_data.oam_com_params.ipaddr);
	oam_port = oam_atoi((const char *) comm_data.oam_com_params.port);

	OAM_LOG(OAM, OAM_DETAILED,"OAM PORT: %d ",oam_port);

	oam_address.sin_port = oam_htons(oam_port);
	u->rx_sd = makeserver_udp( &oam_address );

    if ( OAM_ZERO > u->rx_sd )
    {
        OAM_LOG(OAM, OAM_ERROR,"Unable To Open Receiver Descriptor Of OAM");
        oam_reset_reason_code(OAM_INTERNAL_FAILURE);
        oam_abort();
    }

	/* Send Socket of OAM */
	u->tx_sd = makeclient_udp();
    if ( OAM_ZERO > u->tx_sd )
    {
        OAM_LOG(OAM, OAM_ERROR,"Unable To Open Sender Descriptor Of OAM");
        oam_reset_reason_code(OAM_INTERNAL_FAILURE);
         oam_abort();
    }
	/* For RRM */
	OAM_LOG(OAM, OAM_DETAILED,"RRM ip addr  ======%s. RRM port  ======%d",
        comm_info.rrm_ip_addr, comm_info.rrm_port);

	oam_memset( &rrm_address, OAM_ZERO, sizeof(rrm_address) );
	rrm_address.sin_family = AF_INET;
	rrm_address.sin_port = oam_htons(comm_info.rrm_port);
	rrm_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.rrm_ip_addr);

#ifdef OAM_SON_ENABLE
	/* For SON */
	OAM_LOG(OAM, OAM_DETAILED,"SON ip addr  ======%s. SON port  ======%d",
        comm_info.son_ip_addr, comm_info.son_port);

	oam_memset( &son_address, OAM_ZERO, sizeof(son_address) );
	son_address.sin_family      = AF_INET;
	son_address.sin_port = oam_htons(comm_info.son_port);
	son_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.son_ip_addr);
#endif
	/* For RRC */
	OAM_LOG(OAM, OAM_DETAILED,"L3 ip addr  ======%s. L3 port  ======%d",
        comm_info.l3_ip_addr, comm_info.l3_port);

	oam_memset( &l3_address, OAM_ZERO, sizeof(l3_address) );
	l3_address.sin_family = AF_INET;
	l3_address.sin_port = oam_htons(comm_info.l3_port);
	l3_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.l3_ip_addr);

/* SPR 21882 start */
/* Code removed */
/* SPR 21882 end */

/* SPR 20908 Start */
/* code removed */
/* SPR 20908 End */
	
/*SPR 21889 +- */

	OAM_LOG(OAM, OAM_DETAILED,"CLI ip addr  ======%s. CLI port  ======%d",
        comm_info.cli_ip_addr, comm_info.cli_port);

	oam_memset( &qcli_address, OAM_ZERO, sizeof(qcli_address) );
	qcli_address.sin_family      = AF_INET;
	qcli_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.cli_ip_addr);
	qcli_address.sin_port = oam_htons(comm_info.cli_port);

	OAM_LOG(OAM, OAM_DETAILED,"TR069 ip addr  ======%s. TR069port  ======%d",
        comm_info.tr069_ip_addr, comm_info.tr069_port);

	oam_memset( &tr069_address, OAM_ZERO, sizeof(tr069_address) );
	tr069_address.sin_family      = AF_INET;
	tr069_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.tr069_ip_addr);
	tr069_address.sin_port = oam_htons(comm_info.tr069_port);

	oam_memset( &sdm_address, OAM_ZERO, sizeof(sdm_address) ); 
	sdm_address.sin_family      = AF_INET;
	OAM_LOG(OAM, OAM_DETAILED,"SDM IP: %s ",comm_data.sdm_com_params.ipaddr);
	sdm_address.sin_addr.s_addr = OAM_INET_ADDR((const char *) comm_data.sdm_com_params.ipaddr);
	sdm_port = oam_atoi((const char *) comm_data.sdm_com_params.port);
	sdm_address.sin_port = oam_htons(sdm_port);

	oam_memset( &snmp_address, OAM_ZERO, sizeof(snmp_address) );
	snmp_address.sin_family      = AF_INET;
	snmp_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.snmp_ip_addr);
	snmp_address.sin_port = oam_htons(comm_info.snmp_port);

#ifdef OAM_RADIO_HEAD_SUPPORT
	if(oam_prov_req.rrh_flag && oam_prov_req.rcm_flag) {
		oam_memset(&rrh_address, OAM_ZERO, sizeof(rrh_address));
		rrh_address.sin_family      = AF_INET;
		rrh_address.sin_addr.s_addr = OAM_INET_ADDR(comm_info.rrh_ip_addr);
		rrh_address.sin_port = oam_htons(comm_info.rrh_port);	
	}
#endif

	if (oam_prov_req.fm_cntxt.isRaiseConfigAlarm)
	{
        OAM_ALARM(CRITICAL_CONFIGURATION_FAILURE_ALARM_ID, CRITICAL,
                  NOTIFY_NEW_ALARM,
                  PROCESSING_ERROR_ALARM, UNAVAILABLE, EXPEDITED_ALARM,
        /* Spr 12880 Fix Start*/
                  "Startup Configuration Failure", OAM_NULL,CellId);
        /* Spr 12880 Fix End*/
		oam_abort();
	}
	return u;
}





/*SPR 21096 +-*/
/****************************************************************************
 * Function Name  : make_int_to_str
 * Inputs         : value : Value
 * Outputs        : addr_str : Pointer to address as string
 * Returns        : void
 * Description    : converts address array into string form.
 ****************************************************************************/
void make_int_to_str(
		ULong32 value,
		SInt8* addr_str
		)
{
	SInt8    index           = OAM_ZERO;      /*Table Index*/
	SInt8    index2          = OAM_ZERO;
	UChar8    remain          = OAM_ZERO;
	SInt8    tmp_str[10];

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
	}while(value != OAM_ZERO);

	index--;

	while (index >= OAM_ZERO)
	{
		addr_str[index2] =  tmp_str[index];
		index--;
		index2++;
	}
	addr_str[index2] = '\0';
}


/******************************************************************************
 *   FUNCTION NAME   : oam_receive
 *   DESCRIPTION     : This function is used to receive a single incoming 
 message in to a message buffer and returns it
 *   Inputs          : S : Pointer to S
 *                     sender : Pointer to sender
 *                     receiver : Pointer to receiver
 *                     priority : Pointer to priority
 *   Outputs         : NONE
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

static    void *oam_receive( 
		void *S,
		QMODULE        *sender,
		QMODULE      *receiver,
		signed char *priority ) /*This function receive a single incoming message in to a message buffer and returns it. This function determines the intended receiver, the sender and the priority of the message*/
{

    /*Coverity_CID_58308_FIX_START*/
    /* Code Removed */
    /*Coverity_CID_58308_FIX_END*/
    /* SPR 20908 Start */
    struct sockaddr_in         cli_addr = {OAM_ZERO};
    socklen_t                  length = OAM_ZERO;
    /* SPR 20908 End */
	fd_set                F;
	S32                 n;
	U32                 bytes_read;
	char                 *msg_buf   = OAM_NULL;
	U16                 src_id     = OAM_ZERO;
    U16                 dst_id     = OAM_ZERO;
	U16                 api_id     = OAM_ZERO;
	U16                 msg_size   = OAM_ZERO;
	struct timeval      zero;
	LISTENER           *u = (LISTENER *)S;
    UInt8 received_instance_idx = OAM_ZERO;

	FD_ZERO( &F );
	FD_SET( u->rx_sd, &F );

	zero.tv_sec  = OAM_ZERO;
	zero.tv_usec = OAM_ZERO;
	n = select( FD_SETSIZE - OAM_ONE, &F, OAM_ZERO, OAM_ZERO, &zero );

	if( n < OAM_ZERO )
	{
		oam_perror("select");
	}

	if( FD_ISSET( u->rx_sd, &F) )
	{
        /*Coverity_CID_58308_FIX_START*/
        /* SPR 20908 Start */
        length = sizeof(struct sockaddr);
        bytes_read = OAM_RECVFROM(u->rx_sd, (char *)g_receive_buffer, OAM_MAX_RECV_BUF_SIZE, OAM_ZERO, (struct sockaddr *)&cli_addr, &length);  
        /* SPR 20908 End */
        /*Coverity_CID_58308_FIX_END*/
        if (!bytes_read)
		{
			return OAM_NULL;
		}
		else
		{
			/* Parse header */
            /*Coverity_CID_58308_FIX_START*/
            src_id      = get_word_from_header((UInt8*)g_receive_buffer +
                                                OAM_INTF_HDR_SRC_ID_OFFSET);
            dst_id      = get_word_from_header((UInt8*)g_receive_buffer +
                                                OAM_INTF_HDR_DST_ID_OFFSET);
            api_id      = get_word_from_header((UInt8*)g_receive_buffer +
                                                OAM_EXT_API_ID_FROM_HEADER);
            msg_size    = get_word_from_header((UInt8*)g_receive_buffer +
                                                OAM_INTF_HDR_MSG_SIZE_OFFSET);
            /*Coverity_CID_58308_FIX_END*/

			if(bytes_read < msg_size)
			{
                /* SPR_17377_Warning_fix + */
				OAM_LOG(OAM, OAM_WARNING, "Corrupted msg received due to insufficient number of bytes read, msg_size = %d, bytes_read = %u", msg_size, bytes_read);
                /* SPR_17377_Warning_fix - */
				return OAM_NULL;
			}    
/* SPR 10969 FIx Start */
            /* SPR_17377_Warning_fix + */
            /* SPR 17777 fix */
            OAM_LOG(OAM, OAM_INFO,
			    "Received API[%u] of length[%u] from Module[%u] to Module[%u]", api_id, msg_size, src_id, dst_id);
            /* SPR_17377_Warning_fix - */
/* SPR 10969 FIx End */
            if((src_id == PDCP_MODULE_ID) && msg_size == OAM_ZERO)  
			{
				OAM_LOG(OAM,OAM_WARNING,"Forcefully updating Msg size for PDCP");
				msg_size += 10;
			}
            else if((src_id == RLC_MODULE_ID) && (api_id == 203))   
			{
				OAM_LOG(OAM,OAM_WARNING,"Forcefully updating Msg size for RLC");
				msg_size += 2;
			}
            /* SPR 20908 Start */
                received_instance_idx = *((UInt8*)(g_receive_buffer) + OAM_INTF_HDR_CELL_IDX_OFFSET);
            if(received_instance_idx < OAM_MAX_NUM_L2_INSTANCE)
                {
                    switch(src_id)
                    {
                        case MAC_MODULE_ID:
                        if (api_id == MAC_INIT_LAYER_IND)
                        {
                        memset(l2_comm_info[received_instance_idx].mac_comm_info.ip_addr, 
                                OAM_ZERO, sizeof(l2_comm_info[received_instance_idx].mac_comm_info.ip_addr));
                        /* SPR 21371 Start */
                        l2_comm_info[received_instance_idx].mac_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                        l2_comm_info[received_instance_idx].phy_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                        change_address_format((U8 *)inet_ntoa(cli_addr.sin_addr), 
                                l2_comm_info[received_instance_idx].mac_comm_info.ip_addr);
                        /* SPR 21371 End */
                        l2_comm_info[received_instance_idx].mac_comm_info.port = ntohs(cli_addr.sin_port);
                        l2_comm_info[received_instance_idx].phy_comm_info.ip_addr[0] = 
                            l2_comm_info[received_instance_idx].mac_comm_info.ip_addr[0];
                        l2_comm_info[received_instance_idx].phy_comm_info.ip_addr[1] = 
                            l2_comm_info[received_instance_idx].mac_comm_info.ip_addr[1];
                        l2_comm_info[received_instance_idx].phy_comm_info.ip_addr[2] = 
                            l2_comm_info[received_instance_idx].mac_comm_info.ip_addr[2];
                        l2_comm_info[received_instance_idx].phy_comm_info.ip_addr[3] = 
                            l2_comm_info[received_instance_idx].mac_comm_info.ip_addr[3];
                        l2_comm_info[received_instance_idx].phy_comm_info.port = 
                            l2_comm_info[received_instance_idx].mac_comm_info.port;
                        OAM_LOG(OAM,OAM_INFO,"MAC port=%d",l2_comm_info[received_instance_idx].mac_comm_info.port);
                        }
                        break;
                        case RLC_MODULE_ID:
                        if (api_id == RLC_INIT_LAYER_IND)
                        {
                        memset(l2_comm_info[received_instance_idx].rlc_comm_info.ip_addr, 
                                OAM_ZERO, sizeof(l2_comm_info[received_instance_idx].rlc_comm_info.ip_addr));
                        /* SPR 21371 Start */
                        l2_comm_info[received_instance_idx].rlc_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                        change_address_format((U8 *)inet_ntoa(cli_addr.sin_addr), 
                                l2_comm_info[received_instance_idx].rlc_comm_info.ip_addr);
                        /* SPR 21371 End */
                        l2_comm_info[received_instance_idx].rlc_comm_info.port = ntohs(cli_addr.sin_port);
                        OAM_LOG(OAM,OAM_INFO,"RLC port=%d",l2_comm_info[received_instance_idx].rlc_comm_info.port);
			}
                        break;
                        case PDCP_MODULE_ID:
                        if (api_id == PDCP_INIT_LAYER_IND)
                        {
                        memset(l2_comm_info[received_instance_idx].pdcp_comm_info.ip_addr, 
                                OAM_ZERO, sizeof(l2_comm_info[received_instance_idx].pdcp_comm_info.ip_addr));
                        /* SPR 21371 Start */
                        l2_comm_info[received_instance_idx].pdcp_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                        change_address_format((U8 *)inet_ntoa(cli_addr.sin_addr), 
                                l2_comm_info[received_instance_idx].pdcp_comm_info.ip_addr);
                        /* SPR 21371 End */
                        l2_comm_info[received_instance_idx].pdcp_comm_info.port = ntohs(cli_addr.sin_port);
				        
                        /** Initialising Data_prt as it is not being used **/
                        OAM_LOG(OAM,OAM_INFO,"PDCP port=%d",l2_comm_info[received_instance_idx].pdcp_comm_info.port);
                        }
                        break;
                        case EGTPU_MODULE_ID:
			if (api_id == S1U_INIT_LAYER_IND)
                        {
                        memset(l2_comm_info[received_instance_idx].egtpu_comm_info.ip_addr, 
                                OAM_ZERO, sizeof(l2_comm_info[received_instance_idx].egtpu_comm_info.ip_addr));
                        /* SPR 21371 Start */
                        l2_comm_info[received_instance_idx].egtpu_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                        change_address_format((U8 *)inet_ntoa(cli_addr.sin_addr), 
                                l2_comm_info[received_instance_idx].egtpu_comm_info.ip_addr);
                        /* SPR 21371 End */
                        l2_comm_info[received_instance_idx].egtpu_comm_info.port = ntohs(cli_addr.sin_port);
                        OAM_LOG(OAM,OAM_INFO,"EGTPU port=%d",l2_comm_info[received_instance_idx].egtpu_comm_info.port);
                        }
                        break;

                        /*SPR 21889 Start*/
                    case OAM_AGNT_MODULE_ID:
                        {
                            memset(l2_comm_info[received_instance_idx].agnt_comm_info.ip_addr, 
                                    OAM_ZERO, sizeof(l2_comm_info[received_instance_idx].agnt_comm_info.ip_addr));
                            l2_comm_info[received_instance_idx].agnt_comm_info.bitmask = RRM_OAM_IPV4_ADDRESS_PRESENT;
                            change_address_format((U8 *)inet_ntoa(cli_addr.sin_addr), 
                                    l2_comm_info[received_instance_idx].agnt_comm_info.ip_addr);
                            l2_comm_info[received_instance_idx].agnt_comm_info.port = ntohs(cli_addr.sin_port);
                    }
                        break;
                        /*SPR 21889 End */
                }
            }
            /* SPR 20908 End */
			/* Fault Management change START */

			/* Lower layers send destination Id as OAM_MODULE_ID ,to redirect thread to FM thread destination ID is set as FM_MODULE_ID */
			if ((api_id == OAM_ALARM_NOTIFICATION) ||
#ifdef OAM_SON_ENABLE
                 ((src_id == SON_MODULE_ID) &&
                  (api_id == SON_OAM_EVENT_NOTIFICATION)) ||
#endif
                ((src_id == RRM_MODULE_ID) &&
                 (api_id == RRM_OAM_EVENT_NOTIFICATION)) ||
                ((api_id == MAC_PHY_OAM_ERROR_IND)&&
                  (src_id == MAC_MODULE_ID))   ||
                  /* SPR 13765 changes start */
                ((src_id == EGTPU_MODULE_ID) &&
                 (api_id == PR_GTPU_ERROR_IND)))
                  /* SPR 13765 changes end */
			{
				dst_id = FM_MODULE_ID;
			}

			/* Fault Management change END */

			*sender     = qvGetService(src_id);
			*receiver   = qvGetService(dst_id);
            if(msg_size > OAM_ZERO)  
			{
				msg_buf = qvMsgAlloc(OAM_NULL, OAM_ZERO, OAM_ZERO, msg_size);
				if (msg_buf == OAM_NULL)
				{
                    OAM_LOG(OAM,OAM_ERROR,"Memory allocation to msg_bug failed");
				}
				else
				{
                    /*Coverity_CID_58308_FIX_START*/
					oam_memcpy(msg_buf ,g_receive_buffer, msg_size );
                    /*Coverity_CID_58308_FIX_END*/
				}
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Msg size is Zero. Source Id=[%d], API Id=[%d]",
                        src_id, api_id);
			}
		}
	}
	return msg_buf;
}
/****************************************************************************
 * Function Name  : oam_send
 * Inputs         : S : Pointer to S
 *                  message : Pointer to message
 *                  sender : Pointer to sender
 *                  receiver : Pointer to receiver
 *                  data : Pointer to data
 *                  size : Size
 * Outputs        : None
 * Returns        : void
 * Description    : ENCAPS functionality to send message out of OAM
 ****************************************************************************/

static    void    oam_send( void *S,
		const void *message,
		QMODULE      sender,
		QMODULE    receiver,
		void          *data,
		unsigned int  size ) /*This function sends the message to the appropriate destination after filling the Shell header */
{


	LISTENER      *u = (LISTENER *)S;
	U16            dst_id      = OAM_ZERO;
	U16            api_id      = OAM_ZERO;
	U16            api_size    = OAM_ZERO;
	S8             ret_val     = OAM_ZERO; /* Coverity 71354, 71356 Fix */
    /* SPR 20908 Start */
    UInt16 cell_index = OAM_ZERO;
    UInt8 instance_id = OAM_ZERO;
    struct  sockaddr_in serveraddress;
    /*SPR 21096 +-*/
    /* SPR 20908 End */

    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_DETAILEDALL, " unused parameter %d, %p, %p and %p",size, data, receiver, sender);
    /* SPR 20908 Start */
    RRM_MEMSET( &serveraddress, 0, sizeof(serveraddress) );
    /* SPR 20908 End */
	message += OAM_CSPL_HDR_SIZE;
    dst_id = get_word_from_header((UInt8*)message +
                                  OAM_INTF_HDR_DST_ID_OFFSET);
    api_id = get_word_from_header((UInt8*)message +
                                  OAM_EXT_API_ID_FROM_HEADER);
    api_size = get_word_from_header((UInt8*)message +
                                  OAM_INTF_HDR_MSG_SIZE_OFFSET);
    /* SPR 20908 Start */
    cell_index = get_word_from_header((UInt8*)message + 
                                  OAM_INTF_HDR_CELL_IDX_OFFSET);
    /*SPR 21096 Start*/
    cell_index &= 0xFF00;
    cell_index >>= 8; 
    ret_val = find_l2_instance_id_from_cell_index(cell_index, &instance_id); 
    /*SPR 21096 End*/
    OAM_LOG(OAM,OAM_DETAILED, " cell_index %d, instance_id %d",cell_index, instance_id);
    /* SPR 20908 End */
	OAM_LOG(OAM,OAM_INFO,"dst_mod_id = %d",dst_id);
	if((dst_id==QCLI_MODULE_ID)||(dst_id==OAM_QCLI_TRAP_MODULE_ID))
	{
		ret_val =  sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&qcli_address,sizeof(qcli_address));
	}
#ifdef OAM_SON_ENABLE
	else if(dst_id==SON_MODULE_ID)
	{
		ret_val =	sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&son_address,sizeof(son_address));
	}
#endif
	else if(dst_id==RRM_MODULE_ID)
	{
		ret_val =	sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&rrm_address,sizeof(rrm_address));
	}
	else if(dst_id==RRC_MODULE_ID)
	{
		ret_val =   sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&l3_address,sizeof(l3_address));
	}
	else if(dst_id==PDCP_MODULE_ID)
	{
    /* SPR 20908 Start */
       /*SPR 21096 Start*/
       IP_Port_address_update(&serveraddress, &l2_comm_info[instance_id].pdcp_comm_info);
       /*SPR 21096 End*/
        ret_val =     sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    /* SPR 20908 End */
	}
	else if(dst_id==RLC_MODULE_ID)
	{
    /* SPR 20908 Start */
       /*SPR 21096 Start*/
       IP_Port_address_update(&serveraddress, &l2_comm_info[instance_id].rlc_comm_info);
       /*SPR 21096 End*/
        ret_val =      sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    /* SPR 20908 End */
	}
	else if(dst_id==MAC_MODULE_ID)
	{
    /* SPR 20908 Start */
       /*SPR 21096 Start*/
       IP_Port_address_update(&serveraddress, &l2_comm_info[instance_id].mac_comm_info);
       /*SPR 21096 End*/
       ret_val =   sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)& serveraddress,sizeof(serveraddress));
    /* SPR 20908 End */
	}
	else if(dst_id==EGTPU_MODULE_ID)
	{
    /* SPR 20908 Start */
       /*SPR 21096 Start*/
       IP_Port_address_update(&serveraddress, &l2_comm_info[instance_id].egtpu_comm_info);
       /*SPR 21096 End*/
        ret_val =   sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
    /* SPR 20908 End */
	}
	else if(dst_id==TR069_MODULE_ID)
	{
    /* SPR 20908 Start */
		ret_val =      sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&tr069_address,sizeof(tr069_address));
    /* SPR 20908 End */
	}
        /*SPR 14214 START*/
        /*moved the if condition to end of else */
        /*SPR 14214 END*/
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	else if(oam_prov_req.rrh_flag && oam_prov_req.rcm_flag &&
			dst_id == RCM_MODULE_ID) {
		sendto(u->tx_sd, message, api_size, OAM_ZERO,
				(struct sockaddr*)&rrh_address, sizeof(rrh_address));
	}
#endif
#endif
	else if(dst_id==OAM_SDM_MODULE_ID)
	{
        /* +- SPR 15840 */
		ret_val = sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&sdm_address,sizeof(sdm_address));
	}
	else if(dst_id==OAM_AGNT_MODULE_ID)
	{
        /* +- SPR 15840 */
		/*SPR 21889 Start*/
        /* Instance Id is saved at cell index position in header */
        instance_id = cell_index;
		IP_Port_address_update(&serveraddress, &l2_comm_info[instance_id].agnt_comm_info);
		ret_val = sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
		/*SPR 21889 End */
	}
	else if((dst_id==OAM_SNMP_MODULE_ID) || (dst_id==OAM_SNMP_INTF_MODULE_ID))
	{
		ret_val = sendto(u->tx_sd,message,api_size,OAM_ZERO,(struct sockaddr*)&snmp_address,sizeof(snmp_address));
	}
    else
    {
        /*SPR 14214 START*/
        OAM_LOG(OAM, OAM_WARNING,"Invalid Destination[dest id=%d]", dst_id);
        /*SPR 14214 END*/
    }
    /*SPR 14214 START*/
    if(ret_val != OAM_INVALID_RETURN)
    {
        OAM_LOG(OAM, OAM_DETAILED, "Sending successfully ");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR, "Error Sending message to dest id: %d",dst_id);
    }
    /*SPR 14214 END*/    
#ifdef OAM_SON_ENABLE
	if(api_id==SON_OAM_CELL_INFO_IND)
		sleep(1);
#endif
	/* SPR 21889 +- */
}

/****************************************************************************
 * Function Name  : oam_close
 * Inputs         : data : Pointer to data
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for close
 ****************************************************************************/
static void oam_close( 
		void *data ) /*This function cleans up the IPC listner structure*/
{
	LISTENER    *L = (LISTENER *)data;


	OAM_CLOSE( L->tx_sd );
	OAM_CLOSE( L->rx_sd );
	qvSysFree( data );
}

const QSHELL __operations = {
	oam_timedwait,
	oam_send,
	oam_receive,
	oam_open,
	oam_close,
	0
};
const QSHELL *operations = &__operations;

/******************************************************************************
 *   FUNCTION NAME   : oam_port_process_shutdown
 *   DESCRIPTION     : This function will shutdown of oam process
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

void oam_port_process_shutdown()
{
	return;
}

static size_t oam_curl_trasfer_send(void *buffer, size_t size, size_t nmemb, void *p_stream)
{
    return oam_fread(buffer, size, nmemb, p_stream);
}

static size_t oam_curl_transfer_rcv(void *buffer, size_t size, size_t nmemb, void *p_stream)
{
    return oam_fwrite(buffer, size, nmemb, p_stream);
}

/*SPR 21889 Start*/
/******************************************************************************
 *   FUNCTION NAME   : oam_transfer_file
 *   DESCRIPTION     : This function will transfer binary fron master to agent 
 *   Inputs          : *p_filename, instance, is_file_pull
 *   Outputs         : NONE
 *   RETURNS         : SUCCESS/FAILURE
 ******************************************************************************/
oam_return_et
oam_transfer_file(Char8 *p_filename,
        UInt8 instance,
        oam_bool_et is_file_pull)
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    Char8 ftp_url[OAM_FTP_URL_NAME_SIZE] = {OAM_ZERO};
    oam_return_et retVal = OAM_FAILURE;
    Char8 ip[MAX_IP_V6_STR_SIZE];
    struct  sockaddr_in trsfer_addr;
    FILE    *fptr = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();
    if ((OAM_NULL != p_filename) && (OAM_ZERO != oam_strlen(p_filename)))
    {
        if (l2_comm_info[instance].agnt_comm_info.bitmask & RRM_OAM_IPV4_ADDRESS_PRESENT)
        {
            ip[OAM_ZERO] = '\0';
            IP_Port_address_update(&trsfer_addr, &l2_comm_info[instance].agnt_comm_info);
            inet_ntop(AF_INET, &trsfer_addr.sin_addr.s_addr, ip, sizeof(ip));
        }


        OAM_CURL_GLOBAL_INIT(CURL_GLOBAL_DEFAULT);

        curl = OAM_CURL_EASY_INIT();
        if(curl)
        {
            if (is_file_pull)
            {
                oam_snprintf(ftp_url, OAM_FTP_URL_NAME_SIZE, "sftp://%s/%s/%s",
                    ip, oam_prov_req.l2_setup_info.instance_info[instance].oam_l2_req.oam_agnt_info.bin_path,
                    p_filename);
                fptr = oam_fopen(p_filename,"wb");
                if (OAM_NULL != fptr)
                {
                    OAM_CURL_EASY_SETOPT(curl, CURLOPT_WRITEFUNCTION, oam_curl_transfer_rcv);
                    OAM_CURL_EASY_SETOPT(curl, CURLOPT_WRITEDATA, fptr);
                }
                else
                {
                    OAM_LOG(OAM, OAM_ERROR,"Failed to open file %s. Error code %u", p_filename, errno);	  
                    return OAM_FAILURE;
                }
            }
            else
            {
                oam_snprintf(ftp_url, OAM_FTP_URL_NAME_SIZE, "sftp://%s/%s/%s%s",
                    ip, oam_prov_req.l2_setup_info.instance_info[instance].oam_l2_req.oam_agnt_info.bin_path,
                    p_filename, OAM_BIN_SOFT_UPGRADE_EXT_STR);
                fptr = oam_fopen(p_filename,"rb");
                if (OAM_NULL != fptr)
                {
                    OAM_CURL_EASY_SETOPT(curl, CURLOPT_READFUNCTION, oam_curl_trasfer_send);
                    OAM_CURL_EASY_SETOPT(curl, CURLOPT_UPLOAD, 1L);
                    OAM_CURL_EASY_SETOPT(curl, CURLOPT_READDATA, fptr);
                }
                else
                {
                    OAM_LOG(OAM, OAM_ERROR,"Failed to open file %s. Error code %u", p_filename, errno);	  
                    return OAM_FAILURE;
                }
            }

            OAM_LOG(OAM, OAM_INFO,"Transfer %s", ftp_url);	  
            OAM_CURL_EASY_SETOPT(curl, CURLOPT_URL, ftp_url);
            OAM_CURL_EASY_SETOPT(curl, CURLOPT_USERNAME , g_oam_sim_t.user_name);
            OAM_CURL_EASY_SETOPT(curl, CURLOPT_PASSWORD , g_oam_sim_t.password);

            OAM_CURL_EASY_SETOPT(curl, CURLOPT_VERBOSE, 1L);


            res = OAM_CURL_EASY_PERFORM(curl);
            if(CURLE_OK != res)
            {
                OAM_LOG(OAM, OAM_ERROR, "Failed to transfer file %s. Error:%s",  
                        p_filename, OAM_CURL_EASY_STRERROR(res));	  
                retVal = OAM_FAILURE;
            }
            else
            {
                retVal = OAM_SUCCESS;
            }
            OAM_CURL_EASY_CLEANUP(curl);

            oam_fclose(fptr);
        }

    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR, "Invalid file name received for transfer");
        retVal = OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}

/*SPR 21889 End */
