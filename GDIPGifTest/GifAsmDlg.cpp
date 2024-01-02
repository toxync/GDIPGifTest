// GifAsmDlg.cpp: 구현 파일
#include "pch.h"
#include "GDIPGifTest.h"
#include "GifAsmDlg.h"

DWORD WINAPI CreateFrameBitmaps(void* ap_data)
{
	FrameThreadData* p_data = (FrameThreadData*)ap_data;
	vector<FrameData>* p_frame_vector = p_data->p_frame_vector;
	GpBitmap* p_src_bmp = NULL;
	GpBitmap** p_frame_bmp = NULL;
	GpGraphics* p_graphics = NULL;
	RECT image_rect = { 0 }, frame_rect = { 0 };
	UINT src_cx = 0, src_cy = 0, resized_src_cx = 0, resized_src_cy = 0, kill_flag = 0;
	UINT frame_cx = p_data->frame_cx, frame_cy = p_data->frame_cy;
	int x = 0, y = 0;
	for (int i = 0; i < p_frame_vector->size(); ++i)
	{
		// 이벤트 객체의 값이 1이면 스레드 종료
		if (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0)
		{
			kill_flag = 1;
			// 스레드가 강제로 종료됐다는 사실을 작업 상태 표시 다이얼로그에게 알림
			::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
			break;
		}
		p_src_bmp = (*p_frame_vector)[i].p_src_bmp;
		p_frame_bmp = &((*p_frame_vector)[i].p_frame_bmp);
		GdipGetImageWidth(p_src_bmp, &src_cx);
		GdipGetImageHeight(p_src_bmp, &src_cy);
		// 원본 이미지의 폭과 높이가 GIF 기준 프레임의 폭과 높이랑 동일하다면 원본 이미지를 그대로 복제한
		// 프레임 이미지를 생성함
		if ((frame_cx == src_cx) && (frame_cy == src_cy))
		{
			GdipCloneImage(p_src_bmp, (GpImage**)p_frame_bmp);
		}
		else // 원본 이미지의 폭과 높이가 GIF 기준 프레임의 폭과 높이랑 동일하지 않을 경우
		{
			// GIF 기준 프레임과 크기가 동일한 프레임 이미지를 생성함
			GdipCreateBitmapFromScan0(frame_cx, frame_cy, 0, PixelFormat32bppARGB, NULL, p_frame_bmp);
			// 프레임 이미지와 연결된 Graphics 객체를 생성하여 모든 픽셀을 검은색으로 초기화함
			GdipGetImageGraphicsContext(*p_frame_bmp, &p_graphics);
			// GdipSetInterpolationMode(p_graphics, InterpolationModeNearestNeighbor);
			GdipGraphicsClear(p_graphics, 0xFF000000);
			image_rect.left = image_rect.top = frame_rect.left = frame_rect.top = 0;
			image_rect.right = src_cx;
			image_rect.bottom = src_cy;
			frame_rect.right = frame_cx;
			frame_rect.bottom = frame_cy;
			// 원본 이미지를 화면비 유지한 상태로 프레임 이미지에 맞게 확대/축소하면서 가운데 정렬시켰을 때의
			// 원본 이미지 좌표와 크기를 구함
			CenterFitRectInRect(&image_rect, &frame_rect);
			x = image_rect.left;
			y = image_rect.top;
			resized_src_cx = image_rect.right - image_rect.left;
			resized_src_cy = image_rect.bottom - image_rect.top;
			// 화면비를 유지한 상태로 프레임 이미지에 맞게 확대/축소하면서 가운데 정렬시킨 원본 이미지를
			// 프레임 이미지에 출력함
			GdipDrawImageRectI(p_graphics, p_src_bmp, x, y, resized_src_cx, resized_src_cy);
			// 프레임 이미지와 연결된 Graphics 객체가 더 이상 필요 없으므로 해제함
			GdipDeleteGraphics(p_graphics);
		}
		// 프레임 이미지가 생성될 때마다 작업 하나가 완료되었다는 메시지를 작업 상태 표시 다이얼로그에게 보냄
		::SendMessage(p_data->h_progress_wnd, SINGLE_WORK_COMPLETE, 0, 0);
	}
	// 스레드 작업이 끝났으므로 스레드의 핸들을 닫음
	::CloseHandle(p_data->h_thread);
	// 스레드를 강제로 종료했으나 kill_flag의 값이 갱신되지 않았을 경우에 대비해서 이벤트 객체의 값을 다시 확인함
	// (반복문의 마지막 반복을 수행하는 중에 이벤트 객체의 값을 변경했다면 변경된 이벤트 객체의 값을 확인하지
	// 않은 상태로 반복문이 종료되어 kill_flag의 값이 갱신되지 않음)
	if ((!kill_flag) && (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0))
	{
		kill_flag = 1;
		// 스레드가 강제로 종료됐다는 사실을 작업 상태 표시 다이얼로그에게 알림
		::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
	}
	// 메인 다이얼로그에게 스레드가 종료됐음을 알림
	::PostMessage(p_data->h_main_wnd, POST_FRAME_THREAD_ACTION, kill_flag, (LPARAM)p_data);
	return 0;
}

DWORD WINAPI ConvertFrameBitmaps(void* ap_data)
{
	FrameThreadData* p_data = (FrameThreadData*)ap_data;
	vector<FrameData>* p_frame_vector = p_data->p_frame_vector;
	OctreeQuantizer oct_qtzr;
	GpBitmap* p_src_bmp = NULL;
	UINT frame_cx = p_data->frame_cx, frame_cy = p_data->frame_cy, kill_flag = 0;
	for (int i = 0; i < p_frame_vector->size(); ++i)
	{
		// 이벤트 객체의 값이 1이면 스레드 종료
		if (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0)
		{
			kill_flag = 1;
			// 스레드가 강제로 종료됐다는 사실을 작업 상태 표시 다이얼로그에게 알림
			::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
			break;
		}
		// 프레임 이미지의 8비트 컬러 변환 결과 이미지를 생성함
		p_src_bmp = (*p_frame_vector)[i].p_frame_bmp;
		(*p_frame_vector)[i].p_cvrt_frame_bmp = oct_qtzr.GetQuantizedFrame(p_src_bmp, 5);
		// 프레임 이미지가 생성될 때마다 작업 하나가 완료되었다는 메시지를 작업 상태 표시 다이얼로그에게 보냄
		::SendMessage(p_data->h_progress_wnd, SINGLE_WORK_COMPLETE, 0, 0);
	}
	// 스레드 작업이 끝났으므로 스레드의 핸들을 닫음
	::CloseHandle(p_data->h_thread);
	// 스레드를 강제로 종료했으나 kill_flag의 값이 갱신되지 않았을 경우에 대비해서 이벤트 객체의 값을 다시 확인함
	// (반복문의 마지막 반복을 수행하는 중에 이벤트 객체의 값을 변경했다면 변경된 이벤트 객체의 값을 확인하지
	// 않은 상태로 반복문이 종료되어 kill_flag의 값이 갱신되지 않음)
	if ((!kill_flag) && (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0))
	{
		kill_flag = 1;
		// 스레드가 강제로 종료됐다는 사실을 작업 상태 표시 다이얼로그에게 알림
		::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
	}
	// 메인 다이얼로그에게 스레드가 종료됐음을 알림
	::PostMessage(p_data->h_main_wnd, POST_FRAME_THREAD_ACTION, kill_flag, (LPARAM)p_data);
	return 0;
}

// GifAsmDlg 대화 상자
IMPLEMENT_DYNAMIC(GifAsmDlg, CDialog)

GifAsmDlg::GifAsmDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GIF_ASM_DLG, pParent)
{

}

GifAsmDlg::~GifAsmDlg()
{
}

void GifAsmDlg::SelectFirstInsertedItem(int a_cur_idx)
{
	m_frame_idx = a_cur_idx;
	m_frame_list.SetCurSel(m_frame_idx);
	UpdateCurrentFrame();
}

void GifAsmDlg::CenterFitFrameInViewer(ItemFrameData* ap_frame_data)
{
	m_frame_display_rect.left = m_frame_viewer_rect.left;
	m_frame_display_rect.top = m_frame_viewer_rect.top;
	m_frame_display_rect.right = m_frame_display_rect.left + ap_frame_data->frame_cx;
	m_frame_display_rect.bottom = m_frame_display_rect.top + ap_frame_data->frame_cy;
	// 프레임 이미지를 화면비 유지한 상태로 프레임 출력 영역에 맞게 확대/축소하면서 가운데 정렬시켰을 때의
	// 프레임 이미지 좌표와 크기를 구함
	CenterFitRectInRect(&m_frame_display_rect, &m_frame_viewer_rect);
}

void GifAsmDlg::DrawClientImage()
{
	m_gdip.Clear(RGB24(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B));
	int x = m_frame_viewer_rect.left;
	int y = m_frame_viewer_rect.top;
	UINT width = m_frame_viewer_rect.right - m_frame_viewer_rect.left - 1;
	UINT height = m_frame_viewer_rect.bottom - m_frame_viewer_rect.top - 1;
	// 프레임 출력 공간 전체를 지정한 색상으로 채움
	m_gdip.FillRect(x, y, width, height, 0xFF00FF00);
	m_gdip.DrawRect(x, y, width, height, 0xFF00FF00);
	// 프레임 리스트 박스에 추가된 이미지가 있다면 선택된 인덱스에 해당하는 프레임 이미지를 출력함
	if (m_frame_list.GetCount())
	{
		x = m_frame_display_rect.left;
		y = m_frame_display_rect.top;
		width = m_frame_display_rect.right - m_frame_display_rect.left;
		height = m_frame_display_rect.bottom - m_frame_display_rect.top;
		m_gdip.Draw(m_frame_list.GetFrameBitmap(m_frame_idx), x, y, width, height);
	}
}

void GifAsmDlg::UpdateCurrentFrame()
{
	// 현재 선택된 프레임 이미지를 프레임 출력 영역에 맞게 확대/축소하면서 가운데 정렬시킴
	CenterFitFrameInViewer((ItemFrameData*)m_frame_list.GetItemDataPtr(m_frame_idx));
	DrawClientImage();
	Invalidate(FALSE);
}

void GifAsmDlg::SetSplitBtnCommand(int* a_last_cmd_id, int a_sub_menu_pos, CSplitButton* ap_btn)
{
	// 마지막으로 사용됐거나 현재 사용중인 메시지의 정보가 담긴 주소를 얻어옴(이 함수는 스플릿 버튼의
	// 메뉴가 선택됐을 때 호출되므로 WM_LBUTTONUP 메시지 정보가 담긴 주소를 얻게 됨)
	MSG* p_msg = ::AfxGetCurrentMessage();
	// 선택된 스플릿 버튼 메뉴의 ID를 얻음(WM_LBUTTONUP 메시지 정보 안 wParam 항목의 하위 16비트)
	*a_last_cmd_id = (UINT16)p_msg->wParam;
	CString str;
	CMenu menu, * p_sub_menu = NULL;
	// 스플릿 버튼과 연결된 메뉴 그룹이 포함되어 있는 메뉴 리소스를 읽어옴
	menu.LoadMenu(IDR_FRAME_CTRL_MENU);
	p_sub_menu = menu.GetSubMenu(a_sub_menu_pos); // 스플릿 버튼과 연결된 메뉴의 정보를 얻음
	// 선택된 스플릿 버튼 메뉴의 이름을 얻음
	p_sub_menu->GetMenuString(*a_last_cmd_id, str, MF_BYCOMMAND);
	// 얻은 메뉴의 이름을 스플릿 버튼 이름으로 설정함
	ap_btn->SetWindowText(str);
	menu.DestroyMenu(); // 읽어들인 메뉴 제거
}

void GifAsmDlg::SetFrameDelay(int a_delta)
{
	// 프레임 지연 시간이 최소값인데 감소 동작이 발생했거나 프레임 지연 시간이 최대값인데
	// 증가 동작이 발생했다면 프레임 지연 시간을 변경시키지 않음
	if (((m_frame_delay <= m_min_frame_delay) && !a_delta) ||
		((m_frame_delay >= m_max_frame_delay) && a_delta))
	{
		return;
	}
	// 증가/감소 동작에 따라 프레임 지연 시간을 변경함
	m_frame_delay += (-1 + a_delta * 2) * m_delay_spin_accel;
	SetDlgItemInt(IDC_FRAME_DELAY_EDIT, m_frame_delay);
}

void GifAsmDlg::InitFrameThread(UINT8 a_post_thread_action)
{
	FrameThreadData* p_data = new FrameThreadData;
	LPTHREAD_START_ROUTINE p_thread_routine = NULL;
	// 스레드 종료 이후 수행 동작 상태값에 맞는 스레드가 수행할 함수를 선택함
	switch (a_post_thread_action)
	{
	case POST_TRD_ACT_LOAD_FRAMES:
	case POST_TRD_ACT_RESET_BASE_FRAME:
		p_thread_routine = CreateFrameBitmaps;
		break;
	case POST_TRD_ACT_CONVERT_PREVIEW:
	case POST_TRD_ACT_CREATE_GIF:
		p_thread_routine = ConvertFrameBitmaps;
		break;
	}
	// 변환할 프레임 이미지의 개수를 작업 상태 표시 다이얼로그의 전체 작업량으로 설정함
	m_progress_dlg.SetWorkSize(m_frame_vector.size());
	// 매개 변수로 넘겨 받은 스레드 종료 이후 수행 동작 상태값을 설정함
	p_data->post_thread_action = a_post_thread_action;
	// 스레드가 종료됐을 때 메인 다이얼로그에 메시지를 보내기 위해 필요한 메인 다이얼로그의 핸들을
	// 스레드 데이터에 저장함
	p_data->h_main_wnd = m_hWnd;
	// GIF 기준 프레임의 폭과 높이를 스레드 데이터에 저장함
	p_data->frame_cx = m_frame_cx;
	p_data->frame_cy = m_frame_cy;
	// 원본 이미지들의 정보가 담긴 프레임 벡터를 스레드 데이터에 저장함
	p_data->p_frame_vector = &m_frame_vector;
	// 스레드 동기화에 사용할 이벤트 객체를 생성함
	p_data->h_kill_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	// 정지된 상태의 스레드 생성하면서 스레드 데이터를 스레드에게 넘겨줌
	p_data->h_thread = ::CreateThread(NULL, 0, p_thread_routine, p_data, CREATE_SUSPENDED, NULL);
	// 스레드의 실행 우선 순위를 높임
	::SetThreadPriority(p_data->h_thread, THREAD_PRIORITY_TIME_CRITICAL);
	// 스레드 데이터를 작업 상태 표시 다이얼로그에게 전달(작업 상태 표시 다이얼로그가 스레드를 실행시킴)
	m_progress_dlg.SetThreadDataPtr(p_data);
	// 작업 상태 표시 다이얼로그를 생성함
	m_progress_dlg.DoModal();
}

void GifAsmDlg::CreateGif()
{
	GpBitmap* p_cur_bmp = NULL;
	CLSID cls_id;
	EncoderParameters encoder_param;
	EncoderValue e_value = EncoderValueMultiFrame;
	PropertyItem property_item;
	GpImage* p_gif_image = NULL;
	short property_value = -1; // 제작될 GIF 이미지의 반복 횟수를 무제한으로 설정함
	long* p_value = NULL;
	UINT frame_count = m_frame_vector.size();
	m_gdip.GetEncoderClsid(L"image/gif", &cls_id);
	encoder_param.Count = 1;
	encoder_param.Parameter[0].Guid = EncoderSaveFlag;
	encoder_param.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoder_param.Parameter[0].NumberOfValues = 1;
	encoder_param.Parameter[0].Value = &e_value;
	// GIF 이미지에 첫 번째 프레임 삽입
	p_gif_image = m_frame_vector[0].p_cvrt_frame_bmp;
	// 제작될 GIF 이미지의 반복 횟수 설정
	property_item.id = PropertyTagLoopCount;
	property_item.length = 2;
	property_item.type = PropertyTagTypeShort;
	property_item.value = &property_value;
	GdipSetPropertyItem(p_gif_image, &property_item);
	// 제작될 GIF 이미지의 프레임 지연 시간 설정
	p_value = new long[frame_count];
	for (int i = 0; i < frame_count; ++i)
	{
		p_value[i] = m_frame_delay;
	}
	property_item.id = PropertyTagFrameDelay;
	property_item.length = sizeof(long) * frame_count;
	property_item.type = PropertyTagTypeLong;
	property_item.value = p_value;
	GdipSetPropertyItem(p_gif_image, &property_item);
	delete[] p_value;
	// 미리 설정해둔 저장 위치에 GIF 이미지 파일 생성
	GdipSaveImageToFile(p_gif_image, m_gif_save_path, &cls_id, &encoder_param);
	// GIF 이미지에 두 번째 프레임부터 마지막 프레임까지 삽입
	e_value = EncoderValueFrameDimensionTime;
	for (int i = 1; i < frame_count; ++i)
	{
		p_cur_bmp = m_frame_vector[i].p_cvrt_frame_bmp;
		GdipSaveAddImage(p_gif_image, p_cur_bmp, &encoder_param);
	}
	e_value = EncoderValueFlush;
	GdipSaveAdd(p_gif_image, &encoder_param);
	// GIF 이미지 제작이 완료됐으므로 모든 프레임 이미지를 제거함
	for (int i = 0; i < frame_count; ++i)
	{
		m_gdip.DestroyImage(m_frame_vector[i].p_cvrt_frame_bmp);
	}
	// 제작된 GIF 이미지 제거
	m_gdip.DestroyImage(p_gif_image);
	AfxMessageBox(L"GIF 이미지 제작 완료");
}

void GifAsmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHIFT_DIST_BTN, m_shift_dist_btn);
	DDX_Control(pDX, IDC_REMOVE_FRAME_BTN, m_remove_frame_btn);
}

BEGIN_MESSAGE_MAP(GifAsmDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &GifAsmDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GifAsmDlg::OnBnClickedCancel)
	ON_MESSAGE(TABWND_CLOSE_READY, &GifAsmDlg::On15002)
	ON_MESSAGE(POST_FRAME_THREAD_ACTION, &GifAsmDlg::On16000)
	ON_MESSAGE(ABORT_FRAME_THREAD, &GifAsmDlg::On17000)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_CTLCOLORSTATIC, &GifAsmDlg::OnCtlColorStatic)
	ON_LBN_SELCHANGE(IDC_FRAME_LIST, &GifAsmDlg::OnLbnSelchangeFrameList)
	ON_BN_CLICKED(IDC_PLAY_FRAME_BTN, &GifAsmDlg::OnBnClickedPlayFrameBtn)
	ON_BN_CLICKED(IDC_ADD_FRAME_IMAGE_BTN, &GifAsmDlg::OnBnClickedAddFrameImageBtn)
	ON_COMMAND(ID_SHIFT_ONE, &GifAsmDlg::OnShiftOne)
	ON_COMMAND(ID_SHIFT_FIVE, &GifAsmDlg::OnShiftFive)
	ON_COMMAND(ID_SHIFT_TEN, &GifAsmDlg::OnShiftTen)
	ON_COMMAND(ID_SHIFT_ENDPOS, &GifAsmDlg::OnShiftEndpos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRAME_SHIFT_SPIN, &GifAsmDlg::OnDeltaposFrameShiftSpin)
	ON_BN_CLICKED(IDC_SET_BASE_FRAME_BTN, &GifAsmDlg::OnBnClickedSetBaseFrameBtn)
	ON_BN_CLICKED(IDC_REVERSE_FRAME_ORDER_BTN, &GifAsmDlg::OnBnClickedReverseFrameOrderBtn)
	ON_BN_CLICKED(IDC_REMOVE_FRAME_BTN, &GifAsmDlg::OnBnClickedRemoveFrameBtn)
	ON_COMMAND(ID_REMOVE_FRAME, &GifAsmDlg::OnRemoveFrame)
	ON_COMMAND(ID_REMOVE_ALL_FRAMES, &GifAsmDlg::OnRemoveAllFrames)
	ON_BN_CLICKED(IDC_SAVE_GIF_BTN, &GifAsmDlg::OnBnClickedSaveGifBtn)
	ON_BN_CLICKED(IDC_FRAME_PREVIEW_BTN, &GifAsmDlg::OnBnClickedFramePreviewBtn)
	ON_EN_CHANGE(IDC_FRAME_DELAY_EDIT, &GifAsmDlg::OnEnChangeFrameDelayEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRAME_DELAY_SPIN, &GifAsmDlg::OnDeltaposFrameDelaySpin)
END_MESSAGE_MAP()

// GifAsmDlg 메시지 처리기
BOOL GifAsmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect r;
	GetClientRect(&r);
	// 클라이언트 영역의 크기와 동일한 크기의 GDI+ 출력 객체를 생성함
	m_gdip.CreateGDIP(r.Width(), r.Height());
	m_bk_brush.CreateSolidBrush(m_bk_color);
	// 선택된 프레임의 이동 범위 설정 스플릿 버튼은 IDR_FRAME_CTRL_MENU 메뉴의 두 번째 그룹을 사용함
	m_shift_dist_btn.SetDropDownMenu(IDR_FRAME_CTRL_MENU, 1);
	// 프레임 삭제 스플릿 버튼은 IDR_FRAME_CTRL_MENU 메뉴의 세 번째 그룹을 사용함
	m_remove_frame_btn.SetDropDownMenu(IDR_FRAME_CTRL_MENU, 2);
	m_frame_list.SubclassDlgItem(IDC_FRAME_LIST, this);
	// 프레임 리스트 박스의 다이얼로그 기준 좌표를 구함
	m_frame_list.GetWindowRect(&r);
	ScreenToClient(&r);
	// 프레임 리스트 박스의 다이얼로그 기준 좌측 좌표를 다이얼로그 가이드라인과 다이얼로그 테두리
	// 사이의 간격으로 설정함
	m_dlg_guide_spacing = r.left;
	m_frame_list_height = r.Height(); // 프레임 리스트 박스의 높이를 저장해둠
	// GIF 이미지 프레임 출력 공간의 다이얼로그 기준 좌표를 구함
	GetDlgItem(IDC_IMAGE_VIEWER_RECT)->GetWindowRect(&m_frame_viewer_rect);
	ScreenToClient(&m_frame_viewer_rect);
	// 프레임 리스트 박스의 상단 좌표와 GIF 이미지 프레임 출력 공간의 하단 좌표 사이의 차이를
	// 프레임 출력 영역과 프레임 리스트 박스 사이의 간격으로 설정함
	m_frame_viewer_spacing = r.top - m_frame_viewer_rect.bottom;
	// 프레임 지연 시간의 초기값을 설정함
	SetDlgItemInt(IDC_FRAME_DELAY_EDIT, m_frame_delay);
	// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
	swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
	SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
	DrawClientImage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void GifAsmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	m_gdip.Draw(dc.m_hDC);
}

void GifAsmDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnOK();
}

void GifAsmDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialog::OnCancel();
}

BOOL GifAsmDlg::PreTranslateMessage(MSG* pMsg)
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

afx_msg LRESULT GifAsmDlg::On15002(WPARAM wParam, LPARAM lParam)
{
	FrameThreadData* p_data = m_progress_dlg.GetThreadDataPtr();
	// 메인 다이얼로그에게 이 다이얼로그의 종료 준비 상태 알림 메시지를 보냄(이 다이얼로그에서는
	// 추가 스레드를 사용하기 때문에 스레드의 실행 상태에 따라 종료 준비 상태를 설정함)
	::PostMessage(GetParent()->m_hWnd, TABWND_CLOSE_READY, ((p_data) ? 0 : 1), 0);
	return 0;
}

afx_msg LRESULT GifAsmDlg::On16000(WPARAM wParam, LPARAM lParam)
{
	FrameThreadData* p_data = (FrameThreadData*)lParam;
	// 스레드가 종료됐으므로 이벤트 객체를 제거함
	::CloseHandle(p_data->h_kill_event);
	// 스레드가 정상적으로 종료됐다면 스레드 데이터에 명시된 스레드 종료 이후 동작을 수행함
	if (!wParam)
	{
		GpBitmap* p_src_bmp = NULL, * p_frame_bmp = NULL;
		int insert_count = m_frame_vector.size();
		switch (p_data->post_thread_action)
		{
		case POST_TRD_ACT_LOAD_FRAMES:
			// 프레임 벡터에 저장된 이미지들을 프레임 리스트 박스 항목의 추가 메모리에 저장함
			for (int i = 0; i < insert_count; ++i)
			{
				p_src_bmp = m_frame_vector[i].p_src_bmp;
				p_frame_bmp = m_frame_vector[i].p_frame_bmp;
				m_frame_list.InsertItemBitmaps(p_src_bmp, p_frame_bmp);
			}
			// 프레임 리스트 박스에 추가된 항목들 중 첫 번째 항목을 선택함
			SelectFirstInsertedItem(m_frame_list.GetCount() - insert_count);
			break;
		case POST_TRD_ACT_RESET_BASE_FRAME:
			// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
			swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
			SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
			// 프레임 벡터에 저장된 프레임 이미지들을 리스트 박스 항목들의 프레임 이미지 데이터로 설정함
			for (int i = 0; i < insert_count; ++i)
			{
				m_gdip.DestroyImage(m_frame_list.GetFrameBitmap(i));
				m_frame_list.SetFrameBitmap(i, m_frame_vector[i].p_frame_bmp);
			}
			// 변경된 GIF 기준 프레임에 맞게 바뀐 현재 프레임 이미지를 출력함
			UpdateCurrentFrame();
			break;
		case POST_TRD_ACT_CONVERT_PREVIEW:
			// 8비트 변환된 프레임 이미지를 전달 받은 미리 보기 다이얼로그를 생성함
			m_preview_dlg.SetPreviewBitmap(m_frame_vector[0].p_cvrt_frame_bmp);
			m_preview_dlg.DoModal();
			break;
		case POST_TRD_ACT_CREATE_GIF:
			// 프레임 리스트 박스에 추가된 모든 프레임 이미지들로 GIF 이미지를 제작함
			CreateGif();
			break;
		}
		// 프레임 벡터가 더 이상 필요 없으므로 저장되어 있던 데이터들을 모두 버림
		m_frame_vector.clear();
		// 스레드 종료 이후 동작까지 정상적으로 수행됐으면 스레드 데이터를 제거함
		delete p_data;
	}
	// 스레드가 정상적으로 종료되지 않았다면 스레드 강제 종료 함수가 스레드 데이터를 제거할 수 있도록 스레드
	// 데이터 안의 스레드 핸들만 무효화시킴(스레드 강제 종료 함수가 스레드 데이터의 스레드 핸들을 확인하는 도중에
	// 이 함수에서 스레드 데이터를 제거해버리면 스레드 강제 종료 함수에서 스레드 데이터의 스레드 핸들을 확인할 때
	// 문제가 발생하므로 이 함수는 스레드 강제 종료 함수에게 스레드가 종료됐음을 알리는 역할만을 수행함)
	else
	{
		p_data->h_thread = NULL;
	}
	return 0;
}

afx_msg LRESULT GifAsmDlg::On17000(WPARAM wParam, LPARAM lParam)
{
	FrameThreadData* p_data = m_progress_dlg.GetThreadDataPtr();
	MSG msg;
	GpBitmap* p_src_bmp = NULL, * p_frame_bmp = NULL;
	int insert_count = 0;
	// 작업 스레드의 이벤트 객체 상태값을 변경해서 스레드를 강제로 종료시킴
	::SetEvent(p_data->h_kill_event);
	// 작업 스레드에서 메인 스레드로 보낸 메시지가 있다면 다이얼로그의 기본 메시지 루틴이 처리하지 않고
	// 반복문 안에서 메시지를 강제로 처리함
	while (p_data->h_thread)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	switch (p_data->post_thread_action)
	{
		// 강제 종료된 작업 스레드의 종료 이후 동작이 프레임 리스트 박스 채우기라면 프레임 이미지 생성이
		// 완료된 이미지들만 프레임 리스트 박스에 채워 넣음
	case POST_TRD_ACT_LOAD_FRAMES:
		// 프레임 벡터에 저장된 이미지들을 프레임 리스트 박스 항목의 추가 메모리에 저장함
		for (int i = 0; i < m_frame_vector.size(); ++i)
		{
			if (m_frame_vector[i].p_frame_bmp)
			{
				p_src_bmp = m_frame_vector[i].p_src_bmp;
				p_frame_bmp = m_frame_vector[i].p_frame_bmp;
				m_frame_list.InsertItemBitmaps(p_src_bmp, p_frame_bmp);
				++insert_count;
			}
			else
			{
				// 프레임 이미지 생성이 되지 못한 이미지들은 제거함
				m_gdip.DestroyImage(m_frame_vector[i].p_src_bmp);
			}
		}
		// 프레임 리스트 박스에 추가된 항목들 중 첫 번째 항목을 선택함
		SelectFirstInsertedItem(m_frame_list.GetCount() - insert_count);
		break;
		// 강제 종료된 작업 스레드의 종료 이후 동작이 프레임 이미지 변경이라면 GIF 기준 프레임의 크기를
		// 재설정하기 이전으로 복원시킴
	case POST_TRD_ACT_RESET_BASE_FRAME:
		m_frame_cx = m_old_frame_cx;
		m_frame_cy = m_old_frame_cy;
		// 생성된 모든 프레임 이미지들을 제거함
		for (int i = 0; i < m_frame_vector.size(); ++i)
		{
			if (m_frame_vector[i].p_frame_bmp)
			{
				m_gdip.DestroyImage(m_frame_vector[i].p_frame_bmp);
			}
		}
		break;
		// 강제 종료된 작업 스레드의 종료 이후 동작이 8비트 변환된 프레임 이미지 미리 보기라면
		// 8비트 변환된 프레임 이미지를 제거함
	case POST_TRD_ACT_CONVERT_PREVIEW:
		m_gdip.DestroyImage(m_frame_vector[0].p_cvrt_frame_bmp);
		break;
		// 강제 종료된 작업 스레드의 종료 이후 동작이 GIF 이미지 제작이라면 생성된 모든 8비트
		// 프레임 이미지들을 다 제거함
	case POST_TRD_ACT_CREATE_GIF:
		for (int i = 0; i < m_frame_vector.size(); ++i)
		{
			if (m_frame_vector[i].p_cvrt_frame_bmp)
			{
				m_gdip.DestroyImage(m_frame_vector[i].p_cvrt_frame_bmp);
			}
		}
		break;
	}
	// 프레임 벡터가 더 이상 필요 없으므로 저장되어 있던 데이터들을 모두 버림
	m_frame_vector.clear();
	// 스레드가 종료됐으므로 스레드 데이터를 제거함
	delete p_data;
	return 0;
}

void GifAsmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_frame_list.m_hWnd && cx && cy)
	{
		m_gdip.CreateGDIP(cx, cy);
		int x = 0, y = 0, width = 0, height = 0;
		// 프레임 리스트 박스의 좌표를 조정함
		x = m_dlg_guide_spacing;
		y = cy - m_frame_list_height - m_dlg_guide_spacing;
		width = cx - m_dlg_guide_spacing * 2;
		height = m_frame_list_height;
		m_frame_list.SetWindowPos(NULL, x, y, width, height, 0);
		// GIF 이미지 프레임 출력 공간의 좌표를 조정함(이동시킬 필요 없이 크기만 조정)
		width = cx - m_frame_viewer_rect.left - m_dlg_guide_spacing - 1;
		height = cy - m_frame_list_height - m_dlg_guide_spacing * 2 - m_frame_viewer_spacing - 1;
		GetDlgItem(IDC_IMAGE_VIEWER_RECT)->SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE);
		GetDlgItem(IDC_IMAGE_VIEWER_RECT)->GetWindowRect(&m_frame_viewer_rect);
		ScreenToClient(&m_frame_viewer_rect);
		// 변경된 출력 공간의 크기에 맞추어 출력 공간 안에 그려질 프레임 이미지의 좌표를 조정함
		if (m_frame_list.GetCount())
		{
			CenterFitFrameInViewer((ItemFrameData*)m_frame_list.GetItemDataPtr(m_frame_idx));
		}
		DrawClientImage();
		Invalidate(FALSE);
	}
}

void GifAsmDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 읽어들인 프레임들을 재생중이었다면 재생 중단
	if (m_play_flag)
	{
		OnBnClickedPlayFrameBtn();
	}
	wchar_t temp_path[MAX_PATH] = { 0 }, file_ext[16] = { 0 };
	wchar_t* p_ext = NULL;
	int drop_count = ::DragQueryFile(hDropInfo, -1, NULL, 0);
	int path_len = 0, start_idx = m_frame_list.GetCount(), insert_count = 0;
	GpBitmap* p_src_bmp = NULL;
	for (int i = 0; i < drop_count; ++i)
	{
		path_len = ::DragQueryFile(hDropInfo, i, temp_path, MAX_PATH);
		// 디렉토리는 드롭할 수 없도록 설정함
		if ((::GetFileAttributes(temp_path) & FILE_ATTRIBUTE_DIRECTORY) >> 4)
		{
			continue;
		}
		memset(file_ext, 0, sizeof(file_ext));
		// 드롭된 파일의 경로 문자열에서 파일 확장자 앞 '.' 문자를 찾을 때 파일 경로 문자열의 가장 끝에
		// 있는 '.' 문자를 찾아야 하므로 문자 탐색에 사용할 포인터를 문자열의 끝에 위치시킴
		p_ext = temp_path + path_len;
		// 파일 확장자 앞 '.' 문자의 위치를 찾아냄
		while (*p_ext != L'.')
		{
			--p_ext;
		}
		// 파일 확장자만 추출할 수 있도록 파일 확장자 앞 '.' 문자의 다음 위치로 이동함
		++p_ext;
		wcscpy_s(file_ext, p_ext); // 파일 확장자 추출
		// 추출한 파일 확장자를 모두 소문자로 변환함(파일 확장자를 확인할 때 파일 확장자에 대문자가
		// 있는지 확인해볼 필요성을 없애버림)
		_wcslwr_s(file_ext);
		// 이미지 파일이 아닌 파일은 드롭할 수 없도록 설정함
		if (wcscmp(file_ext, L"png") && wcscmp(file_ext, L"jpg") && wcscmp(file_ext, L"jpeg") &&
			wcscmp(file_ext, L"bmp"))
		{
			continue;
		}
		else
		{
			// 이미지를 읽어들여서 프레임 벡터에 저장함
			p_src_bmp = m_gdip.LoadBitmap(temp_path);
			m_frame_vector.push_back(FrameData());
			m_frame_vector[insert_count].index = insert_count;
			m_frame_vector[insert_count].p_src_bmp = p_src_bmp;
			m_frame_vector[insert_count].p_frame_bmp = NULL;
			// GIF 기준 프레임의 폭이나 높이가 0이라면 GIF 기준 프레임이 설정되지 않았다(프레임 리스트 박스에
			// 추가된 이미지가 없다)는 뜻이기 때문에 현재 이미지를 GIF 기준 프레임으로 설정함
			if (!m_frame_cx || !m_frame_cy)
			{
				GdipGetImageWidth(p_src_bmp, &m_frame_cx);
				GdipGetImageHeight(p_src_bmp, &m_frame_cy);
				// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
				swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
				SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
			}
			// 읽어들인 이미지 개수를 증가시킴
			++insert_count;
		}
	}
	// 읽어들인 이미지가 존재할 경우에만 스레드 작업을 실행함
	if (insert_count)
	{
		// 스레드가 종료되면 프레임 리스트 박스 채우기 작업이 수행되는 상태값으로 설정된 스레드를 생성함
		InitFrameThread(POST_TRD_ACT_LOAD_FRAMES);
	}
	CDialog::OnDropFiles(hDropInfo);
}

void GifAsmDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 읽어들인 프레임들을 재생중이었다면 재생 중단
	if (m_play_flag)
	{
		OnBnClickedPlayFrameBtn();
	}
}

void GifAsmDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		m_frame_idx = (m_frame_idx + 1) % m_frame_list.GetCount();
		m_frame_list.SetCurSel(m_frame_idx);
		UpdateCurrentFrame();
	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}
}

BOOL GifAsmDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (::GetFocus() == ::GetDlgItem(m_hWnd, IDC_FRAME_DELAY_EDIT))
	{
		// 프레임 지연 시간 에디트 컨트롤에 포커스가 있는 상태에서 마우스 휠 동작이 발생하면
		// 프레임 지연 시간 변경 동작을 수행함
		SetFrameDelay(zDelta > 0);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT GifAsmDlg::OnCtlColorStatic(WPARAM wParam, LPARAM lParam)
{
	// 다이얼로그 배경색을 스태틱 컨트롤의 배경색으로 설정함
	::SetBkColor((HDC)wParam, m_bk_color);
	return (LRESULT)(HBRUSH)m_bk_brush;
}

void GifAsmDlg::OnLbnSelchangeFrameList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_frame_list.GetCurSel() != m_frame_idx)
	{
		m_frame_idx = m_frame_list.GetCurSel();
		UpdateCurrentFrame();
	}
}

void GifAsmDlg::OnBnClickedPlayFrameBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스의 항목 개수가 1개 이하라면 이 동작을 수행하지 않음
	if (m_frame_list.GetCount() < 2)
	{
		return;
	}
	if (!m_play_flag) // 읽어들인 프레임들의 재생을 실행시킴
	{
		GetDlgItem(IDC_PLAY_FRAME_BTN)->SetWindowText(L"중지");
		m_play_flag = 1;
		// GIF 이미지 제작에 사용하는 프레임 지연 시간의 1은 1ms가 아닌 10ms이기 때문에
		// 설정된 프레임 지연 시간에 10을 곱한 값이 실제 프레임 지연 시간
		SetTimer(1, m_frame_delay * 10, NULL);
	}
	else // 읽어들인 프레임들의 재생을 중지시킴
	{
		GetDlgItem(IDC_PLAY_FRAME_BTN)->SetWindowText(L"재생");
		m_play_flag = 0;
		KillTimer(1);
	}
}

void GifAsmDlg::OnBnClickedAddFrameImageBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 읽어들인 프레임들을 재생중이었다면 재생 중단
	if (m_play_flag)
	{
		OnBnClickedPlayFrameBtn();
	}
	wchar_t ext_filter[] = L"이미지 파일(*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png||";
	wchar_t init_path[MAX_PATH] = { 0 }, image_path[MAX_PATH] = { 0 };
	wchar_t* p_file_str = NULL;
	int buf_size = 0, dir_path_len = 0, file_name_len = 0, insert_count = 0;
	// 한 번에 여러 개의 파일을 읽어들일 수 있도록 플래그 값을 설정함
	DWORD file_dlg_flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;
	CFileDialog load_dlg(TRUE, NULL, NULL, file_dlg_flags, ext_filter);
	// 파일 다이얼로그의 기본 경로를 바탕화면으로 설정
	::SHGetSpecialFolderPath(NULL, init_path, CSIDL_DESKTOP, FALSE);
	load_dlg.m_ofn.lpstrInitialDir = init_path;
	// 대량의 이미지들을 읽어들일 수 있도록 이미지들의 경로를 저장하기 위한 문자열을 미리 동적할당함
	buf_size = (MAX_FILE_COUNT * (MAX_PATH + 1)) + 1;
	p_file_str = new wchar_t[buf_size];
	load_dlg.m_ofn.lpstrFile = p_file_str;
	load_dlg.m_ofn.nMaxFile = buf_size;
	if (load_dlg.DoModal() == IDOK)
	{
		GpBitmap* p_src_bmp = NULL;
		wchar_t* p_start = p_file_str, * p_buf_end = p_file_str + buf_size - 2;
		wchar_t* p_itr = p_file_str;
		// 이미지 경로 문자열의 첫 번째 문자열을 읽어서 디렉토리인지 파일인지 확인함
		while ((p_itr < p_buf_end) && (*p_itr))
		{
			image_path[dir_path_len] = *p_itr;
			++dir_path_len;
			++p_itr;
		}
		if (p_itr > p_start)
		{
			// 파일 다이얼로그에서 여러 개의 파일을 선택했다면 이미지 경로 문자열의 첫 번째 문자열은
			// 이미지들을 읽어들인 디렉토리의 경로
			if ((::GetFileAttributes(image_path) & FILE_ATTRIBUTE_DIRECTORY) >> 4)
			{
				// 이미지를 읽어들이려면 디렉토리 경로 문자열 끝에 '\\' 문자를 붙여서 이미지 이름 문자열과
				// 연결해야 함
				image_path[dir_path_len++] = L'\\';
				// AfxMessageBox(image_path);
				// AfxMessageBox(p_start);
			}
			// 파일 다이얼로그에서 한 개의 파일을 선택했다면 이미지 경로 문자열의 첫 번째 문자열은
			// 디렉토리의 경로가 포함된 이미지 파일의 경로
			else
			{
				// 이미지를 읽어들여서 프레임 벡터에 저장함
				p_src_bmp = m_gdip.LoadBitmap(image_path);
				m_frame_vector.push_back(FrameData());
				m_frame_vector[insert_count].index = insert_count;
				m_frame_vector[insert_count].p_src_bmp = p_src_bmp;
				// GIF 기준 프레임의 폭이나 높이가 0이라면 GIF 기준 프레임이 설정되지 않았다(프레임 리스트
				// 박스에 추가된 이미지가 없다)는 뜻이기 때문에 현재 이미지를 GIF 기준 프레임으로 설정함
				if (!m_frame_cx || !m_frame_cy)
				{
					GdipGetImageWidth(p_src_bmp, &m_frame_cx);
					GdipGetImageHeight(p_src_bmp, &m_frame_cy);
					// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
					swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
					SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
				}
				++insert_count;
			}
			// 다음 문자열을 읽을 수 있도록 공백 문자를 건너뜀
			++p_itr;
			// 디렉토리 경로 문자열 이후에는 디렉토리 경로가 없는 이미지 이름 문자열들이 저장되어 있음
			while ((p_itr < p_buf_end) && (*p_itr))
			{
				p_start = p_itr;
				file_name_len = 0;
				// 디렉토리 경로 문자열에 이미지 이름 문자열을 연결함
				while ((p_itr < p_buf_end) && (*p_itr))
				{
					image_path[dir_path_len + file_name_len] = *p_itr;
					++file_name_len;
					++p_itr;
				}
				if (p_itr > p_start)
				{
					// 이미지를 읽어들여서 프레임 벡터에 저장함
					p_src_bmp = m_gdip.LoadBitmap(image_path);
					m_frame_vector.push_back(FrameData());
					m_frame_vector[insert_count].index = insert_count;
					m_frame_vector[insert_count].p_src_bmp = p_src_bmp;
					m_frame_vector[insert_count].p_frame_bmp = NULL;
					// GIF 기준 프레임의 폭이나 높이가 0이라면 GIF 기준 프레임이 설정되지 않았다(프레임 리스트
					// 박스에 추가된 이미지가 없다)는 뜻이기 때문에 현재 이미지를 GIF 기준 프레임으로 설정함
					if (!m_frame_cx || !m_frame_cy)
					{
						GdipGetImageWidth(p_src_bmp, &m_frame_cx);
						GdipGetImageHeight(p_src_bmp, &m_frame_cy);
						// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
						swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
						SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
					}
					++insert_count;
					// AfxMessageBox(image_path);
					// AfxMessageBox(p_start);
				}
				// 다음 이미지 이름 문자열을 디렉토리 경로 문자열과 연결하기 위해 현재 이미지 이름 문자열을 제거함
				memset(image_path + dir_path_len, 0, (MAX_PATH - dir_path_len) << 1);
				// 다음 문자열을 읽을 수 있도록 공백 문자를 건너뜀
				++p_itr;
			}
		}
		// 읽어들인 이미지가 존재할 경우에만 스레드 작업을 실행함
		if (insert_count)
		{
			// 스레드가 종료되면 프레임 리스트 박스 채우기 작업이 수행되는 상태값으로 설정된 스레드를 생성함
			InitFrameThread(POST_TRD_ACT_LOAD_FRAMES);
		}
	}
	// 이미지들의 경로를 저장하기 위해 동적할당한 문자열을 해제함
	delete[] p_file_str;
}

void GifAsmDlg::OnShiftOne()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_shift_dist = 1;
	SetSplitBtnCommand(&m_last_shift_frame_cmd, 1, &m_shift_dist_btn);
}

void GifAsmDlg::OnShiftFive()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_shift_dist = 5;
	SetSplitBtnCommand(&m_last_shift_frame_cmd, 1, &m_shift_dist_btn);
}

void GifAsmDlg::OnShiftTen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_shift_dist = 10;
	SetSplitBtnCommand(&m_last_shift_frame_cmd, 1, &m_shift_dist_btn);
}

void GifAsmDlg::OnShiftEndpos()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_shift_dist = 0;
	SetSplitBtnCommand(&m_last_shift_frame_cmd, 1, &m_shift_dist_btn);
}

void GifAsmDlg::OnDeltaposFrameShiftSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 프레임 리스트 박스의 항목 개수가 1개 이하거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if ((m_frame_list.GetCount() < 2) || m_play_flag)
	{
		return;
	}
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int dest_idx = 0, last_idx = m_frame_list.GetCount() - 1;
	void* p_data = NULL;
	// 현재 선택된 프레임 리스트 박스 항목을 이전 위치로 이동
	if (pNMUpDown->iDelta > 0)
	{
		// 선택된 항목의 현재 인덱스가 0이라면 이동시키지 않음
		if (!m_frame_idx)
		{
			return;
		}
		// '마지막 위치로' 이동한다면 선택된 항목의 새 인덱스는 첫 번째 인덱스
		if (m_last_shift_frame_cmd == ID_SHIFT_ENDPOS)
		{
			dest_idx = 0;
		}
		// '마지막 위치로' 이동하지 않는다면 선택된 항목의 새 인덱스는 현재 인덱스에서 이동 거리만큼 이전 인덱스
		// (현재 인덱스의 이동 거리만큼 이전 인덱스가 허용된 인덱스 범위 밖이라면 새 인덱스를 첫 번째 인덱스로 설정)
		else
		{
			dest_idx = (m_frame_idx > m_shift_dist) ? (m_frame_idx - m_shift_dist) : 0;
		}
	}
	// 현재 선택된 프레임 리스트 박스 항목을 다음 위치로 이동
	else
	{
		// 선택된 항목의 현재 인덱스가 마지막 인덱스라면 이동시키지 않음
		if (m_frame_idx == last_idx)
		{
			return;
		}
		// '마지막 위치로' 이동한다면 선택된 항목의 새 인덱스는 마지막 인덱스
		if (m_last_shift_frame_cmd == ID_SHIFT_ENDPOS)
		{
			dest_idx = -1;
		}
		// '마지막 위치로' 이동하지 않는다면 선택된 항목의 새 인덱스는 현재 인덱스에서 이동 거리만큼 다음 인덱스
		// (현재 인덱스의 이동 거리만큼 다음 인덱스가 허용된 인덱스 범위 밖이라면 새 인덱스를 마지막 인덱스로 설정)
		else
		{
			dest_idx = ((m_frame_idx + m_shift_dist) < last_idx) ? (m_frame_idx + m_shift_dist) : -1;
		}
	}
	// 선택된 항목의 추가 메모리 주소를 백업해둠
	p_data = m_frame_list.GetItemDataPtr(m_frame_idx);
	// 선택된 항목을 삭제한 다음 새 인덱스에 항목을 추가하면서 백업해둔 추가 메모리 주소를 가져옴
	m_frame_list.DeleteString(m_frame_idx);
	m_frame_idx = m_frame_list.InsertString(dest_idx, L"");
	m_frame_list.SetItemDataPtr(m_frame_idx, p_data);
	// 새 인덱스에 추가된 항목을 선택함
	m_frame_list.SetCurSel(m_frame_idx);
	*pResult = 0;
}

void GifAsmDlg::OnBnClickedSetBaseFrameBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스가 비어있거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if (!m_frame_list.GetCount() || m_play_flag)
	{
		return;
	}
	ItemFrameData* p_item_data = (ItemFrameData*)m_frame_list.GetItemDataPtr(m_frame_idx);
	// 현재 선택된 프레임 리스트 박스 항목 원본 이미지의 폭과 높이가 GIF 기준 프레임의 폭과 높이랑
	// 동일하다면 GIF 기준 프레임을 변경하는 작업을 수행하지 않음
	if ((p_item_data->src_cx == m_frame_cx) && (p_item_data->src_cy == m_frame_cy))
	{
		AfxMessageBox(L"선택한 프레임의 폭과 높이가 현재 기준 프레임의 폭과 높이랑 동일함");
		return;
	}
	// GIf 기준 프레임의 변경 이전 폭과 높이를 저장해둠
	m_old_frame_cx = m_frame_cx;
	m_old_frame_cy = m_frame_cy;
	// GIF 기준 프레임의 폭과 높이를 변경함
	m_frame_cx = p_item_data->src_cx;
	m_frame_cy = p_item_data->src_cy;
	// 프레임 리스트 박스에 저장된 항목들의 원본 이미지를 프레임 벡터에 저장함
	for (int i = 0; i < m_frame_list.GetCount(); ++i)
	{
		m_frame_vector.push_back(FrameData());
		m_frame_vector[i].index = i;
		m_frame_vector[i].p_src_bmp = m_frame_list.GetSourceBitmap(i);
		m_frame_vector[i].p_frame_bmp = NULL;
	}
	// 스레드가 종료되면 프레임 이미지 변경 작업이 수행되는 상태값으로 설정된 스레드를 생성함
	InitFrameThread(POST_TRD_ACT_RESET_BASE_FRAME);
}

void GifAsmDlg::OnBnClickedReverseFrameOrderBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스의 항목 개수가 1개 이하거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if ((m_frame_list.GetCount() < 2) || m_play_flag)
	{
		return;
	}
	void* p_data = NULL;
	// 프레임 리스트 박스의 모든 항목들을 역순으로 정렬했을 때 현재 선택된 항목의 인덱스가 어떤
	// 인덱스로 바뀌는지 미리 계산해둠
	m_frame_idx = m_frame_list.GetCount() - m_frame_idx - 1;
	// 프레임 리스트 박스의 마지막 인덱스 앞 항목부터 역순으로 항목을 삭제했다가 프레임 리스트 박스의
	// 끝에 항목을 다시 추가함(항목을 삭제하기 전에 항목의 추가 메모리 주소를 백업해뒀다가 항목을
	// 삭제하고 나서 다시 추가했을 때 백업해둔 주소를 가져옴)
	for (int i = m_frame_list.GetCount() - 2; i >= 0; --i)
	{
		p_data = m_frame_list.GetItemDataPtr(i);
		m_frame_list.DeleteString(i);
		m_frame_list.SetItemDataPtr(m_frame_list.AddString(L""), p_data);
	}
	m_frame_list.SetCurSel(m_frame_idx);
}

void GifAsmDlg::OnBnClickedRemoveFrameBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스가 비어있거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if (!m_frame_list.GetCount() || m_play_flag)
	{
		return;
	}
	switch (m_last_remove_frame_cmd)
	{
	case ID_REMOVE_FRAME:
		// 프레임 리스트 박스에서 선택한 항목을 제거한 뒤에도 남은 항목이 있다면 항목 선택
		// 상태를 조정함
		if (m_frame_list.RemoveItem(m_frame_idx))
		{
			// 프레임 리스트 박스에서 제거한 항목이 마지막 항목이라면(제거된 항목의 인덱스가 항목을
			// 제거한 프레임 리스트 박스의 항목 개수와 동일하다면) 항목이 제거된 프레임 리스트 박스의
			// 마지막 항목이 선택되도록 현재 선택한 항목의 인덱스를 조정함
			if (m_frame_idx == m_frame_list.GetCount())
			{
				--m_frame_idx;
			}
			m_frame_list.SetCurSel(m_frame_idx);
		}
		break;
	case ID_REMOVE_ALL_FRAMES:
		m_frame_list.ResetData();
		break;
	}
	// 프레임 리스트 박스가 비어있다면 GIF 기준 프레임을 무효화시킴
	if (!m_frame_list.GetCount())
	{
		m_frame_cx = m_frame_cy = 0;
		// 제작될 GIF 이미지 기준 프레임의 폭과 높이를 표시함
		swprintf_s(m_frame_size_str, FRAME_SIZE_STR_LEN, L"%d * %d", m_frame_cx, m_frame_cy);
		SetDlgItemText(IDC_BASE_FRAME_SIZE_EDIT, m_frame_size_str);
	}
	DrawClientImage();
	Invalidate(FALSE);
}

void GifAsmDlg::OnRemoveFrame()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetSplitBtnCommand(&m_last_remove_frame_cmd, 2, &m_remove_frame_btn);
}

void GifAsmDlg::OnRemoveAllFrames()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetSplitBtnCommand(&m_last_remove_frame_cmd, 2, &m_remove_frame_btn);
}

void GifAsmDlg::OnBnClickedSaveGifBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스가 비어있거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if (!m_frame_list.GetCount() || m_play_flag)
	{
		return;
	}
	wchar_t init_path[MAX_PATH] = { 0 };
	DWORD file_dlg_flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	CFileDialog save_dlg(FALSE, L"gif", NULL, file_dlg_flags, L"GIF 이미지(*.gif)|*.gif||");
	// 파일 다이얼로그의 기본 경로를 바탕화면으로 설정
	::SHGetSpecialFolderPath(NULL, init_path, CSIDL_DESKTOP, FALSE);
	save_dlg.m_ofn.lpstrInitialDir = init_path;
	if (save_dlg.DoModal() == IDOK)
	{
		// 파일 다이얼로그에서 설정한 제작될 GIF 이미지의 저장 경로와 파일 이름을 저장함
		memcpy(m_gif_save_path, save_dlg.GetPathName(), (wcslen(save_dlg.GetPathName()) + 1) << 1);
		// 프레임 리스트 박스 안에 저장된 항목들의 프레임 이미지들을 프레임 벡터에 저장함
		for (int i = 0; i < m_frame_list.GetCount(); ++i)
		{
			m_frame_vector.push_back(FrameData());
			m_frame_vector[i].index = i;
			m_frame_vector[i].p_frame_bmp = m_frame_list.GetFrameBitmap(i);
			// 8비트 컬러로 변환된 프레임 이미지는 스레드에서 생성되기 때문에 지금은 비워둠
			m_frame_vector[i].p_cvrt_frame_bmp = NULL;
		}
		// 스레드가 종료되면 GIF 이미지 제작 작업이 수행되는 상태값으로 설정된 스레드를 생성함
		InitFrameThread(POST_TRD_ACT_CREATE_GIF);
	}
}

void GifAsmDlg::OnBnClickedFramePreviewBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 리스트 박스가 비어있거나 읽어들인 프레임들을 재생중이었다면 이 동작을 수행하지 않음
	if (!m_frame_list.GetCount() || m_play_flag)
	{
		return;
	}
	// 프레임 리스트 박스에서 현재 선택된 항목의 프레임 이미지를 프레임 벡터에 저장함
	m_frame_vector.push_back(FrameData());
	m_frame_vector[0].index = 0;
	m_frame_vector[0].p_frame_bmp = m_frame_list.GetFrameBitmap(m_frame_idx);
	// 8비트 컬러로 변환된 프레임 이미지는 스레드에서 생성되기 때문에 지금은 비워둠
	m_frame_vector[0].p_cvrt_frame_bmp = NULL;
	// 스레드가 종료되면 8비트 변환된 프레임 미리 보기 작업이 수행되는 상태값으로 설정된 스레드를 생성함
	InitFrameThread(POST_TRD_ACT_CONVERT_PREVIEW);
}

void GifAsmDlg::OnEnChangeFrameDelayEdit()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	swprintf_s(m_real_delay_str, 16, L"%dms(%0.2fs)", m_frame_delay * 10, m_frame_delay / 100.0f);
	SetDlgItemText(IDC_REAL_FRAME_DELAY_EDIT, m_real_delay_str);
}

void GifAsmDlg::OnDeltaposFrameDelaySpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 프레임 지연 시간 에디트 컨트롤과 연결된 스핀 컨트롤 동작이 발생하면 프레임 지연 시간 변경 동작을 수행함
	SetFrameDelay(pNMUpDown->iDelta < 0);
	*pResult = 0;
}
