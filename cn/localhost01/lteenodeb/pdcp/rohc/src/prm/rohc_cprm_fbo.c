/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_fbo.c,v 1.6.34.3 2010/11/09 05:23:52 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the entry points of the CPM module. It
 *                     is for the compressor and contains the event handlers.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_fbo.c,v $
 * Revision 1.6.34.3  2010/11/09 05:23:52  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.6.34.2  2010/10/13 03:56:42  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.6.34.1  2010/09/23 05:19:57  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.6  2009/05/28 13:50:43  gur19140
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/28 04:26:50  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_cprm_proto.h"
#include "rohc_ccpm_ext_if.h"
#include "rohc_led_ext_if.h"
#include "rohc_stats_def.h"
#include "rohc_opt_proto.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
static rohc_return_t rohc_cprm_p1_process_fb_type1  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,
            rohc_csc_rtp_t      *p_csc      ,
            rohc_U8bit_t        *tmp_buff   ,
            rohc_error_code_t   *p_ecode                        
            ));                                       

static rohc_return_t rohc_cprm_p1_process_fb_type2  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,
            rohc_csc_rtp_t      *p_csc      ,
            rohc_U8bit_t        *tmp_buff   ,
            rohc_U8bit_t        len         ,
            rohc_error_code_t   *p_ecode                        
            ));                                       

static rohc_return_t rohc_cprm_p2_process_fb_type1  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,
            rohc_csc_udp_t      *p_csc      ,
            rohc_U8bit_t        *tmp_buff   ,
            rohc_error_code_t   *p_ecode                        
            ));                                       

static rohc_return_t rohc_cprm_p2_process_fb_type2  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,
            rohc_csc_udp_t      *p_csc      ,
            rohc_U8bit_t        *tmp_buff   ,
            rohc_U8bit_t        len         ,
            rohc_error_code_t   *p_ecode                        
            ));                                       
/* - SPR 17439 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* assumed ROHC_MODE_U is minimum */
const rohc_U8bit_t 
rohc_init_mt_matix[ROHC_MODE_R - ROHC_MODE_U +1 ][ROHC_MODE_R - ROHC_MODE_U +1] =
{
    {ROHC_MT_NOT_REQ,       ROHC_C_U_MT_U_TO_O_ET,  ROHC_C_U_MT_U_TO_R_ET},
    {ROHC_C_O_MT_O_TO_U_ET, ROHC_MT_NOT_REQ,        ROHC_C_O_MT_O_TO_R_ET},
    {ROHC_C_R_MT_R_TO_U_ET, ROHC_C_R_MT_R_TO_U_ET,  ROHC_MT_NOT_REQ},
};
/* for ack-nack-static nack */
const rohc_U8bit_t 
rohc_init_ans_matix[ROHC_MODE_R - ROHC_MODE_U +1 ][ROHC_STATIC_NACK-ROHC_ACK+1] =
{
    {ROHC_C_U_ACK_U_ET,     0,  0},
    {ROHC_C_O_ACK_O_ET, ROHC_C_O_NACK_O_ET,     ROHC_C_O_STATIC_NACK_O_ET},
    {ROHC_C_R_ACK_R_ET, ROHC_C_R_NACK_ET,  ROHC_C_R_STATIC_NACK_ET},
};
const rohc_U8bit_t rohc_cprm_mode[3]={"UOR"};

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* SPR 14084 fix start */
/* flag ROHC_MODE_TRIGGER has been removed */
/* SPR 14084 fix end */
static const rohc_U8bit_t
rohc_c_mt_stats_matrix[ROHC_MODE_R - ROHC_MODE_U +1 ][ROHC_MODE_R - ROHC_MODE_U +1] =
{
    {0, ROHC_NO_OF_MODE_TRANS_U_TO_O_RCVD,  ROHC_NO_OF_MODE_TRANS_U_TO_R_RCVD},
    {ROHC_NO_OF_MODE_TRANS_O_TO_U_RCVD,0,   ROHC_NO_OF_MODE_TRANS_O_TO_R_RCVD},
    {ROHC_NO_OF_MODE_TRANS_R_TO_U_RCVD,ROHC_NO_OF_MODE_TRANS_R_TO_O_RCVD,0}
}; 


/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p1_pr_fb                                            
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**                0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              | 1   1   1   1   0 |   Code    |  feedback type octet
**              +---+---+---+---+---+---+---+---+
**              :             Size              :  if Code = 0
**              +---+---+---+---+---+---+---+---+
**              /         feedback data         /  variable length
**              +---+---+---+---+---+---+---+---+
**              
**              
**                 0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              :         Add-CID octet         : if for small CIDs and (CID != 0)
**              +---+---+---+---+---+---+---+---+
**              :                               :
**              /  large CID (4.5.6 encoding)   / 1-2 octets if for large CIDs
**              :                               :
**              +---+---+---+---+---+---+---+---+
**  p_buff ->   /           feedback            / 
**              +---+---+---+---+---+---+---+---+
**           
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              p_buff  [IN]      Pointer to the feedback buffer
**              len     [IN]      Length of p_buff  
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
                                                                                 
rohc_return_t rohc_cprm_p1_pr_fb  ROHC_PARAMS(         
            (p_entity, p_csc, p_buff, len, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            rohc_csc_rtp_t      *p_csc      _AND_                 
            rohc_U8bit_t        *p_buff     _AND_              
            rohc_U8bit_t        len         _AND_
            rohc_error_code_t   *p_ecode                        
            )                                       
{                                                   
    rohc_return_t return_value = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_cprm_p1_pr_fb");

    /* SPR 13319 fix start */
    rohc_U8bit_t  feedback_len = 0; 
    rohc_U8bit_t  fb_size_present = 0; 
    rohc_U8bit_t  fb_code = 0; 

    fb_code = *p_buff & ROHC_FB_CODE_MASK;

    if (fb_code) /* code field is non 0, so size field is not present in FB */
    {    
        feedback_len = fb_code;
    }    
    else 
    {    
        feedback_len = *(p_buff+1);
        fb_size_present = 1; /* size field is present in the feedback packet */
    }    
    /* FB-1 */
    if (feedback_len == 1)
    {    
        return_value = rohc_cprm_p1_process_fb_type1(p_entity, p_csc,
                p_buff + fb_size_present, p_ecode);
    }    
    else /* FB-2*/
    {    
        return_value = rohc_cprm_p1_process_fb_type2(p_entity, p_csc,
                p_buff + fb_size_present, len, p_ecode);
    }    
    /* SPR 13319 fix end */

    ROHC_EXIT_FN("rohc_cprm_p1_pr_fb");
    return return_value;
}

/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p1_process_fb_type1                                            
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              tmp_buff[IN]      Pointer to the feedback buffer
**              p_ecode [OUT]     Error code 
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
                                                                                 
static rohc_return_t rohc_cprm_p1_process_fb_type1  ROHC_PARAMS(         
            (p_entity, p_csc, tmp_buff, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            rohc_csc_rtp_t      *p_csc      _AND_                 
            rohc_U8bit_t        *tmp_buff     _AND_              
            rohc_error_code_t   *p_ecode                        
            )                                       
{
    rohc_U8bit_t temp = 0;                          
    rohc_U32bit_t sn  = 0;                    
    rohc_U8bit_t mode = 0;                       

    ROHC_ENTER_FN("rohc_cprm_p1_process_fb_type1");
    /* SPR 13319 fix start */
    tmp_buff++;
    tmp_buff++;
    /* SPR 13319 fix end */
    temp  = ROHC_GET_U8BIT(tmp_buff);

    sn = temp;
    /* :- call encode-decode to verify the validatity */
    if(rohc_ack_handling_p1(p_csc,sn,ROHC_ACK))
    {
        /* call list compression  */
        /* +- SPR 17777 */
        if(ROHC_FAILURE == rohc_cled_p1_ack_handling(p_csc,
                    (rohc_U16bit_t)sn))
            return ROHC_FAILURE;
        /*:- call cpm with ACK event */
        if((mode == p_csc->c_mode))
        {   
            rohc_ccpm_p1_exec_direct_evt(p_entity ,p_csc,
                    ROHC_ACK_ET,p_ecode);
        }   
    }
    else
    {

        ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                "ROHC_ACK received with SN[%u]\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,\
                ROHC_GET_CSC_CID(p_csc), sn);

    }
    return ROHC_SUCCESS;
}


/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p1_process_fb_type2                                            
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              tmp_buff[IN]      Pointer to the feedback buffer
**              p_ecode [OUT]     Error code 
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
                                                                                 
static rohc_return_t rohc_cprm_p1_process_fb_type2  ROHC_PARAMS(         
            (p_entity, p_csc, tmp_buff, len, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            rohc_csc_rtp_t      *p_csc      _AND_                 
            rohc_U8bit_t        *tmp_buff   _AND_              
            rohc_U8bit_t        len         _AND_
            rohc_error_code_t   *p_ecode                        
            )                                       
{
    rohc_U8bit_t temp = 0;                          
    rohc_U32bit_t sn  = 0;                    
    rohc_U8bit_t mode = 0;                       
    rohc_U8bit_t read_oct = 0;
    rohc_U8bit_t acktype  =  0;                     
    rohc_U8bit_t sn_not_vaild  = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_cprm_p1_process_fb_type2");

    tmp_buff ++;
    tmp_buff ++;
    read_oct += 2;
    temp  = ROHC_GET_U8BIT(tmp_buff);
    read_oct++;
    sn = temp & ROHC_FB_SN_MASK;
    acktype = ((temp & ROHC_FB_ACKT_MASK) >> 6);
    mode  =  ((temp & ROHC_FB_MODE_MASK) >> 4);
    tmp_buff ++;
    sn = (sn<<8)| ROHC_GET_U8BIT(tmp_buff);
    tmp_buff++;
    read_oct++;

    while(read_oct < len)
    {
        temp = ROHC_GET_U8BIT(tmp_buff++);
        read_oct++;
        switch(( (temp&ROHC_FB_OPTYP_MASK) >> 4) )
        {
            case ROHC_FBO_CRC: 
                /* don't need to any thing crc is applied by main module */
                /*temp = ROHC_GET_U8BIT(tmp_buff++);*/
                tmp_buff++;
                read_oct++;
                break;          

            case ROHC_FBO_REJECT: 

                p_entity->dont_create_csc_flag = ROHC_TRUE;
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_REJECT received",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_CSC_CID(p_csc));
                break;      
            case ROHC_FBO_SN_NOT_VALID:
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_SN_NOT_VALID received\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_CSC_CID(p_csc));
                sn_not_vaild = ROHC_TRUE;
                break;

            case ROHC_FBO_SN      :
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                sn = (sn << 8) | temp ;
                break;      
            case ROHC_FBO_CLOCK   :
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_CLOCK received\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc));
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                p_csc->clk_resolution = temp;
                p_csc->jit_clk_res_flag |= ROHC_CLK_RES_PRESENT;
                break;      
            case ROHC_FBO_JITTER  :
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_JITTER received\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc));
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                p_csc->max_jitter = temp;
                p_csc->jit_clk_res_flag |= ROHC_JIT_PRESENT;
                break;      
            case ROHC_FBO_LOSS    :
                /* need to adjust the size of reference window */
                /* As it is not mandatory, nothing is done here */
                /* If possible will be taken care in next release */ 
                /*          temp = ROHC_GET_U8BIT(tmp_buff++); */
                tmp_buff++;
                read_oct++;
                break;        

        }   

    }

    /*  call ED to verify the existence of SN in GSW */

    switch(acktype)
    {                    
        case ROHC_ACK        :
            if(!sn_not_vaild)
            {
                if(rohc_ack_handling_p1(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /* +- SPR 17777 */
                    if(ROHC_FAILURE == rohc_cled_p1_ack_handling(p_csc,
                                (rohc_U16bit_t)sn))
                        return ROHC_FAILURE;
                    /*call cpm with ACK event */   
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_ACK received with SN[%u]\n",                                              
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfACKProcessed++;

                    p_csc->ack_recvd = ROHC_TRUE;
                    if(mode == p_csc->c_mode)
                    {   
                        rohc_ccpm_p1_exec_direct_evt(p_entity ,p_csc,
                                rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                                [acktype - ROHC_ACK],
                                p_ecode);
                    }
                }
                else
                {

                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "Element is not found in GSW "
                            "for the received ACK with SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);
                }
            }   
            else
            {

                *p_ecode = ROHC_INVALID_ACK_RECEIVED;
                /* SPR 9591 fix start */
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_ACK received with invalid SN [%u]\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                /* SPR 9591 fix end */
            }    
            break;
        case ROHC_NACK  :
            {
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_NACK received with SN[%u]",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                if(rohc_ack_handling_p1(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /*rohc_cled_ack_handling(p_csc,sn);*/
                    /*call cpm with ACK event */   

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfNACKProcessed++;

                    rohc_ccpm_p1_exec_direct_evt(p_entity ,p_csc ,
                            rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                            [acktype - ROHC_ACK],  p_ecode);
                }
                else
                {

                    *p_ecode = ROHC_INVALID_NACK_RECEIVED;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_NACK received with invalid SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);     
                }    

            }
            break;
        case ROHC_STATIC_NACK:
            {
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_STATIC_NACK received with SN[%u]\n",                                              
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                if(rohc_ack_handling_p1(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /*rohc_cled_ack_handling(p_csc,sn);*/
                    /*call cpm with ACK event */   

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfSNACKProcessed++;

                    /* call cpm with ACK event */                       
                    rohc_ccpm_p1_exec_direct_evt(p_entity ,p_csc ,
                            rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                            [acktype - ROHC_ACK],p_ecode);
                }
                else
                {

                    *p_ecode = ROHC_INVALID_STATIC_NACK_RECEIVED;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_ACK received with invalid SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);     
                }    

            }break;

    }
    /* initiate mode transition */
    if(rohc_init_mt_matix[p_csc->c_mode -ROHC_MODE_U][mode-ROHC_MODE_U])
    {
        ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                "Initiating mode transition %c->%c\n",                                           
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,\
                ROHC_GET_CSC_CID(p_csc),
                rohc_cprm_mode[p_csc->c_mode - ROHC_MODE_U],
                rohc_cprm_mode[mode - ROHC_MODE_U]);

        /* Incrementing Stats */
        /* SPR 14084 fix start */
        /* flag ROHC_MODE_TRIGGER has been removed */
        /* SPR 14084 fix end */
        rohcStatsModeTransUpdate(rohc_c_mt_stats_matrix
                [p_csc->c_mode -ROHC_MODE_U][mode-ROHC_MODE_U],
                ROHC_DIR_TX);

        rohc_ccpm_p1_exec_direct_evt(p_entity, p_csc,
                rohc_init_mt_matix[p_csc->c_mode -ROHC_MODE_U]
                [mode-ROHC_MODE_U], p_ecode);

    }
    return ROHC_SUCCESS;
}
/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p2_pr_fb                                            
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**                0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              | 1   1   1   1   0 |   Code    |  feedback type octet
**              +---+---+---+---+---+---+---+---+
**              :             Size              :  if Code = 0
**              +---+---+---+---+---+---+---+---+
**              /         feedback data         /  variable length
**              +---+---+---+---+---+---+---+---+
**              
**              
**                 0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              :         Add-CID octet         : if for small CIDs and (CID != 0)
**              +---+---+---+---+---+---+---+---+
**              :                               :
**              /  large CID (4.5.6 encoding)   / 1-2 octets if for large CIDs
**              :                               :
**              +---+---+---+---+---+---+---+---+
**  p_buff ->   /           feedback            / 
**              +---+---+---+---+---+---+---+---+
**           
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              p_buff  [IN]      Pointer to the feedback buffer
**              len     [IN]      Length of p_buff  
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
rohc_return_t rohc_cprm_p2_pr_fb  ROHC_PARAMS(         
            (p_entity, p_csc, p_buff, len, p_ecode),              
            rohc_entity_t     *p_entity _AND_           
            rohc_csc_udp_t    *p_csc    _AND_                 
            rohc_U8bit_t      *p_buff   _AND_              
            rohc_U8bit_t      len       _AND_
            rohc_error_code_t *p_ecode                        
            )                                       
{                                                   
    rohc_return_t return_value = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_cprm_p2_pr_fb");

    /* SPR 13319 fix start */
    rohc_U8bit_t  feedback_len = 0; 
    rohc_U8bit_t  fb_size_present = 0; 
    rohc_U8bit_t  fb_code = 0; 

    fb_code = *p_buff & ROHC_FB_CODE_MASK;

    if (fb_code) /* code field is non 0, so size field is not present in FB */
    {    
        feedback_len = fb_code;
    }    
    else 
    {    
        feedback_len = *(p_buff+1);
        fb_size_present = 1; /* size field is present in the feedback packet */
    }    
    /* FB-1 */
    if (feedback_len == 1)
    {    
        return_value = rohc_cprm_p2_process_fb_type1(p_entity, p_csc,
                p_buff + fb_size_present, p_ecode);
    }    
    else /* FB-2*/
    {    
        return_value = rohc_cprm_p2_process_fb_type2(p_entity, p_csc,
                p_buff + fb_size_present, len, p_ecode);
    }    
    /* SPR 13319 fix end */

    ROHC_EXIT_FN("rohc_cprm_p2_pr_fb");
    return return_value;
}


/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p2_process_fb_type1                                            
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              tmp_buff[IN]      Pointer to the feedback buffer
**              p_ecode [OUT]     Error code 
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
static rohc_return_t rohc_cprm_p2_process_fb_type1  ROHC_PARAMS(         
            (p_entity, p_csc, tmp_buff, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            /* + SPR 17439 */
            rohc_csc_udp_t      *p_csc      _AND_                 
            /* - SPR 17439 */
            rohc_U8bit_t        *tmp_buff     _AND_              
            rohc_error_code_t   *p_ecode                        
            )                                       
{
    rohc_U8bit_t temp = 0;                          
    rohc_U32bit_t sn  = 0;                    
    rohc_U8bit_t mode = 0;                       

    ROHC_ENTER_FN("rohc_cprm_p2_process_fb_type1");

    /* SPR 13319 fix start */
    tmp_buff++;
    tmp_buff++;
    /* SPR 13319 fix end */
    temp  = ROHC_GET_U8BIT(tmp_buff);

    sn = temp;
    /* :- call encode-decode to verify the validatity */
    if(rohc_ack_handling_p2(p_csc,sn,ROHC_ACK))
    {
        /* call list compression  */
        /* +- SPR 17777 */
        if(ROHC_FAILURE == rohc_cled_p2_ack_handling(p_csc,
                    (rohc_U16bit_t)sn))
            return ROHC_FAILURE;
        /*:- call cpm with ACK event */
        if((mode == p_csc->c_mode))
        {   
            rohc_ccpm_p2_exec_direct_evt(p_entity ,
                    p_csc,ROHC_ACK_ET,p_ecode);
        }   
    }
    else
    {

        ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                "ROHC_ACK received with SN[%u]\n", 
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,\
                ROHC_GET_CSC_CID(p_csc),sn);

    }
    return ROHC_SUCCESS;
}
                                                    
/*****************************************************************************   
**      FUNCTION:                                                                
**              rohc_cprm_p2_process_fb_type2                                           
******************************************************************************   
**                                                                               
**      DESCRIPTION:                                                             
**              This is funtion provides the handling for received packet  
**              at the compressor side. 
**
**      ARGUMENTS:    
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block  
**              tmp_buff[IN]      Pointer to the feedback buffer
**              p_ecode [OUT]     Error code 
**                                                                      
**      RETURN VALUE:    rohc_return_t
**                                                         
**      NOTES:                                                                   
**                                                                               
*****************************************************************************/   
static rohc_return_t rohc_cprm_p2_process_fb_type2  ROHC_PARAMS(         
            (p_entity, p_csc, tmp_buff, len, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            /* + SPR 17439 */
            rohc_csc_udp_t      *p_csc      _AND_                 
            /* - SPR 17439 */
            rohc_U8bit_t        *tmp_buff   _AND_              
            rohc_U8bit_t        len         _AND_
            rohc_error_code_t   *p_ecode                        
            )                                       
{
    rohc_U8bit_t temp = 0;                          
    rohc_U32bit_t sn  = 0;                    
    rohc_U8bit_t mode = 0;                       
    rohc_U8bit_t read_oct = 0;
    rohc_U8bit_t acktype  =  0;                     
    rohc_U8bit_t sn_not_vaild  = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_cprm_p2_process_fb_type2");

    tmp_buff ++;
    tmp_buff ++;
    read_oct += 2;
    temp  = ROHC_GET_U8BIT(tmp_buff);
    read_oct++;
    sn = temp & ROHC_FB_SN_MASK;
    acktype = ((temp & ROHC_FB_ACKT_MASK) >> 6);
    mode  =  ((temp & ROHC_FB_MODE_MASK) >> 4);
    tmp_buff ++;
    sn = (sn<<8)| ROHC_GET_U8BIT(tmp_buff);
    tmp_buff++;
    read_oct++;

    while(read_oct < len)
    {
        temp = ROHC_GET_U8BIT(tmp_buff++);
        read_oct++;
        switch(( (temp&ROHC_FB_OPTYP_MASK) >> 4) )
        {
            case ROHC_FBO_CRC: 
                /* don't need to any thing crc is applied by main module */
                /*temp = ROHC_GET_U8BIT(tmp_buff++);*/
                tmp_buff++;
                read_oct++;
                break;          

            case ROHC_FBO_REJECT: 

                p_entity->dont_create_csc_flag = ROHC_TRUE;
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_REJECT received",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_CSC_CID(p_csc));
                break;      
            case ROHC_FBO_SN_NOT_VALID:
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_SN_NOT_VALID received\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_CSC_CID(p_csc));
                sn_not_vaild = ROHC_TRUE;
                break;

            case ROHC_FBO_SN      :
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                sn = (sn << 8) | temp ;
                break;      
            case ROHC_FBO_CLOCK   :
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_CLOCK received\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc));
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                p_csc->clk_resolution = temp;
                p_csc->jit_clk_res_flag |= ROHC_CLK_RES_PRESENT;
                break;      
            case ROHC_FBO_JITTER  :
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_FBO_JITTER received\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc));
                temp = ROHC_GET_U8BIT(tmp_buff++);
                read_oct++;
                p_csc->max_jitter = temp;
                p_csc->jit_clk_res_flag |= ROHC_JIT_PRESENT;
                break;      
            case ROHC_FBO_LOSS    :
                /* need to adjust the size of reference window */
                /* As it is not mandatory, nothing is done here */
                /* If possible will be taken care in next release */ 
                /*          temp = ROHC_GET_U8BIT(tmp_buff++); */
                tmp_buff++;
                read_oct++;
                break;        

        }   

    }

    /*  call ED to verify the existence of SN in GSW */

    switch(acktype)
    {                    
        case ROHC_ACK        :
            if(!sn_not_vaild)
            {
                if(rohc_ack_handling_p2(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /* +- SPR 17777 */
                    if(ROHC_FAILURE == rohc_cled_p2_ack_handling(p_csc,
                                (rohc_U16bit_t)sn))
                        return ROHC_FAILURE;
                    /*call cpm with ACK event */   
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_ACK received"
                            "with SN[%u]\n",                                              
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfACKProcessed++;

                    p_csc->ack_recvd = ROHC_TRUE;
                    if(mode == p_csc->c_mode)
                    {   
                        rohc_ccpm_p2_exec_direct_evt(p_entity ,p_csc,
                                rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                                [acktype - ROHC_ACK],
                                p_ecode);
                    }
                }
                else
                {

                    ROHC_LOG(LOG_INFO, "CPRM:EID[%u:%u]:CID[%u]:"\
                            "Element is not found in GSW "
                            "for the received ACK with SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);
                }
            }   
            else
            {

                *p_ecode = ROHC_INVALID_ACK_RECEIVED;
                /* SPR 9591 fix start */
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_ACK received"
                        "with invalid SN [%u]\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                /* SPR 9591 fix end */
            }    
            break;
        case ROHC_NACK  :
            {
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_NACK received"
                        "with SN[%u]",                                              
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                if(rohc_ack_handling_p2(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /*rohc_cled_ack_handling(p_csc,sn);*/
                    /*call cpm with ACK event */  

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfNACKProcessed++;

                    rohc_ccpm_p2_exec_direct_evt(p_entity ,p_csc ,
                            rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                            [acktype - ROHC_ACK],  p_ecode);
                }
                else
                {

                    *p_ecode = ROHC_INVALID_NACK_RECEIVED;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_NACK received"
                            "with invalid SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);     
                }    

            }
            break;
        case ROHC_STATIC_NACK:
            {
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_STATIC_NACK received"
                        "with SN[%u]\n",                                              
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), sn);     
                if(rohc_ack_handling_p2(p_csc,sn,acktype))
                {
                    /* call list compression  */
                    /*rohc_cled_ack_handling(p_csc,sn);*/
                    /*call cpm with ACK event */  

                    /* Incrementing Stats */
                    pdcpStats.statsRohc.numberOfSNACKProcessed++;

                    /* call cpm with ACK event */                       
                    rohc_ccpm_p2_exec_direct_evt(p_entity ,p_csc ,
                            rohc_init_ans_matix[p_csc->c_mode - ROHC_MODE_U]
                            [acktype - ROHC_ACK],p_ecode);
                }
                else
                {

                    *p_ecode = ROHC_INVALID_STATIC_NACK_RECEIVED;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_ACK received"
                            "with invalid SN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), sn);     
                }    

            }break;

    }
    /* initiate mode transition */
    if(rohc_init_mt_matix[p_csc->c_mode -ROHC_MODE_U][mode-ROHC_MODE_U])
    {
        ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                "Initiating mode transition "
                "%c->%c\n",                         
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,\
                ROHC_GET_CSC_CID(p_csc),
                rohc_cprm_mode[p_csc->c_mode - ROHC_MODE_U],
                rohc_cprm_mode[mode - ROHC_MODE_U]
                );

        /* Incrementing Stats */
        /* SPR 14084 fix start */
        /* flag ROHC_MODE_TRIGGER has been removed */
        /* SPR 14084 fix end */
        rohcStatsModeTransUpdate(rohc_c_mt_stats_matrix
                [p_csc->c_mode -ROHC_MODE_U][mode-ROHC_MODE_U], 
                ROHC_DIR_TX);

        rohc_ccpm_p2_exec_direct_evt(p_entity, p_csc,
                rohc_init_mt_matix[p_csc->c_mode -ROHC_MODE_U]
                [mode-ROHC_MODE_U], p_ecode);

    }
    return ROHC_SUCCESS;
}

rohc_return_t rohc_cprm_p6_pr_fb  ROHC_PARAMS(         
            (p_entity, p_csc, p_buff, len, p_ecode),              
            rohc_entity_t       *p_entity   _AND_           
            rohc_csc_tcp_t      *p_csc      _AND_                 
            rohc_U8bit_t        *p_buff     _AND_              
            rohc_U8bit_t        len         _AND_
            rohc_error_code_t   *p_ecode                        
            )                                       
{                                                   
    rohc_U8bit_t temp = 0;                          
    rohc_U8bit_t acktype  =  0;                     
    rohc_U32bit_t temp_msn        = 0;                    
    rohc_U32bit_t dec_msn        = 0;                    
    rohc_U8bit_t *tmp_buff  = p_buff + 2;/* tmp_buff points to
                                            feedback data*/
    rohc_U8bit_t read_oct = 0;
    rohc_U8bit_t feedback_data_len = 0;
    rohc_U8bit_t sn_not_vaild  = ROHC_FALSE;
    ROHC_ENTER_FN("rohc_cprm_p6_pr_fb");
    /* SPR 9591 fix start */
    feedback_data_len = *p_buff & ROHC_FB_CODE_MASK;
    /* SPR 9591 fix end */
    /* FB-1 */
    if(feedback_data_len == 1)
    {
        temp_msn  = ROHC_GET_U8BIT(tmp_buff);

        /* :- call encode-decode to verify the validatity */
        /* decode the lsb encoded msn received in feedback*/
        if(ROHC_SUCCESS == rohc_p6_decode_msn_in_feedback(p_entity,p_csc,
                    temp_msn,&dec_msn,p_ecode))
        {

            if(rohc_ack_handling_p6(p_csc,dec_msn,ROHC_ACK))
            {
                /*:- call cpm with ACK event */
                rohc_ccpm_p6_exec_direct_evt(p_entity ,p_csc,
                        ROHC_C_ACK_ET,p_ecode);
            }
            else
            {
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_ACK received with MSN[%u]\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), temp_msn);
            }
        }
        else
        {

            ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                    "ROHC_ACK received with MSN[%u] error-code[%u]\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,\
                    ROHC_GET_CSC_CID(p_csc), temp_msn, *p_ecode);
        }   

    }
    else /* FB-2 */
    {
        read_oct += 2;
        temp  = ROHC_GET_U8BIT(tmp_buff);
        read_oct++;
        temp_msn = temp & ROHC_FB_MSN_MASK;
        acktype = ((temp & ROHC_FB_ACKT_MASK) >> 6);
        tmp_buff++;
        temp_msn = (temp_msn<<8)| ROHC_GET_U8BIT(tmp_buff);
        tmp_buff++;
        read_oct++;
        /* tmp_buff currently pointing to crc.
         * Incrementing to make it point to feedback data*/
        tmp_buff++;
        read_oct++;

        while(read_oct < len)
        {
            temp = ROHC_GET_U8BIT(tmp_buff++);
            read_oct++;
            switch(( (temp&ROHC_FB_OPTYP_MASK) >> 4) )
            {
                case ROHC_FBO_REJECT: 
                    p_entity->dont_create_csc_flag = ROHC_TRUE;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_FBO_REJECT received",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,
                            ROHC_GET_CSC_CID(p_csc));
                    break;      
                case ROHC_FBO_SN_NOT_VALID: 
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_FBO_SN_NOT_VALID received\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,
                            ROHC_GET_CSC_CID(p_csc));
                    sn_not_vaild = ROHC_TRUE;
                    break;
                case ROHC_FBO_SN :
                    temp = ROHC_GET_U8BIT(tmp_buff++);
                    temp = temp >> 6;
                    read_oct++;
                    temp_msn = (temp_msn << 2) | temp ;
                    break;      
                case ROHC_FBO_CONTEXT_MEM :
                    p_entity->dont_create_csc_flag = ROHC_TRUE;
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_FBO_REJECT received",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,
                            ROHC_GET_CSC_CID(p_csc));
                    break;  
                default:
                    /* unknown option, ignore */
                    read_oct += (temp & ROHC_FB_OPLEN_MASK);
                    tmp_buff += (temp & ROHC_FB_OPLEN_MASK);
            }
        }
        /*  call ED to verify the existence of MSN in GSW */
        switch(acktype)
        {                    
            case ROHC_ACK :
                if(!sn_not_vaild)
                {
                    if(ROHC_SUCCESS == rohc_p6_decode_msn_in_feedback
                            (p_entity,p_csc,temp_msn,&dec_msn,p_ecode))
                    {
                        if(rohc_ack_handling_p6(p_csc,temp_msn,acktype))
                        {
                            /*call cpm with ACK event */
                            /* SPR 9591 fix start */
                            ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                                    "ROHC_ACK received"
                                    "with MSN[%u]\n",                 
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,\
                                    ROHC_GET_CSC_CID(p_csc), temp_msn);
                            /* SPR 9591 fix end */

                            /* Incrementing Stats */
                            pdcpStats.statsRohc.numberOfACKProcessed++;
                            rohc_ccpm_p6_exec_direct_evt(p_entity ,p_csc,
                                    ROHC_C_ACK_ET,p_ecode);
                        }
                        else
                        {
                            ROHC_LOG(LOG_INFO, "CPRM:EID[%u:%u]:CID[%u]:"\
                                    "Element is not found in GSW "
                                    "for the received ACK with MSN[%u]\n",
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,\
                                    ROHC_GET_CSC_CID(p_csc), temp_msn);
                        }
                    }
                    else
                    {
                        ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                                "ROHC_ACK received with MSN[%u] error-code[%u]\n",
                                p_entity->rohc_entity_id.ueIndex,
                                p_entity->rohc_entity_id.lcId,\
                                ROHC_GET_CSC_CID(p_csc), temp_msn, *p_ecode);
                    }
                }

                else
                {
                    *p_ecode = ROHC_INVALID_ACK_RECEIVED;
                    /* SPR 9591 fix start */
                    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                            "ROHC_ACK received"
                            "with invalid MSN[%u]\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,\
                            ROHC_GET_CSC_CID(p_csc), temp_msn); 
                    /* SPR 9591 fix end */
                }    
                break;
            case ROHC_NACK:
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_NACK received"
                        "with MSN[%u]",                            
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), temp_msn);     
                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfNACKProcessed++;

                rohc_ccpm_p6_exec_direct_evt(p_entity ,p_csc ,
                        ROHC_C_NACK_ET, p_ecode);
                break;
            case ROHC_STATIC_NACK:
                ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"\
                        "ROHC_STATIC_NACK received"
                        "with MSN[%u]\n",                   
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,\
                        ROHC_GET_CSC_CID(p_csc), temp_msn);     
                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfSNACKProcessed++;

                /* call cpm with STATIC NACK event */               
                rohc_ccpm_p6_exec_direct_evt(p_entity ,p_csc ,
                        ROHC_C_STATIC_NACK_ET,p_ecode);
                break;
        }
    }
    ROHC_EXIT_FN("rohc_cprm_p6_pr_fb");
    return ROHC_SUCCESS;
}

