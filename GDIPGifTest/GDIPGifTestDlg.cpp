// GDIPGifTestDlg.cpp: 구현 파일
#include "pch.h"
#include "framework.h"
#include "GDIPGifTest.h"
#include "GDIPGifTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGDIPGifTestDlg 대화 상자
CGDIPGifTestDlg::CGDIPGifTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GDIPGIFTEST_DIALOG, pParent), TXYC_CustomCaption(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGDIPGifTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGDIPGifTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGDIPGifTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGDIPGifTestDlg::OnBnClickedCancel)
	ON_MESSAGE(TABWND_LBTNDOWN, &CGDIPGifTestDlg::OnLBtnDownTabWnd)
	ON_MESSAGE(TABWND_CHANGETAB, &CGDIPGifTestDlg::OnChangeTabWnd)
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCMOUSELEAVE()
	ON_WM_SIZE()
	ON_WM_INPUT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CGDIPGifTestDlg 메시지 처리기
BOOL CGDIPGifTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	OnInitCaption(); // 사용자 정의 캡션에서 사용할 색상들을 초기화함
	// 캡션 탭 다이얼로그에 포커스를 설정해야 하므로 FALSE를 반환함
	return FALSE;
	// return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CGDIPGifTestDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_gdip.Draw(dc.m_hDC);
		// CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGDIPGifTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGDIPGifTestDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void CGDIPGifTestDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

afx_msg LRESULT CGDIPGifTestDlg::OnLBtnDownTabWnd(WPARAM wParam, LPARAM lParam)
{
	// 캡션 탭 다이얼로그 안에서 마우스 클릭이 발생했음을 사용자 정의 캡션에게 알림
	NotifyTabWndLBtnDown();
	return 0;
}

afx_msg LRESULT CGDIPGifTestDlg::OnChangeTabWnd(WPARAM wParam, LPARAM lParam)
{
	// 'Ctrl + Tab'이나 'Ctrl + Shift + Tab' 키 입력이 발생해서 선택된 캡션 탭 다이얼로그가
	// 변경됐음을 사용자 정의 캡션에게 알림
	// wParam의 값이 1이라면 'Ctrl + Shift + Tab' 키 입력이 발생했다는 의미이므로 이전 캡션 탭으로 이동함
	if (wParam)
	{
		NotifyPrevTabWnd();
	}
	// wParam의 값이 0이라면 'Ctrl + Tab' 키 입력이 발생했다는 의미이므로 다음 캡션 탭으로 이동함
	else
	{
		NotifyNextTabWnd();
	}
	return 0;
}

void CGDIPGifTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// ESC 키의 입력으로 윈도우를 닫는 동작은 금지하지만 닫기 버튼이나 'ALT + F4' 단축키의 입력으로
	// 윈도우를 닫는 동작은 허용하며 모든 캡션 탭 다이얼로그들이 닫힐 준비가 되어있을 경우에만
	// 메인 다이얼로그를 닫음
	if (((nID & 0xFFF0) == SC_CLOSE) && GetCloseReadyStatus())
	{
		EndDialog(IDCANCEL);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

int CGDIPGifTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 마우스 모니터링을 시작하고 사용자 정의 캡션에 필요한 사각형 영역들의 좌표를 구함
	OnCreateCaption();
	// 원하는 다이얼로그가 연결된 캡션 탭을 원하는 개수만큼 추가함
	AddCaptionTabDialog(new GifDasmDlg, L"GIF 분해", IDD_GIF_DASM_DLG);
	AddCaptionTabDialog(new GifAsmDlg, L"GIF 제작", IDD_GIF_ASM_DLG);
	AddCaptionTabDialog(new GifExtraDlg, L"추가 캡션 탭", IDD_GIF_EXTRA_DLG);
	// 캡션 탭들과 캡션 탭에 연결된 다이얼로그들의 좌표를 설정함(이 작업은 반드시 캡션 탭의 추가 작업이
	// 끝난 다음에 수행해야 함)
	SetCaptionTabPos();
	return 0;
}

void CGDIPGifTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EndEventCapture(); // 프로그램이 종료되면 마우스 모니터링 종료
	// 모든 캡션 탭 다이얼로그들을 제거함
	DestroyCaptionTabDialogs();
}

void CGDIPGifTestDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 윈도우의 활성화 상태를 사용자 정의 캡션에 적용시킴
	OnActivateCaption(nState, pWndOther, bMinimized);
}

void CGDIPGifTestDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!OnNcCalcSizeCaption(bCalcValidRects, lpncsp))
	{
		// 사용자 정의 캡션의 WM_NCCALCSIZE 메시지 처리 동작이 수행되지 않았을 경우에는
		// 여기서 원하는 작업을 처리함
		CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}

LRESULT CGDIPGifTestDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 상단 프레임을 제외한 나머지 크기 조절 프레임 영역은 기본 동작이 처리하도록 설정
	LRESULT hit = CDialog::OnNcHitTest(point);
	switch (hit)
	{
	case HTNOWHERE:
	case HTTOPLEFT:
	case HTLEFT:
	case HTBOTTOMLEFT:
	case HTBOTTOM:
	case HTBOTTOMRIGHT:
	case HTRIGHT:
	case HTTOPRIGHT:
		return hit;
	}
	// 매개 변수로 넘겨받은 좌표는 화면 좌표계 기준 좌표이지만 캡션 버튼과 캡션 탭의 좌표는
	// 윈도우 좌표계 기준 좌표이기 때문에 매개 변수로 넘겨받은 좌표를 윈도우 좌표계 기준으로 변환함
	::ScreenToClient(m_hWnd, &point);
	// 사용자 정의 캡션에서 수행한 마우스 히트 테스트 결과를 윈도우의 마우스 히트 테스트 결과로 설정함
	return OnNcHitTestCaption(point);
	// return CDialog::OnNcHitTest(point);
}

BOOL CGDIPGifTestDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 사용자 정의 캡션이 마우스 커서를 조작할 필요가 있는지 확인함
	BOOL result = OnSetCursorCaption(pWnd, nHitTest, message);
	// OnSetCursorCaption() 함수의 반환값에 의해 이 함수의 반환값이 결정됨
	return result ? result : CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CGDIPGifTestDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 윈도우가 WM_NCMOUSELEAVE 메시지를 받기 위해 필요한 작업을 수행함
	OnNcMouseMoveCaption(nHitTest, point);
	CDialog::OnNcMouseMove(nHitTest, point);
}

void CGDIPGifTestDlg::OnNcMouseLeave()
{
	// 이 기능을 사용하려면 Windows 2000 이상이 필요합니다.
	// _WIN32_WINNT 및 WINVER 기호는 0x0500보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 마우스의 윈도우 논클라이언트 영역 이탈 상태를 사용자 정의 캡션에 적용시킴
	OnNcMouseLeaveCaption();
	CDialog::OnNcMouseLeave();
}

void CGDIPGifTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnSizeCaption(nType, cx, cy); // 변경된 윈도우의 크기를 사용자 정의 캡션에 적용시킴
}

void CGDIPGifTestDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 윈도우 밖에서 마우스 클릭이 해제되어도 사용자 정의 캡션에 마우스 클릭 해제 상태가 정상적으로
	// 반영될 수 있도록 처리함
	OnRawInputCaption(nInputcode, hRawInput);
	CDialog::OnRawInput(nInputcode, hRawInput);
}

void CGDIPGifTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!OnLBtnDownCaption(nFlags, point))
	{
		// 사용자 정의 캡션의 WM_LBUTTONDOWN 메시지 처리 동작이 수행되지 않았을 경우에는
		// 여기서 원하는 작업을 처리함
		CDialog::OnLButtonDown(nFlags, point);
	}
}

void CGDIPGifTestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!OnLBtnUpCaption(nFlags, point))
	{
		// 사용자 정의 캡션의 WM_LBUTTONUP 메시지 처리 동작이 수행되지 않았을 경우에는
		// 여기서 원하는 작업을 처리함
		CDialog::OnLButtonUp(nFlags, point);
	}
}

void CGDIPGifTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 마우스 이동으로 인해 캡션 버튼이나 캡션 탭의 호버링 상태가 변경됐는지 확인함
	OnMouseMoveCaption(nFlags, point);
	CDialog::OnMouseMove(nFlags, point);
}

void CGDIPGifTestDlg::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 마우스의 윈도우 클라이언트 영역 이탈 상태를 사용자 정의 캡션에 적용시킴
	OnMouseLeaveCaption();
	CDialog::OnMouseLeave();
}
