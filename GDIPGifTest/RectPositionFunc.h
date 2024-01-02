#ifndef _RECT_POSITION_FUNCTIONS_H_
#define _RECT_POSITION_FUNCTIONS_H_

// ù ��° �Ű� ������ �־��� �簢�� ������ ���� ������ ������ �����ϸ� �� ��° �Ű� ������
// �־��� �簢�� ������ ����� �ʵ��� ù ��° �Ű� ������ �־��� �簢�� ������ ũ�⸦
// �����ϸ鼭 ù ��° �Ű� ������ �־��� �簢�� ������ �� ��° �Ű� ������ �־��� �簢��
// ������ ���� ���߾ӿ� ��ġ��Ű�� �Լ�
void CenterFitRectInRect(RECT* ap_inner_rect, const RECT* ap_outer_rect);
// ù ��° �Ű� ������ �־��� �簢�� ������ �� ��° �Ű� ������ �־��� �簢�� ������ ���� ��������
// ũ�� �����ϴ� �Լ�
void ResizeByWidth(RECT* ap_inner_rect, const RECT* ap_outer_rect);
// ù ��° �Ű� ������ �־��� �簢�� ������ �� ��° �Ű� ������ �־��� �簢�� ������ ���̸� ��������
// ũ�� �����ϴ� �Լ�
void ResizeByHeight(RECT* ap_inner_rect, const RECT* ap_outer_rect);

#endif // !_RECT_POSITION_FUNCTIONS_H_