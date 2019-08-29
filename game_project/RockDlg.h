#pragma once

#include "ClientSocket.h"
// CRockDlg dialog

class CRockDlg : public CDialog
{
	DECLARE_DYNAMIC(CRockDlg)

public:
	CRockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRockDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CClientSocket *m_clientSocket;
	afx_msg void OnClickedBtnRock();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int m_choice;
	void OnImagePrint(int choice,int x,int y);
	afx_msg void OnClickedBtnScissors();
	afx_msg void OnClickedBtnPaper();

};
