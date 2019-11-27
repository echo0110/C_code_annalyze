export PATH=$PATH:/home/zhanghh/arm_tools/gcc-linaro-7.4.1-2019.02-i686_arm-linux-gnueabi/bin
export LTE_ROOT=`pwd`
export LTE_AR=arm-linux-gnueabi-ar rv
export LTE_RANLIB=ranlib
export HOST_COMPILATION=1
export GTPU_SPLIT=1
export SET_PDCP_GTPU_SPLIT_FLAG=1
export RLC_PDCP_SPLIT_FLAG=0
export DPDK_ENABLED=0
export ASTRI_FLEXRAN=1
export ARM_INCLUDE=/home/zhanghh/arm_tools/sysroot-glibc-linaro-2.25-2019.02-arm-linux-gnueabi/usr/include
export ARM_LIB="-L/home/zhanghh/arm_tools/gcc-linaro-7.4.1-2019.02-i686_arm-linux-gnueabi/arm-linux-gnueabi/libc/usr/lib -L/home/zhanghh/arm_tools/gcc-linaro-7.4.1-2019.02-i686_arm-linux-gnueabi/arm-linux-gnueabi/libc/lib"
export ARM_LIB
export WORK_DIR=$LTE_ROOT/../..
export LINUX_PC_TEST=false
export PROJ_MODE=debug
export ARM_CFLAGS=-mcpu=cortex-a7
export LTE_LOG_DIR=$WORK_DIR/LTE_IPR/lteenodeb/common/lib
if [ $LINUX_PC_TEST == "true" ]; then
    export TEST_INC_DIR=$WORK_DIR/TEST/Frame
    export TEST_LIB_DIR=$WORK_DIR/TEST/lib
fi

if [ $ASTRI_FLEXRAN == 1 ]; then

    echo "ASTRI FLEXRAN is set"

fi


ARCH=arm

export LTE_CC=arm-linux-gnueabi-gcc
export LTE_MAKE_BIN=arm-linux-gnueabi-gcc
export LTE_LD='arm-linux-gnueabi-ld -r'
export BIT_64_SYSTEM=0

echo "KNOWN_ARCH variable changed to $ARCH in simulators/protocolSimulatorTool/flags.mk"

sed -i "s/URLC_PDCP_IPC_INTF/DRLC_PDCP_IPC_INTF/g" make.inc
sed -i "s/DDL_DATA_SEPARATION/UDL_DATA_SEPARATION/g" make.inc
sed -i "s/UPDCP_GTPU_INTF/DPDCP_GTPU_INTF/g" make.inc
# SPR 22575 Changes End #
sed -i "s/UHIGH_SCALABILITY_L2_MAX_UE/DHIGH_SCALABILITY_L2_MAX_UE/g" make.inc
sed -i "s/UHIGH_SCALABILITY_L3_MAX_UE/DHIGH_SCALABILITY_L3_MAX_UE/g" make.inc
# SPR 22575 Changes End #
sed -i "s/DLINUX_PC_TEST/ULINUX_PC_TEST/g" $LTE_ROOT/make.inc
