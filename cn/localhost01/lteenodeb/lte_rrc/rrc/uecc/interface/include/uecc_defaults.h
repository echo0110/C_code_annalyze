/******************************************************************************
*
*   FILE NAME:
*       uecc_defaults.h
*
*   DESCRIPTION:
*       This is the defaults values for RRC protocol IEs.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   19 Aug 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_DEFAULTS_H
#define UECC_DEFAULTS_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "rrc_rrm_intf.h"
/* SPR 14984 Fix Start */
#include "uecc_global_ctx.h"
/* SPR 14984 Fix Stop */
/****************************************************************************
 * Global variables definitions
 ****************************************************************************/

/* 3GPP TS 36331-850 - 9.2.1.1   SRB configurations - SRB1 */
/* LTE RRC - Rel 0.5 API Reference Manual - 4.7.8 RRC_RRM_UE_ADMISSION_RESP,
    4.7.8.4 - srbConfiguration_t */
extern const rrm_srb_config_t srb1_config_default;

/* 3GPP TS 36331-850 - 9.2.1.2   SRB configurations - SRB2 */
/* LTE RRC - Rel 0.5 API Reference Manual - 4.7.8 RRC_RRM_UE_ADMISSION_RESP,
    4.7.8.4 - srbConfiguration_t */
extern const rrm_srb_config_t srb2_config_default;

/* 3GPP TS 36331-850 - 9.2.2 Default MAC main configuration */
/* LTE RRC - Rel 0.5 API Reference Manual - 4.7.8 RRC_RRM_UE_ADMISSION_RESP,
    4.7.8.1 - macMainConfiguration_t */
extern const rrm_mac_config_t    mac_config_default;

/* 3GPP TS 36331-850 - 9.2.3 Default semi-persistent scheduling configuration */
/* LTE RRC - Rel 0.5 API Reference Manual - 4.7.8 RRC_RRM_UE_ADMISSION_RESP,
    4.7.8.3 - spsConfiguration_t */
extern const rrc_sps_config_t sps_config_default;

/* 3GPP TS 36331-850 - 9.2.4   Default physical channel configuration */
/* LTE RRC - Rel 0.5 API Reference Manual - 4.7.8 RRC_RRM_UE_ADMISSION_RESP,
    4.7.8.4 - physicalDedicatedConfiguration_t */
extern rrc_phy_physical_config_dedicated_t
    physical_config_dedicated_default;

/****************************************************************************
 * Global functions definitions
 ****************************************************************************/
void uecc_defaults_apply_srb_config(
    rrm_srb_config_t *p_srb_config,
/*SPR 14984 Fix Start */
        rrm_srb_config_t const *p_srb_config_default,
        uecc_ue_context_t      *p_uecc_ue_context,
        U8                              srb_identity
/*SPR 14984 Fix Stop */
);
 /*SPR 14984 Fix Start */
rrc_return_et uecc_validate_antenna_info(
        rrc_phy_antenna_information_dedicated_t *p_antenna_info,
        uecc_ue_context_t   *p_uecc_ue_context                  
        );
 /*SPR 14984 Fix Stop */

#endif /* UECC_DEFAULTS_H */

