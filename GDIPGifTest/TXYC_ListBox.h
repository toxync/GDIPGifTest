#ifndef _TXYC_LIST_BOX_H_
#define _TXYC_LIST_BOX_H_

// 클래스 작성자: 박정현(bac6536@naver.com)
// 이 클래스는 다른 개발자가 작성한 클래스를 기반으로 작성되었음
// 원본 클래스 작성자: 김성엽(tipsware@naver.com)
// 원본 클래스 배포 주소: https://cafe.naver.com/mythread

// TXYC_ListBox
class TXYC_ListBox : public CListBox
{
	DECLARE_DYNAMIC(TXYC_ListBox)
protected:
	HBRUSH mh_list_bk_brush; // 리스트 박스 배경색으로 사용할 브러시 객체
	// 상황별 브러시 색상을 기억할 변수들
	COLORREF m_select_brush_color, m_focus_brush_color, m_bk_brush_color;
	// 상황별 펜 색상을 기억할 변수들
	COLORREF m_select_pen_color, m_focus_pen_color, m_bk_pen_color;
	// 현재 처리중인 Owner-Draw 구조체 정보(매개 변수로 계속 넘기지 않기 위해 구조체 주소를
	// 보관해서 사용, 나중에 사용하면 주소가 유효하지 않을 수 있으니 주의해야 함)
	LPDRAWITEMSTRUCT mp_last_lpds;
	CPoint m_last_dbclk_pos; // 리스트 박스를 마우스로 더블 클릭했을 때의 좌표
public:
	TXYC_ListBox();
	virtual ~TXYC_ListBox();
	// WM_CREATE 메시지를 사용하지 않고 리스트 박스가 생성될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void CreateUserObject() {}
	// WM_DESTROY 메시지를 사용하지 않고 리스트 박스가 제거될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void DeleteUserObject() {}
	// 각 항목의 배경을 그리기 위해 사용할 함수(기본 코드 존재)
	virtual void BaseDraw(HDC ah_dc, RECT* ap_rect);
	// 각 항목에 출력될 정보를 그리는 함수
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data) {}
	// 리스트 박스의 특정 항목 위치만 갱신하는 함수
	void UpdateItemInfo(int a_index, int a_count);
	// 항목을 더블 클릭했을 때 호출되는 함수
	virtual void ProcessDoubleClicked(int a_index) {}
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#endif // !_TXYC_LIST_BOX_H_