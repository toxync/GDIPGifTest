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
	// ��¥ ��� �׵θ��� ���� Ŭ���̾�Ʈ ������ ���� �����ϰ� ����
	*ap_fake_top_border_rect = m_client_rect;
	ap_fake_top_border_rect->bottom = ap_fake_top_border_rect->top + FAKE_TOP_BORDER_HEIGHT;
}

void TXYC_CustomCaption::SetCaptionRectPos()
{
	// ĸ�� ������ ��ǥ ����
	UINT caption_height = m_btn_height + CAPTION_TOPBOTTOM_BORDERS;
	// �����찡 ��üȭ���� �ƴ϶�� ĸ�� ���� �׷����� ���� Ȯ���� ���� ĸ�� ������ ���̸� �ø�
	if (!IsWindowMaximized())
	{
		// ĸ�� �ǰ� ������ ��� ������ ���̿� ������ �߰��ؼ� �Ǽ��� ��� �������� Ŭ���ϴ� ���� ����
		caption_height += m_top_border_height + GetDPIScale(TOP_FRAME_SPACING, m_dpi);
	}
	m_cptn_rects.Caption = m_client_rect; // ĸ���� ���� Ŭ���̾�Ʈ ������ ���� �����ϰ� ����
	m_cptn_rects.Caption.bottom = m_cptn_rects.Caption.top + caption_height;
	// ĸ�� ��ư���� ��ǥ ����
	// ���ʿ��� ���������� '�ּ�ȭ ��ư - �ִ�ȭ ��ư - �ݱ� ��ư' ������� ��ġ�ǰ� �ݱ� ��ư�� ĸ����
	// ���κа� ���� ������ ������ ĸ�� ������ ��ǥ�� ���� �ݱ� ��ư�� ��ǥ�� ���� ���� ������ ����
	// �ݱ� ��ư�� ��ǥ�� �ִ�ȭ ��ư�� ��ǥ��, �ִ�ȭ ��ư�� ��ǥ�� �ּ�ȭ ��ư�� ��ǥ�� ������
	m_cptn_rects.CloseBtn = m_cptn_rects.Caption;
	// �ݱ� ��ư�� ������ ĸ�� ��ư���� ��¥ ��� �׵θ� ���� �������� �ʵ��� �ݱ� ��ư�� ��ǥ ����
	m_cptn_rects.CloseBtn.top += FAKE_TOP_BORDER_HEIGHT;
	// �ݱ� ��ư�� ��ǥ ����
	m_cptn_rects.CloseBtn.bottom = m_cptn_rects.CloseBtn.top + m_btn_height;
	m_cptn_rects.CloseBtn.left = m_cptn_rects.CloseBtn.right - m_btn_width;
	// �ִ�ȭ ��ư�� ��ǥ ����
	m_cptn_rects.MaximizeBtn = m_cptn_rects.CloseBtn;
	m_cptn_rects.MaximizeBtn.left -= m_btn_width;
	m_cptn_rects.MaximizeBtn.right -= m_btn_width;
	// �ּ�ȭ ��ư�� ��ǥ ����
	m_cptn_rects.MinimizeBtn = m_cptn_rects.MaximizeBtn;
	m_cptn_rects.MinimizeBtn.left -= m_btn_width;
	m_cptn_rects.MinimizeBtn.right -= m_btn_width;
	// ��� ������ ������ ��ǥ ����
	// ��� �������� ���� ��� ��ǥ�� ĸ�� ������ ���� ��� ��ǥ�� ����
	m_cptn_rects.TopFrame = m_cptn_rects.Caption;
	// ��� �������� ���� ��ǥ�� �ּ�ȭ ��ư�� ���� ��ǥ�� ����
	m_cptn_rects.TopFrame.right = m_cptn_rects.MinimizeBtn.left;
	// ��� �������� �ϴ� ��ǥ�� �����ص� ��� ������ ���� ������ ����
	m_cptn_rects.TopFrame.bottom = m_top_border_height;
}

void TXYC_CustomCaption::SetCaptionTabPos()
{
	// ĸ�� �� ��ü ������ ���� �ϴ� ��ǥ�� ĸ�� ������ ���� �ϴ� ��ǥ�� ����
	m_cptn_rects.CaptionTab = m_cptn_rects.Caption;
	// ĸ�� �� ��ü ������ ��� ��ǥ�� ĸ�� �� ��ü ������ �ϴ� ��ǥ���� ĸ�� �� ���̸�ŭ �� ��ǥ
	m_cptn_rects.CaptionTab.top = m_cptn_rects.CaptionTab.bottom - m_tab_height;
	// ĸ�� �� ��ü ������ ���� ��ǥ�� (ĸ�� �� �� ���� ��) * (ĸ�� �� ����)
	m_cptn_rects.CaptionTab.right = m_tab_width * m_tab_count;
	UINT left = 0, top = 0, width = 0, height = 0;
	UINT text_spacing = GetDPIScale(CAPTION_TAB_TEXT_SPACING, m_dpi);
	for (int i = 0; i < m_tab_count; ++i)
	{
		// �� ĸ�� ���� �̸� ���ڿ� ��� ���� ��ǥ�� ������
		m_tab_data[i].TextArea.X = m_cptn_rects.CaptionTab.left + i * m_tab_width + text_spacing;
		m_tab_data[i].TextArea.Y = m_cptn_rects.CaptionTab.top + text_spacing;
		m_tab_data[i].TextArea.Width = m_tab_width - text_spacing * 2;
		m_tab_data[i].TextArea.Height = m_tab_height - text_spacing * 2;
		// ĸ�� �ǰ� ����� ���̾�α��� ��ġ�� ũ�⸦ ������
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
	// ��üȭ�� ���� �������� Ŭ���̾�Ʈ ������ ����� �������� �۾� ǥ������ ������ ����
	*ap_rect = monitor_info.rcWork;
}

void TXYC_CustomCaption::AdjustUnMaximizedClientRect(RECT* ap_rect)
{
	// ��üȭ�� ���°� �ƴ� �������� Ŭ���̾�Ʈ ������ ������ ��ü �������� ������ ������ ������ ����
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
	// ���콺 RAWINPUTDEVICE ����ü ����
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
	// ���콺 RAWINPUTDEVICE ����ü ����
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
	// ��ü Ŭ���̾�Ʈ ���� ä���
	m_gdip.Clear(m_cptn_colors.BgColor);
	// ĸ�� ���� �׸���
	// �������� Ȱ��ȭ ���¿� ���� ĸ���� ä��� ���� ����
	p_caption_color = m_active_state ? &m_cptn_colors.ActiveFillColor : &m_cptn_colors.InactiveFillColor;
	p_rect = &m_cptn_rects.Caption;
	left = p_rect->left;
	top = p_rect->top;
	width = p_rect->right - p_rect->left;
	height = p_rect->bottom - p_rect->top;
	m_gdip.FillRect(left, top, width, height, *p_caption_color);
	m_gdip.DrawRect(left, top, width, height, *p_caption_color);
	// ĸ�� �� �׸���
	p_rect = &m_cptn_rects.CaptionTab;
	left = p_rect->left;
	top = p_rect->top;
	width = p_rect->right - p_rect->left;
	height = p_rect->bottom - p_rect->top;
	// ��ü ĸ�� �� ���� ä���
	m_gdip.FillRect(left, top, width, height, m_cptn_colors.UnselectedTabColor);
	m_gdip.DrawRect(left, top, width, height, 0xFF000000);
	// ȣ������ ĸ�� �� �׸���
	width = m_tab_width;
	height = m_tab_height;
	if (m_hovered_tab != m_selected_tab && !m_is_clicked)
	{
		left = p_rect->left + m_tab_width * m_hovered_tab;
		top = p_rect->top;
		m_gdip.FillRect(left, top, width, height, m_cptn_colors.HoveredTabColor);
	}
	// ���õ� ĸ�� �� �׸���
	left = p_rect->left + m_tab_width * m_selected_tab;
	top = p_rect->top;
	m_gdip.FillRect(left, top, width, height, m_cptn_colors.SelectedTabColor);
	// ĸ�� �ǵ� ������ ���м� �׸���
	m_gdip.SetPenColor(0xFF000000);
	for (int i = 1; i < m_tab_count; ++i)
	{
		sx = p_rect->left + m_tab_width * i;
		sy = p_rect->top;
		ex = sx;
		ey = p_rect->bottom;
		m_gdip.DrawLine(sx, sy, ex, ey);
	}
	// ĸ�� �ǵ��� �̸� ���ڿ� ���
	for (int i = 0; i < m_tab_count; ++i)
	{
		left = (UINT)m_tab_data[i].TextArea.GetLeft();
		top = (UINT)m_tab_data[i].TextArea.GetTop();
		width = (UINT)m_tab_data[i].TextArea.Width;
		height = (UINT)m_tab_data[i].TextArea.Height;
		// m_gdip.DrawRect(left, top, width, height, 0xFFFF0000);
		m_gdip.DrawText(&m_tab_data[i].TextArea, m_tab_data[i].TabText);
	}
	// ĸ�� ��ư �׸���
	m_icon_dimension = GetDPIScale(10, m_dpi);
	p_btn_color = p_caption_color;
	// ȣ�����̳� Ŭ�� ���¿� ���� ĸ�� ��ư ä��� ���� ����
	switch (m_hovered_btn)
	{
	case SelectedBtn_Minimize:
		// �����찡 ��Ȱ��ȭ�� ������ ������ ���� ��ư�� ���콺�� ȣ���� ���̶�� Ŭ�� ���¿� ��� ����
		// ��ư�� ȣ���� ���¸� ǥ����
		if (!m_active_state)
		{
			p_btn_color = &m_cptn_colors.InactiveBtnHoverColor;
		}
		// �����찡 Ȱ��ȭ�� ������ ������ Ŭ���� ĸ�� ��ư�̳� ĸ�� ���� ���� ���� ��ư�� ���콺��
		// ȣ���� ���� ��쿡�� ��ư�� ȣ���� ���¸� ǥ����
		else if (!m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnHoverColor;
		}
		// ���� ��ư�� Ŭ���� ���·� ���콺�� ���� ��ư�� ȣ���� ���� ��쿡�� ��ư�� Ŭ�� ���¸� ǥ����
		// (���� ��ư�� �ƴ� �ٸ� ��ư�� Ŭ���� ���·� ���� ��ư�� ���콺�� ȣ���� ���̶�� ���� ��ư��
		// ȣ�����̳� Ŭ�� ���¸� ǥ������ ����)
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
		// �����찡 ��Ȱ��ȭ�� ������ ������ ���� ��ư�� ���콺�� ȣ���� ���̶�� Ŭ�� ���¿� ��� ����
		// ��ư�� ȣ���� ���¸� ǥ����
		if (!m_active_state)
		{
			p_btn_color = &m_cptn_colors.InactiveBtnHoverColor;
		}
		// �����찡 Ȱ��ȭ�� ������ ������ Ŭ���� ĸ�� ��ư�̳� ĸ�� ���� ���� ���� ��ư�� ���콺��
		// ȣ���� ���� ��쿡�� ��ư�� ȣ���� ���¸� ǥ����
		else if (!m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.ActiveBtnHoverColor;
		}
		// ���� ��ư�� Ŭ���� ���·� ���콺�� ���� ��ư�� ȣ���� ���� ��쿡�� ��ư�� Ŭ�� ���¸� ǥ����
		// (���� ��ư�� �ƴ� �ٸ� ��ư�� Ŭ���� ���·� ���� ��ư�� ���콺�� ȣ���� ���̶�� ���� ��ư��
		// ȣ�����̳� Ŭ�� ���¸� ǥ������ ����)
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
		// �����찡 ��Ȱ��ȭ�� �����̰ų� �����찡 Ȱ��ȭ�� �����̸鼭 Ŭ���� ĸ�� ��ư�̳� ĸ�� ���� ���� ��쿡��
		// ������ ĸ�� ��ư����� �ٸ� ȣ���� �������� �ݱ� ��ư�� ȣ���� ���¸� ǥ����
		if (!m_active_state || !m_is_clicked)
		{
			p_btn_color = &m_cptn_colors.CloseBtnHoverColor;
			p_close_btn_item_color = &m_cptn_colors.CloseBtnItemColor;
		}
		// ���� ��ư�� Ŭ���� ���·� ���콺�� ���� ��ư�� ȣ���� ���� ��쿡�� ������ ĸ�� ��ư����� �ٸ�
		// Ŭ�� �������� �ݱ� ��ư�� Ŭ�� ���¸� ǥ����(���� ��ư�� �ƴ� �ٸ� ��ư�� Ŭ���� ���·� ���� ��ư��
		// ���콺�� ȣ���� ���̶�� ���� ��ư�� ȣ�����̳� Ŭ�� ���¸� ǥ������ ����)
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
	// �������� ��Ŀ�� ���¿� ���� ĸ�� ��ư �������� ���� ����
	p_item_color = m_active_state ? &m_cptn_colors.ActiveItemColor : &m_cptn_colors.InactiveItemColor;
	// '-' ����� �ּ�ȭ ��ư ������ �׸���
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
	// �ִ�ȭ ��ư ������ �׸���
	m_icon_rect = { 0,0,0,0 };
	// �����찡 ��üȭ�� ������ ��쿡�� �ִ�ȭ ��ư�� �������� ���� �簢�� 2���� ����
	if (IsWindowMaximized())
	{
		// �����찡 ��üȭ�� ������ �� ���콺�� �ִ�ȭ ��ư�� ȣ�������� ��찡 �ƴ϶�� �ִ�ȭ ��ư��
		// ������ ���� ä��� ������ ������ �ٱ��� �ִ�ȭ ��ư ����� ������ ĸ�� ä��� �����̾�� ��
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
		// �ִ�ȭ ��ư �Ѱ���� �簢�� �ϳ� ���� �׸�
		m_gdip.DrawRect(left, top, width, height, *p_item_color);
		// ���� �׸� �簢���� ���� �ϴ����� �̵��� ������ �簢�� �ϳ� �� �׸�
		p_rect->left -= GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->top += GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->right -= GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		p_rect->bottom += GetDPIScale(MAXIMIZE_BTN_RECT_OFFSET, m_dpi);
		// �� �簢���� ������ �κ��� ������ �ʵ��� ���߿� �׸� �簢���� ���θ� ä��
		left = p_rect->left;
		top = p_rect->top;
		m_gdip.FillRect(left, top, width, height, *p_btn_color);
		// ���߿� �׸� �簢���� �׵θ� �׸�
		m_gdip.DrawRect(left, top, width, height, *p_item_color);
	}
	else // �����찡 ��üȭ�� ���°� �ƴ� ��쿡�� �ִ�ȭ ��ư�� �������� ū �簢�� 1���� ����
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
	// Ŭ���� ĸ�� ��ư�̳� ĸ�� ���� ���� �ݱ� ��ư�� ���콺�� ȣ���� ���̰ų� �ݱ� ��ư�� Ŭ���� ���·�
	// ���콺�� �ݱ� ��ư�� ȣ���� ���� ��쿡�� �ݱ� ��ư ���� ������ ������ ����
	if (!p_close_btn_item_color)
	{
		p_close_btn_item_color = p_item_color;
	}
	// 'X' ����� �ݱ� ��ư ������ �׸���
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
	// ��¥ ��� �׵θ� �׸���
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
	// �Ű� ������ �Ѱܹ��� ���̾�α��� �ּҿ� ���ҽ� ID�� �ڽ� ���̾�α� ������ ����
	m_tab_data[m_tab_count].p_TabDlg = ap_dlg;
	m_tab_data[m_tab_count].p_TabDlg->Create(a_dlg_id, mp_wnd);
	// wcslen() �Լ��� NULL ���ڸ� ������ ���ڿ��� ���̸� ��ȯ�ϹǷ� �Ű� ������ �Ѱ� ���� ���ڿ���
	// NULL ���ڰ� ���Ե� ���ڿ� ���̴� ((wcslen() �Լ��� ��ȯ�� ���ڿ� ����) + 1)
	UINT64 tab_text_len = wcslen(ap_text) + 1;
	// �����ڵ�(wchar_t �ڷ���) ���ڿ��� �� ������ ũ�Ⱑ 2����Ʈ�̹Ƿ� �����Ϸ��� �����ڵ� ���ڿ���
	// ��ü ũ��� (�����ڵ� ���ڿ��� ��ü ����) * 2
	memcpy(m_tab_data[m_tab_count].TabText, ap_text, tab_text_len << 1);
	++m_tab_count; // ĸ�� �� ���� ����
}

UINT8 TXYC_CustomCaption::GetCloseReadyStatus()
{
	UINT8 received_target_msg = 0, close_ready_status = 1;
	MSG msg;
	for (int i = 0; i < m_tab_count; ++i)
	{
		received_target_msg = 0;
		// ĸ�� �� ���̾�α׿��� ���� �غ� �ƴ��� Ȯ���ϴ� �޽����� ����
		::SendMessage(m_tab_data[i].p_TabDlg->m_hWnd, TABWND_CLOSE_READY, 0, 0);
		// ĸ�� �� ���̾�α׷κ��� ���� �غ� ���� �޽����� ���� ������ ���� �޽������� ��� Ȯ����
		while (!received_target_msg)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// ĸ�� �� ���̾�α׷κ��� ���� �غ� ���� �޽����� �޾Ҵٸ�
				if (msg.message == TABWND_CLOSE_READY)
				{
					// �޽����� �� �̻� Ȯ������ �ʵ��� �ݺ��� ���ǰ��� �����Ŵ
					received_target_msg = 1;
					// ĸ�� �� ���̾�α��� ���� �غ� ���¸� Ȯ����(���� �غ� ���� ���� ĸ�� ��
					// ���̾�αװ� �ϳ��� �ִٸ� ���� �غ� ���¸� �������� ������)
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
	// ���� ĸ�� �ǰ� ����� ���̾�α׸� ������ �ʰ� ����� ���� ĸ�� ���� ���� �ǰ�
	// ����� ���̾�α׸� ���̰� ����
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_HIDE);
	m_selected_tab = (m_selected_tab + 1) % m_tab_count;
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
	// ���õ� ĸ�� ���� ������ �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	// ���õ� ĸ�� �� ���̾�α׿��� ��Ŀ���� ������
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::NotifyPrevTabWnd()
{
	// ���� ĸ�� �ǰ� ����� ���̾�α׸� ������ �ʰ� ����� ���� ĸ�� ���� ���� �ǰ�
	// ����� ���̾�α׸� ���̰� ����
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
	// ���õ� ĸ�� ���� ������ �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	// ���õ� ĸ�� �� ���̾�α׿��� ��Ŀ���� ������
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::OnInitCaption()
{
	// ĸ�� �� �̸� ���ڿ��� ����� �۲��� ������ ũ�⸦ ������
	m_gdip.ChangeFont(L"���� ���", GetDPIScale(CAPTION_TAB_FONT, m_dpi));
	// ����� ���� ĸ�ǿ��� ����� ������� �ʱ�ȭ
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
	// ���� �������� ũ�⿡ �´� GDI+ ��� ��ġ�� ������ ���� ĸ���� �׸�
	m_gdip.CreateGDIP(m_client_rect.right + 1, m_client_rect.bottom + 1);
	DrawCaptionImage();
	// ���� ���õ� ĸ�� �ǰ� ����� ���̾�α׸� ���̰� ��
	m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
	// ���õ� ĸ�� �� ���̾�α׿��� ��Ŀ���� ������
	::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
}

void TXYC_CustomCaption::OnCreateCaption()
{
	StartEventCapture(); // ���α׷��� �����ϸ� ���콺 ����͸� ����
	m_dpi = ::GetDpiForWindow(mp_wnd->m_hWnd); // Ȯ�� ������ ����� ���� ����� ���� �ý����� DPI �� ����
	m_top_border_height = GetDPIScale(TOP_FRAME_HEIGHT, m_dpi); // ��� �������� ���� ����
	// ĸ�� ��ư ũ�� ����
	m_btn_width = GetDPIScale(CAPTION_BTN_WIDTH, m_dpi);
	m_btn_height = GetDPIScale(CAPTION_BTN_HEIGHT, m_dpi);
	// ĸ�� �� ũ�� ����
	m_tab_width = GetDPIScale(CAPTION_TAB_WIDTH, m_dpi);
	m_tab_height = m_btn_height + CAPTION_TOPBOTTOM_BORDERS;
	// ĸ�� �������� Ȯ��� Ŭ���̾�Ʈ ������ ������ �׸��� ���� WM_NCCALCSIZE �޽����� �߻����Ѽ�
	// ���α׷����� �������� ������� �˸�
	mp_wnd->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	SetCaptionRectPos(); // ����� ���� ĸ�ǿ� �ʿ��� �簢�� �������� ��ǥ�� ����
}

void TXYC_CustomCaption::OnActivateCaption(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	m_active_state = nState != WA_INACTIVE;
	// �����찡 ��Ȱ��ȭ�� ���¶�� Ŭ���� ĸ�� ��ư ��ȿȭ
	if (!m_active_state)
	{
		m_clicked_btn = SelectedBtn_None;
	}
	// �������� Ȱ��ȭ ���°� �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}

UINT TXYC_CustomCaption::OnNcCalcSizeCaption(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (bCalcValidRects)
	{
		// �����찡 ��üȭ�� ������ ���� �ƴ� ��츦 ������ ������ ũ�� ����
		if (IsWindowMaximized())
		{
			AdjustMaximizedClientRect(lpncsp->rgrc);
		}
		else
		{
			AdjustUnMaximizedClientRect(lpncsp->rgrc);
		}
		return 1; // �� �Լ� �ȿ��� ����� �۾��� ����
	}
	else
	{
		return 0; // �� �Լ� �ȿ��� ����� �۾��� ����
	}
}

LRESULT TXYC_CustomCaption::OnNcHitTestCaption(CPoint point)
{
	// �Ʒ��� ��� �����鿡 ���� ����� ������ ��쿡 ����Ͽ� ���콺�� ���� ȣ�������� ĸ�� ��ư�̳�
	// ĸ�� ���� ���ٰ� ������
	m_hovered_btn = SelectedBtn_None;
	m_hovered_tab = -1;
	// ĸ�� ��ư ���� ���� ��� ����
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
	// ĸ�� �� ���� ���� ��� ����
	if (::PtInRect(&m_cptn_rects.CaptionTab, point))
	{
		m_hovered_tab = point.x / m_tab_width;
		return HTCLIENT;
	}
	// WM_NCCALCSIZE �޽��� ó�� �Լ� ���� ������ �������� ��� �������� ���������� ���� ������
	// ��� ������ �������� ��üȭ�� ���¿����� �������� ��� �������� ���� ��ó�� �����
	if (::PtInRect(&m_cptn_rects.TopFrame, point) && !IsWindowMaximized())
	{
		return HTTOP;
	}
	// ĸ�� ������ ���� �����߱� ������ ĸ�� ������ ���� ���� ���۵� ������ ������
	if (point.y < m_cptn_rects.Caption.bottom)
	{
		return HTCAPTION;
	}
	// ���� ��� ���� ���ۿ� �ش����� �ʴ� ������ Ŭ���̾�Ʈ ���� ���� �������� ó��
	return HTCLIENT;
}

BOOL TXYC_CustomCaption::OnSetCursorCaption(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// ���콺�� ĸ�� ��ư�̳� ĸ�� ��, Ŭ���̾�Ʈ ������ Ŭ���� ���¶�� ���콺�� ũ�� ���� ������ �ȿ�
	// �־ ���콺 Ŀ���� �⺻ ȭ��ǥ Ŀ���� ������
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
	// ���콺�� ��Ŭ���̾�Ʈ ���� �ٱ����� ������ ���콺�� ������ ȣ�����ϴ� ĸ�� ��ư�̳� ĸ�� ����
	// ���ٰ� ������
	m_old_hovered_btn = SelectedBtn_None;
	m_old_hovered_tab = -1;
	// ���콺 �̵� ���°� �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}

void TXYC_CustomCaption::OnSizeCaption(UINT nType, int cx, int cy)
{
	// �����찡 �ּ�ȭ�� ���¿����� ĸ�� ��ư�� ĸ�� �ǵ��� ���� ����� ���� ����(�����찡 �ּ�ȭ��
	// ���¿����� cx, cy �Ű� ������ 0�� �Ѿ��)
	if (cx && cy)
	{
		// ����� ������ ũ�⿡ �°� Ŭ���̾�Ʈ ���� ũ�� ������ ������
		m_client_rect.right = cx;
		m_client_rect.bottom = cy;
		SetCaptionRectPos(); // ����� ������ ũ�⿡ �´� ����� ���� ĸ�ǿ� �ʿ��� �簢�� �������� ��ǥ�� ����
		SetCaptionTabPos(); // ĸ�� �ǵ�� ĸ�� �ǿ� ����� ���̾�α׵��� ��ǥ�� ������
		m_gdip.CreateGDIP(cx, cy); // ����� ������ ũ�⿡ �´� GDI+ ��� ��ġ�� ������
		// ����� ������ ũ�Ⱑ �ݿ��� ĸ���� �׸�
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
	}
}

void TXYC_CustomCaption::OnRawInputCaption(UINT nInputcode, HRAWINPUT hRawInput)
{
	::ZeroMemory(&m_rawinput, m_nSize); // RAWINPUT �Է� ������ �ʱ�ȭ
	// RAWINPUT �Է� �����͸� ����
	::GetRawInputData(hRawInput, RID_INPUT, &m_rawinput, &m_nSize, sizeof(RAWINPUTHEADER));
	if (m_rawinput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
	{
		// ���콺 Ŭ���� �����Ǹ� Ŭ���� Ŭ���̾�Ʈ ������ ������ ����
		m_is_clicked = Clicked_None;
		// �Ʒ� ������ ���� ����� ������ ��쿡 ����Ͽ� ����� ������ ĸ�� ��ư�� ���ٰ� ������
		m_triggered_btn = SelectedBtn_None;
		// ���콺�� Ŭ���ߴ� ĸ�� ��ư ������ ���콺 Ŭ���� �����ߴٸ� ĸ�� ��ư ��� ������ ����
		// Ŭ�������� ĸ�� ��ư�� �����
		if (m_hovered_btn == m_clicked_btn)
		{
			m_triggered_btn = m_clicked_btn;
		}
		// ���콺 Ŭ���� �����Ǹ� ���� Ŭ���� ĸ�� ��ư�� ������ ����
		m_clicked_btn = SelectedBtn_None;

	}
}

UINT TXYC_CustomCaption::OnLBtnDownCaption(UINT nFlags, CPoint point)
{
	// ���콺 Ŭ���� �߻��ߴµ� ��� ĸ�� ��ư�� ĸ�� �ǵ� Ŭ������ �ʾҴٸ� Ŭ���̾�Ʈ ������ Ŭ����
	if (!m_hovered_btn && m_hovered_tab == -1)
	{
		m_is_clicked = Clicked_ClientArea; // Ŭ���̾�Ʈ ������ Ŭ���� �������� ǥ����
		return 0; // �� �Լ� �ȿ��� ����� �۾��� ����
	}
	// Ŭ���� ĸ�� ��ư�� �巡�� ���� ������ �����ϱ� ���� �⺻ ��ƾ�� �������� ����
	if (m_hovered_btn)
	{
		// ���� ȣ�������� ĸ�� ��ư�� Ŭ���ߴٸ� �ش� ��ư�� Ŭ���� ĸ�� ��ư���� ����
		m_clicked_btn = m_hovered_btn;
		m_is_clicked = Clicked_CaptionBtn; // ĸ�� ��ư�� Ŭ���� �������� ǥ����
	}
	// Ŭ���� ĸ�� ���� �巡�� ���� ������ �����ϱ� ���� �⺻ ��ƾ�� �������� ����
	else if (m_hovered_tab != -1)
	{
		// ���õǾ� �ִ� ĸ�� ���� Ŭ������ ��쿡�� ������ ĸ�� ���� ������ �ʿ� ����
		if (m_selected_tab != m_hovered_tab)
		{
			// ������ �����ߴ� ĸ�� �ǰ� ����� ���̾�α׸� ������ �ʰ� ����� ���� ȣ�������� ĸ�� �ǰ�
			// ����� ���̾�α׸� ���̰� ����
			m_tab_data[m_selected_tab].p_TabDlg->ShowWindow(SW_HIDE);
			m_tab_data[m_hovered_tab].p_TabDlg->ShowWindow(SW_SHOWNA);
			// ���� ȣ�������� ĸ�� ���� Ŭ���߱� ������ �ش� ���� ������ ĸ�� ������ ����
			m_selected_tab = m_hovered_tab;
			// ���õ� ĸ�� �� ���̾�α׿��� ��Ŀ���� ������
			::SetFocus(m_tab_data[m_selected_tab].p_TabDlg->m_hWnd);
		}
		m_is_clicked = Clicked_CaptionTab; // ĸ�� ���� Ŭ���� �������� ǥ����
	}
	// ���콺 Ŭ�� ���°� �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
	return 1; // �� �Լ� �ȿ��� ����� �۾��� ����
}

UINT TXYC_CustomCaption::OnLBtnUpCaption(UINT nFlags, CPoint point)
{
	// Ŭ���� ��ư�� Ŭ�������� ��ư�� �ٸ��ٸ� ĸ�� ��ư�� ����� �������� ����
	switch (m_triggered_btn)
	{
	case SelectedBtn_Minimize:
		mp_wnd->ShowWindow(SW_MINIMIZE); // �����츦 �ּ�ȭ��Ŵ
		// �ּ�ȭ ��ư�� ����� �����ϰ� ������ ���콺�� �ּ�ȭ ��ư�� ȣ���������� Ȯ����
		OnNcHitTestCaption(point);
		// ���콺 Ŭ�� ���� ���°� �ݿ��� ĸ���� �׸�
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 1;
	case SelectedBtn_Maximize:
		// ��üȭ�� ���°� �ƴ� ������� �ִ�ȭ�ϰ� ��üȭ�� ������ ������� ���� ũ��� ������Ŵ
		mp_wnd->ShowWindow(IsWindowMaximized() ? SW_NORMAL : SW_MAXIMIZE);
		// �ִ�ȭ ��ư�� ����� �����ϰ� ������ ���콺�� �ִ�ȭ ��ư�� ȣ���������� Ȯ����
		OnNcHitTestCaption(point);
		// ���콺 Ŭ�� ���� ���°� �ݿ��� ĸ���� �׸�
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 1;
	case SelectedBtn_Close:
		mp_wnd->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0); // �����쿡�� �ݱ� ����� ����
		return 1;
	default:
		// ���콺 Ŭ�� ���� ���°� �ݿ��� ĸ���� �׸�
		DrawCaptionImage();
		mp_wnd->Invalidate(FALSE);
		return 0; // ����� ������ ĸ�� ��ư�� ���ٸ� �� �Լ� �ȿ��� ����� �۾��� ����
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
	// ���콺�� ���� ȣ�������� ĸ�� ��ư�� ������ ȣ�����ߴ� ĸ�� ��ư�� �ٸ� ���
	if (m_old_hovered_btn != m_hovered_btn)
	{
		// ������ ȣ�����ߴ� ĸ�� ��ư�� ���� ȣ�������� ĸ�� ��ư���� ������
		m_old_hovered_btn = m_hovered_btn;
		// ����� ĸ�� ��ư ȣ���� ���°� �ݿ��� ĸ���� �׸�
		DrawCaptionImage();
		mp_wnd->InvalidateRect(&m_cptn_rects.Caption, FALSE);
	}
	// ���콺�� ���� ȣ�������� ĸ�� ���� ������ ȣ�����ߴ� ĸ�� �ǰ� �ٸ� ���
	if (m_old_hovered_tab != m_hovered_tab)
	{
		// ������ ȣ�����ߴ� ĸ�� ���� ���� ȣ�������� ĸ�� ������ ������
		m_old_hovered_tab = m_hovered_tab;
		// ����� ĸ�� �� ȣ���� ���°� �ݿ��� ĸ���� �׸�
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
	// ���콺�� Ŭ���̾�Ʈ ���� �ٱ����� ������ ���콺�� ���� ȣ�������� ĸ�� ��ư�̳� ĸ�� �ǵ� ����
	// ���콺�� ������ ȣ�����ϴ� ĸ�� ��ư�̳� ĸ�� �ǵ� ���ٰ� ������
	m_hovered_btn = m_old_hovered_btn = SelectedBtn_None;
	m_hovered_tab = m_old_hovered_tab = -1;
	// ���콺 �̵� ���°� �ݿ��� ĸ���� �׸�
	DrawCaptionImage();
	mp_wnd->Invalidate(FALSE);
}