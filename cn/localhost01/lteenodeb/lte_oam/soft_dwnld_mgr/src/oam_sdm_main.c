/*******************************************************************************
 * File Name   : oam_sdm_main.c
 *
 * Description : This file contains main function and global data
 *                description.
 * Revision History:
 *
 *   Date        Author                Reference          Comments
 *   -----       ------                 ---------         --------
 * January,2013  Vinod Vyas     OAM Design Document  Initial Version
 * MAY 2014      Shalu Sagar    Klockwork Fix
 * 
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "oam_sdm_main.h"
/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End */
/* SPR 14655 Fix Start */
#include <sys/wait.h>
/* SPR 14655 Fix End */

/*******************************************************************************
 * Exported Variables
 *******************************************************************************/
extern UInt8 time_buffer[TIME_SIZE];
extern UInt32 g_sdm_poll_counter;
extern char **environ;
/*******************************************************************************
  Private Types
 *******************************************************************************/
#ifdef OAM_EVM_TARGET    
UInt8 g_bin_backup_array[MAX_NO_OF_BACKUP][MAX_BACKUP_STR_LENGTH] = {{OAM_SDM_ZERO}};
#else
UInt8 g_bin_backup_array[MAX_NO_OF_BACKUP][MAX_BACKUP_STR_LENGTH] = {{OAM_SDM_ZERO},
                                                                    {OAM_SDM_ZERO}};
#endif
UInt32 backup_counter = OAM_SDM_ZERO;
UInt32 g_oam_sdm_event_ongoing = OAM_SDM_FALSE;
UInt8 g_file_name[MAX_BACKUP_STR_LENGTH] = {OAM_SDM_ZERO};
/******************************************************************************
  Global Types
 ******************************************************************************/
LOCAL UInt32 oam_sdm_poll_time = OAM_SDM_ZERO;
LOCAL UInt32 oam_num_of_backup = MAX_NO_OF_BACKUP;
UInt16 g_flag_for_oam_active_ind = OAM_SDM_FALSE;
SInt32 g_no_of_arguments;
Char8 **g_list_of_init_arguments;
prov_request_t prov_req;
LOCAL version_info_t oam_sdm_backup_info;
UInt8 g_oam_process_number = OAM_SDM_ZERO;
sdm_ext_com_config_t comm_data;
/*****************************************************************************
 * Gloabal structure  : g_oam_sdm_t
 * Description        : Process table describing all the process that will be
 *                      spawned by the lte_oam
 *****************************************************************************/
struct _oam_sdm_sim_table_t g_oam_sdm_t = {OAM_SDM_ZERO};

/****************************************************************************
 * Function Name  : main
 * Description    : This function is the entry point of OAM_SDM.
 *                  Interfaces
 * Inputs         : Command line arguments.
 * Outputs        : none
 * Returns        : none
 ******************************************************************************/

int main (int argc, char* arg[])
{
 
    SInt32 arg_count = OAM_SDM_ZERO;
    /*SPR 9435 Fix Start*/
    cpu_set_t       cpuSetSch;
    unsigned int    coreNum = OAM_SDM_ZERO;

    if ((argc < OAM_SDM_THREE)||(argc > OAM_SDM_FOUR))
    {
        oam_sdm_fprintf (stderr,
                "\n Insufficent Parameter.\n Please Provide XML configuration files.");
        oam_sdm_fprintf (stderr, "\n USAGE:");
        oam_sdm_fprintf (stderr,
                "\n %s <Proprietary XML Configuration File> <TR196 XML Configuration File><CoreNum For OAM & SDM(Optional)>\n\n",
                arg[OAM_SDM_ZERO]);
        return OAM_SDM_FAILURE;
    }

    if ((OAM_SDM_NULL != arg[OAM_SDM_THREE]) && (argc > OAM_SDM_THREE))
    {
	    coreNum = oam_sdm_atoi(arg[OAM_SDM_THREE]);
	    if (coreNum >= (OAM_SDM_EIGHT * sizeof(cpuSetSch)))
	    {
		    oam_sdm_fprintf(stderr,"Invalid core number[%d]\n", coreNum);
		    return 0;
	    }
	    OAM_SDM_CPU_ZERO(&cpuSetSch);
	    OAM_SDM_CPU_SET(coreNum ,&cpuSetSch);
	    pid_t p = OAM_SDM_ZERO;
	    if (OAM_SDM_ZERO != oam_sdm_sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch))
	    {
		    oam_sdm_fprintf(stderr, " oam_sdm_sched_setaffinity Returned Error in SDM %s Core not Binded", arg[3]);
		    oam_sdm_exit(0);
	    }
    }
    else {
	    oam_sdm_fprintf(stderr, "OAM & SDM Will Bind to Default Core");  
    }
    /*SPR 9435 Fix End*/

    /* INFO FOR LOGGING */
    prov_req.log_level = OAM_SDM_DETAILED;
    prov_req.p_out = oam_sdm_fopen ("oam_sdm_logs.log", "w+");

    if (OAM_SDM_NULL == prov_req.p_out)
    {
        oam_sdm_fprintf (stderr,
                "\n UNABLE TO OPEN 'oam_sdm_logs.log' For OAM_SDM LOGS");
        oam_sdm_exit (1);
    }
    
    g_no_of_arguments = argc;
        g_list_of_init_arguments = (char **)SDM_CALLOC(argc+1 , sizeof(char *));

    if(OAM_SDM_NULL == g_list_of_init_arguments)
    {
       oam_sdm_fprintf (stderr,"\n Memory Allocation Failed !!!!!");
       oam_sdm_exit(1);
    }
    for (arg_count = OAM_SDM_ZERO; arg_count<g_no_of_arguments; arg_count++) 
    {
        if(oam_sdm_strnlen(arg[arg_count],MAX_STR_LEN) < MAX_STR_LEN)
        {   
            g_list_of_init_arguments[arg_count] = (char *)SDM_MALLOC(oam_sdm_strlen(arg[arg_count])+OAM_SDM_ONE);
            if(OAM_SDM_NULL != g_list_of_init_arguments[arg_count])
            {
                oam_sdm_memcpy(g_list_of_init_arguments[arg_count],arg[arg_count],oam_sdm_strlen(arg[arg_count])); 
            } 
            else 
            {
              SInt32 arg_count_1 = OAM_SDM_ZERO;
              if(arg_count > OAM_SDM_ZERO)
              {    
                  while(arg_count_1 <= arg_count)
                  {     
                      SDM_FREE(g_list_of_init_arguments[arg_count_1]);
                      arg_count_1++;
                  }
              }
              SDM_FREE(g_list_of_init_arguments); 
              oam_sdm_fprintf (stderr,"\n argument Validation failed");
              oam_sdm_exit(1);
            } 
        }
    }
    if (OAM_SDM_FAILURE == oam_sdm_main ())
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "SDM initialization failed");
    }

    return OAM_SDM_ZERO;

}

/*****************************************************************************
 * Function Name  : oam_sdm_main
 * Description    : Function to handle spawning of OAM process. 
 * Inputs         : TR & Non-TR XML file
 * Outputs        : none
 * Returns        : none
 ******************************************************************************/
/* + SPR 17439 */
    oam_sdm_return_et
oam_sdm_main (void)
/* - SPR 17439 */
{
    OAM_SDM_FUN_ENTRY_TRACE ();

    if (OAM_SDM_FAILURE == oam_sdm_get_available_backup_info())
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Failed to retrive available backups Info !!!");
    }
    /*Spawn OAM Process */
    if (OAM_SDM_FAILURE == oam_sdm_spawn_lte_oam ())
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "UNABLE TO SPAWN OAM Process");
        return OAM_SDM_FAILURE;
    }
  
    /* In case receive a request to stop (kill -TERM or kill -INT) */
    oam_sdm_register_signal_handler(SDM_SIGTERM, oam_sdm_signal_handler); /*porting changes*/
    oam_sdm_register_signal_handler(SDM_SIGINT, oam_sdm_signal_handler);  /*porting changes*/
 
    oam_sdm_single_thd_init ();

    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_sdm_spawn_lte_oam 
 * Description    : This function spawns the oam Process 
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ******************************************************************************/

/* + SPR 17439 */
    oam_sdm_return_et
oam_sdm_spawn_lte_oam (void)
/* - SPR 17439 */
{
    UInt32 proc_num;
    proc_num = OAM_SDM_ZERO;			/* process table entry for oam Process */
    Int retVal = OAM_SDM_ZERO; 
    OAM_SDM_FUN_ENTRY_TRACE ();
    
    /* Change The First Argument of g_list_of_init_arguments to "lte_oam" as buffer used to Spawn lte_oam further */
    /* KLOCWORK warning fix start */
    oam_sdm_strncpy(g_list_of_init_arguments[0], "lte_oam",sizeof("lte_oam"));

    g_oam_sdm_t.p_info[proc_num].pid = oam_sdm_fork ();

    if (FORK_ERROR == g_oam_sdm_t.p_info[proc_num].pid)
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Process Fork Failure");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    else if (FORK_CHILD == g_oam_sdm_t.p_info[proc_num].pid)
    {
        OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "Process ID : %d",
                oam_sdm_getpid ());
        retVal = oam_sdm_execve ("lte_oam", g_list_of_init_arguments,
                                 environ);/* Coverity FIX 34785*/
        /* KLOCWORK warning fix end */
        if(-1 == retVal) {
            if(ENOENT == errno) {
                OAM_SDM_LOG(SDM, OAM_SDM_ERROR,
                        "ERROR:lte_oam not found. Aborting!!!");
                OAM_SDM_FUN_EXIT_TRACE();
            }
            else if(ENOMEM == errno) {
                OAM_SDM_LOG(SDM, OAM_SDM_ERROR,
                        "ERROR:Not enough memory.Aborting!!!");
                OAM_SDM_FUN_EXIT_TRACE();
            }
            else {
                OAM_SDM_LOG(SDM, OAM_SDM_ERROR,
                        "ERROR:Error #%d in exec. Aborting!!!", errno);
            }

            OAM_SDM_FUN_EXIT_TRACE();
            return OAM_SDM_FAILURE;
        }
    }

    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

/******************************************************************************
 * Function Name  : oam_sdm_upgarde_req_handler  
 * Description    : This function handles file download Indication from OAM
 * Inputs         : p_msg : Pointer to message Buffer
 * Output         : 
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 *******************************************************************************/
    oam_sdm_return_et
oam_sdm_handle_upgarde_req(UInt8 * p_msg)
{
    UInt8 file_type = OAM_SDM_ZERO;
    UInt32 file_size = OAM_SDM_ZERO;
    UInt16 file_name_length = OAM_SDM_ZERO;
    UInt16 file_des_len = OAM_SDM_ZERO;
    UInt8 file_des[OAM_SDM_DEST_FILE_SIZE] = { OAM_SDM_ZERO };
    UInt32 counter = OAM_SDM_ZERO;
    UInt16 transaction_id = OAM_SDM_ZERO;
    UInt8 parameter_node = OAM_SDM_ZERO;
    UInt16 oui_length = OAM_SDM_ZERO;
    UInt16 product_class_length = OAM_SDM_ZERO;
    UInt16 hw_version_length = OAM_SDM_ZERO;
    UInt16 soft_version_length = OAM_SDM_ZERO;
    OAM_SDM_FUN_ENTRY_TRACE ();


    counter += OAM_SDM_INTF_HEADER_SIZE;
    transaction_id = oam_sdm_get_word_from_header ((UInt8 *) (p_msg) + OAM_SDM_ZERO);

    OAM_SDM_LOG (SDM, OAM_SDM_INFO, "Transaction ID: %d", transaction_id);

    /*Reserved Field */
    counter += sizeof (UInt32);

    oam_sdm_memcpy (&file_type, p_msg + counter, sizeof (UInt8));

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "FILE TYPE = %d", file_type);
    counter += sizeof (UInt8);

    counter += sizeof (UInt8) + sizeof (UInt8) + sizeof (UInt8);	/* Reserved Field */
    file_size |= (UInt32) ((UInt8 *) p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_TWENTY_FOUR;
    file_size |= (UInt32) ((UInt8 *) p_msg + counter)[OAM_SDM_ONE]  << OAM_SDM_SIXTEEN;
    file_size |= (UInt32) ((UInt8 *) p_msg + counter)[OAM_SDM_TWO]  << OAM_SDM_EIGHT;
    file_size |= (UInt32) ((UInt8 *) p_msg + counter)[OAM_SDM_THREE];

    counter += sizeof (UInt32);

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "FILE SIZE = %d", file_size);

    file_name_length = file_name_length | (UInt16) ((p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
    file_name_length = file_name_length | (UInt16) ((p_msg + counter)[OAM_SDM_ONE]);

    counter += sizeof (UInt16) + sizeof (UInt16);

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "FILE NAME LENGTH = %d",
            file_name_length);

    oam_sdm_memcpy (g_file_name, p_msg + counter, file_name_length);
    counter += file_name_length;


    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "FILE NAME = %s", g_file_name);

    file_des_len = p_msg[counter];
    counter += sizeof (UInt8);
    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "FILE DESCRIPTION LENGTH = %d",
            file_des_len);

    counter += sizeof (UInt8) + sizeof (UInt16);
    oam_sdm_memcpy (file_des, p_msg + counter, file_des_len);
    counter += file_des_len;

    parameter_node = p_msg[counter];
    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "No of Additional Parameters = %d",
            parameter_node);
    counter += sizeof (UInt8);

    counter += sizeof (UInt8) + sizeof (UInt8) + sizeof (UInt8);	/* Reserved Field */

    oui_length= oui_length| (UInt16) ((p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
    oui_length= oui_length| (UInt16) ((p_msg + counter)[OAM_SDM_ONE]);

    counter += sizeof (UInt16) + sizeof (UInt16);

    oam_sdm_memcpy (oam_sdm_backup_info.oui, p_msg + counter, oui_length);
    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "%s", oam_sdm_backup_info.oui);
    counter += oui_length;

    product_class_length = product_class_length| (UInt16) ((p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
    product_class_length=  product_class_length| (UInt16) ((p_msg + counter)[OAM_SDM_ONE]);

    counter += sizeof (UInt16) + sizeof (UInt16);
    oam_sdm_memcpy (oam_sdm_backup_info.product_class, p_msg + counter, product_class_length);

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "%s", oam_sdm_backup_info.product_class);
    counter += product_class_length;

    hw_version_length= hw_version_length| (UInt16) ((p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
    hw_version_length=  hw_version_length| (UInt16) ((p_msg + counter)[OAM_SDM_ONE]);

    counter += sizeof (UInt16) + sizeof (UInt16);
    oam_sdm_memcpy (oam_sdm_backup_info.hw_version, p_msg + counter, hw_version_length);

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "%s", oam_sdm_backup_info.hw_version);
    counter += hw_version_length;

    soft_version_length = soft_version_length| (UInt16) ((p_msg + counter)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
    soft_version_length =  soft_version_length| (UInt16) ((p_msg + counter)[OAM_SDM_ONE]);

    counter += sizeof (UInt16) + sizeof (UInt16);

    oam_sdm_memcpy (oam_sdm_backup_info.soft_version, p_msg + counter, soft_version_length);

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "%s", oam_sdm_backup_info.soft_version);
    counter += soft_version_length;

    if ((OAM_SDM_ZERO == file_name_length)) 

    {
        OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Unable to access file");
        if (OAM_SDM_FAILURE ==
                oam_sdm_send_upgrade_start_resp (OAM_SDM_RESULT_FAILURE,
                    OAM_SDM_ERR_FILE_ACCESS_FAILURE,
                    transaction_id,
                    (UInt8 *) "File Access Failure"))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Error in sending Response");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;

        }
    }
    else
    {
        if (OAM_SDM_FAILURE ==
                oam_sdm_send_upgrade_start_resp (OAM_SDM_RESULT_SUCCESS,
                    OAM_SDM_ZERO, transaction_id,
                    (UInt8 *) "File Access Success"))
        {
            OAM_SDM_LOG (SDM,OAM_SDM_ERROR, "Error in sending Response");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
        OAM_SDM_LOG (SDM,OAM_SDM_DETAILED,"File Access Success Response sent to OAM");
    }
    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

/****************************************************************************
 *  Function Name  : oam_sdm_existing_software_backup 
 *  Description    : This function Takes the existing software Backup 
 *  Inputs         : none     
 *  Outputs        : none
 *  Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
    oam_sdm_return_et
oam_sdm_existing_software_backup ()
{
    UInt8 command[OAM_SDM_TAR_SW_CMD_SIZE]             = { OAM_SDM_ZERO };
    UInt8 command1[OAM_SDM_BKP_SW_CMD_SIZE]            = { OAM_SDM_ZERO };
    Char8 backup_file_name[OAM_SDM_BKP_FILE_NAME_SIZE] = { OAM_SDM_ZERO };
    /* coverity 35994 */
    oam_sdm_return_et retVal = OAM_SDM_SUCCESS;

#ifndef OAM_EVM_TARGET    
    char *filelist = "../bin/lte_oam ../bin/lte_oamCli ../bin/lte_oamSnmpAgt ../bin/lte_oamSDM ../bin/lte_tr069 ../bin/lteLayer2 ../bin/lte_son ../bin/lte_rrm ../bin/rrc_entity";
#else    
    char *filelist = "../bin/lte_oam ../bin/lte_oamCli ../bin/lte_oamSnmpAgt ../bin/lte_oamSDM ../bin/rcm ../bin/lte_son ../bin/lte_rrm ../bin/rrc_entity ../bin/dsp-core*";
    UInt8 command_cp[OAM_SDM_BKP_SW_CMD_SIZE]="cp -rf /lib/firmware/dsp-core0.out /lib/firmware/dsp-core1.out /lib/firmware/dsp-core2.out /lib/firmware/dsp-core3.out ../bin/";
    UInt8 command_rm[OAM_SDM_BKP_SW_CMD_SIZE]="rm -rf ../bin/dsp-core0.out ../bin/dsp-core1.out ../bin/dsp-core2.out ../bin/dsp-core3.out";
    if (OAM_SDM_STD_ERROR == oam_sdm_system ((char *)command_rm))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                "Error in creating backup of existing Software");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    if (OAM_SDM_STD_ERROR == oam_sdm_system ((char *)command_cp))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                "Error in creating backup of existing Software");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }

#endif            
    char *cfg_filelist = "../cfg";
    OAM_SDM_FUN_ENTRY_TRACE ();

    oam_sdm_create_backup_file_name(backup_file_name);
    oam_sdm_create_time_stamp ();

    oam_sdm_snprintf ((char *)command, OAM_SDM_TAR_SW_CMD_SIZE,
            "tar -cvf ../backup/%s_%s.tar %s %s", backup_file_name,time_buffer, filelist, cfg_filelist);

    /* Storing Backup In Round Robin Fashion */
    if (backup_counter < oam_num_of_backup)
    {
        if (*g_bin_backup_array[backup_counter] != OAM_SDM_ZERO)
        {
            oam_sdm_snprintf ((char *)command1, OAM_SDM_BKP_SW_CMD_SIZE, "../backup/%s",
                    g_bin_backup_array[backup_counter]);
            if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)command1))
            {
                OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                        "Error in Removing old backup");
                OAM_SDM_FUN_EXIT_TRACE ();
                return OAM_SDM_FAILURE;
            }
        }
        oam_sdm_sprintf ((char *)&g_bin_backup_array[backup_counter], "%s_%s.tar",
                backup_file_name,time_buffer);
    }
    else if (backup_counter >= oam_num_of_backup)
    {
        backup_counter = OAM_SDM_ZERO;

        oam_sdm_snprintf ((char *)command1, OAM_SDM_BKP_SW_CMD_SIZE, "../backup/%s",
                g_bin_backup_array[backup_counter]);

        if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)command1))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                    "Error in Removing old backup");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
        oam_sdm_sprintf((char *)&g_bin_backup_array[backup_counter],"%s_%s.tar",
                backup_file_name ,time_buffer);

    }
    oam_sdm_snprintf ((char *)command, OAM_SDM_TAR_SW_CMD_SIZE,
            "tar -cvf ../backup/%s_%s.tar %s %s", backup_file_name,time_buffer, filelist, cfg_filelist);
    if (OAM_SDM_STD_ERROR == oam_sdm_system ((char *)command))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                "Error in creating backup of existing Software");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    backup_counter++;

    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,"Backup of Existing Software Taken Successfully");


    OAM_SDM_FUN_EXIT_TRACE ();
    return retVal;
}

/****************************************************************************
 *  Function Name  : oam_sdm_replace_software
 *  Description    : This function replaces the software package with new one
 *  Inputs         : none     
 *  Outputs        : none
 *  Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_sdm_return_et
oam_sdm_replace_software (void)
{
	Char8 command[OAM_SDM_TAR_SW_CMD_SIZE] = { OAM_SDM_ZERO };

	OAM_SDM_FUN_ENTRY_TRACE ();

	oam_sdm_snprintf (command,OAM_SDM_TAR_SW_CMD_SIZE,"tar -xvf %s -C ../", g_file_name);
    
    if (OAM_SDM_STD_ERROR == oam_sdm_system (command))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                "Error In Untaring Software Package");
        return OAM_SDM_FAILURE;
    }
	if(g_oam_sdm_event_ongoing != OAM_SDM_ROLLBACK)
	{
        if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)g_file_name))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                                        "Error in Removing Downloaded file from bin folder");
        }
		if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)g_file_name))
		{
			OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
					"Error in Removing Downloaded file from bin folder");
		}
	}
#ifdef OAM_EVM_TARGET    
    /* copy the DSP images to respective directory on the board */  
    UInt8 cp_command[]="cp -rf ../bin/dsp-core0.out ../bin/dsp-core1.out ../bin/dsp-core2.out ../bin/dsp-core3.out /lib/firmware/";
    if (OAM_SDM_STD_ERROR == oam_sdm_system((char *)cp_command))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Error in Replacing The Cfg Files");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
#endif
	OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,
			"Succesfully Replaced Existing Software With new Software");
	OAM_SDM_FUN_EXIT_TRACE ();
	return OAM_SDM_SUCCESS;
}

/*****************************************************************************
 * Function Name  : oam_sdm_send_upgrade_start_resp 
 * Description    : This function sends message to OAM in response to  
 *                  oam_sdm_upgarde_req   
 * Inputs         : status : Indicates success and failure  
 *                  error_code : Indicates type of Error 
 *                  err_string : Description of Error 
 * Output         : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ******************************************************************************/
oam_sdm_return_et
oam_sdm_send_upgrade_start_resp(UInt8 status, UInt16 error_code,
                                UInt16 transaction_id, UInt8 *err_string)
{

    UInt8 *p_api_msg = OAM_SDM_NULL;
    UInt8 *p_msg = OAM_SDM_NULL;
    UInt16 msg_length = OAM_SDM_ZERO;
    UInt16 err_string_len = OAM_SDM_ZERO;
    UInt8 error_str[OAM_SDM_ERR_STRG_SIZE] = { OAM_SDM_ZERO };

    OAM_SDM_FUN_ENTRY_TRACE ();

    err_string_len = oam_sdm_strlen ((char *) err_string);

    msg_length += OAM_CSPL_HDR_SIZE + OAM_SDM_INTF_HEADER_SIZE;
    msg_length += sizeof (UInt8);
    msg_length += sizeof (UInt16);
    msg_length += sizeof (UInt16);
    msg_length += sizeof (UInt16);
    msg_length += sizeof (UInt8);
    msg_length += err_string_len;

    if (OAM_SDM_ZERO != err_string_len)
        msg_length += OAM_SDM_ONE;		/*For Null character*/

    if (OAM_SDM_FAILURE == oam_sdm_mem_alloc (msg_length, (void *) &p_msg, &error_code))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                "Memory allocation to p_msg failed with Error Code:%d",
                error_code);
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }

    oam_memset (p_msg, OAM_SDM_ZERO, msg_length);
    p_api_msg = p_msg;

    /* encode the CSPL header in the message */
    oam_sdm_construct_api_header (p_api_msg, OAM_SDM_VERSION, OAM_SDM_MODULE_ID,
            OAM_MODULE_ID,
            SDM_OAM_RESPONSE_UPGRADE_START, msg_length);

    p_api_msg += OAM_CSPL_HDR_SIZE;

    /* encode API header in message */
    /* SPR 17777 fix */
    oam_sdm_construct_mgmt_api_hdr ((UInt8 *) p_api_msg,
            transaction_id,
            OAM_SDM_MODULE_ID,
            OAM_MODULE_ID,
            SDM_OAM_RESPONSE_UPGRADE_START,
            msg_length - OAM_CSPL_HDR_SIZE,
            OAM_SDM_ZERO);

    p_api_msg = p_api_msg + OAM_SDM_INTF_HEADER_SIZE;

    /* Pack NULL Character also in error string */
    if (OAM_SDM_ZERO != err_string_len)
    {
        /* KLOCWORK warning fix start */
        oam_sdm_strncpy((Char8 *)error_str, (Char8 *)err_string, sizeof(error_str) - 1);
        /* KLOCWORK warning fix end */
        err_string_len += OAM_SDM_ONE;
    }
    oam_sdm_encode_get_params_resp_ctrl_blk ((char *) p_api_msg,
            status, error_code, OAM_SDM_ZERO,
            OAM_SDM_ZERO, err_string_len,
             (char *)error_str);

    if (OAM_SDM_FAILURE == oam_sdm_send_message (p_msg, OAM_MODULE_ID, (SInt32 *)&error_code))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Error in sending message to OAM: %d",
                error_code);
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

/*************************************************************************
 * Function Name  : oam_sdm_send_message
 * Description    : This is used for sending message to external modules
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 ******************************************************************************/
    oam_sdm_return_et
oam_sdm_send_message (void *p_msg,	/* Message that will be passed */
        int dst_module,	/* Module id for which message will be passed */
        int *p_error_code)
{
    OAM_SDM_FUN_ENTRY_TRACE ();
    UInt16 src_id =oam_sdm_get_word_from_header((UInt8 *)p_msg + OAM_SDM_ONE);
    UInt16 dst_id =oam_sdm_get_word_from_header((UInt8 *)p_msg + OAM_SDM_THREE);
    UInt16 api_id =oam_sdm_get_word_from_header((UInt8 *)p_msg + OAM_SDM_FIVE);
    UInt16 length =oam_sdm_get_word_from_header((UInt8 *)p_msg + OAM_SDM_SEVEN);

    if((OAM_SDM_ZERO==src_id)||(OAM_SDM_ZERO==dst_id))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,"Either Source Id[%u] or "
            "Destination Id[%u] received is ZERO", src_id,dst_id);
        return OAM_SDM_FAILURE;
    }
    
    OAM_SDM_LOG (SDM, OAM_SDM_INFO, "Sending API[%u] of length[%u]"
        " from Module[%u] to Module[%u]", api_id, length, src_id, dst_id);

    QMODULE service = qvGetService (dst_module);
    qvSendEx (service, OAM_SDM_ZERO, p_msg, (int *) p_error_code);

    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;

}

/****************************************************************************
 * Function Name  : sdm_get_config_info
 * Description    : This is used to read configuration file 
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 *****************************************************************************/
    oam_sdm_return_et
sdm_get_config_info (void)
{
    char read_buf[OAM_SDM_READ_BUFF_SIZE] = { OAM_SDM_ZERO };
    char tempStr[OAM_SDM_READ_BUFF_SIZE]   = { OAM_SDM_ZERO };
    FILE *fp = OAM_SDM_NULL;
    FILE *fp_port_info = OAM_SDM_NULL;
    OAM_SDM_FUN_ENTRY_TRACE ();

    oam_memset(&comm_data, 0, sizeof(sdm_ext_com_config_t));
    fp_port_info = oam_sdm_fopen("../cfg/eNodeB_Configuration.cfg","r");
    if (OAM_SDM_NULL == fp_port_info) 
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,"Not able to open cfg file. Shutting down eNodeB");

        OAM_SDM_FUN_EXIT_TRACE ();
        oam_sdm_exit(OAM_SDM_RESULT_FAILURE);
    }
    fp = oam_sdm_fopen ("../cfg/lte_oam_sdm.cfg", "r");
    if (OAM_SDM_NULL == fp)
    {
        OAM_SDM_LOG (SDM, OAM_SDM_INFO,
                "Not able to open cfg file reading from header file....");
        oam_sdm_poll_time = OAM_SDM_POLL_TIME;
        oam_num_of_backup = MAX_NO_OF_BACKUP;
    }
    else 
    {
        while (oam_sdm_fgets ((char *) read_buf, OAM_SDM_READ_BUFF_SIZE, fp))
        {
            if (snscanf((const char *) read_buf, sizeof(read_buf),"OAM_SDM_POLL_TIME=%s", tempStr) >
                    OAM_SDM_ZERO )
            {
                OAM_SDM_LOG (SDM, OAM_SDM_INFO,"OAM_SDM_POLL_TIME=%s", tempStr);
                oam_sdm_poll_time = oam_sdm_atoi (tempStr);
            }
            else if (snscanf ((const char *) read_buf,sizeof(read_buf),"OAM_NUM_OF_BACKUP=%s", tempStr)
                    > OAM_SDM_ZERO)
                {
                    OAM_SDM_LOG (SDM, OAM_SDM_INFO,"OAM_NUM_OF_BACKUP=%s", tempStr);
                oam_num_of_backup = oam_sdm_atoi (tempStr);
                }
        }
    oam_sdm_fclose(fp);
#ifdef OAM_EVM_TARGET    
        oam_num_of_backup = MAX_NO_OF_BACKUP;
#endif
    }
    while (oam_sdm_fgets((char *)read_buf,OAM_SDM_READ_BUFF_SIZE,fp_port_info))
    {
        if(snscanf((const char *)read_buf,sizeof("ENB_OAM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,
                          "ENB_OAM_IP_ADDRESS=%s",comm_data.oam_com_params.ipaddr) > OAM_SDM_ZERO )
        {
            if( !oam_sdm_isValidIp4(comm_data.oam_com_params.ipaddr) )
            {
                OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"LTE_OAM_IP_ADDRESS is INVALID. Shutting down eNodeB");
                oam_sdm_abort();
            }
            OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"LTE_OAM_IP_ADDRESS : %s",
                    comm_data.oam_com_params.ipaddr);
}
        else if (snscanf((const char *)read_buf,sizeof("ENB_OAM_RX_PORT=")+MAX_PORT_LENGTH,
                            "ENB_OAM_RX_PORT=%s",comm_data.oam_com_params.port) > OAM_SDM_ZERO )
        {
            OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"LTE_OAM_RX_PORT : %s",
                    comm_data.oam_com_params.port);
        }
        else if(snscanf((const char *)read_buf,sizeof("ENB_SDM_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
                                     "ENB_SDM_IP_ADDRESS=%s",comm_data.sdm_com_params.ipaddr) > OAM_SDM_ZERO )
        {
            if( !oam_sdm_isValidIp4(comm_data.sdm_com_params.ipaddr) )
            {
                OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"ENB_SDM_IP_ADDRESS is INVALID. Shutting down eNodeB");
                oam_sdm_abort();
            }
            OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"LTE_SDM_IP_ADDRESS : %s",
                    comm_data.sdm_com_params.ipaddr);
        }
        else if (snscanf((const char *)read_buf,sizeof("ENB_SDM_RX_PORT=")+MAX_PORT_LENGTH,
                           "ENB_SDM_RX_PORT=%s",comm_data.sdm_com_params.port) > OAM_SDM_ZERO )
        {

            OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"LTE_SDM_RX_PORT : %s",
                    comm_data.sdm_com_params.port);
        }
    }
    oam_sdm_fclose(fp_port_info);

    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_sdm_handle_active_ind 
 * Description    : This function is used to handle Active Ind from OAM   
 * Inputs         : none
 * Outputs        : none
 * Returns        : none
 *****************************************************************************/
    void
oam_sdm_handle_active_ind(void)
{
    UInt8 *p_temp = OAM_SDM_NULL;
    UInt16 sdm_msg_len = OAM_SDM_ZERO;
    /* SPR 17777 fix */
    /* code removed */
    g_flag_for_oam_active_ind = OAM_SDM_TRUE;

    OAM_SDM_FUN_ENTRY_TRACE ();
    
    /* SPR 17777 fix */
    p_temp = (UInt8 *)oam_sdm_create_polling_message ();

    sdm_msg_len = oam_sdm_get_word_from_header ((UInt8 *) p_temp + OAM_SDM_EIGHT);

    g_oam_sdm_event_ongoing = OAM_SDM_FALSE;
    /* coverity 32992 */
    if (OAM_SDM_NULL == oam_start_new_timer (OAM_SDM_MODULE_ID, OAM_SDM_MODULE_ID,
                SDM_OAM_POLL_TIMER_TYPE, oam_sdm_poll_time * OAM_SDM_TIME_CONV_S_TO_MS,
                (void *) p_temp, sdm_msg_len, (oam_sdm_bool_et) OAM_SDM_ZERO))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                "Failed To start the timer for polling");
    }
    OAM_SDM_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_sdm_start_upgrade 
 * Description    : This is used for Upgrading Software  
 * Inputs         : p_msg
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 ******************************************************************************/
oam_sdm_return_et
oam_sdm_start_upgrade (UInt8 * p_msg)
{

    UInt16 api_id = OAM_SDM_ZERO;
    UInt8 commandkey = OAM_SDM_ZERO;
    FILE *fptr = OAM_SDM_NULL;
    char* oui_value = OAM_SDM_NULL;
    char* productClass_value=OAM_SDM_NULL;
    char* hw_value=OAM_SDM_NULL;
    char* sw_value=OAM_SDM_NULL;
    UInt8 temp_file_name[MAX_BACKUP_STR_LENGTH]= {OAM_SDM_ZERO};
    /* SPR 19444 FIXED START*/
    /* code removed */
    /* SPR 19444 FIXED END*/
    OAM_SDM_FUN_ENTRY_TRACE ();

    g_sdm_poll_counter = OAM_SDM_ZERO;   
    g_oam_sdm_event_ongoing = OAM_SDM_TRUE;
    OAM_SDM_SLEEP(OAM_SDM_ONE); /* Sleep for One Sec to Ensure That OAM Shutdown all process spawned by it */
    g_flag_for_oam_active_ind = OAM_SDM_FALSE; 

    /* SPR 14655 Fix Start */
    UInt8 pid = OAM_SDM_ZERO;
    SInt32 status = OAM_SDM_ZERO;
    /* SPR 14655 Fix End */


    /* SPR 12731 Fix Start */
    UInt16 file_name_length = OAM_SDM_ZERO;
    /* SPR 12731 Fix End */

    OAM_SDM_LOG (SDM,OAM_SDM_DETAILED,
                "Event Marked as Event Ongoing");

    /* SPR 14655 Fix Start */
    oam_sdm_kill(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,SIGTERM);
    pid = waitpid (g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,&status , WNOHANG);
    if(pid == g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid)
    {
	    OAM_SDM_LOG(SDM, OAM_SDM_INFO,
			    "Process %s having pid : %d killed successfully",
			    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);
    }
    else if(pid == OAM_SDM_ZERO)
    {
	    OAM_SDM_LOG(SDM, OAM_SDM_INFO,
			    "Process %s having pid : %d no state change",
			    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);

	    oam_sdm_kill(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,SIGKILL);
	    pid = waitpid(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid, &status, 0);
	    OAM_SDM_LOG(SDM, OAM_SDM_INFO,
			    "Process %s having pid : %d killed successfully",
			    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);
    }
    else
    {
        OAM_SDM_LOG(SDM, OAM_SDM_WARNING,
			    "Process %s having pid : %d Error in killing",
			    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);
    }
    /* SPR 14655 Fix End */

    api_id = oam_sdm_get_word_from_header ((U8 *) p_msg + OAM_SDM_SIX);
    p_msg += OAM_SDM_TWELVE;
    commandkey = *((U8 *) p_msg);
    
    OAM_SDM_SLEEP(OAM_SDM_ONE); /* Sleep for One Sec to Ensure That Layers Spawned by oam,close their ports*/
    if (api_id == OAM_SDM_ACK_UPGRADE_START)
    {   
        OAM_SDM_LOG (SDM,OAM_SDM_DETAILED,
                "Upgrade Ack received from OAM, starting Upgrade");

        if (OAM_SDM_FAILURE == oam_sdm_existing_software_backup ())
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Error in Taking Backup");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }

        if (OAM_SDM_FAILURE == oam_sdm_replace_software ())
        {
            OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                    "Error in Replacing OAM with New Binary");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
        
        fptr = oam_sdm_fopen ("../cfg/oam_version_info.txt", "w");
        oam_sdm_strcpy((char *)temp_file_name,(char *)g_file_name);

        /* SPR 12731 Fix Start */
        file_name_length = oam_sdm_strlen((char *)temp_file_name);
        temp_file_name[file_name_length - OAM_SDM_FOUR]= '\0'; /* removing .tar extension from file name */
        /* SPR 12731 Fix End */

        if (OAM_SDM_NULL != fptr)
        {
            oui_value=oam_sdm_strtok((char *)temp_file_name, "_");

            if (oui_value != OAM_SDM_NULL)
                oam_sdm_fprintf (fptr,"ManufacturerOUI=%s\n", oui_value);

            productClass_value=oam_sdm_strtok(OAM_SDM_NULL, "_");

            if (productClass_value != OAM_SDM_NULL)
                oam_sdm_fprintf (fptr, "ProductClass=%s\n",productClass_value);

            hw_value = strtok(OAM_SDM_NULL, "_");

            if (hw_value != OAM_SDM_NULL)
                oam_sdm_fprintf (fptr, "HardwareVersion=%s\n",hw_value);

            /*SPR 15583 START*/
            sw_value = oam_sdm_strtok(OAM_SDM_NULL, "tar");
            /*Cov 94817 fix start*/
            if (sw_value != OAM_SDM_NULL)
            {
            /* SPR 19444 FIXED START*/
            /*  code removed */
            /* SPR 19444 FIXED END */
                /*SPR 15583 END*/
                oam_sdm_fprintf (fptr, "SoftwareVersion=%s\n",sw_value );
            }
            /*Cov 94817 fix end*/

            oam_sdm_fclose(fptr);
        }
        else
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Error in fillig version info");
        }
    }
    else if ((api_id == OAM_SDM_CONTROL_COMMAND_REQ)
            && (commandkey == FACTORY_RESET))
    {
        
        UInt8 command[]="cp ../factory/eNodeB_Data_Model_TR_196_based.xml ../factory/Proprietary_eNodeB_Data_Model.xml ../cfg";
        g_oam_sdm_event_ongoing = FACTORY_RESET;
        OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,
                "Reset command received from OAM with reset type %d",commandkey);
        if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)"../cfg/eNodeB_Data_Model_TR_196_based.xml"))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                    "Error in Removing old backup");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
        if (OAM_SDM_STD_ERROR == oam_sdm_remove ((char *)"../cfg/Proprietary_eNodeB_Data_Model.xml"))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                    "Error in Removing old backup");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
        if (OAM_SDM_STD_ERROR == oam_sdm_system((char *)command))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Error in Replacing The Cfg Files");
            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }

    }
    else if ((api_id == OAM_SDM_CONTROL_COMMAND_REQ)
            && (commandkey == SDM_SHUTDOWN_CMD))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,"Terminating lte_oamSDM after receiving Shutdown Request");
        oam_sdm_cleanup();
        OAM_SDM_SLEEP(OAM_SDM_ONE); /* Sleep for One Sec to Ensure That OAM Shutdown all process spawned by it */
        oam_sdm_exit(1);
    }


#ifdef OAM_EVM_TARGET
    if ((api_id == OAM_SDM_CONTROL_COMMAND_REQ)
            && (commandkey == 1))
    {
        system("reboot");
    }
#endif
    if (OAM_SDM_FAILURE ==
            oam_sdm_spawn_lte_oam ())
    {
        OAM_SDM_LOG (SDM, OAM_SDM_CRITICAL,
                "Error In Spawning lte_oam with new binaries");
        OAM_SDM_FUN_EXIT_TRACE ();
        return OAM_SDM_FAILURE;
    }
    /* coverity 35993 */
    if (OAM_SDM_NULL == oam_start_new_timer (OAM_SDM_MODULE_ID, OAM_SDM_MODULE_ID,
                SDM_OAM_ACTIVE_IND_TIMER_TYPE, SDM_OAM_ACTIVE_TIME_OUT * OAM_SDM_TIME_CONV_S_TO_MS,
                (void *)OAM_SDM_NULL,OAM_SDM_ZERO, (oam_sdm_bool_et)OAM_SDM_ZERO))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
                "Failed To start the timer for Active Indication");
    }
    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,"lte_oam succesfully Spawned");
    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

/******************************************************************************
 * Function Name  : oam_sdm_handle_polling_response
 * Description    : This function is used handles polling responce from OAM  
 * Inputs         : p_msg
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 *******************************************************************************/
    void
oam_sdm_handle_polling_response (UInt8 * p_msg)
{


    OAM_SDM_FUN_ENTRY_TRACE ();

    UInt8 process_counter = OAM_SDM_ONE;
    UInt16 process_pid   = OAM_SDM_ZERO;
    g_sdm_poll_counter = OAM_SDM_ZERO; /* Making Counter back to zero as
                                          Poll received from OAM */ 
    UInt16 error_code;
    p_msg = p_msg + OAM_SDM_INTF_HEADER_SIZE;
    g_oam_process_number = *((UInt8 *) p_msg);

    p_msg = p_msg + sizeof (UInt8);
    p_msg = p_msg + OAM_SDM_THREE * sizeof (UInt8); /*Reserved Field*/

    /* SPR 21889 start */
    if (OAM_SDM_NULL == g_oam_sdm_t.p_info)
    {
        if (OAM_SDM_FAILURE == oam_sdm_mem_alloc (g_oam_process_number *
                    sizeof(struct _oam_sdm_process_info_t), (void *)
                    &g_oam_sdm_t.p_info, &error_code))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                    "Error in Memory Allocation with Error Code:%d",
                    error_code);
            OAM_SDM_FUN_EXIT_TRACE ();
            return;
        }
    }
    else if (g_oam_sdm_t.max_no_process != g_oam_process_number)
    {
		qvMsgFree(g_oam_sdm_t.p_info);
        if (OAM_SDM_FAILURE == oam_sdm_mem_alloc (g_oam_process_number *
                    sizeof(struct _oam_sdm_process_info_t), (void *)
                    &g_oam_sdm_t.p_info, &error_code))
        {
            OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                    "Error in Memory Allocation with Error Code:%d",
                    error_code);
            OAM_SDM_FUN_EXIT_TRACE ();
            return;
        }
    }
    /* SPR 21889 end */
    while (process_counter < g_oam_process_number + OAM_SDM_ONE)
    {
        process_pid = process_pid | (UInt16) ((p_msg)[OAM_SDM_ZERO] << OAM_SDM_EIGHT);
        process_pid = process_pid | (UInt16) ((p_msg)[OAM_SDM_ONE]);
        g_oam_sdm_t.p_info[process_counter].pid = process_pid;
        p_msg = p_msg + sizeof (UInt16);
        process_counter++;
        process_pid = OAM_SDM_ZERO;
    }
    OAM_SDM_LOG (SDM,OAM_SDM_DETAILED,
            "PID of all process stored Successfully");
    OAM_SDM_FUN_EXIT_TRACE ();
}
/**********************************************************************
 * Function Name  : oam_sdm_get_available_backup_info 
 * Description    : This function is used for retriving the backup list  
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 *************************************************************************/
    oam_sdm_return_et
oam_sdm_get_available_backup_info()
{

    DIR *   dir_des[NO_OF_BACKUP_DIR] = {OAM_SDM_NULL};
    struct  dirent* file = {OAM_SDM_NULL};
    UInt32  loop_counter= OAM_SDM_ZERO;
    UInt8   backup_dir[NO_OF_BACKUP_DIR][MAX_BACKUP_STR_LENGTH]={"../backup"};
    UInt8   factory_bin_backup[MAX_BACKUP_STR_LENGTH]= {OAM_SDM_ZERO};

    OAM_SDM_FUN_ENTRY_TRACE ();

    /* Scanning the backup directories */
    for (loop_counter=OAM_SDM_ZERO; loop_counter < NO_OF_BACKUP_DIR; loop_counter++ )
    {
        if (OAM_SDM_NULL == (dir_des[loop_counter] = OAM_SDM_OPENDIR ((char *)backup_dir[loop_counter])))
        {
            OAM_SDM_LOG (SDM,OAM_SDM_WARNING,
                    "Error : Failed to open backup directory");

            OAM_SDM_FUN_EXIT_TRACE ();
            return OAM_SDM_FAILURE;
        }
    }
    for (loop_counter=OAM_SDM_ZERO; loop_counter < NO_OF_BACKUP_DIR; loop_counter++ )
    {
        while ((file = OAM_SDM_READDIR(dir_des[loop_counter]))!= OAM_SDM_NULL )
        {
            /* Skiping current and parent directories */
            if (!oam_sdm_strcmp (file->d_name, "."))
                continue;
            if (!oam_sdm_strcmp (file->d_name, ".."))
                continue;
            if(OAM_SDM_ZERO == (strcmp((char *)backup_dir[loop_counter],"../backup")))
            {
                if (backup_counter < oam_num_of_backup)
                {
                    oam_sdm_sprintf((char *)&g_bin_backup_array[backup_counter],"%s",file->d_name);
                    OAM_SDM_LOG(SDM,OAM_SDM_INFO,"In backup folder Backup file no %d = %s",
                            backup_counter+1,g_bin_backup_array[backup_counter]);
                    backup_counter++;
                }
            }
            else
            {
                oam_sdm_strcpy((char *)factory_bin_backup,file->d_name);
                OAM_SDM_LOG(SDM,OAM_SDM_INFO,"In Factory folder software file name = %s",
                        factory_bin_backup);
            }
        }
    }
    OAM_SDM_LOG (SDM,OAM_SDM_DETAILED,"Available backups Info retrived succesfully !!!");
    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

/*************************************************************************
 * Function Name  : oam_sdm_create_backup_file_name 
 * Description    : This function is used for create backup file name from  
 *                  Version Parameter received from OAM 
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 ***************************************************************************/
    oam_sdm_return_et
oam_sdm_create_backup_file_name(Char8 *backup_file_name)
{

    OAM_SDM_FUN_ENTRY_TRACE ();

    UInt8 *oui_value=OAM_SDM_NULL;
    UInt8 *productClass_value=OAM_SDM_NULL;
    UInt8 *hw_value=OAM_SDM_NULL;
    UInt8 *sw_value=OAM_SDM_NULL;

    oam_sdm_strtok((char *)oam_sdm_backup_info.oui, "=");
    oui_value = (UInt8 *)oam_sdm_strtok(OAM_SDM_NULL, "=");


    if (oui_value != OAM_SDM_NULL)
    {
        oam_sdm_strcpy(backup_file_name,(char *)oui_value);
        oam_sdm_strcat(backup_file_name,"_");
    }
    else
    {
        oam_sdm_strcpy(backup_file_name,"_");
    }

    oam_sdm_strtok((char *)oam_sdm_backup_info.product_class, "=");
    productClass_value =(UInt8 *)oam_sdm_strtok(OAM_SDM_NULL, "=");

    if (productClass_value != OAM_SDM_NULL)
        oam_sdm_strcat(backup_file_name,(char *)productClass_value);

    oam_sdm_strcat(backup_file_name,"_");

    oam_sdm_strtok((char *)oam_sdm_backup_info.hw_version, "=");
    hw_value = (UInt8 *)strtok(OAM_SDM_NULL, "=");

    if (hw_value != OAM_SDM_NULL)
        oam_sdm_strcat(backup_file_name,(char *)hw_value);

    oam_sdm_strcat(backup_file_name,"_");

    oam_sdm_strtok((char *)oam_sdm_backup_info.soft_version, "=");
    sw_value = (UInt8 *)oam_sdm_strtok(OAM_SDM_NULL, "=");

    if (sw_value != OAM_SDM_NULL)
        oam_sdm_strcat(backup_file_name,(char *)sw_value);

    OAM_SDM_FUN_EXIT_TRACE ();
    return OAM_SDM_SUCCESS;
}

void oam_sdm_signal_handler(int sig) {
    oam_sdm_cleanup();
    /* SPR 17777 fix */
    OAM_SDM_LOG(SDM,OAM_SDM_DETAILED," Unused Parameters %d ", sig);
    oam_sdm_exit(1);
}
 
/* + SPR 17439 */
void oam_sdm_cleanup(void)
/* - SPR 17439 */
{
   SInt32 arg_count = OAM_SDM_ZERO;  
   for (arg_count = OAM_SDM_ONE; arg_count<g_no_of_arguments; arg_count++)
   {
     SDM_FREE(g_list_of_init_arguments[arg_count]);  
     g_list_of_init_arguments[arg_count]=OAM_SDM_NULL;  
   }
   OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"Cleaned, all Buffer allocated");
}
