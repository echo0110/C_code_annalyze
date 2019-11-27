#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


#define CLIENT_API_ID   0xFFF0

static unsigned short int  oamPort = 13657;
char oamIP [20] = {0};
typedef struct msg{
    unsigned short int transactionId;	
    unsigned short int srcId;	
    unsigned short int dstId;	
    unsigned short int apiId;
    unsigned short int msgLen;
    char data[512];
}Msg;

void diep(char *s)
{
    perror(s);
    return ;
}
void ReadSocketsConfig(const char  *filename_p)
{
  FILE *fp;
  unsigned int  input=0;
  char buf[512] = {0,};
  char getstring[512] = {0};

  fp = fopen(filename_p,"r");
  if(fp == NULL) {
    printf("Fail to open the  config file %s",filename_p);
  }


  /*read the port numbers */
  while( fgets(buf,100,fp))
  {
    if(sscanf(buf,"OAM_SIMULATOR_PORT =%d",&input) > 0)
    {
      if(input > 0 && input < 65535 )
      {
        oamPort = (unsigned short int)input;
      
      }
    }
    else if (sscanf(buf,"OAM_MODULE_HOST_NAME =%s",getstring) > 0){
        strcpy(oamIP,getstring);
    }
  } 

}

int main(int argc,char*argv[])
{

	if(argc != 2)
	{
		printf("USAGE : <client> <script_path>\n");
		return -1;
	}

	/*Read config*/
	const char  * filename_p = "../cfg/lteLayer2PortConfig.cfg";
	ReadSocketsConfig(filename_p);
	struct sockaddr_in si_other;
	int sockId = -1,  slen=sizeof(si_other);
	Msg  message = {0};
	message.apiId = CLIENT_API_ID;
	if ((sockId =socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(oamPort);
    if (inet_aton(oamIP, &si_other.sin_addr)==0) {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
    }
	strcpy(message.data,argv[1]);
	size_t len = 11 + strlen(argv[1]);

	if (sendto(sockId, &message, len, 0, (struct sockaddr*)&si_other, slen)==-1)
		diep("sendto()");

	close(sockId);
	return 0;
}

