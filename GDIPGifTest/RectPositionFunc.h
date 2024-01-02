#ifndef _RECT_POSITION_FUNCTIONS_H_
#define _RECT_POSITION_FUNCTIONS_H_

// 첫 번째 매개 변수로 주어진 사각형 영역이 폭과 높이의 비율을 유지하며 두 번째 매개 변수로
// 주어진 사각형 영역을 벗어나지 않도록 첫 번째 매개 변수로 주어진 사각형 영역의 크기를
// 조정하면서 첫 번째 매개 변수로 주어진 사각형 영역을 두 번째 매개 변수로 주어진 사각형
// 영역의 내부 정중앙에 위치시키는 함수
void CenterFitRectInRect(RECT* ap_inner_rect, const RECT* ap_outer_rect);
// 첫 번째 매개 변수로 주어진 사각형 영역을 두 번째 매개 변수로 주어진 사각형 영역의 폭을 기준으로
// 크기 조정하는 함수
void ResizeByWidth(RECT* ap_inner_rect, const RECT* ap_outer_rect);
// 첫 번째 매개 변수로 주어진 사각형 영역을 두 번째 매개 변수로 주어진 사각형 영역의 높이를 기준으로
// 크기 조정하는 함수
void ResizeByHeight(RECT* ap_inner_rect, const RECT* ap_outer_rect);

#endif // !_RECT_POSITION_FUNCTIONS_H_