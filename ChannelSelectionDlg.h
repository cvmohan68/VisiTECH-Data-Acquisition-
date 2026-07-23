#if !defined(AFX_CHANNELSELECTIONDLG_H__AB28284F_FFB5_44AC_A298_B406DC81E3A5__INCLUDED_)
#define AFX_CHANNELSELECTIONDLG_H__AB28284F_FFB5_44AC_A298_B406DC81E3A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelSelectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelSelectionDlg dialog

class CChannelSelectionDlg : public CDialog
{

public:
	
	// Construction
public:
	CChannelSelectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChannelSelectionDlg)
	enum { IDD = IDD_CHANNEL_SELECT };
	int		m_nChannelType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CleanLists();
	void SetSelectedList(eChannelType eCh, int* pItems, short nNoSelectedItems);
	short GetChannelIDFromTagName(BOOL bRTDisplay, eChannelType eCh, CString strTagName);

	// Generated message map functions
	//{{AFX_MSG(CChannelSelectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelect();
	afx_msg void OnRemove();
	virtual void OnOK();
	afx_msg void OnSet();
	afx_msg void OnAnalogSelected();
	afx_msg void OnDiscreteSelected();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELSELECTIONDLG_H__AB28284F_FFB5_44AC_A298_B406DC81E3A5__INCLUDED_)
