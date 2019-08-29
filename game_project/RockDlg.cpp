// RockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "game_project.h"
#include "RockDlg.h"
#include "afxdialogex.h"

#include "OthelloDlg.h"
// CRockDlg dialog

IMPLEMENT_DYNAMIC(CRockDlg, CDialog)

CRockDlg::CRockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ROCK, pParent)
	, m_choice(0)
{
}

CRockDlg::~CRockDlg()
{
}

void CRockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRockDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ROCK, &CRockDlg::OnClickedBtnRock)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_SCISSORS, &CRockDlg::OnClickedBtnScissors)
	ON_BN_CLICKED(IDC_BTN_PAPER, &CRockDlg::OnClickedBtnPaper)
END_MESSAGE_MAP()


// CRockDlg message handlers
void CRockDlg::OnImagePrint(int choice,int x,int y) {
	//srand(time(NULL));
	CImage img;
	CClientDC dc(this);
	if (choice == 1) {
		img.Load(_T("r.jpg"));
		img.BitBlt(dc.m_hDC, x, y);

	}
	else if (choice == 2) {
		img.Load(_T("s.jpg"));
		img.BitBlt(dc.m_hDC, x, y);
	}
	else if (choice == 3) {
		img.Load(_T("p.jpg"));
		img.BitBlt(dc.m_hDC, x, y);
	}
	//이미지 파괴하기
	//img.Destroy();
	//페이지 새로고침
}

void CRockDlg::OnClickedBtnRock()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_choice = 1;
	UpdateData(FALSE);

	OnImagePrint(m_choice,170,70);

	choiceRecvMessage* temp = new choiceRecvMessage;
	temp->id = 401;
	temp->size = sizeof(choiceMessage);
	temp->data.choice = m_choice;
	/*포트넘*/

	m_clientSocket->Send((char *)temp, sizeof(choiceRecvMessage)*2);
	delete temp;

}


int CRockDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	COthelloDlg *p_dlg = (COthelloDlg*)GetParent();

	m_clientSocket = p_dlg->m_clientSocket;

	return 0;
}


void CRockDlg::OnClickedBtnScissors()
{
	// TODO: Add your control notification handler code here
	m_choice = 2;
	UpdateData(FALSE);
	OnImagePrint(m_choice,170,70);

	choiceRecvMessage* temp = new choiceRecvMessage;
	temp->id = 401;
	temp->size = sizeof(choiceMessage);
	temp->data.choice = m_choice;

	/*포트넘*/

	m_clientSocket->Send((char *)temp, sizeof(choiceRecvMessage) * 2);
	delete temp;

}


void CRockDlg::OnClickedBtnPaper()
{
	// TODO: Add your control notification handler code here
	m_choice = 3;
	UpdateData(FALSE);
	OnImagePrint(m_choice,170,70);


	choiceRecvMessage* temp = new choiceRecvMessage;
	temp->id = 401;
	temp->size = sizeof(choiceMessage);
	temp->data.choice = m_choice;

	/*포트넘*/

	m_clientSocket->Send((char *)temp, sizeof(choiceRecvMessage) * 2);
	

	delete temp;
}

