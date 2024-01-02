// TXYC_CustomProgress.cpp: 구현 파일
#include "pch.h"
#include "TXYC_CustomProgress.h"

// TXYC_CustomProgress
IMPLEMENT_DYNAMIC(TXYC_CustomProgress, CWnd)

TXYC_CustomProgress::TXYC_CustomProgress()
{

}

TXYC_CustomProgress::~TXYC_CustomProgress()
{
}

void TXYC_CustomProgress::CreateProgress(CWnd* ap_parent, int a_guide_ctrl_id, int a_ctrl_id, int a_min, int a_max)
{
	RECT rect;
	// 매개 변수로 넘겨 받은 컨트롤 ID를 가진 컨트롤의 좌표를 프로그레스 바 윈도우 좌표로 설정함
	ap_parent->GetDlgItem(a_guide_ctrl_id)->GetWindowRect(&rect);
	ap_parent->ScreenToClient(&rect);
	// 매개 변수로 넘겨 받은 최소값과 최대값을 프로그레스 바의 범위로 설정하면서 최소값을 현재 위치로 설정함
	SetRange(a_min, a_max);
	SetPos(m_min);
	Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, ap_parent, a_ctrl_id);
	// 생성된 프로그레스 바 윈도우의 초기 상태를 그려서 출력함
	DrawWorkImage();
}

void TXYC_CustomProgress::DrawWorkImage()
{
	// 프로그레스 바의 현재 진행량을 나타내는 진행 바 폭을 구함
	int progress_width = (m_pos - m_min) * (m_width - 3) / (m_max - m_min), str_len = 0;
	// 프로그레스 바의 배경 색상을 채움
	m_gdip.Clear(m_bk_color);
	// 진행량이 0이 아니라면 미리 구한 진행 바의 폭만큼 진행 바를 그림
	if (progress_width)
	{
		m_gdip.FillRect(1, 1, progress_width, m_height - 3, m_fore_color);
	}
	// 프로그레스 바 안에 프로그레스 바의 현재 진행 상태를 표시하는 문자열을 출력함
	str_len = swprintf_s(m_progress_str, PROGRESS_STR_LEN, L"%d / %d", m_pos, m_max);
	m_gdip.DrawText(&m_text_rect, m_progress_str, str_len);
}

void TXYC_CustomProgress::SetBkColor(ARGB a_bk_color)
{
	m_bk_color = a_bk_color;
}

void TXYC_CustomProgress::SetForeColor(ARGB a_fore_color)
{
	m_fore_color = a_fore_color;
}

void TXYC_CustomProgress::SetTextColor(ARGB a_text_color)
{
	m_text_color = a_text_color;
}

void TXYC_CustomProgress::SetRange(int a_min, int a_max)
{
	// 프로그레스 바 범위의 최소값과 최대값을 같게 설정했다면 범위 변경 취소
	if (a_min == a_max)
	{
		return;
	}
	// 프로그레스 바 범위의 최대값이 최소값보다 크다면 인자로 넘어온 최소값과 최대값 교환
	if (a_max < a_min)
	{
		m_min = a_max;
		m_max = a_min;
	}
	else
	{
		m_min = a_min;
		m_max = a_max;
	}
}

void TXYC_CustomProgress::SetStep(int a_step)
{
	m_step = a_step;
}

void TXYC_CustomProgress::SetPos(int a_pos)
{
	m_pos = a_pos;
	// 현재 위치값을 프로그레스 바 최소값보다 작은 값으로 설정했다면 현재 위치값을 최소값으로 설정
	if (m_pos < m_min)
	{
		m_pos = m_min;
	}
	// 현재 위치값을 프로그레스 바 최대값보다 큰 값으로 설정했다면 현재 위치값을 최대값으로 설정
	else if (m_pos > m_max)
	{
		m_pos = m_max;
	}
}

int TXYC_CustomProgress::GetPos()
{
	return m_pos;
}

void TXYC_CustomProgress::UpdatePos(int a_pos)
{
	// 매개 변수로 주어진 값을 현재 위치로 설정함
	SetPos(a_pos);
	// 변경된 현재 위치에 맞게 프로그레스 바를 갱신함
	DrawWorkImage();
	Invalidate(FALSE);
}

int TXYC_CustomProgress::StepForward()
{
	// 현재 위치에다가 1회 진행 증가량만큼 증가시킨 위치를 현재 위치로 변경시킴
	UpdatePos(m_pos + m_step);
	// 변경된 현재 위치를 반환함
	return m_pos;
}

BEGIN_MESSAGE_MAP(TXYC_CustomProgress, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// TXYC_CustomProgress 메시지 처리기
void TXYC_CustomProgress::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	m_gdip.Draw(dc.m_hDC);
}

int TXYC_CustomProgress::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);
	// 프로그레스 바의 크기를 설정함
	m_width = rect.Width();
	m_height = rect.Height();
	// 프로그레스 바에 출력될 문자열의 출력 영역을 설정함
	m_text_rect.X = 1;
	m_text_rect.Y = 1;
	m_text_rect.Width = m_width - 3;
	m_text_rect.Height = m_height - 3;
	// 프로그레스 바의 크기와 일치하는 GDI+ 출력 객체를 생성함
	m_gdip.CreateGDIP(m_width, m_height);
	// 프로그레스 바에 출력될 문자열의 크기와 색상을 설정함
	m_gdip.ChangeFont(L"맑은 고딕", 40);
	m_gdip.SetTextColor(m_text_color);
	return 0;
}
