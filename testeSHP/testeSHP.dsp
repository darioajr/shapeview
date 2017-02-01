# Microsoft Developer Studio Project File - Name="testeSHP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=testeSHP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "testeSHP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testeSHP.mak" CFG="testeSHP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testeSHP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "testeSHP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "testeSHP - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "testeSHP - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "testeSHP - Win32 Release"
# Name "testeSHP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DIB.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\testeSHP.cpp
# End Source File
# Begin Source File

SOURCE=.\testeSHP.rc
# End Source File
# Begin Source File

SOURCE=.\testeSHPDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DIB.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RetangleD.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\testeSHP.h
# End Source File
# Begin Source File

SOURCE=.\testeSHPDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\testeSHP.ico
# End Source File
# Begin Source File

SOURCE=.\res\testeSHP.rc2
# End Source File
# End Group
# Begin Group "GIS"

# PROP Default_Filter ""
# Begin Group "DataSource"

# PROP Default_Filter ""
# Begin Group "SHP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.SHPReader.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.SHPReader.h
# End Source File
# Begin Source File

SOURCE=.\GIS.SHPThread.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.SHPThread.h
# End Source File
# End Group
# Begin Group "DXF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.DXFReader.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.DXFReader.h
# End Source File
# End Group
# End Group
# Begin Group "IO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.BinaryReader.h
# End Source File
# Begin Source File

SOURCE=.\GIS.FileStream.h
# End Source File
# End Group
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.DbaseReader.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.DbaseReader.h
# End Source File
# Begin Source File

SOURCE=.\GIS.DbaseReaderIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.DbaseReaderIndex.h
# End Source File
# End Group
# Begin Group "GDI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.GDIThread.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.GDIThread.h
# End Source File
# End Group
# Begin Group "GEO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GIS.MapMetrics.h
# End Source File
# Begin Source File

SOURCE=.\GIS.PointD.h
# End Source File
# Begin Source File

SOURCE=.\GIS.RetangleD.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GIS.IMapLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.IMapLayer.h
# End Source File
# Begin Source File

SOURCE=.\GIS.Map.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.Map.h
# End Source File
# Begin Source File

SOURCE=.\GIS.MapControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GIS.MapControl.h
# End Source File
# Begin Source File

SOURCE=.\GIS.Message.h
# End Source File
# End Group
# End Target
# End Project
