#include "pch.h"
#include "TXYC_FrameThreadData.h"

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
		// �̺�Ʈ ��ü�� ���� 1�̸� ������ ����
		if (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0)
		{
			kill_flag = 1;
			// �����尡 ������ ����ƴٴ� ����� �۾� ���� ǥ�� ���̾�α׿��� �˸�
			::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
			break;
		}
		p_src_bmp = (*p_frame_vector)[i].p_src_bmp;
		p_frame_bmp = &((*p_frame_vector)[i].p_frame_bmp);
		GdipGetImageWidth(p_src_bmp, &src_cx);
		GdipGetImageHeight(p_src_bmp, &src_cy);
		// ���� �̹����� ���� ���̰� GIF ���� �������� ���� ���̶� �����ϴٸ� ���� �̹����� �״�� ������
		// ������ �̹����� ������
		if ((frame_cx == src_cx) && (frame_cy == src_cy))
		{
			GdipCloneImage(p_src_bmp, (GpImage**)p_frame_bmp);
		}
		else // ���� �̹����� ���� ���̰� GIF ���� �������� ���� ���̶� �������� ���� ���
		{
			// GIF ���� �����Ӱ� ũ�Ⱑ ������ ������ �̹����� ������
			GdipCreateBitmapFromScan0(frame_cx, frame_cy, 0, PixelFormat32bppARGB, NULL, p_frame_bmp);
			// ������ �̹����� ����� Graphics ��ü�� �����Ͽ� ��� �ȼ��� ���������� �ʱ�ȭ��
			GdipGetImageGraphicsContext(*p_frame_bmp, &p_graphics);
			// GdipSetInterpolationMode(p_graphics, InterpolationModeNearestNeighbor);
			GdipGraphicsClear(p_graphics, 0xFF000000);
			image_rect.left = image_rect.top = frame_rect.left = frame_rect.top = 0;
			image_rect.right = src_cx;
			image_rect.bottom = src_cy;
			frame_rect.right = frame_cx;
			frame_rect.bottom = frame_cy;
			// ���� �̹����� ȭ��� ������ ���·� ������ �̹����� �°� Ȯ��/����ϸ鼭 ��� ���Ľ����� ����
			// ���� �̹��� ��ǥ�� ũ�⸦ ����
			CenterFitRectInRect(&image_rect, &frame_rect);
			x = image_rect.left;
			y = image_rect.top;
			resized_src_cx = image_rect.right - image_rect.left;
			resized_src_cy = image_rect.bottom - image_rect.top;
			// ȭ��� ������ ���·� ������ �̹����� �°� Ȯ��/����ϸ鼭 ��� ���Ľ�Ų ���� �̹�����
			// ������ �̹����� �����
			GdipDrawImageRectI(p_graphics, p_src_bmp, x, y, resized_src_cx, resized_src_cy);
			// ������ �̹����� ����� Graphics ��ü�� �� �̻� �ʿ� �����Ƿ� ������
			GdipDeleteGraphics(p_graphics);
		}
		// ������ �̹����� ������ ������ �۾� �ϳ��� �Ϸ�Ǿ��ٴ� �޽����� �۾� ���� ǥ�� ���̾�α׿��� ����
		::SendMessage(p_data->h_progress_wnd, SINGLE_WORK_COMPLETE, 0, 0);
	}
	// ������ �۾��� �������Ƿ� �������� �ڵ��� ����
	::CloseHandle(p_data->h_thread);
	// �����带 ������ ���������� kill_flag�� ���� ���ŵ��� �ʾ��� ��쿡 ����ؼ� �̺�Ʈ ��ü�� ���� �ٽ� Ȯ����
	// (�ݺ����� ������ �ݺ��� �����ϴ� �߿� �̺�Ʈ ��ü�� ���� �����ߴٸ� ����� �̺�Ʈ ��ü�� ���� Ȯ������
	// ���� ���·� �ݺ����� ����Ǿ� kill_flag�� ���� ���ŵ��� ����)
	if ((!kill_flag) && (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0))
	{
		kill_flag = 1;
		// �����尡 ������ ����ƴٴ� ����� �۾� ���� ǥ�� ���̾�α׿��� �˸�
		::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
	}
	// ���� ���̾�α׿��� �����尡 ��������� �˸�
	::PostMessage(p_data->h_main_wnd, POST_FRAME_THREAD_ACTION, kill_flag, (LPARAM)p_data);
	return 0;
}

DWORD WINAPI ConvertFrameBitmaps(void* ap_data)
{
	FrameThreadData* p_data = (FrameThreadData*)ap_data;
	vector<FrameData>* p_frame_vector = p_data->p_frame_vector;
	TXYC_OctreeQuantizer oct_qtzr;
	GpBitmap* p_src_bmp = NULL;
	UINT frame_cx = p_data->frame_cx, frame_cy = p_data->frame_cy, kill_flag = 0;
	for (int i = 0; i < p_frame_vector->size(); ++i)
	{
		// �̺�Ʈ ��ü�� ���� 1�̸� ������ ����
		if (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0)
		{
			kill_flag = 1;
			// �����尡 ������ ����ƴٴ� ����� �۾� ���� ǥ�� ���̾�α׿��� �˸�
			::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
			break;
		}
		// ������ �̹����� 8��Ʈ �÷� ��ȯ ��� �̹����� ������
		p_src_bmp = (*p_frame_vector)[i].p_frame_bmp;
		(*p_frame_vector)[i].p_cvrt_frame_bmp = oct_qtzr.GetQuantizedFrame(p_src_bmp, 5);
		// ������ �̹����� ������ ������ �۾� �ϳ��� �Ϸ�Ǿ��ٴ� �޽����� �۾� ���� ǥ�� ���̾�α׿��� ����
		::SendMessage(p_data->h_progress_wnd, SINGLE_WORK_COMPLETE, 0, 0);
	}
	// ������ �۾��� �������Ƿ� �������� �ڵ��� ����
	::CloseHandle(p_data->h_thread);
	// �����带 ������ ���������� kill_flag�� ���� ���ŵ��� �ʾ��� ��쿡 ����ؼ� �̺�Ʈ ��ü�� ���� �ٽ� Ȯ����
	// (�ݺ����� ������ �ݺ��� �����ϴ� �߿� �̺�Ʈ ��ü�� ���� �����ߴٸ� ����� �̺�Ʈ ��ü�� ���� Ȯ������
	// ���� ���·� �ݺ����� ����Ǿ� kill_flag�� ���� ���ŵ��� ����)
	if ((!kill_flag) && (::WaitForSingleObject(p_data->h_kill_event, 0) == WAIT_OBJECT_0))
	{
		kill_flag = 1;
		// �����尡 ������ ����ƴٴ� ����� �۾� ���� ǥ�� ���̾�α׿��� �˸�
		::SendMessage(p_data->h_progress_wnd, DESTROY_PROGRESS_DLG, 0, 0);
	}
	// ���� ���̾�α׿��� �����尡 ��������� �˸�
	::PostMessage(p_data->h_main_wnd, POST_FRAME_THREAD_ACTION, kill_flag, (LPARAM)p_data);
	return 0;
}