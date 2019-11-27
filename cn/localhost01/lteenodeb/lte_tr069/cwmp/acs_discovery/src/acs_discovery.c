/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: acs_discovery.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the function for ipr_cwmp_acs_discovery 
 *      
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 17, 2012   TR-069 Amendment 4   Initial Creation
 *   Anant K Sharma   Mar 25, 2013                        removed compilation warnings
******************************************************************************/

#include <acs_discovery.h>
#include <logger.h>

/****************************************************************************
 * Function Name  : ipr_cwmp_acs_discovery
 * Inputs         : UInt8 recent_connection_status
 *                 
 * Outputs        : None
 * Returns        : IP of the ACS
 * Description    : This function returns a URL to the connection
 *                  Manager. It also takes care of affinity of ACS URL
 *                  
 ****************************************************************************/

UInt8 * ipr_cwmp_acs_discovery(
			UInt8 recent_connection_status)

{
	TRACE ( 3 , "ACS_DISCOVERY", "Entering ACS Discovery ");
	UInt8 * ACS_URL = PNULL;
	static UInt8 IP[IP_BUFFER] = {'\0'};
	UInt8 * null_check = PNULL;
	int dhcp_check = 0;
	static UInt8 method = USER_INPUT;

	//Connecting for the first time
	if( 'n' == recent_connection_status  || 'e' == recent_connection_status)
	{
		TRACE ( 3 , "ACS_DISCOVERY", "Checking for presence of URL in Management Model Server --------- ");
		ACS_URL = (UInt8 *)ipr_cwmp_getACSURL();
		if ( (PNULL == ACS_URL) || 
			(0 == ENB_STRLEN((const char*)ACS_URL)) )
		{
			TRACE ( 1 , "ACS_DISCOVERY", "Discovering ACS URL from DHCP --------- ");
			method = DHCP_WAY;
			do
			{
				ACS_URL = (UInt8 *)ipr_cwmp_dhcp_client();
				dhcp_check ++;
				if( 10 == dhcp_check )
				{
					TRACE ( 1 , "ACS_DISCOVERY", "DHCP process has failed after several repeated attempts --------- ");
					ipr_cwmp_setACSURL(PNULL);
					return PNULL;
				} 
			}
			while ( PNULL == ACS_URL );
		}
		TRACE ( 1 , "ACS_DISCOVERY", "URL Discovered --------- ");
			null_check = (UInt8 *)ipr_cwmp_ip_manager( ACS_URL, recent_connection_status );
			if(PNULL == null_check)
			{
				TRACE ( 1 , "ACS_DISCOVERY", "IP manager fails to return URL --------- ");
				ipr_cwmp_setACSURL(PNULL);
				/*Crash fix */
				return PNULL;
			}
			else
			{
				TRACE ( 2 , "ACS_DISCOVERY", "Returning URL --------- ");
				tr_strncpy ((char*)IP, (char*)null_check);
				ipr_cwmp_setACSURL((char*)IP);	
				return IP;
			}
	}

	//Successfully connected the previous time
	if( 's' == recent_connection_status )
	{
		TRACE ( 1 , "ACS_DISCOVERY", "Requesting same URL as previous time because of success --------- ");
		null_check = (UInt8 *)ipr_cwmp_ip_manager( PNULL , recent_connection_status );
		if( PNULL == null_check )
		{
			TRACE ( 1 , "ACS_DISCOVERY", "Problem with IP manager --------- ");
			return PNULL;
		}
		else
		{	
			tr_strncpy((char*)IP, (char*)null_check);
			ipr_cwmp_setACSURL((char*)IP);
			return IP;
		}
	}
	dhcp_check = 0;
	//Did not connect the previous time
	if( 'u' == recent_connection_status )
	{
		ACS_URL = (UInt8 *)ipr_cwmp_getACSURL();
		TRACE ( 1 , "ACS_DISCOVERY", "Requesting  for alternate URL --------- ");
		null_check = (UInt8 *) ipr_cwmp_ip_manager(ACS_URL, recent_connection_status );
		if( PNULL == null_check && method == DHCP_WAY )	
		{
		TRACE ( 1 , "ACS_DISCOVERY", "Discovering ACS URL from DHCP --------- ");
			do
			{
				ACS_URL = (UInt8 *)ipr_cwmp_dhcp_client();
				dhcp_check ++;
				if( 10 == dhcp_check )
				{
					TRACE ( 1 , "ACS_DISCOVERY", "DHCP process has failed after several repeated attempts --------- ");
					ipr_cwmp_setACSURL(PNULL);
					return PNULL;
				} 
			}while( PNULL == ACS_URL );

		TRACE ( 1 , "ACS_DISCOVERY", "URL found using DHCP --------- ");

			null_check = (UInt8 *)ipr_cwmp_ip_manager( ACS_URL, 'n' );
			
			if ( PNULL == null_check )
			{
				TRACE ( 1 , "ACS_DISCOVERY", "DNS has failed  --------- ");
				ipr_cwmp_setACSURL(PNULL);
				return PNULL;
			}
			else
			{
				TRACE ( 2 , "ACS_DISCOVERY", "Proper URL returned by IP Manager --------- ");
				tr_strncpy((char*)IP, (char*)null_check);
				ipr_cwmp_setACSURL((char*)IP);
				return IP;
			}
		}
		else
		{
/*SPR 21547 FIXED START*/
		        if( PNULL == null_check)
                        {
                                TRACE ( 1 , "ACS_DISCOVERY", "NO MORE URLs------ ");
                                /* SPR 21671 FIXED START */
                                /* Code Removed */
                                /* SPR 21671 FIXED End */
                                return   PNULL;

/*SPR 21547 FIXED START*/
                        }
			TRACE ( 1 , "ACS_DISCOVERY", "Alternate URL returned by IP Manager --------- ");
			tr_strncpy((char*)IP, (char*)null_check);
			ipr_cwmp_setACSURL((char*)IP);
		}
	}
	return IP;
}
