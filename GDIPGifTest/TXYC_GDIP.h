#ifndef _TXYC_GDIP_H_
#define _TXYC_GDIP_H_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)
// �� Ŭ������ �ٸ� �����ڰ� �ۼ��� Ŭ������ ������� �ۼ��Ǿ���
// ���� Ŭ���� �ۼ���: �輺��(tipsware@naver.com)
// ���� Ŭ���� ���� �ּ�: https://cafe.naver.com/mythread

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
	GpBitmap* mp_bitmap; // GDI+ ����� ������ ��Ʈ�� ��ü
	GpGraphics* mp_graphics; // ��Ʈ�ʿ� ����� GDI+ ��� ��ü
	GpPen* mp_stock_pen = NULL; // ��¿� ����� �� ��ü
	// ��¿� ����� ä��� �귯��, �۲� �귯�� ��ü
	GpSolidFill* mp_stock_brush, * mp_stock_font_brush;
	GpFontFamily* mp_stock_font_family; // ���� ���� ��ü
	GpFont* mp_stock_font; // �۲� ��ü
	// ���ڿ��� �����ؼ� ����� �� ����ϱ� ���� StringFormat ��ü
	GpStringFormat* mp_stock_str_format;
	int m_cx, m_cy; // ��� ������ ���� ����
	static ULONG_PTR m_token; // GDI+ ���̺귯�� ��� ����
	static UINT m_object_count; // GDI+ ��ü ���� ī����
public:
	TXYC_GDIP();
	TXYC_GDIP(int a_cx, int a_cy);
	virtual ~TXYC_GDIP();
	void Init(); // ��ü �ʱ�ȭ �Լ�
	int CreateGDIP(int a_cx, int a_cy); // GDI+ ��� ��ġ ���� �Լ�
	void Clear(ARGB a_color); // ��� ���� ��ü�� a_color�� ä��� �Լ�
	// ���ο� ������ �������� �ʰ� ���� �귯�÷� �簢���� �׸��� �Լ�(���� ä��⸸ ����)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy);
	// ������ �������� �簢���� �׸��� �Լ�(���� ä��⸸ ����)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color);
	// ������ �귯�÷� �簢���� �׸��� �Լ�(���� ä��⸸ ����)
	void FillRect(int a_x, int a_y, int a_cx, int a_cy, GpBrush* ap_brush);
	// ���ο� ������ �������� �ʰ� ���� ������ �簢���� �׸��� �Լ�(�׵θ� �׸��⸸ ����)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy);
	// ������ �������� �簢���� �׸��� �Լ�(�׵θ� �׸��⸸ ����)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy, ARGB a_color);
	// ������ ������ �簢���� �׸��� �Լ�(�׵θ� �׸��⸸ ����)
	void DrawRect(int a_x, int a_y, int a_cx, int a_cy, GpPen* ap_pen);
	// ������ ����ϴ� ��� �귯�÷� �簢���� �׸��� �Լ�(���� ä���� �׵θ� �׸��� ��� ����)
	void Rectangle(int a_x, int a_y, int a_cx, int a_cy);
	// ���� ������ ���� �׸��� �Լ�(���� ���� �� �� ���)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey);
	// ������ �������� ���� �׸��� �Լ�(���� ���� �� �� ���)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, ARGB a_color);
	// ������ ������ ���� �׸��� �Լ�(���� ���� �� �� ���)
	void DrawLine(int a_sx, int a_sy, int a_ex, int a_ey, GpPen* ap_pen);
	// ���ڿ��� ������ ��ġ�� ����ϴ� �Լ�(���� ���� ����)
	void TextOut(int a_x, int a_y, const wchar_t* ap_str, int a_str_len = -1);
	void TextOut(GpFont* ap_font, int a_x, int a_y, const wchar_t* ap_str, int a_str_len = -1);
	// ���ڿ��� ������ ��ġ�� �����ؼ� ����ϴ� �Լ�
	void DrawText(RectF* ap_rect, const wchar_t* ap_str, int a_str_len = -1,
		StringAlignment a_x_align = StringAlignmentCenter, StringAlignment a_y_align = StringAlignmentCenter);
	void DrawText(GpFont* ap_font, RectF* ap_rect, const wchar_t* ap_str, int a_str_len = -1,
		StringAlignment a_x_align = StringAlignmentCenter, StringAlignment a_y_align = StringAlignmentCenter);
	// ���޵� �̹��� ��ü�� ����ϴ� �Լ�(�ٸ� �̹��� ��ü�� ���� �̹��� ��ü�� ����ϴ� �Լ�)
	void Draw(GpImage* ap_image, int a_x, int a_y);
	// ���޵� �̹��� ��ü�� ����ϴ� �Լ�(�ٸ� �̹��� ��ü�� ���� �̹��� ��ü�� ����ϴ� �Լ�)
	void Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy);
	// ���޵� �̹��� ��ü�� ����ϴ� �Լ�(�̹��� �Ӽ��� �����ϴ� �Լ�)
	void Draw(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// ���޵� �̹��� ��ü�� ���� ��ǥ�� ũ�⸦ �����ؼ� ����ϴ� �Լ�(�̹��� �Ӽ��� �����ϴ� �Լ�)
	void Draw(GpImage* ap_image, int a_x, int a_y, int a_cx, int a_cy, GpImageAttributes* ap_attr);
	// ���޵� �̹��� ��ü�� ��ü ������ ����ϴ� �Լ�(�̹��� �Ӽ��� �����ϴ� �Լ�)
	void DrawFullImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// ���޵� �̹��� ��ü ũ��� ������ ����ϴ� �Լ�(�̹��� �Ӽ��� �����ϴ� �Լ�)
	void DrawRealImage(GpImage* ap_image, int a_x, int a_y, GpImageAttributes* ap_attr);
	// ���޵� DC�� ���� �̹����� ����ϴ� �Լ�
	void Draw(HDC ah_dc, int a_x = 0, int a_y = 0);
	// ���޵� DC�� ���� �̹����� ����ϴ� �Լ�
	void Draw(HDC ah_dc, int a_x, int a_y, int a_cx, int a_cy);
	void SetBrushColor(ARGB a_color); // �귯�� ��ü�� ������ �����ϴ� �Լ�
	void SetPenColor(ARGB a_color); // �� ��ü�� ������ �����ϴ� �Լ�
	void SetTextColor(ARGB a_color); // �۲��� ������ �����ϴ� �Լ�
	// �۲� ������ �����ϴ� �Լ�
	void ChangeFont(const wchar_t* ap_font_name, REAL a_size = 15, INT a_style = FontStyleRegular);
	GpFontFamily* CreateFontFamily(const wchar_t* ap_font_name); // �۲� ���� ���� �Լ�
	GpFont* CreateFont(GpFontFamily* ap_family, REAL a_size, INT a_style); // �۲� ���� �Լ�
	void DeleteFontFamily(GpFontFamily* ap_family); // �۲� ���� ���� �Լ�
	void DeleteFont(GpFont* ap_font); // �۲� ���� �Լ�
	void DeleteFontInfo(GpFontFamily* ap_family, GpFont* ap_font); // �۲� ���� ���� �Լ�
	void SetSmoothingMode(UINT8 a_smooth = 1); // ��Ƽ����� ��� ���� ���� ���� �Լ�
	GpImage* LoadImage(const wchar_t* ap_path); // �̹��� ������ �о �̹��� ��ü�� ����� �Լ�
	GpBitmap* LoadBitmap(const wchar_t* ap_path); // �̹��� ������ �о ��Ʈ�� ��ü�� ����� �Լ�
	void DestroyImage(GpImage* ap_image); // �̹��� ��ü ���� �Լ�
	// ������ �̹����� a_width * a_height ũ���� ������ �̹����� �����ϴ� �Լ�
	GpImage* CreateThumbnail(GpImage* ap_src_image, int a_width, int a_height);
	UINT GetEncoderClsid(const wchar_t* ap_format, CLSID* ap_clsid); // �̹��� ���ڴ��� CLSID ���� ��� �Լ�
	GpPen* GetStockPen(); // ���������� �����Ǿ� �ִ� �� ��ü�� �ּҸ� ��� �Լ�
	GpBrush* GetStockBrush(); // ���������� �����Ǿ� �ִ� �귯�� ��ü�� �ּҸ� ��� �Լ�
	GpGraphics* GetGraphics(); // GDI+ ��� ��ü�� �ּҸ� ��� �Լ�
	GpImage* GetStockImage(); // GDI+ ��¿� ����ϴ� �̹��� ��ü�� �ּҸ� ��� �Լ�
	int GetWidth(); // ��� ������ ���� ��� �Լ�
	int GetHeight(); // ��� ������ ���̸� ��� �Լ�
};

#endif // !_TXYC_GDIP_H_