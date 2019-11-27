#ifndef LTE_PC_TEST
#define LTE_PC_TEST
#define RRM_WARNING 0
#define RRM_MEMSET memset
#define RRM_MEMCPY memcpy
#define RRM_TRACE 
#define RRM_COMP_PARSE 0
#define RRM_DETAILED 0
#define RRM_INFO 0
#define RRM_ERROR 0
#define RRM_ASSERT 
#define RRC_WARNING 10
#define RRC_MEMSET memset
#define RRC_MEMCPY memcpy
#define RRC_TRACE(level,format,...) if(RRC_WARNING<=level)printf(format"\n",##__VA_ARGS__);
#define RRC_COMP_PARSE 0
#define RRC_DETAILED 0
#define RRC_INFO 0
#define RRC_ERROR 11
#define RRC_ASSERT 
#define SET_CELL_INDEX 
#include "winsock.h"
#include "string.h"
#include "stdio.h"
#include	<cspl.h>

#include "rrm_api_defines.h"
#include "rrc_defines.h"
#include "rrc_cp_common.h"
#include "rrc_msg_mgmt.h"

typedef U8 rrm_cell_index_t;

#define memset_wrapper memset
#define l3_memcpy_wrapper memcpy
#define rrc_send_message(p_msg,dst_module)

#define rrm_construct_api_header rrc_construct_api_header
#define rrm_construct_interface_api_header rrc_construct_interface_api_header
#define rrm_send_message(...)

void*
rrm_msg_mem_get
(
 rrm_size_t size /* Size of buffer which will be allocated */
 );
void
rrm_msg_mem_free
(
	void* p_buffer /* Pointer to buffer which will be freed */
);
void*
rrc_msg_mem_get
(
	rrc_size_t size /* Size of buffer which will be allocated */
);
void
rrc_msg_mem_free
(
	void* p_buffer /* Pointer to buffer which will be freed */
);

typedef enum
{
	OAM_FAILURE = 0,
	OAM_SUCCESS
} oam_return_et;

typedef enum _log_priority_et
{
	OAM_CRITICAL  ,        /* Critical conditions, such as hard device errors */
	OAM_ERROR  ,              /* Error messages */
	OAM_WARNING  ,          /* Warning messages */
	OAM_INFO  ,                /* Info about the resources*/
	OAM_BRIEF  ,              /* Brief about the resources*/
	OAM_DETAILED  ,        /* Detail about all the messg */
	OAM_DETAILEDALL,  /* Detail including comp parse logs about all the messg */
} log_priority_et;
typedef char oam_error_code_et;
#define OAM_NULL_CHECK(x)

#define oam_log_message(...)
#define oam_rrm_send_message(...)
#include "rrc_ext_api.h"
void* oam_msg_mem_get(size_t size);
void *oam_memset(void *s, int c, size_t n);
oam_return_et
oam_mem_free
(
 void *p_buffer, /* Pointer to buffer which will be freed */
 oam_error_code_et *p_error_code
 );

#define oam_construct_api_header rrc_construct_api_header
#define oam_construct_interface_api_header rrc_construct_interface_api_header
#define oam_cp_pack_UInt16 rrc_cp_pack_U16
#define OAM_CP_PACK_UINT16 rrc_cp_pack_U16
#define OAM_CP_PACK_UINT8 rrc_cp_pack_U8
#define OAM_CP_PACK_UINT32 rrc_cp_pack_U32
#define OAM_CP_PACK_SINT8 rrc_cp_pack_S8
#define OAM_CP_PACK_SINT32 rrc_cp_pack_S32
#define OAM_CP_UNPACK_UINT16 rrc_cp_unpack_U16
#define OAM_CP_UNPACK_UINT8 rrc_cp_unpack_U8
#define OAM_CP_UNPACK_UINT32 rrc_cp_unpack_U32
#define OAM_CP_UNPACK_SINT8 rrc_cp_unpack_S8
#define OAM_CP_UNPACK_SINT32 rrc_cp_unpack_S32

//if include winsock2.h, there will be many compilation failure for redefinition error
//so we just define sockaddr_storage here
#if ULONG_MAX > 0xffffffff
# define __ss_aligntype U64
#else
# define __ss_aligntype U32
#endif
#define sa_family_t short
#define _SS_SIZE        128
#define _SS_PADSIZE		(_SS_SIZE - (2 * sizeof(__ss_aligntype)))

struct sockaddr_storage
{
     sa_family_t ss_family;      /* Address family */
     __ss_aligntype __ss_align;  /* Force desired alignment.  */
     char __ss_padding[_SS_PADSIZE];
};
#endif
