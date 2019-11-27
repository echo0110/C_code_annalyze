#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<logger.h>
#include<lteTypes.h>
#include<ipr_cwmp_read_config.h>

#define LOG_CFG_PATH "LOG_CFG_PATH"

void config_log (void)
{
	UInt8* cert_file_path = NULL;
	FILE *fd = NULL;
	char *str = NULL;
	char *str1 = NULL;
	char tmpBuffer[2048] = {'\0'};
	char *name = NULL; 
	char *value= NULL;
	int res=0;



	cert_file_path = log_cfg_path;
	if (cert_file_path)
	{
		fd = fopen((char*)cert_file_path , "r");
		if (fd)
		{
			res =  ENB_FSCANF (fd,"%255s",tmpBuffer);

	str = strtok(tmpBuffer , "\n");

	while (str != NULL && res > 0)
	{
		str1 = strchr( str , '=');
		name =  strtok(str,"=");
		if (str1 != NULL)
			value = strtok(str1+1,"=");

		if (str1 != NULL)
		{

			ENB_PRINTF ("\nName=  : %s\nValue=%s\n",  name, value);
			if (name!=NULL && strstr (name , "LOG_LEVEL") != NULL)
			{
				if (NULL != value)
				{
					logger_setLogLevel(atoi(value));	
				}
			}
			else if(name!=NULL) 
			{
				logger_addModule(name , atoi(value));
			}
		}
		res= ENB_FSCANF(fd,"%255s",tmpBuffer);
		str = strtok(tmpBuffer , "\n");
	}
	fclose(fd);
		}
	}
}

