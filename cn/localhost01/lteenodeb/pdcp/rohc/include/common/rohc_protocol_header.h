/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_protocol_header.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_protocol_header.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:55  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_PROTOCOL_HEADER_H_
#define _ROHC_PROTOCOL_HEADER_H_

#include "rohc_typ.h"

#define ROHC_IPPROTO_AH         (51U)
#define ROHC_IPPROTO_ESP        (50U)
#define ROHC_IPPROTO_IP4        (4U)
#define ROHC_IPPROTO_UDP        (17U)
#define ROHC_IPPROTO_TCP        (6U)
#define ROHC_IPPROTO_IP6        (41U)
#define ROHC_IPPROTO_GRE        (47U)
#define ROHC_IPPROTO_MINENC     (55U)
#define ROHC_IPPROTO_HOPOPTS    (0U)
#define ROHC_IPPROTO_DSTOPTS    (60U)
#define ROHC_IPPROTO_FRAGOPTS   (44U)
#define ROHC_IPPROTO_ROUTOPTS   (43U)

/*************************************************************
IPv4 Header
*************************************************************/
typedef struct rohc_ip4_t 
{
    rohc_U8bit_t  vhl;            /* header length, version */
#define ROHC_IP4_V(ip)  (((ip)->vhl & 0xf0U) >> 4U)
#define ROHC_IP4_HL(ip) ((ip)->vhl & 0x0fU)
    rohc_U8bit_t  tos;            /* type of service */
    rohc_U16bit_t len;            /* total length */
    rohc_U16bit_t id;             /* identification */
    rohc_U16bit_t off;            /* fragment offset field */
#define ROHC_IP4_DF 0x4000U     /* dont fragment flag */
#define ROHC_IP4_MF 0x2000U     /* more fragments flag */
#define ROHC_IP4_OFFMASK 0x1fffU    /* mask for fragmenting bits */
    rohc_U8bit_t  ttl;            /* time to live */
    rohc_U8bit_t  proto;          /* protocol */
    rohc_U16bit_t cksum;          /* checksum */
    rohc_U32bit_t   src,dst;      /* source and dest address */
    /* SPR 9662 changes start */
#ifdef STRUCT_ALIGNMENT
} __attribute__ ((packed)) rohc_ip4_t;
#else
}rohc_ip4_t;
#endif
    /* SPR 9662 changes end */

/*************************************************************
IPv6 Header
*************************************************************/
typedef struct rohc_ip6_t
{
    union 
    {
        struct ip6_hdrctl
        {
            rohc_U32bit_t un1_flow;/* 4 bit version,8 bit TC,20 bits flow-ID */
#define ROHC_IP6_VER_MASK       0xf0000000U         
#define ROHC_IP6_TRAFFICCLASS_MASK  0x0ff00000U         
#define ROHC_IP6_FLOWLABEL_MASK         0x000fffffU         
            rohc_U16bit_t un1_plen;/* payload length */
            rohc_U8bit_t  un1_nxt;/* next header */
            rohc_U8bit_t  un1_hlim;/* hop limit */
        } ip6_un1;
        rohc_U8bit_t un2_vfc;     /*4 bit version, 4 top bit traffic class */
    } ip6_ctlun;
    rohc_U32bit_t src[4],dst[4];  /*source and dest address */
    /* SPR 9662 changes start */
#ifdef STRUCT_ALIGNMENT
} __attribute__ ((packed)) rohc_ip6_t;
#else
}rohc_ip6_t;
#endif
    /* SPR 9662 changes end */
/*************************************************************
UDP Header
*************************************************************/
typedef struct rohc_udp_t
{
    rohc_U16bit_t sport;      /* source port */
    rohc_U16bit_t dport;      /* destination port */
    rohc_U16bit_t len;        /* udp length */
    rohc_U16bit_t cksum;      /* udp checksum */
}rohc_udp_t;


/*************************************************************
TCP Header
*************************************************************/
typedef struct rohc_tcp_t
{
    rohc_U16bit_t sport;      /* source port */
    rohc_U16bit_t dport;      /* destination port */
    rohc_U32bit_t seq;        /* Sequence number field */
    rohc_U32bit_t ack;        /* Acknowledge Number */
    rohc_U8bit_t offset;       /* 4bit Offset ,4 bit reserved bits */
#define ROHC_TCP_OFFSET_MASK       0xf0U         
#define ROHC_TCP_RES_MASK          0x0fU         
    rohc_U8bit_t flags;       /* 8bit flags */
#define ROHC_TCP_ECN_MASK       0xC0U         
#define ROHC_TCP_URG_MASK       0x20U         
#define ROHC_TCP_ACK_MASK       0x10U         
#define ROHC_TCP_PSH_MASK       0x08U        
#define ROHC_TCP_RST_MASK       0x04U         
#define ROHC_TCP_SYN_MASK       0x02U         
#define ROHC_TCP_FIN_MASK       0x01U         
    rohc_U16bit_t window;       /* Window */
    rohc_U16bit_t checksum;      /* checksum */
    rohc_U16bit_t urg_ptr;       /* UrgentPointer */
}rohc_tcp_t;

#define INDEX_NOP           0
#define INDEX_EOL           1
#define INDEX_MSS           2
#define INDEX_WINDOW_SCAL   3
#define INDEX_TIMESTAMP     4
#define INDEX_SACK_PERM     5
#define INDEX_SACK          6

/* strutures of TCP options */
typedef struct rohc_tcp_eol_opt_t
{
    rohc_U8bit_t  kind;
}rohc_tcp_eol_opt_t;

typedef struct rohc_tcp_nop_opt_t
{
    rohc_U8bit_t  kind;
}rohc_tcp_nop_opt_t;

typedef struct rohc_tcp_mss_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    rohc_U16bit_t val;
}rohc_tcp_mss_opt_t;

typedef struct rohc_tcp_ws_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    rohc_U8bit_t val;
}rohc_tcp_ws_opt_t;

typedef struct rohc_tcp_sack_p_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
}rohc_tcp_sack_p_opt_t;

typedef struct rohc_tcp_sack_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    /* variable length */
}rohc_tcp_sack_opt_t;

typedef struct rohc_tcp_ts_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    /* SPR 9662 changes start */
#ifdef STRUCT_ALIGNMENT
    rohc_U8bit_t  padding[2];
#endif
    /* SPR 9662 changes end */
    rohc_U32bit_t val[2];
}rohc_tcp_ts_opt_t;

typedef struct rohc_tcp_alt_cksum_req_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    rohc_U8bit_t val;
}rohc_tcp_alt_cksum_req_opt_t;

typedef struct rohc_tcp_alt_cksum_data_opt_t
{
    rohc_U8bit_t  kind;
    rohc_U8bit_t  length;
    /* variable length */
}rohc_tcp_alt_cksum_data_opt_t;

/*************************************************************
Authentication Header (AH)
*************************************************************/
typedef struct rohc_ah_t 
{
    rohc_U8bit_t  nxt;        /* Next Header */
    rohc_U8bit_t  len;        /* Length of AH data + 1, in 32bit */
    rohc_U16bit_t reserve;    /* Reserved for future use */
    rohc_U32bit_t spi;        /* Security parameter index */
    rohc_U32bit_t seq;        /* Sequence number field */
    /* variable size, 32bit bound Authentication data */
}rohc_ah_t;

/*************************************************************
ESP Header and Trailer
*************************************************************/
typedef struct rohc_esp_t
{
    rohc_U32bit_t spi;        /* ESP */
    rohc_U32bit_t seq;        /* Sequence number */
    /*variable size*/       /* (IV and) Payload data */
    /*variable size*/       /* padding */
    /*8bit*/                /* pad size */
    /*8bit*/                /* next header */
    /*variable size, 32bit bound Authentication data */
}rohc_esp_t;

typedef struct rohc_esptail_t
{
    rohc_U8bit_t  padlen; /* pad length */
    rohc_U8bit_t  nxt;    /* Next header */
    /*variable size, 32bit bound Authentication data */
}rohc_esptail_t;

/*************************************************************
GRE RFC 2784 and 2890
*************************************************************/
typedef struct rohc_gre_t
{
    rohc_U16bit_t flags;
    rohc_U16bit_t proto;
}rohc_gre_t;

/* RFC 2784 - GRE */
#define ROHC_GRE_CP         0x8000U /* Checksum Present */
#define ROHC_GRE_VER_MASK   0x0007U /* Version */

/* RFC 2890 - Key and Sequence extensions to GRE */
#define ROHC_GRE_KP     0x2000U     /* Key Present */
#define ROHC_GRE_SP     0x1000U     /* Sequence Present */

/*************************************************************
Miminal Encapsulation for IP RFC 2004
*************************************************************/
typedef struct rohc_minenc_t 
{
    rohc_U8bit_t protocol;
    rohc_U8bit_t osrc_flag;
    /* S flag 7 bits reserved */
#define ROHC_OSRC_PRES  0x80U /* old source is present */
    rohc_U16bit_t cksum;
    rohc_U32bit_t odst;
    rohc_U32bit_t osrc;   /*optional field */
}rohc_minenc_t ;


/*************************************************************
IPv6 Extension Headers
*************************************************************/
/* Hop-by-Hop options header */
typedef struct rohc_hopopts_t 
{
    rohc_U8bit_t nxt; /* next header */
    rohc_U8bit_t len; /* length in units of 8 octets */
    /* followed by variable length options */
}rohc_hopopts_t;

/* Destination options header */
typedef struct rohc_dstoptions_t 
{
    rohc_U8bit_t nxt; /* next header */
    rohc_U8bit_t len; /* length in units of 8 octets */
    /* followed by variabel length options */
}rohc_dstoptions_t ;

/* Routing header */
typedef struct rohc_routopts_t
{
    rohc_U8bit_t  nxt;        /* next header */
    rohc_U8bit_t  len;        /* length in units of 8 octets */
    rohc_U8bit_t  type;       /* routing type */
    rohc_U8bit_t  segleft;    /* segments left */
    /* followed by variable length routing type specific data */
}rohc_routopts_t ;

/* Fragment header */
typedef struct rohc_fragopts_t
{
    rohc_U8bit_t  nxt;            /* next header */
    rohc_U8bit_t  reserved;       /* reserved field */
    rohc_U16bit_t offlg;          /* offset, reserved, and flag */
    rohc_U32bit_t ident;          /* identification */
}rohc_fragopts_t;

typedef struct rohc_rtp_t
{
    rohc_U8bit_t  v_p_x_cc;
#define ROHC_VER_MASK   0xc0U
#define ROHC_PAD_MASK   0x20U
#define ROHC_X_MASK     0x10U
#define ROHC_CC_MASK    0x0fU
    rohc_U8bit_t  m_pt;
#define ROHC_M_MASK     0x80U
#define ROHC_PT_MASK    0x7fU
    rohc_U16bit_t sn;
    rohc_U32bit_t ts;
    rohc_U32bit_t ssrc;
    /* var length 32 bit CSRC list */
}rohc_rtp_t;

#endif /* _ROHC_PROTOCOL_HEADER_H_ */
