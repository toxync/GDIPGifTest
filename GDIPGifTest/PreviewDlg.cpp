// PreviewDlg.cpp: 구현 파일
#include "pch.h"
#include "GDIPGifTest.h"
#include "afxdialogex.h"
#include "PreviewDlg.h"

// PreviewDlg 대화 상자
IMPLEMENT_DYNAMIC(PreviewDlg, CDialog)

PreviewDlg::PreviewDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PREVIEW_DLG, pParent)
{

}

PreviewDlg::~PreviewDlg()
{
}

void PreviewDlg::SetPreviewBitmap(GpBitmap* ap_bmp)
{
	mp_preview_bmp = ap_bmp;
	GdipGetImageWidth(mp_preview_bmp, &m_cx);
	GdipGetImageHeight(mp_preview_bmp, &m_cy);
}

void PreviewDlg::CenterFitBitmap()
{
	m_preview_bmp_rect.left = m_client_rect.left;
	m_preview_bmp_rect.top = m_client_rect.top;
	m_preview_bmp_rect.right = m_cx;
	m_preview_bmp_rect.bottom = m_cy;
	CenterFitRectInRect(&m_preview_bmp_rect, &m_client_rect);
}

void PreviewDlg::DrawClientImage()
{
	m_gdip.Clear(0xFF00FF00);
	int x = m_preview_bmp_rect.left;
	int y = m_preview_bmp_rect.top;
	UINT width = m_preview_bmp_rect.right - m_preview_bmp_rect.left;
	UINT height = m_preview_bmp_rect.bottom - m_preview_bmp_rect.top;
	m_gdip.Draw(mp_preview_bmp, x, y, width, height);
}

void PreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(PreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &PreviewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &PreviewDlg::OnBnClickedCancel)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// PreviewDlg 메시지 처리기
BOOL PreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetClientRect(&m_client_rect);
	CenterFitBitmap();
	DrawClientImage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void PreviewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	m_gdip.Draw(dc.m_hDC);
}

void PreviewDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void PreviewDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

void PreviewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// ESC 키의 입력으로 윈도우를 닫는 동작은 금지하지만 닫기 버튼이나 'ALT + F4' 단축키의 입력으로
	// 윈도우를 닫는 동작은 허용함
	if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDOK);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void PreviewDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (mp_preview_bmp)
	{
		m_gdip.DestroyImage(mp_preview_bmp);
		mp_preview_bmp = NULL;
	}
}

void PreviewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (cx && cy)
	{
		m_gdip.CreateGDIP(cx, cy);
		m_client_rect.right = cx;
		m_client_rect.bottom = cy;
		CenterFitBitmap();
		DrawClientImage();
		Invalidate(FALSE);
	}
}
