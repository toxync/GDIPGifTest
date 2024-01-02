// ProgressDlg.cpp: 구현 파일
#include "pch.h"
#include "GDIPGifTest.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"

// ProgressDlg 대화 상자
IMPLEMENT_DYNAMIC(ProgressDlg, CDialog)

ProgressDlg::ProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PROGRESS_DLG, pParent)
{

}

ProgressDlg::~ProgressDlg()
{
}

void ProgressDlg::SetWorkSize(int a_work_size)
{
	m_work_size = a_work_size;
}

void ProgressDlg::SetThreadDataPtr(FrameThreadData* ap_data)
{
	mp_frame_thread_data = ap_data;
}

FrameThreadData* ProgressDlg::GetThreadDataPtr()
{
	return mp_frame_thread_data;
}

void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ProgressDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &ProgressDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ProgressDlg::OnBnClickedCancel)
	ON_MESSAGE(SINGLE_WORK_COMPLETE, &ProgressDlg::On20000)
	ON_MESSAGE(DESTROY_PROGRESS_DLG, &ProgressDlg::On20001)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABORT_BTN, &ProgressDlg::OnBnClickedAbortBtn)
END_MESSAGE_MAP()

// ProgressDlg 메시지 처리기
BOOL ProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 최대값이 전체 작업량인 사용자 정의 프로그레스 바를 생성함
	m_work_progress.CreateProgress(this, IDC_PROGRESS_RECT, 1500, 0, m_work_size);
	if (mp_frame_thread_data)
	{
		// 작업 스레드가 작업 하나를 끝낼 때마다 이 다이얼로그로 메시지를 보낼 때 사용하기 위한
		// 이 다이얼로그의 핸들값을 스레드 데이터에 저장함
		mp_frame_thread_data->h_progress_wnd = m_hWnd;
		// 정지된 상태로 생성된 작업 스레드를 실행시킴
		::ResumeThread(mp_frame_thread_data->h_thread);
	}
	else
	{
		AfxMessageBox(L"invalid frame thread data");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ProgressDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}

void ProgressDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void ProgressDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

afx_msg LRESULT ProgressDlg::On20000(WPARAM wParam, LPARAM lParam)
{
	// 작업 스레드의 작업이 하나 끝났다는 메시지를 받을 때마다 프로그레스 바를 한 단계 진행시킴
	if (m_work_progress.StepForward() == m_work_size)
	{
		// 프로그레스 바의 위치가 전체 작업량과 동일해지면 작업 스레드가 종료됐다(모든 작업이 끝났다)는
		// 의미이므로 이 다이얼로그를 제거함
		EndDialog(IDOK);
	}
	return 0;
}

afx_msg LRESULT ProgressDlg::On20001(WPARAM wParam, LPARAM lParam)
{
	// 스레드가 강제로 종료됐으므로 작업 상태 표시 다이얼로그를 제거함
	EndDialog(IDCANCEL);
	return 0;
}

void ProgressDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 다이얼로그가 제거될 때 가지고 있던 스레드 데이터의 주소를 무효화시킴
	mp_frame_thread_data = NULL;
}

void ProgressDlg::OnBnClickedAbortBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 현재 실행중인 프레임 스레드의 중지 메시지를 메인 다이얼로그에게 보냄
	::SendMessage(mp_frame_thread_data->h_main_wnd, ABORT_FRAME_THREAD, 0, 0);
}
