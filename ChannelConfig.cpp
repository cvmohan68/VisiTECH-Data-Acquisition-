// ChannelConfig.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "ChannelConfig.h"
#include "ChannelConfigSheet.h"
#include "ChannelInfo.h"
#include "TagSpecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig1 property page

IMPLEMENT_DYNCREATE(CChannelConfig1, CPropertyPage)

CChannelConfig1::CChannelConfig1() : CPropertyPage(CChannelConfig1::IDD)
{
	//{{AFX_DATA_INIT(CChannelConfig1)
	m_nChannelAddr = 0;
	m_strTagName = _T("");
	m_strDescName = _T("");
	m_nTimeScale = 0;		//5_MIN_TREND 
	m_nYMax = 100;
	m_nYMin = 0;
	m_bYAutoScale = TRUE;
	m_bDisplaySP = FALSE;
	m_bDisplayHighAlarm = FALSE;
	m_bDisplayLowAlarm = FALSE;
	m_bDisplayHiHiAlarm = FALSE;
	m_bDisplayLoLoAlarm = FALSE;
	//}}AFX_DATA_INIT
	nCurrentChAddr = 0;
}

CChannelConfig1::~CChannelConfig1()
{
}

void CChannelConfig1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelConfig1)
	DDX_CBIndex(pDX, IDC_CHANNEL_ADDR, m_nChannelAddr);
	DDX_Text(pDX, IDC_TAGNAME, m_strTagName);
	DDV_MaxChars(pDX, m_strTagName, 10);
	DDX_Text(pDX, IDC_DESCNAME, m_strDescName);
	DDV_MaxChars(pDX, m_strDescName, 20);
	DDX_Radio(pDX, IDC_5MIN_TREND, m_nTimeScale);
	DDX_Text(pDX, IDC_YMAX, m_nYMax);
	DDX_Text(pDX, IDC_YMIN, m_nYMin);
	DDX_Check(pDX, IDC_YSCALE_CHECK, m_bYAutoScale);
	DDX_Check(pDX, IDC_TREND_SP, m_bDisplaySP);
	DDX_Check(pDX, IDC_TREND_HALARM, m_bDisplayHighAlarm);
	DDX_Check(pDX, IDC_TREND_LALARM, m_bDisplayLowAlarm);
	DDX_Check(pDX, IDC_TREND_HHALARM, m_bDisplayHiHiAlarm);
	DDX_Check(pDX, IDC_TREND_LLALARM, m_bDisplayLoLoAlarm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChannelConfig1, CPropertyPage)
	//{{AFX_MSG_MAP(CChannelConfig1)
	ON_BN_CLICKED(IDC_YSCALE_CHECK, OnYScaleTypeChanged)
	ON_CBN_SELCHANGE(IDC_CHANNEL_ADDR, OnChannelAddrChange)
	ON_BN_CLICKED(IDC_5MIN_TREND, OnXScaleResolutionChanged)
	ON_EN_UPDATE(IDC_YMAX, OnYMaxChanged)
	ON_EN_UPDATE(IDC_YMIN, OnYMinChanged)
	ON_BN_CLICKED(IDC_BTN_TAGINFO, OnSetTagInfo)
	ON_BN_CLICKED(IDC_TREND_SP, OnDisplaySetPoint)
	ON_BN_CLICKED(IDC_TREND_LALARM, OnDisplayLowAlarm)
	ON_BN_CLICKED(IDC_TREND_HALARM, OnDisplayHighAlarm)
	ON_BN_CLICKED(IDC_15MIN_TREND, OnXScaleResolutionChanged)
	ON_BN_CLICKED(IDC_30MIN_TREND, OnXScaleResolutionChanged)
	ON_BN_CLICKED(IDC_1HOUR_TREND, OnXScaleResolutionChanged)
	ON_BN_CLICKED(IDC_6HOUR_TREND, OnXScaleResolutionChanged)
	ON_BN_CLICKED(IDC_TREND_HHALARM, OnDisplayHiHiAlarm)
	ON_BN_CLICKED(IDC_TREND_LLALARM, OnDisplayLoLoAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig1 message handlers

BOOL CChannelConfig1::OnInitDialog() 
{
	CString strBuf;

	CPropertyPage::OnInitDialog();

	CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_CHANNEL_ADDR);
	
	short nCount = pDoc->chInfoList.GetCount();
	ASSERT(nCount == pDoc->nANoChannels);

	for(short i = 0; i < pDoc->nANoChannels; i++)
	{
		strBuf.Format("%d ", pDoc->nAStartAddress + i);
		pComb->InsertString(-1, strBuf);
	}
	
	if(!nCurrentChAddr)
		nCurrentChAddr = pDoc->nAStartAddress;
	else if(nCurrentChAddr < pDoc->nAStartAddress || nCurrentChAddr >= pDoc->nAStartAddress + pDoc->nANoChannels)
		nCurrentChAddr = pDoc->nAStartAddress;

	pComb->SetCurSel(nCurrentChAddr - pDoc->nAStartAddress);
	OnChannelAddressChange(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CChannelConfig1::OnSetActive() 
{
	CheckSpAlarmsValidity();	
	return CPropertyPage::OnSetActive();
}
void CChannelConfig1::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage::OnOK();
}

void CChannelConfig1::OnChannelAddrChange()		//In Combobox addressed changes 
{
	OnChannelAddressChange(); 
}	

void CChannelConfig1::OnSetTagInfo() 
{
	CTagSpecDlg  tagDlg;
	tagDlg.m_nChType = INPUT_REGISTER;
	tagDlg.m_nChAddress = nCurrentChAddr-1;
	tagDlg.DoModal();
	OnChannelAddressChange(); 
}

void CChannelConfig1::OnChannelAddressChange() 
{
	CChannelInfo* pChInfo;
	POSITION pos;

	UpdateData(TRUE);
	CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	nCurrentChAddr = pDoc->nAStartAddress + m_nChannelAddr;
	
	for(pos = pDoc->chInfoList.GetHeadPosition(); pos != NULL; )
	{
		pChInfo = pDoc->chInfoList.GetAt(pos);	
		if(pChInfo->nAAddress == nCurrentChAddr)
			break;
		pDoc->chInfoList.GetNext(pos);
	}
	ASSERT(pChInfo->nAAddress == nCurrentChAddr);
	
	m_strTagName = pDoc->GetTagString(&pDoc->tagInfoList, INPUT_REGISTER, nCurrentChAddr);
	m_strDescName = pDoc->GetDescName(&pDoc->tagInfoList, INPUT_REGISTER, nCurrentChAddr);
	m_nTimeScale = GetTimeScalePos(pChInfo->nTimeScale);
	m_bYAutoScale = pChInfo->bYAutoScale;
	m_nYMax = pChInfo->nYMax;
	m_nYMin = pChInfo->nYMin;
	m_bDisplaySP= pChInfo->bDisplaySP;
	m_bDisplayHighAlarm = pChInfo->bDisplayHiAlarm;
	m_bDisplayLowAlarm = pChInfo->bDisplayLoAlarm;
	m_bDisplayHiHiAlarm = pChInfo->bDisplayHiHiAlarm; 
	m_bDisplayLoLoAlarm = pChInfo->bDisplayLoLoAlarm; 

	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	
	
	//For SP & Alarm Page
	pChConfigSheet->m_spAlarmConfig.m_nAddr = nCurrentChAddr;
	pChConfigSheet->m_spAlarmConfig.m_strTagName = m_strTagName;
	pChConfigSheet->m_spAlarmConfig.m_strDescName = m_strDescName;
	pChConfigSheet->m_spAlarmConfig.cfgChInfo = *pChInfo; 	//All the parameter transfers one shot. 
	
	//For Scaling Display page
	pChConfigSheet->m_chConfig3.m_nAddr = nCurrentChAddr;
	pChConfigSheet->m_chConfig3.m_strTagName = m_strTagName;
	pChConfigSheet->m_chConfig3.m_strDescName = m_strDescName;
	pChConfigSheet->m_chConfig3.m_nSignalConditioning = pChInfo->nSignalConditioning;
	pChConfigSheet->m_chConfig3.m_bEnableCustomScaling = pChInfo->bEndabledCustomScaling;
	pChConfigSheet->m_chConfig3.m_nProcessedMax = pChInfo->nYMax;
	pChConfigSheet->m_chConfig3.m_nProcessedMin = pChInfo->nYMin;
	pChConfigSheet->m_chConfig3.m_nRawMax = pChInfo->nRawYMax;
	pChConfigSheet->m_chConfig3.m_nRawMin = pChInfo->nRawYMin;
	if(m_bYAutoScale)											//Just for Integrity. 
		pChConfigSheet->m_chConfig3.m_bEnableCustomScaling = FALSE; 

	//For Alarm & Event Config Page
	pChConfigSheet->m_almEvtPropPage.strCaption = _T("Alm & Event");
//	pChConfigSheet->m_almEvtPropPage.m_nChannelAddr = m_nChannelAddr;
	
	UpdateData(FALSE);
	OnYScaleTypeChanged();
	SetModified(FALSE);			//It's making true in OnYScaleTypeChanged 
}

void CChannelConfig1::OnYScaleTypeChanged() 
{
	UpdateData(TRUE);	
	
	CEdit* pMax = (CEdit*) GetDlgItem(IDC_YMAX);
	CEdit* pMin = (CEdit*) GetDlgItem(IDC_YMIN);
	
	if(m_bYAutoScale)
	{
		pMax->EnableWindow(FALSE);
		pMin->EnableWindow(FALSE);
		MarkSpAlarmsDirty();
		CheckSpAlarmsValidity();
	}
	else
	{
		pMax->EnableWindow(TRUE);
		pMin->EnableWindow(TRUE);
	}	
	
	SetModified(TRUE);
}

void CChannelConfig1::OnXScaleResolutionChanged()   //Zoom related stuff. 
{
	SetModified(TRUE);
}
void CChannelConfig1::OnYMaxChanged() 
{
	UpdateData(TRUE);
	MarkSpAlarmsDirty();
	CheckSpAlarmsValidity();
	SetModified(TRUE);
}
void CChannelConfig1::OnYMinChanged() 
{
	UpdateData(TRUE);
	MarkSpAlarmsDirty();
	CheckSpAlarmsValidity();
	SetModified(TRUE);
}

void CChannelConfig1::OnDisplaySetPoint() 
{
	UpdateData(TRUE);
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidSP && m_bDisplaySP)
	{
		m_bDisplaySP = FALSE;
		UpdateData(FALSE);
		CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc);
		pDoc->DAMessageBox(_T("Invalid Set Point Configuration !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	}
	SetModified(TRUE);
}

void CChannelConfig1::OnDisplayLowAlarm() 
{
	UpdateData(TRUE);
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoAlarm && m_bDisplayLowAlarm)
	{
		m_bDisplayLowAlarm = FALSE;
		UpdateData(FALSE);
		CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc);
		pDoc->DAMessageBox(_T("Invalid  Low  Alarm Configuration!"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	}
	
	SetModified(TRUE);
}

void CChannelConfig1::OnDisplayHighAlarm() 
{
	UpdateData(TRUE);
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiAlarm && m_bDisplayHighAlarm)
	{
		m_bDisplayHighAlarm = FALSE;
		UpdateData(FALSE);
		CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc);
		pDoc->DAMessageBox(_T("Invalid  High  Alarm Configuration!"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	}
	SetModified(TRUE);
}

void CChannelConfig1::OnDisplayHiHiAlarm() 
{
	UpdateData(TRUE);
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiHiAlarm && m_bDisplayHiHiAlarm)
	{
		m_bDisplayHiHiAlarm = FALSE;
		UpdateData(FALSE);
		CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc);
		pDoc->DAMessageBox(_T("Invalid  High High Alarm Configuration !"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);

	}
	SetModified(TRUE);
}

void CChannelConfig1::OnDisplayLoLoAlarm() 
{
	UpdateData(TRUE);
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);
	
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoLoAlarm && m_bDisplayLoLoAlarm)
	{
		m_bDisplayLoLoAlarm = FALSE;
		UpdateData(FALSE);
		CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
		ASSERT_VALID(pDoc);
		pDoc->DAMessageBox(_T("Invalid  Low Low Alarm Configuration!"), _T("DA Package!"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	}
	SetModified(TRUE);
}


BOOL CChannelConfig1::OnApply() 
{
	CChannelInfo* pInfo;
	CChannelInfo* pChInfo = new CChannelInfo();

	UpdateData(TRUE);
	CDADoc* pDoc = (CDADoc*) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);

	//First second page. 
	*pChInfo = pChConfigSheet->m_spAlarmConfig.cfgChInfo; 
	
	//Now first page. 
	pChInfo->nAAddress = nCurrentChAddr;
	pChInfo->nTimeScale = GetTimeScaleValue(m_nTimeScale);
	pChInfo->nYMax = m_nYMax;
	pChInfo->nYMin = m_nYMin;
	pChInfo->bYAutoScale = m_bYAutoScale;
	pChInfo->bDisplaySP = m_bDisplaySP;
	pChInfo->bDisplayHiAlarm = m_bDisplayHighAlarm;
	pChInfo->bDisplayLoAlarm = m_bDisplayLowAlarm;
	pChInfo->bDisplayHiHiAlarm = m_bDisplayHiHiAlarm;
	pChInfo->bDisplayLoLoAlarm = m_bDisplayLoLoAlarm;

	//Now Third page. 
	pChInfo->nSignalConditioning = pChConfigSheet->m_chConfig3.m_nSignalConditioning;
	pChInfo->nRawYMax = pChConfigSheet->m_chConfig3.m_nRawMax;
	pChInfo->nRawYMin = pChConfigSheet->m_chConfig3.m_nRawMin;
	if(m_bYAutoScale)
		pChConfigSheet->m_chConfig3.m_bEnableCustomScaling = FALSE; 
	pChInfo->bEndabledCustomScaling = pChConfigSheet->m_chConfig3.m_bEnableCustomScaling;
	
	//Now update Channel config List. 
	for(POSITION pos = pDoc->chInfoList.GetHeadPosition(); pos != NULL; )
	{
		pInfo = pDoc->chInfoList.GetAt(pos);	
		if(pInfo->nAAddress == nCurrentChAddr)
		{
			pDoc->chInfoList.SetAt(pos, pChInfo);
			delete pInfo;
			break;
		}
		pDoc->chInfoList.GetNext(pos);
	}

	return CPropertyPage::OnApply();
}

void CChannelConfig1::MarkSpAlarmsDirty()
{
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);

	pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidSP = FALSE;
	pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiAlarm = FALSE;
	pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoAlarm = FALSE;
	pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiHiAlarm = FALSE;
	pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoLoAlarm = FALSE;
}

void CChannelConfig1::CheckSpAlarmsValidity()
{
	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());
	ASSERT_VALID(pSheet);
	CChannelConfigSheet* pChConfigSheet = DYNAMIC_DOWNCAST(CChannelConfigSheet, pSheet);
	ASSERT_VALID(pChConfigSheet);

	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidSP)
		m_bDisplaySP = FALSE;
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiAlarm)
		m_bDisplayHighAlarm = FALSE;
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoAlarm)
		m_bDisplayLowAlarm = FALSE;
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidHiHiAlarm)
		m_bDisplayHiHiAlarm = FALSE;
	if(!pChConfigSheet->m_spAlarmConfig.cfgChInfo.bValidLoLoAlarm)
		m_bDisplayLoLoAlarm = FALSE;
	
	UpdateData(FALSE);
}

short CChannelConfig1::GetTimeScalePos(short nMins)
{
	if(nMins  == TREND_5MIN_TIME)
		return TREND_5MIN_SCALE; 
	else if(nMins  == TREND_15MIN_TIME)
		return TREND_15MIN_SCALE; 
	else if(nMins  == TREND_30MIN_TIME)
		return TREND_30MIN_SCALE; 
	else if(nMins  == TREND_1HOUR_TIME)
		return TREND_1HOUR_SCALE;
	else if(nMins  == TREND_6HOUR_TIME)
		return TREND_6HOUR_SCALE;
	else
		return TREND_5MIN_SCALE; 
}

short CChannelConfig1::GetTimeScaleValue(short nPos)
{
	switch(nPos)
	{
	case TREND_5MIN_SCALE:
		return TREND_5MIN_TIME;
	case TREND_15MIN_SCALE:
		return TREND_15MIN_TIME;
	case TREND_30MIN_SCALE:
		return TREND_30MIN_TIME;
	case TREND_1HOUR_SCALE:
		return TREND_1HOUR_TIME;
	case TREND_6HOUR_SCALE:
		return TREND_6HOUR_TIME;
	default:
		return TREND_5MIN_TIME; 
	}
}


