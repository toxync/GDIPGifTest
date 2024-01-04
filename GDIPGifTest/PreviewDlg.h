#pragma once
#include "afxdialogex.h"
#include "TXYC_RectPositionFunc.h"

// PreviewDlg 대화 상자
class PreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(PreviewDlg)
private:
	TXYC_GDIP m_gdip;
	GpBitmap* mp_preview_bmp = NULL;
	RECT m_client_rect = { 0 };
	RECT m_preview_bmp_rect = { 0 };
	UINT m_cx = 0, m_cy = 0;
public:
	PreviewDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~PreviewDlg();
	void SetPreviewBitmap(GpBitmap* ap_bmp);
	void CenterFitBitmap();
	void DrawClientImage();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PREVIEW_DLG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
