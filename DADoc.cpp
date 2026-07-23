// DADoc.cpp : implementation of the CDADoc class
//
#include "stdafx.h"
#include "DA.h"

#include "MainFrm.h"
#include "DADoc.h"
#include "TrendView.h"
#include "DAView.h"
#include "LoginDlg.h"
#include "AlmEvtViewDlg.h"
#include "DAMsgBox.h"
#include "DASplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR* pDeviceConfig = "Device.cfg";
const TCHAR* pAnalogChannelConfig = "AnalogChannel.cfg";
const TCHAR* pTagInfoConfig = "TagInfo.cfg";
const TCHAR* pAlmEvtConfig = "AlmEvt.cfg";
const TCHAR* pOpcItemConfig = "OpcItem.cfg";

/////////////////////////////////////////////////////////////////////////////
// CDADoc

IMPLEMENT_DYNCREATE(CDADoc, CDocument)

BEGIN_MESSAGE_MAP(CDADoc, CDocument)
	//{{AFX_MSG_MAP(CDADoc)
	ON_COMMAND(ID_FILE_OPEN, OnHistoricFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateHistoricFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnDataSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateDataSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDADoc construction/destruction
CDADoc::CDADoc()
{
	if(!ReadDeviceConfiguration())
	{
		nDevId = 1;
		nComPort = 1;
		nPollTime = 10;
		nTimeOut = 100;
		nSamplingRate = 1; 
		bFileStorage = TRUE;
		nDSTime = DSTIME_24HOURS;
		dwIPAddress = 2130706433;			//127.0.0.1 equivalent value. 
		nIpPort  = 502;
		nAStartAddress = 1;
		nDStartAddress = 1;
		nANoChannels = 16;
		nDNoChannels = 16;
		nARWStartAddress = 1;
		nDRWStartAddress = 1;
		nARWNoChannels = 16;
		nDRWNoChannels = 16;
		strProtocolName = _T("OPC");
		strOpcMachineName = _T("LOCAL");
		strOpcSrvName.Empty();
		strOpcGrName = _T("__DAGROUP__");
		nOpcScanTime = 2000;
		fOpcDeadBandPercentage = float(0.05);
		nOpcReadDataType = 0;				// 0 = CACHE 1 = DEVICE

		nPrinterColWidth = ID_PRINTR_COL80; 
		bEnalbleAlmBeep = FALSE; 
		nBeepInterval = 60;    //Seconds
		nBeepMode	= ID_ALM_BEEPMODE_MOMENTARY; 
		nBeepForAlarmType = ID_BEEP_ALMONLY; 

		BuildDefaultProtocolInfo();
		BuildDefaultUserInfo(); 
//		BuildDirectoryInfo(); 
	}
	
	readAData = new short[nANoChannels];
	readDData = new BOOL[nDNoChannels];
	for(int i = 0; i< nANoChannels; i++)
		readAData[i] = 0;					
	for(i = 0; i< nDNoChannels; i++)
		readDData[i] = FALSE;

	readRWAData = new short[nARWNoChannels];
	oldRWAData  = new short[nARWNoChannels];
	readRWDData = new BOOL[nDRWNoChannels];
	oldRWDData = new BOOL[nDRWNoChannels];
	for(i = 0; i< nARWNoChannels; i++)
	{
		readRWAData[i] = 0;					
		oldRWAData[i] = 0;
	}
	for(i = 0; i< nDRWNoChannels; i++)
	{
		readRWDData[i] = FALSE;
		oldRWDData[i] = FALSE;
	}

	pComLib = NULL; 
	hComMod = NULL;
	bStart = FALSE;

	ReadConfigurationInfo();			//Analog Channel config, Tag, Alm & Event, Opc Item config. 
	ReadTodayAlmEvtInfo();

	m_pBkgColorBrush = new CBrush(FRONTPANEL_BKG_COLOR);
	pCurrentUser = NULL; 
	bEvaluationMode = FALSE; 
	
	//License Related Variables
	strLicUserName = _T("Evaluation"); 
	strLicCompanyName = _T("Evaluation"); 
	strDAPackageVersion = _T("1.0"); 
	strLicKey = _T("xxxx-xxxx-xxxx-xxxx"); 
}

CDADoc::~CDADoc()
{
}

BOOL CDADoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	if(!CheckForLicenseKey())
	{
		CString strMsg = _T("License Key not found ! It will run in Evaluation mode and  Automatically Shutdowns after 2 Hours ! Can I Continue !"); 
		if(DAMessageBox(strMsg, "License Key Not Found", MSGBOX_BTN_OKCANCEL, MSGBOX_BMP_QUESTION) != IDOK)
			return FALSE; 
		bEvaluationMode = TRUE; 
	}

	CLoginDlg dlg;
	dlg.pDADoc = this; 
	if(dlg.DoModal() != IDOK)
		return FALSE;

	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CDADoc serialization

void CDADoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDADoc diagnostics
#ifdef _DEBUG
void CDADoc::AssertValid() const
{
	CDocument::AssertValid();
}
void CDADoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CDADoc commands
void CDADoc::DeleteContents() 
{
	CDocument::DeleteContents();
}

void CDADoc::OnCloseDocument() 
{
	CLogInfo* pLogInfo  = new CLogInfo(CTime::GetCurrentTime(), _T("DA Package Shutting Down"), FALSE); 
	logList.AddTail(pLogInfo); 

	if(hComMod != NULL)				//Closing Communicatin driver. 
		UnInitializeCommunicationDriver();

	SaveDeviceConfiguration();		//In SDI case whenever application terminates it calls
	SaveConfigurationInfo();		//Remaining config. 
	SaveTodayAlmEvtInfo();
	SaveTodayLogInfo(); 
	
	//If any child trend is open closing now to clear memory.
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if(pFrame != NULL)
		pFrame->CloseAllActiveChildTrends();
	
	delete[] readAData;
	delete[] readDData;
	delete[] readRWAData;
	delete[] readRWDData;
	delete[] oldRWAData;
	delete[] oldRWDData;

	while(dataList.GetHeadPosition())
		delete dataList.RemoveHead();
	while(historicDataList.GetHeadPosition())
		delete historicDataList.RemoveHead();
	
	while(opcItemList.GetHeadPosition())		//Opc Items information. 
		delete opcItemList.RemoveHead();

	while(chInfoList.GetHeadPosition())		//Channel inforatmtion related stuff. 
		delete chInfoList.RemoveHead();
	while(historicChInfoList.GetHeadPosition())		//Channel inforatmtion related stuff. 
		delete historicChInfoList.RemoveHead();
	
	while(almEvtConfigList.GetHeadPosition())
		delete almEvtConfigList.RemoveHead();
	while(almEvtList.GetHeadPosition())
		delete almEvtList.RemoveHead();
	
	while(tagInfoList.GetHeadPosition())	//Tag Related inforatmtion related stuff. 
		delete tagInfoList.RemoveHead();
	while(historicTagInfoList.GetHeadPosition())	//Tag Related inforatmtion related stuff. 
		delete historicTagInfoList.RemoveHead();

	
	while(rtPointList.GetHeadPosition())	//Related to Trend Point stuff. 	
		delete rtPointList.RemoveHead();
	while(hPointList.GetHeadPosition())
		delete hPointList.RemoveHead();

	while(rtACList.GetHeadPosition())
		delete rtACList.RemoveHead();
	while(hACList.GetHeadPosition())
		delete hACList.RemoveHead();

	while(protocolList.GetHeadPosition())
		delete protocolList.RemoveHead();

	while(userList.GetHeadPosition())
		delete userList.RemoveHead(); 
	while(logList.GetHeadPosition())
		delete logList.RemoveHead(); 

	delete m_pBkgColorBrush; 
	CDocument::OnCloseDocument();
}

BOOL CDADoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

void CDADoc::OnFileSaveAs() 
{
	CString strFileName;
	DWORD dwFlags = OFN_OVERWRITEPROMPT; 

	GetTodayFileName(strFileName, _T(".dat"));
	CFileDialog dlg(FALSE, "dat", strFileName, dwFlags,"DA*.dat", NULL); //FALSE = Save As, 
	dlg.m_ofn.lpstrInitialDir  = strDataDir;						
	dlg.m_ofn.lpstrTitle = _T("Save Real Time Data"); 
	
	if(dlg.DoModal() == IDOK)
	{	
		strFileName = dlg.GetPathName();		//Path + File Name, If any change in File Name you will get updated name. 
		if(SaveDataToFile(strFileName) != TRUE)
			DAMessageBox(_T("Unable to save the Data in file !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		else
			DAMessageBox(_T("Successfully Data saved !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
	}
}
void CDADoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if(!dataList.GetCount() || !bFileStorage)
		pCmdUI->Enable(FALSE);
}

void CDADoc::OnFilePrint() 
{
	if(bEvaluationMode)
		DAMessageBox(_T("Can't print in Evaluation mode !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	else
		DAMessageBox(_T("Not Implemented !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

void CDADoc::OnHistoricFileOpen() 
{
	CTrendView* pTrendView = (CTrendView*)GetTrendView();  
	CString strFileName;
	
	CFileDialog dlg(TRUE, "dat", "DA*.dat");
	dlg.m_ofn.lpstrInitialDir  = strDataDir;							
	dlg.m_ofn.lpstrTitle = _T("Open Historic Data File"); 
	
	if(dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();
		if(ReadDataFromFile(strFileName, &historicDataList, &historicTagInfoList, &historicChInfoList) != 0)
		{
			DAMessageBox(_T("Historic Data Loaded Successfully !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
			if(pTrendView != NULL)
			{
				pTrendView->BuildAddressInfo();		//Here just making valid nStartAddr, nNoOfChannels. 
				pTrendView->nHistoricChannelPos = 0; //Just resetting to first channel. 
				if(!pTrendView->bRTDisplay)
					pTrendView->nChannelPos = 0;		//Resetting the channel pos. 
			}
		}
		else
			DAMessageBox(_T("Unable to Load the file !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	}
}

void CDADoc::OnUpdateHistoricFileOpen(CCmdUI* pCmdUI) 
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CView* pView = pFrame->GetActiveView();
	ASSERT(pView);

	if( pView->GetRuntimeClass() != RUNTIME_CLASS(CDAFormView))
		pCmdUI->Enable(FALSE);
}

void CDADoc::OnDataSave() 
{
	CString strFileName;
	GetTodayFileName(strFileName, _T(".dat"));
	
	strFileName = strDataDir + strFileName; 
	if(SaveDataToFile(strFileName) != TRUE)
		DAMessageBox(_T("Unable to save the data file !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	else
		DAMessageBox(_T("Successfully Data saved !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
}

void CDADoc::OnUpdateDataSave(CCmdUI* pCmdUI) 
{
	if(!dataList.GetCount() || !bFileStorage)
		pCmdUI->Enable(FALSE);
}
/////////////////////////////////////////////////////////////////////////////////////
//Misc helping functons. 
int CDADoc::BuildChannelInfoList()
{
	CChannelInfo* pChInfo;
	
	while(chInfoList.GetHeadPosition())
		delete chInfoList.RemoveHead();
	for(int i = 0; i < nANoChannels; i++)
	{
		pChInfo = new CChannelInfo();
		pChInfo->nAAddress = nAStartAddress + i;
		chInfoList.AddTail(pChInfo);
	}
	
	return nANoChannels;
}

int CDADoc::BuildTagInfoList()
{
	CTagSpecInfo* pTagInfo;
	
	while(tagInfoList.GetHeadPosition())
		delete tagInfoList.RemoveHead();
	
	for(int i = 0; i < nANoChannels; i++)
	{
		pTagInfo = new CTagSpecInfo();
		pTagInfo->nChannelAddr = nAStartAddress + i;
		pTagInfo->nChannelType = INPUT_REGISTER;
		tagInfoList.AddTail(pTagInfo);
	}
	for(i = 0; i < nDNoChannels; i++)
	{
		pTagInfo = new CTagSpecInfo();
		pTagInfo->nChannelAddr = nDStartAddress + i;
		pTagInfo->nChannelType = INPUT_DISCRETE;
		tagInfoList.AddTail(pTagInfo);
	}
	for(i = 0; i < nARWNoChannels; i++)
	{
		pTagInfo = new CTagSpecInfo();
		pTagInfo->nChannelAddr = nARWStartAddress + i;
		pTagInfo->nChannelType = HOLDING_REGISTER;
		tagInfoList.AddTail(pTagInfo);
	}
	for(i = 0; i < nDRWNoChannels; i++)
	{
		pTagInfo = new CTagSpecInfo();
		pTagInfo->nChannelAddr = nDRWStartAddress + i;
		pTagInfo->nChannelType = OUTPUT_COIL;
		tagInfoList.AddTail(pTagInfo);
	}
	
	return (nANoChannels + nDNoChannels + nARWNoChannels + nDRWNoChannels);
}

void CDADoc::GetTodayFileName(CString& strFileName, CString strExt) 
{
	CString temp;
	SYSTEMTIME storageTime;
	CTime time = CTime::GetCurrentTime(); 

	strFileName += "DA";
	time.GetAsSystemTime(storageTime);
	temp.Format("%d", storageTime.wYear);	
	strFileName += temp.Right(2);
	if(storageTime.wMonth < 10)
		temp.Format("0%d", storageTime.wMonth);
	else
		temp.Format("%d", storageTime.wMonth);
	strFileName += temp;
	
	if(storageTime.wDay < 10)
		temp.Format("0%d", storageTime.wDay);
	else
		temp.Format("%d", storageTime.wDay);	

	strFileName += temp;
	strFileName += strExt;
}

CString CDADoc::GetTimeString(CTime almInTime)
{
	CString strTime;
	CString temp;
	SYSTEMTIME storageTime;
	
	almInTime.GetAsSystemTime(storageTime);

	if(storageTime.wHour < 10)
		temp.Format("0%d:", storageTime.wHour);
	else
		temp.Format("%d:", storageTime.wHour);
	strTime += temp;
		
	if(storageTime.wMinute < 10)
		temp.Format("0%d:", storageTime.wMinute);
	else
		temp.Format("%d:", storageTime.wMinute);
	strTime += temp;
	
	if(storageTime.wSecond < 10)
		temp.Format("0%d", storageTime.wSecond);
	else
		temp.Format("%d", storageTime.wSecond);	
	strTime += temp;
	
	return strTime;
}

CView* CDADoc::GetFrontPanelView()	
{
	CView* pView;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if(pView->IsKindOf( RUNTIME_CLASS(CDAFormView)))
			break;
	}   
	return pView;
}
CView* CDADoc::GetTrendView()	
{
	CView* pView;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if(pView != NULL)
		{
			if(pView->IsKindOf( RUNTIME_CLASS(CTrendView)))
				return pView; 
		}
	}
	return NULL; 
}

///////////////////////////////////////////////////////////////////////////////////
/////Tag Related Routines
CString CDADoc::GetTagString(CTagSpecInfoList* pList, eChannelType nType, int nCh)
{
	POSITION pos;
	CTagSpecInfo* pInfo;
	CString strTagName;
	
	//CTagSpecInfoList* pList = &tagInfoList;
	for(pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pInfo = (CTagSpecInfo*)pList->GetAt(pos);
		if(pInfo->nChannelType == nType && pInfo->nChannelAddr == nCh)
		{
			strTagName = pInfo->strTagName;
			break;
		}
		pList->GetNext(pos);
	}
	
//	ASSERT(pos != NULL); 
	return strTagName;
}

CString CDADoc::GetDescName(CTagSpecInfoList* pList, eChannelType nType, int nCh)
{
	POSITION pos;
	CTagSpecInfo* pInfo;
	CString strTagDesc;
	
	for(pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pInfo = pList->GetAt(pos);
		if(pInfo->nChannelType == nType && pInfo->nChannelAddr == nCh)
		{
			strTagDesc = pInfo->strTagDesc;
			break;
		}
		pList->GetNext(pos);
	}
//	ASSERT(pos != NULL); 
	return strTagDesc;
}

CString CDADoc::GetEngUnits(CTagSpecInfoList* pList, eChannelType nType, int nCh)
{
	POSITION pos;
	CTagSpecInfo* pInfo;
	CString strEngUnits;

	for(pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pInfo = pList->GetAt(pos);
		if(pInfo->nChannelType == nType && pInfo->nChannelAddr == nCh)
		{
			strEngUnits = pInfo->strEngUnits;
			break;
		}
		pList->GetNext(pos);
	}
//	ASSERT (pos != NULL); 
	return strEngUnits;
}

CString CDADoc::GetDiscreteCaption(CTagSpecInfoList* pList, eChannelType nType, int nCh, CString& onBuf, CString& offBuf)
{
	POSITION pos;
	CTagSpecInfo* pInfo;
	CString strCaption;
	short nPos;

	for(pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pInfo = pList->GetAt(pos);
		if(pInfo->nChannelType == nType && pInfo->nChannelAddr == nCh)
		{
			strCaption = pInfo->strDiscreteCaption;
			break;
		}
		pList->GetNext(pos);
	}
//	ASSERT(pos != NULL); 

	nPos = strCaption.Find(_T('/'));
	if(nPos != -1 && nPos < strCaption.GetLength())
	{
		onBuf = strCaption.Left(nPos);
		offBuf = strCaption.Right(strCaption.GetLength()-nPos - 1);
	}
	else
	{
		onBuf = _T("ON");
		offBuf = _T("OFF");
	}
	return strCaption;
}

///////////////////////////////////////////////////////////////////////////////////
////Read Write Configuration Information Related Stuff. 

BOOL CDADoc::ReadDeviceConfiguration()
{
	CFile fDevConfig;
	char buf[512];
	CFileException e;
	CString strMessage;
	CString strCfgFile; 
	
	if(strCfgDir.IsEmpty())
		BuildDirectoryInfo(); 
	
	strCfgFile = strCfgDir + pDeviceConfig; 
	if( !fDevConfig.Open( strCfgFile, CFile::modeRead, &e) ) 
	{
		strMessage.Format(_T("Unable to open Device Configuration file ! Making all configurations to default values !"));
		DAMessageBox(strMessage, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
//		e.ReportError(); 
		return FALSE;
	}
	
	CArchive ar(&fDevConfig, CArchive::load, 512, buf);

	ar >> nDevId >> nComPort >> nPollTime >> nTimeOut >> nDSTime >> nSamplingRate >> bFileStorage;
	ar >> nAStartAddress >> nANoChannels >> nDStartAddress >> nDNoChannels;
	ar >> nARWStartAddress >> nARWNoChannels >> nDRWStartAddress >> nDRWNoChannels;
	ar >> dwIPAddress >> nIpPort;
	ar >> strOpcMachineName >> strOpcSrvName >> strOpcGrName >> nOpcScanTime >> fOpcDeadBandPercentage >> nOpcReadDataType;
	ar >> strProtocolName;
	ar >> strExeDir >> strCfgDir >> strDataDir >> strAlmDir >> strRptDir >> strHlpDir >> strDriversDir >> strLogDir; 
	ar >> nPrinterColWidth >> bEnalbleAlmBeep >> nBeepInterval >> nBeepMode >> nBeepForAlarmType; 

	//Now Read Serial Port Info.
	for(int i = 0; i < MAX_PORTS; i++)
		ports[i].Serialize(ar);		
	
	//Now Read Protocol Info. 
	try {
		protocolList.Serialize(ar);
		userList.Serialize(ar); 
	}
	catch(CException* e){
		e->ReportError(MB_OK);
	}
	
	ar.Close();
	fDevConfig.Close();
	return TRUE;
}

void CDADoc::SaveDeviceConfiguration()
{
	CFile fDevConfig;
	char buf[512];
	CFileException e;
	CString strMessage;
	CString strCfgFile; 
	
	if(strCfgDir.IsEmpty())
		BuildDirectoryInfo(); 
	strCfgFile = strCfgDir + pDeviceConfig; 

	if( !fDevConfig.Open( strCfgFile, CFile::modeCreate | CFile::modeWrite, &e) ) 
	{
		strMessage.Format(_T("Unable to open Device Configuration file : \n Configurations  Lost !"));
		DAMessageBox(strMessage, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		e.ReportError(); 
		return;
	}
	
	CArchive ar(&fDevConfig, CArchive::store, 512, buf);
	
	ar << nDevId << nComPort << nPollTime << nTimeOut << nDSTime << nSamplingRate << bFileStorage;
	ar << nAStartAddress << nANoChannels << nDStartAddress << nDNoChannels;
	ar << nARWStartAddress << nARWNoChannels << nDRWStartAddress << nDRWNoChannels;
	ar << dwIPAddress << nIpPort ;
	ar << strOpcMachineName << strOpcSrvName << strOpcGrName << nOpcScanTime << fOpcDeadBandPercentage << nOpcReadDataType;
	ar << strProtocolName;	
	ar << strExeDir << strCfgDir << strDataDir << strAlmDir << strRptDir << strHlpDir << strDriversDir << strLogDir; 
	ar << nPrinterColWidth << bEnalbleAlmBeep << nBeepInterval << nBeepMode << nBeepForAlarmType; 

	//Now save Serial Port Settings. 
	for(int i = 0; i < MAX_PORTS; i++)
		ports[i].Serialize(ar);
	//Now save Protocol Info. 
	try {
		protocolList.Serialize(ar);
		userList.Serialize(ar); 
	}
	catch(CException* e){
		e->ReportError(MB_OK);
	}

	ar.Close();
	fDevConfig.Close();	
}

void CDADoc::ReadConfigurationInfo()
{
	CString strCfgFile; 
	ASSERT(!strCfgDir.IsEmpty());

	//First clear all config lists. 
	while(chInfoList.GetHeadPosition())
		delete chInfoList.RemoveHead();
	while(tagInfoList.GetHeadPosition())
		delete tagInfoList.RemoveHead();
	while(almEvtConfigList.GetHeadPosition())		
		almEvtConfigList.RemoveHead();
	while(opcItemList.GetHeadPosition())
		opcItemList.RemoveHead();

	//Analog Channel Configuratin Realted Stuff !
	strCfgFile = strCfgDir + pAnalogChannelConfig; 
	if(!ReadConfigurationList(strCfgFile, &chInfoList, _T("Analog Channel Config")))
		BuildChannelInfoList();	
	if(chInfoList.GetCount() != nANoChannels)
		BuildChannelInfoList();

	//Tag Configuratin Related Stuff. 
	strCfgFile = strCfgDir + pTagInfoConfig; 	
	if(!ReadConfigurationList(strCfgFile, &tagInfoList, _T("Tag Info Config")))
		BuildChannelInfoList();	
	if(tagInfoList.GetCount() != (nANoChannels + nDNoChannels + nARWNoChannels + nDRWNoChannels))
		BuildTagInfoList();
	
	//Now Alarm & Event realted Lists. 
	strCfgFile = strCfgDir + pAlmEvtConfig; 
	if(!ReadConfigurationList(strCfgFile, &almEvtConfigList, _T("Alarm & Event Config")))
	{
		while(almEvtConfigList.GetHeadPosition())		//If unable to read file clear the stuff. 
			almEvtConfigList.RemoveHead();
	}
	
	//Now OPC Item related configuration list
	strCfgFile = strCfgDir + pOpcItemConfig; 
	if(!ReadConfigurationList(strCfgFile, &opcItemList, _T("OPC Item Config")))
	{
		while(opcItemList.GetHeadPosition())
			opcItemList.RemoveHead();
	}
}

void CDADoc::SaveConfigurationInfo()
{
	CString strCfgFile; 
	ASSERT(!strCfgDir.IsEmpty());

	strCfgFile = strCfgDir + pAnalogChannelConfig; 
	if(!SaveConfigurationList(strCfgFile, &chInfoList, "Analog Channel Configuration"))
		DAMessageBox(_T("Unable to save the Analog Channel Config Info"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	strCfgFile = strCfgDir + pTagInfoConfig; 
	if(!SaveConfigurationList(strCfgFile, &tagInfoList, "Tag Info Configuration"))
		DAMessageBox(_T("Unable to save Tag Cofiguraton Info !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	strCfgFile = strCfgDir + pAlmEvtConfig; 
	if(!SaveConfigurationList(strCfgFile, &almEvtConfigList, "Alarm & Event Configuration"))
		DAMessageBox(_T("Unable to save Alm & Event Cofiguraton Info !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	strCfgFile = strCfgDir + pOpcItemConfig; 
	if(!SaveConfigurationList(strCfgFile, &opcItemList, "Opc Item Configuration"))
		DAMessageBox(_T("Unable to save OPC Item Cofiguraton Info !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

BOOL CDADoc::ReadConfigurationList(CString strFileName, CObList* pList, CString strFileDesc)
{
	CFile fConfig;
	CString strMessage;
	CFileException e;
	char buf[512];
	
	if(!fConfig.Open( strFileName, CFile::modeRead, &e)) 
	{
	//	e.ReportError(); 
		return FALSE;
	}

	CArchive ar(&fConfig, CArchive::load, 512, buf);
	try{
		pList->Serialize(ar);	
	}
	catch(CException* e){
		e->ReportError(MB_OK);
		ar.Close();
		fConfig.Close();
		return FALSE;
	}
	
	ar.Close();
	fConfig.Close();	
	return TRUE;
}

BOOL CDADoc::SaveConfigurationList(CString strFileName, CObList* pList, CString strFileDesc)
{
	CFile fConfig;
	char buf[512];
	CFileException e;
	CString strMessage;
	
	if( !fConfig.Open( strFileName, CFile::modeCreate | CFile::modeWrite, &e) ) 
	{
		strMessage.Format(_T("Unable to open %s file! \n %s Information lost!"), strFileDesc, strFileDesc);
		DAMessageBox(strMessage, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		e.ReportError(); 
		return FALSE;
	}

	CArchive ar(&fConfig, CArchive::store, 512, buf);
	
	try {
		pList->Serialize(ar);
	}
	catch(CException* e) {
		e->ReportError();
		ar.Close();
		fConfig.Close();	
		return FALSE;
	}
	
	ar.Close();
	fConfig.Close();	
	return TRUE;
}

BOOL CDADoc::ReadDataFromFile(CString strFileName, CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList)
{
	CFile fData;
	CFileException e;
	char buf[512];
	bool bError = false;
	
	if(!fData.Open( strFileName, CFile::modeRead, &e)) 
	{
		e.ReportError();
		return FALSE;
	}
	CArchive ar(&fData, CArchive::load, 512, buf);

	ClearDataStorageLists(pList, pTagInfoList, pChInfoList);
	try{
		pTagInfoList->Serialize(ar); 
		pChInfoList->Serialize(ar);
		pList->Serialize(ar);	
	}
	catch(CException* e){
		e->ReportError(MB_OK);
		ar.Close();
		fData.Close();
		ClearDataStorageLists(pList, pTagInfoList, pChInfoList); 
		return FALSE;
	}

	ar.Close();
	fData.Close();	
	
	if(!ValidateDataStoreageList(pList, pTagInfoList, pChInfoList))
	{
		ClearDataStorageLists(pList, pTagInfoList, pChInfoList); 
		return FALSE;
	}
		
	return TRUE;
}

BOOL CDADoc::ValidateDataStoreageList(CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList)
{
	long nStartAddr;
	short nChannelCount;
	CStorageData* pData;
	CTagSpecInfo* pTag; 
	CChannelInfo* pChInfo; 
	short nCount = 0;

	if(!pList->GetCount())
		return FALSE;

	pData = (CStorageData*) pList->GetHead();
	nStartAddr = pData->m_nAStartAddress;
	nChannelCount = pData->m_nANoChannels; 

	if(pChInfoList->GetCount() != nChannelCount)
		return FALSE; 
	
	for(POSITION pos = pTagInfoList->GetHeadPosition(); pos != NULL; )
	{
		pTag = (CTagSpecInfo*)pTagInfoList->GetAt(pos);
		if(pTag->nChannelType == INPUT_REGISTER && (pTag->nChannelAddr >= nStartAddr && pTag->nChannelAddr < (nStartAddr+nChannelCount)))
			nCount++;
		pTagInfoList->GetNext(pos);
	}
	
	if(nCount != nChannelCount)
		return FALSE; 

	nCount = 0;
	for(pos = pChInfoList->GetHeadPosition(); pos != NULL; )
	{
		pChInfo = (CChannelInfo*)pChInfoList->GetAt(pos);
		if(pChInfo->nAAddress >= nStartAddr && pChInfo->nAAddress < (nStartAddr+nChannelCount))
			nCount++;
		pChInfoList->GetNext(pos);
	}
	
	if(nCount != nChannelCount)
		return FALSE; 

	//Now time to check for Discrete Channels. For these channels no ChannelInfo inforamtion. 
	nCount = 0;
	nStartAddr = pData->m_nDStartAddress;
	nChannelCount = pData->m_nDNoChannels; 
	for(pos = pTagInfoList->GetHeadPosition(); pos != NULL; )
	{
		pTag = (CTagSpecInfo*)pTagInfoList->GetAt(pos);
		if( (pTag->nChannelType == INPUT_DISCRETE) && (pTag->nChannelAddr >= nStartAddr && pTag->nChannelAddr < (nStartAddr+nChannelCount)))
			nCount++;
		pTagInfoList->GetNext(pos);
	}
	
	if(nCount != nChannelCount)
		return FALSE; 

	return TRUE;
}
void CDADoc::ClearDataStorageLists(CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList)
{
	//Clear data storage related lists. 
	while(pList->GetHeadPosition())		
		delete pList->RemoveHead();
	while(pTagInfoList->GetHeadPosition())		
		delete pTagInfoList->RemoveHead();
	while(pChInfoList->GetHeadPosition())		
		delete pChInfoList->RemoveHead();
}

BOOL CDADoc::SaveDataToFile(CString strFileName)
{	
	CFile fData;
	char buf[512];
	CFileException e;
	CString strMessage;
	
	if( !fData.Open( strFileName, CFile::modeCreate | CFile::modeWrite, &e) ) 
	{
		strMessage.Format(_T("Unable to open %s file! \n %s Information lost!"), _T("Device Data"), _T("Device Data"));
		DAMessageBox(strMessage, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		e.ReportError(); 
		return FALSE;
	}

	CArchive ar(&fData, CArchive::store, 512, buf);
	
	try {
		tagInfoList.Serialize(ar); 
		chInfoList.Serialize(ar); 
		dataList.Serialize(ar);		//You need to care about lock of this list if you store at data acquisition time. 
	}
	catch(CException* e) {
		e->ReportError();
		ar.Close();
		fData.Close();	
		return FALSE;
	}
	
	ar.Close();
	fData.Close();	
	return TRUE;
}


BOOL CDADoc::ReadTodayAlmEvtInfo()
{
	CString strAlmFileName;
	
	GetTodayFileName(strAlmFileName, _T(".EVT"));
	strAlmFileName = strAlmDir + strAlmFileName; 
	while(almEvtList.GetHeadPosition())		//If list is not clear first clear it. 
		delete almEvtList.RemoveHead();

	if(!ReadConfigurationList(strAlmFileName, &almEvtList, _T("Alarm & Events")))
	{
		while(almEvtList.GetHeadPosition())		//If unable to read file clear the stuff. 
			delete almEvtList.RemoveHead();
		return FALSE;
	}
	return TRUE;
}

BOOL CDADoc::SaveTodayAlmEvtInfo()
{	
	CString strAlmFileName;
	
	GetTodayFileName(strAlmFileName, _T(".EVT"));
	strAlmFileName = strAlmDir + strAlmFileName;
	if(!SaveConfigurationList(strAlmFileName, &almEvtList, _T("Alarm & Events")))
	{
		DAMessageBox(_T("Unable to Save Alarm & Events Info !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return FALSE;
	}
	return TRUE; 
}

BOOL CDADoc::SaveTodayLogInfo()
{	
	CString strBuf;
	CFile fLogData;
	CFileException e;
	TCHAR CRLF[2] = {0x0D, 0x0A}; 
	
	if(!logList.GetCount())
		return TRUE; 

	CString strLogFileName;
	GetTodayFileName(strLogFileName, _T(".LOG"));
	strLogFileName = strLogDir + strLogFileName; 

	if( !fLogData.Open( strLogFileName, CFile::modeCreate |CFile::modeNoTruncate    | CFile::modeReadWrite , &e )) 
	{
		strBuf.Format(_T("Unable to open %s file to Export Log Data!"), strLogFileName);
		DAMessageBox(strBuf, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		e.ReportError(); 
		return FALSE;
	}
	fLogData.SeekToEnd();
		
	try
	{
		for(POSITION pos = logList.GetHeadPosition(); pos != NULL; )
		{
			CLogInfo* pLogInfo = (CLogInfo*) logList.GetAt(pos);
			ASSERT(pLogInfo);
			if(pLogInfo->bIsError)
				strBuf = _T("[ERROR] : ");
			else
				strBuf = _T("[MSG]       : ");
			strBuf += GetTimeString(pLogInfo->logTime);
			strBuf += _T(" : ");
			strBuf += pLogInfo->strLogMsg; 
			fLogData.Write(strBuf, strBuf.GetLength()); 
			fLogData.Write(CRLF, 2); 
			logList.GetNext(pos);
		}
		fLogData.Write(CRLF, 2); 	
	}
	catch(CFileException* e)
	{
		e->ReportError(); 					
		fLogData.Close(); 
		return FALSE;
	}
	fLogData.Close(); 
	return TRUE; 
}

void CDADoc::CreateNewAlarm(CString strVal, CTime almInTime, CAlmEvtConfigInfo* pAlmConfig)
{
	ASSERT(pAlmConfig != NULL);
	
	CAlmEvtInfo* pAlm = new CAlmEvtInfo(*pAlmConfig, almInTime, strVal);
	ASSERT(pAlm != NULL);
	almEvtList.AddTail(pAlm);
	
	//Send Notification to Alarm and Event view dig; If it is opened. 		
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT_VALID(pFrame);
	if(pFrame->pAlmEvtDlg->bIsDisplayed)
		pFrame->pAlmEvtDlg->SendMessage(WM_NEWALARM, 0, 0);
	
	if(pAlmConfig->nActionRegister)
		ImplementAlarmAction(pAlmConfig); 
}

void CDADoc::ImplementAlarmAction(CAlmEvtConfigInfo* pAlmConfig)
{
	short nOffSet = 0; 
	CString strBuf, strErr; 

	CDAFormView* pView = (CDAFormView*) GetFrontPanelView();
	ASSERT_VALID(pView);

	if(pAlmConfig->bIsAnalogAction)
	{
		nOffSet = (short)(pAlmConfig->nActionRegister - nARWStartAddress); 
		if(!(nOffSet >= 0 && nOffSet < nARWNoChannels))
		{
			DAMessageBox(_T("Alarm Action Register is out of range of presently configured Holding Registrs ! \n Invalidating this Action !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			pAlmConfig->nActionRegister = 0; 
			return;
		}
		else  //Perform Control Action on Holding Register. 
		{
			
			ASSERT(hComMod != NULL && pComLib != NULL);
			BOOL bRes = pComLib->WriteAnalogRWSingleRegister(nDevId, pAlmConfig->nActionRegister, pAlmConfig->nActionValue);
			if(!bRes)
			{
				strErr.Format(_T("%s!\n"), pComLib->GetLastProtocolError());
				pView->ErrorMessage(strErr, TRUE);
				return;
			}
			else
			{
				strErr.Format(_T("Successfully Performed Control Action on Holding Reg %d!"), pAlmConfig->nActionRegister);
				pView->ErrorMessage(strErr, TRUE);
			}
						
			//Now updating the Flexigrids 
			CMSFlexGrid* pFlexGrid = (CMSFlexGrid*)pView->GetDlgItem(IDC_RWANALOG);
			ASSERT_VALID(pFlexGrid); 
			readRWAData[nOffSet] = pAlmConfig->nActionValue; 
			strBuf.Format(_T(" %d %s"), pAlmConfig->nActionValue, GetEngUnits(&tagInfoList, HOLDING_REGISTER, pAlmConfig->nActionRegister));
			pFlexGrid->SetCol(1);
			pFlexGrid->SetRow(nOffSet+1);
			pFlexGrid->SetText(strBuf);
			
			//Now Prcess for alarms & events for Control Action Registers. 
			pView->ProcessForAlarmAndEvents(HOLDING_REGISTER);
			oldRWAData[nOffSet] = readRWAData[nOffSet];
		}
	}//End of bIsAnalogAction. 
	else
	{
		nOffSet = (short) (pAlmConfig->nActionRegister - nDRWStartAddress); 
		if(!(nOffSet >= 0 && nOffSet< nDRWNoChannels))
		{
			DAMessageBox(_T("Alarm Action Register is out of range of presently configured Output Coils ! \n Invalidating this Action !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			pAlmConfig->nActionRegister = 0; 
			return;
		}
		else   //Perform Control Action on Output Coils. 
		{
			ASSERT(hComMod != NULL && pComLib != NULL);
			BOOL bRes = pComLib->WriteDiscreteRWSingleRegister(nDevId, pAlmConfig->nActionRegister, (pAlmConfig->nActionValue ? TRUE : FALSE));
			if(!bRes)
			{
				strErr.Format(_T("%s!\n"), pComLib->GetLastProtocolError());
				pView->ErrorMessage(strErr, TRUE);
				return;
			}
			else
			{
				strErr.Format(_T("Successfully Performed Control Action on Output Coil %d!"), pAlmConfig->nActionRegister);
				pView->ErrorMessage(strErr, TRUE);
			}

			//Now updating the Flexigrids 
			CString strON, strOFF; 
			CMSFlexGrid* pFlexGrid = (CMSFlexGrid*)pView->GetDlgItem(IDC_RWDISCRETE);
			ASSERT_VALID(pFlexGrid); 
			readRWDData[nOffSet] = pAlmConfig->nActionValue;    //This value either 0 or 1. 
			GetDiscreteCaption(&tagInfoList, OUTPUT_COIL, pAlmConfig->nActionRegister, strON, strOFF);
			strBuf.Format(" %s",  pAlmConfig->nActionValue ? strON : strOFF);
			pFlexGrid->SetCol(1);
			pFlexGrid->SetRow(nOffSet+1);
			pFlexGrid->SetText(strBuf);
			
			//Now Prcess for alarms & events for Control Action Registers. 
			pView->ProcessForAlarmAndEvents(OUTPUT_COIL);
			oldRWDData[nOffSet] = readRWDData[nOffSet];
		}
	}

}

void CDADoc::BuildDefaultProtocolInfo()
{
	while(protocolList.GetHeadPosition())
		delete protocolList.RemoveHead();

	CProtocolInfo* pInfo = new CProtocolInfo();
	
	pInfo->nComMode = 2;		//OPC Server Dependent
	pInfo->strProtocolName = _T("OPC Client");
	pInfo->strDllName = _T("OpcClient.DLL");
	pInfo->strProtocolDesc = _T("With this protocol you can acquire date from any device that have valid OPC DA Server. Also with OPC you can acquire the data from SCADA & DCS systems");
	protocolList.AddTail(pInfo);

	pInfo = new CProtocolInfo();
	pInfo->nComMode = 0;		
	pInfo->strProtocolName = _T("MODBUS Serial-RTU");
	pInfo->strDllName = _T("ModbusSerialRTU.DLL");
	pInfo->strProtocolDesc = _T("With this protocol you can communicate with any MODBUS Complaint devies over RS232 Serial line.");
	protocolList.AddTail(pInfo);

	pInfo = new CProtocolInfo();
	pInfo->nComMode = 0;		
	pInfo->strProtocolName = _T("MODBUS Serial-ASCII");
	pInfo->strDllName = _T("ModbusSerialASCII.DLL");
	pInfo->strProtocolDesc = _T("With this protocol you can communicate with any MODBUS Complaint devies over RS232 Serial line.");
	protocolList.AddTail(pInfo);

	pInfo = new CProtocolInfo();
	pInfo->nComMode = 1;		
	pInfo->strProtocolName = _T("MODBUS Ethernet");
	pInfo->strDllName = _T("ModbusEthernet.DLL");
	pInfo->strProtocolDesc = _T("With this protocol you can communicate with any MODBUS Complaint devies over ETHERNET.");
	protocolList.AddTail(pInfo);
}

void CDADoc::BuildDefaultUserInfo()
{
	while(userList.GetHeadPosition())
		delete userList.RemoveHead();
	
	CUserInfo* pUser = new CUserInfo(_T("Administrator"), _T("AUTOMATION"), TRUE); 
	userList.AddTail(pUser); 

	pUser = new CUserInfo(_T("Guest"), _T("123"), FALSE); 
	userList.AddTail(pUser); 
}

void CDADoc::BuildDirectoryInfo()
{
	CString strBuf; 
	TCHAR szPath[MAX_PATH];

	::GetCurrentDirectory(sizeof(szPath)/ sizeof(TCHAR), szPath);

	strExeDir = szPath; 
	strExeDir += _T('\\'); 

	strCfgDir = strExeDir + _T("Config\\"); 
	CheckAndCreateDirectory(strCfgDir); 
	
	strDataDir = strExeDir + _T("Data\\"); 
	CheckAndCreateDirectory(strDataDir);
	
	strAlmDir = strExeDir + _T("Alarm\\"); 
	CheckAndCreateDirectory(strAlmDir);

	strRptDir = strExeDir + _T("Report\\"); 
	CheckAndCreateDirectory(strRptDir);
	
	strHlpDir = strExeDir + _T("Help\\"); 
	CheckAndCreateDirectory(strHlpDir);
	
	strDriversDir = strExeDir + _T("Drivers\\"); 
	CheckAndCreateDirectory(strDriversDir);

	strLogDir = strExeDir + _T("Log\\"); 
	CheckAndCreateDirectory(strLogDir);
}

void CDADoc::CheckAndCreateDirectory(CString strDirPath)
{
	CString strBuf; 

	DWORD dwFileAttrib = ::GetFileAttributes(strDirPath); //This func works for files and directories.
	if(dwFileAttrib == 0xFFFFFFFF  )
	{
		DWORD dwError = ::GetLastError();
		if(dwError ==  ERROR_FILE_NOT_FOUND)
		{
			if(!::CreateDirectory(strDirPath, NULL))
			{
				strBuf.Format(_T("I am unable to create the directoy : %s ! Please create it manually !"), strDirPath);  
				DAMessageBox(strBuf, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			}

		}
		else if (dwError == ERROR_PATH_NOT_FOUND)
			DAMessageBox(_T("Path not found Create dirctory manually !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	}
	else if(!(dwFileAttrib & FILE_ATTRIBUTE_DIRECTORY))							//|| dwFileAttrib !=FILE_ATTRIBUTE_ARCHIVE )
	{
		if(!::CreateDirectory(strDirPath, NULL))
		{
			strBuf.Format(_T("I am unable to create the directoy : %s ! Please create it manually !"), strDirPath);  
			DAMessageBox(strBuf, _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Communication Driver Relted Stuff. 
void CDADoc::ReconnectCommDriver()
{
	InitializeCommunicationDriver();	
}

void CDADoc::DisplayCommunicationError(CString strError)
{
	CDAFormView* pView = (CDAFormView*) GetFrontPanelView();
	ASSERT_VALID(pView);
	pView->ErrorMessage(strError, TRUE);

	UnInitializeCommunicationDriver();
}

BOOL CDADoc::UnInitializeCommunicationDriver()
{
	if(hComMod != NULL)  
	{
		if(pComLib != NULL && pComLib->IsConnected())
			pComLib->DisConnect();
		
		if(pComLib != NULL)
			DestroyCommunicationLibrary();
		AfxFreeLibrary(hComMod);
		hComMod = NULL;
	}
	
	ASSERT(pComLib == NULL); 
	return TRUE;
}

void CDADoc::DestroyCommunicationLibrary()
{
	ASSERT(hComMod != NULL); 
	FARPROC pnProc = GetProcAddress(hComMod, "DestroyComLibrary");
	ASSERT(pnProc != NULL);
	DESTROYCOMLIB pnDestroyComLib = (DESTROYCOMLIB)pnProc;
	pnDestroyComLib();
	pComLib = NULL; 
}

BOOL CDADoc::InitializeCommunicationDriver()
{
	CString strComDriverDll;
	short nComMode;

	if(hComMod != NULL)
	{
		if(pComLib != NULL &&  pComLib->IsConnected() == TRUE)
			pComLib->DisConnect();

		if(pComLib != NULL)
			DestroyCommunicationLibrary();

		AfxFreeLibrary(hComMod);
		hComMod = NULL;
	}
	
	ASSERT(pComLib == NULL);			

	for(POSITION pos = protocolList.GetHeadPosition(); pos != NULL;)
	{
		CProtocolInfo* pInfo = (CProtocolInfo*) protocolList.GetAt(pos);
		if(pInfo->strProtocolName == strProtocolName)
		{
			strComDriverDll = pInfo->strDllName;
			nComMode = pInfo->nComMode;
			break;
		}
		protocolList.GetNext(pos);
	}

	//ASSERT(pos != NULL);   
	if(pos == NULL)
	{
		DAMessageBox(_T("Communication Protocol Not Selected! Please Select it first !"), _T(" Select Protocol Driver !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return FALSE; 
	}
	
	strComDriverDll = strDriversDir + strComDriverDll; 
	hComMod = AfxLoadLibrary(strComDriverDll);
	if (NULL == hComMod)
	{
		DAMessageBox(_T("Unable to load Communication Driver DLL !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return FALSE;
	}
	
	FARPROC pnProc = GetProcAddress(hComMod, "CreateComLibrary");
	if(pnProc == NULL)
	{
		DAMessageBox(_T("Unable to create Comunication Driver Library ! Invalide Driver !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return FALSE;
	}
    
	CREATECOMLIB pnCreateComLib = (CREATECOMLIB)pnProc;
	pComLib = pnCreateComLib();
	if(pComLib == NULL)
	{
		DAMessageBox(_T("Unable to create Comunication Driver Library ! Invalide Driver !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return FALSE;
	}

	if(!pComLib->SetCommunicationMedia(nComMode))
		return FALSE;
		
	if(nComMode == RS232_MODE)
	{
		short nParity;
		if(ports[nComPort-1].m_strParity == _T("NONE"))
			nParity = 0;
		else if(ports[nComPort-1].m_strParity == _T("ODD"))
			nParity = 1;
		else if(ports[nComPort-1].m_strParity == _T("EVEN"))
			nParity = 2;
		else
			nParity = 0;

		if(!pComLib->SetSerialSettings(nComPort, ports[nComPort-1].m_nBaudRate, ports[nComPort-1].m_nDataBits, 
				ports[nComPort-1].m_nStopBits, nParity))
		{
			CString strErr =  (CString) pComLib->GetLastProtocolError();
			DisplayCommunicationError(strErr);
			return FALSE; 
		}
	}
	else if(nComMode == TCPIP_MODE)
	{
		if(!pComLib->SetTcpIpSettings(dwIPAddress, nIpPort))
		{
			CString strErr =  (CString) pComLib->GetLastProtocolError();
			DisplayCommunicationError(strErr);
			return FALSE; 
		}
	}
	else if(nComMode == OPC_MODE)
	{
		LPTSTR szOpcMachineName = const_cast< LPTSTR >((LPCTSTR)strOpcMachineName);
		LPTSTR szOpcSrvName = const_cast< LPTSTR >((LPCTSTR)strOpcSrvName);
		LPTSTR szOpcGrName = const_cast< LPTSTR >((LPCTSTR)strOpcGrName);
		if(!pComLib->SetOpcSettings(&opcItemList, szOpcMachineName, szOpcSrvName, szOpcGrName, nOpcScanTime, fOpcDeadBandPercentage, nOpcReadDataType))
		{
			CString strErr =  (CString) pComLib->GetLastProtocolError();
			DisplayCommunicationError(strErr);
			return FALSE; 
		}
	}
	
	pComLib->SetTimeOut(nTimeOut);
	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	if(!pComLib->Connect())
	{
		CString strErr =  (CString) pComLib->GetLastProtocolError();
		DisplayCommunicationError(strErr);
		SetCursor(hOldCursor);
		return FALSE;
	}
	SetCursor(hOldCursor);
	return TRUE; 
}

//////////////////////////////////////////////////////////////////////////////////////////
////Just OPC related Stuff. 
BOOL CDADoc::ConnectOpcServer(CString strMachineName, CString strSrvName, IOPCClient*& pClient, CTKInterface*& pInterface)
{
	tagConnectInfo coninfo;
	LPWSTR szMachineName;
	LPWSTR szSrvName;
	
	if(pClient != NULL)
		DisconnectOpcServer(pClient, pInterface);

	strMachineName.MakeUpper();
	if(strMachineName ==_T("LOCAL") || strMachineName ==_T(".") || strMachineName ==_T("LOCALHOST") || 
					strMachineName ==_T("127.0.0.1") || strMachineName.IsEmpty() )
	{
		szMachineName = NULL;
	}
	else
	{
		szMachineName = strMachineName.AllocSysString();
		coninfo.m_szMachineId = new WCHAR[wcslen(szMachineName)+1];
		wcscpy(coninfo.m_szMachineId, szMachineName);
		::SysFreeString(szMachineName);
	}

	szSrvName = strSrvName.AllocSysString();
	coninfo.m_szOpcServerId = new WCHAR[wcslen(szSrvName)+1];
	wcscpy(coninfo.m_szOpcServerId, szSrvName);
		
    pInterface = new CTKInterface;	//fill in the toolkit interface pointer
	coninfo.m_pITKInterface = pInterface;

	pClient = IOPCClient::Create();			//create the opc client
	BOOL bRes = pClient->Connect(coninfo);  //Make connection. 
	
	::SysFreeString(szSrvName);
	Free_tagConnectInfo(coninfo);  //Free the memory allocated by serverinfo. 

	return bRes;
}

void CDADoc::DisconnectOpcServer(IOPCClient*& pClient, CTKInterface*& pInterface)
{
	if(pClient != NULL)
	{
		pClient->Disconnect();
		IOPCClient::Delete(pClient);
		pClient = NULL;
	}
	if(pInterface != NULL)
	{
		delete pInterface;
		pInterface = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
//////////Colour related stuff
HBRUSH CDADoc::ChangeDlgBkgColor(COLORREF bkgColor, CBrush* pBrush, CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	
	switch (nCtlColor) 
	{

       case CTLCOLOR_STATIC:
           //pDC->SetTextColor(RGB(0, 0, 255));
			pDC->SetBkColor(bkgColor);
			return (HBRUSH)(pBrush->GetSafeHandle());
	   
       case CTLCOLOR_DLG:
				return (HBRUSH)(pBrush->GetSafeHandle());

	   case CTLCOLOR_BTN:
			return (HBRUSH)(pBrush->GetSafeHandle());

	   case CTLCOLOR_EDIT: 
	   case CTLCOLOR_MSGBOX: 
			pDC->SetTextColor(RGB(255, 0, 0));
            pDC->SetBkColor(bkgColor);
			return (HBRUSH)(pBrush->GetSafeHandle());

		case CTLCOLOR_LISTBOX:
		    pDC->SetBkColor(bkgColor);
		   return (HBRUSH)(pBrush->GetSafeHandle());
       
	   default:
		   return (HBRUSH)(pBrush->GetSafeHandle());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//////////DA Dialog Box related stuff
short CDADoc::DAMessageBox(CString strMsg, CString strTitle, short nBtns, short nIconType)
{
	CDAMsgBox dlg;
	dlg.strMsg = strMsg;
	dlg.strTitle = strTitle;
	dlg.nBtns = nBtns;
	dlg.nIconType = nIconType; 
	return dlg.DoModal(); 
}

BOOL CDADoc::CheckForLicenseKey()
{
	return FALSE; 
}
