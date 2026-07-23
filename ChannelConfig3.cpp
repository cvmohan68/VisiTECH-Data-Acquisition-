// ChannelConfig3.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "ChannelConfig3.h"
#include "ChannelConfigSheet.h"
#include "MainFrm.h" 
#include "DADoc.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig3 property page

IMPLEMENT_DYNCREATE(CChannelConfig3, CPropertyPage)

CChannelConfig3::CChannelConfig3() : CPropertyPage(CChannelConfig3::IDD)
{
	//{{AFX_DATA_INIT(CChannelConfig3)
	m_nAddr = 0;
	m_strDescName = _T("");
	m_strTagName = _T("");
	m_nSignalConditioning = 0;
	m_nRawMin = 0;
	m_nRawMax = 0;
	m_nProcessedMin = 0;
	m_nProcessedMax = 0;
	m_bEnableCustomScaling = FALSE;
	//}}AFX_DATA_INIT
}

CChannelConfig3::~CChannelConfig3()
{
}

void CChannelConfig3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelConfig3)
	DDX_Text(pDX, IDC_CHANNEL_ADDR, m_nAddr);
	DDV_MinMaxInt(pDX, m_nAddr, 1, 65500);
	DDX_Text(pDX, IDC_DESCNAME, m_strDescName);
	DDX_Text(pDX, IDC_TAGNAME, m_strTagName);
	DDX_Radio(pDX, IDC_SC_LINEAR, m_nSignalConditioning);
	DDX_Text(pDX, IDC_RAW_MIN, m_nRawMin);
	DDX_Text(pDX, IDC_RAW_MAX, m_nRawMax);
	DDX_Text(pDX, IDC_PROCESSED_MIN, m_nProcessedMin);
	DDX_Text(pDX, IDC_PROCESSED_MAX, m_nProcessedMax);
	DDX_Check(pDX, IDC_CHECK_CUSTOMSCALING, m_bEnableCustomScaling);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChannelConfig3, CPropertyPage)
	//{{AFX_MSG_MAP(CChannelConfig3)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMSCALING, OnCustomScalingClikced)
	ON_BN_CLICKED(IDC_SC_LINEAR, OnSignalConditioningClicked)
	ON_EN_CHANGE(IDC_RAW_MAX, OnRawValueChange)
	ON_BN_CLICKED(IDC_SC_LOG, OnSignalConditioningClicked)
	ON_BN_CLICKED(IDC_SC_SQRROOT, OnSignalConditioningClicked)
	ON_EN_CHANGE(IDC_RAW_MIN, OnRawValueChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig3 message handlers

BOOL CChannelConfig3::OnSetActive() 
{
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	
	if(pChConfigSheet->m_chConfig1.m_bYAutoScale && m_bEnableCustomScaling)
		m_bEnableCustomScaling = FALSE;
	
	m_nProcessedMax = pChConfigSheet->m_chConfig1.m_nYMax;
	m_nProcessedMin = pChConfigSheet->m_chConfig1.m_nYMin;
	UpdateData(FALSE);
	
	OnSignalConditioningClicked(); 
	OnCustomScalingChange();	
	
	return CPropertyPage::OnSetActive();
}

void CChannelConfig3::OnSignalConditioningClicked() 
{
	UpdateData(TRUE);
	if(m_bEnableCustomScaling)
		m_nSignalConditioning = 0;
	UpdateData(FALSE);
	SetModified(TRUE);
}

void CChannelConfig3::OnRawValueChange() 
{
	SetModified(TRUE);	
}

void CChannelConfig3::OnCustomScalingClikced() 
{
	CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	UpdateData(TRUE);	

	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);

	if(m_bEnableCustomScaling )
	{
		if(pChConfigSheet->m_chConfig1.m_bYAutoScale)
		{
			pDoc->DAMessageBox(_T("Custom Scaling doesn't work in Auto Scale Mode !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
			m_bEnableCustomScaling = FALSE;
		}
		else
			m_nSignalConditioning = 0;
	}

	UpdateData(FALSE);
	SetModified(TRUE);
	OnCustomScalingChange();
}

void CChannelConfig3::OnCustomScalingChange()
{
	CEdit* pMax = (CEdit*) GetDlgItem(IDC_RAW_MAX);
	CEdit* pMin = (CEdit*) GetDlgItem(IDC_RAW_MIN);

	if(m_bEnableCustomScaling)
	{
		pMax->EnableWindow(TRUE);
		pMin->EnableWindow(TRUE);
	}
	else
	{
		pMax->EnableWindow(FALSE);
		pMin->EnableWindow(FALSE);
	}
}
