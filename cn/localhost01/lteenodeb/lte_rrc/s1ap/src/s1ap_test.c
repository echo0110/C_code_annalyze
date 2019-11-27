#ifdef LINUX_PC_TEST
#include "s1ap_global_ctx.h"
#include "s1ap_test.h"
#include "s1ap_logger.h"



S1AP_SCTP_SETTING_S g_s1ap_sctp_setting;

void test_s1ap_init(void)
{
    memset(&g_s1ap_sctp_setting,0,sizeof(S1AP_SCTP_SETTING_S));
}

U16 s1ap_set_sctp_node(U32 sockfd , U16 streams)
{
	U8 idx;
	test_log(0xff, 3, "s1ap_set_sctp_node:sockfd= %d,streams= %d",sockfd, streams);

	if(sockfd == 0)
	{
		test_log(0xff, 3, "s1ap_set_sctp_node:invalid sockfd!");
		return 0;//fail
	}

	if(streams > 10 || streams == 0)
	{
		test_log(0xff, 3, "s1ap_set_sctp_node:invalid streams number!");
		return 0;//fail
	}

	if(g_s1ap_sctp_setting.num == 10)
	{
		test_log(0xff, 3, "s1ap_set_sctp_node: stack overflow!");
		return 0; //fail
	}

	idx = g_s1ap_sctp_setting.num;

	g_s1ap_sctp_setting.item[idx].socket = sockfd;
	g_s1ap_sctp_setting.item[idx].streamNum = streams;

	g_s1ap_sctp_setting.num++;

	test_log(0xff, 3, "s1ap_set_sctp_node:setting is succ!");
	
	return 1;//succ
}


void s1ap_set_mme_stream_info(stream_info_t *pStreamInfo, U8 streamNum)
{
	U8 i = 0;

	pStreamInfo->number_of_streams = streamNum;
		
	for(i = 0; i< pStreamInfo->number_of_streams; i++)
	{
		pStreamInfo->stream_ids[i]= i+1;		
	}
	
	return;
}

S32 s1ap_set_sock_fd_for_test(mme_context_t *p_mme_ctx)
{
	S32 sockd = g_s1ap_sctp_setting.item[0].socket;
	U16 streamNum = g_s1ap_sctp_setting.item[0].streamNum;
	U8 i;

	if (g_s1ap_sctp_setting.num == 0 )
	{
		RRC_S1AP_TRACE(RRC_INFO,"s1ap_set_sock_fd_for_test: empty stack!");
		return -1;//fail
	}	

	p_mme_ctx->sctp_sd = sockd;
	s1ap_set_mme_stream_info(&p_mme_ctx->stream_info,streamNum);		

	g_s1ap_sctp_setting.num--;
	//remove used value;
	for(i=0;i<g_s1ap_sctp_setting.num;i++)
		g_s1ap_sctp_setting.item[i] = g_s1ap_sctp_setting.item[i+1];

	g_s1ap_sctp_setting.item[i].socket = 0;
	g_s1ap_sctp_setting.item[i].streamNum = 0;
	
	return sockd;
}

#endif

