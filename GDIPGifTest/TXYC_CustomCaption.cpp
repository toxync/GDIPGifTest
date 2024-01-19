#include "pch.h"
#include "TXYC_CustomCaption.h"

TXYC_CustomCaption::TXYC_CustomCaption(CWnd* ap_wnd)
{
	mp_wnd = ap_wnd;
}

UINT TXYC_CustomCaption::GetDPIScale(UINT a_value, UINT a_dpi)
{
	return (UINT)((double)a_value * a_dpi / 96.0);
}

void TXYC_CustomCaption::SetFakeWndTopBorderRect(RECT* ap_fake_top_border_rect)
{
	// 가짜 상단 테두리의 폭을 클라이언트 영역의 폭과 동일하게 설정
	*ap_fake_top_border_rect = m_client_rect;
	ap_fake_top_border_rect->bottom = ap_fake_top_border_rect->top + FAKE_TOP_BORDER_HEIGHT;
}

void TXYC_CustomCaption::SetCaptionRectPos()
{
	// 캡션 영역의 좌표 설정
	UINT caption_height = m_btn_height + CAPTION_TOPBOTTOM_BORDERS;
	// 원도우가 전체화면이 아니라면 캡션 탭을 그려넣을 공간 확보를 위해 캡션 영역의 높이를 늘림
	if (!IsWindowMaximized())
	{
		// 캡션 탭과 윈도우 상단 프레임 사이에 공백을 추가해서 실수로 상단 프레임을 클릭하는 현상 방지
		caption_height += m_top_border_height + GetDPIScale(TOP_FRAME_SPACING, m_dpi);
	}
	m_cptn_rects.Caption = m_client_rect; // 캡션의 폭을 클라이언트 영역의 폭과 동일하게 설정
	m_cptn_rects.Caption.bottom = m_cptn_rects.Caption.top + caption_height;
	// 캡션 버튼들의 좌표 설정
	// 왼쪽에서 오른쪽으로 '최소화 버튼 - 최대화 버튼 - 닫기 버튼' 순서대로 배치되고 닫기 버튼이 캡션의
	// 끝부분과 가장 가깝기 때문에 캡션 영역의 좌표를 통해 닫기 버튼의 좌표를 제일 먼저 설정한 다음
	// 닫기 버튼의 좌표로 최대화 버튼의 좌표를, 최대화 버튼의 좌표로 최소화 버튼의 좌표를 설정함
	m_cptn_rects.CloseBtn = m_cptn_rects.Caption;
	// 닫기 버튼과 나머지 캡션 버튼들이 가짜 상단 테두리 선에 겹쳐지지 않도록 닫기 버튼의 좌표 설정
	m_cptn_rects.CloseBtn.top += FAKE_TOP_BORDER_HEIGHT;
	// 닫기 버튼의 좌표 설정
	m_cptn_rects.CloseBtn.bottom = m_cptn_rects.CloseBtn.top + m_btn_height;
	m_cptn_rects.CloseBtn.left = m_cptn_rects.CloseBtn.right - m_btn_width;
	// 최대화 버튼의 좌표 설정
	m_cptn_rects.MaximizeBtn = m_cptn_rects.CloseBtn;
	m_cptn_rects.MaximizeBtn.left -= m_btn_width;
	m_cptn_rects.MaximizeBtn.right -= m_btn_width;
	// 최소화 버튼의 좌표 설정
	m_cptn_rects.MinimizeBtn = m_cptn_rects.MaximizeBtn;
	m_cptn_rects.MinimizeBtn.left -= m_btn_width;
	m_cptn_rects.MinimizeBtn.right -= m_btn_width;
	// 상단 프레임 영역의 좌표 설정
	// 상단 프레임의 좌측 상단 좌표는 캡션 영역의 좌측 상단 좌표와 동일
	m_cptn_rects.TopFrame = m_cptn_rects.Caption;
	// 상단 프레임의 우측 좌표는 최소화 버튼의 좌측 좌표와 동일
	m_cptn_rects.TopFrame.right = m_cptn_rects.MinimizeBtn.left;
	// 상단 프레임의 하단 좌표는 저장해둔 상단 프레임 높이 값으로 설정
	m_cptn_rects.TopFrame.bottom = m_top_border_height;
}

void TXYC_CustomCaption::SetCaptionTabPos()
{
	// 캡션 탭 전체 영역의 좌측 하단 좌표는 캡션 영역의 좌측 하단 좌표와 동일
	m_cptn_rects.CaptionTab = m_cptn_rects.Caption;
	// 캡션 탭 전체 영역의 상단 좌표는 캡션 탭 전체 영역의 하단 좌표에서 캡션 탭 높이만큼 뺀 좌표
	m_cptn_rects.CaptionTab.top = m_cptn_rects.CaptionTab.bottom - m_tab_height;
	// 캡션 탭 전체 영역의 우측 좌표는 (캡션 탭 한 개의 폭) * (캡션 탭 개수)
	m_cptn_rects.CaptionTab.right = m_tab_width * m_tab_count;
	UINT left = 0, top = 0, width = 0, height = 0;
	UINT text_spacing = GetDPIScale(CAPTION_TAB_TEXT_SPACING, m_dpi);
	for (int i = 0; i < m_tab_count; ++i)
	{
		// 각 캡션 탭의 이름 문자열 출력 공간 좌표를 설정함
		m_tab_data[i].TextArea.X = m_cptn_rects.CaptionTab.left + i * m_tab_width + text_spacing;
		m_tab_data[i].TextArea.Y = m_cptn_rects.CaptionTab.top + text_spacing;
		m_tab_data[i].TextArea.Width = m_tab_width - text_spacing * 2;
		m_tab_data[i].TextArea.Height = m_tab_height - text_spacing * 2;
		// 캡션 탭과 연결된 다이얼로그의 위치와 크기를 조정함
		left = m_cptn_rects.Caption.left;
		top = m_cptn_rects.Caption.bottom + 1;
		width = m_cptn_rects.Caption.right - left;
		height = m_client_rect.bottom - top;
		m_tab_data[i].p_TabDlg->SetWindowPos(NULL, left, top, width, height, 0);
	}
}

BOOL TXYC_CustomCaption::IsWindowMaximized()
{
	WINDOWPLACEMENT placement = { 0 };
	placement.length = sizeof(WINDOWPLACEMENT);
	if (::GetWindowPlacement(mp_wnd->m_hWnd, &placement))
	{
		return placement.showCmd == SW_SHOWMAXIMIZED;
	}
	return FALSE;
}

void TXYC_CustomCaption::CenterRectInRect(RECT* ap_center_rect, const RECT* ap_outer_rect)
{
	UINT center_width = ap_center_rect->right - ap_center_rect->left;
	UINT center_height = ap_center_rect->bottom - ap_center_rect->top;
	UINT outer_width = ap_outer_rect->right - ap_outer_rect->left;
	UINT outer_height = ap_outer_rect->bottom - ap_outer_rect->top;
	UINT padding_x = (outer_width - center_width) / 2;
	UINT padding_y = (outer_height - center_height) / 2;
	ap_center_rect->left = ap_outer_rect->left + padding_x;
	ap_center_rect->top = ap_outer_rect->top + padding_y;
	ap_center_rect->right = ap_center_rect->left + center_width;
	ap_center_rect->bottom = ap_center_rect->top + center_height;
}

void TXYC_CustomCaption::AdjustMaximizedClientRect(RECT* ap_rect)
{
	HMONITOR h_mon = ::MonitorFromWindow(mp_wnd->m_hWnd, MONITOR_DEFAULTTONEAREST);
	if (h_mon == NULL)
	{
		return;
	}
	MONITORINFO monitor_info = { 0 };
	monitor_info.cbSize = sizeof(MONITORINFO);
	if (!::GetMonitorInfo(h_mon, &monitor_info))
	{
		return;
	}
	// 전체화면 상태 윈도우의 클라이언트 영역은 모니터 영역에서 작업 표시줄을 제외한 영역
	*ap_rect = monitor_info.rcWork;
}

void TXYC_CustomCaption::AdjustUnMaximizedClientRect(RECT* ap_rect)
{
	// 전체화면 상태가 아닌 윈도우의 클라이언트 영역은 윈도우 전체 영역에서 프레임 영역을 제외한 영역
	int frame_x = ::GetSystemMetricsForDpi(SM_CXFRAME, m_dpi);
	int frame_y = ::GetSystemMetricsForDpi(SM_CYFRAME, m_dpi);
	int padding = ::GetSystemMetricsForDpi(SM_CXPADDEDBORDER, m_dpi);
	ap_rect->left += frame_x + padding;
	ap_rect->right -= frame_x + padding;
	ap_rect->bottom -= frame_y + padding;
}

void TXYC_CustomCaption::StartEventCapture()
{
	RAWINPUTDEVICE raw_input;
	::ZeroMemory(&raw_input, sizeof(RAWINPUTDEVICE));
	// 마우스 RAWINPUTDEVICE 구조체 설정
	raw_input.usUsagePage = 0x01;
	raw_input.usUsage = 0x02;
	raw_input.dwFlags = RIDEV_INPUTSINK;
	raw_input.hwndTarget = mp_wnd->m_hWnd;
	if (!::RegisterRawInputDevices(&raw_input, 1, sizeof(RAWINPUTDEVICE)))
	{
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		AfxMessageBox(str);
	}
}

void TXYC_CustomCaption::EndEventCapture()
{
	RAWINPUTDEVICE raw_input;
	::ZeroMemory(&raw_input, sizeof(RAWINPUTDEVICE));
	// 마우스 RAWINPUTDEVICE 구조체 설정
	raw_input.usUsagePage = 0x01;
	raw_input.usUsage = 0x02;
	raw_input.dwFlags = RIDEV_REMOVE;
	raw_input.hwndTarget = NULL;
	if (!::RegisterRawInputDevices(&raw_input, 1, sizeof(RAWINPUTDEVICE)))
	{
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		AfxMessageBox(str);
	}
}

void TXYC_CustomCaption::DrawCaptionImage()
{
	ARGB* p_caption_color = NULL;
	ARGB* p_btn_color = NULL;
	ARGB* p_item_color = NULL;
	ARGB* p_close_btn_item_color = NULL;
	RECT* p_rect = NULL;
	UINT left = 0, top = 0, width = 0, height = 0;
	UINT sx = 0, sy = 0, ex = 0, ey = 0;
	// 전체 클라이언트 영역 채우기
	m_gdip.Clear(m_cptn_colors.BgColor);
	// 캡션 영역 그리기
	// 윈도우의 활성화 상태에 따라 캡션의 채우기 색상 설정
	p_caption_color = m_active_state ? &m_cptn_colors.ActiveFillColor : &m_cptn_colors.InactiveFillColor;
	p_rect = &m_cptn_rects.Caption;
	left = p_rect->left;
	top = p_rect->top;
	width = p_rect->right - p_rect->left;
	height = p_rect->bottom - p_rect->top;
	m_gdip.FillRect(left, top, width, height, *p_caption_color);
	m_gdip.DrawRect(left, top, width, height, *p_caption_color);
	// 캡션 탭 그리기
	p_rect = &m_cptn_rects.CaptionTab;
	left = p_rect->left;
	top = p_rect->top;
	width = p_rect->right - p_rect->left;
	height = p_rect->bottom - p_rect->top;
	// 전체 캡션 탭 영역 채우기
	m_gdip.FillRect(left, top, width, height, m_cptn_colors.UnselectedTabColor);
	m_gdip.DrawRect(left, top, width, height, 0xFF000000);
	// 호버링된 캡션 탭 그리기
	width = m_tab_width;
	height = m_tab_height;
	if (m_hovered_tab != m_selected_tab && !m_is_clicked)
	{
		left = p_rect->left + m_tab_width * m_hovered_tab;
		top = p_rect->top;
		m_gdip.FillRect(left, top, width, height, m_cptn_colors.HoveredTabColor);
	}
	// 선택된 캡션 탭 그리기
	left = p_rect->left + m_tab_width * m_selected_tab;
	top = p_rect->top;
	m_gdip.FillRect(left, top, width, height, m_cptn_colors.SelectedTabColor);
	// 캡션 탭들 사이의 구분선 그리기
	m_gdip.SetPenColor(0xFF000000);
	for (int i = 1; i < m_tab_count; ++i)
	{
		sx = p_rect->left + m_tab_width * i;
		sy = p_rect->top;
		ex = sx;
		ey = p_rect->bottom;
		m_gdip.DrawLine(sx, sy, ex, ey);
	}
	// 캡션 탭들의 이름 문자열 출력
	for (int i = 0; i < m_tab_count; ++i)
	{
		left = (UINT)m_tab_data[i].TextArea.GetLeft();
		top = (UINT)m_tab_data[i].TextArea.GetTop();
		width = (UINT)m_tab_data[i].TextArea.Width;
		height = (UINT)m_tab_data[i].TextArea.Height;
		// m_gdip.DrawRect(left, top, width, height, 0xFFFF0000);
		m_gdip.DrawText(&m_tab_data[i].TextArea, m_tab_data[i].TabText);
	}
	// 캡션 버튼 그리기
	m_icon_dimension = GetDPIScale(10, m_dpi);
	p_btn_color = p_caption_color;
	// 호버링이나 클릭 상태에 따른 캡션 버튼 채우기 색상 설정
	switch (m_hovered_btn)
	{
	case SelectedBtn_Minimize:
		// 윈도우가 비활성화된 상태일 때에는 현재 버튼에 마우스가 호버링 중이라면 클릭 상태에 상관 없이
		// 버튼의 호버링 상태를 표현함
		if (!m_active_state)
		{
			p_btn_color = &m_cptn_colors.InactiveBtnHoverColor;
		}
		// 윈도우가 활성화된 상태일 때에는 클릭된 캡션 버튼이나 캡션 탭이 없고 현재 버튼에 마우스가
		// 호버링 중인 경우에만 버튼의 호버링 상태를 표현함
		else if (!m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnHoverColor;
		}
		// 현재 버튼이 클릭된 상태로 마우스가 현재 버튼에 호버링 중인 경우에는 버튼의 클릭 상태를 표현함
		// (현재 버튼이 아닌 다른 버튼이 클릭된 상태로 현재 버튼에 마우스가 호버링 중이라면 현재 버튼에
		// 호버링이나 클릭 상태를 표현하지 않음)
		else if (m_clicked_btn == m_hovered_btn)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnClickColor;
		}
		p_rect = &m_cptn_rects.MinimizeBtn;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		m_gdip.FillRect(left, top, width, height, *p_btn_color);
		break;
	case SelectedBtn_Maximize:
		// 윈도우가 비활성화된 상태일 때에는 현재 버튼에 마우스가 호버링 중이라면 클릭 상태에 상관 없이
		// 버튼의 호버링 상태를 표현함
		if (!m_active_state)
		{
			p_btn_color = &m_cptn_colors.InactiveBtnHoverColor;
		}
		// 윈도우가 활성화된 상태일 때에는 클릭된 캡션 버튼이나 캡션 탭이 없고 현재 버튼에 마우스가
		// 호버링 중인 경우에만 버튼의 호버링 상태를 표현함
		else if (!m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnHoverColor;
		}
		// 현재 버튼이 클릭된 상태로 마우스가 현재 버튼에 호버링 중인 경우에는 버튼의 클릭 상태를 표현함
		// (현재 버튼이 아닌 다른 버튼이 클릭된 상태로 현재 버튼에 마우스가 호버링 중이라면 현재 버튼에
		// 호버링이나 클릭 상태를 표현하지 않음)
		else if (m_clicked_btn == m_hovered_btn)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnClickColor;
		}
		p_rect = &m_cptn_rects.MaximizeBtn;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		m_gdip.FillRect(left, top, width, height, *p_btn_color);
		break;
	case SelectedBtn_Close:
		// 윈도우가 비활성화된 상태이거나 윈도우가 활성화된 상태이면서 클릭된 캡션 버튼이나 캡션 탭이 없을 경우에는
		// 나머지 캡션 버튼들과는 다른 호버링 색상으로 닫기 버튼의 호버링 상태를 표현함
		if (!m_active_state || !m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.CloseBtnHoverColor;
			p_close_btn_item_color = &m_cptn_colors.CloseBtnItemColor;
		}
		// 현재 버튼이 클릭된 상태로 마우스가 현재 버튼에 호버링 중인 경우에는 나머지 캡션 버튼들과는 다른
		// 클릭 색상으로 닫기 버튼의 클릭 상태를 표현함(현재 버튼이 아닌 다른 버튼이 클릭된 상태로 현재 버튼에
		// 마우스가 호버링 중이라면 현재 버튼에 호버링이나 클릭 상태를 표현하지 않음)
		else if (m_clicked_btn == m_hovered_btn)
		{
			p_btn_color = &m_cptn_colors.CloseBtnClickColor;
			p_close_btn_item_color = &m_cptn_colors.CloseBtnItemColor;
		}
		p_rect = &m_cptn_rects.CloseBtn;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		m_gdip.FillRect(left, top, width, height, *p_btn_color);
		break;
	}
	// 윈도우의 포커스 상태에 따라 캡션 버튼 아이콘의 색상 설정
	p_item_color = m_active_state ? &m_cptn_colors.ActiveItemColor : &m_cptn_colors.InactiveItemColor;
	// '-' 모양의 최소화 버튼 아이콘 그리기
	m_icon_rect = { 0,0,0,0 };
	m_icon_rect.right = m_icon_dimension;
	m_icon_rect.bottom = 1;
	CenterRectInRect(&m_icon_rect, &m_cptn_rects.MinimizeBtn);
	p_rect = &m_icon_rect;
	left = p_rect->left;
	top = p_rect->top;
	width = p_rect->right - p_rect->left;
	height = p_rect->bottom - p_rect->top;
	m_gdip.DrawRect(left, top, width, height, *p_item_color);
	// 최대화 버튼 아이콘 그리기
	m_icon_rect = { 0,0,0,0 };
	// 윈도우가 전체화면 상태인 경우에는 최대화 버튼의 아이콘을 작은 사각형 2개로 설정
	if (IsWindowMaximized())
	{
		// 윈도우가 전체화면 상태일 때 마우스가 최대화 버튼에 호버링중인 경우가 아니라면 최대화 버튼의
		// 아이콘 내부 채우기 색상이 아이콘 바깥의 최대화 버튼 색상과 동일한 캡션 채우기 색상이어야 함
		if (m_hovered_btn != SelectedBtn_Maximize)
		{
			p_btn_color = p_caption_color;
		}
		m_icon_dimension = GetDPIScale(8, m_dpi);
		m_icon_rect.right = m_icon_dimension;
		m_icon_rect.bottom = m_icon_dimension;
		CenterRectInRect(&m_icon_rect, &m_cptn_rects.MaximizeBtn);
		p_rect = &m_icon_rect;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		// 최대화 버튼 한가운데에 사각형 하나 먼저 그림
		m_gdip.DrawRect(left, top, width, height, *p_item_color);
		// 먼저 그린 사각형의 좌측 하단으로 이동한 영역에 사각형 하나 더 그림
		p_rect->left -= GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->top += GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->right -= GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->bottom += GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		// 두 사각형의 겹쳐진 부분이 보이지 않도록 나중에 그린 사각형의 내부를 채움
		left = p_rect->left;
		top = p_rect->top;
		m_gdip.FillRect(left, top, width, height, *p_btn_color);
		// 나중에 그린 사각형의 테두리 그림
		m_gdip.DrawRect(left, top, width, height, *p_item_color);
	}
	else // 윈도우가 전체화면 상태가 아닌 경우에는 최대화 버튼의 아이콘을 큰 사각형 1개로 설정
	{
		m_icon_rect.right = m_icon_dimension;
		m_icon_rect.bottom = m_icon_dimension;
		CenterRectInRect(&m_icon_rect, &m_cptn_rects.MaximizeBtn);
		p_rect = &m_icon_rect;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		m_gdip.DrawRect(left, top, width, height, *p_item_color);
	}
	// 클릭된 캡션 버튼이나 캡션 탭이 없고 닫기 버튼에 마우스가 호버링 중이거나 닫기 버튼이 클릭된 상태로
	// 마우스가 닫기 버튼에 호버링 중인 경우에는 닫기 버튼 전용 아이콘 색상을 선택
	if (!p_close_btn_item_color)
	{
		p_close_btn_item_color = p_item_color;
	}
	// 'X' 모양의 닫기 버튼 아이콘 그리기
	m_icon_rect = { 0,0,0,0 };
	m_icon_dimension = GetDPIScale(10, m_dpi);
	m_icon_rect.right = m_icon_dimension;
	m_icon_rect.bottom = m_icon_dimension;
	CenterRectInRect(&m_icon_rect, &m_cptn_rects.CloseBtn);
	p_rect = &m_icon_rect;
	m_gdip.SetPenColor(*p_close_btn_item_color);
	sx = p_rect->left;
	sy = p_rect->top;
	ex = p_rect->right + 1;
	ey = p_rect->bottom + 1;
	m_gdip.DrawLine(sx, sy, ex, ey);
	sy = p_rect->bottom;
	ey = p_rect->top - 1;
	m_gdip.DrawLine(sx, sy, ex, ey);
	if (!IsWindowMaximized())
	{
		p_rect = &m_cptn_rects.TopFrame;
		left = p_rect->left;
		top = p_rect->top;
		width = p_rect->right - p_rect->left;
		height = p_rect->bottom - p_rect->top;
		m_gdip.DrawRect(left, top, width, height, 0xFFFF0000);
	}
	// 가짜 상단 테두리 그리기
	/*
	COLORREF fake_top_border_color = m_active_state ? RGB24(255, 255, 255) : RGB24(0, 0, 0);
	RECT fake_top_border_rect = { 0 };
	GetWndFakeTopBorderRect(&fake_top_border_rect);
	p_rect = &fake_top_border_rect;
	m_gdip.DrawRect(p_rect->left, p_rect->top, p_rect->right + 1, p_rect->bottom + 1);
	*/
}

void TXYC_CustomCaption::AddCaptionTabDialog(CDialog* ap_dlg, const wchar_t* ap_text, int a_dlg_id)
{
	m_tab_data.push_back(CaptionTabData());
	// 매개 변수로 넘겨받은 다이얼로그의 주소와 리소스 ID로 자식 다이얼로그 윈도우 생성
	m_tab_data[m_tab_count].p_TabDlg = ap_dlg;
	m_tab_data[m_tab_count].p_TabDlg->Create(a_dlg_id, mp_wnd);
	// wcslen() 함수는 NULL 문자를 제외한 문자열의 길이를 반환하므로 매개 변수로 넘겨 받은 문자열의
	// NULL 문자가 포함된 문자열 길이는 ((wcslen() 함수가 반환한 문자열 길이) + 1)
	UINT64 tab_text_len = wcslen(ap_text) + 1;
	// 유니코드(wchar_t 자료형) 문자열은 한 글자의 크기가 2바이트이므로 복사하려는 유니코드 문자열의
	// 전체 크기는 (유니코드 문자열의 전체 길이) * 2
	memcpy(m_tab_data[m_tab_count].TabText, ap_text, tab_text_len << 1);
	++m_tab_count; // 캡션 탭 개수 증가
}

UINT8 TXYC_CustomCaption::GetCloseReadyStatus()
{
	UINT8 received_target_msg = 0, close_ready_status = 1;
	MSG msg;
	for (int i = 0; i < m_tab_count; ++i)
	{
		received_target_msg = 0;
		// 캡션 탭 다이얼로그에게 닫힐 준비가 됐는지 확인하는 메시지를 보냄
		::SendMessage(m_tab_data[i].p_TabDlg->m_hWnd, TABWND_CLOSE_READY, 0, 0);
		// 캡션 탭 다이얼로그로부터 종료 준비 상태 메시지를 받을 때까지 받은 메시지들을 계속 확인함
		while (!received_target_msg)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// 캡션 탭 다이얼로그로부터 종료 준비 상태 메시지를 받았다면
				if (msg.message == TABWND_CLOSE_READY)
				{
					// 메시지를 더 이상 확인하지 않도록 반복문 조건값을 변경시킴
					received_target_msg = 1;
					// 캡션 탭 다이얼로그의 종료 준비 상태를 확인함(종료 준비가 되지 않은 캡션 탭
					// 다이얼로그가 하나라도 있다면 종료 준비 상태를 거짓으로 설정함)
					close_ready_status &= (UINT8)msg.wParam;
				}
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
	return close_ready_status;
}

void TXYC_CustomCaption::DestroyCaptionTabDialogs()
{
	for (int i = 0; i < m_tab_count; ++i)
	{
		m_tab_data[i].p_TabDlg->DestroyWindow();
		delete m_tab_data[i].p_TabDlg;
	}
	m_tab_data.clear();
}

void TXYC_CustomCaption::NotifyTabWndLBtnDown()
{
	m_is_clicked = Clicked_ClientArea;
}

void TXYC_CustomCaption::NotifyNextTabWnd()
{
	// 현재 캡션 탭과 연결된 다이얼로그를 보이지 않게 만들고 현재 캡션 탭의 다음 탭과
	// 연결된 다이얼로그를 보이게 만듦
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_HIDE);
	m_selected_tab = (m_selected_tab + 1) % m_tab_count;
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
	// 선택된 캡션 탭의 변경이 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	// 선택된 캡션 탭 다이얼로그에게 포커스를 설정함
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::NotifyPrevTabWnd()
{
	// 현재 캡션 탭과 연결된 다이얼로그를 보이지 않게 만들고 현재 캡션 탭의 이전 탭과
	// 연결된 다이얼로그를 보이게 만듦
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_HIDE);
	if (!m_selected_tab)
	{
		m_selected_tab = m_tab_count - 1;
	}
	else
	{
		m_selected_tab = (m_selected_tab - 1) % m_tab_count;
	}
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
	// 선택된 캡션 탭의 변경이 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	// 선택된 캡션 탭 다이얼로그에게 포커스를 설정함
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::OnInitCaption()
{
	// 캡션 탭 이름 문자열에 사용할 글꼴의 종류와 크기를 설정함
	m_gdip.ChangeFont(L"맑은 고딕", GetDPIScale(CAPTION_TAB_FONT, m_dpi));
	// 사용자 정의 캡션에서 사용할 색상들을 초기화
	// m_cptn_colors.BgColor = RGB24(240, 240, 240);
	m_cptn_colors.BgColor = RGB24(255, 0, 0);
	m_cptn_colors.ActiveFillColor = RGB24(40, 40, 40);
	m_cptn_colors.InactiveFillColor = RGB24(150, 200, 240);
	m_cptn_colors.ActiveBtnHoverColor = RGB24(80, 80, 80);
	m_cptn_colors.InactiveBtnHoverColor = RGB24(130, 180, 220);
	m_cptn_colors.ActiveItemColor = RGB24(0, 255, 255);
	m_cptn_colors.InactiveItemColor = RGB24(40, 40, 40);
	m_cptn_colors.ActiveBtnClickColor = RGB24(120, 120, 120);
	m_cptn_colors.CloseBtnHoverColor = RGB24(255, 0, 0);
	m_cptn_colors.CloseBtnClickColor = RGB24(0, 0, 255);
	m_cptn_colors.CloseBtnItemColor = RGB24(255, 255, 255);
	m_cptn_colors.SelectedTabColor = RGB24(255, 255, 255);
	m_cptn_colors.HoveredTabColor = RGB24(210, 210, 210);
	m_cptn_colors.UnselectedTabColor = RGB24(180, 180, 180);
	// 현재 윈도우의 크기에 맞는 GDI+ 출력 장치를 생성한 다음 캡션을 그림
	m_gdip.CreateGDIP(m_client_rect.right + 1, m_client_rect.bottom + 1);
	DrawCaptionImage();
	// 현재 선택된 캡션 탭과 연결된 다이얼로그를 보이게 함
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
	// 선택된 캡션 탭 다이얼로그에게 포커스를 설정함
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::OnCreateCaption()
{
	StartEventCapture(); // 프로그램이 시작하면 마우스 모니터링 시작
	m_dpi = ::GetDpiForWindow(mp_wnd->m_hWnd); // 확대 배율이 적용된 값의 계산을 위해 시스템의 DPI 값 구함
	m_top_border_height = GetDPIScale(TOP_FRAME_HEIGHT, m_dpi); // 상단 프레임의 높이 설정
	// 캡션 버튼 크기 설정
	m_btn_width = GetDPIScale(CAPTION_BTN_WIDTH, m_dpi);
	m_btn_height = GetDPIScale(CAPTION_BTN_HEIGHT, m_dpi);
	// 캡션 탭 크기 설정
	m_tab_width = GetDPIScale(CAPTION_TAB_WIDTH, m_dpi);
	m_tab_height = m_btn_height + CAPTION_TOPBOTTOM_BORDERS;
	// 캡션 영역으로 확장된 클라이언트 영역을 강제로 그리기 위해 WM_NCCALCSIZE 메시지를 발생시켜서
	// 프로그램에게 프레임이 변경됨을 알림
	mp_wnd->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	SetCaptionRectPos(); // 사용자 정의 캡션에 필요한 사각형 영역들의 좌표를 구함
}

void TXYC_CustomCaption::OnActivateCaption(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	m_active_state = nState != WA_INACTIVE;
	// 윈도우가 비활성화된 상태라면 클릭된 캡션 버튼 무효화
	if (!m_active_state)
	{
		m_clicked_btn = SelectedBtn_None;
	}
	// 윈도우의 활성화 상태가 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}

UINT TXYC_CustomCaption::OnNcCalcSizeCaption(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (bCalcValidRects)
	{
		// 윈도우가 전체화면 상태인 경우와 아닌 경우를 나눠서 윈도우 크기 조정
		if (IsWindowMaximized())
		{
			AdjustMaximizedClientRect(lpncsp->rgrc);
		}
		else
		{
			AdjustUnMaximizedClientRect(lpncsp->rgrc);
		}
		return 1; // 이 함수 안에서 수행된 작업이 있음
	}
	else
	{
		return 0; // 이 함수 안에서 수행된 작업이 없음
	}
}

LRESULT TXYC_CustomCaption::OnNcHitTestCaption(CPoint point)
{
	// 아래의 모든 판정들에 대한 결과가 거짓일 경우에 대비하여 마우스가 현재 호버링중인 캡션 버튼이나
	// 캡션 탭이 없다고 설정함
	m_hovered_btn = SelectedBtn_None;
	m_hovered_tab = -1;
	// 캡션 버튼 영역 판정 기능 수행
	if (::PtInRect(&m_cptn_rects.MinimizeBtn, point))
	{
		m_hovered_btn = SelectedBtn_Minimize;
		return HTCLIENT;
	}
	else if (::PtInRect(&m_cptn_rects.MaximizeBtn, point))
	{
		m_hovered_btn = SelectedBtn_Maximize;
		return HTCLIENT;
	}
	else if (::PtInRect(&m_cptn_rects.CloseBtn, point))
	{
		m_hovered_btn = SelectedBtn_Close;
		return HTCLIENT;
	}
	// 캡션 탭 영역 판정 기능 수행
	if (::PtInRect(&m_cptn_rects.CaptionTab, point))
	{
		m_hovered_tab = point.x / m_tab_width;
		return HTCLIENT;
	}
	// WM_NCCALCSIZE 메시지 처리 함수 동작 때문에 윈도우의 상단 프레임이 없어졌으니 직접 설정한
	// 상단 프레임 영역으로 전체화면 상태에서는 윈도우의 상단 프레임이 없는 것처럼 취급함
	if (::PtInRect(&m_cptn_rects.TopFrame, point) && !IsWindowMaximized())
	{
		return HTTOP;
	}
	// 캡션 영역을 직접 설정했기 때문에 캡션 영역에 대한 선택 동작도 별도로 수행함
	if (point.y < m_cptn_rects.Caption.bottom)
	{
		return HTCAPTION;
	}
	// 위의 모든 선택 동작에 해당하지 않는 동작은 클라이언트 영역 선택 동작으로 처리
	return HTCLIENT;
}

BOOL TXYC_CustomCaption::OnSetCursorCaption(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// 마우스가 캡션 버튼이나 캡션 탭, 클라이언트 영역에 클릭된 상태라면 마우스가 크기 조절 프레임 안에
	// 있어도 마우스 커서를 기본 화살표 커서로 유지함
	if (m_is_clicked)
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	}
	return FALSE;
}

void TXYC_CustomCaption::OnNcMouseMoveCaption(UINT nHitTest, CPoint point)
{
	if (!m_nc_hover_state)
	{
		m_nc_hover_state = 1;
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT),TME_NONCLIENT,mp_wnd->m_hWnd };
		::TrackMouseEvent(&tme);
	}
}

void TXYC_CustomCaption::OnNcMouseLeaveCaption()
{
	if (m_nc_hover_state)
	{
		m_nc_hover_state = 0;
	}
	// 마우스가 논클라이언트 영역 바깥으로 나가면 마우스가 이전에 호버링하던 캡션 버튼이나 캡션 탭이
	// 없다고 설정함
	m_old_hovered_btn = SelectedBtn_None;
	m_old_hovered_tab = -1;
	// 마우스 이동 상태가 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}

void TXYC_CustomCaption::OnSizeCaption(UINT nType, int cx, int cy)
{
	// 윈도우가 최소화된 상태에서는 캡션 버튼과 캡션 탭들의 영역 계산을 하지 않음(윈도우가 최소화된
	// 상태에서는 cx, cy 매개 변수에 0이 넘어옴)
	if (cx && cy)
	{
		// 변경된 윈도우 크기에 맞게 클라이언트 영역 크기 정보를 수정함
		m_client_rect.right = cx;
		m_client_rect.bottom = cy;
		SetCaptionRectPos(); // 변경된 윈도우 크기에 맞는 사용자 정의 캡션에 필요한 사각형 영역들의 좌표를 구함
		SetCaptionTabPos(); // 캡션 탭들과 캡션 탭에 연결된 다이얼로그들의 좌표를 설정함
		m_gdip.CreateGDIP(cx, cy); // 변경된 윈도우 크기에 맞는 GDI+ 출력 장치를 생성함
		// 변경된 윈도우 크기가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
	}
}

void TXYC_CustomCaption::OnRawInputCaption(UINT nInputcode, HRAWINPUT hRawInput)
{
	::ZeroMemory(&m_rawinput, m_nSize); // RAWINPUT 입력 데이터 초기화
	// RAWINPUT 입력 데이터를 얻어옴
	::GetRawInputData(hRawInput, RID_INPUT, &m_rawinput, &m_nSize, sizeof(RAWINPUTHEADER));
	if (m_rawinput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
	{
		// 마우스 클릭이 해제되면 클릭한 클라이언트 영역은 무조건 없음
		m_is_clicked = Clicked_None;
		// 아래 판정에 대한 결과가 거짓일 경우에 대비하여 기능을 수행할 캡션 버튼이 없다고 설정함
		m_triggered_btn = SelectedBtn_None;
		// 마우스로 클릭했던 캡션 버튼 위에서 마우스 클릭을 해제했다면 캡션 버튼 기능 수행을 위해
		// 클릭해제한 캡션 버튼을 기억함
		if (m_hovered_btn == m_clicked_btn)
		{
			m_triggered_btn = m_clicked_btn;
		}
		// 마우스 클릭이 해제되면 현재 클릭된 캡션 버튼은 무조건 없음
		m_clicked_btn = SelectedBtn_None;

	}
}

UINT TXYC_CustomCaption::OnLBtnDownCaption(UINT nFlags, CPoint point)
{
	// 마우스 클릭이 발생했는데 어떠한 캡션 버튼도 캡션 탭도 클릭되지 않았다면 클라이언트 영역이 클릭됨
	if (!m_hovered_btn && m_hovered_tab == -1)
	{
		m_is_clicked = Clicked_ClientArea; // 클라이언트 영역이 클릭된 상태임을 표시함
		return 0; // 이 함수 안에서 수행된 작업이 없음
	}
	// 클릭한 캡션 버튼의 드래그 동작 실행을 방지하기 위해 기본 루틴을 실행하지 않음
	if (m_hovered_btn)
	{
		// 현재 호버링중인 캡션 버튼을 클릭했다면 해당 버튼을 클릭한 캡션 버튼으로 설정
		m_clicked_btn = m_hovered_btn;
		m_is_clicked = Clicked_CaptionBtn; // 캡션 버튼이 클릭된 상태임을 표시함
	}
	// 클릭한 캡션 탭의 드래그 동작 실행을 방지하기 위해 기본 루틴을 실행하지 않음
	else if (m_hovered_tab != -1)
	{
		// 선택되어 있는 캡션 탭을 클릭했을 경우에는 선택한 캡션 탭을 변경할 필요 없음
		if (m_selected_tab != m_hovered_tab)
		{
			// 이전에 선택했던 캡션 탭과 연결된 다이얼로그를 보이지 않게 만들고 현재 호버링중인 캡션 탭과
			// 연결된 다이얼로그를 보이게 만듦
			m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_HIDE);
			m_tab_data[m_hovered_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
			// 현재 호버링중인 캡션 탭을 클릭했기 때문에 해당 탭을 선택한 캡션 탭으로 설정
			m_selected_tab = m_hovered_tab;
			// 선택된 캡션 탭 다이얼로그에게 포커스를 설정함
			::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
		}
		m_is_clicked = Clicked_CaptionTab; // 캡션 탭이 클릭된 상태임을 표시함
	}
	// 마우스 클릭 상태가 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	return 1; // 이 함수 안에서 수행된 작업이 있음
}

UINT TXYC_CustomCaption::OnLBtnUpCaption(UINT nFlags, CPoint point)
{
	// 클릭한 버튼과 클릭해제한 버튼이 다르다면 캡션 버튼의 기능을 수행하지 않음
	switch (m_triggered_btn)
	{
	case SelectedBtn_Minimize:
		mp_wnd->ShowWindow(SW_MINIMIZE); // 윈도우를 최소화시킴
		// 최소화 버튼의 기능을 수행하고 나서도 마우스가 최소화 버튼에 호버링중인지 확인함
		OnNcHitTestCaption(point);
		// 마우스 클릭 해제 상태가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 1;
	case SelectedBtn_Maximize:
		// 전체화면 상태가 아닌 윈도우는 최대화하고 전체화면 상태인 윈도우는 이전 크기로 복원시킴
		mp_wnd->ShowWindow(IsWindowMaximized() ? SW_NORMAL : SW_MAXIMIZE);
		// 최대화 버튼의 기능을 수행하고 나서도 마우스가 최대화 버튼에 호버링중인지 확인함
		OnNcHitTestCaption(point);
		// 마우스 클릭 해제 상태가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 1;
	case SelectedBtn_Close:
		mp_wnd->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0); // 윈도우에게 닫기 명령을 보냄
		return 1;
	default:
		// 마우스 클릭 해제 상태가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 0; // 기능을 수행할 캡션 버튼이 없다면 이 함수 안에서 수행된 작업이 없음
	}
}

void TXYC_CustomCaption::OnMouseMoveCaption(UINT nFlags, CPoint point)
{
	if (!m_hover_state)
	{
		m_hover_state = 1;
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT),TME_HOVER | TME_LEAVE,mp_wnd->m_hWnd };
		::TrackMouseEvent(&tme);
	}
	// 마우스가 현재 호버링중인 캡션 버튼이 이전에 호버링했던 캡션 버튼과 다를 경우
	if (m_old_hovered_btn != m_hovered_btn)
	{
		// 이전에 호버링했던 캡션 버튼을 현재 호버링중인 캡션 버튼으로 변경함
		m_old_hovered_btn = m_hovered_btn;
		// 변경된 캡션 버튼 호버링 상태가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->InvalidateRect(&m_cptn_rects.Caption, FALSE);
	}
	// 마우스가 현재 호버링중인 캡션 탭이 이전에 호버링했던 캡션 탭과 다른 경우
	if (m_old_hovered_tab != m_hovered_tab)
	{
		// 이전에 호버링했던 캡션 탭을 현재 호버링중인 캡션 탭으로 변경함
		m_old_hovered_tab = m_hovered_tab;
		// 변경된 캡션 탭 호버링 상태가 반영된 캡션을 그림
		DrawCaptionImage();
		mp_wnd->InvalidateRect(&m_cptn_rects.CaptionTab, FALSE);
	}
}

void TXYC_CustomCaption::OnMouseLeaveCaption()
{
	if (m_hover_state)
	{
		m_hover_state = 0;
	}
	// 마우스가 클라이언트 영역 바깥으로 나가면 마우스가 현재 호버링중인 캡션 버튼이나 캡션 탭도 없고
	// 마우스가 이전에 호버링하던 캡션 버튼이나 캡션 탭도 없다고 설정함
	m_hovered_btn = m_old_hovered_btn = SelectedBtn_None;
	m_hovered_tab = m_old_hovered_tab = -1;
	// 마우스 이동 상태가 반영된 캡션을 그림
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}