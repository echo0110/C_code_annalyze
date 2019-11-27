
#include <stdio.h>
#include <time.h>
#include "../include/ManagementServerModel.h"


void sunny_day();
void rainy_day();

int main()
{
//	ENB_PRINTF("Sunny Day\n");
//	sunny_day();
	ENB_PRINTF("\n\n\n\n Rainy Day \n");
	rainy_day();
	return 0;
}




void sunny_day()
{

	setPeriodicInformEnable(true);
	setPeriodicInformInterval(2);
	setUsername("USER");
	setPassword("PASS");
	setACSURL("www.google.com");
	setPeriodicInformTime(1, 1, 1, 1, 1, 1);
	setPeriodicInformInterval ( 500 );
	setPeriodicInformEnable(1 );
	setCWMPRetryIntervalMultiplier ( 1  );
	setCWMPRetryMinimumWaitInterval ( 1  );
	setDefaultActiveNotificationThrottle ( 25  );
	setUDPConnectionRequestAddress( "Downing Street" );
	setDownloadProgressURL ( "www.progress.com"  );
	setKickURL ( "www.kickass.com" );
	setUpgradesManaged ( true );
	setConnectionRequestPassword( "con pass"  );
	setConnectionRequestUsername( "con user" );
	setParameterKey ( "PARAM_KEY");
	setEnableCWMP(1);
	setConnectionRequestURL ( "CONREQ_URL.com" );
	setAlias("Larry");
	setManufacturerOUI("sdbdsfb6ABC41");
	setSerialNumber("546");
	setProductClass("3223f");
	setHost("Moves Like Jagger");




	char *URL = (char *)getACSURL();
	bool a = getPeriodicInformEnable();
	ENB_PRINTF("periodic inform enable IS %d\n", a);
	
	unsigned int b = getPeriodicInformInterval();
	ENB_PRINTF("periodic inform interval is %u", b);
	long c = (long)getPeriodicInformTime();
	ENB_PRINTF("periodic inform time = %ld\n", c);
	char * d = (char *)getUsername();
	ENB_PRINTF("username = %s\n", d);
	char * e = (char *)getPassword();
	ENB_PRINTF("password = %s\n", e);
	int f = (int )getEnableCWMP ();
	ENB_PRINTF("enable cwmp = %d\n", f);
	char * g = (char *)getParameterKey ();
	ENB_PRINTF("parameter key = %s", g);
	char * h = (char *)getConnectionRequestURL ();
	ENB_PRINTF("con req url = %s\n", h);
	char * i = (char *)getConnectionRequestUsername ();
	ENB_PRINTF("con req username = %s", i);
	char * j = (char *)getConnectionRequestPassword ();
	ENB_PRINTF("con password = %s\n", j);
	bool k = getUpgradesManaged ();
	ENB_PRINTF("Upgrades Managed is %d\n", k);
	char * l = (char *)getKickURL ();
	ENB_PRINTF("kick URL = %s\n", l);
	char * m = (char *)getDownloadProgressURL ();
	ENB_PRINTF("Download Progress URL = %s\n", m);
	char * n = (char *)getUDPConnectionRequestAddress ();
	ENB_PRINTF("UDP Connection Request Address = %u\n", n);
	unsigned int o = getDefaultActiveNotificationThrottle ( );
	ENB_PRINTF("Throttle = %u\n", o);
	unsigned int p = getCWMPRetryMinimumWaitInterval (  );
	ENB_PRINTF("imin wait interval = %u\n", p);
	unsigned int q = getCWMPRetryIntervalMultiplier (  );
	ENB_PRINTF("retrymin wait int =  %u\n", q);

	char * alias = (char *)getAlias();
	ENB_PRINTF("ALias is %s\n", alias);	

	char * manoui = (char *)getManufacturerOUI();
	ENB_PRINTF("Manufacturer OUI is %s\n", manoui);	
	
	char * prodclas = (char *)getProductClass();
	ENB_PRINTF("Product Class is %s\n", prodclas);	
	
	char * s_no  = (char *)getSerialNumber();
	ENB_PRINTF("SerialNumber is %s\n",s_no);	
	
	char * host = (char *)getHost();
	ENB_PRINTF("Host is %s\n", host);	

}



void rainy_day()
{

	setPeriodicInformEnable(0);
	setUsername(NULL);
	setPassword(NULL);
	setACSURL(NULL);
//	setPeriodicInformTime(60,60,60,60,60,60);
//	setPeriodicInformInterval (10000000);
	setPeriodicInformEnable(false);
//	setCWMPRetryIntervalMultiplier (1234567890);
//	setCWMPRetryMinimumWaitInterval (1234567890);
//	setDefaultActiveNotificationThrottle (1234567890);
	setUDPConnectionRequestAddress(NULL);
	setDownloadProgressURL (NULL );
	setKickURL (NULL );
	setUpgradesManaged (0);
	setConnectionRequestPassword(NULL);
	setConnectionRequestUsername(NULL );
	setParameterKey (NULL);
	setEnableCWMP(0);
	setConnectionRequestURL ( NULL);
	setAlias(NULL);
	setManufacturerOUI(NULL);
	setSerialNumber(NULL);
	setProductClass(NULL);
	setHost(NULL);




	char *URL = (char *)getACSURL();
	bool a = getPeriodicInformEnable();
	ENB_PRINTF("periodic inform enable IS %d\n", a);
	
	unsigned int b = getPeriodicInformInterval();
	ENB_PRINTF("periodic inform interval is %u", b);
	long c = (long)getPeriodicInformTime();
	ENB_PRINTF("periodic inform time = %ld\n", c);
	char * d = (char *)getUsername();
	ENB_PRINTF("username = %s\n", d);
	char * e = (char *)getPassword();
	ENB_PRINTF("password = %s\n", e);
	int f = (int )getEnableCWMP ();
	ENB_PRINTF("enable cwmp = %d\n", f);
	char * g = (char *)getParameterKey ();
	ENB_PRINTF("parameter key = %s", g);
	char * h = (char *)getConnectionRequestURL ();
	ENB_PRINTF("con req url = %s\n", h);
	char * i = (char *)getConnectionRequestUsername ();
	ENB_PRINTF("con req username = %s", i);
	char * j = (char *)getConnectionRequestPassword ();
	ENB_PRINTF("con password = %s\n", j);
	bool k = getUpgradesManaged ();
	ENB_PRINTF("Upgrades Managed is %d\n", k);
	char * l = (char *)getKickURL ();
	ENB_PRINTF("kick URL = %s\n", l);
	char * m = (char *)getDownloadProgressURL ();
	ENB_PRINTF("Download Progress URL = %s\n", m);
	char * n = (char *)getUDPConnectionRequestAddress ();
	ENB_PRINTF("UDP Connection Request Address = %s\n", n);
	unsigned int o = getDefaultActiveNotificationThrottle ( );
	ENB_PRINTF("Throttle = %u\n", o);
	unsigned int p = getCWMPRetryMinimumWaitInterval (  );
	ENB_PRINTF("imin wait interval = %u\n", p);
	unsigned int q = getCWMPRetryIntervalMultiplier (  );
	ENB_PRINTF("retrymin wait int =  %u\n", q);

	char * alias = (char *)getAlias();
	ENB_PRINTF("ALias is %s\n", alias);	

	char * manoui = (char *)getManufacturerOUI();
	ENB_PRINTF("Manufacturer OUI is %s\n", manoui);	
	
	char * prodclas = (char *)getProductClass();
	ENB_PRINTF("Product Class is %s\n", prodclas);	
	
	char * s_no  = (char *)getSerialNumber();
	ENB_PRINTF("SerialNumber is %s\n",s_no);	
	
	char * host = (char *)getHost();
	ENB_PRINTF("Host is %s\n", host);	

}



