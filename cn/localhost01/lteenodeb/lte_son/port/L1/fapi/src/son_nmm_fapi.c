/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $Id: son_nmm_fapi.c  $
 *
 ****************************************************************************
 *
 * File Description: Contains function for nmm fsm
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *
 * $Log: son_nmm_fapi.c  $
 * Revision 0.1  2012/01/02 09:50:42  gur26210
 * Initial code
 *
 ****************************************************************************/



/****************************************************************************
 *                              Standard Library Includes
 ****************************************************************************/


/****************************************************************************
 *                              Project Includes
 ****************************************************************************/
#include<son_defines.h>
#include<son_nmm_fapi.h>
#include<son_fapi_composer.h>
/****************************************************************************
 *                              Private Definitions
 ****************************************************************************/



/*****************************************************************************/
/*                              global variables                             */
/*****************************************************************************/



/****************************************************************************
 *                              Exported variables
 ****************************************************************************/


/***************************************************************************
 *                              Private Types
 ****************************************************************************/

/****************************************************************************
 *                              Macro definitions
 ****************************************************************************/

/****************************************************************************
 *                              Private Function Prototypes
 ****************************************************************************/
    


/****************************************************************************
 *                              Variable initialisation
 ****************************************************************************/

/*****************************************************************************
                                Functions forward declarations                      
*****************************************************************************/


/*****************************************************************************
 * Function Name  : son_nmm_initialise_phy
 * Inputs         : None
 * Outputs        : son_return_et
 * Returns        : SON_SUCCESS
 * Description    : This function initialise phy
 ****************************************************************************/
son_return_et
son_nmm_initialise_phy()
{
    return SON_SUCCESS;
}

/*****************************************************************************
 * Function Name  : son_nmm_clean_phy
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    :  
 ****************************************************************************/
void
son_nmm_clean_phy()
{
    /*do nothing*/
}

/*****************************************************************************
 * Function Name  : son_phy_get_api_id
 * Inputs         : nmm_fsm_msg_id
 * Outputs        : None
 * Returns        : None
 * Description    : This function returns nmm_fsm_msg_id
 ****************************************************************************/
void
son_phy_get_api_id
(
     U16 *nmm_fsm_msg_id          /*msg id*/
)
{
    *nmm_fsm_msg_id = *nmm_fsm_msg_id - NMM_FSM_MSG_PLATFORM_BASE;
    SON_UT_TRACE_EXIT();

}


/*****************************************************************************
 * Function Name  : son_nmm_phy_msg_process
 * Inputs         : void *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_phy_msg_process
(
    /*unused*/void * p_msg
)
{
    /*coverity_86392 Start*/
    return (phy_nmm_error_code_et)SON_SUCCESS; /*Represent SUCCESS*/
    /*coverity_86392 Stop*/
}


/*****************************************************************************
 * Function Name  : son_nmm_rssi_meas_cnf_process 
 * Inputs         : son_phy_rssi_meas_cnf_t *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno for 
 *                  son_nmm_rssi_meas_cnf msg
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_rssi_meas_cnf_process
(
    /*unused*/son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf
)
{
    /*coverity_86414 Start*/
    return (phy_nmm_error_code_et)SON_NULL; /*Represent SUCCESS*/
    /*coverity_86414 End*/
}



/*****************************************************************************
 * Function Name  : son_nmm_cell_search_cnf_process 
 * Inputs         : son_phy_cell_search_cnf_t *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno for 
 *                  son_nmm_cell_search_cnf msg
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_cell_search_cnf_process
(
    /*unused*/son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf
)
{
    /*coverity_86414 Start*/
    return (phy_nmm_error_code_et)SON_NULL; /*Represent SUCCESS*/
    /*coverity_86414 End*/
}






/*****************************************************************************
 * Function Name  : son_nmm_pbch_config_cnf_process 
 * Inputs         : son_phy_pbch_config_cnf_t *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno for 
 *                  son_nmm_pbch_config_cnf msg
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_pbch_config_cnf_process
(
    /*unused*/son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf
)
{
    /*coverity_86414 Start*/
    return (phy_nmm_error_code_et)SON_NULL; /*Represent SUCCESS*/
    /*coverity_86414 End*/
}


/*****************************************************************************
 * Function Name  : son_nmm_bcch_config_cnf_process 
 * Inputs         : son_phy_bcch_config_cnf_t *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno for 
 *                  son_nmm_bcch_config_cnf msg
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_bcch_config_cnf_process
(
    /*unused*/son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf
)
{
    /*coverity_86414 Start*/
    return (phy_nmm_error_code_et)SON_NULL; /*Represent SUCCESS*/
    /*coverity_86414 End*/
}

/*****************************************************************************
 * Function Name  : son_nmm_sib1_config_cnf_process
 * Inputs         : son_phy_sib1_config_cnf_t *
 * Outputs        : phy_nmm_error_code_et
 * Returns        : phy_nmm_error_code_et
 * Description    : This function set the son_nmm_errno for 
 *                  son_nmm_sib1_config_cnf msg
 ****************************************************************************/
phy_nmm_error_code_et 
son_nmm_sib1_config_cnf_process
(
    /*unused*/son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf
)
{
    /*coverity_86414 Start*/
    return (phy_nmm_error_code_et)SON_NULL; /*Represent SUCCESS*/
    /*coverity_86414 Stop*/
}




/*****************************************************************************
 * Function Name  : son_nmm_construct_interface_api_header 
 * Inputs         : p_header, length, src_id, dst_id, msg_id
 * Outputs        : None
 * Returns        : None
 * Description    : This function _construct_interface_api_header for L1
 ****************************************************************************/
void
son_phy_construct_interface_api_header
(
    U8 *p_header,
    U8 msg_id,
    U8 vender_spc_length,
    U16 length
              
)
{
    /*Version*/
    p_header[0] = msg_id;  /*HI*/
    p_header[1] = vender_spc_length;

    p_header[2] = (U8)((length & 0xFF00) >> 8);
    p_header[3] = (U8)(length & 0x00FF);

    return;
}


