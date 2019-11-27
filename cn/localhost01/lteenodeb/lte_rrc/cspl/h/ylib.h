/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Utility Library Interface Definition                                   **/
/**                                                                        **/
/****************************************************************************/
/*This file is changed corrosponding to SPR-4987*/
#ifndef __CommonStackPortingLayerUtilityLibraryHss__
#define __CommonStackPortingLayerUtilityLibraryHss__
#ifndef _MSC_VER
#include	<os.h>
#endif

#define T_INORDER 0
#define T_PREORDER 1
#define T_POSTORDER 2



#define  YOFFSETOF(s,f)   ((unsigned long)( &(((s *)0)->f)))
#define	 __YMEMBEROF(s,f,p) ((s *)((unsigned long)(p) - (unsigned long)&((s *)0)->f))
#define	 YMEMBEROF(s,f,p)   ((p)?__YMEMBEROF(s,f,p):((s *)0))
#define  YALIGN(p,n)	  ((((p) + ((n) - 1))/(n))*(n))

#define	__EX(oc, ic, rs, m, x) (oc(( (ic(x)) >> rs)&m))
#define	YGETNIBBLE(x,n)	__EX( (unsigned char), (unsigned long), (4*(n)), 0x0000000F, (x) )
#define	YGETBYTE(x,n)	__EX( (unsigned char), (unsigned long), (8*(n)), 0x000000FF, (x) )
#define	YGETWORD(x,n)	__EX( (unsigned short), (unsigned long), (16*(n)), 0x0000FFFF, (x) )

#define	__EY(n)		(((n)+31)/32)
#define	YMAPSIZE(n)	(1 + 1 + 1 + 6 + 6 + \
				__EY(n) + \
				2*__EY(__EY(n)) + \
				2*__EY(__EY(__EY(n))) + \
				2*__EY(__EY(__EY(__EY(n)))) + \
				2*__EY(__EY(__EY(__EY(__EY(n))))) + \
				2*__EY(__EY(__EY(__EY(__EY(__EY(n)))))) )

enum	{
	__BE = 1,
	__LE = 2
};

typedef	struct {
	unsigned char	*byte;
	unsigned char	bit;
	unsigned char	endian;
}	YBITPTR;

typedef	unsigned long	YBITMAP;

typedef	struct {
	unsigned long	initial, final, poly;
	unsigned long	mask, topbit;
	unsigned int	width;
	unsigned int	r_data, r_result;
	unsigned long	table[256];
}	YCRCTAB;

typedef struct {
	unsigned long state[4];
	unsigned long count[2];
	unsigned char buffer[64];
} YMD5;

typedef union __ynode__ YNODE;
typedef struct __ylnode__ YLNODE;
typedef struct __yhnode__ YHNODE; 
typedef	struct __ypnode__ YPNODE;
typedef struct __ytnode__ YTNODE;
typedef struct __ysnode__ YSNODE;

struct __ylnode__ {
	YLNODE *next;
	YLNODE *previous;
};

struct __ytnode__ {
	YTNODE *child[2];
	YTNODE *parent;	
	unsigned char red;
};

struct __ysnode__ {
	struct	{
		YSNODE *next;
		YSNODE *previous;
	} v, h;
};

struct __yhnode__ {
	YHNODE *link;
};

struct __ypnode__ {
	YPNODE *link;
};

union __ynode__ {
	YLNODE	l;
	YTNODE	t;
	YHNODE	h;
	YSNODE	s;
};

typedef struct{
	YLNODE node;
	unsigned int count;
} YLIST;

typedef struct  {
	YTNODE	root;
	int	(*compare)(const void *, const void *);	
	const void	*(*keyof)(const YTNODE *);
	unsigned int	
		count;
} YTREE;

typedef struct  {
	unsigned long
		(*hash)(const void *);	
	int	(*compare)(const void *, const void *);	
	const void	*(*keyof)(const YHNODE *);
	YHNODE	**table;
	unsigned int	
		nbuckets;
	unsigned int	
		count;
} YHASH;

typedef struct  {
	YPNODE	*link;
	unsigned int	
		count;
} YPOOL;

typedef struct  {
	YSNODE	*head;
	YSNODE	*tail;
	unsigned int	
		count;
	int	(*priority)(const YSNODE *);
} YSQUEUE;

/** Red-black Trees *********************************************************/
void	ytInit( YTREE *, int (*)(const void *, const void *), const void *(*)(const YTNODE *) );

unsigned int	
	ytCount(const YTREE *);
void	ytWalk(const YTREE *, void (*)(YTNODE *, void *), void * );
void	ytWalkOrder(const YTREE *, void (*)(YTNODE *, void *), void *, int );
YTNODE	*ytFirst(const YTREE *, int );
YTNODE	*ytLast(const YTREE *, int );
YTNODE	*ytNext(const YTNODE *, int );
YTNODE	*ytPrev(const YTNODE *, int );

void	ytInsert(YTREE *, YTNODE *);
void	ytDelete(YTREE *, YTNODE *);

YTNODE	*ytFind(const YTREE *, const void *);
#define	ytEmpty(T)	((T)->count == 0)

unsigned int	__ytHeight(const YTREE *);
int	__ytAudit(YTREE *);
/*SPR_16680_START*/
void ytInsertWithDuplicate(YTREE *, YTNODE *);
/*SPR_16680_END*/

/** AVL Trees ***************************************************************/
#define	yaInit	ytInit
#define	yaCount	ytCount
#define	yaWalk	ytWalk
#define	yaFirst	ytFirst
#define	yaLast	ytLast
#define	yaNext	ytNext
#define	yaPrev	ytPrev

/*The following two AVL tree functions are mapped on to RB tree 
functions & their implemementation have been removed so now RB tree
 & AVL trees in CSPL are exactly similar*/
#define yaDelete ytDelete
#define yaInsert ytInsert

#define	yaFind	ytFind
#define	yaEmpty(T)	((T)->count == 0)

#define	__yaHeight __ytHeight

/** Doubly Linked List ******************************************************/
void	ylInit(YLIST *pList);

unsigned int	
	ylCount(const YLIST *pList);
YLNODE	*ylFirst(const YLIST *pList);
YLNODE	*ylLast(const YLIST *pList);
YLNODE	*ylNext(const YLNODE *pNode);
YLNODE	*ylPrev(const YLNODE *pNode);

#define	ylEmpty(L)	((L)->count == 0)
void	ylInsertAfter(YLIST *pList, YLNODE *pPrev, YLNODE *pNode);
void	ylInsertBefore(YLIST *pList, YLNODE *pNext, YLNODE *pNode);
void	ylDelete(YLIST *pList, YLNODE *pNode);

void	ylPushHead(YLIST *pList, YLNODE *pNode);
void	ylPushTail(YLIST *pList, YLNODE *pNode);
YLNODE	*ylPopHead(YLIST *pList);
YLNODE	*ylPopTail(YLIST *pList);

void	ylInsert(YLIST *, YLNODE *, const void *(*)(const YLNODE *), int (*)(const void *, const void *) );
void	ylWalk(const YLIST *, void (*)(YLNODE *, void *), void * );
YLNODE	*ylFind(const YLIST *, const void *, const void *(*)(const YLNODE *), int (*)(const void *, const void *) );
void	ylSort(YLIST *, const void *(*)(const YLNODE *), int (*)(const void *, const void *) );

void	ylConcat(YLIST *pDstList, YLIST *pAddList);
YLNODE	*ylNth(const YLIST *pList, unsigned int nodenum);
YLNODE	*ylStep(YLNODE *pNode, int nStep);
int	ylIndex(const YLIST *pList, YLNODE *pNode);

/** Doubly Linked List (Alternate) ******************************************/
void	ycInit(YLIST *pList);
#define	ycFirst	ylFirst
#define	ycLast	ylLast
#define	ycNext	ylNext
#define	ycPrev	ylPrev

#define	ycEnd(L) (&((L)->node))

void	ycInsertAfter(YLNODE *pPrev, YLNODE *pNode);
void	ycInsertBefore(YLNODE *pNext, YLNODE *pNode);
YLNODE	*ycDelete( YLNODE *pNode);

#define	ycPushHead(L, N)	ycInsertAfter( ycEnd(L), N )
#define	ycPushTail(L, N)	ycInsertBefore( ycEnd(L), N )
#define	ycPopHead(L)		ycDelete( ycEnd(L)->next )
#define	ycPopTail(L)		ycDelete( ycEnd(L)->previous )

void	ycConcat(YLIST *pDstList, YLIST *pAddList);
void	ycExtract(YLIST *pSrcList, YLNODE *pStartNode, YLNODE *pEndNode, YLIST *pDstList);

YLNODE	*ycFind(const YLIST *, const void *, const void *(*)(const YLNODE *), int (*)(const void *, const void *) );
void	ycSort(YLIST *, const void *(*)(const YLNODE *), int (*)(const void *, const void *) );

int	ycEmpty( const YLIST * );

/** Hash Tables *************************************************************/
void	yhInit( 
			YHASH *, unsigned int, unsigned long (*)(const void *), 
			int (*)(const void *, const void *), const void *(*)(const YHNODE *), YHNODE ** );
void	yhInsert(YHASH *, YHNODE *);
void	yhDelete(YHASH *, YHNODE *);
unsigned int	yhCount(const YHASH *);
#define	yhEmpty(L)	((L)->count == 0)
YHNODE	*yhFind(const YHASH *, const void *);
YHNODE	*yhFirst( const YHASH *H );
YHNODE	*yhNext( const YHASH *H, const YHNODE *N );

/** Pools (Free Lists) ******************************************************/
void	ypInit(YPOOL *);
unsigned int	ypCount(const YPOOL *);
#define	ypEmpty(L)	((L)->count == 0)
YPNODE	*ypGet( YPOOL * );
void	ypPut( YPOOL *, YPNODE * );

/** Priority Queues *********************************************************/
void	ysInit(YSQUEUE *, int (*)(const YSNODE *) );
unsigned int	ysCount(const YSQUEUE *);
#define	ysEmpty(L)	((L)->count == 0)
YSNODE	*ysPopHead( YSQUEUE * );
void	ysPushTail( YSQUEUE *, YSNODE * );

/** Miscellaneous ***********************************************************/
int		yzFfs( unsigned long N );
unsigned short	yzSwapU16( unsigned short N );
unsigned long	yzSwapU32( unsigned long N );
unsigned char	yzReflectU8( unsigned char N );
unsigned short	yzReflectU16( unsigned short N );
unsigned long	yzReflectU32( unsigned long N );

/** Checksums and Digests ***************************************************/
void		yeCrcInit(YCRCTAB *T, unsigned int width, unsigned long poly, 
			unsigned long init, unsigned long final,
			int r_data, int r_result);
#define	yeCrcBegin(T)	((unsigned long)((T)->initial))
unsigned long	yeCrcUpdate(const YCRCTAB *T, const unsigned char *buffer, 
			unsigned int count, unsigned long value );
unsigned long	yeCrcEnd( const YCRCTAB *T, unsigned long value );
unsigned long	yeCrcCompute(const YCRCTAB *T, const unsigned char *buffer, unsigned int count );

void		yeMd5Digest(const unsigned char *buffer, unsigned int count, unsigned char out[16] );
void		yeMd5Begin(YMD5 *);
void		yeMd5Update(YMD5 *, const unsigned char *, unsigned int);
void		yeMd5End(YMD5 *, unsigned char[16]);

/** Bitstrings **************************************************************/
void		ybSetup( YBITPTR *B, unsigned char *message, unsigned char endian );
int		ybPtrCompare( YBITPTR *l, YBITPTR *r );
unsigned int	ybEnough( const YBITPTR *here, const YBITPTR *end, int nbits );
void		ybPeek( YBITPTR *here, unsigned long *to, unsigned int nbits );
void		ybSkip( YBITPTR *here, unsigned int nbits );
void		ybGet( YBITPTR *here, unsigned long *to, unsigned int nbits );
void		ybAdd( YBITPTR *here, unsigned long from, unsigned int nbits );
void		ybGetBuf( YBITPTR *here, unsigned char *to, unsigned int nbytes );
void		ybAddBuf( YBITPTR *here, unsigned char *from, unsigned int nbytes );
void		ybGetU8( YBITPTR *here, unsigned char *to );
void		ybAddU8( YBITPTR *here, unsigned char from );
void		ybGetU16( YBITPTR *here, unsigned short *to );
void		ybAddU16( YBITPTR *here, unsigned short from );
void		ybGetU32( YBITPTR *here, unsigned long *to );
void		ybAddU32( YBITPTR *here, unsigned long from );

/** Octet Buffers, Build and Parse ******************************************/
unsigned char	*yfAddBuf( unsigned char *buf, unsigned int count, unsigned char *newbuffer );
unsigned char	*yfGetBuf( unsigned char *buf, unsigned int count, unsigned char *newbuffer );
unsigned char	*yfAddU8( unsigned char *buf, unsigned char value );
unsigned char	*yfGetU8( unsigned char *buf, unsigned char *value );
unsigned char	yfGetU8At( unsigned char *buf, int offset );
void		yfSetU8At( unsigned char *buf, int offset, unsigned char value );
unsigned char	*yfAddU16( unsigned char *buf, unsigned short value );
unsigned char	*yfGetU16( unsigned char *buf, unsigned short *value );
unsigned short	yfGetU16At( unsigned char *buf, int offset );
void		yfSetU16At( unsigned char *buf, int offset, unsigned short value );
unsigned char	*yfAddU32( unsigned char *buf, unsigned long value );
unsigned char	*yfGetU32( unsigned char *buf, unsigned long *value );
unsigned long	yfGetU32At( unsigned char *buf, int offset );
void		yfSetU32At( unsigned char *buf, int offset, unsigned long value );
unsigned char	*yfFill( unsigned char *buf, unsigned int count, unsigned char c );
unsigned char	*yfSkip( unsigned char *buf, unsigned int count );
unsigned char	*yfPad( unsigned char *scan, unsigned char *start, unsigned int to, unsigned char c );

/** Bitmaps *****************************************************************/
void		ymInit(YBITMAP *M, unsigned int nbits, int level);
void		ymSet(YBITMAP *M, unsigned int n);
void		ymClear(YBITMAP *M, unsigned int n);
int		ymBit(YBITMAP *M, unsigned int n);
unsigned int	ymFirstSet( YBITMAP *M );
unsigned int	ymFirstClear( YBITMAP *M );
unsigned int	ymNextSet( YBITMAP *M, unsigned int n );
unsigned int	ymNextClear( YBITMAP *M, unsigned int n );

#endif	
