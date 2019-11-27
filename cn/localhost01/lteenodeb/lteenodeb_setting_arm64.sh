export PATH=$PATH:/home/zhanghh/gcc-linaro-aarch64/bin/
export LTE_ROOT=`pwd`
export LTE_AR=aarch64-linux-gnu-ar rv
export LTE_RANLIB=ranlib
export HOST_COMPILATION=1
export GTPU_SPLIT=1
export SET_PDCP_GTPU_SPLIT_FLAG=1
export RLC_PDCP_SPLIT_FLAG=0
export DPDK_ENABLED=0
export ASTRI_FLEXRAN=1

export WORK_DIR=$LTE_ROOT/../..
export LINUX_PC_TEST=false
export PROJ_MODE=debug

export LTE_LOG_DIR=$WORK_DIR/LTE_IPR/lteenodeb/common/lib
if [ $LINUX_PC_TEST == "true" ]; then
    export TEST_INC_DIR=$WORK_DIR/TEST/Frame
    export TEST_LIB_DIR=$WORK_DIR/TEST/lib
fi

if [ $ASTRI_FLEXRAN == 1 ]; then

    echo "ASTRI FLEXRAN is set"

fi


ARCH=aarch64

export LTE_CC=aarch64-linux-gnu-gcc
export LTE_MAKE_BIN=aarch64-linux-gnu-gcc
export LTE_LD='aarch64-linux-gnu-ld -r'
export BIT_64_SYSTEM=1

echo "KNOWN_ARCH variable changed to $ARCH in simulators/protocolSimulatorTool/flags.mk"

sed -i "s/UDL_DATA_SEPARATION/DDL_DATA_SEPARATION/g" make.inc
sed -i "s/UCRAN_RLC_PDCP_SPLIT/DCRAN_RLC_PDCP_SPLIT/g" make.inc
sed -i "s/DKPI_STATS/UKPI_STATS/g" make.inc
sed -i "s/DPERF_STATS/UPERF_STATS/g" make.inc
sed -i "s/DPDCP_GTPU_INTF/UPDCP_GTPU_INTF/g" $LTE_ROOT/make.inc
if [ $LINUX_PC_TEST == "true" ]; then
    sed -i "s/ULINUX_PC_TEST/DLINUX_PC_TEST/g" $LTE_ROOT/make.inc
else
    sed -i "s/DLINUX_PC_TEST/ULINUX_PC_TEST/g" $LTE_ROOT/make.inc
fi
sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" $LTE_ROOT/make.inc
