# Microsoft Developer Studio Project File - Name="DA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DA.mak" CFG="DA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DA - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DA - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DA - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OPC_MFC_MODEL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wtopcdaclib.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DA - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OPC_MFC_MODEL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wtopcdaclibD.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libmbusmaster.lib" /pdbtype:sept /DEFAULTLIB:libmbusmaster.lib
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DA - Win32 Release"
# Name "DA - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddNewUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddProtocolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmControlActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmEvtConfigInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmEvtConfigSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmEvtPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmEvtViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePwd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelAddressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelConfig3.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelConfigSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\childtrend.cpp
# End Source File
# Begin Source File

SOURCE=.\DA.cpp
# End Source File
# Begin Source File

SOURCE=.\DA.rc
# End Source File
# Begin Source File

SOURCE=.\DADoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DAMsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DASettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DASplash.cpp
# End Source File
# Begin Source File

SOURCE=.\DAView.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoricDataInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoricDataStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\LocationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MCTChannelSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MCTChannelSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\MCTMaxMinSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiTrendView.cpp
# End Source File
# Begin Source File

SOURCE=.\OpcItemConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpcItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportView.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RptDataTagInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RptFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\RptSplitterView.cpp
# End Source File
# Begin Source File

SOURCE=.\RptSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SPAlarmConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StorageData.cpp
# End Source File
# Begin Source File

SOURCE=.\TagSpecDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TagSpecInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TextView.cpp
# End Source File
# Begin Source File

SOURCE=.\TKInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Trend.cpp
# End Source File
# Begin Source File

SOURCE=.\TrendColorSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrendPen.cpp
# End Source File
# Begin Source File

SOURCE=.\TrendView.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMgmtDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddNewUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddProtocolDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlmControlActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlmEvtConfigInfo.h
# End Source File
# Begin Source File

SOURCE=.\AlmEvtConfigSheet.h
# End Source File
# Begin Source File

SOURCE=.\AlmEvtPropPage.h
# End Source File
# Begin Source File

SOURCE=.\AlmEvtViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangePwd.h
# End Source File
# Begin Source File

SOURCE=.\ChannelAddressDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChannelConfig.h
# End Source File
# Begin Source File

SOURCE=.\ChannelConfig3.h
# End Source File
# Begin Source File

SOURCE=.\ChannelConfigSheet.h
# End Source File
# Begin Source File

SOURCE=.\ChannelInfo.h
# End Source File
# Begin Source File

SOURCE=.\ChannelSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\childtrend.h
# End Source File
# Begin Source File

SOURCE=.\ComLib.h
# End Source File
# Begin Source File

SOURCE=.\DA.h
# End Source File
# Begin Source File

SOURCE=.\DACONSTANTS.h
# End Source File
# Begin Source File

SOURCE=.\DADoc.h
# End Source File
# Begin Source File

SOURCE=.\DAMsgBox.h
# End Source File
# Begin Source File

SOURCE=.\DASettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DASplash.h
# End Source File
# Begin Source File

SOURCE=.\DAView.h
# End Source File
# Begin Source File

SOURCE=.\FolderSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistoricDataInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistoricDataStatistics.h
# End Source File
# Begin Source File

SOURCE=.\hmplatf.h
# End Source File
# Begin Source File

SOURCE=.\hmserio.hpp
# End Source File
# Begin Source File

SOURCE=.\hmtimer.hpp
# End Source File
# Begin Source File

SOURCE=.\IItemConfig.h
# End Source File
# Begin Source File

SOURCE=.\IItemRegistrar.h
# End Source File
# Begin Source File

SOURCE=.\IOPCClient.h
# End Source File
# Begin Source File

SOURCE=.\ITKInterface.h
# End Source File
# Begin Source File

SOURCE=.\LocationDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogInfo.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MbusAsciiMasterProtocol.hpp
# End Source File
# Begin Source File

SOURCE=.\MbusMasterFunctions.hpp
# End Source File
# Begin Source File

SOURCE=.\MbusRtuMasterProtocol.hpp
# End Source File
# Begin Source File

SOURCE=.\MbusRtuOverTcpMasterProtocol.hpp
# End Source File
# Begin Source File

SOURCE=.\MbusSerialMasterProtocol.hpp
# End Source File
# Begin Source File

SOURCE=.\MbusTcpMasterProtocol.hpp
# End Source File
# Begin Source File

SOURCE=.\MCTChannelSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\MCTChannelSettings.h
# End Source File
# Begin Source File

SOURCE=.\MCTMaxMinSettings.h
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\MultiTrendView.h
# End Source File
# Begin Source File

SOURCE=.\opcda.h
# End Source File
# Begin Source File

SOURCE=.\OpcItemConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpcItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolInfo.h
# End Source File
# Begin Source File

SOURCE=.\ReportView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichEdit.h
# End Source File
# Begin Source File

SOURCE=.\RptDataTagInfo.h
# End Source File
# Begin Source File

SOURCE=.\RptFormView.h
# End Source File
# Begin Source File

SOURCE=.\RptSplitterView.h
# End Source File
# Begin Source File

SOURCE=.\RptSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\SettingsSheet.h
# End Source File
# Begin Source File

SOURCE=.\SPAlarmConfig.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StorageData.h
# End Source File
# Begin Source File

SOURCE=.\TagSpecDlg.h
# End Source File
# Begin Source File

SOURCE=.\TagSpecInfo.h
# End Source File
# Begin Source File

SOURCE=.\TextView.h
# End Source File
# Begin Source File

SOURCE=.\TKInterface.h
# End Source File
# Begin Source File

SOURCE=.\Trend.h
# End Source File
# Begin Source File

SOURCE=.\TrendColorSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrendPen.h
# End Source File
# Begin Source File

SOURCE=.\TrendView.h
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserInfo.h
# End Source File
# Begin Source File

SOURCE=.\UserMgmtDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\About Box.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\About Box1.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\bmp\Alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\Bavamo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bawani.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Comm.ICO
# End Source File
# Begin Source File

SOURCE=".\res\DA Login.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\DA.ico
# End Source File
# Begin Source File

SOURCE=.\res\DA.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DA1.ico
# End Source File
# Begin Source File

SOURCE=.\res\DADoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\DAMedium.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DASmall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DISK06.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Drives.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Error.bmp
# End Source File
# Begin Source File

SOURCE=.\res\event.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fig1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fig3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FigDColor1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grfwnd.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\INFO.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Information.bmp
# End Source File
# Begin Source File

SOURCE=.\res\item.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LInvoices.ICO
# End Source File
# Begin Source File

SOURCE=".\res\List View.ICO"
# End Source File
# Begin Source File

SOURCE=.\res\List1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Login.ico
# End Source File
# Begin Source File

SOURCE=.\res\Login1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mdl.ico
# End Source File
# Begin Source File

SOURCE=.\res\message.bmp
# End Source File
# Begin Source File

SOURCE=.\res\note.ico
# End Source File
# Begin Source File

SOURCE=.\res\OPC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PHONE01.ICO
# End Source File
# Begin Source File

SOURCE=.\res\PHONE15.ICO
# End Source File
# Begin Source File

SOURCE=.\res\plus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PROP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Question.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Sales .ICO"
# End Source File
# Begin Source File

SOURCE=".\res\Sales Report.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Settings.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Splash1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splash3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splash4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Trend1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Trend2.ICO
# End Source File
# Begin Source File

SOURCE=.\res\user1.ico
# End Source File
# Begin Source File

SOURCE=".\res\VisiTECH About.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\VisiTECH.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section DA : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section DA : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
