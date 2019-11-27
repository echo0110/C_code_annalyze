/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_c_parse_ip.c,v 1.3.28.23 2010/11/26 05:35:52 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Compressor Packet Sentinel module Implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_c_parse_ip.c,v $
 * Revision 1.3.28.23  2010/11/26 05:35:52  gur22059
 * Use of register variables for profile 1
 *
 * Revision 1.3.28.22  2010/11/24 03:43:12  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.28.21  2010/11/09 05:22:55  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.28.20  2010/10/13 03:55:22  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.28.19  2010/09/28 06:56:49  gur11835
 * Corrected usage of ROHC context block pointer stored at PDCP layer
 *
 * Revision 1.3.28.18  2010/09/23 05:19:13  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3.28.17  2010/09/13 04:09:47  gur22059
 * Packet context retrieval done at PDCP.
 *
 * Revision 1.3.28.16  2010/09/10 10:14:02  gur22059
 * Locality of reference & use of register variables for better performamce
 *
 * Revision 1.3.28.15  2010/08/19 09:01:26  gur22059
 * SPR 574: Corrected sn_diff calculation by typecasting it into
 *          its original type.
 *
 * Revision 1.3.28.14  2010/08/19 08:58:37  gur22059
 * SPR 570 : Packet count was not being incremented, which affects
 *           TS Stride calculation in case of profile-1 RTP packets.
 *
 * Revision 1.3.28.13  2010/08/19 08:47:05  gur22059
 * SPR 568 FIX : Context creation for profile-1 (RTP) packets is updated
 *
 * Revision 1.3.28.12  2010/08/19 07:02:43  gur22059
 * SPR 573: Corrected calculation of TS stride
 *
 * Revision 1.3.28.5  2010/07/08 09:39:10  gur21145
 * code review comments of prof 0 , 2 enhancement activity
 *
 * Revision 1.3.28.4  2010/07/07 11:34:44  gur21145
 * PROFILE2_comments_and PROFILE0_comments_inc
 *
 * Revision 1.3.28.2  2010/06/25 10:05:18  gur21145
 * PROFILE2_ENHANCEMENTS
 *
 * Revision 1.3  2009/05/28 03:43:19  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_c_parse_ip.h"
#include "rohc_gcom_mcr.h"
#ifdef PERF_STATS
 /* + PERF_CA */
#include "ltePdcpContext.h"
 /* - PERF_CA */
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/************************************************************************
 * Name     :   rohc_c_parse_ip
 * Desc     :   C-PPM
 * Params   :   IP Datagram
                IP Datagram length 
 *              ROHC entity context
 *              O/P parameter - pointer to Beh Det Block
 *              CSC
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t
rohc_c_parse_ip
    ROHC_PARAMS((p_pkt,length,p_entity,p_p_csc,p_header_length, p_ecode),
        rohc_U8bit_t                * p_pkt _AND_
        rohc_U32bit_t               length  _AND_   
        struct rohc_entity_t        * p_entity _AND_
        rohc_csc_common_t           **p_p_csc   _AND_
        rohc_U32bit_t               *p_header_length _AND_
        rohc_error_code_t           *p_ecode)
{

    rohc_U8bit_t                    ip_version=ROHC_GET_UPPER_NIBBLE(*p_pkt); 
    rohc_U16bit_t                   payload_len = 0; 
    struct rohc_cpib_t              * p_cpib=PNULL;
    struct rohc_c_temp_context_t    temp_fields;

    register rohc_U8bit_t determined_profile;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_entity->rohc_entity_id.ueIndex);
    LteCellPdcpDLPerfStats *lteCellPdcpDLPerfStats_p =
        &gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpDLPerfStats;
	/* - PERF_CA */
#endif

    register rohc_U8bit_t profile;

    ROHC_ENTER_FN("rohc_c_parse_ip");
#ifdef ROHC_UT_FLAG
    /* print uncompressed buffer as input */
    rohc_U8bit_t * temp = p_pkt;
    rohc_U8bit_t i = 0;
    ROHC_UT_TRACE((stderr," inbuff len = %u\n",length));
    while(i <=39)
    {
        ROHC_UT_TRACE((stderr,"0x%x \n", (unsigned char)*(temp+i)));
        i++;
    }
#endif
    if( (p_entity->profilesSupported == ROHC_UNCOMP ) || 
            ( ip_version != 4 && ip_version != 6) )
    {
        *p_p_csc = rohc_db_get_uncomp_csc(p_entity,p_ecode);
        /* Klocwork Fix- Null check for p_p_csc removed since
         * it was unreachable code */

        return ROHC_SUCCESS;    
    }/* end if non IP Packet  */

    ROHC_MEMSET(&temp_fields,0,sizeof(struct rohc_c_temp_context_t));
    p_cpib = &rohc_global.cpib;
    ROHC_MEMSET(p_cpib,0,sizeof(rohc_cpib_t));
    temp_fields.p_cpib=p_cpib;
    rohc_db_create_list(&p_cpib->inneripextlist);
    rohc_db_create_list(&p_cpib->outeripextlist);
    temp_fields.p_cpib->total_pkt_len=length;

    switch(ip_version)
    {
        case 4:
        if( ROHC_FAILURE == rohc_parse_ip4_pkt(
                    (rohc_ip4_t *)p_pkt,p_entity,
                    &temp_fields,length,p_ecode) )
        {
            return ROHC_FAILURE;
        }
        break;  
        case 6:
        if( ROHC_FAILURE == rohc_parse_ip6_pkt(
                    (rohc_ip6_t *)p_pkt,
                    p_entity,&temp_fields,length,p_ecode) )
        {
            return ROHC_FAILURE;
        }
        break;  
    }/* end switch IP version*/ 

    if((p_cpib->inner_ip4 == ROHC_NULL) && (p_cpib->inner_ip6 == ROHC_NULL))
    {
        if(p_cpib->outer_ip4)
        {
            p_cpib->inner_ip4 = p_cpib->outer_ip4;
            p_cpib->outer_ip4 = ROHC_NULL;
        }
        else
        {
            p_cpib->inner_ip6 = p_cpib->outer_ip6;
            p_cpib->outer_ip6 = ROHC_NULL;
        }
        p_cpib->inneripextlist = p_cpib->outeripextlist;

        if(ROHC_CPIB_GET_OUTER_AH_PTR(p_cpib))
        {
            ROHC_CPIB_SET_INNER_AH_PTR(p_cpib,
                    ROHC_CPIB_GET_OUTER_AH_PTR(p_cpib));
            ROHC_CPIB_SET_OUTER_AH_PTR(p_cpib,ROHC_NULL);
            ROHC_CPIB_SET_INNER_AH_LEN(p_cpib,
                    ROHC_CPIB_GET_OUTER_AH_LEN(p_cpib));
            ROHC_CPIB_SET_OUTER_AH_LEN(p_cpib,0);
        }
        if(ROHC_CPIB_GET_OUTER_GRE_PTR(p_cpib))
        {
            ROHC_CPIB_SET_INNER_GRE_PTR(p_cpib,
                    ROHC_CPIB_GET_OUTER_GRE_PTR(p_cpib));
            ROHC_CPIB_SET_OUTER_GRE_PTR(p_cpib,ROHC_NULL);
        }
    }

    determined_profile = p_cpib->determined_profile;
    if(determined_profile != ROHC_UNCOMP)
    {
        ROHC_CPIB_SET_PAYLOAD(p_cpib,p_pkt+temp_fields.len_parsed);
        *p_header_length = temp_fields.len_parsed;
        if(ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib))
            p_cpib->temp_ipid = ROHC_NTOHS(
                    ((rohc_ip4_t *)ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib))->id);
        if(ROHC_CPIB_GET_OUTER_IP4_PTR(p_cpib))
            p_cpib->temp_ipid2 = ROHC_NTOHS(
                    ((rohc_ip4_t *)ROHC_CPIB_GET_OUTER_IP4_PTR(p_cpib))->id);
    }
    else
    {
        ROHC_CPIB_SET_PAYLOAD(p_cpib,p_pkt);
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        switch(ip_version)
        {
            case 4:
            if(p_entity->rohc_entity_id.qci != 0)
            {
                /*for ROHC Profile ZERO:
                 * Update KPI Counter for the Total uncompressed header volume that has been
                 *  received in the downlink direction in the PDCP layer */
                lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL\
                    [p_entity->rohc_entity_id.qci-1] += sizeof(struct rohc_ip4_t);
            }
            break;

            case 6:
            if(p_entity->rohc_entity_id.qci != 0)
            {
                /*for ROHC Profile ZERO:
                 * Update KPI Counter for the Total uncompressed header volume that has been
                 *  received in the downlink direction in the PDCP layer */
                lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL\
                    [p_entity->rohc_entity_id.qci-1] += sizeof(struct rohc_ip6_t);
            }
            break;
        }
#endif
    }

    if( ROHC_FAILURE == rohc_db_get_csc(p_cpib,p_entity,
                p_cpib->determined_profile,
                p_p_csc,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    if (*p_p_csc == PNULL ) 
    {
        if( (*p_p_csc = rohc_db_create_csc_new_cid(p_entity,
                        p_cpib,p_ecode)) == PNULL)
        {
            /* SPR 12654 fix start */
            *p_header_length = 0;
            ROHC_CPIB_SET_PAYLOAD(p_cpib,p_pkt);
            /* SPR 12654 fix end */

            ROHC_LOG(LOG_INFO,"EID[%u:%u]:"
                    " Unable to allocate memory for csc",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId);
            return ROHC_FAILURE;
        }
    }

    profile = (*p_p_csc)->profile;

    /* 
     ** START_SPR_570_FIX : Packet count was not being incremented, which 
     ** affects TS Stride calculation in case of profile-1 RTP packets.
     */
    (*p_p_csc)->no_of_pkt_recvd++;
    /* END_SPR_570_FIX */

    if( ROHC_CPIB_GET_RTP_PTR(p_cpib) && 
            ((*p_p_csc)->profile == ROHC_IP_UDP_RTP) )
    {
        p_cpib->temp_ts = 
            ROHC_NTOHL(((rohc_rtp_t *)ROHC_CPIB_GET_RTP_PTR(p_cpib))->ts);
        p_cpib->temp_sn = 
            ROHC_NTOHS(((rohc_rtp_t *)ROHC_CPIB_GET_RTP_PTR(p_cpib))->sn);
        if((*p_p_csc)->no_of_pkt_recvd == 1)
        {
            ROHC_CSC_SET_OLD_RTP_TS((rohc_csc_rtp_t *)*p_p_csc,p_cpib->temp_ts);

            ROHC_CSC_SET_OLD_RTP_SN((rohc_csc_rtp_t *)*p_p_csc,p_cpib->temp_sn);
        }
        else
        {
#ifndef ROHC_COMPUTE_TS_STRIDE_ONCE     
            /* + SPR 17439 */
            rohc_c_check_ts_stride_beh((rohc_csc_rtp_t *)*p_p_csc);
            /* - SPR 17439 */
#else
            if((*p_p_csc)->no_of_pkt_recvd == 2)
                rohc_c_check_ts_stride_beh(*p_p_csc);
#endif
        }
    }
    if(profile == ROHC_IP_UDP)
    {
        (((rohc_csc_udp_t *)(*p_p_csc))->udp_sn)++;
        p_cpib->temp_sn=((rohc_csc_udp_t *)(*p_p_csc))->udp_sn; 
    }   

    if(profile == ROHC_IP_TCP)
    {
        p_cpib->temp_sn = 
            ROHC_NTOHL(((rohc_tcp_t *)ROHC_CPIB_GET_TCP_PTR(p_cpib))->seq);
        p_cpib->temp_ack = 
            ROHC_NTOHL(((rohc_tcp_t *)ROHC_CPIB_GET_TCP_PTR(p_cpib))->ack);

        if((*p_p_csc)->no_of_pkt_recvd == 1)
        {
            ROHC_CSC_SET_OLD_TCP_ACK((rohc_csc_tcp_t *)*p_p_csc,p_cpib->temp_ack);
            /* Initialize msn to a random number between 1-10 */
            /* Coverity Fix 40736 start */
            ((rohc_csc_tcp_t *)(*p_p_csc))->tcp_msn = ( p_cpib->temp_sn % 10 ) + 1;
            /* Coverity Fix 40736 end */
            /*initilaize end_index of sliding window */
            (*p_p_csc)->rohc_gsw.end_index = ROHC_MAX_LENGTH_GSW_PROF6 - 1;
            ((rohc_csc_tcp_t *)(*p_p_csc))->sn_scaling_factor_estab_counter = 0;
            ((rohc_csc_tcp_t *)(*p_p_csc))->ack_scaling_factor_estab_counter = 0;
            ((rohc_csc_tcp_t *)(*p_p_csc))->num_of_co_common_pkts_sent = 0;
        }
        else
        {
            /* just increment msn sequentially */
            (((rohc_csc_tcp_t *)(*p_p_csc))->tcp_msn)++;
            /* + SPR 17439 */
            rohc_c_check_ack_stride_beh((rohc_csc_tcp_t *)*p_p_csc);
            /* - SPR 17439 */
            if(((rohc_csc_tcp_t *)(*p_p_csc))->ack_stride)
            {
                ROHC_CSC_SET_TCP_ACK_RESIDUE((rohc_csc_tcp_t *)*p_p_csc,p_cpib->temp_ack);
            }
            else
            {
                ((rohc_csc_tcp_t *)(*p_p_csc))->temp_ack_residue = p_cpib->temp_ack;
            }
            /* update sn_stride which is payload size calculated as-
             * tot_len field of ip header - (size of ip hdr + size of tcp hdr)*/

            if(ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib))
            {
                payload_len = ROHC_NTOHS(
                        ((rohc_ip4_t *)ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib))->len) -
                    (ROHC_IPV4_MIN_HDR_LEN + ((ROHC_GET_UPPER_NIBBLE(
                                                                     ((rohc_tcp_t *)ROHC_CPIB_GET_TCP_PTR(p_cpib))->offset))*4));
            }
            else
            {
                /* calculate payload len from ipv6 hdr*/
            }
            ROHC_CSC_SET_TCP_SN_STRIDE((rohc_csc_tcp_t *)*p_p_csc,payload_len);
            ROHC_UT_TRACE((stderr,"calculated sn_stride = %u\n",((rohc_csc_tcp_t *)
                            *p_p_csc)->sn_stride));
            if(((rohc_csc_tcp_t *)(*p_p_csc))->sn_stride)
            {
                ROHC_CSC_SET_TCP_SN_RESIDUE((rohc_csc_tcp_t *)*p_p_csc,p_cpib->temp_sn);
                ROHC_UT_TRACE((stderr,"calculated sn_residue = %u\n",((rohc_csc_tcp_t *)
                                *p_p_csc)->temp_sn_residue));
            }
            else
            {
                ((rohc_csc_tcp_t *)(*p_p_csc))->temp_sn_residue = p_cpib->temp_sn;
            }

        }
        /* check for ip-id behavior- This has to modified later to support
         * sequential and seq swapped  beh. */
        if(((rohc_ip4_t *)ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib))->id)
        {
            ((rohc_csc_tcp_t *)(*p_p_csc))->inner_ipid_beh = 
                IP_ID_BEHAVIOR_RANDOM;
        }
        else
        {
            ((rohc_csc_tcp_t *)(*p_p_csc))->inner_ipid_beh = 
                IP_ID_BEHAVIOR_ZERO;
        }
    }   

    ROHC_EXIT_FN("rohc_c_parse_ip");    
    return ROHC_SUCCESS;
}   /* end function rohc_c_parse_ip*/  

/************************************************************************
 * Name     :   rohc_parse_ip4_pkt
 * Desc     :   Parse IPv4 Datagram
 * Params   :   IP Datagram
 *              ROHC entity context
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t rohc_parse_ip4_pkt
    ROHC_PARAMS((p_pkt,p_entity,p_temp_fields,len,p_ecode),
    struct rohc_ip4_t               * p_pkt _AND_
    struct rohc_entity_t            * p_entity _AND_    
    struct rohc_c_temp_context_t    * p_temp_fields _AND_
    rohc_U32bit_t           len _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U32bit_t       advance=0;
    rohc_U8bit_t        *p_pkt_org = (rohc_U8bit_t*)p_pkt;      
    rohc_U8bit_t        curr_proto;     
    rohc_U8bit_t        flag=1;
    rohc_U8bit_t        *ip_pkt = (rohc_U8bit_t *)p_pkt; 
    /* SPR 8697 fix start */
    rohc_U32bit_t       tcp_header_len = 0;
    /* SPR 8697 fix end */
    ROHC_ENTER_FN("rohc_parse_ip4_pkt");
    if(p_pkt->vhl != 0x45 || ((ROHC_NTOHS(p_pkt->off)&ROHC_IP4_OFFMASK) != 0) ||
                                    ((ROHC_NTOHS(p_pkt->off)&ROHC_IP4_MF) !=0))
    {
        ROHC_CPIB_SET_PROFILE(p_temp_fields->p_cpib,ROHC_UNCOMP);  
        return ROHC_SUCCESS;
    }/* end if IP fragments cannot be compressed */

    p_temp_fields->p_cpib->ip4_count++;
    if((p_temp_fields->p_cpib->ip4_count +
        p_temp_fields->p_cpib->ip6_count) == 1)
    {
        ROHC_CPIB_SET_OUTER_IP4_PTR(p_temp_fields->p_cpib,
                            (rohc_U8bit_t *)p_pkt);
        p_temp_fields->cur_ip=ROHC_OUTERIP;
    }   /* First IP header found */
    else if((p_temp_fields->p_cpib->ip4_count + 
            p_temp_fields->p_cpib->ip6_count) == 2)
    {
        ROHC_CPIB_SET_INNER_IP4_PTR(p_temp_fields->p_cpib,
                            (rohc_U8bit_t *)p_pkt);
        p_temp_fields->cur_ip=ROHC_INNERIP;
    } /* end if second IP header*/
    else
    {
        ROHC_CPIB_SET_PROFILE(p_temp_fields->p_cpib,ROHC_UNCOMP);   
        return ROHC_SUCCESS;
    }/*end if More than 2 IP Headers cannot be compressed */
    
    curr_proto=p_pkt->proto;
    ip_pkt=(rohc_U8bit_t *)p_pkt+ROHC_IPV4_MIN_HDR_LEN; /*IPv4 processing over */
    p_temp_fields->len_parsed+=ROHC_IPV4_MIN_HDR_LEN;
    while(flag)     
    {
        switch(curr_proto)
        {
            case ROHC_IPPROTO_UDP :
                advance=rohc_parse_udp_pkt((struct rohc_udp_t *)ip_pkt,
                                                p_temp_fields);
				ip_pkt+=advance;
                /* SPR 2470 Changes Start */
                if((p_temp_fields->p_cpib->esp == ROHC_NULL)&&(p_entity->profilesSupported & ROHC_IP_UDP))
                /* SPR 2470 Changes End */
                {
                    p_temp_fields->p_cpib->determined_profile = ROHC_IP_UDP;
                }

                if(!(p_entity->profilesSupported & ROHC_IP_UDP_RTP))
                {
                    /* SPR 9155 fix start */
                    /* If packet size < header size set uncompressed profile */
                    /* IPv4 options are not supported */
                    if (ROHC_UDP_HDR_LEN + ROHC_IPV4_MIN_HDR_LEN > len )
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_UNCOMP;
                    }
                    /* SPR 9155 fix stop */
                    flag=0; 
                    break;
                }

                if(ROHC_GET_2MSB(*(rohc_U8bit_t *)ip_pkt)==0x02 &&
                !(ROHC_NTOHS(((struct rohc_udp_t *)p_temp_fields->p_cpib->udp)->
                                                        dport)%2))
                {   
                    advance = rohc_parse_rtp_pkt((struct rohc_rtp_t *)ip_pkt,
                                        p_temp_fields);
                    ip_pkt+=advance;
                    /* SPR 9155 fix start */
                    /* If packet size < header size set uncompressed profile */
                    /* IPv4 options are not supported */
                    if (advance + ROHC_UDP_HDR_LEN + ROHC_IPV4_MIN_HDR_LEN > len )
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_UNCOMP;
                    }
                    else
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_IP_UDP_RTP;
                    }
                    /* SPR 9155 fix stop */
                }   /*end if RTP found */
                flag=0; 
                break;
        
            /* IP in IP Tunneling*/
            case ROHC_IPPROTO_IP4 :
                if( ROHC_FAILURE == rohc_parse_ip4_pkt(
                            (struct rohc_ip4_t *)ip_pkt,p_entity,
                            p_temp_fields,
                            len - ((rohc_U8bit_t*)ip_pkt - p_pkt_org),p_ecode))
                {
                    return ROHC_FAILURE;
                }
                flag=0; 
                break;
                
            case ROHC_IPPROTO_IP6 :
                if( ROHC_FAILURE == rohc_parse_ip6_pkt(
                           (struct rohc_ip6_t *)ip_pkt,p_entity,
                            p_temp_fields,
                            len - ((rohc_U8bit_t*)ip_pkt - p_pkt_org ),p_ecode) )
                {
                    return ROHC_FAILURE;
                }
                flag=0; 
                break;
            case ROHC_IPPROTO_TCP :
                rohc_parse_tcp_pkt((struct rohc_tcp_t *)ip_pkt,
                                                p_temp_fields);
                if(p_entity->profilesSupported & ROHC_IP_TCP_SUPPORTED)
                {
                    /* SPR 8697 fix start */
                    tcp_header_len = (ROHC_GET_UPPER_NIBBLE(
                            ((rohc_tcp_t *)ROHC_CPIB_GET_TCP_PTR(
                                p_temp_fields->p_cpib))->offset)*4);
                    /* IPv4 options not supported, so fixed size 20 is taken
                     * len = length of buffer from PDCP. Preventive check has 
                     * been added to ensure that if the size of the buffer 
                     * received is less than IP+TCP header, then uncompressed 
                     * profile should be selected. */
                    if (tcp_header_len + ROHC_IPV4_MIN_HDR_LEN > len )
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_UNCOMP;
                    }
                    else
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_IP_TCP;
                    }
                    /* SPR 8697 fix end */
                }
                flag=0; 
                break;
            default :   /* unrecognised protocol field in IP */         
                p_temp_fields->p_cpib->determined_profile=ROHC_UNCOMP; 
                flag=0;
                break;
        }/* end switch Protocol field*/
    } /* end while */
    ROHC_EXIT_FN("rohc_parse_ip4_pkt"); 
    return ROHC_SUCCESS;
}   /*end function rohc_parse_ip4_pkt */

/****************************************************************************
 * Name     :   rohc_parse_rtp_pkt
 * Desc     :   RTP header parsing
 * Params   :   RTP packet
                Pointer to C-PPM temp context
 * Returns  :   Length of header parsed
 **************************************************************************/
rohc_U32bit_t rohc_parse_rtp_pkt
    ROHC_PARAMS((p_pkt,p_temp_fields),
    struct rohc_rtp_t * p_pkt _AND_ 
    struct rohc_c_temp_context_t * p_temp_fields)
{
    rohc_U32bit_t   pkt_len;    
    
    ROHC_ENTER_FN("rohc_parse_rtp_pkt");
    p_temp_fields->p_cpib->rtp=(rohc_U8bit_t *)p_pkt;
    pkt_len=12+((ROHC_GET_LOWER_NIBBLE(*(rohc_U8bit_t *)p_pkt))*4);

    p_temp_fields->len_parsed+=pkt_len;
    ROHC_EXIT_FN("rohc_parse_rtp_pkt");
    return pkt_len;
} /* end function rohc_parse_rtp_pkt  */    

/****************************************************************************
 * Name     :   rohc_parse_tcp_pkt
 * Desc     :   TCP header parsing
 * Params   :   TCP packet
                Pointer to C-PPM temp context
 * Returns  :   Length of header parsed
 **************************************************************************/
rohc_void_t rohc_parse_tcp_pkt
    ROHC_PARAMS((p_pkt,p_temp_fields),
    struct rohc_tcp_t * p_pkt _AND_ 
    struct rohc_c_temp_context_t * p_temp_fields)
{
    rohc_U32bit_t   pkt_len;    
    
    ROHC_ENTER_FN("rohc_parse_tcp_pkt");
    if(p_pkt->checksum)
    {
        ROHC_CPIB_SET_TCP_CKSUM_PRESENT(p_temp_fields->p_cpib,ROHC_TRUE);
    }
    p_temp_fields->p_cpib->tcp=(rohc_U8bit_t *)p_pkt;
    pkt_len=((ROHC_GET_UPPER_NIBBLE(p_pkt->offset))*4);  
    p_temp_fields->len_parsed+=pkt_len;
    ROHC_EXIT_FN("rohc_parse_tcp_pkt");
    
} /* end function rohc_parse_tcp_pkt  */  

#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_parse_ah_pkt
 * Desc     :   AH header parsing
 * Params   :   AH packet
                Pointer to C-PPM temp context
 *              Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t rohc_parse_ah_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_ah_t * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t       *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)
{
    
    ROHC_ENTER_FN("rohc_parse_ah_pkt");
    *p_pkt_len = (p_pkt->len+2)*4;  
    
    p_temp_fields->p_cpib->ah_count++;

    if(p_temp_fields->p_cpib->ah_count ==1)
    {
        ROHC_CPIB_SET_OUTER_AH_PTR(p_temp_fields->p_cpib,
                (rohc_U8bit_t *)p_pkt);
        ROHC_CPIB_SET_OUTER_AH_LEN(p_temp_fields->p_cpib,
                                    *p_pkt_len);
    }   /*end if first AH */
    else if(p_temp_fields->p_cpib->ah_count == 2)
    {   /*already one AH exists */
        ROHC_CPIB_SET_INNER_AH_PTR(p_temp_fields->p_cpib,
                (rohc_U8bit_t *)p_pkt);
        ROHC_CPIB_SET_INNER_AH_LEN(p_temp_fields->p_cpib,
                                    *p_pkt_len);
    }   /*end else second AH */
    else 
    {
        ROHC_CPIB_SET_PROFILE(p_temp_fields->p_cpib,ROHC_UNCOMP);   
        p_temp_fields->profile_over=ROHC_TRUE;  
        *p_pkt_len = 0;
        return ROHC_SUCCESS;
    }
    
    if( ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_AH,
                    p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode))
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_ah_pkt");
    return ROHC_SUCCESS;
} /* end function rohc_parse_ah_pkt */
#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_parse_esp_pkt
 * Desc     :   ESP header parsing
 * Params   :   ESP packet
                Pointer to C-PPM temp context
 * Returns  :   Length of header parsed
 **************************************************************************/
rohc_U32bit_t rohc_parse_esp_pkt
    ROHC_PARAMS((p_pkt,p_temp_fields),
    struct rohc_esp_t * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields)
{
    
    ROHC_ENTER_FN("rohc_parse_esp_pkt");
    ROHC_CPIB_SET_ESP_PTR(p_temp_fields->p_cpib,(rohc_U8bit_t *)p_pkt);
    p_temp_fields->len_parsed+=sizeof(struct rohc_esp_t);
    ROHC_EXIT_FN("rohc_parse_esp_pkt"); 
    return(sizeof(struct rohc_esp_t));
}/* end function rohc_parse_esp_pkt */
#endif
/****************************************************************************
 * Name     :   rohc_parse_udp_pkt
 * Desc     :   UDP header parsing
 * Params   :   UDP packet
                Pointer to C-PPM temp context
 * Returns  :   Length of header parsed
 **************************************************************************/
rohc_U32bit_t rohc_parse_udp_pkt
    ROHC_PARAMS((p_pkt,p_temp_fields),
    struct rohc_udp_t *p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields)
{
    ROHC_ENTER_FN("rohc_parse_udp_pkt");
    if(p_pkt->cksum)
    {
        ROHC_CPIB_SET_UDP_CKSUM_PRESENT(p_temp_fields->p_cpib,ROHC_TRUE);
    }

    ROHC_CPIB_SET_UDP_PTR(p_temp_fields->p_cpib,(rohc_U8bit_t *)p_pkt);
    p_temp_fields->len_parsed+=sizeof(struct rohc_udp_t);
    ROHC_EXIT_FN("rohc_parse_udp_pkt");
    return sizeof(struct rohc_udp_t);
} /* end function rohc_parse_udp_pkt */ 
#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_parse_gre_pkt
 * Desc     :   GRE header parsing
 * Params   :   GRE packet
                Pointer to C-PPM temp context
 *              Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_parse_gre_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_gre_t *p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t               *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)
{
    
    ROHC_ENTER_FN("rohc_parse_gre_pkt");
    *p_pkt_len=sizeof(struct rohc_gre_t);   
    
    p_temp_fields->p_cpib->gre_count++;
    if(p_temp_fields->p_cpib->gre_count > 2)
    {
        ROHC_CPIB_SET_PROFILE(p_temp_fields->p_cpib,ROHC_UNCOMP);   
        p_temp_fields->profile_over=ROHC_TRUE;  
        return ROHC_SUCCESS;
    }
    else if(p_temp_fields->p_cpib->gre_count ==1)
    {
        ROHC_CPIB_SET_OUTER_GRE_PTR(p_temp_fields->p_cpib,
                        (rohc_U8bit_t *)p_pkt);
    }   /*end if first GRE */
    else
    {   /*already one GRE exists */
        ROHC_CPIB_SET_INNER_GRE_PTR(p_temp_fields->p_cpib,
                    (rohc_U8bit_t *)p_pkt);
    }   /*end else second GRE */
    
    if(p_pkt->flags&ROHC_GRE_CP)
    {
        *p_pkt_len+=4;
    }
    if(p_pkt->flags&ROHC_GRE_KP)
    {
        *p_pkt_len+=4;
    }
    if(p_pkt->flags&ROHC_GRE_SP)
    {
        *p_pkt_len+=4;
    }

    if( ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_GRE,
                    p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_gre_pkt");
    return ROHC_SUCCESS;
} /* end function rohc_parse_gre_pkt */     
#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_parse_minenc_pkt
 * Desc     :   MINENC header parsing
 * Params   :   MINENC packet
                Pointer to C-PPM temp context
                Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE 
 **************************************************************************/
rohc_return_t 
rohc_parse_minenc_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_minenc_t        * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t               *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)
{
    ROHC_ENTER_FN("rohc_parse_minenc_pkt");
    *p_pkt_len=sizeof(struct rohc_minenc_t);    
    
    if(!(p_pkt->osrc_flag & ROHC_OSRC_PRES))
    {
        *p_pkt_len-=4;
    }
    if( ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_MINENC,
                        p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_minenc_pkt");
    return ROHC_SUCCESS;
} /* end function rohc_parse_minenc_pkt */  
#endif

/****************************************************************************
 * Name     :   rohc_parse_ip6_pkt
 * Desc     :   IP6 header parsing
 * Params   :   IP6 packet
                Pointer to entity context
                Pointer to C-PPM temp context
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t rohc_parse_ip6_pkt
    ROHC_PARAMS((p_pkt,p_entity,p_temp_fields,len,p_ecode),
    struct rohc_ip6_t               * p_pkt _AND_
    struct rohc_entity_t            * p_entity _AND_    
    struct rohc_c_temp_context_t    * p_temp_fields _AND_
    rohc_U32bit_t           len _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t        *p_pkt_org = (rohc_U8bit_t*)p_pkt;      
    rohc_U32bit_t           advance=0;
    rohc_U8bit_t            curr_proto;
    rohc_U8bit_t            flag=1;
    rohc_U8bit_t         *ip_pkt = (rohc_U8bit_t*)p_pkt; 
    ROHC_ENTER_FN("rohc_parse_ip6_pkt");    
    p_temp_fields->p_cpib->ip6_count++;
    if((p_temp_fields->p_cpib->ip4_count + 
                p_temp_fields->p_cpib->ip6_count) == 1)
    {
        ROHC_CPIB_SET_OUTER_IP6_PTR(p_temp_fields->p_cpib,
                (rohc_U8bit_t *)p_pkt);
        p_temp_fields->cur_ip=ROHC_OUTERIP;
    }   /* First IP header found */
    else if(p_temp_fields->p_cpib->ip4_count +
            p_temp_fields->p_cpib->ip6_count == 2)
    {
        ROHC_CPIB_SET_INNER_IP6_PTR(p_temp_fields->p_cpib,
                (rohc_U8bit_t *)p_pkt);
        p_temp_fields->cur_ip=ROHC_INNERIP;
    } /* end if second IP header*/
    else
    {
        ROHC_CPIB_SET_PROFILE(p_temp_fields->p_cpib,ROHC_UNCOMP);   
        p_temp_fields->profile_over=ROHC_TRUE;  
        return ROHC_SUCCESS;
    }/*end if More than 2 IP Headers*/

    curr_proto=p_pkt->ip6_ctlun.ip6_un1.un1_nxt;
    ip_pkt=
        ((rohc_U8bit_t *)p_pkt+sizeof(struct rohc_ip6_t));/*IPv6 processing */
    p_temp_fields->len_parsed+=sizeof(struct rohc_ip6_t);
    while(flag == 1)
    {
        switch(curr_proto)
        {

            case ROHC_IPPROTO_UDP :
                advance=rohc_parse_udp_pkt((struct rohc_udp_t *)ip_pkt,
                        p_temp_fields);     
                ip_pkt+=advance;
                if((p_temp_fields->p_cpib->esp == ROHC_NULL) &&
                        (p_entity->profilesSupported & ROHC_IP_UDP))
                {

                    p_temp_fields->p_cpib->determined_profile=ROHC_IP_UDP;

                    /* 
                     ** START_SPR_568_FIX: Initialize C-PPM CPIB expected
                     ** profile to ROHC_IP_UDP. This part has not been tested
                     ** as yet and shall be tested in future releases.
                     */
                    p_temp_fields->p_cpib->expected_profile = ROHC_IP_UDP;

                    /* END_SPR_568_FIX */

                }
                else
                {
                    p_temp_fields->p_cpib->expected_profile = ROHC_IP_UDP;
                }
                if(!(p_entity->profilesSupported & ROHC_IP_UDP_RTP))
                {
                    /* SPR 9155 fix start */
                    /* If packet size < header size set uncompressed profile */
                    /* IPv6 extension headers are not supported */
                    if (ROHC_UDP_HDR_LEN + ROHC_IPV6_MIN_HDR_LEN > len )
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_UNCOMP;
                    }
                    /* SPR 9155 fix stop */

                    flag = 0;
                    break;
                }
                if(ROHC_GET_2MSB(*(rohc_U8bit_t *)ip_pkt)==0x02 &&
                        !(ROHC_NTOHS(((struct rohc_udp_t *)p_temp_fields->p_cpib->udp)->
                                dport)%2))
                {   
                    advance = rohc_parse_rtp_pkt((struct rohc_rtp_t *)ip_pkt,
                            p_temp_fields);
                    ip_pkt+=advance;
                    /* SPR 9155 fix start */
                    /* If packet size < header size set uncompressed profile */
                    /* IPv6 extension headers are not supported */
                    if (advance + ROHC_UDP_HDR_LEN + ROHC_IPV6_MIN_HDR_LEN > len )
                    {
                        p_temp_fields->p_cpib->determined_profile = ROHC_UNCOMP;
                    }
                    else
                    {
                        p_temp_fields->p_cpib->determined_profile=ROHC_IP_UDP_RTP;
                    }
                    /* SPR 9155 fix stop */
                    p_temp_fields->p_cpib->expected_profile=ROHC_IP_UDP_RTP;
                }   /*end if RTP found */
                else
                {
                    if((p_temp_fields->p_cpib->esp) == ROHC_NULL)
                        p_temp_fields->profile_over = ROHC_TRUE;
                }
                flag=0;
                break;

                /* IP in IP Tunneling*/
            case ROHC_IPPROTO_IP4 :
                if( ROHC_FAILURE == rohc_parse_ip4_pkt(
                            (struct rohc_ip4_t *)ip_pkt,
                            p_entity,p_temp_fields,
                            len - ((rohc_U8bit_t*)ip_pkt - p_pkt_org ),p_ecode))
                {
                    return ROHC_FAILURE;
                }
                flag=0;
                break;
            case ROHC_IPPROTO_IP6 :
                if( ROHC_FAILURE == rohc_parse_ip6_pkt(
                            (struct rohc_ip6_t *)ip_pkt,
                            p_entity,p_temp_fields,
                            len - ((rohc_U8bit_t*)ip_pkt - p_pkt_org ),p_ecode) )
                {
                    return ROHC_FAILURE;
                }
                flag=0;
                break;
            /* START SPR 7923 */
            #ifdef IPV6_PROF6
            case ROHC_IPPROTO_TCP :
                rohc_parse_tcp_pkt((struct rohc_tcp_t *)ip_pkt,
                                                p_temp_fields);
                if((p_temp_fields->p_cpib->esp == ROHC_NULL)&&
                        (p_entity->profilesSupported & ROHC_IP_TCP_SUPPORTED))
                {
                    p_temp_fields->p_cpib->determined_profile = ROHC_IP_TCP;
                    p_temp_fields->p_cpib->expected_profile = ROHC_IP_TCP;
                }
                flag=0; 
                break;
            #endif
            /* END SPR 7923 */
            default :           
                p_temp_fields->p_cpib->determined_profile=ROHC_UNCOMP;  
                p_temp_fields->profile_over=ROHC_TRUE;  
                flag=0; 
                break;
        }/* end switch IPv6 Protocol field*/
    }/* end while (flag)    */
    ROHC_EXIT_FN("rohc_parse_ip6_pkt"); 
    return ROHC_SUCCESS;
}   /*end function rohc_parse_ip6_pkt */

#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_parse_hopopts_pkt
 * Desc     :   HOP OPTS header parsing
 * Params   :   HOP OPTS packet
                Pointer to C-PPM temp context
                Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_parse_hopopts_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_hopopts_t       * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t           *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)   
{
    
    ROHC_ENTER_FN("rohc_parse_hopopts_pkt");
    *p_pkt_len=(p_pkt->len+1)<<3;
    if(ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_HOPOPTS,
                            p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_hopopts_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_parse_hopopts_pkt*/                           
/****************************************************************************
 * Name     :   rohc_parse_dstopts_pkt
 * Desc     :   dest options header parsing
 * Params   :   dest options packet
                Pointer to C-PPM temp context
                Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_parse_dstopts_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_dstoptions_t        * p_pkt _AND_
    struct rohc_c_temp_context_t    * p_temp_fields _AND_
    rohc_U32bit_t           *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)   
{
    
    ROHC_ENTER_FN("rohc_parse_dstopts_pkt");
    *p_pkt_len=(p_pkt->len+1)<<3;
    if(ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_DSTOPTS,
                            p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_dstopts_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_parse_dstopts_pkt */  
/****************************************************************************
 * Name     :   rohc_parse_fragopts_pkt
 * Desc     :   fragment options header parsing
 * Params   :   fragment options packet
                Pointer to C-PPM temp context
                Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_parse_fragopts_pkt 
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_fragopts_t       * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t           *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)   
{
    ROHC_ENTER_FN("rohc_parse_fragopts_pkt ");
    *p_pkt_len=sizeof(rohc_fragopts_t); 
    
    if(ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_FRAGOPTS,
                        p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_fragopts_pkt ");
    return ROHC_SUCCESS;
}/* end function rohc_parse_dstopts_pkt */  
/****************************************************************************
 * Name     :   rohc_parse_routopts_pkt
 * Desc     :   route options header parsing
 * Params   :   route options packet
                Pointer to C-PPM temp context
                Length of header parsed
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_parse_routopts_pkt
    ROHC_PARAMS((p_entity,p_pkt,p_temp_fields,p_pkt_len,p_ecode),
    rohc_entity_t *p_entity _AND_
    struct rohc_routopts_t       * p_pkt _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t               *p_pkt_len _AND_
    rohc_error_code_t   *p_ecode)   
{
    ROHC_ENTER_FN("rohc_parse_routopts_pkt");
    *p_pkt_len=(p_pkt->len+1)<<3;
    if(ROHC_FAILURE == rohc_push_protocol_node(p_entity,ROHC_IPPROTO_ROUTOPTS,
                        p_temp_fields,*p_pkt_len,(rohc_U8bit_t *)p_pkt,p_ecode) )
    {
        return ROHC_FAILURE;
    }
    p_temp_fields->len_parsed+=*p_pkt_len;
    ROHC_EXIT_FN("rohc_parse_routopts_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_parse_routopts_pkt  */    
#endif

/****************************************************************************
 * Name     :   rohc_c_check_ts_stride_beh
 * Desc     :   TS STRIDE behavior determination
 * Params   :   Pointer to CSC context
 * Returns  :   -
 **************************************************************************/
void 
rohc_c_check_ts_stride_beh
    ROHC_PARAMS((p_stream_context),
    struct rohc_csc_rtp_t * p_stream_context)
{
    rohc_U32bit_t   ts_diff,sn_diff;
    
    ROHC_ENTER_FN("rohc_c_check_ts_stride_beh");
   
    /* START_SPR_573_FIX: Corrected calculation of TS stride.*/
    
    if(p_stream_context->no_of_pkt_recvd < ROHC_PKT_BEH_NO)
    
    /* END_SPR_573_FIX */
    {   
        ROHC_CSC_SET_OLD_RTP_TS(p_stream_context,
                        p_stream_context->p_cpib->temp_ts);
        ROHC_CSC_SET_OLD_RTP_SN(p_stream_context,
                        p_stream_context->p_cpib->temp_sn);
        return;
    }       
/*SPR #1393 */
    /* Start for SPR#6563*/
    if(p_stream_context->p_cpib->temp_ts <=
                ROHC_CSC_GET_OLD_RTP_TS(p_stream_context))
    {
     	ts_diff = 0xFFFFFFFF - ROHC_CSC_GET_OLD_RTP_TS(p_stream_context);
     	ts_diff += (p_stream_context->p_cpib->temp_ts + 1); 
        p_stream_context->ts_wrap_flag = ROHC_TRUE; 
    }
    else
    {
    ts_diff= p_stream_context->p_cpib->temp_ts -
                 ROHC_CSC_GET_OLD_RTP_TS(p_stream_context);

        p_stream_context->ts_wrap_flag = ROHC_FALSE;
    }	
    /* End for SPR#6563*/
    /* 
    ** START_SPR_574_FIX: For profile-1, in case SN is dynamic (incremented
    ** in every packet), packets drop is seen after SN rollover. Corrected the
    ** sn_diff calculation by typecasting it into its original type. 
    */
    sn_diff= (rohc_U16bit_t)(p_stream_context->p_cpib->temp_sn -
              ROHC_CSC_GET_OLD_RTP_SN(p_stream_context));
    /* END_SPR_574_FIX */
    /* Start for SPR#6563*/
    if ( sn_diff ) {
    ROHC_CSC_SET_TS_STRIDE(p_stream_context,ts_diff/sn_diff);}
    /* End for SPR#6563*/
    ROHC_CSC_SET_OLD_RTP_TS(p_stream_context,
            p_stream_context->p_cpib->temp_ts);
    ROHC_CSC_SET_OLD_RTP_SN(p_stream_context,
            p_stream_context->p_cpib->temp_sn);

    ROHC_EXIT_FN("rohc_c_check_ts_stride_beh");
} /* end function rohc_c_check_ts_stride_beh*/

/****************************************************************************
 * Name     :   rohc_c_check_ack_stride_beh
 * Desc     :   ACK STRIDE behavior determination
 * Params   :   Pointer to CSC context
 * Returns  :   -
 **************************************************************************/
void 
rohc_c_check_ack_stride_beh
    ROHC_PARAMS((p_stream_context),
    struct rohc_csc_tcp_t * p_stream_context)
{
    rohc_U32bit_t  ack_diff;
    
    ROHC_ENTER_FN("rohc_c_check_ack_stride_beh");
 
    if(p_stream_context->p_cpib->temp_ack <=
                ROHC_CSC_GET_OLD_TCP_ACK(p_stream_context))
    {

        ROHC_CSC_SET_ACK_STRIDE(p_stream_context,0);
        ROHC_CSC_SET_OLD_TCP_ACK(p_stream_context,
            p_stream_context->p_cpib->temp_ack);
    }
    else
    {
    ack_diff= p_stream_context->p_cpib->temp_ack -
                 ROHC_CSC_GET_OLD_TCP_ACK(p_stream_context);

    ROHC_CSC_SET_ACK_STRIDE(p_stream_context,ack_diff);
    ROHC_CSC_SET_OLD_TCP_ACK(p_stream_context,
            p_stream_context->p_cpib->temp_ack);
    }

    ROHC_EXIT_FN("rohc_c_check_ack_stride_beh");
} /* end function rohc_c_check_ack_stride_beh*/

#ifdef ROHC_FUNC_ENHANCEMENT
/****************************************************************************
 * Name     :   rohc_push_protocol_node
 * Desc     :   append protocol node to extension header list in CPIB
 * Params   :   Protocol Identifier
                Pointer to C_PPM temporary context
                Length of packet
                Pointer to packet   
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t 
rohc_push_protocol_node
    ROHC_PARAMS((p_entity,proto,p_temp_fields,pkt_len,p_pkt,p_ecode),
    rohc_entity_t *p_entity _AND_
    rohc_U8bit_t proto _AND_
    struct rohc_c_temp_context_t * p_temp_fields _AND_
    rohc_U32bit_t pkt_len _AND_
    rohc_U8bit_t    * p_pkt _AND_
    rohc_error_code_t   *p_ecode)
{                       
    struct rohc_protocol_node_t * node=PNULL;    
    
    ROHC_ENTER_FN("rohc_push_protocol_node");
    node=(struct rohc_protocol_node_t *)ROHC_MALLOC(
                    sizeof(rohc_protocol_node_t));
    if(!node)
    {
        ROHC_LOG(LOG_INFO,"CPPM :EID[%u:%u]:"
                    " Unable to allocate memory for protocol node",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId);
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_FAILURE;
    }
    node->protocol=proto;
    node->length=pkt_len;
    node->ptr=p_pkt;
    
    if(p_temp_fields->cur_ip == ROHC_INNERIP)
    {
        rohc_db_add_last_node(&p_temp_fields->p_cpib->inneripextlist,
                    (rohc_list_node_t *)node);
    }
    else if(p_temp_fields->cur_ip == ROHC_OUTERIP)
    {
        rohc_db_add_last_node(&p_temp_fields->p_cpib->outeripextlist,
                    (rohc_list_node_t *)node);
    }
    /*START_COVERITY_25503_FIX*/
    else
    {
        ROHC_FREE(node);
        node = ROHC_NULL;
    }
    /*END_COVERITY_25503_FIX*/
    ROHC_EXIT_FN("rohc_push_protocol_node");
    return ROHC_SUCCESS;
}   /* end if rohc_push_protocol_node */
#endif

/****************************************************************************
 * Name     :   rohc_esp_beh_det
 * Desc     :   ESP Profiledetermination
 * Params   :   Pointer to CPIB
 *          :   Pointer to Behavior determination block
 *          :   Pointer to IP Packet
 *          :   Pointer to memory location where pointer to BD is to be output
 * Returns  :   -
 **************************************************************************/
/*
void 
rohc_esp_beh_det
    ROHC_PARAMS((p_cpib,p_beh_det,p_pkt,p_p_beh_det_blk),
    struct rohc_cpib_t              * p_cpib _AND_
    struct rohc_beh_det_blk_t       * p_beh_det _AND_
    rohc_U8bit_t                    * p_pkt _AND_
    struct rohc_beh_det_blk_t       * * p_p_beh_det_blk)
{
    rohc_U8bit_t i=0,flag=0;
    
    ROHC_ENTER_FN("rohc_esp_beh_det");
    if(p_beh_det->csc)
        return;
    p_beh_det->no_of_pkt_recvd++;
    if( p_beh_det->no_of_pkt_recvd == ROHC_PKT_BEH_NO)
    {
        for(i=0;i<ROHC_PKT_BEH_NO;i++)
        {
            if(p_beh_det->cpib_array[i]->determined_profile != 
                p_beh_det->cpib_array[i+1]->determined_profile )    
            {   flag=1; }   
        }
        if(flag ==1)
        {
            for(i=0;i<ROHC_PKT_BEH_NO;i++)
            {
                p_beh_det->cpib_array[i]->determined_profile = 
                ROHC_IP_ESP;
            }
        }   
        *p_p_beh_det_blk=p_beh_det;
    }
    else
    {
        p_beh_det->cpib_array[p_beh_det->no_of_pkt_recvd]=
            p_cpib;
        p_beh_det->pkt_array[p_beh_det->no_of_pkt_recvd]=
            p_pkt;
    }
    ROHC_EXIT_FN("rohc_esp_beh_det");
}
*//* end function rohc_esp_beh_det */   
/***************************************************************************/

