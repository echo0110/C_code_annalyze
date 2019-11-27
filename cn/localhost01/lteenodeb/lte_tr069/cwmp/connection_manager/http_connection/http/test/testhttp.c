#include <stdio.h>
#include <stdlib.h>


int send_http_msg(char *msg, int method, char *URL, char *response_message);
void test_send_recv(void);
void test_send_empty(void);
void test_send_bad_URL(void);

#define SUCCESS 0
#define FAILURE 1
#define EMPTY 2


int main()
{
//	test_send_recv();
	test_send_bad_URL();
	//test_send_empty();
	return 0;
}



void test_send_empty()
{
	char response_message[1024] = {'\0'};
	int status;
	status = send_http_msg("", 1, "http://10.203.152.214:8080/ACS/hello.txt", response_message);
	
	if(FAILURE == status)
	{
		ENB_PRINTF ("IN MAIN: Status Failure\n");
	}
	else if(SUCCESS == status)
	{
		ENB_PRINTF ("IN MAIN: It is a Success \n");
	}
	else if(EMPTY == status)
	{
	         ENB_PRINTF ("IN MAIN: Empty Message\n");
	}
}





void test_send_recv()
{
	char response_message[1024];
	int status;
	status = send_http_msg("Hellooooo", 1, "http://10.203.152.214:8080/ACS/hello.txt", response_message);
	
	if(0 == status)
	{
		ENB_PRINTF("response Received \n");
	}
	else
	{
		ENB_PRINTF("It is a Failure \n");
	}
}



void test_send_bad_URL()
{
	
	char response_message[1024] = {'\0'};
	int status;
	status = send_http_msg("", 1, "htt://10.203.152.214:8080/ACS/hello.txt", response_message);
	
	if(FAILURE == status)
	{
		ENB_PRINTF("IN MAIN: Status Failure\n");
	}
	else if(SUCCESS == status)
	{
		ENB_PRINTF("IN MAIN: It is a Success \n");
	}
	else if(EMPTY == status)
	{
		ENB_PRINTF("IN MAIN: Empty Message\n");
	}

}
