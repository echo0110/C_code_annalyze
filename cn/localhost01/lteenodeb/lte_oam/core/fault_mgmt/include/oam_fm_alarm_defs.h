/*****************************************************************************
 *File Name :oam_fm_alarm_defs.h
 *
 *Description : This file contains #defines for all alarm IDs supported by OAM
 *              
 *
 *Revision Histroy:
 *
 *Date            Author      	     References        	  Comments
 *------          --------    	     ------------     	  -------------
 *Aug 23, 2013 Vivek Kumar Gupta      OAM 2.3          Initial Version
 *May 2014     Ekta JAin                               SPR 11399 FIX
 *May 2014     Ekta JAin                               SPR 11479,11481 FIX
 *June 2014    Ekta Jain                               SPR 11833 FIX
 *Nov,2014     Karuna Bhardwaj                         EICIC Enhancement
 *Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ******************************************************************************/
#ifndef OAM_FM_ALARM_DEFS_H
#define OAM_FM_ALARM_DEFS_H


/****************************************************************************
 *              ALARM-ID DEFINES                                            *
 ****************************************************************************
 */

/* SPR 11051 Fix Start */
/*Duplicated Alarm ID removed*/
/*SPR 11051 Fix End */

#define CRITICAL_CONFIGURATION_FAILURE_ALARM_ID                           150
#define PM_DATA_UPLOAD_FAILURE_ALARM_ID                                   151
#define CONFIG_FILE_PERSISTENCE_FAILURE_ALARM_ID                          152
#define IMAGE_DOWNLOAD_FAILURE_ALARM_ID                                   153
#define LOG_ARCHIVE_FAILURE_ALARM_ID                                      154
#define ARCHIVED_LOG_UPLOAD_FAILURE_ALARM_ID                              155
#define OAM_PKG_INTEGRITY_CHECK_FAILURE_ALARM_ID                          156
#define OAM_LAYER_STARTUP_FAILURE_ALARM_ID                                158
#define HM_RESPONSE_NOT_RECVD_ALARM_ID                                    159
/* RT SPR 12768 FIX START */
/*SPR 11833 FIX START*/
#define MAC_PHY_OAM_ERR_MSG_INVALID_STATE_ALARM_ID                        161
#define MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD_ALARM_ID   162
#define MAC_PHY_OAM_ERR_MSG_BCH_MISSING_ALARM_ID                          163
#define MAC_PHY_OAM_ERR_MSG_HI_ERR_ALARM_ID                               164
/*SPR 11833 FIX END*/
/* SPR 12653 FIX START */
#define OAM_SON_PCI_CONFLICT_NOTIFICATION                                 165
#define OAM_X2_LINK_UP_IND_ALARM_ID                                       166
#define OAM_SON_PCI_CONFUSION_NOTIFICATION                                167
/* SPR 12653 FIX END */
#define OAM_X2_LINK_DOWN_IND_ALARM_ID                                     168
/* RT SPR 12768 FIX END */

/*eICIC_PHASE_1_2_CHANGES_START*/
/*SPR 14811 start*/
#define ABS_PATTERN_NOT_FOUND_FOR_LOW_LOAD_ALARM_ID                       169
#define ABS_PATTERN_NOT_FOUND_FOR_MID_LOAD_ALARM_ID                       170
#define ABS_PATTERN_NOT_FOUND_FOR_HIGH_LOAD_ALARM_ID                      171
#define ABS_PATTERN_NOT_FOUND_FOR_OVER_LOAD_ALARM_ID                      172
#define ABS_PATTERN_FOUND_FOR_LOW_LOAD_ALARM_ID                           173
#define ABS_PATTERN_FOUND_FOR_MID_LOAD_ALARM_ID                           174
#define ABS_PATTERN_FOUND_FOR_HIGH_LOAD_ALARM_ID                          175
#define ABS_PATTERN_FOUND_FOR_OVER_LOAD_ALARM_ID                          176
#define AGGR_ABS_NOT_APPLIED_FOR_LOW_LOAD_ALARM_ID                        177
#define AGGR_ABS_APPLIED_FOR_LOW_LOAD_ALARM_ID                            178
#define USABLE_ABS_NOT_APPLIED_ALARM_ID                                   179
#define USABLE_ABS_APPLIED_ALARM_ID                                       180
#define VICTM_ABS_NOT_FOUND_ALARM_ID                                      181
#define VICTM_ABS_FOUND_ALARM_ID                                          182 
#define VICTM_MEAS_SUBSET_NOT_FOUND_ALARM_ID                              183
#define VICTM_MEAS_SUBSET_FOUND_ALARM_ID                                  184 
#define AGGR_ABS_NOT_APPLIED_FOR_MID_LOAD_ALARM_ID                        185
#define AGGR_ABS_APPLIED_FOR_MID_LOAD_ALARM_ID                            186
#define AGGR_ABS_NOT_APPLIED_FOR_HIGH_LOAD_ALARM_ID                       187
#define AGGR_ABS_APPLIED_FOR_HIGH_LOAD_ALARM_ID                           188
#define AGGR_ABS_NOT_APPLIED_FOR_OVER_LOAD_ALARM_ID                       189
#define AGGR_ABS_APPLIED_FOR_OVER_LOAD_ALARM_ID                           190
/*SPR 14811 end*/
/*eICIC_PHASE_1_2_CHANGES_END*/

/* SPR 13765  changes start */
#define EGTPU_S1U_LINK_DOWN_IND_ALARM_ID                                  191
#define EGTPU_S1U_LINK_UP_IND_ALARM_ID                                    192
/* SPR 13765  changes end */

/* EMBMS Changes Start */
#define SYSTEM_FAILURE_ALARM_ID                                           7
#define M2AP_PROVISIONING_FAILURE_ALARM_ID                                193 
#define M2AP_SCTP_ASSOC_FAIL_ALARM_ID                                     194
#define RRC_M2AP_CONNECTION_SUCC_ALARM_ID                                 195
#define RRC_M2AP_CONNECTION_FAIL_ALARM_ID                                 196
#define EMBMS_NOT_SUPPORTED_ALARM_ID                                      197
/* EMBMS Changes End */
/* SPR 21889 start */
#define OAM_AGENT_PROCESS_SPAWN_ERROR                                       193
#define OAM_AGENT_PROCESS_STARTUP_FAILED                                    194
#define OAM_AGENT_PROCESS_SHUTDOWN_FAILED                                   195
#define OAM_AGENT_LOG_COLLECTION_FAILED                                     196
#define OAM_AGENT_FILE_TRANSFER_ERROR                                       197
/* SPR 21889 end */

/*SPR 11479 FIX START*/
   /*OAM_PROCESS_MON_LAYER_FAILURE_ALARM_ID(alarm id 157 deleted)*/
/*SPR 11479 FIX END*/
/*SPR 11481 FIX START*/
   /* CHILD_KILL_SIGNAL_ALARM_ID (alarm id 160 deleted)*/
/*SPR 11481 FIX END*/

#define OAM_NTP_SYNC_FAILURE_ALARM_ID                                     301    

#define OAM_SDM_SHUTDOWN_FAILURE_ALARM_ID                                 421 	
#define OAM_PDCP_SECURITY_ENGINE_ERROR_ALARM_ID                           422
/*SPR 15281 START */
#define OAM_SON_PCI_RESELECTION_FAILURE                    168
/*SPR 15281 START */

#endif

