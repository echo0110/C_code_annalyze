/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_asn_gen.c,v 1.4 2010/10/13 07:04:18 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_asn_gen.c,v $
 * Revision 1.4  2010/10/13 07:04:18  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.1  2010/09/09 10:08:11  gur22047
 * modified during s1ap UT
 *
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.7  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.6  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.5  2009/12/19 09:37:19  gur18569
 * changes during ssit
 *
 * Revision 1.1.2.4  2009/12/08 14:13:13  gur18569
 * using message id from ASN files SPR:161
 *
 * Revision 1.1.2.3  2009/12/04 13:55:16  gur18569
 * chng
 *
 * Revision 1.1.2.2  2009/11/30 08:52:49  gur18569
 * added asn msg to fwd
 *
 * Revision 1.1.2.1  2009/11/25 13:11:04  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.4  2009/11/22 11:49:38  gur18569
 * added more buff types
 *
 * Revision 1.1.2.3  2009/11/10 11:41:55  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.2  2009/10/29 06:19:24  gur18569
 * added template
 *
 *
 *****************************************************************************/



#include "s1ap_asn_codec.h"

#define MAX_CHARS_PER_LINE   12

U8 ue_id_pair;

U8* s1ap_EP_test_str[INVALID_S1AP_EP] =
{
    "S1_SETUP_REQUEST",
    "S1_SETUP_RESPONSE",
    "S1_SETUP_FAILURE",
    "PAGING",
    "RESET",
    "RESET_ACK",
    "INITIAL_CONTEXT_SETUP_REQUEST",
    "DL_NAS_TRANSPORT",
    "UL_NAS_TRANSPORT",
    "ASN_MSG_TO_FORWARD"
};



/*****************************************************************************
 * Function Name  : dump_asn_msg 
 * Inputs         : p_asn_msg - pointer to ASN message 
 *                  asn_msg_len - length of ASN message
 * Outputs        : None
 * Returns        : None
 * Description    : This function dumps ASN msg
 ********************************************************************************/
void dump_asn_msg(U8 * p_asn_msg, U32 asn_msg_len)
{
    U32 index = 0;
    U32 max_chars_per_line = 0;

    printf_wrapper("\n\n======================= Asn msg start len %d ================\n\n",asn_msg_len);
    for (index = 0; index < asn_msg_len; index++)
    {
        if (max_chars_per_line++ == MAX_CHARS_PER_LINE)
        {
            fprintf_wrapper(stdout,"\n");
            max_chars_per_line = 0;
        }
        if (*p_asn_msg <= 15)
        {
            fprintf_wrapper(stdout,"0%x ",*p_asn_msg++);
        }
        else
        {
            fprintf_wrapper(stdout,"%x ",*p_asn_msg++);
        }
    }
    printf_wrapper("\n\n======================= Asn msg end ================\n\n");
}

enum {
    /* All fields are present */
    S1_SETUP_REQ_TEST1 =1,
    /* Optional fields are absent */
    S1_SETUP_REQ_TEST2,
    /* Num of Supported TAs == 0 */
    S1_SETUP_REQ_TEST3,
    /* Num of Supporte TAs == 1 but it's num broadcast PLMNs == 0 */
    S1_SETUP_REQ_TEST4,
};

/*****************************************************************************
 * Function Name  : set_s1_setup_req 
 * Inputs         : p_s1_setup_req - pointer to S1 setup requst  
 * Outputs        : p_s1_setup_req
 * Returns        : None
 * Description    : This function sets the s1 setup requset and fills in to 
 *                  p_s1_setup_req
 ********************************************************************************/
void set_s1_setup_req(s1_setup_req_t * p_s1_setup_req)
{
    U8 enb_name_buff[MAX_ENB_NAME_STR_SIZE] =  "TEST:MY STACK ENB";
    U32 plmnId = 13;
    U32 macro_enb_id = 14;
    U32 csg_id_arr[MAX_CSG_ID_LIST] = {0};
    U16 tac =15;
    U32 bplmnId = 16;
    csg_id_arr[0] = 17;
    U16 num_supp_tas = 0;
    U16 num_bplmn = 0;
    U16 num_csg_ids = 0;

    /** enb name is optional**/
    if (p_s1_setup_req->bitmask & S1AP_OAM_ENB_NAME_PRESENT)
    {
        /*coverity_fix_531*/
        strncpy_wrapper(p_s1_setup_req->enb_name, enb_name_buff,MAX_ENB_NAME_STR_SIZE);
        /*coverity_fix_531*/
    }

    l3_memcpy_wrapper(&p_s1_setup_req->gb_enb_id.plmn_identity.plmn_id,
            &plmnId,MAX_PLMN_ID_BYTES);
    p_s1_setup_req->gb_enb_id.enb_id.presence_bitmask |= S1AP_OAM_MACRO_ENB_ID_PRESENT;
    l3_memcpy_wrapper(p_s1_setup_req->gb_enb_id.enb_id.macro_enb_id.eNB_id,
            &macro_enb_id,MACRO_ENB_ID_OCTET_SIZE);

    for ( num_supp_tas =0;num_supp_tas <  p_s1_setup_req->supp_ta_list.num_supported_tais
            ;num_supp_tas++)
    {
        l3_memcpy_wrapper(p_s1_setup_req->supp_ta_list.supp_tais[num_supp_tas].tac,
                &tac,TAC_OCTET_SIZE);
        for (num_bplmn =0 ; num_bplmn < p_s1_setup_req->
                supp_ta_list.supp_tais[num_supp_tas].bc_plmn_list.num_bplmn;
                num_bplmn++)
        {
            l3_memcpy_wrapper(p_s1_setup_req->supp_ta_list.
                    supp_tais[num_supp_tas].bc_plmn_list.
                    plmn_identity[num_bplmn].plmn_id,
                    &bplmnId,MAX_PLMN_ID_BYTES);
        }
    }


    p_s1_setup_req->default_paging_drx = 64;

    /** csg Id is optional*/
    for (num_csg_ids = 0 ; num_csg_ids < p_s1_setup_req->csg_id_list.num_csg_ids;
            num_csg_ids++)
    {
        l3_memcpy_wrapper(p_s1_setup_req->csg_id_list.csg_ids[0].csg_id,
                &csg_id_arr[0],
                CSG_ID_OCTET_SIZE);
    }
}

/*****************************************************************************
 * Function Name  : configure_s1_setup_req_test_case 
 * Inputs         : p_s1_setup_req - pointer to s1_setup_req_t
 *                  testCase - testcase id  
 * Outputs        : p_s1_setup_req
 * Returns        : None
 * Description    : This function configures the s1 setup req test case according 
 *                  to the passed test case.
 ********************************************************************************/
void configure_s1_setup_req_test_case(U32 testCase,s1_setup_req_t * p_s1_setup_req)
{
    if (testCase == S1_SETUP_REQ_TEST1)
    {
        p_s1_setup_req->bitmask |= S1AP_OAM_ENB_NAME_PRESENT;
        p_s1_setup_req->supp_ta_list.num_supported_tais = 1;
        p_s1_setup_req->supp_ta_list.supp_tais[0].bc_plmn_list.num_bplmn = 1;
        p_s1_setup_req->csg_id_list.num_csg_ids = 1;
    }
    else if (testCase == S1_SETUP_REQ_TEST2)
    {
        p_s1_setup_req->bitmask = 0;
        p_s1_setup_req->supp_ta_list.num_supported_tais = 1;
        p_s1_setup_req->supp_ta_list.supp_tais[0].bc_plmn_list.num_bplmn = 1;
        p_s1_setup_req->csg_id_list.num_csg_ids = 0;
    }
    else if (testCase == S1_SETUP_REQ_TEST3)
    {
        p_s1_setup_req->bitmask = 1;
        p_s1_setup_req->supp_ta_list.num_supported_tais = 0;
        p_s1_setup_req->supp_ta_list.supp_tais[0].bc_plmn_list.num_bplmn = 1;
        p_s1_setup_req->csg_id_list.num_csg_ids = 1;
    }
    else if (testCase == S1_SETUP_REQ_TEST4)
    {
        p_s1_setup_req->bitmask = 1;
        p_s1_setup_req->supp_ta_list.num_supported_tais = 1;
        p_s1_setup_req->supp_ta_list.supp_tais[0].bc_plmn_list.num_bplmn = 0;
        p_s1_setup_req->csg_id_list.num_csg_ids = 1;
    }
    set_s1_setup_req(p_s1_setup_req);
}

/*****************************************************************************
 * Function Name  : test_s1_setup_request 
 * Inputs         : testType - type of test case
 * Outputs        : None 
 * Returns        : None
 * Description    : This function tests the s1_setup_request
 ********************************************************************************/
void test_s1_setup_request(U32 testType)
{
    s1_setup_req_t s1_setup_req;
    U8 asn_msg[1024];
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0 , enbUeS1apId = 0;

    printf_wrapper("\n====TEST: S1 SETUP REQ : TESTCASE %d START====\n",testType);

    /******************** S1 SETUP REQ TEST****************************/
    memset_wrapper(&s1_setup_req,0,sizeof(s1_setup_req_t));

    configure_s1_setup_req_test_case(testType,&s1_setup_req);

    if (RRC_FAILURE == compose_asn_s1_setup_req (
                &s1_setup_req,
                asn_msg,
                &asn_msg_len))
    {
        printf_wrapper(" compose_asn_s1_setup_req returnd Failure\n");
    }
    else 
    {
        dump_asn_msg(asn_msg,asn_msg_len);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
        }
    }
    printf_wrapper("\n====TEST: S1 SETUP REQ : TESTCASE %d END====\n",testType);
}


enum {
    /* Reset type : S1 Intf */
    RESET_TEST1 =1,
    /* Reset Type : part of S1 Intf (num of s1 connection items ==1)*/
    RESET_TEST2,
    /* Reset Type : part of S1 Intf (num of s1 connection items ==256)*/
    RESET_TEST3,
    /* Reset Type : part of S1 Intf (num of s1 connection items ==1)
     * both mmeUeS1apId and enbUeS1apId are invalid*/
    RESET_TEST4,
    /* Reset Type : part of S1 Intf (num of s1 connection items ==5)*/
    RESET_TEST5,
    /* Reset Type : part of S1 Intf (num of s1 connection items ==6)*/
    RESET_TEST6,
    RESET_TEST7,
    RESET_TEST8

};


/*****************************************************************************
 * Function Name  : set_reset_req 
 * Inputs         : p_reset_req - pointer to reset_req_t 
 * Outputs        : p_reset_req
 * Returns        : None
 * Description    : This function resets the S1AP request and fills info in to 
 *                  p_reset_req.
 ********************************************************************************/
void set_reset_req(reset_req_t *p_reset_req)
{
    U32 num_items = 0;
    U32 start_enbues1apid =0x00;

    /**** Cause ****/
    p_reset_req->cause.t = T_s1ap_Cause_radioNetwork;
    p_reset_req->cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;

    /*** reset info ****/
    for (num_items= 0 ; num_items <p_reset_req->reset_info.num_of_items ;
            num_items++)
    {
            p_reset_req->reset_info.id_pair[num_items].bitmask |= MME_UE_S1AP_ID_PRESENT;
    }
    p_reset_req->reset_info.id_pair[num_items].mmeUeS1apId = start_enbues1apid+ num_items;
    p_reset_req->reset_info.id_pair[num_items].enbUeS1apId = start_enbues1apid+ num_items;
}

/*****************************************************************************
 * Function Name  : configure_reset_test_case 
 * Inputs         : testCase - test case id 
 *                  p_reset_req - pointer to the reset_req_t
 * Outputs        : p_reset_req
 * Returns        : None
 * Description    : This function configures reset test case  request. 
 ********************************************************************************/
void configure_reset_test_case(U32 testCase, reset_req_t * p_reset_req)
{
    if (testCase == RESET_TEST1)
    {
        p_reset_req->reset_info.num_of_items = 0;
        set_reset_req(p_reset_req);
    }
    else if (testCase == RESET_TEST2)
    {
        p_reset_req->reset_info.num_of_items = 1;
        set_reset_req(p_reset_req);
    }
    else if (testCase == RESET_TEST3)
    {
        p_reset_req->reset_info.num_of_items = 256;
        set_reset_req(p_reset_req);
    }
    else if (testCase == RESET_TEST4)
    {
        /**** Cause ****/
        p_reset_req->cause.t = T_s1ap_Cause_radioNetwork;
        p_reset_req->cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;

        /*** reset info ****/
        p_reset_req->reset_info.num_of_items = 1;
        p_reset_req->reset_info.id_pair[0].bitmask = 0;
    }
    else if (testCase == RESET_TEST5)
    {
        p_reset_req->reset_info.num_of_items = 3;
        set_reset_req(p_reset_req);
    }
    else if (testCase == RESET_TEST6)
    {
        p_reset_req->reset_info.num_of_items = 6;
        set_reset_req(p_reset_req);
        p_reset_req->reset_info.id_pair[0].enbUeS1apId = 0x0000024d;
    }
    else if (testCase == RESET_TEST7)
    {
        p_reset_req->reset_info.num_of_items = 7;
        set_reset_req(p_reset_req);
        p_reset_req->reset_info.id_pair[0].enbUeS1apId = 0x0002024d;
        p_reset_req->reset_info.id_pair[1].enbUeS1apId = 0x00010258;
    }
    else if (testCase == RESET_TEST8)
    {
        p_reset_req->reset_info.num_of_items = 1;
        set_reset_req(p_reset_req);
        p_reset_req->reset_info.id_pair[0].mmeUeS1apId = 0x00000000;
    }
}

/*****************************************************************************
 * Function Name  : test_reset_req 
 * Inputs         : testCase - test case id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function tests the reset request.
 ********************************************************************************/
void test_reset_req(U32 testCase)
{
    reset_req_t reset_req;
    reset_req_t reset_req_recv;
    U8 asn_msg[1024];
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0 , enbUeS1apId = 0;

    printf_wrapper("\n====TEST: RESET REQ : TESTCASE %d START====\n",testCase);
    /******************** RESET TEST****************************/
    memset_wrapper(&reset_req,0,sizeof(reset_req_t));
    memset_wrapper(&reset_req_recv,0,sizeof(reset_req_t));

    configure_reset_test_case(testCase, &reset_req);

    if (RRC_FAILURE ==  compose_asn_reset(
                &reset_req,
                asn_msg,
                &asn_msg_len))
    {
        printf_wrapper(" compose_asn_reset returnd Failure\n");
    }
    else 
    {
        dump_asn_msg(asn_msg,asn_msg_len);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
            if (RRC_FAILURE == parse_asn_reset(
                        asn_msg,
                        asn_msg_len,
                        &reset_req_recv))
            {
                printf_wrapper(" parse_asn_reset returnd Failure\n");
            }
        }
    }

    printf_wrapper("\n====TEST: RESET REQ : TESTCASE %d END====\n",testCase);
}



enum {
    /* S1 connetion list absent */
    RESET_ACK_TEST1 =1,
    /* S1 connetion list present */
    RESET_ACK_TEST2,
    /* S1 connetion list present  & num of s1 connection items == 1 but
     * mmeUeS1apId and enbUeS1apId both invalid*/
    RESET_ACK_TEST3,
    RESET_ACK_TEST4,
    RESET_ACK_TEST5,
    RESET_ACK_TEST6,
    RESET_ACK_TEST7
};

/*****************************************************************************
 * Function Name  : set_reset_ack_req 
 * Inputs         : p_reset_ack_info - pointer to p_reset_ack_info
 * Outputs        : p_reset_ack_info
 * Returns        : None
 * Description    : This function sets and resets the ack_req and fills in to 
 *                  p_reset_ack_info.
 ********************************************************************************/
void set_reset_ack_req(s1ap_reset_info_t *p_reset_ack_info)
{
    U32 num_items =0;
    U32 start_enbues1apid =0x00;

    for (num_items =0 ; num_items < p_reset_ack_info->num_of_items ;
            num_items++)
    {
        p_reset_ack_info->id_pair[num_items].bitmask = 0;
        if (num_items < 2)
        {
            p_reset_ack_info->id_pair[num_items].bitmask |= MME_UE_S1AP_ID_PRESENT;
    }
    p_reset_ack_info->id_pair[num_items].mmeUeS1apId = start_enbues1apid+num_items;
    p_reset_ack_info->id_pair[num_items].enbUeS1apId = start_enbues1apid+num_items;
    }
}

/*****************************************************************************
 * Function Name  : configure_reset_ack_test_case 
 * Inputs         : testCase - test case id
 *                  p_reset_ack_info - pointer to s1ap_reset_info_t
 * Outputs        : p_reset_ack_info
 * Returns        : None
 * Description    : This function configures and resets test case for ack_req 
 *                  and populates reset info to p_reset_ack_info.
 ********************************************************************************/
void configure_reset_ack_test_case(U32 testCase, s1ap_reset_info_t * p_reset_ack_info)
{
    memset_wrapper(p_reset_ack_info,0,sizeof(s1ap_reset_info_t));
    if (RESET_ACK_TEST1 == testCase)
    {
        p_reset_ack_info->num_of_items = 0;
        set_reset_ack_req(p_reset_ack_info);
    }
    else if (RESET_ACK_TEST2 == testCase)
    {
        p_reset_ack_info->num_of_items = 3;
        set_reset_ack_req(p_reset_ack_info);
    }
    else if (RESET_ACK_TEST3 == testCase)
    {
        p_reset_ack_info->num_of_items = 1;
        p_reset_ack_info->id_pair[0].bitmask = 0;
    }
    else if (RESET_ACK_TEST4 == testCase)
    {
        p_reset_ack_info->num_of_items = 5;
        set_reset_ack_req(p_reset_ack_info);
    }
    else if (RESET_ACK_TEST5 == testCase)
    {
        p_reset_ack_info->num_of_items = 6;
        set_reset_ack_req(p_reset_ack_info);
        p_reset_ack_info->id_pair[0].enbUeS1apId = 0x0000024d;
    }
    else if (RESET_ACK_TEST6 == testCase)
    {
        p_reset_ack_info->num_of_items = 7;
        set_reset_ack_req(p_reset_ack_info);
        p_reset_ack_info->id_pair[0].enbUeS1apId = 0x0002024d;
        p_reset_ack_info->id_pair[1].enbUeS1apId = 0x00010258;
    }
    else if (RESET_ACK_TEST7 == testCase)
    {
        p_reset_ack_info->num_of_items = 1;
        set_reset_ack_req(p_reset_ack_info);
        p_reset_ack_info->id_pair[0].mmeUeS1apId = 0x00000000;
    }

}

/*****************************************************************************
 * Function Name  : test_reset_ack 
 * Inputs         : testCase - test case id
 * Outputs        : None
 * Returns        : None
 * Description    : This function tests and resets for Acknowlegde mode.
 ********************************************************************************/
void test_reset_ack(U32 testCase)
{
    s1ap_reset_info_t reset_ack_info;
    s1ap_reset_info_t reset_ack_info_recv;
    U8 asn_msg[1024];
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0 , enbUeS1apId = 0;

    printf_wrapper("\n====TEST: RESET ACK : TESTCASE %d START====\n",testCase);
    /******************** RESET ACK TEST****************************/
    memset_wrapper(&reset_ack_info,0,sizeof(s1ap_reset_info_t));
    memset_wrapper(&reset_ack_info_recv,0,sizeof(s1ap_reset_info_t));


    configure_reset_ack_test_case(testCase, &reset_ack_info);

    if (RRC_FAILURE ==  compose_asn_reset_ack(
                &reset_ack_info,
                asn_msg,
                &asn_msg_len))
    {
        printf_wrapper(" compose_asn_reset returnd Failure\n");
    }
    else 
    {
        dump_asn_msg(asn_msg,asn_msg_len);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
            if (RRC_FAILURE ==  parse_asn_reset_ack (
                        asn_msg,
                        asn_msg_len,
                        &reset_ack_info_recv))
            {
                printf_wrapper(" parse_asn_reset_ack returnd Failure\n");
            }
        }
    }
    printf_wrapper("\n====TEST: RESET ACK : TESTCASE %d END====\n",testCase);
}


enum {
    /* No time to wait */
    S1_SETUP_FAILURE_TEST1 =1,
    /* Time to wait present */
    S1_SETUP_FAILURE_TEST2
};

/*****************************************************************************
 * Function Name  : configure_s1_setup_failure_test_case 
 * Inputs         : testCase - test case id
 *                  p_s1_setup_failure - pointer to s1_setup_failure_t
 * Outputs        : p_s1_setup_failure
 * Returns        : None
 * Description    : This function configures the s1_setup_failure test case 
 *                  and populates p_s1_setup_failure with test case info.
 ********************************************************************************/
void configure_s1_setup_failure_test_case(U32 testCase, s1_setup_failure_t * p_s1_setup_failure)
{
    p_s1_setup_failure->cause.t = T_s1ap_Cause_transport;
    p_s1_setup_failure->cause.u.radioNetwork = s1ap_transport_resource_unavailable;

    if (S1_SETUP_FAILURE_TEST1 == testCase)
    {
        p_s1_setup_failure->time_to_wait = 0;
    }
    else if (S1_SETUP_FAILURE_TEST2 == testCase)
    {
        p_s1_setup_failure->time_to_wait = s1ap_v5s;
    }
}

/*****************************************************************************
 * Function Name  : test_s1_setup_failure 
 * Inputs         : testCase - test case id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function tests s1_setup_failure.
 ********************************************************************************/
void test_s1_setup_failure(U32 testCase)
{
    s1_setup_failure_t s1_setup_failure;
    s1_setup_failure_t s1_setup_failure_recv;
    U8 asn_msg[1024];
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0 , enbUeS1apId = 0;

    printf_wrapper("\n====TEST: S1 SETUP FAILURE ACK : TESTCASE %d START====\n",testCase);
    /******************** TEST****************************/
    memset_wrapper(&s1_setup_failure,0,sizeof(s1_setup_failure_t));
    memset_wrapper(&s1_setup_failure_recv,0,sizeof(s1_setup_failure_t));


    configure_s1_setup_failure_test_case(testCase, &s1_setup_failure);

    if (RRC_FAILURE ==  compose_asn_s1_setup_failure(
                &s1_setup_failure,
                asn_msg,
                &asn_msg_len))
    {
        printf_wrapper(" compose_asn_s1_setup_failure returnd Failure\n");
    }
    else 
    {
        dump_asn_msg(asn_msg,asn_msg_len);

        printf_wrapper(" Time to wait encoded %d \n",s1_setup_failure.time_to_wait);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
            if (RRC_FAILURE ==  parse_asn_s1_setup_failure (
                        asn_msg,
                        asn_msg_len,
                        &s1_setup_failure_recv.time_to_wait))
            {
                printf_wrapper(" parse_asn_s1_setup_failure returnd Failure\n");
            }
            printf_wrapper("Time to wait decoded is : %d\n",s1_setup_failure_recv.time_to_wait);
        }
    }
    printf_wrapper("\n====TEST: S1 SETUP FAILURE ACK : TESTCASE %d END====\n",testCase);
}

enum {
    /* MME name present */
    S1_SETUP_RESPONSE_TEST1 =1,
    S1_SETUP_RESPONSE_TEST2 =2,
    S1_SETUP_RESPONSE_TEST3 =3
};

/*****************************************************************************
 * Function Name  : configure_s1_setup_response_test_case 
 * Inputs         : testCase - test case id
 *                  p_s1_setup_resp - pointer to parsed_s1_setup_rsp_t
 * Outputs        : parsed_s1_setup_rsp_t
 * Returns        : None
 * Description    : This function configures s1_setup_response test case and
 *                  populates test case info to parsed_s1_setup_rsp_t.
 ********************************************************************************/
void configure_s1_setup_response_test_case(U32 testCase, parsed_s1_setup_rsp_t * p_s1_setup_resp,
        U8* p_mme_name)
{
    p_s1_setup_resp->num_served_gummeis = 1;
}

/*****************************************************************************
 * Function Name  : test_s1_setup_response 
 * Inputs         : testCase - test case id
 * Outputs        : None
 * Returns        : None
 * Description    : This function tests the s1_setup_response case. 
 ********************************************************************************/
void test_s1_setup_response(U32 testCase)
{
    parsed_s1_setup_rsp_t s1_setup_response;
    parsed_s1_setup_rsp_t s1_setup_response_recv;
    U8 asn_msg[1024];
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0 , enbUeS1apId = 0;
    U8 mme_name[150] = "TEST:DUMMY MME name";

    printf_wrapper("\n====TEST: S1 SETUP RESPONSE : TESTCASE %d START====\n",testCase);
    /******************** TEST****************************/
    memset_wrapper(&s1_setup_response,0,sizeof(parsed_s1_setup_rsp_t));
    memset_wrapper(&s1_setup_response_recv,0,sizeof(parsed_s1_setup_rsp_t));

    l3_strncpy_wrapper(s1_setup_response.p_mme_name,mme_name,150);
    configure_s1_setup_response_test_case(testCase, &s1_setup_response,mme_name);

    if (RRC_FAILURE ==  compose_asn_s1_setup_response(
                &s1_setup_response,
                asn_msg,
                &asn_msg_len))
    {
        printf_wrapper(" compose_asn_s1_setup_response returnd Failure\n");
    }
    else 
    {
        dump_asn_msg(asn_msg,asn_msg_len);

        printf_wrapper("MME name encoded : %s\n",s1_setup_response.p_mme_name);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
            if (RRC_FAILURE ==  parse_asn_s1_setup_response (
                        asn_msg,
                        asn_msg_len,
                        &s1_setup_response_recv))
            {
                printf_wrapper(" parse_asn_s1_setup_response returnd Failure\n");
            }
            printf_wrapper("MME name decoded : %s\n",s1_setup_response_recv.p_mme_name);
        }
    }
    printf_wrapper("\n====TEST: S1 SETUP RESPONSE : TESTCASE %d END====\n",testCase);
}

/*****************************************************************************
 * Function Name  : test_partial_decode_func 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function tests partial decode function. 
 ********************************************************************************/
void test_partial_decode_func()
{
    U8 asn_msg[1024]= {0x00, 0x17, 0x00 ,0x12 ,0x00 ,0x00 ,0x02 ,0x00 ,0x63 ,0x00 ,0x06 ,0x04 ,0x01 ,0x00 ,0x40 ,0x02 ,0x00 ,0x00 ,0x02 ,0x40 ,0x01 ,0x44 ,0x00 ,0x00 ,0x00};
    U16 asn_msg_len = 0;
    s1ap_EP_et s1ap_EP = INVALID_S1AP_EP;
    U32 mmeUeS1apId = 0x00010152 , enbUeS1apId = 0x00010152;

    printf_wrapper("\n====TEST: PARTIAL:INITAL CTX SETUP:  START====\n");
    /******************** TEST****************************/
    {

        mmeUeS1apId = 0 ; 
        enbUeS1apId = 0;
        asn_msg_len = 22;
        dump_asn_msg(asn_msg,asn_msg_len);

        if (RRC_FAILURE ==  s1ap_partially_decode_asn_msg (
                    asn_msg,
                    asn_msg_len,
                    &s1ap_EP,
                    &mmeUeS1apId,
                    &enbUeS1apId,
                    &ue_id_pair))
        {
            printf_wrapper(" partially_decode_S1AP_PDU returnd Failure\n");
        }
        else
        {
            printf_wrapper("Partial dec res:: EP %s mmeUeS1apId %u enbUeS1apId %u\n",
                    s1ap_EP_test_str[s1ap_EP],
                    mmeUeS1apId,
                    enbUeS1apId);
        }
    }
}


/*****************************************************************************
 * Function Name  : main 
 * Inputs         : None
 * Outputs        : None
 * Returns        : Int
 * Description    : this function internally calls a function which tests and 
 *                  resets for Acknowledge mode.
 ********************************************************************************/
int main()
{
    test_reset_ack(RESET_ACK_TEST7);

    return 1;
}
