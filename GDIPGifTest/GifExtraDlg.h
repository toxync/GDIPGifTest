#pragma once
#include "afxdialogex.h"
#include "TXYC_CaptionControl.h"

// GifExtraDlg 대화 상자
class GifExtraDlg : public CDialog
{
	DECLARE_DYNAMIC(GifExtraDlg)
private:
public:
	GifExtraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GifExtraDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIF_EXTRA_DLG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT On15002(WPARAM wParam, LPARAM lParam);
};
