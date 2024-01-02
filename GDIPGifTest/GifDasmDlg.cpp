// GifDasmDlg.cpp: 구현 파일
#include "pch.h"
#include "GDIPGifTest.h"
#include "GifDasmDlg.h"

// GifDasmDlg 대화 상자
IMPLEMENT_DYNAMIC(GifDasmDlg, CDialog)

GifDasmDlg::GifDasmDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GIF_DASM_DLG, pParent)
{

}

GifDasmDlg::~GifDasmDlg()
{
}

void GifDasmDlg::LoadGif(const wchar_t* ap_path)
{
	mp_gif_image = m_gdip.LoadImage(ap_path); // GIF 이미지를 읽어옴
	// 읽어들인 GIF 이미지 프레임의 폭과 높이를 저장함
	GdipGetImageWidth(mp_gif_image, &m_gif_width);
	GdipGetImageHeight(mp_gif_image, &m_gif_height);
	// 프레임 배열의 수를 얻어옴((GIF 이미지의 전체 프레임 수) = (프레임 인덱스) * (애니메이션 개수))
	GdipImageGetFrameDimensionsCount(mp_gif_image, &m_dim_count);
	// 프레임 배열에 대한 GUID를 저장할 메모리를 할당함
	mp_id_list = new GUID[m_dim_count];
	// 프레임 그룹에 대한 GUID를 얻음
	GdipImageGetFrameDimensionsList(mp_gif_image, mp_id_list, m_dim_count);
	// 현재 프레임 그룹에 대한 프레임 개수를 얻음
	GdipImageGetFrameCount(mp_gif_image, mp_id_list, &m_frame_count);
	// GIF 이미지의 모든 프레임을 저장하기 위한 배열 생성
	// mp_frame_list = new GpBitmap * [m_frame_count];
	// 프레임간의 지연 시간이 포함되어 있는 정보의 크기를 얻음
	GdipGetPropertyItemSize(mp_gif_image, PropertyTagFrameDelay, &m_total_size);
	// 지연 시간 정보를 저장하기 위한 메모리를 할당함
	mp_item = (PropertyItem*)malloc(m_total_size);
	// 지연 시간 정보를 얻어옴
	GdipGetPropertyItem(mp_gif_image, PropertyTagFrameDelay, m_total_size, mp_item);
	// 프레임 지연 시간 목록을 얻어옴
	mp_delay_list = (UINT*)mp_item[0].value;
	// GpGraphics* p_graphics = NULL;
	// PixelFormat image_format;
	// GdipGetImagePixelFormat(mp_gif_image, &image_format);
	GUID guid = FrameDimensionTime;
	// 프레임 개수만큼 썸네일 이미지를 생성하여 썸네일 리스트 박스에 추가
	for (UINT i = 0; i < m_frame_count; ++i)
	{
		// i번쨰 프레임을 선택함
		GdipImageSelectActiveFrame(mp_gif_image, &guid, i);
		// 현재 프레임에 대한 썸네일 이미지를 생성해서 썸네일 리스트 박스에 추가
		m_thumbnail_list.InsertThumbnailImage(mp_gif_image);
		// 프레임 목록에 현재 프레임을 추가
		/*
		GdipCreateBitmapFromScan0(m_gif_width, m_gif_height, 0, image_format, NULL, (mp_frame_list + i));
		GdipGetImageGraphicsContext(mp_frame_list[i], &p_graphics);
		GdipDrawImageI(p_graphics, mp_gif_image, 0, 0);
		GdipDeleteGraphics(p_graphics);
		*/
	}
	// 출력 공간의 좌측 상단 좌표랑 일치하는 좌측 상단 좌표와 설정한 좌측 상단 좌표에다가
	// GIF 이미지 프레임의 원래 폭과 높이가 적용된 우측 하단 좌표로 구성된 사각형 좌표를
	// 출력 공간 안에 그려질 GIF 이미지 프레임 좌표의 초기 좌표로 설정함
	m_gif_display_rect.left = m_frame_viewer_rect.left;
	m_gif_display_rect.top = m_frame_viewer_rect.top;
	m_gif_display_rect.right = m_gif_display_rect.left + m_gif_width - 1;
	m_gif_display_rect.bottom = m_gif_display_rect.top + m_gif_height - 1;
	// 출력 공간 안에 그려질 GIF 이미지 프레임이 폭과 높이 비율을 유지한 상태로 출력 공간의 영역을
	// 정확하게 채우면서 출력 공간의 정중앙에 출력되도록 출력 공간 안에 그려질 GIF 이미지 프레임의
	// 좌표를 조정함
	CenterFitRectInRect(&m_gif_display_rect, &m_frame_viewer_rect);
	m_frame_index = 0; // 애니메이션은 0번 프레임부터 시작
	// 현재 프레임 인덱스에 해당하는 프레임을 선택함
	GdipImageSelectActiveFrame(mp_gif_image, &guid, m_frame_index);
	// 썸네일 리스트 박스에 현재 재생 위치를 지정함
	m_thumbnail_list.SetCurSel(m_frame_index);
	DrawClientImage();
}

void GifDasmDlg::DestroyGif()
{
	// 파일 확장자와 파일 경로를 제거한 파일 이름 문자열 제거
	memset(m_gif_title, 0, sizeof(m_gif_title));
	// 읽어들인 GIF 이미지의 프레임 썸네일들이 저장되어 있는 썸네일 리스트 박스의 모든 항목 제거
	m_thumbnail_list.ResetContent();
	m_gdip.DestroyImage(mp_gif_image); // 읽어들인 GIF 이미지 제거
	mp_gif_image = NULL;
	/*
	for (int i = 0; i < m_frame_count; ++i)
	{
		GdipDisposeImage(mp_frame_list[i]);
	}
	delete[] mp_frame_list; // 읽어들인 GIF 이미지의 모든 프레임을 저장해두었던 메모리 해제
	mp_frame_list = NULL;
	*/
	delete[] mp_id_list; // GUID 목록을 저장하기 위해 사용했던 메모리 해제
	mp_id_list = NULL;
	free(mp_item); // 프레임 정보를 저장하기 위해 사용했던 메모리 해제
	mp_item = NULL;
}

void GifDasmDlg::DrawClientImage()
{
	m_gdip.Clear(RGB24(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B));
	int x = m_frame_viewer_rect.left;
	int y = m_frame_viewer_rect.top;
	UINT width = m_frame_viewer_rect.right - m_frame_viewer_rect.left - 1;
	UINT height = m_frame_viewer_rect.bottom - m_frame_viewer_rect.top - 1;
	// 프레임 출력 공간 전체를 지정한 색상으로 채움
	m_gdip.FillRect(x, y, width, height, 0xFF00FF00);
	m_gdip.DrawRect(x, y, width, height, 0xFF00FF00);
	// 읽어들인 GIF 이미지가 있다면 현재 선택된 프레임을 프레임 출력 공간에 출력함
	if (mp_gif_image)
	{
		x = m_gif_display_rect.left;
		y = m_gif_display_rect.top;
		width = m_gif_display_rect.right - m_gif_display_rect.left;
		height = m_gif_display_rect.bottom - m_gif_display_rect.top;
		m_gdip.Draw(mp_gif_image, x, y, width, height);
		// m_gdip.Draw(mp_frame_list[m_frame_index], x, y, width, height);
	}
}

void GifDasmDlg::UpdateCurrentFrame()
{
	GUID guid = FrameDimensionTime;
	// 현재 프레임 인덱스에 해당하는 프레임을 선택함
	GdipImageSelectActiveFrame(mp_gif_image, &guid, m_frame_index);
	// 선택한 프레임을 화면에 출력함
	DrawClientImage();
	Invalidate(FALSE);
}

void GifDasmDlg::SetFrameSaveCommand()
{
	// 마지막으로 사용됐거나 현재 사용중인 메시지의 정보가 담긴 주소를 얻어옴(이 함수는 스플릿 버튼의
	// 메뉴가 선택됐을 때 호출되므로 WM_LBUTTONUP 메시지 정보가 담긴 주소를 얻게 됨)
	MSG* p_msg = ::AfxGetCurrentMessage();
	// 선택된 스플릿 버튼 메뉴의 ID를 얻음(WM_LBUTTONUP 메시지 정보 안 wParam 항목의 하위 16비트)
	m_last_save_cmd = (UINT16)p_msg->wParam;
	CString str;
	CMenu menu, * p_sub_menu = NULL;
	// 스플릿 버튼과 연결된 메뉴 그룹이 포함되어 있는 메뉴 리소스를 읽어옴
	menu.LoadMenu(IDR_FRAME_CTRL_MENU);
	p_sub_menu = menu.GetSubMenu(0); // 스플릿 버튼과 연결된 메뉴의 정보를 얻음
	// 선택된 스플릿 버튼 메뉴의 이름을 얻음
	p_sub_menu->GetMenuString(m_last_save_cmd, str, MF_BYCOMMAND);
	// 얻은 메뉴의 이름을 스플릿 버튼 이름으로 설정함
	m_save_frame_btn.SetWindowText(str);
	menu.DestroyMenu(); // 읽어들인 메뉴 제거
}

void GifDasmDlg::SaveFrame()
{
	wchar_t save_name[MAX_PATH] = { 0 };
	// 저장할 GIF 이미지 프레임의 파일 이름을 '(파일 이름)_(프레임 인덱스).png'로 설정함
	swprintf_s(save_name, L"%s_%04d.png", m_gif_title, m_frame_index);
	CLSID cls_id;
	// PNG 인코더의 CLSID 값을 얻음
	m_gdip.GetEncoderClsid(L"image/png", &cls_id);
	// 현재 출력중인 프레임을 PNG 파일로 저장함
	wchar_t ext_filter[] = L"PNG 이미지(*.png)|*.png||";
	DWORD file_dlg_flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	CFileDialog save_dlg(FALSE, L"png", save_name, file_dlg_flags, ext_filter);
	// 파일 다이얼로그의 기본 경로를 바탕화면으로 설정
	wchar_t init_path[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(NULL, init_path, CSIDL_DESKTOP, FALSE);
	save_dlg.m_ofn.lpstrInitialDir = init_path;
	if (save_dlg.DoModal() == IDOK)
	{
		GdipSaveImageToFile(mp_gif_image, save_dlg.GetPathName(), &cls_id, NULL);
	}
}

void GifDasmDlg::SaveAllFrames()
{
	GUID guid = FrameDimensionTime;
	CLSID cls_id;
	// PNG 인코더의 CLSID 값을 얻음
	m_gdip.GetEncoderClsid(L"image/png", &cls_id);
	wchar_t save_path[MAX_PATH] = { 0 }, save_name[MAX_PATH] = { 0 }, init_path[MAX_PATH] = { 0 };
	// 읽어들인 GIF 이미지의 모든 프레임을 보관할 폴더 생성 위치를 폴더 선택 다이얼로그를 통해 설정함
	CFolderPickerDialog folder_dlg(NULL, OFN_PATHMUSTEXIST);
	// 폴더 다이얼로그의 기본 경로를 바탕화면으로 설정
	::SHGetSpecialFolderPath(NULL, init_path, CSIDL_DESKTOP, FALSE);
	folder_dlg.m_ofn.lpstrInitialDir = init_path;
	if (folder_dlg.DoModal() == IDOK)
	{
		// '(폴더 선택 다이얼로그를 통해 설정한 경로)\\(확장자와 경로가 제거된 GIF 이미지 이름)' 위치를
		// GIF 이미지 프레임들이 저장될 위치로 설정함((폴더 선택 다이얼로그를 통해 설정한 경로)에다가
		// 폴더 이름이 (확장자와 경로가 제거된 GIF 이미지 이름)인 폴더를 만들어야 함)
		int path_len = swprintf_s(save_path, L"%s\\%s", folder_dlg.GetPathName(), m_gif_title);
		// 저장 위치 문자열 끝에다가 저장할 GIF 이미지의 파일 이름을 붙인 문자열이 GIF 이미지 프레임들의
		// 실제 저장 경로이기 때문에 저장 위치 문자열의 끝부분을 가리키는 포인터를 선언해둠
		wchar_t* p = save_path + path_len;
		// (폴더 선택 다이얼로그를 통해 설정한 경로) 안에 폴더 이름이 (확장자와 경로가 제거된 GIF 이미지 이름)인
		// 폴더를 생성함
		::CreateDirectory(save_path, NULL);
		for (int i = 0; i < m_frame_count; ++i)
		{
			// 저장할 GIF 이미지 프레임의 파일 이름을 '(파일 이름)_(프레임 인덱스).png'로 설정함
			path_len = swprintf_s(save_name, L"\\%s_%04d.png", m_gif_title, i);
			// 저장 위치 문자열 끝에 저장할 GIF 이미지의 파일 이름을 복사해넣음
			memcpy_s(p, MAX_PATH, save_name, (path_len + 1) << 1);
			// 저장할 GIF 이미지 프레임을 선택해서 저장함
			GdipImageSelectActiveFrame(mp_gif_image, &guid, i);
			GdipSaveImageToFile(mp_gif_image, save_path, &cls_id, NULL);
		}
		// GIF 이미지의 모든 프레임들을 저장하는 작업이 끝나면 원래 선택되어 있던 프레임 인덱스에 해당하는
		// 프레임을 다시 선택함
		GdipImageSelectActiveFrame(mp_gif_image, &guid, m_frame_index);
		AfxMessageBox(L"GIF 이미지의 모든 프레임 저장 완료");
	}
}

void GifDasmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAVE_FRAME_BTN, m_save_frame_btn);
}

BEGIN_MESSAGE_MAP(GifDasmDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &GifDasmDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GifDasmDlg::OnBnClickedCancel)
	ON_MESSAGE(TABWND_CLOSE_READY, &GifDasmDlg::On15002)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_THUMBNAIL_LIST, &GifDasmDlg::OnLbnSelchangeThumbnailList)
	ON_BN_CLICKED(IDC_PLAY_GIF_BTN, &GifDasmDlg::OnBnClickedPlayGifBtn)
	ON_BN_CLICKED(IDC_SAVE_FRAME_BTN, &GifDasmDlg::OnBnClickedSaveFrameBtn)
	ON_COMMAND(ID_SAVE_FRAME, &GifDasmDlg::OnSaveFrame)
	ON_COMMAND(ID_SAVE_ALL_FRAMES, &GifDasmDlg::OnSaveAllFrames)
	ON_BN_CLICKED(IDC_FIND_GIF_BTN, &GifDasmDlg::OnBnClickedFindGifBtn)
END_MESSAGE_MAP()

// GifDasmDlg 메시지 처리기
BOOL GifDasmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect r;
	GetClientRect(&r);
	// 클라이언트 영역의 크기와 동일한 크기의 GDI+ 출력 객체를 생성함
	m_gdip.CreateGDIP(r.Width(), r.Height());
	// 프레임 저장 스플릿 버튼은 IDR_FRAME_CTRL_MENU 메뉴의 첫 번째 그룹을 사용함
	m_save_frame_btn.SetDropDownMenu(IDR_FRAME_CTRL_MENU, 0);
	m_thumbnail_list.SubclassDlgItem(IDC_THUMBNAIL_LIST, this);
	// 썸네일 리스트 박스의 다이얼로그 기준 좌표를 구함
	m_thumbnail_list.GetWindowRect(&r);
	ScreenToClient(&r);
	// 썸네일 리스트 박스의 다이얼로그 기준 좌측 좌표를 다이얼로그 가이드라인과 다이얼로그 테두리
	// 사이의 간격으로 설정함
	m_dlg_guide_spacing = r.left;
	m_thumb_list_height = r.Height(); // 썸네일 리스트 박스의 높이를 저장해둠
	// GIF 이미지 프레임 출력 공간의 다이얼로그 기준 좌표를 구함
	GetDlgItem(IDC_FRAME_VIEWER_RECT)->GetWindowRect(&m_frame_viewer_rect);
	ScreenToClient(&m_frame_viewer_rect);
	// 썸네일 리스트 박스의 상단 좌표와 GIF 이미지 프레임 출력 공간의 하단 좌표 사이의 차이를
	// 프레임 출력 영역과 썸네일 리스트 박스 사이의 간격으로 설정함
	m_frame_viewer_spacing = r.top - m_frame_viewer_rect.bottom;
	DrawClientImage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void GifDasmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	m_gdip.Draw(dc.m_hDC);
}

void GifDasmDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void GifDasmDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

BOOL GifDasmDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 이 다이얼로그에서 마우스 클릭이 발생했음을 부모 다이얼로그에게 알림
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		::PostMessage(GetParent()->m_hWnd, TABWND_LBTNDOWN, 0, 0);
	}
	// 이 다이얼로그에서 'Ctrl + Tab'이나 'Ctrl + Shift + Tab' 키 입력이 발생했음을
	// 부모 다이얼로그에게 알림(WM_KEYDOWN 메시지의 lParam 매개 변수 30번 비트가 0이면 메시지를
	// 발생시킨 키가 이전에 입력되지 않은 상태로 메시지가 발생했다는 의미이고 WM_KEYDOWN 메시지의
	// lParam 매개 변수 30번 비트가 1이면 메시지를 발생시킨 키가 이전에 입력된 상태로 메시지가
	// 발생했다는 의미임)
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && !((pMsg->lParam >> 30) & 1) &&
		(::GetAsyncKeyState(VK_CONTROL) & 0x8000))
	{
		// 'Ctrl + Shift + Tab' 키 입력이 발생했다면 이전 탭으로 이동
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			::PostMessage(GetParent()->m_hWnd, TABWND_CHANGETAB, 1, 0);
		}
		// 'Ctrl + Tab' 키 입력이 발생했다면 다음 탭으로 이동
		else
		{
			::PostMessage(GetParent()->m_hWnd, TABWND_CHANGETAB, 0, 0);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

afx_msg LRESULT GifDasmDlg::On15002(WPARAM wParam, LPARAM lParam)
{
	// 메인 다이얼로그에게 이 다이얼로그의 종료 준비 상태를 알림 메시지를 보냄(이 다이얼로그에서는
	// 추가 스레드를 사용하지 않기 때문에 항상 종료할 준비가 되어 있음)
	::PostMessage(GetParent()->m_hWnd, TABWND_CLOSE_READY, 1, 0);
	return 0;
}

void GifDasmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_thumbnail_list.m_hWnd && cx && cy)
	{
		m_gdip.CreateGDIP(cx, cy);
		int x = 0, y = 0, width = 0, height = 0;
		// 썸네일 리스트 박스의 좌표를 조정함
		x = m_dlg_guide_spacing;
		y = cy - m_thumb_list_height - m_dlg_guide_spacing;
		width = cx - m_dlg_guide_spacing * 2;
		height = m_thumb_list_height;
		m_thumbnail_list.SetWindowPos(NULL, x, y, width, height, 0);
		// GIF 이미지 프레임 출력 공간의 좌표를 조정함(이동시킬 필요 없이 크기만 조정)
		width = cx - m_frame_viewer_rect.left - m_dlg_guide_spacing - 1;
		height = cy - m_thumb_list_height - m_dlg_guide_spacing * 2 - m_frame_viewer_spacing - 1;
		GetDlgItem(IDC_FRAME_VIEWER_RECT)->SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE);
		GetDlgItem(IDC_FRAME_VIEWER_RECT)->GetWindowRect(&m_frame_viewer_rect);
		ScreenToClient(&m_frame_viewer_rect);
		// 변경된 출력 공간의 크기에 맞추어 출력 공간 안에 그려질 GIF 이미지 프레임의 좌표를 조정함
		if (mp_gif_image)
		{
			m_gif_display_rect.left = m_frame_viewer_rect.left;
			m_gif_display_rect.top = m_frame_viewer_rect.top;
			m_gif_display_rect.right = m_gif_display_rect.left + m_gif_width;
			m_gif_display_rect.bottom = m_gif_display_rect.top + m_gif_height;
			CenterFitRectInRect(&m_gif_display_rect, &m_frame_viewer_rect);
		}
		DrawClientImage();
		Invalidate(FALSE);
	}
}

void GifDasmDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	wchar_t temp_path[MAX_PATH] = { 0 };
	// 한 번의 드롭으로 한 개의 GIF 이미지만 드롭할 수 있도록 설정함
	if (::DragQueryFile(hDropInfo, -1, NULL, 0) > 1)
	{
		AfxMessageBox(L"한 개의 항목만 드롭할 수 있습니다.");
	}
	else
	{
		int path_len = ::DragQueryFile(hDropInfo, 0, temp_path, MAX_PATH);
		// 디렉토리는 드롭할 수 없도록 설정함
		if ((::GetFileAttributes(temp_path) & FILE_ATTRIBUTE_DIRECTORY) >> 4)
		{
			AfxMessageBox(L"폴더는 드롭할 수 없습니다.");
		}
		else
		{
			// 드롭된 파일의 경로 문자열에서 파일 확장자 앞 '.' 문자와 파일 이름 앞 '\\' 문자를 찾을 때
			// 파일 경로 문자열의 가장 끝에 있는 '.' 문자와 '\\' 문자를 찾아야 하므로 문자 탐색에
			// 사용할 포인터들을 문자열의 끝에 위치시킴
			wchar_t* p_ext = temp_path + path_len, * p_title = p_ext;
			wchar_t file_ext[16] = { 0 };
			// 파일 확장자 앞 '.' 문자의 위치를 찾아냄
			while (*p_ext != L'.')
			{
				--p_ext;
			}
			// 파일 확장자만 추출할 수 있도록 파일 확장자 앞 '.' 문자의 다음 위치로 이동함
			++p_ext;
			wcscpy_s(file_ext, p_ext); // 파일 확장자 추출
			// 추출한 파일 확장자를 모두 소문자로 변환함(파일 확장자가 gif인지 확인할 때 파일 확장자에
			// 대문자가 있는지 확인해볼 필요성을 없애버림)
			_wcslwr_s(file_ext);
			// GIF 이미지 파일이 아닌 파일은 드롭할 수 없도록 설정함
			if (wcscmp(file_ext, L"gif"))
			{
				AfxMessageBox(L"GIF 이미지만 드롭할 수 있습니다.");
			}
			else
			{
				// 읽어들인 GIF 이미지를 재생중이었다면 재생을 중지함
				if (m_play_flag)
				{
					OnBnClickedPlayGifBtn();
				}
				// 읽어들인 GIF 이미지가 존재한다면 해당 GIF 이미지를 제거함
				if (mp_gif_image)
				{
					DestroyGif();
				}
				// 파일 이름 앞 '\\' 문자의 위치를 찾아냄
				while (*p_title != L'\\')
				{
					--p_title;
				}
				// 파일 확장자와 파일 경로를 제거한 파일 이름만 추출할 수 있도록 파일 이름 앞 '\\' 문자의
				// 다음 위치로 이동함
				++p_title;
				// 파일 확장자와 파일 경로가 제거된 파일 이름 추출
				memcpy_s(m_gif_title, MAX_PATH, p_title, (p_ext - p_title - 1) << 1);
				LoadGif(temp_path); // 드롭된 GIF 이미지를 읽어들임
				Invalidate(FALSE);
			}
		}
	}
	CDialog::OnDropFiles(hDropInfo);
}

void GifDasmDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 읽어들인 GIF 이미지를 재생중이었다면 재생을 중지함
	if (m_play_flag)
	{
		OnBnClickedPlayGifBtn();
	}
	// 읽어들인 GIF 이미지가 존재한다면 해당 GIF 이미지를 제거함
	if (mp_gif_image)
	{
		DestroyGif();
	}
}

void GifDasmDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		// 현재 프레임의 지연 시간이 이전 프레임의 지연 시간과 다를 수 있기 때문에 이전 프레임의
		// 지연 시간에 맞게 실행중인 타이머를 중지시킴
		KillTimer(1);
		// 현재 출력할 프레임에 대응하는 썸네일 리스트 박스의 항목을 선택함
		m_thumbnail_list.SetCurSel(m_frame_index);
		// 현재 출력할 프레임을 화면에 출력함
		UpdateCurrentFrame();
		// 읽어들인 GIF 이미지로부터 얻어온 프레임 지연 시간의 1은 1ms가 아닌 10ms이기 때문에
		// 얻어온 프레임 지연 시간에 10을 곱한 값이 실제 프레임 지연 시간
		SetTimer(1, mp_delay_list[m_frame_index] * 10, NULL);
		// 다음 프레임으로 이동함
		m_frame_index = (m_frame_index + 1) % m_frame_count;
	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}
}

void GifDasmDlg::OnLbnSelchangeThumbnailList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 썸네일 리스트 박스에서 선택한 항목 인덱스가 GIF 이미지의 현재 프레임 인덱스와 다르다면
	// 출력할 프레임을 변경함
	if (m_thumbnail_list.GetCurSel() != m_frame_index)
	{
		// 썸네일 리스트 박스에서 선택한 항목 인덱스를 출력할 프레임 인덱스로 변경
		m_frame_index = m_thumbnail_list.GetCurSel();
		// 변경된 프레임을 화면에 출력
		UpdateCurrentFrame();
	}
}

void GifDasmDlg::OnBnClickedPlayGifBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!mp_gif_image)
	{
		return;
	}
	if (!m_play_flag) // GIF 이미지 재생을 실행시킴
	{
		GetDlgItem(IDC_PLAY_GIF_BTN)->SetWindowText(L"중지");
		m_play_flag = 1;
		// 읽어들인 GIF 이미지로부터 얻어온 프레임 지연 시간의 1은 1ms가 아닌 10ms이기 때문에
		// 얻어온 프레임 지연 시간에 10을 곱한 값이 실제 프레임 지연 시간
		SetTimer(1, mp_delay_list[m_frame_index] * 10, NULL);
		// 다음 프레임으로 이동함
		m_frame_index = (m_frame_index + 1) % m_frame_count;
	}
	else // GIF 이미지 재생을 중지시킴
	{
		GetDlgItem(IDC_PLAY_GIF_BTN)->SetWindowText(L"재생");
		m_play_flag = 0;
		KillTimer(1);
	}
}

void GifDasmDlg::OnBnClickedSaveFrameBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 읽어들인 GIF 이미지가 없는 상태라면 프레임 저장 작업을 수행하지 않음
	if (!mp_gif_image)
	{
		return;
	}
	// 읽어들인 GIF 이미지를 재생중이었다면 재생을 중지함
	if (m_play_flag)
	{
		OnBnClickedPlayGifBtn();
	}
	switch (m_last_save_cmd)
	{
	case ID_SAVE_FRAME:
		SaveFrame();
		break;
	case ID_SAVE_ALL_FRAMES:
		SaveAllFrames();
		break;
	}
}

void GifDasmDlg::OnSaveFrame()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetFrameSaveCommand();
}

void GifDasmDlg::OnSaveAllFrames()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetFrameSaveCommand();
}

void GifDasmDlg::OnBnClickedFindGifBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 읽어들인 GIF 이미지를 재생중이었다면 재생을 중지함
	if (m_play_flag)
	{
		OnBnClickedPlayGifBtn();
	}
	wchar_t ext_filter[] = L"GIF 이미지(*.gif)|*.gif||", init_path[MAX_PATH] = { 0 };
	CFileDialog load_dlg(TRUE, L"gif", NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, ext_filter);
	// 파일 다이얼로그의 기본 경로를 바탕화면으로 설정
	::SHGetSpecialFolderPath(NULL, init_path, CSIDL_DESKTOP, FALSE);
	load_dlg.m_ofn.lpstrInitialDir = init_path;
	if (load_dlg.DoModal() == IDOK)
	{
		// 읽어들인 GIF 이미지가 존재한다면 해당 GIF 이미지를 제거함
		if (mp_gif_image)
		{
			DestroyGif();
		}
		// 파일 확장자와 파일 경로가 제거된 파일 이름 추출
		wcscpy_s(m_gif_title, load_dlg.GetFileTitle());
		LoadGif(load_dlg.GetPathName()); // 드롭된 GIF 이미지를 읽어들임
		Invalidate(FALSE);
	}
}
