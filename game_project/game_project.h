
// game_project.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cgame_projectApp:
// �� Ŭ������ ������ ���ؼ��� game_project.cpp�� �����Ͻʽÿ�.
//

class Cgame_projectApp : public CWinApp
{
public:
	Cgame_projectApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cgame_projectApp theApp;