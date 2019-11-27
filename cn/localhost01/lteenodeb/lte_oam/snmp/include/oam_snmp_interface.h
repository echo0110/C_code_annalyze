/******************************************************************************
*   FILE NAME    : oam_snmp_interface.h
*
*   DESCRIPTION  : SNMP Agent Interface header file
*
*       DATE            AUTHOR          REFERENCE       REASON
*   25 July 2013   Purshottam Gupta     ---------       Initial
*   April 2014     Vinod Vyas           CSR 57877 
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef __OAM_SNMP_IF_H__
#define __OAM_SNMP_IF_H__

/*******************************************************************************
*                        Header Files
*******************************************************************************/
/* CSR 57877 Fix Start */
#include <lteTypes.h>
#include <oam_mgmt_intf_api.h>
/* CSR 57877 Fix End*/


#define OAM_SNMP_LOG(source, log_priority, formate, ...) \
        oam_snmp_trace_message(log_priority,(SInt8 *) __FILE__,(SInt8 *)__FUNCTION__,__LINE__,(SInt8 *)formate,##__VA_ARGS__)


/*******************************************************************************
*                         #DEFINES
*******************************************************************************/
#define OAM_MGMT_IF_MAX_PARAM_NAME_LEN      			                  128
#define OAM_MGMT_IF_MAX_PARENT_NAME_LEN      			                  256
#define OAM_MGMT_IF_MAX_ERR_STRING_LEN       			                   80
#define OAM_MGMT_IF_MIN_RESP_PAYLOAD_LEN      			                    8
#define OAM_MGMT_IF_MIN_PARENT_INFO_LEN       			                    8
#define OAM_MGMT_IF_MIN_PARAM_INFO_LEN        			                    4
#define OAM_VERSION                           			                    1
#define OAM_INVALID_RETURN			      			            -1
#define OAM_ZERO			      			                                0
#define OAM_NULL                                                         NULL
#define OAM_MGMT_CSPL_HDR_SIZE                                             16
#define OAM_MGMT_INTF_HEADER_SIZE                                          12
#define OAM_MGMT_API_START                                                135
#define ENBSWIMAGEFILE_NAME_MAX_SIZE                                      256
#define SNMP_ROW_INSTANCE_ID_MAX_LEN                                       10
/*SPR 14657 Fixed Start*/
#define SNMP_OAM_IF_RESP_TIMEOUT_S                                         60
/*SPR 14657 Fixed End*/
#define SNMP_OAM_IF_RESP_TIMEOUT_US                                         0
#define SNMP_MASTER_AGENT_POLLING_TIME_S                                    0
#define SNMP_MASTER_AGENT_POLLING_TIME_US                                 200
#define SNMP_OAM_IF_MESSAGE_RECEIVED                                        1
#define MAX_LINE_LENGTH                                                   100
#define MAX_RECEIVE_BUFFER_SIZE                                          2048
#define SNMP_OAM_IF_WAKEUP_TIMEDOUT                                         2
#define ONE_BYTE_SHIFT                                                      8
#define BLOCKING_CALL                                                       1
#define OAM_SNMP_VA_START                                           va_start
#define OAM_SNMP_VA_END                                             va_end

#define SNMP_OAM_IF_MSG_SRC_ID_OFFSET                                       2
#define SNMP_OAM_IF_MSG_DST_ID_OFFSET     (SNMP_OAM_IF_MSG_SRC_ID_OFFSET + 2)
#define SNMP_OAM_IF_API_ID_OFFSET         (SNMP_OAM_IF_MSG_DST_ID_OFFSET + 2)
#define SNMP_OAM_IF_MSG_LEN_OFFSET            (SNMP_OAM_IF_API_ID_OFFSET + 2)
#define CSPL_HDR_DST_ID_OFFSET                                              3
#define CSPL_HDR_PAYLOADSIZE_OFFSET                                        24

#define SNMP_OAM_IF_MSG_WORD_LEN                                            2
/* CSR 57877 Changes Start*/
/* moved API to oam_mgmt_intf_api.h*/
/* CSR 57877 Changes End*/
/* Coverity FIX 41203 */
#define OAM_NULL_CHECK(s)\
     if(s) {}\
     else { OAM_SNMP_LOG (SNMP, OAM_SNMP_DETAILED,"\n NULL CHECK FAILURE ! \n"); return OAM_SNMP_GEN_ERROR;}

/*************************************************************
*                                                            *
*       OAM-SNMP ERROR CODES MAPPING                         *
*                                                            *
*************************************************************/
#define SNMP_OAM_IF_ERR_NOERR              0


/*************************************************************
*                                                            *
*       OAM-SNMP PARAMETER TYPES                             *
*                                                            *
*************************************************************/
#define SNMP_OAM_SCALAR_PARAM                                               0
#define SNMP_OAM_TABLE_PARAM                                                1

/*************************************************************
*                                                            *
*       OAM-SNMP SIGNAL MACROS                               *
*                                                            *
*************************************************************/
/*porting changes*/
#define SNMP_SIGTERM 15
#define SNMP_SIGKILL 9
#define SNMP_SIGINT 2




/****************************************************************************
*                        ENUMERATION DEFINITIONS
****************************************************************************/
#define OID_PARAM_MIN 0

enum oid_param {
    OID_PARAM_0 = OID_PARAM_MIN,
    OID_PARAM_1,
    OID_PARAM_2,
    OID_PARAM_3,
    OID_PARAM_4,
    OID_PARAM_5,
    OID_PARAM_6,
    OID_PARAM_7,
    OID_PARAM_8,
    OID_PARAM_9,
    OID_PARAM_10,
    OID_PARAM_11,
    OID_PARAM_12,
    OID_PARAM_13,
    OID_PARAM_14,
    OID_PARAM_15,
    OID_PARAM_16,
    OID_PARAM_17,
    OID_PARAM_18,
    OID_PARAM_19,
    OID_PARAM_20,
    OID_PARAM_21,
    OID_PARAM_22,
    OID_PARAM_23,
    OID_PARAM_24,
    OID_PARAM_25,
    OID_PARAM_26,
    OID_PARAM_27,
    OID_PARAM_28,
    OID_PARAM_29,
    OID_PARAM_30,
    OID_PARAM_31,
    OID_PARAM_32,
    OID_PARAM_33,
    OID_PARAM_34,
    OID_PARAM_35,
    OID_PARAM_36,
    OID_PARAM_37,
    OID_PARAM_38,
    OID_PARAM_39,
    OID_PARAM_40,
    OID_PARAM_41,
    OID_PARAM_42,
    OID_PARAM_43,
    OID_PARAM_44,
    OID_PARAM_45,
    OID_PARAM_46,
    OID_PARAM_47,
    OID_PARAM_48,
    OID_PARAM_49,
    OID_PARAM_50,
    OID_PARAM_51,
    OID_PARAM_52,
    OID_PARAM_53,
    OID_PARAM_54,
    OID_PARAM_55,
    OID_PARAM_56,
    OID_PARAM_57,
    OID_PARAM_58,
    OID_PARAM_59,
    OID_PARAM_60,
    OID_PARAM_61,
    OID_PARAM_62,
    OID_PARAM_63,
    OID_PARAM_64,
    OID_PARAM_65,
    OID_PARAM_66,
    OID_PARAM_67,
    OID_PARAM_68,
    OID_PARAM_69,
    OID_PARAM_70,
    OID_PARAM_71,
    OID_PARAM_72,
    OID_PARAM_73,
    OID_PARAM_74,
    OID_PARAM_75,
    OID_PARAM_76,
    OID_PARAM_77,
    OID_PARAM_78,
    OID_PARAM_79,
    OID_PARAM_80,
    OID_PARAM_81,
    OID_PARAM_82,
    OID_PARAM_83,
    OID_PARAM_84,
    OID_PARAM_85,
    OID_PARAM_86,
    OID_PARAM_87,
    OID_PARAM_88,
    OID_PARAM_89,
    OID_PARAM_90,
    OID_PARAM_91,
    OID_PARAM_92, 
    OID_PARAM_93,
    OID_PARAM_94,
    OID_PARAM_95,
    OID_PARAM_96,
    OID_PARAM_97,
    OID_PARAM_98,
    OID_PARAM_99,
    OID_PARAM_100,
    OID_PARAM_101,
    OID_PARAM_102,
    OID_PARAM_103,
    OID_PARAM_104,
    OID_PARAM_105,
    OID_PARAM_106,
    OID_PARAM_107,
    OID_PARAM_108,
    OID_PARAM_109,
    OID_PARAM_110,
    OID_PARAM_111,
    OID_PARAM_112,
    OID_PARAM_113,
    OID_PARAM_114,
    OID_PARAM_115,
    OID_PARAM_116,
    OID_PARAM_117,
    OID_PARAM_118,
    OID_PARAM_119,
    OID_PARAM_120,
    OID_PARAM_121,
    OID_PARAM_122,
    OID_PARAM_123,
    OID_PARAM_124,
    OID_PARAM_125,
    OID_PARAM_126,
    OID_PARAM_127,
    OID_PARAM_128,
    OID_PARAM_129,
    OID_PARAM_130,
    OID_PARAM_131,
    OID_PARAM_132,
    OID_PARAM_133,
    OID_PARAM_134,
    OID_PARAM_135,
    OID_PARAM_136,
    OID_PARAM_137,
    OID_PARAM_138,
    OID_PARAM_139,
    OID_PARAM_140,
    OID_PARAM_141,
    OID_PARAM_142,
    OID_PARAM_143,
    OID_PARAM_144,
    OID_PARAM_145,
    OID_PARAM_146,
    OID_PARAM_147,
    OID_PARAM_148,
    OID_PARAM_149,
    OID_PARAM_150,
    OID_PARAM_151,
    OID_PARAM_152,
    OID_PARAM_153,
    OID_PARAM_154,
    OID_PARAM_155,
    OID_PARAM_156,
    OID_PARAM_157,
    OID_PARAM_158,
    OID_PARAM_159,
    OID_PARAM_160,
    OID_PARAM_161,
    OID_PARAM_162,
    OID_PARAM_163,
    OID_PARAM_164,
    OID_PARAM_165,
    OID_PARAM_166,
    OID_PARAM_167,
    OID_PARAM_168,
    OID_PARAM_169,
    OID_PARAM_170,
    OID_PARAM_171,
    OID_PARAM_172,
    OID_PARAM_173,
    OID_PARAM_174,
    OID_PARAM_175,
    OID_PARAM_176,
    OID_PARAM_177,
    OID_PARAM_178,
    OID_PARAM_179,
    OID_PARAM_180,
    OID_PARAM_181,
    OID_PARAM_182,
    OID_PARAM_183,
    OID_PARAM_184,
    OID_PARAM_185,
    OID_PARAM_186,
    OID_PARAM_187,
    OID_PARAM_188,
    OID_PARAM_189,
    OID_PARAM_190,
    OID_PARAM_191,
    OID_PARAM_192, 
    OID_PARAM_193,
    OID_PARAM_194,
    OID_PARAM_195,
    OID_PARAM_196,
    OID_PARAM_197,
    OID_PARAM_198,
    OID_PARAM_199,
    OID_PARAM_200,
    OID_PARAM_201,
    OID_PARAM_202,
    OID_PARAM_203,
    OID_PARAM_204,
    OID_PARAM_205,
    OID_PARAM_206,
    OID_PARAM_207,
    OID_PARAM_208,
    OID_PARAM_209,
    OID_PARAM_210,
    OID_PARAM_211,
    OID_PARAM_212,
    OID_PARAM_213,
    OID_PARAM_214,
    OID_PARAM_215,
    OID_PARAM_216,
    OID_PARAM_217,
    OID_PARAM_218,
    OID_PARAM_219,
    OID_PARAM_220,
    OID_PARAM_221,
    OID_PARAM_222,
    OID_PARAM_223,
    OID_PARAM_224,
    OID_PARAM_225,
    OID_PARAM_226,
    OID_PARAM_227,
    OID_PARAM_228,
    OID_PARAM_229,
    OID_PARAM_230,
    OID_PARAM_231,
    OID_PARAM_232,
    OID_PARAM_233,
    OID_PARAM_234,
    OID_PARAM_235,
    OID_PARAM_236,
    OID_PARAM_237,
    OID_PARAM_238,
    OID_PARAM_239,
    OID_PARAM_240,
    OID_PARAM_241,
    OID_PARAM_242,
    OID_PARAM_243,
    OID_PARAM_244,
    OID_PARAM_245,
    OID_PARAM_246,
    OID_PARAM_247,
    OID_PARAM_248,
    OID_PARAM_249,
    OID_PARAM_250,
    OID_PARAM_251,
    OID_PARAM_252,
    OID_PARAM_253,
    OID_PARAM_254,
    OID_PARAM_255,
    OID_PARAM_256,
    OID_PARAM_257,
    OID_PARAM_258,
    OID_PARAM_259,
    OID_PARAM_260,
    OID_PARAM_261,
    OID_PARAM_262,
    OID_PARAM_263,
    OID_PARAM_264,
    OID_PARAM_265,
    OID_PARAM_266,
    OID_PARAM_267,
    OID_PARAM_268,
    OID_PARAM_269,
    OID_PARAM_270,
    OID_PARAM_271,
    OID_PARAM_272,
    OID_PARAM_273,
    OID_PARAM_274,
    OID_PARAM_275,
    OID_PARAM_276,
    OID_PARAM_277,
    OID_PARAM_278,
    OID_PARAM_279,
    OID_PARAM_280,
    OID_PARAM_281,
    OID_PARAM_282,
    OID_PARAM_283,
    OID_PARAM_284,
    OID_PARAM_285,
    OID_PARAM_286,
    OID_PARAM_287,
    OID_PARAM_288,
    OID_PARAM_289,
    OID_PARAM_290,
    OID_PARAM_291,
    OID_PARAM_292, 
    OID_PARAM_293,
    OID_PARAM_294,
    OID_PARAM_295,
    OID_PARAM_296,
    OID_PARAM_297,
    OID_PARAM_298,
    OID_PARAM_299,
    OID_PARAM_300,
    OID_PARAM_301,
    OID_PARAM_302,
    OID_PARAM_303,
    OID_PARAM_304,
    OID_PARAM_305,
    OID_PARAM_306,
    OID_PARAM_307,
    OID_PARAM_308,
    OID_PARAM_309,
    OID_PARAM_310,
    OID_PARAM_311,
    OID_PARAM_312,
    OID_PARAM_313,
    OID_PARAM_314,
    OID_PARAM_315,
    OID_PARAM_316,
    OID_PARAM_317,
    OID_PARAM_318,
    OID_PARAM_319,
    OID_PARAM_320,
    OID_PARAM_321,
    OID_PARAM_322,
    OID_PARAM_323,
    OID_PARAM_324,
    OID_PARAM_325,
    OID_PARAM_326,
    OID_PARAM_327,
    OID_PARAM_328,
    OID_PARAM_329,
    OID_PARAM_330,
    OID_PARAM_331,
    OID_PARAM_332,
    OID_PARAM_333,
    OID_PARAM_334,
    OID_PARAM_335,
    OID_PARAM_336,
    OID_PARAM_337,
    OID_PARAM_338,
    OID_PARAM_339,
    OID_PARAM_340,
    OID_PARAM_341,
    OID_PARAM_342,
    OID_PARAM_343,
    OID_PARAM_344,
    OID_PARAM_345,
    OID_PARAM_346,
    OID_PARAM_347,
    OID_PARAM_348,
    OID_PARAM_349,
    OID_PARAM_350,
    OID_PARAM_351,
    OID_PARAM_352,
    OID_PARAM_353,
    OID_PARAM_354,
    OID_PARAM_355,
    OID_PARAM_356,
    OID_PARAM_357,
    OID_PARAM_358,
    OID_PARAM_359,
    OID_PARAM_360,
    OID_PARAM_361,
    OID_PARAM_362,
    OID_PARAM_363,
    OID_PARAM_364,
    OID_PARAM_365,
    OID_PARAM_366,
    OID_PARAM_367,
    OID_PARAM_368,
    OID_PARAM_369,
    OID_PARAM_370,
    OID_PARAM_371,
    OID_PARAM_372,
    OID_PARAM_373,
    OID_PARAM_374,
    /*SPR 14225 start*/
    OID_PARAM_375,
    OID_PARAM_376,
    OID_PARAM_377,
    OID_PARAM_378,
    OID_PARAM_379,
    OID_PARAM_380,
    OID_PARAM_381,
    OID_PARAM_382,
    OID_PARAM_383,
    OID_PARAM_384,
    OID_PARAM_385,
    OID_PARAM_386,
    OID_PARAM_387,
    OID_PARAM_388,
    /*SPR 14225 end*/
    /* EMBMS Changes Start */
    OID_PARAM_389,
    OID_PARAM_390,
    OID_PARAM_391,
    OID_PARAM_392,
    OID_PARAM_393,
    OID_PARAM_394,
    OID_PARAM_395,
    OID_PARAM_396,
    OID_PARAM_397
    /* EMBMS Changes End */

};

#define OID_PARAM_MAX  OID_PARAM_374

typedef enum __oam_mgmt_if_mode_et {
    OAM_MGMT_IF_SET_REQ,
    OAM_MGMT_IF_GET_REQ,
    OAM_MGMT_IF_SET_RESP,
    OAM_MGMT_IF_GET_RESP
} oam_mgmt_if_mode_et;

typedef enum {   
    OAM_SNMP_GEN_ERROR           = 0,
    OAM_SNMP_SUCCESS             = 1,
    OAM_SNMP_UNEXPECTED_MSG      = 2,
/* CSR 57877 Changes */
/* Code Moved to oam_mgmt_intf_api.h */
    OAM_SNMP_TIMEOUT             = 9000,
    OAM_SNMP_NOACCESS            = 9001,
    OAM_SNMP_RESOURCEUNAVAILABLE = 9004,
    OAM_SNMP_NOSUCHNAME          = 9005,
/******BUG 1233 FIX START************/
    OAM_SNMP_INVALID_VALUE       = 9007,
/******BUG 1233 FIX STOP************/
    OAM_SNMP_NOTWRITABLE         = 9008,
    OAM_SNMP_INT_ERROR           = 9040
} mgmt_return_et;


typedef enum __log_priority_et {
  OAM_SNMP_DETAILED,             
  OAM_SNMP_INFO,                 
  OAM_SNMP_WARNING,              
  OAM_SNMP_ERROR,                
  OAM_SNMP_CRITICAL              
} log_priority_et;

/****************************************************************************
*                        STRUCTURE DEFINITIONS
****************************************************************************/
typedef struct __oam_mgmt_if_param_info_t {
   /* Param name may be of type "param=val" if the request mode is SET */
    Char8                         param_name[OAM_MGMT_IF_MAX_PARAM_NAME_LEN];
    struct __oam_mgmt_if_param_info_t      *next;
} oam_mgmt_if_param_info_t;

typedef struct __oam_mgmt_if_req_t {
    Char8                       parent_name[OAM_MGMT_IF_MAX_PARENT_NAME_LEN];
    UInt8                       num_params;
    oam_mgmt_if_param_info_t    *param_list;
} oam_mgmt_if_req_t;

typedef struct __oam_mgmt_if_resp_status_t {
    UInt8              status;
    UInt16             err_code;
    UInt8              err_str_len;
    Char8              err_string[OAM_MGMT_IF_MAX_ERR_STRING_LEN];
} oam_mgmt_if_resp_status_t;


typedef oam_mgmt_if_resp_status_t oam_mgmt_if_set_resp_t;

/* CSR 57877 Changes*/
/* Code Moved to oam_mgmt_intf_api.h */

typedef struct __oam_mgmt_if_get_resp_t {
    oam_resp_status_et          resp;
    Char8                       parent_name[OAM_MGMT_IF_MAX_PARENT_NAME_LEN];
    UInt8                       num_params;
    oam_mgmt_if_param_info_t    *param_list;
} oam_mgmt_if_get_resp_t;

typedef struct __oam_mgmt_if_param_tab_t {
    UInt16          index;
    Char8           *parent;
    Char8           *param;
} oam_mgmt_if_param_tab_t;

/****************************************************************************
*                        FUNCTION PROTOTYPES
****************************************************************************/

void
oam_snmp_trace_message (SInt32 log_level,    /* log level of this message */
                        SInt8 * pFile,
                        SInt8 * pFun,
                        UInt16 Line,
                        SInt8 * format,  /* format string like in printf */
                        ...     /* optional parameters */
                        );

mgmt_return_et
oam_snmp_get_req_handler
(
        oam_mgmt_if_req_t *p_req,
        void *p_resp_info
);

int
oam_snmp_sprintf (char *str, const char *format, ...);

mgmt_return_et
oam_snmp_decode_mgmt_if_resp_msg
(
        UInt8 *p_mgmt_if_resp,
        oam_mgmt_if_mode_et mode,
        void *p_resp_info
);

mgmt_return_et
oam_snmp_decode_mgmt_if_get_resp_info
(
        UInt8 *p_mgmt_if_resp,
        SInt32 len_left,
        oam_mgmt_if_get_resp_t *p_resp
);

mgmt_return_et
oam_snmp_decode_mgmt_if_get_resp_parent_info
(
        UInt8 *p_mgmt_if_resp,
        SInt32 len_left,
        UInt16 *parent_len,
        oam_mgmt_if_get_resp_t *p_resp
);

mgmt_return_et
oam_snmp_decode_mgmt_if_resp_status
(
        UInt8 *p_resp_payload,
        SInt32 payload_len,
        oam_mgmt_if_resp_status_t *p_resp
);

/* + SPR 17439 */
UInt16
oam_snmp_create_new_trans_id(void);
/* - SPR 17439 */

void
oam_snmp_fill_mgmt_if_req_payload
(
        UInt8 *ptr_msg,
        UInt16 rem_len,
        oam_mgmt_if_req_t *p_req
);

int
oam_snmp_fprintf
(
    FILE *str,
    const char *format,
    ...
);


mgmt_return_et
oam_mgmt_mem_alloc
(
    size_t size,
    void **p_msg
);

mgmt_return_et
oam_snmp_set_req_handler
(
    oam_mgmt_if_req_t *p_req,
    void *p_resp_info
);

mgmt_return_et
oam_mgmt_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
);

FILE*
oam_snmp_fopen
(
    const Char8 *filename ,
    const Char8 *mode
);


void
oam_snmp_fclose
(
FILE *
);


void *oam_mgmt_memset(void *s, int c, size_t n);

typedef struct _prov_req_t
{
  /* File pointer for loging */
  FILE *p_out;
  /* File pointer for loging */
  FILE *p_fp;
  /* Log level for Loging */
  UInt8 log_level;

}prov_request_t;

SInt32 snmp_oam_handle_oam_notif(UInt8 *p_api_buf);

void *snmp_memcpy(void *dest, const void *src, size_t n);

/*porting changes*/
void (*oam_snmp_register_signal_handler (int sig, void (*free_resources) (int))) (int); 

int
oam_snmp_sprintf (char *str, const char *format, ...);

void *snmp_memcpy(void *dest, const void *src, size_t n);

void oam_snmp_exit(int status);

char *oam_snmp_strncpy(Char8 *s1, const Char8 *s2, size_t n);

time_t oam_snmp_time(time_t *t);

struct tm *oam_snmp_localtime_r(const time_t *timep, struct tm *result);

Char8* oam_snmp_strncat(Char8 *str1, const Char8 *str2, size_t n);

Char8* oam_snmp_strcat(Char8 *str1, const Char8 *str2);

int oam_snmp_strncmp(const char *str1, const char *str2, size_t length);

size_t
oam_snmp_strlen
(
    const char *str
);

char *
oam_snmp_strchr
(
    const char *str,
    char var
);

long  oam_snmp_strtol(const  char  *str, char ** endptr, int base);

int
oam_snmp_atoi
(
    const char *str
);

int oam_snmp_snprintf(char *str, size_t n, const char *format, ...);

unsigned long oam_snmp_strtoul(char * str,
              char **endptr, int base);
#endif
