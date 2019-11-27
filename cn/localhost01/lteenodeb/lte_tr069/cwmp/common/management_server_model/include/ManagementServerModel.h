/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ManagementServerModel.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions to set and get objects in 
 *                   management server model 
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2012/11/07 gur24604  Initial Version 1.0
 *   2013/03/25 gur24198  Removed Warnings
 *
******************************************************************************/

#ifndef _MANAGEMENTSERVERMODEL_H_
#define _MANAGEMENTSERVERMODEL_H_

/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lteTypes.h>
#include <logger.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <strfun.h>

#ifdef __bool_true_false_are_defined
#   undef true
#   undef false
#   undef bool
 
#   define true 1
#   define false 0
#   define bool UInt8
#endif
/*******************************************************************************
 *
 *               MACROS
 *
 *******************************************************************************/
#define SUCCESS 0
#define FAILED 1
//#define TRACE trace
#define STR_MAX_LEN 64
#define STRING_MAX_LEN 256
#define BUFF_MAX_LEN 1024
#define BUFFER_MAX_LEN 2048
#define DEFAULT_WAIT_INTERVAL  5
#define DEFAULT_INTERVAL_MULTIPILER  2000
#ifdef HAVE_IPSEC
#define TRO69_IP_SAVE "./tr069_ip_save" 
#endif

/********************************************************************************
*				  STRUCTURES
*
*******************************************************************************/

typedef struct Device_ManagementServer_ManageableDevice_i
{
	UInt8 Alias[STR_MAX_LEN + 1];
	UInt8 ManufacturerOUI[7]; // [6:6]
	UInt8 SerialNumber[STR_MAX_LEN + 1];
	UInt8 ProductClass[STR_MAX_LEN + 1];
	UInt8 Host[STR_MAX_LEN + 1];
    UInt8 SoftVers[STR_MAX_LEN + 1];
}Device_ManagementServer_ManageableDevice_i;
	
typedef struct Device_ManagementServer_AutonomousTransferCompletePolicy
{
	bool Enable;
	UInt8 * TransferTypeFilter;  // Enums
	UInt8 * ResultTypeFilter;    // Enums
	UInt8 FileTypeFilter[BUFF_MAX_LEN + 1];
}Device_ManagementServer_AutonomousTransferCompletePolicy;

typedef struct Device_ManagementServer_DownloadAvailability_Announcement_Group_i
{
	bool Enable;
	UInt8 *Status; //Enum
	UInt8 URL[STRING_MAX_LEN + 1];
}Device_ManagementServer_DownloadAvailability_Announcement_Group_i;
	
typedef struct Device_ManagementServer_DownloadAvailability_Announcement
{
	bool Enable;
	UInt8 * Status; // Enum
	 UInt32 GroupNumberOfEntries;
	struct Device_ManagementServer_DownloadAvailability_Announcement_Group_i Group_i;	
}Device_ManagementServer_DownloadAvailability_Announcement;

typedef struct Device_ManagementServer_DownloadAvailability
{
	struct  Device_ManagementServer_DownloadAvailability_Announcement Announcement;
}Device_ManagementServer_DownloadAvailability;

typedef struct Device_ManagementServer_DUStateChangeComplPolicy
{
	bool Enable;
	UInt8 OperationTypeFilter; //ENum
	UInt8 ResultTypeFilter; //Enum
	UInt8 FaultCodeFilter; //Enum
}Device_ManagementServer_DUStateChangeComplPolicy;

typedef struct Device_ManagementServer
{
	bool EnableCWMP;
	UInt8 URL[STRING_MAX_LEN + 1];
	UInt8 Username[STRING_MAX_LEN + 1];
	UInt8 Password[STRING_MAX_LEN + 1];
	bool PeriodicInformEnable;
	S32 PeriodicInformInterval;   // [1:]
	time_t PeriodicInformTime;
	UInt8 ParameterKey[32];
	UInt8 ConnectionRequestURL[STRING_MAX_LEN + 1];
	UInt8 ConnectionRequestUsername[STRING_MAX_LEN + 1];
	UInt8 ConnectionRequestPassword[STRING_MAX_LEN + 1];
	bool UpgradesManaged;
	UInt8 KickURL[STRING_MAX_LEN + 1];
	UInt8 DownloadProgressURL[STRING_MAX_LEN + 1];
	UInt32 DefaultActiveNotificationThrottle;
	UInt32 CWMPRetryMinimumWaitInterval ;  // [1:65535]
	UInt32 CWMPRetryIntervalMultiplier ;    // [1000:65535]
	UInt8 UDPConnectionRequestAddress[STRING_MAX_LEN + 1];
	bool STUNEnable;
	UInt8 STUNServerAddress[STRING_MAX_LEN + 1];
	UInt32 STUNServerPort;  // [0:65535]
	UInt8 STUNUsername[STRING_MAX_LEN + 1];
	UInt8 STUNPassword[STRING_MAX_LEN + 1];
	UInt32 STUNMaximumKeepAlivePeriod;  //[-1:]
	UInt32 STUNMinimumKeepAlivePeriod;
	bool NATDetected;
	UInt32 ManageableDeviceNumberOfEntries;
	struct Device_ManagementServer_ManageableDevice_i DMS_i;
	struct Device_ManagementServer_AutonomousTransferCompletePolicy Aut_Trans_Comp;
	struct Device_ManagementServer_DUStateChangeComplPolicy DUState_Change;
}Device_ManagementServer; 

//IPSEC phase 2 code starts
#ifdef HAVE_IPSEC
typedef struct device_services_fapservice_1_fapcontrol_lte_gateway
{
    UInt8 SecGWServer1[STR_MAX_LEN + 1];//Gateways that FAP is connected to
    UInt8 SecGWServer2[STR_MAX_LEN + 1];//Second SecGW the LTE FAP attempts to establish connection with. Either hostname or IPaddress
    UInt8 SecGWServer3[STR_MAX_LEN + 1];//Third SecGW the LTE FAP attempts to establish connection with. Either hostname or IPaddress
} ipr_cwmp_device_services_fapservice_1_fapcontrol_lte_gateway_t;

typedef struct device_fap_tunnel
{
	UInt32 crypto_profile_number_of_entries;//The number of entries in the CryptoProfile table.
} ipr_cwmp_device_fap_tunnel_t;

typedef struct device_management_server_fap_tunnel_crypto_profile
{
    UInt8       enable; //Enables and disables this entry
    UInt8       authmethod[STRING_MAX_LEN + 1]; //Specifies the Security mechanism and set of credentials used by the device to authenticate itself 
    UInt64      ike_rekey_lifetime; //IKEv2 SA rekey timeout in seconds.
    UInt64      ipsec_rekey_lifetime_time;  //IPsec SA rekey timeout in seconds.
} ipr_cwmp_device_fap_tunnel_crypto_profile_t;

typedef struct crypto_params
{
    ipr_cwmp_device_services_fapservice_1_fapcontrol_lte_gateway_t  gw;
    ipr_cwmp_device_fap_tunnel_t                                    tunnel;
    ipr_cwmp_device_fap_tunnel_crypto_profile_t                     crypto_prof;
} ipr_cwmp_crypto_params_t;
#endif
//IPSEC phase 2 code ends

/********************************************************************************
*
*                                 FUNCTION PROTOTYPES
*
*******************************************************************************/

//Getter Functions
UInt8* ipr_cwmp_get_acs_host_name(void);
UInt8* ipr_cwmp_get_session_acs_host_name(void);
bool ipr_cwmp_getPeriodicInformEnable(void);
UInt32 ipr_cwmp_getPeriodicInformInterval(void);
time_t ipr_cwmp_getPeriodicInformTime(void);
UInt8* ipr_cwmp_getUsername(void);
UInt8* ipr_cwmp_getPassword(void);
bool ipr_cwmp_getEnableCWMP (void);
UInt8* ipr_cwmp_getParameterKey (void);
UInt8* ipr_cwmp_getConnectionRequestURL (void);
UInt8* ipr_cwmp_getConnectionRequestUsername (void);
UInt8* ipr_cwmp_getConnectionRequestPassword (void);
bool ipr_cwmp_getUpgradesManaged (void);
UInt8* ipr_cwmp_getKickURL (void);
UInt8* ipr_cwmp_getDownloadProgressURL (void);
UInt8* ipr_cwmp_getUDPConnectionRequestAddress (void);
UInt32 ipr_cwmp_getDefaultActiveNotificationThrottle ( void );
UInt32 ipr_cwmp_getCWMPRetryMinimumWaitInterval ( void );
UInt32 ipr_cwmp_getCWMPRetryIntervalMultiplier ( void );
UInt8* ipr_cwmp_getACSURL(void);
UInt8* ipr_cwmp_getAlias(void);
UInt8* ipr_cwmp_getManufacturerOUI(void);
UInt8* ipr_cwmp_getSerialNumber(void);
UInt8* ipr_cwmp_getProductClass(void);
UInt8* ipr_cwmp_getHost(void);

//Setter Functions
void ipr_cwmp_set_acs_host_name(UInt8* name);
void ipr_cwmp_set_session_acs_host_name(UInt8* name);
UInt32 ipr_cwmp_setPeriodicInformTime(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
UInt32 ipr_cwmp_setPassword(Char8*);
UInt32 ipr_cwmp_setUsername(Char8 *);
UInt32 ipr_cwmp_setPeriodicInformInterval(UInt32);
UInt32 ipr_cwmp_setPeriodicInformEnable(bool);
UInt32 ipr_cwmp_setACSURL(Char8 * URL);
UInt32 ipr_cwmp_setCWMPRetryIntervalMultiplier(UInt32);
UInt32 ipr_cwmp_setCWMPRetryMinimumWaitInterval(UInt32);
UInt32 ipr_cwmp_setDefaultActiveNotificationThrottle(UInt32);
UInt32 ipr_cwmp_setUDPConnectionRequestAddress(Char8*);
UInt32 ipr_cwmp_setDownloadProgressURL(Char8*);
UInt32 ipr_cwmp_setKickURL(Char8*);
UInt32 ipr_cwmp_setUpgradesManaged(bool upgrades_managed);
UInt32 ipr_cwmp_setConnectionRequestPassword(Char8*);
UInt32 ipr_cwmp_setConnectionRequestUsername(Char8*);
UInt32 ipr_cwmp_setParameterKey(Char8*);
UInt32 ipr_cwmp_setEnableCWMP(bool);
UInt32 ipr_cwmp_setConnectionRequestURL(Char8*);
UInt32 ipr_cwmp_setAlias(UInt8*);
UInt32 ipr_cwmp_setManufacturerOUI(UInt8 *);
UInt32 ipr_cwmp_setSerialNumber(UInt8 *);
UInt32 ipr_cwmp_setProductClass(UInt8 *);
UInt32 ipr_cwmp_setHost(UInt8 *);
//IPSec phase 2 functions
#ifdef HAVE_IPSEC
UInt32 ipr_cwmp_setSecGWServer1(UInt8* sec_gw_server1);
UInt8*  ipr_cwmp_getSecGWServer1();
UInt32 ipr_cwmp_setSecGWServer2(UInt8* sec_gw_server2);
UInt8*  ipr_cwmp_getSecGWServer2();
UInt32 ipr_cwmp_setSecGWServer3(UInt8* sec_gw_server3);
UInt8*  ipr_cwmp_getSecGWServer3();
UInt32  ipr_cwmp_get_crypto_profile_no_entries();
UInt32 ipr_cwmp_set_crypto_profile_no_entries(UInt32 crypto_profile_number_of_entries);
void ipr_cwmp_add_gw1_ip(UInt8* ip);
void ipr_cwmp_add_gw2_ip(UInt8* ip);
void ipr_cwmp_add_gw3_ip(UInt8* ip);
UInt8* ipr_cwmp_get_authmethod(void);


//Global varibales for IPSec params
extern ipr_cwmp_device_fap_tunnel_crypto_profile_t                      g_crypto_profile;
extern ipr_cwmp_device_services_fapservice_1_fapcontrol_lte_gateway_t   g_ipr_cwmp_lte_gw;
extern ipr_cwmp_device_fap_tunnel_t                                     g_ipr_cwmp_fap_tunnel;
extern ipr_cwmp_crypto_params_t*                                         g_ipr_cwmp_crypto_params;
#define CRYPTO_PARAMS_FILE  "./.crypto_params_data_model"
#endif

UInt32 ipr_cwmp_setSoftVers(UInt8* SoftVers);
UInt8* ipr_cwmp_getSoftVers(void);

#endif
