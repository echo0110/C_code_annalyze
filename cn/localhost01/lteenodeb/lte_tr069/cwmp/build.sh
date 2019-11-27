##############################################################################################
#
#   FILE NAME   : make.sh
#
#   DESCRIPTION: This file is build script for glue layer with libtr069 and libipsec
#
#   REVISION HISTORY:
#
#   NAME              DATE                REFERENCE              REASON
#
#   Anant K Sharma    Aug 17, 2012                              Initial Creation
#   Anant K Sharma    JAN 25, 2013                              Added compilation flag HAVE_IPSEC
#                                                               and added some lib linking
#   Anant K Sharma    MAR 15, 2013                              Removed compilation flag HAVE_IPSEC for 
#                                                               libtr069 delivery without glue layer    
#   Anant K Sharma    MAR 25, 2013                              Added flag Wall 
#
#
#   All Rights Reserved. Copyright @ Aricent Group, 2012.
#
###############################################################################################


#!/bin/bash

WDIR=`pwd`
CSPL_DIR=$LTE_ROOT/lte_rrc/
IPSEC_DIR=../../glue_layer/ipsec
GLU_DIR=../../glue_layer
LIB_DIR=../../libs

# SPR 21314 changes start #
#Changes under this SPR tag are done to generate the file wise coverity 
#report for lte_tr069. Earlier all the .c and .h files were copied and compiled
#at obj path.Now, all the files will be compiled at their respective places.

DRIVER_DIR=$WDIR/Driver/
ACS_DISCOVERY_DIR=$WDIR/acs_discovery/
ACS_CONN_LIST_DIR=$WDIR/acs_conn_listner
DNS_CLIENT_DIR=$WDIR/acs_discovery/dns_client/
ACS_DISCOVERY_IP_DIR=$WDIR/acs_discovery/ip_manager/
ACS_DISCOVERY_DHCP_DIR=$WDIR/acs_discovery/dhcp_client/
COMMON_DIR=$WDIR/common
COMMON_MANAGEMENT_DIR=$WDIR/common/management_server_model/
COMMON_UTILITY_DIR=$WDIR/common/utility/
CONN_MANAGER_DIR=$WDIR/connection_manager/
CONN_MANAGER_DIGEST_DIR=$WDIR/connection_manager/authentication/digest/md5/
CONN_MANAGER_COMMON_DIR=$WDIR/connection_manager/authentication/common/
CONN_MANAGER_HTTP_DIR=$WDIR/connection_manager/http_connection/http/
LOGGER_DIR=$WDIR/logger/
RPC_DIR=$WDIR/rpc/rpc_handler/
SESSION_MANAGER_DIR=$WDIR/session_manager/session/
CLIENT_MANAGER_DIR=$WDIR/session_manager/client_req_manager/
EVENT_MANAGER_DIR=$WDIR/session_manager/event_manager/
SOAP_DIR=$WDIR/soap/

TARGET_CURL_INC=$WDIR/../3rd_party/curl/include/
TARGET_OPENSSL=$WDIR/../3rd_party/ssl/include/

\rm -rf $WDIR/obj

mkdir $WDIR/obj


cd $WDIR/obj

rm -f a.out
rm -f *.o

if [ "$2" = "HAVE_IPSEC" ]
then
$LTE_CC -Wall -Wno-unused-but-set-varible -c $DRIVER_DIR/src/*.c $ACS_DISCOVERY_DIR/src/*.c $ACS_CONN_LIST_DIR/src/*.c $DNS_CLIENT_DIR/src/*.c $ACS_DISCOVERY_IP_DIR/src/*.c $ACS_DISCOVERY_DHCP_DIR/src/*.c $COMMON_DIR/src/*.c $COMMON_MANAGEMENT_DIR/src/*.c $COMMON_UTILITY_DIR/src/*.c $CONN_MANAGER_DIR/src/*.c $CONN_MANAGER_COMMON_DIR/src/*.c $CONN_MANAGER_HTTP_DIR/src/*.c $LOGGER_DIR/src/*.c $RPC_DIR/src/*.c $SESSION_MANAGER_DIR/src/*.c  $CLIENT_MANAGER_DIR/src/*.c $EVENT_MANAGER_DIR/src/*.c $SOAP_DIR/src/*.c -I $DRIVER_DIR/include/ -I $ACS_DISCOVERY_DIR/include/ -I $ACS_CONN_LIST_DIR/include/ -I $DNS_CLIENT_DIR/include -I $ACS_DISCOVERY_IP_DIR/include -I $ACS_DISCOVERY_DHCP_DIR/include -I $COMMON_DIR/include/ -I $COMMON_MANAGEMENT_DIR/include -I $COMMON_UTILITY_DIR/include -I $CONN_MANAGER_DIR/include -I $CONN_MANAGER_DIGEST_DIR/include -I $CONN_MANAGER_COMMON_DIR/include -I $CONN_MANAGER_HTTP_DIR/include -I $LOGGER_DIR/include -I $RPC_DIR/include -I $SESSION_MANAGER_DIR/include/ -I $CLIENT_MANAGER_DIR/include -I $EVENT_MANAGER_DIR/include -I $SOAP_DIR/include -I $CSPL_DIR/cspl/h -I $CSPL_DIR/cspl/ports/linux/ -I . -I ./common/include/ -I $IPSEC_DIR/include/ -DVENDOR_SPECIFIC -DHAVE_IPSEC -I $TARGET_CURL_INC -I $TARGET_OPENSSL -DNSN_SPECIFIC
else
$LTE_CC -Wall -Wno-unused-but-set-varible -c $DRIVER_DIR/src/*.c $ACS_DISCOVERY_DIR/src/*.c $ACS_CONN_LIST_DIR/src/*.c $DNS_CLIENT_DIR/src/*.c $ACS_DISCOVERY_IP_DIR/src/*.c $ACS_DISCOVERY_DHCP_DIR/src/*.c $COMMON_DIR/src/*.c $COMMON_MANAGEMENT_DIR/src/*.c $COMMON_UTILITY_DIR/src/*.c $CONN_MANAGER_DIR/src/*.c $CONN_MANAGER_COMMON_DIR/src/*.c $CONN_MANAGER_HTTP_DIR/src/*.c $LOGGER_DIR/src/*.c $RPC_DIR/src/*.c $SESSION_MANAGER_DIR/src/*.c  $CLIENT_MANAGER_DIR/src/*.c $EVENT_MANAGER_DIR/src/*.c $SOAP_DIR/src/*.c  -I $DRIVER_DIR/include/ -I $ACS_DISCOVERY_DIR/include/ -I $ACS_CONN_LIST_DIR/include/ -I $DNS_CLIENT_DIR/include -I $ACS_DISCOVERY_IP_DIR/include -I $ACS_DISCOVERY_DHCP_DIR/include -I $COMMON_DIR/include/ -I $COMMON_MANAGEMENT_DIR/include -I $COMMON_UTILITY_DIR/include -I $CONN_MANAGER_DIR/include -I $CONN_MANAGER_DIGEST_DIR/include -I $CONN_MANAGER_COMMON_DIR/include -I $CONN_MANAGER_HTTP_DIR/include -I $LOGGER_DIR/include -I $RPC_DIR/include -I $SESSION_MANAGER_DIR/include/ -I $CLIENT_MANAGER_DIR/include -I $EVENT_MANAGER_DIR/include -I $SOAP_DIR/include -I $CSPL_DIR/cspl/h -I $CSPL_DIR/cspl/ports/linux/ -I . -I ./common/include/ -I $IPSEC_DIR/include/ -DVENDOR_SPECIFIC -I $TARGET_CURL_INC -I $TARGET_OPENSSL -DNSN_SPECIFIC
fi


$LTE_AR rcs libtr069.a *.o

mv *.a ../../libs

# SPR 21314 changes end #
