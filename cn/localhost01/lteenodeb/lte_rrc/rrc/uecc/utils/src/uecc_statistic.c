/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_statistic.c,v 1.3 2010/03/24 09:50:44 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC statistic
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_statistic.c,v $
 * Revision 1.3  2010/03/24 09:50:44  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.3  2009/12/26 12:10:10  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.2  2009/12/08 16:06:02  ukr15916
 * LG_35 fixed
 *
 * Revision 1.1.6.1  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.14  2009/11/24 12:12:52  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.13  2009/11/24 11:32:30  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.12.2.3  2009/11/24 08:33:15  ukr18880
 * comments changed
 *
 * Revision 1.1.4.12.2.2  2009/11/24 08:29:05  ukr15916
 * Reviewed
 *
 *
 ****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "uecc_statistic.h"
#include "uecc_logger.h"
#include "uecc_global_ctx.h"
#include "uecc_common.h"

/******************************************************************************
 * Private variable definitions
 *****************************************************************************/

/******************************************************************************
 * Private functions definitions
 *****************************************************************************/
static void uecc_status_cell_update_num_of_srb1(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_srb
);

static void uecc_status_cell_update_num_of_srb2(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_srb2
);

static void uecc_status_cell_update_num_of_drb(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_drb
);


/******************************************************************************
 * Function implementation
 *****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_engine_init
*
*   INPUT:         rrc_csc_context_t  *p_csc_context,
*                  uecc_gb_context_t*  p_uecc_gb_context
*
*   OUTPUT:        None
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
*   DESCRIPTION:   Inits statistic engine after global context creation.
*
******************************************************************************/
rrc_return_et uecc_status_cell_engine_init(
    rrc_csc_context_t  *p_csc_context,
    uecc_gb_context_t*  p_uecc_gb_context)
{
    rrc_return_et           result = RRC_FAILURE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_csc_context);

    do
    {
        p_csc_context->p_rrc_uecc_statistic =
            (rrc_uecc_statistic_t*)rrc_mem_get(sizeof(rrc_uecc_statistic_t));

        if(PNULL == p_csc_context->p_rrc_uecc_statistic)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "uecc_statistic_engine_init: memory allocation failed.");
            break;
        }

        memset_wrapper(p_csc_context->p_rrc_uecc_statistic, 0,
            sizeof(rrc_uecc_statistic_t));

        /* Set the system time */
        RRC_GB_START_TIME_PERFORMANCE_COUNTER(p_uecc_gb_context,
                p_csc_context->p_rrc_uecc_statistic->
                rrc_timer_statistic[RRC_GB_AVG_NUM_SIM_ERABS], RRC_GB_AVG_NUM_SIM_ERABS);

        result = RRC_SUCCESS;
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}


/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_init
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Init all status data in specified UE
*
******************************************************************************/
void uecc_status_ue_init(
    uecc_ue_context_t*  p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->ue_status.ue_num_of_srb1 = 0;
    p_ue_context->ue_status.ue_num_of_srb2 = 0;
    p_ue_context->ue_status.ue_num_of_drb = 0;
    p_ue_context->ue_status.ue_state = ECM_IDLE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_release
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Releases statistic for UE
*
******************************************************************************/
void uecc_status_ue_release(
    uecc_ue_context_t*  p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_status_cell_update_num_of_srb1(
            p_ue_context,
            (S8)(-p_ue_context->ue_status.ue_num_of_srb1) );

    uecc_status_cell_update_num_of_srb2(
            p_ue_context,
            (S8)(-p_ue_context->ue_status.ue_num_of_srb2) );

    uecc_status_cell_update_num_of_drb(
            p_ue_context,
            (S8)(-p_ue_context->ue_status.ue_num_of_drb) );

    p_ue_context->ue_status.ue_num_of_srb1 = 0;
    p_ue_context->ue_status.ue_num_of_srb2 = 0;
    p_ue_context->ue_status.ue_num_of_drb = 0;
    p_ue_context->ue_status.ue_state = ECM_IDLE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_update_num_of_ue
*
*   INPUT:         uecc_gb_context_t*      p_uecc_gb_context,
*                  rrc_uecc_statistic_t*   p_rrc_uecc_statistic,
*                  rrc_bool_et             is_incr
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of UE for CELL
*
* ******************************************************************************/
void uecc_status_cell_update_num_of_ue(
        uecc_gb_context_t*      p_uecc_gb_context,
        rrc_uecc_statistic_t*   p_rrc_uecc_statistic,
        rrc_bool_et             is_incr
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_rrc_uecc_statistic);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",
                p_uecc_gb_context);/*SPR 17777 +-*/
    if(RRC_TRUE == is_incr)
    {
        p_rrc_uecc_statistic->rrc_cell_status.rrc_num_of_ue_per_cell++;
    }
    else
    {
        p_rrc_uecc_statistic->rrc_cell_status.rrc_num_of_ue_per_cell--;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_srb1
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of SRB1 for specified UE
*
******************************************************************************/
void uecc_status_ue_update_num_of_srb1(
    uecc_ue_context_t*  p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->ue_status.ue_num_of_srb1++;
    uecc_status_cell_update_num_of_srb1(p_ue_context, 1);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_srb2
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of SRB2 for specified UE
*
******************************************************************************/
void uecc_status_ue_update_num_of_srb2(
    uecc_ue_context_t*  p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->ue_status.ue_num_of_srb2++;
    uecc_status_cell_update_num_of_srb2(p_ue_context, 1);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_drb
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*                  S8                  count_of_drb
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of DRB for specified UE
*
******************************************************************************/
void uecc_status_ue_update_num_of_drb(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_drb
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if( ((S8)p_ue_context->ue_status.ue_num_of_drb)+count_of_drb >= 0 )
    {
        p_ue_context->ue_status.ue_num_of_drb=
            (U8)(p_ue_context->ue_status.ue_num_of_drb + count_of_drb);
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                "%s:(ue_num_of_drb=%u)+(count_of_drb=%i)<0",
                __FUNCTION__,
                p_ue_context->ue_status.ue_num_of_drb,
                count_of_drb);
    }

    uecc_status_cell_update_num_of_drb(p_ue_context,count_of_drb);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_update_num_of_srb1
*
*   INPUT:         uecc_ue_context_t*  p_ue_context,
*                  S8                  count_of_srb
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of SRB1 for CELL
*
******************************************************************************/
void uecc_status_cell_update_num_of_srb1(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_srb
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (PNULL != p_ue_context->p_gb_context)
    {
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index]->p_rrc_uecc_statistic)
                {
                    if( ((S32)p_ue_context->p_gb_context->p_p_csc_context
                                [p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                rrc_cell_status.rrc_num_of_srb1_per_cell)+count_of_srb >= 0 )
                    {
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_rrc_uecc_statistic->rrc_cell_status.rrc_num_of_srb1_per_cell =
                            (U32)(((S32)p_ue_context->p_gb_context->p_p_csc_context
                                        [p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                        rrc_cell_status.rrc_num_of_srb1_per_cell) + count_of_srb);
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                                "%s:(rrc_num_of_srb1_per_cell=%u)+(count_of_srb=%i)<0",
                                __FUNCTION__,
                                p_ue_context->p_gb_context->p_p_csc_context
                                [p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                rrc_cell_status.rrc_num_of_srb1_per_cell,
                                count_of_srb);
                    }
                }
            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_update_num_of_srb2
*
*   INPUT:         uecc_ue_context_t*  p_ue_context,
*                  S8                  count_of_srb
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of SRB2 for CELL
*
******************************************************************************/
void uecc_status_cell_update_num_of_srb2(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_srb
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if (PNULL != p_ue_context->p_gb_context)
    {
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index]->p_rrc_uecc_statistic)
                {
                    if( ((S32)p_ue_context->p_gb_context->p_p_csc_context
                                [p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                rrc_cell_status.rrc_num_of_srb2_per_cell)+count_of_srb >= 0 )
                    {
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_rrc_uecc_statistic->rrc_cell_status.rrc_num_of_srb2_per_cell =
                            (U32)(((S32)p_ue_context->p_gb_context->p_p_csc_context
                                        [p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                        rrc_cell_status.rrc_num_of_srb2_per_cell) + count_of_srb);
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                                "%s:(rrc_num_of_srb1_per_cell=%u)+(count_of_srb=%i)<0",
                                __FUNCTION__,
                                p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_rrc_uecc_statistic->rrc_cell_status.rrc_num_of_srb2_per_cell,
                                count_of_srb);
                    }
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_update_num_of_drb
*
*   INPUT:         uecc_ue_context_t*  p_ue_context,
*                  S8                  count_of_drb
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   Updates Number of DRB for CELL
*
******************************************************************************/
void uecc_status_cell_update_num_of_drb(
    uecc_ue_context_t*  p_ue_context,
    S8                  count_of_drb
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (PNULL != p_ue_context->p_gb_context)
    {
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index]->p_rrc_uecc_statistic)
                {
                    if( ((S32)p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                rrc_cell_status.rrc_num_of_drb_per_cell)+count_of_drb >= 0 )
                    {
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic->rrc_cell_status.
                            rrc_num_of_drb_per_cell =
                            (U32)(((S32)p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                        rrc_cell_status.rrc_num_of_drb_per_cell) + count_of_drb);
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                                "%s:(rrc_num_of_drb_per_cell=%u)+(count_of_drb=%i)<0",
                                __FUNCTION__,
                                p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic->
                                rrc_cell_status.rrc_num_of_drb_per_cell,
                                count_of_drb);
                    }
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

