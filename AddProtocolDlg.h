#if !defined(AFX_ADDPROTOCOLDLG_H__986AEB5E_448D_478F_BCBD_09EC10EFCB9A__INCLUDED_)
#define AFX_ADDPROTOCOLDLG_H__986AEB5E_448D_478F_BCBD_09EC10EFCB9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddProtocolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddProtocolDlg dialog

class CAddProtocolDlg : public CDialog
{
// Construction
public:
	CAddProtocolDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddProtocolDlg)
	enum { IDD = IDD_PROTOCOL_ADD };
	CString	m_strProtocolDesc;
	CString	m_strDllName;
	CString	m_strProtocolName;
	int		m_nComMedium;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddProtocolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddProtocolDlg)
	afx_msg void OnAddProtocol();
	afx_msg void OnProtocolAddClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDPROTOCOLDLG_H__986AEB5E_448D_478F_BCBD_09EC10EFCB9A__INCLUDED_)
