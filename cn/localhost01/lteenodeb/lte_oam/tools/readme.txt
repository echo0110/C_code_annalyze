###############################################################################
#-----------------------------------------------------------------------------#
# File Name   : readme.txt                                                    #
# Description : This file contains steps to run scripts                       #
#-----------------------------------------------------------------------------#
# Date        Author               Comments                                   #
# -----       ------               --------                                   #
# 31/1/2013   OAM SDM Devlopement  Initial Version                            #
# 11/9/2013   OAM Team             Updates as per EVM Target requirements     #
#                                                                             #
# Copyright (c) 2013, Aricent Group.                                          #
###############################################################################

===============================================================================
1. create_eNB_package.sh
===============================================================================

Description:This script will create the software package with the given Version 
            Info by the user.

Inputs    : version information 
            <ManufacturerOUI> <ProductClass> <HardwareVersion> <SoftwareVersion>

Note      : 1. Please run this script from the directory where bin/ cfg/ and 
               factory/ folder exist.
               a) copy the following files in the bin directory
               - lte_oamSDM
               - lte_oam
               - lte_rrc
               - lte_rrm
               - lte_son
               - rrc_entity
               - lte_oamSnmpAgt
               - lte_oamCli
               - lteLayer2
                              
               b) copy the required configuration files in the cfg directory
               
               c) update the files cfg_filelist filelist for cfg directory and bin directory contents respectively. 

            2. incase any version info not available please provide <_> in place of that

example   : 1.if correspondig version info is a, b, c, d then please run script 
              as ./release.sh a b c d
            2.if any version info not available e.g HardwareVersion not available
              run script as ./release.sh a b _ d

=================================================================================

=================================================================================
2. install.sh
=================================================================================

Description : This script will install the software package to the user defined 
              directory path.

Inputs      : 1. Software package tar name without extension.
              2. Path of the directory where to install the software package.

Note        : 1. The script path and the software package path shall be present at the same path.
              2. The The script path and the software package path shall be present at the same path.

example     : if package name is a_b_c_d.tar.gz then please run script as
              ./install.sh a_b_c_d <installation-path>
=================================================================================


=================================================================================
3. uninstall.sh
=================================================================================

Description : This script will uninstall the software package to the
user defined directory path.

Inputs      1. Path of the directory where software is installed

example     : 
./uninstall.sh <path of installed package>
=================================================================================





