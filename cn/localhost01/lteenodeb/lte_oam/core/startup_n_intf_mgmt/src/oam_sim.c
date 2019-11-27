/* vim:ts=4:et:ai:ci:sw=4
*/
/********************************************************************************
 * File Name    : oam_sim.c
 * Description : This file contains the startup procedure for forking all the LTE
 *               layers (L2,L3,RRM,SON) according to the process table.
 * Revision History :
 * Date          Author             Reference             Comments
 * May,2012      Brijesh madolia    OAM Design Document   Initial Version
 * October,2012  Sunita			   Fault Managmenet      Initialization	 
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *********************************************************************************/

#include<oam_sim.h>
#include<oam_sim_utils.h>
#include<oam_xml_header.h>
#include<oam_l2_if.h>
#include<oam_fault_mgmt.h>
#include<oam_port_utils.h>
#include<sys/resource.h>
#include<oam_fm_alarm_defs.h>
/* SPR 13937 Fixed Start */
# include <sys/types.h>
# include <stdio.h>
# include <sys/wait.h>
# include <signal.h>
#include <oam_defines.h>
/* SPR 13937 Fixed End */
oam_ext_com_config_t comm_data;
oam_prov_req_t oam_prov_req;
extern oam_prov_req_t local_prov_req;
/* + SPR 17439 */
extern void oam_handle_invalid_startup_config(void);
/* - SPR 17439 */
extern oam_return_et oam_libxml_tr069_init(int argc,char *arg[]);
int g_no_of_arguments;
Char8** g_list_of_init_arguments;
extern char **environ;
extern int init_time_startup;
extern const QSYSOP* os;

/*SPR 21889 +- */


/****************************************************************************
 * Gloabal structure  : g_oam_sim_t
 * Description        : Process table describing all the process that will be
 *                      spawned by the SIM module
 *******************************************************************************/
oam_sim_table_t g_oam_sim_t = {{OAM_ZERO}};

/*SPR 21889 +- */

/****************************************************************************
 * Function Name  : oam_startup_init()
 * Description    : This function is the startup function of OAM it initializes
 *                  the ethernet interface and spawns the LTE stack layers.
 * Inputs         : p_error_code :Pointer to Error Code 
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_startup_init(oam_sim_error_info_et *p_error_code)
{
/* SPR 23215 CHANGES START*/
#if  defined(INTEL_NOCA_L1)
    return OAM_SUCCESS;
#else
/* SPR 23215 CHANGES END*/
    /*SPR 21889 Start*/
    /*SPR 21889 End */
    UInt8 process_number = OAM_ZERO;
    OAM_FUN_ENTRY_TRACE();

    /*Because oam will NOT wait for child processes, if children exception occur, 
    they will be remain as zombie process and will not generate core dump file.
    Here to ignore SIGCHLD signal to let child processes recycled by OS and core will be generated*/
    signal(SIGCHLD, SIG_IGN);

    OAM_LOG(OAM, OAM_INFO, "Root process pid = %d", oam_getpid());

    while(g_oam_sim_t.max_no_process > process_number)
    {
        if (OAM_FALSE == g_oam_sim_t.p_info[process_number].is_remote)
        {
            /* SPR 22071 Start */
            if (OAM_ZERO == oam_strncmp(OAM_TR069_BIN_NAME, g_oam_sim_t.p_info[process_number].name, MAX_BIN_NAME_LEN))
            {
                OAM_LOG(OAM, OAM_INFO, "Skipping TR when starting rest of the process");
            }
            else
            {
                g_oam_sim_t.p_info[process_number].pid = oam_fork();
                /* SPR 22071 End */
                switch(g_oam_sim_t.p_info[process_number].pid)
                {
                    case FORK_ERROR:
                        {
                            OAM_LOG(OAM,OAM_ERROR,"fork failed for Process %s Errno %d",
                                    g_oam_sim_t.p_info[process_number].name, errno);
                            OAM_ALARM(OAM_LAYER_STARTUP_FAILURE_ALARM_ID,
                                    CRITICAL,
                                    NOTIFY_NEW_ALARM,
                                    PROCESSING_ERROR_ALARM,
                                    SOFTWARE_PROGRAM_ERROR,
                                    EXPEDITED_ALARM,
                                    /* Spr 12880 Fix Start*/
                                    "eNodeB Statup Failure", OAM_NULL, OAM_INVALID_CELL_ID);
                            /* Spr 12880 Fix End*/
                            OAM_FUN_EXIT_TRACE();
                            *p_error_code = SIM_FORK_FAILED;
                            return  OAM_FAILURE;
                        }

                    case FORK_CHILD:
                        {
                            /* SPR 22071 End */
                            if(-1 ==  oam_execve(g_oam_sim_t.p_info[process_number].name,
                                        g_oam_sim_t.p_info[process_number].pp_args, environ))
                            {
                                /* SPR 14093 Fixed End */
                                OAM_LOG(OAM, OAM_ERROR, "Failed to start process %s. Error %s .Aborting!!!",
                                        g_oam_sim_t.p_info[process_number].name, oam_strerror(errno));
                                if (OAM_SUCCESS != oam_kill(oam_getpid(),OAM_SIG_KILL))
                                {
                                    OAM_LOG(OAM, OAM_ERROR,
                                            "Failed to kill PID %u.  Error %s", oam_getpid(),
                                            oam_strerror(errno));
                                }
                            }
                        }
                        /* SPR 22071 End */
                        break;

                    default:
                        break;
                }
            }
        }
        process_number++;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
/* SPR 23215 CHANGES START*/
#endif	
/* SPR 23215 CHANGES END*/
}

/****************************************************************************
 * Function Name  : oam_get_process_num()
 * Description    : This function gets the index of a process in g_oam_sim_t
 *                  structure
 * Inputs         : p_name :process name
 * Outputs        : none
 * Returns        : UInt16 process_index 
 * Note           : if(process_index > OAM_SIM_MAX_PROC) it implies process
 *                  name not found in g_oam_sim_t structure.                       
 ******************************************************************************/

UInt16 
oam_get_process_num(
		Char8* p_name
		)
{
	UInt16 process_number = OAM_ZERO;
	/*OAM_REVIEW_CHANGES*/
	for (process_number = OAM_ZERO; process_number < g_oam_sim_t.max_no_process; process_number++)
		/*OAM_REVIEW_CHANGES*/
	{
		if(OAM_ZERO == oam_strncmp(p_name, g_oam_sim_t.p_info[process_number].name, 
					oam_strlen(p_name)))
			break; 
	}   
	return process_number;
}

/****************************************************************************
 *Function Name  : oam_fork_process()
 * Description    : This function forks the process at index process_number
 *                  in the g_oam_sim_t structure
 * Inputs         : process_number :process number
 * Outputs        : p_error_code : Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ******************************************************************************/

oam_return_et
oam_fork_process
(
 UInt16 process_number, 
 oam_sim_error_info_et *p_error_code
 )
{
	UInt16 temp_process_number = OAM_ZERO;     

    if (OAM_FALSE == g_oam_sim_t.p_info[process_number].is_remote)
    {
        g_oam_sim_t.p_info[process_number].pid = oam_fork();
        switch(g_oam_sim_t.p_info[process_number].pid)
        {
            case FORK_ERROR:
                OAM_LOG(OAM,OAM_DETAILED,"fork failed");
                OAM_FUN_EXIT_TRACE();
                *p_error_code = SIM_FORK_FAILED;
                return  OAM_FAILURE;

            case FORK_CHILD:
                {
                    temp_process_number = process_number;
                    /* Coverity 71353 Change Start */
                    if (-1 == oam_execve(g_oam_sim_t.p_info[temp_process_number].name,
                                g_oam_sim_t.p_info[temp_process_number].pp_args,
                                environ))
                    {
                        OAM_LOG(OAM,OAM_DETAILED,"Process oam_execve Failure");
                    }
                    OAM_LOG(OAM,OAM_ERROR,"Error #%d in exec. Aborting!!!", errno);
                    OAM_FUN_EXIT_TRACE();
                    *p_error_code = SIM_GENERAL_ERROR_EXEC;
                    oam_killall();
                }
                break;
            default:
                OAM_LOG(OAM,OAM_DETAILED,"Default case");
                process_number++;
        }
    }
    else
    {
        /* Start binary on remote location */
    }
	return  OAM_SUCCESS;
}

#ifdef OAM_SON_ENABLE
/****************************************************************************
 * Function Name  : oam_startup_init_rrm_son()
 * Description    : This function is the startup function of OAM it initializez
 *                  the ethernet interface and spawns the LTE stack layers.
 * Inputs         : none
 * Outputs        : p_error_code : Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_startup_init_rrm_son
(
 oam_sim_error_info_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 process_number = OAM_ZERO;
	process_number = oam_get_process_num(OAM_SON_BIN_NAME);
	if (OAM_FAILURE == oam_fork_process(process_number, p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR, "Process %s could not be forked",
				OAM_RRM_BIN_NAME);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
#ifdef SPAWN_RRM
	process_number = oam_get_process_num(OAM_RRM_BIN_NAME);
	if (OAM_FAILURE == oam_fork_process(process_number, p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR, "Process %s could not be forked",
				OAM_RRM_BIN_NAME);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
#endif

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

#endif
/****************************************************************************
 * Function Name  : oam_spawn_tr_069_proc 
 * Description    : This function spawns the tr069 Process 
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ******************************************************************************/

/* + SPR 17439 */
oam_return_et 
oam_spawn_tr_069_proc(
void
)
/* + SPR 17439 */
{

	/* SPR 13937 Fixed Start */
	oam_return_et ret_val = OAM_SUCCESS; /* Coverity 71113 Change */
	/* SPR 13937 Fixed End */
	UInt32 temp_proc_num;
	char *const ps_env[] = {"PATH=$PATH:$PWD",OAM_ZERO};
	UInt8 i = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	for (i = OAM_ZERO; i < g_oam_sim_t.max_no_process; i++)
    {
        /* SPR 22071 End */
        if (OAM_ZERO == oam_strncmp(OAM_TR069_BIN_NAME, g_oam_sim_t.p_info[i].name, MAX_BIN_NAME_LEN))
        /* SPR 22071 Start */
        {
            if (OAM_TRUE != g_oam_sim_t.p_info[i].is_remote)
            {
                g_oam_sim_t.p_info[i].pid = oam_fork();

                if(FORK_ERROR == g_oam_sim_t.p_info[i].pid) 
                {
                    OAM_LOG(OAM,OAM_DETAILED,"Process Fork Failure"); 
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;

                }
                else if(FORK_CHILD == g_oam_sim_t.p_info[i].pid)
                {
                    temp_proc_num = i;
                    OAM_LOG(OAM,OAM_INFO,"TR069 Process ID : %d",oam_getpid()); 
                    /* SPR 13937 Fixed Start */
                    if(-1 ==  oam_execve(g_oam_sim_t.p_info[temp_proc_num].name,
                                g_oam_sim_t.p_info[temp_proc_num].pp_args, ps_env))/* Coverity 71115 Change */
                    {

                        setsid();
                        OAM_LOG(OAM,OAM_ERROR,"Process oam_execve Failure. Errno %d", errno);
                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;

                    }
                    /* SPR 13937 Fixed End */
                } 
            }
            else
            {
                /* TR069 needs to be executed on remote location */
            }
            break;
        }
    }

    if (i == g_oam_sim_t.max_no_process)
    {
        OAM_LOG(OAM,OAM_ERROR,"%s binary not found in xml file", OAM_TR069_NICK_NAME);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

	OAM_FUN_EXIT_TRACE();
	/* SPR 13937 Fixed Start */
	return ret_val;
	/* SPR 13937 Fixed End */
}


/******************************************************************************
 *   FUNCTION NAME   : oam_sim_main
 *   DESCRIPTION     : This function is main function
 *   Inputs          : argc :no of command line arguements
 *                   : arg[] :array of command line arguements   
 *   Outputs         : NONE
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

	oam_return_et
oam_sim_main(SInt32 argc, Char8 *arg[])
{
	g_no_of_arguments =  argc;
	g_list_of_init_arguments = arg;
	oam_return_et retVal = OAM_FAILURE;


	/* Initializing CSPL library */
	qvInit(OAM_ZERO, os);

	if(argc < OAM_MIN_COMMANDLINE_ARGS || argc > OAM_MAX_COMMANDLINE_ARGS) {
		OAM_LOG(OAM, OAM_ERROR,"Invalid commandline arguments."
				"USAGE:%s <Proprietary XML config file> "
				"<TR196 XML config file> " "[Core Number]", arg[0]);
		return retVal;
	}


#if defined(OAM_EVM_TARGET) && (AUTOMATED_STARTUP)
	/* handle vendor specific startup logic */
	retVal = oam_vendor_specific_startup();
	if(OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_CRITICAL, "startup failed, OAM will be aborted");
		oam_reset_reason_code(OAM_STARTUP_FAILURE); 
		return retVal;
	}

#endif


	/* Removing redundant code for TR069 specific parameter reading */

	retVal = oam_libxml_init(g_no_of_arguments, g_list_of_init_arguments);
	if(OAM_SUCCESS == retVal) {
		OAM_LOG(OAM, OAM_INFO, "Parameters populated succesfully");
	}
	else {
		OAM_LOG(OAM, OAM_CRITICAL, "Parameters population failed");
		oam_reset_reason_code(PARAMETER_POPULATION_FAILURE); 
		oam_handle_invalid_startup_config();
		return retVal;
	}

/*SPR 21534 Start */
#if  defined(OAM_FREESCALE_TARGET) || defined(OAM_EVM_TARGET)
/*SPR 21534 End */
# else
	oam_eth_intf_up();
#endif
	/* SPR 9233 Fix Start*/ 
	if(oam_prov_req.fm_cntxt.isRaiseConfigAlarm != OAM_ONE)
	{
		if(OAM_TRUE == oam_prov_req.oam_tr069_req.tr069_mode)
		{ 
			init_time_startup = OAM_ONE; 
			if(OAM_FAILURE == oam_spawn_tr_069_proc())
			{
				/* SPR 13937 Fixed Start */
				oam_prov_req.fm_cntxt.isRaiseConfigAlarm  = OAM_ONE;
				OAM_LOG(OAM,OAM_ERROR,"UNABLE TO SPAWN TR069 Process. "
				    "TR069 Binary are not present in bin folder ");
                /*Coverity fix start : 72084 */
                if(OAM_FAILURE ==  oam_kill(getppid(),OAM_SIG_KILL))
                {

                    OAM_LOG(OAM,OAM_ERROR,"UNABLE TO Kill Process");
                }
                /*Coverity fix End : 72084 */
                /*SPR 21889 Start*/
				oam_killall();
                /*SPR 21889 End */
				/* SPR 13937 Fixed End */
				return OAM_FAILURE;
			}
		}
        /* SPR 16975 FIX START */
        else if(ENODEB_ADMIN_STATE_LOCKED == oam_prov_req.system_status.admin_state)
        {
            OAM_LOG(OAM,OAM_ERROR," Admin State LOCKED not spawning layers");
        }
        /* SPR 16975 FIX END */
		else
		{
			if(oam_prov_req.fm_cntxt.isRaiseConfigAlarm != OAM_ONE)
			{
				init_time_startup = OAM_ZERO; 
				oam_proceed_with_init_flow(argc, arg);
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Configuration Failure Occurs not spawning layers");
	}
	/* SPR 9233 Fix End*/
#ifdef OAM_SON_ENABLE 
	/*OAM_REVIEW_CHANGES*/
	oam_memcpy(&(oam_prov_req.oam_igd_params.nw_scan_status.scan_status),
			oam_nw_scan_status[OAM_ZERO],
			MAX_NW_SCAN_STATUS_STR_SIZE); 
	/*OAM_REVIEW_CHANGES*/
#endif
	oam_memcpy(&local_prov_req, &oam_prov_req, sizeof(oam_prov_req_t));

	oam_multi_thd_init();

	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_init_msg_ipcs
 * Description    : This function makes message queue limit unlimited
 * Inputs         : none
 * Outputs        : makes message queue limit unlimited
 * Returns        : none
 ******************************************************************************/
void oam_init_msg_ipcs(void)
{
	/* for  "ulimit -q unlimited" */
	struct rlimit rlim = {RLIM_INFINITY, RLIM_INFINITY};
	if (OAM_SETRLIMIT(RLIMIT_MSGQUEUE, &rlim) != 0)
	{
		OAM_LOG(OAM,OAM_ERROR,"setrlimit() failed");
	}
}

/******************************************************************************
 *   FUNCTION NAME   : oam_killall
 *   DESCRIPTION     : This function will kill all processes.
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

void oam_killall()
{
    UInt8 i = OAM_ZERO;
    Char8   cmd[OAM_MAX_COMMANDLINE_ARGS];
    OAM_FUN_ENTRY_TRACE();
    oam_bool_et report_exist = OAM_FALSE;

    for (i = OAM_ZERO; i < g_oam_sim_t.max_no_process; i++)
    {
        if (OAM_FALSE == g_oam_sim_t.p_info[i].is_remote)
        {
            /* Send kill request for remote process */
            oam_snprintf(cmd, OAM_MAX_COMMANDLINE_ARGS, "kill -9 %s", g_oam_sim_t.p_info[i].name);
            system(cmd);
        }
        else if (OAM_REMOTE_SPAWN_SUCCESS == g_oam_sim_t.p_info[i].remote_spawn_status)
        {
            /* Send kill request for remote process */
            report_exist = OAM_TRUE;
        }
    }
    if (OAM_FALSE != report_exist)
    {
        oam_send_kill_to_all_agent();
    }
    /* This binary is not forked by OAM hence not kept in the process table */
    oam_snprintf(cmd, OAM_MAX_COMMANDLINE_ARGS, "kill -9 %s", OAM_LIBBINREAD_BIN_NAME);
    oam_reset_reason_code(OAM_INTERNAL_FAILURE); 	

    /* Not saving this timer as we don't want to stop it because it will be trigger for exit */
    oam_start_new_timer(OAM_SUB_MODULE_AGENT, OAM_MODULE_ID, OAM_SHUTDOWN_TIMER, OAM_SHUTDOWN_TIME_OUT * OAM_THOUSAND,
            OAM_NULL, OAM_ZERO,(oam_bool_et)OAM_ZERO);
}

/*SPR 21889 +- */





/*****************************************************************************
 * Function Name  : oam_update_enodeb_product_info() 
 * Description    : This function updates the product info read by SDM Module 
 * Inputs         : none 
 * Outputs        : none
 * Returns        : none 
 ***************************************************************************/
	void
oam_update_enodeb_product_info()
{
	FILE * fptr = OAM_NULL;
	UInt8 read_buf[MAX_PRODUCT_INFO_LENGTH]= {OAM_ZERO};
	UInt8 ProductInfo[MAX_PRODUCT_INFO_LENGTH]= {OAM_ZERO};
	UInt16 len = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	OAM_LOG(OAM,OAM_DETAILED,"Updating Product Info read from oam_version_info.txt");

	fptr = oam_fopen ("../cfg/oam_version_info.txt", "r");
	if(OAM_NULL != fptr)
	{
		while (OAM_FGETS((char *)read_buf,MAX_PRODUCT_INFO_LENGTH,fptr))
		{
			OAM_LOG(OAM,OAM_DETAILED,"%s",read_buf);
			if(snscanf((const char *)read_buf,sizeof(read_buf),"ManufacturerOUI=%s",ProductInfo) > OAM_ZERO)
			{
				OAM_LOG(OAM,OAM_DETAILED,"ManufacturerOUI read from oam_version_info.txt= %s",ProductInfo);
				len = sizeof(oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI);
				oam_memset(oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,OAM_ZERO,len);
				len = oam_strlen((char *)ProductInfo);
				oam_memcpy(oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,ProductInfo,len);
				ProductInfo[OAM_ZERO] = '\0'; 
				oam_update_device_info("ManufacturerOUI", (char*)oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI);
			}
			if(snscanf((const char *)read_buf,sizeof(read_buf),"ProductClass=%s",ProductInfo) > OAM_ZERO)
			{
				OAM_LOG(OAM,OAM_DETAILED,"ProductClass read from oam_version_info.txt= %s",ProductInfo);
				len = sizeof(oam_prov_req.oam_igd_params.device_info_params.ProductClass);
				oam_memset(oam_prov_req.oam_igd_params.device_info_params.ProductClass,OAM_ZERO,len);
				len = oam_strlen((char *)ProductInfo);
				oam_memcpy(oam_prov_req.oam_igd_params.device_info_params.ProductClass,ProductInfo,len);
				ProductInfo[OAM_ZERO]= '\0';
				oam_update_device_info("ProductClass", (char*)oam_prov_req.oam_igd_params.device_info_params.ProductClass);
			}
			if(snscanf((const char *)read_buf,sizeof(read_buf),"HardwareVersion=%s",ProductInfo) > OAM_ZERO)
			{
				OAM_LOG(OAM,OAM_DETAILED,"HardwareVersion read from oam_version_info.txt= %s",ProductInfo);
				len = sizeof(oam_prov_req.oam_igd_params.device_info_params.HardwareVersion);
				oam_memset(oam_prov_req.oam_igd_params.device_info_params.HardwareVersion,OAM_ZERO,len);
				len = oam_strlen((char *)ProductInfo);
				oam_memcpy(oam_prov_req.oam_igd_params.device_info_params.HardwareVersion,ProductInfo,len);
				ProductInfo[OAM_ZERO]= '\0';
				oam_update_device_info("HardwareVersion", (char*)oam_prov_req.oam_igd_params.device_info_params.HardwareVersion);
			}
			if(snscanf((const char *)read_buf,sizeof(read_buf),"SoftwareVersion=%s",ProductInfo) > OAM_ZERO)
			{
				OAM_LOG(OAM,OAM_DETAILED,"SoftwareVersion read from oam_version_info.txt = %s",ProductInfo);
				len = sizeof(oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion);
				oam_memset(oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion,OAM_ZERO,len);
				len = oam_strlen((char *)ProductInfo);
				oam_memcpy(oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion,ProductInfo,len);
				ProductInfo[OAM_ZERO]= '\0';
				oam_update_device_info("SoftwareVersion", (char*)oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion);
				break;
			}
		}
		OAM_FCLOSE(fptr);
	}
	else 
	{
		OAM_LOG(OAM,OAM_WARNING,"Failure in opening oam_version_info.txt");
	}
	OAM_FUN_EXIT_TRACE();
}
