/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <assert.h>
#include "ylib.h"

#define	QV_NIL	((YLNODE *)0)

/** Pools (Free-Lists) ******************************************************/
void	ypInit(YPOOL *F)
{
	F->count = 0;
	F->link = 0;
}

unsigned int	ypCount(const YPOOL *F)
{
	return F->count;
}

YPNODE	*ypGet( YPOOL *F )
{
	YPNODE	*p = F->link;

	if( !p ) {
		return 0;
	}

	F->link = p->link;
	F->count--;
	return p;
}

void	ypPut( YPOOL *F, YPNODE *N )
{
	N->link = F->link;
	F->link = N;
	F->count++;
}

