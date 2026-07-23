#if !defined(AFX_CHANGEPWD_H__26DA4E4D_BA90_4182_9A62_2753954DBC54__INCLUDED_)
#define AFX_CHANGEPWD_H__26DA4E4D_BA90_4182_9A62_2753954DBC54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePwd.h : header file
//

#include "UserInfo.h" 
/////////////////////////////////////////////////////////////////////////////
// CChangePwd dialog

class CChangePwd : public CDialog
{

public:

// Construction
public:
	CChangePwd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePwd)
	enum { IDD = IDD_CHANGE_PWD };
	CString	m_strOldPwd;
	CString	m_strNPwd;
	CString	m_strRNPwd;
	CString	m_strUserName;
	//}}AFX_DATA

public:
	CUserInfo* pCurrentUser; 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePwd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePwd)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnUserNameChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPWD_H__26DA4E4D_BA90_4182_9A62_2753954DBC54__INCLUDED_)
