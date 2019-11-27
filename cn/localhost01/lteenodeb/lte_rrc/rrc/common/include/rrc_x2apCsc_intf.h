/******************************************************************************
*
*   FILE NAME   : x2apCsc_intf.h
*
*   DESCRIPTION : This file contains basic X2AP types definitions.
*
*   DATE            AUTHOR         REFERENCE       REASON
*   12-JULY-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _X2APCSC_INTF_H_ 
#define _X2APCSC_INTF_H_

#include "rrc_defines.h"
#include "rrc_intrl_api.h"
#include "x2ap_api.h"

#define RRC_X2APCSC_MODULE_ID RRC_CSC_MODULE_ID

/*X2AP_CSC interface structure for csc cell Add req 
*/
typedef struct  
{
   rrc_cell_index_t         cell_index; /*^ M,0,H,0,7 ^*/  /* MAX_NUM_CELLS - 1 */
   /*SPR_16723_START*/
   U8                       eutran_cell_id[HOME_ENB_ID_OCTET_SIZE]; 
   U16                      phys_cell_id;
   U32                      dl_earfcn;
   U16                      ul_earfcn;
   /*SPR_16723_END*/
   /*SPR 21554 Start*/
    x2ap_broadcast_plmn_t             broadcast_plmn_info;
   /*SPR 21554 End*/
}x2ap_csc_cell_add_t; /*^ API, X2AP_CSC_CELL_ADD_IND ^*/

/*X2AP_CSC interface structure for csc cell Modify req 
*/
typedef struct  
{
    rrc_cell_index_t         cell_index; /*^ M,0,H,0,7 ^*/  /* MAX_NUM_CELLS - 1 */
    x2apOam_srvd_cell_arry_t  served_cell_to_upd; /*^ M,0,N,0,0 ^*/
}x2ap_csc_cell_upd_t; /*^ API, X2AP_CSC_CELL_UPD_IND ^*/

/*X2AP_CSC interface structure for csc cell Delete req 
*/
typedef struct  
{
    rrc_cell_index_t         cell_index; /*^ M,0,H,0,7 ^*/ /* MAX_NUM_CELLS - 1 */
}x2ap_csc_cell_del_t; /*^ API, X2AP_CSC_CELL_DEL_IND ^*/

/* X2AP_CSC interface structure for CSC Cell Status Req 
*/
typedef enum
{
    X2AP_CSC_CELL_DOWN,
    X2AP_CSC_CELL_UP
}x2ap_csc_cell_status_et;

typedef struct  
{
    rrc_cell_index_t         cell_index; /*^ M,0,H,0,7 ^*/ /* MAX_NUM_CELLS - 1 */
    x2ap_csc_cell_status_et    status;  /*^ M, 0, H, 0, 1 ^*/   /* x2ap_csc_cell_status_et */
}x2ap_csc_cell_status_info_resp_t; 

typedef struct  
{
    rrc_cell_index_t         cell_index; /*^ M,0,H,0,7 ^*/ /* MAX_NUM_CELLS - 1 */
}x2ap_csc_cell_status_info_req_t; 

typedef struct  
{
    rrc_counter_t                      num_served_cells;
    x2ap_csc_cell_status_info_req_t   cell_info[MAX_SERVED_CELLS];/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}x2ap_csc_cell_status_req_t; /*^ API, X2AP_CSC_CELL_STATUS_REQ ^*/

/* X2AP_CSC interface structure for CSC Cell Status Res 
*/
typedef struct  
{
    rrc_counter_t                      num_served_cells;
    x2ap_csc_cell_status_info_resp_t  cell_status_info[MAX_SERVED_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE^*/
    U8                                 no_of_uecc_instances;
}x2ap_csc_cell_status_resp_t; /*^ API, X2AP_CSC_CELL_STATUS_RESP ^*/
   

#endif
