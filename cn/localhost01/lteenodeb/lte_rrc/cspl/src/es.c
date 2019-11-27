/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Utilities for Standard Shells           **/
/**                                                                        **/
/****************************************************************************/

#include	"shell.h"


typedef struct DNODE {
	YLNODE	__header__;
	unsigned long service;
} DNODE;

static	DNODE *__find( YLIST *L, unsigned long service )
{
	DNODE	*p;

	for( p = (DNODE *)ylFirst(L); p; p = (DNODE *)ylNext((YLNODE *)p) ) {
		if ( p->service == service ) {
			return p;
		}
	}
	return 0;
}

static	unsigned long	*__resolve( const ESLIST *stacklist )
{
	const ESLIST	
		*P;
	YLIST	slist, dlist;
	DNODE	*d;
	unsigned long
		*U;
	unsigned int
		ndest = 0;

	ylInit( &slist );
	for( P = stacklist; P->stack; P++ ) {
		unsigned long service =  P->service?P->service:P->stack->service;

		if( service ) {
			if( __find(&slist, service) ) {
				qvPanic("Duplicate Service %lu\n", service );
			} else {
				d = (DNODE *)qvSysMalloc( sizeof(DNODE) );
				d->service = service;
				ylPushTail( &slist, (YLNODE *)d );
			}
		}
	}

	ylInit( &dlist );
	for( P = stacklist; P->stack; P++ ) {
		const unsigned long	*L = P->stack->destinations;

		if( L ) {
			for( ; *L; L++ ) {
				if( !__find(&slist, *L) && !__find(&dlist, *L) ) {
					d = (DNODE *)qvSysMalloc( sizeof(DNODE) );
					d->service = *L;
					ylPushTail( &dlist, (YLNODE *)d );
				}
			}
		}
	}

	U = (unsigned long *)qvSysMalloc( (ylCount(&dlist) + 1)*sizeof(unsigned long) );
	while( (d = (DNODE *)ylPopHead(&slist)) != 0 ) {
		qvSysFree( (void *)d );
	}

	while( (d = (DNODE *)ylPopHead(&dlist)) != 0 ) {
		U[ndest] = d->service; ndest++;
		qvSysFree( (void *)d );
	}

	U[ndest] = 0; ndest++;

	return U;
}
/*****************************************************************************
* FUNCTION: __setup
*
* DESCRIPTION: 
* 			This function opens the server and sets the destinations
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static void *__setup( ESPARAM *E, const unsigned long *destinations, const QSHELL *ipc_operations )
{
	int	C = E->argc;
	char	**V = E->argv;

	void	*listener = 0;
	int	i;
	const	unsigned long	*d;

	if( !C ) {
		qvPanic( "Nothing to listen on");
		return 0;
	}

	if( C == 1 ) {
		qvWarning( "No destinations?");
	}

	for( d = destinations; *d; d++ ) {
		for( i=1; i<C; i++ ) {
			char	*s, *t, *u = V[i];
			unsigned long	srvname;
			s = strchr( u, '=' );
			if( s ) {
				srvname = 0;
				t = u;

				while( t != s ) {
					srvname = (10*srvname) + (*t - '0');
					t++;
				}

				if( srvname == *d ) {
					break;
				}
			} else {
				break;
			}
		}

		if( i == C ) {
			qvWarning("Destination %lu not addressed", *d);
		}
	}

	for( i=1; i<C; i++ ) {
		char	*s, *t, *u = V[i];
		unsigned long	srvname;

		s = strchr( u, '=' );
		if( s ) {
			srvname = 0;
			t = u;

			while( t != s ) {
				srvname = (10*srvname) + (*t - '0');
				t++;
			}

			s++;
			for( d = destinations; *d; d++ ) {
				if( srvname == *d ) {
					break;
				}
			}
			if( *d == 0 ) {
				qvWarning("Addressed destination %lu not required", srvname);
			}
		} else {
		}
	}

	qvPrintf("Server: %s\n", V[0] );
	listener = qvOpen( ipc_operations, V[0] );

	if( !listener ) {
		qvPanic("Cannot start server");
		return 0;
	}

	for( i=1; i<C; i++ ) {
		char	*s, *t, *u = V[i];
		unsigned long	srvname;

		s = strchr( u, '=' );
		if( s ) {
			srvname = 0;
			t = u;

			while( t != s ) {
				srvname = (10*srvname) + (*t - '0');
				t++;
			}

			s++;
			qvPrintf("Destination %lu --> %s\n", srvname, s );
			qvSetDestination( srvname, ipc_operations, listener, s );
		} else {
			qvPrintf("Default Destination --> %s\n", u );
			qvSetDestination( 0, ipc_operations, listener, u );
		}
	}

	/* CSR  1-4926001 Memory leak fixed */
	qvSysFree( (void *)destinations );

	return listener;
}
/*****************************************************************************
* FUNCTION: __singlethread
*
* DESCRIPTION: 
* 			This function gives the execution flow for singlethreaded
* 			implementation
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static void __singlethread( ESPARAM *data, const ESLIST *stacklist,
			const QSHELL *ipc_operations, const QSYSOP *param_os, const QWAIT *wait )
{
	const ESLIST	
		*P;
	void	*listener = 0;

	qvSimpleInit( param_os );

	if( !stacklist ) {
		qvPanic("No Stacks!");
	   /* Code for Null check is added to remove errors appeared with klocwork SPR No: 2639 */
		return;

	}

	if( (listener = __setup(data, __resolve(stacklist), ipc_operations)) == 0 ) {
		qvPanic("No Listener!");
	}

	for( P = stacklist; P->stack; P++ ) {
		qvRegister( 0, 0, 0, (P->stack), (P->data) ); 
	}

	qvRun( ipc_operations, listener, wait, 0 );
}
/*****************************************************************************
* FUNCTION: __multithread
*
* DESCRIPTION: 
* 			This function gives the execution flow for multithreaded
* 			implementation
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static void __multithread( ESPARAM *data, const ESLIST *stacklist, 
			const QSHELL *ipc_operations, const QSYSOP *param_os, const QWAIT *wait,
			void (*createthread)(QDRIVER, const QWAIT *) )
{
	const ESLIST	
		*P;
	QDRIVER	*driverlist;
	void	*listener = 0;
	unsigned int
		numthread = 1,
		i;

	qvInit( 0, param_os );

	qvAssert( wait && createthread );

	if( !stacklist ) {
		qvPanic("No Stacks!");
		/* Code for Null check is added to remove errors appeared with klocwork SPR No: 2639 */
		return;
	}

	for( P = stacklist; P->stack; P++ ) {
		if( (P->thread + 1) > numthread ) {
			numthread = P->thread + 1;
		}
	}

	driverlist = (QDRIVER *)qvSysMalloc( numthread * sizeof(QDRIVER) );
	for( i=0; i<numthread; i++ ) {
		driverlist[i] = qvNewDriver();
	} 

	if( (listener = __setup(data, __resolve(stacklist), ipc_operations)) == 0 ) {
		qvPanic("No Listener!");
	}

	for( P = stacklist; P->stack; P++ ) {
		qvRegister( driverlist[P->thread], 0, 0, (P->stack), (P->data) ); 
	}

	for( i=0; i<numthread; i++ ) {
		createthread( driverlist[i], wait );
	} 

	qvInputLoop( ipc_operations, listener );

    /* SES-488 Coverity 48251 */
    qvSysFree( (QDRIVER *)driverlist); 
    /* SES-488 Coverity 48251 */
}

void	esMain( ESPARAM *data, const ESLIST *stacklist, const ESPORT *port,
			void (*createthread)(QDRIVER, const QWAIT *) )
{
	if( createthread ) {
		__multithread( data, stacklist, port->ipc, port->os, port->wait, createthread );
	} else {
		__singlethread( data, stacklist, port->ipc, port->os, port->wait );
	}
}
