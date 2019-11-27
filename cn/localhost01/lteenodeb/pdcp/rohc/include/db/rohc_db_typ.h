/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_db_typ.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_db_typ.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:02:21  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_DB_TYP_H_
#define _ROHC_DB_TYP_H_

#include "rohc_db_def.h"

typedef rohc_void_t (*p_walker)(rohc_tree_node_t    *, rohc_void_tp );

/* STATIC-DEF fields of IPv4 */
typedef struct rohc_static_def_ip4_t
{
	rohc_U32bit_t	src_addr, dst_addr;
}rohc_static_def_ip4_t;

/* STATIC-DEF fields of IPv6 */
typedef struct rohc_static_def_ip6_t
{
	rohc_U32bit_t	src_addr[4], dst_addr[4], flow_label;
}rohc_static_def_ip6_t;

/* STATIC-DEF fields of RTP */
typedef struct rohc_static_def_rtp_t
{
	rohc_U32bit_t ssrc;
}rohc_static_def_rtp_t;

/* STATIC-DEF fields of UDP */
typedef struct rohc_static_def_udp_t
{
    rohc_U16bit_t src_port;
    rohc_U16bit_t dst_port;
}rohc_static_def_udp_t;

/* STATIC-DEF fields of TCP */
typedef struct rohc_static_def_tcp_t
{
    rohc_U16bit_t src_port;
    rohc_U16bit_t dst_port;
}rohc_static_def_tcp_t;

/* STATIC-DEF fields of ESP */
typedef struct rohc_static_def_esp_t
{
	rohc_U32bit_t	spi;
}rohc_static_def_esp_t;

typedef struct
{
	rohc_U8bit_t	ip_version;
	union{
		rohc_static_def_ip4_t	ipv4;
		rohc_static_def_ip6_t	ipv6;
	}un1;
}rohc_static_def_ip_t;

/* Key fields for Profile 1 (IP/UDP/RTP) */
typedef struct rohc_profile1_t
{
	rohc_static_def_ip_t	ip;
	rohc_static_def_rtp_t	rtp;
	rohc_static_def_udp_t	udp;
}rohc_profile1_t;

/* Key fields for Profile 2 (IP/UDP) */
typedef struct rohc_profile2_t
{
	rohc_static_def_ip_t	ip;
	rohc_static_def_udp_t	udp;
}rohc_profile2_t;

/* Key fields for Profile 3 (IP/ESP) */
typedef struct rohc_profile3_t
{
	rohc_static_def_ip_t	ip;
	rohc_static_def_esp_t	esp;
}rohc_profile3_t;

/* Key fields for Profile 6 (TCP/IP) */
typedef struct rohc_profile6_t
{
	rohc_static_def_ip_t	ip;
	rohc_static_def_tcp_t	tcp;
}rohc_profile6_t;

/* Structure to access all the nodes in DB */
typedef struct	rohc_db_list_t
{
	rohc_U32bit_t	counter;
	rohc_void_tp	p_data[ROHC_MAX_STREAM_CONTEXTS];
}rohc_db_list_t;

#endif /* _ROHC_DB_TYP_H_ */
