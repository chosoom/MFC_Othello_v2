// OthelloDlg.cpp : implementation file
//

#include "stdafx.h"
#include "game_project.h"
#include "OthelloDlg.h"
#include "afxdialogex.h"

#include "game_projectDlg.h"
#include "RockDlg.h"
#define PORT 9999

// COthelloDlg dialog

IMPLEMENT_DYNAMIC(COthelloDlg, CDialog)

COthelloDlg::COthelloDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OTHELLODLG, pParent)
	, m_1_count(2)
	, m_2_count(2)
	, m_turn(false)
	, m_game_count(0)
	, m_player1(_T(""))
	, m_ready(false)
	, m_stone_color(0)
	, m_player2(_T(""))
{
}

COthelloDlg::~COthelloDlg()
{
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_COUNT1, m_1_count);
	DDX_Text(pDX, IDC_STATIC_COUNT2, m_2_count);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edit_msg);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
	DDX_Text(pDX, IDC_PLAYER1, m_player1);
	DDX_Text(pDX, IDC_PLAYER2, m_player2);
}


BEGIN_MESSAGE_MAP(COthelloDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PASS, &COthelloDlg::OnClickedBtnPass)
	ON_BN_CLICKED(IDC_BTN_SEND, &COthelloDlg::OnClickedBtnSend)
	ON_MESSAGE(WM_CLIENT_RECV, &COthelloDlg::OnClientRecv)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_READY, &COthelloDlg::OnClickedBtnReady)
	ON_MESSAGE(WM_CLIENT_RECV_READY, &COthelloDlg::OnClientRecvReady)
	ON_MESSAGE(WM_CLIENT_MAP_RECV, &COthelloDlg::OnClientMapRecv)
	ON_MESSAGE(WM_CLIENT_TURN, &COthelloDlg::OnClientTurn)
	ON_MESSAGE(WM_CLIENT_KINGBEOM, &COthelloDlg::OnClientKingbeom)
END_MESSAGE_MAP()


// COthelloDlg message handlers


int COthelloDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	Cgame_projectDlg *p_dlg = (Cgame_projectDlg*)GetParent();

	m_clientSocket = new CClientSocket;
	m_clientSocket->SetWnd(this->m_hWnd);
	m_clientSocket->Create();
	if (m_clientSocket->Connect(p_dlg->m_serverIP, PORT) == FALSE) {
		AfxMessageBox(_T("ERROR: Failed to connect Server"));
		PostQuitMessage(0);
	}	

	m_player1 = p_dlg->m_edit_name;

	m_bitmap[0].LoadBitmap(IDB_RED);
	m_bitmap[1].LoadBitmap(IDB_YELLOW);
	return 0;
}


void COthelloDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	if (point.x > 420 || point.x < 10 || point.y > 420 || point.y < 10) {
		return;
	}

	if (!m_ready) {
		AfxMessageBox(_T("시작되야 시작합니다."));
		return;
	}
	if (!m_turn) {
		AfxMessageBox(_T("차례에 둘 수 있습니다."));
		return;
	}
	int m_x = (point.x - 20) / 50, m_y = (point.y - 20) / 50;
	if (m_map[m_y][m_x] != 0) {
		AfxMessageBox(_T("이미 둔 곳 입니다."));
		return;
	}

	int* pointX, *pointY, **pointXY;
	pointX = judgeStoneX(m_x, m_y);
	pointY = judgeStoneY(m_x, m_y);
	pointXY = judgeStoneXY(m_x, m_y);

	if ((pointX[0] == -1) & (pointX[1] == -1) & (pointY[0] == -1) & (pointY[1] == -1) &
		(pointXY[0][0] == -1) & (pointXY[1][0] == -1) & (pointXY[2][0] == -1) & (pointXY[3][0] == -1)) {
		AfxMessageBox(_T("잘못된 선택입니다."), MB_ICONWARNING);
	}
	else {
		ChangeStoneX(m_x, pointX[0], m_y);
		ChangeStoneX(m_x, pointX[1], m_y);
		ChangeStoneY(m_x, m_y, pointY[0]);
		ChangeStoneY(m_x, m_y, pointY[1]);
		ChangeStoneXY(m_x, pointXY[0][0], m_y, pointXY[0][1]);
		ChangeStoneXY(m_x, pointXY[1][0], m_y, pointXY[1][1]);
		ChangeStoneXY(m_x, pointXY[2][0], m_y, pointXY[2][1]);
		ChangeStoneXY(m_x, pointXY[3][0], m_y, pointXY[3][1]);
		m_map[m_y][m_x] = m_stone_color + 1;
		m_game_count++;
		if (m_game_count == 60) {
			CString str;
			str.Format(_T("player1: %d player2: %d \r\n winner: %s"), m_1_count, m_2_count, (m_1_count > m_2_count ? "player1" : "player2"));
			AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		}
		mapSend();
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void COthelloDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages

	DrawLine();
	CBitmap *pOldBitmap;
	CDC   MemDC, *pDC;

	pDC = this->GetDC();
	MemDC.CreateCompatibleDC(pDC);

	pOldBitmap = MemDC.SelectObject(&m_bitmap[0]);
	pDC->BitBlt(171, 171, 48, 48, &MemDC, 0, 0, SRCCOPY);
	pDC->BitBlt(221, 221, 48, 48, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);

	pOldBitmap = MemDC.SelectObject(&m_bitmap[1]);
	pDC->BitBlt(221, 171, 48, 48, &MemDC, 0, 0, SRCCOPY);
	pDC->BitBlt(171, 221, 48, 48, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);

}


void COthelloDlg::OnClickedBtnPass()
{
	// TODO: Add your control notification handler code here
	if (!m_turn) {
		AfxMessageBox(_T("차례에 넘길 수 있습니다."));
		return;
	}
	turnRecvMessage *msg = new turnRecvMessage;
	msg->id = 7;
	msg->size = sizeof(turnMessage);
	msg->data.turn = true;
	m_clientSocket->Send((char*)msg, sizeof(turnRecvMessage) * 2);
}


void COthelloDlg::OnClickedBtnSend()
{
	// TODO: Add your control notification handler code here

	CString str;
	UpdateData(TRUE);
	m_edit_msg.GetWindowTextW(str);

	msgMessage *msg = new msgMessage;
	msg->id = 2;
	msg->size = sizeof(msgRecvMessage);
	_tcscpy_s(msg->data.str,str);
	_tcscpy_s(msg->data.name, m_player1);
	m_clientSocket->Send((char*)msg, sizeof(msgMessage) * 2);

	m_edit_msg.SetWindowTextW(_T(""));
	UpdateData(FALSE);

	delete msg;
}


afx_msg LRESULT COthelloDlg::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpData = (LPCTSTR)lParam;

	m_list_msg.InsertString(-1, lpData);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}

void COthelloDlg::DrawLine()
{
	CClientDC pDC(this);
	CRect r;
	GetClientRect(r);
	CPen myPen, *oldPen;
	myPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = pDC.SelectObject(&myPen);

	for (int i = 0; i < 9; i++) {
		pDC.MoveTo(20, 50 * i + 20);
		pDC.LineTo(20 + 50 * 8, 50 * i + 20);
	}
	for (int i = 0; i < 9; i++) {
		pDC.MoveTo(50 * i + 20, 20);
		pDC.LineTo(50 * i + 20, 20 + 50 * 8);
	}
}

int COthelloDlg::DrawStone(int x, int y)
{

	CBitmap *pOldBitmap;
	CDC   MemDC, *pDC;

	pDC = this->GetDC();
	int m_x = (x - 20) / 50, m_y = (y - 20) / 50;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(&m_bitmap[m_map[m_x][m_y]-1]);
	//m_map[m_y][m_x] = m_stone_color + 1;

	pDC->BitBlt(y, x, 48, 48, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

	return 0;
}

int* COthelloDlg::judgeStoneX(int x, int y) {
	int t, i;
	int* pointX = new int[2]{ -1,-1 };
	if (m_stone_color==0) t = 1;
	else t = 2;

	for (i = x - 1; i > 0; i--) {
		if (m_map[y][i] == 0) break;
		if (m_map[y][i] == t) {
			if (i == x - 1) break;
			pointX[0] = i;
			break;
		}
	}

	for (i = x + 1; i < 8; i++) {
		if (m_map[y][i] == 0)break;
		if (m_map[y][i] == t) {
			if (i == x + 1) break;
			pointX[1] = i;
			break;
		}
	}
	return pointX;
}

int* COthelloDlg::judgeStoneY(int x, int y) {
	int t, i;
	int* pointY = new int[2]{ -1,-1 };
	if (m_stone_color == 0) t = 1;
	else t = 2;

	for (i = y - 1; i > 0; i--) {
		if (m_map[i][x] == 0) break;
		if (m_map[i][x] == t) {
			if (i == y - 1) break;
			pointY[0] = i;
			break;
		}
	}

	for (i = y + 1; i < 8; i++) {
		if (m_map[i][x] == 0) break;
		if (m_map[i][x] == t) {
			if (i == y + 1) break;
			pointY[1] = i;
			break;
		}
	}
	return pointY;
}

int** COthelloDlg::judgeStoneXY(int x, int y) {
	int **pointXY = new int*[4];
	for (int a = 0; a < 4; a++) {
		pointXY[a] = new int[2]{ -1,-1 };
	}
	int t, i_x, i_y;
	if (m_stone_color==0) t = 1;
	else t = 2;
	i_x = x - 1;
	i_y = y - 1;
	while (i_x>0 && i_y>0) {
		if (m_map[i_y][i_x] == 0)break;
		if (m_map[i_y][i_x] == t) {
			if ((i_x == x - 1) && (i_y = y - 1)) break;
			pointXY[0][0] = i_x;
			pointXY[0][1] = i_y;
			break;
		}
		i_x--; i_y--;
	}
	i_x = x - 1;
	i_y = y + 1;
	while (i_x>0 && i_y<8) {
		if (m_map[i_y][i_x] == 0)break;
		if (m_map[i_y][i_x] == t) {
			if ((i_x == x - 1) && (i_y = y + 1)) break;
			pointXY[1][0] = i_x;
			pointXY[1][1] = i_y;
			break;
		}
		i_x--; i_y++;
	}
	i_x = x + 1;
	i_y = y - 1;
	while (i_x<8 && i_y>0) {
		if (m_map[i_y][i_x] == 0)break;
		if (m_map[i_y][i_x] == t) {
			if ((i_x == x + 1) && (i_y = y - 1)) break;
			pointXY[2][0] = i_x;
			pointXY[2][1] = i_y;
			break;
		}
		i_x++; i_y--;
	}
	i_x = x + 1;
	i_y = y + 1;
	while (i_x<8 && i_y<8) {
		if (m_map[i_y][i_x] == 0)break;
		if (m_map[i_y][i_x] == t) {
			if ((i_x == x + 1) && (i_y = y + 1)) break;
			pointXY[3][0] = i_x;
			pointXY[3][1] = i_y;
			break;
		}
		i_x++; i_y++;
	}

	return pointXY;
}

void COthelloDlg::ChangeStoneX(int x1, int x2, int y) {
	if (x2 == -1) return;
	for (int i = (x1 < x2 ? x1 + 1 : x2 + 1); i <= (x1 > x2 ? x1 : x2); i++) {
		//DrawStone(i * 50 + 20, y * 50 + 21);
		m_map[y][i] = m_stone_color + 1;
	}
}

void COthelloDlg::ChangeStoneY(int x, int y1, int y2) {
	if (y2 == -1) return;
	for (int i = (y1 < y2 ? y1 + 1 : y2 + 1); i <= (y1 > y2 ? y1 : y2); i++) {
		//DrawStone(x * 50 + 20, i * 50 + 21);
		m_map[i][x] = m_stone_color + 1;
	}
}

void COthelloDlg::ChangeStoneXY(int x1, int x2, int y1, int y2) {
	if ((x2 == -1) & (y2 == -1)) return;
	int i_x = x1 < x2 ? x1 + 1 : x2 + 1, i_y = x1<x2 ? y1 : y2, j_x = x1 > x2 ? x1 : x2, j_y = x1 > x2 ? y1 : y2, add = i_y>j_y ? -1 : 1;

	i_y += add;
	while (i_x <= j_x && i_y <= j_x) {
		//DrawStone(i_x * 50 + 20, i_y * 50 + 21);
		m_map[i_y][i_x] = m_stone_color + 1;
		i_x++;
		i_y += add;
	}
}
void COthelloDlg::SetCount() {
	int cnt1 = 0, cnt2 = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (m_map[j][i] == 1) cnt1++;
			if (m_map[j][i] == 2) cnt2++;
		}
	}
	UpdateData(true);
	m_1_count = cnt1;
	m_2_count = cnt2;
	UpdateData(false);
}

void COthelloDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	AfxGetMainWnd()->PostMessageW(WM_CLOSE);
	CDialog::OnClose();
}


void COthelloDlg::OnClickedBtnReady()
{
	if (m_ready) {
		AfxMessageBox(_T("이미 준비됐습니다."));
		return;
	}
	// TODO: Add your control notification handler code here	
	readyMessage *msg = new readyMessage;
	msg->id = 3;
	msg->size = sizeof(readyRecvMessage);
	msg->data.ready = true;
	_tcscpy_s(msg->data.name, m_player1);
	m_clientSocket->Send((char*)msg, sizeof(readyMessage) * 2);
	m_ready = true;

	delete msg;
}


afx_msg LRESULT COthelloDlg::OnClientRecvReady(WPARAM wParam, LPARAM lParam)
{
	readyRecvMessage *msg = (readyRecvMessage*)lParam;

	if(msg->ready == -1){ 
		AfxMessageBox(_T("플레이어가 꽉찼습니다.")); return 0; 
	}

	m_player2 = msg->name;
	UpdateData(false);
	r_dlg = new CRockDlg(this);
	//r_dlg->Create(IDD_ROCK);
	//r_dlg->ShowWindow(SW_SHOW);
	r_dlg->DoModal();

	delete msg;
	return 0;
}


afx_msg LRESULT COthelloDlg::OnClientMapRecv(WPARAM wParam, LPARAM lParam)
{
	gameMessage *msg = (gameMessage*)lParam;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_map[i][j] = msg->m_map[i][j];
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (m_map[i][j] != 0) {
				DrawStone(i * 50 + 21, j * 50 + 21);
			}
		}
	}
	SetCount();
	m_turn = !m_turn;

	delete msg;
	return 0;
}


afx_msg LRESULT COthelloDlg::OnClientTurn(WPARAM wParam, LPARAM lParam)
{
	m_turn = !m_turn;
	return 0;
}

void COthelloDlg::mapSend() {
	gameRecvMessage *msg = new gameRecvMessage;
	msg->id = 6;
	msg->size = sizeof(gameMessage);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			msg->data.m_map[i][j] = m_map[i][j];
		}
	}
	m_clientSocket->Send((char*)msg, sizeof(gameRecvMessage) * 2);

	delete msg;
}


afx_msg LRESULT COthelloDlg::OnClientKingbeom(WPARAM wParam, LPARAM lParam)
{
	choiceMessage *msg = (choiceMessage*)lParam;
	CString str;
	
	if (msg->result == 0) {
		str.Format(_T("비겼습니다! 다시 해주세요!"));

	}
	else if (msg->result == 1) {
		r_dlg->EndDialog(IDOK);
		str.Format(_T("이겼습니다. 먼저 시작합니다."));
		m_turn = true;
		m_stone_color = 0;
	}
	else {
		r_dlg->EndDialog(IDOK);
		str.Format(_T("졌습니다. %s님이 먼저 시작합니다."),m_player2);
		m_stone_color = 1;
	}
	AfxMessageBox(str);
	
	delete msg;
	return 0;
}
