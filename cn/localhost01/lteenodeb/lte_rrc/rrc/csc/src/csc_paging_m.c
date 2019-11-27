/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ***************************************************************************
 * File Details
 * ------------
 *  $Id: csc_paging_m.c,v 1.9 2010/10/13 07:02:38 gur18569 Exp $
 ***************************************************************************
 *
 *  File Description : RRC Paging procedure implementation with
 *                     multi UE paging supporting.
 *                     Paging records storage implementation.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * Jan 8, 2010   grag -   initial creation
 * $Log: csc_paging_m.c,v $
 * Revision 1.9  2010/10/13 07:02:38  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.3  2010/10/08 12:36:39  gur21006
 * TDD changes merged
 *
 * Revision 1.6.4.2  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.6.4.1  2010/09/13 13:03:39  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.6  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.5  2010/04/20 05:43:18  gur21006
 * paging for si mod period added
 *
 * Revision 1.4  2010/04/07 08:17:39  gur21006
 * Klocwork warning Removed
 *
 * Revision 1.3  2010/03/11 14:54:00  ukr16032
 * CSC Paging fixed
 *
 * Revision 1.2  2010/03/11 09:25:40  ukr16032
 * CSC - Multi UE Paging functionality updated according to new requirements.
 * -- CSC_SFN_OFFSET added
 * -- UE Paging records are rearranged now on receiving new SFN indication or due to periodical SFN synchronization.
 *
 * Revision 1.1  2010/01/13 16:38:56  ukr16032
 * CSC Multi UE Paging feature has been implemented.
 *
 *
 ***************************************************************************/

/****************************************************************************
 * Inculdes

 ****************************************************************************/
#include "rrc_intrl_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_csc_llim_intf.h"
#include "csc_logger.h"
#include "csc_paging_m.h"
#include "csc_utils.h"
#include "rtsrc/rtCopy.h"
#include "csc_cell_m_fsm.h"
#include "rrc_shared_stats.h"


/****************************************************************************
 Local Definitions
****************************************************************************/

#define NUM_OCTET_IN_S1AP_PLMN_ID       3
#define FILLER_DIGIT_IN_S1AP            0xF

#define PAGING_SFN_GAP_FOR_BIG_SF       1
#define PAGING_SFN_GAP_FOR_SMALL_SF     2
#define PAGING_BIG_SMALL_SF_BOUNDARY    5

#define NUM_PAGING_I_S_VALUES           4
#define NUM_PAGING_NS_VALUES            4
#define DUMMY_SUBFRAME_VALUE            10

#define PAGING_Q_INIT(p_paging_queue)\
       ylInit( p_paging_queue )

#define PAGING_Q_GET_COUNT(p_paging_queue)\
       ylCount( p_paging_queue )

#define PAGING_Q_ENQUEUE(p_paging_queue, p_paging_record)\
       ylPushTail( p_paging_queue, &(p_paging_record)->sNode )

#define PAGING_Q_DEQUEUE(p_paging_queue, p_paging_record){\
    YLNODE *snode = ylPopHead( (p_paging_queue) );\
    p_paging_record = YMEMBEROF( csc_paging_record_t, sNode, snode );\
}
#define MAX_NUM_PAGING_RECORDS_FOR_PAGING_OCCASION  16
/****************************************************************************
 * Types definitions
 ****************************************************************************/
typedef YLIST csc_paging_queue_t;

typedef struct
{
    YLNODE      sNode;
    rrc_bitmask_t  bitmask;
#define PAGING_RECORD_PAGING_PRIORITY_PRESENT   0x01
    rrc_sfn_t   sfn;
    rrc_sf_t    sf;

    /* AK */
    s1ap_CNDomain cn_domain;
    s1ap_UEPagingID  ue_paging_id;
    s1ap_PagingDRX   paging_drx;
    s1ap_PagingPriority   paging_priority;
    /* O, PAGING_RECORD_PAGING_PRIORITY_PRESENT */
} csc_paging_record_t;

typedef struct
{
    U16         cur_queues_count;
    U16         cur_paging_records_count;
    csc_paging_queue_t*  sfn_sf_queues[SFN_UPPER_LIMIT][SF_MAX_LIMIT];
} csc_paging_storage_t;

/****************************************************************************
 Local Variables
****************************************************************************/
static const U8
    paging_subframe_patterns_fdd[NUM_PAGING_NS_VALUES][NUM_PAGING_I_S_VALUES] =
{
    {   9,                      DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    {   4,                      9,                      DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    /* Ns coudn't be 3 */
    {   DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    {   0,                      4,                      5,
        9                       }
};

/***** TDD changes start *****/
static const U8
paging_subframe_patterns_tdd[NUM_PAGING_NS_VALUES][NUM_PAGING_I_S_VALUES] =
{
    {   0,                      DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    {   0,                      5,                      DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    /* Ns coudn't be 3 */
    {   DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,   DUMMY_SUBFRAME_VALUE,
        DUMMY_SUBFRAME_VALUE    },
    {   0,                      1,                      5,
        6                       }
};
/***** TDD changes stop *****/



/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static csc_paging_record_t* csc_paging_storage_insert
(
        csc_gl_ctx_t*      p_csc_gl_ctx,
        void*              p_storage,
        rrc_sfn_t          sfn,
        rrc_sf_t           sf,
        rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging
);

static rrc_return_t csc_paging_storage_reinsert
(
        void*                   p_storage,
        /* Bug 11748 Fix Start */
        csc_paging_record_t     *p_paging_record,
        U8                      cell_index
        /* Bug 11748 Fix Stop */
);

static csc_paging_record_t* csc_paging_storage_get
(
        void* p_storage,
        rrc_sfn_t   sfn,
        rrc_sf_t    sf
);

/*SPR 21554 +-*/

static rrc_bool_et csc_paging_is_ue_suitable_for_paging
(
    /* Pointer to S1AP ASN.1 based internal representation of paging message */
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,
    /* Pointer to cell access related info from SIB1 */
    cell_access_related_info_t  *p_curr_cell_info
);

static rrc_return_t csc_paging_determine_paging_sfn_sf
(
    /* Pointer to S1AP ASN.1 based internal representation of paging message */
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,
    /* Pointer to pcch config from SIB2 */
    pcch_config_t               *p_pcch_config,
    /* Current SFN in RRC cell */
    rrc_sfn_t                   rrc_curr_sfn,
    /* Target SFN of Paging message */
    rrc_sfn_t                   *p_paging_frame,
    /* Target SF of Paging message */
    rrc_sf_t                    *p_paging_sub_frame,
    /***** TDD changes start *****/
    /* Duplexing mode */
    duplexing_mode_et            duplexing_mode
    /***** TDD changes stop *****/
    );



static rrc_return_t csc_paging_build_and_send_llim_pcch_msg(
        OSCTXT           *p_asn1_ctx,
        PCCH_Message     *p_pcch_message,
        rrc_sfn_t        sfn,
        rrc_sf_t         sf,
        csc_cell_ctx_t   *p_cell_ctx
);

static rrc_return_t csc_paging_add_paging_record_to_rrc_paging_message
(
    OSCTXT                      *p_asn1_ctx, /* ASN.1 context */
    PCCH_Message                *p_pcch_message,
    csc_paging_record_t         *p_csc_paging_record
);

/* Not static fro UT */
rrc_return_t csc_paging_prepare_rrc_paging_message
(
    OSCTXT                      *p_asn1_ctx, /* ASN.1 context */
    PCCH_Message                *p_pcch_message,
    csc_cell_ctx_t              *p_cell_ctx 
);

rrc_return_t csc_paging_build_and_send_si_paging_msg
(
    rrc_sfn_t    sfn
);
static void csc_paging_print_paging_record(
        csc_paging_record_t         *p_csc_paging_record
);

static rrc_sfn_t csc_paging_rearrange_sfn(
        rrc_sfn_t     sfn_index,
        rrc_sfn_t     sfn_end,
        s1ap_PagingDRX     ue_drx
);

void csc_paging_build_and_send_broadcast_info
(
        rrc_sfn_t       sfn,
        U8              paging_type,
        csc_cell_ctx_t  *p_cell_ctx
);
rrc_return_t csc_delete_low_priority_paging_record
( 
        csc_paging_queue_t    *p_paging_queue,
        csc_paging_record_t   *p_paging_record 
);

/******************************************************************************
 * Function implementation
 *****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_init
*   INPUT        : void
*   OUTPUT       : None
*   RETURNS      : pointer on paging storage
*   DESCRIPTION  : Allocates and initializes CSC Paging storage.
******************************************************************************/
void* csc_paging_storage_init(void)
{
    csc_paging_storage_t *p_paging_storage = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    /* Initialize storage */

    p_paging_storage =
        (csc_paging_storage_t*)rrc_mem_get(sizeof(csc_paging_storage_t));

    if (PNULL != p_paging_storage)
    {
        memset_wrapper(p_paging_storage, 0, sizeof(csc_paging_storage_t));
        RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] The Paging storage initialized.");
    }

    RRC_CSC_UT_TRACE_EXIT();

    return p_paging_storage;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_free
*   INPUT        : void* p_storage
*                  rrc_bool_et is_cell_stopped
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Clean up CSC Paging storage and frees all allocated resources.
******************************************************************************/
void csc_paging_storage_free(void* p_storage, rrc_bool_et is_cell_stopped)
{
    csc_paging_storage_t *p_paging_storage = PNULL;
    rrc_sfn_t     sfn_index;
    rrc_sf_t     sf_index;
    csc_paging_record_t *p_paging_record = PNULL;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_storage);

    p_paging_storage = (csc_paging_storage_t*)p_storage;
    if (PNULL != p_paging_storage)
    {
        /* Clean up storage */
        for (sfn_index = 0; sfn_index < SFN_UPPER_LIMIT; sfn_index++)
        {
            for (sf_index = 0; sf_index < SF_MAX_LIMIT; sf_index++)
            {
                while ( PNULL !=
                        (p_paging_record =
                         csc_paging_storage_get(
                             p_paging_storage,
                             sfn_index, sf_index)))
                {
                    /* free paging record */
                    if (PNULL != p_paging_record->ue_paging_id.u.s_TMSI)
                    {
                      rrc_mem_free(p_paging_record->ue_paging_id.u.s_TMSI);
                      p_paging_record->ue_paging_id.u.s_TMSI = PNULL;
                    }
                    if (PNULL != p_paging_record->ue_paging_id.u.iMSI)
                    {
                      rrc_mem_free(p_paging_record->ue_paging_id.u.iMSI);
                      p_paging_record->ue_paging_id.u.iMSI = PNULL;
                    }
                    if (PNULL != p_paging_record->ue_paging_id.u.extElem1)
                    {
                      if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                      {
                        rrc_mem_free((void *)p_paging_record->ue_paging_id.u.extElem1->data);
                        p_paging_record->ue_paging_id.u.extElem1->data = PNULL;
                      }
                      rrc_mem_free(p_paging_record->ue_paging_id.u.extElem1);
                      p_paging_record->ue_paging_id.u.extElem1 = PNULL;
                    }
                    rrc_mem_free(p_paging_record);
                    p_paging_record = PNULL;
                }
            }
        }

        if (RRC_FALSE == is_cell_stopped)
        {
            rrc_mem_free(p_paging_storage);
        }
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "[PAGING] The Paging storage is NULL.");
    }

    RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] The Paging storage deinitialized.");

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_insert
*   INPUT        : csc_gl_ctx_t    *p_csc_gl_ctx
*                  void*            p_storage,
*                  rrc_sfn_t        sfn,
*                  rrc_sf_t         sf,
*                  rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging
*   OUTPUT       : None
*   RETURNS      : csc_paging_record_t* or PNULL in case of failure
*   DESCRIPTION  : Adds Paging record to the CSC Paging storage.
******************************************************************************/
csc_paging_record_t* csc_paging_storage_insert
(
        csc_gl_ctx_t    *p_csc_gl_ctx,
        void*            p_storage,
        rrc_sfn_t        sfn,
        rrc_sf_t         sf,
        rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging
)
{
    rrc_return_t    result = RRC_FAILURE;
    csc_paging_record_t     *p_paging_record = PNULL;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_storage);
    RRC_ASSERT(PNULL != p_s1ap_intrl_paging);

    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_csc_gl_ctx=%p ", p_csc_gl_ctx);
    
    do {
        p_paging_record =
            (csc_paging_record_t*)rrc_mem_get(sizeof(csc_paging_record_t));

        if (PNULL == p_paging_record)
        {
            break;
        }

        p_paging_record->sfn = sfn;
        p_paging_record->sf = sf;
        p_paging_record->cn_domain = p_s1ap_intrl_paging->cn_domain;
        p_paging_record->paging_drx = p_s1ap_intrl_paging->paging_drx;
        p_paging_record->ue_paging_id.t = p_s1ap_intrl_paging->ue_paging_id.t;
        if( p_s1ap_intrl_paging->bitmask & RRC_S1AP_INTRL_PAGING_PRIORITY_PRESENT)
        {
            p_paging_record->bitmask = PAGING_RECORD_PAGING_PRIORITY_PRESENT;
            p_paging_record->paging_priority = p_s1ap_intrl_paging->paging_priority;

        }
        if (T_s1ap_UEPagingID_s_TMSI == p_paging_record->ue_paging_id.t)
        {
            p_paging_record->ue_paging_id.u.s_TMSI =
                (s1ap_S_TMSI *)rrc_mem_get(sizeof(s1ap_S_TMSI));
            if ( PNULL != p_paging_record->ue_paging_id.u.s_TMSI )
            {
                l3_memcpy_wrapper (p_paging_record->ue_paging_id.u.s_TMSI,
                        p_s1ap_intrl_paging->ue_paging_id.u.s_TMSI,
                        sizeof(s1ap_S_TMSI));
            }

        }
        else if (T_s1ap_UEPagingID_iMSI == p_paging_record->ue_paging_id.t)
        {
            p_paging_record->ue_paging_id.u.iMSI =
                (s1ap_IMSI *)rrc_mem_get(sizeof(s1ap_IMSI));
            if ( PNULL != p_paging_record->ue_paging_id.u.iMSI )
            {
                l3_memcpy_wrapper (p_paging_record->ue_paging_id.u.iMSI,
                        p_s1ap_intrl_paging->ue_paging_id.u.iMSI,
                        sizeof(s1ap_IMSI));
            }
        }
        else if (T_s1ap_UEPagingID_extElem1 == p_paging_record->ue_paging_id.t)
        {
            p_paging_record->ue_paging_id.u.extElem1 = 
               (ASN1OpenType *)rrc_mem_get(sizeof(ASN1OpenType));
            if ( PNULL != p_paging_record->ue_paging_id.u.extElem1 )
            {
                p_paging_record->ue_paging_id.u.extElem1->numocts =
                    p_s1ap_intrl_paging->ue_paging_id.u.extElem1->numocts;
                p_paging_record->ue_paging_id.u.extElem1->data =
                    (OSOCTET *)rrc_mem_get(p_s1ap_intrl_paging->ue_paging_id.u.extElem1->numocts);
                if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                {
                    l3_memcpy_wrapper ((OSUINT8 *)p_paging_record->ue_paging_id.u.extElem1->data,
                        p_s1ap_intrl_paging->ue_paging_id.u.extElem1->data,
                        p_s1ap_intrl_paging->ue_paging_id.u.extElem1->numocts);
                }
            }
        }
        if (RRC_SUCCESS !=
            csc_paging_storage_reinsert(
                p_storage, 
                /* Bug 11748 Fix Start */
                p_paging_record, RRC_INVALID_CELL_INDEX))
                /* Bug 11748 Fix Stop */
        {
            break;
        }

        result = RRC_SUCCESS;
    } while (0);

    if (RRC_SUCCESS != result)
    {
        /* process failure */
        if (PNULL != p_paging_record)
        {
            if (PNULL != p_paging_record->ue_paging_id.u.s_TMSI)
            {
                rrc_mem_free(p_paging_record->ue_paging_id.u.s_TMSI);
                p_paging_record->ue_paging_id.u.s_TMSI = PNULL;
            }
            if (PNULL != p_paging_record->ue_paging_id.u.iMSI)
            {
                rrc_mem_free(p_paging_record->ue_paging_id.u.iMSI);
                p_paging_record->ue_paging_id.u.iMSI = PNULL;
            }
            if (PNULL != p_paging_record->ue_paging_id.u.extElem1)
            {
                if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                {
                    rrc_mem_free((void *)p_paging_record->ue_paging_id.u.extElem1->data);
                    p_paging_record->ue_paging_id.u.extElem1->data = PNULL;
                }
                rrc_mem_free(p_paging_record->ue_paging_id.u.extElem1);
                p_paging_record->ue_paging_id.u.extElem1 = PNULL;
            }
            /* free paging record */
            rrc_mem_free(p_paging_record);
            p_paging_record = PNULL;
        }
    }

    RRC_CSC_UT_TRACE_EXIT();

    return p_paging_record;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_reinsert
*   INPUT        : void*                 p_storage
*                  csc_paging_record_t   *p_paging_record
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS or RRC_FAILURE in case of failure
*   DESCRIPTION  : Reinsert UE Paging record into the CSC Paging storage. The memory for
*                  UE Paging record isn't allocated by this function.
*                  SFN,SF values are taken from UE Paging record.
******************************************************************************/
rrc_return_t csc_paging_storage_reinsert
(
        void*                 p_storage,
        /* Bug 11748 Fix Start */
        csc_paging_record_t   *p_paging_record,
        U8                    cell_index
        /* Bug 11748 Fix Stop */

)
{
    rrc_return_t    result = RRC_FAILURE;
    rrc_return_t    ret_code = RRC_FAILURE;
    csc_paging_storage_t    *p_paging_storage = PNULL;
    csc_paging_queue_t      *p_paging_queue = PNULL;
    rrc_sfn_t   sfn = RRC_NULL;
    rrc_sf_t    sf = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_storage);
    RRC_ASSERT(PNULL != p_paging_record);

    do {
        p_paging_storage = (csc_paging_storage_t*)p_storage;

        sfn = p_paging_record->sfn;
        sf = p_paging_record->sf;

        /* Store Paging record */
        p_paging_queue =
            p_paging_storage->
            sfn_sf_queues[sfn][sf];

        if (PNULL == p_paging_queue)
        {
            p_paging_queue =
                (csc_paging_queue_t*)rrc_mem_get(sizeof(csc_paging_queue_t));
            if (PNULL == p_paging_queue)
            {
                break;
            }

            PAGING_Q_INIT(p_paging_queue);
            p_paging_storage->sfn_sf_queues[sfn][sf] = p_paging_queue;
            p_paging_storage->cur_queues_count++;
        }

        if (MAX_NUM_PAGING_RECORDS_FOR_PAGING_OCCASION == 
                PAGING_Q_GET_COUNT(p_paging_queue))
        {
            /* Check whether received Paging is for High Priority user or not */
            if (!(p_paging_record->bitmask & PAGING_RECORD_PAGING_PRIORITY_PRESENT))
            {


                /* Maximum Number of paging records have alredy been added 
                 * for this paging occasion. Discard and update performance
                 * counter. */
                /* Do not free the paging record here. It will be freed from caller
                 * function.*/
                /* No need to free paging Queue as record is for already existing
                 * queue. */
                return result;

            }
                /* Find and delete Low Priority Paging record from Paging Q */
            ret_code = csc_delete_low_priority_paging_record( p_paging_queue, p_paging_record);
            if (ret_code == RRC_FAILURE)
            {
                return result;
            }
            
            /* Bug 11748 Fix Start */
            if ( (PNULL != g_rrc_stats.p_rrc_stats) &&
                    (RRC_INVALID_CELL_INDEX != cell_index) )
            {
                RRC_CSC_UPDATE_PERFORMACE_COUNTER(g_rrc_stats.p_rrc_stats->
                        rrc_csc_stats.rrc_csc_cell_stats[cell_index].
                        rrc_paging_related_stats.pag_discardedNbr, 0, 1);
            }
            /* Bug 11748 Fix Stop */

            RRC_CSC_TRACE(RRC_INFO, "PAGING Queue count after record deletion: %d", 
                    PAGING_Q_GET_COUNT(p_paging_queue));
        p_paging_storage->cur_paging_records_count--;

        }
        PAGING_Q_ENQUEUE(p_paging_queue, p_paging_record);

        result = RRC_SUCCESS;
        p_paging_storage->cur_paging_records_count++;

        RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] queues[0x%u] records[0x%u]",
                p_paging_storage->cur_queues_count,
                p_paging_storage->cur_paging_records_count);

    } while (0);


    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_storage_get
*   INPUT        : void* p_storage
*                  rrc_sfn_t   sfn
*                  rrc_sf_t    sf
*   OUTPUT       : none
*   RETURNS      : csc_paging_record_t* - pinter on Paging record
*                 (or PNULL if it has not been found)
*   DESCRIPTION  : Finds Paging record in the CSC Paging storage.
******************************************************************************/
csc_paging_record_t* csc_paging_storage_get
(
        void* p_storage,
        rrc_sfn_t   sfn,
        rrc_sf_t    sf
)
{
    csc_paging_storage_t    *p_paging_storage = PNULL;
    csc_paging_record_t     *p_paging_record = PNULL;
    csc_paging_queue_t      *p_paging_queue = PNULL;

    RRC_ASSERT(PNULL != p_storage);

    p_paging_storage = (csc_paging_storage_t*)p_storage;

    /* Get Paging record */
    p_paging_queue = p_paging_storage->sfn_sf_queues[sfn][sf];
    if (PNULL != p_paging_queue)
    {
        PAGING_Q_DEQUEUE(p_paging_queue, p_paging_record);

        if (0 == PAGING_Q_GET_COUNT(p_paging_queue))
        {
            if (PNULL != p_paging_queue)
            {
            /* The paging queue is empty - free */
            rrc_mem_free(p_paging_queue);
                p_paging_queue = PNULL;
            }

            p_paging_storage->sfn_sf_queues[sfn][sf] = PNULL;
            p_paging_storage->cur_queues_count--;
        }

        if (PNULL != p_paging_queue)
        {
            p_paging_storage->cur_paging_records_count--;

            RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] queues[0x%u] records[0x%u]",
                    p_paging_storage->cur_queues_count,
                    p_paging_storage->cur_paging_records_count);
        }
    }


    return p_paging_record;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_process_ue_paging_request
*   INPUT        : csc_cell_ctx_t              *p_cell_ctx
*                  rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging
*                  rrc_sfn_t                   rrc_curr_sfn
*                  csc_gl_ctx_t                *p_csc_gl_ctx
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Processes UE Paging request.
*                  If UE is suitable for paging the corresponding Paging record will be
*                  stored in the CSC Paging storage and UE will be paged
*                  within appropriate System frame.
******************************************************************************/
rrc_return_t csc_paging_process_ue_paging_request(
        csc_cell_ctx_t              *p_cell_ctx,
        rrc_s1ap_asn_intrl_paging_t *p_s1ap_intrl_paging,
        /* Current SFN in RRC cell */
        rrc_sfn_t                   rrc_curr_sfn,
        csc_gl_ctx_t                *p_csc_gl_ctx
)
{
    rrc_return_t result = RRC_FAILURE;
    rrc_sfn_t   paging_sfn = RRC_NULL;
    rrc_sf_t    paging_sf = RRC_NULL;
    csc_paging_record_t *p_paging_record = PNULL;
    rrc_sfn_t   sfn_drx = RRC_NULL;

    time_t time1 ;
    time_wrapper(&time1);

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_s1ap_intrl_paging);
    RRC_ASSERT(PNULL != p_cell_ctx->p_paging_storage);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    do {
        /* CSC checks that eNB cell is in UE's PAGING List of TAIs */
        /* CSC checks that UE belong to CSG of eNB cell if applicable.*/
        if (RRC_TRUE != csc_paging_is_ue_suitable_for_paging(
                            p_s1ap_intrl_paging,
                            &p_cell_ctx->cfg_cell_param.sib_type_1_Info.
                                cell_access_related_info))
        {
            /* Ignore message */
            RRC_CSC_TRACE(RRC_WARNING,
                "[CELL:%u][PAGING] The UE isn't suitable for paging",
                p_cell_ctx->cell_index);
            break;
        }
        /* CR 025 changes start*/
        /*Updating the Performance Counter */
        if(PNULL != g_rrc_stats.p_rrc_stats)
        {
            RRC_CSC_UPDATE_PERFORMACE_COUNTER(g_rrc_stats.p_rrc_stats->
                    rrc_csc_stats.rrc_csc_cell_stats[p_cell_ctx->cell_index].
                    rrc_paging_related_stats.pag_receivedNbr, 0, 1);
        }
        /* CR 025 changes stop*/

        /* Select useful Paging DRX - It is enum value */
        if (!(RRC_S1AP_INTRL_PAGING_DRX_PRESENT & p_s1ap_intrl_paging->bitmask)
            ||
            (p_cell_ctx->cfg_cell_param.sib_type_2_Info.
             radio_resource_config_common_sib.pcch_config.
             default_paging_cycle <= p_s1ap_intrl_paging->paging_drx))
        {
            /* use default paging DRX */
            p_s1ap_intrl_paging->bitmask |= RRC_S1AP_INTRL_PAGING_DRX_PRESENT;
            p_s1ap_intrl_paging->paging_drx =
                    p_cell_ctx->cfg_cell_param.sib_type_2_Info.
                    radio_resource_config_common_sib.pcch_config.
                    default_paging_cycle;
        }

        /* Calculate SFN and SF */
        /* CSC determine the pair of Paging frame and subframe for paging message */

        /*** TDD changes start ***/
        if (RRC_SUCCESS != csc_paging_determine_paging_sfn_sf(
                                p_s1ap_intrl_paging,
                                &p_cell_ctx->cfg_cell_param.sib_type_2_Info.
                                    radio_resource_config_common_sib.pcch_config,
                                rrc_curr_sfn,
                                &paging_sfn, &paging_sf,
                                p_cell_ctx->duplexing_mode) )
        /*** TDD changes stop ***/
            
        
        {
            /* Ignore message */
            RRC_CSC_TRACE(RRC_WARNING,
                "[CELL:%u][PAGING] The (SFN, SF) determination failure",
                p_cell_ctx->cell_index);
            break;
        }
        RRC_CSC_TRACE(RRC_INFO,"TARGET sfn : %d SF :%d CURRENT sfn :%d",
                paging_sfn,paging_sf,rrc_curr_sfn);

        /* First condition checks if the difference bewteen current and target SFN
         * is less than equal to 5. 
         * The second one checks if the target SFN has rolled the SFN_UPPER_LIMIT */ 

        if (((paging_sfn > rrc_curr_sfn) && 
                    (CSC_SFN_OFFSET >= (paging_sfn - rrc_curr_sfn))) ||
              ((rrc_curr_sfn > paging_sfn) && 
                 (CSC_SFN_OFFSET >= ((paging_sfn + SFN_UPPER_LIMIT) - rrc_curr_sfn))))
        {
            /* Convert PagingDRX from enum to SFN */
            sfn_drx = (rrc_sfn_t)(32 << (U16)(p_s1ap_intrl_paging->paging_drx));

            paging_sfn = (rrc_sfn_t)((paging_sfn + sfn_drx) % SFN_UPPER_LIMIT);
        }

       
        /* Store Paging records */
        p_paging_record = csc_paging_storage_insert(
                p_csc_gl_ctx,
                p_cell_ctx->p_paging_storage,
                paging_sfn, paging_sf,
                            p_s1ap_intrl_paging
                            );
        if (PNULL ==  p_paging_record)
        {
            if(PNULL != g_rrc_stats.p_rrc_stats)
            {
                RRC_CSC_UPDATE_PERFORMACE_COUNTER(g_rrc_stats.p_rrc_stats->
                        rrc_csc_stats.rrc_csc_cell_stats[p_cell_ctx->cell_index].
                        rrc_paging_related_stats.pag_discardedNbr, 0, 1);

                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING,
                        "[CELL:%u][PAGING] The Paging record storing failure",
                        p_cell_ctx->cell_index);
                break;
            }
        }
        else
        {

            RRC_CSC_TRACE(RRC_BRIEF,
            "[CELL:%u][PAGING] The UE Paging request has been scheduled",
            p_cell_ctx->cell_index);

                        csc_paging_print_paging_record(p_paging_record
                        );

            result = RRC_SUCCESS;
        }
    } while (0);
    /* SPR 20007 Fix Start */
    /* SPR 20007 : CID 110665 fix start */
    /* Code Removed */
    /* SPR 20007 : CID 110665 fix end */
    /* SPR 20007 Fix End */
    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_page_ues
*   INPUT        : csc_cell_ctx_t      *p_cell_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : Makes and sends UE Paging requests to all UEs for which
*                  Paging records with appropriate SFN are stored in CSC Paging storage.
******************************************************************************/
void csc_paging_page_ues(
        csc_cell_ctx_t      *p_cell_ctx
        /*SPR 17777 +-*/
)
{
    OSCTXT          asn1_ctx;
    PCCH_Message    pcch_message;
    rrc_sf_t        sf_index;
    U16             ue_paged;
    csc_paging_record_t     *p_paging_record = PNULL;
    rrc_sfn_t       sfn_with_offset;


    RRC_ASSERT(PNULL != p_cell_ctx);

    sfn_with_offset =
        (rrc_sfn_t)((p_cell_ctx->sfn + CSC_SFN_OFFSET) % SFN_UPPER_LIMIT);

    for(sf_index = 0; sf_index<SF_MAX_LIMIT; sf_index++)
    {
        ue_paged = 0;

        /* The PCCH message should be sent for each pair of SFN,SF */


        /* Init ASN1 context */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_CSC_TRACE(RRC_ERROR,
                "[CELL:%u][PAGING] ASN1 context initialization failed.",
                p_cell_ctx->cell_index);
            break;
        }

        while (PNULL != (p_paging_record =
                            csc_paging_storage_get(
                                p_cell_ctx->p_paging_storage,
                                sfn_with_offset, sf_index)))
        {
            if (0 == ue_paged)
            {
                if (RRC_SUCCESS !=
                        csc_paging_prepare_rrc_paging_message(&asn1_ctx, &pcch_message,p_cell_ctx))
                {
                    RRC_CSC_TRACE(RRC_ERROR,
                            "[CELL:%u][PAGING] RRC Paging message initialization failed",
                            p_cell_ctx->cell_index);
                    break;
                }
                /* eMTC changes stop */
            }

            if (RRC_SUCCESS ==
                csc_paging_add_paging_record_to_rrc_paging_message(
                        &asn1_ctx, &pcch_message, p_paging_record))
	    {
		    /*SPR_18230_START*/
		    RRC_CSC_TRACE(RRC_INFO,
				    "[CELL:%u][PAGING] The UE has been paged.",
				    p_cell_ctx->cell_index);
		    ue_paged++;
		    if(PNULL != g_rrc_stats.p_rrc_stats)
		    {
			    RRC_CSC_UPDATE_PERFORMACE_COUNTER(g_rrc_stats.p_rrc_stats->
					    rrc_csc_stats.rrc_csc_cell_stats[p_cell_ctx->cell_index].
					    rrc_paging_related_stats.pag_succNbr, 0, 1);
		    }
		    /*SPR_18230_END*/
	    }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "[CELL:%u][PAGING] UE Paging record insertion failed",
                    p_cell_ctx->cell_index);
            }

            csc_paging_print_paging_record(p_paging_record
            );

            if (PNULL != p_paging_record)
            {
              if (PNULL != p_paging_record->ue_paging_id.u.s_TMSI)
              {
                rrc_mem_free(p_paging_record->ue_paging_id.u.s_TMSI);
                p_paging_record->ue_paging_id.u.s_TMSI = PNULL;
              }
              if (PNULL != p_paging_record->ue_paging_id.u.iMSI)
              {
                rrc_mem_free(p_paging_record->ue_paging_id.u.iMSI);
                p_paging_record->ue_paging_id.u.iMSI = PNULL;
              }
              if (PNULL != p_paging_record->ue_paging_id.u.extElem1)
              {
                if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                {
                    rrc_mem_free((void *)p_paging_record->ue_paging_id.u.extElem1->data);
                    p_paging_record->ue_paging_id.u.extElem1->data = PNULL;
                }
                rrc_mem_free(p_paging_record->ue_paging_id.u.extElem1);
                p_paging_record->ue_paging_id.u.extElem1 = PNULL;
              }
              /* free paging record */
              rrc_mem_free(p_paging_record);
              p_paging_record = PNULL;
            }
        }

        if (0 < ue_paged)
        {
            /* Send PCCH message */
            if (RRC_SUCCESS != csc_paging_build_and_send_llim_pcch_msg(
                                    &asn1_ctx,
                                    &pcch_message,
                                    sfn_with_offset,
                                    sf_index,
                                    p_cell_ctx
                                    ))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                        "[CELL:%u][PCCH_Message] Delivery failed",
                        p_cell_ctx->cell_index);
            }
            /*SPR_18230_START*/
            /*code removed */ 
            /*SPR_18230_END*/
        }

        /* Free ASN.1 context */
        rtFreeContext(&asn1_ctx);
    }

}

/******************************************************************************
*   FUNCTION NAME: csc_paging_plmn_identity_rrc_to_s1ap
*   INPUT        : U8              *p_plmn_id_as_s1ap
*                  plmn_identity_t *p_plmn_identity
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This helper function converts PLMN Id representation from RRC to S1AP.
*                  Attention this function use history for MCC digits!
******************************************************************************/
void csc_paging_plmn_identity_rrc_to_s1ap
(
    /* Destination - size must be NUM_OCTET_IN_S1AP_PLMN_ID */
    U8              *p_plmn_id_as_s1ap,
    /* Source - C struct that represent PLMN Id from RRC specification */
    plmn_identity_t *p_plmn_identity
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_plmn_id_as_s1ap);
    RRC_ASSERT(PNULL != p_plmn_identity);

    if (p_plmn_identity->presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG)
    {
        RRC_ASSERT(MCC_OCTET_SIZE == 3);
        p_plmn_id_as_s1ap[0] = p_plmn_identity->mcc[0];
        p_plmn_id_as_s1ap[0] = (U8)(p_plmn_id_as_s1ap[0] |
                               p_plmn_identity->mcc[1] << 4);
        p_plmn_id_as_s1ap[1] = p_plmn_identity->mcc[2];
    }

    if (3 == p_plmn_identity->mnc.count)
    {
        p_plmn_id_as_s1ap[1] = (U8)(p_plmn_id_as_s1ap[1] |
                               p_plmn_identity->mnc.mnc[0] << 4);
        p_plmn_id_as_s1ap[2] = p_plmn_identity->mnc.mnc[1];
        p_plmn_id_as_s1ap[2] = (U8)(p_plmn_id_as_s1ap[2] |
                               p_plmn_identity->mnc.mnc[2] << 4);
    }
    else if (2 == p_plmn_identity->mnc.count)
    {
        p_plmn_id_as_s1ap[1] = (U8)(p_plmn_id_as_s1ap[1] |
                               FILLER_DIGIT_IN_S1AP << 4);
        p_plmn_id_as_s1ap[2] = p_plmn_identity->mnc.mnc[0];
        p_plmn_id_as_s1ap[2] = (U8)(p_plmn_id_as_s1ap[2] |
                               p_plmn_identity->mnc.mnc[1] << 4);
    }
    else
    {
        RRC_ASSERT(0);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_is_ue_suitable_for_paging
*   INPUT        : rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging
*                  cell_access_related_info_t  *p_curr_cell_info
*   OUTPUT       : none
*   RETURNS      : RRC_TRUE - if we should page UE
*                  RRC_FALSE - if we shouldn't page UE
*   DESCRIPTION  : This function checks is UE suitable for paging in this cell or not.
******************************************************************************/
rrc_bool_et csc_paging_is_ue_suitable_for_paging
(
    /* Pointer to S1AP ASN.1 based internal representation of paging message */
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,
    /* Pointer to cell access related info from SIB1 */
    cell_access_related_info_t  *p_curr_cell_info
)
{
    OSRTDListNode           *p_tai_list_node    = RRC_NULL;
    TAIListIntrl_element    *p_tai_list_elem    = RRC_NULL;
    rrc_bool_et             ue_suitable_paging  = RRC_FALSE;
    rrc_bool_et             ue_could_be_paged_in_not_primaty_plmn = RRC_FALSE;

    RRC_CSC_UT_TRACE_ENTER();

    /* CSC checks that eNB cell is in UE's PAGING List of TAIs */
    for (p_tai_list_node = p_s1ap_asn_intrl_paging->tai_list.head; (0 !=
    p_tai_list_node) && (RRC_FALSE == ue_suitable_paging); p_tai_list_node =
        p_tai_list_node->next)
    {
        if ( ((s1ap_TAIList_element*)p_tai_list_node->data)->value.u._TAIItemIEs_1 == PNULL)
        {
            break;
        }
        p_tai_list_elem = (TAIListIntrl_element*)((s1ap_TAIList_element*)p_tai_list_node->data)->value.u._TAIItemIEs_1;

        if (    (sizeof(p_curr_cell_info->tac) == p_tai_list_elem->value.tAI.tAC
            .numocts) &&
                (0 == memcmp_wrapper(p_curr_cell_info->tac, p_tai_list_elem->value.
                    tAI.tAC.data, p_tai_list_elem->value.tAI.tAC.numocts)) )
        {
            /* The same TAC. Check PLMN identity */
            U16 plmn_id_index = RRC_NULL;
            U8  curr_cell_curr_plmn_id_as_s1ap[NUM_OCTET_IN_S1AP_PLMN_ID];
            memset_wrapper(curr_cell_curr_plmn_id_as_s1ap, 0, sizeof(U8)*NUM_OCTET_IN_S1AP_PLMN_ID );

            /* Must be at list one element in list. This point should be checked
             *  on receiving SIB1 info */
            RRC_ASSERT(p_curr_cell_info->plmn_Id_info_list.count > 0);
            /* MCC is mandatory for the first element in list. This point should
             *  be checked on receiving SIB1 info */
            RRC_ASSERT(p_curr_cell_info->plmn_Id_info_list.plmn_identity_info[0]
               .plmn_identity.presence_bitmask&PLMN_IDENTITY_MCC_PRESENCE_FLAG);

            for (plmn_id_index = 0; plmn_id_index < p_curr_cell_info->
                plmn_Id_info_list.count; ++plmn_id_index)
            {
                plmn_identity_t *p_curr_cell_curr_plmn_id = &p_curr_cell_info->
                plmn_Id_info_list.plmn_identity_info[plmn_id_index].
                    plmn_identity;

                /* Convert from RRC C structure representation to ASN.1 S1AP
                 *  representation */
                csc_paging_plmn_identity_rrc_to_s1ap(
                        curr_cell_curr_plmn_id_as_s1ap,
                        p_curr_cell_curr_plmn_id);

                if (    (NUM_OCTET_IN_S1AP_PLMN_ID == p_tai_list_elem->value.tAI
                    .pLMNidentity.numocts) &&
                        (0 == memcmp_wrapper(curr_cell_curr_plmn_id_as_s1ap,
                        p_tai_list_elem->value.tAI.pLMNidentity.data,
                            NUM_OCTET_IN_S1AP_PLMN_ID)) )
                {
                    /* This UE could belong to our cell */
                    ue_suitable_paging = RRC_TRUE;
                    if (0 != plmn_id_index)
                    {
                        ue_could_be_paged_in_not_primaty_plmn = RRC_TRUE;
                    }

                    break;
                }
            }
        }
    }

    if (RRC_TRUE != ue_suitable_paging)
    {
        RRC_CSC_TRACE(RRC_INFO, "We got S1AP PAGING for UE that doesn't belong"
            " to this cell.");
    }
    else
    {
        /* CSC checks that UE belong to CSG of eNB cell if applicable. */
        if (    (p_s1ap_asn_intrl_paging->bitmask &
            RRC_S1AP_INTRL_CSG_ID_LIST_PRESENT) &&
                (p_curr_cell_info->presence_bitmask &
                    CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG) &&
                (RRC_FALSE == ue_could_be_paged_in_not_primaty_plmn) )
        {
            /* Check that UE could be paged in not primary PLMN of this cell */
            /* Start check from the next element! */
            for(;(0 != p_tai_list_node) && (RRC_FALSE ==
            ue_could_be_paged_in_not_primaty_plmn); p_tai_list_node =
                p_tai_list_node->next)
            {
                p_tai_list_elem = (TAIListIntrl_element *)p_tai_list_node->data;

                if (    (sizeof(p_curr_cell_info->tac) == p_tai_list_elem->value
                    .tAI.tAC.numocts) &&
                        (0 == memcmp_wrapper(p_curr_cell_info->tac, p_tai_list_elem
                        ->value.tAI.tAC.data, p_tai_list_elem->value.tAI.tAC.
                            numocts)) )
                {
                    /* The same TAC. Check PLMN identity */
                    U16 plmn_id_index = RRC_NULL;
                    U8
                        curr_cell_curr_plmn_id_as_s1ap[NUM_OCTET_IN_S1AP_PLMN_ID];
                    memset_wrapper(curr_cell_curr_plmn_id_as_s1ap, 0, sizeof(U8)*NUM_OCTET_IN_S1AP_PLMN_ID );

                    /* Must be at list one element in list. This point should be
                     *  checked on receiving SIB1 info */
                    RRC_ASSERT(p_curr_cell_info->plmn_Id_info_list.count > 0);
                    /* MCC is mandatory for the first element in list. This
                     *  point should be checked on receiving SIB1 info */
                    RRC_ASSERT(p_curr_cell_info->plmn_Id_info_list.
                    plmn_identity_info[0].plmn_identity.presence_bitmask &
                        PLMN_IDENTITY_MCC_PRESENCE_FLAG);
                    /* Convert MCC */
                    csc_paging_plmn_identity_rrc_to_s1ap(
                            curr_cell_curr_plmn_id_as_s1ap,
                            &p_curr_cell_info->plmn_Id_info_list.
                            plmn_identity_info[0].plmn_identity);

                    /* Except primary PLMN */
                    for (plmn_id_index = 1; plmn_id_index < p_curr_cell_info->
                        plmn_Id_info_list.count; ++plmn_id_index)
                    {
                        plmn_identity_t *p_curr_cell_curr_plmn_id =
                        &p_curr_cell_info->plmn_Id_info_list.
                            plmn_identity_info[plmn_id_index].plmn_identity;

                        /* Convert from RRC C structure representation to ASN.1
                         *  S1AP representation */
                        csc_paging_plmn_identity_rrc_to_s1ap(
                                curr_cell_curr_plmn_id_as_s1ap,
                                p_curr_cell_curr_plmn_id);

                        if (    (NUM_OCTET_IN_S1AP_PLMN_ID == p_tai_list_elem->
                            value.tAI.pLMNidentity.numocts) &&
                                (0 == memcmp_wrapper(curr_cell_curr_plmn_id_as_s1ap,
                                p_tai_list_elem->value.tAI.pLMNidentity.data,
                                    NUM_OCTET_IN_S1AP_PLMN_ID)) )
                        {
                            /* This UE could be paged in not primary PLMN of
                             *  this cell */
                            ue_could_be_paged_in_not_primaty_plmn = RRC_TRUE;
                            break;
                        }
                    }
                }
            }

            if (RRC_FALSE == ue_could_be_paged_in_not_primaty_plmn)
            {
                /* This UE could be paged in this cell only in primaty PLMN */
                /* And this is CSG cell and UE is CSG UE. In this case UE must
                 *  be from cell CSG */

                OSRTDListNode   *p_node = RRC_NULL;
                s1ap_CSG_IdList_Item *p_elem = RRC_NULL;

                ue_suitable_paging = RRC_FALSE;

                /* CSG id has size 27 bit - clear last bits of 4-th octet */
                p_curr_cell_info->csg_identity[3] =
                    (U8)(p_curr_cell_info->csg_identity[3] & 0xE0);

                for (p_node = p_s1ap_asn_intrl_paging->csg_id_list.head; (0 !=
                p_node) && (RRC_FALSE == ue_suitable_paging); p_node = p_node->
                    next)
                {
                    p_elem = (s1ap_CSG_IdList_Item *)p_node->data;

                    if (    (sizeof(p_elem->cSG_Id.data) == sizeof
                        (p_curr_cell_info->csg_identity)) &&
                            (0 == memcmp_wrapper(p_elem->cSG_Id.data,
                            p_curr_cell_info->csg_identity, sizeof
                                (p_curr_cell_info->csg_identity))) )
                    {
                        /* UE belong to CSG of this cell */
                        ue_suitable_paging = RRC_TRUE;
                        break;
                    }
                }
            }
        }

        if (RRC_TRUE != ue_suitable_paging)
        {
            RRC_CSC_TRACE(RRC_INFO, "We got S1AP PAGING for CSG UE that "
                "doesn't belong to CSG of this cell.");
        }
    }

    RRC_CSC_UT_TRACE_EXIT();

    return ue_suitable_paging;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_determine_paging_sfn_sf
*   INPUT        : rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging
*                  pcch_config_t               *p_pcch_config
*                  rrc_sfn_t                   rrc_curr_sfn
*                  rrc_sfn_t                   *p_paging_frame
*                  rrc_sf_t                    *p_paging_sub_frame
*                  duplexing_mode_et            duplexing_mode
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : This function determine target SFN and SF for paging message.
******************************************************************************/
rrc_return_t csc_paging_determine_paging_sfn_sf
(
    /* Pointer to S1AP ASN.1 based internal representation of paging message */
    rrc_s1ap_asn_intrl_paging_t *p_s1ap_asn_intrl_paging,
    /* Pointer to pcch config from SIB2 */
    pcch_config_t               *p_pcch_config,
    /* Current SFN in RRC cell */
    rrc_sfn_t                   rrc_curr_sfn,
    /* Target SFN of Paging message */
    rrc_sfn_t                   *p_paging_frame,
    /* Target SF of Paging message */
    rrc_sf_t                    *p_paging_sub_frame,
    
    /****** TDD changes start */
    /* Duplexing Mode */
    duplexing_mode_et            duplexing_mode
    /****** TDD changes stop */

)
{
    rrc_return_t result = RRC_FAILURE;
    rrc_sfn_t   t       = RRC_NULL;
    rrc_sfn_t   nb      = p_pcch_config->nb;
    rrc_sfn_t   n       = RRC_NULL;
    U8          ns      = RRC_NULL;
    rrc_sfn_t   sfn_mt  = RRC_NULL;
    U8          i_s     = RRC_NULL;
    U16         ue_id   = RRC_NULL;

    rrc_sfn_t   sfn_g   = RRC_NULL;
    rrc_sfn_t   sfn_e   = RRC_NULL;

    rrc_sfn_t   paging_frame        = RRC_NULL;
    rrc_sf_t    paging_sub_frame    = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();

    do {
        /* Useful Paging DRX is prepared by caller */
        t = (rrc_sfn_t)p_s1ap_asn_intrl_paging->paging_drx;

        /* Convert t to SFN */
        t = (rrc_sfn_t)(32 << t);

        /* Convert nb to SFN as part of T */
        if (RRM_RRC_NB_ONE_T >= nb)
        {
            nb = (rrc_sfn_t)(t << (RRM_RRC_NB_ONE_T - nb));
        }
        else
        {
            nb = (rrc_sfn_t)(t >> (nb - RRM_RRC_NB_ONE_T));
        }

        /* Convert UE_ID. */
        if (10 != p_s1ap_asn_intrl_paging->ue_identity_index_value.numbits)
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "[PAGING] S1AP ue_identity_index_value size invalid [%u]",
                    p_s1ap_asn_intrl_paging->ue_identity_index_value.numbits);
            break;
        }
        ue_id = (U16)
           (((U16)p_s1ap_asn_intrl_paging->ue_identity_index_value.data[0] << 2)
           |
           (p_s1ap_asn_intrl_paging->ue_identity_index_value.data[1] >> 6));

        /* Actually here n and ns are power of 2, so we could use instead of
         *  % a - & (a - 1), but I guess it is not necessary here */
        if (nb > t)
        {
            n = t;
            ns = (U8)(nb / t);
            /* Because of n == t : */
            sfn_mt = ue_id % n;
            i_s = (U8)((ue_id / n) % ns);
        }
        else
        {
            n = nb;
            ns = 1;
            sfn_mt = (rrc_sfn_t)((t / n) * (ue_id % n));
            /* Because of ns == 1 : */
            i_s = 0;
        }
        if (sfn_mt >= t)
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "[PAGING] sfn_mt value invalid sfn_mt[%u] >= t[%u]", sfn_mt, t);
            break;
        }

        /* Determine target paging sub frame. */
        if (  (ns - 1) < 0 ||
             ((ns - 1) >= NUM_PAGING_NS_VALUES)
            || (i_s >= NUM_PAGING_I_S_VALUES))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "[PAGING] (ns[%u] - 1) >= NUM_PAGING_NS_VALUES[%u] or "
                "i_s[%u] >= NUM_PAGING_I_S_VALUES[%u])",
                ns, NUM_PAGING_NS_VALUES, i_s, NUM_PAGING_I_S_VALUES);
            break;
        };

        /* It depends on cell type (FDD or TDD)*/
        /****** TDD changes start *******/
        if (DUPLEXING_MODE_FDD == duplexing_mode)
        {
            RRC_CSC_TRACE(RRC_INFO,
                    "[PAGING] calculating paging_sub_frame value according to FDD [%u]",
                    paging_sub_frame);
            paging_sub_frame = paging_subframe_patterns_fdd[ns - 1][i_s];
        }
        else
        {
            RRC_CSC_TRACE(RRC_INFO,
                    "[PAGING] calculating paging_sub_frame value according to TDD [%u]",
                    paging_sub_frame);
            paging_sub_frame = paging_subframe_patterns_tdd[ns - 1][i_s];
        }

        /****** TDD changes stop *******/


        if (DUMMY_SUBFRAME_VALUE == paging_sub_frame)
        {
            RRC_CSC_TRACE(RRC_WARNING,
              "[PAGING] paging_sub_frame value invalid [%u]", paging_sub_frame);
            break;
        }

        /* Determine target paging frame. */
        sfn_g = (rrc_sfn_t)((paging_sub_frame > PAGING_BIG_SMALL_SF_BOUNDARY) ?
                PAGING_SFN_GAP_FOR_BIG_SF : PAGING_SFN_GAP_FOR_SMALL_SF);
        sfn_e = (rrc_sfn_t)((rrc_curr_sfn + sfn_g) % SFN_UPPER_LIMIT);
        paging_frame = (rrc_sfn_t)(sfn_mt + (sfn_e / t) * t);
        if (paging_frame < sfn_e)
        {
            if ( (paging_frame + t) <= sfn_e)
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "[PAGING] (paging_frame[%u] + t[%u]) <= sfn_e[%u] ",
                    paging_frame, t, sfn_e);
                break;
            }
            paging_frame = (rrc_sfn_t)((paging_frame + t) % SFN_UPPER_LIMIT);
        }
        if (paging_frame >= SFN_UPPER_LIMIT)
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "[PAGING] paging_frame[%u] >= SFN_UPPER_LIMIT[%u] ",
                paging_frame, SFN_UPPER_LIMIT);
            break;
        }

        *p_paging_frame = paging_frame;
        *p_paging_sub_frame = paging_sub_frame;

        result = RRC_SUCCESS;
    } while (0);

    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_build_and_send_llim_pcch_msg
*   INPUT        : OSCTXT       *p_asn1_ctx,
*                  PCCH_Message *p_pcch_message,
*                  rrc_sfn_t    sfn,
*                  rrc_sf_t     sf,
*                  csc_cell_ctx_t   *p_cell_ctx
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : This function bulds and sends prepared ASN1 encoded message to LLIM
******************************************************************************/
rrc_return_t csc_paging_build_and_send_llim_pcch_msg(
        OSCTXT       *p_asn1_ctx,
        PCCH_Message *p_pcch_message,
        rrc_sfn_t    sfn,
        rrc_sf_t     sf,
        csc_cell_ctx_t   *p_cell_ctx
)
{
    rrc_return_t    result = RRC_FAILURE;
    OSINT32 encoded_rrc_paging_msg_buffer_length = RRC_NULL;
    U8      *p_encoded_rrc_paging_msg = PNULL;
    void    *p_intrl_msg = PNULL;
    rrc_csc_llim_pcch_data_req_t *p_csc_llim_pcch_data = PNULL;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;


    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    do {
        pu_setBuffer(p_asn1_ctx, 0, 0, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_PCCH_Message(p_asn1_ctx, p_pcch_message))
        {
            RRC_CSC_TRACE(RRC_WARNING, "[PCCH_Message] ASN1 encoding failed");
            break;
        }

        p_encoded_rrc_paging_msg = pe_GetMsgPtr(
                                       p_asn1_ctx,
                                       &encoded_rrc_paging_msg_buffer_length);

        if (PNULL == p_encoded_rrc_paging_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,
                    "[PCCH_Message] ASN.1 failure");
            break;
        }

        RRC_CSC_TRACE( RRC_BRIEF,
             "RRC_MSG: PAGING %s",
             change_data_to_str(encoded_rrc_paging_msg_buffer_length, p_encoded_rrc_paging_msg));

        /* contela issue fix */
        /* contela issue fix */

        /* Prepare message to LLIM */
        p_intrl_msg = csc_alloc_intrl_msg(
                       RRC_LLIM_MODULE_ID,
                       CSC_LLIM_PCCH_DATA_REQ,
                       (U16)(sizeof(rrc_csc_llim_pcch_data_req_t) +
                               (U16)encoded_rrc_paging_msg_buffer_length));

        if (PNULL == p_intrl_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,
                  "[CSC_LLIM_PCCH_DATA_REQ] Memory allocation failure");
            break;
        }

        /* Generating Protocol  Event LOCAL_RRC_PAGING*/
        p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                (U32)encoded_rrc_paging_msg_buffer_length);
        if (PNULL != p_event_header)
        {
            memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                        (U32)encoded_rrc_paging_msg_buffer_length));

            p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
                    (U32)encoded_rrc_paging_msg_buffer_length);
            p_event_header->event_id = LOCAL_RRC_PAGING;
            p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
            p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = (U32)encoded_rrc_paging_msg_buffer_length;	    

            l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
                    sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));

            p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                EVENT_PARAM_CELL_ID;

            l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                        sizeof(EVENT_EXTERNAL_HEADER)),
                    (const void*)p_encoded_rrc_paging_msg,
                    (U32)encoded_rrc_paging_msg_buffer_length);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
        }

        /* Fill and send message */
        p_csc_llim_pcch_data =
            (rrc_csc_llim_pcch_data_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);

        /* Fill LLIM PCCH message */
        p_csc_llim_pcch_data->transaction_id = RRC_TRANSACTION_ID_ABSENT;
        p_csc_llim_pcch_data->paging_frame = sfn;
        /* SPR fix 22994 start */
        /* SPR fix 22994 stop */
        p_csc_llim_pcch_data->paging_sub_frame = sf;
        
        p_csc_llim_pcch_data->num_of_sfn = 1; 
        p_csc_llim_pcch_data->cell_index = p_cell_ctx->cell_index;


        /*Set the SI modification flag */
        p_csc_llim_pcch_data->paging_msg_buffer_length =
            (rrc_counter_t)encoded_rrc_paging_msg_buffer_length;
        l3_memcpy_wrapper(p_csc_llim_pcch_data->paging_msg_buffer,
                  p_encoded_rrc_paging_msg,
                  (size_t)encoded_rrc_paging_msg_buffer_length);

        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

        result = RRC_SUCCESS;
    }while (0);

    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_prepare_rrc_paging_message
*   INPUT        : OSCTXT                      *p_asn1_ctx
*                  PCCH_Message                *p_pcch_message
*                  csc_cell_ctx_t              *p_cell_ctx
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Prepares general part of RRC Paging message.
******************************************************************************/
rrc_return_t csc_paging_prepare_rrc_paging_message
(
    OSCTXT                      *p_asn1_ctx, /* ASN.1 context */
    PCCH_Message                *p_pcch_message,
    csc_cell_ctx_t              *p_cell_ctx 
)
{
    rrc_return_t    result = RRC_FAILURE;
    Paging          *p_rrc_paging       = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    do {
        asn1Init_PCCH_Message(p_pcch_message);

        /* PCCH_MessageType_c1 */
        p_pcch_message->message.t = T_PCCH_MessageType_c1;
        p_pcch_message->message.u.c1 =
            rtxMemAllocType(p_asn1_ctx, PCCH_MessageType_c1);
        if (NULL == p_pcch_message->message.u.c1)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR,
                  "[PCCH_Message] Memory allocation failure");
            break;
        }
        asn1Init_PCCH_MessageType_c1( p_pcch_message->message.u.c1 );

        /* Paging */
        p_pcch_message->message.u.c1->t = T_PCCH_MessageType_c1_paging;
        p_rrc_paging =  rtxMemAllocType(p_asn1_ctx, Paging);
        if (NULL == p_rrc_paging)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR,
                  "[PCCH_Message] Memory allocation failure");
            break;
        }

        asn1Init_Paging( p_rrc_paging );

        p_pcch_message->message.u.c1->u.paging = p_rrc_paging;

       if ((S32)(p_cell_ctx->cell_reconfig_counter - CELL_RECONFIG_OFFSET) > 0)
       {
           /* Set the SI Modification bit*/
           p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;

           p_pcch_message->message.u.c1->u.paging->systemInfoModification = true_;
           /* The pagingRecordList IE has been already initialized
            * in asn1Init_Paging()*/

       }
       if ((S32)(p_cell_ctx->etws_paging_counter) > PWS_OFFSET)
       {
           /* Set the ETWS bit*/
           p_pcch_message->message.u.c1->u.paging->m.etws_IndicationPresent = 1;

           p_pcch_message->message.u.c1->u.paging->etws_Indication = true__1;
       }
       if ((S32)(p_cell_ctx->cmas_paging_counter) > PWS_OFFSET)
       {
           /* Set the CMAS bit*/
           p_pcch_message->message.u.c1->u.paging->m.nonCriticalExtensionPresent = 1;
           p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.m.\
              nonCriticalExtensionPresent = 1;
           p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
              nonCriticalExtension.m.cmas_Indication_r9Present = 1;
           p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
              nonCriticalExtension.cmas_Indication_r9 = true__2;
       }
        result = RRC_SUCCESS;
    } while (0);

    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_add_paging_record_to_rrc_paging_message
*   INPUT        : OSCTXT                      *p_asn1_ctx
*                  PCCH_Message                *p_pcch_message
*                  csc_paging_record_t         *p_csc_paging_record
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Adds UE Paging record to the RRC Paging message.
******************************************************************************/
rrc_return_t csc_paging_add_paging_record_to_rrc_paging_message
(
    OSCTXT                      *p_asn1_ctx, /* ASN.1 context */
    PCCH_Message                *p_pcch_message,
    csc_paging_record_t         *p_csc_paging_record
)
{
    rrc_return_t    result = RRC_FAILURE;
    OSRTDListNode   *p_node             = PNULL;
    PagingRecord    *p_paging_record    = PNULL;



    RRC_CSC_UT_TRACE_ENTER();

    do {
        rtxDListAllocNodeAndData(
                p_asn1_ctx, PagingRecord, &p_node, &p_paging_record);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR,
                  "[PCCH_Message][PagingRecord] Memory allocation failure");
            break;
        }

        asn1Init_PagingRecord(p_paging_record);

        if (T_s1ap_UEPagingID_s_TMSI == p_csc_paging_record->ue_paging_id.t)
        {
            S_TMSI      *p_s_tmsi = PNULL;
            s1ap_S_TMSI *p_s1ap_s_tmsi =
                            p_csc_paging_record->ue_paging_id.u.s_TMSI;

            p_paging_record->ue_Identity.t = T_PagingUE_Identity_s_TMSI;
            p_s_tmsi = rtxMemAllocType(p_asn1_ctx, S_TMSI);
            if(NULL == p_s_tmsi)
            {
                /* Not enough memory */
                RRC_CSC_TRACE( RRC_ERROR,
                     "[PCCH_Message][PagingRecord] Memory allocation failure");
                break;
            }

            asn1Init_S_TMSI( p_s_tmsi );

            p_paging_record->ue_Identity.u.s_TMSI = p_s_tmsi;

            if (p_s1ap_s_tmsi->mMEC.numocts >
                sizeof(p_s_tmsi->mmec.data)/sizeof(p_s_tmsi->mmec.data[0]))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                      "[PCCH_Message][PagingRecord]"
                      " Too much octets in S1AP MMEC for RRC MMEC");
                break;
            }
            p_s_tmsi->mmec.numbits = 8 * p_s1ap_s_tmsi->mMEC.numocts;
            l3_memcpy_wrapper(p_s_tmsi->mmec.data,
                    p_s1ap_s_tmsi->mMEC.data,
                    sizeof(p_s1ap_s_tmsi->mMEC.data));

            /* Bug in TS 36.413-851! IE m-TMSI M-TMSI is absent in S-TMSI in
             *  ASN.1 description. Fixed in TS 36.413-861. */
            RRC_ASSERT(
                    sizeof(p_s1ap_s_tmsi->m_TMSI.data) ==
                    sizeof(p_s_tmsi->m_TMSI.data));

            if (p_s1ap_s_tmsi->m_TMSI.numocts >
                sizeof(p_s_tmsi->m_TMSI.data)/sizeof(p_s_tmsi->m_TMSI.data[0]))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "[PCCH_Message][PagingRecord]"
                    "Too much octets in S1AP M_TMSI for RRC M_TMSI");
                break;
            }

            p_s_tmsi->m_TMSI.numbits = 8 * p_s1ap_s_tmsi->m_TMSI.numocts;
            l3_memcpy_wrapper(p_s_tmsi->m_TMSI.data,
                       p_s1ap_s_tmsi->m_TMSI.data,
                       sizeof(p_s1ap_s_tmsi->m_TMSI.data));
        }
        else if (T_s1ap_UEPagingID_iMSI == p_csc_paging_record->ue_paging_id.t)
        {
            IMSI      *p_imsi = PNULL;
            s1ap_IMSI *p_s1ap_imsi = p_csc_paging_record->ue_paging_id.u.iMSI;

            p_paging_record->ue_Identity.t = T_PagingUE_Identity_imsi;
            p_imsi = rtxMemAllocType(p_asn1_ctx, IMSI);
            if (NULL == p_imsi)
            {
                /* Not enough memory */
                RRC_CSC_TRACE( RRC_ERROR,
                     "[PCCH_Message][PagingRecord] Memory allocation failure");
                break;
            }

            asn1Init_IMSI( p_imsi );

            p_paging_record->ue_Identity.u.imsi = p_imsi;

            if (p_s1ap_imsi->numocts)
            {
                U32 i = RRC_NULL;
                U8 the_last_digit = RRC_NULL;
                if ( p_s1ap_imsi->numocts  >
                         sizeof(p_s1ap_imsi->data)/sizeof(p_s1ap_imsi->data[0]))
                {
                     RRC_CSC_TRACE(RRC_WARNING,
                         "[PCCH_Message][PagingRecord] "
                         "Number of Octets=%u is exceeding"
                         "S1AP IMSI array boundary.", p_s1ap_imsi->numocts);
                     break;
                } 

                the_last_digit =
                    (p_s1ap_imsi->data[p_s1ap_imsi->numocts - 1] >> 4) & 0xF;

                /* Process full octets */
                for (i = 0; i < p_s1ap_imsi->numocts - 1; i++)
                {
                    p_imsi->elem[p_imsi->n++] =
                        (IMSI_Digit)(p_s1ap_imsi->data[i] & 0xF);

                    /* IMSI SPR FIX START */
                    if (i == 1)
                    {
                        if (((p_s1ap_imsi->data[i]>>4)&0xF) == 0xF)
                        {
                            RRC_CSC_TRACE(RRC_INFO,
                                    "[[PCCH_Message][PagingRecord]"
                                    "Filler Digit in MNC ignoring it");
                            continue;
                        }
                    }
                    /* IMSI SPR FIX STOP */

                    p_imsi->elem[p_imsi->n++] =
                        (IMSI_Digit)((p_s1ap_imsi->data[i] >> 4)&0xF);
                }
                /* Process the last octet */
                p_imsi->elem[p_imsi->n++] =
                    (IMSI_Digit)(p_s1ap_imsi->data[i] & 0xF);
                if (FILLER_DIGIT_IN_S1AP != the_last_digit)
                {
                    p_imsi->elem[p_imsi->n++] = the_last_digit;
                }
            }
        }
        else if (T_s1ap_UEPagingID_extElem1 == p_csc_paging_record->ue_paging_id.t)
        {
            p_paging_record->ue_Identity.t = T_PagingUE_Identity_extElem1;
            p_paging_record->ue_Identity.u.extElem1 =
                rtxMemAllocType(p_asn1_ctx, ASN1OpenType);
            if(NULL == p_paging_record->ue_Identity.u.extElem1)
            {
                /* Not enough memory */
                RRC_CSC_TRACE( RRC_ERROR,
                     "[PCCH_Message][PagingRecord] Memory allocation failure");
                break;
            }
            /* Coverity 114606 Fixed Start */
            if(!rtCopyOpenType(p_asn1_ctx,
                    p_csc_paging_record->ue_paging_id.u.extElem1,
                        p_paging_record->ue_Identity.u.extElem1))
            {
                RRC_CSC_TRACE( RRC_ERROR,
                        "Function rtCopyOpenType Returning Failure");
                break;
            }
            /* Coverity 114606 Fixed End */
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "[PCCH_Message][PagingRecord] "
                "S1AP UE Paging Identity type is unknown [%u]",
                p_csc_paging_record->ue_paging_id.t);
            break;
        }

        p_paging_record->cn_Domain = p_csc_paging_record->cn_domain;

        rtxDListAppendNode(
             &p_pcch_message->message.u.c1->u.paging->pagingRecordList, p_node);

        p_pcch_message->message.u.c1->u.paging->m.pagingRecordListPresent = 1;

        result = RRC_SUCCESS;
    } while (0);

    RRC_CSC_UT_TRACE_EXIT();

    return result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_print_paging_record
*   INPUT        : csc_paging_record_t         *p_csc_paging_record
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : Prints CSC Paging records data.
******************************************************************************/
void csc_paging_print_paging_record(
        csc_paging_record_t         *p_csc_paging_record
)
{
    RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] SFN[%u] SF[%u] ",
            p_csc_paging_record->sfn, p_csc_paging_record->sf);

    /* eMTC changes stop */

    RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] CNDomain\t[0x%02X]",
            p_csc_paging_record->cn_domain);
    RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] PagingDRX\t[0x%02X]",
            p_csc_paging_record->paging_drx);

    if (T_s1ap_UEPagingID_s_TMSI == p_csc_paging_record->ue_paging_id.t)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] "
                "S_TMSI.MMEC   [0x%02X] "
                "S_TMSI.M_TMSI [0x%02X 0x%02X 0x%02X 0x%02X]",
              p_csc_paging_record->ue_paging_id.u.s_TMSI->mMEC.data[0],
              p_csc_paging_record->ue_paging_id.u.s_TMSI->m_TMSI.data[0],
              p_csc_paging_record->ue_paging_id.u.s_TMSI->m_TMSI.data[1],
              p_csc_paging_record->ue_paging_id.u.s_TMSI->m_TMSI.data[2],
              p_csc_paging_record->ue_paging_id.u.s_TMSI->m_TMSI.data[3]);
    }
    else if (T_s1ap_UEPagingID_iMSI == p_csc_paging_record->ue_paging_id.t)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] IMSI "
                "[0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X]",
              p_csc_paging_record->ue_paging_id.u.iMSI->data[0],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[1],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[2],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[3],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[4],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[5],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[6],
              p_csc_paging_record->ue_paging_id.u.iMSI->data[7]);
    }
    else
    {
        RRC_CSC_TRACE(RRC_BRIEF, "[PAGING] Identity unknown");
    }
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_rearrange_sfn
*   INPUT        : rrc_sfn_t     sfn_index
*                  rrc_sfn_t     sfn_end
*                  s1ap_PagingDRX     ue_drx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : Calculates new SFN value during UE Paging record rearranging.
******************************************************************************/
rrc_sfn_t csc_paging_rearrange_sfn(
        rrc_sfn_t     sfn_index,
        rrc_sfn_t     sfn_end,
        s1ap_PagingDRX     ue_drx
)
{
    rrc_sfn_t sfn_result = RRC_NULL;
    U16 n;
    rrc_sfn_t sfn_drx = RRC_NULL;

    if (sfn_end < sfn_index)
    {
        sfn_end = (rrc_sfn_t)(sfn_end + SFN_UPPER_LIMIT);
    }

    /* Convert PagingDRX from enum to SFN */
    sfn_drx = (rrc_sfn_t)(32 << (U16)ue_drx);

    /* calculate n using formula:
     * ((sfn_index + n * ue_drx) > (sfn_mac + CSC_SFN_OFFSET)*/
    n = (U16) (((sfn_end - sfn_index) / sfn_drx) + 1);

    /* calculate new SFN  using formula:
         * sfn_result = (sfn_index + n * ue_drx) */
    sfn_result = (rrc_sfn_t)((sfn_index + n * sfn_drx) % SFN_UPPER_LIMIT);

    return sfn_result;
}

/******************************************************************************
*   FUNCTION NAME: csc_paging_rearrange_ues
*   INPUT        : csc_cell_ctx_t  *p_cell_ctx
*                  U16             sfn_mac
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : Rearranges UE Paging requests in case when CSC current SFN is
*                changed by MAC.
******************************************************************************/
void csc_paging_rearrange_ues(
/*SPR 17777 -*/        
        csc_cell_ctx_t  *p_cell_ctx,
        U16             sfn_mac
)
{
    rrc_sfn_t     sfn_begin = RRC_NULL;
    rrc_sfn_t     sfn_end = RRC_NULL;
    rrc_sfn_t     sfn_index = RRC_NULL;
    rrc_sfn_t     sfn_new = RRC_NULL;
    rrc_sf_t      sf_index = RRC_NULL;
    csc_paging_record_t     *p_paging_record = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    sfn_begin =
        (rrc_sfn_t)((p_cell_ctx->sfn + CSC_SFN_OFFSET + 1) % SFN_UPPER_LIMIT);
    sfn_end = (rrc_sfn_t)((sfn_mac + CSC_SFN_OFFSET) % SFN_UPPER_LIMIT);

    sfn_index = sfn_begin;

    do {
        /* rearrange UE paging requests for sfn_index */

        for(sf_index = 0; sf_index < SF_MAX_LIMIT; sf_index++)
        {
            while (PNULL != (p_paging_record =
                             csc_paging_storage_get(
                                    p_cell_ctx->p_paging_storage,
                                    sfn_index, sf_index)))
            {
                /* Calculate new SFN */
                sfn_new = csc_paging_rearrange_sfn(
                        sfn_index,
                        sfn_end,
                        p_paging_record->paging_drx);

                if(p_paging_record->sfn != sfn_new)
                {
                    /* Update paging record */
                    p_paging_record->sfn = sfn_new;

                    /* Store paging record*/
                    if (RRC_SUCCESS != csc_paging_storage_reinsert(
                                p_cell_ctx->p_paging_storage,
                                /* Bug 11748 Fix Start */
                                p_paging_record, p_cell_ctx->cell_index))
                                /* Bug 11748 Fix Stop */
                    {
                        RRC_CSC_TRACE(RRC_ERROR,
                                "[PAGING] UE Paging record rearranging failed");
                        csc_paging_print_paging_record(p_paging_record
                        );
                        if (PNULL != p_paging_record)
                        {   
                            if(PNULL != g_rrc_stats.p_rrc_stats)
                            {
                                RRC_CSC_UPDATE_PERFORMACE_COUNTER(g_rrc_stats.p_rrc_stats->
                                        rrc_csc_stats.rrc_csc_cell_stats[p_cell_ctx->cell_index].
                                        rrc_paging_related_stats.pag_discardedNbr, 0, 1);
                            }
                            /* free paging record */
                            if (PNULL != p_paging_record->ue_paging_id.u.s_TMSI)
                            {
                              rrc_mem_free(p_paging_record->ue_paging_id.u.s_TMSI);
                              p_paging_record->ue_paging_id.u.s_TMSI = PNULL;
                            }
                            if (PNULL != p_paging_record->ue_paging_id.u.iMSI)
                            {
                              rrc_mem_free(p_paging_record->ue_paging_id.u.iMSI);
                              p_paging_record->ue_paging_id.u.iMSI = PNULL;
                            }
                            if (PNULL != p_paging_record->ue_paging_id.u.extElem1)
                            {
                              if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                              {
                                rrc_mem_free((void *)p_paging_record->ue_paging_id.u.extElem1->data);
                                p_paging_record->ue_paging_id.u.extElem1->data = PNULL;
                              }
                              rrc_mem_free(p_paging_record->ue_paging_id.u.extElem1);
                              p_paging_record->ue_paging_id.u.extElem1 = PNULL;
                            }
                            rrc_mem_free(p_paging_record);
                            p_paging_record = PNULL;
                        }
                    }
                    else
                    {
                        RRC_CSC_TRACE(RRC_BRIEF, "[CELL:%u][PAGING] "
                                "The UE Paging request has been rearranged",
                                p_cell_ctx->cell_index);

                        csc_paging_print_paging_record(p_paging_record
                            );
                    }
                }
                else
                { /* New SFN is same as current SFN for this paging record. So,
		   * this can't be scheduled because MAC SFN is less than 
		   * this SFN. So, dropping this paging record. */
                    RRC_CSC_TRACE(RRC_INFO,
                            "[PAGING] UE Paging record dropped.");

                    csc_paging_print_paging_record(p_paging_record
                        );
                    /*SPR_18230_START*/
                    /* code removed */
                    /*SPR_18230_END*/
                    /* free paging record */
                    if (PNULL != p_paging_record->ue_paging_id.u.s_TMSI)
                    {
                      rrc_mem_free(p_paging_record->ue_paging_id.u.s_TMSI);
                      p_paging_record->ue_paging_id.u.s_TMSI = PNULL;
                    }
                    if (PNULL != p_paging_record->ue_paging_id.u.iMSI)
                    {
                      rrc_mem_free(p_paging_record->ue_paging_id.u.iMSI);
                      p_paging_record->ue_paging_id.u.iMSI = PNULL;
                    }
                    if (PNULL != p_paging_record->ue_paging_id.u.extElem1)
                    {
                      if (PNULL != p_paging_record->ue_paging_id.u.extElem1->data)
                      {
                        rrc_mem_free((void *)p_paging_record->ue_paging_id.u.extElem1->data);
                        p_paging_record->ue_paging_id.u.extElem1->data = PNULL;
                      }
                      rrc_mem_free(p_paging_record->ue_paging_id.u.extElem1);
                      p_paging_record->ue_paging_id.u.extElem1 = PNULL;
                    }
                    rrc_mem_free(p_paging_record);
                    p_paging_record = PNULL;
                }
            }
        }

        /* Finish rearranging */
        if (sfn_index == sfn_end)
        {
            break;
        }

        /* go to the next SFN*/
        sfn_index = (rrc_sfn_t)((sfn_index + 1) % SFN_UPPER_LIMIT);

    } while (1);

    RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
  *   FUNCTION NAME: csc_paging_build_and_send_si_paging_msg
  *   INPUT        : rrc_sfn_t    sfn
  *   OUTPUT       : none
  *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
  *   DESCRIPTION  : Prepares Paging message due to change in System Info (Modification).
  ***************************************************************************/

rrc_return_t csc_paging_build_and_send_si_paging_msg(
    rrc_sfn_t    sfn
)
{
    OSCTXT          *p_asn1_ctx = PNULL;
    PCCH_Message    *p_pcch_message = PNULL;


    Paging          *p_si_paging       = PNULL;
    U8      *p_encoded_rrc_paging_msg = PNULL;
    void    *p_intrl_msg = PNULL;
    rrc_csc_llim_pcch_data_req_t *p_csc_llim_si_mod_pcch_data = PNULL;

    PCCH_Message    pcch_message ;
    OSCTXT          asn1_ctx ;
    OSINT32 encoded_rrc_paging_msg_buffer_length = RRC_NULL;
    rrc_return_t    result = RRC_FAILURE;
    Paging_systemInfoModification     paging_si_mod =RRC_NULL;

    p_pcch_message = &pcch_message;
    p_asn1_ctx     = &asn1_ctx;


    RRC_CSC_TRACE(RRC_INFO,"Sending SI Paging API at SFN = %d",sfn);

    RRC_CSC_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR,
                "[ SI Modification PAGING] ASN1 context initialization failed.");
        return result;
    }
    do {
        asn1Init_PCCH_Message(p_pcch_message);

        /* PCCH_MessageType_c1 */
        p_pcch_message->message.t = T_PCCH_MessageType_c1;
        p_pcch_message->message.u.c1 = rtxMemAllocType(p_asn1_ctx,
                PCCH_MessageType_c1);
        if (NULL == p_pcch_message->message.u.c1)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR, "[PCCH_Message]Memory allocation failure");
            break;
        }
        asn1Init_PCCH_MessageType_c1( p_pcch_message->message.u.c1 );

        /* Paging */
        p_pcch_message->message.u.c1->t = T_PCCH_MessageType_c1_paging;

        p_si_paging =  rtxMemAllocType(p_asn1_ctx, Paging);
        if (NULL == p_si_paging)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR,"[PCCH_Message] Memory allocation failure");
            break;
        }
        asn1Init_Paging( p_si_paging );

        p_pcch_message->message.u.c1->u.paging = p_si_paging;

        /* Set the SI Modification bit*/
        paging_si_mod = true_;
        p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;

        p_pcch_message->message.u.c1->u.paging->systemInfoModification = paging_si_mod;

        pu_setBuffer(p_asn1_ctx, 0, 0, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_PCCH_Message(p_asn1_ctx,p_pcch_message))
        {
            RRC_CSC_TRACE(RRC_WARNING,"[PCCH_Message] ASN1 encoding failed");
            break;
        }

        p_encoded_rrc_paging_msg = pe_GetMsgPtr(p_asn1_ctx,&encoded_rrc_paging_msg_buffer_length);
        if (PNULL == p_encoded_rrc_paging_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,"[PCCH_Message]ASN.1 failure");
            break;
        }
       /* contela issue fix */
       /* contela issue fix */

        /* Prepare message to LLIM */
        p_intrl_msg = csc_alloc_intrl_msg(RRC_LLIM_MODULE_ID,CSC_LLIM_PCCH_DATA_REQ,
                (U16)(sizeof(rrc_csc_llim_pcch_data_req_t) + (U16)encoded_rrc_paging_msg_buffer_length));
        if (PNULL == p_intrl_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,"[CSC_LLIM_PCCH_DATA_REQ]Memory allocation failure");
            break;
        }
        /* Fill and send message */
        p_csc_llim_si_mod_pcch_data = (rrc_csc_llim_pcch_data_req_t *)((U8 *)p_intrl_msg + 
                RRC_API_HEADER_SIZE);

        /* Fill LLIM PCCH message */
        p_csc_llim_si_mod_pcch_data->paging_frame = (rrc_sfn_t)((sfn + CSC_SFN_OFFSET) % 
                SFN_UPPER_LIMIT);
        p_csc_llim_si_mod_pcch_data->paging_sub_frame = 0;

        p_csc_llim_si_mod_pcch_data->paging_msg_buffer_length = 
            (U16)encoded_rrc_paging_msg_buffer_length;
        l3_memcpy_wrapper(p_csc_llim_si_mod_pcch_data->paging_msg_buffer,p_encoded_rrc_paging_msg,
                (size_t)encoded_rrc_paging_msg_buffer_length);
        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);        

        result = RRC_SUCCESS;
    } while (0);
    /*+ SPR 22111 Fix*/
    if(PNULL != p_pcch_message->message.u.c1)
    {
        rtxMemFreeType(p_asn1_ctx,p_pcch_message->message.u.c1);
    }
    rtFreeContext(&asn1_ctx);  
    /*- SPR 22111 Fix*/
    RRC_CSC_UT_TRACE_EXIT();
    return result;                             
}

/*****************************************************************************
  *   FUNCTION NAME: csc_paging_build_and_send_broadcast_info
  *   INPUT        : rrc_sfn_t    sfn
  *                  U8          paging_type
  *                  csc_cell_ctx_t  *p_cell_ctx
  *   OUTPUT       :  none
  *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
  *   DESCRIPTION  : Prepares Paging message due to change in either 
  *                  --System Info (Modification).
  *                  --ETWS
  *                  --CMAS
  *                  --combination of them (SI+ETWS, SI+CMAS, ETWS+CMAS, SI+ETWS+CMAS)
  *
  ***************************************************************************/

void csc_paging_build_and_send_broadcast_info(
    rrc_sfn_t    sfn,
    U8          paging_type,
    csc_cell_ctx_t  *p_cell_ctx
)
{
    OSCTXT          *p_asn1_ctx = PNULL;
    PCCH_Message    *p_pcch_message = PNULL;


    Paging          *p_paging_msg       = PNULL;
    U8      *p_encoded_rrc_paging_msg = PNULL;
    void    *p_intrl_msg = PNULL;
    rrc_csc_llim_pcch_data_req_t *p_csc_llim_paging_data = PNULL;

    PCCH_Message    pcch_message ;
    OSCTXT          asn1_ctx ;
    OSINT32 encoded_rrc_paging_msg_buffer_length = RRC_NULL;
    /* Declaring SI and PWS paging records*/
    Paging_systemInfoModification         paging_si_mod = RRC_NULL;
    Paging_etws_Indication                paging_etws = RRC_NULL;

    p_pcch_message = &pcch_message;
    p_asn1_ctx     = &asn1_ctx;


    RRC_CSC_TRACE(RRC_INFO,"Broadcasting Paging API at SFN = %d",sfn);

    RRC_CSC_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR,
                "[ Broadcasting PAGING] ASN1 context initialization failed.");
        return;
    }
    do {
        asn1Init_PCCH_Message(p_pcch_message);

        /* PCCH_MessageType_c1 */
        p_pcch_message->message.t = T_PCCH_MessageType_c1;
        p_pcch_message->message.u.c1 = rtxMemAllocType(p_asn1_ctx,
                PCCH_MessageType_c1);
        if (NULL == p_pcch_message->message.u.c1)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR, "[PCCH_Message]Memory allocation failure");
            break;
        }
        asn1Init_PCCH_MessageType_c1( p_pcch_message->message.u.c1 );

        /* Paging */
        p_pcch_message->message.u.c1->t = T_PCCH_MessageType_c1_paging;

        p_paging_msg =  rtxMemAllocType(p_asn1_ctx, Paging);
        if (NULL == p_paging_msg)
        {
            /* Not enough memory */
            RRC_CSC_TRACE( RRC_ERROR,"[PCCH_Message] Memory allocation failure");
            break;
        }
        asn1Init_Paging( p_paging_msg );

        p_pcch_message->message.u.c1->u.paging = p_paging_msg;

        if(PAGE_SI == paging_type)
        {
            /* Set the SI Modification bit*/
            paging_si_mod = true_;
            p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->systemInfoModification = paging_si_mod;
        }
        else if(PAGE_ETWS == paging_type)
        {
            /* Set ETWS bit*/
            paging_etws = true__1;
            p_pcch_message->message.u.c1->u.paging->m.etws_IndicationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->etws_Indication = paging_etws;
        }
        else if(PAGE_CMAS == paging_type)
        {
            /* Set CMAS bit*/
            p_pcch_message->message.u.c1->u.paging->m.nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.m.\
               nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.m.cmas_Indication_r9Present = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.cmas_Indication_r9 = true__2;
        }
        else if(PAGE_SI_ETWS == paging_type)
        {
            /* Set ETWS bit*/
            paging_etws = true__1;
            p_pcch_message->message.u.c1->u.paging->m.etws_IndicationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->etws_Indication = paging_etws;

            /* Set SI  bit*/
            paging_si_mod = true_;
            p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->systemInfoModification = paging_si_mod;
        }
        else if(PAGE_SI_CMAS == paging_type)
        {
            /* Set CMAS bit*/
            p_pcch_message->message.u.c1->u.paging->m.nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.m.\
               nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.m.cmas_Indication_r9Present = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.cmas_Indication_r9 = true__2;

            /* Set SI  bit*/
            paging_si_mod = true_;
            p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->systemInfoModification = paging_si_mod;
        }
        else if(PAGE_ETWS_CMAS == paging_type)
        {
            /* Set ETWS bit*/
            paging_etws = true__1;
            p_pcch_message->message.u.c1->u.paging->m.etws_IndicationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->etws_Indication = paging_etws;
            
            /* Set CMAS bit*/
            p_pcch_message->message.u.c1->u.paging->m.nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.m.\
               nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.m.cmas_Indication_r9Present = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.cmas_Indication_r9 = true__2;
        }
        else if(PAGE_SI_ETWS_CMAS == paging_type)
        {
            /* Set ETWS bit*/
            paging_etws = true__1;
            p_pcch_message->message.u.c1->u.paging->m.etws_IndicationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->etws_Indication = paging_etws;
            
            /* Set CMAS bit*/
            p_pcch_message->message.u.c1->u.paging->m.nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.m.\
               nonCriticalExtensionPresent = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.m.cmas_Indication_r9Present = 1;
            p_pcch_message->message.u.c1->u.paging->nonCriticalExtension.\
               nonCriticalExtension.cmas_Indication_r9 = true__2;

            /* Set SI  bit*/
            paging_si_mod = true_;
            p_pcch_message->message.u.c1->u.paging->m.systemInfoModificationPresent = 1;
            p_pcch_message->message.u.c1->u.paging->systemInfoModification = paging_si_mod;
        }

        pu_setBuffer(p_asn1_ctx, 0, 0, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_PCCH_Message(p_asn1_ctx,p_pcch_message))
        {
            RRC_CSC_TRACE(RRC_WARNING,"[PCCH_Message] ASN1 encoding failed");
            break;
        }

        p_encoded_rrc_paging_msg = pe_GetMsgPtr(p_asn1_ctx,&encoded_rrc_paging_msg_buffer_length);
        if (PNULL == p_encoded_rrc_paging_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,"[PCCH_Message]ASN.1 failure");
            break;
        }

        /* contela issue fix */
        /* contela issue fix */

        /* Prepare message to LLIM */
        p_intrl_msg = csc_alloc_intrl_msg(RRC_LLIM_MODULE_ID,CSC_LLIM_PCCH_DATA_REQ,
                (U16)(sizeof(rrc_csc_llim_pcch_data_req_t) + (U16)encoded_rrc_paging_msg_buffer_length));
        if (PNULL == p_intrl_msg)
        {
            RRC_CSC_TRACE( RRC_ERROR,"[CSC_LLIM_PCCH_DATA_REQ]Memory allocation failure");
            break;
        }
        /* Fill and send message */
        p_csc_llim_paging_data = (rrc_csc_llim_pcch_data_req_t *)((U8 *)p_intrl_msg + 
                RRC_API_HEADER_SIZE);

        /* Fill LLIM PCCH message */
        if((PAGE_SI == paging_type) || (PAGE_SI_ETWS == paging_type) ||
           (PAGE_SI_CMAS == paging_type) || (PAGE_SI_ETWS_CMAS == paging_type))
        {
            p_csc_llim_paging_data->transaction_id = 
                p_cell_ctx->transaction_id; /* Valid transaction Id is set only 
                                             * in case of SI modification so 
                                             * that on receiving SFN ERROR IND
                                             * from MAC, Cell reconfiguration
                                             * can be stopped.
                                             */
            p_csc_llim_paging_data->cell_index = p_cell_ctx->cell_index;
            p_csc_llim_paging_data->paging_frame = (rrc_sfn_t)((sfn + CSC_SFN_OFFSET) % 
                    SFN_UPPER_LIMIT);
            p_csc_llim_paging_data->paging_sub_frame = 0;
            /* SPR 22994 fix start */
            /* SPR 22994 fix stop */

            p_csc_llim_paging_data->num_of_sfn = (U16)p_cell_ctx->mod_period;
            /* eMTC changes stop*/
        }
        else if((PAGE_ETWS == paging_type) || (PAGE_CMAS == paging_type) ||
                (PAGE_ETWS_CMAS == paging_type))
        {
            p_csc_llim_paging_data->cell_index = p_cell_ctx->cell_index;
            p_csc_llim_paging_data->transaction_id = RRC_TRANSACTION_ID_ABSENT;
            p_csc_llim_paging_data->paging_frame = (rrc_sfn_t)((sfn + CSC_SI_PWS_SFN_OFFSET) % 
                    SFN_UPPER_LIMIT);
            p_csc_llim_paging_data->paging_sub_frame = 0;
            p_csc_llim_paging_data->num_of_sfn = (U16)((p_cell_ctx->mod_period + 
                p_cell_ctx->mod_period_boud_counter) - CSC_SI_PWS_SFN_OFFSET);
            if (p_csc_llim_paging_data->num_of_sfn < p_cell_ctx->mod_period)
            {
                p_csc_llim_paging_data->num_of_sfn = (U16)(p_csc_llim_paging_data->num_of_sfn +
                                                         p_cell_ctx->mod_period); 
            }    
        }
        p_csc_llim_paging_data->paging_msg_buffer_length = 
            (U16)encoded_rrc_paging_msg_buffer_length;
        l3_memcpy_wrapper(p_csc_llim_paging_data->paging_msg_buffer,p_encoded_rrc_paging_msg,
                (size_t)encoded_rrc_paging_msg_buffer_length);
        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);        
        /*+ SPR 22111 Fix*/
        /* Code Moved */
        /*- SPR 22111 Fix*/
        /*SPR_18125_START*/
        /* Code Moved */
        /*SPR_18125_END*/


    } while (0);

    /*+ SPR 22111 Fix*/
    if(PNULL != p_paging_msg)
    {
        rtxMemFreeType(p_asn1_ctx,p_paging_msg);
    }
    if(PNULL != p_pcch_message->message.u.c1)
    {
        rtxMemFreeType(p_asn1_ctx,p_pcch_message->message.u.c1);
    }
    /*- SPR 22111 Fix*/
    /*SPR_18125_START*/
    rtFreeContext(p_asn1_ctx);
    /*SPR_18125_END*/
    RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
  *   FUNCTION NAME: csc_delete_low_priority_paging_record
  *   INPUT        : csc_paging_queue_t    *p_paging_queue
  *                  csc_paging_record_t   *p_paging_record
  *   OUTPUT       : none
  *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
  *   DESCRIPTION  : Find and deletes paging recored with least priority in Paging Queue
  ***************************************************************************/
rrc_return_t csc_delete_low_priority_paging_record
( 
        csc_paging_queue_t    *p_paging_queue,
        csc_paging_record_t   *p_paging_record 
)
{
	YLNODE	*p= PNULL;

    csc_paging_record_t   *p_lowest_priority_paging_record = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(p_paging_queue != PNULL);
    RRC_ASSERT(p_paging_record != PNULL);

    for( p = ylFirst(p_paging_queue); p; p = ylNext(p) ) 
    {
        /* find out the Least Priority paging */
        if (!((csc_paging_record_t*)p)->bitmask & PAGING_RECORD_PAGING_PRIORITY_PRESENT)
        {
            /*Least Priority Paging with out Priority Indication */
            ylDelete(p_paging_queue, p);
            RRC_CSC_TRACE(RRC_WARNING,"Deleting Paging Record without Prority");
            return RRC_SUCCESS;

        }
        if ( p_lowest_priority_paging_record == PNULL || ((csc_paging_record_t*)p)->paging_priority > 
                p_lowest_priority_paging_record->paging_priority)
        {
            p_lowest_priority_paging_record = (csc_paging_record_t*)p;
        }
    }
    /* klocwork fix */  
    if ((PNULL !=  p_lowest_priority_paging_record) && 
        (p_lowest_priority_paging_record->paging_priority > p_paging_record->paging_priority))
    {
	    ylDelete(p_paging_queue, (YLNODE*)p_lowest_priority_paging_record);
	    RRC_CSC_TRACE(RRC_WARNING,"Deleting Paging Rcord with priority: %d",
			    p_lowest_priority_paging_record->paging_priority);

	    return RRC_SUCCESS;
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_FAILURE;
}

