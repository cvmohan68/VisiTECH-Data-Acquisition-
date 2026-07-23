// ChannelConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "ChannelConfigSheet.h"
#include "MainFrm.h"
#include "DADoc.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelConfigSheet

IMPLEMENT_DYNAMIC(CChannelConfigSheet, CPropertySheet)

CChannelConfigSheet::CChannelConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CChannelConfigSheet::CChannelConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_chConfig1);
	AddPage(&m_spAlarmConfig);
	AddPage(&m_chConfig3);
	AddPage(&m_almEvtPropPage);

}

CChannelConfigSheet::~CChannelConfigSheet()
{
	if (m_fntPage.m_hObject)
		m_fntPage.DeleteObject ();
}

BEGIN_MESSAGE_MAP(CChannelConfigSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CChannelConfigSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelConfigSheet message handlers
void CChannelConfigSheet::BuildPropPageArray()
{
	CPropertySheet::BuildPropPageArray();

	CPropertyPage* pPage = GetPage (0);   	// get first page
	ASSERT (pPage);
	// dialog template class in afxpriv.h
	CDialogTemplate dlgtemp;
	// load the dialog template
	VERIFY (dlgtemp.Load (pPage->m_psp.pszTemplate));
	// get the font information
	CString strFace;
	WORD	wSize;
	VERIFY (dlgtemp.GetFont (strFace, wSize));
	if (m_fntPage.m_hObject)
		VERIFY (m_fntPage.DeleteObject ());
	// create a font using the info from first page
	VERIFY (m_fntPage.CreatePointFont (wSize*10, strFace));
}


BOOL CChannelConfigSheet::OnInitDialog() 
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

