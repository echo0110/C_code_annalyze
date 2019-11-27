/******************************************************************************
*
*   FILE NAME:
*       uecc_event_queue.c
*
*   DESCRIPTION:
*       This is the UECC Even Queue implementation file.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   21 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "rrc_common_utils.h"
#include "uecc_event_queue.h"
#include "uecc_ue_ctx.h"
#include "uecc_global_ctx.h"
#include "uecc_logger.h"
/*SPR_17976_START*/
#include "rrc_msg_mgmt.h"
/*SPR_17976_END*/
/****************************************************************************
 * Function implementation
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_event_queue_init
*
*   INPUT:         uecc_event_queue_t      *p_queue   
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function initializes Event queue.
*
******************************************************************************/
void uecc_event_queue_init(
        uecc_event_queue_t      *p_queue /* Pointer on Event queue */
)
{
    ylInit(p_queue);
}

/******************************************************************************
*   FUNCTION NAME: uecc_event_queue_push_event
*
*   INPUT:         uecc_fsm_event_et   uecc_event_id, 
                   uecc_ue_context_t*  p_ue_context,  
                   void*               p_msg_info 
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function allocates memory for uecc_event_t and push it into the
*                  Event queue.
*
******************************************************************************/
void uecc_event_queue_push_event(
    uecc_fsm_event_et   uecc_event_id, /* event id */
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_msg_info     /* Pointer on Event data */
)
{
    uecc_event_t    *event_p = PNULL;

    event_p = (uecc_event_t*)rrc_mem_get(sizeof(uecc_event_t));

    if (PNULL == event_p)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                "uecc_event_queue_push_event: memory allocation failed");
        qvPanic("uecc_event_queue_push_event: memory allocation failed");
    }
    else
    {
        event_p->event_id = uecc_event_id;
        event_p->p_ue_context = p_ue_context;
        event_p->p_msg_info = p_msg_info;

        ylPushTail(p_ue_context->p_gb_context->p_event_queue,
            (uecc_event_queue_node_t*)event_p);
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_event_queue_pop_event
*
*   INPUT:         uecc_event_queue_t* p_queue
*
*   OUTPUT:        None
*
*   RETURNS:       Pointer on UECC FSM Event uecc_event_t.
*
*   DESCRIPTION:   This function pops the event queue
*
******************************************************************************/
uecc_event_t* uecc_event_queue_pop_event(
        uecc_event_queue_t* p_queue     /* Pointer on Event queue */
)
{
    uecc_event_t    *event_p = PNULL;

    event_p = (uecc_event_t*)ylPopHead(p_queue);

    return event_p;
}

/*SPR_16416_START*/
/******************************************************************************
*   FUNCTION NAME: uecc_event_queue_push_ul_nas_transport_event
*
*   INPUT:         uecc_fsm_event_et   uecc_event_id, 
                   uecc_ue_context_t*  p_ue_context,  
                   void*               p_msg_info 
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function allocates memory for uecc_event_t and push it 
*                  into the UL NAS Transport Event queue.
******************************************************************************/
void uecc_event_queue_push_ul_nas_transport_event(
    uecc_fsm_event_et   uecc_event_id, /* event id */
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    DedicatedInfoNAS    *p_dedicatedInfoNAS,
    uecc_event_queue_t* p_queue        /* Pointer on Event queue */
)
{
    uecc_event_t        *event_p = PNULL;
    DedicatedInfoNAS    *p_dedicated_ul_nas_info = PNULL;

    event_p = (uecc_event_t*)rrc_mem_get(sizeof(uecc_event_t));

    if (PNULL == event_p)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                "uecc_event_queue_push_ul_nas_transport_event: "
                "memory allocation failed for event_p");
    }
    else
    {
        p_dedicated_ul_nas_info = rrc_mem_get(sizeof(DedicatedInfoNAS));
        if (PNULL == p_dedicated_ul_nas_info)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                    "uecc_event_queue_push_ul_nas_transport_event: "
                    "memory allocation failed for p_dedicated_ul_nas_info");
            /*Coverity 94803 fix start*/
            rrc_mem_free(event_p);
            event_p = PNULL;
            /*Coverity 94803 fix end*/
        }
        else
        {
            p_dedicated_ul_nas_info->numocts = p_dedicatedInfoNAS->numocts;
            p_dedicated_ul_nas_info->data = rrc_mem_get(p_dedicatedInfoNAS->numocts);
            if (PNULL == p_dedicated_ul_nas_info->data)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                        "uecc_event_queue_push_ul_nas_transport_event: "
                        "memory allocation failed for p_dedicated_ul_nas_info->data");
                /*Coverity 94800 fix start*/
                rrc_mem_free(p_dedicated_ul_nas_info);
                p_dedicated_ul_nas_info = PNULL;
                rrc_mem_free(event_p);
                event_p = PNULL;
                /*Coverity 94800 fix end*/
            }
            else
            {
                l3_memcpy_wrapper((U8 *)(p_dedicated_ul_nas_info->data), 
                    p_dedicatedInfoNAS->data, 
                    p_dedicatedInfoNAS->numocts);

                event_p->event_id = uecc_event_id;
                event_p->p_ue_context = p_ue_context;
                event_p->p_msg_info = p_dedicated_ul_nas_info;
                ylPushTail(p_queue, (uecc_event_queue_node_t*)event_p);
            }
        }
    }
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_delete_ul_nas_transport_queue
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_event_queue_t*     p_uecc_event_queue
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function deletes UECC UL NAS event queue
 *
 *******************************************************************/
void uecc_delete_ul_nas_transport_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
)
{
    uecc_event_t*       p_event = PNULL;
    DedicatedInfoNAS    *p_dedicated_ul_nas_info = PNULL;

    RRC_ASSERT(PNULL != p_uecc_event_queue);
    
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,p_uecc_ue_context->p_gb_context->facility_name
            , RRC_DETAILEDALL, "Unused variable p_uecc_ue_context=%p",
                p_uecc_ue_context);/*SPR 17777 +-*/

    while ( PNULL!=(p_event = uecc_event_queue_pop_event(p_uecc_event_queue)) )
    {
        /* Release the memory allocated for UL NAS message
         * in function uecc_event_queue_push_ul_nas_transport_event */
        p_dedicated_ul_nas_info = p_event->p_msg_info;
        if(PNULL != p_dedicated_ul_nas_info)
        {
            if(PNULL != p_dedicated_ul_nas_info->data)
            {
                rrc_mem_free((void *)(p_dedicated_ul_nas_info->data));
                p_dedicated_ul_nas_info->data = PNULL;
            }
            rrc_mem_free(p_dedicated_ul_nas_info);
            p_dedicated_ul_nas_info = PNULL;
            p_event->p_msg_info = PNULL;
        }

        /* Free event */
        rrc_mem_free(p_event);
        p_event = PNULL;
    }
}
/*SPR_16416_END*/

/*SPR_17976_START*/
/******************************************************************************
*   FUNCTION NAME: uecc_event_queue_push_tenb_cre_event
*
*   INPUT:         uecc_fsm_event_et   uecc_event_id, 
                   uecc_ue_context_t*  p_ue_context,  
                   void*               p_msg_info 
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function allocates memory for uecc_event_t and push
*                  into it the HO specific event receiving at TeNB during 
*                  re-establishment procedure.
******************************************************************************/
void uecc_event_queue_push_tenb_cre_event(
    uecc_fsm_event_et   uecc_event_id, /* event id */
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void                *p_api,             /* Input API buffer */
    uecc_event_queue_t* p_queue        /* Pointer on Event queue */
)
{
    uecc_event_t        *event_p = PNULL;
    void                *p_api_queue = PNULL;
    /* Coverity 99887 Fix Start */
	U16                 buf_size = 0;
    /* Coverity 99887 Fix End */

    event_p = (uecc_event_t*)rrc_mem_get(sizeof(uecc_event_t));

    if (PNULL == event_p)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                "uecc_event_queue_push_tenb_cre_event: "
                "memory allocation failed for event_p");
    }
    else
    {
        /* Coverity 99887 Fix Start */
        buf_size = rrc_get_api_buf_size(p_api);
        p_api_queue = rrc_mem_get(buf_size);
        /* Coverity 99887 Fix End */
        if (PNULL == p_api_queue)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                    "uecc_event_queue_push_tenb_cre_event: "
                    "memory allocation failed");
           event_p->p_msg_info = PNULL;
        }
        else
        {
           /* Coverity 99887 Fix Start */
           l3_memcpy_wrapper((void *)p_api_queue, (const void *)p_api, buf_size);
           /* Coverity 99887 Fix End */

           event_p->event_id = uecc_event_id;
           event_p->p_ue_context = p_ue_context;
           event_p->p_msg_info = p_api_queue;
           ylPushTail(p_queue, (uecc_event_queue_node_t*)event_p);

           RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                   p_ue_context->p_gb_context->facility_name,RRC_INFO,
                   "uecc_event_queue_push_tenb_cre_event: "
                   "Queued Event:%u for UE:%u", 
                   uecc_event_id, p_ue_context->ue_index);
        }
    }
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_delete_tenb_cre_queue
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_event_queue_t*     p_uecc_event_queue
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function deletes TeNB queue used for
 *                  storing HO specific events during re-establishment.
 *
 *******************************************************************/
void uecc_delete_tenb_cre_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
)
{
    uecc_event_t*       p_event = PNULL;

    RRC_ASSERT(PNULL != p_uecc_event_queue);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
            "uecc_delete_tenb_cre_queue: "
            "Deleting queue for UE:%u", 
            p_uecc_ue_context->ue_index);

    while ( PNULL!=(p_event = uecc_event_queue_pop_event(p_uecc_event_queue)) )
    {
        /* Release the memory allocated for UL NAS message
         * in function uecc_event_queue_push_ul_nas_transport_event */
        if(PNULL != p_event->p_msg_info)
        {
            rrc_mem_free(p_event->p_msg_info);
            p_event->p_msg_info = PNULL;
        }
        /* Free event */
        rrc_mem_free(p_event);
        p_event = PNULL;
    }
}
/*SPR_17976_END*/

