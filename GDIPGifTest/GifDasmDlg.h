#pragma once
#include "afxdialogex.h"
#include "TXYC_ThumbnailList.h"
#include "TXYC_RectPositionFunc.h"

// GifDasmDlg 대화 상자
class GifDasmDlg : public CDialog
{
	DECLARE_DYNAMIC(GifDasmDlg)
private:
	TXYC_GDIP m_gdip;
	TXYC_ThumbnailListBox m_thumbnail_list;
	CSplitButton m_save_frame_btn; // 프레임 저장 방식을 결정하기 위한 스플릿 버튼
	GpImage* mp_gif_image = NULL; // 읽어들인 GIF 이미지 객체의 주소
	GUID* mp_id_list = NULL; // 읽어들인 GIF 이미지의 첫 번째 프레임 GUID 목록
	PropertyItem* mp_item = NULL; // 프레임의 속성 정보
	// 프레임 지연 시간이 저장된 목록의 주소
	UINT* mp_delay_list;
	// 첫 프레임 그룹에 포함된 프레임 수, 현재 재생중인 프레임의 번호, 읽어들인 GIF 이미지의 폭과 높이
	UINT m_frame_count = 0, m_frame_index = 0, m_gif_width = 0, m_gif_height = 0;
	// GIF 이미지의 재생 상태 정보(0: 정지, 1: 재생중)
	UINT8 m_play_flag = 0;
	RECT m_frame_viewer_rect = { 0 }; // 읽어들인 GIF 이미지의 프레임을 그리는 출력 공간의 좌표
	RECT m_gif_display_rect = { 0 }; // 프레임 출력 공간 안에 그려질 프레임 이미지의 좌표
	// 읽어들인 GIF 이미지의 확장자와 파일 경로를 제외한 파일 이름 문자열
	wchar_t m_gif_title[MAX_PATH] = { 0 };
	// 다이얼로그 가이드라인과 다이얼로그 테두리 사이의 간격
	int m_dlg_guide_spacing = 0;
	// 프레임 출력 영역과 썸네일 리스트 박스 사이의 간격
	int m_frame_viewer_spacing = 0;
	// 다이얼로그의 크기 변경에 맞게 썸네일 리스트 박스의 크기를 조정해도 썸네일 리스트 박스의 높이는
	// 고정시켜 놓기 위해 별도로 저장해두는 썸네일 리스트 박스의 높이
	int m_thumb_list_height = 0;
	// 프레임 저장 방식을 결정하는 스플릿 버튼에서 선택한 메뉴의 ID를 기억하기 위한 변수
	int m_last_save_cmd = ID_SAVE_FRAME;
public:
	GifDasmDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GifDasmDlg();
	void LoadGif(const wchar_t* ap_path); // GIF 이미지를 읽어오는 함수
	void DestroyGif(); // 읽어들인 GIF 이미지를 제거하는 함수
	// GDI+ 출력 객체와 연결된 클라이언트 영역 이미지를 갱신하는 함수
	void DrawClientImage();
	// GDI+ 출력 객체와 연결된 이미지에 그려진 현재 프레임을 화면에 출력하는 함수
	void UpdateCurrentFrame();
	// 프레임 저장 방식을 결정하는 스플릿 버튼의 메뉴 선택에 따라 읽어들인 GIF 이미지의 프레임
	// 저장 방식을 변경하는 함수
	void SetFrameSaveCommand();
	void SaveFrame(); // 읽어들인 GIF 이미지의 프레임 하나를 저장하는 함수
	void SaveAllFrames(); // 읽어들인 GIF 이미지의 모든 프레임을 저장하는 함수
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIF_DASM_DLG };
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnSelchangeThumbnailList();
	afx_msg void OnBnClickedPlayGifBtn();
	afx_msg void OnBnClickedSaveFrameBtn();
	afx_msg void OnSaveFrame();
	afx_msg void OnSaveAllFrames();
	afx_msg void OnBnClickedFindGifBtn();
};
