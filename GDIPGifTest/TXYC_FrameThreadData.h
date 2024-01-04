#ifndef _TXYC_FRAME_THREAD_DATA_H_
#define _TXYC_FRAME_THREAD_DATA_H_

#include "TXYC_OctreeQuantizer.h"
#include "TXYC_RectPositionFunc.h"
#include <vector>

// 작업 스레드가 정상적으로 종료됐을 때 스레드를 생성한 다이얼로그로 보낼 때 사용할 메시지 번호
#define POST_FRAME_THREAD_ACTION 16000
// 작업 상태 다이얼로그에서 스레드를 생성한 다이얼로그로 스레드 강제종료 명령을 보낼 때 사용할 메시지 번호
#define ABORT_FRAME_THREAD 17000
// 작업 상태 다이얼로그에게 스레드가 작업 하나를 완료했다는 메시지를 보낼 때 사용할 메시지 번호
#define SINGLE_WORK_COMPLETE 20000
// 작업 상태 다이얼로그에게 스레드가 강제로 종료됐으므로 다이얼로그를 닫으라는 명령을 보낼 때 사용할 메시지 번호
#define DESTROY_PROGRESS_DLG 20001
// 작업 스레드가 종료된 뒤에 수행할 동작이 프레임 리스트 박스 채우기 작업임을 명시하는 값
#define POST_TRD_ACT_LOAD_FRAMES 0
// 작업 스레드가 종료된 뒤에 수행할 동작이 기준 프레임 변경 작업(프레임 이미지 변경 작업)임을 명시하는 값
#define POST_TRD_ACT_RESET_BASE_FRAME 1
// 작업 스레드가 종료된 뒤에 수행할 동작이 8비트 변환된 프레임 미리 보기 작업임을 명시하는 값
#define POST_TRD_ACT_CONVERT_PREVIEW 2
// 작업 스레드가 종료된 뒤에 수행할 동작이 GIF 이미지 제작임을 명시하는 값
#define POST_TRD_ACT_CREATE_GIF 3

using std::vector;

// 프레임 벡터의 한 항목에 저장할 프레임 이미지 관련 데이터
typedef struct _FrameData
{
	// 이 이미지 데이터가 저장되어 있던 프레임 리스트 박스 항목의 인덱스
	int index;
	// 원본 이미지 데이터의 주소, 프레임 이미지 데이터의 주소, 8비트 변환된 프레임 이미지 데이터의 주소
	GpBitmap* p_src_bmp, * p_frame_bmp, * p_cvrt_frame_bmp;
} FrameData;

// 프레임 이미지를 생성하는 작업 스레드에서 사용할 데이터
typedef struct _FrameThreadData
{
	HANDLE h_thread; // 작업 스레드의 핸들
	HANDLE h_kill_event; // 스레드 동기화에 사용할 이벤트 객체의 핸들
	HWND h_main_wnd; // 메인 다이얼로그의 핸들
	HWND h_progress_wnd; // 작업 상태 다이얼로그의 핸들
	// 프레임 이미지 데이터들이 저장되어 있는 프레임 벡터의 주소
	vector<FrameData>* p_frame_vector;
	// GIF 기준 프레임의 폭과 높이
	UINT frame_cx, frame_cy;
	// 작업 스레드가 종료된 뒤에 수행할 동작의 종류를 구분하기 위한 변수
	UINT8 post_thread_action;
} FrameThreadData;

// 작업 대상 이미지들의 프레임 이미지를 생성하는 스레드가 사용하는 함수
DWORD WINAPI CreateFrameBitmaps(void* ap_data);
// 작업 대상 프레임 이미지들의 8비트 변환된 이미지를 생성하는 스레드가 사용하는 함수
DWORD WINAPI ConvertFrameBitmaps(void* ap_data);

#endif // !_TXYC_FRAME_THREAD_DATA_H_