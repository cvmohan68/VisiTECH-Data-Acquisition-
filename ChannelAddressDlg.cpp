// ChannelAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "ChannelAddressDlg.h"
#include "TagSpecInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLG_BKG_COLOR	FRONTPANEL_BKG_COLOR				//LOGIN_DLG_BKG_COLOR


/////////////////////////////////////////////////////////////////////////////
// CChannelAddressDlg dialog

CChannelAddressDlg::CChannelAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChannelAddressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChannelAddressDlg)
	m_nANoChannels = 16;
	m_nAStartAddress = 1;
	m_nDNoChannels = 16;
	m_nDStartAddress = 1;
	m_nPV = 0;
	//}}AFX_DATA_INIT
}


void CChannelAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelAddressDlg)
	DDX_Text(pDX, IDC_ANOCHANNELS, m_nANoChannels);
	DDV_MinMaxInt(pDX, m_nANoChannels, 0, 200);
	DDX_Text(pDX, IDC_ASADDRESS, m_nAStartAddress);
	DDV_MinMaxInt(pDX, m_nAStartAddress, 0, 65536);
	DDX_Text(pDX, IDC_DNOCHANNELS, m_nDNoChannels);
	DDV_MinMaxInt(pDX, m_nDNoChannels, 0, 200);
	DDX_Text(pDX, IDC_DSADDRESS, m_nDStartAddress);
	DDV_MinMaxInt(pDX, m_nDStartAddress, 0, 65536);
	DDX_Radio(pDX, IDC_PV, m_nPV);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChannelAddressDlg, CDialog)
	//{{AFX_MSG_MAP(CChannelAddressDlg)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_BN_CLICKED(IDC_PV, OnPVClicked)
	ON_BN_CLICKED(IDC_PARAMETERS, OnParametersClicked)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelAddressDlg message handlers


BOOL CChannelAddressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateDlgParameters();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				// EXCEPTION: OCX Property Pages should return FALSE
}

void CChannelAddressDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CChannelAddressDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CChannelAddressDlg::OnSet() 
{
	UpdateData(TRUE);
	if(m_nPV == 0)
	{
		m_nARONoChannels = m_nANoChannels; 
		m_nAROStartAddress = m_nAStartAddress;
		m_nDRONoChannels = m_nDNoChannels;
		m_nDROStartAddress = m_nDStartAddress;
		
	}
	else
	{
		m_nARWNoChannels = m_nANoChannels; 
		m_nARWStartAddress = m_nAStartAddress;
		m_nDRWNoChannels = m_nDNoChannels; 
		m_nDRWStartAddress = m_nDStartAddress;
	}
}

void CChannelAddressDlg::OnPVClicked() 
{
	UpdateData(TRUE);
	UpdateDlgParameters();
	UpdateData(FALSE);
}

void CChannelAddressDlg::OnParametersClicked() 
{
	UpdateData(TRUE);
	UpdateDlgParameters();
	UpdateData(FALSE);
}

void CChannelAddressDlg::UpdateDlgParameters()
{
	if(m_nPV == 0)
	{
		m_nANoChannels = m_nARONoChannels;
		m_nAStartAddress = m_nAROStartAddress;
		m_nDNoChannels = m_nDRONoChannels;
		m_nDStartAddress = m_nDROStartAddress;
	}
	else
	{
		m_nANoChannels = m_nARWNoChannels;
		m_nAStartAddress = m_nARWStartAddress;
		m_nDNoChannels = m_nDRWNoChannels;
		m_nDStartAddress = m_nDRWStartAddress;	
	}
	UpdateData(FALSE);
}
  

HBRUSH CChannelAddressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CDADoc* pDoc = (CDADoc*) pFrame->GetActiveDocument();
	ASSERT_VALID(pDoc); 

	return pDoc->ChangeDlgBkgColor(FRONTPANEL_BKG_COLOR, pDoc->m_pBkgColorBrush, pDC, pWnd, nCtlColor); 
}
