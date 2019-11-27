#**********************************************************************************#
# DISCRIPTION: Script to uninstall package.
# AUTHOR     : VISHWA PRAKASH PANDEY
# DATE       : 26-02-2013
#**********************************************************************************#
#!/bin/bash
if [ $# -ne 1 ]
then
echo -e "\n**************************************************************************** "
echo -e "<Please Provide Path name to the Build Folder>"
echo -e "Usage - $0   <Path name to be uninstalled>"
echo -e " Note: Path name shall not end with '/'"
echo -e "\n**************************************************************************** "
exit 1
fi

echo -e "\nThe contents of $1 will be removed. Do you want to continue:(yes/no)"
read continue
if [ $continue == yes ]; then
    echo -e "\nUninstallation started."
else
    echo -e "\nUninstallation stopped."
    echo "exiting!!!"
    exit 1
fi


##################################
#Removing the existing binary files
##################################
echo -e "Removing the existing binary files"
if `rm -rf $1/bin/*`;then
echo -e "\n$1/bin directory removed successfully"
else
echo -e "\ncannot remove $1/bin/"
exit 1
fi
##################################
#Removing the existing configuration files
##################################
echo -e "Removing the existing configuration files"
if `rm -rf $1/cfg/*`;then
echo -e "\n$1/cfg directory removed successfully"
else 
echo -e "\ncannot remove $1/cfg"
exit 1
fi

##################################
#Removing the existing factory-configuration files
##################################
echo -e "Removing the existing factory-configuration files"
if `rm -rf $1/factory/*`;then
echo -e "\n$1/factory directory removed successfully"
echo -e "\nUninstallation done."
else
echo -e "\ncannot remove $1/factory"
exit 1
fi

