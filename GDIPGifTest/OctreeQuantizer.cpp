#include "pch.h"
#include "OctreeQuantizer.h"

// ���� Ʈ���� �ִ� ���� �ʱ�ȭ
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
	// ���� ����� ���̰� ���� Ʈ���� �ִ� ���̿� �����ϴٸ� �� ��带 ���� ���� ������
	if (m_depth == m_max_depth)
	{
		++(*ap_leaf_cnt);
	}
	// �� ��尡 ���� ���� �ƴ����� Ư�� ���(��Ʈ ���, ���� ���� ��� ����Ʈ�� ���� ���)�� �ƴ϶��
	// ���� ���� ��� ����Ʈ�� �� �� ��ġ�� �߰��� ���� ���� �켱 ������ ������
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
	++m_pixel_cnt; // ���� ��忡 �Ҽӵ� �ȼ� ������ ������Ŵ
	// ���� ��尡 ���� ��尡 �ƴ� ���
	if (m_depth < m_max_depth)
	{
		// ���� ���� ��� ����Ʈ�� ���� ��� ���� �ٸ� ��尡 �ִٸ� ���� �켱 ������ ������
		if (mp_next_reducible)
		{
			UpdateReducePriority();
		}
		// ���� ����� �������� �ڽ� ����� �ε����� �����
		UINT8 index = GetChildIndex(a_color);
		// ������ �ε����� �ڽ� ��尡 �������� �ʴ´ٸ� �ش� �ε����� �ڽ� ��带 ������
		if (!mp_child[index])
		{
			mp_child[index] = new OctreeNode(m_depth + 1, ap_reduce_head, ap_leaf_cnt);
			++m_child_cnt;
		}
		// ������ �ε����� �ڽ� ��忡 �� �ٸ� �ڽ� ��带 �߰��� �ʿ䰡 �ִ��� Ȯ����
		mp_child[index]->SetColor(a_color, ap_reduce_head, ap_leaf_cnt);
	}
	// ���� ��尡 ���� ����� �� ���� ��忡 �Ҽӵ� �ȼ����� R, G, B �� ������ ������Ŵ
	else
	{
		m_r_sum += R_RGB(a_color);
		m_g_sum += G_RGB(a_color);
		m_b_sum += B_RGB(a_color);
	}
}

UINT8 OctreeQuantizer::OctreeNode::GetChildIndex(UINT a_color) const
{
	// ������ R, G, B ����Ʈ���� ������ (7 - ���� ��� ����)��° ��Ʈ ������ R ���� 2�� ��Ʈ,
	// G ���� 1�� ��Ʈ, B ���� 0�� ��Ʈ�� 0 ~ 7 ������ �ڽ� ��� �ε����� �ǵ��� ������
	UINT8 index = ((1 & (UINT8)(a_color >> (16 + (7 - m_depth)))) << 2);
	index ^= ((1 & (UINT8)(a_color >> (8 + (7 - m_depth)))) << 1);
	index ^= (1 & (UINT8)(a_color >> (7 - m_depth)));
	return index;
}

UINT8 OctreeQuantizer::OctreeNode::GetPaletteIndex(UINT a_color) const
{
	// ���� ��尡 ���� ����� ���� ����� �ȷ�Ʈ �ε��� ���� ��ȯ��
	if (!m_child_cnt)
	{
		return m_palette_idx;
	}
	// ���� ��尡 ���� ��尡 �ƴ϶�� ���� ����� �������� �ڽ� ����� �ε����� ����� ����
	// �ڽ� ��尡 ���� ������� Ȯ����
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
		// ���� ����� R, G, B �� ���տ� ���� ����� R, G, B �� ������ ��ģ ���� ���� ��带 ������
		m_r_sum += mp_child[i]->m_r_sum;
		m_g_sum += mp_child[i]->m_g_sum;
		m_b_sum += mp_child[i]->m_b_sum;
		delete mp_child[i];
		mp_child[i] = NULL;
		--m_child_cnt;
		--(*ap_leaf_cnt);
	}
	// ���� ��忡 �ڽ� ��尡 �ϳ��� ���ٸ� ���� ��带 ���ο� ���� ���� ������
	if (!m_child_cnt)
	{
		++(*ap_leaf_cnt);
	}
}

void OctreeQuantizer::OctreeNode::FillPalette(ARGB* ap_palette_entry, UINT8* ap_index)
{
	// ���� ��尡 ���� ����� ���� ����� ������ �ȷ�Ʈ�� ������
	if (!m_child_cnt)
	{
		UINT8 r_val = 0, g_val = 0, b_val = 0;
		// ���� ��忡 �Ҽӵ� �ȼ����� R, G, B �� ������ ���� ��忡 �Ҽӵ� �ȼ� ������ ������ ����
		// R, G, B �� ����� �ȷ�Ʈ�� ������
		r_val = (UINT8)(m_r_sum / m_pixel_cnt);
		g_val = (UINT8)(m_g_sum / m_pixel_cnt);
		b_val = (UINT8)(m_b_sum / m_pixel_cnt);
		ap_palette_entry[*ap_index] = RGB24(r_val, g_val, b_val);
		// ���� ����� �ȷ�Ʈ �ε��� ���� ������
		m_palette_idx = *ap_index;
		// ���� ������ �ȷ�Ʈ�� ������ �� �ֵ��� �ȷ�Ʈ �ε��� ���� �ϳ� ������Ŵ
		++(*ap_index);
	}
	// ���� ��尡 ���� ��尡 �ƴ϶�� ���� ����� �ڽ� ��尡 ���� ������� Ȯ����
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
	// ���� ���� ��� ����Ʈ �ȿ��� ���� ����� ���� ��ġ�� ��尡 �����ϸ� ���� ��尡 ���� ��ġ ��庸��
	// ���� �켱 ������ ���ٸ� ���� ����� ���� ��ġ�� ���� ����� �� ��ġ�� ������
	while (p_new_next_reducible && !ReduceBefore(p_new_next_reducible))
	{
		p_new_next_reducible = p_new_next_reducible->mp_next_reducible;
	}
	// ���� ����� �� ��ġ�� ���� ����� ���� ��ġ�� �ٸ��ٸ�(���� ����� ���� �켱 ������ �������ٸ�)
	// ���� ��带 ���� ���� ��� ����Ʈ�� ���� ��ġ���� ������ ���� �� ��ġ�� ������
	if (p_new_next_reducible != mp_next_reducible)
	{
		RemoveFromReducibles();
		InsertBefore(p_new_next_reducible);
	}
}
void OctreeQuantizer::OctreeNode::RemoveFromReducibles()
{
	// ���� ��� ���� ��ġ ����� ���� ��� = ���� ����� ���� ���
	mp_prev_reducible->mp_next_reducible = mp_next_reducible;
	// ���� ��� ���� ��ġ ����� ���� ��� = ���� ��� ���� ���
	mp_next_reducible->mp_prev_reducible = mp_prev_reducible;
}
void OctreeQuantizer::OctreeNode::InsertBefore(OctreeNode* ap_next_reducible)
{
	// ���� ����� ���� ��� = �Ű� ������ �Ѱ� ���� ����� ���� ��ġ ���
	mp_prev_reducible = ap_next_reducible->mp_prev_reducible;
	// ���� ����� ���� ��� = �Ű� ������ �Ѱ� ���� ���
	mp_next_reducible = ap_next_reducible;
	// ���� ��� ���� ��ġ ����� ���� ��� = ���� ���
	mp_prev_reducible->mp_next_reducible = this;
	// ���� ��� ���� ��ġ ����� ���� ��� = ���� ���
	mp_next_reducible->mp_prev_reducible = this;
}

void OctreeQuantizer::OctreeNode::InsertAfter(OctreeNode* ap_prev_reducible)
{
	// ���� ��带 �Ű� ������ �Ѱ� ���� ����� ���� ��� ���� ��ġ�� ������
	InsertBefore(ap_prev_reducible->mp_next_reducible);
}

GpBitmap* OctreeQuantizer::GetQuantizedFrame(GpBitmap* ap_src_bmp, UINT8 a_max_depth)
{
	// ���� Ʈ���� �ִ� ���̸� ������
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
	// ���� Ʈ���� ��Ʈ ��� ����
	OctreeNode root(0, NULL, &leaf_cnt);
	// ���� ���� ��� ����Ʈ�� ���� ��� ����(�� ���� �׻� ���� ���� ���� �켱 ������ ������ �ϱ� ������
	// �� ����� ���̴� ���� Ʈ���� �ִ� ���̰� �Ǿ�� �ϴµ�(����Ʈ�� �� �� �ִ� ����� ���� ���� ���̴�
	// (�ִ� ���� - 1)) �� ��带 �ִ� ���̷� �����ϸ� ���� ��尡 �ż� ���� ����� ������ ������ ���ܹ����Ƿ�
	// �� ���� �ִ� ���̺��� �� �ܰ� ���� ���̷� �����ߴٰ� ���߿� �ִ� ���̷� ������)
	OctreeNode reducibles_head(m_max_depth - 1, NULL, &leaf_cnt);
	reducibles_head.m_depth = m_max_depth;
	// ��Ʈ ��带 ���� ���� ��� ����Ʈ�� ������(��Ʈ ���� �׻� ���� ���� ��� ����Ʈ�� �������� ��ġ��)
	reducibles_head.mp_next_reducible = &root;
	root.mp_prev_reducible = &reducibles_head;
	// �Ű� ������ �Ѱ� ���� ���� �̹����� ������ ũ�⸦ ���� 8��Ʈ �÷� �̹����� ������
	GdipGetImageWidth(ap_src_bmp, &cx);
	GdipGetImageHeight(ap_src_bmp, &cy);
	GdipCreateBitmapFromScan0(cx, cy, 0, dest_pixel_fmt, NULL, &p_dest_bmp);
	total_pixel_cnt = cx * cy;
	// ���� �̹����� �ȼ� ũ�⸦ ����(���� �̹����� 24��Ʈ �÷��� ����Ѵٸ� ���� �ȼ��� 3����Ʈ �ڿ� ����
	// �ȼ��� �ְ� ���� �̹����� 32��Ʈ �÷��� ����Ѵٸ� ���� �ȼ��� 4����Ʈ �ڿ� ���� �ȼ��� ����)
	GdipGetImagePixelFormat(ap_src_bmp, &src_pixel_fmt);
	src_pixel_size = GetPixelFormatSize(src_pixel_fmt) >> 3;
	GdipBitmapLockBits(ap_src_bmp, NULL, ImageLockModeRead, src_pixel_fmt, &src_bmp_data);
	GdipBitmapLockBits(p_dest_bmp, NULL, ImageLockModeRead, dest_pixel_fmt, &dest_bmp_data);
	p_src_scan0 = (UINT8*)src_bmp_data.Scan0;
	p_dest_scan0 = (UINT8*)dest_bmp_data.Scan0;
	// ���� �̹����� stride ���� �о��
	if (src_bmp_data.Stride >= 0)
	{
		src_stride = src_bmp_data.Stride;
	}
	// ���� �̹����� stride ���� ������� �̹����� ù �ȼ� ��ġ�� �����ؾ� ��
	else
	{
		p_src_scan0 += (cy * src_bmp_data.Stride);
		src_stride = -src_bmp_data.Stride;
	}
	// ��ȯ�� �̹����� stride ���� �о��
	if (dest_bmp_data.Stride >= 0)
	{
		dest_stride = dest_bmp_data.Stride;
	}
	// ��ȯ�� �̹����� stride ���� ������� �̹����� ù �ȼ� ��ġ�� �����ؾ� ��
	else
	{
		p_dest_scan0 += (cy * dest_bmp_data.Stride);
		dest_stride = -dest_bmp_data.Stride;
	}
	// �̹����� ù �ȼ� ��ġ�� �̹��� �� ���� ��ġ�� ������
	p_src_row = p_src_scan0;
	for (int i = 0; i < total_pixel_cnt; ++i)
	{
		// ���� �ȼ��� �̹��� ���� ù ��° �ȼ��̶�� �̸� �����ص� �̹��� �� ���� ��ġ��
		// ���� �б� ��ġ�� ������
		if (!(i % cx))
		{
			p_src_itr = p_src_row;
		}
		r_val = *(p_src_itr + 2);
		g_val = *(p_src_itr + 1);
		b_val = *p_src_itr;
		// ���� �ȼ��� RGB ������ ���� Ʈ���� ������
		root.SetColor(RGB24(r_val, g_val, b_val), &reducibles_head, &leaf_cnt);
		// ���� �ȼ��� �̵�
		p_src_itr += src_pixel_size;
		// ���� �ȼ��� �̹��� ���� ������ �ȼ��̶�� ���� �̹��� ���� ���󰪵��� ���� �� �ֵ���
		// �̹��� �� ���� ��ġ�� ������
		if ((i % cx) == (cx - 1))
		{
			p_src_row += src_stride;
		}
	}
	// ���� Ʈ���� ���� ��� ������ �ȷ�Ʈ ���� �������� ���ٸ� ��带 �����ؼ� ���� ��� ������ ����
	while (leaf_cnt > m_max_color_cnt)
	{
		// ���� ���� ��� ����Ʈ���� ���� �켱 ������ ���� ���� ��带 ������
		p_cur_reducible = reducibles_head.mp_next_reducible;
		// ���� ��尡 ���� ���� ��� ����Ʈ�� ������ ����� ������ �ߴ���
		if (!p_cur_reducible->mp_next_reducible)
		{
			break;
		}
		// ���� ��带 ���� ���� ��� ����Ʈ���� ������ ���� ���� ����� �ڽ� ������ ���� �����Ͽ�
		// ���� ��带 ���� ���� ����
		p_cur_reducible->RemoveFromReducibles();
		p_cur_reducible->ReduceOctree(&leaf_cnt);
	}
	// ���� ���� ������ ���� �������� �ȷ�Ʈ�� ä������
	gif_palette.Palette.Flags = PaletteFlagsHasAlpha;
	gif_palette.Palette.Count = GIF_PALETTE_COLORS;
	memset(gif_palette.Palette.Entries, 0, sizeof(ARGB) << 8);
	root.FillPalette(gif_palette.Palette.Entries, &index);
	// �ϼ��� �ȷ�Ʈ�� 8��Ʈ �÷� �̹����� �ȷ�Ʈ�� ������
	GdipSetImagePalette(p_dest_bmp, &gif_palette.Palette);
	// �̹����� ù �ȼ� ��ġ�� �̹��� �� ���� ��ġ�� ������
	p_src_row = p_src_scan0;
	p_dest_row = p_dest_scan0;
	for (int i = 0; i < total_pixel_cnt; ++i)
	{
		// ���� �ȼ��� �̹��� ���� ù ��° �ȼ��̶�� �̸� �����ص� �̹��� �� ���� ��ġ��
		// ���� �б� ��ġ�� ������
		if (!(i % cx))
		{
			p_src_itr = p_src_row;
			p_dest_itr = p_dest_row;
		}
		r_val = *(p_src_itr + 2);
		g_val = *(p_src_itr + 1);
		b_val = *p_src_itr;
		// ���� �ȼ��� RGB ���󰪿� �ش��ϴ� �ȷ�Ʈ �ε����� 8��Ʈ �÷� �̹����� �ȼ� �ε����� ������
		*p_dest_itr = root.GetPaletteIndex(RGB24(r_val, g_val, b_val));
		// ���� �ȼ��� �̵�
		p_src_itr += src_pixel_size;
		++p_dest_itr;
		// ���� �ȼ��� �̹��� ���� ������ �ȼ��̶�� ���� �̹��� ���� ���󰪵��� ���� �� �ֵ���
		// �̹��� �� ���� ��ġ�� ������
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