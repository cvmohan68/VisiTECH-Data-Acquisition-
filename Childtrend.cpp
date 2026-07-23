#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "ChildTrend.h"
#include "HistoricDataInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildTrend dialog
IMPLEMENT_DYNCREATE(CChildTrend, CDialog)

CChildTrend::CChildTrend(CWnd* pParent /*=NULL*/)
	: CDialog(CChildTrend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChildTrend)
	m_nXAxisChannel = -1;
	m_nYAxisChannel = 0;
	//}}AFX_DATA_INIT

	nChildTrendID = 0;
}

void CChildTrend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildTrend)
	DDX_CBIndex(pDX, IDC_X_CHANNEL, m_nXAxisChannel);
	DDX_CBIndex(pDX, IDC_Y_CHANNEL, m_nYAxisChannel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChildTrend, CDialog)
	//{{AFX_MSG_MAP(CChildTrend)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_DISPLAYTREND, OnDisplayTrend)
	ON_BN_CLICKED(ID_BTN_HISTORIC, OnHistoricTrend)
	ON_BN_CLICKED(IDC_BTN_CHILD_GNEXT, OnGraphNextClicked)
	ON_BN_CLICKED(IDC_BTN_CHILD_GPREV, OnGraphPrevClicked)
	ON_CBN_SELCHANGE(IDC_Y_CHANNEL, OnYChannelChannge)
	ON_BN_CLICKED(ID_BTN_HDINFO, OnHistoricDataInfo)
	ON_MESSAGE  (WM_NEWDATA, OnNewData)
	ON_MESSAGE	(WM_NEWSCREEN, OnNewScreen)
	ON_CBN_SELCHANGE(IDC_X_CHANNEL, OnXChannelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CChildTrend message handlers
//////////////////////////////////////////////////////////////////////////////////
////Helping Functions. 
CDADoc* CChildTrend::GetDocument() // non-debug version is inline
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDADoc)));
	return (CDADoc*)pDoc;
}

void CChildTrend::InitializeTrendVaraibles()
{
	bRestoreTrend = FALSE;
	nChannelStartAddr = 0;
	nNoChannels = 0;
	nChannelPos = 0;
	nHistoricChannelPos = 0;
	nRTChannelPos = 0; 
	nNoScreens = 1;
	nScreenPos = 1;

	pTrend = new CTrend();
	pTrend->bIsChildTrend = TRUE;
	pTrend->pParent = this;
	pTrend->xOrgin = 50;
	pTrend->yOrgin = 250; 
	pTrend->nYScaleMaxPixels = 200;
	pTrend->nXScaleMaxPixels = MAX_CHILD_SCREEN_PIXELS;
	pTrend->nTrendPenThickNess = 1; 
	
	bRTDisplay = TRUE; 
	CButton* pButton = (CButton*)GetDlgItem(ID_BTN_HISTORIC);
	pButton->SetWindowText(_T("Historic"));

	//MultiAxisTrend Related Stuff. 
	bMultiAxisTrend = FALSE; 
	bRTMultiAxisTrend = FALSE; 
	bHistoricMultiAxisTrend = FALSE; 
	nXChannelPos = -1;		//i.e., -1
	nRTXChannelPos = 0; 
	nHistoricXChannelPos = 0; 

	BuildAddressInfo();				//It checks the address range and intializes the list box. 
	SetChannelInfo();
//	OnDisplayTrend();			
}
void CChildTrend::OnPaint() 
{
	CTagSpecInfoList* pTagList; 
	CDADoc* pDoc = (CDADoc*) GetDocument(); 

	CPaintDC dc(this); // device context for painting
	CDC* pDC = (CDC*) &dc;
	
	long nAddr = nChannelStartAddr + nChannelPos;
	long nXAddr = nChannelStartAddr + nXChannelPos;
	if(bRTDisplay)
		pTagList = &pDoc->tagInfoList;
	else
		pTagList = &historicTagInfoList; 
	
	CTrend::DrawTrendHeader(pDC, bRTDisplay, TRUE, FALSE);
	CTrend::DrawXYAxis(pDC, nAddr, pTagList, TRUE, bRTDisplay, bMultiAxisTrend,nXAddr); 
		

	CTime time = CTime::GetCurrentTime();
	pTrend->DisplayMaxMinValues(&dc, time); 

	if(bRestoreTrend)
		RestoreTrend();
}

BOOL CChildTrend::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hWhiteBrush = CreateSolidBrush(RGB(255,255,255));
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CChildTrend::OnDestroy() 
{
	CDialog::OnDestroy();
	//Your destruction code goes here. 
}

void CChildTrend::OnClose() 
{

	while(historicDataList.GetHeadPosition())
		delete historicDataList.RemoveHead();
	while(historicTagInfoList.GetHeadPosition())
		delete historicTagInfoList.RemoveHead();
	while(historicChInfoList.GetHeadPosition())
		delete historicChInfoList.RemoveHead();
	
	while(hACList.GetHeadPosition())
		delete hACList.RemoveHead();

	while(rtPointList.GetHeadPosition())	//Related to Trend Point stuff. 	
		delete rtPointList.RemoveHead();
	while(hPointList.GetHeadPosition())
		delete hPointList.RemoveHead();
	while(rtXAxisTPList.GetHeadPosition())
		delete rtXAxisTPList.RemoveHead();
	while(hXAxisTPList.GetHeadPosition())
		delete hXAxisTPList.RemoveHead();
	
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT_VALID(pFrame);
	pFrame->OnChildTrendClose(nChildTrendID);
	nChildTrendID = 0;

	delete pTrend;

	CDialog::OnClose();
}

void CChildTrend::OnDisplayTrend() 
{
	UpdateData(TRUE);
	nChannelPos = m_nYAxisChannel;
	
	if(m_nXAxisChannel > 0 )		//0 For Time any Other value for Multi Axis Trend.  initiallly -1. 
	{
		bMultiAxisTrend = TRUE; 
		pTrend->bMultiAxisTrend = TRUE; 
		nXChannelPos = m_nXAxisChannel - 1;		//Remove 1 for Time 
		nNoScreens = 1;
		nScreenPos = 1;
		UpdateScreenButtons();
	}
	else	//Normal Trend case 
	{
		bMultiAxisTrend = FALSE; 
		pTrend->bMultiAxisTrend = FALSE; 
		nNoScreens = 1;
		nScreenPos = 1;
		UpdateScreenButtons();

	}	
	if(DisplayTrend(nChannelStartAddr + nChannelPos) != TRUE)
	{
		Invalidate(TRUE);
		UpdateWindow();
	}
	
}

void CChildTrend::OnHistoricTrend() 
{
	CString strFileName;
	CDADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(bRTDisplay)
	{
		if(historicDataList.GetCount() == 0)
		{
			CFileDialog dlg(TRUE, "dat", "DA*.dat");
			dlg.m_ofn.lpstrInitialDir  = pDoc->strDataDir;							
			dlg.m_ofn.lpstrTitle = _T("Open Historic Data File"); 
			if(dlg.DoModal() == IDOK)
			{
				strFileName = dlg.GetPathName();
				pDoc->ReadDataFromFile(strFileName, &historicDataList, &historicTagInfoList, &historicChInfoList);
			}
			else
				return;
		}
		if(historicDataList.GetCount() == 0)
		{
			pDoc->DAMessageBox(_T("Sorry! No historic data found in the given file !"), _T("DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			return;
		}
		CButton* pButton = (CButton*) GetDlgItem(ID_BTN_HISTORIC);
		pButton->SetWindowText("Real Time");
		bRTDisplay = FALSE;
		pTrend->bRTDisplay = FALSE;
		nRTChannelPos = nChannelPos; 
		nRTXChannelPos = nXChannelPos; 
		bRTMultiAxisTrend = bMultiAxisTrend;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(ID_BTN_HISTORIC);
		pButton->SetWindowText("Historic");
		bRTDisplay = TRUE;
		pTrend->bRTDisplay = TRUE;
		nHistoricChannelPos = nChannelPos;
		nHistoricXChannelPos = nXChannelPos;
		bHistoricMultiAxisTrend = bMultiAxisTrend;
	}

	if(bRTDisplay)			//While swiching between real time and historic channels keep holding their channel pos. 
	{
		nChannelPos = nRTChannelPos;
		nXChannelPos = nRTXChannelPos;
		bMultiAxisTrend = bRTMultiAxisTrend; 
	}
	else
	{
		nChannelPos = nHistoricChannelPos; 
		nXChannelPos = nHistoricXChannelPos; 
		bMultiAxisTrend = bHistoricMultiAxisTrend; 
	}
	pTrend->bMultiAxisTrend = bMultiAxisTrend; 

	nScreenPos = 1;
	nNoScreens = 1;
	pTrend->nTrendPointStart = 0;
	pTrend->nTrendPointEnd = 0;

	BuildAddressInfo();
	SetChannelInfo();
	UpdateScreenButtons();
	
	bRestoreTrend = FALSE;
	Invalidate(TRUE);
	UpdateWindow();
	
	if(DisplayTrend(nChannelStartAddr+nChannelPos) != TRUE)
	{
		Invalidate(TRUE);
		UpdateWindow();
	}
	ActivateHistoricDataInfoBtn(); 
}

void CChildTrend::OnGraphPrevClicked() 
{
	nScreenPos--;
	bRestoreTrend = FALSE;
	Invalidate(TRUE);
	UpdateWindow();
	UpdateScreenButtons();
	DisplayTrendScreen(nScreenPos);
	bRestoreTrend = TRUE;
}
void CChildTrend::OnGraphNextClicked() 
{
	nScreenPos++;
	bRestoreTrend = FALSE;
	Invalidate(TRUE);
	UpdateWindow();
	UpdateScreenButtons();
	DisplayTrendScreen(nScreenPos);
	bRestoreTrend = TRUE;
}

void CChildTrend::OnYChannelChannge() 
{
	UpdateData(TRUE); 
	OnDisplayTrend();	
}
void CChildTrend::OnXChannelChange() 
{
	UpdateData(TRUE); 
	OnDisplayTrend();	
}

void CChildTrend::OnOK() 
{
	CDialog::OnOK();
	SendMessage(WM_CLOSE);
}

HBRUSH CChildTrend::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CChildTrend::OnNewData()
{
	POSITION pos;
	CStorageData* pData;
	CTrendPoint* pPrevPoint;
	short nChID, nXChID;
	short nPresentValue = 0;
	short nXPresentValue = 0;
	CTrendPointList* pTrendPtList; 
	CTrendPointList* pXAxisTPList; 

	CDADoc* pDoc = (CDADoc*) GetDocument();
	ASSERT_VALID(pDoc);
	
	if(!bRTDisplay ||  nScreenPos != nNoScreens)
		return;

	CDataList* pDataList = &pDoc->dataList;
	pTrendPtList = &rtPointList;
	
	if(!pTrendPtList->GetCount())
	{
		OnDisplayTrend(); 
		return;
	}

	nChID = nChannelStartAddr + nChannelPos;
	

	if(nChID < pDoc->nAStartAddress || nChID >= (pDoc->nAStartAddress + pDoc->nANoChannels) )
		return;
	pPrevPoint = pTrendPtList->GetTail();
	
	if(bMultiAxisTrend)
	{
		nXChID = nChannelStartAddr + nXChannelPos;
		ASSERT(nXChID >= pDoc->nAStartAddress && nXChID < (pDoc->nAStartAddress + pDoc->nANoChannels));
		pXAxisTPList = &rtXAxisTPList; 
		ASSERT(pTrendPtList->GetCount() == pXAxisTPList->GetCount()); 
	}

	for(pos = pDataList->GetTailPosition(); pos != NULL; )  //To get Previous drawn position. 
	{
		pData = (CStorageData*)pDataList->GetAt(pos);
		if(pPrevPoint->dataTime >= pData->dataTime)
			break;
		pDataList->GetPrev(pos);
	}

	for(pos; pos != NULL;)
	{
		pData = (CStorageData*)pDataList->GetAt(pos);
		if( nChID >= pData->m_nAStartAddress && nChID < (pData->m_nAStartAddress + pData->m_nANoChannels))
		{
			if(pData->dataTime > pPrevPoint->dataTime)
			{
				nPresentValue = pData->pAData[nChID - pData->m_nAStartAddress]; 
				CTrendPoint* pPoint = new CTrendPoint(pData->dataTime, nPresentValue);
				pTrendPtList->AddTail(pPoint);
				
				if(bMultiAxisTrend)  
				{
					ASSERT(nXChID >= pData->m_nAStartAddress && nXChID < (pData->m_nAStartAddress + pData->m_nANoChannels));
					nXPresentValue = pData->pAData[nXChID - pData->m_nAStartAddress]; 
					CTrendPoint* pXPoint = new CTrendPoint(pData->dataTime, nXPresentValue);
					pXAxisTPList->AddTail(pXPoint);
				}
			}
		}
		pDataList->GetNext(pos);
	}//End for loop.

	if(bMultiAxisTrend)
		ASSERT(pTrendPtList->GetCount() == pXAxisTPList->GetCount()); 	
	
	int nCount = pTrendPtList->GetCount();
	if(!nCount) 
		return;
	ASSERT(pTrend->nTrendPointEnd < nCount);

	bRestoreTrend = FALSE;
	if(!pTrend->bTrendInfoDone || (pTrend->pCurrentChInfo->bYAutoScale && (nPresentValue >= pTrend->yMax || nPresentValue <= pTrend->yMin)))  //Resizing the Y-axis scale. 
		DisplayTrend(nChID);

	if(bMultiAxisTrend)
	{
		if(nXPresentValue >= pTrend->xMax || nXPresentValue <= pTrend->xMin)
		{
			AdjustXAxisScaleValues(nXPresentValue);
			DisplayTrend(nChID);	
		}
	}
	
	CClientDC dc(this);
	pTrend->UpdateTrend(&dc, pTrend->nTrendPointEnd, nCount-1 , pTrendPtList, nScreenPos, nNoScreens, pXAxisTPList); 	
	
	pTrend->DisplayPV(&dc, nPresentValue, TRUE);	//To display PV and SP, etc., 
	pTrend->nTrendPointEnd = nCount-1; 
	bRestoreTrend = TRUE;
}

void CChildTrend::UpdateScreenButtons()
{
	CButton *pPrevBtn, *pNextBtn;

	pPrevBtn = (CButton*) GetDlgItem(IDC_BTN_CHILD_GPREV);
	pNextBtn = (CButton*) GetDlgItem(IDC_BTN_CHILD_GNEXT);
	pPrevBtn->EnableWindow(TRUE);
	pNextBtn->EnableWindow(TRUE);
	if(nScreenPos <= 1)
		pPrevBtn->EnableWindow(FALSE);
	if(nScreenPos >= nNoScreens)
		pNextBtn->EnableWindow(FALSE);
}

void CChildTrend::SetTrendTitle()
{
	CString strTitle, strXTitle;
	CString strBuf; 
	CDADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(bRTDisplay)
	{
		strTitle = pDoc->GetDescName(&pDoc->tagInfoList, INPUT_REGISTER, nChannelStartAddr+nChannelPos);
		strXTitle = pDoc->GetDescName(&pDoc->tagInfoList, INPUT_REGISTER, nChannelStartAddr+nXChannelPos);
	}
	else
	{
		strTitle = pDoc->GetDescName(&historicTagInfoList, INPUT_REGISTER, nChannelStartAddr+nChannelPos);
		strXTitle = pDoc->GetDescName(&historicTagInfoList, INPUT_REGISTER, nChannelStartAddr+nXChannelPos);
	}
	
	if(strTitle.IsEmpty())
		strTitle.Format(_T("CHANNEL:%d"), nChannelStartAddr+nChannelPos);
	
	if(strXTitle.IsEmpty())
		strXTitle.Format(_T("CHANNEL:%d"), nChannelStartAddr+nXChannelPos);
	
	if(!bMultiAxisTrend)
		strBuf.Format(_T("Trend for %s"), strTitle); 
	else
		strBuf.Format(_T("Trend between %s Vs %s"), strXTitle, strTitle); 
	
	SetWindowText(strBuf);
}
/////////////////////////////////////////////////////////////////////////////////
///Trend related functions. 
void CChildTrend::SetChannelInfo()
{
	CDADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CComboBox* pXAxisComb = (CComboBox*) GetDlgItem(IDC_X_CHANNEL);
	ASSERT(pXAxisComb);
	CComboBox* pYAxisComb = (CComboBox*) GetDlgItem(IDC_Y_CHANNEL);
	ASSERT(pYAxisComb);
	
	pXAxisComb->ResetContent();
	pYAxisComb->ResetContent();
	
	pXAxisComb->InsertString(-1, _T("Time"));
	CString strBuf;	
	for(long i = nChannelStartAddr; i < nChannelStartAddr+nNoChannels; i++)
	{
		strBuf.Empty();
		if(bRTDisplay)
			strBuf = pDoc->GetTagString(&pDoc->tagInfoList, INPUT_REGISTER, i);	
		else
			strBuf = pDoc->GetTagString(&historicTagInfoList, INPUT_REGISTER, i);
		
		if (strBuf.IsEmpty())
			strBuf.Format(_T("Channel %d"), i);
		pXAxisComb->InsertString(-1, strBuf);
		pYAxisComb->InsertString(-1, strBuf);
	}
	
	if(bMultiAxisTrend)
		pXAxisComb->SetCurSel(nXChannelPos + 1);		// +1 to nNo of Channels 1 for Time at 0 indexed. 
	else
		pXAxisComb->SetCurSel(0);			//Non multi Axis trend so always Time at 0 indexed. 
	
	pYAxisComb->SetCurSel(nChannelPos);  
	UpdateData(TRUE); 
}

//////////////////////////////////////////////////////////////////////////////////
///Miscelleneous Helper functions. 
void CChildTrend::BuildAddressInfo()
{
	CAddressChannelInfo* pChInfo;
	CString tmpStr;

	CDADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(bRTDisplay)
	{
		pTrend->BuildMultiAddressList(&pDoc->dataList, &pDoc->rtACList, bRTDisplay);
		if(!pDoc->rtACList.IsEmpty())
		{
			pChInfo = (CAddressChannelInfo*) pDoc->rtACList.GetHead();
			nChannelStartAddr = pChInfo->m_nAStartAddress;
			pTrend->nChannelStartAddr = nChannelStartAddr;
			nNoChannels = pChInfo->m_nANoChannels;
		}
	}
	else
	{
		pTrend->BuildMultiAddressList(&historicDataList, &hACList, bRTDisplay);
		if(!hACList.IsEmpty())
		{
			pChInfo = (CAddressChannelInfo*) hACList.GetHead();
			nChannelStartAddr = pChInfo->m_nAStartAddress;
			nNoChannels = pChInfo->m_nANoChannels;
			pTrend->nChannelStartAddr = nChannelStartAddr;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
///Miscelleneous Routines. 
BOOL CChildTrend::DisplayTrend(long nCh)
{
	CTrendPointList* pTPList = NULL;
	CTrendPointList* pXAxisTPList = NULL; 
	CDataList* pDataList;
	
	CDADoc* pDoc = (CDADoc*)GetDocument();
	ASSERT_VALID(pDoc);

	if(bRTDisplay)
	{
		pDataList = &pDoc->dataList;			//Current data list maininated at one Place only. 
		pTPList = &rtPointList;
		if(bMultiAxisTrend)
			pXAxisTPList = &rtXAxisTPList; 
	}
	else
	{
		pDataList = &historicDataList;
		pTPList = &hPointList;
		if(bMultiAxisTrend)
			pXAxisTPList = &hXAxisTPList; 
	}
	
	
	ASSERT(pTrend != NULL);
	SetTrendTitle();		
	if(bMultiAxisTrend)
	{
		long nXCh = nChannelStartAddr + nXChannelPos; //nXChannelPos is 0 based no need to subract 1 already done. 
		if(!pTrend->BuildXAxisTrendInfo(nXCh, pDataList, pXAxisTPList))  //Always call before BuildTrendInfo;  
			return FALSE; 
	}

	if(!pTrend->BuildTrendInfo(nCh, pDataList, pTPList))  //It will overrides what ever the info done in MultiAxis trend case 
		return FALSE;									//Alwasy call X axis first then Y-asix	


	nScreenPos = 1;
	nNoScreens = 1;
	UpdateScreenButtons();	

	//Invalidating and display the complete trend. 
	bRestoreTrend = FALSE;
	Invalidate(TRUE);
	UpdateWindow();

	CClientDC dc(this);
	pTrend->DisplayCompleteTrend(&dc, nNoScreens, pTPList,  pXAxisTPList);  

	nScreenPos = nNoScreens; 
	UpdateScreenButtons();
	bRestoreTrend = TRUE;
	return TRUE;
}

void CChildTrend::RestoreTrend()
{
	CClientDC dc(this);
	CTrendPointList* pTPList;
	CTrendPointList* pXAxisTPList; 

	if(bRTDisplay)
	{
		pTPList = &rtPointList;
		pXAxisTPList = &rtXAxisTPList; 
	}
	else
	{
		pTPList = &hPointList;
		pXAxisTPList = &hXAxisTPList; 
	}

	pTrend->DisplayTrendScreen(&dc, nScreenPos, nNoScreens, pTPList, pXAxisTPList );
}

void CChildTrend::DisplayTrendScreen(short nScreen)
{
	RestoreTrend();
}

void CChildTrend::OnNewScreen()
{
	bRestoreTrend = FALSE;
	nScreenPos++;
	nNoScreens++;
	Invalidate(TRUE);
	UpdateWindow();
	UpdateScreenButtons();
	bRestoreTrend = TRUE;
}

//The below funciton is called from MainFrame to display the initial trend. 
void CChildTrend::DisplayCurrentTrend()
{
	OnDisplayTrend();
}

void CChildTrend::OnHistoricDataInfo() 
{
	CHistoricDataInfoDlg dlg;
	dlg.nHDCurrentChannelType = INPUT_REGISTER; 
	dlg.nHDCurrentChannelAddress = nChannelStartAddr + nChannelPos;	 //
	dlg.pTagInfoList = &historicTagInfoList;
	dlg.pChInfoList = &historicChInfoList; 
	dlg.pDataList = &historicDataList; 
	dlg.DoModal(); 
	OnDisplayTrend();	
}

void CChildTrend::ActivateHistoricDataInfoBtn()
{
	CButton* pBtn = (CButton*) GetDlgItem(ID_BTN_HDINFO);
	ASSERT(pBtn); 

	if(bRTDisplay)
		pBtn->EnableWindow(FALSE); 
	else
		pBtn->EnableWindow(TRUE); 
}
/////////////////////////////////////////////////////////////////////////////////////////////
//////MultiAxisTrend related Stuff. 	
void CChildTrend::AdjustXAxisScaleValues(short nXPresentValue)
{
	if(nXPresentValue >= pTrend->xMax)
	{
		pTrend->xMax = (short) (nXPresentValue+ (float)nXPresentValue * 20.0/100);
		pTrend->xMax = ((pTrend->xMax/4)+1) * 4;		//Roundoff to multiples of 4
	}
	if(nXPresentValue <= pTrend->xMin)
	{
		pTrend->xMin = (short) (nXPresentValue+ (float)nXPresentValue * 20.0/100);
		pTrend->xMin = ((pTrend->xMin/4)+1) * 4;		//Roundoff to multiples of 4
				
	}
}