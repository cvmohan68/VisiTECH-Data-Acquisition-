#if !defined(AFX_CHANNELADDRESSDLG_H__59E2E50D_8E17_4715_A733_B0296E283BEE__INCLUDED_)
#define AFX_CHANNELADDRESSDLG_H__59E2E50D_8E17_4715_A733_B0296E283BEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelAddressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelAddressDlg dialog

class CChannelAddressDlg : public CDialog
{
// Construction
public:
	CChannelAddressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChannelAddressDlg)
	enum { IDD = IDD_ADDRESS };
	int		m_nANoChannels;
	int		m_nAStartAddress;
	int		m_nDNoChannels;
	int		m_nDStartAddress;
	int		m_nPV;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelAddressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int		m_nARONoChannels;
	int		m_nAROStartAddress;
	int		m_nDRONoChannels;
	int		m_nDROStartAddress;
	
	int		m_nARWNoChannels;
	int		m_nARWStartAddress;
	int		m_nDRWNoChannels;
	int		m_nDRWStartAddress;

// Implementation
protected:
	void UpdateDlgParameters(void);
	// Generated message map functions
	//{{AFX_MSG(CChannelAddressDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSet();
	virtual BOOL OnInitDialog();
	afx_msg void OnPVClicked();
	afx_msg void OnParametersClicked();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELADDRESSDLG_H__59E2E50D_8E17_4715_A733_B0296E283BEE__INCLUDED_)
