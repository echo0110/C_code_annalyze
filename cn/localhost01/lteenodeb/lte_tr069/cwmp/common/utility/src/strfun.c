/******************************************************************************** *
 *      FILE NAME   : strfun.c
 *
 *      DESCRIPTION : This file is used for different function call
 *
 *      REVISION HISTORY:
 *
 *      NAME             DATE            REFERENCE               REASON
 *_______________________________________________________________________________
 *
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 29,2013                          Added HAVE_IPSEC
 *   Anant K Sharma    Apr 08,2013                          Added check for connectivity of servers after tunnel establishment
 *
 *
 *       All Rights Reserved. Copyright @ Aricent Group, 2012.
 *
 ********************************************************************************/


#include "strfun.h"
#include <lteTypes.h>
#include <cspl.h>
#include <ipr_cwmp_enb_wrapper.h>

#ifdef HAVE_IPSEC
/****************************************************************************
 * Function Name  : ipr_cwmp_get_ip
 * Inputs         : UInt8* fqdn
 *                 
 * Outputs        : None
 * Returns        : UInt8*
 ****************************************************************************/

UInt8* ipr_cwmp_get_ip(UInt8* fqdn)
{
	if(fqdn == PNULL)
		return PNULL;
	struct addrinfo hints, *res = NULL;
	int errcode;
	UInt8* addrstr = ALLOCATE( 16);
	if(NULL == addrstr)
	{
		ENB_PRINTF(" ERR in ENB_MALLOC \n");
		return NULL; 
	}
	void* p_host_addr = NULL;
	int count = 0;

	ENB_PRINTF("fqdn received is %s\n", fqdn);
	memset(addrstr, 0, 16);
	memset(&hints, 0, sizeof (hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	errcode = getaddrinfo(fqdn, NULL, &hints, &res);
	if (errcode != 0)
	{
		fflush(stdout);
		DEALLOCATE(addrstr);
		return NULL;
	}

	ENB_PRINTF("%s\n", addrstr);

	ENB_INET_NTOP(res->ai_family, res->ai_addr->sa_data, (char*)addrstr, 16);
	switch (res->ai_family)
	{
		case AF_INET:
			p_host_addr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
			break;

		case AF_INET6:
			p_host_addr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
			break;
	}
	ENB_INET_NTOP(res->ai_family, p_host_addr, (char*)addrstr, 16);

	ENB_PRINTF("%s\n", addrstr);
	return addrstr;
}
#endif
/****************************************************************************
 * Function Name  : itoa 
 * Inputs         : UInt32 integer, Char8 *string
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void itoa(UInt32 integer, Char8 *string)
{
        ++integer;
        *string++ = '-';
        *sput_ip1(-integer, string) = '\0';
}

/****************************************************************************
 * Function Name  : sput_i
 * Inputs         : int integer, char *string
 *
 * Outputs        : None
 * Returns        : string
 ****************************************************************************/

char *sput_i(int integer, char *string)
{
    if (integer / 10 != 0)
    {
        string = sput_i(integer / 10, string);
    }
    *string++ = (char)('0' + integer % 10);
    return string;
}

/****************************************************************************
 * Function Name  : sput_ip1
 * Inputs         : int integer, char *string
 *
 * Outputs        : None
 * Returns        : string
 ****************************************************************************/

char *sput_ip1(int integer, char *string)
{
    int digit;

    digit = (integer % 10 + 1) % 10;
    if (integer / 10 != 0)
    {
        string = (digit == 0 ? sput_ip1 : sput_i)(integer / 10, string);
        *string++ = (char)('0' + digit);
    } else
    {
        if (digit == 0)
        {
            *string++ = '1';
        }
        *string++ = (char)('0' + digit);
    }
    return string;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_hex_to_str 
 * Inputs         : UInt8* src
 *
 * Outputs        : None
 * Returns        : Char8*
 ****************************************************************************/

Char8* ipr_cwmp_hex_to_str(UInt8* src)
{
    int temp = 0;
    int i = 0;
    int idx = 0;
    char *ret_str = NULL;
    char character[] = "0123456789abcdef";

    ret_str = (char *) ALLOCATE( 33);
	if(NULL == ret_str)
	{
		ENB_PRINTF(" ERR in Realloc \n");
		return PNULL; 
	}
    memset (ret_str, '\0', 33);
    memcpy(&temp,&src[0], 4);

    for (idx = 0; idx < 31; idx = idx + 2)
    {
        if(i%8 == 0)
        {
            memcpy(&temp,&src[idx/2], 4);
        }
        ret_str[i + 1] = character[(temp & 0x0f)];
        temp = temp >> 4;
        ret_str[i] = character[(temp & 0x0f)];
        temp = temp >> 4;
        i = i + 2;
    }
    ret_str[32] = '\0';
    return  ret_str;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_hex_2_str
 * Inputs         : UInt8* src, UInt8* ret_str
 *
 * Outputs        : None
 * Returns        : void 
 ****************************************************************************/

void ipr_cwmp_hex_2_str(UInt8* src, UInt8* ret_str)
{
    UInt32 temp = 0;
    UInt32 i = 0;
    UInt32 idx = 0;
    UInt8 character[] = "0123456789abcdef";

    ENB_MEMSET(ret_str, '\0', 33);
    ENB_MEMCPY(&temp,&src[0], 4);

    for (idx = 0; idx < 31; idx = idx + 2)
    {
        if(i%8 == 0)
        {
            ENB_MEMCPY(&temp,&src[idx/2], 4);
        }
        ret_str[i + 1] = (UInt32)character[(temp & 0x0f)];
        temp = temp >> 4;
        ret_str[i] = (UInt32)character[(temp & 0x0f)];
        temp = temp >> 4;
        i = i + 2;
    }
    ret_str[32] = '\0';
    return ;
}

/****************************************************************************
 * Function Name  :add_brackets 
 * Inputs         : UInt8* buf
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void add_brackets(UInt8* buf)
{
	UInt8 output[300];
	Int len=strlen((const char *)buf);
	Int i=0;
	Int j=0;

	for (i=0;i<len;i++)
	{

		if (*(buf+i+0)==46)
		{
			if ((*(buf+i+1)>=48) && (*(buf+i+1)<=57))
			{
				if (*(buf+i+2)==46)
				{
					*(output+j)=46;
					*(output+j+1)=91;
					*(output+j+2)=*(buf+i+1);
					*(output+j+3)=93;
					i=i+2;
					*(output+j+4)=46;
					j=j+5;
				}
				else if ((*(buf+i+2)>=48) && (*(buf+i+2)<=57))
				{
					if (*(buf+i+3)==46)
					{
						*(output+j)=46;
						*(output+j+1)=91;
						*(output+j+2)=*(buf+i+1);
						*(output+j+3)=*(buf+i+2);
						*(output+j+4)=93;
						i=i+3;
						*(output+j+5)=46;
						j=j+6;
					}
					else if ((*(buf+i+3)>=48) && (*(buf+i+3)<=57))
					{
						if (*(buf+i+4)==46)
						{
							*(output+j)=46;
							*(output+j+1)=91;
							*(output+j+2)=*(buf+i+1);
							*(output+j+3)=*(buf+i+2);
							*(output+j+4)=*(buf+i+3);
							*(output+j+5)=93;
							i=i+4;
							*(output+j+6)=46;
							j=j+7;
						}
						else
						{
							*(output+j)=*(buf+i);
							j++;
						}
					}
					else
					{
						*(output+j)=*(buf+i);
						j++;
					}
				}
				else
				{
					*(output+j)=*(buf+i);
					j++;
				}
			}
			else
			{
				*(output+j)=*(buf+i);
				j++;
			}
		}
		else
		{
			*(output+j)=*(buf+i);
			j++;
		}
	}

	*(output+j)='\0';
/* SPR 16448 Fix Start */
/* Code Removed */
		ENB_MEMSET(buf, '\0', len);
		ENB_STRNCPY((char *)buf , (const char *)output , 300);  
/* SPR 16448 Fix End */
}

/****************************************************************************
 * Function Name  : remove_brackets
 * Inputs         : UInt8* buf
 *
 * Outputs        : None
 * Returns        : void
 ****************************************************************************/

void remove_brackets(UInt8* buf)
{
     UInt8 output[300];
     Int len=strlen((const char *)buf);
     Int i=0;
     Int j=0;
     for (i=0;i<len;i++)
     {

          if ( (*(buf+i+0)==46) && (*(buf+i+1)==91) )
          {
               if ((*(buf+i+2)>=48) && (*(buf+i+2)<=57))
               {
                     if ( (*(buf+i+4)==46) && (*(buf+i+3)==93) )
                     {
                           *(output+j)=46;
                           *(output+j+1)=*(buf+i+2);
                           *(output+j+2)=46;
                           i=i+3;
                           j=j+2;
                     }
                     else if ((*(buf+i+3)>=48) && (*(buf+i+3)<=57))
                     {
                          if ( (*(buf+i+5)==46) && (*(buf+i+4)==93) )
                          {
                                *(output+j)=46;
                                *(output+j+1)=*(buf+i+2);
                                *(output+j+2)=*(buf+i+3);
                                *(output+j+3)=46;
                                i=i+4;
                                j=j+3;
                          }
                         else if ((*(buf+i+4)>=48) && (*(buf+i+4)<=57))
                         {
                              if ( (*(buf+i+6)==46) && (*(buf+i+5)==93) )
                              {
                                     *(output+j)=46;
                                     *(output+j+1)=*(buf+i+2);
                                     *(output+j+2)=*(buf+i+3);
                                     *(output+j+3)=*(buf+i+4);
                                     *(output+j+4)=46;
                                     i=i+5;
                                     j=j+4;
                              }
                              else
                              {
                                   *(output+j)=*(buf+i);
                                   j++;
                              }
                         }
                         else
                         {
                              *(output+j)=*(buf+i);
                              j++;
                         }
                     }
                     else
                     {
                         *(output+j)=*(buf+i);
                         j++;
                     }
               }
               else
               {
                   *(output+j)=*(buf+i);
                   j++;
               }
          }
          else
          {
             *(output+j)=*(buf+i);
              j++;
          }
      }

     *(output+j)='\0';
      if (strlen((const char *)output) > 256)
      {
        buf = TR_REALLOC(buf , 300);
	     if(NULL == buf)
	     {
		     ENB_PRINTF(" ERR in Realloc \n");
		     return; 
	     }
        ENB_MEMSET(buf, '\0', len);
        ENB_STRNCPY((char *)buf , (const char *)output , 300);
	//coverity fix
	DEALLOCATE(buf);        
	return;
      }
      ENB_MEMSET(buf, '\0', len);
      ENB_STRNCPY((char *)buf , (const char *)output , 256);
}
