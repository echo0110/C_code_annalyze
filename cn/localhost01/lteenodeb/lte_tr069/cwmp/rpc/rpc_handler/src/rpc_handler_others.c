/********************************************************************************
*
*       FILE NAME   : 	rpc_handler_others.c
*
*       DESCRIPTION : 	This file contains the function for all rpc functionalities
*				like SetParameterAttribute.						
*
*
*       REVISION HISTORY:
*
*       NAME           DATE                 REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Dec 17, 2012         TR-069 Amendment 4 phase-2   Initial Creation
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/


#include <rpc_handler.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <logger.h>
#include <curl/curl.h>
#include <lteTypes.h>
#include <dirent.h>
#include <cspl.h>
#include <map_glue.h>
#include <pthread.h>
#include <signal.h>
#include <client_req_manager.h>
#include <ipr_cwmp_enb_wrapper.h>



#define INVALID 1
#define VALID 0
/* SPR 20142 FIXED START */
#define CURLE_LOGIN_FAILURE 530
/* SPR 20142 FIXED END */
/******************************************************************************
  Global Variables
******************************************************************************/
pthread_t g_rpc_sch_dwn_th;
//ParameterValueList g_ParamValList = {0};
ParameterValueList *g_ParamValList = NULL;
U8 g_rpc_IstxQ_init = 0;
U8 g_rpc_IsSchDwnQ_init = 0;

pthread_t g_th_upload;

YLIST g_Txn_List;
YLIST g_Txn_List_Sch_Dwn;
/*download upload change starts*/
extern UInt32 queue_upload_max_size;
/*download upload change ends*/


/******************************************************************************
  Function Declarations
******************************************************************************/

CHAR8 check_user_component (CHAR8 * url_string);
void create_transfer_complete (CHAR8 * CommandKey , CHAR8 *FaultCode ,
                               CHAR8 * FaultString , time_t t1 , time_t t2);
ParameterInfoList * ipr_cwmp_getallListInfo(void);
void * ipr_cwmp_actualScheduleDownload (void * arg);
void * ipr_cwmp_actual_ScheduleInform (void *arg);
/*GPA Change Start*/
GetParamAttrRes * ipr_cwmp_GetParameterAttribute(ParameterNames *);
/*GPA Change End*/
U8 ipr_cwmp_performUpload(ThreadUpload *thArg_t);
extern int ipr_cwmp_connManager_ScheduleDownload (ScheduleDownload *schDwn_t);
extern int connManagerUpload (Upload *arg_upload_t, char * arg_file_path);
extern int ipr_cwmp_addEvent(sEventStruct *event , ParameterValueList * spstParam);


/* SPR 17268 FIXED START */
#define CONFIG_PARAM_LEN 256
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */

/******************************************************************************
  Function Definitions
******************************************************************************/

/********************************************************************************************
*       Function Name   : ipr_cwmp_setParameterAttribute 
*       Description     : rpc call for Set Parameter Attribute .
*       Args            : SetParameterAttributesList * : (Pointer to Parameter list for
                                                          which attributes should be applied)
*       Returns         : SetParamAttrRes * : (Pointer to SPA response given to ACS)  
*********************************************************************************************/
SetParamAttrRes * ipr_cwmp_setParameterAttribute (SetParameterAttributesList *arg_paramList_t)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);

    SetParameterAttributesStruct ** l_Set_ParamAttr_t = NULL;
    SetParameterAttributesList *glue_Paramlist_t = NULL;
    SetParameterAttributesStruct ** glue_ParamAttr_t = NULL;
    SetParamAttrRes *glue_res = NULL;
    UINT16 l_size = 0;
    UINT16 l_count = 0;
    CHAR8 *l_Name = NULL;
    UINT16 l_glue_size = 1;  

    if (NULL != arg_paramList_t && NULL != arg_paramList_t->__ptrSetParameterAttributesStruct)
    {
        l_Set_ParamAttr_t = arg_paramList_t->__ptrSetParameterAttributesStruct;
        l_size = arg_paramList_t->__size;
        TRACE (LOG_CRITICAL , RPC_HANDLER, "Size of Input Parameter list :%d",l_size);
    }

    if  (l_Set_ParamAttr_t)
    {
	    while (l_count < l_size)
	    {
		    l_Name = l_Set_ParamAttr_t[l_count]->Name;    
		    if (l_Name != NULL)
		    {
			    TRACE (LOG_CRITICAL , RPC_HANDLER, "Parameter [ %d ]  ==>  :%s", l_count , l_Name);
			    if (strncmp(l_Name , CPE_PARAM_PREFIX, CPE_PARAM_PREFIX_LEN) !=  0)
			    {
				    glue_ParamAttr_t = (SetParameterAttributesStruct **)TR_REALLOC(glue_ParamAttr_t ,
						    sizeof(SetParameterAttributesStruct *)*l_glue_size);
				    if(NULL ==  glue_ParamAttr_t)
				    {
					    TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
					    return NULL;
				    }

				    glue_ParamAttr_t[l_glue_size-1] = l_Set_ParamAttr_t[l_count];
				    l_glue_size++;
			    }
		    }
		    l_count++;
	    }
	    if (l_glue_size > 1)
	    {
		    TRACE (LOG_CRITICAL , RPC_HANDLER, "No. Of parameters for Glue layer : %d", l_glue_size - 1);
		    glue_Paramlist_t = (SetParameterAttributesList *)ALLOCATE( sizeof(SetParameterAttributesList));
		    if (glue_Paramlist_t)
		    {
			    glue_Paramlist_t->__ptrSetParameterAttributesStruct = glue_ParamAttr_t;
			    glue_Paramlist_t->__size = l_glue_size - 1;
			    TRACE (LOG_CRITICAL ,  RPC_HANDLER, "Invoking glue layer callback");
				/* SPA Change Start */
			    //glue_res = stCallbackFunList.invokeCallback((void *)glue_ParamAttr_t , RPC_SETPARAMATTRIBUTE , NULL);
                glue_res = (SetParamAttrRes *)stCallbackFunList.invokeCallback((void *)glue_Paramlist_t ,
					    RPC_SETPARAMATTRIBUTE , NULL); 
				/* SPA Change End */
		    }
	    }
    } 
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
    //coverity fix
    DEALLOCATE(glue_Paramlist_t);
    DEALLOCATE(glue_ParamAttr_t);    
    return glue_res;
}


/********************************************************************************************
*       Function Name   : ipr_cwmp_GetParameterAttribute
*       Description     : rpc call for Get Parameter Attribute .
*       Args            : ParameterNames * : (Pointer to Parameter name list for
                                                          which attributes should be retrieved)
*       Returns         : GetParamAttrRes * : (Pointer to GPA response given to ACS)
*********************************************************************************************/
/* GPA Change Start */
GetParamAttrRes * ipr_cwmp_GetParameterAttribute (ParameterNames  *arg_pNames_t)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s ", __FUNCTION__);
    CHAR8 **arrGlueNames = NULL;
    CHAR8 **arrArgNames = NULL;
    CHAR8 *l_cTmpName = NULL ;
    GetParamAttrRes *glue_res = NULL;
    ParameterNames *newParamName = NULL;
    CHAR8 *l_cCPEName = NULL;
    SInt32 iSize  = 0;
    SInt32 iCount = 0;
    SInt32 iGlueNames_Size = 0;
    TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering in %s ",__FUNCTION__);
    if (NULL == arg_pNames_t || (NULL == arg_pNames_t->__ptrstring))
    {
        TRACE(LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL Parameter Names List....");
        return NULL;
    }

    iSize = arg_pNames_t->__size;

    newParamName = (ParameterNames * ) ALLOCATE(sizeof (ParameterNames) );
    if (newParamName != NULL)
    {
	    memset(newParamName , '\0', sizeof (ParameterNames));
        newParamName->__ptrstring = (Uint8 **) ALLOCATE((iSize * sizeof(Uint8 *)) );
        if (!(newParamName->__ptrstring))
        {
            TRACE(LOG_CRITICAL , RPC_HANDLER,"Fail to allocate memory newParamName->__ptrstring");
            DEALLOCATE(newParamName);
            return NULL;  
            //ret = 1;
        }
	    memset(newParamName->__ptrstring, '\0', sizeof (Uint8 *));
    }else
    {
        TRACE(LOG_CRITICAL , RPC_HANDLER,"Fail to allocate memory newParamName");
	    return NULL;
    }
    arrArgNames = (Uint8 **)arg_pNames_t->__ptrstring;

    
   arrGlueNames = (Uint8 **)newParamName->__ptrstring;

    TRACE(LOG_CRITICAL , RPC_HANDLER, " Size of Parameter List ; %d", iSize);

    while (iCount < iSize)
    {
        l_cTmpName = arrArgNames[iCount];
        if (l_cTmpName != NULL)
        {
           // if ((strncmp(l_cTmpName , CPE_PARAM_PREFIX, CPE_PARAM_PREFIX_LEN)) != 0)
            //{
                TRACE(LOG_CRITICAL , RPC_HANDLER, "ManagementServer Req");
                // Put into
                l_cCPEName = (CHAR8 *)ALLOCATE( MAX_PARAM_NAME_SIZE);
                if(NULL == l_cCPEName)
                {
                    perror("alloc fail");
                    exit(1);
                }
	    	memset(l_cCPEName, '\0', MAX_PARAM_NAME_SIZE);

                if (NULL == l_cCPEName)
                    continue;
                ENB_STRNCPY(l_cCPEName, l_cTmpName, MAX_PARAM_NAME_SIZE);
                arrGlueNames[iGlueNames_Size] = l_cCPEName;
                iGlueNames_Size++;
            //}
            //else
            //{
            TRACE(LOG_CRITICAL , RPC_HANDLER, "iGlueNames_Size :%d",iGlueNames_Size);
            //}
        }
        iCount++;
    } // While ends

    if (iGlueNames_Size > 0)
    {
        newParamName->__size = iGlueNames_Size;
        //glue_res = stCallbackFunList.invokeCallback((void *)newParamName,RPC_GETPARAMETERATTRIBUTE , NULL);
        glue_res = (GetParamAttrRes *)stCallbackFunList.invokeCallback((void *)newParamName,RPC_GETPARAMETERATTRIBUTE , NULL);
        if(glue_res != NULL)
        {
            TRACE(LOG_CRITICAL , RPC_HANDLER, "Received Response from glue layer");
        }
        else
        {
            TRACE(LOG_CRITICAL , RPC_HANDLER, "RESPONSE is empty");
        }
    }

    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s ", __FUNCTION__);
    /* SPR 21563 FIXED START */
    Free_GetParameterAttribute(newParamName);
    /* SPR 21563 FIXED END */

    DEALLOCATE(newParamName);
    return glue_res;
}
/* GPA Change End */

/********************************************************************************************
* Function Name   : ipr_cwmp_actual_ScheduleDownload
* Description     : Thread function for ScheduleDownload rpc.
* Args            : void  * : (ScheduleDownload *)
* Returns         : void *
*********************************************************************************************/
void * ipr_cwmp_actualScheduleDownload (void * arg)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s", __FUNCTION__);
    //ScheduleDownload * l_schDwn_t = (ScheduleDownload *) arg;
    
    time_t actual_time;
    UINT16 res = 0; 
    
    DownloadStatus * glue_res = NULL;

    U8 timeWin_used = 0;

    time_t start_time;
    Uint8* FaultCode = NULL;
    Uint8* FaultString =  NULL;
    UInt32 list_size = 0;
    UInt8 indx = 0;

    if (NULL == arg)
        return NULL;

    time(&actual_time);


   list_size = ylCount(&g_Txn_List_Sch_Dwn);


    while (list_size > 0)
    {

        indx = 0;

        while (indx < list_size)
        {
            ThreadSchDwn *l_th_t = NULL;
            l_th_t = (ThreadSchDwn *)ylNth(&g_Txn_List_Sch_Dwn , indx);

            if (l_th_t != NULL)
            {
                ScheduleDownload * l_schDwn_t = l_th_t->ptr_dwn_t;
                if (((actual_time - l_th_t->rec_time) >=  l_th_t->timeWin_1_start) 
                    || (l_th_t->timeWin_2_start > 0 
                        && ((actual_time - l_th_t->rec_time) >= l_th_t->timeWin_2_start)))
                {
                   //   Pick this node
                    TRACE (LOG_CRITICAL , RPC_HANDLER, "New Schedule Download Request Picked");
                    l_th_t->Status =  RPC_FALSE;
                    if ((actual_time - l_th_t->rec_time) >=  l_th_t->timeWin_1_start)
                    timeWin_used = 1;
                    else
                    timeWin_used = 2;
            
                    start_time = actual_time;

                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Received status :%d",l_th_t->cancelStatus);
                    if (l_th_t->cancelStatus == CANCEL_FALSE)
                    {
                        res = ipr_cwmp_connManager_ScheduleDownload(l_schDwn_t);
			FaultCode = (Uint8 *)ALLOCATE( FAULT_CODE_SIZE);
			if(NULL == FaultCode )
				return NULL;
			 FaultString = (Uint8 *)ALLOCATE( FAULT_STRING_SIZE);
			if(NULL ==  FaultString)
			{
				DEALLOCATE(FaultCode);
				return NULL;
			}
			/* SPR-16455 START */
			memset(FaultCode ,'\0', FAULT_CODE_SIZE);
			memset(FaultString ,'\0', FAULT_STRING_SIZE);
			/* SPR-16455 END */

                            ENB_STRNCPY(FaultCode, "0",2);
                            ENB_STRNCPY(FaultString , " ", 2);
                        if (res != RESULT_OK)
                        {
                            TRACE (LOG_CRITICAL , RPC_HANDLER, "File Transfer Failed with return status :%d",res);
			    /*SPR 16138  Fixed Start*/
			    if(NULL != strstr(l_schDwn_t->URL,"@")){

				    strncpy(FaultCode, "9003", FAULT_CODE_SIZE);
				    strncpy(FaultString ,
						    "File Transfer failed: Userifo persent for file transfer",
						    FAULT_STRING_SIZE);

			    }

			    else{
			    /*SPR 16138  Fixed End*/

                            switch (res)
                            {
                                case CURLE_LOGIN_DENIED :
                                    {
                                        ENB_STRNCPY(FaultCode, "9012", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString ,
                                                "File Transfer failed: Server authentication failure",
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                case CURLE_UNSUPPORTED_PROTOCOL :
				    {
					    ENB_STRNCPY(FaultCode, "9013", FAULT_CODE_SIZE);
					    ENB_STRNCPY(FaultString ,
							    "File Transfer failed: Unsupported protocol for file transfer",
							    FAULT_STRING_SIZE);
				    }

                                    break;
                                case CURLE_COULDNT_CONNECT :
                                    {
                                        ENB_STRNCPY(FaultCode, "9015", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString ,
                                                "File Transfer failed: unable to contact file server",
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                case CURLE_REMOTE_FILE_NOT_FOUND :
                                    {   
                                        ENB_STRNCPY(FaultCode, "9016", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString , "File Transfer failed: Unable to access file", FAULT_STRING_SIZE);
                                    }
                                    break;
                                case 405 :
                                    {
                                        ENB_STRNCPY(FaultCode, "9017", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString , "File Transfer failed: Unable to complete download", FAULT_STRING_SIZE);
                                    }
                                    break;
                                case 402 :
                                    {
                                        ENB_STRNCPY(FaultCode, "9018", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString , "File Transfer failed: File corrupted", FAULT_STRING_SIZE);
                                    }
                                    break;
                                case 403 :
                                    {
                                        ENB_STRNCPY(FaultCode, "9019", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString , "File Transfer failed: File authentication failure", FAULT_STRING_SIZE);
                                    }
                                    break;
                                case 404 :
                                    {
                                        ENB_STRNCPY(FaultCode, "9016", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString ,
                                                "File Transfer failed: File Transfer failed: Unable to access file",
                                                FAULT_STRING_SIZE);
                                    }   
                                    break;
                                default :
                                    {
                                        ENB_STRNCPY(FaultCode, "9010", FAULT_CODE_SIZE);
                                        ENB_STRNCPY(FaultString , "File Transfer failed", FAULT_STRING_SIZE);
                                    }
                                    break;
                            }
			    /*SPR 16138  Fixed Start*/
			    }
			    /*SPR 16138  Fixed End*/

			}
                        else
                        {
                            glue_res = stCallbackFunList.invokeCallback((void *)l_schDwn_t , RPC_SCHEDULE_DOWNLOAD , NULL);
                            (void ) time(&actual_time);

                            if ((timeWin_used == 1 && (l_th_t->rec_time + l_th_t->timeWin_1_end) <= actual_time)
                                    || (timeWin_used == 2  && (l_th_t->rec_time + l_th_t->timeWin_2_end) <= actual_time))
                            {
                                //Success
                                if (glue_res &&  FAULT == glue_res->eStatus)
                                {
                                    //If we have to retrieve the Error codes
                                }    
                                else
                                {
                                    ENB_STRNCPY(FaultCode, "0",2);
                                    ENB_STRNCPY(FaultString , " ", 2);
                                }
                            }
                        }
                        if (NULL != l_schDwn_t->CommandKey)
                        {
                            time(&actual_time);
                            if (res != RESULT_OK)
                                create_transfer_complete (l_schDwn_t->CommandKey , FaultCode , FaultString , -1, -1);
                            else
                                create_transfer_complete (l_schDwn_t->CommandKey , FaultCode , FaultString ,
                                        (start_time), actual_time);
			}
			//coverity fix 
			DEALLOCATE(FaultString);
			DEALLOCATE(FaultCode);   
                    }
                    if(-1 == remove(l_th_t->dw_logfilename))
   		 TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR in remove Entering %s  ",__FUNCTION__);
                    ylDelete(&g_Txn_List_Sch_Dwn , (YLNODE *)l_th_t);
                }
            }
            indx++;
	    time(&actual_time);

 }
        list_size =  ylCount(&g_Txn_List_Sch_Dwn);
        usleep(10000);
    }

    g_rpc_sch_dwn_th = 0;

return NULL;
}   


/********************************************************************************************
 *       Function Name   : ipr_cwmp_Schedule_Download
 *       Description     : rpc call for Schedule_Download.
 *       Args            : ParameterNames * : (Pointer to Parameter name list for
 which attributes should be retrieved)
 *       Returns         : GetParamAttrRes * : (Pointer to GPA response given to ACS)
 *********************************************************************************************/
SInt16 ipr_cwmp_startScheduleDownload (ScheduleDownload * arg_schDwn_t)
{
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
	SInt32 rc = 0;
	UInt8 *glue_res = NULL; 
	UINT32 timeWin_1_start = 0;
	UINT32 timeWin_1_end = 0;
	UINT32 timeWin_2_end = 0;
	UINT32 timeWin_2_start = 0;
	SInt32 l_size = 0;
	U8 l_count = 0;
	FILE *dw_fp = NULL;

	Uint8 *dw_logfilename = NULL;

	time_t t_other ;
	SInt32 r = 0;

	//coverity 5sep 
	int randomData = open("/dev/random", O_RDONLY);
	if(randomData >=0)
	{
		if( -1 == read(randomData, &r, sizeof(r)))
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "In  %s  error in reading random data ",__FUNCTION__);
			ENB_CLOSE(randomData);
			return FAILED;
		}
		close(randomData);
	}
	time (&t_other);

	dw_logfilename = (Uint8 *) ALLOCATE(MAX_DW_LOG_NAME_LEN );
	if(NULL ==  dw_logfilename)
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
		return FAILED;
	}


	if (!g_rpc_IsSchDwnQ_init)
	{
		init_list(&g_Txn_List_Sch_Dwn);
		g_rpc_IsSchDwnQ_init = 1;
	}


	if (NULL == arg_schDwn_t || NULL == arg_schDwn_t->URL)
	{
		//coverity fix
		DEALLOCATE(dw_logfilename);
		return 9002;
	}
	if (check_user_component(arg_schDwn_t->URL) == INVALID)
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "Error:User component in URL");
		//coverity fix
		DEALLOCATE(dw_logfilename);
		return 9003; 
	}

	if (arg_schDwn_t->TimeWindowList)
	{
		l_size = arg_schDwn_t->TimeWindowList->__size;

		while (l_count < l_size)
		{
			if (l_count == 0)
			{
				timeWin_1_start = arg_schDwn_t->TimeWindowList->__ptrTimeWindowStruct[l_count]->WindowStart;
				timeWin_1_end = arg_schDwn_t->TimeWindowList->__ptrTimeWindowStruct[l_count]->WindowEnd;
			}
			else if (l_count == 1)
			{
				timeWin_2_start = arg_schDwn_t->TimeWindowList->__ptrTimeWindowStruct[l_count]->WindowStart;
				timeWin_2_end = arg_schDwn_t->TimeWindowList->__ptrTimeWindowStruct[l_count]->WindowEnd;
			}
			l_count++;
		}
		if ( (timeWin_2_start > 0  && timeWin_2_start < timeWin_1_end)
				|| (timeWin_1_end < timeWin_1_start)  || (timeWin_2_start > 0 && timeWin_2_end < timeWin_1_end)
				|| (timeWin_1_end == 0)
		   )
		{
			//  Return with error code 9003
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Invalid Time windows");
			//coverity fix
			DEALLOCATE(dw_logfilename);
			return 9003;
		}
	}

	if ((dw_logfilename != NULL))
	{
		snprintf(dw_logfilename ,  MAX_DW_LOG_NAME_LEN, "%s_%ld_%d", SCH_DW_FILE_PATH_NAME , t_other,r) ;

        /* SPR 17268 FIXED START */
        UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath1,(char *)ABSPath);
        TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath before concat %s ",ABSPath1 );

        /*  coverity 96691 fix start */
        if((strlen(dw_logfilename) + strlen((char *)ABSPath1)) < CONFIG_PARAM_LEN)
        {
            strncat((char *)ABSPath1 , dw_logfilename, strlen(dw_logfilename));
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath after concat %s ",ABSPath1 );
        }
        /*  coverity 96691 fix end */
        
        dw_fp = fopen((char *)ABSPath1 ,"w+");
        /* SPR 17268 FIXED END */

		//dw_fp = fopen(dw_logfilename ,"w+");
		if (dw_fp != NULL)
		{
			ENB_FPRINTF(dw_fp , "%s\n%s\n%s\n%s\n%s\n%u\n%s\n%d\n%u\n%u\n%u\n%u\n%ld",
					(strcmp(arg_schDwn_t->CommandKey,"") == 0) ? "-" : arg_schDwn_t->CommandKey,
					(strcmp (arg_schDwn_t->FileType,"")==0) ? "-" : arg_schDwn_t->FileType,
					strcmp(arg_schDwn_t->URL,"")==0 ? "-" : arg_schDwn_t->URL,
					strcmp(arg_schDwn_t->Username,"")==0 ? "-" : arg_schDwn_t->Username,
					strcmp(arg_schDwn_t->Password, "")== 0  ? "-" : arg_schDwn_t->Password,
					arg_schDwn_t->FileSize,
					strcmp(arg_schDwn_t->TargetFileName, "")==0  ? "-" : arg_schDwn_t->TargetFileName,l_size
					,timeWin_1_start,timeWin_1_end, timeWin_2_start , timeWin_2_end , 
					t_other);
			fclose(dw_fp);
		}
	}



	//Thread to start  schedule download 
	glue_res = stCallbackFunList.invokeCallback((void *)arg_schDwn_t , RPC_PRESCHEDULE_DOWNLOAD , NULL);

	if (glue_res != NULL && *glue_res != 0)
	{
		ThreadSchDwn *th_SchDwn = ALLOCATE(sizeof(ThreadSchDwn)  );

		if (!th_SchDwn)
		{
			DEALLOCATE(dw_logfilename);
			return 9002; 
		}
		th_SchDwn->timeWin_1_start =  timeWin_1_start;
		th_SchDwn->timeWin_1_end = timeWin_1_end;
		th_SchDwn->timeWin_2_start = timeWin_2_start;
		th_SchDwn->timeWin_2_end = timeWin_2_end; 
		th_SchDwn->ptr_dwn_t = arg_schDwn_t;
		th_SchDwn->dw_logfilename = dw_logfilename;
		time(&th_SchDwn->rec_time);


		th_SchDwn->Status = RPC_TRUE;
		th_SchDwn->cancelStatus = CANCEL_FALSE;


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
			ADD_IN_LIST(&g_Txn_List_Sch_Dwn, (YLNODE *)th_SchDwn);

			// set stack size attibute
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			rc = pthread_create(&g_rpc_sch_dwn_th, &attr, ipr_cwmp_actualScheduleDownload  , (void *)th_SchDwn);
            /*  coverity 86524 fix start */
            if(0 != rc)
                TRACE(LOG_INFO , RPC_HANDLER, "Thread creation failed with %d error code \n", rc);
            /*  coverity 86524 fix end */
			pthread_attr_destroy(&attr);
			pthread_detach(g_rpc_sch_dwn_th);
		}

		//coverity fix

	}

	TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);

	return TRUE;
}


/********************************************************************************************
 *       Function Name   : create_transfer_complete 
 *       Description     : Call to create a new client request for Transfer Complete.
 *       Args            : CommandKey , FaultCode , FaultString , start time and end time. 
 *       Returns         : void 
 *********************************************************************************************/
void create_transfer_complete (CHAR8 * CommandKey , CHAR8 *FaultCode , 
        CHAR8 * FaultString , time_t t1 , time_t t2)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    ClientRequest *sClientReq = NULL;
    if (CommandKey != NULL)
    {
        sClientReq = (ClientRequest *)ALLOCATE( sizeof(ClientRequest) );
        if (sClientReq)
        {
            sClientReq->eReqtype = TRANS_COMPLETE_REQ;
            sClientReq->pstTxComplete = ALLOCATE(sizeof (struct _cwmp__TransferComplete) );

            if (sClientReq->pstTxComplete  != NULL)
            {
                sClientReq->pstTxComplete->CommandKey =  ALLOCATE(COMMAND_KEY_SIZE );
                sClientReq->EventCode = ALLOCATE(EVENT_CODE_SIZE );
                if (sClientReq->pstTxComplete->CommandKey != NULL)
                {
                    ENB_STRNCPY(sClientReq->pstTxComplete->CommandKey ,
                            CommandKey, COMMAND_KEY_SIZE-1);
                }
                if (sClientReq->EventCode != NULL)
                {
                    ENB_STRNCPY(sClientReq->EventCode, SCH_DOWNLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);
                }
                sClientReq->pstTxComplete->StartTime = t1;
                sClientReq->pstTxComplete->CompleteTime = t2;

                sClientReq->pstTxComplete->FaultStruct = ALLOCATE(
                        sizeof(struct cwmp__TransferCompleteFaultStruct));
                sClientReq->pstTxComplete->FaultStruct->FaultCode = FaultCode;
                sClientReq->pstTxComplete->FaultStruct->FaultString = FaultString;
                ipr_cwmp_addClientRequest(sClientReq);
            }
        }
    }
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
}	



/********************************************************************************************
 *       Function Name   : check_user_component 
 *       Description     : to check if user component exist in URL.
 *       Args            : U8 * : Pointer to URL string  
 *       Returns         : CHAR8 : INVALID 1  or VALID 0 
 *********************************************************************************************/
CHAR8 check_user_component (CHAR8 * url_string)
{

    CHAR8 *start = NULL;
    U8 offset = 0;
    U8 ret_val = VALID; 
    CHAR8 *end = NULL;

    if (NULL == url_string || strlen(url_string) == 0)  
    {
        return INVALID;
    }

    //check for User component 
    if ( *(url_string) == 'h' || *(url_string) == 'H' || *(url_string) == 'f')
    {
        if (*(url_string) == 'f')
            start = url_string + 6;
        else
            start = url_string + 7;      

        end = strchr(start , '/');

        if (end != NULL)
        {
            while ((start + offset) < end)
            {
                if (*(start + offset) == '@')
                {
                    ret_val = INVALID;
                    break;
                }
                offset++;
            }

        }
    }
    return ret_val;
}


/********************************************************************************************
 *       Function Name   : ipr_cwmp_actual_ScheduleInform 
 *       Description     : Thread function for ScheduleInform rpc.
 *       Args            : void  * : (ScheduleInform *)
 *       Returns         : void * 
 *********************************************************************************************/
void * ipr_cwmp_actual_ScheduleInform (void *arg)
{
	TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering in %s ",__FUNCTION__);
	sEventStruct *newEvent = NULL;
	sEventStruct *newEvent1 = NULL;

	ScheduleInform *l_Sh_Inform_t = (ScheduleInform *)arg;
	UINT32 wait_time = 0;

	wait_time  = l_Sh_Inform_t->DelaySeconds;

	if (wait_time > 0)
	{
		TRACE(LOG_CRITICAL , RPC_HANDLER, "Waiting for %d seconds", wait_time); 
		sleep(wait_time);
		newEvent1 = (sEventStruct *)ALLOCATE(sizeof(sEventStruct)  );
		if (NULL != newEvent1)
		{
			newEvent1->EventCode = ALLOCATE(EVENT_CODE_SIZE  );

			if (NULL != newEvent1->EventCode)
			{
				ENB_STRNCPY(newEvent1->EventCode ,"3 SCHEDULED",  EVENT_CODE_SIZE-1);
				newEvent1->EventCode[EVENT_CODE_SIZE-1] = '\0';

				newEvent1->CommandKey = ALLOCATE(COMMAND_KEY_SIZE  );
				if (NULL != newEvent1->CommandKey && l_Sh_Inform_t->CommandKey)
				{
					ENB_STRNCPY(newEvent1->CommandKey , l_Sh_Inform_t->CommandKey , COMMAND_KEY_SIZE);
				}
			}

		}
		newEvent = (sEventStruct *)ALLOCATE(sizeof(sEventStruct) );
		if (NULL != newEvent)
		{
			newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE  );

			if (NULL != newEvent->EventCode)
			{
				ENB_STRNCPY(newEvent->EventCode ,SCH_INFRM_EVNT_CODE,  EVENT_CODE_SIZE-1);
				newEvent->EventCode[EVENT_CODE_SIZE-1] = '\0';

				newEvent->CommandKey = ALLOCATE(COMMAND_KEY_SIZE );
				if (NULL != newEvent->CommandKey && l_Sh_Inform_t->CommandKey)
				{
					ENB_STRNCPY(newEvent->CommandKey , l_Sh_Inform_t->CommandKey , COMMAND_KEY_SIZE);
				}
				lock_EQueue();
				if (newEvent1)
					ipr_cwmp_addEvent(newEvent1, g_ParamValList);
				ipr_cwmp_addEvent(newEvent , g_ParamValList);
				unlock_EQueue();
			}
		}
	} 
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
	return NULL;
}


/********************************************************************************************
 *       Function Name   : ipr_cwmp_ScheduleInform
 *       Description     : RPC function for ScheduleInform which starts new thread for inform.
 *       Args            : (ScheduleInform *) : pointer to stucture ScheduleInform.
 *********************************************************************************************/
SInt32 ipr_cwmp_ScheduleInform (ScheduleInform *arg_Sh_Inform_t)
{
    TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering in %s ",__FUNCTION__);
    if (NULL == arg_Sh_Inform_t)
    {
        TRACE(LOG_CRITICAL , RPC_HANDLER, "Error :Not Enough Memory available");
        return 9002;
    }

    if (arg_Sh_Inform_t->DelaySeconds == 0)
        return 9003;

    pthread_t l_th_sch_Inform;

    // set stack size attibute
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    /*Coverity 109687 Fix Start*/
    SInt32 rc = 0;
    rc =pthread_create(&l_th_sch_Inform , &attr, ipr_cwmp_actual_ScheduleInform , (void *)arg_Sh_Inform_t); 
	
	 if(0 != rc)
                TRACE(LOG_INFO , RPC_HANDLER, "Thread creation failed with %d error \n", rc);
    /*Coverity 109687 Fix End*/



    pthread_attr_destroy(&attr);
    pthread_detach(l_th_sch_Inform);


    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
    return TRUE;
}


/********************************************************************************************
 *       Function Name   : ipr_cwmp_actual_Upload
 *       Description     : Thread function for Upload rpc.
 *       Args            : void  * : (ThreadUpload *)
 *       Returns         : void *
 *********************************************************************************************/
void * ipr_cwmp_actualUpload (void *thArg_t)
{
    TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    Upload *l_upload_t = NULL;
   /* 
    UINT32 wait_time = 0;
    U8 cont = 1; 
    Uint8* FaultCode = NULL;
    Uint8* FaultString =  NULL;
    UINT16 res = 0;
    UINT16 *glue_res = NULL;
    ClientRequest *sClientReq = NULL;
    U8 cont = 0;
   */
    //U8 ret_val = 0;
    if(NULL == thArg_t)
    {
	   TRACE (LOG_CRITICAL ,"RPC_HANDLER", "thArg_t NULL\n");
    }    

    UInt32 list_size = 0;
    UInt8 indx = 0;
    //Int retVal = 0;
    time_t currTime;

    time(&currTime);

    list_size = ylCount(&g_Txn_List); 

    while (list_size > 0)
    {

        indx = 0;

        while (indx < list_size)
        {
            ThreadUpload *l_th_up_t = NULL;
            l_th_up_t = (ThreadUpload *)ylNth(&g_Txn_List , indx);

            if (l_th_up_t != NULL)
            {
                if ((currTime - l_th_up_t->recv_time) >= l_th_up_t->delay)
                {
                    //   Pick this node  
                    TRACE (LOG_CRITICAL , RPC_HANDLER, "New Upload Request Picked");
                    l_th_up_t->Status =  RPC_FALSE;


                    if (l_th_up_t->cancelStatus == CANCEL_FALSE)
                         ipr_cwmp_performUpload(l_th_up_t);    

                    ylDelete(&g_Txn_List , (YLNODE *)l_th_up_t);
                    if (l_th_up_t->upl_logfilename)
                    {
//coverity fix
			    if(-1 == remove(l_th_up_t->upl_logfilename))
			    TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR in remove Entering %s  ",__FUNCTION__);
                        /* Upload RPC changes start */
                        DEALLOCATE(l_th_up_t->upl_logfilename);
                        DEALLOCATE(l_th_up_t->local_path);
                        /* Upload RPC changes end */
                    }
                    if (l_th_up_t->pstupload)
                    {
                        l_upload_t = l_th_up_t->pstupload;
                        /* Upload RPC changes start */
                        DEALLOCATE(l_upload_t->CommandKey);
                        DEALLOCATE(l_upload_t->FileType);
                        DEALLOCATE(l_upload_t->URL);
                        DEALLOCATE(l_upload_t->Username);
                        DEALLOCATE(l_upload_t->Password);
                        DEALLOCATE(l_upload_t);
                        /* Upload RPC changes end */
                    }
                    /* Upload RPC changes start */
                    DEALLOCATE(l_th_up_t);
                    /* Upload RPC changes end */
                }
            }
            indx++;

        } // While for list traversing
        time (&currTime);
        list_size =  ylCount(&g_Txn_List);
        TR069_USLEEP(10000);
    }
    g_th_upload = 0;
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
    return NULL;
}


/********************************************************************************************
 *       Function Name   : ipr_cwmp_performUpload
 *       Description     : Thread function for Upload rpc.
 *       Args            : ThreadUpload *)
 *       Returns         : 0 
 *********************************************************************************************/
U8 ipr_cwmp_performUpload(ThreadUpload *thArg_t)    
{
	TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
	ThreadUpload *th_up_t = NULL;
	Upload *l_upload_t = NULL;
	//UINT32 wait_time = 0;
	U8 cont = 1;
	Uint8* FaultCode = NULL;
	Uint8* FaultString =  NULL;
	UINT16 res = 0;
	UINT16 *glue_res = NULL;
	ClientRequest *sClientReq = NULL;

	th_up_t = (ThreadUpload *) thArg_t;  

	if (NULL == th_up_t || NULL == th_up_t->pstupload)
	{
		cont = 0; 
		TRACE(LOG_CRITICAL , RPC_HANDLER, " Recieved NULL argument, return "); 
	}


	if (cont)
	{
		l_upload_t = (Upload *)th_up_t->pstupload;
	}

	time_t t1, t2;

	(void) time(&t1);

	if (cont){
		res = connManagerUpload(l_upload_t , th_up_t->local_path);
		(void) time(&t2);

		if (res != RESULT_OK)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "File Transfer Failed with return status :%d",res);
			FaultCode = (Uint8 *)ALLOCATE(FAULT_CODE_SIZE );
			if(NULL ==  FaultCode)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				return FAILED;
			}
			FaultString = (Uint8 *)ALLOCATE(FAULT_STRING_SIZE );
			if(NULL ==  FaultString)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				DEALLOCATE(FaultCode);
				return FAILED;
			}
			/*SPR 16138  Fixed Start*/
			if( strstr(l_upload_t->URL,"@")!= NULL){

				strncpy(FaultCode, "9003", FAULT_CODE_SIZE);
				strncpy(FaultString ,
						"File Transfer failed: Userifo persent for file transfer",
						FAULT_STRING_SIZE);

			}
			else
			{

			/*SPR 16138  Fixed End*/

				switch (res)
				{
                /* SPR 20142 FIXED START */
                case CURLE_LOGIN_FAILURE:
               /* SPR 20142 FIXED END */
					case CURLE_LOGIN_DENIED :
						{
							ENB_STRNCPY(FaultCode, "9012", FAULT_CODE_SIZE);
							ENB_STRNCPY(FaultString ,
									"File Transfer failed: Server authentication failure",
									FAULT_STRING_SIZE);
						}
						break;
					case CURLE_UNSUPPORTED_PROTOCOL:
							{
								ENB_STRNCPY(FaultCode, "9013", FAULT_CODE_SIZE);
								ENB_STRNCPY(FaultString ,
										"File Transfer failed: Unsupported protocol for file transfer",
										FAULT_STRING_SIZE);
							}
							break;
/*download upload change starts*/
                    /* SPR 21824 FIXED START*/
                    case CURLE_FILE_COULDNT_READ_FILE:
                    {
                        ENB_STRNCPY(FaultCode, "9011", FAULT_CODE_SIZE);
                        ENB_STRNCPY(FaultString ,
                                   "File Open failed: A file given with FILE couldn't be opened",
                                    FAULT_STRING_SIZE);
                    }    
                    break;
                    /* SPR 21824 FIXED END*/

					case 7:
							{
			                    TRACE (LOG_CRITICAL , RPC_HANDLER, " CURLE_COULDNT_CONNECT File Transfer Failed with return status :%d",res);
								ENB_STRNCPY(FaultCode, "9011", FAULT_CODE_SIZE);
							
								/*SPR 18267 FIXED START*/
								ENB_STRNCPY(FaultString ,
										"Upload failure : Failed to connect to FTP server",
										FAULT_STRING_SIZE);
								/*SPR 18267 FIXED END*/

							}
							break;
/*download upload change ends*/
							default :
							{
								ENB_STRNCPY(FaultCode, "9011", FAULT_CODE_SIZE);
								ENB_STRNCPY(FaultString , "Upload failure", FAULT_STRING_SIZE);
							}
							break;
						}
				/*SPR 16138  Fixed Start*/
			}
			/*SPR 16138  Fixed End*/


				}
				else
				{
			/*  Call to  Glue Layer */
			glue_res = (UINT16 *)stCallbackFunList.invokeCallback((void *)l_upload_t ,
					RPC_UPLOAD,NULL);

			if(glue_res)
			{
				//do nothing
			}
			FaultCode = (Uint8 *)ALLOCATE(FAULT_CODE_SIZE );
			if(NULL ==  FaultCode)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				return FAILED;
			}
			FaultString = (Uint8 *)ALLOCATE(FAULT_STRING_SIZE );
			if(NULL ==  FaultString)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				DEALLOCATE(FaultCode);
				return FAILED;
			}
			ENB_STRNCPY(FaultCode, "0",2);
			ENB_STRNCPY(FaultString , " ", 2);
		}
        /* SPR 21563_p1 FIXED START */
        if(glue_res)
        {
            DEALLOCATE(glue_res);
        }
        /* SPR 21563_p1 FIXED END */ 

	}
	(void) time(&t2);
	sClientReq = (ClientRequest *)ALLOCATE(sizeof(ClientRequest) );
	if (sClientReq)
	{
		sClientReq->eReqtype = TRANS_COMPLETE_REQ;
		sClientReq->pstTxComplete = ALLOCATE(sizeof (struct _cwmp__TransferComplete) );

		if (sClientReq->pstTxComplete  != NULL)
		{
			sClientReq->pstTxComplete->CommandKey =  ALLOCATE(COMMAND_KEY_SIZE );
			sClientReq->EventCode = ALLOCATE(EVENT_CODE_SIZE  );
			if (sClientReq->pstTxComplete->CommandKey != NULL)
			{
				if (cont && NULL != l_upload_t->CommandKey)
					ENB_STRNCPY(sClientReq->pstTxComplete->CommandKey ,
							l_upload_t->CommandKey, COMMAND_KEY_SIZE);
				else
					ENB_STRNCPY(sClientReq->pstTxComplete->CommandKey , "", 2);
			}
			if ( sClientReq->EventCode != NULL)
			{
				ENB_STRNCPY(sClientReq->EventCode, UPLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);
			}
			if (res != RESULT_OK)
			{
                /* spr 17961 fix start */
				sClientReq->pstTxComplete->StartTime = 0;
				sClientReq->pstTxComplete->CompleteTime = 0;
                /* spr 17961 fix end */
			}
			else
			{
				sClientReq->pstTxComplete->StartTime = t1;
				sClientReq->pstTxComplete->CompleteTime = t2;
			}

			sClientReq->pstTxComplete->FaultStruct = ALLOCATE(
					sizeof(struct cwmp__TransferCompleteFaultStruct));
			if(NULL ==  sClientReq->pstTxComplete->FaultStruct)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				DEALLOCATE(sClientReq->EventCode);
				DEALLOCATE(sClientReq);
				return FAILED;
			}
			sClientReq->pstTxComplete->FaultStruct->FaultCode = FaultCode;
			sClientReq->pstTxComplete->FaultStruct->FaultString = FaultString;
			ipr_cwmp_addClientRequest (sClientReq);
		}
	}

	TRACE(LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
	return 0;
}


/********************************************************************************************
 *       Function Name   : ipr_cwmp_Upload
 *       Description     : Function call for Upload rpc.
 *       Args            :(Upload *)
 *       Returns         :SInt32 :0 or 1 
 *********************************************************************************************/
SInt16 ipr_cwmp_Upload (Upload *arg_upload_t)
{
	TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);

    /*SPR 19268 FIXED START*/
	UpldResp *ret = NULL;
    /*SPR 19268 FIXED END*/
        /* spr 19123 fix start */
	Uint8* FaultCode = NULL;
	Uint8* FaultString =  NULL;
	ClientRequest *sClientReq = NULL;
        /* spr 19123 fix end */
    /*  coverity 86545 fix start */
	SInt32 rc;
    /*  coverity 86545 fix end */
	ThreadUpload *thArg_t = NULL;

	Upload *l_upld_t = NULL;
	SInt32 r = 0;
    /*download upload change starts*/
	FILE *queue_fp = NULL;
	Uint8 queue_file_string[4] ={0};
	char command[256] = {0};
	UInt32 queue_file_count = 0;
   /*download upload change ends*/
	//coverity 5sep
	int randomData = open("/dev/random", O_RDONLY);
	if(randomData >=0)
	{    
		if( -1 == read(randomData, &r, sizeof(r)))
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "In  %s  error in reading random data ",__FUNCTION__);
			close(randomData);
			return 0;
		}
		close(randomData);
	}

	Char8* upl_logfilename  = NULL;
	FILE *upl_fp = NULL;
	time_t t_other;

	if (NULL == arg_upload_t)
	{
		TRACE(LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL argument for Upload ");
		return FAULT_9002;
	} 

	if (check_user_component(arg_upload_t->URL) == INVALID)
	{
		TRACE(LOG_CRITICAL , RPC_HANDLER, "Error: check URL, return with 9003");
		return 9003;
	}


	if (!g_rpc_IstxQ_init)
	{
		init_list(&g_Txn_List);
		g_rpc_IstxQ_init = 1;
	}

#ifdef VENDOR_SPECIFIC
	UInt32 list_size = 0;
	list_size =  ylCount(&g_Txn_List);
    /*download upload change starts*/
	TRACE(LOG_CRITICAL , RPC_HANDLER, "In VENDOR_SPECIFIC case list_size = %d\n", list_size);
    TRACE(LOG_CRITICAL , RPC_HANDLER, "In VENDOR_SPECIFIC case");
	if (list_size >= queue_upload_max_size)
	{
		return FAULT_9004;
	}
    /*download upload change ends*/
#endif

     /*download upload change starts*/
	if((g_th_upload) &&(0 == arg_upload_t->DelaySeconds))
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s since upload is already in progress and upload request with\
				zero delay seconds is received so rejecting this upload request\n",__FUNCTION__);
		return 9001;
	}
	snprintf (command, 256, "ls %s | grep %s | wc -l", DW_DIR, "upl");
	TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s command generated = %s\n",__FUNCTION__,command);

	queue_fp = popen(command, "r");
	if (queue_fp == NULL)
	{
		return 0;
	}

	if( NULL != fgets(queue_file_string, 4, queue_fp))
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue_file_string = %s\n", __FUNCTION__, queue_file_string);
	}

	queue_file_count = strtol(queue_file_string,NULL,0);
	TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue_file_count=%d\n",__FUNCTION__, queue_file_count);
	TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue_upload_max_size =%d\n",__FUNCTION__, queue_upload_max_size);
	if(queue_file_count == queue_upload_max_size)
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue for upload is full so request not handled\n", __FUNCTION__);
        /* Coverity 96632 + */
	    pclose(queue_fp);
        /* Coverity 96632 - */
		return 9004;
	}
    /*download upload change ends*/
	l_upld_t = ALLOCATE(sizeof(Upload)  );
	if(NULL ==  l_upld_t)
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
        /* Coverity 96632 + */
	    pclose(queue_fp);
        /* Coverity 96632 - */
		return 0;
	}

	if (l_upld_t)
	{
		l_upld_t->CommandKey = (char *)ALLOCATE(COMMAND_KEY_SIZE  );
		if(NULL ==  l_upld_t->CommandKey)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			DEALLOCATE(l_upld_t);
			/* COVERITY 96632 FIX START */
	        pclose(queue_fp);
            /* Coverity 96632 - */
			return 0;
		}
		l_upld_t->FileType = (char *)ALLOCATE(FILE_TYPE_SIZE  );
		if(NULL ==  l_upld_t->FileType)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			DEALLOCATE(l_upld_t->CommandKey);
			DEALLOCATE(l_upld_t);
			/* COVERITY 96632 FIX START */
	        pclose(queue_fp);
            /* Coverity 96632 - */
			return 0;
		}
		l_upld_t->URL = (char *)ALLOCATE(256  );
		if(NULL ==  l_upld_t->URL)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			DEALLOCATE(l_upld_t->CommandKey);
			DEALLOCATE(l_upld_t->FileType);
			DEALLOCATE(l_upld_t);
			/* COVERITY 96632 FIX START */
	        pclose(queue_fp);
            /* Coverity 96632 - */
			return 0;
		}
		l_upld_t->Username = (char *)ALLOCATE(256 );
		if(NULL ==  l_upld_t->Username)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			DEALLOCATE(l_upld_t->URL);
			DEALLOCATE(l_upld_t->CommandKey);
			DEALLOCATE(l_upld_t->FileType);
			DEALLOCATE(l_upld_t);
			/* COVERITY 96632 FIX START */
	        pclose(queue_fp);
            /* Coverity 96632 - */
			return 0;
		}
		l_upld_t->Password  = (char *)ALLOCATE(256 );
		if(NULL ==  l_upld_t->Password)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			DEALLOCATE(l_upld_t->Username);
			DEALLOCATE(l_upld_t->URL);
			DEALLOCATE(l_upld_t->CommandKey);
			DEALLOCATE(l_upld_t->FileType);
			DEALLOCATE(l_upld_t);
			/* COVERITY 96632 FIX START*/
	        pclose(queue_fp);
            /* Coverity 96632 - */
			return 0;
		}
		l_upld_t->DelaySeconds = arg_upload_t->DelaySeconds;

		tr_strncpy(l_upld_t->CommandKey , arg_upload_t->CommandKey);
		tr_strncpy(l_upld_t->URL , arg_upload_t->URL); 
		tr_strncpy(l_upld_t->FileType ,  arg_upload_t->FileType);
		tr_strncpy(l_upld_t->Username ,  arg_upload_t->Username);
		tr_strncpy(l_upld_t->Password , arg_upload_t->Password);

	}


    /*SPR 19268 FIXED START*/
	ret = (UpldResp*)stCallbackFunList.invokeCallback((void *)arg_upload_t, RPC_PREUPLOAD , NULL);
    /*SPR 19268 FIXED END*/

    /* SPR 17268 FIXED START */
    UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
    /* SPR 17268 FIXED END */
    /*SPR 19268 FIXED START*/
	if (NULL != ret)
	{
        if( RPC_SUCCESS == ret->eStatus)
	    {
		TRACE(LOG_DETAIL , RPC_HANDLER, " In %s , recieved Path : %s ", __FUNCTION__ , ret->filepath); 
    /*SPR 19268 FIXED END*/

		// Queue The upload
		time(&t_other);

		upl_logfilename = (Char8 *) ALLOCATE(MAX_UPL_LOG_NAME_LEN );

		if (NULL != upl_logfilename)
		{
			snprintf(upl_logfilename ,  MAX_UPL_LOG_NAME_LEN, "%s_%ld_%d", UPL_FILE_PATH_NAME , t_other,r) ;
		
        /* SPR 17268 FIXED START */
            //UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
           strcpy((char*)ABSPath1,(char*)ABSPath);

            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath before concat %s ",ABSPath1 );
            /*  coverity 96655 fix start */
            if((strlen(upl_logfilename) + strlen((char *)ABSPath1)) < CONFIG_PARAM_LEN)
            {
                strncat((char *)ABSPath1 ,upl_logfilename , strlen(upl_logfilename));
                /* SPR 21563_p1 FIXED START */
                DEALLOCATE(upl_logfilename);
                /* SPR 21563_p1 FIXED END */
                TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath after concat %s ",ABSPath1 );
            }
            /*  coverity 96655 fix end */

            upl_fp = fopen((char *)ABSPath1 , "w+");
            /* SPR 17268 FIXED END */



//           	upl_fp = fopen(upl_logfilename , "w+");
			if (NULL != upl_fp)
			{
                /*SPR 19268 FIXED START*/
				ENB_FPRINTF(upl_fp , "%s\n%s\n%s\n%s\n%s\n%u\n%s\n%ld",
						(NULL == arg_upload_t->CommandKey || strlen(arg_upload_t->CommandKey) == 0)? "-" :arg_upload_t->CommandKey,
						(NULL == arg_upload_t->FileType || strlen(arg_upload_t->FileType) == 0)? "-" :arg_upload_t->FileType,
						(NULL == arg_upload_t->URL  || strlen(arg_upload_t->URL)  == 0)? "-" :arg_upload_t->URL,
						(NULL == arg_upload_t->Username || strlen(arg_upload_t->Username) == 0)? "-" :arg_upload_t->Username,
						(NULL == arg_upload_t->Password || strlen(arg_upload_t->Password) == 0)? "-" :arg_upload_t->Password,
						arg_upload_t->DelaySeconds, ret->filepath , t_other);
				fclose(upl_fp);
                /*SPR 19268 FIXED END*/
			}
		}

		thArg_t = (ThreadUpload *) ALLOCATE(sizeof (ThreadUpload) );
		if (NULL == thArg_t)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error:thArg_t Memory allocation failure");
                /*SPR 19268 FIXED START*/
            DEALLOCATE(ret->filepath);
            ret->filepath = NULL;
            DEALLOCATE(ret); 
            ret = NULL;
                /*SPR 19268 FIXED END*/
			exit(0);
		}
		//thArg_t->local_path = ret;
		thArg_t->local_path = ALLOCATE(256 );
		if(NULL ==  thArg_t->local_path)
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			/* COVERITY 96632 FIX START*/
			pclose(queue_fp);
			/* COVERITY 96632 FIX END */
			/*SPR 19268 FIXED START*/
			DEALLOCATE(ret->filepath);
			ret->filepath = NULL;
			DEALLOCATE(ret); 
			ret = NULL;
			/*SPR 19268 FIXED END*/


			exit(0);		
		}
                /*SPR 19268 FIXED START*/
		ENB_STRCPY(thArg_t->local_path , ret->filepath);
                /*SPR 19268 FIXED END*/
		//thArg_t->pstupload = arg_upload_t; 
		thArg_t->pstupload = l_upld_t;
        thArg_t->upl_logfilename =  (Char8 *) ALLOCATE(MAX_UPL_LOG_NAME_LEN );
		strcpy(thArg_t->upl_logfilename,(char *)ABSPath1);
		//thArg_t->upl_logfilename = upl_logfilename;
		thArg_t->delay = arg_upload_t->DelaySeconds;
		thArg_t->recv_time = t_other;
		thArg_t->Status = RPC_TRUE;
		thArg_t->cancelStatus = CANCEL_FALSE;


		if (g_th_upload)
		{
			rc =  pthread_kill(g_th_upload , 0);
			if (!rc)
			{
				//Just Add in List  
				ADD_IN_LIST(&g_Txn_List , (YLNODE *)thArg_t);
			}
		}
		else
		{
			ADD_IN_LIST(&g_Txn_List , (YLNODE *)thArg_t);

			// set stack size attibute
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			//  Start Of thread  to upload 
			rc = pthread_create(&g_th_upload , &attr, ipr_cwmp_actualUpload , (void *)thArg_t);
            /*  coverity 86543 fix start */
            if(0 != rc)
                TRACE(LOG_INFO , RPC_HANDLER, "Thread creation failed with %d error \n", rc);
            /*  coverity 86543 fix end */
			pthread_attr_destroy(&attr);
			pthread_detach(g_th_upload);
		}
                /*SPR 19268 FIXED START*/
        DEALLOCATE(ret->filepath);
        ret->filepath = NULL;
        /* SPR 21563_p1 FIXED START */
        DEALLOCATE(ret->fault_string);
        /* SPR 21563_p1 FIXED END */
        DEALLOCATE(ret);
        ret = NULL;
                /*SPR 19268 FIXED END*/
	}

        /* spr 19123 fix start */
	else{
		sClientReq = (ClientRequest *)ALLOCATE(sizeof(ClientRequest) );
		if (sClientReq)
		{
			sClientReq->eReqtype = TRANS_COMPLETE_REQ;
			sClientReq->pstTxComplete = ALLOCATE(sizeof (struct _cwmp__TransferComplete) );

			if (sClientReq->pstTxComplete  != NULL)
			{
				sClientReq->pstTxComplete->CommandKey =  ALLOCATE(COMMAND_KEY_SIZE );
				sClientReq->EventCode = ALLOCATE(EVENT_CODE_SIZE  );
				if (sClientReq->pstTxComplete->CommandKey != NULL)
				{
					if ( NULL != l_upld_t->CommandKey)
						ENB_STRNCPY(sClientReq->pstTxComplete->CommandKey ,
								l_upld_t->CommandKey, COMMAND_KEY_SIZE);
					else
						ENB_STRNCPY(sClientReq->pstTxComplete->CommandKey , "", 2);
				}
				if ( sClientReq->EventCode != NULL)
				{
					ENB_STRNCPY(sClientReq->EventCode, UPLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);
				}
				/* spr 17961 fix start */
				sClientReq->pstTxComplete->StartTime = 0;
				sClientReq->pstTxComplete->CompleteTime = 0;
				/* spr 17961 fix end */

				sClientReq->pstTxComplete->FaultStruct = ALLOCATE(
						sizeof(struct cwmp__TransferCompleteFaultStruct));
				if(NULL ==  sClientReq->pstTxComplete->FaultStruct)
				{
					TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
					DEALLOCATE(sClientReq->EventCode);
					DEALLOCATE(sClientReq);
					/* COVERITY 96632 FIX START */
					pclose(queue_fp);
					/* COVERITY 96632 FIX END */                    /*SPR 19268 FIXED START*/
					DEALLOCATE(ret->fault_string);
					ret->fault_string = NULL;
					DEALLOCATE(ret); 
					ret = NULL;
					/*SPR 19268 FIXED END*/
                                       /* SPR 19162 FIXED START */
					return TR069_ZERO;
					/*SPR 19162 FIXED END */

	}
				FaultCode = (Uint8 *)ALLOCATE(FAULT_CODE_SIZE );
				if(NULL ==  FaultCode)
				{
					TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
					/* COVERITY 96632 FIX START */
					pclose(queue_fp);

					/* COVERITY 96632 FIX END */
					/*SPR 19268 FIXED START*/
					DEALLOCATE(sClientReq->EventCode);
					DEALLOCATE(sClientReq);
					DEALLOCATE(ret->fault_string);
					ret->fault_string = NULL;
					DEALLOCATE(ret);
					ret = NULL;
					/*SPR 19268 FIXED END*/
                                       /* SPR 19162 FIXED START */
					return TR069_ZERO;
					/*SPR 19162 FIXED END */

				}
				FaultString = (Uint8 *)ALLOCATE(FAULT_STRING_SIZE );
				if(NULL ==  FaultString)
				{
					TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
					DEALLOCATE(FaultCode);
					/* COVERITY 96632 FIX START */
					pclose(queue_fp);
					/* COVERITY 96632 FIX END */
					/*SPR 19268 FIXED START*/
					DEALLOCATE(sClientReq->EventCode);
					DEALLOCATE(sClientReq);
					DEALLOCATE(ret->fault_string);
					ret->fault_string = NULL;
					DEALLOCATE(ret);
					ret = NULL;
					/*SPR 19268 FIXED END*/
                                       /* SPR 19162 FIXED START */
					return TR069_ZERO;
					/*SPR 19162 FIXED END */


				}
                    /*SPR 19268 FIXED START*/
                 
    	            tr_sprintf(FaultCode ,  "%d", ret->fault_code) ;
				    ENB_STRCPY(FaultString ,ret->fault_string );						
                    /*SPR 19268 FIXED END*/
						

				sClientReq->pstTxComplete->FaultStruct->FaultCode = FaultCode;
				sClientReq->pstTxComplete->FaultStruct->FaultString = FaultString;
				ipr_cwmp_addClientRequest (sClientReq);
			}
		}
        /*SPR 19268 FIXED START*/
        DEALLOCATE(ret->fault_string);
        ret->fault_string = NULL;
        DEALLOCATE(ret);
        ret = NULL;
        /*SPR 19268 FIXED END*/

    }

    /* SPR 19268 FIXED START*/
    }
    /*SPR 19268 FIXED END*/

    /* spr 19123 fix end */

	TRACE(LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
    /* Coverity 96632 + */
    if(queue_fp) {
	pclose(queue_fp); }
    /* Coverity 96632 - */

	/*	DEALLOCATE(upl_logfilename);
	DEALLOCATE(l_upld_t->Username);
	DEALLOCATE(l_upld_t->CommandKey);
	DEALLOCATE(l_upld_t->URL);
	DEALLOCATE(l_upld_t->FileType);
	DEALLOCATE(l_upld_t->Password);
	DEALLOCATE(l_upld_t);
		*/
	return TRUE;
}


/* Phase 2 */

/***************************************************************************************
 * Function Name : ipr_cwmp_AddObject
 *
 * Description   : This function is a method to perform functionality of
 *                 rpc method i.e addobject , it takes the input as parameter
 *                 list of name and key and populates the internal structure
 *                 for all CPE parameters and pass these structure to management
 *                 module to set the paramaters. and give the list to glue layer
 *                 to perform the set parameter functionality. ( through callback)
 *
 *Arguments     : ParameterList , list of (name , key)
 *
 *Returns       : AddObjectStatus_t * (pointer tio StStatus structure)
 *                RPC_SUCCESS :  if successfully peformed
 *                RPC_FAULT   :  if Not able to add due to some error in inputs
 *                Fault structre filled if failed to perform .
 *
 ****************************************************************************************/
AddObjectStatus_t  * ipr_cwmp_AddObject (sAddObject *ParamVal)
{
    Uint8 *cTmpName = NULL;
    //Uint8 *cTmpKey = NULL;
    AddObjectStatus_t *stGlueRet = NULL;

    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);

    if (NULL == ParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;
    }

    cTmpName = ParamVal->ObjectName;
    //cTmpKey = ParamVal->ParameterKey;
    if(cTmpName != NULL )
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "Received ObjectName %s  ",cTmpName);
        if (strncmp(cTmpName , CPE_PARAM_PREFIX, CPE_PARAM_PREFIX_LEN) == 0)
        {
            TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Invalid Parameter Received in Add Object Request.");
            return NULL;
        }
        else
        {
            ///Calling Glue Layer to perform the functionilty with Input of Parameter List
            stGlueRet = (AddObjectStatus_t *) stCallbackFunList.invokeCallback ((void   *) ParamVal,
                    RPC_ADDOBJECT, NULL);

            TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
            return stGlueRet;
        }
    }
    else
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;

    }
}

/******************************************************************************
 * *   FUNCTION NAME  : ipr_cwmp_deleteobject
 * *   Inputs         : sDeleteObject *ParamVal
 * *   Outputs        : DeleteObjectStatus_t * (pointer to DeleteObjectStatus_t structure)
 *                      RPC_SUCCESS :  if successfully peformed
 *                      RPC_FAULT   :  if Not able to delete object due to some error
 *                      Fault structre filled if failed to perform .
 * *   RETURNS        : DeleteObjectStatus_t * (pointer to DeleteObjectStatus_t structure)
 * *   DESCRIPTION    : This function is a method to perform functionality of
 *                      rpc method i.e DeleteObject
 * *******************************************************************************/
DeleteObjectStatus_t  * ipr_cwmp_deleteobject(sDeleteObject *ParamVal)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    Uint8 *cTmpName = NULL;
    DeleteObjectStatus_t *stGlueRet = NULL;

    if (NULL == ParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;
    }

    cTmpName = ParamVal->ObjectName;

    if (cTmpName != NULL )
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "Received Object   %s  ",cTmpName);
        if (strncmp(cTmpName , CPE_PARAM_PREFIX, CPE_PARAM_PREFIX_LEN) == 0)
        {
            TRACE (LOG_CRITICAL , RPC_HANDLER, 
                                    "ERROR :Invalid Parameter Received in Delete Object Request.");
            return NULL;
        }
        else
        {
            ///Calling Glue Layer to perform the functionilty with Input of Parameter List

            stGlueRet = (DeleteObjectStatus_t *) stCallbackFunList.invokeCallback ((void   *) cTmpName,
                    RPC_DELETEOBJECT, NULL);

            TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
            return stGlueRet;
        }
    }
    else
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;

    }



}

/******************************************************************************
 * *   FUNCTION NAME  : ipr_cwmp_reboot
 * *   Inputs         : None
 * *   Outputs        : RebootStatus_t * (pointer to RebootStatus_t structure)
 *                      RPC_SUCCESS :  if successfully peformed
 *                      RPC_FAULT   :  if Not able to reboot due to some error
 *                      Fault structre filled if failed to perform .
 * *   RETURNS        : RebootStatus_t * (pointer tio RebootStatus_t structure)
 * *   DESCRIPTION    : This function is a method to perform functionality of
 *                      rpc method i.e REBOOT
 * *******************************************************************************/
RebootStatus_t  * ipr_cwmp_reboot(void)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    RebootStatus_t *stGlueRet = NULL;

    // Calling the Glue Layer to perform  functionilty with Input of Parameter List

    stGlueRet = (RebootStatus_t *) stCallbackFunList.invokeCallback (NULL,
            RPC_REBOOT, NULL);

    
    /* Spr 16890 Fix Start */
    /* Code Deleted */
    /* Spr 16890 Fix Start */
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting  %s  ",__FUNCTION__);

    return stGlueRet;

}

/******************************************************************************
 * *   FUNCTION NAME  : ipr_cwmp_factoryreset
 * *   Inputs         : None
 * *   Outputs        : FactoryResetStatus_t * (pointer to FactoryResetStatus_t structure)
 *                      RPC_SUCCESS :  if successfully peformed
 *                      RPC_FAULT   :  if Not able to reboot due to some error
 *                      Fault structre filled if failed to perform .
 * *   RETURNS        : FactoryResetStatus_t * (pointer to FactoryResetStatus_t structure)
 * *   DESCRIPTION    : This function is a method to perform functionality of
 *                      rpc method i.e FACTORY RESET
 * *******************************************************************************/
FactoryResetStatus_t  * ipr_cwmp_factoryreset(void)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    FactoryResetStatus_t *stGlueRet = NULL;

    // Calling the Glue Layer to perform  functionilty with Input of Parameter List

    stGlueRet = (FactoryResetStatus_t *) stCallbackFunList.invokeCallback (NULL,
            RPC_FACTORYRESET, NULL);

    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);

    return stGlueRet;

}

/******************************************************************************
 * * *   FUNCTION NAME  : ipr_cwmp_getparametername
 * * *   Inputs         : None
 * * *   Outputs        : GetParameterNameStatus_t * (pointer to GetParameterNameStatus_t structure)
 * *                      RPC_SUCCESS :  if successfully peformed
 * *                      RPC_FAULT   :  if Not able to get param due to some error
 * *                      Fault structre filled if failed to perform .
 * * *   RETURNS        : GetParameterNameStatus_t * (pointer to GetParameterNameStatus_t structure)
 * * *   DESCRIPTION    : This function is a method to perform functionality of
 * *                      rpc method i.e GetParameterName
 *  * *******************************************************************************/

GetParameterNamesStatus_t  * ipr_cwmp_getparametername(sGetParameterNames *ParamVal)
{
    /* Spr 16990 Fix Start */
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    GetParameterNamesStatus_t *stGlueRet = NULL;

    if (NULL == ParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;
    }
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Passing request to glue layer ....");
    /* Calling Glue Layer to perform the functionilty with Input of Parameter List */

    stGlueRet = (GetParameterNamesStatus_t *) stCallbackFunList.invokeCallback ((void   *) ParamVal,
            RPC_GETPARAMETERNAME, NULL);
    return stGlueRet;
    /* Spr 16990 Fix End */
}

/******************************************************************************
 * * * *   FUNCTION NAME  : ipr_cwmp_getallListInfo
 * * * *   Inputs         : None
 * * * *   Outputs        : ParameterInfoList * (pointer to ParameterInfoList structure)
 * * *                      RPC_SUCCESS :  if successfully peformed
 * * *                      RPC_FAULT   :  if Not able to reboot due to some error
 * * *                      Fault structre filled if failed to perform .
 * * * *   RETURNS        : ParameterInfoList * (pointer to ParameterInfoList structure)
 * * * *   DESCRIPTION    : This function is a method to return all the cpe supported parameter
 * * * *******************************************************************************/
ParameterInfoList * ipr_cwmp_getallListInfo(void) 
{

    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);
    ParameterInfoList *stParamList  = NULL; //typedef struct ParameterInfoList  ParameterInfoList;
    ParameterInfoStruct  *stTmpParam[18]; // typedef cwmp__ParameterInfoStruct ParameterInfoStruct;
    ParameterInfoStruct    **stTmpParamArr = NULL;
    SInt32 i = 0;
    //    pstParameterList = *ptrToParam;
    //   stcpeParam = pstParameterList->__ptrParameterInfoStruct;
 
    /*SPR 16774 FIX START */
    char * parant_str = "InternetGatewayDevice.ManagementServer.";

    /*SPR 16774 FIX END */

    stParamList = (ParameterInfoList *) ALLOCATE(sizeof(ParameterInfoList));
    if (stParamList == NULL)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR Could not allocate memory ....");
        return NULL;
    }

    stTmpParamArr = (ParameterInfoStruct **) ALLOCATE( sizeof(ParameterInfoStruct*)*18);

    if (stTmpParamArr == NULL)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR Could not allocate memory ....");
//coverity fix
DEALLOCATE(stParamList );        
return NULL;
    }

    while (i < 18)
    {
        stTmpParam[i] = (ParameterInfoStruct *)ALLOCATE( sizeof(ParameterInfoStruct));

        if (stTmpParam[i] != NULL)
        {
            stTmpParam[i]->Name = (Uint8 *) ALLOCATE( MAX_PARAM_NAME_SIZE);
            if (!(stTmpParam[i]->Name))
	    {
		    //coverity fix
		    DEALLOCATE(stParamList );        
		    DEALLOCATE(stTmpParamArr);        
                return NULL;
            }
            stTmpParamArr[i] = stTmpParam[i];
        }
	else{
		//coverity fix
		DEALLOCATE(stTmpParamArr);	
		DEALLOCATE(stParamList );        
            return NULL;
}
        i++;
    }
     /*SPR 16774 FIX START */
    ENB_STRNCPY(stTmpParam[0]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[0]->Name , ENABLECWMP, strlen(ENABLECWMP)+1);
    stTmpParam[0]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[1]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[1]->Name , URL_STRING, strlen(URL_STRING)+1);
    stTmpParam[1]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[2]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[2]->Name , USERNAME, strlen(USERNAME)+1);
    stTmpParam[2]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[3]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[3]->Name , PWD, strlen(PWD)+1);
    stTmpParam[3]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[4]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[4]->Name , PERIOD_INFORM_EN, strlen(PERIOD_INFORM_EN)+1);
    stTmpParam[4]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[5]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[5]->Name ,PERIOD_INFORM_INT, strlen(PERIOD_INFORM_INT)+1);
    stTmpParam[5]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[6]->Name , parant_str, MAX_PARAM_NAME_SIZE);
   strncat(stTmpParam[6]->Name , PERIOD_INFORM_TIME, strlen(PERIOD_INFORM_TIME)+1);
    stTmpParam[6]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[7]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[7]->Name , CONN_REQ_URL, strlen(CONN_REQ_URL)+1);
    stTmpParam[7]->Writable = xsd__boolean__false_;

    ENB_STRNCPY(stTmpParam[8]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[8]->Name , CONN_REQ_USR, strlen(CONN_REQ_USR)+1);
    stTmpParam[8]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[9]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[9]->Name , CONN_REQ_PWD, strlen(CONN_REQ_PWD)+1);
    stTmpParam[9]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[10]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[10]->Name , UPGRD_MANAGED, strlen(UPGRD_MANAGED)+1);
    stTmpParam[10]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[11]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[11]->Name , KICKURL, strlen(KICKURL)+1);
    stTmpParam[11]->Writable = xsd__boolean__false_;
   ENB_STRNCPY(stTmpParam[12]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[12]->Name , DWN_PROG_URL, strlen(DWN_PROG_URL)+1);
    stTmpParam[12]->Writable = xsd__boolean__false_;


    ENB_STRNCPY(stTmpParam[13]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[13]->Name , DF_ACT_NOTIF, strlen(DF_ACT_NOTIF)+1);
    stTmpParam[13]->Writable = xsd__boolean__true_;

    ENB_STRNCPY(stTmpParam[14]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[14]->Name , UDP_CONN_REQ_ADDR, strlen(UDP_CONN_REQ_ADDR)+1);
    stTmpParam[14]->Writable =  xsd__boolean__false_;

    ENB_STRNCPY(stTmpParam[15]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[15]->Name ,PARAM_KEY, strlen(PARAM_KEY)+1);
    stTmpParam[15]->Writable = xsd__boolean__false_;


    TRACE (LOG_CRITICAL , RPC_HANDLER, "COPY done ....");
    if(stParamList != NULL)
    {
        stParamList->__ptrParameterInfoStruct = stTmpParamArr;
        stParamList->__size = 16;
    }
    /*SPR 16774 FIX END */


   TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting from  %s  ",__FUNCTION__);
    return stParamList;

}
/******************************************************************************
 * * *   FUNCTION NAME  : ipr_cwmp_canceltransfer
 * * *   Inputs         : None
 * * *   Outputs        : CancelTransferStatus_t * (pointer to CancelTransferStatus_t structure)
 * *                      RPC_SUCCESS :  if successfully peformed
 * *                      RPC_FAULT   :  if Not able to cancel tranfer request due to some error
 * *                      Fault structre filled if failed to perform .
 * * *   RETURNS        : CancelTransferStatus_t * (pointer to GetParameterNameStatus_t structure)
 * * *   DESCRIPTION    : This function is a method to perform functionality of
 * *                      rpc method i.e Cencel tansfer 
 * * *******************************************************************************/
CancelTransferStatus_t  * ipr_cwmp_canceltransfer(sCancelTransfer *ParamVal)
{
    Uint8 *cTmpName = NULL;
    CancelTransferStatus_t *stGlueRet = NULL;
    UInt32 indx = 0;
    SInt32 cmp_res = 0;
    SInt32 res_up =0 ;
    SInt32 res_sch_dw =0 ;
    UInt32 list_size = 0;
    UInt32 sch_dwn_list_size = 0;
    Upload *l_upload_t = NULL;
    ScheduleDownload *l_sch_dw_t = NULL;
    SInt32 res_dw = 0;
    UInt32 dwn_list_size = 0;
    STDownload *pstDownload = NULL;
    


    //g_Txn_List_Sch_Dwn
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);

    if (NULL == ParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL Parameter List....");
        return NULL;
    }

    cTmpName = ParamVal->CommandKey;

    if(cTmpName == NULL)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Received NULL command key....");
        return NULL;
    }
    else
    {
        if(strlen(ParamVal->CommandKey) > 0) 
        {
            /* size of upload queue */
            list_size = ylCount(&g_Txn_List);
            TRACE (LOG_CRITICAL , RPC_HANDLER, " Received upload request  %d", list_size);
            /* size of schedule downlaod queue */
            sch_dwn_list_size = ylCount(&g_Txn_List_Sch_Dwn);
            TRACE (LOG_CRITICAL , RPC_HANDLER, " Received sch dwn  request : %d", sch_dwn_list_size);
            if (list_size == 0 )
            {
                TRACE (LOG_CRITICAL , RPC_HANDLER, " UPLOAD Queue is empty.");
                res_up = 0;
            }
            //     indx = 0;
            if(list_size > 0)
            {
                while (indx < list_size)
                {
                    TRACE (LOG_CRITICAL , RPC_HANDLER, " received indx: %d", indx);
                    ThreadUpload *l_th_up_t = NULL;
                    l_th_up_t = (ThreadUpload *)ylNth(&g_Txn_List , indx);

                    if (l_th_up_t != NULL)
                    {
                        if (l_th_up_t->pstupload)
                        {
                            l_upload_t = l_th_up_t->pstupload;
                            if(l_upload_t->CommandKey == NULL)
							{
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Could not retrieve cmd key ");
							}
                            else
                            { 
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Comparing received cmd key %s with %s",cTmpName,l_upload_t->CommandKey);
                                cmp_res = strcmp(cTmpName, l_upload_t->CommandKey);
                                if(cmp_res == 0)
                                {
                                    //ret = 1;
                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Matched Found ");
                                    /*check can_cancel flag value */   
                                    if(l_th_up_t->Status == RPC_TRUE)
                                    {
                                        l_th_up_t->cancelStatus = CANCEL_TRUE;
                                        res_up = 0;
                                    }
                                    if(l_th_up_t->Status == RPC_FALSE)
                                    {
                                        res_up = 1;
                                    }

                                }     
                                else
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "cmd key not matched ");
                            }     
                        }
                    }
                    indx++;
                }// whilea
            }//if
            indx = 0;
            if (sch_dwn_list_size == 0)
            {
                TRACE (LOG_CRITICAL , RPC_HANDLER, "schdule download Queue is empty");
                res_sch_dw = 0; 
            }

            if(sch_dwn_list_size > 0)
            {
                while (indx < sch_dwn_list_size)
                {
                    TRACE (LOG_CRITICAL , RPC_HANDLER, " received indx: %d", indx);
                    TRACE (LOG_CRITICAL , RPC_HANDLER, " Total list size in sch downlaod received indx: %d", sch_dwn_list_size);
                    ThreadSchDwn *th_sch_dwn = NULL;
                    th_sch_dwn = (ThreadSchDwn *)ylNth(&g_Txn_List_Sch_Dwn , indx);

                    if (th_sch_dwn != NULL)
                    {
                        if (th_sch_dwn->ptr_dwn_t)
                        {
                            l_sch_dw_t = th_sch_dwn->ptr_dwn_t;
                            TRACE (LOG_CRITICAL , RPC_HANDLER, "Comparing received command key ");
                            if(l_sch_dw_t->CommandKey == NULL){
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Could not retrieve cmd key ");
							}
                            else
                            {
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Comparing received cmd key %s with %s",
                                                                    cTmpName,l_sch_dw_t->CommandKey);
                                cmp_res = strcmp(cTmpName, l_sch_dw_t->CommandKey);
                                if(cmp_res == 0)
                                {
                                    //ret = 1;
                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Matched Found ");
                                    /*check can_cancel flag value */
                                    if(th_sch_dwn->Status == RPC_TRUE)
                                    {
                                        th_sch_dwn->cancelStatus = CANCEL_TRUE;
                                        res_sch_dw = 0;
                                    }
                                    if(th_sch_dwn->Status == RPC_FALSE)
                                    {
                                        res_sch_dw = 1 ;
                                    }

                                }
                                else
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "cmd key not matched ");
                            }
                        }
                    }
                    indx++;
                }// whilea
            }

         indx = 0; 
            dwn_list_size =  ylCount(&g_Txn_List_Download);
            if (dwn_list_size == 0)
            {
                TRACE (LOG_CRITICAL , RPC_HANDLER, "schdule download Queue is empty");
                res_dw = 0;
            }

            if(dwn_list_size > 0)
            {
                while (indx < dwn_list_size)
                {
                    TRACE (LOG_CRITICAL , RPC_HANDLER, " received indx: %d", indx);
                    TRACE (LOG_CRITICAL , RPC_HANDLER, " Total list size in downlaod received indx: %d", dwn_list_size);
                    ThreadDownload *th_dwn = NULL;
                    th_dwn = (ThreadDownload *)ylNth(&g_Txn_List_Download , indx);

                    if (th_dwn != NULL)
                    {
                        if (th_dwn->pstDownload)
                        {
                            pstDownload = th_dwn->pstDownload;
                            TRACE (LOG_CRITICAL , RPC_HANDLER, "Comparing received command key ");
                            if(pstDownload->CommandKey == NULL){
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Could not retrieve cmd key ");
                            }    
                            else
                            {
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Comparing received cmd key %s with %s",cTmpName,pstDownload->CommandKey);
                                cmp_res = strcmp(cTmpName, pstDownload->CommandKey);
                                if(cmp_res == 0)
                                {
                                    //ret = 1;
                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Matched Found ");
                                    /*check can_cancel flag value */
                                    if(th_dwn->Status == RPC_TRUE)
                                    {
                                        th_dwn->cancelStatus = CANCEL_TRUE;
                                        res_dw = 0;
                                    }
                                    if(th_dwn->Status == RPC_FALSE)
                                    {
                                        res_dw = 1 ;

                                    }

                                }
                                else
                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "cmd key not matched ");
                            }
                        }
                    }
                    indx++;
                }// whilea
            }
	    if(res_up == 0 && res_sch_dw ==0 && res_dw == 0)
	    {
		    /*  return success response */
		    stGlueRet =  (CancelTransferStatus_t *) ALLOCATE (sizeof(CancelTransferStatus_t) );
		    if(NULL ==  stGlueRet)
		    {
			    TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			    return NULL;
		    }
		    stGlueRet->eStatus = RPC_SUCCESS;
		    return stGlueRet;
	    }
	    if(res_up == 1 || res_sch_dw == 1 || res_dw == 1)
	    {
		    stGlueRet =  (CancelTransferStatus_t *) ALLOCATE (sizeof(CancelTransferStatus_t) );
		    if(NULL ==  stGlueRet)
		    {
			    TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
			    return NULL;
		    }
		    stGlueRet->eStatus = RPC_FAULT;
		    stGlueRet->fault_code = 9021;
		    stGlueRet->fault_string = ALLOCATE(1000 );
		    if(NULL ==  stGlueRet->fault_string)
		    {
			    TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
		
			    DEALLOCATE(stGlueRet);
			    return NULL;
		    }
		    ENB_STRNCPY(stGlueRet->fault_string,"Cancelation of file transfer not permitted in current transfer state",999);
		    TRACE (LOG_CRITICAL , RPC_HANDLER, "Cancelation of file transfer not permitted in current transfer state ");
		    return stGlueRet;
	    }
	}
    }//else
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s",__FUNCTION__);
    return NULL;
}

/********************************************************************************************
 *       Function Name   : ipr_cwmp_GetAllQueuedTransfers
 *       Description     : RPC function call for GetAllQueuedTransfers rpc.
 *       Args            : void
 *       Returns         : AllTransferList * :  Pointer to list of all queued transfers.
 *********************************************************************************************/
AllTransferList* ipr_cwmp_GetAllQueuedTransfers(void)
{
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__);

	AllTransferList *res_TransferList_t = NULL;
	struct cwmp__AllQueuedTransferStruct ** arrAQT_t = NULL;
	struct cwmp__AllQueuedTransferStruct *ptrAQT_t = NULL;
	Uint8 ind_list = 1;
	struct dirent *entry;
	DIR *dp;
	FILE *dw_fp = NULL;
	Char8 strPath[MAX_DW_LOG_NAME_LEN];
	Char8 *tmpBuffer = NULL;
	Char8 *str = NULL;
	time_t currtime;
	time_t t1;
	LongInt  diff = 0;
	Int ret = 0 ;
	S32 delay_sec = 0;
	UInt16 l_size = 0;
	S32 timeWin_1_start = 0;
	S32 timeWin_2_start = 0;

	res_TransferList_t = (AllTransferList *)ALLOCATE(sizeof(AllTransferList) );
	if (!res_TransferList_t)
	{
		return NULL;
	}
	memset(res_TransferList_t ,'\0', sizeof(AllTransferList));
	dp = ENB_OPENDIR(DW_DIR);

	if (dp == NULL)
	{
		perror("opendir: Path does not exist or could not be read.");
		DEALLOCATE(res_TransferList_t);        
		return NULL;
	}

	while ((entry = ENB_READDIR(dp)))
	{
		if (strncmp(entry->d_name , "dwn", 3 ) == 0)
		{
			ENB_STRNCPY(strPath , DW_DIR, 11);
			strncat(strPath, "/", 2);
			strncat(strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-13));
			arrAQT_t = (struct cwmp__AllQueuedTransferStruct **)TR_REALLOC(arrAQT_t ,
					sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list);
			if(NULL ==  arrAQT_t)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				closedir(dp);
				DEALLOCATE(res_TransferList_t);        
				return NULL;
			}

			memset(arrAQT_t ,'\0', sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list );
		
            /* SPR 17268 FIXED START */
            UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
            strcpy((char*)ABSPath1,(char*)ABSPath);
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath before concat %s ",ABSPath1 );
            strncat((char *)ABSPath1 , strPath ,strlen(strPath));
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath after concat %s ",ABSPath1 );
            dw_fp = fopen((char *)ABSPath1, "r");
            /* SPR 17268 FIXED END */
	
			//dw_fp = fopen(strPath, "r");
			if (dw_fp != NULL)
			{
				tmpBuffer = (Char8 *)ALLOCATE(1024 );
				if (tmpBuffer == NULL)
                {
                    fclose(dw_fp);
                    continue;
                }
                    
				ret = fread((void *)tmpBuffer,1024, 1, dw_fp);
				if (ret != 0)
                {
                    fclose(dw_fp);
                    continue;
                }
                    
				tmpBuffer[1023] = '\0';
			
            fclose(dw_fp);
			

				ptrAQT_t = (struct cwmp__AllQueuedTransferStruct *)ALLOCATE(
						sizeof(struct cwmp__AllQueuedTransferStruct));
				if (NULL == ptrAQT_t)
					continue;

				ptrAQT_t->CommandKey = (Char8 *)ALLOCATE(COMMAND_KEY_SIZE );
				if(!ptrAQT_t->CommandKey)
				{
					continue;
				}			
				memset(ptrAQT_t->CommandKey ,'\0', COMMAND_KEY_SIZE);

				str = strtok(tmpBuffer , "\n");
				if (str != NULL)
				{

					if (*str == '-')
						ENB_STRNCPY(ptrAQT_t->CommandKey , "", 2);
					else
						ENB_STRNCPY(ptrAQT_t->CommandKey , str, COMMAND_KEY_SIZE);
				}

				ptrAQT_t->FileType  = (Char8 *)ALLOCATE(FILE_TYPE_SIZE );
				if(NULL ==  ptrAQT_t->FileType)
				{
					TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
					DEALLOCATE(tmpBuffer);
					DEALLOCATE(ptrAQT_t->CommandKey);
					DEALLOCATE(ptrAQT_t);
					DEALLOCATE(res_TransferList_t);
                    /* Coverity 40602 + */
                    DEALLOCATE(arrAQT_t);	
                    /* Coverity 40602 - */
					closedir(dp);
					return NULL;
				}
				memset(ptrAQT_t->FileType,'\0', FILE_TYPE_SIZE);
				str = strtok(NULL , "\n");
				if (str != NULL)
				{

					if (*str == '-')
						ENB_STRNCPY(ptrAQT_t->FileType , "", 2);
					else
						ENB_STRNCPY(ptrAQT_t->FileType , str, FILE_TYPE_SIZE-1);
				}
				//coverity fix		
				str = strtok(NULL , "\n"); //FOR URL
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str , __FUNCTION__);
				str = strtok(NULL , "\n"); // For Username
				TRACE (LOG_CRITICAL , RPC_HANDLER, "username %s  Entering %s  ",str,  __FUNCTION__);
				str = strtok(NULL , "\n"); // For Password
				TRACE (LOG_CRITICAL , RPC_HANDLER, "pass  %s  Entering %s  ",str,  __FUNCTION__);
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					ptrAQT_t->FileSize = strtoul(str, NULL, 0);
				}

				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					ptrAQT_t->TargetFileName = (Char8 *)ALLOCATE(TRG_NAME_SIZE  );
					if (!ptrAQT_t->TargetFileName)
					{				
						continue;
					}
					memset(ptrAQT_t->TargetFileName ,'\0', TRG_NAME_SIZE );
					
					if (*str == '-')
						ENB_STRNCPY( ptrAQT_t->TargetFileName , "", 2);
					else
						ENB_STRNCPY(ptrAQT_t->TargetFileName , str , TRG_NAME_SIZE);
				}
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					delay_sec = strtoul(str , NULL, 0);
				}
				str = strtok(NULL , "\n"); //For success URL
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str ,  __FUNCTION__);
				str = strtok(NULL , "\n"); // For Failure URL
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str ,  __FUNCTION__);
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					time(&currtime);
					t1 = strtol(str, NULL,10);
					diff =  currtime - t1;
					if (diff < delay_sec)
						ptrAQT_t->State = cwmp__TransferStateType__1;
					else
						ptrAQT_t->State = cwmp__TransferStateType__2;
				}

				DEALLOCATE(tmpBuffer);
				memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
				ptrAQT_t->IsDownload = xsd__boolean__true_;
				arrAQT_t[ind_list-1] = ptrAQT_t;
				ind_list++;
			}

		}
		else if (strncmp(entry->d_name , "upl", 3 ) == 0)
		{
			ENB_STRNCPY(strPath , DW_DIR, 11);
			strncat(strPath, "/", 2);
			strncat(strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-13));


          /* SPR 17268 FIXED START */
            UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
            strcpy((char*)ABSPath2,(char*)ABSPath);
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath before concat %s ",ABSPath2 );
            strncat((char *)ABSPath2 , strPath , strlen(strPath));
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath after concat %s ",ABSPath2 );
            dw_fp = fopen((char *)ABSPath2, "r");
            /* SPR 17268 FIXED END */

//			dw_fp = fopen(strPath, "r");
			if(NULL ==  dw_fp)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: In file open");
				exit(0);			
			}
			arrAQT_t = (struct cwmp__AllQueuedTransferStruct **)TR_REALLOC(arrAQT_t ,
					sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list);
			if(NULL ==  arrAQT_t)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				exit(0);			
			}

			memset(arrAQT_t ,'\0', sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list );

			tmpBuffer = (Char8 *)ALLOCATE( 1024 );
			if (tmpBuffer == NULL)
			{
                    fclose(dw_fp);
				continue;
			}
			memset(tmpBuffer ,'\0', 1024);

			ret = fread((void *)tmpBuffer,1024, 1, dw_fp);
			if (ret != 0)
                {
                    fclose(dw_fp);
                    continue;
                }
			tmpBuffer[1023] = '\0';

            fclose(dw_fp);

			ptrAQT_t = (struct cwmp__AllQueuedTransferStruct *)ALLOCATE (
					sizeof(struct cwmp__AllQueuedTransferStruct));
			if (NULL == ptrAQT_t)
			{
				continue;
			}
			memset(ptrAQT_t,'\0', sizeof(struct cwmp__AllQueuedTransferStruct));

			ptrAQT_t->CommandKey = (Char8 *)ALLOCATE(COMMAND_KEY_SIZE );
			if(!ptrAQT_t->CommandKey)
			{
				continue;
			}

			memset(ptrAQT_t->CommandKey ,'\0', COMMAND_KEY_SIZE);
			str = strtok(tmpBuffer , "\n");
			if (str != NULL)
			{

				if (*str == '-')
					ENB_STRNCPY(ptrAQT_t->CommandKey , "", 2);
				else
					ENB_STRNCPY(ptrAQT_t->CommandKey , str, COMMAND_KEY_SIZE);
			}

			ptrAQT_t->FileType  = (Char8 *)ALLOCATE(FILE_TYPE_SIZE );
			if(NULL ==  ptrAQT_t->FileType)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				exit(0);			
			}
			memset(ptrAQT_t->FileType ,'\0', FILE_TYPE_SIZE);

			str = strtok(NULL , "\n");
			if (str != NULL)
			{

				if (*str == '-')
					ENB_STRNCPY(ptrAQT_t->FileType , "", 2);
				else
					ENB_STRNCPY(ptrAQT_t->FileType , str, FILE_TYPE_SIZE-1);
			}
			str = strtok(NULL , "\n"); //  For URL
			TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str,  __FUNCTION__);
			str = strtok(NULL , "\n"); // For Username
			TRACE (LOG_CRITICAL , RPC_HANDLER, "username %s  Entering %s  ",str,  __FUNCTION__);
			str = strtok(NULL , "\n"); // For Password
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Pass %s  Entering %s  ",str , __FUNCTION__);
			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				delay_sec = strtoul(str , NULL, 0);
			}
			str = strtok(NULL , "\n"); // For Local Path
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Pass %s  Entering %s  ",str , __FUNCTION__);
			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				time(&currtime);
				t1 = strtol(str, NULL,10);
				diff =  currtime - t1;
				if (diff < delay_sec)
					ptrAQT_t->State = cwmp__TransferStateType__1;
				else
					ptrAQT_t->State = cwmp__TransferStateType__2;
			}

			DEALLOCATE(tmpBuffer);
			memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
			ptrAQT_t->TargetFileName = (Char8 *)ALLOCATE(TRG_NAME_SIZE );
			if(NULL ==  ptrAQT_t->TargetFileName)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				exit(0);			
			}
			//ptrAQT_t->State = cwmp__TransferStateType__1;
			ptrAQT_t->IsDownload = xsd__boolean__false_;
			arrAQT_t[ind_list-1] = ptrAQT_t;
			ind_list++;
			//	ptrAQT_t = NULL;
		}
		else if (strncmp(entry->d_name , "schdwn", 3 ) == 0)
		{
			ENB_STRNCPY(strPath , DW_DIR, 11);
			strncat(strPath, "/", 2);
			strncat(strPath ,entry->d_name, (MAX_DW_LOG_NAME_LEN-13));

          /* SPR 17268 FIXED START */
             UInt8 ABSPath3[CONFIG_PARAM_LEN] = {'\0'};
             strcpy((char *)ABSPath3,(char *)ABSPath);

            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath before concat %s ",ABSPath3 );
            strncat((char *)ABSPath3 , strPath , strlen(strPath));
            TRACE ( LOG_CRITICAL , RPC_HANDLER , "ABSPath after concat %s ",ABSPath3 );
            dw_fp = fopen((char *)ABSPath3, "r");
            /* SPR 17268 FIXED END */

		//	dw_fp = fopen(strPath, "r");
			if(NULL ==  dw_fp)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: In file open");
				DEALLOCATE(res_TransferList_t);
				closedir(dp);
                DEALLOCATE(arrAQT_t);	
				return NULL;
			}
			arrAQT_t = (struct cwmp__AllQueuedTransferStruct **)TR_REALLOC(arrAQT_t ,
					sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list);
			if(NULL ==  arrAQT_t)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				ENB_FCLOSE(dw_fp);
				DEALLOCATE(res_TransferList_t);
				closedir(dp);
				return NULL;
			}
			
			memset(arrAQT_t ,'\0', sizeof(struct cwmp__AllQueuedTransferStruct *)*ind_list );

			tmpBuffer = (Char8 *)ALLOCATE( 1024 );
			if (tmpBuffer == NULL)
			{
                    fclose(dw_fp);
				continue;
			}
			memset(tmpBuffer, '\0' , 1024);

			ret = fread((void *)tmpBuffer,1024, 1, dw_fp);
			if (ret != 0)
                {
                    fclose(dw_fp);
                    continue;
                }
			tmpBuffer[1023] = '\0';

            ENB_FCLOSE(dw_fp);
			ptrAQT_t = (struct cwmp__AllQueuedTransferStruct *)ALLOCATE(
					sizeof(struct cwmp__AllQueuedTransferStruct));
			if (NULL == ptrAQT_t)
				continue;

			memset(ptrAQT_t ,'\0', sizeof(struct cwmp__AllQueuedTransferStruct ));
			
			ptrAQT_t->CommandKey = (Char8 *)ALLOCATE(COMMAND_KEY_SIZE );
			if(!ptrAQT_t->CommandKey)
				continue;

			memset(ptrAQT_t->CommandKey,'\0' , COMMAND_KEY_SIZE );
			str = strtok(tmpBuffer , "\n");
			if (str != NULL)
			{

				if (*str == '-')
					ENB_STRNCPY(ptrAQT_t->CommandKey , "", 2);
				else
					ENB_STRNCPY(ptrAQT_t->CommandKey , str, COMMAND_KEY_SIZE);
			}

			ptrAQT_t->FileType  = (Char8 *)ALLOCATE(FILE_TYPE_SIZE );
			if(NULL ==  ptrAQT_t->FileType)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
				DEALLOCATE(arrAQT_t);
				DEALLOCATE(tmpBuffer);
				DEALLOCATE(ptrAQT_t->CommandKey);
				DEALLOCATE(ptrAQT_t);
				DEALLOCATE(res_TransferList_t);
				closedir(dp);
				return NULL;
			}
			
			memset(ptrAQT_t->FileType ,'\0', FILE_TYPE_SIZE);
			str = strtok(NULL , "\n");
			if (str != NULL)
			{


				if (*str == '-')
					ENB_STRNCPY(ptrAQT_t->FileType , "", 2);
				else
					ENB_STRNCPY(ptrAQT_t->FileType , str, FILE_TYPE_SIZE-1);
			}
			str = strtok(NULL , "\n"); //  For URL
			TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str ,  __FUNCTION__);
			str = strtok(NULL , "\n"); // For Username
			TRACE (LOG_CRITICAL , RPC_HANDLER, "username %s  Entering %s  ",str , __FUNCTION__);
			str = strtok(NULL , "\n"); // For Password
			TRACE (LOG_CRITICAL , RPC_HANDLER, "Pass %s  Entering %s  ",str,  __FUNCTION__);
			str = strtok(NULL , "\n"); //  For file  size
			TRACE (LOG_CRITICAL , RPC_HANDLER, "File size %s  Entering %s  ",str ,  __FUNCTION__);
			str = strtok(NULL , "\n"); // For Target Name 
			TRACE (LOG_CRITICAL , RPC_HANDLER, "target name %s  Entering %s  ",str ,  __FUNCTION__);
			str = strtok(NULL , "\n"); //    for list size
			if (str != NULL)
			{
				l_size = atoi(str);
			}

			if (l_size > 0)
			{
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					timeWin_1_start = strtoul(str , NULL, 0);
				}
				str = strtok(NULL , "\n");
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str ,  __FUNCTION__);

			}       
			if (l_size > 1)
			{
				str = strtok(NULL , "\n");
				if (str != NULL)
				{
					timeWin_2_start = strtoul(str , NULL, 0);

				}
				str = strtok(NULL , "\n");
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str,  __FUNCTION__);
			}
			else
			{
				str = strtok(NULL , "\n"); // Time window second
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str,  __FUNCTION__);
				str = strtok(NULL , "\n");
				TRACE (LOG_CRITICAL , RPC_HANDLER, "URl %s  Entering %s  ",str,  __FUNCTION__);
			}

			str = strtok(NULL , "\n");
			if (str != NULL)
			{
				t1 = strtol(str, NULL,10);
				time(&currtime);
				if ((currtime - t1) >= timeWin_1_start) 
				{
					ptrAQT_t->State = cwmp__TransferStateType__2;
				}
				else
				{
					if (timeWin_2_start > 0 && (currtime - t1) >= timeWin_2_start)
					{
						ptrAQT_t->State = cwmp__TransferStateType__2;    
					}
					else
						ptrAQT_t->State = cwmp__TransferStateType__1;
				}   
			}
			DEALLOCATE(tmpBuffer);
			memset(strPath , '\0' , MAX_DW_LOG_NAME_LEN);
			ptrAQT_t->TargetFileName = (Char8 *)ALLOCATE(TRG_NAME_SIZE );
			if(NULL ==  ptrAQT_t->TargetFileName)
			{
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Error: Memory allocation failure");
                /* Coverity 40602 + */
				DEALLOCATE(arrAQT_t);
                /* Coverity 40602 - */
				DEALLOCATE(ptrAQT_t->FileType);
				DEALLOCATE(ptrAQT_t->CommandKey);
				DEALLOCATE(ptrAQT_t);
				DEALLOCATE(res_TransferList_t);
				closedir(dp);
				return NULL;
			}
			ptrAQT_t->IsDownload = xsd__boolean__true_;
			arrAQT_t[ind_list-1] = ptrAQT_t;
			ind_list++;
			//ptrAQT_t = NULL;
		}

	}
	closedir(dp);

	if(arrAQT_t)
	{	
	res_TransferList_t->__ptrAllQueuedTransferStruct = arrAQT_t;
	res_TransferList_t->__size = ind_list - 1;
	}else
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "I exited in fun %s  ",__FUNCTION__);
		exit(0);
	}

	TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);

    /*  coverity 86504 fix start */
    /* code removed */
    /*  coverity 86504 fix end */

	return res_TransferList_t;


}
