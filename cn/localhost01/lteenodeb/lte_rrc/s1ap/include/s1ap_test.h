#ifndef _S1AP_TEST_H_
#define _S1AP_TEST_H_
#ifdef LINUX_PC_TEST
#include "rrc_defines.h"

typedef struct{
	U32 socket;
	U16 streamNum;
}s1ap_sctp_one_set_s;

typedef struct{
	U8 num;
	s1ap_sctp_one_set_s item[10];
}S1AP_SCTP_SETTING_S;


S32 s1ap_get_sock_fd_for_test(U8 mmeid);
void s1ap_set_mme_stream_info(stream_info_t *pStreamInfo, U8 mmeid);


#endif
#endif
