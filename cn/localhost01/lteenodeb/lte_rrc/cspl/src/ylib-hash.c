/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <assert.h>
#include "ylib.h"

#define	QV_NIL	((YLNODE *)0)

/** Hash Tables *************************************************************/
void	yhInit( 
		YHASH *H, 
		unsigned int nbuckets, 
		unsigned long (*hash)(const void *), 
		int (*compare)(const void *, const void *), 
		const void *(*keyof)(const YHNODE *), 
		YHNODE **table 
)
{
	unsigned int	i;

	H->count = 0;
	H->nbuckets = nbuckets;
	H->hash = hash;
	H->keyof = keyof;
	H->compare = compare;
	H->table = table;

	for( i=0; i<nbuckets; i++ ) {
		table[i] = 0;
	}
}
void	yhInsert(YHASH *H, YHNODE *N)
{
	unsigned int	idx;

	idx = (H->hash(H->keyof(N))) % (H->nbuckets);
	N->link = H->table[idx];
	H->table[idx] = N;
	H->count++;
}

void	yhDelete(YHASH *H, YHNODE *N)
{
	unsigned int	idx;
	YHNODE	*p;

	idx = (H->hash(H->keyof(N))) % (H->nbuckets);
	p = H->table[idx];

	if( p == N ) {
		H->table[idx] = N->link;
		N->link = 0;
		H->count--;
	} else {
		while( p && (p->link != N) ) {
			p = p->link;
		}
		if( !p ) {
			return;
		}
		p->link = N->link;
		N->link = 0;
		H->count--;
	}
}

unsigned int	yhCount(const YHASH *H)
{
	return H->count;
}

YHNODE	*yhFind(const YHASH *H, const void *item)
{
	unsigned int	idx;
	YHNODE	*p;

	idx = (H->hash(item)) % (H->nbuckets);
	p = H->table[idx];

	while( p && H->compare( item, H->keyof(p) ) ) {
		p = p->link;
	}

	return p;
}

YHNODE	*yhFirst( const YHASH *H )
{
	unsigned int	idx;

	for( idx=0; idx<H->nbuckets; idx++ ) {
		if( H->table[idx] ) {
			return H->table[idx];
		}
	}
	return 0;
}

YHNODE	*yhNext( const YHASH *H, const YHNODE *N )
{
	unsigned int	idx;

	if( N->link ) {
		return N->link;
	}

	idx = (H->hash(H->keyof(N))) % (H->nbuckets);
	idx++;
	for( ; idx<H->nbuckets; idx++ ) {
		if( H->table[idx] ) {
			return H->table[idx];
		}
	}
	return 0;
}

