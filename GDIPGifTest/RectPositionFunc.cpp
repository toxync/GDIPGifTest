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
	// (안쪽 사각형 폭) * (바깥쪽 사각형 높이) 값이 (안쪽 사각형 높이) * (바깥쪽 사각형 폭) 값보다 크면
	// 안쪽 사각형을 바깥쪽 사각형의 폭에 맞추어 크기를 조정함
	if (in_w_out_h > in_h_out_w)
	{
		ResizeByWidth(ap_inner_rect, ap_outer_rect);
		inner_height = ap_inner_rect->bottom - ap_inner_rect->top;
		// 안쪽 사각형의 크기 조정이 끝나고 나면 안쪽 사각형이 바깥쪽 사각형의 정중앙에 위치하도록
		// ((안쪽 사각형과 바깥쪽 사각형의 높이 차이) / 2)만큼 안쪽 사각형을 아래로 이동시킴
		padding = (outer_height - inner_height) / 2;
		ap_inner_rect->top += padding;
		ap_inner_rect->bottom += padding;
	}
	// (안쪽 사각형 폭) * (바깥쪽 사각형 높이) 값이 (안쪽 사각형 높이) * (바깥쪽 사각형 폭) 값보다 작으면
	// 안쪽 사각형을 바깥쪽 사각형의 높이에 맞추어 크기를 조정함
	else if (in_w_out_h < in_h_out_w)
	{
		ResizeByHeight(ap_inner_rect, ap_outer_rect);
		inner_width = ap_inner_rect->right - ap_inner_rect->left;
		// 안쪽 사각형의 크기 조정이 끝나고 나면 안쪽 사각형이 바깥쪽 사각형의 정중앙에 위치하도록
		// ((안쪽 사각형과 바깥쪽 사각형의 폭 차이) / 2)만큼 안쪽 사각형을 왼쪽으로 이동시킴
		padding = (outer_width - inner_width) / 2;
		ap_inner_rect->left += padding;
		ap_inner_rect->right += padding;
	}
	// (안쪽 사각형 폭) * (바깥쪽 사각형 높이) 값이 (안쪽 사각형 높이) * (바깥쪽 사각형 폭) 값과 동일하면
	// 안쪽 사각형을 바깥쪽 사각형과 동일한 크기로 조정함
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