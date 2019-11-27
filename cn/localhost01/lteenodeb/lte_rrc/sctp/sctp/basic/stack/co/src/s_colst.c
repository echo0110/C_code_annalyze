/***************************************************************************
 *  FILE NAME:
 *      s_colst.c
 *
 *  DESCRIPTION:
 *      List processing functions used by timer,
 *         freelist for sack and transmit queue and hashtables.
 *
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  -------------------------------------
 *  22Mar00  sigtran team      -----  Original
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/

#include    "s_colst.h"

#ifndef NIL
#define    NIL    ((NODE *)0)
#endif

/****************************************************************************
 *    Function:     lstInit
 *
 *    Description:
 *                   Initialize the Linked List
 *
 *    Returns:
 *                   None
 ****************************************************************************/
void
lstInit(
    LIST *pList )
{
    pList->count = 0;
    pList->node.next = NIL;
    pList->node.previous = NIL;
}

/****************************************************************************
 *    Function:     lstAdd
 *
 *    Description:
 *                   Add to the end of the list
 *
 *    Returns:
 *                   None
 ****************************************************************************/
void
lstAdd(
    LIST *pList,
    NODE *pNode )
{
    if( pList->node.next == NIL )
    {
        /** This is the first element of the list **/
        pList->node.next = pNode;
        pList->node.previous = pNode;
        pNode->next = NIL;
        pNode->previous = NIL;
        pList->count = 1;
    }

    else
    {
        pNode->next = NIL;
        pNode->previous = pList->node.previous;
        pList->node.previous->next = pNode;
        pList->node.previous = pNode;
        pList->count++;
    }
}

/****************************************************************************
 *    Function:     lstCount
 *
 *    Description:
 *                   Count the elements in the list
 *
 *    Returns:
 *                   No. of Elements
 ****************************************************************************/
int
lstCount(
    LIST *pList )
{
    return( pList->count );
}

/****************************************************************************
 *    Function:     lstDelete
 *
 *    Description:
 *                   Delete a node from the list
 *
 *    Returns:
 *                   None
 ****************************************************************************/
void
lstDelete(
    LIST *pList,
    NODE *pNode )
{
    if( pNode->next == NIL )
    {
        /** Last in List **/
        pList->node.previous = pNode->previous;
    }

    else
    {
        pNode->next->previous = pNode->previous;
    }

    if( pNode->previous == NIL )
    {
        /** First in List **/
        pList->node.next = pNode->next;
    }

    else
    {
        pNode->previous->next = pNode->next;
    }

    pList->count--;
}

/****************************************************************************
 *    Function:     lstFirst
 *
 *    Description:
 *                   Return the first node in the list
 *
 *    Returns:
 *                   Node
 ****************************************************************************/
NODE *
lstFirst(
    LIST *pList )
{
    return( pList->node.next );
}


/****************************************************************************
 *    Function:     lstInsert
 *
 *    Description:
 *    Insert a node in the list
 *
 *    Returns:
 *                   None
 ****************************************************************************/
void
lstInsert(
    LIST *pList,
    NODE *pPrev,
    NODE *pNode )
{
    if( pList->count == 0 )
    {
        /** Empty List, treat the same a lstAdd **/
        lstAdd( pList, pNode );
        return;
    }

    if( pPrev == NIL )
    {
        pNode->next = pList->node.next;
        pNode->previous = NIL;
        pList->node.next->previous = pNode;
        pList->node.next = pNode;
    }

    else if( pPrev->next == NIL )
    {
        pNode->next = pPrev->next;
        pNode->previous = pPrev;
        pPrev->next = pNode;
        pList->node.previous = pNode;
    }

    else
    {
        pNode->next = pPrev->next;
        pNode->previous = pPrev;
        pPrev->next->previous = pNode;
        pPrev->next = pNode;
    }

    pList->count++;
}

/****************************************************************************
 *    Function:     lstLast
 *
 *    Description:
 *        Return the last node in the list
 *
 *    Returns:
 *                   Node
 ****************************************************************************/
NODE *
lstLast(
    LIST *pList )
{
    return( pList->node.previous );
}


/****************************************************************************
 *    Function:     lstNext
 *
 *    Description:
 *       Return the next node in the list
 *
 *    Returns:
 *                    Node
 ****************************************************************************/
NODE *
lstNext(
    NODE *pNode )
{
    return( pNode->next );
}

/****************************************************************************
 *    Function:     lstPrevious
 *
 *    Description:
 *       Return the previous node in the list
 *
 *    Returns:
 *                   Node
 ****************************************************************************/
NODE *
lstPrevious(
    NODE *pNode )
{
    return( pNode->previous );
}

/****************************************************************************
 *    Function:     lstGet
 *
 *    Description:
 *    Remove and return the first node in the list
 *
 *    Returns:
 *                   Node
 ****************************************************************************/
NODE *
lstGet(
    LIST *pList )
{
    NODE    *N;

    N = pList->node.next;

    if( N )
    {
        lstDelete( pList, N );
    }

    return( N );
}

/****************************************************************************
 *    Function:     lstNStep
 *
 *    Description:
 *        Stepover n nodes in the list
 *
 *    Returns:
 *                   Node
 ****************************************************************************/
NODE *
lstNStep(
    NODE *pNode,
    int  nStep )
{
    int    i;
    NODE   *p = pNode;

    if( nStep > 0 )
    {
        for( i = 0; i < nStep; i++ )
        {
            if( p->next )
            {
                p = p->next;
            }

            else
            {
                return NIL;
            }
        }
    }

    else if( nStep < 0 )
    {
        for( i = 0; i < ( -nStep ); i++ )
        {
            if( p->previous )
            {
                p = p->previous;
            }

            else
            {
                return NIL;
            }
        }
    }

    return ( p );
}

/****************************************************************************
 *    Function:     lstNth
 *
 *    Description:
 *    Return the nth node from the list
 *
 *    Returns:
 *                      Node
 ****************************************************************************/
NODE *
lstNth(
    LIST  *pList,
    int   nodenum )
{
    NODE    *p;

    p = lstFirst( pList );

    /** Assuming that numbering starts from 1 **/
    return( lstNStep( p, ( nodenum - 1 ) ) );
}

/****************************************************************************
 *    Function:     lstFind
 *
 *    Description:
 *    Find a given node in the list
 *
 *    Returns:
 *                   index of the node in the list.
 ****************************************************************************/
int
lstFind(
    LIST *pList,
    NODE *pNode )
{
    NODE   *p;
    int    i = 1;

    p = lstFirst( pList );

    for( ; ; i++, p = p->next )
    {
        if( p == pNode )
        {
            return i;
        }

        if( p == NIL )
        {
            return ( -1 );
        }
    }

}


/*NOTYET*/  extern  void lstConcat( LIST *pDstList, LIST *pAddList );
/*NOTYET*/
extern  void lstExtract( LIST *pSrcList,
                         NODE *pStartNode, NODE *pEndNode, LIST *pDstList );



