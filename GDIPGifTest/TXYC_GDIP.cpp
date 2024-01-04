#include "pch.h"
#include "TXYC_GDIP.h"

// static ��� ���� �ʱ�ȭ
UINT TXYC_GDIP::m_object_count = 0;
ULONG_PTR TXYC_GDIP::m_token = 0;

TXYC_GDIP::TXYC_GDIP()
{
	Init();
}

TXYC_GDIP::TXYC_GDIP(int a_cx, int a_cy)
{
	Init();
	CreateGDIP(a_cx, a_cy);
}

TXYC_GDIP::~TXYC_GDIP()
{
	if (mp_stock_pen)
	{
		GdipDeletePen(mp_stock_pen); // ����ϴ� �� ��ü ����
		GdipDeleteBrush(mp_stock_brush); // ����ϴ� ä��� �귯�� ��ü ����
		GdipDeleteBrush(mp_stock_font_brush); // ����ϴ� �۲� �귯�� ��ü ����
		GdipDeleteFont(mp_stock_font); // ����ϴ� �۲� ��ü ����
		GdipDeleteFontFamily(mp_stock_font_family); // ����ϴ� �۲� ���� ��ü ����
		GdipDeleteStringFormat(mp_stock_str_format); // ����ϴ� StringFormat ��ü ����
	}
	GdipDeleteGraphics(mp_graphics); // ����ϴ� ��Ʈ�� ��� GDI+ ��ü ����
	GdipDisposeImage(mp_bitmap); // ����ϴ� ��Ʈ�� ��ü ����
	--m_object_count; // GDIP ��ü ���� ����
	// �� ��ü�� ������ GDIP ��ü��� GDI+ ��� ����
	if (!m_object_count)
	{
		GdiplusShutdown(m_token);
	}
}

void TXYC_GDIP::Init()
{
	if (!m_object_count) // �� ��ü�� ù GDIP ��ü��� GDI+ �ʱ�ȭ
	{
		GdiplusStartupInput gpsi;
		GdiplusStartup(&m_token, &gpsi, NULL);
	}
	++m_object_count; // GDIP ��ü ���� ����
	m_cx = m_cy = 0; // GDI+ ��¿� ��Ʈ���� ũ�� ����
	mp_bitmap = NULL; // ��µ� �̹����� ������ ��Ʈ�� ��ü
	mp_graphics = NULL; // ��Ʈ�ʿ� ����� GDI+ ��� ��ü
	// 0xFF000000 ������ ���� �۲� �귯�� ��ü ����
	GdipCreateSolidFill(0xFF000000, &mp_stock_font_brush);
	// 0xFFFFFFFF ������ ���� ä��� �귯�� ��ü ����
	GdipCreateSolidFill(0xFFFFFFFF, &mp_stock_brush);
	// 0xFF000000 ������ ������ ���Ⱑ 1�̸� �Ǽ� �Ӽ��� ���� �� ��ü ����
	GdipCreatePen1(0xFF000000, 1, UnitWorld, &mp_stock_pen);
	// '���� ���' �۲� ������ �����ϴ� ��ü ����
	GdipCreateFontFamilyFromName(L"���� ���", NULL, &mp_stock_font_family);
	// �۲��� ũ�Ⱑ 15�̰� Ư���� �Ӽ��� ���� �۲� ����
	GdipCreateFont(mp_stock_font_family, 15, FontStyleRegular, UnitPixel, &mp_stock_font);
	// ���ڿ��� �����ؼ� ����ϱ� ���� StringFormat ��ü ����
	GdipCreateStringFormat(0, LANG_KOREAN, &mp_stock_str_format);
}

int TXYC_GDIP::CreateGDIP(int a_cx, int a_cy)
{
	// ��� ��ü�� �����Ǿ� ���� �ʰų� ���� ���̰� �޶����߸� ��� ��ü�� ������
	if (mp_graphics == NULL || m_cx != a_cx || m_cy != a_cy)
	{
		// ���ο� ���� ���̷� ����
		m_cx = a_cx;
		m_cy = a_cy;
		// ������ ����ϴ� ��ü���� ����
		if (mp_graphics != NULL)
		{
			GdipDeleteGraphics(mp_graphics); // ����ϴ� ��Ʈ�� ��� GDI+ ��� ��ü ����
		}
		if (mp_bitmap != NULL)
		{
			GdipDisposeImage(mp_bitmap); // ����ϴ� ��Ʈ�� ��ü ����
		}
		// ���ο� ũ�⿡ �°� ��Ʈ�� ��ü ����
		GdipCreateBitmapFromScan0(a_cx, a_cy, 0, PixelFormat32bppARGB, NULL, &mp_bitmap);
		// ��Ʈ�� ��ü�� �����ؼ� ����ϴ� GDI+ ��� ��ü ����
		GdipGetImageGraphicsContext(mp_bitmap, &mp_graphics);
		// 'AntiAlias'�� ����ϵ��� ����(��� ������ SmoothingModeInvalid)
		GdipSetSmoothingMode(mp_graphics, SmoothingModeAntiAlias);
		GdipSetTextRenderingHint(mp_graphics, TextRenderingHintAntiAliasGridFit);
		return 1;
	}
	return 0;
}

void TXYC_GDIP::Clear(ARGB a_color)
{
	GdipGraphicsClear(mp_graphics, a_color); // ������ �������� ��ü ��� ������ ä��
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // �簢�� ���� ä���
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_brush, a_color); // �귯�� ������ ������ �������� ����
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // �簢�� ���� ä���
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy, GpBrush* ap_brush)
{
	GdipFillRectangleI(mp_graphics, ap_brush, a_x, a_y, a_cx, a_cy); // �簢�� ���� ä���
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // �簢�� �׵θ� �׸���
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // �� ������ ������ �������� ����
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // �簢�� �׵θ� �׸���
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy, GpPen* ap_pen)
{
	GdipDrawRectangleI(mp_graphics, ap_pen, a_x, a_y, a_cx, a_cy); // �簢�� �׵θ� �׸���
}

void TXYC_GDIP::Rectangle(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // �簢�� ���� ä���
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // �簢�� �׵θ� �׸���
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey)
{
	GdipDrawLineI(mp_graphics, mp_stock_pen, a_sx, a_sy, a_ex, a_ey); // �� �׸���
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // �� ������ ������ �������� ����
	GdipDrawLineI(mp_graphics, mp_stock_pen, a_sx, a_sy, a_ex, a_ey); // �� �׸���
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, GpPen* ap_pen)
{
	GdipDrawLineI(mp_graphics, ap_pen, a_sx, a_sy, a_ex, a_ey); // �� �׸���
}

void TXYC_GDIP::TextOut(int a_x, int a_y, const wchar_t* ap_str, int a_str_len)
{
	RectF rect((REAL)a_x, (REAL)a_y, 0, 0); // ���ڿ� ��� ��ġ�� �簢������ ����
	// mp_stock_font_brush�� ������ ��ġ�� ���ڿ��� ���
	GdipDrawString(mp_graphics, ap_str, a_str_len, mp_stock_font, &rect, NULL, mp_stock_font_brush);
}

void TXYC_GDIP::TextOut(GpFont* ap_font, int a_x, int a_y, const wchar_t* ap_str, int a_str_len)
{
	RectF rect((REAL)a_x, (REAL)a_y, 0, 0); // ���ڿ� ��� ��ġ�� �簢������ ����
	// mp_stock_font_brush�� ������ ��ġ�� ���ڿ��� ���
	GdipDrawString(mp_graphics, ap_str, a_str_len, ap_font, &rect, NULL, mp_stock_font_brush);
}

void TXYC_GDIP::DrawText(RectF* ap_rect, const wchar_t* ap_str, int a_str_len,
	StringAlignment a_x_align, StringAlignment a_y_align)
{
	// ���� ����(x��) ����: ����, ���, ������
	GdipSetStringFormatAlign(mp_stock_str_format, a_x_align);
	// ���� ����(y��) ����: ����, ���, �Ʒ���
	GdipSetStringFormatLineAlign(mp_stock_str_format, a_y_align);
	// mp_stock_font_brush�� ������ ��ġ�� ���ڿ��� ���
	GdipDrawString(mp_graphics, ap_str, a_str_len, mp_stock_font, ap_rect, mp_stock_str_format, mp_stock_font_brush);
}

void TXYC_GDIP::DrawText(GpFont* ap_font, RectF* ap_rect, const wchar_t* ap_str, int a_str_len,
	StringAlignment a_x_align, StringAlignment a_y_align)
{
	// ���� ����(x��) ����: ����, ���, ������
	GdipSetStringFormatAlign(mp_stock_str_format, a_x_align);
	// ���� ����(y��) ����: ����, ���, �Ʒ���
	GdipSetStringFormatLineAlign(mp_stock_str_format, a_y_align);
	// mp_stock_font_brush�� ������ ��ġ�� ���ڿ��� ���
	GdipDrawString(mp_graphics, ap_str, a_str_len, ap_font, ap_rect, mp_stock_str_format, mp_stock_font_brush);
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y)
{
	GdipDrawImageI(mp_graphics, ap_image, a_x, a_y); // ���޹��� �̹��� ���
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy)
{
	GdipDrawImageRectI(mp_graphics, ap_image, a_x, a_y, a_cx, a_cy); // ���޹��� �̹��� ���
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	// �̹��� ��ü�� �Ӽ��� �����Ͽ� ���޹��� �̹��� ���
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, m_cx, m_cy,
		0, 0, m_cx, m_cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy, GpImageAttributes* ap_attr)
{
	// �̹��� ��ü�� �Ӽ��� �����Ͽ� ���޹��� �̹��� ���
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, a_cx, a_cy,
		0, 0, a_cx, a_cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::DrawFullImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	UINT cx = 0, cy = 0;
	GdipGetImageWidth(ap_image, &cx);
	GdipGetImageHeight(ap_image, &cy);
	// �̹��� ��ü�� �Ӽ��� �����Ͽ� ���޹��� �̹��� ���
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, m_cx, m_cy,
		0, 0, cx, cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::DrawRealImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	UINT cx = 0, cy = 0;
	GdipGetImageWidth(ap_image, &cx);
	GdipGetImageHeight(ap_image, &cy);
	// �̹��� ��ü�� �Ӽ��� �����Ͽ� ���޹��� �̹��� ���
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, cx, cy,
		0, 0, cx, cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::Draw(HDC ah_dc, int a_x, int a_y)
{
	GpGraphics* p_graphics = NULL;
	GdipCreateFromHDC(ah_dc, &p_graphics); // HDC�� �����ؼ� ����ϴ� GDI+ ��� ��ü ����
	GdipDrawImageI(p_graphics, mp_bitmap, a_x, a_y); // ��Ʈ���� ȭ�� (a_x, a_y) ��ġ�� ���
	GdipDeleteGraphics(p_graphics); // ����ϴ� GDI+ ��� ��ü ����
}

void TXYC_GDIP::Draw(HDC ah_dc, int a_x, int a_y, int a_cx, int a_cy)
{
	GpGraphics* p_graphics = NULL;
	GdipCreateFromHDC(ah_dc, &p_graphics); // HDC�� �����ؼ� ����ϴ� GDI+ ��� ��ü ����
	GdipDrawImageRectI(p_graphics, mp_bitmap, a_x, a_y, a_cx, a_cy); // ��Ʈ���� ȭ�� (a_x, a_y) ��ġ�� ���
	GdipDeleteGraphics(p_graphics); // ����ϴ� GDI+ ��� ��ü ����
}

void TXYC_GDIP::SetBrushColor(ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_brush, a_color); // ä��� �귯�� ������ ������ �������� ����
}

void TXYC_GDIP::SetPenColor(ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // �� ������ ������ �������� ����
}

void TXYC_GDIP::SetTextColor(ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_font_brush, a_color); // �۲� �귯�� ������ ������ �������� ����
}

void TXYC_GDIP::ChangeFont(const wchar_t* ap_font_name, REAL a_size, INT a_style)
{
	GdipDeleteFont(mp_stock_font); // ����ϴ� �۲� ��ü ����
	wchar_t font_name[128];
	GdipGetFamilyName(mp_stock_font_family, font_name, LANG_KOREAN);
	if (wcscmp(font_name, ap_font_name)) // ����ϴ� �۲��� ���� ���հ� �� �۲��� ���� ������ �ٸ� ���
	{
		GdipDeleteFontFamily(mp_stock_font_family); // ����ϴ� �۲��� �۲� ���� ��ü ����
		// �� �۲��� ���� �۲� ���� ��ü ����
		GdipCreateFontFamilyFromName(ap_font_name, NULL, &mp_stock_font_family);
	}
	// ũ�Ⱑ a_size�̰� a_style �Ӽ��� ���� �۲� ����
	GdipCreateFont(mp_stock_font_family, a_size, a_style, UnitPixel, &mp_stock_font);
}

GpFontFamily* TXYC_GDIP::CreateFontFamily(const wchar_t* ap_font_name)
{
	GpFontFamily* p_family = NULL;
	GdipCreateFontFamilyFromName(ap_font_name, NULL, &p_family); // �۲� ���� ��ü ����
	return p_family;
}

GpFont* TXYC_GDIP::CreateFont(GpFontFamily* ap_family, REAL a_size, INT a_style)
{
	GpFont* p_font = NULL;
	// ũ�Ⱑ a_size�̰� a_style �Ӽ��� ���� �۰� ����
	GdipCreateFont(ap_family, a_size, a_style, UnitPixel, &p_font);
	return p_font;
}

void TXYC_GDIP::DeleteFontFamily(GpFontFamily* ap_family)
{
	GdipDeleteFontFamily(ap_family); // ����ϴ� �۲� ���� ��ü ����
}

void TXYC_GDIP::DeleteFont(GpFont* ap_font)
{
	GdipDeleteFont(ap_font); // ����ϴ� �۲� ��ü ����
}

void TXYC_GDIP::DeleteFontInfo(GpFontFamily* ap_family, GpFont* ap_font)
{
	GdipDeleteFontFamily(ap_family); // ����ϴ� �۲� ���� ��ü ����
	GdipDeleteFont(ap_font); // ����ϴ� �۲� ��ü ����
}

void TXYC_GDIP::SetSmoothingMode(UINT8 a_smooth)
{
	if (a_smooth)
	{
		GdipSetSmoothingMode(mp_graphics, SmoothingModeAntiAlias);
	}
	else
	{
		GdipSetSmoothingMode(mp_graphics, SmoothingModeNone);
	}
}

GpImage* TXYC_GDIP::LoadImage(const wchar_t* ap_path)
{
	GpImage* p_image = NULL;
	// �־��� ��ο� ��ġ�� �̹��� ������ �о �̹��� ��ü�� ����
	GdipLoadImageFromFile(ap_path, &p_image);
	return p_image;
}

GpBitmap* TXYC_GDIP::LoadBitmap(const wchar_t* ap_path)
{
	GpBitmap* p_bitmap = NULL;
	// �־��� ��ο� ��ġ�� �̹��� ������ �о ��Ʈ�� ��ü�� ����
	GdipCreateBitmapFromFile(ap_path, &p_bitmap);
	return p_bitmap;
}

void TXYC_GDIP::DestroyImage(GpImage* ap_image)
{
	GdipDisposeImage(ap_image); // �̹��� ��ü ����
}

GpImage* TXYC_GDIP::CreateThumbnail(GpImage* ap_src_image, int a_width, int a_height)
{
	GpImage* p_thumb_image = NULL;
	GdipGetImageThumbnail(ap_src_image, a_width, a_height, &p_thumb_image, NULL, NULL);
	return p_thumb_image;
}

UINT TXYC_GDIP::GetEncoderClsid(const wchar_t* ap_format, CLSID* ap_clsid)
{
	// ���ڴ��� ������ ���ڴ� ������ ũ��
	UINT enc_num = 0, enc_data_size = 0, index = -1;
	// ���ڴ��� ũ�� ������ ����
	GetImageEncodersSize(&enc_num, &enc_data_size);
	if (!enc_data_size)
	{
		return -1; // ���ڴ� ���� ������ ����
	}
	ImageCodecInfo* p_image_codec_info = (ImageCodecInfo*)malloc(enc_data_size);
	GetImageEncoders(enc_num, enc_data_size, p_image_codec_info);
	for (UINT i = 0; i < enc_num; ++i)
	{
		if (!wcscmp(p_image_codec_info[i].MimeType, ap_format))
		{
			*ap_clsid = p_image_codec_info[i].Clsid;
			index = i;
			break;
		}
	}
	free(p_image_codec_info);
	return index;
}

GpPen* TXYC_GDIP::GetStockPen()
{
	return mp_stock_pen;
}

GpBrush* TXYC_GDIP::GetStockBrush()
{
	return mp_stock_brush;
}

GpGraphics* TXYC_GDIP::GetGraphics()
{
	return mp_graphics;
}

GpImage* TXYC_GDIP::GetStockImage()
{
	return mp_bitmap;
}

int TXYC_GDIP::GetWidth()
{
	return m_cx;
}

int TXYC_GDIP::GetHeight()
{
	return m_cy;
}