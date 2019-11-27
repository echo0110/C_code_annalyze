/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Packaging and the manifest structure    **/
/**                                                                        **/
/****************************************************************************/

#include	"cspl.h"

typedef struct {
	int	(*outgoing)(void *, void *, QMODULE, QMODULE, unsigned int);
	void	*calldata;
} QOUTGOING;

typedef struct {
	void	*data;
	QMANIFEST manifest;
} QPLUGIN;

/** Packaging ***************************************************************/
/*****************************************************************************
* FUNCTION: qvRegister
*
* DESCRIPTION: 
* 			This function registers modules with a thread by calling
* 			qvModuleAttach. Static initialization of the module is 
* 			performed by calling the init function of QMANIFEST.
* ARGS:	
* 			driver			Id of the thread with which to attach the module
* 			name			Name of the module as present in QMANIFEST str
* 			service			Module Id as present in the QMANIFEST Str
* 			manifest		Pointer to the QMANIFEST Str describing a module
* 			data			Pointer to initial data for the stack.
*
* RETURNS:	
*
******************************************************************************/
QMODULE	qvRegister( QDRIVER driver, const char *name, unsigned int service, 
		const QMANIFEST *manifest, void *data)
{
	return qvRegisterEx(driver, name, service, manifest, data, 0, 0);
}

/*****************************************************************************
* FUNCTION: qvRegisterEx
*
* DESCRIPTION: 
* 			This function registers modules with a thread by calling
* 			qvModuleAttach. Static initialization of the module is 
* 			performed by calling the init function of QMANIFEST.
			This function also allows modules to have module specific
			timer pools.
* ARGS:	
* 			driver			Id of the thread with which to attach the module
* 			name			Name of the module as present in QMANIFEST str
* 			service			Module Id as present in the QMANIFEST Str
* 			manifest		Pointer to the QMANIFEST Str describing a module
* 			data			Pointer to initial data for the stack.
			timer_tuning		Tuning for module specific timer pool.
						Positive value specifies no. of timer/buffer,
						which will not be extendible. QV_TPOOL_EXT means
						an empty timer/buffers pool which will be increased
						automatically based on need and QC_TPOOL_NONE will
						mean no timer pool required.
*			cErr			Pointer to output error code.
*
* RETURNS:
*
******************************************************************************/
QMODULE	qvRegisterEx( QDRIVER driver, const char *name, unsigned int service, 
		const QMANIFEST *manifest, void *data, int timer_tuning, int *cErr)
{
	/*Early Init*/
	void	*handle = manifest->init.early?manifest->init.early(data):data;
	QMODULE	module;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	driver = driver?driver:qvThisDriver();
	/*Attach module to the thread driver*/
	module = qvModuleAttach( 
			driver,
			name?name:manifest->name,			
			manifest->init.late, 
			manifest->handler.message, 
			manifest->handler.timer, 
			handle,
			service?service:manifest->service,
			timer_tuning,
			cErr );
	if ( !module ) {
		return 0;
	}

	qvSetMethods( module, (const void *)manifest );
	return module;
}

void	*qvMsgAllocFromModule( QMODULE module, unsigned int size )
{
	const QMANIFEST	*M = (const QMANIFEST *)qvGetMethods(module);
	
	if( M && M->method.alloc ) {
		return M->method.alloc( module, size );
	} else {
		return qvMsgAllocBySize( size );
	}
}
/*****************************************************************************
* FUNCTION: __signin
*
* DESCRIPTION: 
* 			This function unpacks the message buffer received from a module
* 			based on the unpack method defined.
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static void	*__signin( void *message, QMODULE module )
{
	const QMANIFEST	*M = (const QMANIFEST *)qvGetMethods(module);

	if( M && M->method.unpack ) {
		message = M->method.unpack( module, message );
	}

	return message;
}
/*****************************************************************************
* FUNCTION: __signout
*
* DESCRIPTION: 
* 			This function packs the message to be send to the module
* 			specified using the method defined.
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static void	*__signout( void *message, QMODULE module, U32 *size )
{
	const QMANIFEST	*M = (const QMANIFEST *)qvGetMethods(module);

	qvMsgSize( message, size );

	if( M && M->method.pack ) {
		message = M->method.pack( module, message, size );
	}

	return message;
}
/*****************************************************************************
* FUNCTION: __outgoing
*
* DESCRIPTION: 
* 			Dispatch function for each destination
* ARGS:	
* 			message			message to be sent to the named destination.
* 							Points to buffer allocated by qvMsgAlloc...APIs
* 			data			Pointe to QOUTGOING structure
* 			from 			sender
* 			to				receiver
*
* RETURNS:	
*
******************************************************************************/
static	int	__outgoing( void *message, void *data, QMODULE from, QMODULE to )
{
	QOUTGOING *M = (QOUTGOING *)data;
	unsigned int size;

	message = __signout(message, from, (U32 *)&size);
	if( M->outgoing( message, M->calldata, from, to, size ) ) {
		qvMsgFree(message);
	}
	return 0;
}
/*****************************************************************************
* FUNCTION: qvSetOutgoing
*
* DESCRIPTION: 
* 			Registers an outgoing handler for each destination. Called from
* 			qvSetDestination.
* ARGS:	
* 			service					service number for named destination
* 			outgoing				function pointer of the outgoing handler
* 			calldata				pointer to qsend structure.
*
* RETURNS:	
* 			Module identifier, if successful, else 0
*
******************************************************************************/
QMODULE	qvSetOutgoing( unsigned long service, 
		int (*outgoing)(void *, void *, QMODULE, QMODULE, unsigned int), void *calldata )
{
	QOUTGOING	*M = (QOUTGOING *)qvSysMalloc( sizeof(QOUTGOING) );

	M->outgoing = outgoing;
	M->calldata = calldata;

	return qvSetDispatch( service, __outgoing, (void *)M );
}
/*****************************************************************************
* FUNCTION: qvIncoming
*
* DESCRIPTION: 
*			Called when a message is received and the message needs to be
*			put on the queue of appropriate module.
* ARGS:	
* 			from				sender
* 			to					receiver
* 			priority			priority associated with the message
* 			message				pointer to the message to be delivered
*
* RETURNS:	
*
******************************************************************************/
void	qvIncoming( QMODULE from, QMODULE to, signed int priority, void *message )
{
	/* Fix for CSR 1-4696117 */
	void *msg=NULL;

	/* Unpacking the message */
	msg = __signin(message, to);

	/* if message returned by unpacking is not NULL then call qvDeliver */
	if(msg)	{
		/*call qvDeliver*/		
		qvDeliver( from, to, priority, msg);
	}
}

/** Interface Adaptation ****************************************************/
void	qvAdapt( QMODULE module, const QADAPTER *adapt, void *data)
{
	const QMANIFEST	*manifest = (const QMANIFEST *)qvGetMethods(module);
	QPLUGIN	*plugin = (QPLUGIN *)qvSysMalloc(sizeof(QPLUGIN));

	/* Code for Null check is added to remove errors appeared with klocwork SPR No: 2639 */
	if( !manifest)
	{
		return;
	}

	plugin->manifest.name = manifest->name;
	plugin->manifest.service = manifest->service;

	plugin->manifest.init.early = manifest->init.early;
	plugin->manifest.init.late = manifest->init.late;

	plugin->manifest.handler.message = manifest->handler.message;
	plugin->manifest.handler.timer = manifest->handler.timer;

	plugin->manifest.method.alloc = adapt->alloc?adapt->alloc:manifest->method.alloc;
	plugin->manifest.method.pack = adapt->pack?adapt->pack:manifest->method.pack;
	plugin->manifest.method.unpack = adapt->unpack?adapt->unpack:manifest->method.unpack;

	plugin->manifest.destinations = adapt->destinations?adapt->destinations:manifest->destinations;

	if( adapt->init ) {
		plugin->data = adapt->init(module, data);
	}

	qvSetMethods( module, (const void *)&(plugin->manifest) );
	qvSetFilter( module, adapt->in, adapt->out );

    /* SES-488 Coverity 48252 */
    qvSysFree((QPLUGIN *) plugin);
    /* SES-488 Coverity 48252 */

}

void	*qvAdaptData( QMODULE module )
{
	const QMANIFEST	*m = (const QMANIFEST *)qvGetMethods(module);

	return YMEMBEROF( QPLUGIN, manifest, m );
}


