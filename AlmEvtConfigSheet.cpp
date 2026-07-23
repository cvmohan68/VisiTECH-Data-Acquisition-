// AlmEvtConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "AlmEvtConfigSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlmEvtConfigSheet
IMPLEMENT_DYNAMIC(CAlmEvtConfigSheet, CPropertySheet)

CAlmEvtConfigSheet::CAlmEvtConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CAlmEvtConfigSheet::CAlmEvtConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_analogPropPage);
	AddPage(&m_discretePropPage);
	AddPage(&m_analogRWPropPage);
	AddPage(&m_discreteRWPropPage);
}

CAlmEvtConfigSheet::~CAlmEvtConfigSheet()
{
	if (m_fntPage.m_hObject)
		m_fntPage.DeleteObject ();
}

BEGIN_MESSAGE_MAP(CAlmEvtConfigSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAlmEvtConfigSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlmEvtConfigSheet message handlers
void CAlmEvtConfigSheet::BuildPropPageArray()
{
	CPropertySheet::BuildPropPageArray();

	CPropertyPage* pPage = GetPage (0);   	// get first page
	ASSERT (pPage);

	CDialogTemplate dlgtemp;   // dialog template class in afxpriv.h
	
	VERIFY (dlgtemp.Load (pPage->m_psp.pszTemplate)); // load the dialog template
	
	//Now get the font information
	CString strFace;		
	WORD	wSize;
	VERIFY (dlgtemp.GetFont (strFace, wSize));
	if (m_fntPage.m_hObject)
		VERIFY (m_fntPage.DeleteObject ());
	
	// create a font using the info from first page
	VERIFY (m_fntPage.CreatePointFont (wSize*10, strFace));
}

BOOL CAlmEvtConfigSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
		
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT_VALID(pFrame);

	CPropertyPage* pPage = GetActivePage ();	//Get first page
	ASSERT (pPage);

	pFrame->ChangePropSheetFont(this, &m_fntPage);  //Now change font for first sheet.
	for (int iCntr = 0; iCntr < GetPageCount (); iCntr++) //Change font for each page.
	{
		VERIFY (SetActivePage (iCntr));
		CPropertyPage* pPage = GetActivePage ();
		ASSERT (pPage);
		pFrame->ChangePropSheetFont(pPage, &m_fntPage);
	}
	
	VERIFY (SetActivePage (pPage));
	CTabCtrl* pTab = GetTabControl ();  // set and save the size of the page
	ASSERT (pTab);
	
	if (m_psh.dwFlags & PSH_WIZARD)
	{
		pTab->ShowWindow (SW_HIDE);
		GetClientRect (&m_rctPage);
		CWnd* pButton = GetDlgItem (ID_WIZBACK);
		ASSERT (pButton);
		CRect rc;
		pButton->GetWindowRect (&rc);
		ScreenToClient (&rc);
		m_rctPage.bottom = rc.top-2;
	}
	else
	{
		pTab->GetWindowRect (&m_rctPage);
		ScreenToClient (&m_rctPage);
		pTab->AdjustRect (FALSE, &m_rctPage);
	}

	pPage->MoveWindow (&m_rctPage);   // resize the page	
	return bResult;
}
