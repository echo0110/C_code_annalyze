/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_sdvl.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : SDVL: Self Described Variable Length  encoding
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_sdvl.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 15:51:52  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/



/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_typ.h"
#include "rohc_sdvl.h"
/* Coverity Fix 32371 start*/
#include "rohc_db_def.h"
/* Coverity Fix 32371 end*/
/* + SPR 17439 */
#include "rohc_ed_ext_if.h"
/* - SPR 17439 */
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/*****************************************************************************
 * Function Name  : rohc_c_calc_bytes_sdvl
 * Inputs         : Value to be encoded
 * Outputs        : 
 * Returns        : No of Octets
 * Description    : it gives the Octets used for sdvl encoding
 *****************************************************************************/
rohc_U8bit_t rohc_c_calc_bytes_sdvl
	ROHC_PARAMS((value),
	rohc_U32bit_t value)
{
    if(value <= 127) return 1;
    if(value <= 16383) return 2;
    if(value <= 2097151) return 3;
    if(value <= 536870911) return 4;
    return 5;
}

/*****************************************************************************
 * Function Name  : rohc_c_encode_sdvl 
 * Inputs         : encoded Octests, Value to be encoded
 * Outputs        : 
 * Returns        : No of Octets
 * Description    : it does the sdvl encoding
 *****************************************************************************/
rohc_U8bit_t rohc_c_encode_sdvl
	ROHC_PARAMS((dest,value),
	rohc_U8bit_t *dest _AND_
	rohc_U32bit_t value)
{
    rohc_U8bit_t tmp;
    tmp = rohc_c_calc_bytes_sdvl(value);
    if( tmp == 4 ){
        *dest++ = ((7 << 5) ^ ((value >> 24) & 31)) & 255;  /* 7 = bit pattern 111 */
        *dest++ = (value >> 16) & 255;
        *dest++ = (value >> 8) & 255;
        *dest = value & 255;
    } else if( tmp == 3 ){
        *dest++ = ((6 << 5) ^ ((value >> 16) & 31)) & 255;  /* 6 = bit pattern 110 */
        *dest++ = (value >> 8) & 255;
        *dest = value & 255;
    } else if( tmp == 2 ){
        *dest++ = ((2 << 6) ^ ((value >> 8)& 63)) & 255;    /* 2 = bit pattern 10 */
        *dest = value & 255;
    } else if( tmp == 1 ){
        *dest = value & 255;
    }
	return tmp;
}

/*****************************************************************************
 * Function Name  : rohc_d_sdvalue_decode
 * Inputs         : Octets to be decoded, no of Octets
 * Outputs        : 
 * Returns        : decoded value
 * Description    : it does the sdvl decoding
 *****************************************************************************/
rohc_U32bit_t rohc_d_sdvalue_decode
	ROHC_PARAMS((data,p_size),
	const rohc_U8bit_t *data _AND_
	rohc_U8bit_t *p_size)
{
    if(!ROHC_SDVL_GET_BIT_7( data )){                   /*bit  == 0 */
      		*p_size = 1; 
            return ROHC_SDVL_GET_BIT_0_6(data);
    }
    else if(ROHC_SDVL_GET_BIT_6_7(data) == (0x8 >> 2)){ /*bits == 10 */
      		*p_size = 2; 
            return  (ROHC_SDVL_GET_BIT_0_5(data) << 8 |  ROHC_SDVL_GET_BIT_0_7((data+1)));
    }
    else if(ROHC_SDVL_GET_BIT_5_7(data) == (0xc >> 1)){ /*bits == 110 */

      		*p_size = 3; 
            return (ROHC_SDVL_GET_BIT_0_4(data) << 16 |  ROHC_SDVL_GET_BIT_0_7((data+1)) << 8
               | ROHC_SDVL_GET_BIT_0_7((data+2)));
    }
    else if(ROHC_SDVL_GET_BIT_5_7(data) == (0xe >> 1)){ /*bits == 111 */

      		*p_size = 4; 
            return (ROHC_SDVL_GET_BIT_0_4(data) << 24 |  ROHC_SDVL_GET_BIT_0_7((data+1)) << 16
                       | ROHC_SDVL_GET_BIT_0_7((data+2)) << 8 | ROHC_SDVL_GET_BIT_0_7((data+3)));
    }
    else
        /*Coverity Fix 32371 start*/
        return ROHC_INVALID_CONTEXT_ID;  /*should not happen */
        /*Coverity Fix 32371 end*/
}

