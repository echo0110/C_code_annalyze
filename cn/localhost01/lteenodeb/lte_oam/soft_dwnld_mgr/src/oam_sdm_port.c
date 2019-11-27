/*******************************************************************************
 * File Name   : oam_sdm_port.c
 *
 * Description : This file contains Qcli and socket based programming functions
 *
 * Revision History:
 *
 * Date              Author          Reference                Comments
 * -----             ------          ---------                --------
 * February,2013     Vinod Vyas      OAM Design Document      Initial Version
 * May 2014          Shalu Sagar     Klockwork Fix
 *
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/
/*******************************************************************************
 * Includes
 *** ***************************************************************************/
#include "oam_sdm_main.h"
#include <cspl.h>
#include <signal.h>
/*******************************************************************************
  Global Declaration
 *******************************************************************************/
typedef struct sockaddr_in oam_sockaddr_t;
typedef struct in_addr oam_in_addr_t;
typedef struct timeval oam_sdm_timeval;
extern sdm_ext_com_config_t comm_data;
LOCAL oam_sockaddr_t oam_address, sdm_address;

/*******************************************************************************
  Private Definitions
 *******************************************************************************/

/****************************************************************************
 * Function Name  : oam_htons
 * Inputs         : hostshort :unsigned int
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used as a wrapper function for htons().
****************************************************************************/
unsigned int oam_htons(unsigned int hostshort) //WG
{
    return htons(hostshort);
}

/****************************************************************************
 * Function Name  : oam_memset
 * Inputs         : s :Pointer to node
 *                  c :int
 *                  n :size_t variable
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to memset.
****************************************************************************/
void *oam_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}


/****************************************************************************
 * Function Name  : oam_perror
 * Inputs         : string :string
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for perror.
****************************************************************************/       
void oam_perror(const char *string)
{
    perror(string);   /* klocwork fix  */
} 


/****************************************************************************
 * Function Name  : oam_malloc
 * Inputs         : size :size
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for malloc.
****************************************************************************/
static void *oam_malloc (UInt32 size)
{
    return malloc (size);
}

/****************************************************************************
 * Function Name  : oam_memcpy
 * Inputs         : dest :dest str
 *                  src :source str
 *                  n :size_t variable
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for memcopy.
****************************************************************************/     
void *oam_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}

/****************************************************************************
 * Function Name  : oam_abort
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to abort.
****************************************************************************/    
/* + SPR 17439 */
void oam_abort(void)
/* - SPR 17439 */
{
    OAM_SDM_LOG(SDM,OAM_SDM_ERROR, " System Error - oam_abort() called ");
}


/****************************************************************************
 * Function Name  : oam_free
 * Inputs         : m :Pointer to void
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to free node
 ****************************************************************************/     
static void
oam_free (void *m)
{
    free (m);
}


/****************************************************************************
 * Function Name  : oam_vprintf
 * Inputs         : format :format string
 *                  A :va_list
 * Outputs        : None
 * Returns        : Int
 * Description    : This function is used to vfprintf
****************************************************************************/      
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
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    oam_vprintf,
    oam_vprintf,
    oam_abort
};

const QSYSOP *os = &my_os;


static void
oam_walltime (QTIME * now)
{
    oam_sdm_timeval T;
    gettimeofday(&T, OAM_SDM_ZERO);
    now->s = T.tv_sec;
    now->us = T.tv_usec;
}


typedef struct __waitdata
{
    pthread_cond_t condition;
    pthread_mutex_t lock;
    int value;
} __waitdata;


const QWAIT my_waitstruct = {
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    OAM_SDM_ZERO,
    oam_walltime
};


/****************************************************************************
 * Function Name  : oam_get_byte_from_header
 * Inputs         : p_header :header string
 * Outputs        : None
 * Returns        : get byte from header
 * Description    : This function is used to get byte from header.
****************************************************************************/      
UChar8
oam_get_byte_from_header (UChar8 * p_header)
{
    return *p_header;
}

/****************************************************************************
 * Function Name  : oam_errorabort
 * Inputs         : s :Pointer to error string
 * Outputs        : none
 * Returns        : none
 * Description    : Encaps function for errorabort
 ****************************************************************************/

static void oam_errorabort (const Char8 * s)	/*This function print the error and aborts the process */
{
    /* SPR 17777 fix */
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED,"unused parameter %p", s);
    oam_sdm_abort ();
}

/****************************************************************************
 * Function Name  : oam_makeserver_udp
 * Inputs         : self :Pointer to sockaddr_in
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP server socket and bind it.
 ****************************************************************************/
static SInt16 oam_makeserver_udp (const oam_sockaddr_t * self)
{
    OAM_SDM_FUN_ENTRY_TRACE();
    Int sd;	

    if ((sd = socket (AF_INET, SOCK_DGRAM, OAM_SDM_ZERO)) < OAM_SDM_ZERO)
    {
        oam_errorabort ("socket");
        OAM_SDM_FUN_EXIT_TRACE();
    }

    if (bind(sd, (struct sockaddr *) self, sizeof (*self)) < OAM_SDM_ZERO)
    {
        perror ("OAM_LTE:");
        OAM_SDM_FUN_EXIT_TRACE();
        oam_sdm_exit (OAM_SDM_RESULT_SUCCESS);
    }

    OAM_SDM_FUN_EXIT_TRACE();
    return sd;
}
/****************************************************************************
 * Function Name  : oam_makeclient_udp
 * Inputs         : None
 * Outputs        : Creates UDP socket
 * Returns        : socket descriptor sd
 * Description    : This function is used to create a UDP socket.
 ****************************************************************************/
static SInt16 oam_makeclient_udp (void)
{
    OAM_SDM_FUN_ENTRY_TRACE();
    long int sd;			/*socket descriptor */

    if ((sd = socket (AF_INET, SOCK_DGRAM, OAM_SDM_ZERO)) < OAM_SDM_ZERO)
    {
        oam_errorabort ("socket");
    }

    OAM_SDM_FUN_EXIT_TRACE();
    return sd;
}

/****************************************************************************
 * Function Name  : oam_receive
 * Inputs         : sender :sender string
 *                  receiver :receiver string
 *                  priority :priority str
 * Outputs        : None
 * Returns        : void *
 * Description    : Encpas function for receive
 ****************************************************************************/
static void *oam_receive (void *S, QMODULE * sender, QMODULE * receiver, signed char *priority)	/*This function receive a single incoming message in to a
                                                                                                  message buffer and returns it. This function determines the intended
                                                                                                  receiver, the sender and the priority of the message */

{

    char            receive_buffer[OAM_SDM_RECV_BUFFER_SIZE] = { OAM_SDM_ZERO };   
    fd_set          F;
    S32             n;
    U32             bytes_read;
    char            *msg_buf   = OAM_SDM_NULL;
    U16             src_id     = OAM_SDM_MODULE_ID;
    U16             dst_id     = OAM_SDM_ZERO;
    U16             api_id     = OAM_SDM_ZERO;
    U16             msg_size   = OAM_SDM_ZERO;
    struct timeval  zero;
    LISTENER        *u         = (LISTENER *)S;


    FD_ZERO( &F );
    FD_SET( u->rx_sd, &F );


    zero.tv_sec  = OAM_SDM_ZERO;
    zero.tv_usec = OAM_SDM_ZERO;
    
    /* SPR 17777 fix */
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED,"unused parameter %p",priority);

    n = select( FD_SETSIZE - OAM_SDM_ONE, &F, OAM_SDM_ZERO, OAM_SDM_ZERO, &zero );

    if( n < OAM_SDM_ZERO )
    {
        oam_perror("select");
    }

    if( FD_ISSET( u->rx_sd, &F) )
    {

        bytes_read = recvfrom(u->rx_sd, (char *)receive_buffer, OAM_SDM_RECV_BUFFER_SIZE, OAM_SDM_ZERO, 
                                          OAM_SDM_ZERO, OAM_SDM_ZERO);  

        if (!bytes_read)//klocwork fix
        {
            return OAM_SDM_ZERO;
        }
        else
        {
            /* Parse header */
            src_id      = oam_sdm_get_word_from_header((UInt8 *)(receive_buffer + OAM_SDM_TWO));
            dst_id      = oam_sdm_get_word_from_header((UInt8 *)(receive_buffer + OAM_SDM_FOUR));
            api_id      = oam_sdm_get_word_from_header((UInt8 *)(receive_buffer + OAM_SDM_SIX));
            msg_size    = oam_sdm_get_word_from_header((UInt8 *)(receive_buffer + OAM_SDM_EIGHT));

            OAM_SDM_LOG(SDM,OAM_SDM_INFO,"Received API[%u] of length[%u] "
                "from Module[%u] to Module[%u]", api_id, msg_size, src_id, dst_id);


            *sender     = qvGetService(src_id);
            *receiver   = qvGetService(dst_id);

            if(msg_size > OAM_SDM_ZERO && msg_size <= OAM_SDM_RECV_BUFFER_SIZE)  
            {
                msg_buf = qvMsgAlloc(OAM_SDM_NULL, OAM_SDM_ZERO, OAM_SDM_ZERO, msg_size);
                if (msg_buf == OAM_SDM_NULL)
                {
                    OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"Memory allocation to"
                            " msg_buf failed");
                }
                else
                {
                    oam_memcpy(msg_buf ,receive_buffer, msg_size );

                }
            }
            else
            {
                OAM_SDM_LOG(SDM,OAM_SDM_WARNING,"Msg size is invalid[%d]. Source Id=[%d], API Id=[%d]",
                            msg_size, src_id, api_id);
            }
        }
    }
    return msg_buf;
}


/****************************************************************************
 * Function Name  : oam_open
 * Inputs         : data :data str
 * Outputs        : None
 * Returns        : Listner structure.
 * Description    : Opens sockets for communication and xml parsing
 ****************************************************************************/

static void *oam_open (void *data)
{
    OAM_SDM_FUN_ENTRY_TRACE();
    UInt16 oam_port = OAM_SDM_ZERO;
    UInt16 sdm_port = OAM_SDM_ZERO;
    u = (LISTENER *) qvSysMalloc (sizeof (LISTENER));	/*This function
                                                          creates a new listner data structure, accepts a connection and
                                                          returns a pointer to updated listner structure */
    /* Read IP,Port of Lower Layers from enb_address_config.cfg */
    /* Receive Socket of OAM */
    
    sdm_get_config_info ();
    
    oam_memset (&sdm_address, OAM_SDM_ZERO, sizeof (sdm_address));
    sdm_address.sin_family = AF_INET;
    sdm_address.sin_addr.s_addr = inet_addr((const char *) comm_data.sdm_com_params.ipaddr); 
    sdm_port = atoi((const char *) comm_data.sdm_com_params.port);
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED,"SDM IP: %s. SDM Port: %s ",
        comm_data.sdm_com_params.ipaddr, comm_data.sdm_com_params.port);
    sdm_address.sin_port = oam_htons (sdm_port);
    u->rx_sd = oam_makeserver_udp (&sdm_address);
    if (OAM_NEG_ONE == u->rx_sd)
    {
        qvPanic ("ERROR: Failed to create UDP Server\n");
        oam_sdm_abort ();
    }
    /* Send Socket of SDM */
    oam_memset (&oam_address, OAM_SDM_ZERO, sizeof (oam_address));
    oam_address.sin_family = AF_INET;
    oam_address.sin_addr.s_addr = inet_addr((const char *) comm_data.sdm_com_params.ipaddr); 
    oam_port = atoi((const char *) comm_data.oam_com_params.port);
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED,"OAM IP: %s. OAM Port: %s "
        ", unused parameter %p ",
        comm_data.oam_com_params.ipaddr, comm_data.oam_com_params.port, data);
    oam_address.sin_port = oam_htons (oam_port);

    u->tx_sd = oam_makeclient_udp ();
    if (OAM_NEG_ONE == u->tx_sd)
    {
        qvPanic ("ERROR: Failed to create UDP client\n");
        OAM_SDM_FUN_EXIT_TRACE();
        oam_sdm_abort ();
    }
    OAM_SDM_FUN_EXIT_TRACE();
    return u;
}

/****************************************************************************
 * Function Name  : oam_close
 * Inputs         : data :data str
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for close
 ****************************************************************************/

static void oam_close (void *data)	/*This function cleans up the IPC listner structure */
{
    OAM_SDM_FUN_ENTRY_TRACE();
    LISTENER *Lis = (LISTENER *) data;

    close (Lis->tx_sd);
    close (Lis->rx_sd);
    qvSysFree (data);
    OAM_SDM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_sdm_timedwait
 * Inputs         : S :Pointer to void
 *                  T :Pointer to QTIME 
 * Outputs        : None
 * Returns        : void
 * Description    : Encaps function for timedwait
 ****************************************************************************/

static void oam_sdm_timedwait (void *S, const QTIME * T)	/*This function waits until either the listner the object is ready for read,
                                                          or until the specified timed out has occured */
{
    LISTENER        *u = (LISTENER *)S;
    struct timeval  timeout = {0};
    SInt16     n;
    fd_set  F;
    FD_ZERO( &F );
    FD_SET( u->rx_sd, &F );

    if( T )
    {
        timeout.tv_sec = OAM_SDM_TIMEOUT;
        timeout.tv_usec = OAM_SDM_ZERO;
    }
    n = select(  FD_SETSIZE - OAM_SDM_ONE, &F, OAM_SDM_ZERO, OAM_SDM_ZERO, &timeout );
    
    if( n < OAM_SDM_ZERO )
    {
       oam_perror("select");
    }

}

/****************************************************************************
 * Function Name  : oam_send
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

static void oam_send (void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, UInt32 size)	/*This function sends the message to the appropriate destination after
                                                                                                                  filling the Shell header */
{

    LISTENER *u   = (LISTENER *) S;
    U16 api_size  = OAM_SDM_ZERO;
    UInt8 ret_val = OAM_SDM_ZERO;

    message += OAM_CSPL_HDR_SIZE;
    api_size = oam_sdm_get_word_from_header ((U8 *) message + OAM_SDM_EIGHT);

    ret_val= sendto (u->tx_sd, (SInt8 *) message,
            api_size, OAM_SDM_ZERO,
            (struct sockaddr *) &oam_address,
            sizeof (struct sockaddr_in));
    
    if(ret_val)
    {
        /* SPR 17777 fix */
        OAM_SDM_LOG(SDM, OAM_SDM_DETAILED," [oam_send]:Send successfully , unused parameter %p, %p, %p and %p", sender, receiver, data, size);
    }


}

const QSHELL my_shell = {
    oam_sdm_timedwait,
    oam_send,
    oam_receive,
    oam_open,
    oam_close,
    OAM_SDM_ZERO
};
/******************************************************************************
 *   FUNCTION NAME: oam_sdm_fork
 *   RETURNS: Process Id
 *   INPUTS : NONE
 *   DESCRIPTION:function for forking.
 ******************************************************************************/
    pid_t
oam_sdm_fork (void)
{
    return fork ();
}

/******************************************************************************
 *   FUNCTION NAME: oam_sdm_getpid
 *   RETURNS: Process id.
 *   INPUTS : NONE
 *   DESCRIPTION:function to get process id.
 ******************************************************************************/

    pid_t
oam_sdm_getpid (void)
{
    return getpid ();
}

/******************************************************************************
 *   FUNCTION NAME: oam_sdm_execv
 *   RETURNS: process
 *   INPUTS : filename :filename
 *            argv :arguement array
 *            envp :envp array
 *   DESCRIPTION:function to allocate message allocate byte size.
 ******************************************************************************/
/* KLOCWORK warning fix start */
Int
oam_sdm_execve (const char *filename, char *const argv[], char *const envp[])
{
    /* coverity 35995 */
    return execve (filename, argv, envp);
/* KLOCWORK warning fix end */
}

/****************************************************************************
 * Function Name  : oam_sdm_fprintf
 * Inputs         : str,
 *                  format
 * Outputs        : ststus
 * Returns        : None
 * Description    : This function is used to fprintf.
 ****************************************************************************/
int oam_sdm_fprintf(FILE * str, const char *format, ...)
{
  int status = OAM_INVALID_RETURN;
  va_list args;
  OAM_SDM_VA_START(args,format);
  status=vfprintf(str,format,args); 
  OAM_SDM_VA_END(args);
  return status;
}


/******************************************************************************
 *   FUNCTION NAME  : oam_sdm_sprintf
 *   RETURNS        : status
 *   INPUTS         : str :string
 *                    variable arg list
 *   DESCRIPTION    :Prototype of sprintf. 
 ******************************************************************************/
    int
oam_sdm_sprintf (char *str, const char *format, ...)
{
    int status = OAM_NEG_ONE;
    va_list args;
    OAM_SDM_VA_START (args, format);
    status = vsprintf (str, format, args);
    OAM_SDM_VA_END (args);
    return status;
}

/******************************************************************************
 * FUNCTION NAME  : oam_sdm_fopen
 * RETURNS        : File Pointer
 * INPUTS         : filename: name of the file
 *                  mode : file mode
 * DESCRIPTION    :function to open a file.
 ******************************************************************************/

FILE *oam_sdm_fopen(const Char8 *filename , const Char8 *mode )
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

void oam_sdm_fclose(FILE *file_ptr)
{
     fclose(file_ptr);
}


/******************************************************************************
 * FUNCTION NAME  : oam_sdm_strcpy
 * RETURNS        : Pointer to string
 * INPUTS         : dest :Destination string
 *                  src :source string
 * DESCRIPTION    :function to copy string.
 ******************************************************************************/

char *oam_sdm_strcpy(char *dest, const char *src)
{
   return strncpy(dest,src,oam_sdm_strlen(src)+OAM_SDM_ONE);
}

/******************************************************************************
 * FUNCTION NAME  : oam_sdm_strcmp
 * RETURNS        : integer.
 * INPUTS         : s1 :string 1
 *                  s2 :string2 
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
int oam_sdm_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1,s2);
}

/*****************************************************************************
 * FUNCTION NAME  :oam_sdm_strtok 
 * RETURNS        : Pointer to string
 * INPUTS         : str :source str 
 *                  delim :delimiter
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
    char *
oam_sdm_strtok(char *str, const char *delim)
{
    return strtok(str,delim);
}


/*****************************************************************************
 * FUNCTION NAME  : oam_sdm_strcat 
 * RETURNS        : string 
 * INPUTS         : dest :Destination str
 *                  src :Source str
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
    char *
oam_sdm_strcat(char *dest, const char *src)
{
    return strncat(dest,src,oam_sdm_strlen(src)+OAM_SDM_ONE);
}

/******************************************************************************
 *   FUNCTION NAME  : oam_sdm_system
 *   RETURNS        : system call status.
 *   INPUTS         : string :constant string.
 *   DESCRIPTION    : Prototype of system call.
 ******************************************************************************/
SInt8
oam_sdm_system (const char *string)
{
    /* coverity 35997 */
    return system (string);
}

/****************************************************************************
 *   FUNCTION NAME  : oam_sdm_remove
 *   RETURNS        : system call status.
 *   INPUTS         : string :constant string.
 *   DESCRIPTION    : Prototype of system call.
 ******************************************************************************/


SInt8
oam_sdm_remove (const char *string)
{

    /* coverity 35996 */
    return remove (string);

}
/***********************************************************************
 * FUNCTION NAME  : oam_sdm_abort
 * RETURNS        : system call status.
 * INPUTS         : none
 * DESCRIPTION    : Prototype of system call.
 * ******************************************************************************/

void oam_sdm_abort(void)
{
    OAM_SDM_LOG(SDM,OAM_SDM_ERROR, "System Error - abort() called ");
    abort();
}

/******************************************************************************
 *   FUNCTION NAME  : oam_sdm_memcpy
 *   RETURNS        : memcpy status.
 *   INPUTS         : dest :destination string
 *                    src :source string 
 *                    size :size
 *   DESCRIPTION    :Prototype of memcpy.
 ******************************************************************************/
    void *
oam_sdm_memcpy (void *dest, const void *src, size_t n)
{
    return memcpy (dest, src, n);
}

/*************************************************************************
 * Function Name  : oam_sdm_mem_alloc
 * Inputs         : size :size
 *                  p_msg :Pointer to pointer to msg
 * Outputs        : p_error_code :Pointer to error code
 * Returns        : OAM_SDM_SUCCESS/oAM_SDM_FAILURE
 * Description    : This function returns memory buffer from memory pool.
 *****************************************************************************/

    oam_sdm_return_et
oam_sdm_mem_alloc (size_t size,	/* Size of buffer which will be allocated */
        void **p_msg, UInt16 * p_error_code)
{
    void *p_buf = OAM_SDM_NULL;

    p_buf = qvMsgAlloc(OAM_SDM_NULL, OAM_SDM_ZERO, OAM_SDM_ZERO, size);	/* temp mask */

    if (p_buf == OAM_SDM_NULL)
    {
        OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"Memory allocation to"
                " p_buf failed");
        *p_error_code = OAM_SDM_MALLOC_FAIL;

        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    oam_memset(p_buf,0,size);
    *p_msg = p_buf;
    return OAM_SDM_SUCCESS;
}
/* SPR 22371 Fix Start */
/**************************************************************************
 * Function Name  : oam_sdm_kill_all_child
 * Inputs         : proc_id :process id of the parent process
 *                : signal :signal to be send
 * Outputs        : None 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None
 * Description    : wrapper fn to kill all child of parent process
 ****************************************************************************/
oam_sdm_return_et
oam_sdm_kill_all_child        
(                       
 pid_t proc_id,     
 UInt16 signal
 )           
{           
    FILE *fp;
    Char8 line[OAM_SDM_MAX_CMD_SIZE];
    Char8 cmd[OAM_SDM_MAX_CMD_SIZE];
    pid_t child_proc_id = OAM_SDM_ZERO;

    OAM_SDM_FUN_ENTRY_TRACE();
    oam_sdm_snprintf(cmd, OAM_SDM_MAX_CMD_SIZE, "pgrep -P %d", proc_id);
    fp = oam_sdm_popen(cmd, "r");
    if(OAM_SDM_NULL == fp)
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,"popen returned failure with errno=%d"
                "Cannot kill child of process %d",errno, proc_id);
        OAM_SDM_FUN_EXIT_TRACE();
        return OAM_SDM_FAILURE; 
    }
    while (oam_sdm_fgets(line, sizeof(line), fp) != OAM_SDM_NULL)
    {
        child_proc_id = oam_sdm_atoi(oam_sdm_strtok(line, " "));
        if(oam_sdm_kill(child_proc_id,signal) == OAM_SDM_ZERO)
        {                 
            OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Child PID %d killed successfully of Process PID %d",
                    child_proc_id, proc_id);
        }       
        else
        {
            OAM_SDM_LOG(SDM, OAM_SDM_WARNING,
                    "Unable to kill child process having pid %d of process %d with error no %d",
                    child_proc_id, proc_id, errno);
        }
    }
    oam_sdm_pclose(fp);
    OAM_SDM_FUN_EXIT_TRACE();
    return OAM_SDM_SUCCESS; 
}
/* SPR 22371 Fix End */
/**************************************************************************
 * Function Name  : oam_sdm_kill
 * Inputs         : proc_id :process id
 *                : signal :signal to be send
 * Outputs        : None 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None
 * Description    : wrapper fn to kill(pid,signal)
 ****************************************************************************/
oam_sdm_return_et
oam_sdm_kill        
(                       
 UInt16 proc_id,     
 UInt16 signal
 )           
{           
    OAM_SDM_FUN_ENTRY_TRACE();
/* SPR_18609 Fix Start */
    /* SPR 22371 Fix Start */
    oam_sdm_kill_all_child(proc_id, signal);
    /* SPR 22371 Fix End */
    if(kill(proc_id,signal) == OAM_SDM_ZERO)
    {                 
/* SPR_18609 Fix End */
        OAM_SDM_LOG(SDM, OAM_SDM_DETAILED,
                "oam_kill:process having pid : %d killed successfully",proc_id);
        OAM_SDM_FUN_EXIT_TRACE();
        return OAM_SDM_SUCCESS;
    }       
    else
    {
        OAM_SDM_LOG(SDM, OAM_SDM_WARNING,
                "oam_kill:Unable to kill process having pid : %d with error no.-%d",
                proc_id,errno);
        OAM_SDM_FUN_EXIT_TRACE();
        return OAM_SDM_FAILURE;
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
    int                 result = OAM_SDM_ZERO;;
    char               *str2   = OAM_SDM_NULL;
    UInt16 error_code          = OAM_SDM_ZERO;

    if (OAM_SDM_FAILURE == oam_sdm_mem_alloc(( n + OAM_SDM_ONE),(void *)&str2,&error_code))
    {
        OAM_SDM_LOG(SDM,OAM_SDM_ERROR,
                "Error in Memory Allocation with Error Code:%d",error_code);
        OAM_SDM_FUN_EXIT_TRACE();
        /* Not Enough Memory */
        return OAM_SDM_ZERO;
    }

    oam_sdm_strncpy (str2, str, n);
    str2[n] = OAM_SDM_ZERO;
    OAM_SDM_VA_START (args, format);
    result = vsscanf (str2, format, args);
    OAM_SDM_VA_END (args);
    
    /* Free Memory Allocated to STR2 */
    qvMsgFree(str2);
 
    return result;
}

void oam_sdm_exit(int status)
{
   exit(status);
}

size_t oam_sdm_strnlen(const char *s, size_t maxlen)
{
    return strnlen(s, maxlen);
}

sighandler_t oam_sdm_signal(int signum, sighandler_t handler)
{
    return signal(signum, handler);
}
    
char *oam_sdm_strncpy(Char8 *s1, const Char8 *s2, size_t n)
{
    if((s1 != NULL) && (s2 != NULL)){
      return strncpy(s1,s2, n);
    }
    else
    { 
     return OAM_SDM_ZERO;
    }
}

char *oam_sdm_fgets(char *s, int size, FILE *stream)
{
    return fgets(s,size,stream);
}

int
oam_sdm_atoi
(
    const char *str
)
{
    if(NULL != str){/* Coverity FIX 41202 */
     return atoi(str);
    }
    else
    { 
     return OAM_SDM_ZERO;
    }
}

time_t oam_sdm_time(time_t *t)
{
    return time(t);
}

struct tm *oam_sdm_localtime(const time_t *timep)
{
    return localtime(timep);
}

struct tm *oam_sdm_localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_r(timep, result);
}

/**************************************************************************
 * Function Name  : oam_sdm_register_signal_handler
 * Inputs         : sig : int
 *                : fn pointer for oam_sdm_signal_handler
 * Outputs        : None
 * Returns        : void 
 * Variables      : None
 * Description    : wrapper fn to register signal handler
****************************************************************************/
/*porting changes*/
void (*oam_sdm_register_signal_handler (int sig, void (*oam_sdm_signal_handler) (int))) (int) {
if(sig ==  SDM_SIGTERM)
    sig = SIGTERM ;
else if(sig ==  SDM_SIGKILL)
    sig = SIGKILL ;
else if(sig ==  SDM_SIGINT)
    sig = SIGINT ;

(signal(sig, oam_sdm_signal_handler));

    return (*oam_sdm_signal_handler);
}


size_t oam_sdm_strlen(const char *s)
{
        return strlen(s);
}

int oam_sdm_snprintf(char *str, size_t size, const char *format, ...)
{
    int status = OAM_INVALID_RETURN;
    va_list args;
    OAM_SDM_VA_START(args,format);
    status = vsnprintf(str,size,format,args);
    OAM_SDM_VA_END(args);
    return status;
}

int oam_sdm_sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask)
{
    return sched_setaffinity(pid, cpusetsize,mask);
}
/* SPR 22371 Fix Start */
/*************************************************************************
 * Function Name  : oam_sdm_popen
 * Description    : This is a wrapper function to popen
 * Inputs         : Command, read/write mode
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
FILE *oam_sdm_popen(const Char8 *p_cmd, const Char8 *p_mode)
{
    return popen(p_cmd, p_mode);
}

/*************************************************************************
 * Function Name  : oam_sdm_pclose
 * Description    : This is a wrapper function for pclose
 * Inputs         : File stream
 * Outputs        : none
 * Returns        : Return type to system call
 *****************************************************************************/
int oam_sdm_pclose(FILE *p_stream)
{
    return pclose(p_stream);
}
/* SPR 22371 Fix End */


