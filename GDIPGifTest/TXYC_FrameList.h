#ifndef _TXYC_FRAME_LIST_BOX_H_
#define _TXYC_FRAME_LIST_BOX_H_

// 클래스 작성자: 박정현(bac6536@naver.com)

#define THUMB_LIST_ITEM_WIDTH 72
#define THUMB_LIST_ITEM_HEIGHT 48
#define THUMB_LIST_ITEM_PADDING 4

typedef struct ItemFrameImageData
{
	GpImage* p_thumb_image;
	GpBitmap* p_src_bitmap;
	GpBitmap* p_frame_bitmap;
	UINT src_cx, src_cy, frame_cx, frame_cy;
} ItemFrameData;

class TXYC_FrameListBox : public TXYC_ListBox
{
protected:
	TXYC_GDIP m_gdip;
	int m_item_padding = 1;
	int m_item_width = THUMB_LIST_ITEM_WIDTH, m_item_height = THUMB_LIST_ITEM_HEIGHT;
public:
	TXYC_FrameListBox();
	virtual ~TXYC_FrameListBox();
	// WM_CREATE 메시지를 사용하지 않고 리스트 박스가 생성될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void CreateUserObject();
	// WM_DESTROY 메시지를 사용하지 않고 리스트 박스가 제거될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void DeleteUserObject();
	// 각 항목에 출력될 정보를 그리는 함수
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data);
	// 리스트 박스 항목의 추가 메모리를 제거하면서 모든 항목을 제거하는 함수
	void ResetData();
	// 특정 리스트 박스 항목의 추가 메모리를 제거하는 함수
	void RemoveItemData(int a_index);
	// 특정 리스트 박스 항목을 제거하는 함수
	int RemoveItem(int a_index);
	// 리스트 박스에 썸네일 이미지와 원본 이미지, 프레임 이미지를 추가하는 함수
	void InsertItemBitmaps(GpBitmap* ap_src_bmp, GpBitmap* ap_frame_bmp);
	// 리스트 박스 항목의 추가 메모리가 갖고 있는 원본 이미지 주소를 반환하는 함수
	GpBitmap* GetSourceBitmap(int a_index);
	// 리스트 박스 항목의 추가 메모리가 갖고 있는 프레임 이미지 주소를 반환하는 함수
	GpBitmap* GetFrameBitmap(int a_index);
	// 리스트 박스 항목의 추가 메모리가 갖고 있는 원본 이미지를 설정하는 함수
	void SetSourceBitmap(int a_index, GpBitmap* ap_src_bmp);
	// 리스트 박스 항목의 추가 메모리가 갖고 있는 프레임 이미지를 설정하는 함수
	void SetFrameBitmap(int a_index, GpBitmap* ap_frame_bmp);
	// 리스트 박스 항목 크기와 항목에 저장된 썸네일 사이의 여백을 설정하는 함수
	void SetListItemPadding(int a_item_padding);
	// 리스트 박스 항목의 폭을 설정하는 함수
	void SetListItemWidth(int a_item_width);
	// 리스트 박스 항목의 높이를 설정하는 함수
	void SetListItemHeight(int a_item_height);
};

#endif // !_TXYC_FRAME_LIST_BOX_H_