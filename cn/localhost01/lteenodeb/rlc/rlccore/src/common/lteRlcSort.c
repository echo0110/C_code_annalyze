/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcSort.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:13 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcSort.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:13  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:21  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.28.2  2010/07/20 05:47:46  gur19836
 * Code Review Comment: Indentation and 80 column width: Incorporated
 *
 * Revision 1.2.28.1  2010/07/17 11:43:53  gur23054
 * Review Comments fixed
 *
 * Revision 1.2  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcSort.h"
#include "lteRlcPdcpInterface.h" 
#include "lteRlcErrorCode.h"

/****************************************************************************
 * Function Name  : rlcSwap
 * Inputs         : aL - pointer of type UInt32,
 *                  aR - pointer of type UInt32
 * Outputs        : Swapping of two UInt32
 * Returns        : None
 * Description    : This API is used to swap two values. 
 ****************************************************************************/
void rlcSwap(UInt32 *aL,UInt32 *aR)
{
    UInt32 temp = *aL ;
    *aL = *aR ;
    *aR = temp ;
}

/****************************************************************************
 * Function Name  : rlcPartition
 * Inputs         : a - pointer of UInt32,
 *                  low - lower bound of array,
 *                  high - upper bound of array
 * Outputs        : None
 * Returns        : right
 * Description    : This API is used to create the partitions.
 ****************************************************************************/

UInt32 rlcPartition( UInt32 *a, UInt32 low, UInt32 high )
{
    UInt32 left, right;
    UInt32 pivot_item;
    pivot_item = a[low];
    left = low;
    right = high;
    while ( left < right ) 
    {
        /* Move left while item < pivot */
        while( a[left] <= pivot_item ) 
        {
            left++;
        }
        /* Move right while item > pivot */
        while( a[right] > pivot_item ) 
        {
            right--;
        }
        if ( left < right ) 
        {
            rlcSwap(&a[left],&a[right]);
        }
    }
    /* right is final position for the pivot */
    a[low] = a[right];
    a[right] = pivot_item;
    return right;
}

/****************************************************************************
 * Function Name  : rlcQuicksort
 * Inputs         : a - pointer of UInt32 array
 *                  low - lower bound of array,
 *                  high - upper bound of array
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to sort data of array a using quick sort.
 ****************************************************************************/
void rlcQuicksort( UInt32 *a, UInt32 low, UInt32 high )
{
    UInt32 pivot;
    /* Termination condition! */
    if ( high > low )
    {
        pivot = rlcPartition( a, low, high );
        rlcQuicksort( a, low, pivot-1 );
        rlcQuicksort( a, pivot+1, high );
    }
}

/****************************************************************************
 * Function Name  : rlcHeapsort
 * Inputs         : arr - pointer of UInt32 array,
 *                  arr_num_items - number of items in array
 * Outputs        : None
 * Returns        : None
 * Description    : Heap sort
 ****************************************************************************/
void rlcHeapsort(UInt32 *arr,UInt32 arr_num_items)
{
    UInt32 i;
    //call rlcFnSortHeap function for (arr_num_items - 2) times.
    for(i=arr_num_items; i>1; i--)
    {
        rlcFnSortHeap(arr, i - 1);
    }
    return;
}

/****************************************************************************
 * Function Name  : rlcFnSortHeap
 * Inputs         : arr - array input of UInt32,
 *                  arr_ubound - upper bound 
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create heap and sort.
 ****************************************************************************/
void rlcFnSortHeap(UInt32 *arr, UInt32 arr_ubound)
{
    UInt32 i,o;
    UInt32 lChild, rChild, mChild, root, temp;
    /*find the root element of the current element */
    root = DIVIDE_BY_TWO(arr_ubound-1);
    /* creating the heap */
    for(o=root;o>0;o--)
    {
        for(i=root;i>0;i--)
        {
            lChild = (2*i)+1;
            rChild = (2*i)+2;
            if ((lChild <= arr_ubound) && (rChild <= arr_ubound))
            {
                if(arr[rChild] >= arr[lChild])
                    mChild = rChild;
                else
                    mChild = lChild;
            }
            else
            {
                if(rChild > arr_ubound)
                    mChild = lChild;
                else
                    mChild = rChild;
            }
            if (arr[i] < arr[mChild])
            {
                rlcSwap(&arr[i],&arr[mChild]);
            }
        }
    }
    /* move the max element to the end of the array */
    temp = arr[0];
    arr[0] = arr[arr_ubound];
    arr[arr_ubound] = temp;
    return;
}

/****************************************************************************
 * Function Name  : rlcCompare
 * Inputs         : arg - arg to be copared to,
 *                  data - arg to be compared  
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to compare two arguments.
 ****************************************************************************/
UInt32 rlcCompare( void *arg, void *data)
{
    /* SPR 2812 changes start */
    if( ((RlcUeDataReq *)data)->transId == arg )
    {
        return RLC_SUCCESS;
    }
    else
        return RLC_FAILURE;
}
