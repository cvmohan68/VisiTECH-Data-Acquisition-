// ChangePwd.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "DADoc.h"
#include "MainFrm.h"
#include "ChangePwd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangePwd dialog


CChangePwd::CChangePwd(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePwd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangePwd)
	m_strOldPwd = _T("");
	m_strNPwd = _T("");
	m_strRNPwd = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
	
	pCurrentUser = NULL; 
	
}


void CChangePwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangePwd)
	DDX_Text(pDX, IDC_OLDPWD, m_strOldPwd);
	DDX_Text(pDX, IDC_NPWD, m_strNPwd);
	DDX_Text(pDX, IDC_RPWD, m_strRNPwd);
	DDX_Text(pDX, IDC_UNAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangePwd, CDialog)
	//{{AFX_MSG_MAP(CChangePwd)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_UNAME, OnUserNameChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangePwd message handlers

void CChangePwd::OnOK() 
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	UpdateData(TRUE);

	if(m_strNPwd.Compare(m_strRNPwd) != 0)
	{
		pDoc->DAMessageBox(_T("Please type the same text in New Password and Reenter Password text boxes"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return ;
	}
	
	if(m_strNPwd.IsEmpty())
	{
		pDoc->DAMessageBox(_T("Password not entered ! Please enter password"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}

	if(!(m_strOldPwd.Compare(pCurrentUser->strUserPwd) == 0) )  
	{
		pDoc->DAMessageBox(_T("Invalid Old Password !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	
	pCurrentUser->strUserPwd = m_strNPwd;
	pDoc->DAMessageBox(_T("Successfully Changed Password !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);

	CDialog::OnOK();
}

void CChangePwd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CChangePwd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 
}

BOOL CChangePwd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(pCurrentUser != NULL); 
	m_strUserName = pCurrentUser->strUserName; 
	UpdateData(FALSE); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangePwd::OnUserNameChange() 
{
	UpdateData(FALSE); 	
}
