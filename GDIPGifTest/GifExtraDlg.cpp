// GifExtraDlg.cpp: 구현 파일
#include "pch.h"
#include "GDIPGifTest.h"
#include "GifExtraDlg.h"

// GifExtraDlg 대화 상자
IMPLEMENT_DYNAMIC(GifExtraDlg, CDialog)

GifExtraDlg::GifExtraDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GIF_EXTRA_DLG, pParent)
{

}

GifExtraDlg::~GifExtraDlg()
{
}

void GifExtraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(GifExtraDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &GifExtraDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GifExtraDlg::OnBnClickedCancel)
	ON_MESSAGE(TABWND_CLOSE_READY, &GifExtraDlg::On15002)
END_MESSAGE_MAP()

// GifExtraDlg 메시지 처리기
BOOL GifExtraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void GifExtraDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}

void GifExtraDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void GifExtraDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

BOOL GifExtraDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 이 다이얼로그에서 마우스 클릭이 발생했음을 부모 다이얼로그에게 알림
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		::PostMessage(GetParent()->m_hWnd, TABWND_LBTNDOWN, 0, 0);
	}
	// 이 다이얼로그에서 'Ctrl + Tab'이나 'Ctrl + Shift + Tab' 키 입력이 발생했음을
	// 부모 다이얼로그에게 알림(WM_KEYDOWN 메시지의 lParam 매개 변수 30번 비트가 0이면 메시지를
	// 발생시킨 키가 이전에 입력되지 않은 상태로 메시지가 발생했다는 의미이고 WM_KEYDOWN 메시지의
	// lParam 매개 변수 30번 비트가 1이면 메시지를 발생시킨 키가 이전에 입력된 상태로 메시지가
	// 발생했다는 의미임)
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && !((pMsg->lParam >> 30) & 1) &&
		(::GetAsyncKeyState(VK_CONTROL) & 0x8000))
	{
		// 'Ctrl + Shift + Tab' 키 입력이 발생했다면 이전 탭으로 이동
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			::PostMessage(GetParent()->m_hWnd, TABWND_CHANGETAB, 1, 0);
		}
		// 'Ctrl + Tab' 키 입력이 발생했다면 다음 탭으로 이동
		else
		{
			::PostMessage(GetParent()->m_hWnd, TABWND_CHANGETAB, 0, 0);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

afx_msg LRESULT GifExtraDlg::On15002(WPARAM wParam, LPARAM lParam)
{
	// 메인 다이얼로그에게 이 다이얼로그의 종료 준비 상태 알림 메시지를 보냄(이 다이얼로그에서는
	// 추가 스레드를 사용하지 않기 때문에 항상 종료할 준비가 되어 있음)
	::PostMessage(GetParent()->m_hWnd, TABWND_CLOSE_READY, 1, 0);
	return 0;
}