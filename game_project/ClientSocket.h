#pragma once
#include "Message.h"

// CClientSocket command target
#define WM_CLIENT_RECV WM_USER+1
#define WM_CLIENT_RECV_READY WM_USER+2
#define WM_CLIENT_MAP_RECV WM_USER+3
#define WM_CLIENT_TURN WM_USER+4
#define WM_CLIENT_KINGBEOM WM_USER+5

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	void SetWnd(HWND hWnd);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	HWND m_hWnd;
};
