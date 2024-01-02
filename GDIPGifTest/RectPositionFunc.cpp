#include "pch.h"
#include "RectPositionFunc.h"

void CenterFitRectInRect(RECT* ap_inner_rect, const RECT* ap_outer_rect)
{
	UINT inner_width = ap_inner_rect->right - ap_inner_rect->left;
	UINT inner_height = ap_inner_rect->bottom - ap_inner_rect->top;
	UINT outer_width = ap_outer_rect->right - ap_outer_rect->left;
	UINT outer_height = ap_outer_rect->bottom - ap_outer_rect->top;
	UINT in_w_out_h = inner_width * outer_height;
	UINT in_h_out_w = inner_height * outer_width;
	UINT padding = 0;
	// (���� �簢�� ��) * (�ٱ��� �簢�� ����) ���� (���� �簢�� ����) * (�ٱ��� �簢�� ��) ������ ũ��
	// ���� �簢���� �ٱ��� �簢���� ���� ���߾� ũ�⸦ ������
	if (in_w_out_h > in_h_out_w)
	{
		ResizeByWidth(ap_inner_rect, ap_outer_rect);
		inner_height = ap_inner_rect->bottom - ap_inner_rect->top;
		// ���� �簢���� ũ�� ������ ������ ���� ���� �簢���� �ٱ��� �簢���� ���߾ӿ� ��ġ�ϵ���
		// ((���� �簢���� �ٱ��� �簢���� ���� ����) / 2)��ŭ ���� �簢���� �Ʒ��� �̵���Ŵ
		padding = (outer_height - inner_height) / 2;
		ap_inner_rect->top += padding;
		ap_inner_rect->bottom += padding;
	}
	// (���� �簢�� ��) * (�ٱ��� �簢�� ����) ���� (���� �簢�� ����) * (�ٱ��� �簢�� ��) ������ ������
	// ���� �簢���� �ٱ��� �簢���� ���̿� ���߾� ũ�⸦ ������
	else if (in_w_out_h < in_h_out_w)
	{
		ResizeByHeight(ap_inner_rect, ap_outer_rect);
		inner_width = ap_inner_rect->right - ap_inner_rect->left;
		// ���� �簢���� ũ�� ������ ������ ���� ���� �簢���� �ٱ��� �簢���� ���߾ӿ� ��ġ�ϵ���
		// ((���� �簢���� �ٱ��� �簢���� �� ����) / 2)��ŭ ���� �簢���� �������� �̵���Ŵ
		padding = (outer_width - inner_width) / 2;
		ap_inner_rect->left += padding;
		ap_inner_rect->right += padding;
	}
	// (���� �簢�� ��) * (�ٱ��� �簢�� ����) ���� (���� �簢�� ����) * (�ٱ��� �簢�� ��) ���� �����ϸ�
	// ���� �簢���� �ٱ��� �簢���� ������ ũ��� ������
	else
	{
		*ap_inner_rect = *ap_outer_rect;
	}
}

void ResizeByWidth(RECT* ap_inner_rect, const RECT* ap_outer_rect)
{
	UINT inner_width = ap_inner_rect->right - ap_inner_rect->left;
	UINT inner_height = ap_inner_rect->bottom - ap_inner_rect->top;
	UINT outer_width = ap_outer_rect->right - ap_outer_rect->left;
	ap_inner_rect->right = ap_outer_rect->right;
	ap_inner_rect->bottom = ap_outer_rect->top + inner_height * outer_width / inner_width;
	if (ap_outer_rect->bottom - ap_inner_rect->bottom == 1)
	{
		ap_inner_rect->bottom = ap_outer_rect->bottom;
	}
}

void ResizeByHeight(RECT* ap_inner_rect, const RECT* ap_outer_rect)
{
	UINT inner_width = ap_inner_rect->right - ap_inner_rect->left;
	UINT inner_height = ap_inner_rect->bottom - ap_inner_rect->top;
	UINT outer_height = ap_outer_rect->bottom - ap_outer_rect->top;
	ap_inner_rect->bottom = ap_outer_rect->bottom;
	ap_inner_rect->right = ap_outer_rect->left + inner_width * outer_height / inner_height;
	if (ap_outer_rect->right - ap_inner_rect->right == 1)
	{
		ap_inner_rect->right = ap_outer_rect->right;
	}
}