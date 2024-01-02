#ifndef _TXYC_CUSTOM_CAPTION_H_
#define _TXYC_CUSTOM_CAPTION_H_

// 클래스 작성자: 박정현(bac6536@naver.com)

#include "TXYC_GDIP.h"
#include "TXYC_CaptionControl.h"
#include <vector>

// 가짜 상단 테두리 높이(가짜 상단 테두리를 그리지 않는다면 이 값을 0으로 설정)
#define FAKE_TOP_BORDER_HEIGHT 0
// 캡션 영역에 위아래 각각 1픽셀만큼의 테두리를 그리기 위한 값
#define CAPTION_TOPBOTTOM_BORDERS 2
// 윈도우가 전체화면 상태일 때 최대화 버튼 아이콘의 사각형 2개 사이의 간격
#define MAXIMIZE_BTN_RECT_OFFSET 2
// 캡션 버튼의 폭과 높이
#define CAPTION_BTN_WIDTH 48
#define CAPTION_BTN_HEIGHT 32
#define TOP_FRAME_HEIGHT 6 // 캡션의 상단 프레임 높이
#define TOP_FRAME_SPACING 6 // 캡션의 상단 프레임과 캡션 탭 사이의 여백 공간
#define CAPTION_TAB_WIDTH 100 // 캡션 탭 1개의 폭
#define MAX_CAPTION_TAB_COUNT 3 // 캡션 탭의 최대 개수
// 캡션 탭의 이름 문자열이 출력될 출력 공간과 캡션 탭 영역 사이의 간격
#define CAPTION_TAB_TEXT_SPACING 4
#define CAPTION_TAB_FONT 12 // 캡션 탭의 이름 문자열 폰트 크기

using std::vector;

// 사용자 정의 캡션에 사용될 좌표 정보들
typedef struct _CaptionRects
{
	RECT Caption; // 캡션 영역의 좌표 정보
	RECT CaptionTab; // 전체 캡션 탭 영역(모든 캡션 탭들을 다 합친 영역)의 좌표 정보
	RECT TopFrame; // 상단 프레임 영역의 좌표 정보
	RECT MinimizeBtn, MaximizeBtn, CloseBtn; // 캡션 버튼들(최소화, 최대화, 닫기)의 좌표 정보
} CaptionRects;

// 사용자 정의 캡션에 사용될 색상 정보들
typedef struct _CaptionColors
{
	ARGB BgColor; // 윈도우 배경 색상
	// 윈도우 활성화 상태에 따라 사용할 캡션 채우기 색상들
	ARGB ActiveFillColor, InactiveFillColor;
	// 윈도우 활성화 상태에 따라 사용할 캡션 버튼 아이템 색상들
	ARGB ActiveItemColor, InactiveItemColor;
	// 윈도우 활성화 상태에 따라 사용할 호버링된 캡션 버튼 색상들
	ARGB ActiveBtnHoverColor, InactiveBtnHoverColor;
	ARGB ActiveBtnClickColor; // 캡션 버튼 클릭 색상
	// 닫기 버튼 전용 호버링된 캡션 버튼 색상과 클릭된 캡션 버튼 색상
	ARGB CloseBtnHoverColor, CloseBtnClickColor;
	ARGB CloseBtnItemColor; // 닫기 버튼 전용 캡션 버튼 아이템 색상
	ARGB SelectedTabColor; // 선택된 캡션 탭 색상
	ARGB HoveredTabColor; // 호버링된 캡션 탭 색상
	ARGB UnselectedTabColor; // 선택되지 않은 캡션 탭 색상
} CaptionColors;

// 하나의 캡션 탭이 가지고 있는 정보들
typedef struct _CaptionTabData
{
	CDialog* p_TabDlg; // 이 캡션 탭과 연결된 다이얼로그의 주소
	wchar_t TabText[64]; // 이 캡션 탭의 이름 문자열
	RectF TextArea; // 캡션 탭의 이름 문자열이 출력될 출력 공간(실제 캡션 탭의 크기보다 조금 작음)
} CaptionTabData;

// 마우스가 가리킬 수 있는 캡션 버튼의 종류
typedef enum _SelectedCaptionBtn
{
	SelectedBtn_None,
	SelectedBtn_Minimize,
	SelectedBtn_Maximize,
	SelectedBtn_Close
} SelectedCaptionBtn;

// 마우스로 클릭할 수 있는 클라이언트 영역의 종류
typedef enum _ClickedArea
{
	Clicked_None,
	Clicked_CaptionBtn,
	Clicked_CaptionTab,
	Clicked_ClientArea
} ClickedArea;

// 사용자 정의 캡션에 사용될 색상 정보들을 가리키기 위한 인덱스
typedef enum _CaptionColorIndex
{
	BgColor_Index,
	ActiveFillColor_Index,
	InactiveFillColor_Index,
	ActiveItemColor_Index,
	InactiveItemColor_Index,
	ActiveBtnHoverColor_Index,
	InactiveBtnHoverColor_Index,
	ActiveBtnClickColor_Index,
	CloseBtnHoverColor_Index,
	CloseBtnClickColor_Index,
	CloseBtnItemColor_Index,
	SelectedTabColor_Index,
	HoveredTabColor_Index,
	UnselectedTabColor_Index
} CaptionColorIndex;

class TXYC_CustomCaption
{
protected:
	TXYC_GDIP m_gdip; // GDI+ 출력을 위한 객체
	UINT m_dpi = 0; // 모니터의 화면 배율에 맞게 캡션의 크기를 조절하기 위한 모니터 DPI 값
	RECT m_client_rect = { 0 }; // 캡션까지 확장된 클라이언트 영역의 좌표 정보
	RECT m_icon_rect = { 0 }; // 캡션 버튼 아이콘을 그릴 때 사용할 사각형 영역
	CWnd* mp_wnd = NULL; // 사용자 정의 캡션을 사용할 윈도우의 주소
	CaptionRects m_cptn_rects = { {0} }; // 사용자 정의 캡션에 사용할 좌표 정보들
	// WM_INPUT 메시지 처리 함수에서 사용할 변수들
	RAWINPUT m_rawinput = { 0 };
	UINT m_nSize = sizeof(RAWINPUT);
	// 마우스가 현재 호버링중인 캡션 버튼
	SelectedCaptionBtn m_hovered_btn = SelectedBtn_None;
	// 마우스가 이전에 호버링했던 캡션 버튼
	SelectedCaptionBtn m_old_hovered_btn = SelectedBtn_None;
	// 마우스가 클릭한 캡션 버튼
	SelectedCaptionBtn m_clicked_btn = SelectedBtn_None;
	// 기능을 수행할 캡션 버튼
	SelectedCaptionBtn m_triggered_btn = SelectedBtn_None;
	ClickedArea m_is_clicked = Clicked_None; // 현재 클릭한 클라이언트 영역의 종류
	CaptionColors m_cptn_colors = { 0 }; // 캡션 영역을 그릴 때 사용할 색상들
	// CaptionTabData m_tab_data[MAX_CAPTION_TAB_COUNT] = { 0 }; // 각 캡션 탭에 필요한 데이터들
	vector<CaptionTabData> m_tab_data; // 각 캡션 탭에 필요한 데이터들
	UINT8 m_active_state = 0; // 윈도우의 활성화 상태 정보
	UINT8 m_nc_hover_state = 0; // 논클라이언트 영역 호버링 상태 정보
	UINT8 m_hover_state = 0; // 클라이언트 영역 호버링 상태 정보
	UINT m_icon_dimension = 0; // 캡션 버튼 아이콘의 치수 정보
	UINT m_top_border_height = 0; // 캡션의 상단 테두리 높이
	UINT m_btn_width = 0, m_btn_height = 0; // 캡션 버튼의 폭과 높이
	UINT m_tab_width = 0, m_tab_height = 0; // 캡션 탭의 폭과 높이
	UINT8 m_tab_count = 0; // 캡션 탭의 개수
	UINT8 m_selected_tab = 0; // 현재 선택된 캡션 탭
	int m_hovered_tab = -1; // 현재 호버링된 캡션 탭
	int m_old_hovered_tab = -1; // 이전에 호버링했던 캡션 탭
public:
	TXYC_CustomCaption(CWnd* ap_wnd);
	// 첫 번째 매개 변수로 주어진 값을 두 번째 매개 변수인 모니터 DPI 값에 맞게 변경시키는 함수
	UINT GetDPIScale(UINT a_value, UINT a_dpi);
	// 가짜 상단 테두리의 좌표를 설정하는 함수
	void SetFakeWndTopBorderRect(RECT* ap_fake_top_border_rect);
	void SetCaptionRectPos(); // 사용자 정의 캡션에 필요한 사각형 영역들의 좌표를 설정하는 함수
	void SetCaptionTabPos(); // 캡션 탭들과 캡션 탭 다이얼로그들의 좌표를 설정하는 함수
	BOOL IsWindowMaximized(); // 윈도우가 최대화된 상태인지 확인하기 위한 함수
	// 첫 번째 매개 변수로 주어진 사각형 영역의 좌표를 두 번째 매개 변수로 주어진 사각형 영역의 좌표
	// 내부 정중앙에 위치시키는 함수
	void CenterRectInRect(RECT* ap_center_rect, const RECT* ap_outer_rect);
	// 윈도우가 최대화된 상태일 때의 캡션까지 확장된 클라이언트 영역을 구하는 함수
	void AdjustMaximizedClientRect(RECT* ap_rect);
	// 윈도우가 최대화된 상태가 아닐 때의 캡션까지 확장된 클라이언트 영역의 좌표를 설정하는 함수
	void AdjustUnMaximizedClientRect(RECT* ap_rect);
	void StartEventCapture(); // WM_INPUT 메시지 처리 함수를 위한 마우스 모니터링 시작 함수
	void EndEventCapture(); // 마우스 모니터링 종료 함수
	// GDI+ 출력 객체와 연결된 사용자 정의 캡션 이미지를 갱신하는 함수
	void DrawCaptionImage();
	// 추가된 캡션 탭에 연결할 다이얼로그를 설정하는 함수
	void AddCaptionTabDialog(CDialog* ap_dlg, const wchar_t* ap_text, int a_dlg_id);
	// 캡션 탭들의 종료 준비 상태를 반환하는 함수
	UINT8 GetCloseReadyStatus();
	// 사용자 정의 캡션 탭을 사용하는 윈도우가 닫힐 때 각 캡션 탭과 연결된 다이얼로그를 제거하는 함수
	void DestroyCaptionTabDialogs();
	// 사용자 정의 캡션과 연결된 다이얼로그가 사용자 정의 캡션에게 캡션 탭 다이얼로그에서 마우스 클릭이
	// 발생했음을 알릴 때 사용하는 함수
	void NotifyTabWndLBtnDown();
	// 사용자 정의 캡션과 연결된 다이얼로그가 사용자 정의 캡션에게 현재 캡션 탭의 다음 탭이 선택된
	// 캡션 탭으로 변경됐음을 알릴 때 사용하는 함수
	void NotifyNextTabWnd();
	// 사용자 정의 캡션과 연결된 다이얼로그가 사용자 정의 캡션에게 현재 캡션 탭의 이전 탭이 선택된
	// 캡션 탭으로 변경됐음을 알릴 때 사용하는 함수
	void NotifyPrevTabWnd();
	// 사용자 정의 캡션과 연결된 다이얼로그의 함수에서 호출하기 위한 함수들
	// OnInitDialog() 함수에서 호출하는 함수
	void OnInitCaption();
	// WM_CREATE 메시지 처리 함수에서 호출하는 함수
	void OnCreateCaption();
	// WM_ACTIVATE 메시지 처리 함수에서 호출하는 함수
	void OnActivateCaption(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// WM_NCCALCSIZE 메시지 처리 함수에서 호출하는 함수
	UINT OnNcCalcSizeCaption(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	// WM_NCHITTEST 메시지 처리 함수에서 호출하는 함수
	LRESULT OnNcHitTestCaption(CPoint point);
	// WM_SETCURSOR 메시지 처리 함수에서 호출하는 함수
	BOOL OnSetCursorCaption(CWnd* pWnd, UINT nHitTest, UINT message);
	// WM_NCMOUSEMOVE 메시지 처리 함수에서 호출하는 함수
	void OnNcMouseMoveCaption(UINT nHitTest, CPoint point);
	// WM_NCMOUSELEAVE 메시지 처리 함수에서 호출하는 함수
	void OnNcMouseLeaveCaption();
	// WM_SIZE 메시지 처리 함수에서 호출하는 함수
	void OnSizeCaption(UINT nType, int cx, int cy);
	// WM_INPUT 메시지 처리 함수에서 호출하는 함수
	void OnRawInputCaption(UINT nInputcode, HRAWINPUT hRawInput);
	// WM_LBUTTONDOWN 메시지 처리 함수에서 호출하는 함수
	UINT OnLBtnDownCaption(UINT nFlags, CPoint point);
	// WM_LBUTTONUP 메시지 처리 함수에서 호출하는 함수
	UINT OnLBtnUpCaption(UINT nFlags, CPoint point);
	// WM_MOUSEMOVE 메시지 처리 함수에서 호출하는 함수
	void OnMouseMoveCaption(UINT nFlags, CPoint point);
	// WM_MOUSELEAVE 메시지 처리 함수에서 호출하는 함수
	void OnMouseLeaveCaption();
};

#endif // !_TXYC_CUSTOM_CAPTION_H_