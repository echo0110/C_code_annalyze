/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: ylib-hash.c
*
****************************************************************************
*
* File Description: This file contains APIs for Ylib's Hash functionality.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include <assert.h>
#include "ylib.h"

#define	QV_NIL	((YLNODE *)0)

/** Hash Tables *************************************************************/
void yhInit(YHASH *H, unsigned int nbuckets, unsigned long (*hash)(const void *),
			int (*compare)(const void *, const void *), const void *(*keyof)(const YHNODE *),
			YHNODE **table)
{
	unsigned int i;

	H->count = 0;
	H->nbuckets = nbuckets;
	H->hash = hash;
	H->keyof = keyof;
	H->compare = compare;
	H->table = table;

	for (i = 0; i < nbuckets; i++)
		table[i] = 0;
}
void yhInsert(YHASH *H, YHNODE *N)
{
	unsigned int index;

	index = (H->hash(H->keyof(N))) % (H->nbuckets);
	N->link = H->table[index];
	H->table[index] = N;
	H->count++;
}

void yhDelete(YHASH *H, YHNODE *N)
{
	unsigned int index;
	YHNODE *p;

	index = (H->hash(H->keyof(N))) % (H->nbuckets);
	p = H->table[index];

	if (p == N) {
		H->table[index] = N->link;
		N->link = 0;
		H->count--;
	} else {
		while (p && (p->link != N))
			p = p->link;

		if (!p)
			return;

		p->link = N->link;
		N->link = 0;
		H->count--;
	}
}

unsigned int yhCount(const YHASH *H)
{
	return H->count;
}

YHNODE *yhFind(const YHASH *H, const void *item)
{
	unsigned int index;
	YHNODE *p;

	index = (H->hash(item)) % (H->nbuckets);
	p = H->table[index];

	while (p && H->compare(item, H->keyof(p)))
		p = p->link;

	return p;
}

YHNODE *yhFirst(const YHASH *H)
{
	unsigned int index;

	for (index = 0; index < H->nbuckets; index++) {
		if (H->table[index])
			return H->table[index];
	}

	return 0;
}

YHNODE *yhNext(const YHASH *H, const YHNODE *N)
{
	unsigned int index;

	if (N->link)
		return N->link;

	index = (H->hash(H->keyof(N))) % (H->nbuckets);
	index++;
	for ( ; index < H->nbuckets; index++) {
		if (H->table[index]) {
			return H->table[index];
		}
	}
	return 0;
}
