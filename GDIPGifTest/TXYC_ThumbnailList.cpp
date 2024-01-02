#include "pch.h"
#include "TXYC_ListBox.h"
#include "TXYC_ThumbnailList.h"

TXYC_ThumbnailListBox::TXYC_ThumbnailListBox()
{
	// ����Ʈ �ڽ��� ����� ���� ������ ������
	m_bk_pen_color = m_select_brush_color = m_bk_brush_color = m_focus_brush_color = RGB(255, 255, 255);
	m_focus_pen_color = m_select_brush_color = RGB(172, 187, 214);
}

TXYC_ThumbnailListBox::~TXYC_ThumbnailListBox()
{

}

void TXYC_ThumbnailListBox::CreateUserObject()
{
	UINT dpi = ::GetDpiForWindow(m_hWnd);
	// ������� ȭ�� ������ �°� ����� ����Ʈ �ڽ� �׸��� ���� ����, ������ ������
	m_item_width = (int)((double)THUMB_LIST_ITEM_WIDTH * dpi / 96.0);
	m_item_height = (int)((double)THUMB_LIST_ITEM_HEIGHT * dpi / 96.0);
	m_item_padding = (int)((double)THUMB_LIST_ITEM_PADDING * dpi / 96.0);
	// ����Ʈ �ڽ� �׸� �� ���� ũ�⸦ ������
	SetItemHeight(0, m_item_height);
	SetColumnWidth(m_item_width);
	m_gdip.CreateGDIP(210, 210); // GDI+ ��� ��ü ����
}

void TXYC_ThumbnailListBox::DeleteUserObject()
{
	ResetContent(); // ����Ʈ �ڽ��� ��� �׸���� ������
}

void TXYC_ThumbnailListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// �� �׸��� �߰� �޸𸮿� ����� �����͸� ������
	ItemThumbData* p_data = (ItemThumbData*)lpDeleteItemStruct->itemData;
	// �׸� ����� ����� �̹��� ����
	m_gdip.DestroyImage(p_data->p_image);
	delete p_data; // �׸��� �߰� �޸� ����
	TXYC_ListBox::DeleteItem(lpDeleteItemStruct);
}

void TXYC_ThumbnailListBox::UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data)
{
	ItemThumbData* p_data = (ItemThumbData*)ap_data;
	if (p_data->p_image)
	{
		GpGraphics* p_graphics = NULL;
		GdipCreateFromHDC(ah_dc, &p_graphics); // HDC�� �����ؼ� ����ϴ� GDI+ ��� ��ü ����
		// �׸� ����� �̹��� ���
		int x = ap_rect->left + m_item_padding;
		int y = ap_rect->top + m_item_padding;
		int width = m_item_width - m_item_padding * 2;
		int height = m_item_height - m_item_padding * 2;
		GdipDrawImageRectI(p_graphics, p_data->p_image, x, y, width, height);
		GdipDeleteGraphics(p_graphics); // ����ϴ� GDI+ ��� ��ü ����
	}
}

void TXYC_ThumbnailListBox::InsertThumbnailImage(GpImage* ap_image)
{
	ItemThumbData* p_data = new ItemThumbData;
	// ������ ����� ����� �̹��� ũ�⸦ ������
	int thumb_cx = m_item_width - m_item_padding * 2;
	int thumb_cy = m_item_height - m_item_padding * 2;
	// ����� �̹����� �����ؼ� ������
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
