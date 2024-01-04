#ifndef _TXYC_FRAME_LIST_BOX_H_
#define _TXYC_FRAME_LIST_BOX_H_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)

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
	// WM_CREATE �޽����� ������� �ʰ� ����Ʈ �ڽ��� ������ �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void CreateUserObject();
	// WM_DESTROY �޽����� ������� �ʰ� ����Ʈ �ڽ��� ���ŵ� �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void DeleteUserObject();
	// �� �׸� ��µ� ������ �׸��� �Լ�
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data);
	// ����Ʈ �ڽ� �׸��� �߰� �޸𸮸� �����ϸ鼭 ��� �׸��� �����ϴ� �Լ�
	void ResetData();
	// Ư�� ����Ʈ �ڽ� �׸��� �߰� �޸𸮸� �����ϴ� �Լ�
	void RemoveItemData(int a_index);
	// Ư�� ����Ʈ �ڽ� �׸��� �����ϴ� �Լ�
	int RemoveItem(int a_index);
	// ����Ʈ �ڽ��� ����� �̹����� ���� �̹���, ������ �̹����� �߰��ϴ� �Լ�
	void InsertItemBitmaps(GpBitmap* ap_src_bmp, GpBitmap* ap_frame_bmp);
	// ����Ʈ �ڽ� �׸��� �߰� �޸𸮰� ���� �ִ� ���� �̹��� �ּҸ� ��ȯ�ϴ� �Լ�
	GpBitmap* GetSourceBitmap(int a_index);
	// ����Ʈ �ڽ� �׸��� �߰� �޸𸮰� ���� �ִ� ������ �̹��� �ּҸ� ��ȯ�ϴ� �Լ�
	GpBitmap* GetFrameBitmap(int a_index);
	// ����Ʈ �ڽ� �׸��� �߰� �޸𸮰� ���� �ִ� ���� �̹����� �����ϴ� �Լ�
	void SetSourceBitmap(int a_index, GpBitmap* ap_src_bmp);
	// ����Ʈ �ڽ� �׸��� �߰� �޸𸮰� ���� �ִ� ������ �̹����� �����ϴ� �Լ�
	void SetFrameBitmap(int a_index, GpBitmap* ap_frame_bmp);
	// ����Ʈ �ڽ� �׸� ũ��� �׸� ����� ����� ������ ������ �����ϴ� �Լ�
	void SetListItemPadding(int a_item_padding);
	// ����Ʈ �ڽ� �׸��� ���� �����ϴ� �Լ�
	void SetListItemWidth(int a_item_width);
	// ����Ʈ �ڽ� �׸��� ���̸� �����ϴ� �Լ�
	void SetListItemHeight(int a_item_height);
};

#endif // !_TXYC_FRAME_LIST_BOX_H_