#if !defined(AFX_ALMCONTROLACTIONDLG_H__589BD066_3628_4DD5_861E_E0D3EB102B22__INCLUDED_)
#define AFX_ALMCONTROLACTIONDLG_H__589BD066_3628_4DD5_861E_E0D3EB102B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmControlActionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlmControlActionDlg dialog

class CAlmControlActionDlg : public CDialog
{
public:
	eChannelType eCh;
	unsigned long nAlmAddress; 
	short nAlmCode; 
	
	
// Construction
public:
	CAlmControlActionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlmControlActionDlg)
	enum { IDD = IDD_ALM_CONTROLACTION };
	int		m_nActionType;					//0 = Analog Action 1 = Discrete Action
	int		m_nValue;
	int		m_bDValue;
	int		m_nAddress;
	//}}AFX_DATA


private:
	void InitializeAddressComboBox(int nActionType);
	void UpdateExistingControlActionCfg(void);
	void SetAlarmActionConfiguration(void);
	void OnCAAddressChange(BOOL bInvalidate); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlmControlActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlmControlActionDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSet();
	afx_msg void OnAnalogActionSelected();
	afx_msg void OnDiscreteActionSelected();
	afx_msg void OnControlActionAddressChange();
	afx_msg void OnControlActionDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMCONTROLACTIONDLG_H__589BD066_3628_4DD5_861E_E0D3EB102B22__INCLUDED_)
