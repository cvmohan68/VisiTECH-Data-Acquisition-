#if !defined(AFX_ALMEVTVIEWDLG_H__2A412C6A_E12F_4487_965F_CAE58F8225A4__INCLUDED_)
#define AFX_ALMEVTVIEWDLG_H__2A412C6A_E12F_4487_965F_CAE58F8225A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmEvtViewDlg.h : header file
//

#include "AlmEvtConfigInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CAlmEvtViewDlg dialog
class CAlmEvtViewDlg : public CDialog
{
// Construction
public:
	CAlmEvtViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlmEvtViewDlg)
	enum { IDD = IDD_ALARM_EVENT};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	BOOL bIsDisplayed;

protected:
	HBRUSH m_hWhiteBrush;
	BOOL  bRTAlmDisplay; 
	CAlmEvtList historicAlmEvtList;
	CString		strHistoricAlmEvtFileName;
	CImageList* pImageList; 


private:
	void DrawHeader(CDC* pDC);
	void AddListCtrlHeader(void);
	void DisplayDate(CDC* pDC, CTime almEvtDate);
	void AddNewAlarmToListCtrl(CAlmEvtInfo* pAlmInfo);
	void AddAlmEvtsToListCtrl(void);
	void DeleteAlarmFromList(int nOffSet); 
	void SaveHistoriclAlmEvtInfo(void);
	void ChangeHistoricBtnCatpion(void);
	void ChangeAlmEvtInfo(int nItemToChange, CTime& ackTime);
	void FreeItemMemory(void);
	void ActivateDeleteBtn(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlmEvtViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangeAckAllBtnStatus(void);
	void InitializeAlmEvtView(void);
	static int CALLBACK AlarmSortingFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); 

protected:
	// Generated message map functions
	//{{AFX_MSG(CAlmEvtViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnClose();
	afx_msg void OnHistoricAlmEvt();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDelete();
	afx_msg void OnClose();
	afx_msg void OnNewAlarm();
	afx_msg void OnAlarmRecovered(int nPos, CTime* pAlmOutTime);
	afx_msg void OnAlarmEvtPrint();
	afx_msg void OnDestroy();
	afx_msg void OnAlarmEventCtrlDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAlmEvtCtrlColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAlmEvtCtrlClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAlmEvtDisplayInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAcknowledgeAllAlarms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMEVTVIEWDLG_H__2A412C6A_E12F_4487_965F_CAE58F8225A4__INCLUDED_)
