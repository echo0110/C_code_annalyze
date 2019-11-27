/********************************************************************************
*
*       FILE NAME   : 	tr69_driver.c
*
*       DESCRIPTION : 	This file contains the functions to initialize the 
*				TR069 Library which initializes various modules of
				TR069 lib too.
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 18, 2012   TR-069 Amendment 4   Initial Creation
*       Anant K Sharma Dec 31, 2012   Changes for replacing shell env variables 
*       Anant K Sharma Feb 28, 2013   Added crypto params file init 
*       Shivani Pahuja March 25, 2013 Removed warnings 
*       Avinash          Jul 2014      Fix for SPR 8235
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/


/* SPR 16732 FIX START */
#define _GNU_SOURCE
#include <time.h>
/* SPR 16732 FIX END */
#include <tr69_driver.h>
#include <acs_discovery.h>
#include <common_def.h>
#include <dirent.h>
#include <client_req_manager.h>
//OAM SI code changes
#include <lteTypes.h>
#include <ipr_cwmp_read_config.h>
#include <lteTypes.h>
#include <map_glue.h>
#include <signal.h>

/******************************************************************************
  Global Variables
******************************************************************************/

//Used for  storing callback functions from Glue Layer 
sCallbackFunList_t  stCallbackFunList;
/* SPR 16327 Fixed Start  */
UInt8 MaxSessionRetryCount = 0;
/* SPR 16327 Fixed End  */

//OAM SI code changes 
//TR069 LIB configuration global variables
UInt8 log_cfg_path[CONFIG_PARAM_LEN] = {0};
UInt32 debugger_port = 0;
UInt8 log_file_name[CONFIG_PARAM_LEN] = {0};
//moved this to http.c for lib delivery UInt8 cert_file_path[CONFIG_PARAM_LEN] = {0};
UInt32 http_server_port = 0;

extern pthread_t g_th_upload;
extern YLIST g_Txn_List;

extern pthread_t g_rpc_Download;
extern U8 g_rpc_IsDwnQ_init;
/*SPR 21885 FIXED START*/
extern U8 g_rpc_IstxQ_init; 
/*SPR 21885 FIXED END*/

extern YLIST g_Txn_List_Download;

extern U8 g_rpc_IsSchDwnQ_init;
extern YLIST g_Txn_List_Sch_Dwn;
extern pthread_t g_rpc_sch_dwn_th;
extern YSQUEUE g_clientRequestQ;

/* SPR 17268 FIXED START */
extern UInt32 ABS_len;
// UInt8 ABSPath[CONFIG_PARAM_LEN] = {0};
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
extern UInt8 ABSPath_EVENT_CODE_FILE[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */

        /* SPR 22092 FIXED Start */
        /* Code Remove */
        /* SPR 22092 FIXED Start */



/******************************************************************************
 Function Declarations
******************************************************************************/

extern void * ipr_cwmp_actualDownload (void  * arg);
extern void * ipr_cwmp_actualUpload (void *thArg_t);
void initiate_TR069_Inform_session(char * , ParameterValueList *ParamValList);
int autonomousTransferComplete(STATransferComplete *);
int getRPCMethod_ACS (void);
extern void event_manager_init(void);
extern void client_manager_init(void);
void * ipr_cwmp_actualScheduleDownload (void * arg);
int autoDUStateChangeComplete(sAutoDUStateChangeComplete *);
extern void ipr_cwmp_Init_SSL_param(void);
extern ipr_cwmp_ret_t ipr_cwmp_start_server (void);
extern int ipr_cwmp_addEvent(sEventStruct *event , ParameterValueList  * spstParam);

/* SPR 19838: Coverity:12932 FIX START */
void FREE_tr69Parameters(Tr69Parameters_t tr69Parameters)
{
    DEALLOCATE (tr69Parameters.Password);
    DEALLOCATE (tr69Parameters.Username);
    DEALLOCATE (tr69Parameters.ConnectionRequestPassword);
    DEALLOCATE (tr69Parameters.ConnectionRequestUsername);
    DEALLOCATE (tr69Parameters.SoftwareVers);
    DEALLOCATE (tr69Parameters.ConnectionRequestURL);


}

/* SPR 19838: Coverity:12932 FIX STOP */
/******************************************************************************
  Function Definitions
******************************************************************************/

/*****************************************************************************
* FUNCTION:            	copy_TR069Parameters 
* DESCRIPTION:  		The function calls Module Management Server setters to set  
*						the parameters recieved as input during initialization
						of tr69 library.
* ARGS:
*                       Tr69Parameters_t  tr69Parameters 
* RETURNS:
*                       TRUE or FALSE. 
******************************************************************************/

inline  int copy_TR069Parameters (Tr69Parameters_t  tr69Parameters) 
{
	int rt = FALSE;
        int ret_val = SUCCESS;
	rt = ipr_cwmp_setEnableCWMP(tr69Parameters.EnableCWMP);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
               /*SPR 16294 START*/
               TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 EnableCWMP parameter %d",__FUNCTION__,tr69Parameters.EnableCWMP);
               /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setPassword(tr69Parameters.Password);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 Password parameter %s",__FUNCTION__ , tr69Parameters.Password);
                /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setUsername(tr69Parameters.Username);
	/*SPR 13623 STARt*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 Username parameter %s",__FUNCTION__ , tr69Parameters.Username);
                /*SPR 16294 END*/                 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setPeriodicInformInterval(tr69Parameters.PeriodicInformInterval);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
               /*SPR 16294 START*/
               TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 PeriodicInformInterval parameter %d",
				__FUNCTION__ , tr69Parameters.PeriodicInformInterval);
               /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setPeriodicInformEnable(tr69Parameters.PeriodicInformEnable);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 PeriodicInformEnable parameter %d",
				__FUNCTION__ , tr69Parameters.PeriodicInformEnable);
                /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setCWMPRetryIntervalMultiplier(tr69Parameters.CWMPRetryIntervalMultiplier);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 CWMPRetryIntervalMultiplier parameter %d",
				__FUNCTION__ , tr69Parameters.CWMPRetryIntervalMultiplier);
                /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setCWMPRetryMinimumWaitInterval(tr69Parameters.CWMPRetryMinimumWaitInterval);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
               TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 CWMPRetryMinimumWaitInterval parameter %d",
				__FUNCTION__ , tr69Parameters.CWMPRetryMinimumWaitInterval);
               /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setDefaultActiveNotificationThrottle(tr69Parameters.DefaultActiveNotificationThrottle);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
        {       /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 DefaultActiveNotificationThrottle parameter %d",
				__FUNCTION__ , tr69Parameters.DefaultActiveNotificationThrottle);
                /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setUDPConnectionRequestAddress(tr69Parameters.UDPConnectionRequestAddress);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
              /*SPR 16294 START*/
              TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 UDPConnectionRequestAddress parameter %s",
				__FUNCTION__ , tr69Parameters.UDPConnectionRequestAddress);
              /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setDownloadProgressURL(tr69Parameters.DownloadProgressURL);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
               /*SPR 16294 START*/
               TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 DownloadProgressURL parameter %s",
				__FUNCTION__ , tr69Parameters.DownloadProgressURL);
               /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setUpgradesManaged(tr69Parameters.UpgradesManaged);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                 /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 UpgradesManaged parameter %d",
				__FUNCTION__ , tr69Parameters.UpgradesManaged);
                 /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setConnectionRequestPassword(tr69Parameters.ConnectionRequestPassword);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                 /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 ConnectionRequestPassword parameter %s",
				__FUNCTION__ , tr69Parameters.ConnectionRequestPassword);
                 /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setConnectionRequestUsername(tr69Parameters.ConnectionRequestUsername);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 ConnectionRequestUsername parameter %s",
				__FUNCTION__ , tr69Parameters.ConnectionRequestUsername);
                /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
	rt = ipr_cwmp_setParameterKey(tr69Parameters.ParameterKey);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 ParameterKey parameter %s",
				__FUNCTION__ , tr69Parameters.ParameterKey);
                /*SPR 16294 END*/ 
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
#ifdef VENDOR_SPECIFIC
	rt = ipr_cwmp_setSoftVers((UInt8 *)tr69Parameters.SoftwareVers);
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
               /*SPR 16294 START*/
               TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 SoftwareVers parameter %s",
				__FUNCTION__ , tr69Parameters.SoftwareVers);
               /*SPR 16294 END*/
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
#endif
	rt = ipr_cwmp_setConnectionRequestURL (tr69Parameters.ConnectionRequestURL );
	/*SPR 13623 START*/
	if (rt != SUCCESS)
	{
                /*SPR 16294 START*/
                TRACE(LOG_MAJOR,"DRIVER", "In function %s error populating TR069 ConnectionRequestURL parameter %s",
				__FUNCTION__ , tr69Parameters.ConnectionRequestURL);
                /*SPR 16294 END */
                ret_val = FAILURE;
	}
	/*SPR 13623 END*/
        return ret_val;
}

/*download upload change starts*/
/*****************************************************************************
* FUNCTION:             QueueDownload
* DESCRIPTION:		 to start donwloads which are queued  in a directory.
* ARGS:
* RETURNS:
*                       NONE.
******************************************************************************/

void queueDownload (void)
{

	struct dirent *entry;
	DIR *dp;
	FILE *dw_fp = NULL;
	FILE *upl_fp = NULL;
	/*download upload change starts*/
	char strPath[MAX_DW_LOG_NAME_LEN] = {0};
	/*download upload change ends*/
	ThreadDownload *tArg = NULL;
	ThreadUpload *tArg_upl_t = NULL;
	STDownload *pstDownload = NULL;
	ScheduleDownload *pstSCHDwn = NULL;
	Upload *pUpload_t = NULL;
	char *dw_logfilename = NULL;
	char *upl_logfilename = NULL;
	char *tmpBuffer = NULL;
	char *str = NULL;
	char * local_path = NULL;
	/*Review comments incorporated */
	time_t t1 = 0; 
	time_t currtime = 0;
	unsigned int diff = 0;
	int ret = 0 ;
	short rt = 0;
	int rc  = 0;
	SInt32 l_size = 0;
    /*download upload change starts*/    
	 TRACE (LOG_CRITICAL, "DRIVER", "Entering function %s\n",__FUNCTION__);
	 /* SPR 17268 FIXED START */
	 UInt8 ABSPath_queued_download[CONFIG_PARAM_LEN] = {'\0'};
     strcpy((char *)ABSPath_queued_download,(char *)ABSPath);
   /* SPR 20706 FIXED START */
   strcat((char*)ABSPath_queued_download,"/Download/"); 
   /* SPR 20706 FIXED END */
	dp = opendir((const char *)ABSPath_queued_download);
	if (dp == NULL) 
    /*download upload change ends*/
	{
		perror("opendir: Path does not exist or could not be read.");
		return;
	}
	 /*download upload change starts*/
    TRACE (LOG_CRITICAL, "DRIVER", "In %s opendir function is successful\n",__FUNCTION__);
	 /*download upload change ends*/
	while ((entry = readdir(dp)))
	{
		rt = 0;
	    memset(strPath,'\0',sizeof(strPath));	
	    TRACE (LOG_CRITICAL, "DRIVER", "In %s reading entry of directory\n",__FUNCTION__);
		if (strncmp(entry->d_name , "dwn", 3 ) == 0)
		{
			TRACE (LOG_CRITICAL, "DRIVER", "In %s entry read is of download type\n",__FUNCTION__);
			if (!g_rpc_IsDwnQ_init)
			{
				init_list(&g_Txn_List_Download);
				g_rpc_IsDwnQ_init = 1;
			}
			/*download upload change starts*/
         	strncpy((char *)strPath , (const char *)ABSPath_queued_download, strlen((const char *)ABSPath_queued_download));
			strncat(strPath, "/", 1);	
			strncat((char *)strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-((strlen((const char *)ABSPath_queued_download))+ 1)));
			TRACE (LOG_CRITICAL, "DRIVER", "In %s  strpath = %s\n",__FUNCTION__,strPath);


            /* SPR 17268 FIXED START */

          
            dw_fp = fopen((char *)strPath, "r");
            /*download upload change ends*/

            /* SPR 17268 FIXED END */


			if (dw_fp == NULL)
			{
				/*Cov fix for pointers */
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */
				return ;
			}
			tmpBuffer = (char *)ALLOCATE(1024 );
			if (tmpBuffer == NULL)
			{
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */

				ENB_FCLOSE(dw_fp);
				return;
			}			
            ret = fread((void *)tmpBuffer,1024, 1, dw_fp);
			if (ret != 0)
            {
				ENB_FCLOSE(dw_fp);
				DEALLOCATE(tmpBuffer);
				continue;	
            }
			tmpBuffer[1023] = '\0'; 
			str = strtok(tmpBuffer , "\n");
			pstDownload = (STDownload *)ALLOCATE(sizeof (STDownload));
			if (pstDownload != NULL)			
			{
				if (str != NULL)
				{
					pstDownload->CommandKey  = (char *)ALLOCATE(COMMAND_KEY_SIZE);
					if (!pstDownload->CommandKey)
					{
						TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
						ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
						return;
					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->CommandKey , "", 2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->CommandKey = %s\n",__FUNCTION__, pstDownload->CommandKey);
						}
						else
						{
							strncpy(pstDownload->CommandKey , str, COMMAND_KEY_SIZE);				
		                    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->CommandKey = %s\n",__FUNCTION__, pstDownload->CommandKey);
			            }				
					}
				}	

				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->FileType = (char *)ALLOCATE(FILE_TYPE_SIZE);	
					if (!pstDownload->FileType)
					{
						TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
						ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
						return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->FileType , "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->FileType = %s\n",__FUNCTION__, pstDownload->FileType);
						}
						else
						{
							strncpy (pstDownload->FileType , str, FILE_TYPE_SIZE-1);				
		                    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->FileType = %s\n",__FUNCTION__, pstDownload->FileType);				
						}
					}
				}	
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->URL = (char *)ALLOCATE(URL_SIZE);
					if (!pstDownload->URL)
					{
						TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
						ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
						return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->URL , "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->URL = %s\n",__FUNCTION__, pstDownload->URL);
						}
						else
						{
							strncpy(pstDownload->URL , str, URL_SIZE);				
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->URL = %s\n",__FUNCTION__, pstDownload->URL);
						}
					}
				}	
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->Username = (char *)ALLOCATE(USR_NAME_SIZE);
					if (!pstDownload->Username)
					{
						TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
						ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload->URL);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
						return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->Username , "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->Username = %s\n",__FUNCTION__, pstDownload->Username);
						}
						else
						{
							strncpy(pstDownload->Username , str, USR_NAME_SIZE);				
						     TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->Username = %s\n",__FUNCTION__, pstDownload->Username);
						}
					}
				}	

				str = strtok(NULL , "\n");
				if (str != NULL )
				{
					pstDownload->Password = (char *)ALLOCATE(PWD_SIZE);	
					if (!pstDownload->Password)
					{
						TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
						ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload->URL);
						DEALLOCATE(pstDownload->Username);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
						return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->Password , "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->Password = %s\n",__FUNCTION__, pstDownload->Password);
						}
						else
						{
							strncpy(pstDownload->Password , str, PWD_SIZE);				
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->Password = %s\n",__FUNCTION__, pstDownload->Password);
						}
					}
				}	
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->FileSize = strtoul(str, NULL, 0);				
					TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->FileSize = %d\n",__FUNCTION__, pstDownload->FileSize);
				}	

				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->TargetFileName = (char *)ALLOCATE(TRG_NAME_SIZE);
					if (!pstDownload->TargetFileName)
					{
                        TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
                        ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload->URL);
						DEALLOCATE(pstDownload->Username);
						DEALLOCATE(pstDownload->Password);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
                        return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->TargetFileName, "",2); 
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->TargetFileName = %s\n",__FUNCTION__, pstDownload->TargetFileName);
						}
						else
						{
							strncpy(pstDownload->TargetFileName , str, TRG_NAME_SIZE);				
					        TRACE (LOG_CRITICAL, "DRIVER", "In %s pstDownload->TargetFileName = %s\n",__FUNCTION__, pstDownload->TargetFileName);	
						}
					}
				}	

				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					TRACE (LOG_CRITICAL, "DRIVER", "In %s  checking for pstDownload->DelaySeconds\n",__FUNCTION__);
					pstDownload->DelaySeconds = strtoul(str , NULL, 0);
					TRACE (LOG_CRITICAL, "DRIVER", "In %s  pstDownload->DelaySeconds = %d\n",__FUNCTION__,  pstDownload->DelaySeconds);
				}	

				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->SuccessURL = (char *)ALLOCATE(URL_SIZE );
					if (!pstDownload->SuccessURL)
					{
                        TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
                        ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload->URL);
						DEALLOCATE(pstDownload->Username);
						DEALLOCATE(pstDownload->Password);
						DEALLOCATE(pstDownload->TargetFileName);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
                        return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->SuccessURL, "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s  pstDownload->SuccessURL = %s\n",__FUNCTION__,  pstDownload->SuccessURL);
						}
						else
						{
							strncpy(pstDownload->SuccessURL , str,  URL_SIZE);				
		                    TRACE (LOG_CRITICAL, "DRIVER", "In %s  pstDownload->SuccessURL = %s\n",__FUNCTION__,  pstDownload->SuccessURL);				
						}
					}
				}	
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					pstDownload->FailureURL  = (char *)ALLOCATE(URL_SIZE );
					if (!pstDownload->FailureURL)
					{
                        TRACE (LOG_CRITICAL, "DRIVER", "In %s memory allocation failure\n",__FUNCTION__);
                        ENB_FCLOSE(dw_fp);
						DEALLOCATE(tmpBuffer);
						DEALLOCATE(pstDownload->CommandKey);
						DEALLOCATE(pstDownload->FileType);
						DEALLOCATE(pstDownload->URL);
						DEALLOCATE(pstDownload->Username);
						DEALLOCATE(pstDownload->Password);
						DEALLOCATE(pstDownload->TargetFileName);
						DEALLOCATE(pstDownload->SuccessURL);
						DEALLOCATE(pstDownload);
                        /*  coverity 40683 fix start */
                        closedir(dp);
                        /*  coverity 40683 fix end */
                        return;

					}
					else
					{
						if (*str == '-')
						{
							strncpy(pstDownload->FailureURL , "",2);
						    TRACE (LOG_CRITICAL, "DRIVER", "In %s  pstDownload->FailureURL = %s\n",__FUNCTION__, pstDownload->FailureURL);
						}
						else
						{
							strncpy(pstDownload->FailureURL , str,  URL_SIZE);				
		                    TRACE (LOG_CRITICAL, "DRIVER", "In %s  pstDownload->FailureURL = %s\n",__FUNCTION__, pstDownload->FailureURL);				
						}
					}
				}
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					if(!rt)
					{
						time(&currtime);
						t1 = strtol(str, NULL,10);
						diff =  currtime - t1;
						TRACE (LOG_CRITICAL, "DRIVER", "In %s  diff = %d DelaySeconds = %d \n",__FUNCTION__, diff, pstDownload->DelaySeconds);
						if (diff < pstDownload->DelaySeconds)
							pstDownload->DelaySeconds = pstDownload->DelaySeconds - diff;
						else
							pstDownload->DelaySeconds = 0;
					}
				}

				tArg = (ThreadDownload *)ALLOCATE(sizeof (ThreadDownload));
				if (tArg == NULL)
				{
					ENB_FCLOSE(dw_fp);
					DEALLOCATE(tmpBuffer);
					closedir(dp);
					DEALLOCATE(pstDownload->CommandKey);
					DEALLOCATE(pstDownload->FileType);
					DEALLOCATE(pstDownload->URL);
					DEALLOCATE(pstDownload->Username);
					DEALLOCATE(pstDownload->Password);
					DEALLOCATE(pstDownload->TargetFileName);
					DEALLOCATE(pstDownload->SuccessURL);
					DEALLOCATE(pstDownload->FailureURL);
					DEALLOCATE(pstDownload);
					return;
				}
                TRACE (LOG_CRITICAL, "DRIVER", "In %s allocating memory for thread download success\n",__FUNCTION__);
				dw_logfilename = (char *)ALLOCATE(MAX_DW_LOG_NAME_LEN);
				if ( dw_logfilename == NULL)
				{
					DEALLOCATE(tArg);
					ENB_FCLOSE(dw_fp);
					DEALLOCATE(tmpBuffer);
					closedir(dp);
					DEALLOCATE(pstDownload->CommandKey);
					DEALLOCATE(pstDownload->FileType);
					DEALLOCATE(pstDownload->URL);
					DEALLOCATE(pstDownload->Username);
					DEALLOCATE(pstDownload->Password);
					DEALLOCATE(pstDownload->TargetFileName);
					DEALLOCATE(pstDownload->SuccessURL);
					DEALLOCATE(pstDownload->FailureURL);
					DEALLOCATE(pstDownload);
					return;
				}
                /*SPR 17268 FIXED START*/
				strncpy(dw_logfilename , (char *)strPath, MAX_DW_LOG_NAME_LEN);
                /*SPR 17268 FIXED END*/
				tArg->dw_logfilename = dw_logfilename;
				tArg->pstDownload = pstDownload;
				tArg->Status = RPC_TRUE;
				tArg->cancelStatus = CANCEL_FALSE;
				tArg->delay =  pstDownload->DelaySeconds;
				tArg->recv_time = currtime;


				if (g_rpc_Download)
				{
					TRACE (LOG_CRITICAL, "DRIVER", "In %s g_rpc_Download is not null\n",__FUNCTION__);

					rc = pthread_kill(g_rpc_Download, 0);
					if (!rc)
					{
						//Just Add in List
						ADD_IN_LIST(&g_Txn_List_Download, (YLNODE *)tArg);
					}
				}
				else
				{
					//  Start Of thread to actually download the file from remote server
					TRACE (LOG_CRITICAL, "DRIVER", "In %s g_rpc_Download is null\n",__FUNCTION__);

					if (!rt)
					{
						ADD_IN_LIST(&g_Txn_List_Download, (YLNODE *)tArg);

						pthread_attr_t attr;
						pthread_attr_init(&attr);
						pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
						pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s thread created for actual download using ipr_cwmp_actualDownload\n",__FUNCTION__);
						/*coverity 111414 Fix Start*/
						rc =pthread_create(&g_rpc_Download, &attr, ipr_cwmp_actualDownload, (void *)tArg);
						if(0 != rc)
					                TRACE(LOG_INFO , "DRIVER", "Thread creation failed with %d error code \n", rc);
						/*coverity 111414 Fix End*/

						pthread_attr_destroy(&attr);
						pthread_detach(g_rpc_Download);

					}
				}


			}
			DEALLOCATE(tmpBuffer);
			tmpBuffer = NULL;
			memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
			ENB_FCLOSE(dw_fp);
		}//If
		else if (strncmp(entry->d_name , "upl", 3 ) == 0)
        {
          /* SPR 21885 FIXED START */
          if (!g_rpc_IstxQ_init)
          {
              init_list(&g_Txn_List);
              g_rpc_IstxQ_init = 1;
          }
           /* SPR 21885 FIXED END */

		  /*download upload change starts*/	
			TRACE (LOG_CRITICAL, "DRIVER", "In %s entry read is of upload type\n",__FUNCTION__);
			strncpy(strPath , DW_DIR, strlen(DW_DIR));
			strncat(strPath, "/", 1);
			strncat(strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-((strlen(DW_DIR))+ 1)));
			TRACE (LOG_CRITICAL, "DRIVER", "In %s  strpath = %s\n",__FUNCTION__,strPath);
			/*download upload change ends*/
            /* SPR 17268 FIXED START */

   
            upl_fp = fopen((char *)strPath, "r");
            /* SPR 17268 FIXED END */

         //   upl_fp = fopen(strPath, "r");
            if (upl_fp == NULL)
            {
                perror("opendir:  File open error.");
        //        exit(1) ;
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */

                return;          
            }
            //pthread_t thRpc_Upload;
			tmpBuffer = (char *)ALLOCATE(1024 );
            if (tmpBuffer == NULL)
            { 
                perror("opendir: MEM allocation fail.");
          //      exit(1) ;
                /* Coverity 40682 + */
                ENB_FCLOSE(upl_fp);
                /* Coverity 40682 - */
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */

                return;      
            }  
            ret = fread((void *)tmpBuffer,1024, 1, upl_fp);
            if (ret != 0)
            {
                ENB_FCLOSE(upl_fp);
				DEALLOCATE(tmpBuffer);
                continue;
            }
            tmpBuffer[1023] = '\0';
            str = strtok(tmpBuffer , "\n");
			pUpload_t = (Upload *)ALLOCATE(sizeof (Upload));
            if (pUpload_t == NULL)
            { 
                perror("opendir: MEM allocation fail.");
                exit(0);
            }
            /*  coverity 96968 fix code removed */

            if (str != NULL)
            {
				pUpload_t->CommandKey  = (char *)ALLOCATE(COMMAND_KEY_SIZE);
                if (!pUpload_t->CommandKey)
                {
                    ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t);
                    /*  coverity 40683 fix start */
                    closedir(dp);
                    /*  coverity 40683 fix end */
                    return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(pUpload_t->CommandKey , "", 2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->CommandKey = %s\n",__FUNCTION__, pUpload_t->CommandKey);
					}
                    else
					{
                        strncpy(pUpload_t->CommandKey , str, COMMAND_KEY_SIZE);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->CommandKey = %s\n",__FUNCTION__, pUpload_t->CommandKey);
					}
                }
            }

            str = strtok(NULL , "\n");
            if (str != NULL)
            {
				pUpload_t->FileType = (char *)ALLOCATE(FILE_TYPE_SIZE);
                if (!pUpload_t->FileType)
                {
                    ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t->CommandKey);
					DEALLOCATE(pUpload_t);
                    /*  coverity 40683 fix start */
                    closedir(dp);
                    /*  coverity 40683 fix end */
                    return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(pUpload_t->FileType , "",2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->FileType = %s\n",__FUNCTION__, pUpload_t->FileType);
					}
                    else
					{
                        strncpy (pUpload_t->FileType , str, FILE_TYPE_SIZE-1);
					    TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->FileType = %s\n",__FUNCTION__, pUpload_t->FileType);
					}
                }
            }
            str = strtok(NULL , "\n");
            if (str != NULL)
            {
				pUpload_t->URL = (char *)ALLOCATE(URL_SIZE);
                if (!pUpload_t->URL)
                {
                    ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t->CommandKey);
					DEALLOCATE(pUpload_t->FileType);
					DEALLOCATE(pUpload_t);
                    /*  coverity 40683 fix start */
                    closedir(dp);
                    /*  coverity 40683 fix end */
                    return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(pUpload_t->URL , "",2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->URL = %s\n",__FUNCTION__, pUpload_t->URL);
					}
                    else
					{
                        strncpy(pUpload_t->URL , str, URL_SIZE);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->URL = %s\n",__FUNCTION__, pUpload_t->URL);
					}
                }
            }
            str = strtok(NULL , "\n");
            if (str != NULL)
            {
				pUpload_t->Username = (char *)ALLOCATE(USR_NAME_SIZE);
                if (!pUpload_t->Username)
                {
                    ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t->CommandKey);
					DEALLOCATE(pUpload_t->FileType);
					DEALLOCATE(pUpload_t->URL);
					DEALLOCATE(pUpload_t);
                    /* coverity 40683 fix start */
                    if(dp)
                        closedir(dp);
                    /* coverity 40683 fix end */
                    return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(pUpload_t->Username , "",2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->Username = %s\n",__FUNCTION__, pUpload_t->Username);
					}
                    else
					{
                        strncpy(pUpload_t->Username , str, USR_NAME_SIZE);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->Username = %s\n",__FUNCTION__, pUpload_t->Username);
					}
                }
            }

            str = strtok(NULL , "\n");
            if (str != NULL )
            {
				pUpload_t->Password = (char *)ALLOCATE(PWD_SIZE);
                if (!pUpload_t->Password)
                {
                    ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t->CommandKey);
					DEALLOCATE(pUpload_t->FileType);
					DEALLOCATE(pUpload_t->URL);
					DEALLOCATE(pUpload_t->Username);
					DEALLOCATE(pUpload_t);
                    /* Coverity 40683 + */
                    if(dp) {
                    closedir(dp); }
                    /* Coverity 40683 - */
                    return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(pUpload_t->Password , "",2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->Password = %s\n",__FUNCTION__, pUpload_t->Password);
					}
                    else
					{
                        strncpy(pUpload_t->Password , str, PWD_SIZE);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s pUpload_t->Password = %s\n",__FUNCTION__, pUpload_t->Password);
					}
                }
            }

            str = strtok(NULL , "\n");
            if (str != NULL)
            {
                pUpload_t->DelaySeconds = strtoul(str , NULL, 0);
				TRACE (LOG_CRITICAL, "DRIVER", "In %s   pUpload_t->DelaySeconds = %d\n",__FUNCTION__, pUpload_t->DelaySeconds);
            }
            str = strtok(NULL , "\n");
            if (str != NULL)
            {
				local_path = (char *)ALLOCATE(URL_SIZE);
                if (!local_path)
                {
					ENB_FCLOSE(upl_fp);
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(pUpload_t->CommandKey);
					DEALLOCATE(pUpload_t->FileType);
					DEALLOCATE(pUpload_t->URL);
					DEALLOCATE(pUpload_t->Username);
					DEALLOCATE(pUpload_t->Password);
					DEALLOCATE(pUpload_t);
                    /* Coverity 40683 + */
                    if(dp) {
                    closedir(dp); }
                    /* Coverity 40683 - */
					return;
                }
                else
                {
                    if (*str == '-')
					{
                        strncpy(local_path , "",2);
						TRACE (LOG_CRITICAL, "DRIVER", "In %s  local_path = %s\n",__FUNCTION__, local_path);
					}
                    else
					{
                        strncpy(local_path , str, URL_SIZE);
                        TRACE (LOG_CRITICAL, "DRIVER", "In %s  local_path = %s\n",__FUNCTION__, local_path);
					}
                }
            }
            str = strtok(NULL , "\n");
            if (str != NULL)
            {
                /*  coverity 96620 fix code removed */
				TRACE (LOG_CRITICAL, "DRIVER", "In %s DelaySeconds = %d \n",__FUNCTION__, pUpload_t->DelaySeconds);
            }
			tArg_upl_t = (ThreadUpload *)ALLOCATE(sizeof (ThreadUpload));
            if (tArg_upl_t  == NULL)
            {
                ENB_FCLOSE(upl_fp);
				DEALLOCATE(tmpBuffer);
                closedir(dp);
				DEALLOCATE(pUpload_t->CommandKey);
				DEALLOCATE(pUpload_t->FileType);
				DEALLOCATE(pUpload_t->URL);
				DEALLOCATE(pUpload_t->Username);
				DEALLOCATE(pUpload_t->Password);
				DEALLOCATE(pUpload_t);
				DEALLOCATE(local_path );
                return;
            }
			upl_logfilename = (char *)ALLOCATE(MAX_DW_LOG_NAME_LEN);
            if (upl_logfilename == NULL)
            {
                ENB_FCLOSE(upl_fp);
				DEALLOCATE(tmpBuffer);
                closedir(dp);
				DEALLOCATE(pUpload_t->CommandKey);
				DEALLOCATE(pUpload_t->FileType);
				DEALLOCATE(pUpload_t->URL);
				DEALLOCATE(pUpload_t->Username);
				DEALLOCATE(pUpload_t->Password);
				DEALLOCATE(pUpload_t);
				DEALLOCATE(tArg_upl_t);
				DEALLOCATE(local_path );
                return;
            }
            
                /*SPR 17268 FIXED START*/
            strncpy(upl_logfilename ,(char *) strPath, MAX_DW_LOG_NAME_LEN);
                /*SPR 17268 FIXED END*/
            tArg_upl_t->upl_logfilename = upl_logfilename;
            tArg_upl_t->pstupload = pUpload_t;
            tArg_upl_t->local_path = local_path;
            if (pUpload_t)
            {
                tArg_upl_t->delay = pUpload_t->DelaySeconds;
            }
            time(&(tArg_upl_t->recv_time));
            tArg_upl_t->Status = RPC_TRUE;
            tArg_upl_t->cancelStatus = CANCEL_FALSE;


            if (g_th_upload)
            {
				TRACE (LOG_CRITICAL, "DRIVER", "In %s g_th_upload is not null\n",__FUNCTION__);
                rc =  pthread_kill(g_th_upload , 0);
                if (!rc)
                {
                    //Just Add in List
                    ADD_IN_LIST(&g_Txn_List , (YLNODE *)tArg_upl_t);
                }
            }                       
            else
            {
				TRACE (LOG_CRITICAL, "DRIVER", "In %s g_th_upload is null\n",__FUNCTION__);
                /*  coverity 96620 fix code removed */
             /*SPR 21885 FIXED START */
             ADD_IN_LIST(&g_Txn_List , (YLNODE *)tArg_upl_t);

             // set stack size attibute
             pthread_attr_t attr;
             pthread_attr_init(&attr);
             pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
             pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

             rc = pthread_create(&g_th_upload , &attr, ipr_cwmp_actualUpload , (void *)tArg_upl_t);
             if(0 != rc)
                 TRACE(LOG_INFO , "DRIVER", "Thread creation upload failed with %d error \n", rc);
             pthread_attr_destroy(&attr);
             pthread_detach(g_th_upload);
             /*SPR 21885 FIXED END */

            }
			DEALLOCATE(tmpBuffer);
            tmpBuffer = NULL;        
            ENB_FCLOSE(upl_fp);
            memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
        }//If
		else if(strncmp(entry->d_name , "schdwn", 6) == 0)
		{
			rt = 0;
			strncpy(strPath , DW_DIR, 11);
			strncat(strPath, "/", 2);
			strncat(strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-13));
            /* SPR 17268 FIXED START */

            dw_fp = fopen((char *)strPath, "r");
            /* SPR 17268 FIXED END */




			//dw_fp = fopen(strPath, "r");
			if (dw_fp == NULL)
			{
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */

				return ;
			}
			tmpBuffer = (char *)ALLOCATE(1024 );
			if (tmpBuffer == NULL)
			{
                /* COV 113218 fix start */
                if(TR069_ZERO != closedir(dp))
                {
                    TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
                }
                /* COV 113218 fix end */

				ENB_FCLOSE(dw_fp);
				return;
			}            ret = fread((void *)tmpBuffer,1024, 1, dw_fp);
			if (ret != 0)
            {
				ENB_FCLOSE(dw_fp);
				DEALLOCATE(tmpBuffer);
				continue;
            }
			tmpBuffer[1023] = '\0';
			pstSCHDwn = (ScheduleDownload *)ALLOCATE(sizeof(ScheduleDownload));
			if(NULL == pstSCHDwn)
			{
				closedir(dp);
				DEALLOCATE(tmpBuffer);
				ENB_FCLOSE(dw_fp);
				return ;
			}
			ThreadSchDwn *th_SchDwn = ALLOCATE(sizeof(ThreadSchDwn));
			if(NULL == th_SchDwn)
			{
				closedir(dp);
				DEALLOCATE(tmpBuffer);
				ENB_FCLOSE(dw_fp);
				DEALLOCATE(pstSCHDwn);
				return ;
			}
			str = NULL;
			str = strtok(tmpBuffer , "\n");
			if (str != NULL)
			{
				pstSCHDwn->CommandKey  = (char *)ALLOCATE(COMMAND_KEY_SIZE);
				if (!pstSCHDwn->CommandKey)
				{
					rt  = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->CommandKey , "", 2);
					else
						strncpy(pstSCHDwn->CommandKey , str, COMMAND_KEY_SIZE);
				}
			}

			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				pstSCHDwn->FileType = (char *)ALLOCATE(FILE_TYPE_SIZE);
				if (!pstSCHDwn->FileType)
				{
					rt = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->FileType , "",2);
					else
						strncpy (pstSCHDwn->FileType , str, FILE_TYPE_SIZE-1);
				}
			}
			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				pstSCHDwn->URL = (char *)ALLOCATE(URL_SIZE);
				if (!pstSCHDwn->URL)
				{
					rt = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->URL , "",2);
					else
						strncpy(pstSCHDwn->URL , str, URL_SIZE);
				}
			}
			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				pstSCHDwn->Username = (char *)ALLOCATE(USR_NAME_SIZE);
				if (!pstSCHDwn->Username)
				{
					rt = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->Username , "",2);
					else
						strncpy(pstSCHDwn->Username , str, USR_NAME_SIZE);
				}
			}

			str = strtok(NULL , "\n");
			if (str != NULL )
			{
				pstSCHDwn->Password = (char *)ALLOCATE(PWD_SIZE);
				if (!pstSCHDwn->Password)
				{
					rt = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->Password , "",2);
					else
						strncpy(pstSCHDwn->Password , str, PWD_SIZE);
				}
			}
			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				pstSCHDwn->FileSize = strtoul(str, NULL, 0);
			}

			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				pstSCHDwn->TargetFileName = (char *)ALLOCATE(TRG_NAME_SIZE);
				if (!pstSCHDwn->TargetFileName)
				{
					rt = 1;
				}
				else
				{
					if (*str == '-')
						strncpy(pstSCHDwn->TargetFileName, "",2);
					else
						strncpy(pstSCHDwn->TargetFileName , str, TRG_NAME_SIZE);
				}
			}

			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				l_size = atoi(str);
			}

			if (l_size > 0)
			{
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_1_start = strtoul(str , NULL, 0);
				}           
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_1_end = strtoul(str , NULL, 0);
				}                       

			}
			if (l_size > 1)
			{
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_2_start = strtoul(str , NULL, 0);
				}           
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_2_end = strtoul(str , NULL, 0);
				}                       
			}
			else
			{
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_2_start = th_SchDwn->timeWin_1_start;
				}
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					th_SchDwn->timeWin_2_end = th_SchDwn->timeWin_1_end;
				}
			}
			str = strtok(NULL , "\n");
			if (str != NULL)
			{   
				th_SchDwn->rec_time = strtol(str, NULL,10);
			}

			th_SchDwn->Status = RPC_TRUE;
			th_SchDwn->cancelStatus = CANCEL_FALSE;
			th_SchDwn->ptr_dwn_t = pstSCHDwn;

			dw_logfilename = (char *)ALLOCATE(MAX_DW_LOG_NAME_LEN);
			if (dw_logfilename)
			{
                
                /*SPR 17268 FIXED START*/
				strncpy(dw_logfilename , (char *)strPath, MAX_DW_LOG_NAME_LEN);
                /*SPR 17268 FIXED END*/
				th_SchDwn->dw_logfilename = dw_logfilename;

			}
			else
			{
				rt = 1;
			}
			if (!g_rpc_IsSchDwnQ_init)
			{
				init_list(&g_Txn_List_Sch_Dwn);
				g_rpc_IsSchDwnQ_init = 1;
				g_rpc_sch_dwn_th = 0;
			}


			if (g_rpc_sch_dwn_th)
			{
				rc =  pthread_kill(g_rpc_sch_dwn_th , 0);
				if (!rc)
				{
					//Just Add in List
					ADD_IN_LIST(&g_Txn_List_Sch_Dwn, (YLNODE *)th_SchDwn);
				}
			}
			else
			{

				if (!rt)
				{
					ADD_IN_LIST(&g_Txn_List_Sch_Dwn, (YLNODE *)th_SchDwn);

					pthread_attr_t attr;
					pthread_attr_init(&attr);
					pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
					pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
					rc = pthread_create(&g_rpc_sch_dwn_th, &attr, 
							ipr_cwmp_actualScheduleDownload  , (void *)th_SchDwn);

                    /*  coverity 86560 fix start */
                    if(rc == 0)
                        TRACE (LOG_INFO, "DRIVER", "Successfully created thread");
                    /*  coverity 86560 fix end */
					pthread_attr_destroy(&attr);
					pthread_detach(g_rpc_sch_dwn_th);
				}
			}
			DEALLOCATE(tmpBuffer);
			tmpBuffer = NULL;
			ENB_FCLOSE(dw_fp);
			memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
		}

	} //While
     /* COV 113218 fix start */
        if(TR069_ZERO != closedir(dp))
        {
            TRACE (LOG_MAJOR , GLUE_LAYER,"  Error in closedir, errno %d: %s",errno,strerror(errno));
        }
        /* COV 113218 fix end */
    TRACE (LOG_CRITICAL, "DRIVER", "Exiting function %s\n",__FUNCTION__);	
}
/*download upload change ends*/

void sessionInitCallback (char *sessionId ,char * URL)
{
	if (stCallbackFunList.sessionInitCallback != NULL)
	    stCallbackFunList.sessionInitCallback (sessionId , URL);
}

/*****************************************************************************
* FUNCTION:            	initialize_TR069_Client 
*
* DESCRIPTION:          This function initialize the TR069 Library. and creates  
			            a new event. "0 BOOTSTRAP"
* ARGS:                	sDeviceInfo              sDeviceIdInfo  (Info about device)  
			            ParameterValueList     sParamValList  (Parameter List) 
			            Tr69Parameters_t        tr69Parameters (TR69 Parameters )
			            sCallbackFunList_t      sCallbackFunList (Calllback functions)
			            char *                  HeMS_URL ( ACS URL ) 
			            bool Init_flag
*
* RETURNS:              NONE. 
*
******************************************************************************/

void initialize_TR069_Client (sDeviceInfo              sDeviceIdInfo,
		ParameterValueList     sParamValList,
		Tr69Parameters_t        tr69Parameters,
		sCallbackFunList_t      sCallbackFunList,
		char *                  HeMS_URL,
		enum eFlag   			Init_flag) 
{
    /*  coverity 86660 fix start */
	int rt;
    /*  coverity 86660 fix end */
	int rc = 0 ;
    /*SPR 18262 FIXED START*/
    /*SPR 18262 FIXED END*/
	FILE *fp1  = NULL;
	FILE *file = NULL;
	FILE *cmd_key_file = NULL;
	char s1[256] = "";
	Uint8 *event_code = NULL;
	Uint8 *cmd_key = NULL;
	sEventStruct *newEvent1 = NULL;
	sEventStruct *rec_event = NULL;
	ClientRequest *sClientReq = NULL;
	Uint8 *FaultCode = NULL;
	Uint8 *FaultString = NULL;

    /* SPR 16732 FIX START */
    char buff[256] = {0}; 
    Uint8 start_dwn[256] = {0};
    time_t t1,t2;
    struct tm tm = {0};
    /* SPR 16732 FIX END */
	int bytes_read = 0;

	ParameterValueList *stParam = NULL;
	//OAM/IPSEC SI code changes
	read_tr69_lib_config();
	logger_init();
	event_manager_init();
	client_manager_init();


	sEventStruct *newEvent = (sEventStruct *) ALLOCATE(sizeof (sEventStruct)) ;
	if(NULL == newEvent)
	{
		TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
		return ;
	}
	newEvent1 =  (sEventStruct *) ALLOCATE(sizeof (sEventStruct)) ;
	if(NULL == newEvent1)
	{
		TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
		DEALLOCATE(newEvent);       
		return ;
	}
	rec_event =  (sEventStruct *) ALLOCATE(sizeof (sEventStruct)) ;
	if(NULL == rec_event)
	{
		TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
		DEALLOCATE(newEvent);       
		DEALLOCATE(newEvent1);       
		return ;
	}
    /* SPR 18746 FIX START */
    /* code removed */
	stParam = (ParameterValueList *) ALLOCATE( sizeof(ParameterValueList));
    /* SPR 18746 FIX END */
	if(NULL == stParam)
	{
		TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
		DEALLOCATE(rec_event);       
		DEALLOCATE(newEvent);       
		DEALLOCATE(newEvent1);       
		return ;
	}

	stCallbackFunList.sessionInitCallback = sCallbackFunList.sessionInitCallback;
	stCallbackFunList.sessionFailedCallback = sCallbackFunList.sessionFailedCallback;
	stCallbackFunList.sessionClosedCallback = sCallbackFunList.sessionClosedCallback;
	stCallbackFunList.invokeCallback =  sCallbackFunList.invokeCallback;

	/* Copying the Device I dinfor */

	ipr_cwmp_setManufacturerOUI((UInt8 *)sDeviceIdInfo.OUI);
	ipr_cwmp_setSerialNumber((UInt8 *)sDeviceIdInfo.SerialNumber);
	ipr_cwmp_setProductClass((UInt8 *)sDeviceIdInfo.ProductClass);	
	ipr_cwmp_setHost((UInt8 *)sDeviceIdInfo.Manufacturer);

	/*SPR 16920 Fixed Start */
	initialize_tr69Parameters_parameter(&tr69Parameters);
	/*SPR 16920 Fixed Start */

	rt = copy_TR069Parameters(tr69Parameters);
/* SPR 19838: Coverity:12932 FIX START */
    FREE_tr69Parameters(tr69Parameters);
/* SPR 19838: Coverity:12932 FIX STOP */
	/* Success 0 fail 1 */
	if (rt)
	{
		TRACE (LOG_CRITICAL, "DRIVER", "\n Failed during initialization of TR069 Parameters");
	}

	if (HeMS_URL == NULL)
	{
		/*    USe ACS discovery 
		*/
		ipr_cwmp_acs_discovery('n');
	}			
	else
	{
		rc = sscanf(HeMS_URL,"http://%s",s1);
		if(!rc)
			rc = sscanf(HeMS_URL,"https://%s",s1);

		if (rc == 0 )
		{
			TRACE (LOG_CRITICAL, "DRIVER", "\n Invalid URL ");
			exit(1);

		}
		ipr_cwmp_setACSURL (HeMS_URL);
		ipr_cwmp_acs_discovery ('n');
	}

	/*SPR 19863 FIXED START */
	ipr_cwmp_Init_SSL_param();
	/*SPR 19863 FIXED END */

        /* QUEUED DOWNLOAD AND UPLOAD CHANGE START */

	//code remove 
	/* QUEUED DOWNLOAD AND UPLOAD CHANGE END */
	/* SPR 20429 FIXED START */
    FILE *fp = NULL;
	UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
	strcpy((char *)ABSPath2,(char *)ABSPath);

	TRACE (LOG_CRITICAL, "DRIVER", "\n  ABSPath in BOOTSTRAP before concat %s  " , ABSPath2);
	strncat((char *)ABSPath2 , BOOTSTRAP_FILE_NAME,  strlen(BOOTSTRAP_FILE_NAME));
	TRACE (LOG_CRITICAL, "DRIVER", "\n  ABSPath in BOOTSTRAP after concat %s  " , ABSPath2);
	fp = fopen((char *)ABSPath2, "w");
 	
	if (fp != NULL)
		ENB_FCLOSE(fp);

	/* SPR 20429 FIXED END */
        /* QUEUED DOWNLOAD AND UPLOAD CHANGE END */

   /* SPR 20838 FIXED START */
   /* code removed */
   /* SPR 20838 FIXED END */
	lock_EQueue();
	if (newEvent != NULL && stParam != NULL)
	{

		stParam->__ptrParameterValueStruct = sParamValList.__ptrParameterValueStruct;
		stParam->__size = sParamValList.__size;        

        /* SPR 17268 FIXED START */
    
            UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
            strcpy((char *)ABSPath1,(char *)ABSPath);

            TRACE (LOG_CRITICAL, "DRIVER", "\n  ABSPath in BOOTSTRAP before concat %s  " , ABSPath1);
            strncat((char *)ABSPath1 , BOOTSTRAP_FILE_NAME,  strlen(BOOTSTRAP_FILE_NAME));
            TRACE (LOG_CRITICAL, "DRIVER", "\n  ABSPath in BOOTSTRAP after concat %s  " , ABSPath1);
            fp1 = fopen((char *)ABSPath1, "r");  
            /* SPR 17268 FIXED END */




		//fp1 = fopen(BOOTSTRAP_FILE_NAME, "r"); 
		if (NULL == fp1) 
		{
			newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE);
			if (newEvent->EventCode != NULL)
			{
				strncpy(newEvent->EventCode ,BOOTSTRAP_CODE ,  EVENT_CODE_SIZE);
				newEvent->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
				if(NULL == newEvent->CommandKey)
				{ 
					DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);

                    /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					return ;
				}
				strncpy(newEvent->CommandKey, "",2);
			}

			ipr_cwmp_addEvent(newEvent , stParam);
           /* SPR 20838 FIXED START */
           FILE *fp = NULL;
           UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
           strcpy((char *)ABSPath2,(char *)ABSPath);

           TRACE (LOG_DETAIL, "DRIVER", "\n  ABSPath in BOOTSTRAP before concat %s  " , ABSPath2);
           strncat((char *)ABSPath2 , BOOTSTRAP_FILE_NAME,  strlen(BOOTSTRAP_FILE_NAME));
           TRACE (LOG_DETAIL, "DRIVER", "\n  ABSPath in BOOTSTRAP after concat %s  " , ABSPath2);
           fp = fopen((char *)ABSPath2, "w");
           TRACE (LOG_DETAIL, "DRIVER", "\n  ABSPath in BOOTSTRAP file created first time");

           if (fp != NULL)
               ENB_FCLOSE(fp);

           /* SPR 20838 FIXED END */

            /*SPR 18262 FIXED START*/
           /* code remove */
            /*SPR 18262 FIXED END*/
           /* SPR 22092 FIXED Start */
           /* Code Remove */
           /* SPR 22092 FIXED Start */




        }else
        {
			TRACE (LOG_CRITICAL, "DRIVER", "\n BOOTSTRAP file present , closing it now");
            ENB_FCLOSE(fp1);
            /* SPR 21563 FIXED START */
            DEALLOCATE(newEvent);
            /* SPR 21563 FIXED END */
        }

		usleep(50000);

#ifdef VENDOR_SPECIFIC


          /* SPR 17268 FIXED START */
            UInt8 ABSPath3[CONFIG_PARAM_LEN] = {'\0'};
            strcpy((char *)ABSPath3,(char *)ABSPath);

            TRACE (LOG_CRITICAL, "DRIVER", "\n  ABSPath in SOFTWARE_VERS  before concat %s  " , ABSPath3);
            strncat((char *)ABSPath3 , SOFTWARE_VERS,  strlen(SOFTWARE_VERS));
            TRACE (LOG_CRITICAL, "DRIVER", "\n   SOFTWARE_VERS path  is =  %s  " , ABSPath3);
            file = fopen((char *)ABSPath3, "rb");
            /* SPR 17268 FIXED END */
		//file = fopen(SOFTWARE_VERS , "rb");
		if (NULL != file)
		{
			Uint8 *currVers = NULL;
			Uint8 *cmdkey = NULL;
			currVers = (Uint8 *) ALLOCATE( STR_MAX_LEN);
			if(NULL == currVers)
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
				exit(0); 
			}
			if(0 ==  ENB_FSCANF(file, "%64s", currVers))
			{
				perror("ENB_FSCANF");
			}
			ENB_FCLOSE(file);
			/* SPR 17268 FIXED START */
			if( 0 != remove((char *)ABSPath3))
			/* SPR 17268 FIXED END */
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n  remove failed ");
				exit(0); 
			}
			Uint8 *newVers = NULL;

			newVers = (Uint8 *)ipr_cwmp_getSoftVers();
			if(NULL == newVers)
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
				exit(0); 
			}
			FaultCode = ALLOCATE( FAULT_CODE_SIZE);
			if(NULL == FaultCode)
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
				exit(0); 
			}
			FaultString = (Uint8 *)ALLOCATE( FAULT_STRING_SIZE);
			if(NULL == FaultString)
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
				exit(0); 
			}

			if (strcmp(newVers , currVers) == 0)
			{
                                /* SPR 19951 FIXED START */
				TRACE (LOG_CRITICAL, "DRIVER", "same software version\n");
                                /* SPR 19951 FIXED END */
				strncpy(FaultCode , FAULT_CODE_9018 , FAULT_CODE_SIZE);
				strncpy(FaultString , FAULT_CODE_9018_STR , FAULT_STRING_SIZE);
			}
			else
			{
				strncpy(FaultCode , "0", 2);
				strcpy(FaultString , " ");
			}

			sClientReq = (ClientRequest *)ALLOCATE( sizeof(ClientRequest));
			if(NULL == sClientReq)
			{
				/* SPR 21563 FIXED START */
                       /* code removed */ 
                /* SPR 21563 FIXED END */
                /* SPR 18746 FIX START */
				DEALLOCATE(stParam);       
                /* SPR 18746 FIX END */
				DEALLOCATE(rec_event);
                /* SPR 21563 FIXED START */
                    /* code removed */
                /* SPR 21563 FIXED END */
				       
				DEALLOCATE(newEvent1);       
				DEALLOCATE(FaultString);       
				DEALLOCATE(FaultCode);       
				DEALLOCATE(currVers);  
				DEALLOCATE(newVers);       
				return ;
			}
			sClientReq->eReqtype = TRANS_COMPLETE_REQ;
			sClientReq->pstTxComplete = ALLOCATE( sizeof (struct _cwmp__TransferComplete));
			if (sClientReq->pstTxComplete  != NULL)
			{
				sEventStruct* sEvent = NULL;
				sEventStruct* sEvent2 = NULL;
				sEvent = (sEventStruct *)ALLOCATE(sizeof(sEventStruct) );
				if(NULL == sEvent)
				{
				/* SPR 21563 FIXED START */
                       /* code removed */
                /* SPR 21563 FIXED END */
                           
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                   /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					/* SPR 21563 FIXED START */
                          /* code removed */
                    /* SPR 21563 FIXED END */ 
					DEALLOCATE(newEvent1);       
					DEALLOCATE(sClientReq);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(currVers);  
					return ;
				}
				sEvent->EventCode = (char *)ALLOCATE(EVENT_CODE_SIZE );
				if(NULL == sEvent->EventCode)
				{
					/* SPR 21563 FIXED START */
                           /* code removed */
                    /* SPR 21563 FIXED END */       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                    /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					/* SPR 21563 FIXED START */
                            /* code removed */
                    /* SPR 21563 FIXED END */            
					DEALLOCATE(newEvent1);       
					DEALLOCATE(sClientReq);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent);       
					DEALLOCATE(currVers);  
					return ;
				}
				sEvent->CommandKey = (char *)ALLOCATE(EVENT_CODE_SIZE );
				/*SPR 17217 Fixed Start */
				memset(sEvent->CommandKey,0,EVENT_CODE_SIZE);
				/*SPR 17217 Fixed End */

				if(NULL == sEvent->CommandKey)
                {
                    /* SPR 21563 FIXED START */
                    /* code removed */
                    /* SPR 21563 FIXED END */
					       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                   /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					/* SPR 21563 FIXED START */
                          /* code removed */
                    /* SPR 21563 FIXED END */        
					DEALLOCATE(newEvent1);       
					DEALLOCATE(sClientReq);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode);       
					DEALLOCATE(sEvent);       
					DEALLOCATE(currVers);  
					return ;
				}
				cmdkey = ALLOCATE( COMMAND_KEY_SIZE);
				if(NULL == cmdkey)
				{
					DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                   /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(sClientReq);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->CommandKey);       
					DEALLOCATE(sEvent->EventCode);       
					DEALLOCATE(sEvent);       
					DEALLOCATE(currVers);  
					return ;
				}



        /* SPR 17268 FIXED START */
        UInt8 ABSPath4[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath4,(char *)ABSPath);

        TRACE ( LOG_CRITICAL , "DRIVER" , "ABSPath in UPD_DWN_KEY before concat %s ",ABSPath4 );
        strncat((char *)ABSPath4 , UPD_DWN_CMDKEY, strlen(UPD_DWN_CMDKEY));
        TRACE ( LOG_CRITICAL , "DRIVER" , " UPD_DWN_KEY path  is =  %s ",ABSPath4 );
        file = fopen((char *)ABSPath4 ,"r");
        /* SPR 17268 FIXED END */


				if (NULL != file)
				{
                    /* SPR 16732 FIX START */
                    while(fgets(buff, 100, file) != NULL) {
                                            
                                /* SPR 19951 FIXED START */
					   TRACE (LOG_CRITICAL, "DRIVER","file read CMDKEY = %s", buff);
                                /* SPR 19951 FIXED END */
                        if(snscanf((const char *)buff,sizeof(buff),"CMDKEY=%s",cmdkey) > 0)
					    {
                            strtok (buff,"=");
                            strcpy(cmdkey,strtok (NULL,"="));
                            TRACE (LOG_CRITICAL, "DRIVER","CMDKEY=%s",cmdkey);
                /* SPR 17217 fixed start */
                char *pos;
                if ((pos=strchr(cmdkey, '\n')) != NULL)
                {
                    TRACE ( LOG_CRITICAL , "DRIVER" , " Newline found" );
                    *pos = '\0';
               }
                /* SPR 17217 fixed end */


                        } 
                        if(snscanf((const char *)buff,sizeof(buff),"DOWNLOAD_START_TIME=%s",start_dwn) > 0)
                        {
                            strtok (buff,"=");
                            /* Coverity 96625 + */
                            strncpy(start_dwn,strtok (NULL,"="),256);
                            start_dwn[255] = '\0';
                            /* Coverity 96625 - */
                            TRACE (LOG_CRITICAL, "DRIVER"," DOWNLOAD_START_TIME=%s",start_dwn);
                        } 
					}
					ENB_FCLOSE(file);
					
       				 /* SPR 17268 FIXED START */
					if( 0 != remove((char *)ABSPath4))
        			 /* SPR 17268 FIXED END */
					{
						TRACE (LOG_CRITICAL, "DRIVER", "\n  rem failed  ");
						exit(0); 
					}

                    /* SPR 16732 FIX END */
				}
                /* SPR 16732 FIX START */
                strptime(start_dwn, "%F %T", &tm);
                setenv("TZ", "", 1);
                tzset();
                t1 = mktime(&tm);
                time(&t2);
                /* SPR 16732 FIX END */
				sClientReq->pstTxComplete->CommandKey = cmdkey;
				sClientReq->EventCode = ALLOCATE(EVENT_CODE_SIZE);
                /* Coverity 40677 + */
				if (NULL == sClientReq->EventCode)
                {
					DEALLOCATE(newEvent->EventCode);       
					DEALLOCATE(stParam);       
					DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode );    
					DEALLOCATE(sEvent->CommandKey );    
					DEALLOCATE(sEvent);    
					DEALLOCATE(sClientReq->pstTxComplete );    
					DEALLOCATE(sClientReq);       
					DEALLOCATE(currVers);  
					return ;
                }    
                /* Coverity 40677 - */

				strncpy(sClientReq->EventCode, DOWNLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);
                /* SPR 16732 FIX START */
                sClientReq->pstTxComplete->StartTime = t1;
                sClientReq->pstTxComplete->CompleteTime = t2;
                /* SPR 16732 FIX END */
				sClientReq->pstTxComplete->FaultStruct = ALLOCATE(
						sizeof(struct cwmp__TransferCompleteFaultStruct));
				if(NULL == sClientReq->pstTxComplete->FaultStruct)
				{
					DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                    /* SPR 18746 FIX END */
					DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode );    
					DEALLOCATE(sEvent->CommandKey );    
					DEALLOCATE(sEvent);    
					DEALLOCATE(sClientReq->pstTxComplete );    
					DEALLOCATE(sClientReq->EventCode );    
					DEALLOCATE(sClientReq);       
					DEALLOCATE(currVers);  
					return ;
				}
				sClientReq->pstTxComplete->FaultStruct->FaultCode = FaultCode;
				sClientReq->pstTxComplete->FaultStruct->FaultString = FaultString;
				QUEUE(&g_clientRequestQ , (YSNODE *)sClientReq);
				strncpy(sEvent->EventCode ,DOWNLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);

				strncpy(sEvent->CommandKey , cmdkey, COMMAND_KEY_SIZE);

				sEvent2 = (sEventStruct *)ALLOCATE(sizeof(sEventStruct) );
				if(NULL == sEvent2)
				{
					DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
					DEALLOCATE(stParam);       
                    /* SPR 18746 FIX START */
					DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode );    
					DEALLOCATE(sEvent->CommandKey );    
					DEALLOCATE(sEvent);    
					DEALLOCATE(sClientReq->pstTxComplete );    
					DEALLOCATE(sClientReq->EventCode );    
					DEALLOCATE(sClientReq);       
					DEALLOCATE(currVers);  
					return ;
				}
				sEvent2->EventCode = (char *)ALLOCATE(EVENT_CODE_SIZE );
				if(NULL == sEvent2->EventCode )
				{
                    DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
                    DEALLOCATE(stParam);       
                    /* SPR 18746 FIX START */
                    DEALLOCATE(rec_event);       
					DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode );    
					DEALLOCATE(sEvent->CommandKey );    
					DEALLOCATE(sEvent);    
					DEALLOCATE(sClientReq->pstTxComplete );    
					DEALLOCATE(sClientReq->EventCode );    
					DEALLOCATE(sClientReq);    
					DEALLOCATE(sEvent2);       
					DEALLOCATE(currVers);  
					return ;
				}
				sEvent2->CommandKey = (char *)ALLOCATE(EVENT_CODE_SIZE );
				if(NULL == sEvent2->CommandKey)
                {
                    DEALLOCATE(newEvent->EventCode);       
                    /* SPR 18746 FIX START */
                    DEALLOCATE(stParam);       
                    /* SPR 18746 FIX START */
                    DEALLOCATE(rec_event);       
                    DEALLOCATE(newEvent);       
					DEALLOCATE(newEvent1);       
					DEALLOCATE(FaultString);       
					DEALLOCATE(FaultCode);       
					DEALLOCATE(newVers);       
					DEALLOCATE(sEvent->EventCode );    
					DEALLOCATE(sEvent->CommandKey );    
					DEALLOCATE(sEvent);    
					DEALLOCATE(sClientReq->pstTxComplete );    
					DEALLOCATE(sClientReq->EventCode );    
					DEALLOCATE(sClientReq);    
					DEALLOCATE(sEvent2->EventCode);       
					DEALLOCATE(sEvent2);       
					DEALLOCATE(currVers);  
					return ;
				}
				strncpy(sEvent2->EventCode , "7 TRANSFER COMPLETE", EVENT_CODE_SIZE);
				strcpy(sEvent2->CommandKey, "") ;
				ipr_cwmp_addEvent(sEvent2, NULL);

				usleep (10000);
				ipr_cwmp_addEvent(sEvent ,NULL);
			}
            /* Coverity 56073 + */
            else
            {
                if (sClientReq)
                {
                    DEALLOCATE(sClientReq->pstTxComplete);
                    DEALLOCATE(sClientReq);
                    sClientReq = NULL;
                }   

                DEALLOCATE(FaultString);       
                DEALLOCATE(FaultCode);       
            }    
            /* Coverity 56073 - */
			DEALLOCATE(currVers);  
		}
#endif

		
        /* QUEUED DOWNLOAD AND UPLOAD CHANGE START */
	        queueDownload();	

        /* QUEUED DOWNLOAD AND UPLOAD CHANGE END */
		if (newEvent1 != NULL)
		{
			newEvent1->EventCode = ALLOCATE(EVENT_CODE_SIZE );
			if (newEvent1->EventCode != NULL)
			{
				strncpy(newEvent1->EventCode ,BOOT_CODE ,  EVENT_CODE_SIZE);
				newEvent1->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
				if(NULL == newEvent1->CommandKey)
                {
                    DEALLOCATE(newEvent1->EventCode);
                    DEALLOCATE(newEvent1);
                    /* SPR 19838: Coverity:12932 FIX START */
                    DEALLOCATE(stParam);
                    /* SPR 19838: Coverity:12932 FIX STOP */
                    DEALLOCATE(newEvent);
                    DEALLOCATE(rec_event);
                    /* COVERITY CID: 56073 FIX START */
                    /* COVERITY CID: 88483 FIX START */
                    if (sClientReq)
                    {
                        DEALLOCATE(sClientReq->pstTxComplete);
                        DEALLOCATE(sClientReq->EventCode);
                        DEALLOCATE(sClientReq);
                    }
                    /* COVERITY CID: 88483 FIX END */
                    /* COVERITY CID: 56073 FIX END */
					return ;
				}
				strncpy(newEvent1->CommandKey, "", 2);
			}
			ipr_cwmp_addEvent(newEvent1 , stParam);
		}

        /* SPR 17268 FIXED START */
        UInt8 ABSPath5[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath5,(char *)ABSPath);
        TRACE ( LOG_CRITICAL , "DRIVER" , "ABSPath in EVENT_CODE_FILE  before concat %s ",ABSPath5 );
	    strncat((char *)ABSPath_EVENT_CODE_FILE , (char *)ABSPath5 ,strlen((char *)ABSPath));
        strncat((char *)ABSPath_EVENT_CODE_FILE , EVENT_CODE_FILE, strlen(EVENT_CODE_FILE));
        TRACE ( LOG_CRITICAL , "DRIVER" , "ABSPath in EVENT_CODE_FILE after concat %s ",ABSPath5 );
        file = fopen((char *)ABSPath_EVENT_CODE_FILE ,"r");
        /* SPR 17268 FIXED END */




		if (NULL != file) 
		{
			rewind(file);
			event_code = (Uint8 *) ALLOCATE( EVENT_CODE_SIZE);
			if(NULL == event_code)
			{
				TRACE (LOG_CRITICAL, "DRIVER", "\n ALLOCATE failed   ");
				exit(0);
			}

			while( !feof( file ) )
			{
				/* Attempt to read in 10 bytes: */
				bytes_read = fread(event_code, EVENT_CODE_SIZE, 1, file);
				TRACE (LOG_NONE, "DRIVER", "\n  bytes read from cmd key file %d ",bytes_read );
				if( ferror( file ) )      {
					perror( "Read error" );
					break;
				}

			}

            /* Coverity 40938 + */
			event_code[EVENT_CODE_SIZE - 1] = '\0';
            /* Coverity 40938 - */
			TRACE (LOG_CRITICAL, "DRIVER", "\n event code %s   ", event_code);
			ENB_FCLOSE(file);
			if (rec_event != NULL)
			{
				rec_event->EventCode = ALLOCATE(EVENT_CODE_SIZE );
				if (rec_event->EventCode != NULL)
				{
					strncpy(rec_event->EventCode ,event_code ,  EVENT_CODE_SIZE -1);


       /* SPR 17268 FIXED START */
        UInt8 ABSPath6[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath6,(char *)ABSPath);

        TRACE ( LOG_CRITICAL , "DRIVER" , "ABSPath in REBOOT_CMD_KEY before concat %s ",ABSPath6 );
        strncat((char *)ABSPath6 , REBOOT_CMD_KEY, strlen(REBOOT_CMD_KEY));
        TRACE ( LOG_CRITICAL , "DRIVER" , "ABSPath in REBOOT_CMD_KEY after concat %s ",ABSPath6 );
        cmd_key_file = fopen((char *)ABSPath6 ,"r");
        /* SPR 17268 FIXED END */



                
					if (NULL == cmd_key_file) 
					{
						rec_event->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
						if(NULL == rec_event->CommandKey)
						{
							DEALLOCATE(rec_event->EventCode);
							DEALLOCATE(rec_event);
							TRACE (LOG_CRITICAL, "DRIVER", "\n rec_event->CommandKey   ALLOCATE fail    ");
							exit(0);
						}
						strncpy(rec_event->CommandKey, "", 2);
					}
					else
					{
						rewind(cmd_key_file);
						cmd_key  = (Uint8 *) ALLOCATE(COMMAND_KEY_SIZE);
						if(NULL == cmd_key)
						{
							TRACE (LOG_CRITICAL, "DRIVER", "\n  ALLOCATE fail    ");
							exit(0);
						}

						while( !feof( cmd_key_file ) )
						{
							/* Attempt to read in 10 bytes: */
							bytes_read =	fread(cmd_key, COMMAND_KEY_SIZE, 1, cmd_key_file);
							TRACE (LOG_NONE, "DRIVER", "\n  bytes read from cmd key file %d ",bytes_read );
							if( ferror( cmd_key_file ) )      {
								perror( "Read error" );
								break;
							}

						}
						cmd_key[COMMAND_KEY_SIZE - 1] = '\0';

						rec_event->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
						if(NULL == rec_event->CommandKey)
						{
							TRACE (LOG_CRITICAL, "DRIVER", "\n  ALLOCATE fail    ");
							exit(0);
						}
						strncpy(rec_event->CommandKey,cmd_key ,strlen(cmd_key));

						/*SPR 17268 FIXED START */
						if(0 !=   remove((char *)ABSPath6))
						/* SPR 17268 FIXED END */
						{
							TRACE (LOG_CRITICAL, "DRIVER", "\n  ALLOCATE fail    ");
							exit(0);

						}	
						/* SPR 17268 FIXED START */
						if(0 !=  remove((char *)ABSPath_EVENT_CODE_FILE))
						/* SPR 17268 FIXED END */
						{
							TRACE (LOG_CRITICAL, "DRIVER", "\n  ALLOCATE fail    ");
							exit(0);
						}
						ENB_FCLOSE(cmd_key_file);
					}
				}
				ipr_cwmp_addEvent(rec_event, stParam);
			}

		}
        /* SPR 21563 FIXED START */
        else
        {
            DEALLOCATE(rec_event); 
        } 
        /* SPR 21563 FIXED END */
		unlock_EQueue();
	}

	/*SPR 19863 FIXED START */
        /* CODE REMOVE */
        /*SPR 19863 FIXED END */
	if (Init_flag == E_TRUE)
		ipr_cwmp_start_server();	

    /* Coverity 40678 + */
    if(cmd_key)  {
	DEALLOCATE(cmd_key);   }
    /* Coverity 40678 - */
    /* Coverity 40677 + */
    if(event_code) {
    DEALLOCATE(event_code); } 
    /* Coverity 40677 - */

    /* SPR 18137 fix start */

    //code removed
     /* SPR 18137 fix end */
}



void initiate_TR069_Inform_session (char * eventCode, ParameterValueList *ParamValList) 
{

	/*  RPC to be informed about calling the specfic method */
	sEventStruct *newEvent = NULL; 

	newEvent = (sEventStruct *)ALLOCATE(sizeof(sEventStruct));
	if (NULL != newEvent) 
	{
		newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE);
		if (NULL != newEvent->EventCode)
		{
			strncpy(newEvent->EventCode ,eventCode ,  EVENT_CODE_SIZE-1);
			newEvent->EventCode [  EVENT_CODE_SIZE-1 ] = '\0';

			newEvent->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
			if (NULL != newEvent->CommandKey)
			{
				strncpy(newEvent->CommandKey, "", 2);
			}
			lock_EQueue();
			ipr_cwmp_addEvent(newEvent , ParamValList);
			unlock_EQueue();
		}
	}
}	


/*****************************************************************************
* FUNCTION:            	autonomousTransferComplete 
* DESCRIPTION:          This function is used ti inforim ACS abou the completion
			            of file transfer.
* ARGS:                 STATransferComplete *pstAutoTxfComp ( Pointer to 
			            Autonomous Trasnfer Complete Struct)
* RETURNS:             TRUE or FALSE.
******************************************************************************/
int autonomousTransferComplete (STATransferComplete *pstAutoTxfComp) 
{
	STATransferComplete *stAutoTxfComp = NULL;
	ClientRequest *newREquest = NULL;
	int rt = FALSE;

	stAutoTxfComp = (STATransferComplete *)ALLOCATE(sizeof(STATransferComplete)); 
	if(NULL == stAutoTxfComp)
	{
		return FALSE;
	}


	if (stAutoTxfComp != NULL && pstAutoTxfComp != NULL)
	{
		stAutoTxfComp->AnnounceURL = (char *)ALLOCATE(1024);
		if(NULL == stAutoTxfComp->AnnounceURL)
		{
			DEALLOCATE(stAutoTxfComp);
			return FALSE;
		}
		stAutoTxfComp->TransferURL = (char *) ALLOCATE(1024);
		if(NULL == stAutoTxfComp->TransferURL)
		{
			DEALLOCATE(stAutoTxfComp->AnnounceURL);
			DEALLOCATE(stAutoTxfComp);
			return FALSE;
		}
		stAutoTxfComp->FileType  = (char *) ALLOCATE(64);
		if(NULL == stAutoTxfComp->FileType)
		{
			DEALLOCATE(stAutoTxfComp->AnnounceURL);
			DEALLOCATE(stAutoTxfComp->TransferURL);
			DEALLOCATE(stAutoTxfComp);
			return FALSE;
		}
		stAutoTxfComp->TargetFileName = (char *) ALLOCATE(256);
		if(NULL == stAutoTxfComp->TargetFileName)
		{
			DEALLOCATE(stAutoTxfComp->AnnounceURL);
			DEALLOCATE(stAutoTxfComp->FileType);
			DEALLOCATE(stAutoTxfComp->TransferURL);
			DEALLOCATE(stAutoTxfComp);
			return FALSE;
		}
		stAutoTxfComp->FaultStruct = pstAutoTxfComp->FaultStruct;

		if (stAutoTxfComp->AnnounceURL &&  stAutoTxfComp->TransferURL 
				&& stAutoTxfComp->FileType  && stAutoTxfComp->TargetFileName) 
		{
			if (pstAutoTxfComp->AnnounceURL)
				strncpy(stAutoTxfComp->AnnounceURL , pstAutoTxfComp->AnnounceURL, 1024);
			if (pstAutoTxfComp->TransferURL)
				strncpy(stAutoTxfComp->TransferURL , pstAutoTxfComp->TransferURL, 1024);
			if (pstAutoTxfComp->FileType)
				strncpy(stAutoTxfComp->FileType , pstAutoTxfComp->FileType, 64);
			if (pstAutoTxfComp->TargetFileName)
				strncpy(stAutoTxfComp->TargetFileName , pstAutoTxfComp->TargetFileName, 256);
		}
		else
		{
			DEALLOCATE(stAutoTxfComp->TargetFileName);
			DEALLOCATE(stAutoTxfComp->AnnounceURL);
			DEALLOCATE(stAutoTxfComp->FileType);
			DEALLOCATE(stAutoTxfComp->TransferURL);
			DEALLOCATE(stAutoTxfComp);
			return FALSE;
		}					
		stAutoTxfComp->FaultStruct = (struct cwmp__TransferCompleteFaultStruct *)ALLOCATE( 
				sizeof (struct cwmp__TransferCompleteFaultStruct));

		if (stAutoTxfComp->FaultStruct  && pstAutoTxfComp->FaultStruct)
		{
			stAutoTxfComp->FaultStruct->FaultCode = (char *)ALLOCATE(FAULT_CODE_SIZE);
			stAutoTxfComp->FaultStruct->FaultString = (char *)ALLOCATE(FAULT_STRING_SIZE);

			if (stAutoTxfComp->FaultStruct->FaultCode && stAutoTxfComp->FaultStruct->FaultString)
			{
				if (pstAutoTxfComp->FaultStruct->FaultCode)
					strncpy(stAutoTxfComp->FaultStruct->FaultCode , 
							pstAutoTxfComp->FaultStruct->FaultCode , FAULT_CODE_SIZE);

				if (pstAutoTxfComp->FaultStruct->FaultString)
					strncpy(stAutoTxfComp->FaultStruct->FaultString , 
							pstAutoTxfComp->FaultStruct->FaultString, FAULT_STRING_SIZE);
			} 
			else
			{
				DEALLOCATE(stAutoTxfComp->FaultStruct->FaultString);
				DEALLOCATE(stAutoTxfComp->FaultStruct->FaultCode);
				DEALLOCATE(stAutoTxfComp->FaultStruct);
				DEALLOCATE(stAutoTxfComp->AnnounceURL);
				DEALLOCATE(stAutoTxfComp->TargetFileName);
				DEALLOCATE(stAutoTxfComp->FileType);
				DEALLOCATE(stAutoTxfComp->TransferURL);
				DEALLOCATE(stAutoTxfComp);
				return FALSE;
			}					


			newREquest = (ClientRequest *)ALLOCATE(sizeof(ClientRequest));
			if(NULL == newREquest)
			{
				DEALLOCATE(stAutoTxfComp->FaultStruct->FaultString);
				DEALLOCATE(stAutoTxfComp->FaultStruct->FaultCode);
				DEALLOCATE(stAutoTxfComp->FaultStruct);
				DEALLOCATE(stAutoTxfComp->TargetFileName);
				DEALLOCATE(stAutoTxfComp->FileType);
				DEALLOCATE(stAutoTxfComp->TransferURL);
				DEALLOCATE(stAutoTxfComp);
				return FALSE;
			}
			newREquest->eReqtype = AUTONOMOUS_COMPLETE_REQ; 
			newREquest->pstAutoTxComplete = stAutoTxfComp;

			/* Add new Client REquest  for Client Request manager */
			ipr_cwmp_addClientRequest(newREquest);
			rt = TRUE;
		}
	}
	return rt;
}


int getRPCMethod_ACS (void)
{
    ClientRequest *newREquest = NULL;
    int rt = FALSE;

	newREquest = (ClientRequest *)ALLOCATE( sizeof(ClientRequest));
    if(NULL == newREquest)
    {
	    return FALSE;
    }
    newREquest->eReqtype = REQUEST_DOWNLOAD_REQ;
    /* Add new Client REquest  for Client Request manager */
    ipr_cwmp_addClientRequest(newREquest);
    rt = TRUE;
    /*  coverity 56786 fix start */
	DEALLOCATE (newREquest);
    /*  coverity 56786 fix end */
    return rt;
}


//Phase 2
int getDownloadRequest_ACS (void)
{
    ClientRequest *newREquest = NULL;
    int rt = FALSE;
    sRequestDownload *stDownloadReq = NULL;

	stDownloadReq = (sRequestDownload *)ALLOCATE(sizeof(sRequestDownload));
    if(stDownloadReq != NULL)
    {
		stDownloadReq->FileType = (char *)ALLOCATE( 1024);
	    if(NULL == stDownloadReq->FileType)
	    {
			DEALLOCATE(stDownloadReq); 
		    return FALSE;
	    }
    }else
    {
	    return rt;
    }
        strncpy(stDownloadReq->FileType, "Vendor Configuration File", 255);
    
	newREquest = (ClientRequest *)ALLOCATE( sizeof(ClientRequest));
	if(NULL == newREquest)
	{
		DEALLOCATE(stDownloadReq->FileType); 
		DEALLOCATE(stDownloadReq); 
		return FALSE;
	}
	newREquest->eReqtype = REQUEST_DOWNLOAD_REQ;
	newREquest->pstReqDownload = stDownloadReq;
	/* Add new Client REquest  for Client Request manager */
	ipr_cwmp_addClientRequest(newREquest);
	rt = TRUE;
    /*  coverity 56785 fix start */
	DEALLOCATE (newREquest);
    /*  coverity 56785 fix end */
	return rt;
}

int  create_autoDUStateChangeComplete_req(void)
{
	int rt = FALSE;

	sAutoDUStateChangeComplete *dustatecomp     = NULL;
	struct cwmp__AutonOpResultStruct *cpeList = NULL;
	dustatecomp = (sAutoDUStateChangeComplete *)ALLOCATE(sizeof(sAutoDUStateChangeComplete));

	if(dustatecomp)
	{
		dustatecomp->__sizeResults = 1;
		cpeList = ( struct cwmp__AutonOpResultStruct *)ALLOCATE(sizeof(struct cwmp__AutonOpResultStruct));
	}
	if (cpeList != NULL)
	{
		cpeList->UUID = (Uint8 *) ALLOCATE( USR_NAME_SIZE);
		if(NULL == cpeList->UUID)
		{
			DEALLOCATE(cpeList);
			DEALLOCATE(dustatecomp);
			return FALSE;
		}
		cpeList->DeploymentUnitRef = (Uint8 *) ALLOCATE( USR_NAME_SIZE);
		if(NULL == cpeList->DeploymentUnitRef)
		{
			DEALLOCATE(cpeList->UUID);
			DEALLOCATE(cpeList);
			DEALLOCATE(dustatecomp);
			return FALSE;
		}
		cpeList->Version = (Uint8 *) ALLOCATE( USR_NAME_SIZE);
		if(NULL == cpeList->Version)
		{
			DEALLOCATE(cpeList->DeploymentUnitRef);
			DEALLOCATE(cpeList->UUID);
			DEALLOCATE(cpeList);
			DEALLOCATE(dustatecomp);
			return FALSE;
		}
		cpeList->OperationPerformed = (Uint8 *) ALLOCATE( USR_NAME_SIZE);
		if(NULL == cpeList->OperationPerformed)
		{
			DEALLOCATE(cpeList->Version);
			DEALLOCATE(cpeList->DeploymentUnitRef);
			DEALLOCATE(cpeList->UUID);
			DEALLOCATE(cpeList);
			DEALLOCATE(dustatecomp);
			return FALSE;
		}
		cpeList->ExecutionUnitRefList = (Uint8 *) ALLOCATE( USR_NAME_SIZE);
		if(NULL == cpeList->ExecutionUnitRefList)
		{
			DEALLOCATE(cpeList->OperationPerformed);
			DEALLOCATE(cpeList->Version);
			DEALLOCATE(cpeList->DeploymentUnitRef);
			DEALLOCATE(cpeList->UUID);
			DEALLOCATE(cpeList);
			DEALLOCATE(dustatecomp);
			return FALSE;
		}
	}else
	{
		DEALLOCATE(dustatecomp);
		return rt;
	}

	if(cpeList->UUID != NULL)
		strncpy(cpeList->UUID, "2134",USR_NAME_SIZE);
	if(cpeList->DeploymentUnitRef != NULL)
		strncpy(cpeList->DeploymentUnitRef, "2134",USR_NAME_SIZE);
	if(cpeList->Version != NULL)
		strncpy(cpeList->Version, "2134",USR_NAME_SIZE);
	if(cpeList->OperationPerformed  != NULL)
		strncpy( cpeList->OperationPerformed , "UPDATE",USR_NAME_SIZE);
	if(cpeList->ExecutionUnitRefList  != NULL)
		strncpy(cpeList->ExecutionUnitRefList , "111UPDATE",USR_NAME_SIZE);

	cpeList->CurrentState = cwmp__DeploymentUnitState__Uninstalled;
	cpeList->Resolved = xsd__boolean__true_;
	cpeList->StartTime =11;
	cpeList->CompleteTime = 18;

	dustatecomp->Results =  cpeList;
	autoDUStateChangeComplete(dustatecomp);   
	rt = TRUE;
	return rt;
}

/*****************************************************************************
 * * FUNCTION:    autoDUStateChangeComplete
 * *
 * * DESCRIPTION: This function is used to inform ACS about the completion of DU State Change.
 *
 * * ARGS:        sAutoDUStateChangeComplete *pstAutoTxfComp ( Pointer to Autonomous DUStateChangeComplete Struct)                            
 *
 * * RETURNS:      TRUE or FALSE.
 *******************************************************************************/
int autoDUStateChangeComplete(sAutoDUStateChangeComplete *pstAutoTxfComp)
{
    ClientRequest *newREquest = NULL;
    int rt = FALSE;

	newREquest = (ClientRequest *)ALLOCATE( sizeof(ClientRequest));
    if(NULL == newREquest)
    {
	    return FALSE;
    }
    newREquest->eReqtype = AUTONOMOUS_DU_STATE_CHANGE_COMPLETE_REQ;
    newREquest->pstAutoDUStateChangeComplete = pstAutoTxfComp;

    /* Add new Client REquest  for Client Request manager */
    ipr_cwmp_addClientRequest(newREquest);
    rt = TRUE;
    /*  coverity 56783 fix start */
	DEALLOCATE (newREquest);
    /*  coverity 56783 fix end */
    return rt;
}

/*SPR 16920 Fixed Start */
void initialize_tr69Parameters_parameter(Tr69Parameters_t *tr69Parameters)
{
	tr69Parameters->EnableCWMP = true;

}

/*SPR 16920 Fixed End */

/* SPR 16732 FIX START */
int snscanf(const char *str, int n, const char *format, ...) 
{           
    va_list             args;
    int                 result = SUCCESS;
    char               *str2 = NULL;
               
	str2 =(char *)ALLOCATE(n + 1);
   if(!str2)
   {            
       TRACE (LOG_CRITICAL,"DRIVER", "memory allocation failed");
	    return FAILED;
    }           
    strncpy (str2, str, n);
    str2[n] = '\0'; 
    va_start (args, format);
    result = vsscanf (str2, format, args);
    va_end (args);
            
	DEALLOCATE (str2);
    return result;
} 
/* SPR 16732 FIX END */

/* Periodic Inform Changes Start */
/*****************************************************************************
* FUNCTION:            	init_periodic_inform 
* DESCRIPTION:          This function initialize the TR069 periodic inform  
* ARGS:                	NONE
* RETURNS:              NONE 
******************************************************************************/
 void init_periodic_inform(void)
{
    /* SPR 18104 FIXED START*/
    TR069_USLEEP(1000);

    /* SPR 18104 FIXED END*/
	sEventStruct *newEvent = (sEventStruct *) ALLOCATE(sizeof (sEventStruct)) ;
    if(NULL == newEvent)
    {
        TRACE (LOG_CRITICAL, "DRIVER", "\n  alloc failed  ");
        return ;
    }
	newEvent->EventCode = (char *)ALLOCATE(EVENT_CODE_SIZE );

    if (NULL == newEvent->EventCode)
    {
		DEALLOCATE(newEvent);
        return ;
    }

	newEvent->CommandKey = (char *)ALLOCATE(EVENT_CODE_SIZE );

    if(NULL == newEvent->CommandKey)
    {
		DEALLOCATE(newEvent->EventCode);
		DEALLOCATE(newEvent);
        return ;
    }
	strncpy(newEvent->EventCode , "2 PERIODIC", EVENT_CODE_SIZE);
    newEvent->EventCode[EVENT_CODE_SIZE-1] = '\0';
	strcpy(newEvent->CommandKey, "") ;
    lock_EQueue();
    ipr_cwmp_addEvent(newEvent, PNULL);
    unlock_EQueue();
}
/* Periodic Inform Changes End */
