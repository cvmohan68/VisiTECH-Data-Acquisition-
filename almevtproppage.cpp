// AlmEvtPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "da.h"
#include "MainFrm.h"
#include "DADoc.h"
#include "AlmEvtPropPage.h"
#include "TagSpecDlg.h"
#include "AlmControlActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ANALOG_ALARMS	9

CString strAlmType[MAX_ALM_TYPES] = {
				_T("ALARM"),
				_T("EVENT"), 
				_T("MESSAGE")
			};

CString strAlmCode[MAX_ALM_CODES] = {	
				_T("MISC_ALM_CODE"), 
				_T("ON_TO_OFF"), 
				_T("OFF_TO_ON"), 				 
				_T("LTHIHIALMVALUE_TO_GTHIHIALMVALUE"), 
				_T("GTHIHIALMVALUE_TO_LTHIHIALMVALUE"), 
				_T("LTHIALMVALUE_TO_GTHIALMVALUE"), 
				_T("GTHIALMVALUE_TO_LTHIALMVALUE"), 
				_T("GTLOALMVALUE_TO_LTLOALMVALUE"), 
				_T("LTLOALMVALUE_TO_GTLOALMVALUE"), 
				_T("GTLOLOALMVALUE_TO_LTLOLOALMVALUE"), 
				_T("LTLOLOALMVALUE_TO_GTLOLOALMVALUE"), 
				_T("PV_RATE_OF_CHNGE"),
				_T("HR_VALUE_CHANGE")
			};

CString strAlmCaptions[MAX_ANALOG_ALARMS] = {
				_T("< HiHi Alarm Value"), 
				_T("> HiHi Alarm Value"), 
				_T("< Hi Alarm Value"), 
				_T("> Hi Alarm Value"), 
				_T("> Lo Alarm Value"), 
				_T("< Lo Alarm Value"), 
				_T("> LoLo Alarm Value"), 
				_T("< LoLo Alarm Value"), 
				_T("  ROC Alarm"), 
			}; 

void InitializeChannelAddrComb(eChannelType eCh, CComboBox* pComb, long& nStartAddr, CString& strTagName, CString& strDescName);
void UpdateTagInfo(eChannelType eCh, long nChAddr, CString& strTagName, CString& strDescName);
void DisplayTagInfoDlg(eChannelType eCh, long nChAddr);
void AddListCtrlHeader(CListCtrl* pListCtrl);
BOOL IsThisAlarmConfigExists(eChannelType eCh, unsigned long nAddr, short nAlmCode);
void AddAlarmToAlmConfigList(eChannelType eCh, long nAddr, short nAlmType, short nAlmCode, CString strAlmMsg);
void AddAlarmToListCtrl(short nAlmCode, short nAlmType, CString strAlmMsg, CListCtrl* pListCtrl);	
void UpdateAlarmsListCtrl(eChannelType eCh, unsigned long nAddr, CListCtrl* pListCtrl);
short GetAlmCode(CString strAlmCode);
BOOL DeleteAlarmFromList(eChannelType eCh, unsigned long nAddr, short nAlmCode);
void DeleteAlarmFromListCtrl(eChannelType eCh, unsigned long nAddr, CListCtrl* pListCtrl);
void OnControlActionClicked(CListCtrl* pListCtrl, eChannelType eCh, int nAlmAddress ); 

/////////////////////////////////////////////////////////////////////////////
// CAnalogAlmEvtPropPage dialog

IMPLEMENT_DYNCREATE(CAnalogAlmEvtPropPage, CPropertyPage)

CAnalogAlmEvtPropPage::CAnalogAlmEvtPropPage():CPropertyPage(CAnalogAlmEvtPropPage::IDD)
{
	//{{AFX_DATA_INIT(CAnalogAlmEvtPropPage)
	m_nChannelAddr = 0;
	m_strTagName = _T("");
	m_strDescName = _T("");
	m_strAlmMessage = _T("");
	m_nAlmType = 0;
	m_nAIChangeFrom = 0;
	m_nAIChangeTo = 0;
	//}}AFX_DATA_INIT
	strCaption = _T("Analog RO");
	nAStartAddr = 0;
}
CAnalogAlmEvtPropPage::~CAnalogAlmEvtPropPage()
{

}
void CAnalogAlmEvtPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalogAlmEvtPropPage)
	DDX_CBIndex(pDX, IDC_COMB_ARO_CHADDR, m_nChannelAddr);
	DDX_Text(pDX, IDC_EDT_ARO_TAGNAME, m_strTagName);
	DDX_Text(pDX, IDC_EDT_ARO_DESCNAME, m_strDescName);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strAlmMessage);
	DDV_MaxChars(pDX, m_strAlmMessage, 50);
	DDX_CBIndex(pDX, IDC_COMB_ALMTYPE, m_nAlmType);
	DDX_CBIndex(pDX, IDC_COMB_ANALOGALM_FROM, m_nAIChangeFrom);
	DDX_CBIndex(pDX, IDC_COMB_ANALOGALM_TO, m_nAIChangeTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAnalogAlmEvtPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnalogAlmEvtPropPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTN_ARO_SETTAGINFO, OnTagInfo)
	ON_CBN_SELCHANGE(IDC_COMB_ARO_CHADDR, OnChannelAddrChange)
	ON_CBN_SELCHANGE(IDC_COMB_ANALOGALM_FROM, OnAlmConditionFromChange)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_BTN_CONTROLACTION, OnAnalogControlActionClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAnalogAlmEvtPropPage message handlers
BOOL CAnalogAlmEvtPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_ARO_CHADDR);
	InitializeChannelAddrComb(INPUT_REGISTER, pComb, nAStartAddr,  m_strTagName, m_strDescName);
	UpdateData(FALSE);
		
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOG);
	AddListCtrlHeader(pListCtrl);	
	UpdateAlarmsListCtrl(INPUT_REGISTER, nAStartAddr + m_nChannelAddr, pListCtrl);
	
	
	InitializeFromComboBoxAlmCaptions();
	UpdateAlmChangeToComboBox();
	SetWindowText(strCaption);				//Now Working check other method. 
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAnalogAlmEvtPropPage::OnChannelAddrChange() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOG);
	UpdateData(TRUE);
	unsigned long nChAddr = nAStartAddr + m_nChannelAddr;
	UpdateTagInfo(INPUT_REGISTER, nChAddr, m_strTagName, m_strDescName);
	UpdateAlarmsListCtrl(INPUT_REGISTER, nChAddr, pListCtrl);
	m_nAIChangeFrom = 0;
	UpdateData(FALSE);	
	
	UpdateAlmChangeToComboBox();
}

void CAnalogAlmEvtPropPage::OnAlmConditionFromChange() 
{
	UpdateAlmChangeToComboBox();
}

void CAnalogAlmEvtPropPage::OnAdd() 
{
	short nAlmCode = 0;
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	UpdateData(TRUE);
	
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOG);
	ASSERT_VALID(pListCtrl); 
	
	unsigned long nAddr = nAStartAddr + m_nChannelAddr;
	
	nAlmCode = m_nAIChangeFrom + 3; //Skipping 0, 1, 2 Codes
									//if m_nAIChangeFrom = 0 Then nAlmCode = LTHIHIALMVALUE_TO_GTHIHIALMVALUE

	if(!IsThisAlarmConfigExists(INPUT_REGISTER, nAddr, nAlmCode))
	{
		AddAlarmToAlmConfigList(INPUT_REGISTER, nAddr, m_nAlmType, nAlmCode, m_strAlmMessage);
		AddAlarmToListCtrl(nAlmCode, m_nAlmType, m_strAlmMessage, pListCtrl);	
	}
	else
		pDoc->DAMessageBox(_T("Already Configuraiton Exists !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

void CAnalogAlmEvtPropPage::OnDelete() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOG);
	DeleteAlarmFromListCtrl(INPUT_REGISTER, nAStartAddr + m_nChannelAddr, pListCtrl);
}

void CAnalogAlmEvtPropPage::OnTagInfo() 
{
	DisplayTagInfoDlg(INPUT_REGISTER, m_nChannelAddr);
}

void CAnalogAlmEvtPropPage::InitializeFromComboBoxAlmCaptions()
{
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_ANALOGALM_FROM);
	pComb->ResetContent();
	for(int i = 0; i < MAX_ANALOG_ALARMS; i++)
		pComb->InsertString(-1, strAlmCaptions[i]); 
	m_nAIChangeFrom = 0; 
	UpdateData(FALSE); 
}

void CAnalogAlmEvtPropPage::UpdateAlmChangeToComboBox()
{
	UpdateData(TRUE);
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_ANALOGALM_TO);
	pComb->ResetContent();
	
	if(m_nAIChangeFrom != MAX_ANALOG_ALARMS -1 )
	{
		short nModule = m_nAIChangeFrom % 2; 
		if(nModule)
		{
			pComb->InsertString(-1, strAlmCaptions[m_nAIChangeFrom - 1]);
			m_nAlmType = 1; 
		}
		else
		{
			pComb->InsertString(-1, strAlmCaptions[m_nAIChangeFrom + 1]); 
			m_nAlmType = 0; 
		}
		
		m_strAlmMessage = _T(""); 
		pComb->EnableWindow(TRUE); 
	}
	else
	{
		pComb->EnableWindow(FALSE); 
		m_nAlmType = 0;				//ROC is type of alarm. 
		m_strAlmMessage = _T(""); 
	}

	m_nAIChangeTo = 0;
	UpdateData(FALSE);
}

void CAnalogAlmEvtPropPage::OnAnalogControlActionClicked() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOG);	
	ASSERT_VALID(pListCtrl); 

	OnControlActionClicked(pListCtrl, INPUT_REGISTER, nAStartAddr + m_nChannelAddr); 
}

/////////////////////////////////////////////////////////////////////////////
// CDiscreteAlmEvtPropPage dialog

IMPLEMENT_DYNCREATE(CDiscreteAlmEvtPropPage, CPropertyPage)

CDiscreteAlmEvtPropPage::CDiscreteAlmEvtPropPage():CPropertyPage(CDiscreteAlmEvtPropPage::IDD)
{
	//{{AFX_DATA_INIT(CDiscreteAlmEvtPropPage)
	m_strTagName = _T("");
	m_strDescName = _T("");
	m_strAlmMessage = _T("");
	m_nChannelAddr = 0;
	m_nDIChangeFrom = 0;
	m_nDIChangeTo = 0;
	m_nAlmType = 0;
	//}}AFX_DATA_INIT
	nDStartAddr = 0;
}

CDiscreteAlmEvtPropPage::~CDiscreteAlmEvtPropPage()
{
}

void CDiscreteAlmEvtPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiscreteAlmEvtPropPage)
	DDX_Text(pDX, IDC_EDT_DRO_TAGNAME, m_strTagName);
	DDX_Text(pDX, IDC_EDT_DRO_DESCNAME, m_strDescName);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strAlmMessage);
	DDX_CBIndex(pDX, IDC_COMB_DRO_CHADDR, m_nChannelAddr);
	DDX_CBIndex(pDX, IDC_COMB_DISCALM_TO, m_nDIChangeTo);
	DDX_CBIndex(pDX, IDC_COMB_DISCALM_FROM, m_nDIChangeFrom);
	DDX_CBIndex(pDX, IDC_COMB_ALMTYPE, m_nAlmType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiscreteAlmEvtPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDiscreteAlmEvtPropPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTN_DRO_SETTAGINFO, OnTagInfo)
	ON_CBN_SELCHANGE(IDC_COMB_DRO_CHADDR, OnChannelAddrChange)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_COMB_DISCALM_TO, OnAlmConditionToChange)
	ON_CBN_SELCHANGE(IDC_COMB_DISCALM_FROM, OnAlmConditionFromChange)
	ON_BN_CLICKED(IDC_BTN_DCONTROLACTION, OnDiscreteControlActionClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDiscreteAlmEvtPropPage message handlers
BOOL CDiscreteAlmEvtPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_DRO_CHADDR);
	InitializeChannelAddrComb(INPUT_DISCRETE, pComb, nDStartAddr, m_strTagName, m_strDescName);
	UpdateData(FALSE);

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETE);
	AddListCtrlHeader(pListCtrl);	
	UpdateAlarmsListCtrl(INPUT_DISCRETE, nDStartAddr + m_nChannelAddr, pListCtrl);

	UpdateAlmChangeFromToComboBoxes();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDiscreteAlmEvtPropPage::OnChannelAddrChange() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETE);
	UpdateData(TRUE);
	unsigned long nChAddr = nDStartAddr + m_nChannelAddr;
	UpdateTagInfo(INPUT_DISCRETE, nChAddr, m_strTagName, m_strDescName);
	UpdateAlarmsListCtrl(INPUT_DISCRETE, nChAddr, pListCtrl);
	UpdateData(FALSE);	
	
	UpdateAlmChangeFromToComboBoxes();
}

void CDiscreteAlmEvtPropPage::OnAlmConditionFromChange() 
{
	OnAlmConditionToChange();
}

void CDiscreteAlmEvtPropPage::OnAlmConditionToChange() 
{
	UpdateData(TRUE);
	if(!m_nDIChangeFrom && m_nDIChangeTo)
		m_strAlmMessage = strOFF + _T("_TO_") + strON;
	else if(m_nDIChangeFrom && !m_nDIChangeTo)
		m_strAlmMessage = strON + _T("_TO_") + strOFF;
	else
		m_strAlmMessage = _T("INVALID ALM CONFIG");
	UpdateData(FALSE);
}

void CDiscreteAlmEvtPropPage::OnAdd() 
{
	short nAlmCode = 0;
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	UpdateData(TRUE);
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETE);
	if(m_nDIChangeFrom == m_nDIChangeTo)
	{
		pDoc->DAMessageBox(_T("Invalid Alarm Configuration !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return ;
	}
	
	unsigned long nAddr = nDStartAddr + m_nChannelAddr;
	if(!m_nDIChangeFrom)
		nAlmCode = OFF_TO_ON;
	else
		nAlmCode = ON_TO_OFF;
	
	if(!IsThisAlarmConfigExists(INPUT_DISCRETE, nAddr, nAlmCode))
	{
		AddAlarmToAlmConfigList(INPUT_DISCRETE, nAddr, m_nAlmType, nAlmCode, m_strAlmMessage);
		AddAlarmToListCtrl(nAlmCode, m_nAlmType, m_strAlmMessage, pListCtrl);	
	}
	else
		pDoc->DAMessageBox(_T("Already Configuration Exists !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

void CDiscreteAlmEvtPropPage::OnDelete() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETE);
	DeleteAlarmFromListCtrl(INPUT_DISCRETE, nDStartAddr + m_nChannelAddr, pListCtrl);
}
void CDiscreteAlmEvtPropPage::OnTagInfo() 
{
	DisplayTagInfoDlg(INPUT_DISCRETE, m_nChannelAddr);
}

void CDiscreteAlmEvtPropPage::UpdateAlmChangeFromToComboBoxes()
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CComboBox* pCombFrom = (CComboBox*) GetDlgItem(IDC_COMB_DISCALM_FROM);
	CComboBox* pCombTo = (CComboBox*) GetDlgItem(IDC_COMB_DISCALM_TO);
	
	pCombFrom->ResetContent();
	pCombTo->ResetContent();
	
	long nAddr = nDStartAddr + m_nChannelAddr;
	pDoc->GetDiscreteCaption(&pDoc->tagInfoList, INPUT_DISCRETE, nAddr, strON, strOFF);
	pCombFrom->InsertString(-1, strOFF);
	pCombFrom->InsertString(-1, strON);
	pCombTo->InsertString(-1, strOFF);
	pCombTo->InsertString(-1, strON);
	m_nDIChangeFrom = 0;
	m_nDIChangeTo = 1;
	UpdateData(FALSE);
	OnAlmConditionToChange();
}

void CDiscreteAlmEvtPropPage::OnDiscreteControlActionClicked() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETE);	
	ASSERT_VALID(pListCtrl); 

	OnControlActionClicked(pListCtrl, INPUT_DISCRETE, nDStartAddr + m_nChannelAddr); 
}

/////////////////////////////////////////////////////////////////////////////
// CAnalogRWAlmEvtPropPage dialog

IMPLEMENT_DYNCREATE(CAnalogRWAlmEvtPropPage, CPropertyPage)

CAnalogRWAlmEvtPropPage::CAnalogRWAlmEvtPropPage():CPropertyPage(CAnalogRWAlmEvtPropPage::IDD)
{
	//{{AFX_DATA_INIT(CAnalogRWAlmEvtPropPage)
	m_strTagName = _T("");
	m_strDescName = _T("");
	m_strAlmMessage = _T("");
	m_nChannelAddr = 0;
	m_nAlmType = 2;
	//}}AFX_DATA_INIT
	nARWStartAddr = 0;
}

CAnalogRWAlmEvtPropPage::~CAnalogRWAlmEvtPropPage()
{

}

void CAnalogRWAlmEvtPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalogRWAlmEvtPropPage)
	DDX_Text(pDX, IDC_EDT_ARW_TAGNAME, m_strTagName);
	DDX_Text(pDX, IDC_EDT_ARW_DESCNAME, m_strDescName);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strAlmMessage);
	DDX_CBIndex(pDX, IDC_COMB_ARW_CHADDR, m_nChannelAddr);
	DDX_CBIndex(pDX, IDC_COMB_ALMTYPE, m_nAlmType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAnalogRWAlmEvtPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnalogRWAlmEvtPropPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTN_ARW_SETTAGINFO, OnTagInfo)
	ON_CBN_SELCHANGE(IDC_COMB_ARW_CHADDR, OnChannelAddrChange)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAnalogRWAlmEvtPropPage message handlers
BOOL CAnalogRWAlmEvtPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_ARW_CHADDR);
	InitializeChannelAddrComb(HOLDING_REGISTER, pComb, nARWStartAddr, m_strTagName, m_strDescName);
	m_strAlmMessage= _T("PROCESS PARAMETER CHANGE");
	UpdateData(FALSE);

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOGRW);
	AddListCtrlHeader(pListCtrl);	
	UpdateAlarmsListCtrl(HOLDING_REGISTER, nARWStartAddr + m_nChannelAddr, pListCtrl);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAnalogRWAlmEvtPropPage::OnChannelAddrChange() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOGRW);
	
	UpdateData(TRUE);
	unsigned long nChAddr = nARWStartAddr + m_nChannelAddr;
	UpdateTagInfo(HOLDING_REGISTER, nChAddr, m_strTagName, m_strDescName);
	UpdateAlarmsListCtrl(HOLDING_REGISTER, nChAddr, pListCtrl);
	m_strAlmMessage= _T("PROCESS PARAMETER CHANGE");
	UpdateData(FALSE);	
}

void CAnalogRWAlmEvtPropPage::OnAdd() 
{
	short nAlmCode = 0;
	
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOGRW);
	ASSERT(pListCtrl); 

	UpdateData(TRUE);

	unsigned long nAddr = nARWStartAddr + m_nChannelAddr;
	nAlmCode = HR_VALUE_CHANGE;

	if(!IsThisAlarmConfigExists(HOLDING_REGISTER, nAddr, nAlmCode))
	{
		AddAlarmToAlmConfigList(HOLDING_REGISTER, nAddr, m_nAlmType, nAlmCode, m_strAlmMessage);
		AddAlarmToListCtrl(nAlmCode, m_nAlmType, m_strAlmMessage, pListCtrl);	
	}
	else
		pDoc->DAMessageBox(_T("Already Configuraiton Exists !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

void CAnalogRWAlmEvtPropPage::OnDelete() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_ANALOGRW);
	DeleteAlarmFromListCtrl(HOLDING_REGISTER, nARWStartAddr + m_nChannelAddr, pListCtrl);
}

void CAnalogRWAlmEvtPropPage::OnTagInfo() 
{
	DisplayTagInfoDlg(HOLDING_REGISTER, m_nChannelAddr);
}

/////////////////////////////////////////////////////////////////////////////
// CDiscreteRWAlmEvtPropPage dialog
IMPLEMENT_DYNCREATE(CDiscreteRWAlmEvtPropPage, CPropertyPage)

CDiscreteRWAlmEvtPropPage::CDiscreteRWAlmEvtPropPage():CPropertyPage(CDiscreteRWAlmEvtPropPage::IDD)
{
	//{{AFX_DATA_INIT(CDiscreteRWAlmEvtPropPage)
	m_strTagName = _T("");
	m_strDescName = _T("");
	m_strAlmMessage = _T("");
	m_nChannelAddr = 0;
	m_nDOChangeTo = 0;
	m_nDOChangeFrom = 0;
	m_nAlmType = 1;
	//}}AFX_DATA_INIT
	nDRWStartAddr = 0;
}

CDiscreteRWAlmEvtPropPage::~CDiscreteRWAlmEvtPropPage()
{
}

void CDiscreteRWAlmEvtPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiscreteRWAlmEvtPropPage)
	DDX_Text(pDX, IDC_EDT_DRW_TAGNAME, m_strTagName);
	DDX_Text(pDX, IDC_EDT_DRW_DESCNAME, m_strDescName);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strAlmMessage);
	DDX_CBIndex(pDX, IDC_COMB_DRW_CHADDR, m_nChannelAddr);
	DDX_CBIndex(pDX, IDC_COMB_DISCALM_TO, m_nDOChangeTo);
	DDX_CBIndex(pDX, IDC_COMB_DISCALM_FROM, m_nDOChangeFrom);
	DDX_CBIndex(pDX, IDC_COMB_ALMTYPE, m_nAlmType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiscreteRWAlmEvtPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDiscreteRWAlmEvtPropPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTN_DRW_SETTAGINFO, OnTagInfo)
	ON_CBN_SELCHANGE(IDC_COMB_DRW_CHADDR, OnChannelAddrChange)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_COMB_DISCALM_FROM, OnAlmConditionFromChange)
	ON_CBN_SELCHANGE(IDC_COMB_DISCALM_TO, OnAlmConditionToChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiscreteRWAlmEvtPropPage message handlers
BOOL CDiscreteRWAlmEvtPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CComboBox* pComb = (CComboBox*) GetDlgItem(IDC_COMB_DRW_CHADDR);
	InitializeChannelAddrComb(OUTPUT_COIL, pComb, nDRWStartAddr, m_strTagName, m_strDescName);
	UpdateData(FALSE);

	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETERW);
	AddListCtrlHeader(pListCtrl);	
	UpdateAlarmsListCtrl(OUTPUT_COIL, nDRWStartAddr + m_nChannelAddr, pListCtrl);
	UpdateAlmChangeFromToComboBoxes();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDiscreteRWAlmEvtPropPage::OnChannelAddrChange() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETERW);
	UpdateData(TRUE);
	unsigned long nChAddr = nDRWStartAddr + m_nChannelAddr;
	UpdateTagInfo(OUTPUT_COIL, nChAddr, m_strTagName, m_strDescName);
	UpdateAlarmsListCtrl(OUTPUT_COIL, nChAddr, pListCtrl);
	UpdateData(FALSE);	

	UpdateAlmChangeFromToComboBoxes();
}

void CDiscreteRWAlmEvtPropPage::OnAlmConditionFromChange() 
{
	OnAlmConditionToChange(); 
}
void CDiscreteRWAlmEvtPropPage::OnAlmConditionToChange() 
{
	UpdateData(TRUE);
	if(!m_nDOChangeFrom && m_nDOChangeTo)
		m_strAlmMessage = strOFF + _T("_TO_") + strON;
	else if(m_nDOChangeFrom && !m_nDOChangeTo)
		m_strAlmMessage = strON + _T("_TO_") + strOFF;
	else
		m_strAlmMessage = _T("INVALID ALM CONFIG");
	UpdateData(FALSE);
}

void CDiscreteRWAlmEvtPropPage::OnAdd() 
{
	short nAlmCode = 0;
	
	UpdateData(TRUE);
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETERW);
	ASSERT(pListCtrl); 
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	if(m_nDOChangeFrom == m_nDOChangeTo)
	{
		pDoc->DAMessageBox(_T("Invalid Alarm Configuration !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return ;
	}
	
	unsigned long nAddr = nDRWStartAddr + m_nChannelAddr;
	if(!m_nDOChangeFrom)
		nAlmCode = OFF_TO_ON;
	else
		nAlmCode = ON_TO_OFF;
	
	if(!IsThisAlarmConfigExists(OUTPUT_COIL, nAddr, nAlmCode))
	{
		AddAlarmToAlmConfigList(OUTPUT_COIL, nAddr, m_nAlmType, nAlmCode, m_strAlmMessage);
		AddAlarmToListCtrl(nAlmCode, m_nAlmType, m_strAlmMessage, pListCtrl);	
	}
	else
		pDoc->DAMessageBox(_T("Already Configuraiton Exists !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
}

void CDiscreteRWAlmEvtPropPage::OnDelete() 
{
	CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCTRL_ALMEVT_DISCRETERW);
	DeleteAlarmFromListCtrl(OUTPUT_COIL, nDRWStartAddr + m_nChannelAddr, pListCtrl);
}

void CDiscreteRWAlmEvtPropPage::OnTagInfo() 
{
	DisplayTagInfoDlg(OUTPUT_COIL, m_nChannelAddr);
}

void CDiscreteRWAlmEvtPropPage::UpdateAlmChangeFromToComboBoxes()
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CComboBox* pCombFrom = (CComboBox*) GetDlgItem(IDC_COMB_DISCALM_FROM);
	CComboBox* pCombTo = (CComboBox*) GetDlgItem(IDC_COMB_DISCALM_TO);
	
	pCombFrom->ResetContent();
	pCombTo->ResetContent();
	
	long nAddr = nDRWStartAddr + m_nChannelAddr;
	pDoc->GetDiscreteCaption(&pDoc->tagInfoList, OUTPUT_COIL, nAddr, strON, strOFF);
	pCombFrom->InsertString(-1, strOFF);
	pCombFrom->InsertString(-1, strON);
	pCombTo->InsertString(-1, strOFF);
	pCombTo->InsertString(-1, strON);
	m_nDOChangeFrom = 0;
	m_nDOChangeTo = 1;
	UpdateData(FALSE);
	OnAlmConditionToChange();
}

/////////////////////////////////////////////////////////////////////////////
// Now Common Code for all Property Pages. 

void InitializeChannelAddrComb(eChannelType eCh, CComboBox* pComb, long& nStartAddr, CString& strTagName, CString& strDescName)
{
	short nNoChannels;
	CString strCh;

	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	switch(eCh)
	{
	case INPUT_REGISTER:
		nStartAddr = pDoc->nAStartAddress;
		nNoChannels = pDoc->nANoChannels;
		break;
	case INPUT_DISCRETE:
		nStartAddr = pDoc->nDStartAddress;
		nNoChannels = pDoc->nDNoChannels;
		break;
	case HOLDING_REGISTER:
		nStartAddr = pDoc->nARWStartAddress;
		nNoChannels = pDoc->nARWNoChannels;
		break;
	case  OUTPUT_COIL:
		nStartAddr = pDoc->nDRWStartAddress;
		nNoChannels = pDoc->nDRWNoChannels;
		break;
	default:
		nStartAddr = 0;
		nNoChannels = 0;
		break;
	}

	ASSERT(pComb != NULL);
	for(long i = 0; i < nNoChannels; i++)
	{
		strCh.Format(_T(" %d "), nStartAddr + i);
		pComb->InsertString(-1, strCh);
	}
	
	pComb->SetCurSel(0);
	UpdateTagInfo(eCh, nStartAddr, strTagName, strDescName);
}

void UpdateTagInfo(eChannelType eCh, long nChAddr, CString& strTagName, CString& strDescName)
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	strTagName = pDoc->GetTagString(&pDoc->tagInfoList, eCh, nChAddr);
	strDescName = pDoc->GetDescName(&pDoc->tagInfoList, eCh, nChAddr);
}

void DisplayTagInfoDlg(eChannelType eCh, long nChAddr)
{
	CTagSpecDlg  tagDlg;
	tagDlg.m_nChType = eCh;
	tagDlg.m_nChAddress = nChAddr;
	tagDlg.DoModal();
}

void AddListCtrlHeader(CListCtrl* pListCtr)
{
	ASSERT(pListCtr != NULL);
	
	DWORD exStyle = LVS_EX_FULLROWSELECT;
	pListCtr->SetExtendedStyle(exStyle);

	pListCtr->InsertColumn(0, _T("ALARM CODE"), LVCFMT_CENTER, 160, -1);
	pListCtr->InsertColumn(1, _T("TYPE"), LVCFMT_CENTER, 80, -1);
	pListCtr->InsertColumn(2, _T("Message"), LVCFMT_LEFT, 300, -1);
}

BOOL IsThisAlarmConfigExists(eChannelType eCh, unsigned long nAddr, short nAlmCode)
{
	CAlmEvtConfigInfo* pAlmInfo;
	BOOL bExists = FALSE;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	CAlmEvtConfigList* pList = &pDoc->almEvtConfigList;
	
	for(POSITION pos = pList->GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pList->GetAt(pos);
		if(pAlmInfo->nAlmCode == nAlmCode && pAlmInfo->nAddress == nAddr && pAlmInfo->eChType == eCh)
		{
			bExists = TRUE;
			break;
		}
		pList->GetNext(pos);
	}
	return bExists;
}

//Add Alarm to AlmEvtConfig List. 
void AddAlarmToAlmConfigList(eChannelType eCh, long nAddr, short nAlmType, short nAlmCode, CString strAlmMsg)
{
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	CAlmEvtConfigInfo* pAlmInfo = new CAlmEvtConfigInfo(eCh, nAddr, nAlmType, nAlmCode, strAlmMsg);
	pDoc->almEvtConfigList.AddTail(pAlmInfo);
}	

void UpdateAlarmsListCtrl(eChannelType eCh, unsigned long nAddr, CListCtrl* pListCtrl)
{
	CAlmEvtConfigInfo* pAlmInfo;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	ASSERT(pListCtrl != NULL);
	pListCtrl->DeleteAllItems();

	for(POSITION pos = pDoc->almEvtConfigList.GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pDoc->almEvtConfigList.GetAt(pos);
		if(pAlmInfo->eChType == eCh && pAlmInfo->nAddress == nAddr)
			AddAlarmToListCtrl(pAlmInfo->nAlmCode, pAlmInfo->nAlmType, pAlmInfo->strAlmMsg, pListCtrl);
		pDoc->almEvtConfigList.GetNext(pos);
	}
}
	
void AddAlarmToListCtrl(short nAlmCode, short nAlmType, CString strAlmMsg, CListCtrl* pListCtrl)	
{	
	LV_ITEM  lvitem;
	short nActualItem;				//, nPos, nASubItems, nDSubItems;
	CString strBuf;
	LPTSTR  szStrBuf;
	
	ASSERT(pListCtrl != NULL);
	
	szStrBuf = const_cast< LPTSTR >( (LPCTSTR)strAlmCode[nAlmCode]);
	lvitem.mask = LVIF_TEXT; 
	lvitem.iItem = 0;			//Add at top of List Ctrl.
	lvitem.pszText = szStrBuf;
	lvitem.state = LVIS_SELECTED;
	nActualItem = pListCtrl->InsertItem(0, strAlmCode[nAlmCode]); // insert new item			
	
	for (short iSubItem = 0; iSubItem < 2; iSubItem++)
	{
		if(iSubItem == 0)
			strBuf = strAlmType[nAlmType];
		else 
			strBuf = strAlmMsg;
		
		szStrBuf = const_cast< LPTSTR >( (LPCTSTR)strBuf);
		lvitem.mask = LVIF_TEXT; 
		lvitem.iItem = nActualItem;
		lvitem.iSubItem = iSubItem+1;
		lvitem.pszText = szStrBuf;
		lvitem.state = LVIS_SELECTED;
		pListCtrl->SetItem(&lvitem); // modify existing item (the sub-item text)
	}//End of iSubItem for loop. 
}

void DeleteAlarmFromListCtrl(eChannelType eCh, unsigned long nAddr, CListCtrl* pListCtrl)
{
	unsigned short nPos; 
	
	ASSERT(pListCtrl != NULL);
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		pDoc->DAMessageBox(_T("Select Alarm to Delete !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	else
	{
		nPos = pListCtrl->GetNextSelectedItem(pos);
		CString strAlmCode = pListCtrl->GetItemText(nPos, 0);
		short nAlmCode = GetAlmCode(strAlmCode);
		pListCtrl->DeleteItem(nPos);
		
		if(!DeleteAlarmFromList(eCh, nAddr, nAlmCode))
			pDoc->DAMessageBox(_T("Given Alarm not found in the Alarms list !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
	}
}

BOOL DeleteAlarmFromList(eChannelType eCh, unsigned long nAddr, short nAlmCode)
{
	BOOL bAlmDeleted = FALSE;
	CAlmEvtConfigInfo* pAlmInfo;
	
	CDADoc* pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);

	for(POSITION pos = pDoc->almEvtConfigList.GetHeadPosition(); pos != NULL; )
	{
		pAlmInfo = (CAlmEvtConfigInfo*) pDoc->almEvtConfigList.GetAt(pos);
		if( pAlmInfo->nAlmCode == nAlmCode && pAlmInfo->eChType == eCh && pAlmInfo->nAddress == nAddr )
		{
			POSITION pos2 = pos;
			pDoc->almEvtConfigList.GetNext(pos);
			pDoc->almEvtConfigList.RemoveAt(pos2);
			delete pAlmInfo;
			bAlmDeleted = TRUE;
			break;
		}
		pDoc->almEvtConfigList.GetNext(pos);
	}
	return bAlmDeleted;
}

short GetAlmCode(CString strCode)
{
	for(short nAlmCode = MISC_ALM_CODE; nAlmCode < MAX_ALM_CODES; nAlmCode++)
		if(strAlmCode[nAlmCode] == strCode)
			return nAlmCode;		
			
	return MISC_ALM_CODE;
}
	
	

void OnControlActionClicked(CListCtrl* pListCtrl, eChannelType eCh, int nAlmAddress ) 
{
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc);
	
	ASSERT_VALID(pListCtrl); 
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		pDoc->DAMessageBox(_T("Select Alarm to perform Control Action !"), _T(" DA Package !"), MSGBOX_BTN_OKONLY, MSGBOX_BMP_ERROR);
		return;
	}
	
	int nPos = pListCtrl->GetNextSelectedItem(pos);
	CString strAlmCode = pListCtrl->GetItemText(nPos, 0);
	short nAlmCode = GetAlmCode(strAlmCode);
	
	CAlmControlActionDlg dlg; 
	dlg.eCh = eCh;
	dlg.nAlmAddress = nAlmAddress;		
	dlg.nAlmCode = nAlmCode; 
	dlg.DoModal(); 
}




