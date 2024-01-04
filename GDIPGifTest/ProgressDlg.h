#pragma once
#include "afxdialogex.h"
#include "TXYC_FrameThreadData.h"
#include "TXYC_CustomProgress.h"

#pragma comment(lib, "TXYC_CustomProgress.lib")

// ProgressDlg 대화 상자
class ProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(ProgressDlg)
private:
	// 사용자 정의 프로그레스 바
	TXYC_CustomProgress m_work_progress;
	// 메인 다이얼로그로부터 받아올 스레드 데이터의 주소
	FrameThreadData* mp_frame_thread_data = NULL;
	// 프로그레스 바의 전체 작업량
	int m_work_size = 0;
public:
	ProgressDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ProgressDlg();
	// 프로그레스 바의 전체 작업량을 설정하는 함수
	void SetWorkSize(int a_work_size);
	// 메인 다이얼로그로부터 스레드 데이터의 주소를 받아오는 함수
	void SetThreadDataPtr(FrameThreadData* ap_data);
	// 메인 다이얼로그에게 스레드 데이터의 주소를 전달하는 함수
	FrameThreadData* GetThreadDataPtr();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DLG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT On20000(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On20001(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedAbortBtn();
};
