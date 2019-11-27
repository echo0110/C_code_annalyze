/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Buffer and Bitstring Handling                                          **/
/**                                                                        **/
/****************************************************************************/
#include "ylib.h"

static	const unsigned char	bitmask_r[] = {
	0x00,
	0x01,
	0x03,
	0x07,
	0x0F,
	0x1F,
	0x3F,
	0x7F,
	0xFF,
};

static	const unsigned char	bitmask_l[] = {
	0x00,
	0x80,
	0xC0,
	0xE0,
	0xF0,
	0xF8,
	0xFC,
	0xFE,
	0xFF,
};

/** Bitstring Routines ******************************************************/
void	ybSetup( YBITPTR *B, unsigned char *message, unsigned char endian )
{
	B->byte = message;
	B->bit = 0;
	B->endian = endian;
}

int	ybPtrCompare( YBITPTR *l, YBITPTR *r )
{
	if( l->byte == r->byte ) {
		if( l->bit == r->bit ) {
			return	0;
		} else if ( l->bit > r->bit ) {
			return	1;
		} else {
			return	-1;
		}
	} else if ( l->byte > r->byte ) {
		return	1;
	} else {
		return	-1;
	}
}

unsigned int	ybEnough( const YBITPTR *here, const YBITPTR *end, int nbits )
{
	YBITPTR	B = *here;

	B.bit += nbits;
	B.byte += B.bit/8;
	B.bit %= 8;

	if( end->byte < B.byte ) {
		return 0;
	}

	if( (end->byte == B.byte) && (end->bit < B.bit) ) {
		return 0;
	}

	return 1;
}

void	ybSkip( YBITPTR *here, unsigned int nbits )
{
	while( nbits > 0 ) {
		unsigned int	n;

		n = 8 - here->bit;
		if( n > nbits ) {
			n = nbits;
			here->bit += nbits;
		} else {
			here->byte++;
			here->bit = 0;
		}
		nbits -= n;
	}
}

void	ybGet( YBITPTR *here, unsigned long *to, unsigned int nbits )
{
	unsigned long	value = 0;
	unsigned int	counter = 0;

	while( nbits > 0 ) {
		unsigned int	n;
		unsigned long	v = 0;
		unsigned char	*p;
		unsigned int	b;

		p = here->byte;
		b = here->bit;

		n = 8 - b;
		if( n > nbits ) {
			n = nbits;
			here->bit += nbits;
		} else {
			here->byte++;
			here->bit = 0;
		}
		nbits -= n;

		v = *p;

		switch( here->endian ) {
			case __LE:
				v >>= b;
				v &= bitmask_r[n];

				value |= (v << counter);
				break;
			case __BE:
				v <<= b;
				v &= bitmask_l[n];

				value = (value << n) | (v >> (8 - n));
				break;
			default:
				return;
		}
		counter += n;
	}

	if( to ) {
		*to = value;
	}
	return;
}

void	ybPeek( YBITPTR *here,  unsigned long *to, unsigned int nbits )
{
	YBITPTR	B = *here;

	ybGet( &B, to, nbits );
}

void	ybAdd( YBITPTR *here, unsigned long from, unsigned int nbits )
{
	unsigned int	counter = nbits;

	while( nbits > 0 ) {
		unsigned int	n;
		unsigned long	v = 0;
		unsigned char	*p;
		unsigned int	b;

		p = here->byte;
		b = here->bit;

		n = 8 - b;
		if( n > nbits ) {
			n = nbits;
			here->bit += nbits;
		} else {
			here->byte++;
			here->bit = 0;
		}
		nbits -= n;

		switch( here->endian ) {
			case __LE:
				v = from & bitmask_r[n];
				from >>= n;

				v <<= b;
				*p &= ~(bitmask_r[n] << b);
				*p |= v;
				break;
			case __BE:
				v = (from >> (counter - n) ) & bitmask_r[n];
				counter -= n;

				v <<= (8 - n - b);
				*p &= ~(bitmask_l[n] >> b);
				*p |= v;
				break;
			default:
				return;
		}

	}
	return;
}

void	ybGetBuf( YBITPTR *here, unsigned char *to, unsigned int nbytes )
{
	if( here->bit == 0 ) {
		if( to ) {
			memcpy( to, here->byte, nbytes );
		}
		here->byte += nbytes;
	}  else {
		unsigned int	i;

		for( i=0; i<nbytes; i++ ) {
        /* For uninitialazation variable c */                
        /* Start SPR 5921 Changes. */                
			unsigned long	c=0;
        /* End SPR 5921 Changes. */            
			ybGet( here, &c, 8 );
			if( to ) {
				to[i] = (unsigned char)c;
			}
		}
	}
	return;
}

void	ybAddBuf( YBITPTR *here, unsigned char *from, unsigned int nbytes )
{
	if( here->bit == 0 ) {
		memcpy( here->byte, from, nbytes );
		here->byte += nbytes;
	}  else {
		unsigned int	i;

		for( i=0; i<nbytes; i++ ) {
			unsigned long	c = from[i];
			ybAdd( here, c, 8 );
		}
	}
	return;
}

void	ybGetU8( YBITPTR *here,  unsigned char *to )
{
	if( here->bit == 0 ) {
		if( to ) {
			*to = *(here->byte);
		}
		here->byte += sizeof(*to);
	}  else {
        /* For uninitialazation variable c */                
        /* Start SPR 5921 Changes. */            
		unsigned long	c=0;
        /* End SPR 5921 Changes. */
		ybGet( here, &c, 8 );
		if( to ) {
			*to = (unsigned char) c;
		}
	}
	return;
}

void	ybAddU8( YBITPTR *here,  unsigned char from )
{

	if( here->bit == 0 ) {
		*(here->byte) = from;
		here->byte += sizeof(from);
	}  else {
		unsigned long	c = from;
		ybAdd( here, c, 8 );
	}
	return;
}

void	ybGetU16( YBITPTR *here,  unsigned short *to )
{
    /* Start SPR 5921 Changes. */        
	unsigned long	c=0;
    /* End SPR 5921 Changes. */    
	ybGet( here, &c, 16 );
	if( to ) {
		*to = (unsigned short) c;
	}
	return;
}

void	ybAddU16( YBITPTR *here,  unsigned short from )
{
	unsigned long	c = from;
	ybAdd( here, c, 16 );
	return;
}

void	ybGetU32( YBITPTR *here,  unsigned long *to )
{
    /* Start SPR 5921 Changes. */        
	unsigned long	c=0;
    /* End SPR 5921 Changes. */
	ybGet( here, &c, 32 );
	if( to ) {
		*to = (unsigned long) c;
	}
	return;
}

void	ybAddU32( YBITPTR *here,  unsigned long from )
{
	unsigned long	c = from;
	ybAdd( here, c, 32 );
	return;
}

