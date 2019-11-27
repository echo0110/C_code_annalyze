/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ManagementServerModel.c $
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2012/11/07 gur24604  Initial Version 1.0
 *   2013/03/25 gur24198  Removed warnings
******************************************************************************/
	
#include "ManagementServerModel.h"
	
/******************************************************************************
*
*			Static / Global Variables
*
******************************************************************************/
	
/* Structure for CWMP parameters like URL ,  Username and Password and etc */
/* SPR 16568 FIX START */
struct Device_ManagementServer g_ipr_cwmp_S_Device_Man_Ser;
/* SPR 16568 FIX END */

/* For Device parameters like ManufacturerOUI , SerialNumber ,  Host and etc. */
static struct Device_ManagementServer_ManageableDevice_i g_ipr_cwmp_DMS_i;
	
/* For ACS host name */
static UInt8 g_ipr_cwmp_acs_host_name [257] = {'\0'};
	
/* For sessiion ACS host names used by Session Manager */
static UInt8 g_ipr_cwmp_session_acs_host_name [257] = {'\0'};

//IPSec phase 2 code starts
#ifdef HAVE_IPSEC
/* Global pointer variable for IPSec related params */
// moved this to glue layer ipr_cwmp_crypto_params_t*   g_ipr_cwmp_crypto_params = PNULL;
//IPSec phase 2 code ends
#endif
	
	
/******************************************************************************
Function Definitions
******************************************************************************/
	
void ipr_cwmp_set_session_acs_host_name(UInt8* name)
{
	ENB_STRNCPY((char*)g_ipr_cwmp_session_acs_host_name, (char*)name, 256);
	g_ipr_cwmp_acs_host_name[256] = '\0'; 
}
	
UInt8* ipr_cwmp_get_session_acs_host_name(void)
{
	return g_ipr_cwmp_session_acs_host_name;
}

void ipr_cwmp_set_acs_host_name(UInt8* name)
{
	ENB_STRNCPY((char*)g_ipr_cwmp_acs_host_name, (char*)name, 256);
	g_ipr_cwmp_acs_host_name[256] = '\0'; 
}
	
UInt8* ipr_cwmp_get_acs_host_name(void)
{
	return g_ipr_cwmp_acs_host_name;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setPeriodicInformEnable
 * Inputs         : bool periodic_inform_enable
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the periodic parameters such
 *                  as PeriodInformEnable in the Management Server Model Module
 *                  
 * Note           : Whether or not the CPE MUST periodically send CPE info to
 *                  the to the ACS using Inform Method Call
 *
 ****************************************************************************/	
	
UInt32 ipr_cwmp_setPeriodicInformEnable(bool periodic_inform_enable)
{
	
	if((1 < periodic_inform_enable) )
	{
	    TRACE ( 1 , "Management Server Model", "periodic Inform Enable not set ");
	    return FAILED;
	}
	else
	{
    	g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformEnable = periodic_inform_enable;
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getPeriodicInformEnable
 * Inputs         : None
 *                : 
 * Outputs        : None
 * Returns        : bool
 * Description    : This function is used to get PeriodicInformEnable
 *                  parameter from the Management Model Server structure
 *                  
 ****************************************************************************/	
	
bool ipr_cwmp_getPeriodicInformEnable(void)
{
	return g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformEnable;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setPeriodicInformInterval
 * Inputs         : UInt32 periodic_inform_interval
 *
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the PeriodInformEnable in the
 *                  Management Server Model Module
 *
 * Note           : The duration in secs of the interval for which the CPE MUST
 *                  attempt to connect with the ACS and call the Inform method
 *                  if PeriodicInformEnable is true.    
 ****************************************************************************/
	
UInt32 ipr_cwmp_setPeriodicInformInterval (  UInt32 periodic_inform_interval )
{
	if (1 > periodic_inform_interval)
	{
	    TRACE ( 1 , "Management Server Model", "periodic Inform Interval not set ");
    	return FAILED;
	}
	else
	{
    	g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformInterval = periodic_inform_interval;
    	return SUCCESS;
	}
	
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getPeriodicInformInterval
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : Zero/PeriodicInformInterval
 * Description    : This function is used to get PeriodicInformInterval from
 *                  the Management Model Server structure
 *                  
 ****************************************************************************/	
	
UInt32 ipr_cwmp_getPeriodicInformInterval(void)
{
	if ( g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformInterval < 1 )
	{
    	return 0;
	}
	else
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformInterval;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setPeriodicInformTime 
 * Inputs         : UInt32 month,UInt32 day,UInt32 hours,UInt32 minutes
 *                  UInt32 seconds,UInt32 daylight_savings
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the periodic Inform Time in
 *                  the Management Server Model Module
 *
 * Note           : An absolute time reference in UTC to determine when the
 *                  CPE will initiate the periodic Inform method calls. Each
 *                  Inform call MUST occur at this reference time plus or minus
 *                  an UInt32eger multiple of the PeriodicInformInterval.
 ****************************************************************************/
	
	
UInt32 ipr_cwmp_setPeriodicInformTime( UInt32 month,// "0 for January 1 for February 11 for November, 12 for December
                            	UInt32 day, //0-31
                            	UInt32 hours, //0-23
                            	UInt32 minutes, //0-59
                            	UInt32 seconds, //0-59
                            	UInt32 daylight_savings )//1 - yes 0 - no -1 - unknown
{
	struct tm t;
	t.tm_year = 2012-1900;
//coverity fix
	if(month > 12)
	{
    	TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Month Invalid ");
    	return FAILED;
	}
	else
	{
    	t.tm_mon = month-1;
	}
	if( day > 31)
	{
	    TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Day of Month Invalid ");
    	return FAILED;
	}
	else
	{
	    t.tm_mday = day;
	}
	if( hours > 23 )
	{
	    TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Hours Invalid ");
    	return FAILED;
	}
	else
	{
	    t.tm_hour = hours;
	}
	if( minutes > 59 )
	{
	    TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Minutes Invalid ");
    	return FAILED;
	}
	else
	{
	    t.tm_min = minutes;
	}
	if( seconds > 59 )
	{
	    TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Seconds Invalid ");
	    return FAILED;
	}
	else
	{
	    t.tm_sec = seconds;
	}
//coverity fix
	if(daylight_savings > 1 )
	{
	    TRACE ( 1 , "Management Server Model", "Periodic Inform Time: Daylight savings  Invalid ");
    	return FAILED;
	}
	else
	{
    	t.tm_isdst = daylight_savings; 
	}
	g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformTime = mktime(&t); 
	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getPeriodicInformTime
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : time_t PeriodicInformTime
 * Description    : This function gets the PeriodicInformTime from the
 *                  management model server function
 *                  
 ****************************************************************************/	
	
time_t ipr_cwmp_getPeriodicInformTime(void)
{
	return g_ipr_cwmp_S_Device_Man_Ser.PeriodicInformTime;
}
/****************************************************************************
 * Function Name  : ipr_cwmp_setUsername 
 * Inputs         : Char8* username
 *                  
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the username and password in
 *                  the CPE Managament Server Model Structure
 *
 * Note           : Username used to authenticate the CPE when making a connection
 *                  to the ACS using the CPE WAN Management Protocol.
 *                  On a factory reset of the CPE, the value of this parameter
 *                  might be reset to its factory value. If an ACS modifies
 *                  the value of this parameter,it SHOULD be prepared to accommodate
 *                  the situation that the original value is  restored as the result
 *                  of a factory reset.
 ****************************************************************************/
	
	
UInt32 ipr_cwmp_setUsername(Char8* username)
{
	if(PNULL != username )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.Username, (char*)username, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.Username[STRING_MAX_LEN] = '\0';	
    	return SUCCESS;
	}
	else
	{
    	TRACE ( 1 , "Management Server Model", "username invalid ");
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setPassword
 * Inputs         : Char8* password
 *                : 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function sets the password in the Management
 *                  model server structure
 *                  
 ****************************************************************************/	
	
UInt32 ipr_cwmp_setPassword(Char8* password)
{
	if ( PNULL != password)
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.Password, (char*)password, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.Password[STRING_MAX_LEN] = '\0';
    	return SUCCESS;
	}
	else
	{
    	TRACE ( 1 , "Management Server Model", "password invalid ");
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getUsername
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : UInt8** username
 * Description    : This function gets username from management model
 *                  structure
 *                  
 ****************************************************************************/	
	
UInt8* ipr_cwmp_getUsername(void)
{
//coverity fix
	if (0 != g_ipr_cwmp_S_Device_Man_Ser.Username[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.Username;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getPassword
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : UInt8** password
 * Description    : This function gets the password from the Management 
 *                  model server structure
 *                  
 ****************************************************************************/	
	
UInt8* ipr_cwmp_getPassword(void)
{
	if (0 != g_ipr_cwmp_S_Device_Man_Ser.Password[0])
	{
	    return g_ipr_cwmp_S_Device_Man_Ser.Password;
	}
	else
	{
    	return PNULL;
	}
}
/****************************************************************************
 * Function Name  : ipr_cwmp_setEnableCWMP
 * Inputs         : bool enable_CWMP
 *                : 
 * Outputs        : None
 * Returns        : UInt32 Enable CWMP
 * Description    : THis function sets the EnableCWMP option in the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/	
	
UInt32 ipr_cwmp_setEnableCWMP(bool enable_CWMP)
{
	if (1 < enable_CWMP )
	{
    	TRACE ( 1 , "Management Server Model", "Enable CWMP invalid ");
    	return FAILED;
	}
	else
	{
    	g_ipr_cwmp_S_Device_Man_Ser.EnableCWMP = enable_CWMP;
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getEnableCWMP
 * Inputs         : Void
 *                : 
 * Outputs        : None
 * Returns        : bool Enable CWMP
 * Description    : This function get sthe EnableCWMP option from the
 *                  management Model Server Structure
 *                  
 ****************************************************************************/	
	
bool ipr_cwmp_getEnableCWMP(void)
{
	return 	g_ipr_cwmp_S_Device_Man_Ser.EnableCWMP;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setParameterKey
 * Inputs         : Char8* parameter_key
 *                : 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function set the Parameter Key in the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/	
	
UInt32 ipr_cwmp_setParameterKey(Char8* parameter_key)
{
	if ( parameter_key != PNULL)	
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.ParameterKey, (char*)parameter_key, 31);
    	g_ipr_cwmp_S_Device_Man_Ser.ParameterKey[31] = '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
} 
/****************************************************************************
 * Function Name  : ipr_cwmp_getParameterKey
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : UInt8** Parameter Key
 * Description    : This function retrieves the Parameter Key from the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/	
	
UInt8* ipr_cwmp_getParameterKey(void)
{
	if (0 != g_ipr_cwmp_S_Device_Man_Ser.ParameterKey[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.ParameterKey;
	}
	else
	{
    	return PNULL;
	}
}
/****************************************************************************
 * Function Name  : ipr_cwmp_setConnectionRequestURL
 * Inputs         : Char8* con_req_URL
 *                : 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function sets the sets the Connection Request
 *                  URL in the Management Model Server Structure
 *                  
 ****************************************************************************/	
	
UInt32 ipr_cwmp_setConnectionRequestURL(Char8* con_req_URL)
{
	if( PNULL != con_req_URL )
	{
		ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL, (char*)con_req_URL, STRING_MAX_LEN);
		g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL[STRING_MAX_LEN] = '\0';
		return SUCCESS;
	}
	else
	{
		return FAILED;
	}
}
	
/****************************************************************************
 * Function Name  : ipr_cwmp_getConnectionRequestURL 
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : UInt8** conenction request URL
 * Description    : This function sets the ConnectionRequest URL from the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/
	
UInt8* ipr_cwmp_getConnectionRequestURL (void)
{
//coverity fix
	if (0 != g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL[0])
	{
	    return g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestURL;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setConnectionRequestUsername
 * Inputs         : Char8* con_req_username
 *                : 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function is used to to set the Connection Request
 *                  Usernmae in the Management Model Server Structure
 *                  
 ****************************************************************************/	
	
//set Connection Request Username
UInt32 ipr_cwmp_setConnectionRequestUsername(Char8* con_req_username)
{
	if( PNULL != con_req_username )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestUsername, (char*)con_req_username, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestUsername[STRING_MAX_LEN] = '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getConnectionRequestUsername  
 * Inputs         : void
 *                : 
 * Outputs        : None
 * Returns        : UInt8** ConnectionRequestUsername
 * Description    : This function is used to get the Connection Request
 *                  Username from the Management Model Structure
 *                  
 ****************************************************************************/	
	
//get Connection Request username
UInt8* ipr_cwmp_getConnectionRequestUsername (void)
{
//coverity fix
	if ( 0 != g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestUsername[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestUsername;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setConnectionRequestPassword
 * Inputs         : Char8* con_req_password
 *                 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function is used to to set the Connection Request
 *                  Password  in the Management Model Server Structure
 *                  
 ****************************************************************************/	

//set Connection Request Password
UInt32 ipr_cwmp_setConnectionRequestPassword( Char8* con_req_password )
{
	if( PNULL != con_req_password )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestPassword, (char*)con_req_password, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestPassword[STRING_MAX_LEN] = '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getConnectionRequestPassword
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt8** ConnectionRequestPassword
 * Description    : This function is used to get the Connection Request
 *                  Password from the Management Model Structure
 *                  
 ****************************************************************************/
	
//get Connection Request Password
UInt8* ipr_cwmp_getConnectionRequestPassword(void)
{
//coverity fix
	if ( 0 != g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestPassword[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.ConnectionRequestPassword;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setUpgradesManaged
 * Inputs         : bool upgrades_managed
 *                 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function is used to set the UpgrdaesManaged field
 *                  in the Management Model Structure
 *                  
 ****************************************************************************/
	
	
//set Upgrades Managed
UInt32 ipr_cwmp_setUpgradesManaged(bool upgrades_managed)
{
	if (1  < upgrades_managed)
	{
	    return FAILED;
	}
	else
	{
	    g_ipr_cwmp_S_Device_Man_Ser.UpgradesManaged = upgrades_managed;
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getUpgradesManaged
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : bool Upgrades Managed
 * Description    : This function is used to get the Upgrades managed field
 *                  from the Structure of Managemement Model Server
 *                  
 ****************************************************************************/
	
	
//get Upgrades Managed
bool ipr_cwmp_getUpgradesManaged(void)
{
	return g_ipr_cwmp_S_Device_Man_Ser.UpgradesManaged; 
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setKickURL
 * Inputs         : Char8* kick_URL
 *                 
 * Outputs        : None
 * Returns        : UInt32 ipr_cwmp_setKickURL
 * Description    : This function sets KickURL the in the Management Model
 *                  Server Structure
 *                  
 ****************************************************************************/
	
//Set Kick URL
UInt32 ipr_cwmp_setKickURL(Char8* kick_URL)
{
	if( PNULL != kick_URL )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.KickURL, (char*)kick_URL, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.KickURL[STRING_MAX_LEN]= '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getKickURL
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt8** ipr_cwmp_getKickURL
 * Description    : This function gets the KickURL from the Management Model
 *                  Server Structure
 *                  
 ****************************************************************************/
	
//get Kick URL
	
UInt8* ipr_cwmp_getKickURL(void)
{
//coverity fix
	if ( 0 != g_ipr_cwmp_S_Device_Man_Ser.KickURL[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.KickURL;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setDownloadProgressURL
 * Inputs         : Char8* download_progress_URL
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the DownloadProgressURL in the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/
	
// Set Download Progress URL
	
UInt32 ipr_cwmp_setDownloadProgressURL(Char8* download_progress_URL)
{
	if( PNULL != download_progress_URL )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.DownloadProgressURL, (char*)download_progress_URL, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.DownloadProgressURL[STRING_MAX_LEN] = '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getDownloadProgressURL
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt8* DownloadProgressURL
 * Description    : This function gets the DownloadProgressURL from the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/
	
//get Download Progress URL
	
UInt8* ipr_cwmp_getDownloadProgressURL(void)
{
//coverity fix

	if (0 != g_ipr_cwmp_S_Device_Man_Ser.DownloadProgressURL[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.DownloadProgressURL;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setUDPConnectionRequestAddress 
 * Inputs         : Char8* con_req_address
 *                 
 * Outputs        : None
 * Returns        : SUCCESS / FAILED
 * Description    : This function sets the UDPConnectionRequestAddress in the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/
	
//set UDPConnectionRequest Address
	
UInt32 ipr_cwmp_setUDPConnectionRequestAddress(Char8* con_req_address)
{
	if( PNULL != con_req_address )
	{
	    ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.UDPConnectionRequestAddress, (char*)con_req_address, STRING_MAX_LEN);
    	g_ipr_cwmp_S_Device_Man_Ser.UDPConnectionRequestAddress[STRING_MAX_LEN] = '\0';
    	return SUCCESS;
	}
	else
	{
    	return FAILED;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getUDPConnectionRequestAddress
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt8* UDPConnectionRequestAddress
 * Description    : This function gets the UDPConnectionRequestAddress from
 *                  the Management Model Server Structure
 *                  
 ****************************************************************************/
	
//get UDPConnectionRequestAddress
	
UInt8* ipr_cwmp_getUDPConnectionRequestAddress(void)
{
//coverity fix
	if ( 0 != g_ipr_cwmp_S_Device_Man_Ser.UDPConnectionRequestAddress[0])
	{
    	return g_ipr_cwmp_S_Device_Man_Ser.UDPConnectionRequestAddress;
	}
	else
	{
    	return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setDefaultActiveNotificationThrottle
 * Inputs         : UInt32 def_act_not_throttle
 *                 
 * Outputs        : None
 * Returns        : SUCCESS / FAILED
 * Description    : This function sets DefaultActiveNotificationThrottle
 *                  in the Management Model Server Structure
 *                  
 ****************************************************************************/
	
	
//set Default Active Notification Throttle
	
UInt32 ipr_cwmp_setDefaultActiveNotificationThrottle(UInt32 def_act_not_throttle)
{
	g_ipr_cwmp_S_Device_Man_Ser.DefaultActiveNotificationThrottle = def_act_not_throttle;
	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getDefaultActiveNotificationThrottle
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt32 DefaultActiveNotificationThrottle
 * Description    : This function gets the DefaultActiveNotificationThrottle
 *                  from the Management Model Server Structure
 *                  
 ****************************************************************************/
	
//get ipr_cwmp_getDefaultActiveNotificationThrottle
	
UInt32 ipr_cwmp_getDefaultActiveNotificationThrottle (void)
{
	return g_ipr_cwmp_S_Device_Man_Ser.DefaultActiveNotificationThrottle;
} 

/****************************************************************************
 * Function Name  : ipr_cwmp_setCWMPRetryMinimumWaitInterval
 * Inputs         : UInt32 retry_minwait_interval
 *                 
 * Outputs        : None
 * Returns        : SUCCESS / FAILED
 * Description    : This function sets the CWMP retry minimum wait interval
 *                  in the management model server structure
 *                  
 ****************************************************************************/
	
//serCWMPRetryMinimumWaitInterval
	
UInt32 ipr_cwmp_setCWMPRetryMinimumWaitInterval(UInt32 retry_minwait_interval)
{
    	g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryMinimumWaitInterval = retry_minwait_interval;
    	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getCWMPRetryMinimumWaitInterval
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt32 CWMPRetryMinimumWaitInterval
 * Description    : This function sets the CWMP retry minimum wait interval
 *                  in the Management Model Server Structure
 *                  
 ****************************************************************************/
	
//ipr_cwmp_getCWMPRetryMinimumWaitInterval 
	
UInt32 ipr_cwmp_getCWMPRetryMinimumWaitInterval(void)
{
    if ( 0 == g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryMinimumWaitInterval)
    {
        return DEFAULT_WAIT_INTERVAL;
    }

    return g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryMinimumWaitInterval;
} 

/****************************************************************************
 * Function Name  : ipr_cwmp_setCWMPRetryIntervalMultiplier
 * Inputs         : UInt32 retry_interval_multiplier
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function set the CWMPREtry IntervalMultiplier in
 *                  the management model server structure
 *                  
 ****************************************************************************/
	
//set CWMP Retry Interval Multiplier
	
UInt32 ipr_cwmp_setCWMPRetryIntervalMultiplier(UInt32 retry_interval_multiplier)
{
	if ( 1000 > retry_interval_multiplier )
	{
    	TRACE ( 1 , "Management Server Model", "CWMPRetryIntervalMultiplier invalid ");
    	return FAILED;
	}
	else
	{
    	g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryIntervalMultiplier = retry_interval_multiplier;
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getCWMPRetryIntervalMultiplier
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt32 CWMP Retry Interval Multiplier
 * Description    : This function gets the the CWMPREtry Interval Multiplier
 *                  from the Mnaagement Model Server Structure 
 *                  
 ****************************************************************************/
	
//ipr_cwmp_getCWMPRetryMinimumWaitInterval 
	
UInt32 ipr_cwmp_getCWMPRetryIntervalMultiplier(void)
{
	if (0 == g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryIntervalMultiplier)
	{
    	return DEFAULT_INTERVAL_MULTIPILER;
	}
    	return g_ipr_cwmp_S_Device_Man_Ser.CWMPRetryIntervalMultiplier;
} 

/*********************************************************************************
 * Function Name  : ipr_cwmp_setACSURL
 * Inputs         : Char8* URL
 *                  
 * Outputs        : None
 * Returns        : Success/Failed
 * Description    : This function sets the URL in the the CPE Managament
 *                  Server Model Structure
 *
 * Note           : URL,for the CPE to connect to the ACS using the  CPE WAN
 *                  Management Protocol.
 *                  This parameter MUST be in the form of a valid HTTP or HTTPS URL.
 *
 *                  The host portion of this URL is used by the CPE for validating the
 *                  ACScertificate when using SSL or TLS.
 *                  Note that on a factory reset of the CPE, the value of this 
 *                  parameter might be reset to its factory value. If an ACS modifies 
 *                  the value of it SHOULD be prepared to accommodate the situation
 *                  that the original value is restored as the result of a factory reset
 **********************************************************************************/

UInt32 ipr_cwmp_setACSURL(Char8* URL)
{
	if ( PNULL != URL )
	{
		ENB_STRNCPY((char*)g_ipr_cwmp_S_Device_Man_Ser.URL, (char*)URL, STRING_MAX_LEN);
		g_ipr_cwmp_S_Device_Man_Ser.URL[STRING_MAX_LEN] = '\0';
		return SUCCESS;
	}
	else
	{
		ENB_MEMSET(g_ipr_cwmp_S_Device_Man_Ser.URL, 0, STRING_MAX_LEN + 1);
		return 1 ;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getACSURL
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : URL if any or PNULL
 * Description    : This function retrieves the URL from the CPE 
 *                  Management Server Model Structure
 *                  
 ****************************************************************************/
	
	
UInt8* ipr_cwmp_getACSURL()
{
//coverity fix
 
	if( (0 != g_ipr_cwmp_S_Device_Man_Ser.URL[0])  ||
	(ENB_STRLEN((char*)g_ipr_cwmp_S_Device_Man_Ser.URL) == 0) )
	{
		return g_ipr_cwmp_S_Device_Man_Ser.URL;
	}
	else
	{
		return PNULL;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setSoftVers
 * Inputs         : UInt8* SoftVers
 *                 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function is used to set the Software Version in the
 *                  Management Server Model Structure
 *                  
 ****************************************************************************/

UInt32 ipr_cwmp_setSoftVers(UInt8* SoftVers)
{
    if (PNULL == SoftVers)
    {
        ENB_MEMSET(g_ipr_cwmp_DMS_i.SoftVers, 0, STR_MAX_LEN + 1);
        return SUCCESS;
    }
    else
    {
        ENB_MEMSET(g_ipr_cwmp_DMS_i.SoftVers, 0, STR_MAX_LEN + 1);
        ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.SoftVers, (char*)SoftVers, STR_MAX_LEN);
        g_ipr_cwmp_DMS_i.SoftVers[STR_MAX_LEN] = '\0';
        return SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSoftVers
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : Alias
 * Description    : This function gets the Software Version from the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/

UInt8*  ipr_cwmp_getSoftVers(void)
{
//coverity fix
    if ((0 == g_ipr_cwmp_DMS_i.SoftVers[0]) ||
    ( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.SoftVers)) )
    {
        return PNULL;
    }
    else
    {
        return g_ipr_cwmp_DMS_i.SoftVers;
    }
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setAlias
 * Inputs         : UInt8* alias
 *                 
 * Outputs        : None
 * Returns        : SUCCESS or FAILED
 * Description    : This function is used to set the Alias in the
 *                  Management Server Model Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setAlias(UInt8* alias)
{
	if ( PNULL == alias ) 
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.Alias, 0, STR_MAX_LEN + 1);
    	return SUCCESS;
	}
	else 
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.Alias, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.Alias, (char*)alias, STR_MAX_LEN);
    	g_ipr_cwmp_DMS_i.Alias[STR_MAX_LEN] = '\0';
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getAlias
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : Alias
 * Description    : This function gets the Alias from the Management Model
 *                  Server Structure
 *                  
 ****************************************************************************/

UInt8*  ipr_cwmp_getAlias(void)
{
//coverity fix
	if ( ( 0 == g_ipr_cwmp_DMS_i.Alias[0] ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.Alias)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_DMS_i.Alias;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setManufacturerOUI
 * Inputs         : UInt8* man_oui
 *                 
 * Outputs        : None
 * Returns        : SUCCESS / FAILED
 * Description    : This function sets the ManufacturerOUI in the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setManufacturerOUI(UInt8* man_oui)
{
	if ( PNULL == man_oui )
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.ManufacturerOUI, 0, 6);
    	return SUCCESS;
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.ManufacturerOUI, 0, 6);
    	ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.ManufacturerOUI, (char*)man_oui, 6);
    	g_ipr_cwmp_DMS_i.ManufacturerOUI[6] = '\0';	
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getManufacturerOUI
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* Manufacturer OUI
 * Description    : This function gets the Manufactuere OUI from the
 *                  Management Model Server Structure
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getManufacturerOUI(void)
{
//coverity fix
	if ( ( 0 == g_ipr_cwmp_DMS_i.ManufacturerOUI[0] ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.ManufacturerOUI)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_DMS_i.ManufacturerOUI;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setSerialNumber
 * Inputs         : UInt8* s_no
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/ FAILED
 * Description    : This function sets the SerialNumber in the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/

UInt32 ipr_cwmp_setSerialNumber(UInt8* s_no)
{
	if ( PNULL == s_no )
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.SerialNumber, 0, STR_MAX_LEN + 1);
    	return SUCCESS;
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.SerialNumber, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.SerialNumber, (char*)s_no, STR_MAX_LEN);
    	g_ipr_cwmp_DMS_i.SerialNumber[STR_MAX_LEN] = '\0';
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSerialNumber
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* SerialNumber
 * Description    : This function gets the Serial Number from the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getSerialNumber(void)
{
//coverity fix
	if ( (0 == g_ipr_cwmp_DMS_i.SerialNumber[0]) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.SerialNumber)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_DMS_i.SerialNumber;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setProductClass
 * Inputs         : UInt8* prod_class
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the Product Class in the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setProductClass(UInt8* prod_class)
{
	if ( PNULL == prod_class )
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.ProductClass, 0, STR_MAX_LEN + 1);
    	return SUCCESS;
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.ProductClass, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.ProductClass, (char*)prod_class, STR_MAX_LEN);
    	g_ipr_cwmp_DMS_i.ProductClass[STR_MAX_LEN] = '\0';	
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getProductClass
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* product class
 * Description    : This function gets the product class from the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getProductClass(void)
{
//coverity fix
	if ( ( 0 == g_ipr_cwmp_DMS_i.ProductClass[0]) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.ProductClass)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_DMS_i.ProductClass;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setHost
 * Inputs         : UInt8* host
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the Host in the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setHost(UInt8* host)
{
	if ( PNULL == host )
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.Host, 0, STR_MAX_LEN + 1);
    	return SUCCESS;
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_DMS_i.Host, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_DMS_i.Host, (char*)host, STR_MAX_LEN);
    	g_ipr_cwmp_DMS_i.Host[STR_MAX_LEN] = '\0';
    	return SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getHost
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* host
 * Description    : This function gets the Host from the Management
 *                  Model Server Structure
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getHost(void)
{
//coverity fix
	if ((0 == g_ipr_cwmp_DMS_i.Host[0]) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_DMS_i.Host)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_DMS_i.Host;
	}
}

//IPSec phase 2 code starts
#ifdef HAVE_IPSEC
/****************************************************************************
 * Function Name  : ipr_cwmp_setSecGWServer1
 * Inputs         : UInt8* sec_gw_server1
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the SecGWServer1 in the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setSecGWServer1(UInt8* sec_gw_server1)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto params");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( PNULL == sec_gw_server1 )
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer1, 0, STR_MAX_LEN + 1);
        ipr_cwmp_add_gw1_ip(sec_gw_server1);
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer1, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer1, (char*)sec_gw_server1, STR_MAX_LEN);
    	g_ipr_cwmp_crypto_params->gw.SecGWServer1[STR_MAX_LEN] = '\0';
        ipr_cwmp_add_gw1_ip(sec_gw_server1);
	}
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto params");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved crypto params");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSecGWServer1 
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* SecGWServer1
 * Description    : This function gets the SecGWServer1 from the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway Structure
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getSecGWServer1()
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto params");
        return PNULL;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( ( PNULL == g_ipr_cwmp_crypto_params->gw.SecGWServer1 ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer1)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_crypto_params->gw.SecGWServer1;//free this in caller of this function in this case
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_setSecGWServer2
 * Inputs         : UInt8* sec_gw_server2
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the SecGWServer3 in the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway Structure
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_setSecGWServer2(UInt8* sec_gw_server2)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto params");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( PNULL == sec_gw_server2 )
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer2, 0, STR_MAX_LEN + 1);
        ipr_cwmp_add_gw2_ip(sec_gw_server2);
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer2, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer2, (char*)sec_gw_server2, STR_MAX_LEN);
    	g_ipr_cwmp_crypto_params->gw.SecGWServer2[STR_MAX_LEN] = '\0';
        ipr_cwmp_add_gw2_ip(sec_gw_server2);
	}
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto params");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved crypto params");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSecGWServer2
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt8* SecGWServer2
 * Description    : This function gets the SecGWServer2 from the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway
 *                  Structure
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getSecGWServer2()
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto params");
        return PNULL;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( ( PNULL == g_ipr_cwmp_crypto_params->gw.SecGWServer2 ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer2)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_crypto_params->gw.SecGWServer2;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSecGWServer3
 * Inputs         : UInt8* sec_gw_server3
 *
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function gets the SecGWServer2 from the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway
 *                  Structure
 ****************************************************************************/
	
UInt32 ipr_cwmp_setSecGWServer3(UInt8* sec_gw_server3)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save Sec GW3");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( PNULL == sec_gw_server3)
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer3, 0, STR_MAX_LEN + 1);
        ipr_cwmp_add_gw3_ip(sec_gw_server3);
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->gw.SecGWServer3, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer3, (char*)sec_gw_server3, STR_MAX_LEN);
    	g_ipr_cwmp_crypto_params->gw.SecGWServer3[STR_MAX_LEN] = '\0';
        ipr_cwmp_add_gw3_ip(sec_gw_server3);
	}
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto params");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved Sec GW3 ");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_getSecGWServer3
 * Inputs         : None
 *
 * Outputs        : None
 * Returns        : UInt8* SecGWServer3
 * Description    : This function gets the SecGWServer2 from the
 *                  device_services_fapservice_1_fapcontrol_lte_gateway
 *                  Structure
 ****************************************************************************/
	
UInt8*  ipr_cwmp_getSecGWServer3()
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read Sec GW3");
        return PNULL;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( ( PNULL == g_ipr_cwmp_crypto_params->gw.SecGWServer3 ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_crypto_params->gw.SecGWServer3)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_crypto_params->gw.SecGWServer3;
	}
}

/****************************************************************************
 * Function Name  : ipr_cwmp_get_crypto_profile_no_entries
 * Inputs         : None
 *                 
 * Outputs        : None
 * Returns        : UInt32 crypto_profile_number_of_entries
 * Description    : This function gets the SecGWServer3 from the
 *                  device_fap_tunnel Structure
 *                  
 ****************************************************************************/
	
UInt32  ipr_cwmp_get_crypto_profile_no_entries()
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto param number of entries");
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    	return (g_ipr_cwmp_crypto_params->tunnel.crypto_profile_number_of_entries?g_ipr_cwmp_crypto_params->tunnel.crypto_profile_number_of_entries:1);//free this in caller function
        //returning 1 if 0 is stored in the file as we are supporting only one entry in this release
}

/****************************************************************************
 * Function Name  : ipr_cwmp_set_crypto_enable
 * Inputs         : bool enable_crypto
 *                 
 * Outputs        : None
 * Returns        : UInt32
 * Description    : This function sets the Enable field in crypto profile table
 *                  
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_set_crypto_enable(bool enable_crypto)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    //open the file for reading
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param enable");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    //set the value
	if( 
	(1 < enable_crypto) )
	{
	    TRACE ( 1 , "Management Server Model", "Crypto Enable invalid, not set ");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
	    return FAILED;
	}
	else
	{
    	g_ipr_cwmp_crypto_params->crypto_prof.enable = enable_crypto;
    	g_crypto_profile.enable = enable_crypto;
        file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
        if(file == NULL)
        {
            TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param enable");
            DEALLOCATE(g_ipr_cwmp_crypto_params);
            return FAILED;
        }
        else
        {
            ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
            TRACE ( 1 , "Management Server Model", "Saved crypto param enable");
            ENB_FCLOSE(file);
        }
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_get_crypto_enable
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : bool enable from crypto profile table
 * Description    : This function gets the Enable field in crypto profile table
 *                  
 *                  
 ****************************************************************************/
	
bool ipr_cwmp_get_crypto_enable(void)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto param enable");
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    return g_ipr_cwmp_crypto_params->crypto_prof.enable;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_set_ike_rekey_lifetime
 * Inputs         : UInt64 ike_rekey_lifetime
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function set the ike_rekey_lifetime in
 *                  crypto profile table
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_set_ike_rekey_lifetime(UInt64 ike_rekey_lifetime)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    //open the file for reading
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param ike_rekey_lifetime");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    //set the value
	if ( 0 == ike_rekey_lifetime)
	{
    	g_ipr_cwmp_crypto_params->crypto_prof.ike_rekey_lifetime = 0;
    	g_crypto_profile.ike_rekey_lifetime = 0;
	}
	else
	{
    	g_ipr_cwmp_crypto_params->crypto_prof.ike_rekey_lifetime = ike_rekey_lifetime;
    	g_crypto_profile.ike_rekey_lifetime = ike_rekey_lifetime;
	}
    //write this back
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param ike_rekey_lifetime");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved crypto param ike_rekey_lifetime");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_get_ike_rekey_lifetime
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function get the ike_rekey_lifetime in
 *                  crypto profile table
 *                  
 ****************************************************************************/
	
UInt64 ipr_cwmp_get_ike_rekey_lifetime(void)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto param ike_rekey_lifetime");
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    return g_ipr_cwmp_crypto_params->crypto_prof.ike_rekey_lifetime;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_set_ipsec_rekey_lifetime_time
 * Inputs         : UInt64 ipsec_rekey_lifetime_time
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function set the ipsec_rekey_lifetime_time in
 *                  crypto profile table
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_set_ipsec_rekey_lifetime_time(UInt64 ipsec_rekey_lifetime_time)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    //open the file for reading
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param ipsec_rekey_lifetime_time");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    //set the value
	if ( 0 == ipsec_rekey_lifetime_time)
	{
    	g_ipr_cwmp_crypto_params->crypto_prof.ipsec_rekey_lifetime_time = 0;
    	g_crypto_profile.ipsec_rekey_lifetime_time = 0;
	}
	else
	{
    	g_ipr_cwmp_crypto_params->crypto_prof.ipsec_rekey_lifetime_time = ipsec_rekey_lifetime_time;
    	g_crypto_profile.ipsec_rekey_lifetime_time = ipsec_rekey_lifetime_time;
	}
    //write this back
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param ipsec_rekey_lifetime_time");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved crypto param ipsec_rekey_lifetime_time");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_get_ipsec_rekey_lifetime_time
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function get the ipsec_rekey_lifetime_time in
 *                  crypto profile table
 *                  
 ****************************************************************************/
	
UInt64 ipr_cwmp_get_ipsec_rekey_lifetime_time(void)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto param ipsec_rekey_lifetime_time");
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    return g_ipr_cwmp_crypto_params->crypto_prof.ipsec_rekey_lifetime_time;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_set_authmethod
 * Inputs         : UInt8* authmethod
 *                 
 * Outputs        : None
 * Returns        : SUCCESS/FAILED
 * Description    : This function sets the authmethod in crypto profile table
 *                  
 ****************************************************************************/
	
UInt32 ipr_cwmp_set_authmethod(UInt8* authmethod)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    //open the file for reading
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param authmethod");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
    //set the value
	if ( PNULL == authmethod)
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->crypto_prof.authmethod, 0, STR_MAX_LEN + 1);
        //set this gloabl var also to use
    	ENB_MEMSET(g_crypto_profile.authmethod, 0, STR_MAX_LEN + 1);
	}
	else
	{
    	ENB_MEMSET(g_ipr_cwmp_crypto_params->crypto_prof.authmethod, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_ipr_cwmp_crypto_params->crypto_prof.authmethod, (char*)authmethod, STR_MAX_LEN);
    	g_ipr_cwmp_crypto_params->crypto_prof.authmethod[STR_MAX_LEN] = '\0';
        //set this gloabl var also to use
    	ENB_MEMSET(g_crypto_profile.authmethod, 0, STR_MAX_LEN + 1);
    	ENB_STRNCPY((char*)g_crypto_profile.authmethod, (char*)authmethod, STR_MAX_LEN);
    	g_crypto_profile.authmethod[STR_MAX_LEN] = '\0';
	}
    //write this back
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "w");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to save crypto param authmethod");
        DEALLOCATE(g_ipr_cwmp_crypto_params);
        return FAILED;
    }
    else
    {
        ENB_FWRITE(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        TRACE ( 1 , "Management Server Model", "Saved crypto param authmethod");
        ENB_FCLOSE(file);
    }
    DEALLOCATE(g_ipr_cwmp_crypto_params);
   	return SUCCESS;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_get_authmethod
 * Inputs         : void
 *                 
 * Outputs        : None
 * Returns        : UInt8* authmethod
 * Description    : This function gets the authmethod from crypto profile table
 *                  
 ****************************************************************************/
	
UInt8*  ipr_cwmp_get_authmethod(void)
{
    FILE *file = NULL;
    g_ipr_cwmp_crypto_params = (ipr_cwmp_crypto_params_t*) ALLOCATE( sizeof(ipr_cwmp_crypto_params_t));
    ENB_MEMSET(g_ipr_cwmp_crypto_params, 0, sizeof(ipr_cwmp_crypto_params_t));
    file = ENB_FOPEN(CRYPTO_PARAMS_FILE, "r");
    if(file == NULL)
    {
        TRACE ( 1 , "Management Server Model", "Error in opening file to read crypto param authmethod");
        return PNULL;
    }
    else
    {
        ENB_FREAD(g_ipr_cwmp_crypto_params, sizeof(ipr_cwmp_crypto_params_t), 1, file);
        ENB_FCLOSE(file);
    }
	if ( ( PNULL == g_ipr_cwmp_crypto_params->crypto_prof.authmethod ) ||
	( 0 == ENB_STRLEN((char*)g_ipr_cwmp_crypto_params->crypto_prof.authmethod)) )
	{
    	return PNULL;
	}
	else
	{
    	return g_ipr_cwmp_crypto_params->crypto_prof.authmethod;
	}

}

/****************************************************************************
 * Function Name  : ipr_cwmp_add_gw1_ip 
 * Inputs         : UInt8* ip
 *
 * Outputs        : None
 * Returns        : void 
 * Description    : This function gets the authmethod from crypto profile table
 *
 ****************************************************************************/

void ipr_cwmp_add_gw1_ip(UInt8* ip)
{
    FILE *file = NULL;
    config_res_t *tunnel_ips = PNULL;
    UInt8* get_ip_addr = PNULL;

    tunnel_ips = (config_res_t*) ALLOCATE( sizeof(config_res_t));
    memset(tunnel_ips, 0, sizeof(config_res_t));
    file = fopen(TRO69_IP_SAVE, "r");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    else
    {
        fread(tunnel_ips, sizeof(config_res_t), 1, file);
        fclose(file);
    }
    if (ip)
    {
        if(ip[0] < '1' || ip[0] > '9')
        {
            get_ip_addr = (UInt8*)ipr_cwmp_get_ip(ip);
            strncpy(tunnel_ips->SecGWServer1, get_ip_addr,16);
            DEALLOCATE(get_ip_addr);
        }
        else
        {
            strncpy(tunnel_ips->SecGWServer1, ip, strlen(ip));
        }
    }
    else
    {
        DEALLOCATE(tunnel_ips);
        return;
    }

    file = fopen(TRO69_IP_SAVE, "w");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    fwrite(tunnel_ips, sizeof(config_res_t), 1, file);
	TRACE ( 1 , "Management Server Model", "Saved GW IP of SGW1 for use in tunnel APIs");
    fclose(file);
    DEALLOCATE(tunnel_ips);
    return;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_add_gw2_ip
 * Inputs         : UInt8* ip
 *
 * Outputs        : None
 * Returns        : void
 * Description    : This function gets the authmethod from crypto profile table
 *
 ****************************************************************************/

void ipr_cwmp_add_gw2_ip(UInt8* ip)
{
    FILE *file = NULL;
    config_res_t *tunnel_ips = PNULL;
    UInt8* get_ip_addr = PNULL;

    tunnel_ips = (config_res_t*) ALLOCATE( sizeof(config_res_t));
    memset(tunnel_ips, 0, sizeof(config_res_t));
    file = fopen(TRO69_IP_SAVE, "r");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    else
    {
        fread(tunnel_ips, sizeof(config_res_t), 1, file);
        fclose(file);
    }
    if (ip)
    {
        if(ip[0] < '1' || ip[0] > '9')
        {
            get_ip_addr = (UInt8*)ipr_cwmp_get_ip(ip);
            strncpy(tunnel_ips->SecGWServer2, get_ip_addr,16);
            DEALLOCATE(get_ip_addr);
        }
        else
        {
            strncpy(tunnel_ips->SecGWServer2, ip, strlen(ip));
        }
    }
    else
    {
        DEALLOCATE(tunnel_ips);
        return;
    }

    file = fopen(TRO69_IP_SAVE, "w");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    fwrite(tunnel_ips, sizeof(config_res_t), 1, file);
	TRACE ( 1 , "Management Server Model", "Saved SGW2 IP for use in tunnel APIs");
    fclose(file);
    DEALLOCATE(tunnel_ips);
    return;
}

/****************************************************************************
 * Function Name  : ipr_cwmp_add_gw3_ip
 * Inputs         : UInt8* ip
 *
 * Outputs        : None
 * Returns        : void
 * Description    : This function gets the authmethod from crypto profile table
 *
 ****************************************************************************/

void ipr_cwmp_add_gw3_ip(UInt8* ip)
{
    FILE *file = NULL;
    config_res_t *tunnel_ips = PNULL;
    UInt8* get_ip_addr = PNULL;

    tunnel_ips = (config_res_t*) ALLOCATE( sizeof(config_res_t));
    memset(tunnel_ips, 0, sizeof(config_res_t));
    file = fopen(TRO69_IP_SAVE, "r");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    else
    {
        fread(tunnel_ips, sizeof(config_res_t), 1, file);
        fclose(file);
    }
    if (ip)
    {
        if(ip[0] < '1' || ip[0] > '9')
        {
            get_ip_addr = (UInt8*)ipr_cwmp_get_ip(ip);
            strncpy(tunnel_ips->SecGWServer3, get_ip_addr,16);
            DEALLOCATE(get_ip_addr);
        }
        else
        {
            strncpy(tunnel_ips->SecGWServer3, ip, strlen(ip));
        }
    }
    else
    {
        DEALLOCATE(tunnel_ips);
        return;
    }

    file = fopen(TRO69_IP_SAVE, "w");
    if(file == NULL)
    {
	    TRACE ( 1 , "Management Server Model", "Error in opening file to save GW IPs");
        DEALLOCATE(tunnel_ips);
        return;
    }
    fwrite(tunnel_ips, sizeof(config_res_t), 1, file);
	TRACE ( 1 , "Management Server Model", "Saved SGW3 IP for use in tunnel APIs");
    fclose(file);
    DEALLOCATE(tunnel_ips);
    return;
}

#endif
//IPSec phase 2 code ends
