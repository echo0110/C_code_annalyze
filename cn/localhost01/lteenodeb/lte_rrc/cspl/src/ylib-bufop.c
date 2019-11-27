/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include "ylib.h"

unsigned short	yzSwapU16( unsigned short N )
{
	return	(0x00FF & (N >> 8)) |
		(0xFF00 & (N << 8));
}

unsigned long	yzSwapU32( unsigned long N )
{
	return	(0x000000FF & (N >> 24)) |
		(0x0000FF00 & (N >> 8)) |
		(0x00FF0000 & (N << 8)) |
		(0xFF000000 & (N << 24));
}

/** Octet Buffer Routines ***************************************************/
unsigned char	*yfAddBuf( unsigned char *buf, unsigned int count, unsigned char *newbuffer )
{
	unsigned char	*P = buf;
	unsigned int	i;

	if( count > sizeof(unsigned long) ) {
		memcpy( P, newbuffer, count );
		P += count;
	} else {
		for( i=0; i<count; i++ ) {
			*P++ = newbuffer[i];
		}
	}

	return P;
}

unsigned char	*yfGetBuf( unsigned char *buf, unsigned int count, unsigned char *newbuffer )
{
	unsigned char	*P = buf;
	unsigned int	i;

	if (newbuffer == NULL)
		return (P+count);

	if( count > sizeof(unsigned long) ) {
		memcpy( newbuffer, P, count );
		P += count;
	} else {
		for( i=0; i<count; i++ ) {
			newbuffer[i] = *P++;
		}
	}

	return P;
}

unsigned char	*yfAddU8( unsigned char *buf, unsigned char value )
{
	unsigned char	cvalue = value;
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	P = yfAddBuf( P, sizeof(cvalue), Q );
	return P;
}
		
unsigned char	*yfGetU8( unsigned char *buf, unsigned char *value )
{
	/* Start SPR 4990 Changes. */ 
	unsigned char	cvalue = 0;
	/* End SPR 4990 Changes. */ 
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	if (value == NULL)
		return (P + sizeof(cvalue));

	P = yfGetBuf( P, sizeof(cvalue), Q );
	*value = cvalue;
	return P;
}

unsigned char	yfGetU8At( unsigned char *buf, int offset )
{
	unsigned char	cvalue;

	yfGetU8( buf+offset, &cvalue );
	return cvalue;
}

void	yfSetU8At( unsigned char *buf, int offset, unsigned char value )
{
	yfAddU8( buf+offset, value );
}

unsigned char	*yfAddU16( unsigned char *buf, unsigned short value )
{
	unsigned short	cvalue = htons(value);
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	P = yfAddBuf( P, sizeof(cvalue), Q );
	return P;
}

unsigned char	*yfGetU16( unsigned char *buf, unsigned short *value )
{
	/* Start SPR 4990 Changes. */ 
	unsigned short	cvalue = 0;
	/* End SPR 4990 Changes. */ 
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	if (value == NULL)
		return (P + sizeof(cvalue));

	P = yfGetBuf( P, sizeof(cvalue), Q );
	*value = ntohs(cvalue);
	return P;
}

unsigned short	yfGetU16At( unsigned char *buf, int offset )
{
	unsigned short	cvalue;

	yfGetU16( buf+offset, &cvalue );
	return cvalue;
}

void	yfSetU16At( unsigned char *buf, int offset, unsigned short value )
{
	yfAddU16( buf+offset, value );
}

unsigned char	*yfAddU32( unsigned char *buf, unsigned long value )
{
	unsigned long	cvalue = htonl(value);
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	P = yfAddBuf( P, sizeof(cvalue), Q );
	return P;
}

unsigned char	*yfGetU32( unsigned char *buf, unsigned long *value )
{
	/* Start SPR 4990 Changes. */ 
	unsigned long	cvalue = 0;
	/* End SPR 4990 Changes. */ 
	unsigned char	*Q = (unsigned char *)(&cvalue);
	unsigned char	*P = buf;

	if (value == NULL)
		return (P + sizeof(cvalue));

	P = yfGetBuf( P, sizeof(cvalue), Q );
	*value = ntohl(cvalue);
	return P;
}

unsigned long	yfGetU32At( unsigned char *buf, int offset )
{
	unsigned long	cvalue;

	yfGetU32( buf+offset, &cvalue );
	return cvalue;
}

void	yfSetU32At( unsigned char *buf, int offset, unsigned long value )
{
	yfAddU32( buf+offset, value );
}

unsigned char	*yfFill( unsigned char *buf, unsigned int count, unsigned char c )
{
	unsigned char	*P = buf;

	memset( P, c, count );

	P += count;
	return P;
}

unsigned char	*yfSkip( unsigned char *buf, unsigned int count )
{
	unsigned char	*P = buf;

	P += count;
	return P;
}

unsigned char	*yfPad( unsigned char *scan, unsigned char *start, unsigned int to, unsigned char c )
{
	unsigned char	*P = scan;
	unsigned int	n = (P - start) % to;

	if( n ) {
		memset( P, c, n );
		P += n;
	}
	return P;
}

