// DA.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DA.h"

#include "MainFrm.h"
#include "DADoc.h"
#include "DAView.h"
#include "DASplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* MyMainWndClassName = "CDAApp";
/////////////////////////////////////////////////////////////////////////////
// CDAApp

BEGIN_MESSAGE_MAP(CDAApp, CWinApp)
	//{{AFX_MSG_MAP(CDAApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDAApp construction

CDAApp::CDAApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDAApp object

CDAApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDAApp initialization

BOOL CDAApp::InitInstance()
{

	if (!FirstInstance()) 
		return FALSE;

	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("MODBUS Data Acquisition Editor"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_nCmdShow = SW_SHOWMAXIMIZED;		//To maximize the window
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDADoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDAFormView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//Just to change the Icon from default to DAICON type. 
	//SetClassLong(m_pMainWnd->m_hWnd,GCL_HICON, (LONG)LoadIcon(IDR_DAICON));  //It also works find but changed to SetIcon method. 

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{

public:

public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strAboutVisiTECH;
	CString	m_strAddrVisiTECH;
	CString	m_strCompany;
	CString	m_strKey;
	CString	m_strUserName;
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeAboutInfo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strAboutVisiTECH = _T("");
	m_strAddrVisiTECH = _T("");
	m_strCompany = _T("Evaluation");
	m_strKey = _T("xxxx-xxxx-xxxx-xxxx");
	m_strUserName = _T("Evaluation");
	m_strVersion = _T("1.0");
	//}}AFX_DATA_INIT

	m_strAboutVisiTECH  =  _T("We will provide complete Process Automation solutions from the plant floor to enterprise systems"); 
	m_strAddrVisiTECH  = _T("  VisiTECH Automation all rights reserved - 2007   -  HYDERABAD        Ph : +91 9247222893"); 

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT_VISITECHINFO, m_strAboutVisiTECH);
	DDX_Text(pDX, IDC_EDIT_VISITECHADDRESS, m_strAddrVisiTECH);
	DDX_Text(pDX, IDC_EDIT_COMPANY, m_strCompany);
	DDV_MaxChars(pDX, m_strCompany, 20);
	DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);
	DDV_MaxChars(pDX, m_strKey, 20);
	DDX_Text(pDX, IDC_EDIT_UNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, 12);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_VISITECHINFO, OnChangeAboutInfo)
	ON_EN_CHANGE(IDC_EDIT_VISITECHADDRESS, OnChangeAboutInfo)
	ON_EN_CHANGE(IDC_EDIT_UNAME, OnChangeAboutInfo)
	ON_EN_CHANGE(IDC_EDIT_KEY, OnChangeAboutInfo)
	ON_EN_CHANGE(IDC_EDIT_COMPANY, OnChangeAboutInfo)
	ON_EN_CHANGE(IDC_EDIT_VERSION, OnChangeAboutInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDAApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor);
}

/////////////////////////////////////////////////////////////////////////////
// CDAApp message handlers
BOOL CDAApp::InitApplication() 
{
	CWinApp::InitApplication();
	
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));   
    ::GetClassInfo(AfxGetInstanceHandle(), MyMainWndClassName,&wndcls);
    wndcls.lpszClassName = MyMainWndClassName;
    // Register new class and return the result code.
    BOOL bRes = ::RegisterClass(&wndcls);
	return bRes;
}

/* 
Create a named mutex and test that already that mutex exists or not
If existed means some instance of this application is running and just quit back 
*/ 
BOOL CDAApp::FirstInstance()
{
	HANDLE hModbusDA;
	BOOL bFound = FALSE;
	
	hModbusDA = CreateMutex(NULL, TRUE, _T("MODBUS_DATA_ACQUISITION")); 
    if(GetLastError() == ERROR_ALREADY_EXISTS) 
        bFound = TRUE; 
    if(hModbusDA) 
        ReleaseMutex(hModbusDA); 
    
	return !bFound;	
}


/*
The below is the best approach to find is the instance of application exist or not. 
Unfortunately it is not working try some other time 03/07/07 
*/
/*
BOOL CDAApp::FirstInstance()
{
	HWND      hwnd;
	hwnd = ::FindWindow (MyMainWndClassName, NULL);
	if (hwnd != NULL) 
	{
		if (IsIconic(hwnd)) 
			ShowWindow(hwnd, SW_RESTORE);
        ::SetForegroundWindow (hwnd);
		return FALSE;
    }
	return TRUE;
}
*/ 

//Second Approach with some modifications of the above. 
/*
BOOL CDAApp::FirstInstance()
{
	CWnd *PrevCWnd, *ChildCWnd;
    PrevCWnd = CWnd::FindWindow(MyMainWndClassName, NULL);
    if (PrevCWnd != NULL)
    {
        ChildCWnd= PrevCWnd->GetLastActivePopup();
        PrevCWnd->BringWindowToTop();
        if (PrevCWnd->IsIconic())
            PrevCWnd->ShowWindow(SW_MAXIMIZE);
        if (PrevCWnd != ChildCWnd)
            ChildCWnd->BringWindowToTop();
        return FALSE;
    }
    else
        return TRUE;
}
*/

////////////////////////////////////////////////////////////////////////////////
//////////About Dlg realted stuff. 
void CAboutDlg::OnChangeAboutInfo() 
{
	UpdateData(FALSE); 	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	
	m_strUserName = pDoc->strLicUserName; 
	m_strCompany = pDoc->strLicCompanyName; 
	m_strKey = pDoc->strLicKey; 
	m_strVersion = pDoc->strDAPackageVersion; 
	UpdateData(FALSE); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
