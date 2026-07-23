// AddProtocolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "AddProtocolDlg.h"
#include "ProtocolInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddProtocolDlg dialog
CAddProtocolDlg::CAddProtocolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddProtocolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddProtocolDlg)
	m_strProtocolDesc = _T("");
	m_strDllName = _T("");
	m_strProtocolName = _T("");
	m_nComMedium = 0;
	//}}AFX_DATA_INIT
}

void CAddProtocolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddProtocolDlg)
	DDX_Text(pDX, IDC_PROT_DESC, m_strProtocolDesc);
	DDX_Text(pDX, IDC_PROT_DLLNAME, m_strDllName);
	DDV_MaxChars(pDX, m_strDllName, 20);
	DDX_Text(pDX, IDC_PROT_NAME, m_strProtocolName);
	DDV_MaxChars(pDX, m_strProtocolName, 20);
	DDX_CBIndex(pDX, IDC_COMM_MEDIUM, m_nComMedium);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddProtocolDlg, CDialog)
	//{{AFX_MSG_MAP(CAddProtocolDlg)
	ON_BN_CLICKED(IDC_PROT_ADD, OnAddProtocol)
	ON_BN_CLICKED(IDC_PROT_CLOSE, OnProtocolAddClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddProtocolDlg message handlers
void CAddProtocolDlg::OnAddProtocol() 
{
	UpdateData(TRUE);
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	if(m_strProtocolName.IsEmpty() || m_strDllName.IsEmpty() || m_strProtocolDesc.IsEmpty())
	{
		pDoc->DAMessageBox(_T("Please fill All information!"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	
	short nPos = m_strDllName.Find(_T('.'));
	short nExtChars = m_strDllName.GetLength() -(nPos+1);
	if( nExtChars!= 3 || m_strDllName.Right(nExtChars) != _T("DLL"))
	{
		pDoc->DAMessageBox(_T("Invalid DLL Name !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	
	CProtocolList* pList = &pDoc->protocolList;
	
	//Check the list is there any protocol exists with given name. 
	for(POSITION pos = pList->GetHeadPosition(); pos !=NULL; )
	{
		CProtocolInfo* pInfo = (CProtocolInfo*) pList->GetAt(pos);	
		if(pInfo->strProtocolName == m_strProtocolName)
		{
			pDoc->DAMessageBox(_T("Protocol with given name already exists! Give some other name !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			return;
		}
		pList->GetNext(pos);
	}
	
	//Now add Protool Info to List. 
	CProtocolInfo* pInfo = new CProtocolInfo(m_nComMedium, m_strProtocolName, m_strDllName, m_strProtocolDesc);
	pList->AddTail(pInfo);
	pDoc->DAMessageBox(_T("Protocol Successfully Added to List !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);

	m_strProtocolName.Empty();
	m_strDllName.Empty();
	m_strProtocolDesc.Empty();

	UpdateData(FALSE);
}

void CAddProtocolDlg::OnProtocolAddClose() 
{
	CDialog::OnOK();
}

HBRUSH CAddProtocolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	
	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 

}
	