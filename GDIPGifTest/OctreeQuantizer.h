#ifndef _OCTREE_QUANTIZER_
#define _OCTREE_QUANTIZER_

// Ŭ���� �ۼ���: ������(bac6536@naver.com)
// �� Ŭ������ �ٸ� �����ڰ� �ۼ��� Ŭ������ ������� �ۼ��Ǿ���
// ���� Ŭ���� �ۼ���: Sjaak Priester(sjaak@sjaakpriester.nl)
// ���� Ŭ���� ���� �ּ�: https://www.codeguru.com/multimedia/better-gifs-with-octrees/

#include "TXYC_GDIP.h"

// RGB ���󰪿��� R ���� �����ϴ� ����
#define R_RGB(rgb) (UINT8)(rgb >> 16)
// RGB ���󰪿��� G ���� �����ϴ� ����
#define G_RGB(rgb) (UINT8)(rgb >> 8)
// RGB ���󰪿��� B ���� �����ϴ� ����
#define B_RGB(rgb) (UINT8)rgb
// GIF �̹����� �ȷ�Ʈ ���� ����
#define GIF_PALETTE_COLORS 256

typedef struct _GifPalette
{
	ColorPalette Palette;
	ARGB Entries[GIF_PALETTE_COLORS - 1];
} GifPalette;

class OctreeQuantizer
{
public:
	OctreeQuantizer();
	~OctreeQuantizer();
protected:
	// ���� Ʈ���� ���
	class OctreeNode
	{
		// OctreeQuantizer Ŭ������ OctreeNode Ŭ������ private ������� �����Ӱ� ����� �� �ֵ���
		// friend Ŭ������ �����
		friend class OctreeQuantizer;
		OctreeNode(UINT8 a_depth, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt);
		~OctreeNode();
		// ���� Ʈ���� ��带 �߰��ϴ� �Լ�
		void SetColor(UINT a_color, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt);
		// �Ű� ������ �־��� ������ ���� �ڽ� ����� �ε����� ����ؼ� ��ȯ�ϴ� �Լ�
		UINT8 GetChildIndex(UINT a_color) const;
		// �Ű� ������ �־��� ������ �ȷ�Ʈ �ε����� ��ȯ�ϴ� �Լ�
		UINT8 GetPaletteIndex(UINT a_color) const;
		// ���� ���� ��� ����Ʈ�� ������� ������ �����ϸ鼭 ���� Ʈ���� ���� ��� ������ ���ҽ�Ű�� �Լ�
		void ReduceOctree(UINT* ap_leaf_cnt);
		// ���� Ʈ���� ���� ��� ���󰪵�� �ȷ�Ʈ�� ä��� �Լ�
		void FillPalette(ARGB* ap_palette_entry, UINT8* ap_index);
		// ���� ��尡 �� ��� ��庸�� ���� �켱 ������ ������(���� ��尡 ���� ���յž� �ϴ���) Ȯ���ϴ� �Լ�
		inline UINT8 ReduceBefore(const OctreeNode* ap_comp_node) const
		{
			// ���� ���� �� ��� ����� ���̰� ������ �� ���� ����� �ȼ� ������ �� ��� ����� �ȼ� ��������
			// ���ٸ� ���� ����� ���� �켱 ������ ����
			if (m_depth == ap_comp_node->m_depth)
			{
				return m_pixel_cnt < ap_comp_node->m_pixel_cnt;
			}
			// ���� ����� ���̰� �� ��� ����� ���̺��� ��ٸ� ���� ����� ���� �켱 ������ ����
			return m_depth > ap_comp_node->m_depth;
		}
		// ���� ����� ���� �켱 ������ �����ϴ� �Լ�
		void UpdateReducePriority();
		// ���� ��带 ���� ���� ��� ����Ʈ���� �����ϴ� �Լ�
		void RemoveFromReducibles();
		// ���� ���� ��� ����Ʈ�� �Ű� ������ �־��� ��� �� ��ġ�� ���� ��带 �߰��ϴ� �Լ�
		void InsertBefore(OctreeNode* ap_next_reducible);
		// ���� ���� ��� ����Ʈ�� �Ű� ������ �־��� ��� �� ��ġ�� ���� ��带 �߰��ϴ� �Լ�
		void InsertAfter(OctreeNode* ap_prev_reducible);
		OctreeNode* mp_child[8]; // ���� ����� �ڽ� ����
		// ���� ���� ��� ����Ʈ �ȿ��� ���� ���� ����� ���� ���� ���� ��� �ּ�(���� ��尡 �ƴ� ��忡���� ����)
		OctreeNode* mp_next_reducible = NULL;
		OctreeNode* mp_prev_reducible = NULL;
		UINT m_pixel_cnt = 0; // ���� ��忡 �Ҽӵ� �ȼ� ����
		// ���� ��忡 �Ҽӵ� �ȼ����� R, G, B �� ����(���� ��忡���� ����)
		UINT m_r_sum = 0;
		UINT m_g_sum = 0;
		UINT m_b_sum = 0;
		UINT8 m_depth = 0; // ���� ����� ����
		UINT8 m_child_cnt = 0; // ���� ����� �ڽ� ��� ����
		UINT8 m_palette_idx = 0; // �ϼ��� �ȷ�Ʈ �ȿ����� ���� ��� ���� �ε���(���� ��忡���� ����)
	};
	UINT m_max_color_cnt = GIF_PALETTE_COLORS; // �ȷ�Ʈ�� ���� ����
	static UINT8 m_max_depth; // ���� Ʈ���� �ִ� ����
public:
	// �Ű� ������ �־��� �̹����� 8��Ʈ �÷� ��ȯ�� ��� �̹����� ��ȯ�ϴ� �Լ�
	GpBitmap* GetQuantizedFrame(GpBitmap* ap_src_bmp, UINT8 a_max_depth);
};

#endif // !_OCTREE_QUANTIZER_