/*****************************************************************
*
* FILE NAME  :uecc_utils.c
*
* DESCRIPTION : This file have utility function for UECC module
*
*      DATE       NAME          REFERENCE       REASON
*    18 May 2009  Pradeep S                    initial
*
* Copyright 2009, Aricent Inc.
*
******************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_msg_mgmt.h"
#include "uecc_utils.h"
#include "uecc_common.h"
#include "rrc_common_utils.h"
#include "rrc_ext_api.h"
#include "uecc_ed.h"
#include "uecc_mainfsm.h"
#include "uecc_logger.h"
#include "hmac_sha256.h"
#include "rrc_cp_common.h"
#include <lteMisc.h>
/****************************************************************************
 * Private Types
 ****************************************************************************/

#pragma pack(push, 1)

/* For Key generation */
typedef struct
{
    U8 fc;
    U8 p0;
    U8 l0[2];
    U8 p1;
    U8 l1[2];
} rrc_key_s_t;
typedef struct
{
    U8 fc;
    U16 p0;
    U8 l0[2];
    U16 p1;
    U8 l1[2];
} rrc_key_enbstar_s_t;


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
typedef struct
{
    U8  fc;
    U16 p0;
    U8  l0[2];
} rrc_key_skgnb_t;
/*NR_DC Code Change Stop*/
#endif

#pragma pack(pop)

#define MESSAGE_BUFFER_SIZE                          1024
/* SPR 23243 Fix - Start */
#define RRC_KEY_S_SIZE          7
#define RRC_KEY_ENBSTAR_S_SIZE  9
/* SPR 23243 Fix - End */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* Used in logger for identifying CSC (just for avoid string duplication) */
const S8* rrc_uecc_log_facility_name = (const S8*)"eNodeB RRC UECC";

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_send_internal_msg
*
*   INPUT:         rrc_module_id_t     dst_module_id, 
                   U16                 api_id,         
                   U16                 msg_size,       
                   void                *p_msg          
*  
*   OUTPUT:        None
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
*   DESCRIPTION:   Send message only with RRC API Header from UECC.
*                  Is used for sending messages to other local RRC submodules
*   
******************************************************************************/
rrc_return_et uecc_rrc_send_internal_msg
(
    rrc_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API Id */
    U16                 msg_size,       /* Size of message (payload) */
    void                *p_msg          /* Message specific info (payload) */
)
{
    U8 *p_api = PNULL;
    U16 msg_api_length = (U16)(msg_size + RRC_API_HEADER_SIZE);

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL == p_api)
    {
        return RRC_FAILURE;
    }

    memset_wrapper(p_api, 0, msg_api_length);

    rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_UECC_MODULE_ID,
                                dst_module_id, api_id, msg_api_length);

    if((0 != msg_size) && (PNULL != p_msg))
    {
        l3_memcpy_wrapper(p_api + RRC_API_HEADER_SIZE, p_msg, msg_size);
    }

    /* Send message to */
    rrc_send_message(p_api, dst_module_id);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: uecc_alloc_intrl_msg
*
*   
*   INPUT:         rrc_module_id_t         dst_module_id
*                  U16                     api_id,
*                  U16                     msg_size
*
*   OUTPUT:        None
*
*   RETURNS:       PNULL in failure case
*                  pointer on the very begining of message
*
*   DESCRIPTION:   Allocate memory and fill RRC API Header for sending message from UECC.
*                  Is used for sending messages to other RRC submodules.
*
******************************************************************************/
void* uecc_alloc_intrl_msg
(
    rrc_module_id_t         dst_module_id,  /* Destination module identifier */
    U16                     api_id,         /* API Id */
    U32                     msg_size        /* Size of message (payload) */
)
{
    void *p_api = PNULL;
    U32 msg_api_length = (msg_size + RRC_API_HEADER_SIZE);

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL != p_api)
    {
        memset_wrapper(p_api, 0, msg_api_length);

        /* Fill RRC header */
        rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_UECC_MODULE_ID,
                dst_module_id, api_id, msg_api_length);
    }

    return p_api;
}

/******************************************************************************
*   FUNCTION NAME:  uecc_find_index_by_id
*
*
*   INPUT:         U16         id,
*                  uecc_id_t*  p_table,
*                  U16         element_size
*  
*   OUTPUT:        None
*
*   RETURNS:       Table row index or UECC_INTF_LAST_ID if row with such id
*                  isn't found.
*
*   DESCRIPTION:    Function finds the row in table which corresponds
*                   specified id.
*                   Each table element should correspond to the following
*                   conventions:
*                   - each table element is structure;
*                   - the first element field has type uecc_id_t.
******************************************************************************/
U16 uecc_find_index_by_id
(
    U16         id,
    uecc_id_t*  p_table,
    U16         element_size
)
{
    U16 result = UECC_LAST_ID;
    U16 index = 0;
    while ( UECC_LAST_ID != p_table->id )
    {
        if ( p_table->id == id )
        {
            result = index;
            break;
        }
        index++;

        p_table= (uecc_id_t*) ((U8*)p_table + element_size);
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrc_generate_key
 * Inputs         : U8 p0
 *                  U8 algorithm_id
 *                  U8 *key_dest
 *                  U8 *k_enb
 * Outputs        : U8 *key_dest
 * Returns        : None
 * Description    : Generates int/enc keys for RRC
 ****************************************************************************/
void rrc_generate_key(
    U8 p0,
    U8 algorithm_id,
    U8 *key_dest,
    U8 *k_enb
)
{
    rrc_key_s_t s;
    U8 mac_res[SHA_256_DIGEST_LEN];
    /* SPR 23243 Fix - Start */
    U8 key_s[RRC_KEY_S_SIZE];
    /* SPR 23243 Fix - End */

    /* See 3GPP TS 33.401: 3GPP System Architecture Evolution (SAE);
     * “Security architecture", Annex A (normative): Key derivation functions,
     * A.7 Algorithm key derivation functions (S15) */
    s.fc = 0x15;
    s.p0 = p0;
    s.l0[0] = 0;
    s.l0[1] = 1;
    s.p1 = (U8)(algorithm_id & 0xF);
    s.l1[0] = 0;
    s.l1[1] = 1;

    /* SPR 23243 Fix - Start */
    /* Fill the rrc_key_s_t structure values to U8 array to avoid compiler byte padding, if no pragma */
    key_s[0] = s.fc;
    key_s[1] = s.p0;
    key_s[2] = s.l0[0];
    key_s[3] = s.l0[1];
    key_s[4] = s.p1;
    key_s[5] = s.l1[0];
    key_s[6] = s.l1[1];

    /* Generate HMAC-256 */
    hmac_sha_256_algo(k_enb, 32, key_s, RRC_KEY_S_SIZE, mac_res, SHA_256_DIGEST_LEN);
    /* SPR 23243 Fix - End */

    /* Copy least significant 16 bytes */
    l3_memcpy_wrapper(key_dest,mac_res+(SHA_256_DIGEST_LEN-RRC_PDCP_SECURITY_KEY_SIZE),
        RRC_PDCP_SECURITY_KEY_SIZE);

}

/****************************************************************************
 * Function Name  : rrc_generate_kenb_star
 * Inputs         : U8 p0 - PCI (target physical cell id)
 *                  U8 dl_earfcn
 *                  U8 *key_dest
 *                  U8 *k_enb
 * Outputs        : U8 *key_dest
 * Returns        : None
 * Description    : Generates int/enc keys for RRC
 ****************************************************************************/
void rrc_generate_kenb_star(
    U16 p0,
    U16 dl_earfcn,
    U8 *key_dest,
    U8 *k_enb
)
{
    rrc_key_enbstar_s_t s;
    U8 mac_res[SHA_256_DIGEST_LEN];
    /* SPR 23243 Fix - Start */
    U8 key_enbstar_s[RRC_KEY_ENBSTAR_S_SIZE];
    /* SPR 23243 Fix - End */

    /* See 3GPP TS 33.401: 3GPP System Architecture Evolution (SAE);
     * “Security architecture", Annex A (normative): Key derivation functions,
     * A.7 Algorithm key derivation functions (S13) */
     /*-    FC = 0x13
     -  P0 = PCI (target physical cell id)
     -  L0 = length of PCI (i.e. 0x00 0x02)
     -  P1 = EARFCN-DL (target physical cell downlink frequency)
     -  L1 length of EARFCN-DL (i.e. 0x00 0x02)*/

    s.fc = 0x13;
    rrc_pack_U16(&s.p0, &p0);
    s.l0[0] = 0x00;
    s.l0[1] = 0x02;
    rrc_pack_U16(&s.p1, &dl_earfcn);
    s.l1[0] = 0x00;
    s.l1[1] = 0x02;

    /* SPR 23243 Fix - Start */
    /* Fill key_enbstar_s structure values to U8 array to avoid compiler byte padding, if no pragma */
    key_enbstar_s[0] = s.fc;
    key_enbstar_s[1] = (U8)(s.p0);
    key_enbstar_s[2] = (U8)(s.p0 >> 8);
    key_enbstar_s[3] = s.l0[0];
    key_enbstar_s[4] = s.l0[1];
    key_enbstar_s[5] = (U8)(s.p1);
    key_enbstar_s[6] = (U8)(s.p1 >> 8);
    key_enbstar_s[7] = s.l1[0];
    key_enbstar_s[8] = s.l1[1];

    /* Generate HMAC-256 */
    hmac_sha_256_algo(k_enb, 32, key_enbstar_s, RRC_KEY_ENBSTAR_S_SIZE, mac_res, SHA_256_DIGEST_LEN);
    /* SPR 23243 Fix - End */

    /* Copy least significant 16 bytes */
    l3_memcpy_wrapper(key_dest,&mac_res[0],SHA_256_DIGEST_LEN);

}


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
/****************************************************************************
 * Function Name  : rrc_generate_skgnb
 * Inputs         : U8 P0 - Value of the SCG Counter as a non-negative integer 
 *                  U8 *key_dest
 *                  U8 *k_enb
 * Outputs        : U8 *key_dest
 * Returns        : None
 * Description    : Generates Security key for SgNB (S-KgNB/S-KeNB) 
 ****************************************************************************/
void rrc_generate_skgnb(
    U16 p0,
    U8 *key_dest,
    U8 *k_enb
)
{
    rrc_key_skgnb_t s;
    U8 mac_res[SHA_256_DIGEST_LEN];

    /* See 3GPP TS 33.401: 3GPP System Architecture Evolution (SAE);
     * “Security architecture", Annex A (normative): Key derivation functions,
     * A.15 Derivation of S-KeNB or S-KgNB for dual connectivity*/ 
    /* -   FC = 0x1C
     * -   P0 = Value of the SCG Counter as a non-negative integer 
     * -   L0 = length of the SCG Counter value (i.e. 0x00 0x02)*/ 

    s.fc = 0x1C;
    rrc_pack_U16(&s.p0, &p0);
    s.l0[0] = 0x00;
    s.l0[1] = 0x02;

    /* Generate HMAC-256 */
    hmac_sha_256_algo(k_enb,32,(U8*)(&s),sizeof(s),mac_res,SHA_256_DIGEST_LEN);

    /* Copy least significant 16 bytes */
    l3_memcpy_wrapper(key_dest,&mac_res[0],SHA_256_DIGEST_LEN);

}

/****************************************************************************
 * Function Name  : rrc_uecc_count_scg_bearer
 * Inputs         : U8
 * Outputs        : 
 * Returns        : Count of SN_TERMINATED_SCG_BEARER/SN_TERMINATED_SPLIT_BEARER 
 * Description    : Checks iand returns the count scg_bearers already present in
 *                  ue context
 ****************************************************************************/
U8
rrc_uecc_count_scg_bearer
(
    uecc_ue_context_t   *p_ue_context
)
{
    U8 scg_bearer_count = RRC_NULL;
    U8 erab_index       = RRC_NULL;

    /*from ue context, check if any bearer type is SN_TERMINATED_SCG_BEARER
     * or SN_TERMINATED_SPLIT_BEARER if yes, mark result as Success and break*/
    for(erab_index = RRC_NULL ; erab_index < MAX_ERAB_COUNT; erab_index++)
    {
        /*check if p_e_rab_list is not null*/
        if(PNULL != p_ue_context->p_e_rab_list[erab_index])
        {
            if((p_ue_context->p_e_rab_list[erab_index]->m.DC_BearerTypePresent) &&
                    (( SN_TERMINATED_SCG_BEARER ==
                       p_ue_context->p_e_rab_list[erab_index]->dc_bearer_type) ||
                     /*OPTION3X Changes Start*/
                     ( SN_TERMINATED_SPLIT_BEARER ==
                       p_ue_context->p_e_rab_list[erab_index]->dc_bearer_type))
                     /*OPTION3X Changes Stop*/
                    )
            {
                /*increment the scg_bearer_count*/
                scg_bearer_count++;

                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "ERAB ID[%d] is SCG Bearer!",
                        p_ue_context->p_e_rab_list[erab_index]->e_RAB_ID);
            }
        }
    }
    return scg_bearer_count;
}

/*NR_DC Code Change Stop*/
#endif

/****************************************************************************
 * Function Name  : rrc_pack_U16
 * Inputs         : U8 *p_dest
 *                  U8 *p_src
 * Outputs        : U8 *p_src
 * Returns        : None
 * Description    : Packs U16 into big-endian format
 ****************************************************************************/
void
rrc_pack_U16
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Packing U16 */
    ((U8*)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(U16 *)p_src));

}

/****************************************************************************
 * Function Name  : rrc_unpack_U16
 * Inputs         : U8 *p_dest
 *                  U8 *p_src
 * Outputs        : None
 * Returns        : None
 * Description    : unPacks U16 from big-endian format
 ****************************************************************************/
void
rrc_unpack_U16
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Unpacking U16 */

    *(U16 *)p_dest  = 0;

    *(U16 *)p_dest  = (U16)(*(U16 *)p_dest | ((U8*)p_src)[0] << 8);
    *(U16 *)p_dest  = (U16)(*(U16 *)p_dest | ((U8*)p_src)[1]);

}

/****************************************************************************
 * Function Name  : rrc_pack_U32
 * Inputs         : U8 *p_dest
 *                  U8 *p_src
 * Outputs        : None
 * Returns        : None
 * Description    : Packs U32 into big-endian format
 ****************************************************************************/
void
rrc_pack_U32
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Packing U32 */

    ((U8*)p_dest)[0]    = (U8)((*(U32 *)p_src) >> 24);
    ((U8*)p_dest)[1]    = (U8)((*(U32 *)p_src) >> 16);
    ((U8*)p_dest)[2]    = (U8)((*(U32 *)p_src) >> 8);
    ((U8*)p_dest)[3]    = (U8)((*(U32 *)p_src));

}

/****************************************************************************
 * Function Name  : rrc_unpack_U32
 * Inputs         : U8 *p_dest
 *                  U8 *p_src
 * Outputs        : None
 * Returns        : None
 * Description    : unpacks Packs U32 from big-endian format
 ****************************************************************************/
void
rrc_unpack_U32
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Unpacking U32 */

    *(U32 *)p_dest  = 0;

    *(U32 *)p_dest  |= (U32)((U8*)p_src)[0] << 24;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[1] << 16;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[2] << 8;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[3];

}
/******************************************************************************
*   FUNCTION NAME: rrc_uecc_check_is_cell_valid_and_active
*
*   INPUT:         uecc_gb_context_t*   p_uecc_gb_context,  
                   rrc_cell_index_t     cell_index 
 
*   OUTPUT:        None
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
*   DESCRIPTION:   Checks whether cell contexts exist
*
******************************************************************************/

rrc_return_t rrc_uecc_check_is_cell_valid_and_active
(
    uecc_gb_context_t*   p_uecc_gb_context,  /* UECC global context */
    rrc_cell_index_t     cell_index
)
{
    rrc_return_t         result = RRC_FAILURE;
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    
    if((PNULL != p_uecc_gb_context->p_p_csc_context)&&(PNULL != 
               p_uecc_gb_context->p_p_csc_context[cell_index]))
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context[cell_index]->p_csc_init_setup_ind)
        {
            if (RRC_CSC_CELL_ACTIVE == p_uecc_gb_context->
                    p_p_csc_context[cell_index]->p_csc_init_setup_ind->cell_state )
            {
                result = RRC_SUCCESS;
            }
        }
    }
    
    return result;
}

/******************************************************************************
*   FUNCTION NAME: rrc_uecc_get_cell_index_from_rrm_api
*
*   INPUT:         uecc_ue_context_t   *p_ue_context,
*                  void                *p_api,
*                  rrc_cell_index_t    *p_cell_index
*  
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Populates Cell Index In RRM messages
*
******************************************************************************/

void rrc_uecc_get_cell_index_from_rrm_api
(
 uecc_ue_context_t   *p_ue_context,
 void                *p_api,
 rrc_cell_index_t    *p_cell_index
 )
{
    U8       length = 0;

    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_index);

/* cell index changes start */
    switch (rrc_get_api_id(p_api))
    {
        case RRC_RRM_UE_RELEASE_RESP:
        case RRC_RRM_ERB_RELEASE_IND:
        case RRC_RRM_HO_CANCEL_REQ:
        case RRC_RRM_UE_HO_CMD_RESP:
        case RRC_RRM_MEAS_CONFIG_REQ:
        case RRC_RRM_UE_ADMISSION_RESP:
        case RRC_RRM_ERB_SETUP_RESP:
        case RRC_RRM_ERB_MODIFY_RESP:
        case RRC_RRM_UE_CONNECTION_RELEASE_IND:
        case RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ:
        case RRC_RRM_ERB_RELEASE_RESP:
        case RRC_RRM_HO_REQUIRED:
        case RRC_RRM_UE_HO_ADM_RESP:
        case RRC_RRM_UE_CONTEXT_MOD_RESP:
        case RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000:
        case RRC_RRM_COUNTER_CHECK_REQ:
        case RRC_RRM_UE_INFORMATION_REQ:
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
        case RRC_RRM_DC_BEARER_CHANGE_REQ:
#endif
/* OPTION3X Changes End */
            length = RRC_API_HEADER_SIZE + RRC_API_INTERFACE_HEADER_CELL_INDEX;
            rrc_cp_unpack_U8(p_cell_index, (U8 *)p_api + length, "cell_index");
            break;
        default:
            RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "Invalid API Id [%d] received from RRM", rrc_get_api_id(p_api));
            break;
    }
    if( *p_cell_index > MAX_CELL_INDEX)
    {
        *p_cell_index = RRC_INVALID_CELL_INDEX; 
    }
/* cell index changes stop */

    RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);

}
/* Bug 12190 Fix Start */
/******************************************************************************
*   FUNCTION NAME: setNBitVal
*
*   INPUT:         U8**                p_buf
*                  U32*                bitPos
*                  U32                 src_val
*                  U8                  width
*                  p_ue_context        uecc_ue_context_t
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   To pack a list of parameters in an octet string
*
******************************************************************************/
void setNBitVal(
    U8***                p_buf,          /*Buffer to be filled with mobility
                                          parameters*/
    U8**                bitPosition,         /*Position of bit in the current
                                          octet from where the new
                                          parameter would start filling*/
    U32                 src_val,       /*Mobility parameter value to be
                                          filled in octet string*/
    U8                  width,          /*Size in bits of current mobility
                                          parameter*/
    uecc_ue_context_t   *p_ue_context    /* Ue_Context*/
)
{
    U32                 temp;
    U32                 numocts = 0;
    U32                 remainder = 0;
    U8                  index_num= 0;
    U8                  src[4] = {0};
    U8                  *bitPos = PNULL;
 /*SPR 17891 Start*/
    if(PNULL != p_ue_context)
 /*SPR 17891 End*/
    { 
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->p_gb_context->facility_name,
            RRC_DETAILEDALL, "Unused variable p_ue_context=%p",
                p_ue_context);/*SPR 17777 +-*/
    }

    bitPos = (*bitPosition);
    src_val = (src_val << (32- width - *bitPos));

    l3_memcpy_wrapper(src,&src_val,sizeof(src_val));
    
    temp = ((width + *bitPos) / 8);
    remainder = ((width + *bitPos)% 8);
    numocts = ((remainder == 0) ? temp : temp + 1);
    while(numocts)
    {
        /* cyclomatic_FIX start */
        if(index_num < 4)
        {
            ***p_buf = (***p_buf | src[3 - index_num]);
        }
        /* cyclomatic_FIX stop */
        if(numocts != 1)
        {
           (**p_buf)++;
    }
       numocts--;
       index_num++;
                    }
    if ( 0 == remainder)
                    {
       (**p_buf)++;
            }
    *bitPos= remainder;
}
/* Bug 12190 Fix Stop */

/******************************************************************************
*   FUNCTION NAME: rrc_get_rb_direction
*
*   INPUT:         uecc_ue_context_t*                p_uecc_ue_context,
*                  U8                                erab_index,
*                  rrc_rb_direction_t                *rb_direction
*
*  
*   OUTPUT:        None
*
*   RETURNS:       RRC_FAILURE/RRC_SUCCESS
*
*   DESCRIPTION:   This function find rb_direction for a erab
*
******************************************************************************/
rrc_return_et rrc_get_rb_direction(
    uecc_ue_context_t*                p_uecc_ue_context,
    U8                                erab_index,
    rrc_rb_direction_t                *rb_direction
)
{
    RRC_UECC_UT_TRACE_ENTER (p_uecc_ue_context->p_gb_context);

    RRC_ASSERT (p_uecc_ue_context != PNULL);
    RRC_ASSERT (rb_direction != PNULL);

    if (p_uecc_ue_context->p_e_rab_list[erab_index] == PNULL)
    {  
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_WARNING,
                "Counter Check received for a UE but p_e_rab_list is NULL\n");

        return RRC_FAILURE;
    }

    if ((RRC_RLC_CONFIG_AM_CONFIG_PRESENT &
                p_uecc_ue_context->p_e_rab_list[erab_index]->drb_config.rlc_config_enb.rlc_config.bitmask) ||
            (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT &
             p_uecc_ue_context->p_e_rab_list[erab_index]->drb_config.rlc_config_enb.rlc_config.bitmask)
       )
    {
        *rb_direction = RRC_PDCP_RB_DIR_BOTH;
    }
    else if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT &
            p_uecc_ue_context->p_e_rab_list[erab_index]->drb_config.rlc_config_enb.rlc_config.bitmask)
    {
        *rb_direction = RRC_PDCP_RB_DIR_TX;
    }
    else if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT &
            p_uecc_ue_context->p_e_rab_list[erab_index]->drb_config.rlc_config_enb.rlc_config.bitmask)
    {
        *rb_direction = RRC_PDCP_RB_DIR_RX;
    }
    else
    {
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_WARNING,
                "Bitmask value unknown for p_e_rab_list[erab_index]->drb_config.rlc_config_enb.rlc_config\n");

        return  RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT (p_uecc_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_send_inter_cell_internal_msg
*
*
*   INPUT:         rrc_module_id_t     dst_module_id
*                  U16                 api_id
*                  U16                 msg_size
*                  void                *p_msg
*  
*   OUTPUT:        None
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
*   DESCRIPTION:   Send message only with RRC API Header from UECC with X2AP as source
*                  module id in case of INTER CELL HO.
*                  Is used for sending messages to other local RRC submodules
******************************************************************************/
rrc_return_et uecc_rrc_send_inter_cell_internal_msg
(
    rrc_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API Id */
    U16                 msg_size,       /* Size of message (payload) */
    void                *p_msg          /* Message specific info (payload) */
)
{
    U8 *p_api = PNULL;
    U16 msg_api_length = (U16)(msg_size + RRC_API_HEADER_SIZE);

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL == p_api)
    {
        return RRC_FAILURE;
    }

    memset_wrapper(p_api, 0, msg_api_length);

    rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_X2AP_MODULE_ID,
                                dst_module_id, api_id, msg_api_length);

    l3_memcpy_wrapper(p_api + RRC_API_HEADER_SIZE, p_msg, msg_size);

    /* Send message to */
    rrc_send_message(p_api, dst_module_id);

    return RRC_SUCCESS;
}
/*SPR 15896 Fix Start*/
void trace_logging(uecc_ue_context_t *p_ue_context, U8 log_depth, U8 interface, const char* format,...)
{

    va_list argList;
    char userMessageBuffer[MESSAGE_BUFFER_SIZE];
    size_t messageLength;
    FILE *fp_cell = PNULL; 
    FILE *fp_subscriber = PNULL; 

    memset_wrapper(&argList, 0, sizeof(va_list));

    VA_START(argList, format);
    vsnprintf_wrapper((S8 *)userMessageBuffer, (size_t)MESSAGE_BUFFER_SIZE, (const S8 *)format, argList);
    VA_END(argList);

    if(!p_ue_context)
	return;

    messageLength = strlen_wrapper((const Char8 *)userMessageBuffer);
    if ( (messageLength >= MESSAGE_BUFFER_SIZE) || 
            (messageLength <= RRC_NULL))
    {
        return;
    }
    if ( messageLength < 2 )
    {
        if(userMessageBuffer[messageLength - 1] == '\n')
            userMessageBuffer[messageLength - 1] = '\0';
    }
    else
    {
        if(userMessageBuffer[messageLength - 1] == '\n' && userMessageBuffer[messageLength - 2] != '\n')
            userMessageBuffer[messageLength - 1] = '\0';
    }

    if(( p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
	(PNULL != p_ue_context->p_ue_trace_activation_info))
    {
       fp_subscriber = p_ue_context->p_ue_trace_activation_info->fp;
    }
    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
	(PNULL != p_ue_context->p_ue_cell_trace_activation_info))
    {
       fp_cell = p_ue_context->p_ue_cell_trace_activation_info->fp;
    }  

    if(PNULL != fp_subscriber && interface == GEN_MME )
    {
      fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_subscriber);
      return ;
    }

    if(PNULL != fp_cell && interface == GEN_CELL)
    {
      fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_cell);
      return ;
    }

    /* for cell tracing */
    if((PNULL != fp_cell) &&
		    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) && 
		    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & interface))
    {
	    if (((MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				    (MINIMUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)) &&
			    (log_depth & MINIMUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_cell);
	    }
	    if (((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				    (MEDIUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)) &&
			    (log_depth & MEDIUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_cell);
	    }
	    if (((MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				    (MAXIMUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)) &&
			    (log_depth & MAXIMUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_cell);
	    }
    }

    /* for subscriber tracing */
    if((PNULL != fp_subscriber) &&
		    (p_ue_context->p_ue_trace_activation_info != PNULL) && 
		    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & interface))
    {
	    if (((MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				    (MINIMUM == p_ue_context->p_ue_trace_activation_info->trace_depth)) &&
			    (log_depth & MINIMUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_subscriber);
	    }
	    if (((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				    (MEDIUM == p_ue_context->p_ue_trace_activation_info->trace_depth)) &&
			    (log_depth & MEDIUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_subscriber);
	    }
	    if (((MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				    (MAXIMUM == p_ue_context->p_ue_trace_activation_info->trace_depth)) &&
			    (log_depth & MAXIMUM_TRACE))
	    {
		    fwrite_wrapper(userMessageBuffer,messageLength ,1,fp_subscriber);
	    }
    }
}

/******************************************************************************
*   FUNCTION NAME: set_medium_bitmask 
*
*   INPUT:  
*   OUTPUT:        None
*
*   RETURNS:       MEDIUM_TRACE
*
*   DESCRIPTION:   Used to return MEDIUM_TRACE to set depth bitmask
******************************************************************************/
U8 set_medium_bitmask()
{
	return MEDIUM_TRACE;
}

/******************************************************************************
*   FUNCTION NAME: set_mininum_bitmask 
*
*   INPUT:  
*   OUTPUT:        None
*
*   RETURNS:       (U8)MINIMUM_TRACE
*
*   DESCRIPTION:   Used to return MINIMUM_TRACE to set depth bitmask
******************************************************************************/
U8 set_minimum_bitmask()
{
	return MINIMUM_TRACE;
}

/******************************************************************************
*   FUNCTION NAME: set_maximum_bitmask 
*
*   INPUT:  
*   OUTPUT:        None
*
*   RETURNS:       (U8)MAXIMUM_TRACE
*
*   DESCRIPTION:   Used to return MAXIMUM_TRACE to set depth bitmask
******************************************************************************/
U8 set_maximum_bitmask()
{
	return MAXIMUM_TRACE;
}

/******************************************************************************
*   FUNCTION NAME: set_mininum_medium_both_bitmask 
*
*   INPUT:  
*   OUTPUT:        None
*
*   RETURNS:       MINIMUM_TRACE || MEDIUM_TRACE
*
*   DESCRIPTION:   Used to return both MINIMUM_TRACE and MEDIUM_TRACE to set depth bitmask
******************************************************************************/
U8 set_minimum_medium_both_bitmask()
{
 	return (set_minimum_bitmask()|set_medium_bitmask());
}

/******************************************************************************
*   FUNCTION NAME: set_all_trace_bitmask 
*
*   INPUT:  
*   OUTPUT:        None
*
*   RETURNS:       
*
*   DESCRIPTION:   Used to return all trace levels combined to set depth bitmask
******************************************************************************/
U8 set_all_trace_bitmask()
{
 	return (set_minimum_bitmask()|set_medium_bitmask()|set_maximum_bitmask());
}
/*SPR 15896 Fix Stop*/

char* change_data_to_str(U16 len, U8*data)
{
    U16 cnt,curlen=0;
	static U8  buff[4000];
    if (len*2+1 > 4000)
    {
        strcpy(buff, "buffer too long");
        return buff;
    }
    
    sprintf(buff, "(len=%u) \0",len);
    curlen+=strlen(buff);

    for(cnt = 0; cnt < len; cnt++)
    {
        sprintf(buff+curlen, "%02X",data[cnt]);
        curlen+=2;
    }
    buff[curlen] = 0;
    return buff;
}

