#include "pch.h"
#include "OctreeQuantizer.h"

// 팔진 트리의 최대 깊이 초기화
UINT8 OctreeQuantizer::m_max_depth = 7;

OctreeQuantizer::OctreeQuantizer()
{
}

OctreeQuantizer::~OctreeQuantizer()
{
}

OctreeQuantizer::OctreeNode::OctreeNode(UINT8 a_depth, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt)
	:m_depth(a_depth)
{
	memset(mp_child, 0, sizeof(mp_child));
	// 현재 노드의 깊이가 팔진 트리의 최대 깊이와 동일하다면 이 노드를 리프 노드로 설정함
	if (m_depth == m_max_depth)
	{
		++(*ap_leaf_cnt);
	}
	// 이 노드가 리프 노드는 아니지만 특수 노드(루트 노드, 병합 예정 노드 리스트의 시작 노드)도 아니라면
	// 병합 예정 노드 리스트의 맨 앞 위치에 추가한 다음 병합 우선 순위를 갱신함
	else if (ap_reduce_head)
	{
		InsertAfter(ap_reduce_head);
		UpdateReducePriority();
	}
}

OctreeQuantizer::OctreeNode::~OctreeNode()
{
	for (int i = 0; i < 8; ++i)
	{
		if (!mp_child[i])
		{
			continue;
		}
		delete mp_child[i];
	}
}

void OctreeQuantizer::OctreeNode::SetColor(UINT a_color, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt)
{
	++m_pixel_cnt; // 현재 노드에 소속된 픽셀 개수를 증가시킴
	// 현재 노드가 리프 노드가 아닐 경우
	if (m_depth < m_max_depth)
	{
		// 병합 예정 노드 리스트에 현재 노드 말고도 다른 노드가 있다면 병합 우선 순위를 갱신함
		if (mp_next_reducible)
		{
			UpdateReducePriority();
		}
		// 현재 노드의 색상값으로 자식 노드의 인덱스를 계산함
		UINT8 index = GetChildIndex(a_color);
		// 조합한 인덱스에 자식 노드가 존재하지 않는다면 해당 인덱스에 자식 노드를 생성함
		if (!mp_child[index])
		{
			mp_child[index] = new OctreeNode(m_depth + 1, ap_reduce_head, ap_leaf_cnt);
			++m_child_cnt;
		}
		// 조합한 인덱스의 자식 노드에 또 다른 자식 노드를 추가할 필요가 있는지 확인함
		mp_child[index]->SetColor(a_color, ap_reduce_head, ap_leaf_cnt);
	}
	// 현재 노드가 리프 노드라면 이 리프 노드에 소속된 픽셀들의 R, G, B 값 총합을 증가시킴
	else
	{
		m_r_sum += R_RGB(a_color);
		m_g_sum += G_RGB(a_color);
		m_b_sum += B_RGB(a_color);
	}
}

UINT8 OctreeQuantizer::OctreeNode::GetChildIndex(UINT a_color) const
{
	// 색상값의 R, G, B 바이트에서 추출한 (7 - 현재 노드 깊이)번째 비트 값들을 R 값은 2번 비트,
	// G 값은 1번 비트, B 값은 0번 비트인 0 ~ 7 사이의 자식 노드 인덱스가 되도록 조합함
	UINT8 index = ((1 & (UINT8)(a_color >> (16 + (7 - m_depth)))) << 2);
	index ^= ((1 & (UINT8)(a_color >> (8 + (7 - m_depth)))) << 1);
	index ^= (1 & (UINT8)(a_color >> (7 - m_depth)));
	return index;
}

UINT8 OctreeQuantizer::OctreeNode::GetPaletteIndex(UINT a_color) const
{
	// 현재 노드가 리프 노드라면 현재 노드의 팔레트 인덱스 값을 반환함
	if (!m_child_cnt)
	{
		return m_palette_idx;
	}
	// 현재 노드가 리프 노드가 아니라면 현재 노드의 색상값으로 자식 노드의 인덱스를 계산한 다음
	// 자식 노드가 리프 노드인지 확인함
	else
	{
		UINT8 index = GetChildIndex(a_color);
		return mp_child[index]->GetPaletteIndex(a_color);
	}
}

void OctreeQuantizer::OctreeNode::ReduceOctree(UINT* ap_leaf_cnt)
{
	for (int i = 0; i < 8; ++i)
	{
		if (!mp_child[i])
		{
			continue;
		}
		// 현재 노드의 R, G, B 값 총합에 리프 노드의 R, G, B 값 총합을 합친 다음 리프 노드를 제거함
		m_r_sum += mp_child[i]->m_r_sum;
		m_g_sum += mp_child[i]->m_g_sum;
		m_b_sum += mp_child[i]->m_b_sum;
		delete mp_child[i];
		mp_child[i] = NULL;
		--m_child_cnt;
		--(*ap_leaf_cnt);
	}
	// 현재 노드에 자식 노드가 하나도 없다면 현재 노드를 새로운 리프 노드로 설정함
	if (!m_child_cnt)
	{
		++(*ap_leaf_cnt);
	}
}

void OctreeQuantizer::OctreeNode::FillPalette(ARGB* ap_palette_entry, UINT8* ap_index)
{
	// 현재 노드가 리프 노드라면 현재 노드의 색상값을 팔레트에 저장함
	if (!m_child_cnt)
	{
		UINT8 r_val = 0, g_val = 0, b_val = 0;
		// 현재 노드에 소속된 픽셀들의 R, G, B 값 총합을 현재 노드에 소속된 픽셀 개수로 나눠서 얻은
		// R, G, B 값 평균을 팔레트에 저장함
		r_val = (UINT8)(m_r_sum / m_pixel_cnt);
		g_val = (UINT8)(m_g_sum / m_pixel_cnt);
		b_val = (UINT8)(m_b_sum / m_pixel_cnt);
		ap_palette_entry[*ap_index] = RGB24(r_val, g_val, b_val);
		// 현재 노드의 팔레트 인덱스 값을 설정함
		m_palette_idx = *ap_index;
		// 다음 색상값을 팔레트에 저장할 수 있도록 팔레트 인덱스 값을 하나 증가시킴
		++(*ap_index);
	}
	// 현재 노드가 리프 노드가 아니라면 현재 노드의 자식 노드가 리프 노드인지 확인함
	else
	{
		for (int i = 0; i < 8; ++i)
		{
			if (!mp_child[i])
			{
				continue;
			}
			mp_child[i]->FillPalette(ap_palette_entry, ap_index);
		}
	}
}

void OctreeQuantizer::OctreeNode::UpdateReducePriority()
{
	OctreeNode* p_new_next_reducible = mp_next_reducible;
	// 병합 예정 노드 리스트 안에서 현재 노드의 다음 위치에 노드가 존재하며 현재 노드가 다음 위치 노드보다
	// 병합 우선 순위가 낮다면 현재 노드의 다음 위치를 현재 노드의 새 위치로 설정함
	while (p_new_next_reducible && !ReduceBefore(p_new_next_reducible))
	{
		p_new_next_reducible = p_new_next_reducible->mp_next_reducible;
	}
	// 현재 노드의 새 위치가 현재 노드의 기존 위치와 다르다면(현재 노드의 병합 우선 순위가 낮아졌다면)
	// 현재 노드를 병합 예정 노드 리스트의 기존 위치에서 제거한 다음 새 위치에 삽입함
	if (p_new_next_reducible != mp_next_reducible)
	{
		RemoveFromReducibles();
		InsertBefore(p_new_next_reducible);
	}
}
void OctreeQuantizer::OctreeNode::RemoveFromReducibles()
{
	// 현재 노드 이전 위치 노드의 다음 노드 = 현재 노드의 다음 노드
	mp_prev_reducible->mp_next_reducible = mp_next_reducible;
	// 현재 노드 다음 위치 노드의 이전 노드 = 현재 노드 이전 노드
	mp_next_reducible->mp_prev_reducible = mp_prev_reducible;
}
void OctreeQuantizer::OctreeNode::InsertBefore(OctreeNode* ap_next_reducible)
{
	// 현재 노드의 이전 노드 = 매개 변수로 넘겨 받은 노드의 이전 위치 노드
	mp_prev_reducible = ap_next_reducible->mp_prev_reducible;
	// 현재 노드의 다음 노드 = 매개 변수로 넘겨 받은 노드
	mp_next_reducible = ap_next_reducible;
	// 현재 노드 이전 위치 노드의 다음 노드 = 현재 노드
	mp_prev_reducible->mp_next_reducible = this;
	// 현재 노드 다음 위치 노드의 이전 노드 = 현재 노드
	mp_next_reducible->mp_prev_reducible = this;
}

void OctreeQuantizer::OctreeNode::InsertAfter(OctreeNode* ap_prev_reducible)
{
	// 현재 노드를 매개 변수로 넘겨 받은 노드의 다음 노드 이전 위치에 삽입함
	InsertBefore(ap_prev_reducible->mp_next_reducible);
}

GpBitmap* OctreeQuantizer::GetQuantizedFrame(GpBitmap* ap_src_bmp, UINT8 a_max_depth)
{
	// 팔진 트리의 최대 깊이를 제한함
	m_max_depth = a_max_depth;
	OctreeNode* p_cur_reducible = NULL;
	GpBitmap* p_dest_bmp = NULL;
	BitmapData src_bmp_data, dest_bmp_data;
	PixelFormat src_pixel_fmt = 0, dest_pixel_fmt = PixelFormat8bppIndexed;
	GifPalette gif_palette;
	UINT cx = 0, cy = 0, src_stride = 0, dest_stride = 0, total_pixel_cnt = 0, leaf_cnt = 0;
	UINT8 src_pixel_size = 0, r_val = 0, g_val = 0, b_val = 0, index = 0;
	UINT8* p_src_scan0 = NULL, * p_src_itr = NULL, * p_src_row = NULL;
	UINT8* p_dest_scan0 = NULL, * p_dest_itr = NULL, * p_dest_row = NULL;
	// 팔진 트리의 루트 노드 생성
	OctreeNode root(0, NULL, &leaf_cnt);
	// 병합 예정 노드 리스트의 시작 노드 생성(이 노드는 항상 가장 높은 병합 우선 순위를 가져야 하기 때문에
	// 이 노드의 깊이는 팔진 트리의 최대 깊이가 되어야 하는데(리스트에 들어갈 수 있는 노드의 가장 깊은 깊이는
	// (최대 깊이 - 1)) 이 노드를 최대 깊이로 생성하면 리프 노드가 돼서 리프 노드의 개수에 문제가 생겨버리므로
	// 이 노드는 최대 깊이보다 한 단계 낮은 깊이로 생성했다가 나중에 최대 깊이로 변경함)
	OctreeNode reducibles_head(m_max_depth - 1, NULL, &leaf_cnt);
	reducibles_head.m_depth = m_max_depth;
	// 루트 노드를 병합 예정 노드 리스트에 삽입함(루트 노드는 항상 병합 예정 노드 리스트의 마지막에 위치함)
	reducibles_head.mp_next_reducible = &root;
	root.mp_prev_reducible = &reducibles_head;
	// 매개 변수로 넘겨 받은 원본 이미지와 동일한 크기를 가진 8비트 컬러 이미지를 생성함
	GdipGetImageWidth(ap_src_bmp, &cx);
	GdipGetImageHeight(ap_src_bmp, &cy);
	GdipCreateBitmapFromScan0(cx, cy, 0, dest_pixel_fmt, NULL, &p_dest_bmp);
	total_pixel_cnt = cx * cy;
	// 원본 이미지의 픽셀 크기를 구함(원본 이미지가 24비트 컬러를 사용한다면 현재 픽셀의 3바이트 뒤에 다음
	// 픽셀이 있고 원본 이미지가 32비트 컬러를 사용한다면 현재 픽셀의 4바이트 뒤에 다음 픽셀이 있음)
	GdipGetImagePixelFormat(ap_src_bmp, &src_pixel_fmt);
	src_pixel_size = GetPixelFormatSize(src_pixel_fmt) >> 3;
	GdipBitmapLockBits(ap_src_bmp, NULL, ImageLockModeRead, src_pixel_fmt, &src_bmp_data);
	GdipBitmapLockBits(p_dest_bmp, NULL, ImageLockModeRead, dest_pixel_fmt, &dest_bmp_data);
	p_src_scan0 = (UINT8*)src_bmp_data.Scan0;
	p_dest_scan0 = (UINT8*)dest_bmp_data.Scan0;
	// 원본 이미지의 stride 값을 읽어옴
	if (src_bmp_data.Stride >= 0)
	{
		src_stride = src_bmp_data.Stride;
	}
	// 원본 이미지의 stride 값이 음수라면 이미지의 첫 픽셀 위치를 조정해야 함
	else
	{
		p_src_scan0 += (cy * src_bmp_data.Stride);
		src_stride = -src_bmp_data.Stride;
	}
	// 변환된 이미지의 stride 값을 읽어옴
	if (dest_bmp_data.Stride >= 0)
	{
		dest_stride = dest_bmp_data.Stride;
	}
	// 변환된 이미지의 stride 값이 음수라면 이미지의 첫 픽셀 위치를 조정해야 함
	else
	{
		p_dest_scan0 += (cy * dest_bmp_data.Stride);
		dest_stride = -dest_bmp_data.Stride;
	}
	// 이미지의 첫 픽셀 위치를 이미지 줄 시작 위치로 지정함
	p_src_row = p_src_scan0;
	for (int i = 0; i < total_pixel_cnt; ++i)
	{
		// 현재 픽셀이 이미지 줄의 첫 번째 픽셀이라면 미리 설정해둔 이미지 줄 시작 위치를
		// 색상값 읽기 위치로 지정함
		if (!(i % cx))
		{
			p_src_itr = p_src_row;
		}
		r_val = *(p_src_itr + 2);
		g_val = *(p_src_itr + 1);
		b_val = *p_src_itr;
		// 현재 픽셀의 RGB 색상값을 팔진 트리에 삽입함
		root.SetColor(RGB24(r_val, g_val, b_val), &reducibles_head, &leaf_cnt);
		// 다음 픽셀로 이동
		p_src_itr += src_pixel_size;
		// 현재 픽셀이 이미지 줄의 마지막 픽셀이라면 다음 이미지 줄의 색상값들을 읽을 수 있도록
		// 이미지 줄 시작 위치를 갱신함
		if ((i % cx) == (cx - 1))
		{
			p_src_row += src_stride;
		}
	}
	// 팔진 트리의 리프 노드 개수가 팔레트 색상 개수보다 많다면 노드를 병합해서 리프 노드 개수를 줄임
	while (leaf_cnt > m_max_color_cnt)
	{
		// 병합 예정 노드 리스트에서 병합 우선 순위가 가장 높은 노드를 선택함
		p_cur_reducible = reducibles_head.mp_next_reducible;
		// 현재 노드가 병합 예정 노드 리스트의 마지막 노드라면 병합을 중단함
		if (!p_cur_reducible->mp_next_reducible)
		{
			break;
		}
		// 현재 노드를 병합 예정 노드 리스트에서 제거한 다음 현재 노드의 자식 노드들을 전부 병합하여
		// 현재 노드를 리프 노드로 만듦
		p_cur_reducible->RemoveFromReducibles();
		p_cur_reducible->ReduceOctree(&leaf_cnt);
	}
	// 남은 리프 노드들이 가진 색상값으로 팔레트를 채워넣음
	gif_palette.Palette.Flags = PaletteFlagsHasAlpha;
	gif_palette.Palette.Count = GIF_PALETTE_COLORS;
	memset(gif_palette.Palette.Entries, 0, sizeof(ARGB) << 8);
	root.FillPalette(gif_palette.Palette.Entries, &index);
	// 완성된 팔레트를 8비트 컬러 이미지의 팔레트로 설정함
	GdipSetImagePalette(p_dest_bmp, &gif_palette.Palette);
	// 이미지의 첫 픽셀 위치를 이미지 줄 시작 위치로 지정함
	p_src_row = p_src_scan0;
	p_dest_row = p_dest_scan0;
	for (int i = 0; i < total_pixel_cnt; ++i)
	{
		// 현재 픽셀이 이미지 줄의 첫 번째 픽셀이라면 미리 설정해둔 이미지 줄 시작 위치를
		// 색상값 읽기 위치로 지정함
		if (!(i % cx))
		{
			p_src_itr = p_src_row;
			p_dest_itr = p_dest_row;
		}
		r_val = *(p_src_itr + 2);
		g_val = *(p_src_itr + 1);
		b_val = *p_src_itr;
		// 현재 픽셀의 RGB 색상값에 해당하는 팔레트 인덱스를 8비트 컬러 이미지의 픽셀 인덱스로 설정함
		*p_dest_itr = root.GetPaletteIndex(RGB24(r_val, g_val, b_val));
		// 다음 픽셀로 이동
		p_src_itr += src_pixel_size;
		++p_dest_itr;
		// 현재 픽셀이 이미지 줄의 마지막 픽셀이라면 다음 이미지 줄의 색상값들을 읽을 수 있도록
		// 이미지 줄 시작 위치를 갱신함
		if ((i % cx) == (cx - 1))
		{
			p_src_row += src_stride;
			p_dest_row += dest_stride;
		}
	}
	GdipBitmapUnlockBits(p_dest_bmp, &dest_bmp_data);
	GdipBitmapUnlockBits(ap_src_bmp, &src_bmp_data);
	return p_dest_bmp;
}