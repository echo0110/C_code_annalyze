#*****************************************************************************************************#
# DISCRIPTION: Script to make software package.
# AUTHOR     : VISHWA PRAKASH PANDEY
# DATE       : 26-02-2013
#*****************************************************************************************************#

#!/bin/bash

binpath="bin/"
cfgpath="cfg/"
factorypath="factory/"
bin_filelist=""
cfg_filelist=""
factory_filelist=""
space=" "
newline='\n'

if [ $# -ne 4 ]
then
    echo -e "\n***************************************************************************************** "
    echo -e "Usage - create_eNB_package.sh <ManufacturerOUI> <ProductClass> <HardwareVersion> <SoftwareVersion>"
    echo -e "\n***************************************************************************************** "
    echo -e "\nPlease Run this script from base directory where software exists"
    echo -e "\nBefore running this script, please ensure that:"
    echo -e "\n a) bin/ directory contains all the required binary files"
    echo -e "\n b) cfg/ directory contains all the required configuration files"
    echo -e "\n"
    echo -e "\nAt the time of using this script, please provide the version information as per the following format:"
    echo -e "\n OUI - manufacturer ID, mandatory field. String of 6 characters."
    echo -e "\n Product Class (Optional) - Signifies the unique per manufacturer serial number of the product, String of max 64 characters."
    echo -e "\n Hardware Version (Optional) - Signifies a specific eNB hardware version, String of max 64 characters."
    echo -e "\n Software Version - specific HeNB software version, String of max 64 characters."
    echo -e "\n     Format: <Package Name>-<Package Version> where Package Name is user provided string and Package Version format: major.minor.build."
    echo -e "\n"
    echo -e "\n Note: In case any version information is not provided, underscore(_) shall be provided in place of that." 
    echo -e "\n       e.g. if Hardware Version inot available, use the command as ./create_eNB_package.sh XXXXXX YYYY _ R16451-02.02.01"
    echo -e "\n"
    echo -e "\n***************************************************************************************** "
    exit 1
    
fi
echo -e "$PWD"
echo -e "\nversion building started.."
echo -e "\nManufacturerOUI :$1"
echo -e "\nProductClass    :$2"
echo -e "\nHardwareVersion :$3"
echo -e "\nSoftwareVersion :$4"
sleep 5;


while read LINE
do
  LINE=${LINE%newline}
  LINE="$binpath$LINE$space"
  bin_filelist="$bin_filelist$LINE"
done <filelist
echo $bin_filelist

while read LINE
do
  LINE=${LINE%newline}
  LINE="$cfgpath$LINE$space"
  cfg_filelist="$cfg_filelist$LINE"
done <cfg_filelist
echo $cfg_filelist

while read LINE
do
  LINE=${LINE%newline}
  LINE="$factorypath$LINE$space"
  factory_filelist="$factory_filelist$LINE"
done <cfg_filelist
echo $factory_filelist

mkdir $factorypath
echo "copying cfg files to factory folder "
cp -rf $cfg_filelist $factorypath

#if `tar -cf $1_$2_$3_$4.tar  bin/ cfg/ factory/`;

if `tar -cf $1_$2_$3_$4.tar  $bin_filelist $cfg_filelist $factory_filelist`;
then
   echo -e "\nRelease tar $1_$2_$3_$4.tar prepared successfully in path $PWD"
else
   echo -e "\nRelease tar $1_$2_$3_$4.tar preparation failed"
   exit 1
fi

###To calculate checksum md5sum command is used here and in the OAM code.###
check_sum=`md5sum $1_$2_$3_$4.tar |cut -d" " -f1`
echo "++++++++++++++++++++++++++++++++++++++++" > checksum.txt
echo "" >> checksum.txt
echo "Please do not edit this file manually!!!" >> checksum.txt
echo "" >> checksum.txt
echo "CHECKSUM=${check_sum}" >> checksum.txt
echo "" >> checksum.txt
echo "++++++++++++++++++++++++++++++++++++++++" >> checksum.txt


if `tar -czf $1_$2_$3_$4.tar.gz $1_$2_$3_$4.tar checksum.txt`;
then
echo -e "\nPackage prepared successfully by name $1_$2_$3_$4.tar.gz in path $PWD"
else
echo -e "\nPackage preparation failed."
        exit 1
	fi

rm -rf $1_$2_$3_$4.tar;
if [ "$?" = "0" ];
then
echo -e "\nRemoved $1_$2_$3_$4.tar"
else
echo -e "\nTar $1_$2_$3_$4.tar removal failed."
        exit 1
fi

rm -rf $factorypath
if [ "$?" = "0" ];
then
echo -e "\nRemoved factory folder"
else
echo -e "\nfactory folder removal failed."
exit 1
fi

rm -rf checksum.txt;
if [ "$?" = "0" ];
then
echo -e "\nRemoved checksum.txt"
else
echo -e "\nchesksum.txt file removal failed."
exit 1
fi




