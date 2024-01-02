#ifndef _TXYC_GDIP_H_
#define _TXYC_GDIP_H_

// 클래스 작성자: 박정현(bac6536@naver.com)
// 이 클래스는 다른 개발자가 작성한 클래스를 기반으로 작성되었음
// 원본 클래스 작성자: 김성엽(tipsware@naver.com)
// 원본 클래스 배포 주소: https://cafe.naver.com/mythread

#include <gdiplus.h>

#define RGB24(r, g, b) (0xFF000000 | ((UINT8)r << 16) | ((UINT8)g << 8) | (UINT8)b)
#define RGB32(a, r, g, b) (((UINT8)a << 24) | ((UINT8)r << 16) | ((UINT8)g << 8) | (UINT8)b)
#define BG_COLOR_R 225
#define BG_COLOR_G 225
#define BG_COLOR_B 225

using namespace Gdiplus;
using namespace DllExports;

class TXYC_GDIP
{
protected:
	GpBitmap* mp_bitmap; // GDI+ 출력을 저장할 비트맵 객체
	GpGraphics* mp_graphics; // 비트맵에 연결된 GDI+ 출력 객체
	GpPen* mp_stock_pen = NULL; // 출력에 사용할 펜 객체
	// 출력에 사용할 채우기 브러시, 글꼴 브러시 객체
	GpSolidFill* mp_stock_brush, * mp_stock_font_brush;
	GpFontFamily* mp_stock_font_family; // 문자 집합 객체
	GpFont* mp_stock_font; // 글꼴 객체
	// 문자열을 정렬해서 출력할 때 사용하기 위한 StringFormat 객체
	GpStringFormat* mp_stock_str_format;
	int m_cx, m_cy; // 출력 영역의 폭과 높이
	static ULONG_PTR m_token; // GDI+ 라이브러리 사용 정보
	static UINT m_object_count; // GDI+ 객체 개수 카운터
public:
	TXYC_GDIP();
	TXYC_GDIP(int a_cx, int a_cy);
	virtual ~TXYC_GDIP();
	void Init(); // 객체 초기화 함수
	int CreateGDIP(int a_cx, int a_cy); // GDI+ 출력 장치 생성 함수
	void Clear(ARGB a_color); // 출력 영역 전체를 a_color로 채우는 함수
	// 새로운 색상을 지정하지 않고 기존 브러시로 사각형을 그리는 함수(내부 채우기만 수행)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy);
	// 지정된 색상으로 사각형을 그리는 함수(내부 채우기만 수행)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color);
	// 지정된 브러시로 사각형을 그리는 함수(내부 채우기만 수행)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy, GpBrush* ap_brush);
	// 새로운 색상을 지정하지 않고 기존 펜으로 사각형을 그리는 함수(테두리 그리기만 수행)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy);
	// 지정된 색상으로 사각형을 그리는 함수(테두리 그리기만 수행)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color);
	// 지정된 펜으로 사각형을 그리는 함수(테두리 그리기만 수행)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy, GpPen* ap_pen);
	// 기존에 사용하던 펜과 브러시로 사각형을 그리는 함수(내부 채우기와 테두리 그리기 모두 수행)
	void Rectangle(int a_x, int a_y, int a_cx, int a_cy);
	// 기존 펜으로 선을 그리는 함수(시작 점과 끝 점 사용)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey);
	// 지정한 색상으로 선을 그리는 함수(시작 점과 끝 점 사용)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, ARGB a_color);
	// 지정한 펜으로 선을 그리는 함수(시작 점과 끝 점 사용)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, GpPen* ap_pen);
	// 문자열을 지정한 위치에 출력하는 함수(색상 지정 가능)
	void TextOut(int a_x, int a_y, const wchar_t* ap_str, int a_str_len = -1);
	void TextOut(GpFont* ap_font, int a_x, int a_y, const wchar_t* ap_str, int a_str_len = -1);
	// 문자열을 지정한 위치에 정렬해서 출력하는 함수
	void DrawText(RectF* ap_rect, const wchar_t* ap_str, int a_str_len = -1,
		StringAlignment a_x_align = StringAlignmentCenter, StringAlignment a_y_align = StringAlignmentCenter);
	void DrawText(GpFont* ap_font, RectF* ap_rect, const wchar_t* ap_str, int a_str_len = -1,
		StringAlignment a_x_align = StringAlignmentCenter, StringAlignment a_y_align = StringAlignmentCenter);
	// 전달된 이미지 객체를 출력하는 함수(다른 이미지 객체를 현재 이미지 객체에 출력하는 함수)
	void Draw(GpImage* ap_image, int a_x, int a_y);
	// 전달된 이미지 객체를 출력하는 함수(다른 이미지 객체를 현재 이미지 객체에 출력하는 함수)
	void Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy);
	// 전달된 이미지 객체를 출력하는 함수(이미지 속성을 적용하는 함수)
	void Draw(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// 전달된 이미지 객체의 시작 좌표와 크기를 지정해서 출력하는 함수(이미지 속성을 적용하는 함수)
	void Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy, GpImageAttributes* ap_attr);
	// 전달된 이미지 객체의 전체 내용을 출력하는 함수(이미지 속성을 적용하는 함수)
	void DrawFullImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// 전달된 이미지 객체 크기로 내용을 출력하는 함수(이미지 속성을 적용하는 함수)
	void DrawRealImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// 전달된 DC에 현재 이미지를 출력하는 함수
	void Draw(HDC ah_dc, int a_x = 0, int a_y = 0);
	// 전달된 DC에 현재 이미지를 출력하는 함수
	void Draw(HDC ah_dc, int a_x, int a_y, int a_cx, int a_cy);
	void SetBrushColor(ARGB a_color); // 브러시 객체의 색상을 변경하는 함수
	void SetPenColor(ARGB a_color); // 펜 객체의 색상을 변경하는 함수
	void SetTextColor(ARGB a_color); // 글꼴의 색상을 변경하는 함수
	// 글꼴 정보를 변경하는 함수
	void ChangeFont(const wchar_t* ap_font_name, REAL a_size = 15, INT a_style = FontStyleRegular);
	GpFontFamily* CreateFontFamily(const wchar_t* ap_font_name); // 글꼴 집합 생성 함수
	GpFont* CreateFont(GpFontFamily* ap_family, REAL a_size, INT a_style); // 글꼴 생성 함수
	void DeleteFontFamily(GpFontFamily* ap_family); // 글꼴 집합 제거 함수
	void DeleteFont(GpFont* ap_font); // 글꼴 제거 함수
	void DeleteFontInfo(GpFontFamily* ap_family, GpFont* ap_font); // 글꼴 정보 제거 함수
	void SetSmoothingMode(UINT8 a_smooth = 1); // 안티엘리어스 모드 설정 여부 지정 함수
	GpImage* LoadImage(const wchar_t* ap_path); // 이미지 파일을 읽어서 이미지 객체로 만드는 함수
	GpBitmap* LoadBitmap(const wchar_t* ap_path); // 이미지 파일을 읽어서 비트맵 객체로 만드는 함수
	void DestroyImage(GpImage* ap_image); // 이미지 객체 제거 함수
	// 지정한 이미지로 a_width * a_height 크기의 섬네일 이미지를 생성하는 함수
	GpImage* CreateThumbnail(GpImage* ap_src_image, int a_width, int a_height);
	UINT GetEncoderClsid(const wchar_t* ap_format, CLSID* ap_clsid); // 이미지 인코더의 CLSID 값을 얻는 함수
	GpPen* GetStockPen(); // 내부적으로 생성되어 있는 펜 객체의 주소를 얻는 함수
	GpBrush* GetStockBrush(); // 내부적으로 생성되어 있는 브러시 객체의 주소를 얻는 함수
	GpGraphics* GetGraphics(); // GDI+ 출력 객체의 주소를 얻는 함수
	GpImage* GetStockImage(); // GDI+ 출력에 사용하는 이미지 객체의 주소를 얻는 함수
	int GetWidth(); // 출력 영역의 폭을 얻는 함수
	int GetHeight(); // 출력 영역의 높이를 얻는 함수
};

#endif // !_TXYC_GDIP_H_