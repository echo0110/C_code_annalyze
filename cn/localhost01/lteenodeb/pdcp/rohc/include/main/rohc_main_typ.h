/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_main_typ.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the ROHC defined types for the
 *                     main module i.e.. interface module between PDCP and
 *                     ROHC, maily for context relocation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_main_typ.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_MAIN_TYP_H_
#define _ROHC_MAIN_TYP_H_

#include "rohc_d_typ.h"
#include "rohc_main_def.h"

typedef struct rohc_stream_ip6_static_t
{
    rohc_U32bit_t       v_flowlabel_nh;
#define ROHC_IPv6_STATIC_VER_MASK    (0xf0000000U)
#define ROHC_IPv6_STATIC_FLOWLABEL_MASK  (0x0fffff00U)
#define ROHC_IPv6_STATIC_NH_MASK         (0x000000ffU)
    rohc_U32bit_t       src[4];
    rohc_U32bit_t       dst[4];
}rohc_stream_ip6_static_t;

typedef struct rohc_stream_ip4_static_t
{
    rohc_U8bit_t    ver;
#define ROHC_IP4_STATIC_VER_MASK    (0xf0U)
    rohc_U8bit_t    proto;
    rohc_U32bit_t   src;
    rohc_U32bit_t   dst;
}rohc_stream_ip4_static_t;

typedef struct rohc_stream_udp_static_t
{
    rohc_U16bit_t   src;
    rohc_U16bit_t   dst;
}rohc_stream_udp_static_t;

typedef struct rohc_stream_rtp_static_t
{
    rohc_U32bit_t   ssrc;
}rohc_stream_rtp_static_t;

typedef struct rohc_stream_esp_static_t
{
    rohc_U32bit_t   spi;
}rohc_stream_esp_static_t;

typedef struct static_ip
{
	rohc_U8bit_t	ip_version;
	union{
		rohc_stream_ip4_static_t	ip4;
		rohc_stream_ip6_static_t	ip6;
	} ip_union;
}rohc_static_ip_t;

/* STATIC INFO OF ALL 4 PROFILES */ 
typedef struct profile_0
{
	char place_holder;
}profile_0_t;

typedef struct profile_1
{
	rohc_U8bit_t		is_outer_present;	/* To identify whether OUTER IP field is present */
	rohc_static_ip_t	inner_ip;
	rohc_static_ip_t	outer_ip;
	rohc_stream_udp_static_t	udp;
	rohc_stream_rtp_static_t 	rtp; /* RTP info */
} profile_1_t;

typedef struct profile_2
{
	rohc_U8bit_t		is_outer_present;	/* To identify whether OUTER IP field is present */
	rohc_static_ip_t	inner_ip;
	rohc_static_ip_t	outer_ip;
	rohc_stream_udp_static_t	udp;
} profile_2_t;

typedef struct profile_3
{
	rohc_U8bit_t		is_outer_present;	/* To identify whether OUTER IP field is present */
	rohc_static_ip_t	inner_ip;
	rohc_static_ip_t	outer_ip;
    rohc_stream_esp_static_t   esp;            /* ESP */
} profile_3_t;

typedef struct	stream_packet_info
{
	rohc_U8bit_t	cid;		/* CID used */
	rohc_U8bit_t	info_type;	/* whether compressor context or decompressor context */
	rohc_U8bit_t	profile;
	union
	{
		profile_0_t	profile_0;	/*Uncompressed	*/
		profile_1_t	profile_1;	/*RTP/UDP/IP*/
		profile_2_t	profile_2;	/*ROHC UDP -- non-RTP UDP/IP compression (Profile 0x0002) */
		profile_3_t	profile_3;	/*ROHC ESP -- ESP/IP compression (Profile 0x0003)*/
	} profile_union ;
} stream_pkt_info_t;

#endif /* _ROHC_MAIN_TYP_H_ */
