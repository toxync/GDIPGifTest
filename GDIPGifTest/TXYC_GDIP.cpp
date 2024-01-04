#include "pch.h"
#include "TXYC_GDIP.h"

// static 멤버 변수 초기화
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
		GdipDeletePen(mp_stock_pen); // 사용하던 펜 객체 제거
		GdipDeleteBrush(mp_stock_brush); // 사용하던 채우기 브러시 객체 제거
		GdipDeleteBrush(mp_stock_font_brush); // 사용하던 글꼴 브러시 객체 제거
		GdipDeleteFont(mp_stock_font); // 사용하던 글꼴 객체 제거
		GdipDeleteFontFamily(mp_stock_font_family); // 사용하던 글꼴 집합 객체 제거
		GdipDeleteStringFormat(mp_stock_str_format); // 사용하던 StringFormat 객체 제거
	}
	GdipDeleteGraphics(mp_graphics); // 사용하던 비트맵 기반 GDI+ 객체 제거
	GdipDisposeImage(mp_bitmap); // 사용하던 비트맵 객체 제거
	--m_object_count; // GDIP 객체 개수 감소
	// 이 객체가 마지막 GDIP 객체라면 GDI+ 사용 종료
	if (!m_object_count)
	{
		GdiplusShutdown(m_token);
	}
}

void TXYC_GDIP::Init()
{
	if (!m_object_count) // 이 객체가 첫 GDIP 객체라면 GDI+ 초기화
	{
		GdiplusStartupInput gpsi;
		GdiplusStartup(&m_token, &gpsi, NULL);
	}
	++m_object_count; // GDIP 객체 개수 증가
	m_cx = m_cy = 0; // GDI+ 출력용 비트맵의 크기 정보
	mp_bitmap = NULL; // 출력된 이미지를 저장할 비트맵 객체
	mp_graphics = NULL; // 비트맵에 연결된 GDI+ 출력 객체
	// 0xFF000000 색상을 가진 글꼴 브러시 객체 생성
	GdipCreateSolidFill(0xFF000000, &mp_stock_font_brush);
	// 0xFFFFFFFF 색상을 가진 채우기 브러시 객체 생성
	GdipCreateSolidFill(0xFFFFFFFF, &mp_stock_brush);
	// 0xFF000000 색상을 가지고 굵기가 1이며 실선 속성을 가진 펜 객체 생성
	GdipCreatePen1(0xFF000000, 1, UnitWorld, &mp_stock_pen);
	// '맑은 고딕' 글꼴 집합을 관리하는 객체 생성
	GdipCreateFontFamilyFromName(L"맑은 고딕", NULL, &mp_stock_font_family);
	// 글꼴의 크기가 15이고 특별한 속성이 없는 글꼴 생성
	GdipCreateFont(mp_stock_font_family, 15, FontStyleRegular, UnitPixel, &mp_stock_font);
	// 문자열을 정렬해서 출력하기 위한 StringFormat 객체 생성
	GdipCreateStringFormat(0, LANG_KOREAN, &mp_stock_str_format);
}

int TXYC_GDIP::CreateGDIP(int a_cx, int a_cy)
{
	// 출력 객체가 생성되어 있지 않거나 폭과 높이가 달라져야만 출력 객체를 생성함
	if (mp_graphics == NULL || m_cx != a_cx || m_cy != a_cy)
	{
		// 새로운 폭과 높이로 설정
		m_cx = a_cx;
		m_cy = a_cy;
		// 기존에 사용하던 객체들은 제거
		if (mp_graphics != NULL)
		{
			GdipDeleteGraphics(mp_graphics); // 사용하던 비트맵 기반 GDI+ 출력 객체 제거
		}
		if (mp_bitmap != NULL)
		{
			GdipDisposeImage(mp_bitmap); // 사용하던 비트맵 객체 제거
		}
		// 새로운 크기에 맞게 비트맵 객체 생성
		GdipCreateBitmapFromScan0(a_cx, a_cy, 0, PixelFormat32bppARGB, NULL, &mp_bitmap);
		// 비트맵 객체에 연결해서 사용하는 GDI+ 출력 객체 생성
		GdipGetImageGraphicsContext(mp_bitmap, &mp_graphics);
		// 'AntiAlias'를 사용하도록 설정(사용 중지는 SmoothingModeInvalid)
		GdipSetSmoothingMode(mp_graphics, SmoothingModeAntiAlias);
		GdipSetTextRenderingHint(mp_graphics, TextRenderingHintAntiAliasGridFit);
		return 1;
	}
	return 0;
}

void TXYC_GDIP::Clear(ARGB a_color)
{
	GdipGraphicsClear(mp_graphics, a_color); // 지정한 색상으로 전체 출력 영역을 채움
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // 사각형 내부 채우기
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_brush, a_color); // 브러시 색상을 지정된 색상으로 변경
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // 사각형 내부 채우기
}

void TXYC_GDIP::FillRect(int a_x, int a_y, int a_cx, int a_cy, GpBrush* ap_brush)
{
	GdipFillRectangleI(mp_graphics, ap_brush, a_x, a_y, a_cx, a_cy); // 사각형 내부 채우기
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // 사각형 테두리 그리기
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // 펜 색상을 지정된 색상으로 변경
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // 사각형 테두리 그리기
}

void TXYC_GDIP::DrawRect(int a_x, int a_y, int a_cx, int a_cy, GpPen* ap_pen)
{
	GdipDrawRectangleI(mp_graphics, ap_pen, a_x, a_y, a_cx, a_cy); // 사각형 테두리 그리기
}

void TXYC_GDIP::Rectangle(int a_x, int a_y, int a_cx, int a_cy)
{
	GdipFillRectangleI(mp_graphics, mp_stock_brush, a_x, a_y, a_cx, a_cy); // 사각형 내부 채우기
	GdipDrawRectangleI(mp_graphics, mp_stock_pen, a_x, a_y, a_cx, a_cy); // 사각형 테두리 그리기
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey)
{
	GdipDrawLineI(mp_graphics, mp_stock_pen, a_sx, a_sy, a_ex, a_ey); // 선 그리기
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // 펜 색상을 지정된 색상으로 변경
	GdipDrawLineI(mp_graphics, mp_stock_pen, a_sx, a_sy, a_ex, a_ey); // 선 그리기
}

void TXYC_GDIP::DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, GpPen* ap_pen)
{
	GdipDrawLineI(mp_graphics, ap_pen, a_sx, a_sy, a_ex, a_ey); // 선 그리기
}

void TXYC_GDIP::TextOut(int a_x, int a_y, const wchar_t* ap_str, int a_str_len)
{
	RectF rect((REAL)a_x, (REAL)a_y, 0, 0); // 문자열 출력 위치를 사각형으로 지정
	// mp_stock_font_brush로 지정한 위치에 문자열을 출력
	GdipDrawString(mp_graphics, ap_str, a_str_len, mp_stock_font, &rect, NULL, mp_stock_font_brush);
}

void TXYC_GDIP::TextOut(GpFont* ap_font, int a_x, int a_y, const wchar_t* ap_str, int a_str_len)
{
	RectF rect((REAL)a_x, (REAL)a_y, 0, 0); // 문자열 출력 위치를 사각형으로 지정
	// mp_stock_font_brush로 지정한 위치에 문자열을 출력
	GdipDrawString(mp_graphics, ap_str, a_str_len, ap_font, &rect, NULL, mp_stock_font_brush);
}

void TXYC_GDIP::DrawText(RectF* ap_rect, const wchar_t* ap_str, int a_str_len,
	StringAlignment a_x_align, StringAlignment a_y_align)
{
	// 가로 방향(x축) 정렬: 왼쪽, 가운데, 오른쪽
	GdipSetStringFormatAlign(mp_stock_str_format, a_x_align);
	// 세로 방향(y축) 정렬: 위쪽, 가운데, 아래쪽
	GdipSetStringFormatLineAlign(mp_stock_str_format, a_y_align);
	// mp_stock_font_brush로 지정한 위치에 문자열을 출력
	GdipDrawString(mp_graphics, ap_str, a_str_len, mp_stock_font, ap_rect, mp_stock_str_format, mp_stock_font_brush);
}

void TXYC_GDIP::DrawText(GpFont* ap_font, RectF* ap_rect, const wchar_t* ap_str, int a_str_len,
	StringAlignment a_x_align, StringAlignment a_y_align)
{
	// 가로 방향(x축) 정렬: 왼쪽, 가운데, 오른쪽
	GdipSetStringFormatAlign(mp_stock_str_format, a_x_align);
	// 세로 방향(y축) 정렬: 위쪽, 가운데, 아래쪽
	GdipSetStringFormatLineAlign(mp_stock_str_format, a_y_align);
	// mp_stock_font_brush로 지정한 위치에 문자열을 출력
	GdipDrawString(mp_graphics, ap_str, a_str_len, ap_font, ap_rect, mp_stock_str_format, mp_stock_font_brush);
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y)
{
	GdipDrawImageI(mp_graphics, ap_image, a_x, a_y); // 전달받은 이미지 출력
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy)
{
	GdipDrawImageRectI(mp_graphics, ap_image, a_x, a_y, a_cx, a_cy); // 전달받은 이미지 출력
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	// 이미지 객체의 속성을 적용하여 전달받은 이미지 출력
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, m_cx, m_cy,
		0, 0, m_cx, m_cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy, GpImageAttributes* ap_attr)
{
	// 이미지 객체의 속성을 적용하여 전달받은 이미지 출력
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, a_cx, a_cy,
		0, 0, a_cx, a_cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::DrawFullImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	UINT cx = 0, cy = 0;
	GdipGetImageWidth(ap_image, &cx);
	GdipGetImageHeight(ap_image, &cy);
	// 이미지 객체의 속성을 적용하여 전달받은 이미지 출력
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, m_cx, m_cy,
		0, 0, cx, cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::DrawRealImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr)
{
	UINT cx = 0, cy = 0;
	GdipGetImageWidth(ap_image, &cx);
	GdipGetImageHeight(ap_image, &cy);
	// 이미지 객체의 속성을 적용하여 전달받은 이미지 출력
	GdipDrawImageRectRectI(mp_graphics, ap_image, a_x, a_y, cx, cy,
		0, 0, cx, cy, UnitPixel, ap_attr, NULL, NULL);
}

void TXYC_GDIP::Draw(HDC ah_dc, int a_x, int a_y)
{
	GpGraphics* p_graphics = NULL;
	GdipCreateFromHDC(ah_dc, &p_graphics); // HDC에 연결해서 사용하는 GDI+ 출력 객체 생성
	GdipDrawImageI(p_graphics, mp_bitmap, a_x, a_y); // 비트맵을 화면 (a_x, a_y) 위치에 출력
	GdipDeleteGraphics(p_graphics); // 사용하던 GDI+ 출력 객체 제거
}

void TXYC_GDIP::Draw(HDC ah_dc, int a_x, int a_y, int a_cx, int a_cy)
{
	GpGraphics* p_graphics = NULL;
	GdipCreateFromHDC(ah_dc, &p_graphics); // HDC에 연결해서 사용하는 GDI+ 출력 객체 생성
	GdipDrawImageRectI(p_graphics, mp_bitmap, a_x, a_y, a_cx, a_cy); // 비트맵을 화면 (a_x, a_y) 위치에 출력
	GdipDeleteGraphics(p_graphics); // 사용하던 GDI+ 출력 객체 제거
}

void TXYC_GDIP::SetBrushColor(ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_brush, a_color); // 채우기 브러시 색상을 지정된 색상으로 변경
}

void TXYC_GDIP::SetPenColor(ARGB a_color)
{
	GdipSetPenColor(mp_stock_pen, a_color); // 펜 색상을 지정된 색상으로 변경
}

void TXYC_GDIP::SetTextColor(ARGB a_color)
{
	GdipSetSolidFillColor(mp_stock_font_brush, a_color); // 글꼴 브러시 색상을 지정된 색상으로 변경
}

void TXYC_GDIP::ChangeFont(const wchar_t* ap_font_name, REAL a_size, INT a_style)
{
	GdipDeleteFont(mp_stock_font); // 사용하던 글꼴 객체 제거
	wchar_t font_name[128];
	GdipGetFamilyName(mp_stock_font_family, font_name, LANG_KOREAN);
	if (wcscmp(font_name, ap_font_name)) // 사용하던 글꼴의 문자 집합과 새 글꼴의 문자 집합이 다를 경우
	{
		GdipDeleteFontFamily(mp_stock_font_family); // 사용하던 글꼴의 글꼴 집합 객체 제거
		// 새 글꼴을 위한 글꼴 집합 객체 생성
		GdipCreateFontFamilyFromName(ap_font_name, NULL, &mp_stock_font_family);
	}
	// 크기가 a_size이고 a_style 속성을 가진 글꼴 생성
	GdipCreateFont(mp_stock_font_family, a_size, a_style, UnitPixel, &mp_stock_font);
}

GpFontFamily* TXYC_GDIP::CreateFontFamily(const wchar_t* ap_font_name)
{
	GpFontFamily* p_family = NULL;
	GdipCreateFontFamilyFromName(ap_font_name, NULL, &p_family); // 글꼴 집합 객체 생성
	return p_family;
}

GpFont* TXYC_GDIP::CreateFont(GpFontFamily* ap_family, REAL a_size, INT a_style)
{
	GpFont* p_font = NULL;
	// 크기가 a_size이고 a_style 속성을 가진 글골 생성
	GdipCreateFont(ap_family, a_size, a_style, UnitPixel, &p_font);
	return p_font;
}

void TXYC_GDIP::DeleteFontFamily(GpFontFamily* ap_family)
{
	GdipDeleteFontFamily(ap_family); // 사용하던 글꼴 집합 객체 제거
}

void TXYC_GDIP::DeleteFont(GpFont* ap_font)
{
	GdipDeleteFont(ap_font); // 사용하던 글꼴 객체 제거
}

void TXYC_GDIP::DeleteFontInfo(GpFontFamily* ap_family, GpFont* ap_font)
{
	GdipDeleteFontFamily(ap_family); // 사용하던 글꼴 집합 객체 제거
	GdipDeleteFont(ap_font); // 사용하던 글꼴 객체 제거
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
	// 주어진 경로에 위치한 이미지 파일을 읽어서 이미지 객체를 생성
	GdipLoadImageFromFile(ap_path, &p_image);
	return p_image;
}

GpBitmap* TXYC_GDIP::LoadBitmap(const wchar_t* ap_path)
{
	GpBitmap* p_bitmap = NULL;
	// 주어진 경로에 위치한 이미지 파일을 읽어서 비트맵 객체를 생성
	GdipCreateBitmapFromFile(ap_path, &p_bitmap);
	return p_bitmap;
}

void TXYC_GDIP::DestroyImage(GpImage* ap_image)
{
	GdipDisposeImage(ap_image); // 이미지 객체 제거
}

GpImage* TXYC_GDIP::CreateThumbnail(GpImage* ap_src_image, int a_width, int a_height)
{
	GpImage* p_thumb_image = NULL;
	GdipGetImageThumbnail(ap_src_image, a_width, a_height, &p_thumb_image, NULL, NULL);
	return p_thumb_image;
}

UINT TXYC_GDIP::GetEncoderClsid(const wchar_t* ap_format, CLSID* ap_clsid)
{
	// 인코더의 개수와 인코더 정보의 크기
	UINT enc_num = 0, enc_data_size = 0, index = -1;
	// 인코더의 크기 정보를 얻음
	GetImageEncodersSize(&enc_num, &enc_data_size);
	if (!enc_data_size)
	{
		return -1; // 인코더 정보 얻어오기 실패
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