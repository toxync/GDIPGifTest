#ifndef _TXYC_LIST_BOX_H_
#define _TXYC_LIST_BOX_H_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)
// �� Ŭ������ �ٸ� �����ڰ� �ۼ��� Ŭ������ ������� �ۼ��Ǿ���
// ���� Ŭ���� �ۼ���: �輺��(tipsware@naver.com)
// ���� Ŭ���� ���� �ּ�: https://cafe.naver.com/mythread

// TXYC_ListBox
class TXYC_ListBox : public CListBox
{
	DECLARE_DYNAMIC(TXYC_ListBox)
protected:
	HBRUSH mh_list_bk_brush; // ����Ʈ �ڽ� �������� ����� �귯�� ��ü
	// ��Ȳ�� �귯�� ������ ����� ������
	COLORREF m_select_brush_color, m_focus_brush_color, m_bk_brush_color;
	// ��Ȳ�� �� ������ ����� ������
	COLORREF m_select_pen_color, m_focus_pen_color, m_bk_pen_color;
	// ���� ó������ Owner-Draw ����ü ����(�Ű� ������ ��� �ѱ��� �ʱ� ���� ����ü �ּҸ�
	// �����ؼ� ���, ���߿� ����ϸ� �ּҰ� ��ȿ���� ���� �� ������ �����ؾ� ��)
	LPDRAWITEMSTRUCT mp_last_lpds;
	CPoint m_last_dbclk_pos; // ����Ʈ �ڽ��� ���콺�� ���� Ŭ������ ���� ��ǥ
public:
	TXYC_ListBox();
	virtual ~TXYC_ListBox();
	// WM_CREATE �޽����� ������� �ʰ� ����Ʈ �ڽ��� ������ �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void CreateUserObject() {}
	// WM_DESTROY �޽����� ������� �ʰ� ����Ʈ �ڽ��� ���ŵ� �� �Բ� �۾��ؾ� �� ������
	// �߰��ϱ� ���� �����Ǵ� �Լ�
	virtual void DeleteUserObject() {}
	// �� �׸��� ����� �׸��� ���� ����� �Լ�(�⺻ �ڵ� ����)
	virtual void BaseDraw(HDC ah_dc, RECT* ap_rect);
	// �� �׸� ��µ� ������ �׸��� �Լ�
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data) {}
	// ����Ʈ �ڽ��� Ư�� �׸� ��ġ�� �����ϴ� �Լ�
	void UpdateItemInfo(int a_index, int a_count);
	// �׸��� ���� Ŭ������ �� ȣ��Ǵ� �Լ�
	virtual void ProcessDoubleClicked(int a_index) {}
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#endif // !_TXYC_LIST_BOX_H_