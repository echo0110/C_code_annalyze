/********************************************************************************
* File Name    : oam_control_mgmt.c
* Description : This file contains the OAM Contol Module (CoM) specific changes.
*               
* Revision History :
* Date       Author       Reference       Comments
* 03/11/2012 Rohit Kumar                Control Module(CoM) code added
* copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*********************************************************************************/
#include<oam_control_mgmt.h>
#include<oam_fault_mgmt.h>
#include<oam_hm.h>
#include<oam_sim.h>
#include<oam_perf_mgmt.h>
#include<oam_sim_utils.h>

#ifdef OAM_UT_FRAMEWORK
extern const QSYSOP* os;
#endif
/* + SPR 17439 */
extern void oam_ecn_init(void);
/* - SPR 17439 */
extern oam_return_et oam_cm_fsm_init(void);
int g_no_of_arguments;

/****************************************************************************
 * Function Name  : oam_CoM_init_modules
 * Description    : This function will init all modules data structures
 *                  All oam modules needs to resgister their init() function here.
 * Inputs         : none
 * Outputs        : none
 * Returns        : none
******************************************************************************/
/* + SPR 17439 */
void 
oam_CoM_init_modules(void)
/* - SPR 17439 */
{

  OAM_FUN_ENTRY_TRACE();
  
  /* OAM CM FSM module parameter init */
  oam_cm_fsm_init();

  /* OAM HM module parameter init */
  oam_health_monitoring_init();
  /* OAM PM module parameter init */
   oam_pm_init();
   oam_ecn_init();
  OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_CoM_de_init_modules
 * Description    : This function will de-init all modules data structure
 *                  All oam modules needs to resgister their de-init() function here.
 * Inputs         : module_id :Module ID
 * Outputs        : none
 * Returns        : none
******************************************************************************/

void 
oam_CoM_de_init_modules(
  UInt16 module_id
  )
{
  OAM_FUN_ENTRY_TRACE();

  switch(module_id)
  {
	case OAM_MODULE_ID:
		OAM_LOG(OAM,OAM_DETAILED,"OAM_PROC_SUP_RESP from L3(RRC) MODULE_ID : %d",module_id);
		break;
	default:
		OAM_LOG(OAM,OAM_ERROR,"oam_CoM_de_init_modules, Unknown Module_id: %d",module_id);
		break;
   }

  OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_CoM_init_modules
 * Description    : This function is the entry point of OAM.
 *                  Interfaces
 * Inputs         : TR & Non-TR XML file
 * Outputs        : none
 * Returns        : none
******************************************************************************/
int main(
		int argc,
		char *arg[]
	)
{

#ifdef OAM_UT_FRAMEWORK
	/* Initializing CSPL library */
	qvInit(OAM_ZERO, os);
	oam_init_fault_mgmt();
	oam_aut_main(argc, arg);
	return OAM_ZERO;
#endif   
    /* SPR 23303 Fix Start */
	prov_req.log_level = OAM_WARNING;
    /* SPR 23303 Fix End */
	oam_init_fault_mgmt();

	oam_log_data_st log_data_local;
	oam_memset(&log_data_local,OAM_ZERO,sizeof(oam_log_data_st));
	OAM_SNPRINTF(log_data_local.filepath, (OAM_MAX_FILEPATH_SIZE - OAM_ONE),
			"%s/%s", OAM_LOG_FILE_PATH, OAM_LOG_FILE_NAME);
	if(OAM_RESULT_FAILURE ==
			oam_init_logging(OAM_LOG_CATEGORY_FILE,&log_data_local,prov_req.log_level,OAM_LOG_MAX_SIZE)) /*oam_init_logging  call*/
	{   
		oam_fprintf(stderr,"Failed to initialize Logger");
		oam_handle_invalid_startup_config();
	}  

	oam_init_msg_ipcs();    /* for  "ulimit -q unlimited" */

	if(OAM_FAILURE==oam_sim_main(argc, arg))
	{
		OAM_LOG(OAM,OAM_ERROR,"OAM SIM INIT fail");
		oam_CoM_de_init_modules(OAM_MODULE_ID);
		oam_handle_invalid_startup_config(); 
	}

	return OAM_ZERO;
}
