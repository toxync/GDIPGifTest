#ifndef _TXYC_THUMBNAIL_LIST_BOX_H_
#define _TXYC_THUMBNAIL_LIST_BOX_H_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)
// �� Ŭ������ �ٸ� �����ڰ� �ۼ��� Ŭ������ ������� �ۼ��Ǿ���
// ���� Ŭ���� �ۼ���: �輺��(tipsware@naver.com)
// ���� Ŭ���� ���� �ּ�: https://cafe.naver.com/migagong

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
	// WM_CREATE �޽����� ������� �ʰ� ����Ʈ �ڽ��� ������ �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void CreateUserObject();
	// WM_DESTROY �޽����� ������� �ʰ� ����Ʈ �ڽ��� ���ŵ� �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void DeleteUserObject();
	// ����Ʈ �ڽ��� �׸��� ������ �� �׸��� ������ �ִ� �߰� �޸𸮸� �����ϴ� �Լ�
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	// �� �׸� ��µ� ������ �׸��� �Լ�
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data);
	// ����Ʈ �ڽ��� ����� �̹����� �߰��ϴ� �Լ�
	void InsertThumbnailImage(GpImage* ap_image);
	// ����Ʈ �ڽ� �׸� ũ��� �׸� ����� ����� ������ ������ �����ϴ� �Լ�
	void SetListItemPadding(int a_item_padding);
	// ����Ʈ �ڽ� �׸��� ���� �����ϴ� �Լ�
	void SetListItemWidth(int a_item_width);
	// ����Ʈ �ڽ� �׸��� ���̸� �����ϴ� �Լ�
	void SetListItemHeight(int a_item_height);
};

#endif // !_TXYC_THUMBNAIL_LIST_BOX_H_