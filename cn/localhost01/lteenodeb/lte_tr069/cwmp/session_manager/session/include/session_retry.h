#include	<stdio.h>
#include	<session_manager.h>
/* SPR 21002 FIXED START */
#define CONFIG_LEN 256
/* code removed */
#define FOUR 4
/* SPR 21002 FIXED END */
sInformResponse * ipr_cwmp_Session_Retry(sInform *informReq, char *URL, sHeader *, char *, char *, char **);
UINT4 ipr_cwmp_get_retry_count(void);
void ipr_cwmp_set_retry_count(UINT4 status);


