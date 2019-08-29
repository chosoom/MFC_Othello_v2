
// game_projectDlg.h : ��� ����
//

#pragma once
#include "ClientSocket.h"
#include "afxwin.h"
#include "OthelloDlg.h"
// Cgame_projectDlg ��ȭ ����
class Cgame_projectDlg : public CDialogEx
{
// �����Դϴ�.
public:
	Cgame_projectDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	COthelloDlg *m_othelloDlg;
	CListBox m_list_msg;
	CEdit m_edit_send;
	CString m_serverIP;

public:
	afx_msg void OnClickedBtnConnect();
	CString m_edit_name;

};
