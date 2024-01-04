#ifndef _TXYC_FRAME_THREAD_DATA_H_
#define _TXYC_FRAME_THREAD_DATA_H_

#include "TXYC_OctreeQuantizer.h"
#include "TXYC_RectPositionFunc.h"
#include <vector>

// �۾� �����尡 ���������� ������� �� �����带 ������ ���̾�α׷� ���� �� ����� �޽��� ��ȣ
#define POST_FRAME_THREAD_ACTION 16000
// �۾� ���� ���̾�α׿��� �����带 ������ ���̾�α׷� ������ �������� ����� ���� �� ����� �޽��� ��ȣ
#define ABORT_FRAME_THREAD 17000
// �۾� ���� ���̾�α׿��� �����尡 �۾� �ϳ��� �Ϸ��ߴٴ� �޽����� ���� �� ����� �޽��� ��ȣ
#define SINGLE_WORK_COMPLETE 20000
// �۾� ���� ���̾�α׿��� �����尡 ������ ��������Ƿ� ���̾�α׸� ������� ����� ���� �� ����� �޽��� ��ȣ
#define DESTROY_PROGRESS_DLG 20001
// �۾� �����尡 ����� �ڿ� ������ ������ ������ ����Ʈ �ڽ� ä��� �۾����� ����ϴ� ��
#define POST_TRD_ACT_LOAD_FRAMES 0
// �۾� �����尡 ����� �ڿ� ������ ������ ���� ������ ���� �۾�(������ �̹��� ���� �۾�)���� ����ϴ� ��
#define POST_TRD_ACT_RESET_BASE_FRAME 1
// �۾� �����尡 ����� �ڿ� ������ ������ 8��Ʈ ��ȯ�� ������ �̸� ���� �۾����� ����ϴ� ��
#define POST_TRD_ACT_CONVERT_PREVIEW 2
// �۾� �����尡 ����� �ڿ� ������ ������ GIF �̹��� �������� ����ϴ� ��
#define POST_TRD_ACT_CREATE_GIF 3

using std::vector;

// ������ ������ �� �׸� ������ ������ �̹��� ���� ������
typedef struct _FrameData
{
	// �� �̹��� �����Ͱ� ����Ǿ� �ִ� ������ ����Ʈ �ڽ� �׸��� �ε���
	int index;
	// ���� �̹��� �������� �ּ�, ������ �̹��� �������� �ּ�, 8��Ʈ ��ȯ�� ������ �̹��� �������� �ּ�
	GpBitmap* p_src_bmp, * p_frame_bmp, * p_cvrt_frame_bmp;
} FrameData;

// ������ �̹����� �����ϴ� �۾� �����忡�� ����� ������
typedef struct _FrameThreadData
{
	HANDLE h_thread; // �۾� �������� �ڵ�
	HANDLE h_kill_event; // ������ ����ȭ�� ����� �̺�Ʈ ��ü�� �ڵ�
	HWND h_main_wnd; // ���� ���̾�α��� �ڵ�
	HWND h_progress_wnd; // �۾� ���� ���̾�α��� �ڵ�
	// ������ �̹��� �����͵��� ����Ǿ� �ִ� ������ ������ �ּ�
	vector<FrameData>* p_frame_vector;
	// GIF ���� �������� ���� ����
	UINT frame_cx, frame_cy;
	// �۾� �����尡 ����� �ڿ� ������ ������ ������ �����ϱ� ���� ����
	UINT8 post_thread_action;
} FrameThreadData;

// �۾� ��� �̹������� ������ �̹����� �����ϴ� �����尡 ����ϴ� �Լ�
DWORD WINAPI CreateFrameBitmaps(void* ap_data);
// �۾� ��� ������ �̹������� 8��Ʈ ��ȯ�� �̹����� �����ϴ� �����尡 ����ϴ� �Լ�
DWORD WINAPI ConvertFrameBitmaps(void* ap_data);

#endif // !_TXYC_FRAME_THREAD_DATA_H_