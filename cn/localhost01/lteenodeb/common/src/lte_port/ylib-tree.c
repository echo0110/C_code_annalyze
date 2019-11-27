/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: ylib-tree.c
*
****************************************************************************
*
* File Description: This file contains APIs for performing operations on
*                   Ylib's tree data structure.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include <assert.h>
#include "ylib.h"

#define	IS_LEFT(p)	((p) == (p)->parent->child[0])
#define	IS_RIGHT(p)	((p) == (p)->parent->child[1])
#define	IS_ROOT(p)	((p)->parent->parent == 0)
#define SIBLING(p)	(((p)->parent->child[0] == (p)) ? ((p)->parent->child[1]) : ((p)->parent->child[0]))
#define	SLANT(p)	((p) == (p)->parent->child[1])
#define IS_RED(e)	((e) && (e)->red)
#define IS_BLACK(e)	(((e) == 0) || !((e)->red))
#define IS_BLACK_STRICT(e)	((e) && !((e)->red))



/** Local Functions *********************************************************/
static YTNODE *setchild(YTNODE *p, int dir, YTNODE *c)
{
	p->child[dir] = c;

	if (c)
		c->parent = p;

	return c;
}

static YTNODE *__basicrotate(YTNODE *p, int d)
{
	YTNODE *t = p->child[!d];

	setchild(p, !d, t->child[d]);
	setchild(t, d, p);

	return t;
}

static void rotate(YTNODE *n, int d)
{
	YTNODE *p = n->parent, *r;
	int s = SLANT(n);

	r = __basicrotate(n, d);
	setchild(p, s, r);
}

static YTNODE *__singlerotate(YTNODE *p, int d)
{
	YTNODE	*t = __basicrotate(p, d);

	p->red = 1;
	t->red = 0;
	return t;
}

static YTNODE *__doublerotate(YTNODE *p, int d)
{
	setchild(p, !d, __singlerotate(p->child[!d], !d));
	return __singlerotate(p, d);
}

static void __swap(YTNODE *a, YTNODE *b)
{
	YTNODE *ap, *bp, *ac[2], *bc[2];
	int ad, bd, red;

	ap = a->parent;
	ad = (a == ap->child[1]);
	ac[0] = a->child[0];
	ac[1] = a->child[1];

	bp = b->parent;
	bd = (b == bp->child[1]);
	bc[0] = b->child[0];
	bc[1] = b->child[1];

	if (bp == a) {
		setchild(ap, ad, b);
		setchild(b, bd, a);
		setchild(b, !bd, ac[!bd]);

		setchild(a, 0, bc[0]);
		setchild(a, 1, bc[1]);
	} else if (ap == b) {
		setchild(bp, bd, a);
		setchild(a, ad, b);
		setchild(a, !ad, bc[!ad]);

		setchild(b, 0, ac[0]);
		setchild(b, 1, ac[1]);
	} else {
		setchild(ap, ad, b);
		setchild(bp, bd, a);

		setchild(a, 0, bc[0]);
		setchild(a, 1, bc[1]);

		setchild(b, 0, ac[0]);
		setchild(b, 1, ac[1]);
	}

	/*Swap the colours*/
	red = a->red;
	a->red = b->red;
	b->red = red;
}

static int __redblack(YTNODE *root, int (*compare)(const void *, const void *), const void *(*keyof)(const YTNODE *))
{
	int	hl, hr;
	YTNODE	*L, *R;

	if (!root) {
		return 1;
	}

	L = root->child[0];
	R = root->child[1];

	/*Check for parentage violation*/
	if (L && (L->parent != root)) {
		return 0;
	}

	if (R && (R->parent != root)) {
		return 0;
	}

	/* Check for red violation */
	if (root->red) {
		if (L && L->red) {
			return 0;
		}

		if (R && R->red) {
			return 0;
		}
	}

	hl = __redblack(L, compare, keyof);
	hr = __redblack(R, compare, keyof);

	/* Check for binary tree property */
	if (L && (compare( keyof(L), keyof(root)) >= 0) ) {
		return 0;
	}

	if (R && (compare( keyof(R), keyof(root)) <= 0) ) {
		return 0;
	}

	/* Check for black violation */
	if (hl && hr && (hl != hr)) {
		return 0;
	}

	/* Return height */
	if (hl && hr) {
		return root->red ? hl : (hl + 1);
	} else {
		return 0;
	}
}

int __ytAudit(YTREE * tree)
{
	return __redblack(tree->root.child[0], tree->compare, tree->keyof);
}

static void inorder(YTNODE *n, void (*walker)(YTNODE *, void *), void *z)
{
	if (n->child[0]) {
		inorder(n->child[0], walker, z);
	}

	walker(n, z);

	if (n->child[1]) {
		inorder(n->child[1], walker, z);
	}
}

static void preorder(YTNODE *n, void (*walker)(YTNODE *, void *), void *z)
{
	walker(n, z);

	if (n->child[0]) {
		preorder(n->child[0], walker, z);
	}

	if (n->child[1]) {
		preorder(n->child[1], walker, z);
	}
}

static void postorder(YTNODE *n, void (*walker)(YTNODE *, void *), void *z)
{
	if (n->child[0]) {
		postorder(n->child[0], walker, z);
	}

	if (n->child[1]) {
		postorder(n->child[1], walker, z);
	}

	walker(n, z);
}

static unsigned int __treeheight(const YTNODE *n)
{
	unsigned int l = 0, r = 0;

	if (!n) {
		return 0;
	} else {
		l = __treeheight(n->child[0]);
		r = __treeheight(n->child[1]);
		if (l > r) {
			return l + 1;
		} else {
			return r + 1;
		}
	}
}

unsigned int __ytHeight(const YTREE *T)
{
	return __treeheight(T->root.child[0]);
}


/** Red-Black Trees *********************************************************/

static void __adjust(YTNODE *e)
{
	while(1) {
		YTNODE *s, *p, *n[2], *c[2];
		int d;

		if (e->parent == 0) {
			return;
		}

		if (IS_ROOT(e)) {
			/* Case 1: The node is the root*/
			return;
		}

		p = e->parent;
		d = SLANT(e);
		s = SIBLING(e);
		c[0] = e->child[0];
		c[1] = e->child[1];

		if (IS_RED(s)) {
			/* Case 2: The node's sibling is red
			this falls through to the next case*/
			p->red = 1;
			s->red = 0;
			rotate(p, d);

			assert(c[0] == e->child[0]);
			assert(c[1] == e->child[1]);
		}

		p = e->parent;
		d = SLANT(e);
		s = SIBLING(e);
		n[0] = s->child[0];
		n[1] = s->child[1];

		if (IS_BLACK(p) && IS_BLACK_STRICT(s) && IS_BLACK(n[0]) && IS_BLACK(n[1])) {
			/*Case 3: The node's parent, sibling & nephews are black*/
			s->red = 1;
			e = e->parent;

			continue;
		}

		if (IS_RED(p) && IS_BLACK_STRICT(s) && IS_BLACK(n[0]) && IS_BLACK(n[1])) {
			/*Case 4: The node's parent is red, sibling & nephews are black*/
			s->red = 1;
			p->red = 0;
			return;
		}

		if (IS_BLACK_STRICT(s) && IS_RED(n[d]) && IS_BLACK(n[!d])) {
			/* Case 5: The node's sibling is black, near nephew is red and far nephew is black
			-- this falls through to the next case */
			s->red = 1;
			n[d]->red = 0;
			rotate(s, !d);
		}

		p = e->parent;
		d = SLANT(e);
		s = SIBLING(e);
		n[0] = s->child[0];
		n[1] = s->child[1];

		/*Case 6: The node's sibling is black, far nephew is red */
		assert(IS_BLACK(s) && IS_RED(n[!d]));

		s->red = p->red;
		p->red = 0;
		n[!d]->red = 0;
		rotate(p, d);

		return;
	}
}

void ytInit(YTREE *tree, int (*compare)(const void *, const void *), const void *(*keyof)(const YTNODE *))
{
	assert(compare != NULL);
	assert(keyof != NULL);

	tree->root.child[0] = NULL;
	tree->root.child[1] = NULL;
	tree->root.parent = NULL;
	tree->compare = compare;
	tree->keyof = keyof;
	tree->count = 0;
}

/** Binary Tree Routines ****************************************************/
YTNODE	*ytFirst(const YTREE *tree, int order)
{
	YTNODE *p = tree->root.child[0];

	if (!p) {
		return 0;
	}

	switch (order) {
		case T_INORDER:
			while (p->child[0]) {
				p = p->child[0];
			}
			return p;
		case T_POSTORDER:
			while (p->child[0] || p->child[1]) {
				if (p->child[0]) {
					p = p->child[0];
				} else {
					p = p->child[1];
				}
			}
			return p;
		case T_PREORDER:
			return p;
		default:
			return 0;
	}
}

YTNODE *ytLast(const YTREE *tree, int order)
{
	YTNODE	*p = tree->root.child[0];

	if (!p) {
		return 0;
	}

	switch (order) {
		case T_INORDER:
			while (p->child[1]) {
				p = p->child[1];
			}
			return p;
		case T_PREORDER:
			while (p->child[0] || p->child[1]) {
				if (p->child[1]) {
					p = p->child[1];
				} else {
					p = p->child[0];
				}
			}
			return p;
		case T_POSTORDER:
			return p;
		default:
			return 0;
	}
}

YTNODE *ytNext(const YTNODE *p, int order)
{
	switch (order) {
		case T_PREORDER:
			if (p->child[0]) {
				return p->child[0];
			} else if (p->child[1]) {
				return p->child[1];
			} else if (IS_ROOT(p)) {
				return 0;
			} else if (IS_LEFT(p)) {
				if (p->parent->child[1]) {
					return p->parent->child[1];
				} else {
					p = p->parent;
				}
			} else {
				p = p->parent;
			}

			while (!IS_ROOT(p)) {
				if (IS_LEFT(p)) {
					if (p->parent->child[1]) {
						return p->parent->child[1];
					} else {
						p = p->parent;
					}
				} else {
					p = p->parent;
				}
			}
			return 0;

		case T_POSTORDER:
			if (IS_ROOT(p)) {
				return 0;
			} else if (IS_LEFT(p)) {
				if (p->parent->child[1]) {
					p = p->parent->child[1];
					while (p->child[0] || p->child[1]) {
						if (p->child[0]) {
							p = p->child[0];
						} else {
							p = p->child[1];
						}
					}
					return (YTNODE *)p;
				} else {
					return p->parent;
				}
			} else {
				return p->parent;
			}
		case T_INORDER:
			if (p->child[1]) {
				p = p->child[1];
				while (p->child[0]) {
					p = p->child[0];
				}
				return (YTNODE *)p;
			} else {
				while (IS_RIGHT(p)) {
					p = p->parent;
					if (IS_ROOT(p)) {
						return 0;
					}
				}
				return IS_ROOT(p)?0:p->parent;
			}

		default:
			return 0;
	}
}

YTNODE *ytPrev(const YTNODE *p, int order)
{
	switch (order) {
		case T_PREORDER:
			if (IS_ROOT(p)) {
				return 0;
			} else if (IS_RIGHT(p)) {
				if (p->parent->child[0]) {
					p = p->parent->child[0];
					while (p->child[0] || p->child[1]) {
						if (p->child[1]) {
							p = p->child[1];
						} else {
							p = p->child[0];
						}
					}
					return (YTNODE *)p;
				} else {
					return p->parent;
				}
			} else {
				return p->parent;
			}
		case T_POSTORDER:
			if (p->child[1]) {
				return p->child[1];
			} else if (p->child[0]) {
				return p->child[0];
			} else if (IS_ROOT(p)) {
				return 0;
			} else if (IS_RIGHT(p)) {
				if (p->parent->child[0]) {
					return p->parent->child[0];
				} else {
					p = p->parent;
				}
			} else {
				p = p->parent;
			}

			while (!IS_ROOT(p)) {
				if (IS_RIGHT(p)) {
					if (p->parent->child[0]) {
						return p->parent->child[0];
					} else {
						p = p->parent;
					}
				} else {
					p = p->parent;
				}
			}
			return 0;
		case T_INORDER:
			if (p->child[0]) {
				p = p->child[0];
				while (p->child[1]) {
					p = p->child[1];
				}
				return (YTNODE *)p;
			} else {
				while (IS_LEFT(p)) {
					p = p->parent;
					if (IS_ROOT(p)) {
						return 0;
					}
				}
				return IS_ROOT(p)?0:p->parent;
			}
		default:
			return 0;
	}
}

unsigned int ytCount(const YTREE *tree)
{
	assert(tree != 0);
	return tree->count;
}

YTNODE *ytFind(const YTREE *tree, const void *item)
{
	YTNODE *p;

	assert(tree != NULL);
	for (p = tree->root.child[0]; p; ) {
		int diff = tree->compare(item, tree->keyof(p));

		if (diff < 0) {
			p = p->child[0];
		} else if (diff > 0) {
			p = p->child[1];
		} else {
			return p;
		}
	}

	return NULL;
}

void ytWalkOrder(const YTREE *tree, void (*walker)(YTNODE *, void *), void *z, int order)
{
	YTNODE *p;

	assert(tree && walker);
	if ((p = tree->root.child[0]) == 0) {
		return;
	}

	switch (order) {
		case T_PREORDER:
			if (p) {
				preorder(p, walker, z);
			}
			return;
		case T_POSTORDER:
			if (p) {
				postorder(p, walker, z);
			}
			return;
		case T_INORDER:
			if (p) {
				inorder(p, walker, z);
			}
			return;
		default:
			return;
	}
}

void ytWalk(const YTREE *tree, void (*walker)(YTNODE *, void *), void *z)
{
	YTNODE	*p;

	for (p = ytFirst(tree, T_INORDER); p; p = ytNext(p, T_INORDER)) {
		walker(p, z);
	}
}
void ytInsertWithDuplicate(YTREE *tree, YTNODE *entry)
{
	YTNODE	dummy;
	YTNODE	*g = 0, *t = &dummy, *p = 0, *q;
	int	d = 0, last = 0;

	if ((tree == 0) || (entry == 0)) {
		return;
	}

    /*changes for UMR resulting in crashes and asserts */
	entry->parent = entry->child[0] = entry->child[1] = NULL;
    /*changes for UMR resulting in crashes and asserts */

	if ((q = tree->root.child[0]) == 0) {
		/* Empty tree*/
		entry->red = 0;		/*Root is always black*/
		entry->child[0] = entry->child[1] = 0;
		setchild(&(tree->root), 0, entry);
		tree->count++;

		assert(tree->count == 1);
		return;
	}

	dummy.parent = 0;
	dummy.child[0] = 0;
	dummy.child[1] = q;
	dummy.red = 1;

	while(1) {
		int	v;

		if (q == 0) {
			entry->child[0] = entry->child[1] = 0;
			entry->red = 1;
			q = setchild(p, d, entry);
		} else if (q->child[0] && q->child[1] && q->child[0]->red && q->child[1]->red) {
			/*Node with 2 red children -- flip colors*/
			q->red = 1;
			q->child[0]->red = q->child[1]->red = 0;
		}

		assert(q);

		/* Fix red violation*/
		if (p && q->red && p->red) {
			int	e = (g == t->child[1]);

			if (q == p->child[last]) {
				setchild(t, e, __singlerotate(g, !last));
			} else {
				setchild(t, e, __doublerotate(g, !last));
			}
		}

		v = tree->compare(tree->keyof(q), tree->keyof(entry));
		if (v == 0 && entry->parent != 0) {
			break;
		}

		last = d;
		d = (v <= 0);

		if (g) {
			t = g;
		}

		g = p;
		p = q;
		q = q->child[d];
	}

	/* Update the root*/
	setchild(&(tree->root), 0, dummy.child[1]);

	/* Ensure tree root is black*/
	tree->root.child[0]->red = 0;

	tree->count++;
}

void ytInsert(YTREE *tree, YTNODE *entry)
{
	YTNODE	dummy;
	YTNODE	*g = 0, *t = &dummy, *p = 0, *q;
	int	d = 0, last = 0;

	if ((tree == 0) || (entry == 0)) {
		return;
	}

	if ((q = tree->root.child[0]) == 0) {
		/* Empty tree*/
		entry->red = 0;		/*Root is always black*/
		entry->child[0] = entry->child[1] = 0;
		setchild(&(tree->root), 0, entry);
		tree->count++;

		assert(tree->count == 1);
		return;
	}

	dummy.parent = 0;
	dummy.child[0] = 0;
	dummy.child[1] = q;
	dummy.red = 1;

	while(1) {
		int	v;

		if (q == 0) {
			entry->child[0] = entry->child[1] = 0;
			entry->red = 1;
			q = setchild(p, d, entry);
		} else if (q->child[0] && q->child[1] && q->child[0]->red && q->child[1]->red) {
			/*Node with 2 red children -- flip colors*/
			q->red = 1;
			q->child[0]->red = q->child[1]->red = 0;
		}

		assert(q);

		/* Fix red violation*/
		if (p && q->red && p->red) {
			int	e = (g == t->child[1]);

			if (q == p->child[last]) {
				setchild(t, e, __singlerotate(g, !last));
			} else {
				setchild(t, e, __doublerotate(g, !last));
			}
		}

		v = tree->compare(tree->keyof(q), tree->keyof(entry));
		if (v == 0) {
			break;
		}

		last = d;
		d = (v < 0);

		if (g) {
			t = g;
		}

		g = p;
		p = q;
		q = q->child[d];
	}

	/* Update the root*/
	setchild(&(tree->root), 0, dummy.child[1]);

	/* Ensure tree root is black*/
	tree->root.child[0]->red = 0;

	tree->count++;
}
void ytDelete(YTREE *tree, YTNODE *entry)
{
	int	d;
	YTNODE	*p;

	if ((tree == 0) || (entry == 0)) {
		return;
	}

	if (entry->child[0] && entry->child[1]) {
		YTNODE *m = entry->child[0];

		while (m->child[1]) {
			m = m->child[1];
		}

		/* Swap the node and its predecessor*/
		__swap(entry, m);
	}

	assert(!(entry->child[0] && entry->child[1]));

	if (!entry->red) {
		if (IS_RED(entry->child[0])) {
			entry->red = 1;
			entry->child[0]->red = 0;
		} else if (IS_RED(entry->child[1])) {
			entry->red = 1;
			entry->child[1]->red = 0;
		} else {
			__adjust(entry);
		}
	} else {
	}

	assert(!(entry->child[0] && entry->child[1]));

	p = entry->parent;
	d = (entry == p->child[1]);

	if (entry->child[0]) {
		setchild(p, d, entry->child[0]);
	} else {
		setchild(p, d, entry->child[1]);
	}

	entry->parent = entry->child[0] = entry->child[1] = 0;
	tree->count--;
}
