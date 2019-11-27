/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_c_crc_main.c,v 1.3.36.3 2010/11/09 05:10:10 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definitions for rohc
 *                     compressor spcific CRC Computation.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_c_crc_main.c,v $
 * Revision 1.3.36.3  2010/11/09 05:10:10  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:43:03  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/09/23 05:05:47  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/27 13:50:15  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_c_typ.h"
#include "rohc_g_typ.h"
#include "rohc_com_def.h"
#include "rohc_protocol_header.h"
#include "rohc_c_crc_proto.h"
#include "rohc_db_ext_if.h"
#include "rohc_trace_mcr.h"
#include "rohc_gcom_mcr.h"

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




/*****************************************************************************
 * Function Name  : rohc_calculate_crc
 * Inputs         : data, start, end, previous_crc, crc_type
 * Outputs        : None
 * Returns        : CRC
 * Description    : Calculates CRC
 *****************************************************************************/
rohc_U8bit_t    rohc_calculate_crc(void * data, rohc_U32bit_t start,rohc_U32bit_t end, rohc_U8bit_t previous_crc, rohc_U8bit_t crc_type)
{
    return rohc_compute_crc(crc_type,((rohc_U8bit_t *)(data))+start-1,end-start+1,previous_crc);
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_csrc_list_crc_p1
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes list CRC
 *****************************************************************************/
rohc_U8bit_t  rohc_c_compute_csrc_list_crc_p1(rohc_csc_rtp_t    *p_csc,  
                                              rohc_U8bit_t      crc_type, 
                                              rohc_U8bit_t      crc_val)
{
    rohc_U32bit_t       *p_csrc_node;
    rohc_U32bit_t       tot_count = 0;

    p_csrc_node = (rohc_U32bit_t *)((p_csc->p_cpib->rtp) + sizeof(rohc_rtp_t));
    tot_count = (((rohc_rtp_t *)(p_csc->p_cpib->rtp))->v_p_x_cc & ROHC_CC_MASK);

    return rohc_calculate_crc(p_csrc_node,1,(4*tot_count),crc_val,crc_type);

}

/*****************************************************************************
 * Function Name  : rohc_c_compute_ip_static_crc_p1
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes Static IP CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_ip_static_crc_p1 (rohc_csc_rtp_t    *p_csc, 
                                              rohc_U8bit_t      crc_type, 
                                              rohc_U8bit_t      crc_val)
{
    int list_count,i;
    rohc_list_node_t *node; 

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,1,2,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,7,10,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,13,20,
                                                crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,1,4,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,7,40,
                                                crc_val,crc_type);
    }
    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,1,2,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,7,10,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,13,20,
                                                crc_val,crc_type);

    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,1,4,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,7,40,
                                                crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_csc->p_cpib->inneripextlist));
     node = rohc_db_get_first_node(&(p_csc->p_cpib->inneripextlist));
    if(node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,"Empty Extention header list found");
        /* SPR 3444 Changes start*/
        return crc_val;
        /*SPR 3444 Changes End*/
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_GRE:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
                    case ROHC_IPPROTO_FRAGOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_AH:
            {
                /* no static crc req. all fields are crc dynamic*/
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
                /*  no static crc req. all fields are crc dynamic */
                break;

            }
            default:
            ROHC_LOG(LOG_INFO,"Unknown IP Extention header found");

        }
        node = rohc_db_get_next_node(node);
    }

    return crc_val;

}

/*****************************************************************************
 * Function Name  : rohc_c_compute_ip_static_crc_p2
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes Static IP CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_ip_static_crc_p2 (rohc_csc_udp_t *p_csc, 
                                              rohc_U8bit_t crc_type, 
                                              rohc_U8bit_t crc_val)
{
    int list_count,i;
    rohc_list_node_t *node; 

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                                1,2,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                                7,10,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                                13,20,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,
                                                1,4,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,
                                                7,40,crc_val,crc_type);
    }
    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                                1,2,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                                7,10,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                                13,20,crc_val,crc_type);

    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,
                                                1,4,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,
                                                7,40,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_csc->p_cpib->inneripextlist));
     node = rohc_db_get_first_node(&(p_csc->p_cpib->inneripextlist));
    if(node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,"Empty Extention header list found");
        /* SPR  3444 changes start */
        return crc_val;
        /* SPR  3444 changes end */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_GRE:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
                    case ROHC_IPPROTO_FRAGOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_AH:
            {
                /* no static crc req. all fields are crc dynamic*/
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
                /*  no static crc req. all fields are crc dynamic */
                break;

            }
            default:
            ROHC_LOG(LOG_INFO,"Unknown IP Extention header found");

        }
        node = rohc_db_get_next_node(node);
    }

    return crc_val;

}

/*****************************************************************************
 * Function Name  : rohc_c_compute_ip_dynamic_crc_p1
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes IP Dynamic CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_ip_dynamic_crc_p1 (rohc_csc_rtp_t   *p_csc, 
                                               rohc_U8bit_t     crc_type, 
                                               rohc_U8bit_t     crc_val)
{
    rohc_U8bit_t    *p_buf = ROHC_NULL, *p_temp = ROHC_NULL;
    rohc_U8bit_t    list_count;
    rohc_U16bit_t   gre_flags;
    rohc_list_node_t *p_node;
    rohc_U32bit_t i;

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,3,4,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,5,6,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,11,12,
                                                crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,5,6,
                                                crc_val,crc_type);
    }
    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,3,4,
                                                crc_val,crc_type);
        
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,5,6,
                                                crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,11,12,
                                                crc_val,crc_type);

    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,5,6,
                                                crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_csc->p_cpib->inneripextlist));
    p_node  = (rohc_list_node_t *)
               rohc_db_get_first_node(&(p_csc->p_cpib->inneripextlist)) ;
    
    if(p_node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,"Empty Extention header list found");
        /* SPR  3444 changes start */
        return crc_val;
        /* SPR  3444 changes end */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)p_node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_AH:
            {
             /*   // no static crc req. all fields are crc dynamic*/
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
/*                //  no static crc req. all fields are crc dynamic*/
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,1,
                          rohc_proto_node->length,crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_GRE:
            {
                p_buf = ((rohc_protocol_node_t *)p_node)->ptr;
                gre_flags = ((rohc_gre_t *)p_buf)->flags;
                p_temp = p_buf + sizeof(rohc_gre_t);
                if(gre_flags & ROHC_GRE_CP)
                {
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_KP)
                {
                    /* Key present */
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_SP)
                {
                    crc_val = rohc_calculate_crc(p_temp,1,4,crc_val,crc_type);
                }
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_FRAGOPTS:
            {
 /*               //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
  /*              //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            default:
            ROHC_LOG(LOG_INFO,"Unknown IP Extention header found");

        }
        p_node = (rohc_list_node_t *)rohc_db_get_next_node(p_node);
    }

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_ip_dynamic_crc_p2
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes IP Dynamic CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_ip_dynamic_crc_p2 (rohc_csc_udp_t *p_csc, 
                                               rohc_U8bit_t crc_type, 
                                               rohc_U8bit_t crc_val)
{
    rohc_U8bit_t    *p_buf = ROHC_NULL, *p_temp = ROHC_NULL;
    rohc_U8bit_t    list_count;
    rohc_U16bit_t   gre_flags;
    rohc_list_node_t *p_node;
    rohc_U32bit_t i;

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                            3,4,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                            5,6,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip4,
                                            11,12,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->inner_ip6,
                                            5,6,crc_val,crc_type);
    }
    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                            3,4,crc_val,crc_type);
        
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                            5,6,crc_val,crc_type);
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip4,
                                            11,12,crc_val,crc_type);

    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_calculate_crc(p_csc->p_cpib->outer_ip6,
                                            5,6,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_csc->p_cpib->inneripextlist));
    p_node  = (rohc_list_node_t *)rohc_db_get_first_node(&(p_csc->p_cpib->inneripextlist)) ;
    
    if(p_node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,"Empty Extention header list found");
	/* SPR  3444 changes start */
       return crc_val; 
	/* SPR  3444 changes end */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)p_node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_AH:
            {
             /*   // no static crc req. all fields are crc dynamic*/
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
/*                //  no static crc req. all fields are crc dynamic*/
                crc_val = rohc_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_GRE:
            {
                p_buf = ((rohc_protocol_node_t *)p_node)->ptr;
                gre_flags = ((rohc_gre_t *)p_buf)->flags;
                p_temp = p_buf + sizeof(rohc_gre_t);
                if(gre_flags & ROHC_GRE_CP)
                {
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_KP)
                {
                    /* Key present */
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_SP)
                {
                    crc_val = rohc_calculate_crc(p_temp,1,4,crc_val,crc_type);
                }
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_FRAGOPTS:
            {
 /*               //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
  /*              //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            default:
            ROHC_LOG(LOG_INFO,"Unknown IP Extention header found");

        }
        p_node = (rohc_list_node_t *)rohc_db_get_next_node(p_node);
    }

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_ip_p6
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes  IP CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_ip_p6 (rohc_csc_tcp_t    *p_csc, 
                                              rohc_U8bit_t      crc_type, 
                                              rohc_U8bit_t      previous_crc)
{
    rohc_U8bit_t      crc_val;

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        crc_val =  rohc_compute_crc(crc_type,p_csc->p_cpib->inner_ip4,
                ROHC_IPV4_MIN_HDR_LEN,previous_crc);
    }
    else
    {
        /* ipv6 */
        crc_val =  rohc_compute_crc(crc_type,p_csc->p_cpib->inner_ip6,
                sizeof(struct rohc_ip6_t),previous_crc);
    }
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_tcp_p6
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes  IP CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_tcp_p6 (rohc_csc_tcp_t    *p_csc, 
                                              rohc_U8bit_t      crc_type, 
                                              rohc_U8bit_t      previous_crc)
{
    rohc_U8bit_t      crc_val;

    crc_val =  rohc_compute_crc(crc_type,p_csc->p_cpib->tcp,
            (ROHC_GET_UPPER_NIBBLE(((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset))*4,
            previous_crc);
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_static_p1
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes Static CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_static_p1 (
                                            rohc_csc_rtp_t  *p_csc,  
                                            rohc_U8bit_t    crc_type,
                                            rohc_U8bit_t    crc_val)
{
    /*START - static crc calc */
    crc_val = rohc_c_compute_ip_static_crc_p1(p_csc,crc_type,crc_val);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->udp,1,4,crc_val,crc_type);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->rtp,1,1,crc_val,crc_type);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->rtp,9,12,crc_val,crc_type);
    crc_val = rohc_c_compute_csrc_list_crc_p1(p_csc,crc_type,crc_val);
    /*END - static crc calc */

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_static_p2
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes Static CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_static_p2 (rohc_csc_udp_t *p_csc,  
        rohc_U8bit_t crc_type,
        rohc_U8bit_t crc_val)
{
    /*START - static crc calc */
    crc_val = rohc_c_compute_ip_static_crc_p2(p_csc,crc_type,crc_val);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->udp,1,4,crc_val,crc_type);
    /*END - static crc calc */
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_dynamic_p1
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes dynamic CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_dynamic_p1 (rohc_csc_rtp_t  *p_csc,  
                                            rohc_U8bit_t    crc_type, 
                                            rohc_U8bit_t    crc_val )
{

    /*START - dyanamic crc calc */
    crc_val = rohc_c_compute_ip_dynamic_crc_p1 (p_csc,crc_type,crc_val);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->udp,5,8,crc_val,crc_type);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->rtp,2,8,crc_val,crc_type);

    /*END - dyanamic crc calc */
    return crc_val;
}
/*****************************************************************************
 * Function Name  : rohc_c_compute_crc_dynamic_p2
 * Inputs         : p_csc, crc_type, crc_val
 * Outputs        : None
 * Returns        : CRC
 * Description    : Computes dynamic CRC
 *****************************************************************************/
rohc_U8bit_t rohc_c_compute_crc_dynamic_p2 (rohc_csc_udp_t *p_csc,  
                                            rohc_U8bit_t crc_type, 
                                            rohc_U8bit_t crc_val )
{
    /*START - dyanamic crc calc */
    crc_val = rohc_c_compute_ip_dynamic_crc_p2 (p_csc,crc_type,crc_val);
    crc_val = rohc_calculate_crc(p_csc->p_cpib->udp,5,8,crc_val,crc_type);
    /*END - dyanamic crc calc */
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_c_crc_p1_compute
 * Inputs         : p_csc, type
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : Computes CRC
 *****************************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_c_crc_p1_compute ROHC_PARAMS((p_csc,type),
                        rohc_csc_rtp_t  *p_csc      _AND_
                        rohc_U8bit_t    type)
{
    /* SPR5816 Fix Start */
    rohc_U8bit_t crc_val=0xFF;
    /* SPR5816 Fix End */
    crc_val = rohc_c_compute_crc_static_p1(p_csc, type, crc_val);
    if(type == p_csc->crc_typ && crc_val==p_csc->crc_val)
    {
        return ROHC_FAILURE;
    }
    p_csc->field_value[ROHC_CRC_FID-ROHC_VAR_FID_BASE]
		= rohc_c_compute_crc_dynamic_p1(p_csc, type, crc_val);
    p_csc->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE] = type;
    return ROHC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : rohc_c_crc_p2_compute
 * Inputs         : p_csc, type
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : Computes CRC
 *****************************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_c_crc_p2_compute ROHC_PARAMS((p_csc,type),
                        rohc_csc_udp_t *p_csc    _AND_
                        rohc_U8bit_t   type)
{
    /* SPR5816 Fix Start */
    rohc_U8bit_t crc_val=0xFF;
    /* SPR5816 Fix End */
    crc_val = rohc_c_compute_crc_static_p2(p_csc, type, crc_val);
    if(type == p_csc->crc_typ && crc_val==p_csc->crc_val)
    {
        return ROHC_FAILURE;
    }
    p_csc->field_value[ROHC_CRC_FID-ROHC_VAR_FID_BASE]
		= rohc_c_compute_crc_dynamic_p2(p_csc, type, crc_val);
    p_csc->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE] = type;
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_c_crc_p6_compute
 * Inputs         : p_csc, type
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : Computes CRC
 *****************************************************************************/
 /* +- SPR 17777 */
rohc_return_t rohc_c_crc_p6_compute ROHC_PARAMS((p_csc,type),
                        rohc_csc_tcp_t  *p_csc      _AND_
                        rohc_U8bit_t    type)
 /* +- SPR 17777 */
{
    /*Coverity Fix 40845 Start*/
    rohc_U8bit_t crc_val = 0x00;
    /*Coverity Fix 40845 End*/
    if(ROHC_CRC_TYPE_7 == type)
    {
        crc_val = 0x7f;
    }

    if(ROHC_CRC_TYPE_3 == type)
    {
        crc_val = 0x07;
    }
    crc_val = rohc_c_compute_crc_ip_p6(p_csc, type, crc_val);
    if(type == p_csc->crc_typ && crc_val==p_csc->crc_val)
    {
        return ROHC_FAILURE;
    }
    p_csc->field_value[ROHC_CRC_FID-ROHC_VAR_FID_BASE]
		= rohc_c_compute_crc_tcp_p6(p_csc, type, crc_val);
    p_csc->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE] = type;
    return ROHC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : rohc_init_crc
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes CRC
 *****************************************************************************/
rohc_void_t rohc_init_crc()
{
       init_crc();
}
