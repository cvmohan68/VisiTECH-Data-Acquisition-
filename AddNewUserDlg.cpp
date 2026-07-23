// AddNewUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "AddNewUserDlg.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "UserInfo.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNewUserDlg dialog


CAddNewUserDlg::CAddNewUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNewUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNewUserDlg)
	m_strPwd = _T("");
	m_strRPwd = _T("");
	m_strUserName = _T("");
	m_bIsAdministrator = FALSE;
	//}}AFX_DATA_INIT
}


void CAddNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNewUserDlg)
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDV_MaxChars(pDX, m_strPwd, 12);
	DDX_Text(pDX, IDC_EDIT_RPWD, m_strRPwd);
	DDV_MaxChars(pDX, m_strRPwd, 12);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, 15);
	DDX_Check(pDX, IDC_CHK_ADMIN, m_bIsAdministrator);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNewUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAddNewUserDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_ADD, OnAddNewUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNewUserDlg message handlers

HBRUSH CAddNewUserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	
	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 
}

void CAddNewUserDlg::OnAddNewUser() 
{
	UpdateData(TRUE); 

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	
	if(m_strUserName.IsEmpty())
	{
		pDoc->DAMessageBox(_T("Please Enter Valid User Name !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}
	
	if(CheckIsUserAlreadyExists(m_strUserName))
	{
		pDoc->DAMessageBox(_T("Specified User Name already exists ! Enter different User Name !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}

	if(m_strPwd.Compare(m_strRPwd) != 0)
	{
		pDoc->DAMessageBox(_T("Enter Same password in both text boxes !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}

	
	CUserInfo* pInfo = new CUserInfo(m_strUserName, m_strPwd, m_bIsAdministrator);
	pDoc->userList.AddTail(pInfo); 
	pDoc->DAMessageBox(_T("Successfully added new user !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
	

	m_strUserName.Empty();
	m_strPwd.Empty();
	m_strRPwd.Empty();
	m_bIsAdministrator = FALSE; 

	UpdateData(FALSE); 
}

BOOL CAddNewUserDlg::CheckIsUserAlreadyExists(CString strUserName)
{
	BOOL bExists = FALSE; 

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	
	CUserInfoList* pUserList = &pDoc->userList;
	for(POSITION pos = pUserList->GetHeadPosition(); pos != NULL; )
	{
		CUserInfo* pInfo = (CUserInfo*) pUserList->GetAt(pos);
		ASSERT(pInfo);
		if(pInfo->strUserName.Compare(strUserName) == 0)
		{
			bExists = TRUE;
			break;
		}
		pUserList->GetNext(pos); 	
	}

	return bExists;
}
