#pragma once
#include "ClientSocket.h"
#include "afxwin.h"
#include "RockDlg.h"
// COthelloDlg dialog

class COthelloDlg : public CDialog
{
	DECLARE_DYNAMIC(COthelloDlg)

public:
	COthelloDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COthelloDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OTHELLODLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CClientSocket *m_clientSocket;
	UINT m_1_count;
	UINT m_2_count;
	CEdit m_edit_msg;
	CListBox m_list_msg;
	CBitmap m_bitmap[2];
	bool m_turn;
	int m_map[8][8] = {
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,2,0,0,0 },
		{ 0,0,0,2,1,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 }
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnPass();
	afx_msg void OnClickedBtnSend();
	void DrawLine();
	int DrawStone(int x, int y);
	int* judgeStoneX(int x, int y);
	int* judgeStoneY(int x, int y);
	int** judgeStoneXY(int x, int y);
	void ChangeStoneX(int x1, int x2, int y);
	void ChangeStoneY(int x, int y1, int y2);
	void ChangeStoneXY(int x1, int x2, int y1, int y2);
	void SetCount();
protected:
	afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);
public:
	int m_game_count;
	afx_msg void OnClose();
	CString m_player1;
	CString m_player2;
	afx_msg void OnClickedBtnReady();
	bool m_ready;
protected:
	afx_msg LRESULT OnClientRecvReady(WPARAM wParam, LPARAM lParam);
public:
	int m_stone_color;
protected:
	afx_msg LRESULT OnClientMapRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientTurn(WPARAM wParam, LPARAM lParam);
	void mapSend();
	CRockDlg *r_dlg;

public:
protected:
	afx_msg LRESULT OnClientKingbeom(WPARAM wParam, LPARAM lParam);
};
