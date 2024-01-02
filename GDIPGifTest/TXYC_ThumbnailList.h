#ifndef _TXYC_THUMBNAIL_LIST_BOX_H_
#define _TXYC_THUMBNAIL_LIST_BOX_H_

// 클래스 작성자: 박정현(bac6536@naver.com)
// 이 클래스는 다른 개발자가 작성한 클래스를 기반으로 작성되었음
// 원본 클래스 작성자: 김성엽(tipsware@naver.com)
// 원본 클래스 배포 주소: https://cafe.naver.com/migagong

#include "TXYC_GDIP.h"

#define THUMB_LIST_ITEM_WIDTH 72
#define THUMB_LIST_ITEM_HEIGHT 48
#define THUMB_LIST_ITEM_PADDING 4

typedef struct ItemThumbnailImageData
{
	GpImage* p_image;
} ItemThumbData;

class TXYC_ThumbnailListBox : public TXYC_ListBox
{
protected:
	TXYC_GDIP m_gdip;
	int m_item_padding = 1;
	int m_item_width = THUMB_LIST_ITEM_WIDTH, m_item_height = THUMB_LIST_ITEM_HEIGHT;
public:
	TXYC_ThumbnailListBox();
	virtual ~TXYC_ThumbnailListBox();
	// WM_CREATE 메시지를 사용하지 않고 리스트 박스가 생성될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void CreateUserObject();
	// WM_DESTROY 메시지를 사용하지 않고 리스트 박스가 제거될 때 함께 작업해야 할 내용을
	// 추가하기 위해 제공되는 함수
	virtual void DeleteUserObject();
	// 리스트 박스의 항목이 삭제될 때 항목이 가지고 있던 추가 메모리를 해제하는 함수
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	// 각 항목에 출력될 정보를 그리는 함수
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data);
	// 리스트 박스에 썸네일 이미지를 추가하는 함수
	void InsertThumbnailImage(GpImage* ap_image);
	// 리스트 박스 항목 크기와 항목에 저장된 썸네일 사이의 여백을 설정하는 함수
	void SetListItemPadding(int a_item_padding);
	// 리스트 박스 항목의 폭을 설정하는 함수
	void SetListItemWidth(int a_item_width);
	// 리스트 박스 항목의 높이를 설정하는 함수
	void SetListItemHeight(int a_item_height);
};

#endif // !_TXYC_THUMBNAIL_LIST_BOX_H_