#if !defined(AFX_CHANNELCONFIGSHEET_H__B6643C39_2F81_43FB_9C97_4671B86CF076__INCLUDED_)
#define AFX_CHANNELCONFIGSHEET_H__B6643C39_2F81_43FB_9C97_4671B86CF076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelConfigSheet.h : header file
//

#include "ChannelConfig.h"
#include "SPAlarmConfig.h"
#include "ChannelConfig3.h"
#include "AlmEvtPropPage.h"
/////////////////////////////////////////////////////////////////////////////
// CChannelConfigSheet

class CChannelConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CChannelConfigSheet)

// Construction
public:
	CChannelConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CChannelConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CChannelConfig1  m_chConfig1;
	CSPAlarmConfig	 m_spAlarmConfig;
	CChannelConfig3  m_chConfig3;
	CAnalogAlmEvtPropPage m_almEvtPropPage;
	
protected:
	RECT m_rctPage;
	CFont m_fntPage;

private:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelConfigSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChannelConfigSheet();

// Implementation
protected:
	virtual void BuildPropPageArray ();

	//{{AFX_MSG(CChannelConfigSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELCONFIGSHEET_H__B6643C39_2F81_43FB_9C97_4671B86CF076__INCLUDED_)
