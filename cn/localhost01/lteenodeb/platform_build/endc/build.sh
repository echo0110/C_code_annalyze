#!/bin/sh
#########################################################
### Platform specific - this is for host intel
#Date            Author         Remarks
#13.10.2015      gur04568       SPR17558: Modified to support intel only 

#########################################################
export LTE_ROOT=$PWD
#We could not modify the makefile present in RRC code, so exported
#CSPL_FLAG variable to compile CSPL with MODULE_LOGGING option
#Once CSPL makefile is modified to compile with MODULE_LOGGING
#option, below exported variable will be removed
export CSPL_FLAGS="-O2 -Wall -DMODULE_LOGGING -Wimplicit -Wmissing-prototypes -Wshadow -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -Wno-unused"
CSPL_FLAGS+=$L3_OPTIM_FLAGS
INTG=layer2
ENBSON=lte_son
ENBLTERRM=lte_rrm
ENBRRC=l3
ENBLTEOAM=lte_oam
INTGWITHADAPTER=l2integ
L2WITHADAPTER=l2
TR069=lte_tr069
NBSIM=nbsim
UESIMWITHADAPTER=ue
MAC=mac
RLC=rlc
PDCP=pdcp
MACEMU=macemu
UESIM=uesim
FRAMEWORK=framework
CLEANUP=clean
MME=mme
GTPU=gtpu
LTE_GTPU=lte_gtpu
OAMSIMUE=oamsim_ue
ALL=all
FDD=fdd
TDD=tdd
TG=tg
PLATFORM=intel
PLATFORM_PATH=intel_linux
SCTP_STACK=aricent_sctp_ipr
PRODUCTBUILDINFO=LTE_FRAMEWORK_5_2_1
NBIOT_FEAT=disable
LOGGER_TYPE=advanced
# M2AP Changes - Start
EMBMS_FEAT=disable
# M2AP Changes - End
REAL_EPC_TG=epc_tg 
STACKS=stacks

#NR_DC Changes - Start
ENDC_FEAT=enable
#NR_DC Changes - Stop

#the following flag would be passed to make
OPT_J="-j"
OPT_S="-s"

function check_failure()
{
    if [ $? -ne 0 ]
        then
            echo "build failed"
            exit 1
            fi
}

function build_usage()
{
    echo "Usage of build.sh ::"
    echo " ./build.sh <build-option> [duplexing-mode] [platform] [sctp-stack]"
    echo
    echo "Arguments to be passed ::"
    echo " <build-option>   :-> It is a mandatory parameter. "
    echo "                   -> It indicates the component of LTE FRAMEWORK to be built."
    echo "                   -> Following values can be passed for this parameter -" 
    echo "                      { all/ stacks / gtpu / l2 / l3 / nbsim / oamsim_ue / tg / lte_rrm / lte_son / lte_oam / clean }"
    echo "                   -> NOTE: 'nbsim' option compiles the UE Layer2."
    echo "                      NOTE: 'nbsim' cannot be compiled independently. To compile nbsim, L2 needs"
	echo "                            to be compiled before compilation of nbsim."
    echo "                      NOTE: 'oamsim_ue' option compiles the UE OAM."
    echo "                      NOTE: MME Simulator cannot be compiled independently. To compile MME, L3 needs"
	echo "                            to be compiled before compilation of nbsim."
    echo
    echo " [duplexing-mode] :-> It is an optional parameter."
    echo "                   -> It indicates the duplexing mode, for which LTE Framework is to be built."
    echo "                   -> Default value: 'fdd'."
    echo "                   -> Following values can be passed for this parameter - { fdd / tdd }"
    echo
    echo " [platform]       :-> It is an optional parameter."
    echo "                   -> It indicates the platform, for which LTE Framework is to be built"
    echo "                   -> Default value: 'intel'."
    echo "                   -> Following values can be passed for this parameter - { intel / intel_64 }"
    echo "                   -> NOTE: If this parameter needs to be passed then it's mandatory"
    echo "                            to pass the <duplexing-mode>."
    echo
    echo " [sctp-stack]     :-> It is an optional parameter."
    echo "                   -> It indicates the sctp stack, for which LTE Framework is to be built."
    echo "                   -> Default value: 'aricent_sctp_ipr'."
    echo "                   -> Following values can be passed for this parameter - { aricent_sctp_ipr / lksctp }"
    echo "                   -> NOTE: If this parameter needs to be passed then it's mandatory"
    echo "                            to pass the <duplexing-mode> and <platform>."
    echo
}

#if [ $# -lt 1 ]; then
#	echo "run as <exe> <build-option>: [layer2]...[uesim]...[mac]...[rlc]...[pdcp]...[l2integ]...[ue]...[l2]...[nbsim]...[clean]"
#fi

if [ $# -lt 1 ]
then
        build_usage
exit 1
fi

# M2AP Changes - Start
if [ "enable" == $EMBMS_FEAT ];   then 
    sed -i "s/ULTE_EMBMS_SUPPORTED/DLTE_EMBMS_SUPPORTED/g" $LTE_ROOT/make.inc
    sed -i "s/ULTE_EMBMS_UESIM_TESTING/DLTE_EMBMS_UESIM_TESTING/g" $LTE_ROOT/make.inc
    sed -i "s/ULTE_EMBMS_SUPPORTED/DLTE_EMBMS_SUPPORTED/g" $LTE_ROOT/simulators/trafficGenerator/Makefile
    sed -i "s/ULTE_EMBMS_UESIM_TESTING/DLTE_EMBMS_UESIM_TESTING/g" $LTE_ROOT/simulators/trafficGenerator/Makefile
    export LTE_EMBMS_FLAG=1
else    
    sed -i "s/DLTE_EMBMS_SUPPORTED/ULTE_EMBMS_SUPPORTED/g" $LTE_ROOT/make.inc
    sed -i "s/DLTE_EMBMS_UESIM_TESTING/ULTE_EMBMS_UESIM_TESTING/g" $LTE_ROOT/make.inc
    sed -i "s/DLTE_EMBMS_SUPPORTED/ULTE_EMBMS_SUPPORTED/g" $LTE_ROOT/simulators/trafficGenerator/Makefile
    sed -i "s/DLTE_EMBMS_UESIM_TESTING/ULTE_EMBMS_UESIM_TESTING/g" $LTE_ROOT/simulators/trafficGenerator/Makefile
    export LTE_EMBMS_FLAG=0
fi

# M2AP Changes - End


#NR_DC Changes - Start
if [ "enable" == $ENDC_FEAT ];   then 
    sed -i "s/UENDC_ENABLED/DENDC_ENABLED/g" $LTE_ROOT/make.inc
    sed -i "s/UENDC_ENABLED/DENDC_ENABLED/g" $LTE_ROOT/lte_rrc/rrc/bld/Makefile
    sed -i "s/UENDC_ENABLED/DENDC_ENABLED/g" $LTE_ROOT/lte_rrc/runner/Makefile
    sed -i "s/UENDC_ENABLED/DENDC_ENABLED/g" $LTE_ROOT/lte_rrc/x2ap/x2ap.mk
    export ENDC_ENABLE=1
else    
    sed -i "s/DENDC_ENABLED/UENDC_ENABLED/g" $LTE_ROOT/make.inc
    sed -i "s/DENDC_ENABLED/UENDC_ENABLED/g" $LTE_ROOT/lte_rrc/rrc/bld/Makefile
    sed -i "s/DENDC_ENABLED/UENDC_ENABLED/g" $LTE_ROOT/lte_rrc/runner/Makefile
    sed -i "s/DENDC_ENABLED/UENDC_ENABLED/g" $LTE_ROOT/lte_rrc/x2ap/x2ap.mk
    export ENDC_ENABLE=0
fi
#NR_DC Changes - End
if [ $1 != $CLEANUP ]
then
    if [ $# -eq 1 ]
    then
        # Default duplexing mode is FDD
        DUPLEXING_MODE="fdd"
        PLATFORM="intel"
        if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
        then
        source ./.lteenodeb_settings
        fi
        PLATFORM_PATH="intel_linux"
        SCTP_STACK="aricent_sctp_ipr"
    elif [ $# -eq 2 ]
    then
        DUPLEXING_MODE="$2"
        PLATFORM="intel"
        if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
        then
        source ./.lteenodeb_settings
        fi
        PLATFORM_PATH="intel_linux"
        SCTP_STACK="aricent_sctp_ipr"
    elif [ $# -eq 3 ]
    then
        DUPLEXING_MODE="$2"
        SCTP_STACK="aricent_sctp_ipr"
        PLATFORM="$3"
        if [ $PLATFORM == "intel" ]
        then
           PLATFORM_PATH="intel_linux"
         if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
        then
            source ./.lteenodeb_settings
            fi
        elif [ $PLATFORM == "intel_64" ]
        then
           PLATFORM_PATH="intel_linux_64"
           if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
           then
           source ./.lteenodeb_settings
           fi
        else
            echo "Invalid platform choice"
            build_usage
            exit 1
        fi
    elif [ $# -eq 4 ]
    then
        DUPLEXING_MODE="$2"
        PLATFORM="$3"
        if [ $PLATFORM == "intel" ]
        then
           PLATFORM_PATH="intel_linux"
           if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
           then
           source ./.lteenodeb_settings
           fi
        elif [ $PLATFORM == "intel_64" ]
        then
           PLATFORM_PATH="intel_linux_64"
           if [ $SET_PDCP_GTPU_SPLIT_FLAG != 1 ]
           then
           source ./.lteenodeb_settings
           fi
        else
            echo "Invalid platform choice"
            build_usage
            exit 1
        fi
        SCTP_STACK="$4"
        if [ $SCTP_STACK != "aricent_sctp_ipr" ]
        then
            if [ $SCTP_STACK != "lksctp" ]
            then
                echo "Invalid SCTP stack choice"
                build_usage
                exit 1
            fi
        fi
    else
        echo "Invalid no. of arguments"
        build_usage
        exit 1
    fi

if grep -q "DIPR_NBIOT" $LTE_ROOT/make.inc ; then
NBIOT_FEAT=enable
fi
 
if grep -q "DENDC_ENABLED" $LTE_ROOT/make.inc ; then
ENDC_FEAT=enable
fi
 

if [ $PLATFORM == "intel" ] || [ $PLATFORM == "intel_64" ] ; then
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/make.inc
    sed -i "s/-DCYCLE_COUNT_1MS_RDB/-UCYCLE_COUNT_1MS_RDB/g" $LTE_ROOT/make.inc
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/lte_rrc/runner/Makefile
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/lte_rrc/rrc/bld/lterrc.mk 
    
   else
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/make.inc
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/lte_rrc/runner/Makefile
    sed -i "s/-DLOCKLESS_IMPL/-ULOCKLESS_IMPL/g" $LTE_ROOT/lte_rrc/rrc/bld/lterrc.mk
fi

if [ $FDD == $DUPLEXING_MODE ]
    then
        sed -i "s/DTDD_CONFIG/UTDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/DMAC_TDD_WITH_FRAMEWORK/UMAC_TDD_WITH_FRAMEWORK/g" $LTE_ROOT/make.inc
        sed -i "s/UFDD_CONFIG/DFDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/UULHARQ_TIMER_PROC/DULHARQ_TIMER_PROC/g" $LTE_ROOT/make.inc
        if [ $PLATFORM == "mindspeed" ]
            then
                sed -i "s/DULHARQ_TIMER_PROC/UULHARQ_TIMER_PROC/g" $LTE_ROOT/make.inc
            else
                sed -i "s/UULHARQ_TIMER_PROC/DULHARQ_TIMER_PROC/g" $LTE_ROOT/make.inc
        fi            
        sed -i "s/DTDD_CONFIG/UTDD_CONFIG/g" $LTE_ROOT/simulators/statsTool/Makefile
        sed -i "s/DMAC_TDD_WITH_FRAMEWORK/UMAC_TDD_WITH_FRAMEWORK/g" $LTE_ROOT/simulators/statsTool/Makefile
#sed -i "s/UOVERSHOOT_CORRECTION/DOVERSHOOT_CORRECTION/g" $LTE_ROOT/make.inc
        sed -i "s/UHD_FDD_CONFIG/DHD_FDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/DTDD_MODE_FLAG/UTDD_MODE_FLAG/g" $LTE_ROOT/make.inc
    elif [ $TDD == $DUPLEXING_MODE ]
    then
        sed -i "s/UTDD_CONFIG/DTDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/UMAC_TDD_WITH_FRAMEWORK/DMAC_TDD_WITH_FRAMEWORK/g" $LTE_ROOT/make.inc
        sed -i "s/DFDD_CONFIG/UFDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/DULHARQ_TIMER_PROC/UULHARQ_TIMER_PROC/g" $LTE_ROOT/make.inc
        sed -i "s/UTDD_CONFIG/DTDD_CONFIG/g" $LTE_ROOT/simulators/statsTool/Makefile
        sed -i "s/UMAC_TDD_WITH_FRAMEWORK/DMAC_TDD_WITH_FRAMEWORK/g" $LTE_ROOT/simulators/statsTool/Makefile
#sed -i "s/UOVERSHOOT_CORRECTION/DOVERSHOOT_CORRECTION/g" $LTE_ROOT/make.inc
        sed -i "s/DHD_FDD_CONFIG/UHD_FDD_CONFIG/g" $LTE_ROOT/make.inc
        sed -i "s/DHDFDD_STATS/UHDFDD_STATS/g" $LTE_ROOT/make.inc
        sed -i "s/DRLC_PDCP_IPC_INTF/URLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/DDL_UL_SPLIT/UDL_UL_SPLIT/g" $LTE_ROOT/make.inc
        sed -i "s/UTDD_MODE_FLAG/DTDD_MODE_FLAG/g" $LTE_ROOT/make.inc
    #    sed -i "s/DFAPI_4_0_COMPLIANCE/UFAPI_4_0_COMPLIANCE/g" $LTE_ROOT/make.inc
        sed -i "s/DFAPI_RELEASE8/UFAPI_RELEASE8/g" $LTE_ROOT/make.inc
        sed -i "s/DFAPI_RELEASE9/UFAPI_RELEASE9/g" $LTE_ROOT/make.inc
     #   sed -i "s/DFAPI_RELEASE10/UFAPI_RELEASE10/g" $LTE_ROOT/make.inc
    else
        echo "Invalid duplexing mode choice"
        build_usage
        exit 1
    fi
fi

#SPR 21653 changes start#
#code removed#
#SPR 21653 changes end#

if [ "1" == $ASTRI_FLEXRAN ]
    then
        sed -i "s/DFAPI_4_0_COMPLIANCE/UFAPI_4_0_COMPLIANCE/g" $LTE_ROOT/make.inc
        sed -i "s/UFLEXRAN /DFLEXRAN /g" $LTE_ROOT/make.inc
    else
        sed -i "s/UFAPI_4_0_COMPLIANCE/DFAPI_4_0_COMPLIANCE/g" $LTE_ROOT/make.inc
        sed -i "s/DFLEXRAN/UFLEXRAN/g" $LTE_ROOT/make.inc
fi

if [ "0" == $SET_PDCP_GTPU_SPLIT_FLAG ]
then
        sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/lte_rrc/rrc/bld/Makefile
fi
if [ $1 != $CLEANUP ]
then
    sed -i "s/SCTP_STACK\=[a-z_]*$/SCTP_STACK\=$SCTP_STACK/g" $LTE_ROOT/lte_rrc/sctp/sctp.mk
    sed -i "s/SCTP_STACK\=[a-z_]*$/SCTP_STACK\=$SCTP_STACK/g" $LTE_ROOT/simulators/protocolSimulatorTool/flags.mk
    # SPR 20430 Changes Start #
    sed -i "s/PLATFORM\=[a-z_0-9]*$/PLATFORM\=$PLATFORM/g" $LTE_ROOT/lte_rrc/sctp/sctp.mk
    sed -i "s/PLATFORM\=[a-z_0-9]*$/PLATFORM\=$PLATFORM/g" $LTE_ROOT/simulators/protocolSimulatorTool/flags.mk
    # SPR 20430 Changes End #

    echo -e "\n\n****************************    Selected Parameters     *********************"
    echo "          Build Option           :           $1"
    echo "          Duplexing Mode         :           $DUPLEXING_MODE"
    echo "          Platform               :           $PLATFORM"
    echo "          SCTP Stack             :           $SCTP_STACK"
    echo "*****************************************************************************"
    if [ $1 == $ALL ]
    then
        echo -e "\n Now building individual Components of LTE FRAMEWORK... \n  "
    fi
    sleep 2
fi


if [ $1 == $TR069 ]; then
    echo "*************************** BUILDING TR069         *********************"
    echo "*************************** COMPILING CSPL LIB ***************************"
    cd $LTE_ROOT/lte_rrc/
    export SIGTRAN_DIR=$LTE_ROOT/lte_rrc/sctp
    . ./.settings
    make $OPT_S $OPT_J -C cspl/build PORTDIR=../ports/posix/
    check_failure
    echo "*************************** ENDED COMPILATION CSPL LIB ***************************"
    cd $LTE_ROOT/lte_tr069/glue_layer/src
    ./make.sh
    check_failure
    echo "****************************COMPLETED BUILDING TR069 *******************"
    echo ""
    echo "Copying TR069 binary to ../bin folder"
	cp -f $LTE_ROOT/lte_tr069/bin/lte_tr069 $LTE_ROOT/../bin/
	cp -f $LTE_ROOT/lte_tr069/bin/ipr_cwmp_client.cfg $LTE_ROOT/../bin/
    echo ""
    echo ""
fi
if [ $1 == $ENBLTERRM ]; then
    echo "*************************** BUILDING LTE RRM         *********************"
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
        sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
    fi
    echo "*************************** COMPILING CSPL LIB ***************************"
    cd $LTE_ROOT/lte_rrc/
    . ./.settings
    make $OPT_S $OPT_J -C cspl/build PORTDIR=../ports/posix/
    check_failure
    echo "*************************** ENDED COMPILATION CSPL LIB ***************************"
    echo "*************************** COMPILING IN_MEM_LOGGER LIB ***************************"
    cd $LTE_ROOT/common/in_memory_logging/
    make $OPT_S $OPT_J -f Makefile
    check_failure
    echo "*************************** ENDED COMPILATION IN_MEM_LOGGER LIB ***************************"
    cd $LTE_ROOT/lte_rrm/
    make $OPT_S NBIOT_FEAT=$NBIOT_FEAT 
    check_failure
    echo "****************************COMPLETED BUILDING RRM   *******************"
    echo ""
    echo "Copying LTE_RRM binary to ../bin folder"
	cp -f $LTE_ROOT/lte_rrm/build/release/lte_rrm $LTE_ROOT/../bin/
    echo ""
    echo ""
fi
if [ $1 == $ENBSON ]; then
    echo "****************************BUILDING LTE_SON *********************"
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
    fi
    echo "*************************** COMPILING CSPL LIB ***************************"
    cd $LTE_ROOT/lte_rrc/
    . ./.settings
    make $OPT_S $OPT_J -C cspl/build PORTDIR=../ports/posix/
    check_failure
    echo "*************************** ENDED COMPILATION CSPL LIB ***************************"

    echo "*************************** COMPILING IN_MEM_LOGGER LIB ***************************"
    cd $LTE_ROOT/common/in_memory_logging/
    make $OPT_S $OPT_J -f Makefile
    check_failure
    echo "*************************** ENDED COMPILATION IN_MEM_LOGGER LIB ***************************"
    echo "Making directory and copying libasn1per.so and libasn1rt.so to SON library folder"
    mkdir -p $LTE_ROOT/lte_son/build/
    mkdir -p $LTE_ROOT/lte_son/build/lib/
    cp -f $LTE_ROOT/../lib/$PLATFORM_PATH/*.so $LTE_ROOT/lte_son/build/lib/
    echo "*************************** COMPILING SIB_DECODER ***************************"
    cd $LTE_ROOT/sib_decoder/make/
    make $OPT_S  
    check_failure
    echo "*************************** COMPILING UMTS_SIB_DECODER ***************************"
    cd $LTE_ROOT/umts_sib_decoder/make/
    make $OPT_S 
    check_failure
    echo "*************************** ENDED COMPILATION SIB_DECODER ***************************"
	cd $LTE_ROOT/lte_son/
    if [ $PLATFORM == "intel" ] || [ $PLATFORM == "intel_64" ]
        then
        make $OPT_S LTE_SON_PLATFORM=fapi COMPILE_MRO=yes
        else
        make $OPT_S LTE_SON_PLATFORM=mspd COMPILE_MRO=yes
    fi
    check_failure
    echo "Copying LTE_SON binary to ../bin folder"
    cp -f ./build/release/lte_son ../../bin/
    echo "Copying son_oam.cfg file to ../cfg folder"
    cp -f $LTE_ROOT/lte_son/cfg/son_oam.cfg $LTE_ROOT/../cfg/
    check_failure
            
    echo "****************************   COMPLETED BUILDING LTE_SON   **************************"
    echo ""
    echo ""
    echo ""
fi

if [ $1 == $ENBRRC ]; then
if grep -q "DIPR_NBIOT" $LTE_ROOT/make.inc ; then
NBIOT_FEAT=enable
fi
	echo "****************************BUILDING ENBRRC  *********************"
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
    fi
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
    then
    sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/lte_rrc/rrc/bld/Makefile
    fi
    cd lte_rrc/
    cp -f $LTE_ROOT/../lib/$PLATFORM_PATH/libasn1per.so $LTE_ROOT/../lib/
    cp -f $LTE_ROOT/../lib/$PLATFORM_PATH/libasn1rt.so $LTE_ROOT/../lib/
    cp -f $LTE_ROOT/../lib/$PLATFORM_PATH/libasn1per.so $LTE_ROOT/lte_rrc/rrc/asn/lib/
    cp -f $LTE_ROOT/../lib/$PLATFORM_PATH/libasn1rt.so $LTE_ROOT/lte_rrc/rrc/asn/lib/
    sed -i "s/DMAC_AUT_TEST/UMAC_AUT_TEST/g" $LTE_ROOT/make.inc	
    . ./.settings
    . ./.settings
    check_failure
    cd $LTE_ROOT/lte_rrc/sctp/sctp/build/linux
    make $OPT_S  TYPE=healthMonitorEnable -f Makefile
    check_failure

    if [ "advanced" == $LOGGER_TYPE ]
      then
        echo "*************************** COMPILING IN_MEM_LOGGER LIB ***************************"
        cd $LTE_ROOT/common/in_memory_logging/
        make $OPT_S $OPT_J -f Makefile
        check_failure
        echo "*************************** ENDED COMPILATION IN_MEM_LOGGER LIB ***************************"
    fi

    cd $LTE_ROOT/lte_rrc/runner
    #make $OPT_S $OPT_J LOGGER_TYPE=$LOGGER_TYPE LOG_ALM_FRWK=enable CFLAGS="-D_FORTIFY_SOURCE=0" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
# M2AP Changes - Start
    if [ "enable" == $EMBMS_FEAT ]
       then 
    make $OPT_S LOGGER_TYPE=advanced EMBMS_FEAT=$EMBMS_FEAT LOG_ALM_FRWK=enable EXTCFLAGS="-DLTE_EMBMS_SUPPORTED -DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
    #make LOGGER_TYPE=advanced EMBMS_FEAT=$EMBMS_FEAT SECTOR_TYPE=$SECTOR_TYPE LOG_ALM_FRWK=enable CFLAGS="-DLTE_EMBMS_SUPPORTED -DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
    #make LOGGER_TYPE=advanced EMBMS_FEAT=$EMBMS_FEAT SECTOR_TYPE=$SECTOR_TYPE LOG_ALM_FRWK=enable CFLAGS="-DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
# M2AP Changes - End
    elif [ "enable" == $NBIOT_FEAT ]
    then 
    make $OPT_S LOGGER_TYPE=advanced NBIOT_FEAT=$NBIOT_FEAT SECTOR_TYPE=$SECTOR_TYPE LOG_ALM_FRWK=enable EXTCFLAGS="-DIPR_NBIOT -DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
    elif [ "enable" == $ENDC_FEAT ]
       then 
    make $OPT_S LOGGER_TYPE=advanced ENDC_FEAT=$ENDC_FEAT SECTOR_TYPE=$SECTOR_TYPE LOG_ALM_FRWK=enable EXTCFLAGS="-DENDC_ENABLED -DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
    else
    make $OPT_S LOGGER_TYPE=advanced SECTOR_TYPE=$SECTOR_TYPE LOG_ALM_FRWK=enable EXTCFLAGS="-DHEADER_ALIGNMENT -D_FORTIFY_SOURCE=0 -fstack-protector-all" LD_LIBRARY_PATH=$LTE_ROOT/../lib/Intel_32bit_ASN_Lib
    fi
    check_failure
    cp -f config_file release/bin/.
    check_failure
    cd -
    echo ""
    echo "Copying L3 binary to ../bin folder"
	cp -f $LTE_ROOT/lte_rrc/runner/release/bin/rrc_entity $LTE_ROOT/../bin/
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
    then
    sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/lte_rrc/rrc/bld/Makefile
    fi
    echo ""
    echo ""
fi
if [ $1 == $FRAMEWORK ]; then
	echo "*********************BUILDING ALL COMPONENTS of FRAMEWORK*****************"
    CLEANUP=clean
    L2WITHADAPTER=l2
    NBSIM=nbsim
	echo "*********************COMPLETED *****************"
fi
if [ $1 == $INTG ]; then
	echo "****************************BUILDING MAC+RLC+PDCPSIM*********************"
	echo ""
	echo ""
	echo "***************************COMPILING FRAMEWORK***************************"
	make $OPT_S $OPT_J -f ./common/Makefile
    check_failure
	echo "************************ENDED COMPILING FRAMEWORK**************************"
	echo ""
	echo ""
	echo "***************************COMPILING RLCCORE***************************"
	make $OPT_S $OPT_J -f ./rlc/rlccore/Makefile FLAGS=-DGET_TICK_FROM_MAC 
    check_failure
	echo "************************ENDED COMPILING RLCCORE***************************"
	echo ""
	echo ""
	echo "***************************COMPILING MACCORE***************************"
	make $OPT_S $OPT_J -f ./mac/maccore/Makefile FLAGS=-DZERO_BUFFER_COPY 
    check_failure
	echo "************************ENDED COMPILING MACCORE***************************"
	echo ""
	echo ""
	echo "***************************COMPILING PDCP***************************"
	make $OPT_S $OPT_J -f ./pdcp/Makefile 
    check_failure
	echo "************************ENDED COMPILING PDCP***************************"
	echo ""
	echo ""
	echo "***************************COMPILING PHY SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/phy/Makefile enodebphysim
    check_failure
	echo "************************ENDED COMPILING PHY SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING RRC SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/rrc/Makefile FLAGS=-UMAC_SIM_INDIVIDUAL
    check_failure
	echo "************************ENDED COMPILING RRC SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM-RRC SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamRrcExe CTAGS=-DBUILD_EXE
    check_failure
	make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamClientExe
    check_failure
	echo "************************ENDED COMPILING OAM SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM APP***************************"
	make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile layer2
    check_failure
	echo "************************ENDED COMPILING OAM APP***************************"
	echo ""
fi
if [ $1 == $ENBLTEOAM ]; then
    echo "****************************BUILDING LTE_OAM*********************"
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
    then
    sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    echo "*************************** setting split flag for oam ***************************"
    fi
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
    fi
    echo "*************************** COMPILING CSPL LIB ***************************"
    cd $LTE_ROOT/lte_rrc/
    . ./.settings
    make $OPT_S $OPT_J -C cspl/build PORTDIR=../ports/posix/
    check_failure
    echo "*************************** COMPILING IN_MEM_LOGGER LIB ***************************"
    cd $LTE_ROOT/common/in_memory_logging/
    make $OPT_S $OPT_J -f Makefile
    check_failure
    echo "*************************** ENDED COMPILATION IN_MEM_LOGGER LIB ***************************"
    cd $LTE_ROOT/lte_oam/
    make $OPT_S NBIOT_FEAT=$NBIOT_FEAT $OPT_J all CC=$LTE_CC PLATFORM=$PLATFORM 
    check_failure
    echo "****************************COMPLETED BUILDING LTE_OAM *******************"
    echo ""
    echo ""
    echo ""
fi

if [ $1 == $INTGWITHADAPTER ]; then
	echo "**************************** BUILDING LAYER 2 STACK *********************"
        echo ""
        echo ""
        echo "*************************** COMPILING CSPL LIB ***************************"
        cd ./cspl/build/
        make
        check_failure
        cd - 
        echo ""
        echo ""
        echo "*************************** ENDED COMPILATION CSPL LIB ***************************"
        echo ""
        echo ""
        echo "*************************** COMPILING GTPU STACK LIB ***************************"
        cd ./gtpu
        . .gtpu_settings_linux
        check_failure
        make $OPT_S $OPT_J 
        check_failure
        cd -
        echo ""
        echo ""
        echo "*************************** ENDED COMPILATION GTPU STACK LIB ***************************"
	echo ""
	echo ""
	echo "*************************** COMPILING FRAMEWORK LIB ***************************"
	make $OPT_S $OPT_J -f ./common/Makefile
    check_failure
	echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
	echo ""
	echo ""
	echo "*************************** COMPILING RLC STACK ***************************"
	make $OPT_S $OPT_J -f ./rlc/rlccore/Makefile FLAGS=-DGET_TICK_FROM_MAC 
    check_failure
	echo "************************ ENDED COMPILILATION RLC STACK *************************"
	echo ""
	echo ""
	echo "***************************COMPILING MAC STACK ***************************"
	make $OPT_S $OPT_J -f ./mac/maccore/Makefile FLAGS=-DZERO_BUFFER_COPY 
    check_failure
	echo "************************ENDED COMPILING MAC STACK ***************************"
	echo ""
	echo ""
	echo "***************************COMPILING PDCP STACK ***************************"
	make $OPT_S $OPT_J -f ./pdcp/Makefile 
    check_failure
	echo "************************ENDED COMPILILATION PDCP STACK ***********************"
    echo ""
    echo ""
#    echo "***************************COMPILING PDCP ADAPTER LIB ***************************"
#    make $OPT_S $OPT_J -f ./simulators/adapter/pdcp/Makefile 
#    check_failure
#    echo "************************ENDED COMPILIATION PDCP ADAPTER ***************************"
	echo ""
	echo ""
	echo "***************************COMPILING PHY SIMULATOR ***************************"
	make $OPT_S $OPT_J -f ./simulators/phy/Makefile enodebphysim
    check_failure
	echo "************************ENDED COMPILILATION PHY SIMULATOR ***************************"
	echo ""
	echo ""
	echo "*************************** COMPILING OAM-RRC SIMULATOR***************************"
    make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamRrcExe CTAGS=-DBUILD_EXE
    check_failure
    make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamClientExe
    check_failure
    echo "************************ENDED COMPILILATION OAM SIMULATOR***************************"
    echo ""
    echo ""
    echo "*************************** COMPILING CONTROL APP ***************************"
    make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile l2WithAdpt
    check_failure
    echo "*************************** ENDED COMPILIATION CONTROL APP ***************************"
    echo ""
    echo "*************************** COMPILING TRAFFIC GEN APP ***************************"
    make $OPT_S $OPT_J -f ./simulators/trafficGenerator/Makefile 
    check_failure
    echo "*************************** ENDED COMPILIATION TRAFFIC GEN APP ***************************"
    echo ""
fi
if [ $1 == $L2WITHADAPTER ]; then
         sed -i "s/ULOG_PRINT_ENABLED/DLOG_PRINT_ENABLED/g" $LTE_ROOT/make.inc

        if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
        then
           
            sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
#SPR 22575 changes start
            sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
            sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
#SPR 22575 changes end
            
            echo "*************************** COMPILING FRAMEWORK LIB ***************************"
            cd $LTE_ROOT/common/
            make clean
            cd $LTE_ROOT/
        fi

        if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
        then 
        sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" make.inc
        sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
        fi

        echo "**************************** BUILDING LAYER 2 STACK WITHOUT SIMULATORS*********************"
        echo ""
        echo ""
        echo "********************************* COMPILING EGTPU ******************************"
#        sed -i "s/-Wall/-Wall -fstack-protector-all/g" $LTE_ROOT/make.inc
        if [ "0" == $SET_PDCP_GTPU_SPLIT_FLAG ]
        then
        cd $LTE_ROOT/gtpuPacketRelay/packetRelay/
        check_failure
        make
        cd -
        check_failure
        cd $LTE_ROOT/gtpuPacketRelay/egtpu/
        check_failure
    .   ./.egtpu_settings_linux
        make $OPT_S all
        cd -
        check_failure
        fi
        echo "***************************** ENDED COMPILING EGTPU ******************************"
        echo ""
        echo ""
        if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
        then
           
            sed -i "s/UHIGH_SCALABILITY_L2_MAX_UE/DHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
            sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
            
            echo "*************************** COMPILING FRAMEWORK LIB ***************************"
            cd $LTE_ROOT/common/
            make clean
            cd $LTE_ROOT/
        fi
        echo "*************************** COMPILING FRAMEWORK LIB ***************************"
        make $OPT_S $OPT_J -f ./common/Makefile FLAGS=-UGTP_SPLIT 
        check_failure
        echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
        echo ""
        echo ""
        echo "*************************** COMPILING LOGGING FRAMEWORK LIB ***************************"
        cd $LTE_ROOT/common/loggingFrameWork/
        make
        check_failure
        make $OPT_S $OPT_J utils
        check_failure
        cd -
		cp -f $LTE_ROOT/common/loggingFrameWork/bin/ltebinread $LTE_ROOT/../bin/
        echo "************************ ENDED COMPILIATION LOGGING FRAMEWORK LIB**************************"
        echo ""
        echo ""
        if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
        then 
        sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" make.inc
        sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
        fi
        echo "*************************** COMPILING RLC STACK ***************************"
        make $OPT_S $OPT_J -f ./rlc/rlccore/Makefile FLAGS=-DGET_TICK_FROM_MAC
        check_failure
        echo "************************ ENDED COMPILILATION RLC STACK *************************"
        echo ""
        echo ""
        echo "***************************COMPILING MAC STACK ***************************"
        make $OPT_S $OPT_J -f ./mac/maccore/Makefile FLAGS=-DZERO_BUFFER_COPY
        check_failure
        echo "************************ENDED COMPILING MAC STACK ***************************"
        echo ""
        echo ""
        if [ "0" == $RLC_PDCP_SPLIT_FLAG ]
        then    
        echo "***************************COMPILING PDCP STACK ***************************"
        make $OPT_S $OPT_J -f ./pdcp/Makefile
        check_failure
        echo "************************ENDED COMPILILATION PDCP STACK ***********************"
        fi

        echo ""
        echo ""
        if [ "enable" == $EMBMS_FEAT ];   then 
            echo "*************************** COMPILING SYNC MODULE ***************************"
            make -f ./sync/Makefile
            check_failure
            echo "************************ ENDED COMPILILATION SYNC MODULE ***********************"
            echo ""
            echo ""
        fi
        echo""
        echo ""
        echo "*************************** COMPILING CONTROL APP ***************************"
        make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile l2WithAdpt
        check_failure
        echo "*************************** ENDED COMPILIATION CONTROL APP ***************************"
        echo ""
        echo "*************************** COMPILING STATS TOOL ***************************"
        make $OPT_S $OPT_J -C ./simulators/statsTool
        check_failure
        echo "*************************** ENDED COMPILIATION STATS TOOL ***************************"
        echo ""
        echo "Copying L2 binary to ../bin folder"
		cp -f $LTE_ROOT/commonInterface/OAMApp/bin/lteLayer2 $LTE_ROOT/../bin/
        echo ""
        echo ""
fi
if [ $1 == $NBSIM ]; then
        sed -i "s/DLOG_PRINT_ENABLED/ULOG_PRINT_ENABLED/g" $LTE_ROOT/make.inc
        sed -i "s/DOVERSHOOT_CORRECTION/UOVERSHOOT_CORRECTION/g" $LTE_ROOT/make.inc
#SPR 21727 changes start
        sed -i "s/DDL_DATA_SEPARATION/UDL_DATA_SEPARATION/g" $LTE_ROOT/make.inc
        sed -i "s/DRLC_PDCP_IPC_INTF/URLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
#SPR 21727 changes end
        sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/DPDCP_ASYNC_INTR/UPDCP_ASYNC_INTR/g" $LTE_ROOT/make.inc
        sed -i "s/DPDCP_ASYNC_INTERFACE/UPDCP_ASYNC_INTERFACE/g" $LTE_ROOT/make.inc
        if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]; then
            sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        fi
#        sed -i "s/-Wall -fstack-protector-all/-Wall/g" $LTE_ROOT/make.inc
        if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
        then 
        sed -i "s/DCRAN_RLC_PDCP_SPLIT/UCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
        fi
        if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG]
        then
        sed -i "s/UHIGH_SCALABILITY_L2_MAX_UE/DHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L3_MAX_UE/UHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        fi
        echo "**************************** BUILDING NBSIM *********************"
        echo ""
        echo ""
        echo "*************************** COMPILING FRAMEWORK LIB ***************************"
        make $OPT_S $OPT_J -f ./common/Makefile.uesim FLAGS=-DUE_SIM_TESTING
        check_failure
        echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
        echo ""
        echo ""
        echo "*************************** COMPILING LOGGING FRAMEWORK LIB ***************************"
        cd $LTE_ROOT/common/loggingFrameWork/
        pwd
        make
        check_failure
        cd -
        echo "************************ ENDED COMPILIATION LOGGING FRAMEWORK LIB**************************"
        echo ""
        echo ""
	    echo "*************************** COMPILING RLC STACK ***************************"
        make $OPT_S $OPT_J -f ./rlc/rlccore/Makefile.uesim "FLAGS=-DGET_TICK_FROM_MAC -DUE_SIM_TESTING"
        check_failure
	    echo "************************ ENDED COMPILILATION RLC STACK *************************"
        echo ""
        echo ""
        echo "***************************COMPILING PDCP STACK ***************************"
# Flag ROHC_UT_FLAG used for enabling PDCP rohc traces on UeSim
        make $OPT_S $OPT_J -f ./pdcpUesim/Makefile "FLAGS=-DUE_SIM_TESTING -ULAYER2_PDCP_THREAD -UROHC_UT_FLAG"
        check_failure
        echo "************************ENDED COMPILILATION PDCP STACK ***********************"
        echo ""
        echo ""
        echo "***************************COMPILING PDCP ADAPTER LIB ***************************"
        make $OPT_S $OPT_J -f ./simulators/adapter/pdcp/Makefile.uesim FLAGS=-DUE_SIM_TESTING
        check_failure
        echo "************************ENDED COMPILIATION PDCP ADAPTER ***************************"
        echo ""
        echo ""
        echo "*************************** COMPILING OAM-RRC SIMULATOR***************************"
        make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamRrcExe CTAGS=-DBUILD_EXE
        check_failure
        make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamClientExe
        check_failure
        echo "************************ENDED COMPILILATION OAM SIMULATOR***************************"
        echo ""
        echo ""
        echo "*************************** COMPILING MAC EMULATOR ***************************"
        make $OPT_S $OPT_J -f ./simulators/macEmulator/Makefile macEmuLib
        check_failure
        echo "*************************** ENDED COMPILIATION MAC EMULATOR ***************************"
        echo ""
        echo ""
        echo "*************************** COMPILING UE SIMULATOR ***************************"
        make $OPT_S $OPT_J -f ./simulators/ueSim/src/Makefile uesimWithAdapter
        check_failure
        echo "*************************** ENDED COMPILIATION UE SIMULATOR ***************************"
        echo ""
        echo "Copying ueSim binary to ../bin folder"
		cp -f $LTE_ROOT/simulators/ueSim/bin/ueSim $LTE_ROOT/../bin/
        echo ""
        echo ""
fi
if [ $1 == $MAC ]; then
	echo "****************************BUILDING MAC+RLCSIM*********************"
	echo "***************************COMPILING FRAMEWORK***************************"
	make $OPT_S $OPT_J -f ./common/Makefile
    check_failure
	echo "************************ENDED COMPILING FRAMEWORK**************************"
	echo ""
	echo ""
	echo "***************************COMPILING MACCORE***************************"
	make $OPT_S $OPT_J -f ./mac/maccore/Makefile "FLAGS=-DRLC_SIM_TESTING -UZERO_BUFFER_COPY"
    check_failure
	echo "************************ENDED COMPILING MACCORE***************************"
	echo ""
	echo ""
	echo "***************************COMPILING RLCSIM***************************"
	make $OPT_S $OPT_J -f ./mac/simulator/rlc/Makefile 
    check_failure
	echo "************************ENDED COMPILING RLCSIM***************************"
	echo ""
	echo ""
	echo "***************************COMPILING PHY SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./mac/simulator/phy/Makefile
    check_failure
	echo "************************ENDED COMPILING PHY SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING RRC SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/rrc/Makefile "FLAGS=-DMAC_SIM_INDIVIDUAL -DRLC_SIM_TESTING"
    check_failure
	echo "************************ENDED COMPILING RRC SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/oam/Makefile
    check_failure
	echo "************************ENDED COMPILING OAM SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM APP***************************"
	make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile maclayer
    check_failure
	echo "************************ENDED COMPILING OAM APP***************************"
	echo ""
fi
if [ $1 == $RLC ]; then
	echo "****************************BUILDING RLC+MACSIM+PDCPSIM*********************"
	echo "***************************COMPILING FRAMEWORK***************************"
	make $OPT_S $OPT_J -f ./common/Makefile
    check_failure
	echo "************************ENDED COMPILING FRAMEWORK**************************"
	echo ""
	echo ""
	echo "***************************COMPILING RLCCORE***************************"
	make $OPT_S $OPT_J -f ./rlc/rlccore/Makefile  
    check_failure
	echo "************************ENDED COMPILING RLCCORE***************************"
	echo ""
	echo ""
	echo "***************************COMPILING MACSIM******************************"
	make $OPT_S $OPT_J -f ./rlc/simulator/mac/Makefile macSimLib
    check_failure
	echo "************************ENDED COMPILING MACSIM*****************************"
	echo ""
	echo ""
	echo "***************************COMPILING PDCP SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./rlc/simulator/pdcp/Makefile pdcpSimLib
    check_failure
	echo "************************ENDED COMPILING PDCP SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING RRC SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/rrc/Makefile FLAGS=-UMAC_SIM_INDIVIDUAL
    check_failure
	echo "************************ENDED COMPILING RRC SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM SIMULATOR***************************"
	make $OPT_S $OPT_J -f ./simulators/oam/Makefile
    check_failure
	echo "************************ENDED COMPILING OAM SIMULATOR***************************"
	echo ""
	echo ""
	echo "***************************COMPILING OAM APP***************************"
	make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile rlclayer
    check_failure
	echo "************************ENDED COMPILING OAM APP***************************"
	echo ""
fi
if [ $1 == $UESIM ]; then
    echo "*****************BUILDING PHYSIM + MACEMULATOR+RLC+PDCP+PDCP USER***********"
    echo ""
    echo ""
    echo "***************************COMPILING FRAMEWORK***************************"
    make $OPT_S $OPT_J -C ./common/
    check_failure
    echo "************************ENDED COMPILING FRAMEWORK**************************"
    echo ""
    echo ""
    echo "***************************COMPILING RLCCORE***************************"
    make $OPT_S $OPT_J -C ./rlc/rlccore/ FLAGS=-DGET_TICK_FROM_MAC
    check_failure
    echo "************************ENDED COMPILING RLCCORE***************************"
    echo ""
    echo ""
    echo "***************************COMPILING PDCP***************************"
    make $OPT_S $OPT_J -C ./pdcp/ 
    check_failure
    echo "************************ENDED COMPILING PDCP***************************"
    echo ""
    echo ""
    echo "***************************COMPILING MACEMULATOR***************************"
    make $OPT_S $OPT_J -C ./simulators/macEmulator/ macEmuLib FLAGS=-DZERO_BUFFER_COPY
    check_failure
    echo "************************ENDED COMPILING MACEMULATOR************************"
    echo ""
    echo ""
    echo "***************************COMPILING OAMRRC SIMULATOR***************************"
    make $OPT_S $OPT_J -C ./simulators/oamrrc/ oamRrcExe CTAGS=-DBUILD_EXE
    check_failure
    make $OPT_S $OPT_J -C ./simulators/oamrrc/ oamClientExe
    check_failure

    echo "************************ENDED COMPILING OAMRRC SIMULATOR***************************"
    echo ""
    echo ""
    echo "***************************COMPILING PHY SIMULATOR***************************"
    make $OPT_S $OPT_J -C ./simulators/phy/ uesim
    check_failure
    echo "************************ENDED COMPILING PHY SIMULATOR***************************"
    echo ""
    echo ""
fi
if [ $1 == $UESIMWITHADAPTER ]; then
	echo "**************************** BUILDING UESIM STACK *********************"
	echo ""
    echo ""
    echo "***************************COMPILING MACEMULATOR***************************"
    make $OPT_S $OPT_J -f ./simulators/macEmulator/Makefile macEmuLib FLAGS=-DZERO_BUFFER_COPY
    check_failure
    echo "************************ENDED COMPILING MACEMULATOR************************"
    echo ""
    echo ""
    echo "***************************COMPILING UE EXECUTION SCHEDULER***************************"
    make $OPT_S $OPT_J -f ./simulators/ueSim/src/Makefile uesimWithAdapter
    check_failure
    echo "************************ENDED COMPILING UE EXECUTION SCHEDULER ************************"
    echo ""
fi
if [ $1 == $PDCP ]; then

    if [ "1" == $DPDK_ENABLED ]
    then
        sed -i "s/UPDCP_DPDK/DPDCP_DPDK/g" $LTE_ROOT/make.inc
    else
        sed -i "s/DPDCP_DPDK/UPDCP_DPDK/g" $LTE_ROOT/make.inc
    fi
    # If RLC_PDCP SPLIT flag is set , PDCP need to be compiled independently #
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then
        sed -i "s/DRLC_PDCP_IPC_INTF/URLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
        sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
        sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        # Set the flag to indicate that we are compiling standalone PDCP
        export STANDALONE_PDCP=1

        echo "*************************** COMPILING FRAMEWORK LIB ***************************"
        cd $LTE_ROOT/common/
        make clean
	    check_failure

        cd $LTE_ROOT
        sed -i "s/DOVERSHOOT_CORRECTION/UOVERSHOOT_CORRECTION/g" $LTE_ROOT/make.inc
        make -f ./common/Makefile FLAGS=-DGTP_SPLIT 
        check_failure
        echo "************************ ENDED COMPILIATION FRAMEWORK **************************"

        echo "*************************** COMPILING LOGGING FRAMEWORK LIB ***************************"
        cd $LTE_ROOT/common/loggingFrameWork/
        make
        check_failure
        make utils
        check_failure
        cd -
		cp -f $LTE_ROOT/common/loggingFrameWork/bin/ltebinread $LTE_ROOT/../bin/
        echo "************************ ENDED COMPILIATION LOGGING FRAMEWORK LIB**************************"

        echo "***************************COMPILING PDCP STACK ***************************"
        make $OPT_S $OPT_J -f ./pdcp/Makefile
        check_failure
        echo "************************ENDED COMPILATION PDCP STACK ***********************"
        
	if [ "1" == $DPDK_ENABLED ]
	then
		echo "***************************COMPILING DPDK Framework ***************************"
		cd $LTE_ROOT/dpdk_framework/
		make
		check_failure
                cp $LTE_ROOT/dpdk_framework/common/mainlink/build/lte_pdcp $LTE_ROOT/../bin/
		cd -
		echo "************************ENDED COMPILATION Framework ***********************"
    	fi    
	exit 0
    fi

    echo "****************************BUILDING PDCP+PDCPUSER+RLCSIM*********************"
    echo ""
    echo ""
    echo "***************************COMPILING FRAMEWORK***************************"
    make $OPT_S $OPT_J -f ./common/Makefile
    check_failure
    echo "************************ENDED COMPILING FRAMEWORK**************************"
    echo ""
    echo ""
    echo "***************************COMPILING PDCP***************************"
    make $OPT_S $OPT_J -f ./pdcp/Makefile 
    check_failure
    echo "************************ENDED COMPILING PDCP***************************"
    echo ""
    echo ""
    echo "***************************COMPILING PDCP***************************"
    make $OPT_S $OPT_J -f ./pdcp/simulator/rlc/Makefile 
    check_failure
    echo "************************ENDED COMPILING PDCP***************************"
    echo ""
    echo ""
    echo "***************************COMPILING OAMRRC SIMULATOR***************************"
    make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamRrcExe CTAGS=-DBUILD_EXE
    check_failure
    make $OPT_S $OPT_J -f ./simulators/oamrrc/Makefile oamClientExe
    check_failure
    echo "************************ENDED COMPILING OAM SIMULATOR***************************"
    echo ""
    echo ""
    echo "***************************COMPILING OAM APP***************************"
    make $OPT_S $OPT_J -f ./commonInterface/OAMApp/Makefile pdcplayer
    check_failure
    echo "************************ENDED COMPILING OAM APP***************************"
    echo ""
fi
if [ $1 == $CLEANUP ]; then
    if [ $# -lt 2 ]
	then
        echo "Usage: ./build.sh clean <clean-option>(all/stacks/gtpu/l2/nbsim/l3/mme/oamsim_ue/tg/lte_rrm/lte_son/lte_oam)"
    exit 1
	else
	    if [ $2 == $GTPU ]
		then
    	    echo "***************************CLEANUP GTPU***************************"
            cd $LTE_ROOT/gtpuPacketRelay/packetRelay/
	        make $OPT_S $OPT_J -f Makefile clean
    	    cd $LTE_ROOT/gtpuPacketRelay/egtpu/
        	check_failure
	        . ./.egtpu_settings_linux
    	    find ./ -name *.[oad] -exec rm -f {} \;
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
            if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
            then
            cd $LTE_ROOT/gtpuPacketRelay/
            make clean
            cd $LTE_ROOT
            fi
	        echo "***************************ENDED CLEANUP GTPU***************************"
    	    echo ""

        elif [ $2 == $PDCP ] 
		then
            if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
            then
                echo "***************************CLEANUP PDCP***************************"
                cd $LTE_ROOT/pdcp
                make $OPT_S $OPT_J -f Makefile clean
                cd -
		if [ "1" == $DPDK_ENABLED ]
		then
			cd $LTE_ROOT/dpdk_framework
			make clean
			cd -
		fi
                echo "************************ENDED CLEANUP PDCP***************************"
            fi
        elif [ $2 == $L2WITHADAPTER ] 
		then
        	echo "****************************CLEANING UP LTE********************************"
        	echo ""
        	echo ""
        	echo "****************************CLEANING UP LIBFRAMEWORK********************************"
        	cd $LTE_ROOT/common
        	make $OPT_S $OPT_J clean
        	cd -
        	echo "****************************ENDED CLEANUP LIBFRAMEWORK********************************"
        	echo ""
        	echo ""
            $LTE_ROOT/build.sh clean gtpu
            check_failure
        	echo "***************************CLEANUP PDCP***************************"
        	cd $LTE_ROOT/pdcp
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP PDCP***************************"
        	echo ""
        	echo ""
        	echo "*************************** CLEANUP RLC STACK ***************************"
        	cd $LTE_ROOT/rlc/rlccore/
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ ENDED CLEANUP RLC STACK *************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP MAC STACK ***************************"
        	cd $LTE_ROOT/mac/maccore/
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP MAC STACK ***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP PDCP ADAPTER LIB ***************************"
        	cd $LTE_ROOT/simulators/adapter/pdcp
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP PDCP ADAPTER ***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP OAM APP***************************"
	        cd $LTE_ROOT/commonInterface/OAMApp
    	    make $OPT_S $OPT_J -f Makefile clean
        	cd -
       	 	echo "************************ENDED CLEANUP OAM APP***************************"
        	echo ""
        	echo "Removing L2 Binary from ../bin"
			rm -f $LTE_ROOT/../bin/lteLayer2
        	echo ""
        	echo "****************************ENDED CLEANUP LTE********************************"
        elif [ $2 == $NBSIM ] 
		then
        	echo "****************************CLEANING UP NBSIM********************************"
        	echo ""
        	echo ""
        	echo "*************************** CLEANUP RLC STACK ***************************"
        	cd $LTE_ROOT/rlc/rlccore/
        	make $OPT_S $OPT_J -f Makefile.uesim clean
        	cd -
        	echo "************************ ENDED CLEANUP RLC STACK *************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP PDCP***************************"
        	cd $LTE_ROOT/pdcpUesim
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP PDCP***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP PDCP ADAPTER LIB ***************************"
        	cd $LTE_ROOT/simulators/adapter/pdcp
        	make $OPT_S $OPT_J -f Makefile.uesim clean
        	cd -
        	echo "************************ENDED CLEANUP PDCP ADAPTER ***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP MAC EMULATOR ***************************"
        	cd $LTE_ROOT/simulators/macEmulator/
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP MAC EMULATOR ***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP UE SIMULATOR ***************************"
        	cd $LTE_ROOT/simulators/ueSim/src
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo "************************ENDED CLEANUP UE SIMULATOR ***************************"
        	echo ""
        	echo ""
        	echo "***************************CLEANUP UE LIB FRAMEWORK ***************************"
        	cd $LTE_ROOT/common
        	make $OPT_S $OPT_J -f Makefile.uesim clean 
        	cd -
        	echo "***************************ENDED CLEANUP UE LIB FRAMEWORK ***************************"
        	echo ""
        	echo ""
            echo "***************************CLEANUP STATS***************************"
            rm -rf $LTE_ROOT/simulators/statsTool/getStats
#SPR 6924 Changes Start 
            cd $LTE_ROOT/simulators/statsTool/
            make $OPT_S $OPT_J -f Makefile clean
            cd -
#SPR 6924 Changes End 
        	echo ""
        	echo ""
        	echo "Removing ueSim Binary from ../bin"
			rm -f $LTE_ROOT/../bin/ueSim
        	echo ""
        	echo "****************************ENDED CLEANUP NBSIM********************************"
        	echo ""
        	echo ""
        elif [ $2 == $OAMSIMUE ] 
		then
        	echo "***************************CLEANUP OAMRRC SIMULATOR***************************"
        	cd $LTE_ROOT/simulators/oamrrc
        	make $OPT_S $OPT_J -f Makefile clean
        	cd -
        	echo ""
        	echo "Removing oamsim_ue Binary from ../bin"
			rm -f $LTE_ROOT/../bin/oamSim
        	echo "************************ENDED CLEANUP OAM SIMULATOR***************************"
        	echo ""
        	echo ""
        elif [ $2 == $ENBRRC ] 
		then
        	echo "****************************CLEANING UP L3********************************"
	        cd $LTE_ROOT/lte_rrc
    	    . ./.settings
        	cd $LTE_ROOT/lte_rrc/sctp/sctp/build/linux
	        make $OPT_S $OPT_J -f Makefile clean
    	    cd $LTE_ROOT/lte_rrc/runner
        	make $OPT_S $OPT_J -f Makefile cleanall
			cd -
        	echo ""
        	echo "Removing L3 Binary from ../bin"
			rm -f $LTE_ROOT/../bin/rrc_entity
	        echo "***************************ENDED CLEANUP LAYER3***************************"
        	echo ""
        	echo ""
        elif [ $2 == $TR069 ] 
            then
        	echo "***************************CLEANUP TR069***************************"
                cd $LTE_ROOT/lte_rrc/cspl/build/
                make $OPT_S $OPT_J clean
        	echo "Removing TR069 Binary from ../bin"
			rm -f $LTE_ROOT/../bin/lte_tr069
			rm -f $LTE_ROOT/lte_tr069/libs/*.a
			rm -f $LTE_ROOT/lte_tr069/glue_layer/ipsec/src/*.o
			rm -f $LTE_ROOT/lte_tr069/glue_layer/ipsec/src/*.a
        	echo "***************************ENDED CLEANUP TR069***************************"
        	echo ""
        	echo ""
        elif [ $2 == $ENBSON ] 
		then
        	echo "***************************CLEANUP LTE_SON***************************"
            cd $LTE_ROOT/lte_rrc/cspl/build/
            make $OPT_S $OPT_J clean
            cd $LTE_ROOT/sib_decoder/make/
            make $OPT_S $OPT_J clean           
            cd $LTE_ROOT/umts_sib_decoder/make/
            make $OPT_S $OPT_J clean
        	cd $LTE_ROOT/lte_son
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
        	echo ""
        	echo "Removing LTE_SON Binary from ../bin"
            rm -f $LTE_ROOT/../bin/lte_son
        	echo "Removing SON configuarion file from ../cfg"
            rm -f $LTE_ROOT/../cfg/son_oam.cfg
            echo "Removing asn libraries from SON lib directory"
            rm -f $LTE_ROOT/lte_son/build/lib/*.so
        	echo "***************************ENDED CLEANUP LTE_SON***************************"
        	echo ""
        	echo ""
        elif [ $2 == $ENBLTERRM ] 
		then
        	echo "***************************CLEANUP LTE_RRM***************************"
        	cd $LTE_ROOT/lte_rrm
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
        	echo ""
            echo "Removing rrm cfg files from ../cfg"
            rm -f $LTE_ROOT/../cfg/rrm1.cfg
            rm -f $LTE_ROOT/../cfg/rrm2.cfg
            rm -f $LTE_ROOT/../cfg/rrm3.cfg
            rm -f $LTE_ROOT/../cfg/rrm4.cfg
            rm -f $LTE_ROOT/../cfg/rrm5.cfg
            rm -f $LTE_ROOT/../cfg/rrm6.cfg
            rm -f $LTE_ROOT/../cfg/rrm7.cfg
            rm -f $LTE_ROOT/../cfg/rrm8.cfg
        	echo "Removing LTE_RRM Binary from ../bin"
            rm -f $LTE_ROOT/../bin/lte_rrm
        	echo "***************************ENDED CLEANUP LTE_RRM***************************"
        	echo ""
        	echo ""
        elif [ $2 == $MME ] 
		then
        	echo "***************************CLEANUP MME***************************"
	        cd $LTE_ROOT/simulators/protocolSimulatorTool
    	    export SIM_ROOT=$PWD
        	export CSPL_DIR=$LTE_ROOT/lte_rrc/cspl/
	        export RRC_ROOT=$LTE_ROOT/lte_rrc
    	    export SIGTRAN_DIR=$LTE_ROOT/lte_rrc/sctp
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
        	echo ""
        	echo "Removing MME Binary from ../bin"
			rm -f $LTE_ROOT/../bin/protocolSimulator
       		echo "***************************ENDED CLEANUP MME***************************"
       		echo ""
       		echo ""
        elif [ $2 == $TG ] 
		then
        	echo "*************************** CLEANUP TRAFFIC GEN APP ***************************"
        	cd $LTE_ROOT/simulators/trafficGenerator/
        	make $OPT_S $OPT_J -f Makefile clean
			rm -f bin/ulTrafficGen
			rm -f bin/dlTrafficGen
			rm -f bin/lgwDlTrafficGen
			cd -
        	echo ""
        	echo "Removing UL and DL Traffic Generator Binaries from ../bin"
			rm -f $LTE_ROOT/../bin/ulTrafficGen
			rm -f $LTE_ROOT/../bin/dlTrafficGen
			rm -f $LTE_ROOT/../bin/lgwDlTrafficGen
        	echo "*************************** ENDED CLEANUP TRAFFIC GEN APP ***************************"
        	echo ""
        	echo ""
        elif [ $2 == $ENBLTEOAM ]
            then
            echo "***************************CLEANUP LTE_OAM***************************"
            cd $LTE_ROOT/lte_oam
            make $OPT_S $OPT_J clean-all
            echo ""
            echo "Removing OAM Binary from ../bin"
            echo "***************************ENDED CLEANUP LTE_OAMCLIINTF***************************"
            echo ""
            echo ""


        elif [ $2 == $ALL ]
		then
        	echo "***************************CLEANUP FRAMEWORK***************************"
        	cd $LTE_ROOT/common
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
        	cd $LTE_ROOT/common
        	make $OPT_S $OPT_J -f Makefile.uesim clean
			cd -
            cd $LTE_ROOT/common/loggingFrameWork/
            make $OPT_S $OPT_J clean
			cd -
            rm -f $LTE_ROOT/../bin/ltecat
            rm -f $LTE_ROOT/../bin/ltetail
        	echo "************************ENDED CLEANUP FRAMEWORK**************************"
        	echo ""
        	echo ""
        	echo "***************************ENDED CLEANUP CSPL***************************"
            $LTE_ROOT/build.sh clean gtpu
            check_failure
            $LTE_ROOT/build.sh clean l2
            check_failure
	    $LTE_ROOT/build.sh clean pdcp
            check_failure
	    $LTE_ROOT/build.sh clean nbsim
            check_failure
            $LTE_ROOT/build.sh clean l3
            check_failure
            $LTE_ROOT/build.sh clean tg
            check_failure
            $LTE_ROOT/build.sh clean mme
            check_failure
            $LTE_ROOT/build.sh clean oamsim_ue
            check_failure
            $LTE_ROOT/build.sh clean lte_rrm
            check_failure
            $LTE_ROOT/build.sh clean lte_son
            check_failure
            $LTE_ROOT/build.sh clean lte_oam
            check_failure
            $LTE_ROOT/build.sh clean lte_tr069
            check_failure
	    cd $LTE_ROOT/common/in_memory_logging/
	    make $OPT_S $OPT_J clean
	    check_failure

        elif [ $2 == $STACKS ]
		then
        	echo "***************************CLEANUP FRAMEWORK***************************"
        	cd $LTE_ROOT/common
        	make $OPT_S $OPT_J -f Makefile clean
			cd -
            cd $LTE_ROOT/common/loggingFrameWork/
            make $OPT_S $OPT_J clean
			cd -
            rm -f $LTE_ROOT/../bin/ltecat
            rm -f $LTE_ROOT/../bin/ltetail
        	echo "************************ENDED CLEANUP FRAMEWORK**************************"
        	echo ""
        	echo ""
        	echo "***************************ENDED CLEANUP CSPL***************************"
            $LTE_ROOT/build.sh clean gtpu
            check_failure
            $LTE_ROOT/build.sh clean l2
            check_failure
            $LTE_ROOT/build.sh clean l3
            check_failure
            $LTE_ROOT/build.sh clean lte_rrm
            check_failure
            $LTE_ROOT/build.sh clean lte_son
            check_failure
            $LTE_ROOT/build.sh clean lte_oam
            check_failure
            $LTE_ROOT/build.sh clean lte_tr069
            check_failure
	        cd $LTE_ROOT/common/in_memory_logging/
	        make $OPT_S $OPT_J clean
		else
            echo "Usage: ./build.sh clean <clean-option>(all/stacks/gtpu/l2/nbsim/l3/mme/oamsim_ue/tg/lte_rrm/lte_son/lte_oam)"
        exit 1
		fi
	fi
fi

if [ $1 = $MME ]
then
    cd $LTE_ROOT/simulators/protocolSimulatorTool
    check_failure
    export SIM_ROOT=$PWD
    export CSPL_DIR=$LTE_ROOT/lte_rrc/cspl/
    export RRC_ROOT=$LTE_ROOT/lte_rrc
    export SIGTRAN_DIR=$LTE_ROOT/lte_rrc/sctp
    make $OPT_S $OPT_J 
    check_failure
    cd -
    echo ""
    echo "Copying MME binary to ../bin folder"
	cp -f $LTE_ROOT/simulators/protocolSimulatorTool/bin/protocolSimulator $LTE_ROOT/../bin/
    echo ""
    echo ""
fi

if [ $1 = $GTPU ]
then
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
    then
	sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    fi
    echo "********************************* COMPILING EGTPU ******************************"
    cd $LTE_ROOT/gtpuPacketRelay/packetRelay/
    make
    check_failure
    cd $LTE_ROOT/gtpuPacketRelay/egtpu/
    . ./.egtpu_settings_linux
    make all
    check_failure
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc
    fi
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]
    then
    echo "*************************** COMPILING FRAMEWORK LIB ***************************"
        sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
        sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc

        cd $LTE_ROOT/common/
        make clean
	    check_failure
        cd $LTE_ROOT
        sed -i "s/DOVERSHOOT_CORRECTION/UOVERSHOOT_CORRECTION/g" $LTE_ROOT/make.inc
        make -f ./common/Makefile FLAGS=-DGTP_SPLIT 
        check_failure
        echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
        echo ""
        echo ""
        echo "*************************** COMPILING LOGGING FRAMEWORK LIB ***************************"
        cd $LTE_ROOT/common/loggingFrameWork/
        make CFLAGS+=-DGTP_SPLIT
        check_failure
        make utils CFLAGS+=-DGTP_SPLIT
        check_failure
        cd -
		cp -f $LTE_ROOT/common/loggingFrameWork/bin/ltebinread $LTE_ROOT/../bin/
        echo "************************ ENDED COMPILIATION LOGGING FRAMEWORK LIB**************************"
    
        echo "*************************** COMPILING GTP MAIN ***************************"
        make -f ./gtpuPacketRelay/Makefile gtpu
        check_failure
        echo "*************************** COMPILATION SUCCESSFUL ***********************"
    fi
fi

if [ $1 = $OAMSIMUE ]
then
    echo "*************************** COMPILING FRAMEWORK LIB ***************************"
    make $OPT_S $OPT_J -f ./common/Makefile.uesim FLAGS=-DUE_SIM_TESTING
    check_failure
    echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
    echo ""
    make $OPT_S $OPT_J -C $LTE_ROOT/simulators/oamrrc oamRrcExe CTAGS=-DBUILD_EXE FLGS=""
    check_failure
    cp -f $LTE_ROOT/simulators/oamrrc/bin/oamSim $LTE_ROOT/simulators/oamrrc/bin/oamSim_ue
    check_failure
    echo ""
    echo "Copying oamsim_ue binary to ../bin folder"
	cp -f $LTE_ROOT/simulators/oamrrc/bin/oamSim $LTE_ROOT/../bin/
    echo ""
    echo ""
fi

if [ $1 = $TG ]
then
    echo "*************************** COMPILING FRAMEWORK LIB ***************************"
    sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    make $OPT_S $OPT_J -f ./common/Makefile.uesim FLAGS=-DUE_SIM_TESTING
    check_failure
    echo "************************ ENDED COMPILIATION FRAMEWORK **************************"
    echo ""
    echo "************************* COMPILING TRAFFIC GEN APP ****************************"
    echo ""
    make $OPT_S $OPT_J -f ./simulators/trafficGenerator/Makefile dlTrafficGenExe
    check_failure
    make $OPT_S $OPT_J -f ./simulators/trafficGenerator/Makefile lgwDlTrafficGenExe
    check_failure
    make $OPT_S $OPT_J -f ./simulators/trafficGenerator/Makefile ulTrafficGenExe
    check_failure
    echo "********************** ENDED COMPILIATION TRAFFIC GEN *************************"
    echo ""
    echo "Copying UL, DL and LGW traffic generator binaries to ../bin folder"
	cp -f $LTE_ROOT/simulators/trafficGenerator/bin/ulTrafficGen $LTE_ROOT/../bin/
	cp -f $LTE_ROOT/simulators/trafficGenerator/bin/dlTrafficGen $LTE_ROOT/../bin/
	cp -f $LTE_ROOT/simulators/trafficGenerator/bin/lgwDlTrafficGen $LTE_ROOT/../bin/
    echo ""
    echo ""
fi

if [ $1 = $REAL_EPC_TG ]
then 
    echo "************************* COMPILING TRAFFIC GEN APP ****************************"
    echo ""
    make $OPT_S $OPT_J -f ./simulators/trafficGenerator/Makefile dlTgGenExe
    check_failure
    echo "Copying DL traffic generator binaries to ../bin folder"
	cp -f $LTE_ROOT/simulators/trafficGenerator/bin/dlTgGen $LTE_ROOT/../bin/
    echo ""
    echo ""
fi


if [ $1 = $ALL ]
then
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 

    sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
    sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc

    $LTE_ROOT/build.sh l2 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure

    sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
    sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
    
    sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    $LTE_ROOT/build.sh gtpu $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure

    $LTE_ROOT/build.sh pdcp $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure

    $LTE_ROOT/build.sh lte_rrm $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_son $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_oam $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    #$LTE_ROOT/build.sh lte_tr069 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    #check_failure
    $LTE_ROOT/build.sh l3 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure

    sed -i "s/-DSSI_DEBUG_STATS/-USSI_DEBUG_STATS/g" $LTE_ROOT/make.inc              
    $LTE_ROOT/build.sh nbsim $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh tg $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh oamsim_ue $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh mme $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure

    else

    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]; then
    sed -i "s/UHIGH_SCALABILITY_L2_MAX_UE/DHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
    fi
    $LTE_ROOT/build.sh l2 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    if [ "1" == $SET_PDCP_GTPU_SPLIT_FLAG ]; then
    sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
    sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
        $LTE_ROOT/build.sh gtpu $DUPLEXING_MODE $PLATFORM $SCTP_STACK
        check_failure
    fi
    sed -i "s/-DSSI_DEBUG_STATS/-USSI_DEBUG_STATS/g" $LTE_ROOT/make.inc              
    $LTE_ROOT/build.sh nbsim $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh l3 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh tg $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh oamsim_ue $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh mme $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_rrm $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_son $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_oam $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_tr069 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    fi
fi

if [ $1 = $STACKS ]
then
    if [ "1" == $RLC_PDCP_SPLIT_FLAG ]
    then 
    mkdir ../build_logs/
    sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
    sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" $LTE_ROOT/make.inc

    $LTE_ROOT/build.sh l2 $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/L2.log
    check_failure

    sed -i "s/DHIGH_SCALABILITY_L2_MAX_UE/UHIGH_SCALABILITY_L2_MAX_UE/g" $LTE_ROOT/make.inc
    sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" $LTE_ROOT/make.inc
    
    sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
    $LTE_ROOT/build.sh gtpu $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/GTPU.log
    check_failure

    $LTE_ROOT/build.sh pdcp $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/PDCP.log
    check_failure

    $LTE_ROOT/build.sh lte_rrm $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/RRM.log
    check_failure
    $LTE_ROOT/build.sh lte_son $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/SON.log
    check_failure
    $LTE_ROOT/build.sh lte_oam $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/OAM.log
    check_failure
  #  $LTE_ROOT/build.sh lte_tr069 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
  #  check_failure
    $LTE_ROOT/build.sh l3 $DUPLEXING_MODE $PLATFORM $SCTP_STACK 2>&1 | tee ../build_logs/L3.log
    check_failure
    
    else

    $LTE_ROOT/build.sh l2 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh l3 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_rrm $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_son $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_oam $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
    $LTE_ROOT/build.sh lte_tr069 $DUPLEXING_MODE $PLATFORM $SCTP_STACK
    check_failure
   
    fi
fi
