/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Utility Library - Digest and Checksum Routines                         **/
/**                                                                        **/
/****************************************************************************/

#include <ylib.h>

/*****************************************************************************/
/** MD5 Message Digest Routnes - from RFC 1321 *******************************/
/*****************************************************************************/
static void	transform(unsigned long[4], const unsigned char[64]);
static void	encode(unsigned char *, const unsigned long *, unsigned int);
static void	decode(unsigned long *, const unsigned char *, unsigned int);

#define S11	7
#define S12	12
#define S13	17
#define S14	22
#define S21	5
#define S22	9
#define S23	14
#define S24	20
#define S31	4
#define S32	11
#define S33	16
#define S34	23
#define S41	6
#define S42	10
#define S43	15
#define S44	21

static const unsigned char PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

/* F, G, H and I are basic MD5 functions.*/
#define F(x,y,z)		(((x) & (y)) | ((~x) & (z)))
#define G(x,y,z)		(((x) & (z)) | ((y) & (~z)))
#define H(x,y,z)		((x) ^ (y) ^ (z))
#define I(x,y,z)		((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x,n)	(((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4. **/
#define FF(a,b,c,d,x,s,ac)	{ \
	(a) += F ((b),(c),(d)) + (x) + (unsigned long)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); (a) += (b); \
}

#define GG(a,b,c,d,x,s,ac)	{ \
	(a) += G ((b),(c),(d)) + (x) + (unsigned long)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); (a) += (b); \
}

#define HH(a,b,c,d,x,s,ac)	{ \
	(a) += H ((b),(c),(d)) + (x) + (unsigned long)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); (a) += (b); \
}

#define II(a,b,c,d,x,s,ac)	{ \
	(a) += I ((b),(c),(d)) + (x) + (unsigned long)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); (a) += (b); \
}

/** Static computation routines **********************************************/
static void transform( unsigned long state[4], const unsigned char block[64] )
{
	unsigned long a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	memset(x, 0, sizeof(unsigned long)*16);
	decode(x, block, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S11, 0xd76aa478);	/* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756);	/* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db);	/* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee);	/* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf);	/* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a);	/* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613);	/* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501);	/* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8);	/* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af);	/* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1);	/* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be);	/* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122);	/* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193);	/* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e);	/* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821);	/* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], S21, 0xf61e2562);	/* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340);	/* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51);	/* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);	/* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d);	/* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453);	/* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681);	/* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);	/* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6);	/* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6);	/* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87);	/* 27 */
	GG(b, c, d, a, x[8], S24, 0x455a14ed);	/* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905);	/* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8);	/* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9);	/* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);	/* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], S31, 0xfffa3942);	/* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681);	/* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122);	/* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c);	/* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44);	/* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9);	/* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60);	/* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70);	/* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6);	/* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa);	/* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085);	/* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05);	/* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039);	/* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5);	/* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8);	/* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665);	/* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], S41, 0xf4292244);	/* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97);	/* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7);	/* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039);	/* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3);	/* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92);	/* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d);	/* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1);	/* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f);	/* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0);	/* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314);	/* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1);	/* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82);	/* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235);	/* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb);	/* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391);	/* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information. */
	memset((unsigned char *) x, 0, sizeof(x));
}

static void encode( unsigned char *output, const unsigned long *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char) (input[i] & 0xff);
		output[j + 1] = (unsigned char) ((input[i] >> 8) & 0xff);
		output[j + 2] = (unsigned char) ((input[i] >> 16) & 0xff);
		output[j + 3] = (unsigned char) ((input[i] >> 24) & 0xff);
	}
}

static void decode( unsigned long *output, const unsigned char *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[i] = 
			((unsigned long) input[j]) |
			(((unsigned long) input[j+1])<<8) |
			(((unsigned long) input[j+2])<<16) |
			(((unsigned long) input[j+3])<<24);
	}
}

/** The entry points to the algorithm ****************************************/
void yeMd5Begin( YMD5 *C)
{
	C->count[0] = C->count[1] = 0;

	/* Load magic initialization constants. */
	C->state[0] = 0x67452301;
	C->state[1] = 0xefcdab89;
	C->state[2] = 0x98badcfe;
	C->state[3] = 0x10325476;
}

void yeMd5Update( YMD5 *C, const unsigned char *input, unsigned int n )
{
	unsigned int i, idx, partLen;

	/* Compute number of bytes mod 64 */
	idx = (unsigned int) ((C->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((C->count[0] += ((unsigned long) n << 3)) < ((unsigned long) n << 3)) {
		C->count[1]++;
	}

	C->count[1] += ((unsigned long) n >> 29);
	partLen = 64 - idx;

	/* Transform as many times as possible.  */
	if (n >= partLen) {
		memcpy ((unsigned char *) &C->buffer[idx], (const unsigned char *) input, partLen);
		transform(C->state, C->buffer);

		for (i = partLen; i + 63 < n; i += 64) {
			transform(C->state, &input[i]);
		}

		idx = 0;
	} else {
		i = 0;
	}

	/* Buffer remaining input */
	memcpy ((unsigned char *) &C->buffer[idx], (const unsigned char *) &input[i], n-i);
}

void yeMd5End( YMD5 *C, unsigned char digest[16] )
{
	unsigned char bits[8];
	unsigned int idx, padLen;

	/* Save number of bits */
	encode(bits, C->count, 8);

	/* Pad out to 56 mod 64.  */
	idx = (unsigned int) ((C->count[0] >> 3) & 0x3f);
	padLen = (idx < 56) ? (56 - idx) : (120 - idx);
	yeMd5Update(C, PADDING, padLen);

	/* Append length (before padding) */
	yeMd5Update(C, bits, 8);
	/* Store state in digest */
	encode(digest, C->state, 16);

	/* Zeroize sensitive information.  */
	memset((unsigned char *) C, 0, sizeof(*C));
}

void	yeMd5Digest( const unsigned char *buffer, unsigned int length, unsigned char result[16] )
{
	YMD5	C;

	yeMd5Begin( &C );
	yeMd5Update( &C, buffer, length );
	yeMd5End( &C, result );
}

/*****************************************************************************/
/** CRC Computation **********************************************************/
/*****************************************************************************/
static const unsigned char __reflected[256] = {
	0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 
	0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
	0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 
	0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
	0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 
	0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
	0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 
	0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
	0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 
	0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
	0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 
	0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 
	0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 
	0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
	0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 
	0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 
	0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 
	0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 
	0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 
	0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
	0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 
	0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 
	0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 
	0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 
	0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 
	0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
	0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 
	0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 
	0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 
	0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
	0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 
	0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
	
static const unsigned char __normal[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 
	0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 
	0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
	0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
	0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 
	0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 
	0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 
	0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF 
};

static	unsigned long reflect(unsigned long data, unsigned char nBits)
{
	unsigned long  reflection = 0;
	unsigned char bit;

	for (bit = 0; bit < nBits; ++bit) {

		if (data & 0x01) {
			reflection |= (1 << ((nBits - 1) - bit));
		}

		data = (data >> 1);
	}

	return (reflection);
}

void	yeCrcInit(YCRCTAB *T, unsigned int width, unsigned long poly, 
			unsigned long init, unsigned long final,
			int r_data, int r_result )
{
	unsigned long	rem, var_div;
	int		bit;

	T->initial = init;
	T->final = final;
	T->poly = poly;
	T->width = width;
	T->r_data = r_data;
	T->r_result = r_result;

	T->topbit = 1 << (T->width - 1);
	T->mask = ~0UL >> (32 - width);

	for( var_div=0; var_div<256; var_div++ ) {
		rem = var_div << (width - 8);
		for( bit=0; bit<8; bit++ ) {
			if( rem & T->topbit ) {
				rem = (rem << 1) ^ T->poly;
			} else {
				rem = (rem << 1);
			}
		}
		T->table[var_div] = rem & T->mask;
	}
}

unsigned long	yeCrcUpdate(const YCRCTAB *T, const unsigned char *buffer, 
			unsigned int count, unsigned long value )
{
	unsigned int	i;
	unsigned char	byte;
	const unsigned long
			*table = T->table;
	unsigned long	mask = T->mask,
			rem = mask & value;
	unsigned int	W = T->width - 8;
	const unsigned char *translate = (T->r_data)?__reflected:__normal;
	
	for( i=0; i<count; i++ ) {
		byte = (unsigned char)((rem >> W) ^ translate[buffer[i]]);
		rem = mask & (table[byte] ^ (rem << 8));
	}

	return rem;
}

unsigned long	yeCrcEnd( const YCRCTAB *T, unsigned long value )
{
	value = (unsigned long)(T->r_result?reflect(value, (unsigned char)T->width):value);
	return (value ^ T->final) & T->mask;
}

unsigned long	yeCrcCompute(const YCRCTAB *T, const unsigned char *buffer, unsigned int count )
{
	unsigned long	result = yeCrcBegin(T);
	result = yeCrcUpdate( T, buffer, count, result );
	return yeCrcEnd( T, result );
}

unsigned char	yzReflectU8( unsigned char N )
{
	return __reflected[N];
}

unsigned short	yzReflectU16( unsigned short N )
{
	return	(__reflected[YGETBYTE(N,0)] << 8) |
		(__reflected[YGETBYTE(N,1)]);
}

unsigned long	yzReflectU32( unsigned long N )
{
	return	(__reflected[YGETBYTE(N,0)] << 24) |
		(__reflected[YGETBYTE(N,1)] << 16) |
		(__reflected[YGETBYTE(N,2)] << 8) |
		(__reflected[YGETBYTE(N,3)]);
}
