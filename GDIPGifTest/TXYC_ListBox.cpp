// TXYC_ListBox.cpp: 구현 파일
#include "pch.h"
#include "TXYC_ListBox.h"

// TXYC_ListBox
IMPLEMENT_DYNAMIC(TXYC_ListBox, CListBox)

TXYC_ListBox::TXYC_ListBox()
{
	// 리스트 박스가 사용할 색상 정보를 구성함
	m_bk_brush_color = RGB(58, 62, 80);
	m_focus_brush_color = RGB(72, 87, 114);
	m_select_brush_color = m_focus_pen_color = m_select_pen_color = RGB(72, 87, 114);
	m_bk_pen_color = m_bk_brush_color;
}

TXYC_ListBox::~TXYC_ListBox()
{
}

BEGIN_MESSAGE_MAP(TXYC_ListBox, CListBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &TXYC_ListBox::OnLbnDblclk)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// TXYC_ListBox 메시지 처리기
void TXYC_ListBox::BaseDraw(HDC ah_dc, RECT* ap_rect)
{
	// 전체 갱신, 포커스 갱신, 선택 갱신인 경우에 처리함
	if (mp_last_lpds->itemAction & 0x07) // 0x07 = ODA_DRAWENTIRE|ODA_FOCUS|ODA_SELECT
	{
		// 색상만 변경해서 사용하는 펜과 브러시 객체를 선택함
		::SelectObject(ah_dc, ::GetStockObject(DC_BRUSH));
		::SelectObject(ah_dc, ::GetStockObject(DC_PEN));
		// 현재 선택된 상태인지 확인함
		if (mp_last_lpds->itemState & ODS_SELECTED)
		{
			// 선택된 상태의 펜 색상을 설정함
			::SetDCPenColor(ah_dc, m_select_pen_color);
			// 포커스 상태인 경우라면 조금 더 밝은 배경색을 사용하고 아니라면
			// 배경색과 동일한 색상을 사용함
			if (mp_last_lpds->itemState & ODS_FOCUS)
			{
				::SetDCBrushColor(ah_dc, m_focus_brush_color);
			}
			else
			{
				::SetDCBrushColor(ah_dc, m_select_brush_color);
			}
		}
		else
		{
			// 선택된 상태가 아니라면 배경색과 동일한 색상의 브러시와 펜을 사용함
			::SetDCPenColor(ah_dc, m_bk_pen_color);
			::SetDCBrushColor(ah_dc, m_bk_brush_color);
		}
		// 항목의 배경을 그림
		::Rectangle(ah_dc, ap_rect->left, ap_rect->top, ap_rect->right, ap_rect->bottom);
	}
}

void TXYC_ListBox::UpdateItemInfo(int a_index, int a_count)
{
	CRect r;
	if (a_count == 1)
	{
		GetItemRect(a_index, r); // 선택한 항목의 좌표를 얻음
	}
	else
	{
		CRect temp_rect;
		// 원하는 항목의 개수만큼 반복하면서 영역을 합침
		for (int i = 0; i < a_count; ++i)
		{
			GetItemRect(a_index + i, temp_rect); // 선택한 항목의 좌표를 얻음
			r.UnionRect(r, temp_rect); // r 영역에 temp_rect 영역을 합침
		}
	}
	InvalidateRect(r, FALSE); // r 영역만을 갱신함
}

void TXYC_ListBox::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 리스트 박스가 배경 채우기용으로 사용할 브러시를 생성
	mh_list_bk_brush = ::CreateSolidBrush(m_bk_brush_color);
	// 이 클래스를 상속 받은 클래스에서 리스트 박스 생성 시에 함께 작업할 내용이 있다면
	// 이 함수를 오버라이딩해서 사용함
	CreateUserObject();
	CListBox::PreSubclassWindow();
}

HBRUSH TXYC_ListBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  여기서 DC의 특성을 변경합니다.
	// TODO:  부모 처리기가 호출되지 않을 경우 Null이 아닌 브러시를 반환합니다.
	return mh_list_bk_brush;
}

void TXYC_ListBox::DrawItem(LPDRAWITEMSTRUCT lpds)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 각 함수의 인자로 넘어갈 정보들을 멤버로 백업해서 사용함
	mp_last_lpds = lpds;
	// 유효한 항목의 인덱스인지 확인함
	if ((UINT)lpds->itemID < (UINT)GetCount())
	{
		BaseDraw(lpds->hDC, &lpds->rcItem); // 각 항목의 배경을 그림
		// 선택한 항목의 정보를 출력함
		UserDraw(lpds->hDC, &lpds->rcItem, (void*)lpds->itemData);
	}
}

int TXYC_ListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 리스트 박스가 배경 채우기용으로 사용할 브러시를 생성
	mh_list_bk_brush = ::CreateSolidBrush(m_bk_brush_color);
	// 이 클래스를 상속 받은 클래스에서 리스트 박스 생성 시에 함께 작업할 내용이 있다면
	// 이 함수를 오버라이딩해서 사용함
	CreateUserObject();
	return 0;
}

void TXYC_ListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 이 클래스를 상속 받은 클래스에서 리스트 박스 제거 시에 함께 작업할 내용이 있다면
	// 이 함수를 오버라이딩해서 사용함
	DeleteUserObject();
	// 리스트 박스가 배경 채우기용으로 사용했던 브러시 제거
	::DeleteObject(mh_list_bk_brush);
}

void TXYC_ListBox::OnLbnDblclk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL out_side_flag = FALSE;
	// 마우스가 현재 가리키고 있는 항목의 인덱스 값을 구함(마우스가 현재 가리키고 있는 항목이 없을
	// 경우에는 ItemFromPoint() 함수가 리스트 박스의 항목 개수를 반환하며, ItemFromPoint() 함수가
	// 리스트 박스의 항목 개수를 반환했을 때 ItemFromPoint() 함수의 두 번째 인자로 넘겨준 BOOL 값은
	// 1이 되기 때문에 마우스가 현재 가리키고 있는 항목이 없는지 확인할 때에는 ItemFromPoint() 함수의
	// 반환값과 ItemFromPoint() 함수의 두 번째 인자로 넘겨준 BOOL 값을 모두 확인하는 것이 안전함)
	UINT index = ItemFromPoint(m_last_dbclk_pos, out_side_flag);
	int check_index = GetCurSel();
	if (!out_side_flag && index == check_index)
	{
		// index 위치가 유효할 경우에만 더블 클릭을 수행함
		ProcessDoubleClicked(check_index);
	}
}

void TXYC_ListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 리스트 박스를 마우스로 더블 클릭했을 때의 좌표를 기억함
	m_last_dbclk_pos = point;
	CListBox::OnLButtonDblClk(nFlags, point);
}
