/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmArithmetic.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file describes arithmetic operations for AM entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcAmArithmetic.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcAmArithmetic.h"


/****************************************************************************
 * Function Name  : isWithInTxWindow
 * Inputs         : vtA,
 *                  vtSN,
 *                  vtMS
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : T.S Specs 36.322 5.1.3.1.1
 *                  The transmitting side of an AM RLC entity shall maintain 
 *                  a transmitting window according to state variables VT(A) 
 *                  and VT(MS) as follows:
 *                 a SN falls within the transmitting window if VT(A) <= SN < VT(MS);
 *                  a SN falls outside of the transmitting window otherwise.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isWithInTxWindow( UInt16 vtA, UInt16 vtSN, UInt16 vtMS)
/* SPR 5599_129119 Fix End */
{
  
    UInt16 A  = 0;
    UInt16 SN = (vtSN - vtA) & 1023;
    UInt16 MS = (vtMS - vtA) & 1023;
    /*  a SN falls within the transmitting window if VT(A) <= SN < VT(MS); */
    if ( ( A <= SN ) && ( SN < MS ) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}    




/****************************************************************************
 * Function Name  : isInRxWindow
 * Inputs         : vrUR,
 *                  rxSN,
 *                  vrMR
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This Funtion checks is with in RX window or outside 
 ****************************************************************************/

/* SPR 5599_129119 Fix Start */
int isWithInRxWindow( UInt16 vrUR,
/* SPR 5599_129119 Fix End */
                                UInt16 rxSN,
                                UInt16 vrMR   
                              )
{
/*
VT(A) and VR(R) shall be assumed as the modulus base 
at the transmitting side and receiving side of an AM RLC entity, respectively.
This modulus base is subtracted from all the values involved, 
and then an absolute comparison is performed 
*/
   /* Coverity CID 54582 Start */
    /* + SPR 17439 */
   UInt32 SN = (rxSN - vrUR) & 1023;
   /* - SPR 17439 */
   /* Coverity CID 54582 End */
   UInt32 MR = (vrMR - vrUR ) & 1023;

   /* SPR 21037 start  */ 
   UInt32 A = 0;
   /* SPR 21037 end    */

/*a SN falls within the receiving window if VR(R) <= SN < VR(MR);*/
   /* + SPR 17439 */
   /* SPR 21037 start */
   if ( ( A <= SN ) && ( SN < MR ) )
   /* SPR 21037 end   */ 
   /* - SPR 17439 */    
   {
       return 1;
   }
   return 0;
}

/****************************************************************************
 * Function Name  : isAMEqual
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt32 
 * Description    : This function checks is x and y equal 
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isAMEqual( UInt16 x,
/* SPR 5599_129119 Fix End */
                  UInt16 y,
                  UInt16 modBase
                )
{
  
  return ( (( x - modBase) & 1023) == (( y - modBase ) & 1023) );

}

/****************************************************************************
 * Function Name  : isAMGT
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt32 
 * Description    : This function returns the result for x >y
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isAMGT ( UInt16 x,
/* SPR 5599_129119 Fix End */
                UInt16 y,
                UInt16 modBase
              )
{
   return ( (( x - modBase) & 1023 ) > ( ( y - modBase ) & 1023) );
}

/****************************************************************************
 * Function Name  : isAMLT
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt32 
 * Description    : This function returns the result for x<y
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isAMLT ( UInt16 x,
/* SPR 5599_129119 Fix End */
                UInt16 y,
                UInt16 modBase
              )
{
   return ( (( x - modBase) & 1023 ) < ( ( y - modBase ) & 1023) );
}

/****************************************************************************
 * Function Name  : isAMLTE
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt32 
 * Description    : This function returns the result for x<=y
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isAMLTE ( UInt16 x,
/* SPR 5599_129119 Fix End */
                 UInt16 y,
                 UInt16 modBase
               )
{
   return ( (( x - modBase) & 1023 ) <= ( ( y - modBase ) & 1023) );
}

/****************************************************************************
 * Function Name  : isAMGTE
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt32 
 * Description    : This function returns the result for x>=y
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 isAMGTE ( UInt16 x,
/* SPR 5599_129119 Fix End */
                 UInt16 y,
                 UInt16 modBase
               )
{
   return ( (( x - modBase) & 1023 ) >= ( ( y - modBase ) & 1023) );
}
/** RLC Nack List Changes Start **/
/****************************************************************************
 * Function Name  : getNackListSize
 * Inputs         : x- represents UH,
 *                  y - Represents UR,
 *                  modBase
 * Outputs        : None
 * Returns        : UInt16
 * Description    : This function get the nack list size
 ****************************************************************************/
UInt16 getNackListSize ( UInt16 x,
                UInt16 y)
{
   return (( y - x) & 1023 );
}
/** RLC Nack List Changes End **/
