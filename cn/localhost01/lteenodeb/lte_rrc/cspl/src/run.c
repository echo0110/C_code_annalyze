/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Encapsulating Shell Skeleton            **/
/**                                                                        **/
/****************************************************************************/

#include	"cspl.h"

typedef	struct	{
	const QSHELL	*shell;
	void	*listener;
	void	*data;
} qsend;

/** Skeleton Encapsulating Shell ********************************************/
/*****************************************************************************
* FUNCTION: __transmit
*
* DESCRIPTION: 
* 			Outgoing Handler for each destination.
* ARGS:		
* 			message				message to be sent to the named destination.
* 								Points to buffer allocated by qvMsgAlloc...APIs
* 			data				Pointer to the qsend structure.
* 			from				sender
* 			to					receiver
* 			size				size of the message
*
* RETURNS:	
*
******************************************************************************/
static int	__transmit( void *message, void *data, 
		QMODULE from, QMODULE to, unsigned int size )
{
	qsend	*S = (qsend *)data;

	S->shell->send( S->listener, message, from, to, S->data, size );

	return 1;
}
/*****************************************************************************
* FUNCTION: qvSetDestination
*
* DESCRIPTION: 
* 			Called per external destination. Registers an outgoing handler
* 			which is per destination and is used to send messages. 
* ARGS:	
* 			service				service number for named destination
* 			shell				pointer to QSHELL structure which ports
* 								communication services.
* 			listener			Pointer to a listener data structure 
* 								that shall be used in the delivery of 
* 								an outgoing message to a named destination
* 			data				Pointer to any data that will be passed 
* 								as a parameter to the outgoing handler 
* 								function
*
* RETURNS:		
* 			Module identifier, if successful, else 0
*
******************************************************************************/
QMODULE	qvSetDestination( unsigned long service, const QSHELL *shell, void *listener, void *data )
{
	qsend	*s = (qsend *)qvSysMalloc( sizeof(qsend) );

	if( !s ) {
		qvPanic("No Memory");
		return 0;
	}

	s->shell = shell;
	s->listener = listener;
	s->data = shell->hash?shell->hash(listener, data):data;
	return qvSetOutgoing( service, __transmit, s );
}
/*****************************************************************************
* FUNCTION: qvRun
*
* DESCRIPTION: 
* 			This function starts the event-processing loop for a single 
* 			thread.
* 			This function calls qvSchedule function; which processes
* 			all the pending initializations, messages and timers till 
* 			there are none left - then returns to the caller the next
* 			desired wakeup time. Timedwait fn is called, which returns
* 			after a specific interval or when a message is received.
* ARGS:	
* 			S				Pointer to the QSHELL structure
* 			listener		Pointer to Listener data structure
* 			W				Pointer to QWAIT structure
* 			policy			Mode of how to process messages on a listener
*
* RETURNS:	
*
******************************************************************************/
void	qvRun( const QSHELL *S, void *listener, const QWAIT *W, int policy )
{
	QTIME	now, diff;

	qvAssert( S && S->timedwait );

    /* SPR 15822 Fix Start */
    /* Code Removed */
    /* SPR 15822 Fix End */

	for( ; ; ) {
        /* To Remove the Shadows Declaration Warnings */            
        /* Start SPR 5921 Changes. */            
		const QTIME	*wakeup1;

        /* SPR 15822 Fix Start */
        W->walltime( &now );
        qvLogTime( &now );
        /* SPR 15822 Fix Start */

		wakeup1 = qvSchedule( &now ); 

		if( wakeup1 ) {
			if( qvTimeDiff(wakeup1, &now, &diff) < 0 ) {
        /* End SPR 5921 Changes. */                    
				diff.s = diff.us = 0;
			}
			S->timedwait( listener, &diff );/*Wait for specified interval 
											 on the listener*/
		} else {
			S->timedwait( listener, 0 ); /*Wait indefinitely*/
		}
        /* SPR 15822 Fix Start */
        /* Code Removed */
        /* SPR 15822 Fix End */

		for( ; ; ) {
			QMODULE		from, to;
			void		*buffer;
			signed char	priority;

			from = to = 0; priority = 0;
			if( (buffer = S->receive(listener, &from, &to, &priority)) == 0 ) {
				break;/*Nothing is received, so break from the for loop*/
			}
			/*Deliver the message*/
			qvIncoming( from, to, priority, buffer );

			if( !policy ) { /*Call qvSchedule after processing each message
							  else retrieve all messages received*/
				break;
			}
		}
	}
}
/*****************************************************************************
* FUNCTION: qvInputLoop
*
* DESCRIPTION: 
* 			This function starts the event-processing loop for main 
* 			thread in multi thread execution. 
* ARGS:	
* 			S			Pointer to QSHELL structure
* 			listener	Pointer to the listener data structure.
*
* RETURNS:	
*
******************************************************************************/
void	qvInputLoop( const QSHELL *S, void *listener )
{
	QMODULE		from, to;
	void		*buffer;
	signed char	priority;

	if( S->timedwait ) { 
		for( ; ; ) {
			S->timedwait( listener, 0 );/*Call QSHELL timedwait function*/

			from = to = 0; priority = 0;
			/*Deliver the msg to destination module after receiving it*/
			if( (buffer = S->receive(listener, &from, &to, &priority)) != 0 ) {
				qvIncoming( from, to, priority, buffer );
			}
		}
	} else {
		for( ; ; ) {
			from = to = 0; priority = 0;
			if( (buffer = S->receive(listener, &from, &to, &priority)) != 0 ) {
				qvIncoming( from, to, priority, buffer );
			}
		}
	}
}
/*****************************************************************************
* FUNCTION: qvDriverLoop
*
* DESCRIPTION: 
*			This function starts the event-processing loop for message 
*			and timer processing thread in multi thread execution.  			
* ARGS:	
*			driver			Driver Id of the thread
*			W				Pointer to the QWAIT structure
* RETURNS:	
*
******************************************************************************/
void	qvDriverLoop( QDRIVER driver, const QWAIT *W )
{
	/*Get a new mutex*/
	void	*channel = W->newchannel();

	/*Attach the wakeup function for this driver*/
	qvDriverStart( driver, W->wakeup, channel );

	for( ; ; ) {
		const QTIME	
        /* To Remove the Shadows Declaration Warnings */            
        /* Start SPR 5921 Changes. */                
			*wakeup1;
		QTIME	now, diff;

		W->walltime( &now );
		qvLogTime( &now );

		wakeup1 = qvSchedule( &now ); 

		if( wakeup1 ) {
			if( qvTimeDiff(wakeup1, &now, &diff) < 0 ) {
        /* End SPR 5921 Changes. */                    
				diff.s = diff.us = 0;
			}
			W->sleep( channel, &diff );/*Wait for specified interval*/
		} else {
			W->sleep( channel, 0 );/*Wait indefinitely*/
		}

	}
}

