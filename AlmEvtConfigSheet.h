#if !defined(AFX_ALMEVTCONFIGSHEET_H__276F8DB0_7437_4DE1_9CD7_D4C756AA2B95__INCLUDED_)
#define AFX_ALMEVTCONFIGSHEET_H__276F8DB0_7437_4DE1_9CD7_D4C756AA2B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmEvtConfigSheet.h : header file
//

#include "AlmEvtPropPage.h"
/////////////////////////////////////////////////////////////////////////////
// CAlmEvtConfigSheet

class CAlmEvtConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAlmEvtConfigSheet)

// Construction
public:
	CAlmEvtConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAlmEvtConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CAnalogAlmEvtPropPage		m_analogPropPage;
	CDiscreteAlmEvtPropPage		m_discretePropPage;
	CAnalogRWAlmEvtPropPage		m_analogRWPropPage;
	CDiscreteRWAlmEvtPropPage	m_discreteRWPropPage;

protected:
	RECT m_rctPage;
	CFont m_fntPage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlmEvtConfigSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlmEvtConfigSheet();

// Implementation
protected:
	virtual void BuildPropPageArray ();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlmEvtConfigSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMEVTCONFIGSHEET_H__276F8DB0_7437_4DE1_9CD7_D4C756AA2B95__INCLUDED_)
