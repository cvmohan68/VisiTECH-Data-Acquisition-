#if !defined(AFX_CHANNELCONFIG_H__F5F720D6_2DA0_4203_AFA5_3E4C4A869826__INCLUDED_)
#define AFX_CHANNELCONFIG_H__F5F720D6_2DA0_4203_AFA5_3E4C4A869826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelConfig1 dialog

class CChannelConfig1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CChannelConfig1)

// Construction
public:
	CChannelConfig1();
	~CChannelConfig1();

// Dialog Data
	//{{AFX_DATA(CChannelConfig1)
	enum { IDD = IDD_CHANNEL_CONFIG1 };
	int 	m_nChannelAddr;
	CString	m_strTagName;
	CString	m_strDescName;
	int		m_nTimeScale;
	int		m_nYMax;
	int		m_nYMin;
	BOOL	m_bYAutoScale;
	BOOL	m_bDisplaySP;
	BOOL	m_bDisplayHighAlarm;
	BOOL	m_bDisplayLowAlarm;
	BOOL	m_bDisplayHiHiAlarm;
	BOOL	m_bDisplayLoLoAlarm;
	//}}AFX_DATA
	long	nCurrentChAddr;

private:
	void OnChannelAddressChange(void); 

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CChannelConfig1)
	public:
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void CheckSpAlarmsValidity();
	void MarkSpAlarmsDirty();
	short GetTimeScalePos(short nMins);
	short GetTimeScaleValue(short nPos);

protected:
	// Generated message map functions
	//{{AFX_MSG(CChannelConfig1)
	virtual BOOL OnInitDialog();
	afx_msg void OnYScaleTypeChanged();
	afx_msg void OnChannelAddrChange();
	afx_msg void OnXScaleResolutionChanged();
	afx_msg void OnYMaxChanged();
	afx_msg void OnYMinChanged();
	afx_msg void OnSetTagInfo();
	afx_msg void OnDisplaySetPoint();
	afx_msg void OnDisplayLowAlarm();
	afx_msg void OnDisplayHighAlarm();
	afx_msg void OnDisplayHiHiAlarm();
	afx_msg void OnDisplayLoLoAlarm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELCONFIG_H__F5F720D6_2DA0_4203_AFA5_3E4C4A869826__INCLUDED_)
