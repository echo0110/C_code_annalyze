# Microsoft Developer Studio Project File - Name="common_sig_os_util" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=common_sig_os_util - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "common_sig_os_util.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common_sig_os_util.mak" CFG="common_sig_os_util - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "common_sig_os_util - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "common_sig_os_util - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "common_sig_os_util"
# PROP Scc_LocalPath "..\..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "common_sig_os_util - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\h" /I "..\..\..\winnt\h" /I "$(CSPL_DIR)\h" /I "$(CSPL_DIR)\ports\win32" /I "..\..\..\..\gw\genlib\common\h" /I "..\..\..\..\gw\genlib\os\h" /I "..\..\..\..\gw\common\h" /D "NDEBUG" /D "GW_WINNT" /D "GL_WINNT" /D "WIN32" /D "_MBCS" /D "_LIB" /D "SIGTRAN_PORT_WINNT" /D "SIG_COMMON_WITH_CSPL" /D "SIGTRAN_PORT_WINXP" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\obj\winnt\common_sig_os_util.lib"

!ELSEIF  "$(CFG)" == "common_sig_os_util - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\h" /I "..\..\..\winnt\h" /I "$(CSPL_DIR)\h" /I "$(CSPL_DIR)\ports\win32" /I "..\..\..\..\gw\genlib\common\h" /I "..\..\..\..\gw\genlib\os\h" /I "..\..\..\..\gw\common\h" /D "_DEBUG" /D "GW_WINNT" /D "GL_WINNT" /D "WIN32" /D "_MBCS" /D "_LIB" /D "SIGTRAN_PORT_WINNT" /D "SIG_COMMON_WITH_CSPL" /D "SIGTRAN_PORT_WINXP" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\obj\winnt\common_sig_os_util.lib"

!ENDIF 

# Begin Target

# Name "common_sig_os_util - Win32 Release"
# Name "common_sig_os_util - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\sig_hash.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\h\sig_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\h\sig_typs.h
# End Source File
# End Group
# End Target
# End Project
