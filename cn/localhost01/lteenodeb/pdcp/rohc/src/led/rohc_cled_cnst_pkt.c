/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cled_cnst_pkt.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for list
 *                     compression/decompression
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cled_cnst_pkt.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 16:14:06  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_c_typ.h"
#include "rohc_com_def.h"
#include "rohc_protocol_header.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_led_def.h"
#include "rohc_led_mcr.h"
#include "rohc_trace_mcr.h"
/* + SPR 17439 */
#include "rohc_led_proto.h"
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
**
**    FUNCTION:
**        rohc_c_enc_type_0_pkt
**
******************************************************************************
**
**    DESCRIPTION:
**         This function is used to construct an Encoding Type 0 packet
**           0   1   2   3   4   5   6   7
**         +---+---+---+---+---+---+---+---+
**         | ET=0  |GP |PS |    CC = m     |
**         +---+---+---+---+---+---+---+---+
**         :            gen_id             :  1 octet, if GP = 1
**         +---+---+---+---+---+---+---+---+
**         |        XI 1, ..., XI m        |  m octets, or m * 4 bits
**         /                --- --- --- ---/
**         |               :    Padding    :  if PS = 0 and m is odd
**         +---+---+---+---+---+---+---+---+
**         |                               |
**         /       item 1, ..., item n     /  variable
**         |                               |
**         +---+---+---+---+---+---+---+---+
**
**    ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.   
**
**    RETURN VALUE:
**        Total size
**    NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_0_pkt ROHC_PARAMS
    (
        (gen_id, GP_flag, PS_flag, count, XI_list,
		 	p_item_list, size,
            p_compressed_list),
                rohc_U8bit_t        gen_id _AND_
                rohc_U8bit_t        GP_flag _AND_
                rohc_U8bit_t        PS_flag _AND_
                rohc_U8bit_t        count _AND_
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                rohc_U8bit_t        *p_item_list _AND_
                rohc_U8bit_t        size _AND_
                rohc_U8bit_t        *p_compressed_list
    )
{
	/*ROHC_MAX_ENTRIES_IN_TT*/
    rohc_U32bit_t       i;
    rohc_U32bit_t       tot_size = 0;
    
    ROHC_ENTER_FN("rohc_c_enc_type_0_pkt");

    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, count);
    if(PS_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
    }

    if(GP_flag) 
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        p_compressed_list = p_compressed_list + 2;
        tot_size++;
    }
    else
        p_compressed_list = p_compressed_list + 1;
    tot_size++;
    if(PS_flag)
    {
        for(i=0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }   
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* PS = 0 */
    {
        if(count == 1)
        {
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[0]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
        else
        {
            for(i=1; i<count; i=i + 2)
            {
              /* SPR  3444 changes start */
                if(i < ROHC_MAX_ENTRIES_IN_TT)
                {
                   *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
                   *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                    p_compressed_list = p_compressed_list + 1;
                    tot_size++;
                }
            }
            if(((count % 2) != 0) &&(count < ROHC_MAX_ENTRIES_IN_TT)) /* need to set padding */
            {
              /* SPR  3444 changes end */
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
    }

    ROHC_MEMMOVE(p_compressed_list, p_item_list, size);
    
    tot_size = tot_size + size;

    ROHC_EXIT_FN("rohc_c_enc_type_0_pkt");
    return tot_size;
}

/*****************************************************************************
**
**      FUNCTION:
**        rohc_c_enc_type_1_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct Encoding Type 1 Packet
**               0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              | ET=1  |GP |PS |     XI 1      |
**              +---+---+---+---+---+---+---+---+
**              :            gen_id             :  1 octet, if GP = 1
**              +---+---+---+---+---+---+---+---+
**              |            ref_id             |
**              +---+---+---+---+---+---+---+---+
**              /      insertion bit mask       /  1-2 octets
**              +---+---+---+---+---+---+---+---+
**              |            XI list            |  k octets, or (k - 1) * 4 bits
**              /                --- --- --- ---/
**              |               :    Padding    :  if PS = 0 and k is even
**              +---+---+---+---+---+---+---+---+
**              |                               |
**              /       item 1, ..., item n     /  variable
**              |                               |
**              +---+---+---+---+---+---+---+---+
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.   
**
**      RETURN VALUE:
**        rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_1_pkt ROHC_PARAMS
    (
        (gen_id, ref_id, GP_flag, PS_flag, count, insertion_bit_mask, XI_list, p_item_list, size, p_compressed_list),
                rohc_U8bit_t        gen_id _AND_
                rohc_U8bit_t        ref_id  _AND_
                rohc_U8bit_t        GP_flag  _AND_
                rohc_U8bit_t        PS_flag _AND_
                rohc_U8bit_t        count _AND_
                rohc_U16bit_t       insertion_bit_mask _AND_
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                /* + SPR 17439 */
                rohc_U8bit_t        *p_item_list _AND_
                /* - SPR 17439 */
                rohc_U32bit_t       size _AND_
                rohc_U8bit_t        *p_compressed_list
    )
{
    rohc_U32bit_t   tot_size = 0, i;
    ROHC_ENTER_FN("rohc_c_enc_type_1_pkt");

    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 7);
    tot_size++;
    if(GP_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        tot_size++;
    }

    /* Set reference id */
    *(p_compressed_list + tot_size) = ref_id;
    tot_size++;

    /* Set insertion bit mask */
    if(insertion_bit_mask > 127)
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), insertion_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }
    else
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)insertion_bit_mask;
        tot_size++;
    }

    if(insertion_bit_mask == 0)
    {
        /* This implies list is unchanged */
        ROHC_EXIT_FN("rohc_c_enc_type_1_pkt");
        return tot_size;
    }

    if(PS_flag) /* Means XI_list is 8 bit */
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
        p_compressed_list = p_compressed_list + tot_size;
        for (i = 0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* XI_list is 4 bit */
    {
        *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[0]);
        p_compressed_list = p_compressed_list + tot_size;
        for(i = 2; i<count; i = i+2)
        {
              /* SPR  3444 changes start */
            if(i < ROHC_MAX_ENTRIES_IN_TT)
            {
               *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
               *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
        if(((count % 2) == 0) && (count != 1) && (count > 0) && 
            (count < ROHC_MAX_ENTRIES_IN_TT))/* need to set padding */
        {
              /* SPR  3444 changes end */
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count - 1]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
    }
    
    ROHC_MEMMOVE(p_compressed_list, p_item_list, size);

    tot_size = tot_size + size;

    ROHC_EXIT_FN("rohc_c_enc_type_1_pkt");
    return tot_size;
}

/*****************************************************************************
**
**      FUNCTION: 
**        rohc_c_enc_type_2_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct Encoding Type 2 Packet
**              0   1   2   3   4   5   6   7
**            +---+---+---+---+---+---+---+---+
**            | ET=2  |GP |res|     Count     |
**            +---+---+---+---+---+---+---+---+
**            :            gen_id             :  1 octet, if GP = 1
**            +---+---+---+---+---+---+---+---+
**            |            ref_id             |
**            +---+---+---+---+---+---+---+---+
**            /       removal bit mask        /  1-2 octets
**            +---+---+---+---+---+---+---+---+
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_2_pkt  ROHC_PARAMS
    (
        (gen_id, ref_id, GP_flag, count, removal_bit_mask, p_compressed_list),
                rohc_U8bit_t        gen_id _AND_
                rohc_U8bit_t        ref_id _AND_
                rohc_U8bit_t        GP_flag _AND_
                rohc_U8bit_t        count _AND_
                rohc_U16bit_t       removal_bit_mask _AND_
                rohc_U8bit_t        *p_compressed_list
    )
{
    rohc_U32bit_t   tot_size = 0;
    ROHC_ENTER_FN("rohc_c_enc_type_2_pkt");

    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 8);
    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, count);
    tot_size++;

    if (GP_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        tot_size++;
    }
    *(p_compressed_list + tot_size) = ref_id;
    tot_size++;

    /* Set removal bit mask */
    if( (removal_bit_mask >> 8) == 0xFF)
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)removal_bit_mask;
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_UNSET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size++;
    }
    else
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), removal_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }

    ROHC_EXIT_FN("rohc_c_enc_type_2_pkt");
    return tot_size;
}

/*****************************************************************************
**
**      FUNCTION:
**        rohc_c_enc_type_3_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct Encoding Type 3 Packet
**              0   1   2   3   4   5   6   7
**            +---+---+---+---+---+---+---+---+
**            | ET=3  |GP |PS |     XI 1      |
**            +---+---+---+---+---+---+---+---+
**            :            gen_id             :  1 octet, if GP = 1
**            +---+---+---+---+---+---+---+---+
**            |            ref_id             |
**            +---+---+---+---+---+---+---+---+
**            /       removal bit mask        /  1-2 octets
**            +---+---+---+---+---+---+---+---+
**            /      insertion bit mask       /  1-2 octets
**            +---+---+---+---+---+---+---+---+
**            |            XI list            |  k octets, or (k - 1) * 4 bits
**            /                --- --- --- ---/
**            |               :    Padding    :  if PS = 0 and k is even
**            +---+---+---+---+---+---+---+---+
**            |                               |
**            /       item 1, ..., item n     /  variable
**            |                               |
**            +---+---+---+---+---+---+---+---+
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.
**      RETURN VALUE:
**        rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_3_pkt ROHC_PARAMS
    (
        (gen_id, ref_id, GP_flag, PS_flag, count, removal_bit_mask, 
            insertion_bit_mask, XI_list, p_item_list, size, 
            p_compressed_list),
                rohc_U8bit_t      gen_id _AND_
                rohc_U8bit_t      ref_id _AND_
                rohc_U8bit_t      GP_flag _AND_
                rohc_U8bit_t      PS_flag _AND_
                rohc_U8bit_t      count _AND_
                rohc_U16bit_t     removal_bit_mask _AND_
                rohc_U16bit_t     insertion_bit_mask _AND_
                rohc_U8bit_t      XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                /* + SPR 17439 */
                rohc_U8bit_t      *p_item_list _AND_
                /* - SPR 17439 */
                rohc_U32bit_t     size _AND_
                rohc_U8bit_t      *p_compressed_list
    )
{
    rohc_U32bit_t   tot_size = 0, i;
    ROHC_ENTER_FN("rohc_c_enc_type_3_pkt");

    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 7);
    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 8);
    tot_size++;

    if(GP_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        tot_size++;
    }

    /* Set reference id */
    *(p_compressed_list + tot_size) = ref_id;
    tot_size++;

    /* Set removal bit mask */
    if( (removal_bit_mask >> 8) == 0xFF)
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)removal_bit_mask;
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_UNSET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size++;
    }
    else
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), removal_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }

    /* Set Insertion bit mask */
    if(insertion_bit_mask > 127)
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), insertion_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }
    else
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)insertion_bit_mask;
        tot_size++;
    }

    if(PS_flag) /* Means XI_list is 8 bit */
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
        p_compressed_list = p_compressed_list + tot_size;
        for (i = 0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* XI_list is 4 bit */
    {
        *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[0]);
        p_compressed_list = p_compressed_list + tot_size;
        for(i = 2; i<count; i = i+2)
        {
              /* SPR  3444 changes start */
            if(i < ROHC_MAX_ENTRIES_IN_TT)
            {
                *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
        if(((count % 2) == 0) && (count != 1) && (count > 0) && 
            (count < ROHC_MAX_ENTRIES_IN_TT))/* need to set padding */
        {
              /* SPR  3444 changes end */
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count-1]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
    }

    ROHC_MEMMOVE(p_compressed_list, p_item_list, size);

    tot_size = tot_size + size;

    ROHC_EXIT_FN("rohc_c_enc_type_3_pkt");
    return tot_size;
}   

/*****************************************************************************
**
**      FUNCTION:
**        rohc_c_cnst_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct the final Packet
**           0     1     2     3     4     5     6     7
**        +-----+-----+-----+-----+-----+-----+-----+-----+
**        | CL  | ASeq| ESeq| Gseq|          res          |  1 octet
**        +-----+-----+-----+-----+-----+-----+-----+-----+
**        :    compressed AH Seq Number,  1 or 4 octets   :  if ASeq = 1
**         ----- ----- ----- ----- ----- ----- ----- -----
**        :    compressed ESP Seq Number, 1 or 4 octets   :  if Eseq = 1
**         ----- ----- ----- ----- ----- ----- ----- -----
**        :    compressed GRE Seq Number, 1 or 4 octets   :  if Gseq = 1
**         ----- ----- ----- ----- ----- ----- ----- -----
**        :    compressed header list, variable length    :  if CL = 1
**         ----- ----- ----- ----- ----- ----- ----- -----
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.
**      RETURN VALUE:
**        rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_cnst_pkt ROHC_PARAMS
    (
        (aseq_no, eseq_no, gseq_no, p_compressed_list, tot_size, p_final_list),
                rohc_U32bit_t       aseq_no _AND_
                rohc_U32bit_t       eseq_no _AND_
                rohc_U32bit_t       gseq_no _AND_
                rohc_U8bit_t        *p_compressed_list _AND_
                rohc_U32bit_t       tot_size _AND_
                rohc_U8bit_t        *p_final_list
    )
{
    rohc_U32bit_t   length = 1;
    ROHC_ENTER_FN("rohc_c_cnst_pkt");

    if(aseq_no != 0)
    {
        *p_final_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_final_list, ROHC_ASEQ_BIT);
        if(aseq_no > 127)
        {
            ROHC_SET_32BIT((p_final_list + 1), aseq_no);
            length = length + 4;
        }
        else
        {
            *(p_final_list + 1) = aseq_no;
            length++;
        }
    }
    if(eseq_no != 0)
    {
        *p_final_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_final_list, ROHC_ESEQ_BIT);
        if (eseq_no > 127)
        {
            ROHC_SET_32BIT((p_final_list + length), eseq_no);
            length = length + 4;
        }
        else
        {
            *(p_final_list + length) = eseq_no;
            length++;
        }
    }
    if(gseq_no != 0)
    {
        *p_final_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_final_list, ROHC_GSEQ_BIT);
        if (gseq_no > 127)
        {
            ROHC_SET_32BIT((p_final_list + length), gseq_no);
            length = length + 4;
        }
        else
        {
            *(p_final_list + length) = gseq_no;
            length++;
        }
    }

    *p_final_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_final_list, ROHC_CL_BIT);
    ROHC_MEMMOVE(p_final_list + length, p_compressed_list, tot_size);

    length = length + tot_size;

    ROHC_EXIT_FN("rohc_c_cnst_pkt");
    return length;
}

/*****************************************************************************
**
**    FUNCTION:
**        rohc_c_enc_type_0_csrc_pkt
**
******************************************************************************
**
**    DESCRIPTION:
**         This function is used to construct an Encoding Type 0 packet
**           0   1   2   3   4   5   6   7
**         +---+---+---+---+---+---+---+---+
**         | ET=0  |GP |PS |    CC = m     |
**         +---+---+---+---+---+---+---+---+
**         :            gen_id             :  1 octet, if GP = 1
**         +---+---+---+---+---+---+---+---+
**         |        XI 1, ..., XI m        |  m octets, or m * 4 bits
**         /                --- --- --- ---/
**         |               :    Padding    :  if PS = 0 and m is odd
**         +---+---+---+---+---+---+---+---+
**         |                               |
**         /       item 1, ..., item n     /  variable
**         |                               |
**         +---+---+---+---+---+---+---+---+
**
**    ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.   
**
**    RETURN VALUE:
**        rohc_return_t
**    NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_0_csrc_pkt ROHC_PARAMS
    (
        (gen_id, GP_flag, PS_flag, count, XI_list, item_list, size,
            p_compressed_list),
                rohc_U8bit_t        gen_id _AND_
                rohc_U8bit_t        GP_flag _AND_
                rohc_U8bit_t        PS_flag _AND_
                rohc_U8bit_t        count _AND_
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U32bit_t       item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U8bit_t        size _AND_
                rohc_U8bit_t        *p_compressed_list
    )
{
    rohc_U32bit_t       i;
    rohc_U32bit_t       tot_size = 0;
    
    ROHC_ENTER_FN("rohc_c_enc_type_0_csrc_pkt");

    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, count);
    if(PS_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
    }

    if(GP_flag) 
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        p_compressed_list = p_compressed_list + 2;
        tot_size++;
    }
    else
        p_compressed_list = p_compressed_list + 1;
    tot_size++;
    if(PS_flag)
    {
        for(i=0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }   
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* PS = 0 */
    {
        if(count == 1)
        {
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[0]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
        else
        {
            for(i=1; i<count; i=i + 2)
            {
              /* SPR  3444 changes start */
                if(i < ROHC_MAX_ENTRIES_IN_TT)
                {
                    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
                    *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                    p_compressed_list = p_compressed_list + 1;
                    tot_size++;
                }
            }
            if(((count % 2) != 0)  && (count < ROHC_MAX_ENTRIES_IN_TT))/* need to set padding */
            {
              /* SPR  3444 changes end */
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
    }

    ROHC_MEMCPY(p_compressed_list, item_list, 4 * size);
    tot_size = tot_size + (4 * size);

    ROHC_EXIT_FN("rohc_c_enc_type_0_csrc_pkt");
    return tot_size;
}

/*****************************************************************************
**
**      FUNCTION:
**        rohc_c_enc_type_1_csrc_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct Encoding Type 1 Packet
**               0   1   2   3   4   5   6   7
**              +---+---+---+---+---+---+---+---+
**              | ET=1  |GP |PS |     XI 1      |
**              +---+---+---+---+---+---+---+---+
**              :            gen_id             :  1 octet, if GP = 1
**              +---+---+---+---+---+---+---+---+
**              |            ref_id             |
**              +---+---+---+---+---+---+---+---+
**              /      insertion bit mask       /  1-2 octets
**              +---+---+---+---+---+---+---+---+
**              |            XI list            |  k octets, or (k - 1) * 4 bits
**              /                --- --- --- ---/
**              |               :    Padding    :  if PS = 0 and k is even
**              +---+---+---+---+---+---+---+---+
**              |                               |
**              /       item 1, ..., item n     /  variable
**              |                               |
**              +---+---+---+---+---+---+---+---+
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.   
**
**      RETURN VALUE:
**        rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_1_csrc_pkt ROHC_PARAMS
    (
        (gen_id, ref_id, GP_flag, PS_flag, count, insertion_bit_mask, XI_list, item_list, size,  p_compressed_list),
                rohc_U8bit_t        gen_id _AND_
                rohc_U8bit_t        ref_id  _AND_
                rohc_U8bit_t        GP_flag  _AND_
                rohc_U8bit_t        PS_flag _AND_
                rohc_U8bit_t        count _AND_
                rohc_U16bit_t       insertion_bit_mask _AND_
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U32bit_t       item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U32bit_t       size _AND_
                rohc_U8bit_t        *p_compressed_list
    )
{
    rohc_U32bit_t   tot_size = 0, i;
    ROHC_ENTER_FN("rohc_c_enc_type_1_csrc_pkt");

    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 7);
    tot_size++;
    if(GP_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        tot_size++;
    }

    /* Set reference id */
    *(p_compressed_list + tot_size) = ref_id;
    tot_size++;

    /* Set insertion bit mask */
    if(insertion_bit_mask > 127)
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), insertion_bit_mask);
        *(p_compressed_list + tot_size) = 
                (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }
    else
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)insertion_bit_mask;
        tot_size++;
    }

    if(PS_flag) /* Means XI_list is 8 bit */
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
        p_compressed_list = p_compressed_list + tot_size;
        for (i = 0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* XI_list is 4 bit */
    {
        *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[0]);
        p_compressed_list = p_compressed_list + tot_size;
        for(i = 2; i<count; i = i+2)
        {
              /* SPR  3444 changes start */
            if(i < ROHC_MAX_ENTRIES_IN_TT)
            {
                *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
        if(((count % 2) == 0) && (count != 1) && (count < ROHC_MAX_ENTRIES_IN_TT))/* need to set padding */
        {
              /* SPR  3444 changes end */
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
    }
    /* Coverity Fix 24559 Start*/
    if(item_list != ROHC_NULL)
    {
        ROHC_MEMCPY(p_compressed_list, item_list, 4 * size);
    }
    /* Coverity Fix 24559 End*/
    tot_size = tot_size + (4 * size);

    ROHC_EXIT_FN("rohc_c_enc_type_1_csrc_pkt");
    return tot_size;
}

/*****************************************************************************
**
**      FUNCTION:
**        rohc_c_enc_type_3_csrc_pkt
**
******************************************************************************
**
**      DESCRIPTION:
**         This function is used to construct Encoding Type 3 Packet
**              0   1   2   3   4   5   6   7
**            +---+---+---+---+---+---+---+---+
**            | ET=3  |GP |PS |     XI 1      |
**            +---+---+---+---+---+---+---+---+
**            :            gen_id             :  1 octet, if GP = 1
**            +---+---+---+---+---+---+---+---+
**            |            ref_id             |
**            +---+---+---+---+---+---+---+---+
**            /       removal bit mask        /  1-2 octets
**            +---+---+---+---+---+---+---+---+
**            /      insertion bit mask       /  1-2 octets
**            +---+---+---+---+---+---+---+---+
**            |            XI list            |  k octets, or (k - 1) * 4 bits
**            /                --- --- --- ---/
**            |               :    Padding    :  if PS = 0 and k is even
**            +---+---+---+---+---+---+---+---+
**            |                               |
**            /       item 1, ..., item n     /  variable
**            |                               |
**            +---+---+---+---+---+---+---+---+
**
**      ARGUMENTS:
**          All the fields required to construct the packet are taken
**          as input.
**      RETURN VALUE:
**        rohc_return_t
**      NOTES:
**
*****************************************************************************/

rohc_U32bit_t   rohc_c_enc_type_3_csrc_pkt ROHC_PARAMS
    (
        (gen_id, ref_id, GP_flag, PS_flag, count, removal_bit_mask, 
            insertion_bit_mask, XI_list, item_list, size, 
            p_compressed_list),
                rohc_U8bit_t      gen_id _AND_
                rohc_U8bit_t      ref_id _AND_
                rohc_U8bit_t      GP_flag _AND_
                rohc_U8bit_t      PS_flag _AND_
                rohc_U8bit_t      count _AND_
                rohc_U16bit_t     removal_bit_mask _AND_
                rohc_U16bit_t     insertion_bit_mask _AND_
                rohc_U8bit_t      XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U32bit_t      item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] _AND_
                rohc_U32bit_t     size _AND_
                rohc_U8bit_t      *p_compressed_list
    )
{
    rohc_U32bit_t   tot_size = 0, i;
    ROHC_ENTER_FN("rohc_c_enc_type_3_csrc_pkt");

    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 7);
    *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 8);
    tot_size++;

    if(GP_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_GP_BIT);
        *(p_compressed_list + 1) = gen_id;
        tot_size++;
    }

    /* Set reference id */
    *(p_compressed_list + tot_size) = ref_id;
    tot_size++;

    /* Set removal bit mask */
    if((removal_bit_mask >> 8) == 0xFF)
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)removal_bit_mask;
        *(p_compressed_list + tot_size) =
            (rohc_U8bit_t)ROHC_UNSET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size++;
    }
    else
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size), removal_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }

    /* Set insertion bit mask */
    if(insertion_bit_mask > 127)
    {
        ROHC_SET_16BIT((p_compressed_list + tot_size),insertion_bit_mask);
        *(p_compressed_list + tot_size) = 
            (rohc_U8bit_t)ROHC_SET_BIT(*(p_compressed_list + tot_size), 8);
        tot_size = tot_size + 2;
    }
    else
    {
        *(p_compressed_list + tot_size) = (rohc_U8bit_t)insertion_bit_mask;
        tot_size++;
    }

    if(PS_flag) /* Means XI_list is 8 bit */
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, ROHC_PS_BIT);
        p_compressed_list = p_compressed_list + tot_size;
        for (i = 0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* XI_list is 4 bit */
    {
        *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[0]);
        p_compressed_list = p_compressed_list + tot_size;
        for(i = 2; i<count; i = i+2)
        {
            /* SPR  3444 changes start */
            if(i < ROHC_MAX_ENTRIES_IN_TT)
            {

                *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, XI_list[i]);
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[i-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
        if(((count % 2) == 0) && (count != 1) && (count > 0) && 
                (count < ROHC_MAX_ENTRIES_IN_TT)) /* need to set padding */
        {
            /* SPR  3444 changes end */
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, XI_list[count-1]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
    }
    /* Klocwork Fix */
    if(size > 0 && size <= ROHC_MAX_ENTRIES_IN_CSRC_TT)
    {
        ROHC_MEMCPY(p_compressed_list, item_list, (4 * size));
    }
    tot_size = tot_size + (4 * size);

    ROHC_EXIT_FN("rohc_c_enc_type_3_csrc_pkt");
    return tot_size;
}   
