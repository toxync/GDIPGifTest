#ifndef _TXYC_CUSTOM_CAPTION_H_
#define _TXYC_CUSTOM_CAPTION_H_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)

#include "TXYC_GDIP.h"
#include "TXYC_CaptionControl.h"
#include <vector>

// ��¥ ��� �׵θ� ����(��¥ ��� �׵θ��� �׸��� �ʴ´ٸ� �� ���� 0���� ����)
#define FAKE_TOP_BORDER_HEIGHT 0
// ĸ�� ������ ���Ʒ� ���� 1�ȼ���ŭ�� �׵θ��� �׸��� ���� ��
#define CAPTION_TOPBOTTOM_BORDERS 2
// �����찡 ��üȭ�� ������ �� �ִ�ȭ ��ư �������� �簢�� 2�� ������ ����
#define MAXIMIZE_BTN_RECT_OFFSET 2
// ĸ�� ��ư�� ���� ����
#define CAPTION_BTN_WIDTH 48
#define CAPTION_BTN_HEIGHT 32
#define TOP_FRAME_HEIGHT 6 // ĸ���� ��� ������ ����
#define TOP_FRAME_SPACING 6 // ĸ���� ��� �����Ӱ� ĸ�� �� ������ ���� ����
#define CAPTION_TAB_WIDTH 100 // ĸ�� �� 1���� ��
#define MAX_CAPTION_TAB_COUNT 3 // ĸ�� ���� �ִ� ����
// ĸ�� ���� �̸� ���ڿ��� ��µ� ��� ������ ĸ�� �� ���� ������ ����
#define CAPTION_TAB_TEXT_SPACING 4
#define CAPTION_TAB_FONT 12 // ĸ�� ���� �̸� ���ڿ� ��Ʈ ũ��

using std::vector;

// ����� ���� ĸ�ǿ� ���� ��ǥ ������
typedef struct _CaptionRects
{
	RECT Caption; // ĸ�� ������ ��ǥ ����
	RECT CaptionTab; // ��ü ĸ�� �� ����(��� ĸ�� �ǵ��� �� ��ģ ����)�� ��ǥ ����
	RECT TopFrame; // ��� ������ ������ ��ǥ ����
	RECT MinimizeBtn, MaximizeBtn, CloseBtn; // ĸ�� ��ư��(�ּ�ȭ, �ִ�ȭ, �ݱ�)�� ��ǥ ����
} CaptionRects;

// ����� ���� ĸ�ǿ� ���� ���� ������
typedef struct _CaptionColors
{
	ARGB BgColor; // ������ ��� ����
	// ������ Ȱ��ȭ ���¿� ���� ����� ĸ�� ä��� �����
	ARGB ActiveFillColor, InactiveFillColor;
	// ������ Ȱ��ȭ ���¿� ���� ����� ĸ�� ��ư ������ �����
	ARGB ActiveItemColor, InactiveItemColor;
	// ������ Ȱ��ȭ ���¿� ���� ����� ȣ������ ĸ�� ��ư �����
	ARGB ActiveBtnHoverColor, InactiveBtnHoverColor;
	ARGB ActiveBtnClickColor; // ĸ�� ��ư Ŭ�� ����
	// �ݱ� ��ư ���� ȣ������ ĸ�� ��ư ����� Ŭ���� ĸ�� ��ư ����
	ARGB CloseBtnHoverColor, CloseBtnClickColor;
	ARGB CloseBtnItemColor; // �ݱ� ��ư ���� ĸ�� ��ư ������ ����
	ARGB SelectedTabColor; // ���õ� ĸ�� �� ����
	ARGB HoveredTabColor; // ȣ������ ĸ�� �� ����
	ARGB UnselectedTabColor; // ���õ��� ���� ĸ�� �� ����
} CaptionColors;

// �ϳ��� ĸ�� ���� ������ �ִ� ������
typedef struct _CaptionTabData
{
	CDialog* p_TabDlg; // �� ĸ�� �ǰ� ����� ���̾�α��� �ּ�
	wchar_t TabText[64]; // �� ĸ�� ���� �̸� ���ڿ�
	RectF TextArea; // ĸ�� ���� �̸� ���ڿ��� ��µ� ��� ����(���� ĸ�� ���� ũ�⺸�� ���� ����)
} CaptionTabData;

// ���콺�� ����ų �� �ִ� ĸ�� ��ư�� ����
typedef enum _SelectedCaptionBtn
{
	SelectedBtn_None,
	SelectedBtn_Minimize,
	SelectedBtn_Maximize,
	SelectedBtn_Close
} SelectedCaptionBtn;

// ���콺�� Ŭ���� �� �ִ� Ŭ���̾�Ʈ ������ ����
typedef enum _ClickedArea
{
	Clicked_None,
	Clicked_CaptionBtn,
	Clicked_CaptionTab,
	Clicked_ClientArea
} ClickedArea;

// ����� ���� ĸ�ǿ� ���� ���� �������� ����Ű�� ���� �ε���
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
	TXYC_GDIP m_gdip; // GDI+ ����� ���� ��ü
	UINT m_dpi = 0; // ������� ȭ�� ������ �°� ĸ���� ũ�⸦ �����ϱ� ���� ����� DPI ��
	RECT m_client_rect = { 0 }; // ĸ�Ǳ��� Ȯ��� Ŭ���̾�Ʈ ������ ��ǥ ����
	RECT m_icon_rect = { 0 }; // ĸ�� ��ư �������� �׸� �� ����� �簢�� ����
	CWnd* mp_wnd = NULL; // ����� ���� ĸ���� ����� �������� �ּ�
	CaptionRects m_cptn_rects = { {0} }; // ����� ���� ĸ�ǿ� ����� ��ǥ ������
	// WM_INPUT �޽��� ó�� �Լ����� ����� ������
	RAWINPUT m_rawinput = { 0 };
	UINT m_nSize = sizeof(RAWINPUT);
	// ���콺�� ���� ȣ�������� ĸ�� ��ư
	SelectedCaptionBtn m_hovered_btn = SelectedBtn_None;
	// ���콺�� ������ ȣ�����ߴ� ĸ�� ��ư
	SelectedCaptionBtn m_old_hovered_btn = SelectedBtn_None;
	// ���콺�� Ŭ���� ĸ�� ��ư
	SelectedCaptionBtn m_clicked_btn = SelectedBtn_None;
	// ����� ������ ĸ�� ��ư
	SelectedCaptionBtn m_triggered_btn = SelectedBtn_None;
	ClickedArea m_is_clicked = Clicked_None; // ���� Ŭ���� Ŭ���̾�Ʈ ������ ����
	CaptionColors m_cptn_colors = { 0 }; // ĸ�� ������ �׸� �� ����� �����
	// CaptionTabData m_tab_data[MAX_CAPTION_TAB_COUNT] = { 0 }; // �� ĸ�� �ǿ� �ʿ��� �����͵�
	vector<CaptionTabData> m_tab_data; // �� ĸ�� �ǿ� �ʿ��� �����͵�
	UINT8 m_active_state = 0; // �������� Ȱ��ȭ ���� ����
	UINT8 m_nc_hover_state = 0; // ��Ŭ���̾�Ʈ ���� ȣ���� ���� ����
	UINT8 m_hover_state = 0; // Ŭ���̾�Ʈ ���� ȣ���� ���� ����
	UINT m_icon_dimension = 0; // ĸ�� ��ư �������� ġ�� ����
	UINT m_top_border_height = 0; // ĸ���� ��� �׵θ� ����
	UINT m_btn_width = 0, m_btn_height = 0; // ĸ�� ��ư�� ���� ����
	UINT m_tab_width = 0, m_tab_height = 0; // ĸ�� ���� ���� ����
	UINT8 m_tab_count = 0; // ĸ�� ���� ����
	UINT8 m_selected_tab = 0; // ���� ���õ� ĸ�� ��
	int m_hovered_tab = -1; // ���� ȣ������ ĸ�� ��
	int m_old_hovered_tab = -1; // ������ ȣ�����ߴ� ĸ�� ��
public:
	TXYC_CustomCaption(CWnd* ap_wnd);
	// ù ��° �Ű� ������ �־��� ���� �� ��° �Ű� ������ ����� DPI ���� �°� �����Ű�� �Լ�
	UINT GetDPIScale(UINT a_value, UINT a_dpi);
	// ��¥ ��� �׵θ��� ��ǥ�� �����ϴ� �Լ�
	void SetFakeWndTopBorderRect(RECT* ap_fake_top_border_rect);
	void SetCaptionRectPos(); // ����� ���� ĸ�ǿ� �ʿ��� �簢�� �������� ��ǥ�� �����ϴ� �Լ�
	void SetCaptionTabPos(); // ĸ�� �ǵ�� ĸ�� �� ���̾�α׵��� ��ǥ�� �����ϴ� �Լ�
	BOOL IsWindowMaximized(); // �����찡 �ִ�ȭ�� �������� Ȯ���ϱ� ���� �Լ�
	// ù ��° �Ű� ������ �־��� �簢�� ������ ��ǥ�� �� ��° �Ű� ������ �־��� �簢�� ������ ��ǥ
	// ���� ���߾ӿ� ��ġ��Ű�� �Լ�
	void CenterRectInRect(RECT* ap_center_rect, const RECT* ap_outer_rect);
	// �����찡 �ִ�ȭ�� ������ ���� ĸ�Ǳ��� Ȯ��� Ŭ���̾�Ʈ ������ ���ϴ� �Լ�
	void AdjustMaximizedClientRect(RECT* ap_rect);
	// �����찡 �ִ�ȭ�� ���°� �ƴ� ���� ĸ�Ǳ��� Ȯ��� Ŭ���̾�Ʈ ������ ��ǥ�� �����ϴ� �Լ�
	void AdjustUnMaximizedClientRect(RECT* ap_rect);
	void StartEventCapture(); // WM_INPUT �޽��� ó�� �Լ��� ���� ���콺 ����͸� ���� �Լ�
	void EndEventCapture(); // ���콺 ����͸� ���� �Լ�
	// GDI+ ��� ��ü�� ����� ����� ���� ĸ�� �̹����� �����ϴ� �Լ�
	void DrawCaptionImage();
	// �߰��� ĸ�� �ǿ� ������ ���̾�α׸� �����ϴ� �Լ�
	void AddCaptionTabDialog(CDialog* ap_dlg, const wchar_t* ap_text, int a_dlg_id);
	// ĸ�� �ǵ��� ���� �غ� ���¸� ��ȯ�ϴ� �Լ�
	UINT8 GetCloseReadyStatus();
	// ����� ���� ĸ�� ���� ����ϴ� �����찡 ���� �� �� ĸ�� �ǰ� ����� ���̾�α׸� �����ϴ� �Լ�
	void DestroyCaptionTabDialogs();
	// ����� ���� ĸ�ǰ� ����� ���̾�αװ� ����� ���� ĸ�ǿ��� ĸ�� �� ���̾�α׿��� ���콺 Ŭ����
	// �߻������� �˸� �� ����ϴ� �Լ�
	void NotifyTabWndLBtnDown();
	// ����� ���� ĸ�ǰ� ����� ���̾�αװ� ����� ���� ĸ�ǿ��� ���� ĸ�� ���� ���� ���� ���õ�
	// ĸ�� ������ ��������� �˸� �� ����ϴ� �Լ�
	void NotifyNextTabWnd();
	// ����� ���� ĸ�ǰ� ����� ���̾�αװ� ����� ���� ĸ�ǿ��� ���� ĸ�� ���� ���� ���� ���õ�
	// ĸ�� ������ ��������� �˸� �� ����ϴ� �Լ�
	void NotifyPrevTabWnd();
	// ����� ���� ĸ�ǰ� ����� ���̾�α��� �Լ����� ȣ���ϱ� ���� �Լ���
	// OnInitDialog() �Լ����� ȣ���ϴ� �Լ�
	void OnInitCaption();
	// WM_CREATE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnCreateCaption();
	// WM_ACTIVATE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnActivateCaption(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// WM_NCCALCSIZE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	UINT OnNcCalcSizeCaption(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	// WM_NCHITTEST �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	LRESULT OnNcHitTestCaption(CPoint point);
	// WM_SETCURSOR �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	BOOL OnSetCursorCaption(CWnd* pWnd, UINT nHitTest, UINT message);
	// WM_NCMOUSEMOVE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnNcMouseMoveCaption(UINT nHitTest, CPoint point);
	// WM_NCMOUSELEAVE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnNcMouseLeaveCaption();
	// WM_SIZE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnSizeCaption(UINT nType, int cx, int cy);
	// WM_INPUT �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnRawInputCaption(UINT nInputcode, HRAWINPUT hRawInput);
	// WM_LBUTTONDOWN �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	UINT OnLBtnDownCaption(UINT nFlags, CPoint point);
	// WM_LBUTTONUP �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	UINT OnLBtnUpCaption(UINT nFlags, CPoint point);
	// WM_MOUSEMOVE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnMouseMoveCaption(UINT nFlags, CPoint point);
	// WM_MOUSELEAVE �޽��� ó�� �Լ����� ȣ���ϴ� �Լ�
	void OnMouseLeaveCaption();
};

#endif // !_TXYC_CUSTOM_CAPTION_H_