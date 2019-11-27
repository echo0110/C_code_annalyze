/*********************************************************************************    
**********************************************************************************
**    FILE NAME:
**        logger.h
**
**    DESCRIPTION:	Contains all logger apis 
**
**    DATE              AUTHOR              REF         REASON
**    ----              ------              ---         ------
**    21Jan03           svaid           ---         Original
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stropts.h>
#include <poll.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/filio.h>

#include "common_nw_util.h"
#include "logger_typ.h"
#include "logger_flrd.h"
#include "logger_utl.h"
#include "logger_nw_recv.h"

#define	 resolve 	inet_addr

#define __EX(oc, ic, rs, m, x) (oc(( (ic(x)) >> rs)&m))
#define YGETBYTE(x,n)   __EX( (U16), (U32), (8*(n)), 0x000000FF, (x) )

#define	SH_HDRSIZE		16
#define SH_GETATTRSIZE(b)       ((unsigned int)yfGetU16At((b), 2))
#define SH_GETPAYLOADSIZE(b)    ((unsigned int)yfGetU16At((b), 4))
#define SH_GETSOURCE(b)         ((U32)yfGetU32At((b), 8))
#define SH_GETDESTINATION(b)    ((U32)yfGetU32At((b), 12))

typedef struct  {
        U32   s;
        U32   us;
}       QTIME;

/* Logger API	*/
/* Assuming always sending stack calls qvLadderDump(api_id,src_id,dst_id) to avoid duplication so api_name is always searched in src_id's APIs listed*/

typedef struct
{
	U16	api_id;
	U16	src_id;
	U16	dst_id;
}logger_api_traversal_info_t;

#define 	LOGGER_MODULE_ID	999
#define		LOGGER_API_SIZE		6

#define 	MAX_MOD_NAME		15
#define		MAX_API_NAME		60
#define		LOGGER_MIN_MODULES	2
#define		LOGGER_MAX_MODULES	200
#define		LOGGER_TIMESTAMP_LEN	24
#define		DELTASPACE		5

typedef struct
{
	U16	api_id;
	U8	api_name[MAX_API_NAME];
}api_list_t;

typedef struct
{
	U16	mod_id;
	U8	mod_name[MAX_MOD_NAME];
	U16	num_apis;
}module_info_t;

typedef struct
{
	module_info_t	mod_info;
	U8		api_info_present;
	api_list_t	*api_list;
	U16		ladder_pos;
	U16		cursor_x_pos;
}logger_glob_data_t;

typedef struct
{
	S16				num_client_processes;
	S16				num_modules;
#ifdef TCP
	S32				*p_fd;
#endif
}logger_cfg_params_t;

/* Logger Trace Info */

typedef struct
{
	U16	succ_diff_cur_mod;
	U8	api_name[MAX_API_NAME];
	U16	x_cur_pos_src;
	U16	x_cur_pos_dst;
	U16	x_diff_src_dst;
	U16	start_cur;
	U16	end_cur;
	U16	api_start_cur;
	S8	space_str[MAX_API_NAME];
	S8	hyphen_str[MAX_API_NAME];
	U8	arrow;
}logger_trace_info_t;

/* Print Info */
typedef struct rec_title
{
	S8	*timestamp_string;
	S8	*mod_names;
}record_title_t;

typedef struct rec_ladder
{
	S8	*timestamp_string;
	S8	*ladder_str;
}record_ladder_t;

extern void logger_port_trace(const S8 *fmt_str, ...);
#ifdef ENABLE_TRACE
        #define LOGGER_TRACE(arg)  logger_port_trace arg
#else
        #define LOGGER_TRACE
#endif

/* Global variables */

extern	U8               		buffer[];
extern  logger_glob_data_t		*p_glob_data;
extern	logger_api_traversal_info_t	api_traversal_info;
extern	logger_trace_info_t		trace_info;
extern	logger_cfg_params_t		cfg_params;
extern	U16				sender;
extern 	U32				line_count;
extern	conf_file_t      		conf_file;
extern	FILE				*p_log_file;

#endif /*__LOGGER_H__*/
	
