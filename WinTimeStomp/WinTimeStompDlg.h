// WinTimeStompDlg.h : header file
//

#pragma once
#include "afxdtctl.h"


// CWinTimeStompDlg dialog
class CWinTimeStompDlg : public CDialog
{
// Construction
public:
	CWinTimeStompDlg(CWnd* pParent = NULL);	// standard constructor

	FILETIME ConvertTime( LARGE_INTEGER );
	LARGE_INTEGER ConvertTime( CDateTimeCtrl& );
	LARGE_INTEGER ConvertTime( FILETIME );
	BOOL CheckForFileName( int, TCHAR *, int );

// Dialog Data
	enum { IDD = IDD_WINTIMESTOMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedBrowse();
public:
	afx_msg void OnBnClickedGetinfo();
public:
	CDateTimeCtrl m_DateCreated;
public:
	CDateTimeCtrl m_DateAccessed;
public:
	CDateTimeCtrl m_DateChanged;
public:
	afx_msg void OnBnClickedUpdateinfo();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
