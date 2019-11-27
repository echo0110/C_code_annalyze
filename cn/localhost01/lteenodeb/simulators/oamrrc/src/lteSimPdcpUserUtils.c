/************************************************************************************
*
*   FILE NAME  : userUtils.c
*
*   DESCRIPTION: Common service functions implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include "lteOamSimulator.h"
#include "lteSimPdcpEsLteFramework.h"
#include "lteLayer2CommanTypes.h"

#include <ltePdcpLteDefs.h>
#include "lteSimPdcpUserUtils.h"
#include "ltePdcpTlvUtils.h"
extern UChar8 throughputFile_g[256];
extern FILE* throughputFD_g;
extern UInt16 interval_g;
extern UInt8 startCaptureThroughput_g;


/****************************************************************************
 * Private Definitions
 ****************************************************************************/
UChar8 *arrMACResp[50] = {"MAC_FAILURE",
    "MAC_SUCCESS",
    "MAC_PARTIAL_SUCCESS",
    "MAC_INVALID_UE_ID",
    "MAC_UE_ID_EXISTS",
    "MAC_UE_NOT_EXISTS",
    "MAC_MAX_ACTIVE_UE_REACHED_ERR",
    "MAC_CELL_NOT_CONFIGURED",
    "ERROR",
    "ERROR",
    "ERROR",
    "MAC_INTERNAL_ERROR",
    "MAC_SYNTAX_ERROR",
    "ERROR",
    "MAC_LAYER_NOT_INITIALIZED",
    "MAC_LAYER_ALREADY_INITIALIZED",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR",
    "MAC_NO_ACTIVE_UE_IN_SYSTEM", 
    "ERROR",
    "ERROR",
    "ERROR",
    "ERROR"};

UChar8 *arrRLCResp[40] = {"RLC_FAILURE",
    "RLC_SUCCESS",
    "RLC_PARTIAL_SUCCESS",
    "RLC_INVALID_UE_ID",
    "RLC_UE_ID_EXISTS",
    "RLC_UE_NOT_EXISTS",
    "MAC_MAX_ACTIVE_UE_REACHED_ERR",
    "MAC_CELL_NOT_CONFIGURED",
    "ERROR",
    "ERROR",
    "ERROR",
    "RLC_INTERNAL_ERROR",
    "RLC_SYNTAX_ERROR",
    "ERROR",
    "RLC_LAYER_NOT_INITIALIZED",
    "RLC_LAYER_ALREADY_INITIALIZED",
    "RLC_NO_ACTIVE_UE_IN_SYSTEM",
    "ERROR"};



/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

#define MAX_LOG_BUF_LEN 1500

/*****************************************************************************
 * Function Name  :  addTimeStamp
 * Inputs         : string - log file name to add time stamp at the end of it
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for inserting timestamp at the end of file name given. 
 *****************************************************************************/
void addTimeStamp(char *buf)
{
#ifndef WIN32
     struct tm *tmp;
     struct timeval tv;
     struct timezone tz;
     gettimeofday(&tv, &tz);
     tmp = localtime(&tv.tv_sec);
     /* +COVERITY 10244 */
     snprintf(buf,MAXFILENAME,"_%02d.%02d.%04d.%02d-%02d-%02d-%06d", tmp->tm_mday, tmp->tm_mon+1, tmp->tm_year+1900,tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (Int)tv.tv_usec);
     /* Coverity 11406 starts */
     buf[MAXFILENAME - 1] = '\0';
     /* Coverity 11406 ends */
#else
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	snprintf(buf,MAXFILENAME,"_%02d.%02d.%04d.%02d-%02d-%02d-%06d",systime.wDay,systime.wMonth,systime.wYear,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
     /* -COVERITY 10244 */
     /* Coverity 11406 starts */
     buf[MAXFILENAME - 1] = '\0';
     /* Coverity 11406 ends */
#endif
}



/*****************************************************************************
 * Function Name  :  addTime
 * Inputs         : string - add time in hr:min:sec:microsec format 
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for inserting time in log file before the traces. 
 *****************************************************************************/
UInt32 addTime(char *buf)
{
	UInt32 retStringLen = 0;
#ifndef WIN32
     struct tm *tmp;
     struct timeval tv;
     struct timezone tz;
     gettimeofday(&tv, &tz);
     tmp = localtime(&tv.tv_sec);
     /* +COVERITY 10243 */
     retStringLen = snprintf(buf,0xFF,"[%02d:%02d:%02d:%06d] : ", tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (Int)tv.tv_usec);
#else
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	retStringLen = snprintf(buf,0xFF,"[%02d-%02d-%02d-%06d] : ",systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
     /* -COVERITY 10243 */
#endif
	return retStringLen;
}


/*****************************************************************************
 * Function Name  :  createLogFile
 * Inputs         : file name to create 
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : it will create log file with the name provided. 
 *		    In case file already created, it will overwrite the same 
 *		    filename with new time stamp. 
 *****************************************************************************/
void createLogFile(Char8 *file)
{
    /* Coverity 11406 starts */
    Char8 buf[MAXFILENAME] = {0};
    /* Coverity 11406 ends */
    addTimeStamp(buf);
    strcat(file,buf);
    strncat(file,".log",sizeof(".log"));
}

/****************************************************************************
 * Function Name  : lteLog 
 * Inputs         : level - the level of logging required.
 *		    LogModuleDetail - pointer passed of each module(RLC,MAC..etc) 
 *                  format - support for format similar to printf.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for logging for certain user defined levels.
 ****************************************************************************/
SInt32 lteLog(UInt32 level, ModuleLogDetail *det, const Char8 *format, ...)
{
    va_list A;
    UInt32 ret = 0;
    Char8 buffer[MAX_LOG_BUF_LEN];
    Char8 timebuf[0xFF];
    Char8 fullName[MAXFILENAME+64];
    UInt32 length = 0;
    va_start( A, format );

    if (det == NULL)
	{
        ret = qvLog( level, format, A );
    }
	else
	{ 
        if (det->fp == NULL)
		{ 
            ret = qvLog( level, format, A ); // if no file created
        }
		else if(det->level < level)
		{
            /* SPR 4010 Fix Start */
            va_end( A );
            /* SPR 4010 Fix End */
            return 0;
        }
		else
		{
            /*SPR 16855 +-*/
            if(ftell(det->fp) < MAXFILESIZE)
			{
                addTime(timebuf);
                fprintf (det->fp, "%s ",timebuf);
                length = vsprintf (buffer, format, A);
                if (length < MAX_LOG_BUF_LEN)
				{
                    fprintf (det->fp, "%s", buffer);
                    fprintf (det->fp, "\n");
                    fflush(det->fp);
                }
            }
			else
			{
                fclose(det->fp);
                det->fp = PNULL;
                strcpy(fullName,det->filename);
                createLogFile(fullName);
                det->fp = fopen(fullName,"w");
                if (det->fp == PNULL)
				{
                    ltePanic("Unable to open file. %s\n",fullName);
                }
                addTime(timebuf);
                fprintf (det->fp, "%s ", timebuf);
                length = vsprintf (buffer, format, A);
                if (length < 1500)
                {
                    fprintf (det->fp, "%s", buffer);
                    fprintf (det->fp, "\n");
                    fflush(det->fp);
                }
            }
            /*SPR 16855 +-*/
        }
    } 
    va_end( A );
    return ret;
}



/************************************************************************************
 * FUNCTION NAME    :   userStrReplaceChar
 * DESCRIPTION      :   Replaces all char_old to char_new in given string
 * PARAMATERS       :   [IN] UChar8* str
 *                      [IN] UChar8 char_old
 *                      [IN] UChar8 char_new
 * RETURN VALUE     :   None
 ************************************************************************************/
void userStrReplaceChar(UChar8* str, UChar8 char_old, UChar8 char_new)
{
	while(0!=*str)
	{
		if(char_old==*str)
		{
			*str=char_new;
		}
		str++;
	}
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintRawBuffer
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols
 * PARAMATERS       :   [IN] UInt8* buf - buffer with raw data
 *                      [IN] UInt32 len_buf - length of buffer
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before 
 *                      hex digits or not
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintRawBuffer(const UChar8* strHeader, const UInt8* buf, UInt32 len_buf, UInt32 fmt)
{
#ifdef UT_TESTING
	
	UInt16 apiID = 0;
	UInt8 *data_p = PNULL ;

	UInt16 current = 0;
	UInt16  response; /*Ack or Nack*/
	UInt16 transactionId = 0, srcModId = 0, destModId = 0, msgLen = 0;
	data_p = (UInt8 *)buf;
	transactionId = LTE_GET_U16BIT(&data_p[current]);
	current += 2;
	srcModId = LTE_GET_U16BIT(&data_p[current]);
	current += 2;
	destModId = LTE_GET_U16BIT(&data_p[current]);
	current += 2;
	apiID = LTE_GET_U16BIT(&data_p[current]); 
	current += 2;
	msgLen = LTE_GET_U16BIT(&data_p[current]); 
	current += 2;
	data_p = (UInt8 *)buf;

	if (PDCP_GET_STATS_CNF == apiID && 4==destModId)
	{
		response = LTE_GET_U16BIT(&data_p[current]);
		current += 2;
		//lteLog(LOG_MAJOR,PNULL,"Response(NACK-0/ACK-1 ) = %x\n",response);
		switch(response)
		{
			case PDCP_SUCCESS:
				printPdcpStats(data_p,&current,msgLen);
				break;
			case PDCP_ERR_TLV_PARSING_INVALID_LENGTH:
				lteLog(LOG_MAJOR,PNULL,"PDCP ERROR : TLV PARSING INVALID LENGTH. ");
				break;
			case PDCP_ERR_CONTEXT_NOT_INITIALIZED:
				lteLog(LOG_MAJOR,PNULL,"PDCP ERROR : CONTEXT NOT INITIALIZED. ");
				break;
			case PDCP_ERR_TLV_PARSING_INVALID_TAG_ID:
				lteLog(LOG_MAJOR,PNULL,"PDCP ERROR :TLV PARSING INVALID TAG ID.");
				break;
			case PDCP_FAIL:
				//lteLog(LOG_MAJOR,PNULL,"PDCP FAIL.");
				break;
			default:
				lteLog(LOG_MAJOR,PNULL,"PDCP ERROR RESP %u",response);
		}
	}
        else
	{
		/* For testing purpose */


		UInt32 i;
		UChar8 logStr[LOG_MAX_STRING_LEN];
		UChar8 workStr[LOG_MAX_STRING_LEN];

		assert(buf);
		assert(strHeader);

		if ( LOG_DUMP > pdcpGetLogLevel() ) 
		{
			//   Log disabled 
			return;
		}

		sprintf(logStr, "%s: ", strHeader);

		for(i=1; i<=len_buf; i++)
		{
			if(fmt == 1)
			{
				sprintf(workStr, "0x%02x ", buf[i-1]);
			}
			else
			{
				sprintf(workStr, "%02x ", buf[i-1]);
			}
			strcat(logStr, workStr);

			if(i%4 == 0)
			{
				if(i%16 == 0)
				{
					sprintf(workStr, "\n\t\t  ");
				}
				else
				{   
					sprintf(workStr, "\t");
				}
				strcat(logStr, workStr);
			}

			if ( (LOG_MAX_STRING_LEN - 20) < strlen(logStr) )
			{ //str buffer overflow
				sprintf(workStr, "...");
				strcat(logStr, workStr);
				break;
			}
		}
		sprintf(workStr, "\n");
		strcat(logStr, workStr);

		PDCP_LOG(LOG_DUMP, "%s", logStr);
	}
#endif
}


void printPdcpStats(UInt8 *data_p,UInt16 *curr,UInt16 msgLen)
{
    UInt16 current = *curr ;

    UInt16 paramTag = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    //    lteLog(LOG_MAJOR,PNULL,"Param Tag  %u\n",paramTag);

    UInt16 paramLen = 0;
    UInt32 paramItem = 0;
    UInt16 paramIdx = 0;
    while(current < msgLen)
    {
        switch(paramTag)
        {
            case PDCP_GET_INTEGRITY_STATS_CNF:
            case PDCP_GET_ROHC_STATS_CNF:
            case PDCP_GET_CIPHERING_STATS_CNF:
                paramLen = LTE_GET_U16BIT(&data_p[current]);
                current += 2;
                paramIdx = 0;

                /* add switch case to add heading for stats*/

                switch(paramTag)
                {
                    case PDCP_GET_INTEGRITY_STATS_CNF:
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
                        PDCP_LOG(LOG_DUMP, "%s", "*-------------------------------------PDCP INTEGRITY STATS----------------------------------*");
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
                        //lteLog(LOG_MAJOR,PNULL,"\n----------PDCP INTEGRITY STATS----------------\n");
                        break;
                    case PDCP_GET_ROHC_STATS_CNF:
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
                        PDCP_LOG(LOG_DUMP, "%s", "*--------------------------------------PDCP ROHC STATS--------------------------------------*");
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
                        //lteLog(LOG_MAJOR,PNULL,"\n----------PDCP ROHC STATS----------------\n");
                        break;
                    case PDCP_GET_CIPHERING_STATS_CNF:
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
                        PDCP_LOG(LOG_DUMP, "%s", "*------------------------------------PDCP CIPHERING STATS-----------------------------------*");
                        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
                        //  lteLog(LOG_MAJOR,PNULL,"\n----------PDCP CIPHERING STATS------------\n");
                        break;
                }
                while(paramIdx < paramLen-4)
                {
                    paramItem = LTE_GET_U32BIT(&data_p[current]);

                    switch(paramTag)
                    {
                        case PDCP_GET_INTEGRITY_STATS_CNF:
                            printIntegrityStats(paramIdx,paramItem);
                            break;
                        case PDCP_GET_ROHC_STATS_CNF:
                            printRohcStats(paramIdx,paramItem);
                            break;
                        case PDCP_GET_CIPHERING_STATS_CNF:
                            printCipheringStats(paramIdx,paramItem);
                            break;
                    }
                    paramIdx += 4;
                    current += 4;
                }
                break;
            case PDCP_GET_UE_STATS_CNF:

                PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
                PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------PDCP UE STATS-------------------------------------*");
                PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
                //lteLog(LOG_MAJOR,PNULL,"\n----------PDCP UE STATS------------\n\n");

                paramLen = LTE_GET_U16BIT(&data_p[current]);
                current += 2;
                UInt16 numOfActiveUe = LTE_GET_U16BIT(&data_p[current]);
                current += 2;
                PDCP_LOG(LOG_DUMP, "%s :%u","Number Of Active UE :",numOfActiveUe); 
                // lteLog(LOG_MAJOR,PNULL,"Number Of Active UE :  %u",numOfActiveUe);

                UInt16 ue = 0;
                /* need to have loop for per UE */
                for(ue = 0 ; ue < numOfActiveUe ; ue ++)
                {
                    PDCP_LOG(LOG_DUMP, "%s","************************************************");
                    PDCP_LOG(LOG_DUMP, "%s\n","************************************************");

                    /* Decoding for Each UE */
                    UInt16 pdcpUeStats = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    UInt16  pdcpUeStatsLen= LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    UInt16 crnti  = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    UInt16 ueIndex = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    UInt16  numOfActiveLc= LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    PDCP_LOG(LOG_DUMP, "%s :%u\t%s : %u\t%s :%u\t %u:%u","CRNTI",crnti,"UEINDEX",ueIndex,"ACTIVE LCs",numOfActiveLc,"PDCP UE Stats",pdcpUeStats,"PDCP UE Stats Len",pdcpUeStatsLen);
                    PDCP_LOG(LOG_DUMP, "%s","************************************************\n");

                    //printf("\tUE Index : %u\tCrnti : %u\tNumber Of Active LC : %u\n\n",ueIndex,crnti,numOfActiveLc);

                    /* To Print Srb */
                    if (0 == numOfActiveLc)
                    {
                        break;
                    }
                    UInt16 srbDrbTag  = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;

                    UInt16 srbDrbLen = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;
                    UInt16 lcCount = 0;		
                    while(srbDrbTag == SRB_PDCP_UE_STATS || srbDrbTag == DRB_PDCP_UE_STATS)
                    {

                        if(srbDrbTag == SRB_PDCP_UE_STATS)
                        {
                            printSrbStats(data_p,&current);
                        }
                        else
                        {
                            printDrbStats(data_p,&current);
                        }
                        lcCount++;
                        if(current < msgLen && lcCount < numOfActiveLc)
                        {
                            srbDrbTag  = LTE_GET_U16BIT(&data_p[current]);
                            current += 2;

                            srbDrbLen = LTE_GET_U16BIT(&data_p[current]);
                            current += 2;
                        }
                        else
                            break;

                    }//for each srb or drb;

                    PDCP_LOG(LOG_DUMP, "%s","****************************************************************************");
                    PDCP_LOG(LOG_DUMP, "%s\n\n","****************************************************************************");

                }//for each ue

                break;

            default :
                PDCP_LOG(LOG_DUMP, "%s","Not Handled\n"); 
                //lteLog(LOG_MAJOR,PNULL,"Not Handled\n");
        }
        if(current < msgLen )
        {
            paramTag = LTE_GET_U16BIT(&data_p[current]);
            current += 2;
        }
    }
    *curr = current;
}
void printSrbStats(UInt8 *data_p,UInt16 *curr)
{
	UInt16 current = *curr;
	UInt8 lcId = 0;
	UInt32 srbSduReceived = 0;
	UInt32 srbPduTransmitted = 0;
	UInt32 srbPduDropped = 0;
	UInt32 srbSduDiscarded = 0;

	lcId = *(data_p + current);
	current += 1;
	PDCP_LOG(LOG_DUMP, "%s : %u","\t\tLcId",lcId); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t LcId : %u",lcId);

	srbSduReceived = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "%s...........:%u","\t\t\t SRB SDU Received",srbSduReceived); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t SRB SDU Received : %u",srbSduReceived);
	current += 4;


	srbPduTransmitted  = LTE_GET_U32BIT(&data_p[current]);
	PDCP_LOG(LOG_DUMP, "%s........:%u","\t\t\t SRB PDU Transmitted",srbPduTransmitted); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t SRB PDU Transmitted : %u",srbPduTransmitted);
	current += 4;

	srbPduDropped  = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "%s............:%u","\t\t\t SRB PDU Dropped",srbPduDropped); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t SRB PDU Dropped : %u",srbPduDropped);
	current += 4;
	srbSduDiscarded   = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "%s..........:%u","\t\t\t SRB SDU Discarded",srbSduDiscarded); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t SRB SDU Discarded : %u",srbSduDiscarded);
	current += 4;


	*curr = current;
}

void printDrbStats(UInt8 *data_p,UInt16 *curr)
{
	UInt16 current =  *curr;
	UInt8 lcId = 0;
	UInt32 drbSduReceived = 0;
	UInt32 drbPduTransmitted = 0 ;
	UInt32 drbPduDropped = 0;
	UInt32 drbSduDiscarded = 0 ;

	lcId = *(data_p + current);
	current += 1;
	PDCP_LOG(LOG_DUMP, "\t\tLcid : %u",lcId); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t LcId : %u",lcId);

	drbSduReceived = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "\t\t\t DRB SDU Received...........:%u",drbSduReceived); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t DRB SDU Received : %u",drbSduReceived);
	current += 4;


	drbPduTransmitted  = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "\t\t\t DRB PDU Transmitted........:%u",drbPduTransmitted); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t DRB PDU Transmitted : %u",drbPduTransmitted);
	current += 4;

	drbPduDropped  = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "\t\t\t DRB PDU Dropped............:%u",drbPduDropped); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t DRB PDU Dropped : %u",drbPduDropped);
	current += 4;
	drbSduDiscarded   = LTE_GET_U32BIT(&data_p[current]); 
	PDCP_LOG(LOG_DUMP, "\t\t\t DRB SDU Discarded..........:%u",drbSduDiscarded); 
	//lteLog(LOG_MAJOR,PNULL,"\t\t\t DRB SDU Discarded : %u",drbSduDiscarded);
	current += 4;


	*curr = current;
}
void printRohcStats(UInt16 paramIdx,UInt32 paramItem)
{
    switch(paramIdx)
    {
        case 0:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of IR Packets Processed................",paramItem);
            break;
        case 4:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of IR Packets Created..................",paramItem);
            break;
        case 8:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of IRDyn Packets Processed.............",paramItem);
            break;
        case 12:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of IRDyn Packets Created...............",paramItem);
            break;
        case 16:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of ACK Processed.......................",paramItem);
            break;
        case 20:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of ACK Created.........................",paramItem);
            break;
        case 24:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of NACK Processed......................",paramItem);
            break;
        case 28:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of NACK Created........................",paramItem);
            break;
        case 32:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Static NACK Processed...............",paramItem);
            break;
        case 36:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Static NACK Created.................",paramItem);
            break;
        case 40:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition UToR Created........",paramItem);
            break;
        case 44:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition UToO Created........",paramItem);
            break;
        case 48:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition OToR Created........",paramItem);
            break;
        case 52:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition OtoU Created........",paramItem);
        break;
        case 56:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition RToU Created........",paramItem);
            break;
        case 60:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition RToO Created........",paramItem);
            break;
        case 64:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition UToR Processed......",paramItem);
            break;
        case 68:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition UToO Processed......",paramItem);
            break;
        case 72:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition OToR Processed......",paramItem);
            break;
        case 76:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition OtoU Processed......",paramItem);
        break;
        case 80:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition RToU Processed......",paramItem);
            break;
        case 84:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Mode Transition RToO Processed......",paramItem);
            break;
        case 88:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Rejected....................",paramItem);
            break;
        case 92:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Times CRC Varification Failed.......",paramItem);
            break;
        case 96:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-0 Packets Processed...............",paramItem);
            break;
        case 100:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-0-CRC Packets Processed...........",paramItem);
            break;
        case 104:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-0 Packets Processed..............",paramItem);
            break;
        case 108:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1 Packets Processed...............",paramItem);
            break;
        case 112:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1-ID Packets Processed............",paramItem);
            break;
        case 116:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1-TS Packets Processed............",paramItem);
            break;
        case 120:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1 Packets Processesd.............",paramItem);
            break;
        case 124:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1-ID Packets Processed...........",paramItem);
            break;
        case 128:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1-TS Packets Processed...........",paramItem);
            break;
        case 132:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UOR-2 Packets Processed.............",paramItem);
            break;
        case 136:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UOR-2-ID Packets Processed..........",paramItem);
            break;
        case 140:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UOR-2-TS Packets Proceessed.........",paramItem);
            break;
        case 144:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Normal Packets Processed............",paramItem);
            break;
        case 148:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-0 Packets Created.................",paramItem);
            break;
        case 152:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-0-CRC Packets Created.............",paramItem);
            break;
        case 156:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-0 Packets Created................",paramItem);
            break;
        case 160:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1 Packets Created.................",paramItem);
            break;
        case 164:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1-ID Packets Created..............",paramItem);
            break;
        case 168:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of R-1-TS Packets Created..............",paramItem);
            break;
        case 172:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1 Packets Created................",paramItem);
            break;
        case 176:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1-ID Packets Created.............",paramItem);
            break;
        case 180:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UO-1-TS Packets Created.............",paramItem);
            break;
        case 184:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UOR-2-ID Packets Created............",paramItem);
            break;
        case 188:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of UOR-2-ID Packets Created............",paramItem);
            break;
        case 192:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber of UOR-2-TS Packets Created............",paramItem);
            break;
        case 196:
            PDCP_LOG(LOG_DUMP, "%s :%u\n\n\n","\tNumber Of Normal Packets Created............",paramItem);
            break;
    }
}
void printCipheringStats(UInt16 paramIdx,UInt32 paramItem)
{

    switch(paramIdx)
    {
        case 0:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Ciphered Using EEA0..............",paramItem);
            break;
        case 4:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Deciphered Using EEA0............",paramItem);
            break;
        case 8:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Ciphered Using EEA1..............",paramItem);
            break;
        case 12:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Deciphered Using EEA1............",paramItem);
            break;
        case 16:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Ciphered Using EEA2..............",paramItem);
            break;
        case 20:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Deciphering Using EEA2...........",paramItem);
            break;
        case 24:
            PDCP_LOG(LOG_DUMP, "%s :%u\n\n\n","\tNumber Of Packets Rejected.........................",paramItem);
            break;
    }

}

/****************************************************************************
 * Function Name  : printIntegrityStats
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :To print Integrity stats

 ****************************************************************************/

void printIntegrityStats(UInt16 paramIdx,UInt32 paramItem)
{
    switch(paramIdx)
    {
        case 0:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Protected Using EIA0.........",paramItem);	    
            break;
        case 4:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Verified Using EIA0..........",paramItem);
            break;

        case 8:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Protected Using EIA1.........",paramItem);
            break;

        case 12:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Verified Using EIA1..........",paramItem);
            break;

        case 16:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Protected Using EIA2.........",paramItem);
            break;

        case 20:
            PDCP_LOG(LOG_DUMP, "%s :%u","\tNumber Of Packets Integrity Verified Using EIA2..........",paramItem);
            break;

        case 24:
	    PDCP_LOG(LOG_DUMP, "%s :%u\n\n\n","\tNumber Of Packets Rejected...............................",paramItem);
            break;

    }

}

/************************************************************************************
 * FUNCTION NAME    :   userPrintZCBBuffer
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols
 * PARAMATERS       :   [IN] void* zcb_p - ZCB buffer
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before 
 *                      hex digits or not
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintZCBBuffer(const UChar8* strHeader, void* zcb_p, UInt32 fmt)
{
#ifdef UT_TESTING
    UInt32 msize = 0;
    UInt8* ext_msg = PNULL;

    assert(zcb_p);
    assert(strHeader);

    if ( LOG_DUMP > pdcpGetLogLevel() ) 
    {
        /* Log disabled */
        return;
    }

    /* extract message */
    msize = msgSize(zcb_p, PNULL);
    ext_msg = getMemFromPool(msize,PNULL);
    assert(ext_msg);
    msgExtract(zcb_p, 0, ext_msg, msize);
    userPrintRawBuffer(strHeader,ext_msg,msize,fmt);
    freeMemPool(ext_msg);
#endif
}

/************************************************************************************
 * FUNCTION NAME    :   userIniKeyPresent
 * DESCRIPTION      :   Checks if string has KEY=VALUE format
 * PARAMATERS       :   UChar8 *keyName,UChar8 *string
 * RETURN VALUE     :   address of value (after '='), PNULL if no
 ************************************************************************************/
UChar8 *userIniKeyPresent(UChar8 *keyName,UChar8 *string)
{
	UInt32 keyLen=(UInt32)strlen(keyName);
	if(0==strNCmp(keyName,string,keyLen))
	{
		if('='==string[keyLen])
		{
			return string+keyLen+1;
		}
	}
	return PNULL;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStringsFromCfgFile
 * DESCRIPTION      :   Reading configuration file and getting all strings with
 *                      configuration params from it, skipping comments,
 *                      started from symbol '#'
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStringsFromCfgFile(const UChar8* cfg_file)
{
    UChar8*     str_buf = PNULL;
    UChar8**    str_arr = PNULL;
    
    assert(cfg_file);
    
    str_buf = userGetFileStringBuf(cfg_file);
    if(PNULL == str_buf)
    {
        return str_arr;
    }
    str_arr = userGetListStrings(str_buf, "\n", '#');
    freeMemPool(str_buf);
    return str_arr;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStrings
 * DESCRIPTION      :   
 * PARAMATERS       :   [IN] const UChar8* str_buf
 *                      [IN] const UChar8* str_delim
 *                      [IN] UChar8 comment_symbol
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStrings(const UChar8* str_buf, const UChar8* str_delim, UChar8 comment_symbol)
{
    UInt32  i = 0, nlines  = 0;
    UChar8* token = PNULL;
    UChar8 loc_buf[60000] = {0};
    UChar8**        str_arr = PNULL;
    assert(str_buf);
    assert(str_delim);
    strcpy(loc_buf, str_buf);
    nlines = userGetNumLinesInStrBuf(loc_buf, str_delim[0]);
    if(0 == nlines)
        return str_arr;
    str_arr = getMemFromPool((nlines+1)*sizeof(UInt8*),PNULL);
    assert(str_arr);
    token = strtok(loc_buf, str_delim);
    while( token != PNULL )
    {
        if(token[0] != comment_symbol)
        {
            /* we must reserve memory for nul symbol in the end of array*/
            /* otherwise this memory can be heap allocated for another  */
            /* needs and our nul will be destroyed                      */
            str_arr[i] = getMemFromPool(strlen(token)+1, PNULL);
            strcpy(str_arr[i],token);            
            i++;
        }
        token = strtok(PNULL, str_delim);
    }
    str_arr[i] = PNULL;
    return str_arr;    
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStrings
 * DESCRIPTION      :   
 * PARAMATERS       :   [IN] cconst UChar8** str_arr
 * RETURN VALUE     :   [OUT] UInt32 - number of strings in  array
 ************************************************************************************/
UInt32 userGetListStringsCount(const UChar8** str_arr)
{
    UInt32 i=0;
    assert(str_arr);
    for(i=0; str_arr[i]; i++);
    return i;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetNumLinesInStrBuf
 * DESCRIPTION      :   Calculalates number of lines in string buffer
 * PARAMATERS       :   [IN] const UChar8* str_buf
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetNumLinesInStrBuf(const UChar8* str_buf, UChar8 str_delim)
{
    UInt32 i,j;
    assert(str_buf);
    for(i=0,j=0; str_buf[i]; i++)
    {
        /* SPR 2408 Fix Starts */
        if( str_buf[i+1] == '\r' && str_buf[i] == str_delim  &&  str_buf[i-1] == '\r' )
	{
            PDCP_LOG(LOG_DUMP, "file lteLayer2PortConfig.cfg not Compatible With the Release File : Exiting Now");
            abort();
        }
        /* SPR 2408 Fix Ends */
	if(str_buf[i] == str_delim)
        {
            j++;
        }
    }
    if(str_buf[strlen(str_buf)-1] != str_delim)
    {
        j++;
    }
    return j;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetFileLen
 * DESCRIPTION      :   Calculalates length of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetFileLen(const UChar8* file_name)
{
    FILE*       fd      =   PNULL;
    UInt32      flen    = 0;
    
    assert(file_name);
    fd = fopen(file_name, "r");
    if(PNULL == fd)
    {
        PDCP_LOG( LOG_WARNING, "Unable to open %s", file_name);
        return flen;
    }
    fseek(fd, 0, SEEK_END);
    flen = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    fclose(fd);
    return flen;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetFileStringBuf
 * DESCRIPTION      :   Returns string content of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UChar8*
 ************************************************************************************/
UChar8* userGetFileStringBuf(const UChar8* file_name)
{
    FILE*       fd      = PNULL;
    UInt32      flen    = 0;
    UChar8*     buf     = PNULL;
    
    assert(file_name);
    flen = userGetFileLen(file_name);
    if(0 == flen)
    {
        PDCP_LOG( LOG_WARNING, "Length of %s is 0", file_name);
        return buf;
    }
    fd = fopen(file_name, "r");
    if(PNULL == fd)
    {
        PDCP_LOG( LOG_WARNING, "Unable to open %s", file_name);
        return buf;
    }
    
    buf = getMemFromPool(flen+1,PNULL);    
    assert(buf);
    flen = (UInt32)fread(buf, 1, flen, fd);
    buf[flen] = 0;
    fclose(fd);
    return buf;
}

/************************************************************************************
 * FUNCTION NAME    :   userFreeListStrings
 * DESCRIPTION      :   Frees array of heap allocated strings
 * PARAMATERS       :   [IN] UChar8** strings - array with strings
 * RETURN VALUE     :   None
 ************************************************************************************/
void userFreeListStrings(UChar8** strings)
{
    UInt32 i;
    assert(strings);
    
    for(i=0; strings[i]; i++)
    {
        freeMemPool(strings[i]);
    }
    freeMemPool(strings);
}

/************************************************************************************
 * FUNCTION NAME    :   userSleep
 * DESCRIPTION      :   Sleep msec time
 * PARAMATERS       :   [IN] UInt32 msec - time to sleep
 * RETURN VALUE     :   None
 ************************************************************************************/
void userSleep(UInt32 msec)
{
	msec*=DELAY_SIMU;
#ifdef WIN32
    Sleep(msec);
#elif LINUX_PPC
	struct timeval tv;
	UInt32 stamp1;
	UInt32 stamp2;
	msec*=10; /*for slow device*/
	sleep(msec/1000);
	if(msec>0)
	{
		msec%=1000;
		gettimeofday(&tv, PNULL);
		stamp1=(tv.tv_usec/1000) + (tv.tv_sec&0xFFFF)*1000;
		while(1)
		{
			gettimeofday(&tv, PNULL);
			stamp2=(tv.tv_usec/1000) + (tv.tv_sec&0xFFFF)*1000;
			if((stamp2-stamp1)>msec)
			{
				break;
			}
			if(stamp2<stamp1)
			{
				break;
			}
		}
	}
#else
    usleep(msec*1000);
#endif
}

/************************************************************************************
 * FUNCTION NAME    :   userTolower
 * DESCRIPTION      :   Cast string to lowercase
 * PARAMATERS       :   [OUT] UChar8* str - string for cast
 * RETURN VALUE     :   None
 ************************************************************************************/
void userTolower(UChar8* str)
{
    assert(str);
    while( (*(str) = tolower(*str)) != 0 )
    {
       str++;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   prepareTlv
 * DESCRIPTION      :   Parses confuguration file, and creates buffer with parameters
 *                      for callback
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file for
 *                      callback
                        [OUT] UInt8* msgBuffer_p - pointer to message buffer
                        [OUT] UInt16* size - size of message
 * RETURN VALUE     :   [OUT] void* - pointer to buffer with parameters for
 *                      callback
 ************************************************************************************/
void* prepareTlv(const UChar8* cfg_fname, UInt8* msgBuffer_p, UInt16* size)
{
    UChar8      *token = PNULL;
    Int			i=0;
    UInt16      len = 0;
	UChar8**    str_arr = PNULL;
	Int			line_num = 0;
	
    const UChar8* space = "\x20\x9,";
	const Int buf_size=1024*8;

    assert(msgBuffer_p);

	if ( PNULL == cfg_fname )
	{
		return PNULL;
	}
        
    str_arr = userGetListStringsFromCfgFile(cfg_fname);
    if ( PNULL == str_arr )
    {
        return PNULL;
    }
    
	for(line_num=0;PNULL!=str_arr[line_num];line_num++)
	{
		token = strtok(str_arr[line_num], space);
		while( token != PNULL )
		{
			if(i>=buf_size)
			{
				PDCP_LOG( LOG_WARNING, "%s:too long cfg file. Max length=%i",__FUNCTION__,buf_size);
				break;
			}
			if(isxdigit((UInt8)token[0]))
			{
				msgBuffer_p[i++] = (UInt8)strtol(token, PNULL, 16);
			}
			token = strtok(PNULL, space);
		}
	}

    if ( sizeof(TlvHeader) > i )
    {
        PDCP_LOG( LOG_WARNING, "CFG_PARSER: Invalid message size. file[%s] size[%u]", cfg_fname, i );
    }
    else 
    {
	len = LTE_GET_U16BIT(&msgBuffer_p[8]);
        if ( len != i )
	    {
		    PDCP_LOG( LOG_WARNING, 
                "CFG_PARSER: Invalid length in TLV header. file[%s] tlvLength[%u] fileLength[%u]",
                cfg_fname, len, i );
	    }
    }
	userFreeListStrings(str_arr);
    
    *size = i;

    return msgBuffer_p;
}

void printRlcStatus(UInt8 *data_p,UInt16 *curr,UInt16 msgLen)
{
    rlcUeStatus rlcue[NUM_OF_UE];
    UInt16 i = 0;
    UInt16 current = *curr;
    UInt8 numRxEntities = 0,numTxEntities = 0;
    UInt8 lcId = 0;
    //UInt8 maxRxThresHold = 0, pollPDU = 0, pollByte = 0, isReTxTimerRunning = 0;
    //UInt8 sendStatusInd = 0, isRTimerRunning = 0, isSPTimerRunning = 0;
    UInt8 lcStatusPresent = *(data_p + current);
    UInt16 snFieldLength = 0, tReordering = 0, crnti = 0, length=0,tag=0, numOfActiveUE=0, sublength = 0;
    UInt16 ueUR = 0, ueUS = 0, ueUX = 0, ueUH =0;
    //UInt16 vtA = 0, vtMS = 0, vtS = 0, pollSN = 0, pduWithoutPoll = 0, byteWithoutPoll = 0;
    //UInt32 txQsize = 0, reTxQsize = 0, rxStatusPduSize = 0;
    //UInt16 vrR = 0, vrMR = 0, vrX = 0, vrMS = 0, vrH = 0;
    UInt32 tx_count = 0, rx_index = 0,rx_count = 0,tx_index = 0;
    rlcstatistics rlcstats;
    rlcstats.TMstats = 0;
    rlcstats.UMstats = 0;
    rlcstats.AMstats = 0;
    rlcstats.UEstats = 0;
    rlcstats.PFstats = 0;
    ++current;
    if (msgLen < 3)
    {
        PDCP_LOG(LOG_DUMP, "%s", "No Entities Present to Display\n");
    } else {
    msgLen -=3; // For response code
    while(msgLen >0 )
    {
        tx_count = 0;
        rx_count = 0;
        tag= LTE_GET_U16BIT(data_p + current);
        current += 2;
        length = LTE_GET_U16BIT(data_p + current);
        current += 2;
        msgLen -= length; 
        if(RLC_UE_STATUS == tag)
        {
            crnti = LTE_GET_U16BIT(data_p + current);
            rlcue[numOfActiveUE].crnti = crnti;
            current += 2;
            numRxEntities = *(data_p + current);
            rlcue[numOfActiveUE].numRxEntities = numRxEntities;
            current += 1;
            numTxEntities = *(data_p + current);
            rlcue[numOfActiveUE].numTxEntities = numTxEntities;
            current += 1;
            length -= 8;
            while(length >= 7)
            {
                    tag = LTE_GET_U16BIT(data_p + current);
                current += 2;
                    sublength = LTE_GET_U16BIT(data_p + current);
                current += 2;

                length -= sublength;
                if(RLC_TX_UM_ENTITY_STATUS == tag)
                {
                    lcId = *(data_p + current);
                    rlcue[numOfActiveUE].TXumentity[tx_count].lcId = lcId;
                    current += 1;

                       snFieldLength = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].TXumentity[tx_count].SnfieldLength = snFieldLength;
                    current += 2;
                       ueUS = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].TXumentity[tx_count].US = ueUS;
                    current += 2;
                    tx_count++;
                }
                else if(RLC_RX_UM_ENTITY_STATUS == tag)
                {
                    lcId = *(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].lcId = lcId;
                    current += 1;
                       snFieldLength = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].SnfieldLength = snFieldLength;
                    current += 2;
                       tReordering = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].tReordering = tReordering;
                    current += 2;
                       ueUR = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].UR = ueUR;
                    current += 2;
                       ueUH = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].UH = ueUH;
                    current += 2;
                       ueUX = LTE_GET_U16BIT(data_p + current);
                    rlcue[numOfActiveUE].RXumentity[rx_count].UX = ueUX;
                    current += 2;
                    
                       rx_count++;
                   }
                   else if (RLC_TX_AM_ENTITY_STATUS == tag)          
                   {
                    
                       fprintf(stderr,"RLC_TX_AM_ENTITY_STATUS\n");
                    // lcId = *(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].lcId =  *(data_p + current);
                       current += 1;
                    // vtA = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].VTA = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                                                                                
                    // vtMS = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].VTMS = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                                                                                
                    // vtS = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].VTS = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //pollSN = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].PollSN = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //pduWithoutPoll = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].PduWithoutPoll = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //byteWithoutPoll = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].ByteWithoutPoll = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //maxRxThresHold = *(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].MaxRetxThreshold = *(data_p + current);
                       current += 1;
                    
                    //pollPDU =  *(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].PollPDU = *(data_p + current);
                       current += 1;
                    
                    //pollByte =  *(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].PollByte = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //isReTxTimerRunning = *(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].IsRetransmitTimerRunning = *(data_p + current);
                       current += 1;
                    
                    //txQsize = LTE_GET_U32BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].TxQueueSize = LTE_GET_U32BIT(data_p + current);
                       current += 4;
                    
                    //reTxQsize = LTE_GET_U32BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].ReTxQueueSize = LTE_GET_U32BIT(data_p + current);
                       current += 4;
                    
                    //rxStatusPduSize = LTE_GET_U32BIT(data_p + current);
                       rlcue[numOfActiveUE].TXamentity[tx_count].RxStatusPduSize = LTE_GET_U32BIT(data_p + current);
                       current += 4;
                    
                       tx_count++;
                   }
                   else if (RLC_RX_AM_ENTITY_STATUS == tag)
                   {
                       fprintf(stderr,"RLC_RX_AM_ENTITY_STATUS\n");
                    //lcId = *(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].lcId = *(data_p + current);
                       current += 1;
                      
                    //sendStatusInd = *(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].SendStatusInd = *(data_p + current);
                       current += 1;
                                                                
                    //vrR = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].VRR = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                                                                                      
                    //vrMR = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].VRMR = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //vrX = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].VRX = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //vrMS = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].VRMS = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //vrH = LTE_GET_U16BIT(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].VRH = LTE_GET_U16BIT(data_p + current);
                       current += 2;
                    
                    //isRTimerRunning =  *(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].IsRTimerRunning =  *(data_p + current);
                       current += 1;
                                                                
                    //isSPTimerRunning =  *(data_p + current);
                       rlcue[numOfActiveUE].RXamentity[rx_count].IsSPTimerRunning =  *(data_p + current);
                       current += 1;
                    
                    rx_count++;
                }    
            }           
            numOfActiveUE++;
        }
        else
        {
                 //Error
        }
    }  
    }
    if(numOfActiveUE) 
    {
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
        PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------RLC UE STATUS-------------------------------------*");
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
        if(!lcStatusPresent)
        {
            PDCP_LOG(LOG_DUMP,"%s","===========================================================================================\n\n");
            PDCP_LOG(LOG_DUMP,"%s","\tC-RNTI\t\t\tNO OF RX ENTITY\t\t\tNO OF TX ENTITY\n");
            PDCP_LOG(LOG_DUMP,"%s","===========================================================================================\n");
        }
        PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
        for(i=0;i<numOfActiveUE;i++)
        {
           if(lcStatusPresent)
           {
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n\n");
               PDCP_LOG(LOG_DUMP,"%s","\tC-RNTI\t\t\tNO OF RX ENTITY\t\t\tNO OF TX ENTITY\n");
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n");
           }
           PDCP_LOG(LOG_DUMP,"\t%d\t\t\t %d\t\t\t\t  %d\n",
              rlcue[i].crnti, rlcue[i].numRxEntities, rlcue[i].numTxEntities);
           PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
           if (lcStatusPresent && rlcue[i].numRxEntities)
           {
               PDCP_LOG(LOG_DUMP,"%s","*************************************Rx UM ENTITY STATUS *******************************\n\n");
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n");
               PDCP_LOG(LOG_DUMP,"%s","LcId\tSN FIELD LEN\t\tReordering\t\tUR\t\tUH\t\tUX\n");
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n");
               for (rx_index = 0 ; rx_index < rlcue[i].numRxEntities; rx_index++)
               {
                    if( 0 != rlcue[i].RXumentity[rx_index].lcId)
                    {
                        PDCP_LOG(LOG_DUMP,"\t %d\t\t %d\t\t  %d\t\t\t %d\t\t  %d\t\t  %d\n",
                      rlcue[i].RXumentity[rx_index].lcId,
                      rlcue[i].RXumentity[rx_index].SnfieldLength,
                      rlcue[i].RXumentity[rx_index].tReordering, 
                      rlcue[i].RXumentity[rx_index].UR,
                      rlcue[i].RXumentity[rx_index].UH,
                      rlcue[i].RXumentity[rx_index].UX);
               }
           }
      	       PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
               
               PDCP_LOG(LOG_DUMP,"%s","*************************************Rx AM ENTITY STATUS *******************************\n\n");
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n");
               PDCP_LOG(LOG_DUMP,"%s","LcId\tSendStatusInd\t VRR\t VRMR\t VRX\t VRMS\t VRH\tIsRTmrRunning\tIsSPTmrRunning\n");
               PDCP_LOG(LOG_DUMP,"%s","========================================================================================\n");
               for (rx_index = 0 ; rx_index < rlcue[i].numRxEntities; rx_index++)
               {
                    if (0 != rlcue[i].RXamentity[rx_index].lcId)
                    {
                        PDCP_LOG(LOG_DUMP,"\t%d\t   %d\t\t  %d\t  %d\t   %d\t   %d\t   %d\t    %d\t\t    %d\n",
                            rlcue[i].RXamentity[rx_index].lcId,
                            rlcue[i].RXamentity[rx_index].SendStatusInd,
                            rlcue[i].RXamentity[rx_index].VRR,
                            rlcue[i].RXamentity[rx_index].VRMR,
                            rlcue[i].RXamentity[rx_index].VRX,
                            rlcue[i].RXamentity[rx_index].VRMS,
                            rlcue[i].RXamentity[rx_index].VRH,
                            rlcue[i].RXamentity[rx_index].IsRTimerRunning,
                            rlcue[i].RXamentity[rx_index].IsSPTimerRunning);
                    }
               }
      	       PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
           }
          if (lcStatusPresent && rlcue[i].numTxEntities)
          { 
              PDCP_LOG(LOG_DUMP,"%s","\n***************************** TX ENTITY STATUS ***********************************\n");
              PDCP_LOG(LOG_DUMP,"%s","===================================================================================\n\n");
              PDCP_LOG(LOG_DUMP,"%s","\tLcId\t\t\tSN FIELD LEN\t\t\tUS\n");
              PDCP_LOG(LOG_DUMP,"%s","====================================================================================\n");
              for (tx_index = 0 ; tx_index < rlcue[i].numTxEntities; tx_index++)
              {
                 if(0 != rlcue[i].TXumentity[tx_index].lcId)
                 {
                    PDCP_LOG(LOG_DUMP,"\t%d     \t\t\t\t %d   	  \t \t%d\n",
                        rlcue[i].TXumentity[tx_index].lcId,
                        rlcue[i].TXumentity[tx_index].SnfieldLength,
                        rlcue[i].TXumentity[tx_index].US);
              }
          }
      	      PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
              PDCP_LOG(LOG_DUMP,"%s","***************************** TX AM ENTITY STATUS ***************************************\n");
              PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
              PDCP_LOG(LOG_DUMP,"%s","LcId VTA VTMS VTS PollSN PduWoPoll ByteWoPoll MaxThrshold PollPdu PollByte IsReTxTmrRun TxQsz ReTxQsz RxStatusPduSz\n");
              
              PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n");
              for (tx_index = 0 ; tx_index < rlcue[i].numTxEntities; tx_index++)
              {
                if(0 != rlcue[i].TXamentity[tx_index].lcId)
                {
                    PDCP_LOG(LOG_DUMP,"%3d  %3d  %3d %3d   %3d    %3d       %3d       %2d           %3d     %4d    %3d         %3d    %3d    %3d\n",
                        rlcue[i].TXamentity[tx_index].lcId,
                        rlcue[i].TXamentity[tx_index].VTA,
                        rlcue[i].TXamentity[tx_index].VTMS,
                        rlcue[i].TXamentity[tx_index].VTS,
                        rlcue[i].TXamentity[tx_index].PollSN,
                        rlcue[i].TXamentity[tx_index].PduWithoutPoll,
                        rlcue[i].TXamentity[tx_index].ByteWithoutPoll,
                        rlcue[i].TXamentity[tx_index].MaxRetxThreshold,
                        rlcue[i].TXamentity[tx_index].PollPDU,
                        rlcue[i].TXamentity[tx_index].PollByte,
                        rlcue[i].TXamentity[tx_index].IsRetransmitTimerRunning,
                        rlcue[i].TXamentity[tx_index].TxQueueSize,
                        rlcue[i].TXamentity[tx_index].ReTxQueueSize,
                        rlcue[i].TXamentity[tx_index].RxStatusPduSize);
                }
      }
           }
      }
      PDCP_LOG(LOG_DUMP,"%s","=========================================================================================\n\n");
   }
}



void printRlcStats(UInt8 *data_p,UInt16 *curr,UInt16 msgLen)
{
    UInt16 current = *curr;
    rlcstatistics rlcstats;
    rlcstats.TMstats = 0;
    rlcstats.UMstats = 0;
    rlcstats.AMstats = 0;
    rlcstats.UEstats = 0;
    rlcstats.PFstats = 0;
    UInt32 rxLoadPerfstat = 0, txLoadPerfstat = 0,  count_stat = 0, index_stat = 0, txTmdPdu=0, rxUmdSduDiscarded=0;
    UInt16 crnti = 0,response = 0, length = 0,tag = 0, numOfActiveUE = 0;  
    UInt32 rxTmdPdu=0, sduTransmitted = 0, sduReceivedFromUl = 0;
    UInt32 amdPduTransmitted = 0, amdPduRecieved = 0, amdPduDropped = 0;
    UInt32 txUmdPdu=0, rxUmdSduTxUL=0, rxIncompleteUmdSduDiscarded=0, rxUmdPdu=0, rxUmdPduDropped=0, rxOutofOrderPDU=0;
    UInt32 sduReceived=0, pduTransmitted=0, subLength = 0;
    msgLen -= 2;
    while(msgLen > 0)
    {
        tag= LTE_GET_U16BIT(&data_p[current]);;
        current += 2;
        length = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        msgLen -= length; 
        if(GET_TM_STATS_CNF  == tag)
        {
            rxTmdPdu= LTE_GET_U32BIT(&data_p[current]);
            rlcstats.tmstats.rxTmdPdu = rxTmdPdu;
            current += 4;

            txTmdPdu= LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.tmstats.txTmdPdu = txTmdPdu;
            current += 4;
            rlcstats.TMstats++;
        }
        if(GET_UM_STATS_CNF == tag)
        {
            rxUmdSduDiscarded = LTE_GET_U32BIT(&data_p[current]);
            rlcstats.umstats.rxUmdSduDiscarded = rxUmdSduDiscarded;
            current += 4;

            txUmdPdu = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.txUmdPdu = txUmdPdu;
            current += 4;

            rxUmdSduTxUL = LTE_GET_U32BIT(&data_p[current]);
            rlcstats.umstats.rxUmdSduTxUL = rxUmdSduTxUL;
            current += 4;

            rxIncompleteUmdSduDiscarded = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.rxIncompleteUmdSduDiscarded = rxIncompleteUmdSduDiscarded;
            current += 4;

            rxUmdPdu = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.rxUmdPdu = rxUmdPdu;
            current += 4;

            rxUmdPduDropped = LTE_GET_U32BIT(&data_p[current]);
            rlcstats.umstats.rxUmdPduDropped = rxUmdPduDropped;
  
            current += 4;
            sduReceivedFromUl = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.sduReceivedFromUl = sduReceivedFromUl;
            current += 4;

            sduTransmitted = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.sduTransmitted = sduTransmitted;
        
            current += 4;
            rxOutofOrderPDU = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.umstats.rxOutofOrderPDU = rxOutofOrderPDU;

            current += 4;
            rlcstats.UMstats++;
        }
        if(GET_AM_STATS_CNF == tag)
        {
            amdPduTransmitted = *(data_p + current);
            amdPduTransmitted |= (((*(data_p + current + 1)) << 8) & 0xFF00);
            amdPduTransmitted |= (((*(data_p + current + 2)) << 16) & 0x00FF0000);
            amdPduTransmitted |= (((*(data_p + current + 3)) << 24) & 0xFF000000);
            rlcstats.amstats.amdPduTransmitted = amdPduTransmitted;
            
            current += 4;
            amdPduRecieved = *(data_p + current);
            amdPduRecieved |= (((*(data_p + current + 1)) << 8) & 0xFF00);
            amdPduRecieved |= (((*(data_p + current + 2)) << 16) & 0x00FF0000);
            amdPduRecieved |= (((*(data_p + current + 3)) << 24) & 0xFF000000);
            rlcstats.amstats.amdPduReceived = amdPduRecieved;
            
            current += 4;
            amdPduDropped = *(data_p + current);
            amdPduDropped |= (((*(data_p + current + 1)) << 8) & 0xFF00);
            amdPduDropped |= (((*(data_p + current + 2)) << 16) & 0x00FF0000);
            amdPduDropped |= (((*(data_p + current + 3)) << 24) & 0xFF000000);
            rlcstats.amstats.amdPduDropped = amdPduDropped;

            current += 4;
            rlcstats.AMstats++;
        }

        if(GET_UE_STATS_CNF == tag)
        {
           response = LTE_GET_U16BIT(&data_p[current]); 
           current += 2;
           if(response != RLC_SUCCESS) 
              break;
           numOfActiveUE = LTE_GET_U16BIT(&data_p[current]); 
           current += 2;
           rlcstats.uestats.numofactiveue = numOfActiveUE;
           length -= 8;
           while(length)
           {
               tag = LTE_GET_U16BIT(&data_p[current]); 
               current += 2;
               subLength = LTE_GET_U16BIT(&data_p[current]); 
               current += 2;
               crnti = LTE_GET_U16BIT(&data_p[current]); 
               rlcstats.uestats.uestats_cnf[count_stat].crnti = crnti;
               current += 2;

               sduReceived = LTE_GET_U32BIT(&data_p[current]); 
               rlcstats.uestats.uestats_cnf[count_stat].sdurx = sduReceived;
               current += 4;

               pduTransmitted = LTE_GET_U32BIT(&data_p[current]); 
               rlcstats.uestats.uestats_cnf[count_stat].pdutx = pduTransmitted;
               current += 4;

               length -= subLength;
               count_stat++;
           }
           rlcstats.UEstats++;
        }
        if (GET_LOAD_STATS_CNF == tag)
        {
            rxLoadPerfstat = LTE_GET_U32BIT(&data_p[current]);
            rlcstats.pfstats.rxLoadPerfstats = rxLoadPerfstat;
            current += 4;
			
            txLoadPerfstat = LTE_GET_U32BIT(&data_p[current]); 
            rlcstats.pfstats.txLoadPerfstats = txLoadPerfstat;
            current += 4;
            rlcstats.PFstats++;
        }
   }
   PDCP_LOG(LOG_DUMP,"%s","################### RLC STATS START ####################################################### \n\n");
   PDCP_LOG(LOG_DUMP,"%s","=====================================================================================================\n\n");
   if( rlcstats.TMstats) 
   {
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
       PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------RLC TM STATS-------------------------------------*");
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
       PDCP_LOG(LOG_DUMP,"%s","\tTMD PDU [MAC => RLC]\t\t\t\tTMD PDU [RLC => MAC]\n");
       PDCP_LOG(LOG_DUMP,"%s","===================================================================================================\n");
       PDCP_LOG(LOG_DUMP,"%s","\t  %d\t\t\t\t  %d\n\n",rlcstats.tmstats.rxTmdPdu,rlcstats.tmstats.txTmdPdu);
   }
   if( rlcstats.PFstats) 
   {
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
       PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------PERFORMANCE STATS-------------------------------------*");
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
      PDCP_LOG(LOG_DUMP,"%s","\tRx Load PERM STATS\t\t\t\tTX Load PERM STATS\n");
      PDCP_LOG(LOG_DUMP,"%s","=================================================================================================\n");
      PDCP_LOG(LOG_DUMP,"\t  %d\t\t\t\t %d\n\n",rlcstats.pfstats.rxLoadPerfstats,rlcstats.pfstats.txLoadPerfstats);
    }

   if (rlcstats.UMstats)
   {
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
       PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------RLC UM STATS-------------------------------------*");
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");

      PDCP_LOG(LOG_DUMP,"%s","\tDISCARDRD RX UMD SDU \t\tDELIVERED UMD PDU [MAC]\t\t\tDELIVERED UMD SUD [PDCP]\t\t\tINCOMPLETE RX UMD\t\t\tRX NOT IN SYNC PDU\n");
      PDCP_LOG(LOG_DUMP,"%s","=======================================================================================================================\n");
      PDCP_LOG(LOG_DUMP,"%s","\t   %d\t\t\t %d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\n",
        rlcstats.umstats.rxUmdSduDiscarded, rlcstats.umstats.txUmdPdu,rlcstats.umstats.rxUmdSduTxUL,
	    rlcstats.umstats.rxIncompleteUmdSduDiscarded,rlcstats.umstats.rxOutofOrderPDU);
      PDCP_LOG(LOG_DUMP,"%s","=======================================================================================================================\n");
      PDCP_LOG(LOG_DUMP,"\tRx UMD PDU\t\t\tRx UMD PDU Dropped\t\t\tSDU RX [PDCP]\t\t\tSDU Transmitted\n");
      PDCP_LOG(LOG_DUMP,"%s","=================================================================================================\n");
      PDCP_LOG(LOG_DUMP,"\t   %d\t\t\t%d\t\t\t%d\t\t\t%d\n",
        rlcstats.umstats.rxUmdPdu,rlcstats.umstats.rxUmdPduDropped,rlcstats.umstats.sduReceivedFromUl,
	    rlcstats.umstats.sduTransmitted);
    }
   if (rlcstats.AMstats)
   {
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
       PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------RLC AM STATS-------------------------------------*");
       PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");

      PDCP_LOG(LOG_DUMP,"%s","\tTRANSMITTED AMD PDU \t\tRECIEVED AMD PDU\t\t\tDROPPED AMD PDU\n");
      PDCP_LOG(LOG_DUMP,"%s","=======================================================================================================================\n");
      PDCP_LOG(LOG_DUMP,"%s","\t   %d\t\t\t %d\t\t\t\t%d\n",amdPduTransmitted,amdPduRecieved,amdPduDropped);
    }  
    if(rlcstats.UEstats)
    {
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
        PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------RLC UE STATS-------------------------------------*");
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
        PDCP_LOG(LOG_DUMP,"Number of Active UE => %d  \n\n",rlcstats.uestats.numofactiveue);
        if(rlcstats.uestats.numofactiveue)
        {
            PDCP_LOG(LOG_DUMP, "%s", "\t\tC-RNTI\t\t|\t\tSDU Received\t\t|\t\tPDU Transmitted");
    	    PDCP_LOG(LOG_DUMP,"%s", "==========================================================================================\n");
        }    
        for (index_stat = 0;index_stat < count_stat; index_stat++)
        {
	    PDCP_LOG(LOG_DUMP,"\t\t  %d\t\t\t\t\t%d\t\t\t\t%d\n",rlcstats.uestats.uestats_cnf[index_stat].crnti,
               rlcstats.uestats.uestats_cnf[index_stat].sdurx,rlcstats.uestats.uestats_cnf[index_stat].pdutx);
        }
    }
    PDCP_LOG(LOG_DUMP,"%s","\n==================================================================================================================\n\n");
}

void printMacStatus(UInt8 *data_p,UInt16 *curr,UInt16 msgLen)
{
    macUeStatus macuestatus;
    macuestatus.numofActiveUE = 0;
    UInt32 count = 0, lc_count = 0, ul_lc_count = 0, ul_count = 0, index = 0, length = 0, index_lc = 0;
    UInt16 crnti = 0, tag = 0, macUeStatus=0, macUeStatusLen=0;  
    UInt16 current = *curr;
    /* SPR 15909 fix start */
    tickType_t ulLastScheduledTick=0,dlLastScheduledTick=0;
    UInt32 bsrNetOfLCG=0, ulCodingRate=0, queueLoad=0, dlCodingRate=0;
    /* SPR 15909 fix end */
    UInt8 lcId=0,lcgId=0, lcPriority=0,ulModScheme=0, ulMcsIndex=0, ulMaxRB=0,ulNumOfLCs=0, dlMcsIndex=0, dlModScheme=0;
    UInt32 bsrNet0=0,bsrNet1=0,bsrNet2=0,bsrNet3=0;
    UInt8 dlMaxRB=0,dlNumOfLCs=0,uePriority=0;
    while(current != msgLen)
    {
        macUeStatus = LTE_GET_U16BIT(&data_p[current]); 
        current += 2;
        macUeStatusLen = LTE_GET_U16BIT(&data_p[current]); 
        current += 2;
        lteLog(LOG_MAJOR,PNULL,"MAC UE Status tag = %d, len = %d",macUeStatus,macUeStatusLen);
        switch(macUeStatus)
        {
            case MAC_UE_STATUS:
               crnti = LTE_GET_U16BIT(&data_p[current]);
               macuestatus.dluestatus[count].crnti = crnti;
               current += 2;
               uePriority = *(data_p + current);
               macuestatus.dluestatus[count].uePriority = uePriority;
               current += 1;

               dlNumOfLCs = *(data_p + current);
               macuestatus.dluestatus[count].dlNumofLCs = dlNumOfLCs;
               current += 1;
               dlMaxRB = *(data_p + current);
               macuestatus.dluestatus[count].dlMaxRB = dlMaxRB ;
               current += 1;
               dlModScheme = *(data_p + current);
               macuestatus.dluestatus[count].dlModScheme = dlModScheme;
               current += 1;
               dlCodingRate= LTE_GET_U32BIT(&data_p[current]);
               macuestatus.dluestatus[count].dlCodingRate = dlCodingRate;
               current += 4;
               dlMcsIndex = *(data_p + current);
               macuestatus.dluestatus[count].dlMcsIndex = dlMcsIndex;
               current += 1;
               queueLoad = LTE_GET_U32BIT(&data_p[current]); 
               macuestatus.dluestatus[count].queueLoad = queueLoad;
               current += 4;
               dlLastScheduledTick= LTE_GET_U32BIT(&data_p[current]); 
               macuestatus.dluestatus[count].dlLastScheduleTick = dlLastScheduledTick;
               current += 4;
               ulNumOfLCs = *(data_p + current);
               macuestatus.uluestatus[ul_count].ulNumofLCs = ulNumOfLCs;
               current += 1;
               ulMaxRB = *(data_p + current);
               macuestatus.uluestatus[ul_count].ulMaxRB = ulMaxRB;
               current += 1;
               bsrNet0 = LTE_GET_U32BIT(&data_p[current]); 
               macuestatus.uluestatus[ul_count].bsrNet0 = bsrNet0;
               current += 4;
 
               bsrNet1 = LTE_GET_U32BIT(&data_p[current]); 
               macuestatus.uluestatus[ul_count].bsrNet1 = bsrNet1;
               current += 4;
               bsrNet2 = LTE_GET_U32BIT(&data_p[current]);
               lteLog(LOG_MAJOR,PNULL,"BSR Net 2:%d",bsrNet2);
               macuestatus.uluestatus[ul_count].bsrNet2 = bsrNet2;
               current += 4;
               bsrNet3 = LTE_GET_U32BIT(&data_p[current]);
               macuestatus.uluestatus[ul_count].bsrNet3 = bsrNet3;
               current += 4;
               ulModScheme = *(data_p + current);
               macuestatus.uluestatus[ul_count].ulModScheme = ulModScheme;
               current += 1;
               ulCodingRate = LTE_GET_U32BIT(&data_p[current]);
               macuestatus.uluestatus[ul_count].ulCodingRate = ulCodingRate ;
               current += 4;

               ulMcsIndex = *(data_p + current);
               macuestatus.uluestatus[ul_count].ulMcsIndex = ulMcsIndex;
               current += 1;

               ulLastScheduledTick = LTE_GET_U32BIT(&data_p[current]);
               macuestatus.uluestatus[ul_count].ulLastScheduleTick = ulLastScheduledTick;
               current += 4;
               macUeStatusLen -= 51;

               while(macUeStatusLen > 0)
               {
                   tag = LTE_GET_U16BIT(&data_p[current]); 
                   current += 2;
                   length= LTE_GET_U16BIT(&data_p[current]); 
                   current += 2;

                   macUeStatusLen -= length;
                   if(MAC_DL_LOG_CH_STATUS == tag)
                   {
                       lteLog(LOG_MAJOR,PNULL,"*****Downlink LC Info*****");
                       lcId = *(data_p + current);
                       macuestatus.dluestatus[count].lcinfo[lc_count].lcId = lcId;
                       current += 1;

                       lcPriority = *(data_p + current);
                       macuestatus.dluestatus[count].lcinfo[lc_count].lcPriority = lcPriority;
                       current += 1;
                       queueLoad = LTE_GET_U32BIT(&data_p[current]); 
                       macuestatus.dluestatus[count].lcinfo[lc_count].queueLoad = queueLoad;
                       current += 4;
                    }     
                    else if(MAC_UL_LOG_CH_STATUS == tag)
                    {
                        lcId = *(data_p + current);
                        macuestatus.uluestatus[ul_count].lcinfo[ul_lc_count].lcId = lcId;
                        current += 1;
                        lcgId = *(data_p + current);
                        macuestatus.uluestatus[ul_count].lcinfo[ul_lc_count].lcgId = lcgId;
                        current += 1;
                        bsrNetOfLCG= LTE_GET_U32BIT(&data_p[current]); 
                        macuestatus.uluestatus[ul_count].lcinfo[ul_lc_count].bsrNetofLCG = bsrNetOfLCG;
                        current += 4;
                    }    
                }    
                macuestatus.numofActiveUE++;
                count++;
                ul_count++;
                lc_count++;
                ul_lc_count++;
                break;
            }        
        }
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
        PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------MAC DL UE STATUS-------------------------------------*");
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");

        for (index = 0;index < macuestatus.numofActiveUE;index++)
        {
            PDCP_LOG(LOG_DUMP,"%s","\tUE\t\tCRNTI\t\tUE PRIORITY\t\tNO OF LC\t\tMAX RB\t\tMOD SCH\t\tCODING RATE\t\tMCS Index\t\tQueue Load\t\tLAST SCHEDULE TICK\n");
            PDCP_LOG(LOG_DUMP,"%s","==============================================================================================\n\n");
            PDCP_LOG(LOG_DUMP,"\t %d\t\t %d\t\t  %d\t\t   %d\t\t  %d\t\t  %d\t\t    %d\t\t   %d\t\t   %d\t\t      %d\n\n",
                   index,macuestatus.dluestatus[index].crnti,macuestatus.dluestatus[index].uePriority,
                   macuestatus.dluestatus[index].dlNumofLCs,macuestatus.dluestatus[index].dlMaxRB,
                   macuestatus.dluestatus[index].dlModScheme,macuestatus.dluestatus[index].dlCodingRate,
                   macuestatus.dluestatus[index].dlMcsIndex,macuestatus.dluestatus[index].queueLoad,
                   macuestatus.dluestatus[index].dlLastScheduleTick);

           PDCP_LOG(LOG_DUMP,"%s","\t\t\tLC Status\n\n");
           for(index_lc = 0;index_lc < lc_count;index_lc++)
           {
               PDCP_LOG(LOG_DUMP,"%s","\tLC-ID\t\t\tLC-PRIORITY\t\t\t QUEUE-LOAD");
               PDCP_LOG(LOG_DUMP,"\t  %d\t\t  %d\t\t  %d",
                   macuestatus.dluestatus[index].lcinfo[index_lc].lcId,
                   macuestatus.dluestatus[index].lcinfo[index_lc].lcPriority,
                   macuestatus.dluestatus[index].lcinfo[index_lc].queueLoad);
            }
        }
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
        PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------MAC UL UE STATUS-------------------------------------*");
        PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");
        for (index = 0;index < macuestatus.numofActiveUE;index++)
        {
           PDCP_LOG(LOG_DUMP,"%s","\tUE\t\tNum of LCs\t\tMAX RB\t\tBSR-NET0\t\tBSR-NET1\t\tBSR-NET2\t\tBSR-NET3\t\tMOD SCME\t\tCODING RATE\t\tMCS INDX\t\tLast SCHEDULE TICK\n");
           PDCP_LOG(LOG_DUMP,"%s","=================================================================================================\n\n");
           PDCP_LOG(LOG_DUMP,"\t %d\t\t %d\t\t %d\t\t  %d\t\t %d\t\t %d\t\t  %d\t\t   %d\t\t   %d\t\t  %d\t\t  %d\n\n",                   index, macuestatus.uluestatus[index].ulNumofLCs,macuestatus.uluestatus[index].ulMaxRB,
                   macuestatus.uluestatus[index].bsrNet0,macuestatus.uluestatus[index].bsrNet1,
                   macuestatus.uluestatus[index].bsrNet2,macuestatus.uluestatus[index].bsrNet3,
                   macuestatus.uluestatus[index].ulModScheme,macuestatus.uluestatus[index].ulCodingRate,
                   macuestatus.uluestatus[index].ulCodingRate,macuestatus.uluestatus[index].ulLastScheduleTick);
           PDCP_LOG(LOG_DUMP,"%s","*********************************LC Status******************************************\n\n");
           for(index_lc = 0;index_lc < lc_count;index_lc++)
           {
                PDCP_LOG(LOG_DUMP,"%s","\tLCId\t\t\tLCGid\t\t\tBSRNetofLCG");
                PDCP_LOG(LOG_DUMP,"\t %d\t\t\t  %d\t\t\t    %d",
                   macuestatus.uluestatus[index].lcinfo[index_lc].lcId,macuestatus.uluestatus[index].lcinfo[index_lc].lcgId,
                     macuestatus.uluestatus[index].lcinfo[index_lc].bsrNetofLCG);
           }
      }
}

void printMacStats(UInt8 *data_p,UInt16 *cur,UInt16 msgLen)
{
    UDouble32 dlt=0,ult=0;
    static UInt32 count = 0;
    UInt16 current = *cur;
    UInt16 length=0, throughputTag = 0;
    PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************");
    PDCP_LOG(LOG_DUMP, "%s", "*-----------------------------------------MAC UE STATS-------------------------------------*");
    PDCP_LOG(LOG_DUMP, "%s", "*********************************************************************************************\n");

    while(current != msgLen)
    {
        throughputTag = LTE_GET_U16BIT(&data_p[current]); 
        current += 2;
        length = LTE_GET_U16BIT(&data_p[current]); 
        current += 2;
        switch(throughputTag)
        {
             case MAC_DL_THROUGHPUT:
                memCpy(&dlt, &data_p[current], 4);
                PDCP_LOG(LOG_DUMP,"\t\tMAC Downlink Throughput = %f\n", dlt);
                current += 4;
                break;
            case MAC_UL_THROUGHPUT:
                memCpy(&ult, &data_p[current], 4);
                PDCP_LOG(LOG_DUMP,"\t\tMAC Uplink Throughput = %f\n", ult);
                current += 4;
                break;
         }        
    }    
    if ( startCaptureThroughput_g && (throughputFD_g != NULL)){
        fprintf(throughputFD_g,"\t\t%d,\t%6f,\t\t\t%6f\n",++count,dlt,ult);
        fflush(throughputFD_g);
    }
    else {
        //PDCP_LOG(LOG_DUMP,"File Named %s is not opened\n",throughputFile_g);
    }

}
