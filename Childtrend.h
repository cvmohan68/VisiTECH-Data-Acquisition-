#if !defined(AFX_CHILDTREND_H__27BDFB34_7AF2_43F5_8C6A_AFFCAAA862D4__INCLUDED_)
#define AFX_CHILDTREND_H__27BDFB34_7AF2_43F5_8C6A_AFFCAAA862D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildTrend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildTrend dialog
//#include "TrendView.h"
#include "StorageData.h"
#include "Trend.h"

class CChildTrend : public CDialog
{
	DECLARE_DYNCREATE(CChildTrend)
// Construction
public:
	CChildTrend(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CChildTrend();				//Dtor. 

// Dialog Data
	//{{AFX_DATA(CChildTrend)
	enum { IDD = IDD_CHILDTREND };
	int 	m_nXAxisChannel;
	int 	m_nYAxisChannel;
	//}}AFX_DATA

public:
	short	nChildTrendID; 

	CTrendPointList  rtPointList;
	CTrendPointList	 hPointList;
	
	CTrendPointList	 rtXAxisTPList; 
	CTrendPointList	 hXAxisTPList; 
	
	CACInfoList		 hACList;
	CTrend*			 pTrend;	

	CDataList		 historicDataList;
	CTagSpecInfoList historicTagInfoList;
	CChannelInfoList historicChInfoList;


public:
	BOOL	bRTDisplay;

protected:
	HBRUSH	m_hWhiteBrush;
	BOOL	bRestoreTrend;
	long	nChannelStartAddr;
	short   nNoChannels;
	short	nChannelPos;
	short	nHistoricChannelPos;	//Keep hold the channle pos when switching between historic and real time trends. 
	short   nRTChannelPos; 
	short	nNoScreens, nScreenPos;

	BOOL	bMultiAxisTrend; 
	BOOL	bRTMultiAxisTrend; 
	BOOL	bHistoricMultiAxisTrend; 
	short	nXChannelPos;
	short	nHistoricXChannelPos;	//Keep hold the channle pos when switching between historic and real time trends. 
	short   nRTXChannelPos; 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildTrend)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDADoc* GetDocument(void);
	void SetTrendTitle(void);
	void UpdateScreenButtons(void);
	void BuildAddressInfo(void);
	void SetChannelInfo(void);
	BOOL DisplayTrend(long nCh);
	void DisplayTrendScreen(short nScreen);
	void RestoreTrend();
	void ActivateHistoricDataInfoBtn(void);
public:
	void InitializeTrendVaraibles(void);
	void DisplayCurrentTrend(void);
	void AdjustXAxisScaleValues(short nXPresentValue);
	
	// Generated message map functions
	//{{AFX_MSG(CChildTrend)
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnDisplayTrend();
	afx_msg void OnHistoricTrend();
	afx_msg void OnGraphNextClicked();
	afx_msg void OnGraphPrevClicked();
	afx_msg void OnYChannelChannge();
	afx_msg void OnHistoricDataInfo();
	afx_msg void OnXChannelChange();
	//}}AFX_MSG
	afx_msg void OnNewData();
	afx_msg void OnNewScreen();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDTREND_H__27BDFB34_7AF2_43F5_8C6A_AFFCAAA862D4__INCLUDED_)
