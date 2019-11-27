#ifndef __CLIENT_REQ_MGR__
#define __CLIENT_REQ_MGR__

#include        <cspl.h>
#include        <shell.h>
#include        <pthread.h>
#include    	<time.h>
#include        <common_def.h>


#define QUEUE	ysPushTail
#define DEQUEUE	ysPopHead

extern YSQUEUE eventQueue;

int ipr_cwmp_addClientRequest(ClientRequest *stCRequest);


#endif

