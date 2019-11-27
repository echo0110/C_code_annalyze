/*******************************************************************************
 *File Name : oam_populate_l2_rlc_sps.c
 *
 *Description : This file contains functions to read values from xml file
 *
 *
 *Revision Histroy:
 *
 *Date       Author        References              Comments
 *------     --------      ------------            -------------
 *June,2013  KAPIL TAWAR   OAM Design Document     Initial Version    
 *
 *Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ******************************************************************************/


/*******************************************************************************
 * Project Includes
 ******************************************************************************/
#include "oam_xml_header.h"
#include "oam_types.h"
#include "oam_defines.h"
#include "oam_proto.h"
#include "oam_sim.h"
#include "oam_sim_utils.h"


extern oam_prov_req_t oam_prov_req;
extern prov_request_t prov_req;
SInt32 lclTemp_value;
/****************************************************************************
 * Function Name  : oam_populuate_l2_rlc_sps_params
 * Description    : This function populuate l2 rlc sps params
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
******************************************************************************/
oam_return_et 
oam_populuate_l2_rlc_sps_params()
{

UInt8 count= OAM_ZERO;
    oam_mac_init_req_params_t *oam_mac_init_req = OAM_NULL;
    static cell_index = OAM_ZERO;

	oam_mac_init_req = &(oam_prov_req.oam_l2_req.oam_mac_req.mac_init_req[cell_index]);

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.RLCParams,"enableSps"));
    oam_mac_init_req.enableSps = lclTemp_value; 

for(count= OAM_ZERO;count<MAX_QCI;count++)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.qciSpslist_rlc[count],"qciSpslistValue"));
    oam_mac_init_req.qciSpslist[count] = lclTemp_value;
}


for(count= OAM_ZERO;count<SPS_INTERVAL_LIST;count++)
{
    
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.dlSupportedIntervalList_rlc[count],"supportedDlIntervalListValue"));
    oam_mac_init_req.supportedDlIntervalList[count] = lclTemp_value;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.supportedUlIntervalList_rlc[count],"supportedUlIntervalListValue"));
    oam_mac_init_req.supportedUlIntervalList[count] = lclTemp_value;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.rlcSpsIntervalProfileParams_rlc[count],"enableSegConcatSpsRlcSdu"));
    oam_mac_init_req.rlcSpsIntervalProfileParams[count].enableSegConcatSpsRlcSdu = lclTemp_value;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
  xml_struct_g.rlcSpsIntervalProfileParams_rlc[count],"dlJitterHandlingOption"));
    oam_mac_init_req.rlcSpsIntervalProfileParams[count].dlJitterHandlingOption = lclTemp_value;
}
cell_index++;

return OAM_SUCCESS;
}
