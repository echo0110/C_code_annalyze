/******************************************************************************
**
**  FILE NAME     :  rrc_main.c
**
**  DESCRIPTION   :  This file is the main for the CMES - encapsulating
**                   module of RRC
**
**
**  DATE        AUTHOR          REF             REASON
**  --------    -------         ---             ----------
**  1-June-09   Ritika Garg    DDv0.2          Initial
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "stacklayer.h"
#include "cspl.h"
#include "rrc_main.h"


/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

QMODULE        oamhModule;
QMODULE        cscModule;
QMODULE        ueccModule;
QMODULE        llimModule;

/* gur04564- For compilation only - To be removed later - Start */
void* uecc_init(void *buf)
{}
S32 uecc_process_event(void *p_api, void *p_context)
{}
void uecc_process_timer_event(QTIMER timer_id, void *p_buf, void *p_context)
{}

void* csc_init(void *buf)
{}
S32 csc_process_event(void *p_api, void *p_context)
{}
void csc_process_timer_event(QTIMER timer_id, void *p_buf, void *p_context)
{}

void* llim_init(void *buf)
{}
S32 llim_process_event(void *p_api, void *p_context)
{}
void llim_process_timer(QTIMER timer_id, void *p_buf, void *p_context)
{}

void* oamh_init(void *buf)
{}
S32 oamh_process_event(void *p_api, void *p_context)
{}
void oamh_process_timer(QTIMER timer_id, void *p_buf, void *p_context)
{}
/* gur04564- For compilation only - To be removed later - End */

static const unsigned long oamh_dest_list[] =
{
    RRC_OAM_MODULE_ID,
    RRC_UECC_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_CSC_MODULE_ID
};

static const QMANIFEST oamh_module =
{
    "OAMH_MODULE",
    RRC_OAMH_MODULE_ID,
    {oamh_init, 0},
    {oamh_process_event, oamh_process_timer},
    {0, 0, 0 },
    oamh_dest_list
};

static const unsigned long uecc_dest_list[] =
{
    RRC_RRM_MODULE_ID,
    RRC_S1AP_MODULE_ID,
    RRC_OAMH_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_CSC_MODULE_ID
};

/* Pack/Unpack/Alloc utilities to be written in that module's code.
If not required, these can be set to 0 */
static const QMANIFEST uecc_module =
{
    "UECC_MODULE",
    RRC_UECC_MODULE_ID,
    {uecc_init, 0},
    {uecc_process_event, uecc_process_timer_event},
    {0, 0, 0},
    uecc_dest_list
};

static const unsigned long csc_dest_list[] =
{
    RRC_RRM_MODULE_ID,
    RRC_S1AP_MODULE_ID,
    RRC_OAMH_MODULE_ID,
    RRC_UECC_MODULE_ID,
    RRC_LLIM_MODULE_ID
};

static const QMANIFEST csc_module =
{
    "CSC_MODULE",
    RRC_CSC_MODULE_ID,
    {csc_init, 0},
    {csc_process_event, csc_process_timer_event},
    {0, 0, 0},
    csc_dest_list
};

static const unsigned long llim_dest_list[] =
{
    RRC_PDCP_MODULE_ID,
    RRC_RLC_MODULE_ID,
    RRC_MAC_MODULE_ID,
    RRC_PHY_MODULE_ID,
    RRC_RF_MODULE_ID,
    RRC_OAMH_MODULE_ID,
    RRC_UECC_MODULE_ID,
    RRC_CSC_MODULE_ID
};

static const QMANIFEST llim_module =
{
    "LLIM_MODULE",
    RRC_LLIM_MODULE_ID,
    {llim_init, 0},
    {llim_process_event, llim_process_timer},
    {0, 0, 0},
    llim_dest_list
};


static const QSHELL rrcOperations = {
    rrc_timed_wait,
    rrc_send,
    rrc_receive,
    rrc_open,
    rrc_close,
    0
};

static const QSHELL *operations_p = &rrcOperations;

/****************************************************************************
 * Function Name  : main
 * Input          : void    *listener_p = PNULL
 * Output         : none
 * Description    : Main of RRC module.
 * Returns        : 1: Success, -1: Failure
****************************************************************************/
S32 main (void)
{
    void    *listener_p = PNULL;
    listener_p = qvOpen (operations_p, PNULL);

    RRC_ASSERT (PNULL != listener_p);

    ueccModule = qvRegister (0, 0, 0, &uecc_module, 0);
    cscModule = qvRegister (0, 0, 0, &csc_module, 0);
    oamhModule = qvRegister (0, 0, 0, &oamh_module, 0);
    llimModule = qvRegister (0, 0, 0, &llim_module, 0);

    qvSetDestination (RRC_RRM_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_S1AP_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_OAM_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_PDCP_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_RLC_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_MAC_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_PHY_MODULE_ID, operations_p, listener_p, 0);
    qvSetDestination (RRC_RF_MODULE_ID, operations_p, listener_p, 0);

    qvRun(operations_p, listener_p, 0, 0);

    return (1);
}


