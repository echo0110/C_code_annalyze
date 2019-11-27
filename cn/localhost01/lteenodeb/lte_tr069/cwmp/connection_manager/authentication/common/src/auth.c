/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: auth.c $
 *
 *******************************************************************************
 *
 * File Description: This file is used for Authentication 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 25,2013                          Removed warnings from the code
******************************************************************************/

/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ManagementServerModel.h>
#include <logger.h>

//#include "md5.c"
//#include "strfun.c"
/*******************************************************************************
 *
 *               MACROS
 *
 *******************************************************************************/

#define USED_MD5 1
#define HEX_STR_LEN 16
#define MAX_STR_LENGTH 32
#define USED_MD5 1
#define DEBUG_ON 0
#define AUTHENTICATION "AUTHENTICATION"

static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static const int BASE64_INPUT_SIZE = 57;
/*********************************************************************************
 *                FUNCTION PROTOTYPES
 *
 ********************************************************************************/
int auth_for_digest(char *auth_str, char *auth[2]);
char *find_passwd (char* username);
//char * hex_2_str (char *src);
void auth_for_basic(char * buffer, char *auth[2]);

/* vj SPR 17968 fix start */
void digest_auth_md5(char *username, char *realm, char *nonce, char *uri,
		     char *response, char *auth[2]);
/* vj SPR 17968 fix end */             

//Added this prototype to remove warning as there is no interface file in cspl for this
void yeMd5Digest( const unsigned char *buffer, unsigned int length, unsigned char result[16] );

/****************************************************************************
 * Function Name  : user_pwd 
 * Inputs         : char * buffer, char *auth[2]
 *                 
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void user_pwd(char * buffer, char *auth[2])
{
	int i;
	char *arr[2] = {NULL};
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Entering in %s", __FUNCTION__);
	for(i=0;i<2;i++)
	{
		arr[i] = (char *)ALLOCATE(100);
	}
        char *token = NULL;
        token = strtok(buffer,":");
      	i=0;
        while (token != NULL)
        {
                strncpy(arr[i],token, 100);
                token = strtok(NULL, ":");
          	i++;
	}
	
		auth[0] = (char *)ALLOCATE(100);
		auth[1] = (char *)ALLOCATE(100);
		
	strncpy(auth[0],arr[0], 99);
	strncpy(auth[1],arr[1], 99);
	if (DEBUG_ON)
	{
	    TRACE (LOG_CRITICAL, AUTHENTICATION, "Username = %s\tPassword = %s", auth[0], auth[1]);
	}
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Username = %s\tPassword = %s", auth[0], auth[1]);
	for(i=0;i<2;i++)
	{
		DEALLOCATE(arr[i]);
	}
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
}

/****************************************************************************
 * Function Name  : ipr_cwmp_decoder 
 * Inputs         : char * buffer, char *auth[2]
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void ipr_cwmp_decoder(char * buffer, char *auth[2])
{
    char *token = NULL;
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Entering in %s", __FUNCTION__);
	if (DEBUG_ON)
	    TRACE (LOG_CRITICAL, AUTHENTICATION, "In decoder \n buffer = %s\n" , buffer);
        token = strtok(buffer,"\n");
        while (token != NULL)
        {
		if(0 == strncmp(token, "Authorization: Digest", 21))
		{
			if (DEBUG_ON)
	            TRACE (LOG_CRITICAL, AUTHENTICATION, "Authorization is Digest in %s", __FUNCTION__);
			auth_for_digest(token, auth);
			if (DEBUG_ON)
	            TRACE (LOG_CRITICAL, AUTHENTICATION, "Match Found  %s" , token);
			break;
		}
		else  if(0 == strncmp(token, "Authorization: Basic", 20))
		{
			if (DEBUG_ON)
	            TRACE (LOG_CRITICAL, AUTHENTICATION, "Authorization is Basic in %s", __FUNCTION__);
			auth_for_basic(token, auth);
			/* This part is needed to be written in case 
			 * if we are using other Authorization scheme 
			 * instead of Digest
			 */
			break;
		}
                token = strtok(NULL, "\n\r");
        }
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
}

/****************************************************************************
 * Function Name  : isbase64
 * Inputs         : char c
 *
 * Outputs        : None
 * Returns        : int
 ****************************************************************************/

int isbase64(char c)
{
       return c && strchr(table, c) != NULL;
}

/****************************************************************************
 * Function Name  : value 
 * Inputs         : char c
 *
 * Outputs        : None
 * Returns        : char
 ****************************************************************************/

inline char value(char c)
{
   const char *p = strchr(table, c);
   if(p) {
      return p-table;
   } else {
      return 0;
   }
}

/****************************************************************************
 * Function Name  : UnBase64
 * Inputs         : unsigned char *dest, const unsigned char *src, int srclen
 *
 * Outputs        : None
 * Returns        : int 
 ****************************************************************************/

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen)
{
   *dest = 0;
   if(*src == 0) 
   {
      return 0;
   }
   if (DEBUG_ON)
	TRACE (LOG_CRITICAL, AUTHENTICATION, "src is  %s" , src);
   unsigned char *p = dest;
   do
   {
      char a = value(src[0]);
      char b = value(src[1]);
      char c = value(src[2]);
      char d = value(src[3]);
      *p++ = (a << 2) | (b >> 4);
      *p++ = (b << 4) | (c >> 2);
      *p++ = (c << 6) | d;
      if(!isbase64(src[1])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[2])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[3])) 
      {
         p--;
         break;
      }
      src += 4;
      while(*src && (*src == 13 || *src == 10)) src++;
   }
   while(srclen-= 4);
   *p = 0;
   return p-dest;
}

/****************************************************************************
 * Function Name  : parse_string 
 * Inputs         : char *str_name, char *token 
 *
 * Outputs        : None
 * Returns        : char *
 ****************************************************************************/

static char *parse_string(char *str_name, char *token)
{
        char *ret_str = NULL;
        int i = 0;
        int idx = 0;
        if (NULL != strstr(token, str_name))
        {
                while(token[idx] != '\0')
                {
                        if (token[idx] == '"')
                        {
                                if(i == 0)
                                        ret_str = &(token[idx + 1]);
                                i = idx - i;
                        }
                        idx++;
                }
		if (NULL != ret_str)
                	ret_str[i - 1] = '\0';
		if (DEBUG_ON)
	                TRACE (LOG_CRITICAL, AUTHENTICATION, "In parse_string %s  = --%s--\n",str_name, ret_str);
        }
        return ret_str;
}

/****************************************************************************
 * Function Name  : auth_for_digest
 * Inputs         : char *auth_str, char *auth[2]
 *
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 ****************************************************************************/

int auth_for_digest(char *auth_str, char *auth[2])
{
        int check = 0;
        char *token = NULL;
        char *username = NULL;
        char *realm = NULL;
        char *nonce = NULL;
        char *uri = NULL;
        /* vj SPR 17968 fix code removed */
        
        //char response[35] = {'\0'};
        char *response = NULL;
        //char *resp = NULL;
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Entering in %s", __FUNCTION__);

        /* Parsing the following string for username and password etc.
         *
         * Authorization: Digest username="Mufasa",
         *           realm="testrealm@host.com",
         *           nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093",
         *           uri="/dir/index.html",
         *           qop=auth,
         *           nc=00000001,
         *           cnonce="0a4f113b",
         *           response="6629fae49393a05397450978507c4ef1",
         *           opaque="5ccc069c403ebaf9f0171e9517f40e41"
         */

        token = strtok(auth_str," ");
        while (NULL != token)
	{
		if (NULL != strstr(token, "username"))
		{
			username = parse_string("username", token);
			//username = (char *) ALLOCATE(strlen(temp_str) + 1);
			//strncpy(username, temp_str);
		}
                 /* SPR 16818 Fixed Start */
		else if (NULL != strstr(token, "realm")){
			realm = parse_string("realm", token);
			if(NULL == realm)
			{
				TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
				return 1;
			}
		}
		else if (NULL != strstr(token, "nonce")){
			nonce = parse_string("nonce", token);
			if(NULL == nonce)       //coverity fixes
			{
				TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
				return 1;
			}
		}
		else if (NULL != strstr(token, "uri")){
			uri = parse_string("uri", token);
			if(NULL == uri)
			{
				TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
				return 1;
			}
		}
        /* vj SPR 17968 fix code removed */
		else if (NULL != strstr(token, "response")){
			response = parse_string("response", token);
			if(NULL == response)
			{
				TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
				return 1;
			}
		}
             /* SPR 16818 Fixed End */
		if(0 == strcmp(token,"Digest"))
                        check = 1;
                if(1 == check)
                        token = strtok(NULL, ",");
                else
                        token = strtok(NULL, " ");
        }

    /*  coverity 40498/40499/40500/40501 fix start */
    if((NULL == nonce)||(NULL == realm)||
       (NULL == uri)||(NULL == response))
        {
            TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
            return FAILED;
        }
    /*  coverity 40498/40499/40500/40501 fix end */

/* vj SPR 17968 fix start */
	digest_auth_md5(username, realm, nonce, uri, 
			response, auth);
/* vj SPR 17968 fix end */

	TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
	return SUCCESS;
}

/****************************************************************************
 * Function Name  : digest_auth_md5 
 * Inputs         : char *username, char *realm, char *nonce,char *uri, char *qop
 *                  char *nc, char *cnonce,char *opaque, char *response,  char *auth[2]
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

/* vj SPR 17968 fix start */
void digest_auth_md5(char *username, char *realm, char *nonce, 
		     char *uri, char *response,  char *auth[2])
/* vj SPR 17968 fix end */             
{
	/* Currently MD5 algo is used for authentication */	
	unsigned char *password;
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Entering in %s", __FUNCTION__);
	if (USED_MD5)
	{
		char *A1_str = NULL;
		char *A2_str = NULL;
		int A1_str_len = 0;
		int A2_str_len = 0;
		unsigned char HA1_hex[HEX_STR_LEN] = {'\0'} ;
		unsigned char HA1_str[MAX_STR_LENGTH + 1] = {'\0'} ;
		unsigned char HA2_hex[HEX_STR_LEN] = {'\0'} ;
		unsigned char HA2_str[MAX_STR_LENGTH + 1] = {'\0'} ;
		unsigned char resp_hex [HEX_STR_LEN] = {'\0'} ;
		char resp_str [MAX_STR_LENGTH + 1] = {'\0'} ;
		char *create_resp = NULL;
		if (NULL == username)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s User name is NULL", __FUNCTION__);
			return;
		}

		/* This function will be implemented in future */
		/* Search password for given user name */
		/*  PROTOTYPE char *find_password (char* username);  */
		password = ipr_cwmp_getConnectionRequestPassword();
		//coverity fix
		if(NULL == password)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s Password is NULL", __FUNCTION__);
			return;
		}

		A1_str_len = strlen(username) + 1 + strlen(realm) + 1 + strlen((char*)password) + 1;
		A2_str_len = strlen("GET") + 1 + strlen(uri) + 1;
		A1_str = (char *) ALLOCATE(A1_str_len);
		if(NULL == A1_str)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s Password is NULL", __FUNCTION__);
			exit(0);		
		}

		memset(A1_str, '\0', A1_str_len);
		A2_str = (char *) ALLOCATE(A2_str_len);
		if(NULL == A2_str)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s Password is NULL", __FUNCTION__);
			exit(0);		
		}
		memset(A2_str, '\0', A2_str_len);

		tr_strncpy(A1_str, username);
		tr_strcat(A1_str, ":");
		tr_strcat(A1_str, realm);
		tr_strcat(A1_str, ":");
		tr_strcat(A1_str, (char*)password);
		if (DEBUG_ON)
			TRACE (LOG_CRITICAL, AUTHENTICATION, "A1_str = --%s--\n",A1_str);

		strncpy(A2_str, "GET", 4);
		tr_strcat(A2_str, ":");
		tr_strcat(A2_str, uri);
		if (DEBUG_ON)
			TRACE (LOG_CRITICAL, AUTHENTICATION, "A2_str = --%s--\n",A2_str);

		yeMd5Digest((unsigned char*)A1_str, strlen(A1_str), HA1_hex);
		yeMd5Digest((unsigned char*)A2_str, strlen(A2_str), HA2_hex);
		ipr_cwmp_hex_2_str(HA1_hex, HA1_str);
		ipr_cwmp_hex_2_str(HA2_hex, HA2_str);
		DEALLOCATE(A1_str);
		DEALLOCATE(A2_str);
		if (DEBUG_ON)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "hex_2_str HA1 = %s",HA1_str);
			TRACE (LOG_CRITICAL, AUTHENTICATION, "hex_2_str HA2 = %s",HA2_str);
		}

		create_resp = (char *) ALLOCATE(strlen((char*)HA1_str) + 1 + strlen(nonce) + 1 + strlen((char*)HA2_str) + 1 );
		if(NULL == create_resp)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Error : Mem allocation failure \n");
			exit(1);
		}
		strncpy(create_resp, (char*)HA1_str, 33);
		tr_strcat(create_resp, ":");
		tr_strcat(create_resp, nonce);
		tr_strcat(create_resp, ":");
		tr_strcat(create_resp, (char*)HA2_str); 
		if (DEBUG_ON)
			TRACE (LOG_CRITICAL, AUTHENTICATION, "create_resp =%s",create_resp);

		yeMd5Digest((unsigned char*)create_resp, strlen(create_resp), &resp_hex[0]);	
		DEALLOCATE(create_resp);
		ipr_cwmp_hex_2_str (resp_hex, (unsigned char*)resp_str);
		if (DEBUG_ON)
		{
			TRACE (LOG_CRITICAL, AUTHENTICATION, "hex_2_str resp = %s",resp_str);
			TRACE (LOG_CRITICAL, AUTHENTICATION, "Header response = %s",response);
		}
		if (0 == strcmp(resp_str, response))
		{
			if ((username != NULL) && (password != NULL))
			{
				auth[0] = (char *)ALLOCATE(strlen(username) + 1);
				if(NULL == auth[0])
				{
					TRACE (LOG_CRITICAL, AUTHENTICATION, "Error : Mem allocation failure \n");
					exit(1);
				}
				auth[1] = (char *)ALLOCATE(strlen((char*)password) + 1);
				if(NULL == auth[1] )
				{
					TRACE (LOG_CRITICAL, AUTHENTICATION, "Error : Mem allocation failure \n");
					exit(1);
				}

				tr_strncpy(auth[0] , username);
				tr_strncpy(auth[1] , (char*)password);
			}
		}
	}
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
}

/****************************************************************************
 * Function Name  : find_passwd
 * Inputs         : char *username
 *                 
 * Outputs        : None
 * Returns        : char *
 ****************************************************************************/

char * find_passwd (char *username)
{
	return username;

}

/****************************************************************************
 * Function Name  : auth_for_basic
 * Inputs         : char * buffer, char *auth[2]
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void auth_for_basic(char * buffer, char *auth[2])
{
        unsigned char destination[1024]={'\0'};
        unsigned char source[1024]={'\0'};
        int source_length;
        char *token = NULL;
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Entering in %s", __FUNCTION__);

        token = strtok(buffer," ");
        while (token != NULL)
        {
                if(0 == strncmp(token,"Basic", 5))
                {
                	token = strtok(NULL, " ");
                if(token)
                        strncpy((char*)source,token, 100);
                        source_length=strlen((char*)source);
                        UnBase64(destination, source, source_length);
			user_pwd((char*)destination, auth);
                        break;
                }
                token = strtok(NULL, " ");
        }
	TRACE (LOG_CRITICAL, AUTHENTICATION, "Exiting from %s", __FUNCTION__);
}
