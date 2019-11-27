/******************************************************************************
*   FILE NAME    : oam_snmp_port_main.c
*
*   DESCRIPTION  : SNMP modules 
*
*       DATE            AUTHOR          REFERENCE       REASON
*   25 July 2013   Purshottam Gupta     ---------       Initial
*   May 2014       Shalu Sagar          Klockwork Fix
*
*   Copyright (c) 2013, Aricent Inc. All Rights Reserved
******************************************************************************/
/*******************************************************************************
*                        Header Files
*******************************************************************************/
#include        <cspl.h>
#include	<oam_snmp_main.h>
#include 	<oam_snmp_interface.h>
#include    <signal.h>
/*******************************************************************************
*                        GLOBEL VARIBLES
*******************************************************************************/
static struct sockaddr_in oam_address;
static struct sockaddr_in snmp_address;
extern void *channel;
extern const QWAIT* waitstruct;
extern int wakeup_flag;
extern trans_cntrl_ptr p_cntrl_ptr;
typedef QTIMER oam_timer_t;
/* SPR 20584 changes start */
char snmp_master_ip_addr[MAX_IP_ADD_LEN_STR];
int snmp_master_port;
int tcp_flag;
/* SPR 20584 changes end */

/****************************************************************************
* Function Name  : oam_htons 
* Inputs         : host port in short 
****************************************************************************/
unsigned int oam_htons(unsigned int hostshort) //WG
{
    return htons(hostshort);
}

/****************************************************************************
* Function Name  : malloc
* Inputs	 : size
****************************************************************************/
static  void    *s_malloc( unsigned int size )
{
        return malloc( size );
}

/****************************************************************************
* Function Name  : free
* Inputs	 : Memory Pointer
****************************************************************************/
static  void    s_free( void *m )
{
        free(m);
}

/****************************************************************************
* Function Name  : newkey
****************************************************************************/
static  void    *s_newkey( void )
{
        pthread_key_t   *key = (pthread_key_t *)s_malloc( sizeof(pthread_key_t) );
        /* coverity 16256 */
        if(key != NULL) {
            oam_mgmt_memset( key, OAM_ZERO, sizeof (pthread_key_t) ); 
            pthread_key_create( key, 0 );
        }
        return (void *)key;
}

/****************************************************************************
* Function Name  : getkey
****************************************************************************/
static  void    *s_getkey( void *key )
{
        return  (void *)pthread_getspecific( *((pthread_key_t *)key) );
}

/****************************************************************************
* Function Name  : setkey
****************************************************************************/
static  void    s_setkey( void *key, void *value )
{
        pthread_setspecific( *((pthread_key_t *)key), value );
}

/****************************************************************************
* Function Name  : newlock
****************************************************************************/
static  void    *s_newlock( void )
{
        pthread_mutex_t *lock = (pthread_mutex_t *)s_malloc( sizeof(pthread_mutex_t) );
        /* coverity 16257 */
        if(lock != NULL) {
            oam_mgmt_memset( lock, OAM_ZERO, sizeof (pthread_mutex_t) ); 
            pthread_mutex_init( lock, 0 );
        }
        return  (void *)lock;
}

/****************************************************************************
* Function Name  : lock
****************************************************************************/
static  void    s_lock( void *lock )
{
        if(OAM_ZERO != pthread_mutex_lock( (pthread_mutex_t *)lock ))
        {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," s_lock fails ");
        }
}

/****************************************************************************
* Function Name  : unlock
****************************************************************************/
static  void    s_unlock( void *lock )
{
        if(OAM_ZERO != pthread_mutex_unlock( (pthread_mutex_t *)lock ))
        {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," s_unlock fails ");
        }
}

/****************************************************************************
* Function Name  : destroylock
****************************************************************************/
static  void    s_destroylock( void *lock )
{
    if(OAM_ZERO != pthread_mutex_destroy( (pthread_mutex_t *)lock ))
    {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," s_destroylock Fails");
    }
    s_free( lock );
}

/****************************************************************************
* Function Name  : vprintf
****************************************************************************/
static  int     s_vprintf( const char *format, va_list A )
{
        return vfprintf( stderr, format, A );
}

/****************************************************************************
* Function Name  : abort
****************************************************************************/
static  void    s_abort(void)
{
        abort();
}

/****************************************************************************
* Function Name  : oam_perror
****************************************************************************/
void oam_perror(const char *string)
{
    perror(string);//klocwork fix
}

/***************************************************************************/

typedef struct
{
    int rx_sd;   /*Receiver discriptor*/
    int tx_sd;   /*Transmiter discriptor*/
} LISTENER;      /*The porting function for UNIX paltform. Structure QSYSOP is initialized with these functions*/


extern const QSYSOP     __os;
const QSYSOP    __os = {
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
static  void    s_walltime( QTIME *now )
{
        struct timeval  T;

        gettimeofday( &T, 0 );
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
static  void    *s_newchannel( void )
{
        __waitdata      *w = (__waitdata *)s_malloc( sizeof(__waitdata) );
        if(w != NULL) {
            oam_mgmt_memset(w, OAM_ZERO, sizeof(__waitdata));

            pthread_mutex_init( &(w->lock), 0 );
            w->value = 0;
        }
        return (void *)w;
}


/****************************************************************************
 * Function Name  : oam_snmp_fprintf
 * Inputs         : str,
 *                  format
 * Outputs        : ststus
 * Returns        : None
 * Description    : This function is used to fprintf.
****************************************************************************/
int oam_snmp_fprintf(FILE * str, const char *format, ...)
{
  int status = OAM_INVALID_RETURN;
  va_list args;
  OAM_SNMP_VA_START(args,format);
  status=vfprintf(str,format,args); //WG
  OAM_SNMP_VA_END(args);
  return status;
}

/****************************************************************************
 * Function Name  : oam_mgmt_memset
 * Inputs         : s :Pointer to node
 *                  c :int
 *                  n :size_t variable
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to memset.
****************************************************************************/
void *oam_mgmt_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}

/******************************************************************************
*   FUNCTION NAME   : s_sleep
*   DESCRIPTION     : This function is used for sleep
*   Inputs          : channel : Pointer to channel
*                     timeout : Pointer to timeout
*   Outputs         : NONE
*   RETURNS         : NONE
******************************************************************************/
static  void    s_sleep( void *channel, const QTIME *timeout )
{
        __waitdata      *w = (__waitdata *)channel;
        struct timespec T;

        if(OAM_ZERO == pthread_mutex_lock( &(w->lock) ))
        {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED," MUTEX LOCK SUCCESSFUL ");
        }
        if( !w->value ) {
                if( !timeout ) {
                if(OAM_ZERO == pthread_cond_wait( &(w->condition), &(w->lock) ))
                {
                    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,"conditional wait Success");
                }
                } else if ( timeout->s || timeout->us ) {
                        struct timeval  _T;

                        gettimeofday( &_T, 0 );

                        _T.tv_sec += timeout->s;
                        _T.tv_usec += timeout->us;
                        while( _T.tv_usec >= 1000000L ) {
                                _T.tv_usec -= 1000000L;
                                _T.tv_sec++;
                        }

                        T.tv_sec = _T.tv_sec;
                        T.tv_nsec = 1000 * _T.tv_usec;
                        if(OAM_ZERO == pthread_cond_timedwait( &(w->condition), &(w->lock), &T ))
                        {
                             OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,"cond timedwait Success ");
                        }
                } else {
                        /** Nothing -- zero timeout was specified **/
                }
        }
	 w->value = 0;
     if(OAM_ZERO == pthread_mutex_unlock( &(w->lock) ))
     {
         OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED," MUTEX UNLOCK SUCCESSFUL ");
     }
}

/******************************************************************************
*   FUNCTION NAME   : s_wakeup
*   DESCRIPTION     : This function is used for wakeup
*   Inputs          : channel : Pointer to channel
*   Outputs         : NONE
*   RETURNS         : NONE
******************************************************************************/
static  void    s_wakeup( void *channel )
{
        __waitdata      *w = (__waitdata *)channel;

        if(OAM_ZERO == pthread_mutex_lock( &(w->lock) ))
        {
             OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED," MUTEX LOCK SUCCESSFUL ");
        }
        w->value = 1;
        pthread_cond_signal( &(w->condition) );
        if(OAM_ZERO == pthread_mutex_unlock( &(w->lock) ))
        {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED," MUTEX UNLOCK SUCCESSFUL ");
        }
}

/*****************************************************************************/
extern const QWAIT __waitstruct;
const QWAIT __waitstruct = {
        s_newchannel,
        s_sleep,
        s_wakeup,
        s_walltime
};


const QWAIT *waitstruct = &__waitstruct;

/****************************************************************************
 * Function Name  : oam_makeserver_udp
 * Inputs         : pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket and bind it.
 ****************************************************************************/
static int makeserver_udp( const struct sockaddr_in *self )
{
     int sd; /*socket descriptor*/

    if( (sd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0 )
    {
       oam_perror("socket");
       s_abort();
    }

    if( bind (sd, (struct sockaddr *)self, sizeof(*self) ) < 0 )
    {
       s_abort();

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
static int makeclient_udp(void)
{
    int sd; /*socket descriptor*/

    if( (sd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0 )
    {
        oam_perror("socket");
        s_abort();
    }

    return sd;
}


/****************************************************************************
 * Function Name  : oam_timedwait
 * Inputs         : listener, timed out
 * Outputs        : None
 * Returns        : void
 * Description    : This function waits until either the listner the object is ready for read, 
 *		    or until the specified timed out has occured
 ****************************************************************************/
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
    n = select(  FD_SETSIZE - 1, &F, 0, 0, T?&timeout:0 );

    if( n < 0 ) {
                perror("select");
    }
}

/**************************************************************************
 * Function Name  : snscanf
 * Inputs         : str :string 
 *                : n :int
 *                  variable arg list 
 * Outputs        : None
 * Returns        : int
 * Variables      : None
 * Description    : wrapper fn to scanf
****************************************************************************/
int snscanf(const char *str, int n, const char *format, ...)
{
    va_list             args;
    int                 result = OAM_ZERO;;
    char               *str2   = NULL;
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;

    retVal = oam_mgmt_mem_alloc(( n + 1),(void *)&str2);

    if (OAM_SNMP_SUCCESS != retVal) {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR,
               "Memory allocation to str2 failed with Error Code: %d", retVal);
        /* Not Enough Memory */
        return OAM_ZERO;
    }
/* coverity 41030 */
    oam_mgmt_memset((void *)str2, OAM_ZERO, n + 1);
    oam_snmp_strncpy (str2, str, n);
    str2[n] = OAM_ZERO;
    OAM_SNMP_VA_START (args, format);
    result = vsscanf (str2, format, args);
    OAM_SNMP_VA_END (args);

    /* Free Memory Allocated to STR2 */
     qvMsgFree(str2);

    return result;
}

int oam_isValidIp4 (UInt8 *p_str)
{
    SInt32 segs = OAM_ZERO;   /* Segment count. */
    SInt32 chcnt = OAM_ZERO;  /* Character count within segment. */
    SInt32 accum = OAM_ZERO;  /* Accumulator for segment. */

    /* Catch NULL pointer. */
    if(!p_str)
        return 0;


    /* Process every character in p_string. */
    while (*p_str != '\0')
    {
        /* Segment changeover. */
        if('.' == *p_str) {
            /* Must have some digits in segment. */

            if(!chcnt)
                return 0;

            /* Limit number of segments. */

            if(4 == ++segs)
                return 0;

            /* Reset segment values and restart loop. */
            chcnt = accum = 0;
            p_str++;
            continue;
        }
        /* Check numeric. */
        if ((*p_str < '0') || (*p_str > '9'))
            return 0;

        /* Accumulate and check segment. */
        if ((accum = accum * 10 + *p_str - '0') > 255)
            return 0;

        /* Advance other segment specific stuff and continue loop. */

        /* Advance other segment specific stuff and continue loop. */

        chcnt++;
        p_str++;
    }

    /* Check enough segments and enough characters in last segment. */

    if (segs != 3)
        return 0;

    if(!chcnt)
        return 0;

    /* Address okay. */

    return 1;
}

/******************************************************************************
 * FUNCTION NAME  : oam_snmp_fopen
 * RETURNS        : File Pointer
 * INPUTS         : filename: name of the file
 *                  mode : file mode
 * DESCRIPTION    :function to open a file.
 ******************************************************************************/

FILE *oam_snmp_fopen(const Char8 *filename , const Char8 *mode)
{
    return fopen(filename, mode);
}

/******************************************************************************
 * FUNCTION NAME  : oam_snmp_fclose
 * RETURNS        : File Pointer
 * INPUTS         : filename: name of the file
 *                  mode : file mode
 * DESCRIPTION    :function to open a file.
 ******************************************************************************/

void oam_snmp_fclose(FILE *file_ptr)
{
     fclose(file_ptr);
}



void oam_get_snmp_communication_info(oam_snmp_comm_info_t * comm_info)
{
    char read_buf[MAX_LINE_LENGTH]={0};
    FILE *fp           = OAM_NULL;
    fp = oam_snmp_fopen("../cfg/eNodeB_Configuration.cfg","r");
    if(!fp) {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                " Unable to open eNodeB_Configuration.cfg");
        oam_snmp_exit(1);
    }
    while (fgets((char *)read_buf, MAX_LINE_LENGTH, fp)) {
        if(snscanf(read_buf,
                   sizeof("ENB_OAM_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
                   "ENB_OAM_IP_ADDRESS=%s", comm_info->oam_ip_addr)) {
            if(!oam_isValidIp4((UInt8 *)comm_info->oam_ip_addr)) {
                OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," Invalid OAM ip address");
                oam_snmp_exit(1);
            }
        }
        else if(snscanf(read_buf,
                        sizeof("ENB_OAM_RX_PORT=") + MAX_PORT_LENGTH,
                        "ENB_OAM_RX_PORT=%d", &comm_info->oam_port)) {
        }

        else if(snscanf(read_buf,
                        sizeof("ENB_SNMP_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
                        "ENB_SNMP_IP_ADDRESS=%s", comm_info->snmp_ip_addr)) {
            if(!oam_isValidIp4 ((UInt8*)comm_info->snmp_ip_addr)) {
                OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," Invalid SNMP ip address");
                oam_snmp_exit(1);
            }
        }
        else if(snscanf(read_buf,
                        sizeof("ENB_SNMP_RX_PORT=") + MAX_PORT_LENGTH,
                        "ENB_SNMP_RX_PORT=%d", &comm_info->snmp_port)) {
        }

        /* SPR 20584 changes start */
        else if(snscanf(read_buf,
                        sizeof("ENB_SNMP_MASTER_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
                        "ENB_SNMP_MASTER_IP_ADDRESS=%s", snmp_master_ip_addr)) {
            if(!oam_isValidIp4 ((UInt8*)snmp_master_ip_addr)) {
                OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING," Invalid SNMP MASTER ip address");
                oam_snmp_exit(1);
            }
        }
        else if(snscanf(read_buf,
                        sizeof("ENB_SNMP_MASTER_RX_PORT=") + MAX_PORT_LENGTH,
                        "ENB_SNMP_MASTER_RX_PORT=%d", &snmp_master_port)) {
        }
        else if(snscanf(read_buf,
                        sizeof("TCP_FLAG=") + MAX_PORT_LENGTH,
                        "TCP_FLAG=%d", &tcp_flag)) {
        }
        /* SPR 20584 changes end */
    }

    oam_snmp_fclose(fp);
}

/****************************************************************************
 * Function Name  : oam_open
 * Inputs         : data
 * Outputs        : None
 * Returns        : void
 * Description    : This Function accepts a connection and 
 * 		    returns a pointer to updated listner structure
 ****************************************************************************/
static void *oam_open( void *data )
{
    LISTENER *u = (LISTENER *)qvSysMalloc(sizeof(LISTENER)); /*This function creates a new listner data structure, accepts a connection and returns a pointer to updated listner structure*/

    if(u == OAM_NULL)
    {
	/* SPR_18897 Fix Start */
        perror("qvSysMalloc fail in oam_open");
	/* SPR_18897 Fix End */
 	oam_snmp_exit(1);		
    }

    /* SPR 17777 fix */

   /* SPR_18897 Fix Start */
     oam_snmp_fprintf(stderr,"\n unused parameter %p", data);
   /* SPR_18897 Fix End */
    oam_snmp_comm_info_t comm_info;
    oam_get_snmp_communication_info(&comm_info);

    oam_mgmt_memset( &snmp_address, 0, sizeof(snmp_address) );
    snmp_address.sin_family      = AF_INET;
    snmp_address.sin_addr.s_addr = inet_addr(comm_info.snmp_ip_addr);
    snmp_address.sin_port = oam_htons(comm_info.snmp_port);
    u->rx_sd = makeserver_udp( &snmp_address );
    oam_mgmt_memset( &oam_address, 0, sizeof(oam_address) );
    oam_address.sin_family      = AF_INET;
    oam_address.sin_addr.s_addr = inet_addr(comm_info.oam_ip_addr);
    oam_address.sin_port = oam_htons(comm_info.oam_port);
    /* Send Socket of OAM */
    u->tx_sd = makeclient_udp();
    return u;
}

/****************************************************************************
 * Function Name  : oam_receive
 * Inputs         : sender, receiver, priority
 * Outputs        : None
 * Returns        : void
 * Description    : This function determines the intended receiver, 
 *		    the sender and the priority of the message
 ****************************************************************************/
static    void *oam_receive( void *S,
                QMODULE        *sender,
                QMODULE      *receiver,
                signed char *priority ) /*This function receive a single incoming message in to a message buffer and returns it. This function determines the intended receiver, the sender and the priority of the message*/
{

    char                receive_buffer[MAX_RECEIVE_BUFFER_SIZE];
    fd_set              F;
    S32                 n;
    /* coverity 40457*/
    S32                 bytes_read;
    char                *msg_buf    = NULL;
    U16                 src_id     = 0;
    U16                 dst_id     = 0;
    U16                 api_id     = 0;
    U16                 msg_size   = 0;
    struct timeval      zero;
    LISTENER            *u = (LISTENER *)S;


    FD_ZERO( &F );
    FD_SET( u->rx_sd, &F );


    zero.tv_sec  = 0;
    zero.tv_usec = 0;


    n = select( FD_SETSIZE - 1, &F, 0, 0, &zero );

    if( n < 0 ) {
       perror("select");
    }

    if( FD_ISSET( u->rx_sd, &F) ) {
        if((bytes_read = recvfrom(u->rx_sd, receive_buffer,
                        MAX_RECEIVE_BUFFER_SIZE, 0, 0, 0)) < 0) {
            perror("trread:attr");
        }
        else if (!bytes_read) {
            return 0;
        }
        else {
            /* Parse header */
            src_id  = get_word_from_header((UInt8*)(receive_buffer
                        + SNMP_OAM_IF_MSG_SRC_ID_OFFSET));
            dst_id      = get_word_from_header((UInt8*)(receive_buffer
                        + SNMP_OAM_IF_MSG_DST_ID_OFFSET));
            api_id      = get_word_from_header((UInt8*)(receive_buffer
                        + SNMP_OAM_IF_API_ID_OFFSET));
            msg_size    = get_word_from_header((UInt8*)(receive_buffer
                        + SNMP_OAM_IF_MSG_LEN_OFFSET));

            OAM_SNMP_LOG (SNMP, OAM_SNMP_INFO, "Received API[%u] of length[%u] "
                "from Module[%u] to Module[%u]", api_id, msg_size, src_id, dst_id);

            *sender     = qvGetService(src_id);
            *receiver   = qvGetService(dst_id);

            if(msg_size > 0 && msg_size <= MAX_RECEIVE_BUFFER_SIZE) {
                msg_buf = qvMsgAlloc(NULL, 0, 0, msg_size);
                if(!msg_buf) {
                    OAM_SNMP_LOG (SNMP, OAM_SNMP_ERROR, "Memory allocation to "
                            "msg_buf failed");
                }
                else {
                    snmp_memcpy(msg_buf ,receive_buffer, msg_size );
                }
            }
            else {
                OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                    "Insufficient buffer to hold the msg of size: %d",
                    msg_size);
            }
        }
    }
    return msg_buf;
}

/****************************************************************************
 * Function Name  : oam_send
 * Inputs         : message, sender, receiver, data, size
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
    unsigned int   payloadsize = 0;
    U16            dst_module = 0;
    U8            *p_msg   = NULL;
    S16            no_of_char_rec;

    payloadsize = qvMsgSize(message, 0);
    /* Allocate buffer for outgoing message */
#ifdef MEM_CHECK_DOUBLE_FREE
    p_msg = (U8*)s_malloc((size - SH_HDRSIZE + payloadsize));
#else
    p_msg = (U8*)qvAlloc((size - SH_HDRSIZE + payloadsize), NULL);
#endif
    if(p_msg == OAM_NULL)
    {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR,"Memory allocation to p_msg failed");	
       oam_snmp_exit (1);
    }
    /* Copy body without CSPL header */
    snmp_memcpy(p_msg, (U8 *)message + SH_HDRSIZE, (size - SH_HDRSIZE));
    /* Copy payload if any */
    qvMsgExtract(message, 0, (U8 *)p_msg + (size - SH_HDRSIZE), payloadsize);
    /* Get transaction id from header */

    dst_module =
        get_word_from_header((U8 *)message + CSPL_HDR_DST_ID_OFFSET);
    payloadsize =
        get_word_from_header((U8 *)message + CSPL_HDR_PAYLOADSIZE_OFFSET);
    /* SPR 17777 fix */
    OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO," dst_mod_id = %d, unused parameter %p, %p and %p ",dst_module, data, receiver, sender);
    if(OAM_MODULE_ID == dst_module) {   /* coverity 40449 */
        no_of_char_rec= sendto(u->tx_sd,p_msg,payloadsize,0,(struct sockaddr*)&oam_address,sizeof(oam_address));
        if(-1 == no_of_char_rec) {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR," Error while Sending");
        }
    }
    else {
       OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR," Invalid Destination %d", dst_module);
    }

#ifdef MEM_CHECK_DOUBLE_FREE
    s_free( p_msg );
#else
    qvFree(p_msg);
#endif
}

/****************************************************************************
 * Function Name  : oam_close
 * Inputs         : pointer to data
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for close
 ***************************************************************************/
static void oam_close( void *data ) /*This function cleans up the IPC listner structure*/
{
    LISTENER    *L = (LISTENER *)data;

    close( L->tx_sd );
    close( L->rx_sd );
    qvSysFree(data);
}

/****************************************************************************/
const QSHELL __operations = {
    oam_timedwait,
    oam_send,
    oam_receive,
    oam_open,
    oam_close,
    0
};
const QSHELL *operations = &__operations;

/****************************************************************************
 * Function Name  : oam_snmp_register_signal_handler
 * Inputs         : sig : int
                    fn pointer for free_resources
 * Outputs        : None
 * Returns        : void
 * Description    : wrapper to register a signal
 ***************************************************************************/
void (*oam_snmp_register_signal_handler (int sig, void (*free_resources) (int))) (int) {   /*porting changes*/
if(sig ==  SNMP_SIGTERM)
    sig = SIGTERM ;
else if(sig ==  SNMP_SIGKILL)
    sig = SIGKILL ;
else if(sig ==  SNMP_SIGINT)
    sig = SIGINT ;

(signal(sig, free_resources));

    return (*free_resources);
}


/******************************************************************************
 *   FUNCTION NAME  : oam_snmp_sprintf
 *   RETURNS        : status
 *   INPUTS         : str :string
 *                    variable arg list
 *   DESCRIPTION    :Prototype of sprintf. 
 ******************************************************************************/
int
oam_snmp_sprintf (char *str, const char *format, ...) 
{   
    int status = -1;
    va_list args;
    OAM_SNMP_VA_START (args, format);
    status = vsprintf (str, format, args);
    OAM_SNMP_VA_END (args);
    return status;
}
/******************************************************************************
 *   FUNCTION NAME  : snmp_memcpy
 *   RETURNS        : 
 *   INPUTS         : dest :string
 *                    src  : string
 *                  : n    : size
 *   DESCRIPTION    :Prototype of memcpy. 
 ******************************************************************************/
void *snmp_memcpy(void *dest, const void *src, size_t n)
{
            return memcpy(dest, src, n);
}

void oam_snmp_exit(int status)
{
   exit(status);
}

char *oam_snmp_strncpy(Char8 *s1, const Char8 *s2, size_t n)
{
OAM_NULL_CHECK(s1 != NULL);
OAM_NULL_CHECK(s2 != NULL);
return strncpy(s1,s2, n);
}

time_t oam_snmp_time(time_t *t)
{
    return time (t);
}

struct tm *oam_snmp_localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_r(timep, result);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_snmp_strncat
 *   RETURNS:        status
 *   INPUTS :        str1 - source string
 *           str2 - destination string
 *           n    - size to be copied in bytes
 *   DESCRIPTION:    This functions concatinates n bytest of string str2 to end
 *           of str1
 ******************************************************************************/
Char8* oam_snmp_strncat(Char8 *str1, const Char8 *str2, size_t n)
{
    return strncat(str1, str2, n);
}

Char8* oam_snmp_strcat(Char8 *str1, const Char8 *str2)
{
    return strcat(str1, str2);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_snmp_strncmp
 *   RETURS      output
 *   INPUTS:         str1 - string 1 to be compared
 *           str2 - string 2 to be compared
 *           length - number of bytes to be compared
 *   DESCRIPTION:    function returns a pointer to the first occurrence of
 *           charcter var
 ******************************************************************************/
int oam_snmp_strncmp(const char *str1, const char *str2, size_t length)
{
    OAM_NULL_CHECK(str1 != NULL);
    OAM_NULL_CHECK(str2 != NULL);
    return strncmp(str1, str2, length);
}

/****************************************************************************
 * Function Name  : oam_snmp_strlen
 * Inputs         : str - string
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function find s the length of the string passed
 ***************************************************************************/
size_t
oam_snmp_strlen
(
    const char *str
)
{
     int ret_val;
     OAM_NULL_CHECK(str != NULL);
     ret_val = strlen(str);
     return ret_val;
}

/****************************************************************************
 *   FUNCTION NAME:  oam_snmp_strchr
 *   RETURNS:        output
 *   INPUTS :        str - string to be searched
 *           var - charcter to be searched
 *   DESCRIPTION:    function returns a pointer to the first occurrence of
 *           charcter var
 ******************************************************************************/
char *
oam_snmp_strchr
(
    const char *str,
    char var
)
{
    return strchr(str, var);
}

long  oam_snmp_strtol(const  char  *str, char ** endptr, int base)
{
    return strtol(str, endptr, base);
}

unsigned long oam_snmp_strtoul(char * str,
              char **endptr, int base)
{
    return strtoul(str, endptr, base);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_snmp_atoi
 *   RETURNS:        output
 *   INPUTS :
 *   DESCRIPTION:    This function converts array to int value
 ******************************************************************************/
int
oam_snmp_atoi
(
    const char *str
)
{
    OAM_NULL_CHECK(NULL != str);/* Coverity FIX 41202 */
    return atoi(str);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_snmp_snprintf
 *   RETURNS:        status
 *   INPUTS :        str - character buffer to be filled
 *   DESCRIPTION:    This functions vsnprintf() write at most size bytes
 *           to str
 ******************************************************************************/
int oam_snmp_snprintf(char *str, size_t n, const char *format, ...)
{
   int status = OAM_INVALID_RETURN;
   va_list args;
   OAM_SNMP_VA_START(args,format);
   /* Coverity FIX 53741*/
   status = vsnprintf(str,n,format,args);
   OAM_SNMP_VA_END(args);
   return status;
}
