# Microsoft Developer Studio Project File - Name="libgl_ulml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libgl_ulml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libgl_ulml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libgl_ulml.mak" CFG="libgl_ulml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libgl_ulml - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libgl_ulml - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libgl_ulml"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libgl_ulml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\obj\Release\libgl_ulml"
# PROP Intermediate_Dir "..\..\obj\Release\libgl_ulml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\common\h" /I "..\..\common\h" /I "..\..\os\h" /I "..\..\db\h" /I "..\..\..\..\common\h" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "GW_WINNT" /D "_ANSI_C_" /D "GW_INTEGER_IS_32" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\winnt\libgl_ulml.lib"

!ELSEIF  "$(CFG)" == "libgl_ulml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libgl_ulml___Win32_Debug"
# PROP BASE Intermediate_Dir "libgl_ulml___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\obj\Debug\libgl_ulml"
# PROP Intermediate_Dir "..\..\obj\Debug\libgl_ulml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Gm /GX /ZI /Od /I "..\..\..\common\h" /I "..\..\common\h" /I "..\..\os\h" /I "..\..\db\h" /I "..\..\..\..\common\h" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "GW_WINNT" /D "_ANSI_C_" /D "GW_INTEGER_IS_32" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\winnt\libgl_ulml.lib"

!ENDIF 

# Begin Target

# Name "libgl_ulml - Win32 Release"
# Name "libgl_ulml - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\src\gl_ulm_gnrl_utils_api.c
# End Source File
# Begin Source File

SOURCE=..\..\common\src\gl_ulm_stats_api.c
# End Source File
# Begin Source File

SOURCE=..\..\common\src\gl_ulm_str_manipultn_api.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\common\h\gl_ulm_interfaces.h
# End Source File
# End Group
# End Target
# End Project
