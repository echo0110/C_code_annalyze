/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_port.c
 *
 * Description : This file contains Qcli and socket based programming functions
 *
 * Revision History:
 *
 * Date              Author          Reference                Comments
 * -----             ------          ---------                --------
   July,2016	Siddhant Prakash			     Initial Version
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/
/*******************************************************************************
 * Includes
 *** ***************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



#include "oam_agnt_utils.h"
#include "lteTypes.h"
#include "oam_agnt_port.h"
/*******************************************************************************
  Global Declaration
 *******************************************************************************/

/*******************************************************************************
  Private Definitions
 *******************************************************************************/

/****************************************************************************
 * Function Name  : oam_agnt_htons
 * Inputs         : hostshort :unsigned int
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used as a wrapper function for htons().
****************************************************************************/
unsigned int oam_agnt_htons(unsigned int hostshort)
{
    return htons(hostshort);
}

/****************************************************************************
 * Function Name  : oam_agnt_memset
 * Inputs         : s :Pointer to node
 *                  c :int
 *                  n :size_t variable
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to memset.
****************************************************************************/
void *oam_agnt_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}


/****************************************************************************
 * Function Name  : oam_agnt_perror
 * Inputs         : string :string
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for perror.
****************************************************************************/       
void oam_agnt_perror(const char *string)
{
    perror(string);   /* klocwork fix  */
} 


/****************************************************************************
 * Function Name  : oam_agnt_malloc
 * Inputs         : size :size
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for malloc.
****************************************************************************/
void *oam_agnt_malloc (UInt32 size)
{
    return malloc (size);
}


/****************************************************************************
 * Function Name  : oam_agnt_free
 * Inputs         : m :Pointer to void
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to free node
 ****************************************************************************/     
void
oam_agnt_free (void *m)
{
    free (m);
}


/****************************************************************************
 * Function Name  : oam_agnt_vprintf
 * Inputs         : format :format string
 *                  A :va_list
 * Outputs        : None
 * Returns        : Int
 * Description    : This function is used to vfprintf
****************************************************************************/      
Int
oam_agnt_vprintf (const Char8 * format, va_list A)
{
    return vfprintf (stderr, format, A);
}


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

/*SPR 21889 Start*/
	
/******************************************************************************
 *   FUNCTION NAME: oam_agnt_fork
 *   RETURNS: Process Id
 *   INPUTS : NONE
 *   DESCRIPTION:function for forking.
 ******************************************************************************/
    pid_t
oam_agnt_fork (void)
{
    return fork ();
}

/******************************************************************************
 *   FUNCTION NAME: oam_agnt_getpid
 *   RETURNS: Process id.
 *   INPUTS : NONE
 *   DESCRIPTION:function to get process id.
 ******************************************************************************/

    pid_t
oam_agnt_getpid (void)
{
    return getpid ();
}

/******************************************************************************
 *   FUNCTION NAME: oam_agnt_execv
 *   RETURNS: process
 *   INPUTS : filename :filename
 *            argv :arguement array
 *            envp :envp array
 *   DESCRIPTION:function to allocate message allocate byte size.
 ******************************************************************************/
/* KLOCWORK warning fix start */
int
oam_agnt_execve (const char *filename, char *const argv[], char *const envp[])
{
    /* coverity 35995 */
    return execve (filename, argv, envp);
/* KLOCWORK warning fix end */
}

/****************************************************************************
 * Function Name  : oam_agnt_fprintf
 * Inputs         : str,
 *                  format
 * Outputs        : ststus
 * Returns        : None
 * Description    : This function is used to fprintf.
 ****************************************************************************/
int oam_agnt_fprintf(FILE * str, const char *format, ...)
{
  int status = OAM_AGNT_FAILURE;
  va_list args;
  OAM_AGNT_VA_START(args,format);
  status=vfprintf(str,format,args); //WG
  OAM_AGNT_VA_END(args);
  return status;
}


/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_sprintf
 *   RETURNS        : status
 *   INPUTS         : str :string
 *                    variable arg list
 *   DESCRIPTION    :Prototype of sprintf. 
 ******************************************************************************/
    int
oam_agnt_sprintf (char *str, const char *format, ...)
{
    int status = OAM_AGNT_ZERO;
    va_list args;
    OAM_AGNT_VA_START (args, format);
    status = vsprintf (str, format, args);
    OAM_AGNT_VA_END (args);
    return status;
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_fopen
 * RETURNS        : File Pointer
 * INPUTS         : filename: name of the file
 *                  mode : file mode
 * DESCRIPTION    :function to open a file.
 ******************************************************************************/

FILE *oam_agnt_fopen(const Char8 *filename , const Char8 *mode )
{
return fopen(filename, mode);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_fclose
 * RETURNS        : None 
 * INPUTS         : File stream
 * DESCRIPTION    :function to close a file.
 ******************************************************************************/

void oam_agnt_fclose(FILE *file_ptr)
{
     fclose(file_ptr);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_fflush
 * RETURNS        : return value of the system call
 * INPUTS         : File Pointer
 * DESCRIPTION    :function to close a file.
 ******************************************************************************/

int oam_agnt_fflush(FILE *file_ptr)
{
     return fflush(file_ptr);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_socket_close
 * RETURNS        : system call return type
 * INPUTS         : fd
 * DESCRIPTION    :function to close socket descriptor
 ******************************************************************************/

int oam_agnt_socket_close(int fd)
{
    return close(fd);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_sleep
 * RETURNS        : system call return type
 * INPUTS         : time in seconds
 * DESCRIPTION    :function to sleep system call
 ******************************************************************************/
UInt32 oam_agnt_sleep(UInt32 time)
{
    return sleep(time);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_strcpy
 * RETURNS        : Pointer to string
 * INPUTS         : dest :Destination string
 *                  src :source string
 * DESCRIPTION    :function to copy string.
 ******************************************************************************/

char *oam_agnt_strcpy(char *dest, const char *src)
{
   return strncpy(dest,src,oam_agnt_strlen(src)+OAM_AGNT_ONE);
}

/******************************************************************************
 * FUNCTION NAME  : oam_agnt_strcmp
 * RETURNS        : integer.
 * INPUTS         : s1 :string 1
 *                  s2 :string2 
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
int oam_agnt_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1,s2);
}

/*****************************************************************************
 * FUNCTION NAME  :oam_agnt_strtok 
 * RETURNS        : Pointer to string
 * INPUTS         : str :source str 
 *                  delim :delimiter
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
Char8 *
oam_agnt_strtok(char *str, const char *delim)
{
    return strtok(str,delim);
}


/*****************************************************************************
 * FUNCTION NAME  : oam_agnt_strcat 
 * RETURNS        : string 
 * INPUTS         : dest :Destination str
 *                  src :Source str
 * DESCRIPTION    :function to compare string.
 ******************************************************************************/
    char *
oam_agnt_strcat(char *dest, const char *src)
{
    return strncat(dest,src,oam_agnt_strlen(src)+OAM_AGNT_ONE);
}

/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_system
 *   RETURNS        : system call status.
 *   INPUTS         : string :constant string.
 *   DESCRIPTION    : Prototype of system call.
 ******************************************************************************/
SInt8
oam_agnt_system (const char *string)
{
    /* coverity 35997 */
    return system (string);
}

/****************************************************************************
 *   FUNCTION NAME  : oam_agnt_remove
 *   RETURNS        : system call status.
 *   INPUTS         : string :constant string.
 *   DESCRIPTION    : Prototype of system call.
 ******************************************************************************/


SInt8
oam_agnt_remove (const char *string)
{

    /* coverity 35996 */
    return remove (string);

}
/***********************************************************************
 * FUNCTION NAME  : oam_agnt_abort
 * RETURNS        : system call status.
 * INPUTS         : none
 * DESCRIPTION    : Prototype of system call.
 * ******************************************************************************/

void oam_agnt_abort(void)
{
    OAM_AGNT_LOG(AGNT,OAM_AGNT_ERROR, "System Error - abort() called ");
    abort();
}

/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_memcpy
 *   RETURNS        : memcpy status.
 *   INPUTS         : dest :destination string
 *                    src :source string 
 *                    size :size
 *   DESCRIPTION    :Prototype of memcpy.
 ******************************************************************************/
    void *
oam_agnt_memcpy (void *dest, const void *src, size_t n)
{
    return memcpy (dest, src, n);
}

/**************************************************************************
 * Function Name  : oam_agnt_kill
 * Inputs         : proc_id :process id
 *                : signal :signal to be send
 * Outputs        : None 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None
 * Description    : wrapper fn to kill(pid,signal)
 ****************************************************************************/
oam_agnt_return_et
oam_agnt_kill        
(                       
 pid_t proc_id,     
 UInt16 signal
 )           
{           
    OAM_AGNT_FUN_ENTRY_TRACE();
    oam_agnt_kill_all_child(proc_id, signal);
/* SPR_18609 Fix Start */
    if(kill(proc_id,signal) == OAM_AGNT_ZERO)
    {                 
/* SPR_18609 Fix End */
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO,
                "PID : %d killed successfully",proc_id);
        OAM_AGNT_FUN_EXIT_TRACE();
        return OAM_AGNT_SUCCESS;
    }       
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING,
                "Unable to kill process having pid : %d with error no.-%d",
                proc_id,errno);
        OAM_AGNT_FUN_EXIT_TRACE();
        return OAM_AGNT_FAILURE;
    }
}

/**************************************************************************
 * Function Name  : oam_agnt_kill_all_child
 * Inputs         : proc_id :process id of the parent process
 *                : signal :signal to be send
 * Outputs        : None 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None
 * Description    : wrapper fn to kill all child of parent process
 ****************************************************************************/
oam_agnt_return_et
oam_agnt_kill_all_child        
(                       
 pid_t proc_id,     
 UInt16 signal
 )           
{           
    FILE *fp;
    Char8 line[OAM_AGNT_MAX_CMD_SIZE];
    Char8 cmd[OAM_AGNT_MAX_CMD_SIZE];
    pid_t child_proc_id = OAM_AGNT_ZERO;

    OAM_AGNT_FUN_ENTRY_TRACE();
    oam_agnt_snprintf(cmd, OAM_AGNT_MAX_CMD_SIZE, "pgrep -P %d", proc_id);
    fp = oam_agnt_popen(cmd, "r");
    if(OAM_AGNT_NULL == fp)
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,"popen returned failure with errno=%d"
                "Cannot kill child of process %d",errno, proc_id);
        OAM_AGNT_FUN_EXIT_TRACE();
        return OAM_AGNT_FAILURE; 
    }
    while (oam_agnt_fgets(line, sizeof(line), fp) != OAM_AGNT_NULL)
    {
        child_proc_id = oam_agnt_atoi(oam_agnt_strtok(line, " "));
        /* SPR_18609 Fix Start */
        if(oam_agnt_kill(child_proc_id,signal) == OAM_AGNT_ZERO)
        {                 
            /* SPR_18609 Fix End */
            OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO,
                    "Child PID %d killed successfully of Process PID %d",
                    child_proc_id, proc_id);
        }       
        else
        {
            OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING,
                    "Unable to kill child process having pid %d of process %d with error no %d",
                    child_proc_id, proc_id, errno);
        }
    }
    oam_agnt_pclose(fp);
    OAM_AGNT_FUN_EXIT_TRACE();
    return OAM_AGNT_SUCCESS; 
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
    int                 result = OAM_AGNT_ZERO;;
    char               *str2   = OAM_AGNT_NULL;
    /* SPR22031 Start */
    oam_agnt_error_code_et error_code          = OAM_AGNT_NO_ERROR;
    /* SPR22031 End */

    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(( n + OAM_AGNT_ONE),(void *)&str2,&error_code))
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,
                "Memory allocation to str2 failed"
                " with Error Code:%d",
                error_code);
        OAM_AGNT_FUN_EXIT_TRACE();
        /* Not Enough Memory */
        return OAM_AGNT_ZERO;
    }

    oam_agnt_strncpy (str2, str, n);
    str2[n] = OAM_AGNT_ZERO;
    OAM_AGNT_VA_START (args, format);
    result = vsscanf (str2, format, args);
    OAM_AGNT_VA_END (args);
    
    /* Free Memory Allocated to STR2 */
    qvMsgFree(str2);
 
    return result;
}

/*************************************************************************
 * Function Name  : oam_agnt_exit
 * Description    : This is wrapper for system call exit
 * Inputs         : status to be provided on exit
 * Outputs        : none
 * Returns        : None
 ******************************************************************************/
void oam_agnt_exit(int status)
{
   exit(status);
}

/*************************************************************************
 * Function Name  : oam_agnt_strnlen
 * Description    : This is a wrapper function for system call strnlen
 * Inputs         : String and maximum len
 * Outputs        : none
 * Returns        : Return type by system call
 ******************************************************************************/
size_t oam_agnt_strnlen(const char *s, size_t maxlen)
{
    return strnlen(s, maxlen);
}

/*************************************************************************
 * Function Name  : oam_agnt_signal
 * Description    : This is a wrapper function for signal handler registration
 * Inputs         : Signal number and signal handler
 * Outputs        : none
 * Returns        : Return type by system call
 ******************************************************************************/
sighandler_t oam_agnt_signal(int signum, sighandler_t handler)
{
    return signal(signum, handler);
}
    
/*************************************************************************
 * Function Name  : oam_agnt_strncpy
 * Description    : This is a wrapper function for stncpy
 * Inputs         : Source str, Dest Str and len
 * Outputs        : none
 * Returns        : Return type by system call 
 ******************************************************************************/
char *oam_agnt_strncpy(Char8 *s1, const Char8 *s2, size_t n)
{
    if((s1 != NULL) && (s2 != NULL)){
      return strncpy(s1,s2, n);
    }
    else
    { 
     return OAM_AGNT_ZERO;
    }
}

/*************************************************************************
 * Function Name  : oam_agnt_fgets
 * Description    : This is a wrapper function to fgets
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : Return type by system call
 ******************************************************************************/
char *oam_agnt_fgets(char *s, int size, FILE *stream)
{
    return fgets(s,size,stream);
}

/*************************************************************************
 * Function Name  : oam_agnt_atoi
 * Description    : This is a wrapper function to atoi
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
int oam_agnt_atoi ( const char *str)
{
    if(OAM_AGNT_NULL != str)
    {
        return atoi(str);
    }
    return OAM_AGNT_ZERO;
}

/*************************************************************************
 * Function Name  : oam_agnt_time
 * Description    : This is a wrapper function to time
 * Inputs         : time structure pointer
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
time_t oam_agnt_time(time_t *t)
{
    return time(t);
}

/*************************************************************************
 * Function Name  : oam_agnt_localtime
 * Description    : This is a wrapper function to local time
 * Inputs         : Time structure
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
struct tm *oam_agnt_localtime(const time_t *timep)
{
    return localtime(timep);
}

/*************************************************************************
 * Function Name  : oam_agnt_localtime_r
 * Description    : This is used for sending message to external modules
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
struct tm *oam_agnt_localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_r(timep, result);
}

/*************************************************************************
 * Function Name  : oam_agnt_strlen
 * Description    : This is a wrapper function to strlen
 * Inputs         : String
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
size_t oam_agnt_strlen(const char *s)
{
        return strlen(s);
}

/*************************************************************************
 * Function Name  : oam_agnt_snprintf
 * Description    : This is a wrapper function for snprintf
 * Inputs         : String, size, format and variable arguments
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_snprintf(char *str, size_t size, const char *format, ...)
{
    int status = OAM_INVALID_RETURN;
    va_list args;
    OAM_AGNT_VA_START(args,format);
    status = vsnprintf(str,size,format,args);
    OAM_AGNT_VA_END(args);
    return status;
}

/*************************************************************************
 * Function Name  : oam_agnt_sched_setaffinity
 * Description    : This is a wrapper function to set the affinity
 * Inputs         : PID, CPU set, Mask
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask)
{
    return sched_setaffinity(pid, cpusetsize,mask);
}

/*************************************************************************
 * Function Name  : oam_agnt_access
 * Description    : This is a wrapper function to access
 * Inputs         : filename and the mode to be checked
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_access(const Char8 *p_pathname, int mode)
{
    return access(p_pathname, mode);
}

/*************************************************************************
 * Function Name  : oam_agnt_chmod
 * Description    : This is a wrapper function to change access mode of the file
 * Inputs         : filename and the mode to be set
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_chmod(const char *p_pathname, mode_t mode)
{
    return chmod(p_pathname, mode);
}

/*************************************************************************
 * Function Name  : oam_agnt_waitpid
 * Description    : This is a wrapper function to waitpid
 * Inputs         : PID, status and options to be provided
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
pid_t oam_agnt_waitpid(pid_t pid, int *p_status, int options)
{
    return waitpid(pid, p_status, options); 
}

char *oam_agnt_get_cwd(char *p_buf, size_t size)
{
    return  getcwd(p_buf, size);
}

/*************************************************************************
 * Function Name  : oam_agnt_popen
 * Description    : This is a wrapper function to popen
 * Inputs         : Command, read/write mode
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
FILE *oam_agnt_popen(const Char8 *p_cmd, const Char8 *p_mode)
{
    return popen(p_cmd, p_mode);
}

/*************************************************************************
 * Function Name  : oam_agnt_pclose
 * Description    : This is a wrapper function for pclose
 * Inputs         : File stream
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_pclose(FILE *p_stream)
{
    return pclose(p_stream);
}

/*************************************************************************
 * Function Name  : oam_agnt_rename
 * Description    : This is a wrapper function for rename
 * Inputs         : old name and new name
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_agnt_rename(Char8 *p_old_name, Char8 *p_new_name)
{
    return rename(p_old_name, p_new_name);
}

/*************************************************************************
 * Function Name  : oam_agnt_strerror
 * Description    : This is a wrapper function for strerror
 * Inputs         : Errno value set by system call
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
char *oam_agnt_strerror(int errnum)
{
    return strerror(errnum);
}
const QSYSOP my_os = {
    oam_agnt_malloc,
    oam_agnt_free,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    OAM_AGNT_ZERO,
    oam_agnt_vprintf,
    oam_agnt_vprintf,
    oam_agnt_abort
};

const QSYSOP *os = &my_os;

