// DA.h : main header file for the DA application
//

#if !defined(AFX_DA_H__0C177F64_263E_4CB6_B047_0F6E9670C06B__INCLUDED_)
#define AFX_DA_H__0C177F64_263E_4CB6_B047_0F6E9670C06B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDAApp:
// See DA.cpp for the implementation of this class
//

class CDAApp : public CWinApp
{
public:
	CDAApp();
	BOOL FirstInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDAApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDAApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DA_H__0C177F64_263E_4CB6_B047_0F6E9670C06B__INCLUDED_)
