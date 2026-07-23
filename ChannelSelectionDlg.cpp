// ChannelSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "TextView.h"
#include "ChannelSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelSelectionDlg dialog
CChannelSelectionDlg::CChannelSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChannelSelectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChannelSelectionDlg)
	m_nChannelType = 0;
	//}}AFX_DATA_INIT
}

void CChannelSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelSelectionDlg)
	DDX_Radio(pDX, IDC_SC_ANALOG, m_nChannelType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChannelSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CChannelSelectionDlg)
	ON_BN_CLICKED(IDC_SELECT, OnSelect)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_BN_CLICKED(IDC_SC_ANALOG, OnAnalogSelected)
	ON_BN_CLICKED(IDC_SC_DISCRETE, OnDiscreteSelected)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelSelectionDlg message handlers
BOOL CChannelSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OnAnalogSelected(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChannelSelectionDlg::OnSelect() 
{
	int nMaxSelected;
	int* pSelected;
	CString strBuf;
	
	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);
	CListBox* pSelectedList = (CListBox*) GetDlgItem(IDC_SELECTED_CHANNELS);
	
	nMaxSelected = pExistingList->GetSelCount();
	pSelected = new int[nMaxSelected];
	pExistingList->GetSelItems(nMaxSelected, pSelected);

	for(int i = 0; i < nMaxSelected; i++)
	{
		pExistingList->GetText(pSelected[i], strBuf);		
		pSelectedList->InsertString(-1, strBuf );
	}
	
	for(i = 0; i < nMaxSelected; i++)
	{
		pExistingList->GetSelItems(nMaxSelected, pSelected);
		pExistingList->DeleteString(pSelected[0]);
	}
}

void CChannelSelectionDlg::OnRemove() 
{
	int nMaxSelected;
	int* pSelected;
	CString strBuf;
	
	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);
	CListBox* pSelectedList = (CListBox*) GetDlgItem(IDC_SELECTED_CHANNELS);
	
	nMaxSelected = pSelectedList->GetSelCount();
	pSelected = new int[nMaxSelected];
	pSelectedList->GetSelItems(nMaxSelected, pSelected);

	for(int i = 0; i < nMaxSelected; i++)
	{
		pSelectedList->GetText(pSelected[i], strBuf);		
		pExistingList->InsertString(-1, strBuf );
	}
	
	for(i = 0; i < nMaxSelected; i++)
	{
		pSelectedList->GetSelItems(nMaxSelected, pSelected);
		pSelectedList->DeleteString(pSelected[0]);
	}

	delete[] pSelected; 
}

void CChannelSelectionDlg::OnOK() 
{
	CDialog::OnOK();
}

void CChannelSelectionDlg::OnSet() 
{
	char strBuf[20];
	short nVal; 

	UpdateData(TRUE);
	
	CTextView* pView = (CTextView*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
	ASSERT_VALID(pView); 
	CDADoc* pDoc = (CDADoc*) pView->GetDocument(); 
	ASSERT_VALID(pDoc); 

	CListBox* pSelectedList = (CListBox*) GetDlgItem(IDC_SELECTED_CHANNELS);
	short nCount = pSelectedList->GetCount();
	
	if(m_nChannelType == 0)				//Analog case 		
	{
		if(pView->bRTDisplay)
		{
			pView->nANoSelectedItems= nCount;
			if(pView->pASelectedItems != NULL)
				delete[] pView->pASelectedItems;
			pView->pASelectedItems = new int[nCount];
		}
		else
		{
			pView->nHANoSelectedItems = nCount;
			if(pView->pHASelectedItems != NULL)
				delete[] pView->pHASelectedItems;
			pView->pHASelectedItems = new int[nCount];
		}
		for(int i = 0; i <nCount; i++)
		{
			pSelectedList->GetText(i, strBuf);		
			nVal = atoi(strBuf); 
			if(!nVal)
			{
				nVal = GetChannelIDFromTagName(pView->bRTDisplay, INPUT_REGISTER, strBuf);
				if(!nVal)
					pDoc->DAMessageBox(_T("Some problem in Converting from Tag name to ID !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			}
			
			if(pView->bRTDisplay)
				pView->pASelectedItems[i]= nVal;
			else
				pView->pHASelectedItems[i]= nVal;
		}//For loop
	}	//End if Analog case
	else						//Discrete case 
	{
		if(pView->bRTDisplay)
		{
			pView->nDNoSelectedItems= nCount;
			if(pView->pDSelectedItems != NULL)
				delete[] pView->pDSelectedItems;
			pView->pDSelectedItems = new int[nCount];
		}
		else
		{
			pView->nHDNoSelectedItems = nCount;
			if(pView->pHDSelectedItems != NULL)
				delete[] pView->pHDSelectedItems;
			pView->pHDSelectedItems = new int[nCount];
		}
		for(int i = 0; i <nCount; i++)
		{
			pSelectedList->GetText(i, strBuf);
			nVal = atoi(strBuf); 
			if(!nVal)
			{
				nVal = GetChannelIDFromTagName(pView->bRTDisplay, INPUT_DISCRETE, strBuf);
				if(!nVal)
					pDoc->DAMessageBox(_T("Some problem in Converting from Tag name to ID !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			}

			if(pView->bRTDisplay)
				pView->pDSelectedItems[i]= nVal;
			else
				pView->pHDSelectedItems[i]= nVal; 
		}//For loop
	}//End else for discrete. 
}

void CChannelSelectionDlg::OnAnalogSelected() 
{
	int* pASItems;
	short nASelectedItems;
	CString strBuf;
	CTagSpecInfoList* pList;
	
	CTextView* pView = (CTextView*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
	ASSERT_VALID(pView); 
	CDADoc* pDoc = (CDADoc*) pView->GetDocument(); 
	ASSERT(pDoc); 

	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);

	if(pView->bRTDisplay)
	{
		pASItems = pView->pASelectedItems;
		nASelectedItems = pView->nANoSelectedItems;
		pList = &pDoc->tagInfoList; 
	}
	else
	{
		pASItems = pView->pHASelectedItems;
		nASelectedItems = pView->nHANoSelectedItems;
		pList = &pDoc->historicTagInfoList; 
	}

	CleanLists();			//First clear both the lists. 
	for(short i = 0; i< pView->nANoChannels; i++)   
	{
		strBuf  =  pDoc->GetTagString(pList, INPUT_REGISTER, pView->nAStartAddress + i); 
		if(strBuf.IsEmpty())
			strBuf.Format(_T("%d"), pView->nAStartAddress + i);
		pExistingList->InsertString(-1, strBuf);
	}
	
	SetSelectedList(INPUT_REGISTER, pASItems, nASelectedItems);
}

void CChannelSelectionDlg::OnDiscreteSelected() 
{
	int* pDSItems;
	short nDSelectedItems;
	CString strBuf;
	CTagSpecInfoList* pList;
	
	CTextView* pView = (CTextView*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);
	CDADoc* pDoc = (CDADoc*) pView->GetDocument(); 
	ASSERT(pDoc); 

	if(pView->bRTDisplay)
	{
		pDSItems = pView->pDSelectedItems;
		nDSelectedItems = pView->nDNoSelectedItems;
		pList = &pDoc->tagInfoList; 
	}
	else
	{
		pDSItems = pView->pHDSelectedItems;
		nDSelectedItems = pView->nHDNoSelectedItems;
		pList = &pDoc->historicTagInfoList; 
	}

	CleanLists();			//First clear all the contents 
	for(short i = 0; i< pView->nDNoChannels; i++)   
	{
		strBuf  =  pDoc->GetTagString(pList, INPUT_DISCRETE, pView->nDStartAddress + i); 
		if(strBuf.IsEmpty())
			strBuf.Format(_T("%d"), pView->nDStartAddress + i);
		pExistingList->InsertString(-1, strBuf);
	}
	
	SetSelectedList(INPUT_DISCRETE, pDSItems, nDSelectedItems);
}


void CChannelSelectionDlg::CleanLists()
{
	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);
	CListBox* pSelectedList = (CListBox*) GetDlgItem(IDC_SELECTED_CHANNELS);
	
	short nCount = pExistingList->GetCount();
	for(short i = 0; i < nCount;  i++)
		pExistingList->DeleteString(0);

	nCount = pSelectedList->GetCount();
	for(i = 0; i < nCount;  i++)
		pSelectedList->DeleteString(0);
}

void CChannelSelectionDlg::SetSelectedList(eChannelType eCh, int* pItems, short nNoSelectedItems)
{
	CTagSpecInfoList* pList;
	CString strBuf;

	CTextView* pView = (CTextView*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
	ASSERT_VALID(pView); 
	CDADoc* pDoc = (CDADoc*) pView->GetDocument(); 
	ASSERT(pDoc); 

	CListBox* pExistingList = (CListBox*) GetDlgItem(IDC_EXISTING_CHANNELS);
	CListBox* pSelectedList = (CListBox*) GetDlgItem(IDC_SELECTED_CHANNELS);
	
	if(pView->bRTDisplay)
		pList = &pDoc->tagInfoList;
	else
		pList = &pDoc->historicTagInfoList; 

	for(short i = 0; i < nNoSelectedItems ; i++)
	{
		strBuf = pDoc->GetTagString(pList, eCh,  pItems[i]); 
		if(strBuf.IsEmpty())
			strBuf.Format(_T("%d"),  pItems[i]);
		short nSelPos = pExistingList->FindString(-1, strBuf);	
		pExistingList->DeleteString(nSelPos);
		pSelectedList->AddString(strBuf);
	}
}

short CChannelSelectionDlg::GetChannelIDFromTagName(BOOL bRTDisplay, eChannelType eCh, CString strTagName)
{
	short nVal = 0; 
	CTagSpecInfoList* pList;
	CTagSpecInfo* pInfo;
	
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 
		
	if(bRTDisplay)
		pList = &pDoc->tagInfoList;
	else
		pList = &pDoc->historicTagInfoList; 

	for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
	{
		pInfo = pList->GetAt(pos);
		if(pInfo->nChannelType == eCh && pInfo->strTagName == strTagName)
		{
			nVal = pInfo->nChannelAddr;
			break; 
		}
		pList->GetNext(pos); 
	}

	ASSERT(pos != NULL); 

	return nVal; 
}


HBRUSH CChannelSelectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 
}

