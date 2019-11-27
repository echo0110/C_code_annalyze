/* GPN Change Start */
#ifndef __GNU_SOURCE__
#define __GNU_SOURCE__
#define _GNU_SOURCE
/* GPN Change End */
#endif
#include "soapH.h"
#include "cwmp.nsmap"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <common_def.h>
#include <logger.h>
/* GPN Change Start */
#include <error.h>
#include <unistd.h>
#include <fcntl.h>
/* GPN Change End */

#define SOAP_ENCODE_DECODE "Soap_Encode_Decode"

/* SPR 22314 Start*/
void soap_all_attribute_free(struct soap *soap);

/* SPR 22314 End*/
static char *slurpfd(int fda);

int soap_decode(char *xml, int *type, void **resp,struct SOAP_ENV__Header **header )
{
	struct soap *soap;
	//sInformResponse *test = NULL;

	int fd[2];
	TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Entering in %s ",__FUNCTION__);
	TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "In %s \n RESP_XML = %s",__FUNCTION__,xml);

	pipe(fd);
	soap = soap_new();	 
	if (NULL == soap)
	{
		TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s malloc failed ",__FUNCTION__);
		return FAILED;
	}
	soap_init(soap);
	soap->socket = -1;
	soap->recvfd = fd[0];
	if(-1 == write(fd[1], xml , strlen(xml)))
	{
		TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s write failed ",__FUNCTION__);
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
		return FAILED;
	}

	close(fd[1]);

    /* Potential Patch Start*/
    if (soap_begin_recv(soap))
    {
        *resp = NULL;
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
        /* debug print */
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting 1 %s ",__FUNCTION__);
        /* debug print */
        return FAILED;
    }
    if (soap_envelope_begin_in(soap))
    {
        *resp = NULL;
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
        /* debug print */
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting 2 %s ",__FUNCTION__);
        /* debug print */
        return FAILED;
    }
    if (soap_recv_header(soap))
    {
        *resp = NULL;
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
        /* debug print */
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting 3%s ",__FUNCTION__);
        /* debug print */
        return FAILED;
    }

    if (soap_body_begin_in(soap))
    {
        *resp = NULL;
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
        /* debug print */
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting 4 %s ",__FUNCTION__);
        /* debug print */
        return FAILED;
    }

    /* Potential Patch End*/

	if (soap->header)
	{
		*header = soap->header;
	}

	if (soap_recv_fault(soap, 1))
	{
		*resp = NULL;
		TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting  because of fault%s ",__FUNCTION__);
		/* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
        struct _cwmp__Fault * ftmp = NULL;
		if (soap->fault)
		{
			if (soap->fault->detail)
			{
				ftmp = soap->fault->detail->fault;
				if (ftmp && ftmp->FaultCode && ftmp->FaultString)
				{
                    /*SPR 22314 Start*/
                    /* Code Removed */
					*resp =  ftmp;
                    /*SPR 22314 End*/
					/*spr 17966 fix start*/
					*type = SOAP_TYPE_cwmp__ACSFaultCodeType;
					/*spr 17966 fix end*/
				}
			}

		}	
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* Code Removed */
        /* SPR 22314 End*/

		return FAILED;
	}



	*resp = soap_getelement(soap, type);
	if ((soap)->error)
	{
		*resp = NULL;
		TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s ",__FUNCTION__);
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
		return FAILED;
	}

	if (soap_body_end_in(soap)|| soap_envelope_end_in(soap)|| soap_end_recv(soap))
	{
		*resp = NULL;
		TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s ",__FUNCTION__);
        /* SPR 22314 Start*/
        soap_all_attribute_free(soap);
        /* SPR 22314 End*/
		return FAILED;
	}
	TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s ",__FUNCTION__);
    /* SPR 22314 Start*/
    soap_all_attribute_free(soap);
    /* SPR 22314 End*/
	close (fd[0]);
	return SUCCESS;
}

char *soap_encode (void *msg, int methodType, struct SOAP_ENV__Header *header)
{
	/* GPN Change Start */	
	TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Entering in %s ",__FUNCTION__);
    struct soap *soap;
    char *p1 = NULL;

    int fd[2];
    pipe(fd);
    int pipesize = 11048000; //4 * fcntl(fd[1],F_GETPIPE_SZ,&pipesize);
    int err = fcntl(fd[1],F_SETPIPE_SZ,pipesize);
    TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "err = %d errorno = %d ",err,errno);
	/* GPN Change End */
    soap = soap_new();
    if (NULL == soap)
    {
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s malloc failed ",__FUNCTION__);
        return NULL;
    }

    soap_init(soap);
    soap->socket = -1;
    soap->sendfd = fd[1];
    soap->header = header;
    soap_begin_send(soap);
    soap_envelope_begin_out(soap);
    soap_putheader(soap);
    soap_body_begin_out(soap);
	switch(methodType)
	{
		case SOAP_TYPE__cwmp__Inform :
		{
			soap_put__cwmp__Inform(soap, (struct _cwmp__Inform *)msg,"cwmp:Inform",NULL);
			break;
		}
        case SOAP_TYPE__cwmp__ScheduleInformResponse:
        {
            soap_put__cwmp__ScheduleInformResponse (soap, (struct _cwmp__ScheduleInformResponse *) msg ,
                                                   "cwmp:ScheduleInformResponse", NULL);
            break;
        }
		
		case SOAP_TYPE__cwmp__SetParameterValuesResponse :
		{
			soap_put__cwmp__SetParameterValuesResponse(soap, (struct _cwmp__SetParameterValuesResponse *)msg,"cwmp:SetParameterValuesResponse",NULL);
			break;
		}

                case SOAP_TYPE__cwmp__DownloadResponse :
                {
                        soap_put__cwmp__DownloadResponse(soap, (struct _cwmp__DownloadResponse *)msg,"cwmp:DownloadResponse",NULL);
                        break;
                }
        case SOAP_TYPE__cwmp__ScheduleDownloadResponse:
        {
             soap_put__cwmp__ScheduleDownloadResponse(soap, (struct _cwmp__ScheduleDownloadResponse *)msg ,
                                                     "cwmp:ScheduleDownloadResponse", NULL); 
            break;
        }
        case SOAP_TYPE__cwmp__UploadResponse :
        {
            soap_put__cwmp__UploadResponse(soap , (struct _cwmp__UploadResponse *)msg, "cwmp:UploadResponse", NULL);
            break;
        }

		case SOAP_TYPE__cwmp__TransferComplete :
		{
			soap_put__cwmp__TransferComplete(soap, (struct _cwmp__TransferComplete *)msg, "cwmp:TransferComplete", NULL);
			break;
		}
 
		case SOAP_TYPE__cwmp__AutonomousTransferComplete :
		{
			soap_put__cwmp__AutonomousTransferComplete(soap, (struct _cwmp__AutonomousTransferComplete *)msg, "cwmp:AutonomousTransferComplete", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__Fault :
		{
			soap_put__cwmp__Fault(soap, (struct _cwmp__Fault *)msg, "cwmp:Fault", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__GetParameterValuesResponse :
		{
			soap_put__cwmp__GetParameterValuesResponse(soap, (struct _cwmp__GetParameterValuesResponse *)msg, "cwmp:GetParameterValuesResponse", NULL);
			break;
		}
 
		case SOAP_TYPE__cwmp__GetRPCMethodsResponse :
		{
			soap_put__cwmp__GetRPCMethodsResponse(soap, (struct _cwmp__GetRPCMethodsResponse *)msg, "cwmp:GetRPCMethodsResponse", NULL);
			break;
		}
 
		case SOAP_TYPE__cwmp__GetParameterNamesResponse :
		{
			soap_put__cwmp__GetParameterNamesResponse(soap, (struct _cwmp__GetParameterNamesResponse *)msg, "cwmp:GetParameterNamesResponse", NULL);
			break;
		}


		case SOAP_TYPE__cwmp__SetParameterAttributesResponse :
		{
			soap_put__cwmp__SetParameterAttributesResponse(soap, (struct _cwmp__SetParameterAttributesResponse *)msg, "cwmp:SetParameterAttributesResponse", NULL);
			break;
		}

        case SOAP_TYPE__cwmp__GetParameterAttributesResponse :
        {
            TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "In Case1");
                        soap_put__cwmp__GetParameterAttributesResponse(soap, (struct _cwmp__GetParameterAttributesResponse *) msg , "cwmp:GetParameterAttributesResponse" , NULL);
                        break;
        }
		case SOAP_TYPE__cwmp__AddObjectResponse :
		{
			soap_put__cwmp__AddObjectResponse (soap, (struct _cwmp__AddObjectResponse *)msg, "cwmp:AddObjectResponse", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__DeleteObjectResponse :
		{
			soap_put__cwmp__DeleteObjectResponse (soap, (struct _cwmp__DeleteObjectResponse *)msg, "cwmp:DeleteObjectResponse", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__RebootResponse :
		{
			soap_put__cwmp__RebootResponse (soap, (struct _cwmp__RebootResponse *)msg, "cwmp:RebootResponse", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__FactoryResetResponse :
		{
			soap_put__cwmp__FactoryResetResponse (soap, (struct _cwmp__FactoryResetResponse *)msg, "cwmp:FactoryResetResponse", NULL);
			break;
		}
		case SOAP_TYPE_cwmp__CPEFaultCodeType :
                {
                        soap_put_cwmp__CPEFaultCodeType (soap, ( cwmp__CPEFaultCodeType *)msg, "cwmp:CPEFaultCodeType", NULL);
                        break;
                }
                case SOAP_TYPE_SOAP_ENV__Fault :
                {
                        soap_put_SOAP_ENV__Fault (soap, (struct SOAP_ENV__Fault *)msg, NULL, NULL);
                        break;
                }
		case SOAP_TYPE__cwmp__GetRPCMethods :
		{
			soap_put__cwmp__GetRPCMethods(soap, (struct _cwmp__GetRPCMethods *)msg,"cwmp:GetRPCMethods", NULL);
                        break;
		}
		case SOAP_TYPE__cwmp__RequestDownload :
		{
			soap_put__cwmp__RequestDownload(soap, (struct _cwmp__RequestDownload *)msg, "cwmp:RequestDownload", NULL);
			break;
		}
		case SOAP_TYPE__cwmp__AutonomousDUStateChangeComplete :
		{
			soap_put__cwmp__AutonomousDUStateChangeComplete(soap, (struct _cwmp__AutonomousDUStateChangeComplete *)msg, "cwmp:AutonomousDUStateChangeComplete", NULL);
			break;
		}
        case SOAP_TYPE__cwmp__GetAllQueuedTransfersResponse :
        {
            soap_put__cwmp__GetAllQueuedTransfersResponse(soap, (struct _cwmp__GetAllQueuedTransfersResponse *)msg,
                                                 "cwmp:GetAllQueuedTransfersResponse", NULL); 
            break;
        }
        case SOAP_TYPE__cwmp__CancelTransferResponse :
        {
            soap_put__cwmp__CancelTransferResponse(soap, (struct _cwmp__CancelTransferResponse *)msg,
                                                 "cwmp:CancelTransferResponse", NULL); 
            break;
        }
		default : 
			break;
	}
		
    soap_body_end_out(soap);
    soap_envelope_end_out(soap);
    soap_end_send(soap);
    soap_end(soap);
    close(fd[1]);

    p1 = slurpfd(fd[0]);
    TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s ",__FUNCTION__);
    /* SPR 22314 Start*/
    soap_all_attribute_free(soap);
    /* SPR 22314 End */
    return p1;
}

char * slurpfd(int fd)
{
        const int bytes_at_a_time = 2;
        char *read_buffer = NULL;
        int buffer_size = 0;
        int buffer_offset = 0;
        int chars_io;
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Entering in %s ",__FUNCTION__);
        while (1)
        {
                if (buffer_offset + bytes_at_a_time > buffer_size)
                {
                        buffer_size = bytes_at_a_time + buffer_size * 2;
                        read_buffer = (char *) TR_REALLOC(read_buffer, buffer_size);
                        if (!read_buffer)
                        {
                                perror("memory");
                                exit(EXIT_FAILURE);
                        }
                }

                chars_io = read(fd,read_buffer + buffer_offset,bytes_at_a_time);
                if (chars_io <= 0)
                        break;

                buffer_offset += chars_io;
        }
        read_buffer[buffer_offset]='\0';

        if (chars_io < 0)
        {
                perror("read");
                exit(EXIT_FAILURE);
        }

        close(fd);
        TRACE (LOG_CRITICAL , SOAP_ENCODE_DECODE, "Exiting %s ",__FUNCTION__);
        return read_buffer; /* caller gets to free it */
}
/*SPR 22314 Start */
void soap_all_attribute_free(struct soap *soap)
{
    soap_free(soap);
}

/* SPR 22314 End */
