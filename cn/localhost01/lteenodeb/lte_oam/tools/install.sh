#**********************************************************************************#
# DISCRIPTION: Script to manually install a package.
# AUTHOR     : VISHWA PRAKASH PANDEY
# DATE       : 26-02-2013
#**********************************************************************************#

#!/bin/bash
if [ $# -ne 2 ]
then
    echo -e "\n**************************************************************************** "
    echo -e "Usage - $0 <build name without extension> <Path To install>"
    echo -e "\n**************************************************************************** "
    echo -e "\nFor installation Build path and Script path should be same : "
    echo -e "\n e.g. if the package name is x_x_x.tar.gz "
    echo -e "\n      use command:./install.sh x_x_x <installation-path>"
    echo -e "\n"
    echo -e "\n Note: The <installation-path> shall not end with '/'"
    echo -e "\n**************************************************************************** "
    exit 1
fi

OLD_DIR=$(pwd)
cd $2;
if [ "$?" = "0" ]; then
cd -;
echo -e "\nA package is already installed..."
echo -e "\nThe contents of $2 will be removed. Do you want to continue:(yes/no)"
read continue
if [ $continue == yes ]; then
        echo -e "\nRemoving the existing package...";
        rm -rf $2;
          if [ "$?" = "0" ]; then
           echo -e "\nrm command success."
        else
           echo -e "\nrm command failed."
        exit 1
        fi
else
echo "exiting!!!"
exit 1
fi
fi


if `mkdir -p $2`;
then
echo -e "\nDirectory created by name $2"
else
echo -e "\nDirectory $2 creation failed..."
exit 1
fi

cd $2;
if [ "$?" = "0" ]; then
   echo -e "\ninside $PWD directory."
else
   echo -e "\ncd command t0 $2 failed..."
exit 1
fi


cp $OLD_DIR/$1.tar.gz  .;
if [ "$?" = "0" ]; then
echo -e "\ncopy successful."
else
echo -e "\ncopy failed..."
        exit 1
fi

echo -e "\nextracting $1.tar.gz"
if `tar -xzf $1.tar.gz`;
then
echo -e "\nextracting successful."
else
echo -e "\nextracting failed..."
exit 1
fi

echo -e "\nextracting $1.tar"
if `tar -xf $1.tar`;
then
echo -e "\nextracting successful.."
else
echo -e "\nextracting failed..."
exit 1
fi


##################################
#Removing the existing binary files
##################################
echo -e "Removing the existing binary files"
rm -rf $1/bin/*;

##################################
#Removing the existing configuration files
##################################
echo -e "Removing the existing configuration files"
rm -rf $1/cfg/*;

##################################
#Removing the existing factory-configuration files
##################################
echo -e "Removing the existing factory-configuration files"
rm -rf $1/factory/*;

rm -rf checksum.txt;

if [ "$?" = "0" ]; then
echo -e "\ncopy successful."
else
echo -e "\ncopy failed..."
exit 1
fi

echo -e "cleaning up"
rm -rf $1.tar.gz
rm -rf $1.tar






