#if !defined(AFX_ADDNEWUSERDLG_H__C8464A4D_F692_49B8_9EE6_161B65EB474A__INCLUDED_)
#define AFX_ADDNEWUSERDLG_H__C8464A4D_F692_49B8_9EE6_161B65EB474A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddNewUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddNewUserDlg dialog

class CAddNewUserDlg : public CDialog
{
// Construction
public:
	CAddNewUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddNewUserDlg)
	enum { IDD = IDD_ADD_USER };
	CString	m_strPwd;
	CString	m_strRPwd;
	CString	m_strUserName;
	BOOL	m_bIsAdministrator;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddNewUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL CheckIsUserAlreadyExists(CString strUserName);

	// Generated message map functions
	//{{AFX_MSG(CAddNewUserDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAddNewUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDNEWUSERDLG_H__C8464A4D_F692_49B8_9EE6_161B65EB474A__INCLUDED_)
