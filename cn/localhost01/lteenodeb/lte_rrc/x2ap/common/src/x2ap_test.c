
#ifdef LINUX_PC_TEST
#include "x2ap_db.h"
#include "x2ap_test.h"


X2AP_SCTP_SETTING_S g_x2ap_sctp_setting;

extern short test_log(U16 usModId,U16 logLevel,char*fmt,...);

void test_x2ap_init(void)
{
    memset(&g_x2ap_sctp_setting,0,sizeof(X2AP_SCTP_SETTING_S));
}

U16 x2ap_set_sctp_node(S32 sockfd , U16 streams)
{
    U8 idx;
    test_log(0xff, 3, "x2ap_set_sctp_node:sockfd= %d, streams= %d",sockfd, streams);

    if(sockfd <= -1)
    {
        test_log(0xff, 3, "x2ap_set_sctp_node:invalid sockfd!");
        return 0;//fail
    }

    if(streams > 10 || streams < 2)
    {   
        test_log(0xff, 3, "x2ap_set_sctp_node:invalid streams number!stream num range[2~10]");
        return 0;//fail
    }

    if(g_x2ap_sctp_setting.num >= MAX_TEST_X2AP_SOCKET_NUM)
    {
    	test_log(0xff, 3, "x2ap_set_sctp_node: stack overflow!");
    	return 0; //fail
    }

    idx = g_x2ap_sctp_setting.num;

    g_x2ap_sctp_setting.item[idx].socket = sockfd;
    g_x2ap_sctp_setting.item[idx].streamNum = streams;
    g_x2ap_sctp_setting.num++;

    test_log(0xff, 3, "x2ap_set_sctp_node:setting is succ!setting_num=%d", g_x2ap_sctp_setting.num);

    return 1;//succ
}

U16 x2ap_get_peer_enb_streamnum_for_test(S32 sockSd)
{
	U16 streamNum = 0;
	U8 i,j;

	for(i = 0; i< g_x2ap_sctp_setting.num; i++)
	{
            if (g_x2ap_sctp_setting.item[i].socket == sockSd)
            {
                streamNum = g_x2ap_sctp_setting.item[i].streamNum;
                break;
            }
	}

	if (i < g_x2ap_sctp_setting.num)
	{
	    g_x2ap_sctp_setting.num--;
	    
	    for(j=i; j<g_x2ap_sctp_setting.num; j++)
            	g_x2ap_sctp_setting.item[j] = g_x2ap_sctp_setting.item[j+1];

           g_x2ap_sctp_setting.item[j].socket = 0;
           g_x2ap_sctp_setting.item[j].streamNum = 0;
	}
	
	return streamNum;
}

S32 x2ap_get_sock_fd_for_test(x2ap_peer_enb_context_t *p_enb_ctx)
{
	S32 sockd = -1;
	U16 streamNum;
	U8 i;

	if (g_x2ap_sctp_setting.num == 0 )
	{
		test_log(0xff, 3,"x2ap_set_sock_fd_for_test: empty stack!");
		return sockd;//fail
	}	
	
	sockd = g_x2ap_sctp_setting.item[0].socket;
	streamNum = g_x2ap_sctp_setting.item[0].streamNum;
	
	// set stream info
	if (NULL != p_enb_ctx)
	{
	    p_enb_ctx->stream_info.number_of_streams = streamNum;
        		
            for(i = 0; i< p_enb_ctx->stream_info.number_of_streams && i < MAX_NUM_X2AP_SCTP_STREAM; i++)
            {
                p_enb_ctx->stream_info.stream_ids[i]= i;		
            }
	}

        g_x2ap_sctp_setting.num--;
        //remove used value;
        for(i=0;i<g_x2ap_sctp_setting.num;i++)
        	g_x2ap_sctp_setting.item[i] = g_x2ap_sctp_setting.item[i+1];

        g_x2ap_sctp_setting.item[i].socket = 0;
        g_x2ap_sctp_setting.item[i].streamNum = 0;
	
	return sockd;
}

void test_x2ap_global_context_reset(x2ap_gb_context_t* p_x2ap_gb_ctx)
{
    comm_info_init_param_t * comm_info_init_param = (comm_info_init_param_t *)p_x2ap_gb_ctx->p_comm_info_int;
    memset((void*)p_x2ap_gb_ctx, 0, sizeof(x2ap_gb_context_t));
    p_x2ap_gb_ctx->p_comm_info_int = comm_info_init_param;
    p_x2ap_gb_ctx->server_fd_ipv4 = -1;
    p_x2ap_gb_ctx->server_fd_ipv6 = -1;
}

#endif

