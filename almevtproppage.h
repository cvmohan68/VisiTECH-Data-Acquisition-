#if !defined(AFX_ALMEVTPROPPAGE_H__F895D34F_1563_472E_9987_0BC642FB7292__INCLUDED_)
#define AFX_ALMEVTPROPPAGE_H__F895D34F_1563_472E_9987_0BC642FB7292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmEvtPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnalogAlmEvtPropPage dialog

class CAnalogAlmEvtPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAnalogAlmEvtPropPage)

// Construction
public:
	CAnalogAlmEvtPropPage();   // standard constructor
	~CAnalogAlmEvtPropPage();   // standard Destructor
// Dialog Data
	//{{AFX_DATA(CAnalogAlmEvtPropPage)
	enum { IDD = IDD_ALMEVT_ANALOG };
	int 	m_nChannelAddr;
	CString	m_strTagName;
	CString	m_strDescName;
	CString	m_strAlmMessage;
	int 	m_nAlmType;
	int 	m_nAIChangeFrom;
	int 	m_nAIChangeTo;
	//}}AFX_DATA
	CString strCaption; 
protected:
	long nAStartAddr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalogAlmEvtPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateAlmChangeToComboBox(void);
	void InitializeFromComboBoxAlmCaptions(void);

	// Generated message map functions
	//{{AFX_MSG(CAnalogAlmEvtPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnTagInfo();
	afx_msg void OnChannelAddrChange();
	afx_msg void OnAlmConditionFromChange();
	afx_msg void OnDelete();
	afx_msg void OnAnalogControlActionClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDiscreteAlmEvtPropPage dialog

class CDiscreteAlmEvtPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDiscreteAlmEvtPropPage)
// Construction
public:
	CDiscreteAlmEvtPropPage();   // standard constructor
	~CDiscreteAlmEvtPropPage();

// Dialog Data
	//{{AFX_DATA(CDiscreteAlmEvtPropPage)
	enum { IDD = IDD_ALMEVT_DISCRETE };
	CString	m_strTagName;
	CString	m_strDescName;
	CString	m_strAlmMessage;
	int 	m_nChannelAddr;
	int 	m_nDIChangeTo;
	int 	m_nDIChangeFrom;
	int 	m_nAlmType;
	//}}AFX_DATA

protected:
	long nDStartAddr;
	CString strON, strOFF; 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiscreteAlmEvtPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateAlmChangeFromToComboBoxes(void);

	// Generated message map functions
	//{{AFX_MSG(CDiscreteAlmEvtPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnTagInfo();
	afx_msg void OnChannelAddrChange();
	afx_msg void OnDelete();
	afx_msg void OnAlmConditionToChange();
	afx_msg void OnAlmConditionFromChange();
	afx_msg void OnDiscreteControlActionClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CAnalogRWAlmEvtPropPage dialog

class CAnalogRWAlmEvtPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAnalogRWAlmEvtPropPage)
// Construction
public:
	CAnalogRWAlmEvtPropPage();   // standard constructor
	~CAnalogRWAlmEvtPropPage();
// Dialog Data
	//{{AFX_DATA(CAnalogRWAlmEvtPropPage)
	enum { IDD = IDD_ALMEVT_ANALOGRW };
	CString	m_strTagName;
	CString	m_strDescName;
	CString	m_strAlmMessage;
	int 	m_nChannelAddr;
	int 	m_nAlmType;
	//}}AFX_DATA

protected:
	long nARWStartAddr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalogRWAlmEvtPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnalogRWAlmEvtPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnTagInfo();
	afx_msg void OnChannelAddrChange();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDiscreteRWAlmEvtPropPage dialog

class CDiscreteRWAlmEvtPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDiscreteRWAlmEvtPropPage)

// Construction
public:
	CDiscreteRWAlmEvtPropPage();   // standard constructor
	~CDiscreteRWAlmEvtPropPage();

// Dialog Data
	//{{AFX_DATA(CDiscreteRWAlmEvtPropPage)
	enum { IDD = IDD_ALMEVT_DISCRETERW };
	CString	m_strTagName;
	CString	m_strDescName;
	CString	m_strAlmMessage;
	int 	m_nChannelAddr;
	int 	m_nDOChangeTo;
	int 	m_nDOChangeFrom;
	int 	m_nAlmType;
	//}}AFX_DATA

protected:
	long nDRWStartAddr;
	CString strON, strOFF;

// Overrides
protected:
	void UpdateAlmChangeFromToComboBoxes(void);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiscreteRWAlmEvtPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDiscreteRWAlmEvtPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnTagInfo();
	afx_msg void OnChannelAddrChange();
	afx_msg void OnDelete();
	afx_msg void OnAlmConditionFromChange();
	afx_msg void OnAlmConditionToChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMEVTPROPPAGE_H__F895D34F_1563_472E_9987_0BC642FB7292__INCLUDED_)
