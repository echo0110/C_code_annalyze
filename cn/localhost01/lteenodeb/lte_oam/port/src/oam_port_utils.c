/*****************************************************************************
 * File Name   : oam_port_utils.c
 *
 * Description : This file contains wrappers of the system/library calls.
 *
 * Revision History:
 *
 * Date               Author           Reference                      Comments
 * -----             ------           ---------                       --------
 * November, 2013    Rashi Arora    System call removal activity    Initial Version
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ****************************************************************************/
#include <oam_defines.h>
#include <oam_types.h>
#include <oam_proto.h>
#include <string.h>
#include <signal.h>
#include <ifaddrs.h>
#include <sys/wait.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <sys/time.h>
#include <sys/resource.h>

int oam_set_interface_info(const char interface[],const char ip_addr[]);
char ipTable[MAX_IP_INTERFACES][MAX_IP_STR_SIZE];
char ipTablev6[MAX_IP_INTERFACES][MAX_IPV6_ADDR_LEN];


/****************************************************************************
 *   FUNCTION NAME:  oam_inet_ntop
 *   RETURNS:        network address.
 *   INPUTS :        constant string.
 *   DESCRIPTION:    This function converts the network address structure src 
 *		     in network address
 ******************************************************************************/

const char *oam_inet_ntop(int af, const void *src,
                             char *dst, socklen_t cnt)
{
    return inet_ntop( af,src,dst, cnt);
}


time_t oam_time(time_t *t)
{
    return time(t);
}


char *oam_ctime(const time_t *timep)
{
    return ctime(timep);
}

struct tm *oam_gmtime(const time_t *timep)
{
    return gmtime(timep);
}

char *oam_strdup(const char *s)
{
    return strdup(s);
}

struct tm *oam_localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_r(timep, result);
}

time_t oam_mktime(struct tm *tm)
{
    return mktime(tm);
}

char *oam_getcwd(char *buf, size_t size)
{
    return getcwd(buf, size);
}

size_t oam_strnlen(const char *s, size_t maxlen)
{
    return strnlen(s, maxlen);
}

char *oam_fgets(char *s, int size, FILE *stream)
{
    return fgets(s,size,stream);
}

size_t oam_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr,size,nmemb,stream);
}

size_t oam_fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int oam_setrlimit(int resource, const struct rlimit *rlim)
{
   /* Coverity 81707 Fix Start */
    return setrlimit(resource, rlim); /* coverity 66647 fix */
   /* Coverity 81707 Fix End */
}

int oam_getchar(void)
{
    return getchar();
}

sighandler_t oam_signal(int signum, sighandler_t handler)
{
    return signal(signum, handler);
}

int oam_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                                              struct addrinfo **res)
{
    return getaddrinfo(node,service,hints,res);
}

int oam_sched_setaffinity(pid_t pid, size_t cpusetsize,
                             cpu_set_t *mask)
{
    return sched_setaffinity(pid, cpusetsize,mask);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_htons
 *   RETURNS:        status
 *   INPUTS :        hostlong
 *   DESCRIPTION:    converts the unsigned integer hostlong from host 
 *		     byte order to network byte order.
 ******************************************************************************/
UInt16 oam_htons(UInt16 hostshort) 
{
    return htons(hostshort);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_strcmp
 *   RETURNS:        status
 *   INPUTS :        source and destination string address
 *   DESCRIPTION:    compares the source and destination string 
 ******************************************************************************/
int oam_strcmp(const char *s1, const char *s2)
{
    OAM_NULL_CHECK(NULL != s1); 
    return strcmp(s1,s2);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_strstr
 *   RETURNS:        status
 *   INPUTS :        source and destination string address
 *   DESCRIPTION:    checks for existence of the string "needle" in source string
 ******************************************************************************/
char * oam_strstr(const char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_memset
 *   RETURNS:        status
 *   INPUTS :        s	- pointer to source address space
 *		     c	- bytes of memory
 *		     n  - size of memory
 *   DESCRIPTION:    memory is set with the mentioned value "c"
 ******************************************************************************/
void *oam_memset(void *s, int c, size_t n)
{
        return memset(s, c, n);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_memcpy
 *   RETURNS:        status
 *   INPUTS :        src  - pointer to source address space
 *		     dest - pointer to destination address space
 *		     n  - size of memory
 *   DESCRIPTION:    function fills the first n bytes of the memory area
 *		     pointed to by dest with the constant byte src
 ******************************************************************************/
void *oam_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_memcmp
 *   RETURNS:        status
 *   INPUTS :        s1  - pointer to source address space
 *		     s2  - pointer to destination address space
 *		     n  - size of memory
 *   DESCRIPTION:    function compares the 2 memory content
 ******************************************************************************/
int oam_memcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_malloc
 *   RETURNS:        status
 *   INPUTS :        size  - size of memory to be allocated
 *   DESCRIPTION:    This function allocates "size" bytes of memory on heap
 ******************************************************************************/
void *oam_malloc (UInt32 size)
{
    return malloc (size);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_free
 *   RETURNS:        NONE
 *   INPUTS :        m  - Pointer to memory to be freed
 *   DESCRIPTION:    This function frees the memory pointed by pointer m
 ******************************************************************************/
void
oam_free (void *m)
{
    free (m);
}
/* SPR 14997 START */
int
oam_abs (int num)
{
    return abs(num);
}
/* SPR 14997 END */

/****************************************************************************
 *   FUNCTION NAME:  oam_snprintf
 *   RETURNS:        status
 *   INPUTS :        str - character buffer to be filled
 *   DESCRIPTION:    This functions vsnprintf() write at most size bytes
 *		     to str
 ******************************************************************************/
/* SPR 12698 FIX START */
int oam_snprintf(char *str, size_t n, const char *format, ...)
{
   int status = OAM_INVALID_RETURN;
   va_list args;
   OAM_VA_START(args,format);
   /* Coverity FIX 53741*/
   status = vsnprintf(str,n,format,args);
   OAM_VA_END(args);
   return status;
}
/* SPR 12698 FIX END */

/****************************************************************************
 *   FUNCTION NAME:  oam_sprintf
 *   RETURNS:        status
 *   INPUTS :        str - character buffer to be filled
 *   DESCRIPTION:    This functions write at most size bytes
 *		     to str
 ******************************************************************************/
int oam_sprintf(char *str, const char *format, ...)
{
  int status = OAM_INVALID_RETURN;
  va_list args;
  OAM_VA_START(args,format);
  /*Coverity 97513 Fix*/
  status=vsprintf(str,format,args);
  OAM_VA_END(args);
  return status;
}

/****************************************************************************
 *   FUNCTION NAME:  oam_fprintf
 *   RETURNS:        status
 *   INPUTS :        str - character buffer to be filled
 *   DESCRIPTION:    This functions write at most size bytes
 *		     to str
 ******************************************************************************/

int oam_fprintf(FILE * str, const char *format, ...)
{
  int status = OAM_INVALID_RETURN;
  va_list args;
  OAM_VA_START(args,format);
  status=vfprintf(str,format,args); 
  OAM_VA_END(args);
/*SPR 22588 Fix Start*/
  oam_fflush(str);
/*SPR 22588 Fix End*/
  return status;
}

/****************************************************************************
 *   FUNCTION NAME:  oam_fprintf
 *   RETURNS:        status
 *   INPUTS :        str - character buffer to be filled
 *   DESCRIPTION:    This functions scans input according to format
 ******************************************************************************/
int snscanf(const char *str, int n, const char *format, ...) 
{
    va_list             args;
    int                 result = OAM_FAILURE;
    char               *str2 = OAM_NULL;
    oam_error_code_et error_code = NO_ERROR;

   if (OAM_FAILURE == oam_mem_alloc((n + OAM_ONE),(void *)&str2,&error_code))
   {
       OAM_LOG(OAM, OAM_ERROR,
               "Memory allocation to str2 failed"
               " with Error Code:%d",
               error_code);
	   OAM_FUN_EXIT_TRACE();
	   /* Not Enough Memory */
	   return OAM_FAILURE;
    }

    strncpy (str2, str, n);
    str2[n] = OAM_ZERO; 
    OAM_VA_START (args, format);
    result = vsscanf (str2, format, args);
    OAM_VA_END (args);

    oam_mem_free(str2, &error_code);
    return result;
}

/****************************************************************************
 *   FUNCTION NAME:  oam_strtok
 *   RETURNS:        status
 *   INPUTS :        s - character string to parse
 *		     delin - delimiter
 *   DESCRIPTION:    This functions function breaks a string into a sequence 
 *		     of zero or more tokens
 ******************************************************************************/
char *oam_strtok(char *s, const char *delim)
{
    return strtok(s, delim);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strcpy
 *   RETURNS:        status
 *   INPUTS :        src - source string
 *		     dest - destination string
 *   DESCRIPTION:    This functions copies the string pointed to by src
 *		     to the buffer pointed to by dest
 ******************************************************************************/
char *oam_strcpy(char *dest, const char *src)
{
    return strncpy(dest,src,oam_strlen(src)+OAM_ONE);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strcat
 *   RETURNS:        status
 *   INPUTS :        str1 - source string
 *		     str2 - destination string
 *   DESCRIPTION:    This functions concatinates string str2 to end of str1 
 ******************************************************************************/
void *oam_strcat(void *str1, const void *str2)
{
    return strncat(str1, str2, oam_strlen(str2)+OAM_ONE);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strncat
 *   RETURNS:        status
 *   INPUTS :        str1 - source string
 *		     str2 - destination string
 *		     n	  - size to be copied in bytes
 *   DESCRIPTION:    This functions concatinates n bytest of string str2 to end 
 *		     of str1 
 ******************************************************************************/
Char8* oam_strncat(Char8 *str1, const Char8 *str2, size_t n)
{
    return strncat(str1, str2, n);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strncasecmp
 *   RETURNS:        status
 *   INPUTS :        s1 - source string
 *		     s2 - destination string
 *		     n  - number of bytes to be compared
 *   DESCRIPTION:    This functions compares n bytes from string s1 to string s2 
 ******************************************************************************/
int oam_strncasecmp(const char *s1, const char *s2, size_t n)
{
        return  strncasecmp(s1, s2, n);
}

/*SPR 21889 Start*/
/****************************************************************************
 * Function Name  : oam_strerror
 * Inputs         : errno
 * Outputs        : None
 * Returns        : None
 * Description    : This function returns the error string
****************************************************************************/       
char* oam_strerror(int err)
{
    return strerror(err);   
} 
/*SPR 21889 End */

/****************************************************************************
 *   FUNCTION NAME:  oam_strcasecmp
 *   RETURNS:        status
 *   INPUTS :        str1 - source string
 *		     str2 - destination string
 *   DESCRIPTION:    This functions compares string s1 from string s2
 ******************************************************************************/
SInt32 oam_strcasecmp(const Char8 *s1, const Char8 *s2)
{
    SInt32 retVal = 1;

    if(s1 && s2) {
        retVal = strcasecmp(s1, s2);
    }
    else {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error!");
    }

    return retVal;
}
/****************************************************************************
 *   FUNCTION NAME:  oam_fork
 *   RETURNS:        process pid
 *   INPUTS :        
 *   DESCRIPTION:    This functions forks a new process
 ******************************************************************************/
oam_pid_t oam_fork(void)
{
    return fork();
}

/****************************************************************************
 *   FUNCTION NAME:  oam_getpid
 *   RETURNS:        process pid
 *   INPUTS :        
 *   DESCRIPTION:    returns the process ID of the calling process
 ******************************************************************************/

oam_pid_t oam_getpid(void)
{
    return getpid();
}
/****************************************************************************
 *   FUNCTION NAME:  oam_execve
 *   RETURNS:        status
 *   INPUTS :        filename - filename
 *		     argv     - array of argument strings
 *		     envp     - array of strings, of the form key=value
 *   DESCRIPTION:    executes the program pointed to by filename, filename must
 *		     be either a binary executable, or a script
 ******************************************************************************/
int  oam_execve(const  char  *filename,  char  *const argv [], char *const envp[])
{
    return  execve(filename,argv ,envp);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_system
 *   RETURNS:        status
 *   INPUTS :        
 *   DESCRIPTION:    executes the system command
 ******************************************************************************/
int oam_system(const char *string)
{
    return system(string);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_getifaddrs
 *   RETURNS:        status
 *   INPUTS :        ifap - interface name
 *   DESCRIPTION:    function creates a linked list of structures
	             describing the network interfaces of the local system
 ******************************************************************************/
int oam_getifaddrs(oam_ifaddrs_t **ifap)
{
    return getifaddrs(ifap);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_freeifaddrs
 *   RETURNS:        NONE
 *   INPUTS :        
 *   DESCRIPTION:    frees link list node describing the network interfaces 
 *		     of the local system
 ******************************************************************************/
void oam_freeifaddrs(oam_ifaddrs_t *ifa)
{
    freeifaddrs(ifa);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_perror
 *   RETURNS:        process pid
 *   INPUTS :        string - string message
 *   DESCRIPTION:    This function produces a message on the standard error 
 *		     output
 ******************************************************************************/
void oam_perror(const char *string)
{
    perror(string);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strncpy
 *   RETURNS:        string copied
 *   INPUTS :        s1 - string 1
 *		     s2 - string 2
 *		     n - number of bytes to be copied
 *   DESCRIPTION:    returns the process ID of the calling process
 ******************************************************************************/
char *oam_strncpy(char *s1, const char *s2, size_t n)
{
OAM_NULL_CHECK(s1 != NULL);
OAM_NULL_CHECK(s2 != NULL);
    return strncpy(s1,s2,n);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_fopen
 *   RETURNS:        status 
 *   INPUTS :        filename - file name
 *		     mode - mode of the file to be opened
 *   DESCRIPTION:    this function opens a file
 ******************************************************************************/
FILE *oam_fopen(const Char8 *filename , const Char8 *mode )
{
    return fopen(filename, mode);
}

/****************************************************************************
 **   FUNCTION NAME:  oam_fclose
 **   RETURNS:        status
 **   INPUTS :        filename - file name
 **   DESCRIPTION:    this function closes a file
 *******************************************************************************/
void oam_fclose(FILE *file_ptr)
{
     fclose(file_ptr);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_pow
 *   RETURNS:        result of operation
 *   INPUTS :        x - base
 *		     y - power
 *   DESCRIPTION:    This function calculates x to the power y
 ******************************************************************************/
UInt16 oam_pow(UInt16 x, UInt16 y)
{
 UInt16 z = OAM_ONE;

if(y > OAM_ZERO)
{
while(y > OAM_ZERO)
{
 z*=x;
y--;
}
}
 return z;
}

/****************************************************************************
 *   FUNCTION NAME:  oam_ioctl
 *   RETURNS:        output
 *   INPUTS :        
 *   DESCRIPTION:    This function converts array to long value
 ******************************************************************************/
long long
oam_ioctl
(
    int d,
    unsigned long req,
    void *m
)
{
    OAM_NULL_CHECK(NULL != m);
    return ioctl(d, req, m);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_atoll
 *   RETURNS:        output
 *   INPUTS :        
 *   DESCRIPTION:    This function converts array to long value
 ******************************************************************************/
long long
oam_atoll
(
    const char *str
)
{
    OAM_NULL_CHECK(NULL != str);
    return atoll(str);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_atoi
 *   RETURNS:        output
 *   INPUTS :        
 *   DESCRIPTION:    This function converts array to int value
 ******************************************************************************/
int
oam_atoi
(
    const char *str
)
{
    OAM_NULL_CHECK(NULL != str);/* Coverity FIX 41202 */
    return atoi(str);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_strchr
 *   RETURNS:        output
 *   INPUTS :        str - string to be searched
 *		     var - charcter to be searched
 *   DESCRIPTION:    function returns a pointer to the first occurrence of
 *		     charcter var
 ******************************************************************************/
const char *
oam_strchr
(
    const char *str,
    char var
)
{
    return strchr(str, var);
}
/****************************************************************************
 *   FUNCTION NAME:  oam_strncmp
 *   RETURS	     output
 *   INPUTS:         str1 - string 1 to be compared
 *		     str2 - string 2 to be compared
 *		     length - number of bytes to be compared
 *   DESCRIPTION:    function returns a pointer to the first occurrence of
 *		     charcter var
 ******************************************************************************/
int oam_strncmp(const char *str1, const char *str2, size_t length)
{
    OAM_NULL_CHECK(str1 != NULL);
    OAM_NULL_CHECK(str2 != NULL);
    return strncmp(str1, str2, length);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_abort
 *   RETURS	     output
 *   INPUTS:         
 *   DESCRIPTION:    This function aborts oam process
 ******************************************************************************/
/* + SPR 17439 */
void oam_abort(void)
/* - SPR 17439 */
{
OAM_LOG(OAM,OAM_ERROR, " System Error - oam_abort() called ");
    OAM_EXIT(0);  
}

/****************************************************************************
 * Function Name  : oam_kill
 * Inputs         : process id
 *                : signal to be send
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      :
 * Description    : wrapper fn to kill(pid,signal)
 ***************************************************************************/
oam_return_et
oam_kill
(
    UInt16 proc_id,
    UInt16 signal
)
{
    OAM_FUN_ENTRY_TRACE();
/* SPR_18609 Fix Start */
    if(kill(proc_id,signal) == OAM_ZERO)
    {
/* SPR_18609 Fix End */
        OAM_LOG(OAM, OAM_INFO,
               "Process having pid : %d killed successfully",proc_id);
        OAM_FUN_EXIT_TRACE();
        return OAM_SUCCESS;
    }
    else
    {
        OAM_LOG(OAM, OAM_WARNING,
               "Unable to kill process having pid : %d with error no.-%d",
             proc_id,errno);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : oam_strlen
 * Inputs         : str - string 
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function find s the length of the string passed 
 ***************************************************************************/
oam_length_t
oam_strlen
(
    const char *str
)
{
     int ret_val;
     OAM_NULL_CHECK(str != NULL);
     ret_val = strlen(str);
     return ret_val;
}
/*SPR 22554 Fix Start*/
/****************************************************************************
 * Function Name  : oam_pthread_setname_np
 * Inputs         : 
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sets name of the thread 
 ***************************************************************************/
int
oam_pthread_setname_np
(
pthread_t *thread,
Char8 *threadName
)
{
    return (pthread_setname_np(*thread, (const Char8 *)threadName));
}
/*SPR 22554 Fix End*/
/****************************************************************************
 * Function Name  : oam_pthread_create
 * Inputs         : 
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function creates thread 
 ***************************************************************************/
int
oam_pthread_create
(
pthread_t *thread,
const pthread_attr_t *attr,
void *(*start_routine) (void *),
void *arg
)
{
    return (pthread_create(thread, attr,start_routine, arg));
}

/****************************************************************************
 *   FUNCTION NAME:  oam_remove
 *   RETURNS:        system call status.
 *   INPUTS :        constant string.
 *   DESCRIPTION:    Prototype of system call.
 ******************************************************************************/
SInt32
oam_remove (const char *string)
{           
    /* coverity 35621 */
    return (remove (string));
} 

/****************************************************************************
 *   FUNCTION NAME:  oam_waitpid
 *   RETURNS:        process ID.
 *   INPUTS :        process ID
 *		     process status
 *   DESCRIPTION:    used to wait for state changes in a 
 *		     child of the calling process
 ******************************************************************************/
oam_pid_t 
oam_waitpid(pid_t pid, int *status, int options)
{
    return waitpid(pid,status,options);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_inet_pton
 *   RETURNS:        status
 *   INPUTS :        af	 - address family
 *		     src - source address before conversion
 *		     dst - destination address after conversion
 *   DESCRIPTION:    This function converts the character string
 *		     src into a network address structure
 ******************************************************************************/
int oam_inet_pton(int af, const char * src, void * dst)
{
    return inet_pton(af, src, dst);
}

/****************************************************************************
 *   FUNCTION NAME:  oam_atol
 *   RETURNS:        status
 *   INPUTS :        str - charcter string to be converted
 *   DESCRIPTION:    converts the initial portion of the string
		     pointed to by nptr to int
 ******************************************************************************/

UInt32
oam_atol(const char *str)
{
    OAM_NULL_CHECK(NULL != str);
    return atol(str);
}

LOCAL char int_list_g[MAX_INTF_LIST_SIZE];
/****************************************************************************
 * Function Name  : oam_create_address_info
 * Description    : This function creates the list of available ethernet
 *                  Interfaces
 * Inputs         : p_ifa :Pointer to structure oam_ifaddrs_t
 * Outputs        : none
 * Returns        : none
******************************************************************************/
void
oam_create_address_info
(
    oam_ifaddrs_t *p_ifa
)
{
		OAM_FUN_ENTRY_TRACE();
		oam_sockaddr_ipfv4 *p_s4 = OAM_NULL;
		oam_sockaddr_ipv6 *p_s6 = OAM_NULL;
		char host[MAX_BUF_IPADDR_SIZE];
		/* ipv6 addresses have to fit in this buffer */
		char buf[MAX_BUF_IPADDR_SIZE];
		if (p_ifa == OAM_NULL)
		{
				OAM_LOG(OAM,OAM_WARNING," null value received");
				OAM_FUN_EXIT_TRACE();
		}
		else
		{
				if (AF_INET == p_ifa->ifa_addr->sa_family)
				{
						p_s4 = (oam_sockaddr_ipfv4 *)(p_ifa->ifa_addr);
						if (OAM_NULL == oam_inet_ntop(p_ifa->ifa_addr->sa_family,(void *)&(p_s4->sin_addr),
												buf, sizeof(buf)))
						{
								OAM_LOG(OAM,OAM_WARNING,"%s: inet_ntop failed!",p_ifa->ifa_name);
						}
						else
						{
								oam_sprintf(int_list_g, "%s %s",int_list_g,p_ifa->ifa_name);
								OAM_LOG(OAM,OAM_DETAILED,"IPv4 addr %s",buf);
						}
				}
				else if (AF_INET6 == p_ifa->ifa_addr->sa_family)
				{
						p_s6 = (oam_sockaddr_ipv6 *)(p_ifa->ifa_addr);
						if (OAM_NULL == oam_inet_ntop(p_ifa->ifa_addr->sa_family, (void *)&(p_s6->sin6_addr),
												buf, sizeof(buf)))
						{
								OAM_LOG(OAM,OAM_WARNING,"%s: inet_ntop failed!", p_ifa->ifa_name);
						}
						else
						{
								OAM_LOG(OAM,OAM_DETAILED,"IPv6 addr %s ",buf);
						}
				}
				int s = getnameinfo(p_ifa->ifa_addr, (p_ifa->ifa_addr->sa_family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, MAX_BUF_IPADDR_SIZE, OAM_NULL , OAM_ZERO , NI_NUMERICHOST);
				if (s != OAM_ZERO )
				{
						OAM_LOG(OAM, OAM_WARNING, "getnameinfo failed");
				}
				oam_sprintf(int_list_g, "%s %s",int_list_g,host);
		}
		OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 ** Function Name  : oam_set_interface_info
 ** Description    : This function is used to set the info of interface
 ** Inputs         : interface[] :interface
 *                   ip_addr[] :ip address
 ** Outputs        : none
 ** Returns        : OAM_ZERO
 *******************************************************************************/

int
oam_set_interface_info(
    const char interface[],
    const char ip_addr[]
    )
{
    struct ifreq ifr;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(fd != INVALID_ARGUMENT)
    {
       oam_memset(&ifr,OAM_ZERO,sizeof(ifr));
        oam_strncpy(ifr.ifr_name,interface,IFNAMSIZ - OAM_ONE );
        ifr.ifr_addr.sa_family  =  AF_INET;
        struct sockaddr_in* addr=(struct sockaddr_in*)&ifr.ifr_addr;
	oam_inet_pton(AF_INET, ip_addr, (void *)&addr->sin_addr);
        if(oam_ioctl(fd, SIOCSIFADDR, (void *)&ifr)<0)
        {
           OAM_LOG(OAM,OAM_WARNING, "Return value of oam_ioctl function is -1");
           OAM_CLOSE(fd);
           return OAM_INVALID_RETURN;
        }
        OAM_CLOSE(fd);             
    }
    else
    {
        OAM_LOG(OAM,OAM_WARNING, "Error in opening Socket");
    }
    return OAM_ZERO;
}

/****************************************************************************
 ** Function Name  : oam_create_virt_intf
 ** Description    : This function is used to create the virtual interface
 ** Inputs         : p_eth_name :interface name
 *                   p_eth_ip :interface ip address
 ** Outputs        : none
 ** Returns        : none
 *******************************************************************************/
void oam_create_virt_intf
(
    char *p_eth_name,
    char *p_eth_ip
)
{
    OAM_FUN_ENTRY_TRACE();
    int i=OAM_ZERO;
    char eth_name[MAX_BUF_COMMAND_SIZE];
    char eth_ip[MAX_BUF_COMMAND_SIZE];

    oam_memset(eth_name , OAM_ZERO , MAX_BUF_COMMAND_SIZE);
    oam_memset(eth_ip, OAM_ZERO , MAX_BUF_COMMAND_SIZE);

    while(i < (OAM_FOUR- OAM_ONE))
    {
    if (oam_strcmp(p_eth_ip , ipTable[i]) == OAM_ZERO)
    {
        i++;
        continue;
    }
    oam_sprintf(eth_name,"%s:%d ",p_eth_name,i+OAM_TEN);
    oam_sprintf(eth_ip, "%s", ipTable[i]);
    i++;
        if(OAM_INVALID_RETURN == oam_set_interface_info(eth_name,eth_ip))
        {
            OAM_LOG(OAM,OAM_WARNING,"error in applying interface  : %s",eth_ip);
        }
    }
    OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 ** Function Name  : oam_eth_intf_up
 ** Description    : This function calls the routine to check the available
 **                  Interfaces and creates virtual interfaces for it
 ** Inputs         : none
 ** Outputs        : none
 ** Returns        : short int
 *******************************************************************************/
short int
oam_eth_intf_up
(
    void
)
{
    OAM_FUN_ENTRY_TRACE();
    /* COV_FIX_18_JUN */
    oam_ifaddrs_t *p_myaddrs = OAM_NULL , *p_ifa = OAM_NULL ;
    /* COV_FIX_18_JUN */
    int status;
    status = oam_getifaddrs(&p_myaddrs);

    if (OAM_ZERO != status)
    {
        oam_perror("getifaddrs failed!");
        OAM_FUN_EXIT_TRACE();
        return OAM_INVALID_RETURN;
    }

    for (p_ifa = p_myaddrs; p_ifa != OAM_NULL; p_ifa = p_ifa->ifa_next)
    {
        if (OAM_NULL == p_ifa->ifa_addr)
        {
            continue;
        }
        if (OAM_ZERO == (p_ifa->ifa_flags & IFF_UP))
        {
            continue;
        }
        if (OAM_ZERO == !(p_ifa->ifa_flags & IFF_LOOPBACK))
        {
            continue;
        }
        if(AF_INET6 == p_ifa->ifa_addr->sa_family)
        {
            continue;
        }
        if(AF_PACKET == p_ifa->ifa_addr->sa_family)
        {
            continue;
        }
        oam_create_address_info(p_ifa);
     }

    const char delimiters[] = " ";
    char *p_temp = OAM_NULL;
    char *p_temp1 = OAM_NULL;
    p_temp = (char *)oam_strtok( int_list_g, delimiters );
    p_temp1 = (char *)oam_strtok( OAM_NULL, delimiters );
    if(p_temp1 != OAM_NULL)
   oam_create_virt_intf(p_temp, p_temp1);
   oam_freeifaddrs(p_myaddrs);
   OAM_FUN_EXIT_TRACE();
   return OAM_ZERO;
}
/****************************************************************************
 * Function Name  : oam_strtoull
 * Inputs         : str - string 1
 *		    endptr - address of the first invalid character
 *		    base   - base value
 * Outputs        : none
 * Returns        : short int
 * Description    : function converts the initial part of the string in 
 *		    to an unsigned long int value
 *******************************************************************************/
UInt64 oam_strtoull(const Char8* str, Char8** endptr, SInt32 base)
{
    return strtoull(str, endptr, base);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_strrchr
 *   RETURNS:        pointer to last occurence of requested character of NULL
 *   INPUTS :        str - string to be searched
 *	         	     var - charcter to be searched
 *   DESCRIPTION:    function returns a pointer to the last occurrence of
 *		             charcter var
 ***************************************************************************/
Char8* oam_strrchr(Char8 *str, Char8 var)
{
    if (!str) {
        OAM_LOG(OAM, OAM_WARNING, "Null pointer error");
        return str;
    }

    return strrchr(str, var);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_mv
 *   RETURNS:        NONE
 *   INPUTS :        src: source file
 *	         	     dst: destination file
 *   DESCRIPTION:    OAM wrapper function to system mv command
 ***************************************************************************/
void oam_mv(const Char8* src, const Char8* dst)
{
    Char8 sys_cmd_str[OAM_CMD_SIZE] = {OAM_ZERO};
    Char8 *mv_cmd = OAM_MV;
    SInt32 buf_size = OAM_CMD_SIZE - 1;

    oam_strncpy(sys_cmd_str, mv_cmd, buf_size);
    buf_size -= oam_strlen(mv_cmd);

    oam_strncat(sys_cmd_str, " ", buf_size);
    --buf_size;

    oam_strncat(sys_cmd_str, src, buf_size);
    buf_size -= oam_strlen(src);

    oam_strncat(sys_cmd_str, " ", buf_size);
    --buf_size;

    oam_strncat(sys_cmd_str, dst, buf_size);

    oam_system(sys_cmd_str);
}

int oam_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
        return pthread_attr_setstacksize(attr, stacksize);
}

int oam_pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize)
{
        return pthread_attr_getstacksize(attr, stacksize);
}

int oam_inet_aton(const char *cp, struct in_addr *inp)
{
    return inet_aton(cp, inp);
}

int oam_fflush(FILE *stream)
{
    return fflush(stream);
}

struct tm *oam_localtime(const time_t *timep)
{
    return localtime(timep);
}

long oam_strtol(const char *str, char ** endptr, int base)
{
    return strtol(str, endptr, base);
}


/****************************************************************************
 *   FUNCTION NAME:  oam_strtok_r
 *   RETURNS:        string token or OAM_NULL if no further token is found
 *   INPUTS :        s - character string to parse
 *		             delim - delimiter
 *                   save_ptr - pointer to pointer to Char8 for internal use
 *   DESCRIPTION:    This functions function breaks a string into a sequence 
 *		             of zero or more tokens (reinterrant version of 
 *                   oam_strtok
 ***************************************************************************/
Char8* oam_strtok_r(Char8 *s, const Char8 *delim, Char8 **save_ptr)
{
    return strtok_r(s, delim, save_ptr);
}
/*SPR 22398 Fix Start*/
/****************************************************************************
 *   FUNCTION NAME:  oam_ntohs
 *   RETURNS:        integer value
 *   INPUTS :        integer value  
 *   DESCRIPTION:    This function converts function converts the unsigned short
 *                   integer netshort from network byte order to host byte order. 
 ******************************************************************************/
UInt32 oam_ntohs(UInt16 val)
{
    return ntohs(val);
}

/*SPR 22398 Fix End*/
