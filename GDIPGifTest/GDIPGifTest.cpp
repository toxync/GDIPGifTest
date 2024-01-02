// GDIPGifTest.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.

#include "pch.h"
#include "framework.h"
#include "GDIPGifTest.h"
#include "GDIPGifTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGDIPGifTestApp
BEGIN_MESSAGE_MAP(CGDIPGifTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CGDIPGifTestApp 생성
CGDIPGifTestApp::CGDIPGifTestApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CGDIPGifTestApp 개체입니다.
CGDIPGifTestApp theApp;

// CGDIPGifTestApp 초기화
BOOL CGDIPGifTestApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
	CGDIPGifTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}
