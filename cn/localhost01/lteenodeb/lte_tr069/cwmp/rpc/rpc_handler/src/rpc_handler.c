/********************************************************************************
*
*       FILE NAME   : 	rpc_handler.c
*
*       DESCRIPTION : 	This file contains the function for all rpc functionalities
*				like setParameter, getparameter Values and download.						
*
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 16, 2012   TR-069 Amendment 4   Initial Creation
*       Shivani Pahuja March 25 2013                       Removed Warnings
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/


#include <rpc_handler.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <connection_manager.h>
#include <logger.h>
#include <curl/curl.h>
#include <map_glue.h>
#include <client_req_manager.h>
#include <signal.h>
#include <common_def.h>

/* Spr 16783 Fix Start */	
#include <soapH.h>
/* Spr 16783 Fix End */	

/*SPR 16652 Fixed Start */

#define CONN_MANAGER_DW "Connection Manager"
#define MAX_URL_SIZE 256

/*SPR 16652 Fixed End */


/******************************************************************************
  Macros and Inline Functions 
******************************************************************************/
  
/******************************************************************************
  Function Declarations
******************************************************************************/
extern UInt8* ipr_cwmp_acs_discovery (UInt8);

extern UInt8  check_duplicates_spv_flag;

/*download upload change starts*/
extern SInt32 queue_download_max_size;
/*download upload change ends*/

/* SPR 19821 FIXED START*/
/* code remove */
/* SPR 19821 FIXED END*/
   /* SPR 20754 FIXED START */
   /* SPR 20754 FIXED END */
/* SPR 17268 FIXED START */
#define CONFIG_PARAM_LEN 256
extern UInt32 ABS_len;
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */

#define FREE_SETPARAM(a) \
	SInt32 i = 0 ;  	\
	SInt32 s = 0 ;	\
	ParameterValueStruct ** b = NULL; \
	if (a->ParameterList != NULL) \
	{ \
	b = a->ParameterList->__ptrParameterValueStruct; \
	s = a->ParameterList->__size ; \
} \
while ( i <  s) 	\
{			\
	DEALLOCATE (b[i]->Name);  \
	DEALLOCATE (b[i]->Value); \
	DEALLOCATE (b[i]);	\
		i++;   \
} \
DEALLOCATE(b); \
if (a != NULL) \
{\
	DEALLOCATE (a->ParameterList); \
	DEALLOCATE (a);\
}

#define FREE_DOWNLOAD(a)     \
	DEALLOCATE(a->CommandKey);           \
DEALLOCATE(a->FileType);           \
DEALLOCATE(a->URL);           \
DEALLOCATE(a->Username);           \
DEALLOCATE(a->Password);           \
DEALLOCATE(a->TargetFileName);           \
DEALLOCATE(a->SuccessURL);           \
DEALLOCATE(a->FailureURL);           \


/* Inline function to free the memory  for SetParameterValues */

inline void free_setparam (SetParameterValues *a)
{
	/* SPR #16448 Fix Start */
	SInt32 i = 0 ;
	SInt32 s = 0 ;
	if (a == NULL || a->ParameterList == NULL)
		return;
	s = a->ParameterList->__size ;
	ParameterValueStruct ** b = NULL;
	b = a->ParameterList->__ptrParameterValueStruct;
	if (b == NULL)
		return;

	while (i < s)
	{

		if(b[i]->Name != NULL)
			DEALLOCATE(b[i]->Name);
		if(b[i]->Value != NULL)
			DEALLOCATE(b[i]->Value);

		DEALLOCATE(b[i]);
		i++;
	}
	DEALLOCATE(b);
	DEALLOCATE(a->ParameterList);
	DEALLOCATE(a);
}

/* SPR #16448 Fix End */


/* Inline function to free the memory  for parameter List struct */

inline void free_param_cpe (ParameterList *a)
{
    SInt32 i = 0 ;
    SInt32 s = 0 ;

	if (NULL == a)
		return;

	ParameterValueStruct ** b = NULL;
	b = a->__ptrParameterValueStruct;

	s = a->__size ;
	s--;

	if (NULL == b)
	return;

    while ( s >= i)
    {
           /* SPR 16448 Fix Start */
		DEALLOCATE (b[s]->Name);
		DEALLOCATE (b[s]->Value);
		DEALLOCATE (b[s]);
           /* SPR 16448 Fix End */
         
        s--;
    }
	DEALLOCATE (a);
    /*  coverity 86514 fix start */
    /* code removed */
    /*  coverity 86514 fix end */
}

/* Inline function to free the memory  for Parameter Names Struct */

inline void free_getparam (ParameterNames *pNames)
{
    SInt32 s = 0 ;
    Uint8 **ptr = NULL;

	if (NULL == pNames)
	 return;

    s = pNames->__size ;
	ptr = pNames->__ptrstring;
	s -= 1;

    while ( s >= 0)
    {
        /* Spr 14309 FIx Start */
		DEALLOCATE(ptr[s]);
        /* Spr 14309 FIx End */
        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
        s--;
    }
    /* SPR 22314 Start */
    DEALLOCATE(pNames->__ptrstring);
    DEALLOCATE(pNames);
    /* SPR 22314 End */
    /*  coverity 86511 fix start */
    /* code removed */
    /*  coverity 86511 fix end */
}


/******************************************************************************
 Global Variables
******************************************************************************/

//Used  for thread creation in Download RPC */
pthread_t g_rpc_Download;

YLIST g_Txn_List_Download;
U8 g_rpc_IsDwnQ_init = 0;


/******************************************************************************
 Function Declarations 
******************************************************************************/
/* SPR 17596 fixed start */
GetRPC_t * ipr_cwmp_GetRPCMethods_rpc();
/* SPR 17596 fixed end */
GetStatus * ipr_cwmp_getParameterValues(ParameterNames *);
SInt32 ipr_cwmp_startRPCDownLoad(STDownload *p_Download_t );
StStatus* ipr_cwmp_setParameterValues(ParameterList *);
ParameterList * ipr_cwmp_getAllList(void);
inline void ipr_cwmp_assign_Fault_9007(struct _cwmp__Fault_SetParameterValuesFault *, 
							   SInt32 *iFaultSize, Uint8 *Name);
SInt32  ipr_cwmp_ManagementServerModuleGet(ParameterList   **);
extern int connManagerDownload (STDownload *pstDownload);

STFault * check_duplicates_in_set_parameter_values(struct ParameterValueList * List);


/******************************************************************************
 Function Definitions
******************************************************************************/


/***************************************************************************************
*       Function Name : rpc_handler_setParameterValues 
*
*       Description   : This function is a method to perform functionality of	 
*                       rpc method i.e setparameter , it takes the input as parameter 
*			list of name and value. and populates the internal structure 
*			for all CPE parameters.  and pass these structure to management 	 
*			module to set the paramaters. and give the list to glue layer  
*			to perform the set parameter functionality. ( through callback)	
*
*	Arguments     : ParameterList , list of (name , Value)  	
*
*       Returns       : StStatus * (pointer tio StStatus structure) 
*       		 RPC_SUCCESS :  if successfully peformed 
*       		 RPC_FAULT   :	if Not able to set due to some error in inputs  	
*       		 Fault structre filled if failed to perform . (Per parameter basis)
*
****************************************************************************************/

StStatus* ipr_cwmp_setParameterValues (ParameterList *pstParamList)
{
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__); 

    /* SPR 17880 START */
    /* Code removed : removed variable ret*/
    /* SPR 17880 END */
    SInt32 ret1 = 0;
    SInt32 iSize  = 0; 	
    SInt32 iGlueSize = 0 ;	
    SInt32 iCount = 0 ; 
    Uint8 *cTmpName = NULL ;  
    Uint8 *cTmpValue =  NULL;
    ParameterValueStruct **stParamVal = NULL;
    SetParameterValues *stSetParamVal = NULL;
    SetParameterValues *stSetParamVal1 ;
    stSetParamVal1 = NULL;
    StStatus* stRetStatus = NULL;
    StStatus* stGlueRet = NULL;
    SInt32 iTmp1 = 1 ;
    /* SPR 17880 START */
    /* Code removed : removed variable iTmp, stTmpParam*/
    /* SPR 17880 END */
    ParameterValueStruct *stTmpParam1 = NULL;
    ParameterValueStruct **stTmpParamArr = NULL;
    ParameterValueStruct **stTmpParam1Arr1 = NULL;
    struct _cwmp__Fault_SetParameterValuesFault *stTmpArr = NULL;
    struct _cwmp__Fault_SetParameterValuesFault *stTmpArr1 = NULL;
    STFault *pstFault = NULL; 

    if (NULL == pstParamList)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL Parameter List....");  
        return NULL; 
    }	
    stParamVal = (ParameterValueStruct **)pstParamList->__ptrParameterValueStruct;
    if (NULL == stParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL Parameter List....");
        return NULL;
    }
    if (check_duplicates_spv_flag==1)
    {
        if ((pstFault = check_duplicates_in_set_parameter_values(pstParamList)) != NULL)
        {
            /* Duplicate parameters found in parameter list, returning fault 9003 */
            /* Coverity 40626 + */
            stRetStatus = (StStatus *) ALLOCATE(sizeof(StStatus));
            /* Coverity 40626 - */
            if (NULL == stRetStatus) 
            {
                TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
                /*  coverity 56064 fix start */
                DEALLOCATE(pstFault);
                /*  coverity 56064 fix end */
                return NULL;
            }
            memset(stRetStatus , '\0' , sizeof(StStatus) );
            stRetStatus->eStatus = RPC_FAULT;
            stRetStatus->stFault = pstFault; 
            return stRetStatus; 
        }
    }
    /* SPR #16448 Fix Start */
    stSetParamVal = (SetParameterValues *)ALLOCATE( sizeof(SetParameterValues));
    if (NULL == stSetParamVal)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        return NULL;
    }
    stSetParamVal1 = (SetParameterValues *)ALLOCATE( sizeof(SetParameterValues));
    if (NULL == stSetParamVal1)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        DEALLOCATE(stSetParamVal);
        return NULL;
    }
    stSetParamVal->ParameterList = (ParameterList *) ALLOCATE(sizeof(ParameterList));
    if (NULL == stSetParamVal->ParameterList)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        DEALLOCATE(stSetParamVal);
        DEALLOCATE(stSetParamVal1);
        return NULL;
    }
    /* SPR 17880 START */
    /* Code removed: removed variable ret */
    /* SPR 17880 START */
    stSetParamVal1->ParameterList = (ParameterList *) ALLOCATE(sizeof(ParameterList));
    if (NULL == stSetParamVal1->ParameterList)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        DEALLOCATE(stSetParamVal->ParameterList);
        DEALLOCATE(stSetParamVal);
        DEALLOCATE(stSetParamVal1);
        return NULL;
    }
    /* SPR #16448 Fix End */

    ret1 = 1;

    iSize = pstParamList->__size;

    // Iterate the parameter List
    while (iCount < iSize)
    {
        cTmpName =  stParamVal[iCount]->Name;
        cTmpValue = (Uint8 *)stParamVal[iCount]->Value;
        if (cTmpName != NULL)
        {
            /* SPR 17880 START */
            stTmpParam1Arr1 = (ParameterValueStruct **) TR_REALLOC( stTmpParam1Arr1 , 
                    (sizeof(ParameterValueStruct*) * iTmp1) );
            /* SPR #16448 Fix Start*/
            stTmpParam1 = (ParameterValueStruct *)ALLOCATE( sizeof(ParameterValueStruct));
            /* SPR #16448 Fix End*/

            if (stTmpParam1Arr1!= NULL && stTmpParam1 != NULL)
            {
                /* SPR #16448 Fix Start*/
                stTmpParam1->Name = (Uint8 *) ALLOCATE( MAX_PARAM_NAME_SIZE);
                stTmpParam1->Value = (Uint8 *) ALLOCATE( MAX_PARAM_VAL_SIZE);
                /* SPR #16448 Fix End*/

					if ((stTmpParam1->Name != NULL) && (stTmpParam1->Value != NULL))
					{
						strncpy((Uint8 *)stTmpParam1->Name, cTmpName, MAX_PARAM_NAME_SIZE);
						if (cTmpValue != NULL)
							strncpy((Uint8 *)stTmpParam1->Value , cTmpValue, MAX_PARAM_VAL_SIZE);
					}
					else
						ret1 = 0;

                stTmpParam1Arr1[iTmp1-1] = stTmpParam1;
                iTmp1++;
            }
            /* Coverity 40588 + */
            else if((NULL == stTmpParam1Arr1) && (NULL != stTmpParam1))
            {
                DEALLOCATE(stTmpParam1);
            }    
            /* Coverity 40588 - */
            /* SPR 17880 START/END Code removed  */
        } //Outer If		
        iCount++;
    } //while 

    /* Populating in Internal strcutre for string Parameter List */

    /* SPR 17880 START/END  Code removed */
    /* SPR #16448 Fix Start*/
    stRetStatus =  (StStatus *) ALLOCATE( sizeof(StStatus));
    /* SPR #16448 Fix Start*/

    if (NULL == stRetStatus) 
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        /* SPR #16448 Fix Start*/
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
        DEALLOCATE(stTmpParamArr);
        DEALLOCATE(stSetParamVal1->ParameterList);
        DEALLOCATE(stSetParamVal1);
        DEALLOCATE(stSetParamVal->ParameterList);
        DEALLOCATE(stSetParamVal);
        /* SPR #16448 Fix End*/

        return NULL;
    }
    /* SPR 17880 START/END  Code Removed */

    //Calling Glue Layer to perform the functionilty with Input of Parameter List 

    if (ret1 && stSetParamVal1->ParameterList != NULL  && (iTmp1 > 1))
    {

        stSetParamVal1->ParameterList->__size = iTmp1-1;
        stSetParamVal1->ParameterList->__ptrParameterValueStruct = (ParameterValueStruct **)stTmpParam1Arr1;

        /* Calling management Module Server module to set the Parameters */
        TRACE (LOG_CRITICAL , RPC_HANDLER, "\n\t\tParameter List for Glue Layer is");
        stub_print_param_list(stSetParamVal1->ParameterList);

        //if (stCallbackFunList.invokeCallback)
        stGlueRet = (StStatus *) stCallbackFunList.invokeCallback((void   *) stSetParamVal1->ParameterList, 
                RPC_SETPARAMETERVALUES, NULL);	
        if (stGlueRet && stGlueRet->stFault)
        {
            if((stRetStatus && stRetStatus->eStatus == RPC_FAULT)  && ( stGlueRet->eStatus == RPC_FAULT))
            {
                /*Merging of Fault codes  from glue layer and CPE */
                if (stRetStatus->stFault)
                {
                    iSize = stRetStatus->stFault->__sizeSetParameterValuesFault;	
                    iGlueSize =  stGlueRet->stFault->__sizeSetParameterValuesFault;
                    stRetStatus->stFault->__sizeSetParameterValuesFault = iSize; 
                    stTmpArr = stRetStatus->stFault->SetParameterValuesFault;	
                    stTmpArr1= stGlueRet->stFault->SetParameterValuesFault;
                    stTmpArr = TR_REALLOC(stTmpArr , 
                            sizeof (struct _cwmp__Fault_SetParameterValuesFault) * (iSize+iGlueSize));
                    if (stTmpArr == NULL)
                    {
                        /* SPR #16448 Fix Start*/
                        DEALLOCATE(stTmpParamArr);
                        DEALLOCATE(stSetParamVal1->ParameterList);
                        DEALLOCATE(stSetParamVal1);
                        DEALLOCATE(stSetParamVal->ParameterList);
                        DEALLOCATE(stSetParamVal);
                        DEALLOCATE(stRetStatus);
                        return NULL;
                        /* SPR #16448 Fix End*/
                    }	
                    iCount = 0 ;
                    while (iCount < iGlueSize)
                    {
                        stTmpArr[iSize+iCount]  = stTmpArr1[iCount];	 
                        iCount++;	
                    }
                    stRetStatus->stFault->SetParameterValuesFault = stTmpArr;
                    stRetStatus->stFault->__sizeSetParameterValuesFault = (iSize + iGlueSize);
                }
            }
            else if ((stRetStatus && stRetStatus->eStatus == RPC_SUCCESS) && ( stGlueRet->eStatus == RPC_FAULT))
            {
                stRetStatus->eStatus = RPC_FAULT;
                stRetStatus->stFault = stGlueRet->stFault;
            }
        }
        /* Raveen Changes Start */
        else if(stGlueRet && stGlueRet->eStatus == RPC_PARTIAL)
        {
            stRetStatus->eStatus = RPC_PARTIAL; 
        }
        /* SPR 22314 Start */
        else if(stGlueRet && stGlueRet->eStatus == RPC_SUCCESS)
        {
            DEALLOCATE(stGlueRet);
        }
        /* SPR 22314 End */
        /* SPR 17273 Fix Start */
        else if(!stGlueRet)
        {
            pstFault = (STFault *) ALLOCATE ( sizeof(STFault));
            memset(pstFault , '\0' , sizeof(STFault) );
            if (pstFault)
            {
                stTmpArr = ALLOCATE(sizeof (struct _cwmp__Fault_SetParameterValuesFault)); 

                stTmpArr->ParameterName = (Uint8 *) ALLOCATE(MAX_PARAM_NAME_SIZE) ;
                if (stTmpArr->ParameterName == NULL)
                {
                    TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure ");
                    exit(0);
                }
                stTmpArr->FaultCode = (Uint8 *) ALLOCATE(FAULT_CODE_SIZE) ;
                if (stTmpArr->FaultCode == NULL)
                {
                    TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure ");
                    exit(0);
                }
                stTmpArr->FaultString = (Uint8 *) ALLOCATE(FAULT_STRING_SIZE);
                if (stTmpArr->FaultString == NULL)
                {
                    TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure ");
                    exit(0);
                }

                strncpy(stTmpArr->ParameterName ,"", MAX_PARAM_NAME_SIZE) ;
                strncpy(stTmpArr->FaultCode , FAULT_9003, FAULT_CODE_SIZE);
                strncpy(stTmpArr->FaultString , FAULT_9003_STR, FAULT_STRING_SIZE);

                pstFault->SetParameterValuesFault = stTmpArr;
                pstFault->__sizeSetParameterValuesFault = 1;

                stRetStatus->eStatus = RPC_FAULT;   
                stRetStatus->stFault = pstFault;
            }
        }	
        /* SPR 17273 Fix End */
        /* Raveen Changes End */
    }
    /* SPR 16448 Fix Start */
    if(stSetParamVal1)
        free_setparam(stSetParamVal1);
    /* SPR 16448 Fix End */
    /*  Free The Resource used in ManagementServer Module */
    /* SPR #16448 Fix Start */
    /* Memory leak 20 jan fixed start */

    if(stSetParamVal->ParameterList != NULL  || stSetParamVal != NULL )
    {   
        DEALLOCATE(stSetParamVal->ParameterList);
        DEALLOCATE(stSetParamVal);
    }
    /* Memory leak 20 jan fixed end */

    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s ",__FUNCTION__);

    return stRetStatus;
}


/* SPR 17596 fixed start */
/***************************************************************************************
* Function Name       : ipr_cwmp_GetRPCMethods_rpc
* Description         : This function is a method to perform functionality of
*                       rpc method i.e get rpc method , and  through callback
                        it returns the parameter List and we return the combined List to ACS.  
* Arguments           : none 
* Returns             :  MethodList  
****************************************************************************************/

GetRPC_t* ipr_cwmp_GetRPCMethods_rpc()
{                       
    TRACE(LOG_CRITICAL , RPC_HANDLER , "In %s function", __FUNCTION__);
    
    GetRPC_t *mlist = NULL; 


    mlist = (GetRPC_t *) stCallbackFunList.invokeCallback(NULL,
                RPC_GETRPCMETHOD , NULL);
       
    TRACE(LOG_CRITICAL , RPC_HANDLER ,"mlist = %p" , mlist);
    if(mlist == NULL)
    {               
        TRACE(LOG_CRITICAL , RPC_HANDLER , "ERROR : InvokeCallBack Failed...");
    }
    TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);                        
    return mlist;   
}           
/* SPR 17596 fixed end */

/***************************************************************************************
*       Function Name : rpc_handler_getParameterValues
*
*       Description   : This function is a method to perform functionality of
*                       rpc method i.e get parameter , it takes the input as list of 
*                       Parameter names  and populates the internal structure
*                       for all CPE parameters.  and pass these structure to management
*                       module to get the paramaters List . and give the list of name to 
*                       glue Layer too to perform the get parameter functionality.( through callback)
			and it returns the parameter List and we return the combined List to ACS.  
*
*       Arguments     : ParameterNames , list of (Names) 
*
*       Returns       :  ParameterList * 
*                        RPC_SUCCESS :  if successfully peformed
*                        RPC_FAULT   :  if Not able to get due to some error in inputs
*			 if RPC_SUCCESS  than ParameterList	
****************************************************************************************/
GetStatus * ipr_cwmp_getParameterValues (ParameterNames *pNames)
{
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering in %s ",__FUNCTION__);
    /* SPR 22314 Start */
	/* Code Removed */
    /* SPR 22314 End */
	ParameterList *combParamList 	= NULL;
    /* SPR 17880 START */
    /* Code removed : removed variable ret*/
    /* SPR 17880 END */

	SInt32 iSize  = 0;
	SInt32 iCount = 0;
	SInt32 iCPESize = 0;
	SInt32 iComb = 0 ;
	Uint8 *cTmpName = NULL ;
	Uint8 *cCPEName = NULL;
	ParameterNames *newParamName = NULL;
    /* SPR 22314 Start */
	/* Code Removed */
    /* SPR 22314 End */
	ParameterValueStruct **stcombParamArr= NULL;
	ParameterValueStruct **stglueParam 	 = NULL;
    
	/* SPR 17880 START */
    /* Code removed : removed variable */
    /* SPR 17880 END */

	GetStatus *res = NULL;
	GetStatus *res_glue = NULL;
    /* SPR 17880 START */
    /* Code removed : removed variable ret*/
    /* SPR 17880 END */


	//SInt32 iRes = 0 ; //s


	Uint8 **arrTmpNames = NULL;
	Uint8 **arrArgNames = NULL;
	SInt32 iTmp = 0;	
	SInt32 ret = 0;
	SInt32 iCPE = 1 ;
	/*SPR 17440 Fixed Start */
	SInt32 i  = 0;
	/*SPR 17440 Fixed End */


	if (NULL == pNames || (NULL == pNames->__ptrstring))
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL Parameter Names List....");
		return NULL;
	}

	iSize = pNames->__size;

	/*  Create new array of parameter names which would be merged one */

	res = (GetStatus *) ALLOCATE(sizeof(GetStatus) );
	if (NULL == res) 
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
		return NULL;
	}

	memset(res , '\0', sizeof(GetStatus));

	/*SPR 17440 Fixed Start */

	for(i = 0 ;i < iSize ; i++){

		if(strlen(pNames->__ptrstring[i]) > MAX_PARAM_NAME_SIZE){

			TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : getparanmeter size is greterthan 256....");

			res->eStatus = RPC_FAULT;
			res->fault_code = 9005;


			return res;
		}
	}

	/*SPR 17440 Fixed End */


    /* SPR 22314 Start */
	/* Code Removed */
    /* SPR 22314 End */
	combParamList = (ParameterList *) ALLOCATE(sizeof(ParameterList) );
	if (NULL == combParamList) 
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
		DEALLOCATE(res);
        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
		return NULL;
	}
	newParamName = (ParameterNames * ) ALLOCATE(sizeof (ParameterNames) ); 
	if (NULL == newParamName) 
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
		DEALLOCATE(combParamList);
		DEALLOCATE(res);
        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
		return NULL;
	}

	if (newParamName != NULL)
	{
		ret = 1;
		newParamName->__ptrstring = (Uint8 **) ALLOCATE(( iSize * sizeof(Uint8 *)));
		if (NULL == newParamName->__ptrstring) 
		{
			TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
			DEALLOCATE(newParamName);
			DEALLOCATE(combParamList);
			DEALLOCATE(res);
            /* SPR 22314 Start */
            /* Code Removed */
            /* SPR 22314 End */
			return NULL;
		}
	}

	/* Now  Iterate through the names and find the names for CPE only */

	arrArgNames = (Uint8 **)pNames->__ptrstring;

	if (ret)
		arrTmpNames = (Uint8 **)newParamName->__ptrstring;

	while (iCount < iSize)
	{

		cTmpName = arrArgNames[iCount];
		if (cTmpName != NULL)
		{
 		/* SPR 17880 START/END  Code Removed */ 

				cCPEName = (Uint8 *) ALLOCATE(( MAX_PARAM_NAME_SIZE + 50));  

				if (NULL == cCPEName)
					continue;
				strncpy(cCPEName, cTmpName, MAX_PARAM_NAME_SIZE + 50);	
				if(NULL == arrTmpNames)
                    /* SPR 22314 Start */
                {
                    TRACE (LOG_DETAIL , RPC_HANDLER, "arrTmpNames is NULL");
                    DEALLOCATE(cCPEName);
                    cCPEName = NULL;
                    /* SPR 22314 End */
					continue;	
                  /* SPR 22314 Start */
                }
                    /* SPR 22314 End */
				arrTmpNames[iTmp] = cCPEName;
				iTmp++;
				cCPEName = NULL;       
		//	}  //s
		}	
		iCount++;

	} //while ends 	

	if (ret)
	{	
		newParamName->__ptrstring = arrTmpNames;
		newParamName->__size = iTmp;
	}
/* SPR 17880 START/END Code Removed */            

	/* Now calling Glue layer callback mechanism */
	if ( iTmp>0 )	 
		res_glue = (GetStatus *) stCallbackFunList.invokeCallback((void *)newParamName, 
				RPC_GETPARAMETERVALUES , NULL);

	/*  Now merge the paramter values of CPE and glue Layer PArameter Name and Values pair in stParamList */
	if (res_glue && ret)
	{
		if (res_glue->eStatus != RPC_FAULT)
		{
			/* Combine the List of paramterlist */
			if (res_glue->parameterList != NULL)
			{
				iCPESize = (iCPE-1);
				iSize = res_glue->parameterList->__size + iCPESize;
				TRACE (LOG_CRITICAL , RPC_HANDLER, "Now list new size: %d",                                                                                           iSize);
                /* Spr 17463 Fix Start */
				stcombParamArr = ( ParameterValueStruct **) ALLOCATE((iSize * sizeof(ParameterValueStruct*)));
                /* Spr 17463 Fix End */
				if (stcombParamArr != NULL)
				{
					iCount = 0 ;
                    /* Coverity 99395 + */ /* SPR 17880 START */   
			/* Code Removed */ 
                    /* Coverity 99395 - */ /* SPR 17880 END */    
					iCount = 0 ;
					stglueParam = res_glue->parameterList->__ptrParameterValueStruct;
					while (iCount < res_glue->parameterList->__size)
					{
						stcombParamArr[iComb] =  stglueParam[iCount];
						iComb++;
						iCount++;
					}

					if (combParamList != NULL)
					{
						combParamList->__ptrParameterValueStruct = stcombParamArr;
						combParamList->__size = iComb;
						res->parameterList  = combParamList;
						res->eStatus = RPC_SUCCESS;
                        /* SPR 22314 Start */
                        /* Code Removed */
                        /* SPR 22314 End */
                        /* SPR 21563 FIXED START */
                  DEALLOCATE(res_glue->parameterList->__ptrParameterValueStruct);
                  DEALLOCATE(res_glue->parameterList);
                        /* SPR 21563 FIXED END */
                        /* Memory leak 21 jan fixed start */
						DEALLOCATE(res_glue);           
                        free_getparam(newParamName);                          
                        /* Memory leak 21 jan fixed end */  
						return res;
					}
				}
			}
		}
		else
		{
			res->eStatus = RPC_FAULT;
			res->parameterList = NULL;
			res->fault_code = res_glue->fault_code;
			DEALLOCATE(combParamList);
            /* SPR 22314 Start */
            /* Code Removed */
            free_getparam(newParamName);       
            /* SPR 22314 End */
			return res;
		}
	}


	/*      Free the memory used above for parameter list of CPE   */
	free_getparam(newParamName);
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);	
	DEALLOCATE(combParamList);
    /* SPR 14206 Code Removed */
     /* SPR 17880 START */
     /* SPR 17880 END */
    /* SPR 22314 Start */
    /* Code Removed */
    /* SPR 22314 End */



    /*  coverity 40586/40584 fix start */
      /* tr_11_dec fix start*/  
      /*tr_11_dec fix end */
    /*  coverity 40586/40584 fix end */

	return res;
}

/********************************************************************************
*       Function Name : actualDownload 
*
*       Description   : This is the actual Thread function to perform download 
*                       , It interacts with Connection manager to perform download 
*
*       Args 	      : void *   ( which is structure of STDownload * )	
*
*       Returns       : void  *
*
*********************************************************************************/
void * ipr_cwmp_actualDownload (void  * arg)
{
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__); 
    if(NULL == arg)
    {
	   TRACE (LOG_CRITICAL ,"RPC_HANDLER", "Arg NULL\n");
    }    

	STDownload *pstDownload = NULL;
	ClientRequest *sClientReq = NULL;
	DownloadStatus * sDwnStatus = NULL;
	Uint8 *FaultCode = NULL;
	Uint8 *FaultString = NULL;
	UInt32 list_size = 0;
	UInt8 indx =  0;
    /*SPR 19821 FIXED START */
	time_t currTime;

    /*SPR 19821 FIXED END */

   /* SPR 19951 FIXED START */
   char buf[CONFIG_PARAM_LEN] = {0};
   time_t t;
   UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
   struct tm *time_gm; 
   /* SPR 19951 FIXED END */
	short res = 0 ; 
	/*SPR 16652 Fixed Start*/
	struct FtpFile ftpfile;
	char * file_path = NULL;

	/*SPR 16652 Fixed End */
/* SPR 17268 FIXED START */
 UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
    strcpy((char *)ABSPath1,(char *)ABSPath);
    TRACE (LOG_CRITICAL ,RPC_HANDLER ,"ABSPath before concate of SOFTWARE_VERS %s ", ABSPath1);
    strncat((char *)ABSPath1,SOFTWARE_VERS, strlen(SOFTWARE_VERS));
 /* SPR 17268 FIXED END */

#ifdef VENDOR_SPECIFIC
	Uint8 cont = 1;
#endif

    /*SPR 19821 FIXED START */

	time_t t2 =0;
	time(&currTime);
    /*SPR 19821 FIXED END */


	list_size = ylCount(&g_Txn_List_Download);

	while (list_size >0)
	{
		indx = 0;

		while (indx < list_size)
		{
			ThreadDownload *tArg_dwn = NULL;
			tArg_dwn  = (ThreadDownload *) ylNth(&g_Txn_List_Download , indx);
			if (tArg_dwn != NULL)
			{
				/* coverity 55724 fix start */
				/* SPR 19821 FIXED START*/
				if ((currTime - tArg_dwn->recv_time) >= tArg_dwn->delay)
					/* SPR 19821 FIXED END */
					/* Coverity 40504 + */
				{
					/* Coverity 40504 - */
					pstDownload  = (STDownload *)tArg_dwn->pstDownload;
					if(NULL == pstDownload)
					{
						if(FaultCode)
                        {    
							DEALLOCATE(FaultCode);
                        }    
                        /* Coverity 94790 + */
                        if(file_path)
                        {    
							DEALLOCATE(file_path);
                        }   
                        /* Coverity 94790 - */
						return NULL;
					}
					TRACE (LOG_CRITICAL , RPC_HANDLER, "New Download Request Picked");
					tArg_dwn->Status =  RPC_FALSE;
#ifdef VENDOR_SPECIFIC
					if (pstDownload->FileType)
					{
						if (*(pstDownload->FileType) == FIRM_UPGRADE_FILE)
						{
							cont = 0;




                          /* SPR 17268 FIXED START */
                            FILE *fp = fopen( (char *)ABSPath1 , "w");

                            TRACE (LOG_CRITICAL ,RPC_HANDLER ,"SoFTWARE_VERS path  after concate is =  %s ", ABSPath1);
                            /*SPR 17268 FIXED END */

							if(fp)
							{
								Uint8 *buffer = NULL;
								buffer = (Uint8 *)ipr_cwmp_getSoftVers();        
								if (buffer)
								{
									ENB_FPRINTF (fp , "%s",buffer);
									fclose(fp);
								}else
									fclose(fp);
							}
						}
					}
#endif
					/*
					 *         Acutal download of file using connection manager
					 *
					 */

					if (tArg_dwn->cancelStatus == CANCEL_FALSE)
					{
						res = connManagerDownload(pstDownload);
                /* SPR 19951 FIXED START */
						TRACE (LOG_CRITICAL , RPC_HANDLER, "command key ACS=%s cont=%d\n", pstDownload->CommandKey, cont);
                /* SPR 19951 FIXED END */


                      /* SPR 19951 FIXED START */
                      /* SPR 16732 FIX START */
                      t = tArg_dwn->recv_time; 
                      time_gm = gmtime(&t);

                      /*  coverity 96644 fix start */
                      if(time_gm)
                          strftime(buf, sizeof buf, "%F %T", time_gm);
                      /*  coverity 96644 fix end */

                      TRACE (LOG_CRITICAL , RPC_HANDLER, "Download Start Time= %s",buf);
                      /* SPR 16732 FIX END */


                      /*SPR 17268 FIXED START */
                      strcpy((char *)ABSPath2,(char *)ABSPath);

                      TRACE (LOG_CRITICAL , RPC_HANDLER , "ABSPath before concate of UPD_DWN_CMDKEY %s ", ABSPath2);
                      strncat((char *)ABSPath2 , UPD_DWN_CMDKEY , strlen(UPD_DWN_CMDKEY));

                      TRACE (LOG_CRITICAL , RPC_HANDLER , "UPD_DWN_CMDKEY path after concate is =  %s ", UPD_DWN_CMDKEY);
                      FILE *fp = fopen((char *)ABSPath2 , "w");
                      //fp = fopen((char *)ABSPath2 , "w");
                      /* SPR 17268 FIXED END */
                      if(fp)
                      {
                          if (pstDownload->CommandKey)
                          {
                              /* SPR 16732 FIX START */
                              ENB_FPRINTF (fp , "CMDKEY=%s\n",pstDownload->CommandKey);
                          }
                          if(tArg_dwn->recv_time)
                          {
                              ENB_FPRINTF (fp , "DOWNLOAD_START_TIME=%s\n",buf);
                              /* SPR 16732 FIX END */
                          }
                          fclose(fp);
                      }

                      /* SPR 19951 FIXED END */



						if (res != RESULT_OK)
                        {
                            TRACE (LOG_CRITICAL , RPC_HANDLER, 
                                    "File Transfer Failed with return status :%d",res);

                            /*SPR 16652 Fixed Start*/
							file_path  = (char *)ALLOCATE( MAX_URL_SIZE);
                            if (NULL == file_path)
                            {
								TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "In %s , Memory not available");
                                return 0;
                            }
                            ftpfile.stream = NULL;

							/*Coverity 112783 Fix Start*/
							if(NULL ==  pstDownload->URL)
							{
								TRACE(LOG_CRITICAL , CONN_MANAGER_DW,"URL is null");
								/*Coverity 94790 Fix Start*/
								DEALLOCATE(file_path);
								/*Coverity 94790 Fix End*/
								return 0;
							}
							else
                            {
								/*Coverity 112783 Fix End*/

                                if (pstDownload->TargetFileName && strlen(pstDownload->TargetFileName)!=0)
                                    ftpfile.filename = pstDownload->TargetFileName;
                                else{
                                    int len = strlen( pstDownload->URL);
                                    ftpfile.filename =  pstDownload->URL;
                                    while( pstDownload->URL[len-1]!= '/')
                                    {
                                        len--;

                                    }

                                    ftpfile.filename = ftpfile.filename+len;
                                }
                                /*SPR 18294 FIXED START*/
                                UInt8 ABSPath5[CONFIG_PARAM_LEN] = {'\0'};
                                strcpy((char *)ABSPath5,(char *)ABSPath);
                                TRACE (LOG_CRITICAL ,RPC_HANDLER ,"ABSPath before concate of SOFTWARE_VERS %s ", ABSPath5);
                                strncat((char *)ABSPath5,CPE_FILE_DIR_NAME, strlen(CPE_FILE_DIR_NAME)); 
                                snprintf (file_path , MAX_URL_SIZE , "%s%s",ABSPath5,ftpfile.filename);
                                /*SPR 18294 FIXED END*/

                            }
                            if (file_path)
                            {
                                if(0 != remove(file_path))
                                {
                                    perror("file remove error");
                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Successful remove file ....");
									/*Coverity 94790 Fix Start*/
									DEALLOCATE(file_path);
									/*Coverity 94790 Fix End*/
                                    //      continue;
                                }
                                TRACE (LOG_CRITICAL , RPC_HANDLER, " Error remove file ....");
                            }
                            if(ftpfile.stream)
                            {
                                fclose(ftpfile.stream); /* close the local file */
                            }

                            /*SPR 16652 Fixed Start*/

							FaultCode = (Uint8 *)ALLOCATE(FAULT_CODE_SIZE );
                            if (NULL == FaultCode) 
                            {
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
                                /* Coverity 94790 */
								DEALLOCATE(file_path);
                                /*  coverity 96486 fix start */
                                /* code removed */
                                /*  coverity 96486 fix end */
                                /* Coverity 94790 */
                                return NULL;
                            }        
							FaultString = (Uint8 *)ALLOCATE(FAULT_STRING_SIZE );
                            if (NULL == FaultString) 
                            {
                                TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
								DEALLOCATE(FaultCode);
								/* Coverity 94790 start */
								DEALLOCATE(file_path);
								/* Coverity 96486 fix start */
								/* Coverity 96486 fix end */
								/* Coverity 94790 end */
                                return NULL;
                            }
                            /*SPR 16138  Fixed Start*/
                            if( strstr(pstDownload->URL, "@")!= NULL){

                                TRACE (LOG_CRITICAL , RPC_HANDLER, "Received URL :%s",pstDownload->URL);
                                strncpy(FaultCode, "9003", FAULT_CODE_SIZE);
                                strncpy(FaultString ,
                                        "File Transfer failed: Userifo persent for file transfer",
                                        FAULT_STRING_SIZE);
                            }
                            else{
                                /*SPR 16138  Fixed End*/

                                switch (res)
                                {   
                                    /*spr 18210 fix start */
                                    case CURLE_LOGIN_DENIED :
                                    case 530 :
                                    /*spr 18210 fix start */
                                    {
                                        strncpy(FaultCode, "9012", FAULT_CODE_SIZE);
                                        strncpy(FaultString ,
                                                "File Transfer failed: Server authentication failure",
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case CURLE_UNSUPPORTED_PROTOCOL :
                                    {
                                        strncpy(FaultCode, "9013", FAULT_CODE_SIZE);
                                        strncpy(FaultString ,
                                                "File Transfer failed: Unsupported protocol for file transfer",
                                                FAULT_STRING_SIZE);
                                    }

                                    break;
                                    case CURLE_COULDNT_CONNECT :
                                    {
                                        strncpy(FaultCode, "9015", FAULT_CODE_SIZE);
                                        strncpy(FaultString ,
                                                "File Transfer failed: unable to contact file server",
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case CURLE_REMOTE_FILE_NOT_FOUND :
                                    {
                                        strncpy(FaultCode, "9016", FAULT_CODE_SIZE);
                                        strncpy(FaultString , 
                                                "File Transfer failed: Unable to access file", FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case 405 :
                                    {
                                        strncpy(FaultCode, "9017", FAULT_CODE_SIZE);
                                        strncpy(FaultString , 
                                                "File Transfer failed: Unable to complete download", 
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case 402 :
                                    {
                                        strncpy(FaultCode, "9018", FAULT_CODE_SIZE);
                                        strncpy(FaultString , 
                                                "File Transfer failed: File corrupted", FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case 403 :
                                    {
                                        strncpy(FaultCode, "9019", FAULT_CODE_SIZE);
                                        strncpy(FaultString , 
                                                "File Transfer failed: File authentication failure", 
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                    case 404 :
                                    {
                                        strncpy(FaultCode, "9020", FAULT_CODE_SIZE);
                                        strncpy(FaultString ,
                                                "File Transfer failed: Unable to complete download within specified time windows",
                                                FAULT_STRING_SIZE);
                                    }
                                    break;
                                    default :
                                    {
                                        strncpy(FaultCode, "9010", FAULT_CODE_SIZE);
                                        strncpy(FaultString , "File Transfer failed", FAULT_STRING_SIZE);
                                    }
                                    break;
                                }

                                /*SPR 16138  Fixed Start */
                            }
                            /*SPR 16138  Fixed End */

#ifdef VENDOR_SPECIFIC
                            cont = 1;
                            /* SPR 17268 FIXED START */

                            /* Coverity 40722 start */
                            if( 0 != remove((char *)ABSPath1))
                            {
                                perror("file remove error");
                            }
                            /* Coverity 40722 end */
                            /* SPR 17268 FIXED START */
                            //	continue;
#endif
                        }
						else
						{
							/*   Remove Queued Log file for Download */
							if (tArg_dwn->dw_logfilename)
							{
								if(0 != remove(tArg_dwn->dw_logfilename))
                                {
                                    perror("file remove error");
                                    //		continue;
                            		}
							}
							/* Callback to Glue Layer to inform glue layer that Downlaod is performed  */

							sDwnStatus = (DownloadStatus *)stCallbackFunList.invokeCallback((void *)pstDownload ,
									RPC_DOWNLOAD,NULL);

							FaultCode = (Uint8 *)ALLOCATE(FAULT_CODE_SIZE );
							if (NULL == FaultCode) 
							{
								TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
								exit(1);
							}        
							FaultString = (Uint8 *)ALLOCATE(FAULT_STRING_SIZE );
							if (NULL == FaultString) 
							{
								TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
								DEALLOCATE(FaultCode);                            
								exit(1);
							}        

							if (sDwnStatus && FAULT == sDwnStatus->eStatus)
							{
								strncpy(FaultString , sDwnStatus->fault_string, FAULT_STRING_SIZE);
								snprintf(FaultCode, FAULT_CODE_SIZE, "%d", sDwnStatus->fault_code);
#ifdef VENDOR_SPECIFIC
								cont = 1;
                            /* Coverity 40722 start */
                                /* SPR 17268 FIXED START */
                            if( 0 != remove((char *)ABSPath1))
                            {
                                perror("file remove error");
								TRACE (LOG_CRITICAL , RPC_HANDLER, "error in removing file");
                            }


                        /* SPR 19951 FIXED START */
                        TRACE (LOG_CRITICAL , RPC_HANDLER, "fault received from oam and command key ACS=%s\n", pstDownload->CommandKey);
                        /* SPR 19951 FIXED END */
								/* coverity 40722 fix end */
                                /* SPR 17268 FIXED START */
                            /* Coverity 40722 end */
								//	continue;
#endif
							}
							else
							{
								strncpy(FaultCode, "0",2);
								strncpy(FaultString , " ", 2);
#ifdef VENDOR_SPECIFIC
                         /* SPR 19951 FIXED START */
                         TRACE (LOG_CRITICAL , RPC_HANDLER, "success received from oam and command key ACS=%s cont=%d\n", pstDownload->CommandKey, cont);
                         /* SPR 19951 FIXED END */
								if (cont == 0)
								{
                                    /* SPR 16732 FIX START */
                                    time_t t = tArg_dwn->recv_time; 
                                    struct tm *time = gmtime(&t);
                                    char buf[256] = {0};

                                    /*  coverity 96644 fix start */
                                    if(time)
                                    strftime(buf, sizeof buf, "%F %T", time);
                                    /*  coverity 96644 fix end */

                                    TRACE (LOG_CRITICAL , RPC_HANDLER, "Download Start Time= %s",buf);
                                    /* SPR 16732 FIX END */


 
     	 			/*SPR 17268 FIXED START */
                          UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
                          strcpy((char *)ABSPath2,(char *)ABSPath);

                                    TRACE (LOG_CRITICAL , RPC_HANDLER , "ABSPath before concate of UPD_DWN_CMDKEY %s ", ABSPath2);
                                    strncat((char *)ABSPath2 , UPD_DWN_CMDKEY , strlen(UPD_DWN_CMDKEY));

                                    TRACE (LOG_CRITICAL , RPC_HANDLER , "UPD_DWN_CMDKEY path after concate is =  %s ", UPD_DWN_CMDKEY);
                                    FILE *fp = fopen((char *)ABSPath2 , "w");
                                  /* SPR 17268 FIXED END */
									if(fp)
									{
										if (pstDownload->CommandKey)
										{
                                    /* SPR 16732 FIX START */
											ENB_FPRINTF (fp , "CMDKEY=%s\n",pstDownload->CommandKey);
										}
                                        if(tArg_dwn->recv_time)
                                        {
											ENB_FPRINTF (fp , "DOWNLOAD_START_TIME=%s\n",buf);
                                    /* SPR 16732 FIX END */
										}
										fclose(fp);
									}
									// Now Save the URL 
									


                             /* SPR 19951 FIXED START */
                             /* code removed */
                             /* SPR 19951 FIXED END */
                                    /* SPR 17268 FIXED START */
                                    UInt8 ABSPath3[CONFIG_PARAM_LEN] = {'\0'};
                                    strcpy((char*)ABSPath3,(char*)ABSPath);
                                    TRACE (LOG_CRITICAL , RPC_HANDLER , "ABSPath before concate of UPD_DWN_URL %s ", ABSPath3);
                                    strncat((char *)ABSPath3 , UPD_DWN_URL , strlen(UPD_DWN_URL));
                                    TRACE (LOG_CRITICAL , RPC_HANDLER , "UPD_DWN_URL after concate is =  %s ", ABSPath3);
                                    fp = fopen((char *)ABSPath3, "w");
                                    /* SPR 17268 FIXED END */

                                    //fp = fopen(UPD_DWN_URL, "w");
									if(fp)
									{
										ENB_FPRINTF (fp , "%s", ipr_cwmp_getACSURL());
										fclose(fp);
									}

								}
#endif
							}
						}
						if (pstDownload->CommandKey != NULL
#ifdef VENDOR_SPECIFIC
								&& cont
#endif
						   )
						{
							sClientReq = (ClientRequest *)ALLOCATE(sizeof(ClientRequest) );
							if (sClientReq)
							{
								sClientReq->eReqtype = TRANS_COMPLETE_REQ;
								sClientReq->pstTxComplete = ALLOCATE( 
										sizeof (struct _cwmp__TransferComplete));
								if (sClientReq->pstTxComplete  != NULL)
								{
									sClientReq->pstTxComplete->CommandKey =  ALLOCATE(COMMAND_KEY_SIZE );//
									sClientReq->EventCode = ALLOCATE(EVENT_CODE_SIZE  );  //

									if (sClientReq->pstTxComplete->CommandKey != NULL)
									{
										strncpy(sClientReq->pstTxComplete->CommandKey ,
												pstDownload->CommandKey, COMMAND_KEY_SIZE);
									}   
									if (sClientReq->EventCode != NULL)
									{
										strncpy(sClientReq->EventCode, DOWNLOAD_EVENT_CODE, EVENT_CODE_SIZE-1);
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
										sClientReq->pstTxComplete->StartTime = tArg_dwn->recv_time;
										/* SPR 19821 FIXED START*/
										sClientReq->pstTxComplete->CompleteTime = t2;
										/* SPR 19821 FIXED END */
									}

									sClientReq->pstTxComplete->FaultStruct = ALLOCATE (
											sizeof(struct cwmp__TransferCompleteFaultStruct));
									sClientReq->pstTxComplete->FaultStruct->FaultCode = FaultCode;
									sClientReq->pstTxComplete->FaultStruct->FaultString = FaultString;
									ipr_cwmp_addClientRequest(sClientReq);
								}
							}
						}
					}//Inner if  

					ylDelete(&g_Txn_List_Download , (YLNODE *)tArg_dwn);
					if (tArg_dwn->dw_logfilename)
					{
						if(-1 ==  remove(tArg_dwn->dw_logfilename))
						{
							TRACE (LOG_CRITICAL , RPC_HANDLER, "Error in removing file  %s  ",tArg_dwn->dw_logfilename);					                              /*SPR 16573 Fixed Start*/
							g_rpc_Download = 0;
							/*SPR 16573 Fixed End*/

                            /* Coverity 94790 + */
                            if(file_path)
								DEALLOCATE(file_path);
                            /* Coverity 94790 - */
							return NULL;	
						}
                        /* fqdn change start */ 
						DEALLOCATE(tArg_dwn->dw_logfilename);
                        /* fqdn change end*/
						DEALLOCATE(tArg_dwn);
					}
					/* SPR 19821 FIXED START*/
					 time (&currTime);
					/* SPR 19821 FIXED END */
					/*  coverity 86665 fix code removed */

					break;
					/* Coverity 40504 + */
				}
				/* Coverity 40504 - */
			}else
			{
				if(FaultString)
					DEALLOCATE(FaultString);
				if(sClientReq)
					DEALLOCATE(sClientReq);
				if(FaultCode)
					DEALLOCATE(FaultCode);
                /* Coverity 94790 + */
                if(file_path)
					DEALLOCATE(file_path);
                /* Coverity 94790 - */
				return NULL;
         }             
         /* Sanity Fix 25 JAN */ // Revert Coverity 99412 fix as this coverity occured due to incorrrct merging
         /* Coverity 99412 + */
         indx++;
         /* SPR 19821 FIXED START*/

         time (&currTime);

         /* SPR 19821 FIXED END */
         usleep(10000);
         /* Coverity 99412 - */
      }//Outer While
      /* SPR 19821 FIXED START*/

      time (&currTime);

      /* SPR 19821 FIXED END */

      list_size =  ylCount(&g_Txn_List_Download);
      usleep(10000);
   }
   TRACE (LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__);
   g_rpc_Download = 0;
   /*  coverity 94790  start */
   if(file_path)
       DEALLOCATE(file_path);
   /*  coverity 94790 end */

   return NULL; 	
}


/********************************************************************************
*       Function Name : rpcDownload
*
*       Description   : This function is the entry point to donwload rpc. it first 
*                      	 inform Glue layer about the Download. If glue layer returns  
*			 			true than we perform the download by calling startRpcDownload
*			 			function.
*		Args          : STDownload *  - Pointer to Download Structure.  		
*
*       Returns       : Uint16 -  Download Response.
*
*********************************************************************************/
UInt16  ipr_cwmp_Download (STDownload *pstDownload) 
{
	TRACE(LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__); 

    if (!g_rpc_IsDwnQ_init)
    {
         init_list(&g_Txn_List_Download);
         g_rpc_IsDwnQ_init = 1;
    }


    if (NULL == pstDownload)
    {
    	TRACE(LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL argument for Download...");
        return FAULT_9002;
    }

    /*SPR 18232 Fixed Start */
    if (NULL == pstDownload->URL)
    {
	    TRACE(LOG_CRITICAL , RPC_HANDLER, "ERROR :Recieved NULL URL argument for Download...");
	    return FAULT_9002;
    }

    if( strstr(pstDownload->URL, "@")!= NULL)
    {
	    TRACE (LOG_CRITICAL , RPC_HANDLER, "Error:User component in URL");
	    return 9003;
    }

    /*SPR 18232 Fixed End */

/*download upload change start*/
/* Code Removed */	
	 /*download upload change end*/
	SInt32 ret = FALSE;
    /*  coverity 86602 fix start */
	SInt32 *iptr;
    /*  coverity 86602 fix end */

  	iptr = (SInt32 *)stCallbackFunList.invokeCallback((void *)pstDownload, RPC_PREDOWNLOAD, NULL);


	if (NULL != iptr && *(iptr))
	{
		ret = ipr_cwmp_startRPCDownLoad(pstDownload);			
	}
	 /*download upload change start*/
	TRACE(LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__); 
	if( ret <= 1)
	{
		return TRUE;
	}
	else
	{
		return ret;
	}
	 /*download upload change end*/
}


/********************************************************************************
*       Function Name 	: rpc_handler_startRPCDownLoad 
*
*       Description   	: This function start the new thread for download .  
*                        
*	    args		    : STDownload * - pointer to DownLoad Struct.
*
*       Returns       	: SInt32 -  1  As success and 0 as Failure.    
*
*********************************************************************************/
SInt32 ipr_cwmp_startRPCDownLoad (STDownload *pstDownload )
{
	TRACE (LOG_CRITICAL , RPC_HANDLER, "Entering %s  ",__FUNCTION__); 
	FILE *dw_fp = NULL;
    /* SPR 20730 FIXED START */
    FILE *down_fp = NULL;
    /* COV 113182 FIXED START */
    /* code removed */
    /* COV 113182 FIXED END */
    UInt8 *ABSPath11 = ALLOCATE(CONFIG_PARAM_LEN);
    if (NULL == ABSPath11)
    {
        TRACE (LOG_CRITICAL , RPC_HANDLER, "Memory allocation failed"); 
        exit(0);
    }
    int queued_size = 0;

	Uint8 *dw_logfilename = NULL;	
	 /*download upload change starts*/
	Uint8 queue_file_string[4] ={0};
    /* SPR 20730 FIXED END */
	 /*download upload change ends*/

	/* SPR 19821 FIXED START*/
	time_t t_other;
	/* SPR 19821 FIXED END */
   /* SPR 20754 FIXED START */
	SInt32 r = 0; 

    /* SPR 20754 fix start/end  Code Removed  */

	ThreadDownload *tArg = NULL;
	SInt32 rc = 0 ;
	STDownload *stDownload = NULL;
    /*SPR 19821 FIXED START */
    time (&t_other);     	
    /*SPR 19821 FIXED END */

	/*download upload change starts*/
	if((g_rpc_Download) &&(0 == pstDownload->DelaySeconds))
	{
		TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s since download is already in progress and download request with\
				zero delay seconds is received so rejecting this download request\n",__FUNCTION__);
        /* COV 113183 FIXED START */  
        DEALLOCATE(ABSPath11);
        ABSPath11 = NULL;
        /* COV 113183 FIXED END */  
		return 9001;
	}
    
    /* SPR 20730 FIXED START */
    /* code removed */
    ENB_STRCPY((char*)ABSPath11,(char*)ABSPath);     
    ENB_STRCAT((char*)ABSPath11,"/Download/");     
    ENB_STRNCAT((char *)ABSPath11 , "down_file" , ENB_STRLEN("down_file"));
    down_fp = fopen((char *)ABSPath11, "r");
    if (NULL != down_fp)
	{
        if (NULL != fgets(queue_file_string, (queue_download_max_size+1), down_fp))
	{
            queued_size = strlen(queue_file_string);
	}
        fclose(down_fp);
    }
    TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queued_size = %d",__FUNCTION__,queued_size);
	TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue_download_max_size= %d\n",__FUNCTION__,queue_download_max_size);
    if(queued_size == queue_download_max_size)
    {
        if(0 != remove((char *)ABSPath11))
	{
            perror("file remove error");
            TRACE (LOG_CRITICAL , RPC_HANDLER, "Successful remove file ....");
        }
        DEALLOCATE(ABSPath11);
        ABSPath11 = NULL;
        /* SPR 20730 FIXED END */
		TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s queue for download is full so request not handled\n", __FUNCTION__);
		return 9004;
	}
    /*download upload change ends*/
	dw_logfilename = (Uint8 *) ALLOCATE(MAX_DW_LOG_NAME_LEN);
	if (NULL == dw_logfilename ) 
    {
        /* COV 113183 FIXED START */  
        DEALLOCATE(ABSPath11);
        ABSPath11 = NULL;
        /* COV 113183 FIXED END */  
        return 0;
    }

	stDownload = (STDownload *) ALLOCATE(sizeof (STDownload) );
	if (NULL == stDownload) 
    {
        /* COV 113183 FIXED START */  
        DEALLOCATE(ABSPath11);
        ABSPath11 = NULL;
        /* COV 113183 FIXED END */  
        DEALLOCATE(dw_logfilename);
        return 0;
    }

	stDownload->CommandKey = ALLOCATE( COMMAND_KEY_SIZE );
	stDownload->FileType = ALLOCATE( FILE_TYPE_SIZE );
	stDownload->URL = ALLOCATE( URL_SIZE );
	stDownload->Username = ALLOCATE( USR_NAME_SIZE );
	stDownload->Password = ALLOCATE( PWD_SIZE );
	stDownload->TargetFileName = ALLOCATE( TRG_NAME_SIZE );
	stDownload->SuccessURL = ALLOCATE( URL_SIZE );
	stDownload->FailureURL = ALLOCATE( URL_SIZE );

	if (stDownload->CommandKey)
		strncpy (stDownload->CommandKey , pstDownload->CommandKey , COMMAND_KEY_SIZE);
	if (stDownload->FileType)
		strncpy (stDownload->FileType , pstDownload->FileType , FILE_TYPE_SIZE);
	if (stDownload->URL)
		strncpy (stDownload->URL , pstDownload->URL , URL_SIZE);
	if (stDownload->Username)
		strncpy(stDownload->Username , pstDownload->Username ,USR_NAME_SIZE);
	if (stDownload->Password)
		strncpy(stDownload->Password , pstDownload->Password , PWD_SIZE); 
	if (stDownload->TargetFileName)
		strncpy(stDownload->TargetFileName , pstDownload->TargetFileName , TRG_NAME_SIZE);
	if (stDownload->SuccessURL)
		strncpy(stDownload->SuccessURL , pstDownload->SuccessURL , URL_SIZE);
	if (stDownload->FailureURL)
		strncpy(stDownload->FailureURL , pstDownload->FailureURL , URL_SIZE); 
	stDownload->FileSize = pstDownload->FileSize;
	stDownload->DelaySeconds = pstDownload->DelaySeconds;

	/* SPR 19821 FIXED START*/
   /* SPR 20754 FIXED START */
	snprintf(dw_logfilename ,  MAX_DW_LOG_NAME_LEN, "%s_%ld_%d", DW_FILE_PATH_NAME , t_other,r) ;
   /* SPR 20754 FIXED END */
	/* SPR 19821 FIXED END */



    /* SPR 17268 FIXED START */

     /*DW_DIR_REMOVE CHANGE START */
	UInt8 *  ABSPath1;

	ABSPath1 = ALLOCATE( CONFIG_PARAM_LEN);
     /*DW_DIR_REMOVE CHANGE END */

    /*  coverity 99399 fix start */ 
    if (NULL == ABSPath1)
    {    
        TRACE (LOG_CRITICAL , RPC_HANDLER, "ABSPath1 is NULL, Memory allocation failed"); 
        exit(0);
    }   

    strcpy((char*)ABSPath1,(char*)ABSPath);     
    TRACE (LOG_CRITICAL , RPC_HANDLER , "ABSPath before concate of dw_logfilename %s ", ABSPath1);
       /* SPR_18846 Fix Start */
       /* SPR_18846 Fix End */
    /*  coverity 99399 fix end */ 

    /*  coverity 99399 fix start */ 
    /*  coverity 96670/96973 fix start */
    if(((strlen((char *)ABSPath1) + strlen(dw_logfilename)) < 256))
    /*  coverity 99399 fix end */ 
    {
        strncat((char *)ABSPath1 , dw_logfilename , strlen(dw_logfilename));
        TRACE (LOG_CRITICAL , RPC_HANDLER , "ABSPath after concate of dw_logfilename %s ", ABSPath1);
        dw_fp = fopen((char *)ABSPath1 ,"w+");
     /* SPR 17268 FIXED END */
    }
    /*  coverity 96670/96973 fix end */

	if (dw_fp != NULL)
	{
		ENB_FPRINTF (dw_fp , "%s\n%s\n%s\n%s\n%s\n%u\n%s\n%u\n%s\n%s\n%ld", 
				(strcmp(pstDownload->CommandKey,"") == 0) ? "-" : pstDownload->CommandKey, 
				(strcmp (pstDownload->FileType,"")==0) ? "-" : pstDownload->FileType, 
				strcmp(pstDownload->URL,"")==0 ? "-" : pstDownload->URL, 
				strcmp(pstDownload->Username,"")==0 ? "-" : pstDownload->Username, 
				strcmp(pstDownload->Password, "")== 0  ? "-" :pstDownload->Password, 
				pstDownload->FileSize, 
				strcmp(pstDownload->TargetFileName, "")==0  ? "-" : pstDownload->TargetFileName, 
				pstDownload->DelaySeconds, 
				strcmp(pstDownload->SuccessURL,"")==0 ? "-" : pstDownload->SuccessURL, 
				/* SPR 19821 FIXED START*/
				strcmp(pstDownload->FailureURL,"") == 0 ? "-" : pstDownload->FailureURL, t_other);
		/* SPR 19821 FIXED END */

		fclose(dw_fp);
	}

	tArg = (ThreadDownload *)ALLOCATE(sizeof (ThreadDownload)  );	

	if (tArg != NULL)
	{
        /* SPR 17268 FIXED START */
		tArg->dw_logfilename = (char *)ABSPath1;
        /* SPR 17268 FIXED END */
		tArg->pstDownload = stDownload;
		tArg->Status = RPC_TRUE;
		tArg->cancelStatus = CANCEL_FALSE;
		tArg->delay = stDownload->DelaySeconds;
		/* SPR 19821 FIXED START*/
		tArg->recv_time = t_other;
		/* SPR 19821 FIXED END */

		if (g_rpc_Download)
		{   
			rc = pthread_kill(g_rpc_Download, 0);
			if (!rc)
			{
				//Just Add in List
				ADD_IN_LIST(&g_Txn_List_Download, (YLNODE *)tArg); 
			}
		}
		else
		{
			ADD_IN_LIST(&g_Txn_List_Download, (YLNODE *)tArg);	

			// set stack size attibute 
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			//  Start Of thread to actually download the file from remote server 
			rc = pthread_create(&g_rpc_Download, &attr, ipr_cwmp_actualDownload, (void *)tArg);
			pthread_attr_destroy(&attr);
			pthread_detach(g_rpc_Download);
		}

	}

	TRACE(LOG_CRITICAL , RPC_HANDLER, "Exiting %s  ",__FUNCTION__); 
	return rc;
}


/********************************************************************************************
*       Function Name 	:  rpc_handler_ManagementServerModuleSet 
*
*       Description   	: This function is called by setParameterValue  which than  
*                         calls the setter method of ManagementServerModule to set 
*                         the parameter provided in list of ParameterList . 
*
*	    Args 		:     SetParameterValues *  - Pointer to struct SetParameterValues. 
*						  which contains pointer to list of Parameter	
*						  List. 
*
*      Returns       	: StStatus * -  It returns the pointer to StStatus struct whic contains
*       			     the status of setparameter call based on per parameter
*      			    	 basis. 	
*
*********************************************************************************************/
StStatus *  ipr_cwmp_ManagementServerModuleSet (SetParameterValues *stSetParamVal)
{	
	ParameterValueStruct **stTmpParam = NULL;
	SInt32 iSize = 0 ; 
	Uint8 * Name = NULL;
	Uint8 *pName = NULL;
	Char8 * value = NULL;

	Uint8 *ParameterName = NULL;
	Uint8 *FaultCode = NULL;
	Uint8 *FaultString = NULL;

	SInt32 iCount = 0 ; 
	SInt32 iFaultSize =  0 ;
	UINT32 conv = 0 ; 
	time_t tmpTime = 0 ; 
	Uint8 *tok = NULL;
	SInt32 size = 0;

	StStatus  * response = NULL;
	STFault *pstFault = NULL;
	struct _cwmp__Fault_SetParameterValuesFault *stTmpArr = NULL;

	TRACE(LOG_CRITICAL , "Management Server Module" , "Entering in %s ",__FUNCTION__);
	TRACE(LOG_CRITICAL , "Parameter List" , "is");
	//TRACE(LOG_CRITICAL , "" , "",stub_print_param_list(stSetParamVal->ParameterList));
	stub_print_param_list(stSetParamVal->ParameterList);

    /* Coverity 40626 + */
    response = (StStatus *) ALLOCATE(sizeof(StStatus));
    /* Coverity 40626 - */

	if (response == NULL)
	{
		return NULL;
	}	 

	stTmpParam =  (stSetParamVal->ParameterList->__ptrParameterValueStruct);

	if (stTmpParam != NULL)
	{
		iSize = stSetParamVal->ParameterList->__size;
		pName  = ALLOCATE(MAX_PARAM_NAME_SIZE  );
			if (pName  == NULL)
			{
				TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure "); 
				exit(0);
			}	 
		ParameterName = (Uint8 *) ALLOCATE(MAX_PARAM_NAME_SIZE ) ;
			if (ParameterName == NULL)
			{
				TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure "); 
				exit(0);
			}	 
		FaultCode = (Uint8 *) ALLOCATE(FAULT_CODE_SIZE ) ;
			if (FaultCode == NULL)
			{
				TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure "); 
				exit(0);
			}	 
		FaultString = (Uint8 *) ALLOCATE(FAULT_STRING_SIZE );
			if (FaultString == NULL)
			{
				TRACE(LOG_CRITICAL , RPC_HANDLER, "Alloc failure "); 
				exit(0);
			}	 

        while (iCount < iSize)
		{
			strncpy(pName , stTmpParam[iCount]->Name, MAX_PARAM_NAME_SIZE);
			Name = pName;
			value = (Char8 *)stTmpParam[iCount]->Value;

			tok = strtok(pName , ".");

			if (tok != NULL)
			{
				tok = strtok(NULL , ".");
				if (tok != NULL)
				{
					tok = strtok(NULL , ".");
					Name = tok;
					if (Name == NULL)
					{
						stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
								(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
						if (stTmpArr == NULL)
						{
							continue;   /* no Memory  so continue with iteration   */
						}


						if (ParameterName!= NULL && FaultCode != NULL && FaultString != NULL)
						{
							strncpy(ParameterName ,"", MAX_PARAM_NAME_SIZE) ;
							strncpy(FaultCode , FAULT_9005, FAULT_CODE_SIZE);
							strncpy(FaultString , FAULT_9005_STR, FAULT_STRING_SIZE);
						}
						stTmpArr[iFaultSize].ParameterName = ParameterName;
						stTmpArr[iFaultSize].FaultCode= FaultCode;
						stTmpArr[iFaultSize].FaultString = FaultString;
						iFaultSize++;
						continue;
					}
					//IPSEC phase 2 code starts
#ifdef HAVE_IPSEC
					else if(strncmp(Name, "Tunnel", 6) == 0 || strncmp(Name, "FAPService", 10) == 0)
					{
						tok = strtok(NULL , ".");
						Name = tok;
						if(Name != NULL && strncmp(Name, "CryptoProfile", 13) == 0 || strncmp(Name, "[1]", 3) == 0)
						{
							tok = strtok(NULL , ".");
							Name = tok;
							if(Name != NULL && strncmp(Name, "FAPControl", 10) == 0)
							{
								tok = strtok(NULL , ".");
								Name = tok;
								if(Name != NULL && strncmp(Name, "LTE", 3) == 0)
								{
									tok = strtok(NULL , ".");
									Name = tok;
									if(Name != NULL && strncmp(Name, "Gateway", 7) == 0)
									{
										tok = strtok(NULL , ".");
										Name = tok;
									}
								}
							}
							else if(Name != NULL && strncmp(Name, "[1]", 3) == 0)
							{
								tok = strtok(NULL , ".");
								Name = tok;
							}
						}
					}
#endif
					//IPSEC phase 2 code ends
				}
			}

			if (value == NULL)
			{
				stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
						(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
				if (stTmpArr == NULL ) 
				{
					continue;   /* no Memory  so continue with iteration   */
				}

				if ( ParameterName!= NULL && FaultCode != NULL && FaultString != NULL)
				{
					strncpy(ParameterName ,Name, MAX_PARAM_NAME_SIZE) ;
					strncpy(FaultCode , FAULT_9007, FAULT_CODE_SIZE);
					strncpy(FaultString , FAULT_9007_STR, FAULT_STRING_SIZE);
				}
				stTmpArr[iFaultSize].ParameterName = ParameterName;
				stTmpArr[iFaultSize].FaultCode= FaultCode;
				stTmpArr[iFaultSize].FaultString = FaultString;
				iFaultSize++;
				continue;
			}

			if (strcmp(Name, ENABLECWMP) == 0)
			{
				if (*value == 48 ||  
						((strlen((const char *)value) > 4) &&  (*value == 'f' && *(value+1) == 'a' 
							&& *(value+2) == 'l' && *(value+3) == 's' && *(value+4) == 'e')))
				{
					ipr_cwmp_setEnableCWMP(false);
				}
				else if (*value == 49 ||  
						((strlen((const char *)value) > 3) &&  
						 (*value == 't' && *(value+1) == 'r' && *(value+2) == 'u' && *(value+3) == 'e')))
				{
					ipr_cwmp_setEnableCWMP(true);	
				}
				else
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}

			}
			else if (strcmp(Name, URL_STRING) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0)
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Call setter method 
					ipr_cwmp_setACSURL ((Char8 *)value);
					ipr_cwmp_acs_discovery('n');
					//stCallbackFunList.invokeCallback((void *)value , RPC_SET_URL , NULL);
				}
			}
			else if (strcmp(Name, USERNAME) == 0)
			{
				if ((strlen((const char *)value) > USR_NAME_SIZE)  || (strlen((const char *)value) == 0))
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
                /* Coverity 97483 + */ 
                {
					if(SUCCESS != ipr_cwmp_setUsername(value))
                    {
                        TRACE(LOG_MAJOR, RPC_HANDLER, "Invalid value of Username received %s\n",value);
                        continue;
                    }        
                }     
                /* Coverity 97483 - */ 
			}
			else if (strcmp(Name, PWD) == 0)
			{
				if ((strlen((const char *)value) > PWD_SIZE)  || (strlen((const char *)value) == 0))
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
                /* Coverity 97482 + */ 
                {    
                    if(SUCCESS != ipr_cwmp_setPassword(value))
                    {
                        TRACE(LOG_MAJOR, RPC_HANDLER, "Invalid value of Password received %s \n",value);
                        continue;
                    }        
                }    
                /* Coverity 97482 - */ 
			}
			else if (strcmp(Name, PERIOD_INFORM_EN) == 0)
			{
				if (*value == 48  ||  
						((strlen ((const char *)value) > 4) && (*value == 'f' && *(value+1) == 'a' && *(value+2) == 'l' && 
							*(value+3) == 's' && *(value+4) == 'e')))
				{
					ipr_cwmp_setPeriodicInformEnable(false);
				}
				else if (*value == 49  ||  
						((strlen ((const char *)value) > 3) &&  (*value == 't' && 
							*(value+1) == 'r' && *(value+2) == 'u' && *(value+3) == 'e')))
				{
					ipr_cwmp_setPeriodicInformEnable(true);
				}
				else
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);	
				}
			}
			else if (strcmp(Name, PERIOD_INFORM_INT) == 0)
			{
				conv = strtoul((const char *)value , NULL , 10);
				if ((conv == 0  || conv == 4294967295u) && (strncmp((const char *)value , U_INT_RANGE, 10) != 0))
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007 (stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Setter method 		
					ipr_cwmp_setPeriodicInformInterval (conv);	
				}
			}			
			else if (strcmp(Name, PERIOD_INFORM_TIME) == 0)
			{

				Uint8 zone[32];
				*zone = '\0';
				struct tm T;
				memset((void*)&T, 0, sizeof(T));
				//	const Uint8 *tr_t = "%d-%d-%dT%d:%d:%d%31s";

				if(0 == sscanf((const char *)value,"%4d-%2d-%2dT%2d:%2d:%2d%31s", &T.tm_year, &T.tm_mon, &T.tm_mday, &T.tm_hour, &T.tm_min, &T.tm_sec, zone))
                {
                    perror("sscanf error");
                } 
				TRACE(LOG_CRITICAL , RPC_HANDLER, "In %s  year : %d \n month: %d, \n day :%d \n hour : %d \n min :                                                   %d \n sec: %d \n ",__FUNCTION__, T.tm_year, T.tm_mon
						, T.tm_mday , T.tm_hour , T.tm_min , T.tm_sec);
				tmpTime = ipr_cwmp_setPeriodicInformTime(T.tm_mon , T.tm_mday , T.tm_hour , T.tm_min , T.tm_sec,0); 

				if (tmpTime !=  0)
				{
					// fault 9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007 (stTmpArr, &iFaultSize, Name);
				}
			}			
			else if (strcmp(Name, PARAM_KEY) == 0)
			{
				size = strlen((const char *)value);
				//setter method			
				if (size > PARAM_KEY_SIZE  || size == 0 )
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
					ipr_cwmp_setParameterKey(value);
			}
			else if (strcmp (Name, CONN_REQ_URL) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0 )
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
                /* Coverity 97475 + */ 
                {
                    if (SUCCESS != ipr_cwmp_setConnectionRequestURL(value))
                    {
                        TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value received for ConnectionRequestURL %s \n",value);
                        continue;
                    }
                }    
                /* Coverity 97475 - */ 
			}
			else if (strcmp(Name, CONN_REQ_USR) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > USR_NAME_SIZE  || size == 0)
				{ 
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
                /* Coverity 97478 + */ 
                {   
                    if (SUCCESS != ipr_cwmp_setConnectionRequestUsername(value))
                    {
                        TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of ConnectionRequestUsername received %s\n ",value);
                        continue;
                    }
                }
                /* Coverity 97478 - */ 
			}
			else if (strcmp (Name, CONN_REQ_PWD) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > PWD_SIZE || size == 0 )
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */

					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
                /* Coverity 97474 + */ 
                {
                    if (SUCCESS != ipr_cwmp_setConnectionRequestPassword(value))
                    {
                        TRACE(LOG_MAJOR, GLUE_LAYER, "Invalid value of ConnectionRequestPassword received %s \n",value);
                        continue;
                    }
                }    
                /* Coverity 97474 - */ 
			}
			else if (strcmp(Name, UPGRD_MANAGED) == 0)
			{
				if (*value == 48  ||  
						((strlen ((const char *)value) > 4) &&  
						 (*value == 'f' && *(value+1) == 'a' && *(value+2) == 'l' && *(value+3) == 's' 
						  && *(value+4) == 'e' )))
				{
					ipr_cwmp_setUpgradesManaged(false);
				}
				else if (*value == 49  ||  
						((strlen ((const char *)value) > 3) &&  
						 (*value == 't' && *(value+1) == 'r' && *(value+2) == 'u' && *(value+3) == 'e')))
				{
					ipr_cwmp_setUpgradesManaged(true);
				}
				else
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
			}
			else if (strcmp(Name, KICKURL) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0)
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
					ipr_cwmp_setKickURL(value);
			}
			else if (strcmp(Name, DWN_PROG_URL) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0 )
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
					ipr_cwmp_setDownloadProgressURL(value);
			}
			else if ( strcmp (Name, DF_ACT_NOTIF) == 0)
			{
				conv  =  strtoul((const char *)value , NULL , 10);
				if ((conv == 4294967295u ) && (strncmp((const char *)value , U_INT_RANGE, 10) != 0))
				{
					// fault  9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Setter method 								
					ipr_cwmp_setDefaultActiveNotificationThrottle(conv);
				}
			}
			else if (strcmp (Name, CWMP_RETRY_MIN_WAIT) == 0)
			{
				conv = strtoul((const char *)value , NULL , 0);
				if (conv == 0  || conv > CWMP_RET)
				{
					// fault  9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					ipr_cwmp_setCWMPRetryMinimumWaitInterval(conv);
				}
				//Setter method						
			}
			else if (strcmp(Name, CWMP_RETRY_INT_MULT) == 0)
			{
				conv  = strtoul((const char *)value , NULL , 0);
				if (conv == 0 || conv > CWMP_RET || conv < 1000)
				{
					// fault  9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Setter method							
					ipr_cwmp_setCWMPRetryIntervalMultiplier(conv);
				}
			}
			else if (strcmp(Name,UDP_CONN_REQ_ADDR) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0)
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
					ipr_cwmp_setUDPConnectionRequestAddress(value);
			}
#ifdef HAVE_IPSEC
			else if (strcmp(Name, CHILD_CRYPTO_PROF_ENABLE) == 0)
			{
				if (*value == 48 ||  
						((strlen((const char *)value) > 4) &&  (*value == 'f' && *(value+1) == 'a' 
							&& *(value+2) == 'l' && *(value+3) == 's' && *(value+4) == 'e')))
				{
					ipr_cwmp_set_crypto_enable(false);
				}
				else if (*value == 49 ||  
						((strlen((const char *)value) > 3) &&  
						 (*value == 't' && *(value+1) == 'r' && *(value+2) == 'u' && *(value+3) == 'e')))
				{
					ipr_cwmp_set_crypto_enable(true);	
				}
				else
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_IKEREKEYLIFETIME) == 0)
			{
				conv  = strtoul((const char *)value , NULL , 0);
				if (conv == 0 )
				{
					// fault  9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Setter method							
					ipr_cwmp_set_ike_rekey_lifetime(conv);
				}
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_IPSECREKEYLIFETIMETIME) == 0)
			{
				conv  = strtoul((const char *)value , NULL , 0);
				if (conv == 0 )
				{
					// fault  9007
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
					if (stTmpArr == NULL) 
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
				{
					//Setter method							
					ipr_cwmp_set_ipsec_rekey_lifetime_time(conv);
				}
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_AUTHMETHOD) == 0)
			{
				size = strlen((const char *)value);
				//setter method
				if (size > URL_SIZE  || size == 0)
				{
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
							(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));
					if (stTmpArr == NULL)
					{
						continue;   /* no Memory  so continue with iteration   */
					}
					ipr_cwmp_assign_Fault_9007(stTmpArr, &iFaultSize, Name);
				}
				else
					ipr_cwmp_set_authmethod(value);
			}
#endif
			else
			{
				stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) TR_REALLOC(stTmpArr ,  
						(sizeof(struct _cwmp__Fault_SetParameterValuesFault) * (iFaultSize+1)));				
				if (stTmpArr == NULL) 
				{
					continue;   /* no Memory  so continue with iteration   */
				}

				if (ParameterName != NULL && FaultCode != NULL && FaultString != NULL)
				{	
					strncpy(ParameterName ,Name , MAX_PARAM_NAME_SIZE) ;
					strncpy(FaultCode , FAULT_9005, FAULT_CODE_SIZE);
					strncpy(FaultString , FAULT_9005_STR, FAULT_STRING_SIZE);
					stTmpArr[iFaultSize].ParameterName = ParameterName;
					stTmpArr[iFaultSize].FaultCode= FaultCode;
					stTmpArr[iFaultSize].FaultString = FaultString;
					iFaultSize++;
					continue;
				}
			}
			iCount++;
			/*Freeing the pointer as its looping back */
			Name = NULL;
			tok = NULL;
            memset(pName, 0x00, MAX_PARAM_NAME_SIZE);
		} //while

		if (iFaultSize > 0 && stTmpArr != NULL) 
		{
			pstFault = (STFault *) ALLOCATE( sizeof(STFault) );
			response->eStatus = RPC_FAULT;
			if (pstFault)
			{
				pstFault->SetParameterValuesFault = ALLOCATE(sizeof(struct _cwmp__Fault_SetParameterValuesFault) );
				if(pstFault->SetParameterValuesFault )
				{ 
					memcpy(pstFault->SetParameterValuesFault, stTmpArr, sizeof(struct _cwmp__Fault_SetParameterValuesFault));
				
					pstFault->FaultCode = (Uint8 *) ALLOCATE(FAULT_CODE_SIZE ) ;
					if(pstFault->FaultCode)
					{ 
						pstFault->FaultString = (Uint8 *) ALLOCATE(FAULT_STRING_SIZE );
						if(pstFault->FaultString )
						{ 
							if (FaultCode != NULL && FaultString != NULL)
							{
								strncpy(pstFault->FaultCode , stTmpArr[0].FaultCode, FAULT_CODE_SIZE);
								strncpy(pstFault->FaultString ,  stTmpArr[0].FaultString , FAULT_STRING_SIZE);
							}
							pstFault->__sizeSetParameterValuesFault  = iFaultSize;

							response->stFault = pstFault;
						}	
					}
				}
			}
		}
		else
		{
			response->eStatus = RPC_SUCCESS;
		}
	}
	//coverity fix

	/* Reboot fix */
	return response;	
}


/********************************************************************************************
*       Function Name   :  rpc_handler_ManagementServerModuleGet
*
*       Description     : This function is called by getParameterValue  which than
*                         calls the getter method of ManagementServerModule to retrieve 
*                         the parameter provided in list of ParameterListi and set their values
			  in the input structure.
*
*       Args            : ParameterList *  - Pointer to struct ParameterList.
*                                                 which contains pointer to list of ParameterNames
*                                                 and Values.
*
*       Returns         :   0 as  Success -  and Non Zero value returns the Fault code.  
*
*********************************************************************************************/
SInt32  ipr_cwmp_ManagementServerModuleGet (ParameterList   **ptrToParam)
{
	ParameterValueStruct **stcpeParam = NULL;
	ParameterList   *pstParameterList = NULL;
	SInt32 iSize = 0 ; 
	Uint8 * Name = NULL;
	Uint8 * value = NULL;
	Uint8 *pName = NULL;
	SInt32 iCount = 0 ; 
	Uint8 *tok = NULL;
	SInt32 res = 0; 

	pstParameterList = *ptrToParam;
	stcpeParam = pstParameterList->__ptrParameterValueStruct;

	if (stcpeParam != NULL)
	{
		iSize = pstParameterList->__size;
		pName = ALLOCATE( MAX_PARAM_NAME_SIZE) ;
		while (iCount < iSize)
		{
			if (pName == NULL)
			{
				res = 9002;
				break;
			}
			else
			{
				strncpy(pName , stcpeParam [iCount]->Name, MAX_PARAM_NAME_SIZE);
				value = stcpeParam[iCount]->Value;
				tok = strtok (pName , ".");

				if (tok != NULL)
				{
					tok = strtok(NULL , ".");
					if (tok != NULL)
					{
						tok = strtok(NULL , ".");
						Name = tok;
						if (Name == NULL)
						{
 							 /* Spr 17463 ZOMMBI Fix Start */			
							/* Spr 17463 ZOMMBI Fix End */
							*ptrToParam = ipr_cwmp_getAllList();   // To be added to return the list of all.
							if (!(*ptrToParam))
							{
								res = 9002;
								break;
							}	
							else{
								DEALLOCATE(pName);
								return 1;
                            }
						}
						//IPSEC phase 2 code starts
#ifdef HAVE_IPSEC
						else if(strncmp(Name, "Tunnel", 6) == 0 || strncmp(Name, "FAPService", 10) == 0)
						{
							tok = strtok(NULL , ".");
							Name = tok;
							if(Name != NULL && strcmp(Name, "CryptoProfileNumberOfEntries") < 0)
							{
								tok = strtok(NULL , ".");
								Name = tok;
								if(Name != NULL && strncmp(Name, "[1]", 3) == 0)
								{
									tok = strtok(NULL , ".");
									Name = tok;
								}
							}
							else if(Name != NULL && strncmp(Name, "[1]", 3) == 0)
							{
								tok = strtok(NULL , ".");
								Name = tok;
								if(Name != NULL && strncmp(Name, "FAPControl", 10) == 0)
								{
									tok = strtok(NULL , ".");
									Name = tok;
									if(Name != NULL && strncmp(Name, "LTE", 3) == 0)
									{
										tok = strtok(NULL , ".");
										Name = tok;
										if(Name != NULL && strncmp(Name, "Gateway", 7) == 0)
										{
											tok = strtok(NULL , ".");
											Name = tok;
										}
									}
								}
							}
						}
#endif
						//IPSEC phase 2 code ends
					}
					else
					{
						res = 9005;
						break;
					}
				}
				else
				{
					res = 9002;
					break;
				}
			}

			if (strcmp(Name, ENABLECWMP) == 0)
			{				
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getEnableCWMP();
                stcpeParam[iCount]->__type = SOAP_TYPE_xsd__boolean;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, URL_STRING) == 0 )
			{
				//Call getter method 
                                /* SPR 14319 FIX START*/
			        if((const char *)ipr_cwmp_getACSURL() != PNULL)	
				strncpy(value , (const char *)ipr_cwmp_getACSURL(),MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp (Name, USERNAME) == 0)
			{
				//getter
                                /* SPR 14319 FIX START*/
			        if((const char *)ipr_cwmp_getUsername() != PNULL)	
				strncpy(value , (const char *)ipr_cwmp_getUsername(),MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, PWD) == 0)
			{
                /* Spr 16783 Fix Start */	
			        if((const char *)ipr_cwmp_getPassword() != PNULL)	
				tr_strncpy(value ,(char *)ipr_cwmp_getPassword());	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, PERIOD_INFORM_EN) == 0)
			{										
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getPeriodicInformEnable();
                stcpeParam[iCount]->__type = SOAP_TYPE_xsd__boolean;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, PERIOD_INFORM_INT )  == 0)
			{ 
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getPeriodicInformInterval();
                stcpeParam[iCount]->__type = SOAP_TYPE_unsignedInt;
                /* Spr 16783 Fix End */	

			}			
			else if (strcmp (Name, PERIOD_INFORM_TIME) == 0)
			{
				time_t rawtime;
				struct tm * timeinfo;

				rawtime  = ipr_cwmp_getPeriodicInformTime();
				timeinfo = localtime(&rawtime);
				if(timeinfo)
				{
					if (strftime(value , MAX_PARAM_VAL_SIZE , "%Y-%m-%dT%H:%M:%S",timeinfo) == 0)
						res = 9002;
				}	
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}		
			else if (strcmp(Name, PARAM_KEY ) == 0)
			{
                                /* SPR 14319 FIX START*/
			        if((const char *)ipr_cwmp_getParameterKey() != PNULL)	
				strncpy(value , (const char *)ipr_cwmp_getParameterKey(), MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, CONN_REQ_URL) == 0)
			{
                                /* SPR 14319 FIX START*/
                                if((const char *)ipr_cwmp_getConnectionRequestURL() != PNULL)
				strncpy(value , (const char *)ipr_cwmp_getConnectionRequestURL(), MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, CONN_REQ_USR) == 0)
			{ 
                                /* SPR 14319 FIX START*/
                                if((const char *)ipr_cwmp_getConnectionRequestUsername() != PNULL)
				strncpy(value , (const char *)ipr_cwmp_getConnectionRequestUsername(), MAX_PARAM_VAL_SIZE);		
                                /* SPR 14319 FIX END*/	
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, CONN_REQ_PWD ) == 0)
			{
                /* Spr 16783 Fix Start */	
			        if((const char *)ipr_cwmp_getConnectionRequestPassword() != PNULL)	
				tr_strncpy(value  ,(char *)ipr_cwmp_getConnectionRequestPassword());	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, UPGRD_MANAGED) == 0)
			{						
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getUpgradesManaged();
                stcpeParam[iCount]->__type = SOAP_TYPE_xsd__boolean;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, KICKURL) == 0)
			{
                                /* SPR 14319 FIX START*/
                                if((const char *)ipr_cwmp_getKickURL() != PNULL)
				strncpy(value , (const char *)ipr_cwmp_getKickURL(), MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, DWN_PROG_URL) == 0)
			{
                                /* SPR 14319 FIX START*/
                                if((const char *)ipr_cwmp_getDownloadProgressURL() != PNULL)
				strncpy(value , (const char *)ipr_cwmp_getDownloadProgressURL(), MAX_PARAM_VAL_SIZE);
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
			else if ( strcmp (Name, DF_ACT_NOTIF) == 0)
			{
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getDefaultActiveNotificationThrottle();
                stcpeParam[iCount]->__type = SOAP_TYPE_unsignedInt;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, CWMP_RETRY_MIN_WAIT) == 0)
			{	
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getCWMPRetryMinimumWaitInterval();
                stcpeParam[iCount]->__type = SOAP_TYPE_unsignedInt;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name, CWMP_RETRY_INT_MULT)  == 0)
			{
                /* Spr 16783 Fix Start */	
                (*(int *)(value)) = ipr_cwmp_getCWMPRetryIntervalMultiplier();
                stcpeParam[iCount]->__type = SOAP_TYPE_unsignedInt;
                /* Spr 16783 Fix End */	
			}
			else if (strcmp(Name,UDP_CONN_REQ_ADDR)  == 0)
			{
                                /* SPR 14319 FIX START*/
                                if((const char *)ipr_cwmp_getUDPConnectionRequestAddress() != PNULL)
				strncpy(value , (const char *)ipr_cwmp_getUDPConnectionRequestAddress(), MAX_PARAM_VAL_SIZE);		
                                /* SPR 14319 FIX END*/
                /* Spr 16783 Fix Start */	
                stcpeParam[iCount]->__type = SOAP_TYPE_string;
                /* Spr 16783 Fix End */	
			}
#ifdef HAVE_IPSEC
			else if (ENB_STRCMP(Name, CHILD_FAP_TUNNEL) == 0)
			{
				tr_sprintf(value, "%u", ipr_cwmp_get_crypto_profile_no_entries());
			}
			else if (strcmp(Name, CHILD_FAP_TUNNEL) == 0)
			{				
				tr_sprintf(value, "%u",  ipr_cwmp_get_crypto_profile_no_entries());
				DEALLOCATE(g_ipr_cwmp_crypto_params);
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_ENABLE) == 0)
			{				
				snprintf(value, 2,"%d", ipr_cwmp_get_crypto_enable());
				DEALLOCATE(g_ipr_cwmp_crypto_params);
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_IKEREKEYLIFETIME) == 0)
			{				
				tr_sprintf(value, "%u",  ipr_cwmp_get_ike_rekey_lifetime());
				DEALLOCATE(g_ipr_cwmp_crypto_params);
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_IPSECREKEYLIFETIMETIME) == 0)
			{				
				tr_sprintf(value, "%u",  ipr_cwmp_get_ipsec_rekey_lifetime_time());
				DEALLOCATE(g_ipr_cwmp_crypto_params);
			}
			else if (strcmp(Name, CHILD_CRYPTO_PROF_AUTHMETHOD) == 0)
			{				
				if(ipr_cwmp_get_authmethod())
					strncpy(value, ipr_cwmp_get_authmethod(), 256);
				DEALLOCATE(g_ipr_cwmp_crypto_params);
			}
#endif
			else
			{
				res = 9005;
				break;
			}
			iCount++;
            memset(pName, 0x00, MAX_PARAM_NAME_SIZE);
		} //while
		DEALLOCATE(pName);
	}//outer If 

	return res;	
}


/********************************************************************************************
*       Function Name   : ipr_cwmp_getAllList
*
*       Description     : to retrieve all parameter List.
*
*       Args            :  void 
*
*       Returns         :   ParameterList *.
*
*********************************************************************************************/
ParameterList * ipr_cwmp_getAllList (void)
{
	ParameterList *stParamList  = NULL;
	ParameterValueStruct *stTmpParam[18];	
	ParameterValueStruct **stTmpParamArr = NULL;
	SInt32 i = 0 ; 
	UInt8* value = NULL;

    /* Spr 16783 Fix Start */	
    char * parant_str = "InternetGatewayDevice.ManagementServer.";
    /* Spr 16783 Fix End */
     
	/* Spr 17463 ZOMMBI Fix Start */	
	stParamList = (ParameterList *) ALLOCATE(sizeof(ParameterList));
	/* Spr 17463 ZOMMBI Fix End */
	if (stParamList == NULL)
		return NULL;

	stTmpParamArr = (ParameterValueStruct **) ALLOCATE( sizeof(ParameterValueStruct*)*18);

	if (stTmpParamArr == NULL)
	{
		DEALLOCATE(stParamList);
		return NULL;
	}

	while (i < 18) 
	{
		stTmpParam[i] = (ParameterValueStruct *)ALLOCATE( sizeof(ParameterValueStruct));

		if (stTmpParam[i] != NULL)
		{
			stTmpParam[i]->Name = (Uint8 *) ALLOCATE( MAX_PARAM_NAME_SIZE);
			stTmpParam[i]->Value= (Uint8 *) ALLOCATE( MAX_PARAM_VAL_SIZE);
			if (!(stTmpParam[i]->Name) || !(stTmpParam[i]->Value))
			{
				DEALLOCATE(stParamList);
				DEALLOCATE(stTmpParamArr);		
				return NULL;
			}
			stTmpParamArr[i] = stTmpParam[i];	
		}	
		else
		{

			DEALLOCATE(stParamList);
			DEALLOCATE(stTmpParamArr);		
			return NULL;
		}
		i++;
	}

    /* Spr 16783 Fix Start */	
    strncpy(stTmpParam[0]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[0]->Name , ENABLECWMP, strlen(ENABLECWMP)+1);

	(*(int *)((stTmpParam[0]->Value))) = ipr_cwmp_getEnableCWMP();
    stTmpParam[0]->__type = SOAP_TYPE_xsd__boolean; 
    value = ipr_cwmp_getACSURL();

    strncpy(stTmpParam[1]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[1]->Name , URL_STRING, strlen(URL_STRING)+1);
	if (value != NULL)
		strncpy(stTmpParam[1]->Value, (const char *)value ,MAX_PARAM_VAL_SIZE);
    stTmpParam[1]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[2]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[2]->Name , USERNAME, strlen(USERNAME)+1);

    value = ipr_cwmp_getUsername();
		TRACE (LOG_CRITICAL , RPC_HANDLER, "ipr_cwmp_getUserName = %s",value);

	if (value != NULL)	
		strncpy(stTmpParam[2]->Value, (const char *)value ,MAX_PARAM_VAL_SIZE);

    stTmpParam[2]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[3]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[3]->Name , PWD, MAX_PARAM_NAME_SIZE);

    value = ipr_cwmp_getPassword();

	if (value != NULL)	
	strncpy(stTmpParam[3]->Value, (const char *)value, MAX_PARAM_VAL_SIZE);

    stTmpParam[3]->__type = SOAP_TYPE_string; 
	
    strncpy(stTmpParam[4]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[4]->Name , PERIOD_INFORM_EN, MAX_PARAM_NAME_SIZE);

	(*(int *)((stTmpParam[4]->Value))) = ipr_cwmp_getPeriodicInformEnable();
    stTmpParam[4]->__type = SOAP_TYPE_xsd__boolean; 

    strncpy(stTmpParam[5]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[5]->Name ,PERIOD_INFORM_INT, MAX_PARAM_NAME_SIZE);

	(*(int *)((stTmpParam[5]->Value))) = ipr_cwmp_getPeriodicInformInterval();
   
    stTmpParam[5]->__type = SOAP_TYPE_unsignedInt; 

    strncpy(stTmpParam[6]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[6]->Name , PERIOD_INFORM_TIME, MAX_PARAM_NAME_SIZE);

    time_t rawtime;
    struct tm * timeinfo;

    rawtime  = ipr_cwmp_getPeriodicInformTime();
    timeinfo = localtime ( &rawtime );
    if(timeinfo)
    {
	    if (strftime(stTmpParam[6]->Value, MAX_PARAM_VAL_SIZE , "%Y-%m-%dT%H:%M:%S",timeinfo) == 0)
	    {
			DEALLOCATE (stParamList);
			DEALLOCATE (stTmpParamArr);
		    return NULL;
	    }
    }  		
    stTmpParam[6]->__type = SOAP_TYPE_string; 
    strncpy(stTmpParam[7]->Name , parant_str, MAX_PARAM_NAME_SIZE);
    strncat(stTmpParam[7]->Name , CONN_REQ_URL, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getConnectionRequestURL();

	 if (value != NULL)
		strncpy(stTmpParam[7]->Value, (const char *)value, MAX_PARAM_VAL_SIZE);	
    stTmpParam[7]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[8]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat (stTmpParam[8]->Name , CONN_REQ_USR, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getConnectionRequestUsername();

	 if (value != NULL)
		strncpy(stTmpParam[8]->Value, (const char *)value , MAX_PARAM_VAL_SIZE);
    stTmpParam[8]->__type = SOAP_TYPE_string; 
	
    strncpy(stTmpParam[9]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[9]->Name , CONN_REQ_PWD, MAX_PARAM_NAME_SIZE);
    value = ipr_cwmp_getConnectionRequestPassword();

    if (value != NULL)
	strncpy(stTmpParam[9]->Value,(const char *)value, MAX_PARAM_VAL_SIZE);

    stTmpParam[9]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[10]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[10]->Name , UPGRD_MANAGED, MAX_PARAM_NAME_SIZE);

	(*(int *)((stTmpParam[10]->Value))) = ipr_cwmp_getUpgradesManaged();
    stTmpParam[10]->__type = SOAP_TYPE_xsd__boolean; 

    strncpy(stTmpParam[11]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[11]->Name , KICKURL, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getKickURL();
	if (value != NULL)
		strncpy(stTmpParam[11]->Value, (const char *)value, MAX_PARAM_VAL_SIZE);
    stTmpParam[11]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[12]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[12]->Name , DWN_PROG_URL, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getDownloadProgressURL();

	 if (value != NULL)
		strncpy(stTmpParam[12]->Value, (const char *)value , MAX_PARAM_VAL_SIZE);
    stTmpParam[12]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[13]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[13]->Name , DF_ACT_NOTIF, MAX_PARAM_NAME_SIZE);

	(*(int *)((stTmpParam[13]->Value))) = ipr_cwmp_getDefaultActiveNotificationThrottle();
    stTmpParam[13]->__type = SOAP_TYPE_unsignedInt; 


    strncpy(stTmpParam[14]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[14]->Name , UDP_CONN_REQ_ADDR, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getUDPConnectionRequestAddress();
	
	 if (value != NULL)
		strncpy(stTmpParam[14]->Value, (const char *)value, MAX_PARAM_VAL_SIZE);
    stTmpParam[14]->__type = SOAP_TYPE_string; 

    strncpy(stTmpParam[15]->Name , parant_str, MAX_PARAM_NAME_SIZE);
	strncat(stTmpParam[15]->Name ,PARAM_KEY, MAX_PARAM_NAME_SIZE);

	value = ipr_cwmp_getParameterKey();

	 if (value != NULL)	
		strncpy(stTmpParam[15]->Value, (const char *)value, MAX_PARAM_VAL_SIZE);
    stTmpParam[15]->__type = SOAP_TYPE_string; 

	stParamList->__ptrParameterValueStruct = stTmpParamArr;
	stParamList->__size = 16;
    /* Spr 16783 Fix End */	

	return stParamList;
}



inline void ipr_cwmp_assign_Fault_9007 (struct _cwmp__Fault_SetParameterValuesFault *stTmpArr , 
                                            SInt32 *iFaultSize, Uint8 *Name)
{
	Uint8 *ParameterName = NULL;
	Uint8 *FaultCode = NULL;
	Uint8 *FaultString = NULL;

	ParameterName = (Uint8 *) ALLOCATE(MAX_PARAM_NAME_SIZE) ;
	FaultCode =  (Uint8 *) ALLOCATE(FAULT_CODE_SIZE) ;
	FaultString = (Uint8 *)ALLOCATE( FAULT_STRING_SIZE);

	if (ParameterName != NULL && FaultCode != NULL && FaultString != NULL)
	{
		strncpy(ParameterName ,Name , MAX_PARAM_NAME_SIZE) ;
		strncpy(FaultCode , FAULT_9007, FAULT_CODE_SIZE);
		strncpy(FaultString , FAULT_9007_STR, FAULT_STRING_SIZE);
	}
	stTmpArr[*iFaultSize].ParameterName = ParameterName;
	stTmpArr[*iFaultSize].FaultCode= FaultCode;
	stTmpArr[*iFaultSize].FaultString = FaultString;
	*iFaultSize = *iFaultSize + 1;
}


STFault * check_duplicates_in_set_parameter_values(struct ParameterValueList * List)
{
        int i = 0;
        int j = 0;
        struct cwmp__ParameterValueStruct **paramList = NULL;
        STFault *pstFault = NULL;
        struct _cwmp__Fault_SetParameterValuesFault *stTmpArr = NULL;
        Uint8 *ParameterName = NULL;
        Uint8 *FaultCode = NULL;
        Uint8 *FaultString = NULL;

	TRACE (LOG_CRITICAL , GLUE_LAYER, "Entering in %s ",__FUNCTION__);
	paramList = List->__ptrParameterValueStruct;
	for (i = 0; i < List->__size; i++)
	{
		for (j = 0; j < List->__size; j++)
		{
			if (i!=j)
			{
				if (strcmp(paramList[i]->Name, paramList[j]->Name) == 0)
				{
					TRACE (LOG_CRITICAL , RPC_HANDLER, "In %s, Duplicates Found ",__FUNCTION__);
					stTmpArr = (struct _cwmp__Fault_SetParameterValuesFault *) ALLOCATE( sizeof(struct _cwmp__Fault_SetParameterValuesFault));
					if (NULL == stTmpArr) 
					{
						TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
						return NULL;
					}
					ParameterName = (Uint8 *) ALLOCATE(MAX_PARAM_NAME_SIZE) ;
					FaultCode = (Uint8 *) ALLOCATE(FAULT_CODE_SIZE) ;
					FaultString = (Uint8 *) ALLOCATE( FAULT_STRING_SIZE);

                              if (ParameterName!= NULL && FaultCode != NULL && FaultString != NULL)
                              {
                                   strncpy(ParameterName, paramList[i]->Name, MAX_PARAM_NAME_SIZE) ;
                                   strncpy(FaultCode, FAULT_9007, FAULT_CODE_SIZE);
                                   strncpy(FaultString, "parameter appears more than once", FAULT_STRING_SIZE);
                              }

                              stTmpArr[0].ParameterName = ParameterName;
                              stTmpArr[0].FaultCode= FaultCode;
                              stTmpArr[0].FaultString = FaultString;


					pstFault = (STFault *) ALLOCATE(  sizeof(STFault));
			      if (NULL == pstFault) 
			      {
				      TRACE (LOG_CRITICAL , RPC_HANDLER, "ERROR : Not enough memory....");
						DEALLOCATE(stTmpArr);
				      return NULL;
			      }
			      memset(pstFault , '\0' , sizeof(STFault) );

                              pstFault->SetParameterValuesFault = stTmpArr;
					FaultCode = (Uint8 *) ALLOCATE(FAULT_CODE_SIZE) ;
					FaultString = (Uint8 *) ALLOCATE( FAULT_STRING_SIZE);
                              if (FaultCode != NULL && FaultString != NULL)
                              {
                                 strncpy(FaultCode , FAULT_9003, FAULT_CODE_SIZE);
                                 strncpy(FaultString ,  FAULT_9003_STR, FAULT_STRING_SIZE);
                                 pstFault->FaultCode = FaultCode;
                                 pstFault->FaultString = FaultString;
                              }
			      pstFault->__sizeSetParameterValuesFault = 1;
			      //coverity fix
					DEALLOCATE(FaultCode);
					DEALLOCATE(FaultString);
			      return pstFault;
			}
                    }
              }
        }

        TRACE (LOG_CRITICAL , RPC_HANDLER, "No Duplicates Found, Exiting in %s ",__FUNCTION__);
        return NULL;
}


	
