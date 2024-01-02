#ifndef _TXYC_CUSTOM_PROGRESS_H_
#define _TXYC_CUSTOM_PROGRESS_H_

// 클래스 작성자: 박정현(bac6536@naver.com)
// 이 클래스는 다른 개발자가 작성한 클래스를 기반으로 작성되었음
// 원본 클래스 작성자: 김성엽(tipsware@naver.com)
// 원본 클래스 배포 주소: https://cafe.naver.com/mythread

#include "TXYC_GDIP.h"

// 프로그레스 바에 출력될 문자열의 길이
#define PROGRESS_STR_LEN 16

// TXYC_CustomProgress
class TXYC_CustomProgress : public CWnd
{
	DECLARE_DYNAMIC(TXYC_CustomProgress)
protected:
	TXYC_GDIP m_gdip;
	// 프로그레스 바의 배경 색상, 진행 바 색상, 출력될 문자열의 색상
	ARGB m_bk_color = 0xFF0050A0, m_fore_color = 0xFF00A0FF, m_text_color = 0xFFFFFFFF;
	// 프로그레스 바에 출력될 문자열의 출력 영역
	RectF m_text_rect;
	// 프로그레스 바에 출력될 문자열
	wchar_t m_progress_str[PROGRESS_STR_LEN] = { 0 };
	// 프로그레스 바의 현재 위치, 최소값, 최대값, 1회 진행 증가량
	int m_pos = 0, m_min = 0, m_max = 100, m_step = 1;
	// 프로그레스 바의 폭과 높이
	int m_width = 0, m_height = 0;
public:
	TXYC_CustomProgress();
	virtual ~TXYC_CustomProgress();
	// 프로그레스 바의 범위를 설정하면서 윈도우를 생성하는 함수
	void CreateProgress(CWnd* ap_parent, int a_guide_ctrl_id, int a_ctrl_id, int a_min, int a_max);
	// GDI+ 출력 객체와 연결된 클라이언트 영역 이미지를 갱신하는 함수
	void DrawWorkImage();
	// 프로그레스 바의 배경 색상을 변경하는 함수
	void SetBkColor(ARGB a_bk_color);
	// 프로그레스 바의 진행 바 색상을 변경하는 함수
	void SetForeColor(ARGB a_fore_color);
	// 프로그레스 바에 출력될 문자열의 색상을 변경하는 함수
	void SetTextColor(ARGB a_text_color);
	// 프로그레스 바의 범위를 변경하는 함수
	void SetRange(int a_min, int a_max);
	// 프로그레스 바의 1회 진행 위치 증가량을 변경하는 함수
	void SetStep(int a_step);
	// 프로그레스 바의 현재 위치를 변경하는 함수
	void SetPos(int a_pos);
	// 프로그레스 바의 현재 위치를 반환하는 함수
	int GetPos();
	// 매개 변수로 주어진 값에 맞게 프로그레스 바를 갱신하는 함수
	void UpdatePos(int a_pos);
	// 프로그레스 바의 현재 위치를 1회 진행 증가량만큼 변경시키는 함수
	int StepForward();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#endif // !_TXYC_CUSTOM_PROGRESS_H_