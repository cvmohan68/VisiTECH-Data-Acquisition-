// DADoc.h : interface of the CDADoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DADOC_H__58185F68_CE7E_466F_954A_C9D189F69896__INCLUDED_)
#define AFX_DADOC_H__58185F68_CE7E_466F_954A_C9D189F69896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "serialport.h"
#include "Storagedata.h"
#include "ChannelInfo.h"
#include "TagSpecInfo.h"
#include "OpcItemInfo.h"
#include "AlmEvtConfigInfo.h"
#include "ProtocolInfo.h"
#include "UserInfo.h"
#include "LogInfo.h"
#include "ComLib.h"

#include "IOPCClient.h"
#include "TKInterface.h"

class CDADoc : public CDocument
{
protected: // create from serialization only
	CDADoc();
	DECLARE_DYNCREATE(CDADoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDADoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDADoc();

	//Data and Configuration Storage related. 
	BOOL ReadDeviceConfiguration(void);
	void ReadConfigurationInfo(void);
	BOOL ReadDataFromFile(CString strFileName, CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList);
	void ClearDataStorageLists(CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList);
	BOOL ValidateDataStoreageList(CDataList* pList, CTagSpecInfoList* pTagInfoList, CChannelInfoList* pChInfoList);
	BOOL ReadTodayAlmEvtInfo(void);
	void SaveDeviceConfiguration(void);
	void SaveConfigurationInfo(void);
	BOOL SaveDataToFile(CString strFileName); 
	BOOL SaveTodayAlmEvtInfo(void);
	BOOL SaveTodayLogInfo(void);
	BOOL ReadConfigurationList(CString strFileName, CObList* pList, CString strFileDesc);
	BOOL SaveConfigurationList(CString strFileName, CObList* pList, CString strFileDesc);

	//Helper functioner
	int	 BuildChannelInfoList(void);
	int  BuildTagInfoList(void);
	void BuildDefaultProtocolInfo(void);
	void BuildDefaultUserInfo(void); 
	void BuildDirectoryInfo(void);
	CString GetTagString(CTagSpecInfoList* pList, eChannelType nType, int nCh);
	CString GetDescName(CTagSpecInfoList* pList, eChannelType nType, int nCh);
	CString GetEngUnits(CTagSpecInfoList* pList, eChannelType nType, int nCh);
	CString GetDiscreteCaption(CTagSpecInfoList* pList, eChannelType nType, int nCh, CString&, CString& );
	CString GetTimeString(CTime tmpTime);
	void CreateNewAlarm(CString strVal, CTime almInTime, CAlmEvtConfigInfo* pAlm );
	void ImplementAlarmAction(CAlmEvtConfigInfo* pAlmConfig);
	CView* GetFrontPanelView(void);	
	CView* GetTrendView(void);	
	void GetTodayFileName(CString& strFileName, CString strExt); 
	
	//Communication Driver Related stuff. 
	void DisplayCommunicationError(CString strError);
	BOOL InitializeCommunicationDriver(void);
	BOOL UnInitializeCommunicationDriver(void);
	void DestroyCommunicationLibrary(void);
	void ReconnectCommDriver(void);
	BOOL ConnectOpcServer(CString strMachineName, CString strSrvName, IOPCClient*& pClient, CTKInterface*& pInterface);
	void DisconnectOpcServer(IOPCClient*& pClient, CTKInterface*& pInterface);

	HBRUSH ChangeDlgBkgColor(COLORREF bkgColor, CBrush* pBrush, CDC* pDC, CWnd* pWnd, UINT nCtlColor); 
	void CheckAndCreateDirectory(CString strDirPath);
	short DAMessageBox(CString strMsg, CString strTitle, short nBtnType, short nIconType);
	BOOL CheckForLicenseKey(void);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
protected:
	//Protected variables 
public:
	//Device Settings Related
	short nDevId;
	int	  nPollTime;
	int   nTimeOut;
	short nDSTime;				//0 = 6 Hours etc., 
	short nSamplingRate;
	BOOL  bFileStorage;

	//ProtocolRelated Stuff
	CString  strProtocolName; 

	//Serial Port Related
	CSerialPort ports [MAX_PORTS];
	short nComPort;
	
	//TCP/IP Related
	DWORD dwIPAddress;
	unsigned short nIpPort;

	//OPC Related
	CString strOpcMachineName;
	CString strOpcSrvName;
	CString strOpcGrName;
	int		nOpcScanTime;
	float	fOpcDeadBandPercentage;
	int		nOpcReadDataType;
	
	//Miscellaneous
	BOOL bStart; 

	//Analog Register and Discrete Addresses:	
	long  nAStartAddress;
	long  nDStartAddress;
	int   nANoChannels;
	int   nDNoChannels; 

	long  nARWStartAddress;
	long  nDRWStartAddress;
	int   nARWNoChannels;
	int   nDRWNoChannels; 

	short* readAData;
	BOOL* readDData;
	short* readRWAData;
	BOOL* readRWDData;
	short* oldRWAData;
	BOOL* oldRWDData;
	
	CDataList	dataList;
	CDataList   historicDataList;

	CTrendPointList  rtPointList;
	CTrendPointList	 hPointList;

	CACInfoList  rtACList;
	CACInfoList  hACList;

	CChannelInfoList chInfoList;
	CChannelInfoList historicChInfoList;

	CTagSpecInfoList tagInfoList;
	CTagSpecInfoList historicTagInfoList;

	CAlmEvtConfigList  almEvtConfigList;
	CAlmEvtList		   almEvtList;

	CProtocolList      protocolList;
	COpcItemInfoList   opcItemList;

	//Communication Driver Related Stuff. 
	HMODULE hComMod; 
	CComLib* pComLib; 

	//Color related
	CBrush* m_pBkgColorBrush;

	//Location or Dir Settings related. 
	CString strExeDir;
	CString strCfgDir;
	CString strDataDir;
	CString strAlmDir;
	CString strRptDir;
	CString strHlpDir; 
	CString strDriversDir; 
	CString strLogDir; 

	//User Login and Pwd related list
	CUserInfoList	userList;  
	CUserInfo* pCurrentUser; 
	CTime		loginTime; 
	CLogInfoList  logList; 

	//Evaluation & License related stuff
	BOOL	bEvaluationMode; 

	//Printer Settings
	short	nPrinterColWidth; 

	//Alm Beep Settings
	BOOL	bEnalbleAlmBeep; 
	short	nBeepInterval;
	short	nBeepMode;
	short	nBeepForAlarmType; 

	//License Related Variables
	CString	strLicUserName;
	CString	strLicCompanyName;
	CString	strDAPackageVersion;
	CString	strLicKey; 

public:
	afx_msg void OnDataSave();

// Generated message map functions
protected:
	//{{AFX_MSG(CDADoc)
	afx_msg void OnHistoricFileOpen();
	afx_msg void OnUpdateHistoricFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DADOC_H__58185F68_CE7E_466F_954A_C9D189F69896__INCLUDED_)
