# Microsoft Developer Studio Project File - Name="glbuild" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=glbuild - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "glbuild.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "glbuild.mak" CFG="glbuild - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "glbuild - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "glbuild - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "glbuild"
# PROP Scc_LocalPath "."
MTL=midl.exe

!IF  "$(CFG)" == "glbuild - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\common\obj\Release\glbuild"
# PROP Intermediate_Dir "..\..\common\obj\Release\glbuild"
# PROP Target_Dir ""
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Now copying the release files to exe directory
PostBuild_Cmds=echo on	.\scripts\copyrel Debug
# End Special Build Tool

!ELSEIF  "$(CFG)" == "glbuild - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\common\obj\Debug\glbuild"
# PROP Intermediate_Dir "..\..\common\obj\Debug\glbuild"
# PROP Target_Dir ""
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Now copying the release files to exe directory
PostBuild_Cmds=echo on	.\scripts\copyrel Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "glbuild - Win32 Release"
# Name "glbuild - Win32 Debug"
# End Target
# End Project
