#ifndef _OCTREE_QUANTIZER_
#define _OCTREE_QUANTIZER_

// 클래스 작성자: 박정현(bac6536@naver.com)
// 이 클래스는 다른 개발자가 작성한 클래스를 기반으로 작성되었음
// 원본 클래스 작성자: Sjaak Priester(sjaak@sjaakpriester.nl)
// 원본 클래스 배포 주소: https://www.codeguru.com/multimedia/better-gifs-with-octrees/

#include "TXYC_GDIP.h"

// RGB 색상값에서 R 값만 추출하는 연산
#define R_RGB(rgb) (UINT8)(rgb >> 16)
// RGB 색상값에서 G 값만 추출하는 연산
#define G_RGB(rgb) (UINT8)(rgb >> 8)
// RGB 색상값에서 B 값만 추출하는 연산
#define B_RGB(rgb) (UINT8)rgb
// GIF 이미지의 팔레트 색상 개수
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
	// 팔진 트리의 노드
	class OctreeNode
	{
		// OctreeQuantizer 클래스가 OctreeNode 클래스의 private 멤버들을 자유롭게 사용할 수 있도록
		// friend 클래스로 등록함
		friend class OctreeQuantizer;
		OctreeNode(UINT8 a_depth, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt);
		~OctreeNode();
		// 팔진 트리에 노드를 추가하는 함수
		void SetColor(UINT a_color, OctreeNode* ap_reduce_head, UINT* ap_leaf_cnt);
		// 매개 변수로 주어진 색상값을 통해 자식 노드의 인덱스를 계산해서 반환하는 함수
		UINT8 GetChildIndex(UINT a_color) const;
		// 매개 변수로 주어진 색상값의 팔레트 인덱스를 반환하는 함수
		UINT8 GetPaletteIndex(UINT a_color) const;
		// 병합 예정 노드 리스트의 순서대로 노드들을 병합하면서 팔진 트리의 리프 노드 개수를 감소시키는 함수
		void ReduceOctree(UINT* ap_leaf_cnt);
		// 팔진 트리의 리프 노드 색상값들로 팔레트를 채우는 함수
		void FillPalette(ARGB* ap_palette_entry, UINT8* ap_index);
		// 현재 노드가 비교 대상 노드보다 병합 우선 순위가 높은지(현재 노드가 먼저 병합돼야 하는지) 확인하는 함수
		inline UINT8 ReduceBefore(const OctreeNode* ap_comp_node) const
		{
			// 현재 노드와 비교 대상 노드의 깊이가 동일할 때 현재 노드의 픽셀 개수가 비교 대상 노드의 픽셀 개수보다
			// 적다면 현재 노드의 병합 우선 순위가 높음
			if (m_depth == ap_comp_node->m_depth)
			{
				return m_pixel_cnt < ap_comp_node->m_pixel_cnt;
			}
			// 현재 노드의 깊이가 비교 대상 노드의 깊이보다 깊다면 현재 노드의 병합 우선 순위가 높음
			return m_depth > ap_comp_node->m_depth;
		}
		// 현재 노드의 병합 우선 순위를 갱신하는 함수
		void UpdateReducePriority();
		// 현재 노드를 병합 예정 노드 리스트에서 제거하는 함수
		void RemoveFromReducibles();
		// 병합 예정 노드 리스트의 매개 변수로 주어진 노드 앞 위치에 현재 노드를 추가하는 함수
		void InsertBefore(OctreeNode* ap_next_reducible);
		// 병합 예정 노드 리스트의 매개 변수로 주어진 노드 뒤 위치에 현재 노드를 추가하는 함수
		void InsertAfter(OctreeNode* ap_prev_reducible);
		OctreeNode* mp_child[8]; // 현재 노드의 자식 노드들
		// 병합 예정 노드 리스트 안에서 현재 노드와 연결된 이전 노드와 다음 노드 주소(리프 노드가 아닌 노드에서만 사용됨)
		OctreeNode* mp_next_reducible = NULL;
		OctreeNode* mp_prev_reducible = NULL;
		UINT m_pixel_cnt = 0; // 현재 노드에 소속된 픽셀 개수
		// 현재 노드에 소속된 픽셀들의 R, G, B 값 총합(리프 노드에서만 사용됨)
		UINT m_r_sum = 0;
		UINT m_g_sum = 0;
		UINT m_b_sum = 0;
		UINT8 m_depth = 0; // 현재 노드의 깊이
		UINT8 m_child_cnt = 0; // 현재 노드의 자식 노드 개수
		UINT8 m_palette_idx = 0; // 완성된 팔레트 안에서의 현재 노드 색상값 인덱스(리프 노드에서만 사용됨)
	};
	UINT m_max_color_cnt = GIF_PALETTE_COLORS; // 팔레트의 색상 개수
	static UINT8 m_max_depth; // 팔진 트리의 최대 깊이
public:
	// 매개 변수로 주어진 이미지의 8비트 컬러 변환된 결과 이미지를 반환하는 함수
	GpBitmap* GetQuantizedFrame(GpBitmap* ap_src_bmp, UINT8 a_max_depth);
};

#endif // !_OCTREE_QUANTIZER_