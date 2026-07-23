// AlmEvtViewDlg.cpp : implementation file
//
#include "stdafx.h"
#include "da.h"
#include "AlmEvtViewDlg.h"
#include "MainFrm.h"
#include "DADoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//typedef INT (*PFNLVCOMPARE)(LPARAM lParam1, LPARAM lParam2, LPARAM lParmSortCol);
/////////////////////////////////////////////////////////////////////////////
// CAlmEvtViewDlg dialog
CAlmEvtViewDlg::CAlmEvtViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlmEvtViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlmEvtViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bIsDisplayed = FALSE;
}

void CAlmEvtViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlmEvtViewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAlmEvtViewDlg, CDialog)
	//{{AFX_MSG_MAP(CAlmEvtViewDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_HISTORIC, OnHistoricAlmEvt)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	ON_WM_CLOSE()
	ON_MESSAGE (WM_NEWALARM, OnNewAlarm)
	ON_MESSAGE (WM_ALMRECOVERED, OnAlarmRecovered)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnAlarmEvtPrint)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_ALMEVT, OnAlarmEventCtrlDblClick)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCTRL_ALMEVT, OnAlmEvtCtrlColumnClick)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_ALMEVT, OnAlmEvtCtrlClick)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LISTCTRL_ALMEVT, OnAlmEvtDisplayInfo)
	ON_BN_CLICKED(IDC_BTN_ACKALL, OnAcknowledgeAllAlarms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAlmEvtViewDlg message handlers

void CAlmEvtViewDlg::InitializeAlmEvtView()
{
	bIsDisplayed = TRUE;
	bRTAlmDisplay  = TRUE; 
	strHistoricAlmEvtFileName.Empty();
	AddAlmEvtsToListCtrl();
	ChangeHistoricBtnCatpion();
	ActivateDeleteBtn(); 
}

BOOL CAlmEvtViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pImageList = new CImageList();
	m_hWhiteBrush = CreateSolidBrush(RGB(255,255,255));
	AddListCtrlHeader();

	
	return TRUE;  // return TRUE unless you set the focus to a control
}
void CAlmEvtViewDlg::OnPaint() 
{
	CTime almEvtDate; 
	CPaintDC dc(this); // device context for painting
	CDC* pDC = (CDC*) &dc;
	
	DrawHeader(pDC);
	
	if(bRTAlmDisplay || !historicAlmEvtList.GetCount())
		almEvtDate = CTime::GetCurrentTime();
	else
	{
		CAlmEvtInfo* pInfo = historicAlmEvtList.GetHead();
		almEvtDate =  pInfo->almInTime; 
	}
	
	DisplayDate(pDC, almEvtDate); 
}

void CAlmEvtViewDlg::OnClose() 
{
	SaveHistoriclAlmEvtInfo();	//This call also clears historicAlmEvt linked list. 
	
	FreeItemMemory(); 
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);
	pListCtrl->DeleteAllItems();
	bIsDisplayed = FALSE;
	
	CDialog::OnClose();
}
void CAlmEvtViewDlg::OnBtnClose() 
{
	SendMessage(WM_CLOSE);
}

void CAlmEvtViewDlg::OnHistoricAlmEvt() 
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	if(bRTAlmDisplay)
	{
		SaveHistoriclAlmEvtInfo();		//Required this if you delete any alram it need to persist. 
		CFileDialog dlg(TRUE, "EVT", "DA*.EVT");
		dlg.m_ofn.lpstrInitialDir  = pDoc->strAlmDir;							
		dlg.m_ofn.lpstrTitle = _T("Open Historic Alarm & Event File"); 

		if(dlg.DoModal() == IDOK)
		{
			strHistoricAlmEvtFileName = dlg.GetPathName();		//Path + File Name
			if(!pDoc->ReadConfigurationList(strHistoricAlmEvtFileName, &historicAlmEvtList, _T("Alarm & Events")))
			{
				pDoc->DAMessageBox(_T("Unable to Found Alarm & Event Info !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
				return;
			}
			if(!historicAlmEvtList.GetCount())
			{
				pDoc->DAMessageBox(_T("Sorry! No Alarm & Event information found in the given file !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
				return;
			}
			bRTAlmDisplay = FALSE;
		}
	}
	else
		bRTAlmDisplay = TRUE;
	
	Invalidate(TRUE); 
	AddAlmEvtsToListCtrl();
	ChangeHistoricBtnCatpion();
	ChangeAckAllBtnStatus(); 
}

void CAlmEvtViewDlg::OnDelete() 
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		pDoc->DAMessageBox(_T("No Alarm Selectd !, Select Alarm to Delete !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	int nPos = pListCtrl->GetNextSelectedItem(pos);
	int nCount = pListCtrl->GetItemCount();

	int nAlmOffSet = nCount - (nPos + 1);		//Later required to add Synchronization support. 
	pListCtrl->DeleteItem(nPos);
	DeleteAlarmFromList(nAlmOffSet);			//Insted offset if you chaeck channel type, channel ID and Time based to delete it. 
}

void CAlmEvtViewDlg::OnAlarmEvtPrint() 
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->bEvaluationMode)
		pDoc->DAMessageBox(_T("In Evaluation Mode ! No Support for Print Functionality !"), _T("License Key Not Found !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
	else
	{
		pDoc->DAMessageBox(_T("Required to Implement"), _T("License Key Not Found !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_INFORMATION);
	}
}

HBRUSH CAlmEvtViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (nCtlColor)
    {
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,0,0));
	}
	return m_hWhiteBrush;
}

void CAlmEvtViewDlg::OnNewAlarm()
{
	if(!bRTAlmDisplay)
		return;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	CAlmEvtInfo* pAlmInfo =  (CAlmEvtInfo*)pDoc->almEvtList.GetTail();
	AddNewAlarmToListCtrl(pAlmInfo);
}

void CAlmEvtViewDlg::OnAlarmRecovered(int nPos, CTime* pAlmOutTime)
{
	LPTSTR szStrBuf; 

	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);
	
	int nItemCount = pListCtrl->GetItemCount();
	ASSERT(nPos < nItemCount); 
	
	CString strBuf = pDoc->GetTimeString(*pAlmOutTime);
	szStrBuf = const_cast< LPTSTR >( (LPCTSTR) strBuf);
	int nItemPos = nItemCount - nPos - 1; 
	pListCtrl->SetItemText(nItemPos, 2, szStrBuf); 
	
	strBuf = _T("OUT");
	szStrBuf = const_cast< LPTSTR >( (LPCTSTR) strBuf);
	pListCtrl->SetItemText(nItemPos, 4, szStrBuf); 
}
void CAlmEvtViewDlg::ActivateDeleteBtn()
{
	//Activate Delete Btn According user logged in privilages. 
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->pCurrentUser != NULL); 

	CButton* pBtn = (CButton*) GetDlgItem(IDC_BTN_DELETE);
	ASSERT(pBtn);
	pBtn->EnableWindow(pDoc->pCurrentUser->bIsAdministrator); 
}

////////////////////////////////////////////////////////////////////////////////
////Helper functions
void CAlmEvtViewDlg::DeleteAlarmFromList(int nOffSet)
{
	CAlmEvtList* pList;
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	if(bRTAlmDisplay)
		pList = (CAlmEvtList*) &pDoc->almEvtList;
	else
		pList = (CAlmEvtList*) &historicAlmEvtList; 
	
	ASSERT(nOffSet < pList->GetCount());

	POSITION pos = pList->GetHeadPosition();
	for(int i = 0; i < nOffSet; i++)
		pList->GetNext(pos);

	CAlmEvtInfo* pAlmInfo = (CAlmEvtInfo*) pList->GetAt(pos);
	pList->RemoveAt(pos);
	delete pAlmInfo;
}

void CAlmEvtViewDlg::SaveHistoriclAlmEvtInfo()
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	if(historicAlmEvtList.GetCount() && !strHistoricAlmEvtFileName.IsEmpty())
		pDoc->SaveConfigurationList(strHistoricAlmEvtFileName, &historicAlmEvtList, _T("Alarm & Events"));
	
	while(historicAlmEvtList.GetHeadPosition())
		delete historicAlmEvtList.RemoveHead();
}

void CAlmEvtViewDlg::AddNewAlarmToListCtrl(CAlmEvtInfo* pAlmInfo)
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	LV_ITEM  lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM; 
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.iImage = pAlmInfo->almConfigInfo.nAlmType;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = (LPARAM) pAlmInfo;     //pInfo

	if(pListCtrl->InsertItem(&lvi) == -1)
	{
		pDoc->DAMessageBox(_T("Unable to insert item!"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	
	if(bRTAlmDisplay && pAlmInfo->bAcknowledged)   //Only for RT alarms display and those are previously acknowledged. 
		pListCtrl->SetCheck(0, TRUE); 
}

void CAlmEvtViewDlg::AddListCtrlHeader()
{
	CBitmap	bitmap;
	CFont font;
	font.CreatePointFont (120, _T ("Arial"));  

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);
	
	ASSERT(pImageList != NULL);
	pImageList->Create(16, 16, ILC_MASK, 3, 3);	

	bitmap.LoadBitmap(IDB_ALARM);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	
	bitmap.LoadBitmap(IDB_EVENT);				
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_MESSAGE);				
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	pListCtrl->SetImageList(pImageList, LVSIL_SMALL);
	
	pListCtrl->SetFont(&font, TRUE);

	pListCtrl->InsertColumn(0, _T("Tag Name"), LVCFMT_RIGHT, 100, -1);
	pListCtrl->InsertColumn(1, _T("Time In"), LVCFMT_CENTER, 80, -1);
	pListCtrl->InsertColumn(2, _T("Time Out"), LVCFMT_CENTER, 80, -1);
	pListCtrl->InsertColumn(3, _T("Value"), LVCFMT_CENTER, 60, -1);
	pListCtrl->InsertColumn(4, _T("Status"), LVCFMT_CENTER, 60, -1);
	pListCtrl->InsertColumn(5, _T("ACK"), LVCFMT_CENTER, 40, -1);
	pListCtrl->InsertColumn(6, _T("Time Ack"), LVCFMT_CENTER, 80, -1);
	pListCtrl->InsertColumn(7, _T("Ch# Type"), LVCFMT_CENTER, 50, -1);
	pListCtrl->InsertColumn(8, _T("Addr"), LVCFMT_CENTER, 60, -1);
	pListCtrl->InsertColumn(9, _T("Description"), LVCFMT_LEFT, 500, -1);

}

void CAlmEvtViewDlg::DisplayDate(CDC* pDC, CTime almEvtDate)
{
	CString strAlmEvtDate, tmpBuffer;
	CRect clientRect;
	
	GetClientRect(&clientRect);

	
	strAlmEvtDate.Format(_T("Date: %d:"), almEvtDate.GetDay());  
	tmpBuffer.Format(_T("%d:"), almEvtDate.GetMonth());
	strAlmEvtDate += tmpBuffer;
	tmpBuffer.Format(_T("%d"), almEvtDate.GetYear());
	strAlmEvtDate += tmpBuffer;
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->TextOut(clientRect.right - 120, 50, strAlmEvtDate);
}

void CAlmEvtViewDlg::ChangeHistoricBtnCatpion()
{
	CButton* pButton = (CButton*) GetDlgItem(IDC_BTN_HISTORIC);
	if(bRTAlmDisplay)
		pButton->SetWindowText(_T("Historical"));
	else
		pButton->SetWindowText(_T("Real Time"));
}
void CAlmEvtViewDlg::DrawHeader(CDC* pDC)
{
	CString strTitle;
	CRect rect;							
    CFont font;
	short nOffSet;
	
	CRect clientRect;
	GetClientRect(&clientRect);
	
	rect.left = clientRect.right/2 - 200;			//For title display box. 
	rect.top = 0;
	rect.right = clientRect.right/2 + 200;
	rect.bottom = 60;
	nOffSet = 6;
	
	font.CreatePointFont (240, _T ("Arial"));
	CFont* pOldFont =  pDC->SelectObject(&font);
    pDC->SetBkMode(TRANSPARENT);

	strTitle = _T ("ALARM && EVENT VIEW");
    rect.OffsetRect (nOffSet, nOffSet);
	pDC->SetTextColor (RGB (192, 192, 192));
    pDC->DrawText(strTitle, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    rect.OffsetRect (-nOffSet, -nOffSet);
    pDC->SetTextColor (RGB (0, 0, 0));
    pDC->DrawText(strTitle, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	
	CPen thickPen(PS_DASHDOTDOT, 4, (COLORREF)(198,0,255));
	CPen* pOldPen = pDC->SelectObject(&thickPen);
	pDC->MoveTo(clientRect.left, clientRect.top + 80);
	pDC->LineTo(clientRect.right, clientRect.top + 80); 
	
	CPen thinPen(PS_SOLID, 2, (COLORREF)(0,0,0) );
	pDC->SelectObject(&thinPen);
	pDC->MoveTo(clientRect.left, clientRect.top + 80-5);
	pDC->LineTo(clientRect.right, clientRect.top + 80-5); 

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void CAlmEvtViewDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
}

void CAlmEvtViewDlg::AddAlmEvtsToListCtrl()
{
	DWORD exStyle;
	CAlmEvtInfo* pAlmInfo;
	CAlmEvtList* pList;
	
	//Clear the list ctrl first. 
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);
	FreeItemMemory(); 
	pListCtrl->DeleteAllItems(); 
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	if(bRTAlmDisplay)
	{
		pList = (CAlmEvtList*) &pDoc->almEvtList;
		exStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES;
	}
	else
	{
		pList = (CAlmEvtList*) &historicAlmEvtList; 
		exStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	}

	pListCtrl->SetExtendedStyle(exStyle);
	
	for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pAlmInfo = (CAlmEvtInfo*)pList->GetAt(pos);
		AddNewAlarmToListCtrl(pAlmInfo);
		pList->GetNext(pos);
	}
}


void CAlmEvtViewDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(pImageList != NULL)
		pImageList->DeleteImageList();
	
	delete pImageList; 
}

void CAlmEvtViewDlg::OnAlarmEventCtrlDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	LVHITTESTINFO info; 
	
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	if(pNMListView->iSubItem != 0)    
	{
		*pResult = 0;
		return; 
	}

	info.pt = pNMListView->ptAction; 			
	if(pListCtrl->SubItemHitTest(&info) != -1 && info.flags & LVHT_ONITEMSTATEICON )   
	{
		if(pListCtrl->GetCheck(pNMListView->iItem))  //If Already checked don't do any thing. 
			pListCtrl->SetCheck(pNMListView->iItem, !pListCtrl->GetCheck(pNMListView->iItem)); //Once Checked Always force Checked ON. 
	}	
	
	*pResult = 0;
}

void CAlmEvtViewDlg::OnAlmEvtCtrlColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	/*
	Sorting is not for historic alarm display only. 
	For real time case it will effects badly when deleting or when alrm recoved message 
	arrives it change else alarm. At present use for historic, 
	If required we can change it for real time also in the future. 
	*/
	
	if(!bRTAlmDisplay && (pNMListView->iSubItem == 0 || pNMListView->iSubItem == 1))
	{
		PFNLVCOMPARE pfnCompare = CAlmEvtViewDlg::AlarmSortingFunc;    
		pListCtrl->SortItems(pfnCompare, DWORD(pNMListView->iSubItem));		
	}
		
	*pResult = 0;
}

void CAlmEvtViewDlg::OnAlmEvtCtrlClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strBuf; 
	LPTSTR szStrBuf; 
	LVHITTESTINFO info; 
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		
	if(!bRTAlmDisplay || pNMListView->iSubItem != 0)    
	{
		*pResult = 0;
		return; 
	}

	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	info.pt = pNMListView->ptAction; 			
	if(pListCtrl->HitTest(&info) != -1 )      //pListCtrl->HitSubItemHitTest(&info)
	{
		if(info.flags == LVHT_ONITEMSTATEICON)     //Check is clicked on State Icon (i.e., on check box) or some other place. 	
		{
			if(!pListCtrl->GetCheck(pNMListView->iItem))  //If Already checked don't do any thing. 
			{
				strBuf = pListCtrl->GetItemText(pNMListView->iItem, 5); 
				if(strBuf == _T("NO"))
				{
					strBuf = _T("YES"); 
					szStrBuf = const_cast< LPTSTR >( (LPCTSTR) strBuf);
					pListCtrl->SetItemText(pNMListView->iItem, 5, szStrBuf); 
						
					CTime ackTime = CTime::GetCurrentTime(); 
					strBuf = pDoc->GetTimeString(ackTime);
					szStrBuf = const_cast< LPTSTR >( (LPCTSTR) strBuf);
					pListCtrl->SetItemText(pNMListView->iItem, 6, szStrBuf); 
					ChangeAlmEvtInfo(pNMListView->iItem, ackTime); 
				}
				else
					pListCtrl->SetCheck(pNMListView->iItem, !pListCtrl->GetCheck(pNMListView->iItem));   //Wrongly came into this loop just force to Check ON and return. 
			}
			else
				pListCtrl->SetCheck(pNMListView->iItem, !pListCtrl->GetCheck(pNMListView->iItem));		//Once Checked Always force Checked ON. 
		
		}//info.flags & LVHT_ONITEMSTATEICON)
	}

	*pResult = 0;
}

void CAlmEvtViewDlg::ChangeAlmEvtInfo(int nItemToChange, CTime& ackTime)
{
	CString strLabel, strAlmTime;

	ASSERT(bRTAlmDisplay);		

	CAlmEvtList* pList;
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	
	pList = (CAlmEvtList*) &pDoc->almEvtList;
	ASSERT(nItemToChange < pList->GetCount());

	POSITION pos = pList->GetTailPosition();
	for(int i = 0; i < nItemToChange; i++)
		pList->GetPrev(pos);
	
	ASSERT(pos != NULL);
	CAlmEvtInfo* pAlmInfo = (CAlmEvtInfo*) pList->GetAt(pos);

	
#if defined _DEBUG			//Just validating the correct node modifying or not. 
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);
	
	strLabel = pListCtrl->GetItemText(nItemToChange, 0); 
	strAlmTime = pListCtrl->GetItemText(nItemToChange, 1); 
		
	ASSERT((strLabel == pAlmInfo->strTagName));
	ASSERT(strAlmTime == pDoc->GetTimeString(pAlmInfo->almInTime)); 

#endif   //_DEBUG

	if(!pAlmInfo->bAcknowledged)
	{
		pAlmInfo->almAckTime = ackTime; 
		pAlmInfo->bAcknowledged = TRUE;
	}
}

void CAlmEvtViewDlg::FreeItemMemory()
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT);
	ASSERT(pListCtrl != NULL);

	/*
	short nCount = pListCtrl->GetItemCount();
	for(int i=0; i < nCount; i++)
		delete (CAlmEvtInfo*) pListCtrl->GetItemData(i);
		*/
}

int CALLBACK CAlmEvtViewDlg::AlarmSortingFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CAlmEvtInfo* pAlm1 = (CAlmEvtInfo*) lParam1;
	CAlmEvtInfo* pAlm2 = (CAlmEvtInfo*) lParam2;

	if(lParamSort == 0)
		return pAlm1->strTagName.CompareNoCase(pAlm2->strTagName); 
	else if (lParamSort == 1)
	{
		if(pAlm1->almInTime < pAlm2->almInTime)
			return -1;
		else if (pAlm1->almInTime > pAlm2->almInTime)
			return 1; 
		else
			return 0; 
	}	
	else
		return 0; 
}

void CAlmEvtViewDlg::OnAlmEvtDisplayInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strBuf; 
	CString strChType[] = { _T("AI"), _T("HR"), _T("DI"), _T("OC"), _T("MISC_GROUP") };

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	CAlmEvtInfo* pAlmInfo = (CAlmEvtInfo*) pDispInfo->item.lParam;		
	switch(pDispInfo->item.iSubItem)
	{
	case 0:		//Tag Name
		::lstrcpy(pDispInfo->item.pszText, pAlmInfo->strTagName);
		break;

	case 1:	//Alarm In Time
		strBuf = pDoc->GetTimeString(pAlmInfo->almInTime); 
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;
	
	case 2: //Alarm out Time
		if(pAlmInfo->almOutTime != -1)
			strBuf = pDoc->GetTimeString(pAlmInfo->almOutTime); 
		else
			strBuf = _T("------"); 
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;

	case 3:
		::lstrcpy(pDispInfo->item.pszText, pAlmInfo->strValue);
		break;
	case 4:
		strBuf = pAlmInfo->bStatus ? "OUT" : "IN"; 
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;
	case 5:
		strBuf = pAlmInfo->bAcknowledged ? "YES" : "NO"; 
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;
	case 6:
		if(pAlmInfo->almAckTime != -1)
			strBuf = pDoc->GetTimeString(pAlmInfo->almAckTime); 
		else
			strBuf = _T("------");
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;
	case 7:
		::lstrcpy(pDispInfo->item.pszText, strChType[pAlmInfo->almConfigInfo.eChType]);
		break;
	case 8: 
		strBuf.Format(_T("%d"), pAlmInfo->almConfigInfo.nAddress); 
		::lstrcpy(pDispInfo->item.pszText, strBuf);
		break;
	case 9: 
		::lstrcpy(pDispInfo->item.pszText, pAlmInfo->almConfigInfo.strAlmMsg); 
		break; 
	default:
		break; 
	}//End of Switch. 

	*pResult = 0;
}

void CAlmEvtViewDlg::OnAcknowledgeAllAlarms() 
{
	CAlmEvtInfo* pAlmInfo;  
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	CAlmEvtList* pList = &pDoc->almEvtList;
	ASSERT(pList); 

	if(!pList->GetCount())
		return; 
	
	CTime ackTime = CTime::GetCurrentTime(); 
	for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pAlmInfo = (CAlmEvtInfo*)pList->GetAt(pos);
		if(!pAlmInfo->bAcknowledged)
		{
			pAlmInfo->bAcknowledged = TRUE; 
			pAlmInfo->almAckTime = ackTime; 
		}
		pList->GetNext(pos);
	}

	AddAlmEvtsToListCtrl();
}
void CAlmEvtViewDlg::ChangeAckAllBtnStatus()
{
	CButton* pBtn = (CButton*) GetDlgItem(IDC_BTN_ACKALL);
	ASSERT(pBtn);
	pBtn->EnableWindow(bRTAlmDisplay); 
}