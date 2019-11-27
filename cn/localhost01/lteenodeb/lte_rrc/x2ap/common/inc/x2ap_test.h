#ifndef _X2AP_TEST_H_
#define _X2AP_TEST_H_
#ifdef LINUX_PC_TEST
#include "rrc_defines.h"
#include "rrc_handlers.h"

#define MAX_TEST_X2AP_SOCKET_NUM 10

typedef struct{
	U32 socket;
	U16 streamNum;
}x2ap_sctp_one_set_s;

typedef struct{
	U8 num;
	x2ap_sctp_one_set_s item[MAX_TEST_X2AP_SOCKET_NUM];
}X2AP_SCTP_SETTING_S;


U16 x2ap_get_peer_enb_streamnum_for_test(S32 sockSd);
S32 x2ap_get_sock_fd_for_test(x2ap_peer_enb_context_t *p_enb_ctx);
void test_x2ap_global_context_reset(x2ap_gb_context_t* p_x2ap_gb_ctx);

#endif
#endif
