# Microsoft Developer Studio Project File - Name="libgl_gscim_mtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libgl_gscim_mtl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libgl_gscim_mtl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libgl_gscim_mtl.mak" CFG="libgl_gscim_mtl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libgl_gscim_mtl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libgl_gscim_mtl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libgl_gscim_mtl"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libgl_gscim_mtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\obj\Release\libgl_gscim_mtl"
# PROP Intermediate_Dir "..\..\obj\Release\libgl_gscim_mtl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W4 /GX /O2 /I "..\..\..\common\h" /I "..\..\common\h" /I "..\..\os\h" /I "..\..\db\h" /D "NDEBUG" /D "GW_MT_ENABLED" /D "WIN32" /D "_MBCS" /D "_LIB" /D "GW_WINNT" /D "_ANSI_C_" /D "GW_INTEGER_IS_32" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\winnt\libgl_gscim_mtl.lib"

!ELSEIF  "$(CFG)" == "libgl_gscim_mtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\obj\Debug\libgl_gscim_mtl"
# PROP Intermediate_Dir "..\..\obj\Debug\libgl_gscim_mtl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Gm /GX /ZI /Od /I "..\..\..\common\h" /I "..\..\common\h" /I "..\..\os\h" /I "..\..\db\h" /D "_DEBUG" /D "GW_MT_ENABLED" /D "WIN32" /D "_MBCS" /D "_LIB" /D "GW_WINNT" /D "_ANSI_C_" /D "GW_INTEGER_IS_32" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\winnt\libgl_gscim_mtl.lib"

!ENDIF 

# Begin Target

# Name "libgl_gscim_mtl - Win32 Release"
# Name "libgl_gscim_mtl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\os\src\gl_gscim_file_io_opr_wrppr_api.c
# End Source File
# Begin Source File

SOURCE=..\..\os\src\gl_gscim_mutex_oprns_wrppr_api.c
# End Source File
# Begin Source File

SOURCE=..\..\os\src\gl_gscim_semphore_op_wrppr_api.c
# End Source File
# Begin Source File

SOURCE=..\..\os\src\gl_gscim_thrd_oprns_wrppr_api.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\common\h\gl_gscim_interfaces.h
# End Source File
# End Group
# End Target
# End Project
