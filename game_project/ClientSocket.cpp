// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "game_project.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket member functions
void CClientSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CClientSocket::OnReceive(int nErrorCode) {
	int header[2];

	Receive((char*)header, sizeof(int) * 2);
	if (header[0] == 2) { //메세지 이벤트
		msgRecvMessage *msg = new msgRecvMessage;
		ZeroMemory(msg, sizeof(msgRecvMessage));
		Receive((char *)msg, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_RECV, 0, (LPARAM)((LPCTSTR)msg->str));
	}
	if(header[0] == 4){ //게임 준비 이벤트
		readyRecvMessage *msg = new readyRecvMessage;
		ZeroMemory(msg, sizeof(readyRecvMessage));
		Receive((char*)msg, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_RECV_READY, 0, (LPARAM)msg);
	}
	if (header[0] == 5) { //게임 준비 이벤트
		readyRecvMessage *msg = new readyRecvMessage;
		ZeroMemory(msg, sizeof(readyRecvMessage));
		Receive((char*)msg, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_RECV_READY, 0, (LPARAM)msg);
	}
	if (header[0] == 6) {
		gameMessage *msg = new gameMessage;
		ZeroMemory(msg, sizeof(gameMessage));
		Receive((char*)msg, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_MAP_RECV, 0, (LPARAM)msg);
	}
	if (header[0] == 7) {
		SendMessage(m_hWnd, WM_CLIENT_TURN, 0, (LPARAM)this);
	}
	if (header[0] == 402) {
		choiceMessage *msg = new choiceMessage;
		ZeroMemory(msg, sizeof(choiceMessage));
		Receive((char*)msg, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_KINGBEOM, 0, (LPARAM)msg);
	}
	
	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) {
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR: Disconnected from server"));
	PostQuitMessage(0);
}