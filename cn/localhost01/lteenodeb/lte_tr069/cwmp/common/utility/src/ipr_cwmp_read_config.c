/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_read_config.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions to read lib config from config file. 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2012/11/07 Anant K Sharma  Initial Version 1.0
 *   2013/03/26 Shivani Pahuja   Removed Warnings
 *   2013/03/29 Anant K Sharma   replaced Char8 with UInt8 to remove compilation errors
 *   Jul 2014   Avinash           Fix for SPR 8235
 ******************************************************************************/

#include<ipr_cwmp_read_config.h>
/* SPR 16327 Fixed Start  */
extern UInt8 MaxSessionRetryCount;
/* SPR 16327 Fixed End  */

/* SPR 17268 fixed start */
UInt8 auth_type[AUTH_TYPE] = {0};
#define CONFIG_PARAM_LEN 256
//extern ABSPath[CONFIG_PARAM_LEN];
UInt8 ABSPath[CONFIG_PARAM_LEN] = {0};
UInt8 ABSPath_EVENT_CODE_FILE[CONFIG_PARAM_LEN] = {0};
UInt32 ABS_len = 0;
/* SPR 17268 fixed end */

/****************************************************************************
 * Function Name  : getVal
 * Inputs         : UInt8 *str
 *                 
 * Outputs        : None
 * Returns        : UInt32
 ****************************************************************************/

UInt32 getVal(UInt8 *str)
{
    UInt32 i=0;
    while(1)
    {
        if(*(str+i) == '=')
            break;
        else if (i>30)
            return -1;
        i++;
    }
    return(atoi((const char *)str+i+1));
}

/****************************************************************************
 * Function Name  : getValStr  
 * Inputs         : UInt8 *str1, UInt8 *str2
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void getValStr(UInt8 *str1, UInt8 *str2)
{
    UInt32 i=0;
    while(1)
    {
        if(*(str1+i) == '=')
            break;
        else if (i>154)
            return;
        i++;
    }
    /* SPR 18835 fixed start */
    //if((ENB_STRLEN((const char *)str1 + i + 1)- 1 ) < CONFIG_PARAM_LEN)
    ENB_STRNCPY((char *)str2, ((const char *)str1 + i + 1), ENB_STRLEN((const char *)str1 + i + 1)- 1);
    /* SPR 18835 fixed end */
}

/****************************************************************************
 * Function Name  : read_tr69_lib_config
 * Inputs         : None 
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void read_tr69_lib_config(void)
{
	FILE *fp = NULL;
	//UInt8 config_buffer[CONFIG_BUFFER_SIZE] = {0};
	/*KLOCK WORK FIX */
    /* SPR 18835 fix start*/
	UInt8 config_buffer[CONFIG_PARAM_LEN] = {0};
    /* SPR 18835 fix end*/
	ENB_PRINTF( "Reading CWMP Client config File for TR069 LIB config: %s...\n", CONFIG_FILE);
	fp = fopen(CONFIG_FILE, "r");
	if (fp == NULL)
	{
		ENB_PRINTF("\nERROR: Could not open CWMP Client config file%s\n",CONFIG_FILE);
		exit(0);
	}

	if(0 != fseek(fp, 0, SEEK_END))
	{
		ENB_PRINTF( "Error in fseek\n");
		exit(0);
	}
	signed long len = (signed long)ftell(fp);
	if( -1 == len)
	{
		ENB_PRINTF( "Error in ftell\n");
		exit(0);
	}

	if(0 != fseek(fp, 0, SEEK_SET))
	{
		ENB_PRINTF( "Error in fseek\n");
		exit(0);
	}


    /* SPR 18835 fix start*/
	while(fgets((char *)config_buffer, CONFIG_PARAM_LEN, fp) != NULL)
	{
              config_buffer[CONFIG_PARAM_LEN - 1] = '\0';
    /* SPR 18835 fix end*/
		if (strncmp((const char *)config_buffer, "LOG_CFG_PATH=", 13)==0 )
			getValStr(config_buffer, log_cfg_path);
		else if (strncmp((const char *)config_buffer, "DEBUGGER_MYPORT=", 16)==0 )
			debugger_port = getVal(config_buffer);
		else if (strncmp((const char *)config_buffer, "LOG_TRACE=", 10)==0 )
			getValStr(config_buffer, log_file_name);
        /* SPR 18835 fix start*/
		else if (ENB_STRNCMP((const char *)config_buffer, "CERT_PATH_IHEMS=", 16)==0 )
			getValStr(config_buffer, cert_file_path_ihems);
        else if(ENB_STRNCMP((const char*)config_buffer , "CERT_PATH_SHEMS=" ,16)==0 )
            getValStr(config_buffer , cert_file_path_shems);
        /* SPR 18835 fix end*/
        
        /*SPR 19088 FIXED START*/
        /* code removed */
        /*SPR 19088 FIXED END*/
		else if (strncmp((const char *)config_buffer, "ENV_MYPORT=", 11)==0 )
			http_server_port = getVal(config_buffer);
		else if (strncmp((const char *)config_buffer , AUTHENTICATION_TYPE , AUTHENTICATION_TYPE_SIZE) == 0)
			getValStr(config_buffer, auth_type);
		/* SPR 16327 Fixed Start  */
		else if (strncmp((const char *)config_buffer, "MAX_SESSION_RETRY_COUNT=", 24)==0)
			MaxSessionRetryCount = getVal(config_buffer);
		/* SPR 16327 Fixed End  */


        /* SPR 17268 FIXED START */
        else if (strncmp((const char *)config_buffer, "ABS_PATH=" , 9)==0)
            {
            getValStr(config_buffer , ABSPath);

            ABS_len = strlen((char *)ABSPath);
         //   ABSPath[ABS_len]='\0';
        }
        /* SPR 17268 FIXED END */
	}
	fclose(fp);
}
