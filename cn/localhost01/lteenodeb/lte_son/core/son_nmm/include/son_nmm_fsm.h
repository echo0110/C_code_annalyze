/*! \file son_nmm_fsm.h
 *  \brief This file contains the states events and definitions for NMM FSM.
 *  \date January 02, 2012
 *  \author gur26210
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_NMM_FSM_H_
#define _SON_NMM_FSM_H_

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

#ifdef  __SON_FAPI__

/*! \headerfile son_nmm_fapi.h <>
 */
#include <son_nmm_fapi.h>
#endif



/*! \headerfile son_nmm_phy_api.h <>
 */
#include <son_nmm_phy_api.h>

/*! \headerfile sib_decoder_intf.h <>
 */
#include <sib_decoder_intf.h>

/*! \headerfile umts_sib_decoder_intf.h <>
 */
#include <umts_sib_decoder_intf.h>

/*! \headerfile son_nmm_apps_intf.h <>
 */
#include <son_nmm_apps_intf.h>

/*! \def GET_NMM_DB_PARM(temp)
 *  \brief A macro specifies "p_nmm_db->temp" i.e. use to access database members .
 */
#define GET_NMM_DB_PARM(temp) \
    p_nmm_db->temp

/*! \def GET_FREQ_CHANNEL_PARM(index1,temp)
 *  \brief A macro specifies "p_nmm_db->freq_channel[(index1)].temp" i.e. use to access database members .
 */

#define GET_FREQ_CHANNEL_PARM(index1, temp) \
    p_nmm_db->freq_channel[(index1)].temp

/*! \def GET_CELL_PARM(index1,index2,temp)
 *  \brief A macro specifies "p_nmm_db->freq_channel[(index1)].cell[(index2)].temp" i.e. use to access database members .
 */

#define GET_CELL_PARM(index1, index2, temp) \
    p_nmm_db->freq_channel[(index1)].cell[(index2)].temp

/*! \def GET_SIBX_PARM(index1, index2, p_sib_info)
 *  \brief A macro specifies "p_nmm_db->freq_channel[(index1)].cell[(index2)]. p_sib_info" i.e. use to access database members .
 */
#define GET_SIBX_PARM(index1, index2, p_sib_info) \
    p_nmm_db->freq_channel[(index1)].cell[(index2)].p_sib_info

/*! \def SIBX_PRESENCE(index1, index2, p_sib_info, presence_bitmask)
 *  \brief A macro specifies "p_nmm_db->freq_channel[(index1)].cell[(index2)].p_sib_info->presence_bitmask" i.e. use to access database members .
 */
#define SIBX_PRESENCE(index1, index2, p_sib_info, presence_bitmask) \
    p_nmm_db->freq_channel[(index1)].cell[(index2)].p_sib_info->presence_bitmask

/*! \def SCAN_RESP_LIST_INIT()
 *  \brief A macro that specifies "ylInit( &scan_resp_list)" ,use to store the response of scan request as a link list.
 */

#define SCAN_RESP_LIST_INIT(p_scan_resp_list) \
    ylInit( p_scan_resp_list )

/*! \def SCAN_RESP_LIST_GET_COUNT()
 *  \brief A macro specifies "ylCount( &scan_resp_list )" ,use to count the number of response obtained .
 */

#define SCAN_RESP_LIST_GET_COUNT(p_scan_resp_list) \
    ylCount( p_scan_resp_list )

/*! \def NMM_FSM_TIME_OUT_MSG_ID_BASE
 *  \brief A macro that store NONE.
 */
#define NMM_FSM_TIME_OUT_MSG_ID_BASE

/*! \def NMM_FSM_TIME_OUT_MSG_ID
 *  \brief A maro that represents "NMM_FSM_TIME_OUT_MSG_ID_BASE + 1"
 */
#define NMM_FSM_TIME_OUT_MSG_ID NMM_FSM_TIME_OUT_MSG_ID_BASE + 1

/*! \def SON_INDEX_ERR
 *  \brief A maro that represents invalid index in array"
 */
#define SON_INDEX_ERR -1

/*! \def SON_ONE
 *  \brief A maro use as value 1 in code"
 */
#define SON_ONE 1

/*! \def UMTS_MIB_INFO
 *  \brief A macro for MIB bitmask
 */
#define UMTS_MIB_INFO 0x01

/*! \def UMTS_SIB_MSG_3
 *  \brief A macro for SIB3 bitmask
 */
#define UMTS_SIB_MSG_3 0x08

/*! \enum nmm_state_et
 *  \brief An enum that specifies the NMM_FSM states.
 */
typedef enum
{
    NMM_W_FOR_NULL,                          /*!< NMM_FSM state 0 */
    NMM_W_FOR_RSSI_MEAS_IND,                 /*!< NMM_FSM state 1 */
    NMM_W_FOR_CELL_SEARCH_IND,               /*!< NMM_FSM state 2 */
    NMM_W_FOR_PBCH_CONFIG_CNF,               /*!< NMM_FSM state 3 */
    NMM_W_FOR_PBCH_DATA_IND,                 /*!< NMM_FSM state 4 */
    NMM_W_FOR_SIB1_CONFIG_CNF,               /*!< NMM_FSM state 5 */
    NMM_W_FOR_BCCH_DATA_IND_PRE_REQ,         /*!< NMM_FSM state 6 */
    NMM_W_FOR_BCCH_CONFIG_CNF,               /*!< NMM_FSM state 7 */
    NMM_W_FOR_BCCH_DATA_IND,                  /*!< NMM_FSM state 8 */
    MAX_NUM_STATE                            /*!< NMM_FSM state 9  */
}nmm_state_et;
/*! \enum nmm_utran_state_et
 *  \brief An enum that specifies the UMTS NMM_FSM states.
 */

typedef enum
{
    NMM_UTRAN_W_FOR_SCAN_REQ,                 /*!< UMTS NMM_FSM state 0 */
    NMM_UTRAN_W_FOR_SI_MSG_IND,               /*!< UMTS NMM_FSM state 1 */
    MAX_NUM_UTRAN_STATES                      /*!< UMTS NMM_FSM state 2 */
}nmm_utran_state_et;

/*! \enum son_nmm_errno_et
 *  \brief An enum that specifies the NMM_FSM response.
 */

typedef enum
{
    SON_NMM_SUCCESS = MSPD_PHY_OK,    
    SON_NMM_PHY_NOT_PARAM,              
    SON_NMM_PHY_CTX_ALLOC_ERROR,       

    /*used as SON_NMM_MEM_ALLOC_FAILURE insted of
     * SON_NMM_PHY_PART_ALLOC_ERROR */
    SON_NMM_PHY_UNKNOWN_API = \
        MSPD_PHY_UNKNOWN_API,  
    SON_NMM_PHY_UNKNOWN_TYPE,           
    SON_NMM_PHY_UNKNOWN_PARAM,          
    SON_NMM_PHY_CONTEX_ALLOC_ERROR,     
    SON_NMM_PHY_INCORRECT_PARAM,        
    SON_NMM_PHY_NO_VALID_DATA_ERROR,    
    SON_NMM_PHY_HEAP_ALLOC_ERROR,       

    SON_NMM_PHY_NOT_ENABLED = \
        MSPD_NMM_NOT_ENABLED,   /*!<NMM FSM ERRNO 0x81*/
    SON_NMM_PHY_UNEXPECTED_CMD,         /*!<NMM FSM ERRNO 0x82*/
    SON_NMM_PHY_INVALID_FREQ_BAND,      /*!<NMM FSM ERRNO 0x83*/
    SON_NMM_PHY_INVALID_EARFCN,         /*!<NMM FSM ERRNO 0x84*/
    SON_NMM_PHY_FREQ_BAND_NOT_SUPPORTED_BY_HW, /*!<NMM FSM ERRNO 0x85*/
    SON_NMM_PHY_INVALID_NR_CARRIER_LIST, /*!<NMM FSM ERRNO 0x86*/
    SON_NMM_PHY_INVALID_CARRIER_LIST,   /*!<NMM FSM ERRNO 0x87*/
    SON_NMM_PHY_LTE_IN_USE,             /*!<NMM FSM ERRNO 0x88*/
    SON_NMM_PHY_LTE_OUT_OF_MEMORY,      /*!<NMM FSM ERRNO 0x89*/
    SON_NMM_SIB_INCOMPLETE,             /*!<NMM FSM ERRNO 0x8a*/
    SON_NMM_TIMER_EXPIRE = \
        SON_ERR_NMM_TIMER_EXPIRED,  /*!<NMM FSM ERRNO TIMER_EXPIRED*/
    SON_NMM_MEM_ALLOC_FAILURE = \
        SON_ERR_SYS_MEM_ALLOC_FAILURE, /*!<NMM FSM ERRNO MEM_ALLOC*/
    SON_NMM_MSG_SEND_FAILED = \
        SON_ERR_NMM_MSG_SEND_FAILED, /*!<NMM FSM ERRNO MSG_SEND*/
    SON_NMM_INVALID_MSG_REC = \
        SON_ERR_NMM_INVALID_MSG_REC, /*!<NMM FSM ERRNO INVALID_MSG*/
    SON_PHY_NULL_DATA = \
        SON_ERR_NMM_PHY_NULL_DATA, /*!<NMM FSM ERRNO NULL_DATA*/
    SON_NMM_STOP_MSG_REC = \
        SON_ERR_NMM_STOP_SCAN_RUNNING, /*!<NMM FSM ERRNO STOP_SCAN*/
    SON_NMM_UNKNOWN_FAILURE = \
        SON_ERR_NMM_UNKNOWN_FAILURE             /*!<NMM FSM ERRNO UNKNOWN_FAILURE*/
}son_nmm_errno_et;

/*! \enum nmm_utran_fsm_event_et
 *  \brief An enum that specifies the UMTS  NMM_FSM events.
 */

typedef enum
{
    NMM_START_UTRAN_SCAN_EV,                /*!<UMTS NMM FSM EVENT  */
    SON_PHY_UTRAN_SI_MSG_IND_EV,            /*!<UMTS NMM FSM EVENT  */
    MAX_NUM_UTRAN_EV                        /*!<UMTS NMM FSM EVENT  */
}nmm_utran_fsm_event_et;

/*! \enum son_nmm_sib_type_et
 *  \brief An enum that specifies the sib types.
 */

typedef enum
{
    SIB_TYPE_3,                             /*!<SIB TYPE 3 */
    SIB_TYPE_4,                             /*!<SIB TYPE 4 */
    SIB_TYPE_5,                             /*!<SIB TYPE 5 */
    SIB_TYPE_6,                             /*!<SIB TYPE 6 */
    SIB_TYPE_7,                             /*!<SIB TYPE 7 */
    SIB_TYPE_8,                             /*!<SIB TYPE 8 */
    SIB_TYPE_9,                             /*!<SIB TYPE 9 */
}son_nmm_sib_type_et;

/*! \  son_return_et (*nmm_fsm_func)(void *)
 *  \brief Represent the return type for son functions
 */

typedef son_return_et (*nmm_fsm_func)
(
    void *
);

/*! \  struct mib_t
 *  \brief Contains the parameters require to store mib's information.
 *  \param sfn			The radio frame the data was received on.
 *  \param phich_duration       The PHICH duration of the cell.
 *  \param phich_resource       The PHICH resource size of the cell.
 *  \param dl_sys_bandwidth     The DL transmission bandwidth in RBs.
 */

typedef struct _mib_t
{
    U16 sfn;
    U8 phich_duration;
    U8 phich_resource;
    U8 dl_sys_bandwidth;
}mib_t;

/*! \  struct cell_t
 *  \brief  Contains the parameters that stores the cell related information.
 *  \param cell_valid			Cell is valid or not.
 *  \param cell_id              The physical cell identity.
 *  \param rsrp                 Signal strength
 *  \param num_antenna          Number of antenna a cell has.
 *  \param *p_mib               Pointer to mib_t structure
 *  \param p_sib_type_1_Info    pointer to system information block 1
 *  \param p_sib_type_2_Info    pointer to system information block 2
 *  \param p_sib_type_3_Info    pointer to system information block 3
 *  \param p_sib_type_4_Info    pointer to system information block 4
 *  \param p_sib_type_6_Info    pointer to system information block 6
 *  \param p_sib_type_9_Info    pointer to system information block 9
 */

typedef struct _cell_t
{
    U8 cell_valid;
    U16 cell_id;
    U8 rsrp;
    U8 num_antenna;
    mib_t *p_mib;
    U16 sibx_presence_flag;
    sib_type_1_Info_t *p_sib_type_1_Info;
    sib_type_2_Info_t *p_sib_type_2_Info;
    sib_type_3_Info_t *p_sib_type_3_Info;
    sib_type_4_Info_t *p_sib_type_4_Info;
    sib_type_5_Info_t *p_sib_type_5_Info;
    sib_type_6_Info_t *p_sib_type_6_Info;
    sib_type_9_Info_t *p_sib_type_9_Info;
}cell_t;

/*! \  struct freq_channel_t
 *  \brief  Contains the parameters that stores the frequency channel related information.
 *  \param earfcn			The earfcn for which cells should be measured.
 *  \param rssi				The measured RSSI in dBm in steps of 1dB
 *  \param meas_bandwidth		The number of resource blocks which should be used for measuring RSRP.
 *  \param num_pci			The number of cells in the pciList. If 0 all cells on the carrier should be found.
 *  \param cell[SON_MAX_NO_PCI]             The variable of structure cell_t
 */

typedef struct _freq_channel_t
{
    U32 earfcn;
    U8 rssi;
    U8 meas_bandwidth;
    U16 num_pci;
    cell_t cell[SON_MAX_NO_PCI];
}freq_channel_t;

/*NMM database*/

/*! \  struct nmm_db_t
 *  \brief Contains the parameters that store various information obtained from phy and NMM controller.
 *  \param meas_period			The length of the time the NMM measure RSSI over. IN 10ms radio frames
 *  \param retry_count			The number of periods for which decoding of a SIB1 should be retried.
 *  \param freq_band_id			The id number for the frequency band.
 *  \param num_earfcn		The number of carriers which should be measured. In case no carrier (value 0) is specified all carriers in the band shall be measured.
 *  \param freq_channel[SON_MAX_NO_EARFCN]  The variable of structure freq_channel_t
 */

typedef struct _nmm_db_t
{
    U8 meas_period;
    U8 retry_count;
    U8 freq_band_id;
    U16 num_earfcn;
    freq_channel_t freq_channel[SON_MAX_NO_EARFCN];
}nmm_db_t;

/*! \fn extern son_return_et son_nmm_initialise_phy()
 *  \brief initialise L1
 */

extern son_return_et
son_nmm_initialise_phy
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn extern void  son_nmm_clean_phy()
 *  \brief clean L1
 */

extern void
son_nmm_clean_phy
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn extern void son_phy_get_api_id(U16 *nmm_fsm_msg_id)
 *  \brief getting msg id
 *  \param nmm_fsm_msg_id msg id received
 */

extern void
son_phy_get_api_id
(
    U16 *nmm_fsm_msg_id
);

/*! \fn phy_nmm_error_code_et son_nmm_phy_msg_process(void * p_msg)
 *  \brief process msg received from phy
 *  \param *p_msg pointer to the message
 */

extern phy_nmm_error_code_et
son_nmm_phy_msg_process
(
    void *p_msg
);

/*! \fn extern phy_nmm_error_code_et son_nmm_rssi_meas_cnf_process(son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf)
 *  \brief process rssi_meas_cnf msg received from phy
 *  \param p_son_phy_rssi_meas_cnf pointer to the message received from PHY
 */

extern phy_nmm_error_code_et
son_nmm_rssi_meas_cnf_process
(
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf
);

/*! \fn extern phy_nmm_error_code_et son_nmm_cell_search_cnf_process(son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf)
 *  \brief process cell_search_cnf msg received from phy
 *  \param p_son_phy_cell_search_cnf pointer to the message thatis received from PHY
 */

extern phy_nmm_error_code_et
son_nmm_cell_search_cnf_process
(
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf
);

/*! \fn extern phy_nmm_error_code_et son_nmm_pbch_config_cnf_process(son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf)
 *  \brief process pbch_config_cnf msg received from phy
 *  \param p_son_phy_pbch_config_cnf pointer to the message received from PHY.
 */

extern phy_nmm_error_code_et
son_nmm_pbch_config_cnf_process
(
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf
);

/*! \fn extern phy_nmm_error_code_et son_nmm_sib1_config_cnf_process(son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf)
 *  \brief process sib1_config_cnf msg received from phy
 *  \param p_son_phy_sib1_config_cnf pointer to the message received from PHY
 */

extern phy_nmm_error_code_et
son_nmm_sib1_config_cnf_process
(
    son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf
);

/*! \fn extern phy_nmm_error_code_et son_nmm_bcch_config_cnf_process(son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf)
 *  \brief process bcch_config_cnf msg received from phy
 *  \param p_son_phy_bcch_config_cnf pointer to the message received from PHY
 */

extern phy_nmm_error_code_et
son_nmm_bcch_config_cnf_process
(
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf
);

#endif /* ifndef _SON_NMM_FSM_H_ */
