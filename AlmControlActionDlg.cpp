// AlmControlActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "AlmControlActionDlg.h"
#include "MainFrm.h"
#include "DADoc.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlmControlActionDlg dialog


CAlmControlActionDlg::CAlmControlActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlmControlActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlmControlActionDlg)
	m_nActionType = 1;							//Default discrete action. 
	m_nValue = 0;
	m_bDValue = -1;
	m_nAddress = -1; 
	//}}AFX_DATA_INIT

	eCh = MISC_GROUP;
	nAlmAddress = 0;		//Invalid Address. 
	nAlmCode = MISC_ALM_CODE; 
}


void CAlmControlActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlmControlActionDlg)
	DDX_Radio(pDX, IDC_ANALOG, m_nActionType);			
	DDX_Text(pDX, IDC_CA_AVALUE, m_nValue);
	DDX_Radio(pDX, IDC_CA_OFF, m_bDValue);
	DDX_CBIndex(pDX, IDC_CA_ADDRESS, m_nAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlmControlActionDlg, CDialog)
	//{{AFX_MSG_MAP(CAlmControlActionDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CA_SET, OnSet)
	ON_BN_CLICKED(IDC_ANALOG, OnAnalogActionSelected)
	ON_BN_CLICKED(IDC_DISCRETE, OnDiscreteActionSelected)
	ON_CBN_SELCHANGE(IDC_CA_ADDRESS, OnControlActionAddressChange)
	ON_BN_CLICKED(IDC_CA_DELETE, OnControlActionDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlmControlActionDlg message handlers

BOOL CAlmControlActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
		
	InitializeAddressComboBox(m_nActionType); 
	UpdateExistingControlActionCfg(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAlmControlActionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 
}

void CAlmControlActionDlg::OnSet() 
{
	if(!UpdateData(TRUE)) 
		return;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	if(m_nAddress == -1 )
	{
		pDoc->DAMessageBox(_T("Please Select Proper Address Value !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}

	if(m_nActionType == 1 && m_bDValue == -1)
	{
		pDoc->DAMessageBox(_T("Please Select Proper Discrete Action !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return; 
	}
	
	SetAlarmActionConfiguration(); 
}

void CAlmControlActionDlg::OnAnalogActionSelected() 
{
	UpdateData(TRUE); 
	InitializeAddressComboBox(m_nActionType); 
	
}

void CAlmControlActionDlg::OnDiscreteActionSelected() 
{
	UpdateData(TRUE); 
	InitializeAddressComboBox(m_nActionType); 
	
}
void CAlmControlActionDlg::InitializeAddressComboBox(int nActionType)
{
	CString strBuf; 
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	CComboBox* pCombAddr = (CComboBox*) GetDlgItem(IDC_CA_ADDRESS);
	ASSERT_VALID(pCombAddr); 
	CEdit* pValue = (CEdit*) GetDlgItem(IDC_CA_AVALUE);
	ASSERT_VALID(pValue); 
	CButton* pOnRadio = (CButton*) GetDlgItem(IDC_CA_ON); 
	ASSERT_VALID(pOnRadio); 
	CButton* pOffRadio = (CButton*) GetDlgItem(IDC_CA_OFF); 
	ASSERT_VALID(pOffRadio); 

	pCombAddr->ResetContent();

	if(nActionType)			//Discrete Action
	{
		for(short i = 0; i< pDoc->nDRWNoChannels; i++)   
		{
			strBuf  =  pDoc->GetTagString(&pDoc->tagInfoList, OUTPUT_COIL, pDoc->nDRWStartAddress + i); 
			if(strBuf.IsEmpty())
				strBuf.Format(_T("%d"), pDoc->nDRWStartAddress + i);
			pCombAddr->InsertString(-1, strBuf);
		}
		pValue->EnableWindow(FALSE); 
		pOnRadio->EnableWindow(TRUE); 
		pOffRadio->EnableWindow(TRUE); 
	}
	else
	{
		for(short i = 0; i< pDoc->nARWNoChannels; i++)   
		{
			strBuf  =  pDoc->GetTagString(&pDoc->tagInfoList, HOLDING_REGISTER, pDoc->nARWStartAddress + i); 
			if(strBuf.IsEmpty())
				strBuf.Format(_T("%d"), pDoc->nARWStartAddress + i);
			pCombAddr->InsertString(-1, strBuf);
		}
		pValue->EnableWindow(TRUE); 
		pOnRadio->EnableWindow(FALSE); 
		pOffRadio->EnableWindow(FALSE); 
	}

	m_nValue = 0;
	m_bDValue = -1; 
	m_nAddress = -1;
	UpdateData(FALSE); 
}

void CAlmControlActionDlg::OnControlActionAddressChange() 
{
	OnCAAddressChange(TRUE);  
}

void CAlmControlActionDlg::UpdateExistingControlActionCfg()
{
	CAlmEvtConfigInfo* pAlmInfo;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	for(POSITION pos = pDoc->almEvtConfigList.GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pDoc->almEvtConfigList.GetAt(pos);
		if(pAlmInfo->eChType == eCh && pAlmInfo->nAddress == nAlmAddress && pAlmInfo->nAlmCode == nAlmCode)
		{
			if(pAlmInfo->nActionRegister)	// 0 means no control action. 
			{
				
				if(pAlmInfo->bIsAnalogAction)
				{
					InitializeAddressComboBox(0); 
					m_nActionType = 0;				//0 means first item in group i.e., Analog
					m_nValue = pAlmInfo->nActionValue; 
					m_nAddress = pAlmInfo->nActionRegister - pDoc->nARWStartAddress; 

				}
				else
				{
					InitializeAddressComboBox(1); 
					m_nActionType = 1;				// 1 Second item i.e., Discrete. 
					if(pAlmInfo->nActionValue)
						m_bDValue = 1;
					else
						m_bDValue = 0;
					m_nAddress = pAlmInfo->nActionRegister - pDoc->nDRWStartAddress; 
				}
				
			}
			
			UpdateData(FALSE); 
			OnCAAddressChange(FALSE); 
			break; 
		}
		pDoc->almEvtConfigList.GetNext(pos);
	} //End for loop
}


void CAlmControlActionDlg::SetAlarmActionConfiguration()
{
	CAlmEvtConfigInfo* pAlmInfo;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	for(POSITION pos = pDoc->almEvtConfigList.GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pDoc->almEvtConfigList.GetAt(pos);
		if(pAlmInfo->eChType == eCh && pAlmInfo->nAddress == nAlmAddress && pAlmInfo->nAlmCode == nAlmCode)
		{
			if(m_nActionType == 0)	//i.e., Analog Action
			{
				pAlmInfo->nActionValue = m_nValue; 
				pAlmInfo->nActionRegister = pDoc->nARWStartAddress + m_nAddress; 
				pAlmInfo->bIsAnalogAction = TRUE; 
			
			}
			else
			{
				pAlmInfo->nActionValue = m_bDValue;		//i.e., 0 or 1
				pAlmInfo->nActionRegister = pDoc->nDRWStartAddress + m_nAddress; 
				pAlmInfo->bIsAnalogAction = FALSE;
			}
			break; 
		}
		pDoc->almEvtConfigList.GetNext(pos); 
	}

	ASSERT(pos != NULL);
}

void CAlmControlActionDlg::OnCAAddressChange(BOOL bInvalidate) 
{
	CString strON, strOFF; 
	UpdateData(TRUE); 

	if(!m_nActionType && bInvalidate)
		m_nValue = 0; 
	else
	{
		CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc); 
		CButton* pOnRadio = (CButton*) GetDlgItem(IDC_CA_ON); 
		ASSERT_VALID(pOnRadio); 
		CButton* pOffRadio = (CButton*) GetDlgItem(IDC_CA_OFF); 
		ASSERT_VALID(pOffRadio); 
		
		pDoc->GetDiscreteCaption(&pDoc->tagInfoList, OUTPUT_COIL, pDoc->nDRWStartAddress + m_nAddress, strON, strOFF);
		
		pOnRadio->SetWindowText(strON); 
		pOffRadio->SetWindowText(strOFF); 
		if(bInvalidate)
			m_bDValue = -1; 
	}

	UpdateData(FALSE); 
}

void CAlmControlActionDlg::OnControlActionDelete() 
{
	CAlmEvtConfigInfo* pAlmInfo;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	for(POSITION pos = pDoc->almEvtConfigList.GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pDoc->almEvtConfigList.GetAt(pos);
		if(pAlmInfo->eChType == eCh && pAlmInfo->nAddress == nAlmAddress && pAlmInfo->nAlmCode == nAlmCode)
		{
			pAlmInfo->nActionRegister = 0; 
			break; 
		}
		pDoc->almEvtConfigList.GetNext(pos); 
	}
	m_nActionType = 1;			//Make default discrete Action. 
	InitializeAddressComboBox(m_nActionType); //Reinitialize every thing. 
}
