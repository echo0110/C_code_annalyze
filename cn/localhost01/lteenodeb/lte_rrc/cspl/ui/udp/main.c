/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Main Program -- Exerciser for CSPL Programs                     **/
/**                                                                        **/
/****************************************************************************/
#include	<ui.h>

typedef	struct {
	YLNODE __header__;

	unsigned long	
		name;
	void	*destname;
} DEST_T;

extern char *optarg;
extern int optind, opterr, optopt;
YLIST dlist;
void	*server = 0;

void	usend( int C, char **V, int nC, keyval_t *nV ) {
	unsigned char	buffer[1024];
	char	peername[256];
	unsigned int	n;
	void		*to;
	
	if( C < 3 ) {
		return;
	}
	n = hexbuf(V[2], buffer, sizeof(buffer) );
	to = ui_hash(V[1]);
	peername[0] = 0;
	n = ui_send( server, to, buffer, n, peername );
	errprintf("Sent %d bytes to %s\n", n, peername);
	dump( buffer, n, errprintf );
	ui_free( to );
}

void	apisend( int C, char **V, int nC, keyval_t *nV ) 
{
	unsigned int	n;
	unsigned char	buffer[1024], param[256], payload[1024],
			*scan = buffer;
	unsigned short	from, to, api, paramlen = 0, payloadlen = 0, c; 
	unsigned char	priority = 2, version = 1;
	char		*P;
	DEST_T		*D;
	U8		header[SH_HDRSIZE];
	char	peername[256];

	if( (P = keylookup("FROM", nC, nV)) == 0 ) {
		errprintf("Source Missing\n");
		return;
	} else {
		sscanf( P, "%hi", &from );
	}

	if( (P = keylookup("TO", nC, nV)) == 0 ) {
		errprintf("Destination Missing\n");
		return;
	} else {
		sscanf( P, "%hi", &to );
	}

	if( (P = keylookup("API", nC, nV)) == 0 ) {
		errprintf("API Missing\n");
		return;
	} else {
		sscanf( P, "%hi", &api );
	}

	if( (P = keylookup("VERSION", nC, nV)) != 0 ) {
		sscanf( P, "%hi", &c );
		version = (unsigned char) c;
	}

	if( (P = keylookup("PRIORITY", nC, nV)) != 0 ) {
		sscanf( P, "%hi", &c );
		priority = (unsigned char) c;
	}

	if( (P = keylookup("PARAM", nC, nV)) != 0 ) {
		paramlen = hexbuf(P, param, sizeof(param) );
	}

	if( (P = keylookup("PAYLOAD", nC, nV)) != 0 ) {
		payloadlen = hexbuf(P, payload, sizeof(payload) );
	}

	memset( header, 0, sizeof(header) );
	SH_SETATTRSIZE(header, paramlen + CV_HDRSIZE); 
	SH_SETPAYLOADSIZE(header, payloadlen); 
	SH_SETSOURCE(header, from); 
	SH_SETDESTINATION(header, to); 
	SH_SETPRIORITY(header, priority); 

	scan = yfAddBuf( scan, SH_HDRSIZE, header);

	scan = yfAddU8( scan, version );
	scan = yfAddU16( scan, from );
	scan = yfAddU16( scan, to );
	scan = yfAddU16( scan, api );
	scan = yfAddU16( scan, paramlen+payloadlen+16 );
	scan = yfAddU16( scan, paramlen+16 );
	scan = yfAddU16( scan, payloadlen );
	scan = yfAddU8( scan, priority );
	scan = yfAddU16( scan, 0 );
	scan = yfAddBuf( scan, paramlen, param );
	scan = yfAddBuf( scan, payloadlen, payload );

	n = scan - buffer;
	for( D = (DEST_T *)ylFirst(&dlist); D; D = (DEST_T *)ylNext((YLNODE *)D) ) {
		if( to == D->name ) {
			peername[0] = 0;
			n = ui_send( server, D->destname, buffer, n, peername );
			errprintf("Sent %d bytes to %s\n", n, peername);
			dump( buffer, n, errprintf );
			return;
		}
	}

	for( D = (DEST_T *)ylFirst(&dlist); D; D = (DEST_T *)ylNext((YLNODE *)D) ) {
		if( D->name == 0 ) {
			peername[0] = 0;
			n = ui_send( server, D->destname, buffer, n, peername );
			errprintf("Sent %d bytes to %s\n", n, peername);
			dump( buffer, n, errprintf );
			return;
		}
	}
	errprintf("Destination Unknown\n");
}

command_t	cmdlist[] = {
	{"usend", usend, "Raw send to destination"},
	{"apisend", apisend, "Build and send API"},
	{0, 0, 0}
};

void	*thread( void *data )
{
	unsigned char	buffer[MAXMSGSIZE];
	char	peername[256];

	printf( "Receiver Ready\n" );
	for( ; ; ) {
		int n;

		peername[0] = 0;
		n = ui_receive( data, buffer, MAXMSGSIZE, peername );

		if( n >= 0 ) {
			errprintf("Received %d bytes from %s\n", n, peername);
			dump( buffer, n, errprintf );
		}
	}
}

int	main(int C, char **V )
{
	int	c;
	char	*s;
	DEST_T	*D;
	pthread_t
		tid;

/*SPR 22554 Fix Start*/
    	char  threadName[20] = "cspl_udp_main";
/*SPR 22554 Fix End*/
	ylInit( &dlist );

	while ((c = getopt(C, V, "d:")) != EOF) {
		unsigned long srvname;
	
		switch( c ) {
			case 'd':
				if( (s = strchr(optarg, '=')) != 0 ) {
					*s = 0;
					s++;
					srvname = atoi( optarg );
					D = (DEST_T *)malloc(sizeof(DEST_T));
					D->name = srvname;
					D->destname = ui_hash(s);
					ylPushTail( &dlist, (YLNODE *)D );
				} else {
					D = (DEST_T *)malloc(sizeof(DEST_T));
					D->name = 0;
					D->destname = ui_hash(optarg);
					ylPushTail( &dlist, (YLNODE *)D );
				}
				break;
			default:
				errprintf("Unknown Option\n");
				return 0;
		}
	}

	server = ui_makeserver(V[optind]);


	if( pthread_create( &tid, 0, thread, server ) != 0 ) {
		errorabort( "pthread_create" );
	}
/*SPR 22554 Fix Start*/

    	/* Set name of the csp udp main thread */
    	if (0 != pthread_setname_np(tid, threadName))
    	{
        	errprintf("Failed to set name of the cspl udp main thread ");
    	}

/*SPR 22554 Fix End*/
	for( ; ; ) {
		char	*buffer;

		buffer = readline("| ");
		if( !buffer ) {
			exit(0);
		}

		if( buffer[0] ) {
			add_history( buffer );
			commandprocess( buffer, cmdlist );
		}
	
		free( buffer );
	}
}
