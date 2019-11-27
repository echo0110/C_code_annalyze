/*******************************************************************************
 *File Name : oam_populate_rrm_geran_cdma200_params.c
 *
 *Description : This file contains functions to read values from xml file
 *
 *
 *Revision Histroy:
 *
 *Date         Author         References            Comments
 *------       --------       ------------          -------------
 *June,2013    KAPIL TAWAR    OAM Design Document   Initial Version          
 *June,2014    NITIN VIJAY    SPR 11704 FIX         SPR Changes          
 *August,2014  SHALU SAGAR    SPR 12673 FIX           
 *August,2014  Avinash        SPR 13283 FIX
 *Dec,2014     Ankita Jain    SPR-13799 FIX
 *Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ******************************************************************************/

/*******************************************************************************
 * Project Includes
 ******************************************************************************/
#include "oam_xml_header.h"
#include "oam_xml_enum.h"
#include "oam_types.h"
#include "oam_defines.h"
#include "oam_proto.h"
#include "oam_sim.h"
#include "oam_sim_utils.h"

extern oam_prov_req_t oam_prov_req;
extern prov_request_t prov_req;
extern  oam_rrm_cell_config_req_t rrm_cell_config_ptr[OAM_MAX_CELL_PRESENT_MULTICELL];
/* SPR 12932 FIX START */
oam_bool_et cdma_daho_found_flag = OAM_FALSE;
oam_bool_et gsm_daho_found_flag = OAM_FALSE;
/* SPR 12932 FIX END */
/* SPR 13283 START */
UInt64 lclTemp_value3;
/* SPR 13283 END */

/*SPR 9345 FIX START*/
int oam_xml_search_string_to_integer
(
 char  *parmeter_value,
 enumerated_string_fields_mapping_tbl_t  *par_table_address,
 int array_size
 );
/*SPR 9345 FIX END*/
/*******************************************************************************
 *  FUNCTION NAME    : oam_populate_rrm_geran_freq_cells_cell_based
 *  DESCRIPTION      : This function used to populate the geran freq cells
 *  PARAMETERS
 *      IN           : NONE
 *
 *      OUT          : NONE
 *
 *  RETURNS          :  OAM_SUCCESS on Success
 *                      OAM_FAILURE on Failure
 *******************************************************************************/

	oam_return_et
oam_populate_rrm_geran_freq_cells_cell_based(oam_rrm_cell_config_req_t  *cell_config_ptr , XML_struct_cell_config *p_cell_spec_params)
{
    char *temp = OAM_NULL;
    UInt32 count = OAM_ZERO;
    SInt32 lclTemp_value = OAM_ZERO;
    UInt8 *p_temp = OAM_NULL;
    Char8 temp_plmnid[OAM_SEVEN] = {0};

    OAM_FUN_ENTRY_TRACE();
    UInt32 num_of_gsm = oam_atoi((const char *)oam_xmlNodeGetContent(
				p_cell_spec_params->InterRATCell_1,"GSMNumberOfEntries"));

	cell_config_ptr->ran_info.bitmask |= RRM_OAM_NCL_PARAMS_PRESENT;
	cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.num_valid_geran_freq_cell = num_of_gsm;
	cell_config_ptr->ran_info.ncl_params.bitmask |= RRM_OAM_INTER_RAT_NCL_PRESENT;

for(count = OAM_ZERO ;count<num_of_gsm;count++)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->GSM[count],"CI"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].cell_id=lclTemp_value;
 
		temp = (char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"LAC");
            lclTemp_value = oam_atoi((Char8*)temp);
        /*SPR 19721 FIX START */
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].lai.lac[OAM_ONE]=(lclTemp_value & OAM_LOWER_EIGHT_BITS_SET);

		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].lai.lac[OAM_ZERO]=((lclTemp_value  >> OAM_EIGHT) & OAM_LOWER_EIGHT_BITS_SET);
        /*SPR 19721 FIX END */

		temp = (char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"PLMNID");
        oam_convert_plmnid(temp_plmnid,temp);
        /* coverity 41195 */
        OAM_NULL_CHECK(temp != OAM_NULL);
		oam_fill_mcc_mnc_from_plmnid(temp_plmnid,&(cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].lai.plmn_id));


		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
                            "X_VENDOR_DAHO_INDICATION"),OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_DAHO_INDICATION"));
  /* SPR 12932 FIX START */
  UInt32 instance_id = OAM_ZERO;

  if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
                      "X_VENDOR_INSTANCE_ID"),OAM_INVALID_STR))!= OAM_ZERO)
  {
      instance_id = oam_atoi((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"X_VENDOR_INSTANCE_ID"));

  }
  /*Coverity fix start : 78986,71432 */
  if ((instance_id <= 0)||(instance_id>OAM_INSTANCE_ID_MAX_VAL)) {
      OAM_LOG(OAM, OAM_ERROR,
              "Invalid value[%d] of X_VENDOR_INSTANCE_ID valid range is 1-32", instance_id);
      OAM_FUN_EXIT_TRACE();
      return OAM_FAILURE;
  }
  /*Coverity fix End : 78986,71432 */


  /* SPR-10462 Start*/
#ifdef OAM_SON_ENABLE /* +- SPR 13917 fix */
  if((lclTemp_value == OAM_ONE) && (gsm_daho_found_flag == OAM_FALSE))
  {
      gsm_daho_found_flag = OAM_TRUE;
       /*ut_bug_fix__15474_start CID 81776*/
      if (instance_id > OAM_ZERO)
      {
/*SPR 22442 Fix Start*/
          SON_PROV_REQ.inter_rat_gsm_daho_bitmask |= OAM_ONE<<(instance_id - OAM_ONE);
/*SPR 22442 Fix End*/
      }
       /*ut_bug_fix__15474_start CID 81776*/
  }
  /* SPR 12932 FIX END */
#endif

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].daho_indication=lclTemp_value;

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |=  RRM_OAM_GERAN_FREQ_DAHO_IND_PRESENT;
}

/* SPR 19327 Fix Start */

cell_config_ptr->ran_info.ncl_params.inter_rat_ncl.
geran_freq_cells[count].rac = 
oam_atoi((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
            "X_VENDOR_RAC"));
/* SPR 19327 Fix Stop */

		/****** SPR_9345_FIX ****/
		temp = (Char8*)oam_xmlNodeGetContent( p_cell_spec_params->GSM[count],"BandIndicator");
		if((oam_strcmp((const char *)temp,OAM_INVALID_STR))!= OAM_ZERO) {
			lclTemp_value =
				oam_xml_search_string_to_integer(temp,
                                          RRM_GSM_BANDINDICATOR,
                                          OAM_RRM_GSM_BAND_IND_TBL_NUM_ROWS);
    /* coverity 35838 */
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bandindicator=(rrm_oam_band_ind_et)lclTemp_value;
		}
		lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->GSM[count],"BCCHARFCN"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bccharfcn=lclTemp_value;

    /* SPR-13799 START */
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->GSM[count],"BSIC"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].pci=lclTemp_value;
    /* SPR-13799 END */

		p_temp = oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"X_VENDOR_HO_STATUS");
    if ((oam_strcmp((const char *)p_temp,OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_HO_STATUS"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].ho_status=lclTemp_value;

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |= RRM_OAM_GERAN_FREQ_HO_STATUS_PRESENT;

}
		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
                            "X_VENDOR_NETWORK_CONTROL_ORDER"),OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_NETWORK_CONTROL_ORDER"));
    /* coverity 35839 */
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].network_control_order=(rrm_oam_nco_ind_et)lclTemp_value;

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |= RRM_OAM_GERAN_FREQ_CELL_NW_CTRL_ORDER_PRESENT;

}

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
                            "X_VENDOR_DTM_SUPPORTED"),OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_DTM_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].dtm_supported=lclTemp_value;

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |= RRM_OAM_GERAN_FREQ_CELL_DTM_SUPPORTED_PRESENT;

}



		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],
                            "X_VENDOR_DTM_HO_SUPPORTED"),OAM_INVALID_STR))!= OAM_ZERO)
{

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_DTM_HO_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].dtm_ho_supported=lclTemp_value;

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |= RRM_OAM_GERAN_FREQ_CELL_DTM_HO_SUPPORTED_PRESENT;

}

		p_temp = oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"X_VENDOR_VOIP_CAPABLE");
if ((oam_strcmp((const char *)p_temp,OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_VOIP_CAPABLE"));

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].voip_capable=lclTemp_value;
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |=  RRM_OAM_GERAN_FREQ_CELL_VOIP_CAPABLE_PRESENT;

}

		p_temp = oam_xmlNodeGetContent(p_cell_spec_params->GSM[count],"X_VENDOR_PS_HO_SUPPORTED");
if ((oam_strcmp((const char *)p_temp ,OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->GSM[count],"X_VENDOR_PS_HO_SUPPORTED"));

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].voip_capable=lclTemp_value;
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.geran_freq_cells[count].bitmask |=  RRM_OAM_GERAN_FREQ_HO_STATUS_PRESENT;

}

}

OAM_FUN_EXIT_TRACE();
return OAM_SUCCESS;
}

/*******************************************************************************
 *  FUNCTION NAME    : oam_populate_rrm_cdma2000_cell_based
 *  DESCRIPTION      : This function used to populate the geran freq cells
 *  PARAMETERS
 *      IN           : NONE
 *
 *      OUT          : NONE
 *
 *  RETURNS          :  OAM_SUCCESS on Success
 *                      OAM_FAILURE on Failure
*******************************************************************************/

	oam_return_et
oam_populate_rrm_cdma2000_cell_based(oam_rrm_cell_config_req_t  *cell_config_ptr , XML_struct_cell_config *p_cell_spec_params)
{
    UInt32 count = OAM_ZERO;
    SInt32 lclTemp_value = OAM_ZERO;
    Char8 *temp = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    UInt32 num_of_cdma2000 = oam_atoi((const char *)oam_xmlNodeGetContent(
				p_cell_spec_params->InterRATCell_1,"CDMA2000NumberOfEntries"));
	cell_config_ptr->ran_info.bitmask |= RRM_OAM_NCL_PARAMS_PRESENT;
	cell_config_ptr->ran_info.ncl_params.inter_rat_ncl.num_valid_cdma2000_freq_cells = num_of_cdma2000;
	cell_config_ptr->ran_info.ncl_params.bitmask |= RRM_OAM_INTER_RAT_NCL_PRESENT;
 for(count = OAM_ZERO ; count  <num_of_cdma2000 ; count ++)
 {

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->CDMA2000_2[count],"BandClass"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].band_class=lclTemp_value;


  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->CDMA2000_2[count],"ARFCN"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].arfcn=lclTemp_value;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->CDMA2000_2[count],"PNOffset"));
		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].pn_offset=lclTemp_value;

		if(oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],"Type"),"1xRTT")==0)
    {
    /* coverity 35837 */
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].type=(rrm_oam_ncl_type_et)OAM_ZERO;
    /* SPR 13283 START */
    temp =
        (Char8*)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],"CID");

    lclTemp_value3 = oam_atoll((char *)temp);
    oam_conversion_from_int64_to_byte_string(
            cell_config_ptr->ran_info.ncl_params.inter_rat_ncl.cdma2000_freq_cells[count].CID,
            lclTemp_value3,OAM_CDMA_1XRTT_CELL_ID_BITSTRING_SIZE);
    cell_config_ptr->ran_info.ncl_params.
        inter_rat_ncl.cdma2000_freq_cells[count].num_valid_count_cid = OAM_SIX;
    /* SPR 13283 END */
    }

		if(oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],"Type"),"HRPD")==0)
    {
    /* coverity 35836 */
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].type=(rrm_oam_ncl_type_et)OAM_ONE;

    /* SPR 13283 START */
    temp =
        (Char8*)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],"CID");

    cell_config_ptr->ran_info.ncl_params.
        inter_rat_ncl.cdma2000_freq_cells[count].num_valid_count_cid = OAM_SIXTEEN;
    lclTemp_value3 = oam_atoll((char *)temp);

    oam_conversion_from_int128_to_byte_string(
            cell_config_ptr->ran_info.ncl_params.inter_rat_ncl.cdma2000_freq_cells[count].CID,
            lclTemp_value3,OAM_MAX_CDMA_HRPD_INT_BITSTRING_SIZE);
    /* SPR 13283 END */

    }



		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].
    presence_bitmask |= RRM_OAM_CELL_SPECIFIC_PARAMS_PRESENCE_FLAG;

		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.bitmask
    |= RRM_OAM_PZ_ID_HYST_PARAMETERS_INCLUDED_PRESENT;

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_ENABLED"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_ENABLD_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_ENABLED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_enabled=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_INFO_INCL"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_INFO_INCL_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_INFO_INCL"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_info_incl=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_LIST_LEN"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_LIST_LEN_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_LIST_LEN"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_list_len=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_ACT_TIMER"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_ACT_TIMER_PRESENT;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_ACT_TIMER"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_act_timer=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_TIMER_MUL"),OAM_INVALID_STR))!=OAM_ZERO)
    {

			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_TIMER_MUL_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_TIMER_MUL"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_timer_mul=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PKT_ZONE_HYST_TIMER_EXP"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.bitmask |= RRM_OAM_PZ_HYST_TIMER_EXP_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PKT_ZONE_HYST_TIMER_EXP"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pz_hyst_parameters_included.pz_hyst_timer_exp=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PROTOCOL_REV_LEVEL"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_P_REV_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PROTOCOL_REV_LEVEL"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    p_rev=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_MIN_PROTOCOL_REV_LEVEL"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_MIN_P_REV_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_MIN_PROTOCOL_REV_LEVEL"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    min_p_rev=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_NEG_SLOT_CYCLE_INDEX"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_NEG_SLOT_CYCLE_INDEX_SUP_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_NEG_SLOT_CYCLE_INDEX"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    neg_slot_cycle_index_sup=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_ENCRYPT_MODE"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_ENCRYPT_MODE_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_ENCRYPT_MODE"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    encrypt_mode=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_ENCRYPT_SUPPORTED"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_ENC_SUPPORTED_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_ENCRYPT_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    enc_supported=lclTemp_value;
    }
 
		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_SIG_ENCRYPT_SUPPORTED"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_SIG_ENCRYPT_SUP_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_SIG_ENCRYPT_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    sig_encrypt_sup=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_MSG_INTEGRITY_SUPPORTED"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_MSG_INTEGRITY_SUP_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_MSG_INTEGRITY_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    msg_integrity_sup=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_SIG_INTEGRITY_SUPPORTED_INCL"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_SIG_INTEGRITY_SUP_INCL_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_SIG_INTEGRITY_SUPPORTED_INCL"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    sig_integrity_sup_incl=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_SIG_INTEGRITY_SUPPORTED"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_SIG_INTEGRITY_SUP_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_SIG_INTEGRITY_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    sig_integrity_sup=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_MS_INIT_POS_LOC_SUPPORTED_IND"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_MS_INIT_POS_LOC_SUP_IND_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_MS_INIT_POS_LOC_SUPPORTED_IND"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    ms_init_pos_loc_sup_ind=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_BAND_CLASS_INFO_REQ"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_BAND_CLASS_INFO_REQ_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_BAND_CLASS_INFO_REQ"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    band_class_info_req=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_BAND_CLASS"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_BAND_CLASS_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_BAND_CLASS"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    band_class=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_ALT_BAND_CLASS"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_ALT_BAND_CLASS_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_ALT_BAND_CLASS"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    alt_band_class=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_TRACKING_ZONE_MODE_SUPPORTED"),OAM_INVALID_STR))!=OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_TKZ_MODE_SUPPORTED_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_TRACKING_ZONE_MODE_SUPPORTED"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    tkz_mode_supported=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_TRACKING_ZONE_ID"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_TKZ_ID_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_TRACKING_ZONE_ID"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    tkz_id=lclTemp_value;
    }

		cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_FPC_FCH_INCLUDED_PRESENT;

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_FPC_FCH_INIT_SETPT_RC3"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.bitmask |= RRM_OAM_FPC_FCH_INIT_SETPT_RC3_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_FPC_FCH_INIT_SETPT_RC3"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.fpc_fch_init_setpt_rc3=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_FPC_FCH_INIT_SETPT_RC4"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.bitmask |= RRM_OAM_FPC_FCH_INIT_SETPT_RC4_PRESENT;

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_FPC_FCH_INIT_SETPT_RC4"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.fpc_fch_init_setpt_rc4=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_FPC_FCH_INIT_SETPT_RC5"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.bitmask |= RRM_OAM_FPC_FCH_INIT_SETPT_RC5_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_FPC_FCH_INIT_SETPT_RC5"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    fpc_fch_included.fpc_fch_init_setpt_rc5=lclTemp_value;
    }

        /* SPR 12673 START */
        if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_FPC_FCH_INIT_SETPT_RC11"),OAM_INVALID_STR))!= OAM_ZERO)
        {
            cell_config_ptr->ran_info.ncl_params.
                inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
                fpc_fch_included.bitmask |= RRM_OAM_FPC_FCH_INIT_SETPT_RC11_PRESENT;

            lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_FPC_FCH_INIT_SETPT_RC11"));
            cell_config_ptr->ran_info.ncl_params.
                inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
                fpc_fch_included.fpc_fch_init_setpt_rc11=lclTemp_value;
        }

        if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_FPC_FCH_INIT_SETPT_RC12"),OAM_INVALID_STR))!= OAM_ZERO)
        {
            cell_config_ptr->ran_info.ncl_params.
                inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
                fpc_fch_included.bitmask |= RRM_OAM_FPC_FCH_INIT_SETPT_RC12_PRESENT;

            lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_FPC_FCH_INIT_SETPT_RC12"));
            cell_config_ptr->ran_info.ncl_params.
                inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
                fpc_fch_included.fpc_fch_init_setpt_rc12=lclTemp_value;
        }
        /* SPR 12673 END */

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_T_ADD"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_T_ADD_INCLUDED_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_T_ADD"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    t_add=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_PILOT_INC"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    bitmask |= RRM_OAM_PILOT_INC_INCLUDED_PRESENT;
    

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_PILOT_INC"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].cell_specific_params.
    pilot_inc=lclTemp_value;
    }

		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                            "X_VENDOR_DAHO_INDICATION"),OAM_INVALID_STR))!= OAM_ZERO)
    {
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].
    presence_bitmask |= RRM_OAM_CDMA_CELL_DAHO_IND_PRESENT;

    lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_DAHO_INDICATION"));
			cell_config_ptr->ran_info.ncl_params.
    inter_rat_ncl.cdma2000_freq_cells[count].daho_indication=lclTemp_value;
            /* SPR 12932 FIX START */
            UInt32 instance_id = OAM_ZERO;

            if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
                                "X_VENDOR_INSTANCE_ID"),OAM_INVALID_STR))!= OAM_ZERO)
            {
                instance_id = oam_atoi((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_INSTANCE_ID"));

            }
            /*Coverity fix start : 78985 */
            if ((instance_id <= 0)||(instance_id>OAM_INSTANCE_ID_MAX_VAL)) {
                OAM_LOG(OAM, OAM_ERROR,
                        "Invalid value[%d] of X_VENDOR_INSTANCE_ID valid range is 1-32", instance_id);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }
            /*Coverity fix End : 78985 */

#ifdef OAM_SON_ENABLE /* +- SPR 13917 fix */
            if((lclTemp_value == OAM_ONE) && (cdma_daho_found_flag == OAM_FALSE))
            {
                cdma_daho_found_flag = OAM_TRUE;
                /*ut_bug_fix_15479_start CID 81792*/
                if ( instance_id > OAM_ZERO)
                {
                    SON_PROV_REQ.inter_rat_daho_bitmask |= OAM_ONE<<(instance_id - OAM_ONE);
                }
                /*ut_bug_fix_15479_end CID 81792*/
            }
            /* SPR 12932 FIX END */
#endif

    }
		/* SPR_10904_FIXED */
		if ((oam_strcmp((const char *)oam_xmlNodeGetContent(p_cell_spec_params->CDMA2000_2[count],
							"X_VENDOR_HO_STATUS"),OAM_INVALID_STR))!= OAM_ZERO)
		{
			lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
						p_cell_spec_params->CDMA2000_2[count],"X_VENDOR_HO_STATUS"));
			cell_config_ptr->ran_info.ncl_params.
				inter_rat_ncl.cdma2000_freq_cells[count].ho_status=lclTemp_value;
			cell_config_ptr->ran_info.ncl_params.
				inter_rat_ncl.cdma2000_freq_cells[count].presence_bitmask |= RRMCM_RMIF_CDMA_HO_STATUS_PRESENT;

		}
		/* SPR_10904_FIXED */

  
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*******************************************************************************
 *  FUNCTION NAME    : oam_populate_rrm_meas_gap_config_cell_based
 *  DESCRIPTION      : This function used to populate the geran freq cells
 *  PARAMETERS
 *      IN           : NONE
 *
 *      OUT          : NONE
 *
 *  RETURNS          :  OAM_SUCCESS on Success
 *                      OAM_FAILURE on Failure
 *******************************************************************************/

	oam_return_et
oam_populate_rrm_meas_gap_config_cell_based(oam_rrm_cell_config_req_t  *cell_config_ptr , XML_struct_cell_config *p_cell_spec_params)
{

	SInt32 lclTemp_value = OAM_ZERO;
	UInt8 *p_temp = OAM_NULL;


/*SPR 12898 FIX START */
    UInt32 eutran_meas_cnt = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();

    p_temp = oam_xmlNodeGetContent(p_cell_spec_params->InterFreq,"CarrierNumberOfEntries");
    if(oam_strcmp((Char8 *)p_temp, OAM_INVALID_STR)) 
    {
        eutran_meas_cnt = oam_atoi((char *)p_temp);
        if(eutran_meas_cnt == OAM_ZERO) 
        {
            OAM_LOG(OAM, OAM_DETAILED, "No instance of InterFreq Carrier is present");
            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }
    else
    {
        OAM_LOG(OAM, OAM_WARNING, "Mandatory Parameter CarrierNumberOfEntries is missing ");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /*SPR 12898 FIX END */

	p_temp = oam_xmlNodeGetContent(p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_EUTRAN_GAP_OFFSET_TYPE");
    if ((oam_strcmp((const char *)p_temp, OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_EUTRAN_GAP_OFFSET_TYPE"));
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.eutran_gap_offset_type=lclTemp_value;
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.bitmask |= RRM_OAM_E_UTRAN_GAP_OFFSET_TYPE_PRESENT;

		cell_config_ptr->operator_info.
    measurement_configuration.bitmask |= RRM_OAM_MEAS_GAP_CONFIG_PRESENT;

		cell_config_ptr->operator_info.
    bitmask |= RRM_OAM_MEASUREMENT_CONFIG_PRESENT;
}

	p_temp = oam_xmlNodeGetContent(p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_UTRAN_GAP_OFFSET_TYPE");
    if ((oam_strcmp((const char *)p_temp,OAM_INVALID_STR))!= OAM_ZERO)

{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_UTRAN_GAP_OFFSET_TYPE"));
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.utran_gap_offset_type=lclTemp_value;
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.bitmask |= RRM_OAM_UTRAN_GAP_OFFSET_TYPE_PRESENT;

		cell_config_ptr->operator_info.
    measurement_configuration.bitmask |= RRM_OAM_MEAS_GAP_CONFIG_PRESENT;

		cell_config_ptr->operator_info.
    bitmask |= RRM_OAM_MEASUREMENT_CONFIG_PRESENT;

}

   /* coverity 41196 */
	p_temp = oam_xmlNodeGetContent(p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_GERAN_GAP_OFFSET_TYPE");
    if ((oam_strcmp((const char *)p_temp,OAM_INVALID_STR))!= OAM_ZERO)
{

  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_GERAN_GAP_OFFSET_TYPE"));
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.geran_gap_offset_type=lclTemp_value;
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.bitmask |= RRM_OAM_GERAN_GAP_OFFSET_TYPE_PRESENT;

		cell_config_ptr->operator_info.
    measurement_configuration.bitmask |= RRM_OAM_MEAS_GAP_CONFIG_PRESENT;

		cell_config_ptr->operator_info.
    bitmask |= RRM_OAM_MEASUREMENT_CONFIG_PRESENT;

}


	p_temp = oam_xmlNodeGetContent(p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_CDMA2000_GAP_OFFSET_TYPE");
    if ((oam_strcmp((const char *)p_temp,OAM_INVALID_STR))!= OAM_ZERO)
{
  lclTemp_value=oam_atoi((const char *)oam_xmlNodeGetContent(
					p_cell_spec_params->Carrier[OAM_ZERO],"X_VENDOR_CDMA2000_GAP_OFFSET_TYPE"));
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.cdma2000_gap_offset_type=lclTemp_value;
		cell_config_ptr->operator_info.
    measurement_configuration.meas_gap_config.bitmask |= RRM_OAM_CDMA2000_GAP_OFFSET_TYPE_PRESENT;

		cell_config_ptr->operator_info.
    measurement_configuration.bitmask |= RRM_OAM_MEAS_GAP_CONFIG_PRESENT;

		cell_config_ptr->operator_info.
    bitmask |= RRM_OAM_MEASUREMENT_CONFIG_PRESENT;

}

OAM_FUN_EXIT_TRACE();
return OAM_SUCCESS;

}
/* Spr 11954 Fix Start
   Removed Redundant Code
   Spr 11954 Fix End*/
