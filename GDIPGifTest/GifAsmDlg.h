#pragma once
#include "afxdialogex.h"
#include "TXYC_FrameList.h"
#include "TXYC_FrameThreadData.h"
#include "TXYC_RectPositionFunc.h"
#include "ProgressDlg.h"
#include "PreviewDlg.h"

// GIF 기준 프레임의 폭과 높이를 저장할 문자열의 길이
#define FRAME_SIZE_STR_LEN 32
// 파일 다이얼로그에서 한 번에 읽어들일 수 있는 파일 개수의 최대값
#define MAX_FILE_COUNT 1024

// GifAsmDlg 대화 상자
class GifAsmDlg : public CDialog
{
	DECLARE_DYNAMIC(GifAsmDlg)
private:
	TXYC_GDIP m_gdip;
	// 작업 상태 표시 다이얼로그 관리 객체
	ProgressDlg m_progress_dlg;
	// 8비트 변환된 프레임 미리보기 다이얼로그 관리 객체
	PreviewDlg m_preview_dlg;
	// 선택된 프레임의 이동 거리를 결정하기 위한 스플릿 버튼
	CSplitButton m_shift_dist_btn;
	// 프레임 삭제 방식을 결정하기 위한 스플릿 버튼
	CSplitButton m_remove_frame_btn;
	CBrush m_bk_brush; // 다이얼로그 배경색 브러시
	// 다이얼로그 배경색 브러시의 색상값
	COLORREF m_bk_color = RGB(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B);
	// 제작될 GIF 이미지를 구성하는 프레임 이미지들이 저장되어 있는 리스트 박스
	TXYC_FrameListBox m_frame_list;
	// 읽어들인 이미지들을 프레임 리스트 박스에 저장하기 전에 모아놓기 위한 벡터
	vector<FrameData> m_frame_vector;
	// 제작될 GIF 이미지를 구성하는 프레임을 그리는 출력 공간의 좌표
	RECT m_frame_viewer_rect = { 0 };
	// 프레임 출력 공간 안에 그려질 프레임 이미지의 좌표
	RECT m_frame_display_rect = { 0 };
	// 프레임 지연 시간을 조정하는 스핀 컨트롤의 1회 동작 변화량
	UINT m_delay_spin_accel = 1;
	// 10ms 단위 프레임 지연 시간의 최소값과 최대값
	UINT m_min_frame_delay = 1;
	UINT m_max_frame_delay = 200;
	UINT m_frame_delay = 5; // 10ms 단위 프레임 지연 시간
	// 제작될 GIF 이미지 기준 프레임의 폭과 높이
	UINT m_frame_cx = 0;
	UINT m_frame_cy = 0;
	// 제작될 GIF 이미지 기준 프레임의 변경 이전 폭과 높이
	UINT m_old_frame_cx = 0;
	UINT m_old_frame_cy = 0;
	// 선택된 프레임을 이동시킬 거리
	UINT m_shift_dist = 1;
	UINT m_frame_idx = 0; // 현재 재생중인 프레임의 번호
	// 제작될 GIF 이미지를 구성하는 프레임들의 재생 상태 정보(0: 정지, 1: 재생중)
	UINT8 m_play_flag = 0;
	// 제작될 GIF 이미지를 저장할 파일 경로 문자열
	wchar_t m_gif_save_path[MAX_PATH] = { 0 };
	// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 저장할 문자열
	wchar_t m_frame_size_str[FRAME_SIZE_STR_LEN] = { 0 };
	// 실제 프레임 지연 시간을 저장할 문자열
	wchar_t m_real_delay_str[16] = { 0 };
	// 다이얼로그 가이드라인과 다이얼로그 테두리 사이의 간격
	int m_dlg_guide_spacing = 0;
	// 프레임 출력 영역과 프레임 리스트 박스 사이의 간격
	int m_frame_viewer_spacing = 0;
	// 다이얼로그의 크기 변경에 맞게 프레임 리스트 박스의 크기를 조정해도 프레임 리스트 박스의
	// 높이는 고정시켜 놓기 위해 별도로 저장해두는 프레임 리스트 박스의 높이
	int m_frame_list_height = 0;
	// 선택된 프레임의 이동 거리를 결정하는 스플릿 버튼에서 선택한 메뉴의 ID를 저장하는 변수
	int m_last_shift_frame_cmd = ID_SHIFT_ONE;
	// 프레임 삭제 방식을 결정하는 스플릿 버튼에서 선택한 메뉴의 ID를 저장하기 위한 변수
	int m_last_remove_frame_cmd = ID_REMOVE_FRAME;
public:
	GifAsmDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GifAsmDlg();
	// 이미지 읽기로 추가된 프레임 리스트 박스 항목들 중 제일 먼저 추가된 항목을 선택하는 함수
	void SelectFirstInsertedItem(int a_cur_idx);
	// 프레임 이미지를 프레임 출력 공간의 정중앙에 위치시키는 함수
	void CenterFitFrameInViewer(ItemFrameData* ap_frame_data);
	// GDI+ 출력 객체와 연결된 클라이언트 영역 이미지를 갱신하는 함수
	void DrawClientImage();
	// GDI+ 출력 객체와 연결된 이미지에 그려진 현재 프레임을 화면에 출력하는 함수
	void UpdateCurrentFrame();
	// 스플릿 버튼의 메뉴 선택에 따라 스플릿 버튼의 기능을 변경하는 함수
	void SetSplitBtnCommand(int* a_last_cmd_id, int a_sub_menu_pos, CSplitButton* ap_btn);
	// 마우스 휠 동작이나 스핀 컨트롤 동작이 발생했을 때 프레임 지연 시간을 설정하는 함수
	void SetFrameDelay(int a_delta);
	// 정지된 작업 스레드를 생성한 상태로 작업 상태 표시 다이얼로그를 생성하는 함수
	void InitFrameThread(UINT8 a_post_thread_action);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIF_ASM_DLG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT On15002(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On16000(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On17000(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnSelchangeFrameList();
	afx_msg void OnBnClickedPlayFrameBtn();
	afx_msg void OnBnClickedAddFrameImageBtn();
	afx_msg void OnShiftOne();
	afx_msg void OnShiftFive();
	afx_msg void OnShiftTen();
	afx_msg void OnShiftEndpos();
	afx_msg void OnDeltaposFrameShiftSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedSetBaseFrameBtn();
	afx_msg void OnBnClickedReverseFrameOrderBtn();
	afx_msg void OnBnClickedRemoveFrameBtn();
	afx_msg void OnRemoveFrame();
	afx_msg void OnRemoveAllFrames();
	afx_msg void OnBnClickedSaveGifBtn();
	afx_msg void OnBnClickedFramePreviewBtn();
	afx_msg void OnEnChangeFrameDelayEdit();
	afx_msg void OnDeltaposFrameDelaySpin(NMHDR* pNMHDR, LRESULT* pResult);
};
