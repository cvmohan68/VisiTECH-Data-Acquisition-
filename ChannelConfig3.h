#if !defined(AFX_CHANNELCONFIG3_H__0D04032F_6F9A_4747_97A0_95574E507274__INCLUDED_)
#define AFX_CHANNELCONFIG3_H__0D04032F_6F9A_4747_97A0_95574E507274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelConfig3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig3 dialog

class CChannelConfig3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CChannelConfig3)

// Construction
public:
	CChannelConfig3();
	~CChannelConfig3();

// Dialog Data
	//{{AFX_DATA(CChannelConfig3)
	enum { IDD = IDD_CHANNEL_CONFIG3 };
	int		m_nAddr;
	CString	m_strDescName;
	CString	m_strTagName;
	int		m_nSignalConditioning;
	int		m_nRawMin;
	int		m_nRawMax;
	int		m_nProcessedMin;
	int 	m_nProcessedMax;
	BOOL	m_bEnableCustomScaling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CChannelConfig3)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnCustomScalingChange(void);

protected:
	// Generated message map functions
	//{{AFX_MSG(CChannelConfig3)
	afx_msg void OnCustomScalingClikced();
	afx_msg void OnSignalConditioningClicked();
	afx_msg void OnRawValueChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELCONFIG3_H__0D04032F_6F9A_4747_97A0_95574E507274__INCLUDED_)
