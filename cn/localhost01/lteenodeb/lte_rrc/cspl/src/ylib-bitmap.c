/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include "ylib.h"

/** Bitmap Routines *********************************************************/
static	const unsigned long	mask[] = {
	0x80000000UL, 0x40000000UL, 0x20000000UL, 0x10000000UL,
	0x08000000UL, 0x04000000UL, 0x02000000UL, 0x01000000UL,
	0x00800000UL, 0x00400000UL, 0x00200000UL, 0x00100000UL,
	0x00080000UL, 0x00040000UL, 0x00020000UL, 0x00010000UL,
	0x00008000UL, 0x00004000UL, 0x00002000UL, 0x00001000UL,
	0x00000800UL, 0x00000400UL, 0x00000200UL, 0x00000100UL,
	0x00000080UL, 0x00000040UL, 0x00000020UL, 0x00000010UL,
	0x00000008UL, 0x00000004UL, 0x00000002UL, 0x00000001UL,
};

static	const unsigned long	rmask[] = {
	~0x80000000UL, ~0x40000000UL, ~0x20000000UL, ~0x10000000UL,
	~0x08000000UL, ~0x04000000UL, ~0x02000000UL, ~0x01000000UL,
	~0x00800000UL, ~0x00400000UL, ~0x00200000UL, ~0x00100000UL,
	~0x00080000UL, ~0x00040000UL, ~0x00020000UL, ~0x00010000UL,
	~0x00008000UL, ~0x00004000UL, ~0x00002000UL, ~0x00001000UL,
	~0x00000800UL, ~0x00000400UL, ~0x00000200UL, ~0x00000100UL,
	~0x00000080UL, ~0x00000040UL, ~0x00000020UL, ~0x00000010UL,
	~0x00000008UL, ~0x00000004UL, ~0x00000002UL, ~0x00000001UL,
};

static	const unsigned long	ignore[] = {
	0x80000000UL, 0xC0000000UL, 0xE0000000UL, 0xF0000000UL,
	0xF8000000UL, 0xFC000000UL, 0xFE000000UL, 0xFF000000UL,
	0xFF800000UL, 0xFFC00000UL, 0xFFE00000UL, 0xFFF00000UL,
	0xFFF80000UL, 0xFFFC0000UL, 0xFFFE0000UL, 0xFFFF0000UL,
	0xFFFF8000UL, 0xFFFFC000UL, 0xFFFFE000UL, 0xFFFFF000UL,
	0xFFFFF800UL, 0xFFFFFC00UL, 0xFFFFFE00UL, 0xFFFFFF00UL,
	0xFFFFFF80UL, 0xFFFFFFC0UL, 0xFFFFFFE0UL, 0xFFFFFFF0UL,
	0xFFFFFFF8UL, 0xFFFFFFFCUL, 0xFFFFFFFEUL, 0xFFFFFFFFUL,
};

static	const unsigned long	rignore[] = {
	~0x80000000UL, ~0xC0000000UL, ~0xE0000000UL, ~0xF0000000UL,
	~0xF8000000UL, ~0xFC000000UL, ~0xFE000000UL, ~0xFF000000UL,
	~0xFF800000UL, ~0xFFC00000UL, ~0xFFE00000UL, ~0xFFF00000UL,
	~0xFFF80000UL, ~0xFFFC0000UL, ~0xFFFE0000UL, ~0xFFFF0000UL,
	~0xFFFF8000UL, ~0xFFFFC000UL, ~0xFFFFE000UL, ~0xFFFFF000UL,
	~0xFFFFF800UL, ~0xFFFFFC00UL, ~0xFFFFFE00UL, ~0xFFFFFF00UL,
	~0xFFFFFF80UL, ~0xFFFFFFC0UL, ~0xFFFFFFE0UL, ~0xFFFFFFF0UL,
	~0xFFFFFFF8UL, ~0xFFFFFFFCUL, ~0xFFFFFFFEUL, ~0xFFFFFFFFUL,
};

/* Table specifying MSB position of any value */
static	const unsigned char	fstbl[256] =	{
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
};
/*Header For the Bit Map*/
#define	NBITS	0
#define	NTYPE	1
#define	NLEVS	2
#define	NSOFF	3
#define	NCOFF	(NSOFF + 6)
#define	NBASE	(NCOFF + 6)

/*****************************************************************************
* FUNCTION: yzFfs
*
* DESCRIPTION: Returns the MSBit position of any value
*
* RETURNS:
*
******************************************************************************/
int	yzFfs( unsigned long N )
{
	unsigned short	
		msw = (unsigned short) (N >> 16), /* N is of length two words(4 bytes) store the MSW */
		lsw = (unsigned short) (N & 0xffff); /*store less significant word now */
	unsigned char	byte;

	if (N == 0) {
		return (0);
	}

	if (msw) {
		byte = (unsigned char) (msw >> 8); /* most significant byte of number */
		if (byte) {
			return (fstbl[byte] + 24 + 1);
		} else {
			return (fstbl[(unsigned char) msw] + 16 + 1);
		}
	} else {
		byte = (unsigned char) (lsw >> 8);
		if (byte) {
			return (fstbl[byte] + 8 + 1);
		} else {
			/*For Values from 0 to FF */
			/* Start SPR 4990 Changes. */ 
			return (fstbl[(unsigned char) (lsw & 0xff)] + 1);
			/* End SPR 4990 Changes. */ 
		}
	}
}
/*****************************************************************************
* FUNCTION: ymInit
*
* DESCRIPTION: Initialises the bit map header. 
* 				Zeroth Position 	- 	Num of Bits
* 				First				-	Type
* 				Second				-	Num of Levels
* 				Third to Eighth		-	Set Offset
* 				Ninth to Fourteenth	- 	Clear Offset
*
* ARGS:	
* 				M				Pointer to Bitmap
* 				nbits			Bits in the Bitmap
* 				type			Continuous Loop or Heirarchy table TBD
*
* RETURNS:
*
******************************************************************************/void		ymInit(YBITMAP *M, unsigned int nbits, int type)
{
	unsigned long	i, m, n;

	/* Start SPR 4990 Changes. */ 
	size_t  mem_set_size = YMAPSIZE(nbits)*sizeof(unsigned long);
	
	memset( M, 0, mem_set_size );
	/* End SPR 4990 Changes. */ 
		
	M[NBITS] = nbits; /*zeroth(NBfile:///usr/share/doc/HTML/index.htmlITS) position holds 
						the number of bits stored*/
	M[NTYPE] = type;/*first(NTYPE) position holds the type*/

	if( M[NTYPE] ) {
		n = yzFfs( nbits - 1 );
		n = (n + 4)/5;
		M[NLEVS] = n;/*3*/

		M[NSOFF] = M[NCOFF] = NBASE;

		m = nbits; /*2048*/

		for( i=0; i<5; i++ ) {
			m = __EY(m);/*64*/
			M[NSOFF + (i+1)] = M[NCOFF + i] + m;
			M[NCOFF + (i+1)] = M[NSOFF + (i+1)] + __EY(m);
		}

	} else {
		M[NSOFF] = M[NCOFF] = NBASE;
	}
}
/*****************************************************************************
* FUNCTION:  ymSet
*
* DESCRIPTION: 
* 			This function sets the appropriate bit in the bitmap
*
* RETURNS:
*
******************************************************************************/
void		ymSet(YBITMAP *M, unsigned int n)
{
	unsigned int	i, m;
	unsigned long	L;

	n--;

	if( M[NTYPE] ) {
		for( m=n, i=0; i<M[NLEVS]; i++ ) {
			*(M + M[NSOFF + i] + m/32) |= mask[m%32];
			L = *(M + M[NCOFF + i] + m/32) |= mask[m%32];
			if ( L != ~0UL ) {
				break;
			}
			m /= 32;
		}
		for( ; i<M[NLEVS]; i++ ) {
			*(M + M[NSOFF + i] + m/32) |= mask[m%32];
			m /= 32;
		}
	} else {
		*(M + NBASE + n/32) |= mask[n%32];
	}
}
/*****************************************************************************
* FUNCTION:  ymClear
*
* DESCRIPTION: 
* 			This function clears the appropriate bit in the bitmap
*
* RETURNS:
*
******************************************************************************/
void		ymClear(YBITMAP *M, unsigned int n)
{
	unsigned int	i, m;
	unsigned long	L;

	n--;
	if( M[NTYPE] ) {
		for( m=n, i=0; i<M[NLEVS]; i++ ) {
			*(M + M[NCOFF + i] + m/32) &= rmask[m%32];
			L = *(M + M[NSOFF + i] + m/32) &= rmask[m%32];
			if ( L ) {
				break;
			}
			m /= 32;
		}
		for( ; i<M[NLEVS]; i++ ) {
			*(M + M[NCOFF + i] + m/32) &= rmask[m%32];
			m /= 32;
		}
	} else {
		*(M + NBASE + n/32) &= ~mask[n%32];
	}
}

int		ymBit(YBITMAP *M, unsigned int n)
{
	n--;
	if( M[NTYPE] ) {
		return (*(M + M[NSOFF] + n/32) & mask[n%32])?1:0;
	} else {
		return (*(M + NBASE + n/32) & mask[n%32])?1:0;
	}
}
/*****************************************************************************
* FUNCTION:  ymFirstSet
*
* DESCRIPTION: 
* 			This function returns the first position in 
* 			bitmap which is set i.e. the position of the 
* 			first bit which is set to one
*
* RETURNS:
*
******************************************************************************/
unsigned int	ymFirstSet( YBITMAP *M )
{
	unsigned int	i, n;
	unsigned long	nbits = *M;
	unsigned int	nbytes = (nbits/32) + 1;
	unsigned long	*m, *me;

	if( M[NTYPE] ) {
		unsigned int	r = 0;
		n = 0;

		for( i=M[NLEVS]; i; i-- ) {
			n = yzFfs( M[ M[NSOFF+i-1] + r] );
			if( !n ) {
				return 0;
			}
			n = (32 - n);
			r = 32*r + n;
		}
		return r + 1;
	} else {
		M += NBASE;
		me = M + nbytes;
		for( m=M; m<me; m++ ) {
			if( *m ) {
				n = 32*(m-M) + 33 - yzFfs(*m);
				return (n > nbits)?0:n;
			}
		}
		return 0;
	}
}
/*****************************************************************************
* FUNCTION:  ymFirstClear
*
* DESCRIPTION: 
* 			This function returns the first position in
* 			bitmap which is clear i.e. the position of
* 			the first bit which is set to zero
*
* RETURNS:
*
******************************************************************************/
unsigned int	ymFirstClear( YBITMAP *M )
{
	unsigned int	i, n;
	unsigned long	nbits = *M;
	unsigned int	nbytes = (nbits/32) + 1;
	unsigned long	*m, *me;

	if( M[NTYPE] ) {
		unsigned int	r = 0;
		n = 0;

		for( i=M[NLEVS]; i; i-- ) {
			n = yzFfs( ~M[ M[NCOFF+i-1] + r] );
			if( !n ) {
				return 0;
			}
			n = (32 - n);
			r = 32*r + n;
		}
		return r + 1;
	} else {
		M += NBASE;
		me = M + nbytes;
		for( m=M; m<me; m++ ) {
			if( ~(*m) ) {
				n = 32*(m-M) + 33 - yzFfs(~(*m));
				return (n > nbits)?0:n;
			}
		}
		return 0;
	}
}
/*****************************************************************************
* FUNCTION:  ymNextSet
*
* DESCRIPTION: 
*			This function returns the next 'set' position 
*			in bit map starting from position 'n'
* RETURNS:
*
******************************************************************************/
unsigned int	ymNextSet( YBITMAP *M, unsigned int n )
{
	unsigned int	bit, byte, i;
	unsigned long	nbits = *M;
	unsigned int	nbytes = (nbits/32) + 1;
	unsigned long	*m, *me;
	
	n--; 

	if( M[NTYPE] ) {
		unsigned int	r, s, t;
		unsigned long	L;

		s = n%32;
		r = n/32;
		for( i = 0; i < M[NLEVS]; i++ ) {
			L = M[M[NSOFF+i] + r] & rignore[s];
			if( (t = yzFfs( L )) != 0 ) {
				goto found;
			}
			s = r%32;
			r /= 32;
		}
		return 0;

	found:
		s = 32 - t;
		r = 32*r + s;
		if( i ) {
			for( ; i; i-- ) {
				L = M[M[NSOFF+i-1] + r];
				s = yzFfs( L );
				s = 32 - s;
				r = 32*r + s;
			}
		}
		return r + 1;
	} else {
		M += NBASE;
		byte = n/32;
		bit = n%32;
	
		n = yzFfs( M[byte] & rignore[bit] );	
		if( n ) {
			return 32*byte + 33 - n;
		}
		byte++;

		me = M + nbytes;
		for( m=M+byte; m<me; m++ ) {
			if( *m ) {
				n = 32*(m-M) + 33 - yzFfs(*m);
				return (n > nbits)?0:n;
			}
		}
		return 0;
	}
}
/*****************************************************************************
* FUNCTION:  ymNextSet
*
* DESCRIPTION: 
* 			This function returns the next 'clear' position 
* 			in bit map starting from position 'n'
* RETURNS:
*
******************************************************************************/
unsigned int	ymNextClear( YBITMAP *M, unsigned int n )
{
	unsigned int	bit, byte, i;
	unsigned long	nbits = *M;
	unsigned int	nbytes = (nbits/32) + 1;
	unsigned long	*m, *me;
	
	n--; 

	if( M[NTYPE] ) {
		unsigned int	r, s, t;
		unsigned long	L;

		s = n%32;
		r = n/32;
		for( i = 0; i < M[NLEVS]; i++ ) {
			L = M[M[NCOFF+i] + r] | ignore[s];
			if( (t = yzFfs( ~L )) != 0 ) {
				goto found;
			}
			s = r%32;
			r /= 32;
		}
		return 0;

	found:
		s = 32 - t;
		r = 32*r + s;
		if( i ) {
			for( ; i; i-- ) {
				L = M[M[NCOFF+i-1] + r];
				s = yzFfs( ~L );
				s = 32 - s;
				r = 32*r + s;
			}
		}
		return r + 1;
	} else {
		M += NBASE;
		byte = n/32;
		bit = n%32;
	
		n = yzFfs( ~(M[byte] | ignore[bit]) );	
		if( n ) {
			return 32*byte + 33 - n;
		}
		byte++;

		me = M + nbytes;
		for( m=M+byte; m<me; m++ ) {
			if( ~(*m) ) {
				n = 32*(m-M) + 33 - yzFfs(~(*m));
				return (n > nbits)?0:n;
			}
		}
		return 0;
	}
}


