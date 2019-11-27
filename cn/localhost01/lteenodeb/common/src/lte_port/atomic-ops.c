/****************************************************************************/
/**  Copyright (C) 2011 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Architecture dependent atomic operations implementation                **/
/**                                                                        **/
/****************************************************************************/

#include "atomic-ops.h"
#include "lteMisc.h"

#ifdef __PPC__
/* definitions for powerpc */

int atomicCompareAndSwapInt(unsigned int *ptr, unsigned int origVal,
                         unsigned int newVal)
{
    unsigned int val;
    volatile int fail = 0;

    if (fail)
    {
        /* label for returning failure, if stwcx fails.
         * This code will be hit only if stwcx fails */
        __asm__ volatile ("casfail:");
        return ATOMIC_FAILURE;
    }

    /* Read val from pointer, and store context */
    lwarx(val, *ptr);

    /* If the value read, is not the same as orig, return failure */
    if (val != origVal)
    {
        return ATOMIC_FAILURE;
    }

    /* Save the new value, if value in pointer has not changed */
    stwcx(ptr, newVal);

    /* If stwcx failed, goto casfail and return failure */
    __asm__ volatile("bne- casfail");

    /* If stwcx was successful, return success */
    return ATOMIC_SUCCESS;
}

int atomicCompareAndSwapPtr(void **ptr, void *origVal,
                         void *newVal)
{
    void *val;
    volatile int fail = 0;

    if (fail)
    {
        /* label for returning failure, if stwcx fails.
         * This code will be hit only if stwcx fails */
        __asm__ volatile ("casfailptr:");
        return ATOMIC_FAILURE;
    }

    /* Read val from pointer, and store context */
    lwarx(val, *ptr);

    /* If the value read, is not the same as orig, return failure */
    if (val != origVal)
    {
        return ATOMIC_FAILURE;
    }

    /* Save the new value, if value in pointer has not changed */
    stwcx(ptr, newVal);

    /* If stwcx failed, goto casfail and return failure */
    __asm__ volatile("bne- casfailptr");

    /* If stwcx was successful, return success */
    return ATOMIC_SUCCESS;
}

void atomicIncrement(unsigned int *ptr)
{
    unsigned int val;

    /* if stwcx fails, we will redo the procedure - read value, increment,
     * store */
    __asm__ volatile ("atincfail:");

    /* Read val from pointer, and store context */
    lwarx(val, *ptr);

    /* Increment value */
    val++;

    /* Save the new value, if value in pointer has not changed */
    stwcx(ptr, val);

    /* If stwcx failed, reread value and decrement */
    __asm__ volatile("bne- atincfail");
}

void atomicDecrement(unsigned int *ptr)
{
    unsigned int val;

    /* if stwcx fails, we will redo the procedure - read value, decrement,
     * store */
    __asm__ volatile ("atdecfail:");

    /* Read val from pointer, and store context */
    lwarx(val, *ptr);

    /* Increment value */
    val--;

    /* Save the new value, if value in pointer has not changed */
    stwcx(ptr, val);

    /* If stwcx failed, reread value and decrement */
    __asm__ volatile("bne- atdecfail");
}

#else
void atomicIncrement(unsigned int *ptr)
{
    FETCH_AND_ADD(ptr, 1);
}

void atomicDecrement(unsigned int *ptr)
{
    FETCH_AND_SUB(ptr, 1);
}
//#error "Code not yet done for non PPC architectures"
#endif /* __PPC__ - powerpc */
void atomicAddInt(unsigned int *orig, unsigned int addedVal)
{
    FETCH_AND_ADD(orig, addedVal);
}

void atomicSubtractInt(unsigned int *orig, unsigned int subtractedVal)
{
    FETCH_AND_SUB(orig, subtractedVal);
}

