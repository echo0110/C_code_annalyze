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
#   Anant K Sharma    JAN 31, 2013                              Added some include path for phase 2 changes
#   Anant K Sharma    MAR 15, 2013                              Added shared object for libgsoap instead of libgsoap.a
#   Anant K Sharma    MAR 19, 2013                              Removed flag HAVE_IPSEC from make
#                                                               Commented libipsec make
#   Anant K Sharma    APR 05, 2013                              changed some linking
#
#   All Rights Reserved. Copyright @ Aricent Group, 2012.
#
###############################################################################################
#!/bin/bash

#remove the old binary 
\rm -rf lte_tr069

#set the paths of libs and include dirs
CSPL_DIR=$LTE_ROOT/lte_rrc/
IPSEC_DIR=../../glue_layer/ipsec
CWMP_DIR=../../cwmp
LIB_DIR=../../libs
CSPL_LIB=$LTE_ROOT/lte_rrc/cspl/build/


if [ "$1" = "rdb" ]
then
USR_LIB_DIR=./
else
if [ $HOST_COMPILATION -eq 0 ]
then
USR_LIB_DIR=../../../../lib/broadcom_board/
else
USR_LIB_DIR=/usr/lib/
fi
fi
#make IPSec library after removing old library if any
cd $IPSEC_DIR/src
make clean
make
cd -


#make TR068 library, it will remove old TR069 library if any
cd $CWMP_DIR
if [ "$1" = "rdb" ] && [ "$2" = "HAVE_IPSEC" ]
then 
./build.sh rdb HAVE_IPSEC
elif [ "$1" = "rdb" ]
then
./build.sh rdb
elif [ "$2" = "HAVE_IPSEC" ]
then
./build.sh HAVE_IPSEC
else
./build.sh 
fi


cd -


#make Glue Layer binary lte_tr069
if [ "$2" = "HAVE_IPSEC" ]
then
$LTE_CC glue_layer.c -o lte_tr069 -I ./../include/ -I $CWMP_DIR/Driver/include/ -I $CWMP_DIR/common/utility/include/ -I $CWMP_DIR/common/include/ -I $CWMP_DIR/common/management_server_model/include/ -I $CSPL_DIR/cspl/h -I $CSPL_DIR/cspl/ports/linux/ -I $CWMP_DIR/logger/include/ -I $IPSEC_DIR/include/ -I $CWMP_DIR/soap/include/ -L$CSPL_LIB -L$LIB_DIR -L$USR_LIB_DIR -ltr069 -lipsec -lgsoap -lcurl -lcspl -lrt -lpthread -lssl -lcrypto -ldl -DVENDOR_SPECIFIC -DHAVE_IPSEC
else
$LTE_CC  glue_layer.c -o lte_tr069 -I ./../include/ -I $CWMP_DIR/Driver/include/ -I $CWMP_DIR/common/utility/include -I $CWMP_DIR/common/include/ -I $CWMP_DIR/common/management_server_model/include/ -I $CSPL_DIR/cspl/h -I $CSPL_DIR/cspl/ports/linux/ -I $CWMP_DIR/logger/include/ -I $LTE_ROOT/lte_oam/common/include -I $CWMP_DIR/soap/include/ -L$CSPL_LIB -L$LIB_DIR -L$USR_LIB_DIR  -ltr069 -lgsoap -lcurl -lcspl -lrt -lpthread -lssl -lcrypto  -ldl -DVENDOR_SPECIFIC
fi

#place this binary to bin 

mv lte_tr069 ../../bin
