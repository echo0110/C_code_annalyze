/********************************************************************************
*
*               FILE NAME   : 	connection_manager.c
*
*               DESCRIPTION : 	This file contains the functions which use libcurl library 
*				to download the file based on the inputs provided . 
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 27, 2012   TR-069 Amendment 4   Initial Creation
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/

#include <connection_manager.h>
#include <stdio.h>
#include <curl/curl.h>
#include <common_def.h>
#include <logger.h>
#include <ipr_cwmp_enb_wrapper.h>


#define MAX_URL_SIZE 256
/* SPR 17268 FIXED START */
#define CONFIG_PARAM_LEN 256
/* SPR 17268 FIXED END*/
#define STR_DELIM ":"
#define DEFAULT_NAME "Cpe_downloaded_file"
#define CONN_MANAGER_DW "Connection Manager"

/* SPR 16645 START */
extern UInt32 g_Download_time_out_thres;
/* SPR 16645 END */

/*SPR 17268 FIXED START */
extern UInt32 ABS_len;
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */
/******************************************************************************
  Function Declarations
******************************************************************************/
int connManagerDownload (STDownload *pstDownload);

/*****************************************************************************
* FUNCTION:		conn_fwrite		 
*
* DESCRIPTION:
*                       This is the callback write function used during initialization
*                       of libculr. 	   
*
* ARGS:
*                       void *buffer   ( pointer to the target where write the bytes). 
*                       size_t size 	(No. of bytes to be written).
*                       sez_t nmemb   
*                       void *stream   (Pointer to the stream where write the bytes). 
*
* RETURNS:
*                       
*                       size_t   : (No. of bytes written ) . 
*
******************************************************************************/

static size_t conn_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && out->stream!= NULL)
  {
    /* open file for writing */
	return fwrite(buffer, size, nmemb, out->stream);
  }
  else	
  	return -1;  // Return Failure.
}



static size_t conn_read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  //curl_off_t nread;

 /*      this fread() stuff is exactly what the library already would do
 *           by default internally */
  retcode = fread(ptr, size, nmemb, stream);

 // nread = (curl_off_t)retcode;

  //        " bytes from file\n", nread);

  return retcode;
}
/*download upload change starts*/
/* ioctl callback function */ 
static curlioerr conn_ioctl_callback(CURL *handle, curliocmd cmd, void *userp)
{
  
    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "entering in function: %s",__FUNCTION__);
   /*SPR 20636 Changes Start*/ 
  int fd = *((int*)userp);
    
   /*SPR 20636 Changes End*/ 
  (void)handle; /* not used in here */ 

  switch(cmd) {
  case CURLIOCMD_RESTARTREAD:
    /* mr libcurl kindly asks as to rewind the read data stream to start */ 
    if(-1 == lseek(fd, 0, SEEK_SET))
      /* couldn't rewind */ 
      return CURLIOE_FAILRESTART;
 
    break;
 
  default: /* ignore unknown commands */ 
    return CURLIOE_UNKNOWNCMD;
  }
  return CURLIOE_OK; /* success! */ 
}
/*download upload change ends*/
/*****************************************************************************
* FUNCTION:		checkProtocol 
*
* DESCRIPTION:
*                       The function retrieves the type of protocol from the 
*                       input URL. 
*
* ARGS:
*                        char * url -  ( URL ) 
*
* RETURNS:
*                      eProtocolType ( enum  HTTP , FTP, SFTP) 
*
******************************************************************************/

eProtocolType checkProtocol (char * url)
{
	TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Entering %s  ",__FUNCTION__);

	eProtocolType eType = HTTP;
 	char *p = NULL;
 	char *curl = ALLOCATE( MAX_URL_SIZE);

	if (curl != NULL )  
	{
  		strncpy (curl , url, MAX_URL_SIZE);
		p = strtok (curl, STR_DELIM);
        /*download upload change starts*/
 		if (p != NULL)
 		{
        		if ( strcmp (p , "ftp") == 0 )
        		{
				eType = FTP;
        		}
        		else if ( strcmp (p , "sftp") == 0 )
                		{
					eType = SFTP;
                		}		
			else if ( strcmp (p , "https") == 0 )
			{
				eType = HTTPS;
        		}
 		}	
/*download upload change ends*/
		DEALLOCATE (curl);
        /*  coverity 86502 fix start */
        /* code removed */
        /*  coverity 86502 fix end */
	}

	TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Exiting %s  ",__FUNCTION__);
	return eType;
} 


int ipr_cwmp_connManager_ScheduleDownload (ScheduleDownload *schDwn_t)
{
    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Entering %s ",__FUNCTION__);
    STDownload *pstDownload = NULL;
    int res = 0; 

    pstDownload = ALLOCATE( sizeof(STDownload));

    if (pstDownload)
    {
        pstDownload->URL = schDwn_t->URL;
        pstDownload->Username = schDwn_t->Username;
        pstDownload->Password = schDwn_t->Password;
        pstDownload->TargetFileName = schDwn_t->TargetFileName;
        res = connManagerDownload(pstDownload);
        DEALLOCATE(pstDownload);
    }

    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Exiting %s with return value: %d",__FUNCTION__, res);

    return res;
}

int connManagerUpload (Upload *arg_upload_t, char * arg_file_path)
{

    eProtocolType eType = HTTP;
    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
    struct stat file_info = {0};
    int hd = 0; 
    FILE * hd_src = NULL;
    long http_code = 0;
    char upfile[DEFAULT_INP_SIZE] = "Cpefile_";
    //char renamefile[DEFAULT_INP_SIZE] = "Cpefile_";

    char *token = NULL;
    /* SRP 19162 FIXED START */
    char *temp = NULL;
    /* SRP 19162 FIXED END */

    char ftp_url[MAX_URL_SIZE + DEFAULT_INP_SIZE] = "\0";
    

/*
    char buf_1 [DEFAULT_INP_SIZE + MAX_URL_SIZE] = "RNFR ";
    char buf_2[DEFAULT_INP_SIZE + MAX_URL_SIZE] = "RNTO ";
    struct curl_slist *headerlist = NULL;
  
*/  

    

    /* get the file size of the local file */
    hd = open(arg_file_path, O_RDONLY);
    //coverity fix
    if(-1 == hd)
    {
	    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "ERROR :error in  Open function Exiting %s  ",__FUNCTION__);
        /* SPR 21824 FIXED START*/
        /* SPR 20560 fix start */
	    return CURLE_FILE_COULDNT_READ_FILE;
        /* SPR 20560 fix end */
    }
    if (fstat(hd, &file_info) != 0)
    {
	    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "File donot exit in function %s  ",__FUNCTION__);
	    //coverity fix
	    close(hd);    
	    return CURLE_FILE_COULDNT_READ_FILE;
        /* SPR 21824 FIXED END*/
    };
    close(hd) ;

  /* get a FILE * of the same file, could also be made with
  *      fdopen() from the previous descriptor, but they this is just
  *           an example! */

     /* SPR 17268 FIXED START */
     TRACE ( LOG_CRITICAL , CONN_MANAGER_DW , "file path after concate of arg_file_path  %s ",arg_file_path);
     hd_src = fopen((char *)arg_file_path, "rb");
     /* SPR 17268 FIXED END */ 

     // hd_src = fopen(arg_file_path, "rb");

    if (!hd_src)
    {
	    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "File donot exit in function %s  ",__FUNCTION__);
	    return 1;
    }

 
    if (arg_upload_t->URL)
    {
        eType =  checkProtocol(arg_upload_t->URL);  

	    /* SRP 19162 FIXED START */
	    temp = ENB_STRRCHR((char *)arg_upload_t->URL, '/');
	    if (NULL != temp)
	    {
		    temp = temp + TR069_ONE;
		    if ('\0' != *temp)
		    {
			    ENB_STRNCAT((char *)arg_upload_t->URL, "//", TR069_TWO);	
			    TRACE (LOG_DETAIL, CONN_MANAGER_DW, "Received URL is not ending with slash\n");
		    }
	    }
	    /* SRP 19162 FIXED END */
        curl_global_init(CURL_GLOBAL_ALL);
        
        /* get a curl handle */
        curl = curl_easy_init();
        if(curl) 
        {


            /*download upload change starts*/
            if (eType == FTP || eType == SFTP || eType == HTTP ||eType == HTTPS)
            {
                //strncpy (ftp_url , arg_upload_t->URL, MAX_URL_SIZE); 
                token = strrchr(arg_file_path, '/');
                
                if (token != NULL)
                {
                    tr_strncpy (upfile , token + 1);
                    snprintf(ftp_url, MAX_URL_SIZE+DEFAULT_INP_SIZE , "%s/%s",arg_upload_t->URL, upfile); 
           		    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "ftp_url is %s ,%s  ",ftp_url,__FUNCTION__);
                }
                else
		        {
                    snprintf(ftp_url, MAX_URL_SIZE+DEFAULT_INP_SIZE , "%s/%s", arg_upload_t->URL , arg_file_path);
		            TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "ftp_url is %s ,%s  ",ftp_url,__FUNCTION__);
		        }
                /*download upload change ends*/  

/*
                sprintf(buf_1, "RNFR %s_%d", upfile,random_no);
                sprintf(buf_2, "RNTO %s", arg_file_path);
                headerlist = curl_slist_append(headerlist, buf_1);
                headerlist = curl_slist_append(headerlist, buf_2);
*/
           }


            /* we want to use our own read function */
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, conn_read_callback);
            /*SPR 17635 FIxed Start*/
	        /* only allow HTTP, FTP and HTTPS,FTPS */
	        curl_easy_setopt(curl, CURLOPT_PROTOCOLS,
			       CURLPROTO_HTTP | CURLPROTO_FTP | CURLPROTO_SFTP | CURLPROTO_HTTPS);
	        /*SPR 17635 FIxed End*/

            /* enable uploading */
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            if (eType == HTTP)
            {
                /* HTTP PUT please */
                curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            }
			/*download upload change starts*/ 
            /*SPR 17635 FIxed Start*/
	        if (eType ==HTTPS)
            {
                /* HTTPS PUT please */
                curl_easy_setopt(curl, CURLOPT_PUT, 1L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            } 

	        if (eType ==SFTP)
            {
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            }
	        /*SPR 17635 FIxed End*/

             /* specify target URL, and note that this URL should include a file
             *        name, not only a directory */
            if (eType == FTP || eType == SFTP || eType == HTTP || eType == HTTPS)
                curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
            else
            curl_easy_setopt(curl, CURLOPT_URL, arg_upload_t->URL);
            /*download upload change ends*/

/*
            curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
*/

            /* now specify which file to upload */
            curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
            /*download upload change starts*/ 
            /* set the ioctl function */ 
            curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, conn_ioctl_callback);
 
            /* pass the file descriptor to the ioctl callback as well */ 
            curl_easy_setopt(curl, CURLOPT_IOCTLDATA, hd_src);
            /*download upload change ends*/

            /* provide the size of the upload, we specicially typecast the value
            *        to curl_off_t since we must be sure to use the correct data size */
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                            (curl_off_t)file_info.st_size);

            if (arg_upload_t->Username  && arg_upload_t->Password && strlen(arg_upload_t->Username)>0)
            {
            curl_easy_setopt(curl, CURLOPT_USERNAME , arg_upload_t->Username);
            curl_easy_setopt(curl, CURLOPT_PASSWORD , arg_upload_t->Password);
			 /*download upload change starts*/
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST|CURLAUTH_NTLM);
			/*download upload change ends*/ 
            }


            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            /* Now run off and do what you've been told! */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
    		TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

           // curl_slist_free_all (headerlist);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
         }

    }
    fclose(hd_src); /* close the local file */
    TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Exiting %s  ",__FUNCTION__);
    if (401 == http_code)
        return 401;
    return res;
}


/*****************************************************************************
* FUNCTION: qvNewDriver
*
* DESCRIPTION:
*                       The function allocates an identifier for the driver and
*                       initializes its data structures. Modules may be attached to this
*                       driver, while the thread itself may be started later.
*
* ARGS:
*                       None
*
* RETURNS:
*                       Identifier for the driver.
*
******************************************************************************/
int connManagerDownload (STDownload *pstDownload) 
{

	TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Entering %s ",__FUNCTION__);
    /* SPR 21239 FIXED START */
    UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
    /* SPR 21239 FIXED END */
	struct FtpFile ftpfile;
	//eProtocolType eType = HTTP;
  	CURL *curl;
  	CURLcode res = CURLE_FAILED_INIT;
	long http_code = 0;
    /* SPR 20566 fix start*/
	long content_length = 0;
    /* SPR 20566 fix end*/
    char * file_path = NULL;

    file_path  = (char *)ALLOCATE( MAX_URL_SIZE);
    if (NULL == file_path)
    {
        TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "In %s , Memory not available",__FUNCTION__); 
        return 0;
    }
    

	ftpfile.stream = NULL;


	if (NULL != pstDownload->URL )
	{

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

        TRACE( LOG_CRITICAL ,CONN_MANAGER_DW ,"ftpfile.filename is = %s",ftpfile.filename );

        /* SPR 17268 FIXED START */
        UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
        strcpy((char *)ABSPath1,(char *)ABSPath);

      TRACE ( LOG_CRITICAL , CONN_MANAGER_DW , "ABSpath before concate of MAX_URL_SIZE %s ",ABSPath1);
 
       snprintf (file_path , MAX_URL_SIZE , "%s%s%s",ABSPath1,CPE_FILE_DIR_NAME,ftpfile.filename);
      //snprintf (file_path , MAX_URL_SIZE , "%s/%s", ABSPath1,ftpfile.filename);
      TRACE ( LOG_CRITICAL , CONN_MANAGER_DW , "file path after concate of MAX_URL_SIZE %s ",file_path);

		//ftpfile.stream = NULL;
        /* SPR 17268 FIXED END */
			
    		//ftpfile.stream=fopen(ftpfile.filename, "wb");
        ftpfile.stream=fopen(file_path, "wb");

		//curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();

		//eType =  checkProtocol (pstDownload->URL);

		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, pstDownload->URL);
                	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  conn_fwrite);
                	//Sanity Fix file downld 
            /*  coverity 40465 fix start */
			curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *)&ftpfile);
            /*  coverity 40465 fix end */

	                //curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST);

		//	switch (eType)
		//	{
		//		case FTP:
		//		case SFTP:
				
					if ( pstDownload->Username &&  pstDownload->Password)
					{
						curl_easy_setopt (curl, CURLOPT_USERNAME , pstDownload->Username);
						curl_easy_setopt (curl,  CURLOPT_PASSWORD ,  pstDownload->Password);
						 /*download upload change starts*/
						curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST|CURLAUTH_NTLM);
						/*download upload change ends*/
					}
			//}
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
              		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
              		curl_easy_setopt(curl,CURLOPT_POSTREDIR,CURL_REDIR_POST_ALL);
                	curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5L);
			/* SPR 16645 START */
			curl_easy_setopt(curl,CURLOPT_TIMEOUT,g_Download_time_out_thres);
			/* SPR 16645 END */

			res = curl_easy_perform(curl);
         TRACE (LOG_CRITICAL, CONN_MANAGER_DW, "debug resp is %d", res);

         /*SPR 21633 FIXED START */ 
         /*SPR 21876 FIXED START */
          /* Code Remove */  
         /*SPR 21876 FIXED  END*/
         /*SPR 21633 FIXED END */ 
         /* SPR 21239 FIXED START */
         ENB_STRCPY((char *)ABSPath2,(char *)ABSPath);
         ENB_STRCAT((char *)ABSPath2, "/?");

         /* SPR 21239 FIXED END */

            /* SPR 20566 fix start*/
            if (0 == res)
            {
			    curl_easy_getinfo (curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);

             if (0 == content_length)
             {
                /* SPR 21239 FIXED START */
                 if(-1 ==  remove((char *)ABSPath2))
                 {
                     TRACE (LOG_DETAIL , CONN_MANAGER_DW, "ERROR in remove file Exiting %s  ",__FUNCTION__);
                 }
                 TRACE (LOG_CRITICAL, CONN_MANAGER_DW, "curlres: %d, http_code: %d content_length = %d\n", res, http_code, content_length);
                 /* SPR 21239 FIXED END */

                 /* SPR 21239 FIXED START */
                 if(-1 ==  remove(file_path))
                 {

                     TRACE (LOG_DETAIL, CONN_MANAGER_DW, "ERROR in remove file");
                 }
                 DEALLOCATE(file_path);
                 /* SPR 21239 FIXED END */

                 res = CURLE_REMOTE_FILE_NOT_FOUND;
                 curl_easy_cleanup(curl);
                 DEALLOCATE(file_path);
                 /*Coverity 111577/111583 Fix Start*/
                 if(ftpfile.stream)
                 {	
                     fclose(ftpfile.stream); 
                 }
                 /*Coverity 111577/111583 Fix End*/
                 TRACE (LOG_CRITICAL, CONN_MANAGER_DW, "curl_res: %d, http_code: %d \n", res, http_code);

                 return res;
             }
            }
            /* SPR 20566 fix end*/

        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
            TRACE (LOG_CRITICAL, CONN_MANAGER_DW, "curl_res: %d, http_code: %d \n", res, http_code);
            /* SPR 20566 fix end*/

            //curl_global_cleanup();
            curl_easy_cleanup(curl);

             /*SPR 21591 FIXED START */
             /* CODE REMOVE */
             /*SPR 21591 FIXED START */

      }
   }

   if(ftpfile.stream)
   {
       fclose(ftpfile.stream); /* close the local file */
   }
   //curl_global_cleanup();


   TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "Exiting %s  ",__FUNCTION__);
   if (http_code >= 400)
   {
       /* SPR 21239 FIXED START */
       if(-1 ==  remove((char *)ABSPath2))
       {
           TRACE (LOG_DETAIL , CONN_MANAGER_DW, "ERROR in remove file Exiting %s  ",__FUNCTION__);
       }
       /* SPR 21239 FIXED END */
       //coverity fix 
       if(-1 ==  remove(file_path))
       {
           TRACE (LOG_CRITICAL , CONN_MANAGER_DW, "ERROR in remove file Exiting %s  ",__FUNCTION__);
           DEALLOCATE(file_path);
           /* SPR 20274 FIXED START */
           return http_code;
           /* SPR 20274 FIXED END*/

       }
       DEALLOCATE(file_path);
       return http_code;
   }
    /* SPR 21239 FIXED START */
    if (res != TR069_ZERO)
    {
        if(-1 ==  remove(file_path))
        {

            TRACE (LOG_DETAIL, CONN_MANAGER_DW, "ERROR in remove file");
        }
    }
    /* SPR 21239 FIXED END */

   DEALLOCATE(file_path);
   /*
      if (401 == http_code)
      return 401;
      */
   return res;	
}


/*

   void main ()
   {

   STDownload *pstDownload = calloc( 1,sizeof(STDownload));

//First case if  pwd is not provided 
pstDownload->URL = "sftp://10.203.152.192:22/home/jatin/jatin.txt";

ENB_PRINTF ("\n Return value after Download perfomred  :%d",connManagerDownload (pstDownload));

// Second Case if user is added 

pstDownload->Username = "jatin";	
pstDownload->Password = "jatin";

fflush (stdout);

ENB_PRINTF ("\n \n Now after supplying user Name and PWd   :%d" , connManagerDownload (pstDownload));


}
*/



