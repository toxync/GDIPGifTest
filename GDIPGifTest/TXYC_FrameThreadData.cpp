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
	TXYC_OctreeQuantizer oct_qtzr;
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