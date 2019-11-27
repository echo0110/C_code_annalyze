/*****************************************************************************
 * File Name   : oam_curl_utils.c
 *
 * Description : This file contains common functions related to libcurl functionality 
 *
 * Revision History:
 *
 * Date               Author           Reference           Comments
 * -----             ------           ---------           --------
 * Aug 2013       Nitin Vijay      OAM Design Document    Initial Version
 *
 * July,2014      Aditi Singh      Function Header Corrected 
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#include <oam_defines.h>
#include <oam_types.h>
#include <oam_proto.h>
#include <string.h>
#include <signal.h>
#include <ifaddrs.h>
#include <time.h>
#include<sys/wait.h>
#include<oam_perf_mgmt.h>

extern oam_prov_req_t local_prov_req;    
static size_t oam_curl_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
static size_t oam_curl_fread(void *buffer, size_t size, size_t nmemb, void *stream);

/****************************************************************************
 *** Function Name  : oam_curl_fread 
 *** Inputs         : void buffer pointer, size, members to be written, 
 void type stream for file pointer
 *** Returns        : size 
 *** Description    : This function is used to download a package from specified server 
 *****************************************************************************/
static size_t oam_curl_fread(void *buffer, size_t size, size_t nmemb, void *stream)
{
    size_t retcode;
    retcode = oam_fread(buffer, size, nmemb, stream);
    return retcode;
}



/****************************************************************************
 *** Function Name  : oam_file_upload_to_server 
 *** Inputs         : oam_nms_ftp_server_params_t pointer type object , error code
 *** Returns        : OAM_SUCCESS/ OAM_FAILURE 
 *** Description    : This function is used to Upload a package to a specified server 
 *****************************************************************************/
oam_return_et
oam_file_upload_to_server
(
 oam_nms_ftp_server_params_t *p_oam_nms_ftp_server_params,
 char *file_name,
 UInt32 * curl_code
 )
{
    OAM_FUN_ENTRY_TRACE();

    CURL *curl;
    CURLcode res;
    FILE *fptr = NULL; 
    char ftp_url[OAM_FTP_URL_NAME_SIZE] = {OAM_ZERO};
    char local_file_name[OAM_FTP_SW_PACKAGE_NAME_SIZE] = {OAM_ZERO};
    UInt8 len = OAM_ZERO;
    oam_strncat(local_file_name, (char *)p_oam_nms_ftp_server_params->local_download_upload_path, oam_strlen((char *)p_oam_nms_ftp_server_params->local_download_upload_path));

     
    oam_strncat(local_file_name, "/", oam_strlen("/"));
    /* Klokwork Fix  */
    len = oam_strlen(local_file_name); 
    oam_strncat(local_file_name, file_name, sizeof(local_file_name)-(len+OAM_ONE));
    //Klokwork Fix 

    fptr = oam_fopen(local_file_name,"rb");
    if(OAM_NULL == fptr)
    {
        OAM_LOG(OAM, OAM_WARNING, "File opening failed in case of Uploading");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_strncpy((char *)ftp_url, (const char*)p_oam_nms_ftp_server_params->ftp_server_url, oam_strlen((char *)p_oam_nms_ftp_server_params->ftp_server_url));
    oam_strncat(ftp_url, "/", OAM_ONE);
    oam_strncat(ftp_url, (char *)file_name, oam_strlen(file_name));

    OAM_CURL_GLOBAL_INIT(CURL_GLOBAL_DEFAULT);

    curl = OAM_CURL_EASY_INIT();
    if(curl) 
    {
        OAM_CURL_EASY_SETOPT(curl, CURLOPT_READFUNCTION, oam_curl_fread);
        OAM_CURL_EASY_SETOPT(curl, CURLOPT_UPLOAD, 1L);

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_URL,ftp_url);
        OAM_CURL_EASY_SETOPT(curl, CURLOPT_READDATA, fptr);

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_USERNAME , p_oam_nms_ftp_server_params->ftp_server_username);
        OAM_CURL_EASY_SETOPT(curl, CURLOPT_PASSWORD , p_oam_nms_ftp_server_params->ftp_server_password);


        OAM_CURL_EASY_SETOPT(curl, CURLOPT_VERBOSE, 1L);

        res = OAM_CURL_EASY_PERFORM(curl);

        OAM_CURL_EASY_CLEANUP(curl);

        if(CURLE_OK != res)
        {
            *curl_code = (UInt32)res;		
            OAM_LOG(OAM, OAM_WARNING,"OAM_CURL_EASY_PERFORM Failed with error code == %d !! ",(UInt32)res);	  
	    oam_fclose(fptr);
            return OAM_FAILURE;
        }
    }
    OAM_CURL_GLOBAL_CLEANUP();

    oam_fclose(fptr);
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;

}

/****************************************************************************
 *** Function Name  :oam_curl_fwrite 
 *** Inputs         : void buffer pointer, size, members to be written, 
 void type stream for file pointer
 *** Returns        : size 
 *** Description    : This function is used to download a package from specified server 
 *****************************************************************************/
static size_t oam_curl_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
     FtpFile *out=( FtpFile *)stream;
     char local_path[OAM_LOCAL_PATH_NAME_SIZE]= {OAM_ZERO};
     UInt8 oam_curl_len = OAM_ZERO;
    //Klokwork fix
    oam_strncat(local_path, (char*)local_prov_req.oam_nms_ftp_server_params.local_download_upload_path, (sizeof(local_prov_req.oam_nms_ftp_server_params.local_download_upload_path) - OAM_ONE));
    oam_curl_len = oam_strlen(local_path);
    oam_strncat(local_path, out->filename, sizeof(local_path)-(oam_curl_len-OAM_ONE));	
    if(!out->stream) {
        out->stream= oam_fopen(local_path, "wb");
        if(!out->stream)
            return -1;
    }
    return oam_fwrite(buffer, size, nmemb, out->stream);
}
/****************************************************************************
 *** Function Name  : oam_file_download_from_server
 *** Inputs         : oam_nms_ftp_server_params_t pointer type object,curl_code, error code
 *** Returns        : OAM_SUCCESS/ OAM_FAILURE 
 *** Description    : This function is used to download a package from specified server 
 *****************************************************************************/
oam_return_et
oam_file_download_from_server
(
 oam_nms_ftp_server_params_t *p_oam_nms_ftp_server_params,
 CURLcode *curl_code,	
 oam_error_code_et *p_error_code
 )
{
    OAM_FUN_ENTRY_TRACE();
    CURL *curl;
    CURLcode res = CURLE_OK;
    FtpFile *ftpfile = OAM_NULL;
    char ftp_url[OAM_FTP_URL_NAME_SIZE] = {OAM_ZERO};
    oam_return_et retVal = OAM_FAILURE;

    oam_strncpy((char*)ftp_url, (const char *)p_oam_nms_ftp_server_params->ftp_server_url, oam_strlen((char *)p_oam_nms_ftp_server_params->ftp_server_url));


    retVal = oam_mem_alloc(sizeof(FtpFile), (void**)&ftpfile, p_error_code);
	if(OAM_FAILURE == retVal) {
           OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to ftpfile failed"
                " with Error Code:%d",
                        *p_error_code);
           OAM_FUN_EXIT_TRACE();
           return retVal;
	}

	oam_memset((void *)ftpfile->filename, OAM_ZERO, STR_LEN_256);	
	oam_strncpy(ftpfile->filename, (char *)p_oam_nms_ftp_server_params->sw_package_name, oam_strlen((char *)p_oam_nms_ftp_server_params->sw_package_name));

	ftpfile->stream = OAM_NULL;
 
    oam_strncat(ftp_url, "/", OAM_ONE);
    /* Klokwork fix  */
    UInt8 len = oam_strlen(ftp_url);
    /* coverity 62054 */
    oam_strncat(ftp_url, (char *)p_oam_nms_ftp_server_params->sw_package_name, OAM_FTP_URL_NAME_SIZE-(len+OAM_ONE));
    OAM_CURL_GLOBAL_INIT(CURL_GLOBAL_DEFAULT);

    curl = OAM_CURL_EASY_INIT();
    if(curl)
    {

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_URL, ftp_url);


        OAM_CURL_EASY_SETOPT(curl, CURLOPT_WRITEFUNCTION, oam_curl_fwrite);

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_WRITEDATA, ftpfile);

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_USERNAME , p_oam_nms_ftp_server_params->ftp_server_username);
        OAM_CURL_EASY_SETOPT(curl, CURLOPT_PASSWORD , p_oam_nms_ftp_server_params->ftp_server_password);

        OAM_CURL_EASY_SETOPT(curl, CURLOPT_VERBOSE, 1L);


        res = OAM_CURL_EASY_PERFORM(curl);

        OAM_CURL_EASY_CLEANUP(curl);
        if(CURLE_OK != res) {
            *curl_code = res;
            OAM_LOG(OAM, OAM_WARNING,"OAM_CURL_EASY_PERFORM Failed with error code == %d !! ",(UInt32)res);	  
            retVal = OAM_FAILURE;
        }
        else {
            retVal = OAM_SUCCESS;
        }
    }

    if(ftpfile->stream)
        oam_fclose(ftpfile->stream); /* close the local file */

    /* Coverity ID : 72386 Fix Start*/
    oam_mem_free(ftpfile,p_error_code);	
    /* Coverity ID : 72386 Fix End*/
    OAM_FUN_EXIT_TRACE();
    return retVal;
}


/****************************************************************************
 * Function Name  : oam_send_message
 * Inputs         : p_msg, dst_module, error code
 * Outputs        : none
 * Returns        : OAM_FAILURE/OAM_SUCCESS 
 * Description    : This is used for sending message to external modules
 ****************************************************************************/
oam_return_et oam_send_message
(
 void            *p_msg,     /* Message that will be passed */
 rrc_module_id_t dst_module,  /* Module id for which message will be passed */
 oam_error_code_et *p_error_code
 )
{

	/* CSR 00058135 Changes Start */
	UInt16 src_id =get_word_from_header(p_msg + OAM_TWO);
	UInt16 dst_id =get_word_from_header(p_msg + OAM_FOUR);

	if((OAM_ZERO==src_id)||(OAM_ZERO==dst_id))
	{
		OAM_LOG(OAM,OAM_ERROR,"Either Source Id or Destination Id received is ZERO, src_id=%d and dst_id=%d ",src_id,dst_id);
		/*SPR 13857 START*/
		oam_mem_free(p_msg, p_error_code);
        /*  coverity 94916 fix start */
		p_msg = OAM_NULL;
        /*  coverity 94916 fix end */
		/*SPR 13857 END*/

		return OAM_FAILURE;
	}
	/* CSR 00058135 Changes End */      

	if(QCLI_MODULE_ID == dst_module)
	{    
		OAM_LOG(OAM,OAM_INFO,"Destination Module is QCLI_MODULE_ID");
	}
	else if(TR069_MODULE_ID == dst_module)
	{
		OAM_LOG(OAM,OAM_INFO,"Destination Module is TR069_MODULE_ID");
    }
#ifndef OAM_UT_FRAMEWORK
	QMODULE service = qvGetService(dst_module);
	qvSendEx(service, OAM_ZERO, p_msg,(int *)p_error_code);
#endif
	return OAM_SUCCESS;
}
