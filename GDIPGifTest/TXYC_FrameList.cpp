#include "pch.h"
#include "TXYC_ListBox.h"
#include "TXYC_FrameList.h"

TXYC_FrameListBox::TXYC_FrameListBox()
{
	// ����Ʈ �ڽ��� ����� ���� ������ ������
	m_bk_pen_color = m_select_brush_color = m_bk_brush_color = m_focus_brush_color = RGB(255, 255, 255);
	m_focus_pen_color = m_select_brush_color = RGB(172, 187, 214);
}

TXYC_FrameListBox::~TXYC_FrameListBox()
{

}

void TXYC_FrameListBox::CreateUserObject()
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

void TXYC_FrameListBox::DeleteUserObject()
{
	ResetData(); // ����Ʈ �ڽ��� ��� �׸���� ������
}

//void TXYC_FrameListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
//{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// �� �׸��� �߰� �޸𸮿� ����� �����͸� ������
//	FrameData* p_data = (FrameData*)lpDeleteItemStruct->itemData;
	// �׸��� �߰� �޸𸮿� ����� �̹������� ��� ����
//	m_gdip.DestroyImage(p_data->p_thumb_image);
//	m_gdip.DestroyImage(p_data->p_src_bitmap);
//	m_gdip.DestroyImage(p_data->p_frame_bitmap);
//	delete p_data; // �׸��� �߰� �޸� ����
//	TXYC_ListBox::DeleteItem(lpDeleteItemStruct);
//}

void TXYC_FrameListBox::UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data)
{
	ItemFrameData* p_data = (ItemFrameData*)ap_data;
	if (p_data->p_thumb_image)
	{
		GpGraphics* p_graphics = NULL;
		GdipCreateFromHDC(ah_dc, &p_graphics); // HDC�� �����ؼ� ����ϴ� GDI+ ��� ��ü ����
		// �׸� ����� �̹��� ���
		int x = ap_rect->left + m_item_padding;
		int y = ap_rect->top + m_item_padding;
		int width = m_item_width - m_item_padding * 2;
		int height = m_item_height - m_item_padding * 2;
		GdipDrawImageRectI(p_graphics, p_data->p_thumb_image, x, y, width, height);
		GdipDeleteGraphics(p_graphics); // ����ϴ� GDI+ ��� ��ü ����
	}
}

void TXYC_FrameListBox::ResetData()
{
	int count = GetCount();
	for (int i = 0; i < count; ++i)
	{
		RemoveItemData(i);
	}
	ResetContent(); // ����Ʈ �ڽ��� �����ϴ� ��� �׸� ����
}

void TXYC_FrameListBox::RemoveItemData(int a_index)
{
	// �� �׸��� �߰� �޸𸮿� ����� �����͸� ������
	ItemFrameData* p_data = (ItemFrameData*)GetItemDataPtr(a_index);
	// �׸��� �߰� �޸𸮿� ����� �̹������� ��� ����
	m_gdip.DestroyImage(p_data->p_thumb_image);
	m_gdip.DestroyImage(p_data->p_src_bitmap);
	m_gdip.DestroyImage(p_data->p_frame_bitmap);
	delete p_data; // �׸��� �߰� �޸� ����
}

int TXYC_FrameListBox::RemoveItem(int a_index)
{
	RemoveItemData(a_index);
	return DeleteString(a_index);
}

void TXYC_FrameListBox::InsertItemBitmaps(GpBitmap* ap_src_bmp, GpBitmap* ap_frame_bmp)
{
	ItemFrameData* p_data = new ItemFrameData;
	int cur_idx = AddString(L"");
	SetItemDataPtr(cur_idx, p_data);
	// ������ ����� ����� �̹��� ũ�⸦ ������
	int thumb_cx = m_item_width - m_item_padding * 2;
	int thumb_cy = m_item_height - m_item_padding * 2;
	// ����� �̹����� �����ؼ� ������
	p_data->p_thumb_image = m_gdip.CreateThumbnail(ap_src_bmp, thumb_cx, thumb_cy);
	// ���� �̹����� ������ �̹����� ������
	SetSourceBitmap(cur_idx, ap_src_bmp);
	SetFrameBitmap(cur_idx, ap_frame_bmp);
}

GpBitmap* TXYC_FrameListBox::GetSourceBitmap(int a_index)
{
	return ((ItemFrameData*)GetItemDataPtr(a_index))->p_src_bitmap;
}

GpBitmap* TXYC_FrameListBox::GetFrameBitmap(int a_index)
{
	return ((ItemFrameData*)GetItemDataPtr(a_index))->p_frame_bitmap;
}

void TXYC_FrameListBox::SetSourceBitmap(int a_index, GpBitmap* ap_src_bmp)
{
	ItemFrameData* p_data = (ItemFrameData*)GetItemDataPtr(a_index);
	p_data->p_src_bitmap = ap_src_bmp; // ���� �̹����� ������
	// ���� �̹����� ���� ���̸� ������
	GdipGetImageWidth(ap_src_bmp, &(p_data->src_cx));
	GdipGetImageHeight(ap_src_bmp, &(p_data->src_cy));
}

void TXYC_FrameListBox::SetFrameBitmap(int a_index, GpBitmap* ap_frame_bmp)
{
	ItemFrameData* p_data = (ItemFrameData*)GetItemDataPtr(a_index);
	p_data->p_frame_bitmap = ap_frame_bmp; // ������ �̹����� ������
	// ������ �̹����� ���� ���̸� ������
	GdipGetImageWidth(ap_frame_bmp, &(p_data->frame_cx));
	GdipGetImageHeight(ap_frame_bmp, &(p_data->frame_cy));
}

void TXYC_FrameListBox::SetListItemPadding(int a_item_padding)
{
	m_item_padding = a_item_padding;
}

void TXYC_FrameListBox::SetListItemWidth(int a_item_width)
{
	m_item_width = a_item_width;
	SetColumnWidth(m_item_width);
}

void TXYC_FrameListBox::SetListItemHeight(int a_item_height)
{
	m_item_height = a_item_height;
	SetItemHeight(0, m_item_height);
}
