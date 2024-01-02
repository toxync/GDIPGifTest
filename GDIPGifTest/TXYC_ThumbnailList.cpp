#include "pch.h"
#include "TXYC_ListBox.h"
#include "TXYC_ThumbnailList.h"

TXYC_ThumbnailListBox::TXYC_ThumbnailListBox()
{
	// 리스트 박스가 사용할 색상 정보를 구성함
	m_bk_pen_color = m_select_brush_color = m_bk_brush_color = m_focus_brush_color = RGB(255, 255, 255);
	m_focus_pen_color = m_select_brush_color = RGB(172, 187, 214);
}

TXYC_ThumbnailListBox::~TXYC_ThumbnailListBox()
{

}

void TXYC_ThumbnailListBox::CreateUserObject()
{
	UINT dpi = ::GetDpiForWindow(m_hWnd);
	// 모니터의 화면 배율에 맞게 썸네일 리스트 박스 항목의 폭과 높이, 여백을 설정함
	m_item_width = (int)((double)THUMB_LIST_ITEM_WIDTH * dpi / 96.0);
	m_item_height = (int)((double)THUMB_LIST_ITEM_HEIGHT * dpi / 96.0);
	m_item_padding = (int)((double)THUMB_LIST_ITEM_PADDING * dpi / 96.0);
	// 리스트 박스 항목 한 개의 크기를 설정함
	SetItemHeight(0, m_item_height);
	SetColumnWidth(m_item_width);
	m_gdip.CreateGDIP(210, 210); // GDI+ 출력 객체 생성
}

void TXYC_ThumbnailListBox::DeleteUserObject()
{
	ResetContent(); // 리스트 박스의 모든 항목들을 제거함
}

void TXYC_ThumbnailListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 각 항목의 추가 메모리에 저장된 데이터를 가져옴
	ItemThumbData* p_data = (ItemThumbData*)lpDeleteItemStruct->itemData;
	// 항목에 저장된 썸네일 이미지 제거
	m_gdip.DestroyImage(p_data->p_image);
	delete p_data; // 항목의 추가 메모리 제거
	TXYC_ListBox::DeleteItem(lpDeleteItemStruct);
}

void TXYC_ThumbnailListBox::UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data)
{
	ItemThumbData* p_data = (ItemThumbData*)ap_data;
	if (p_data->p_image)
	{
		GpGraphics* p_graphics = NULL;
		GdipCreateFromHDC(ah_dc, &p_graphics); // HDC에 연결해서 사용하는 GDI+ 출력 객체 생성
		// 항목에 썸네일 이미지 출력
		int x = ap_rect->left + m_item_padding;
		int y = ap_rect->top + m_item_padding;
		int width = m_item_width - m_item_padding * 2;
		int height = m_item_height - m_item_padding * 2;
		GdipDrawImageRectI(p_graphics, p_data->p_image, x, y, width, height);
		GdipDeleteGraphics(p_graphics); // 사용하던 GDI+ 출력 객체 제거
	}
}

void TXYC_ThumbnailListBox::InsertThumbnailImage(GpImage* ap_image)
{
	ItemThumbData* p_data = new ItemThumbData;
	// 여백이 적용된 썸네일 이미지 크기를 설정함
	int thumb_cx = m_item_width - m_item_padding * 2;
	int thumb_cy = m_item_height - m_item_padding * 2;
	// 썸네일 이미지를 생성해서 저장함
	p_data->p_image = m_gdip.CreateThumbnail(ap_image, thumb_cx, thumb_cy);
	SetItemDataPtr(AddString(L""), p_data);
}

void TXYC_ThumbnailListBox::SetListItemPadding(int a_item_padding)
{
	m_item_padding = a_item_padding;
}

void TXYC_ThumbnailListBox::SetListItemWidth(int a_item_width)
{
	m_item_width = a_item_width;
	SetColumnWidth(m_item_width);
}

void TXYC_ThumbnailListBox::SetListItemHeight(int a_item_height)
{
	m_item_height = a_item_height;
	SetItemHeight(0, m_item_height);
}
