/******************************************************************************
*
*   FILE NAME:
*       uecc_event_queue.h
*
*   DESCRIPTION:
*       This is the UECC Even Queue definitions file.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   21 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_EVENT_QUEUE_H
#define UECC_EVENT_QUEUE_H

/****************************************************************************
 * Inculdes
 ****************************************************************************/

#include "rrc_defines.h"
#include "ylib.h"
#include "uecc_ue_ctx.h"
#include "uecc_fsm_common.h"

/****************************************************************************
 * Public types definitions
 ****************************************************************************/

/*SPR_16416_START*/
/* Code Removed */
/*SPR_16416_END*/
typedef YLNODE uecc_event_queue_node_t;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* typedef for UECC FSM event definition */
typedef struct
{
    uecc_event_queue_node_t node;           /* internal queue data */
    uecc_fsm_event_et       event_id;       /* event id */
    uecc_ue_context_t       *p_ue_context;  /* UECC UE context */
    void                    *p_msg_info;    /* Pointer on Event data */
} uecc_event_t;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void uecc_event_queue_init(
        uecc_event_queue_t      *p_queue /* Pointer on Event queue */
);

void uecc_event_queue_push_event(
        uecc_fsm_event_et       uecc_event_id,  /* event id */
        uecc_ue_context_t       *p_ue_context,  /* UECC UE context */
        void                    *p_msg_info     /* Pointer on Event data */
);

uecc_event_t* uecc_event_queue_pop_event(
        uecc_event_queue_t* p_queue     /* Pointer on Event queue */
);

/*SPR_16416_START*/
void uecc_event_queue_push_ul_nas_transport_event(
    uecc_fsm_event_et   uecc_event_id, /* event id */
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    DedicatedInfoNAS    *p_dedicatedInfoNAS,
    uecc_event_queue_t* p_queue        /* Pointer on Event queue */
);
/*SPR_16416_END*/
/*SPR_17976_START*/
void uecc_event_queue_push_tenb_cre_event(
    uecc_fsm_event_et   uecc_event_id, /* event id */
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void                *p_api,             /* Input API buffer */
    uecc_event_queue_t* p_queue        /* Pointer on Event queue */
);
void uecc_delete_tenb_cre_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to Event queue */
);
/*SPR_17976_END*/
#endif /* UECC_EVENT_QUEUE_H */
