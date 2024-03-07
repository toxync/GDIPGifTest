# GDIPGifTest

MFC와 GDI+로 제작된 GIF 이미지 분해/제작 소프트웨어입니다.

## 주요 기능
**여러 개의 자식 비정형 다이얼로그들을 관리하는 사용자 정의 캡션**

* 마우스로 캡션 탭을 클릭해서 다른 캡션 탭으로 이동
![CaptionTabMouseDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/caption_tab_mouse_demo.gif)

* 단축키 조합을 입력해서 다른 캡션 탭으로 이동
![CaptionTabKeyDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/caption_tab_key_demo.gif)

**읽어들인 GIF 이미지의 프레임을 분해**

* 드래그 & 드롭으로 읽어들인 GIF 이미지의 프레임 분해
![LoadGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/load_gif_demo.gif)

**분해된 GIF 이미지 프레임을 이미지로 저장**

* 읽어들인 GIF 이미지의 모든 프레임 저장
![SaveGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/save_gif_demo.gif)

**읽어들인 이미지 파일들을 GIF 이미지로 제작**

* 읽어들인 이미지들로 GIF 이미지 제작
![CreateGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/create_gif_demo.gif)

<details>
  <summary>제작된 GIF 이미지(펼치기/접기)</summary><br>

![CreatedGifResult](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color.gif)

</details>

**8비트 컬러로 변환된 프레임 이미지 미리보기**

* 8비트 컬러로 변환된 프레임 이미지 미리 보기 다이얼로그 이동 & 크기 조절
![ConvertPreviewDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/convert_preview_demo.gif)

**[Octree Quantization](#octree-quantization)을 사용한 RGB 컬러 -> 8비트 컬러 변환 최적화**

## 프로젝트 구조

<details>
  <summary>구조 펼치기/접기</summary><br>

* GDIPGifTestDlg.h
  - 캡션 탭 다이얼로그(자식 비정형 다이얼로그)들을 관리하는 프로그램의 메인 다이얼로그입니다.
* GifDasmDlg.h
  - GIF 이미지 분해 기능을 제공하는 캡션 탭 다이얼로그입니다.
* GifAsmDlg.h
  - GIf 이미지 제작 기능을 제공하는 캡션 탭 다이얼로그입니다.
* GifExtraDlg.h
  - 메인 다이얼로그와의 메시지 통신만을 수행하는 캡션 탭 다이얼로그입니다.
* ProgressDlg.h
  - 작업 스레드의 작업 진행 상태를 확인하는 용도로 사용되는 다이얼로그입니다.
  - GifAsmDlg 다이얼로그에서만 사용됩니다.
* PreviewDlg.h
  - 특정 이미지의 8비트 컬러 변환 결과를 보여주는 용도로 사용되는 다이얼로그입니다.
  - GifAsmDlg 다이얼로그에서만 사용됩니다.
* TXYC_CustomCaption.h
  - 캡션 탭 다이얼로그 관리 기능을 제공하는 클래스입니다.
  - 메인 다이얼로그는 CDialog 클래스와 이 클래스를 다중상속 받습니다.
* TXYC_CaptionControl.h
  - 메인 다이얼로그와 캡션 탭 다이얼로그 간의 통신에 사용할 메시지 번호들입니다.
* TXYC_GDIP.h
  - GDI+ 기능들을 제공하는 클래스입니다.
  - 미리 컴파일된 헤더 안에 포함시킨 상태로 사용합니다.
* TXYC_ListBox.h
  - CListBox 클래스를 상속 받은 사용자 정의 리스트 박스 클래스입니다.
* TXYC_ThumbnailList.h
  - TXYC_ListBox 클래스를 상속 받은 사용자 정의 리스트 박스 클래스입니다.
  - 읽어들인 GIF 이미지 프레임들의 섬네일을 보여주는 용도로 사용됩니다.
  - GifDasmDlg 다이얼로그에서만 사용됩니다.
* TXYC_FrameList.h
  - TXYC_ListBox 클래스를 상속 받은 사용자 정의 리스트 박스 클래스입니다.
  - 읽어들인 이미지들의 섬네일을 보여주는 용도로 사용됩니다.
  - GIF 이미지 제작에 사용할 이미지 데이터를 보관하는 용도로 사용됩니다.
  - GifAsmDlg 다이얼로그에서만 사용됩니다.
* TXYC_FrameThreadData.h
  - 작업 스레드가 사용할 함수들이 선언되어 있습니다.
  - 작업 스레드가 사용할 데이터 구조체가 선언되어 있습니다.
  - 사용할 벡터 자료구조의 자료형이 선언되어 있습니다.
  - GifAsmDlg 다이얼로그와 작업 진행 상태 확인 다이얼로그에서만 사용됩니다.
* TXYC_CustomProgress.h
  - GDI+ 출력을 사용하는 사용자 정의 프로그레스 컨트롤 윈도우입니다.
  - 작업 진행 상태 확인 다이얼로그에서만 사용됩니다.
* TXYC_RectPositionFunc.h
  - 사각형 영역의 종횡비를 유지하면서 다른 사각형 영역의 정중앙에 위치시키는 함수들입니다.
* TXYC_OctreeQuantizer.h
  - [Octree Quantization](#octree-quantization) 기능을 제공하는 클래스입니다.
  - GifAsmDlg 다이얼로그의 작업 스레드에서만 사용됩니다.

</details>

## Octree Quantization

* 노드 한 개가 최대 8개의 자식 노드를 가질 수 있는 Octree 자료구조로 이미지의 핵심 색상들을 추출해내고 8비트 컬러 이미지의 팔레트를 최적화하는 알고리즘입니다.
* 리프 노드의 색상값이 이미지의 핵심 색상값이며, 리프 노드를 리프 노드의 부모 노드에 병합하는 방식으로 리프 노드의 개수를 팔레트 색상 개수에 맞추고 8비트 컬러 이미지의 팔레트를 최적화합니다.

### Octree Quantization의 동작 과정

1. 원본 이미지의 모든 픽셀 색상값을 Octree에 삽입

2. 병합 우선 순위가 높은 순서대로 리프 노드가 아닌 노드들을 리프 노드로 병합해서 Octree의 리프 노드 개수를 256개로 감소시킴

3. 리프 노드에 저장된 색상값들을 8비트 컬러 이미지의 팔레트에 삽입

4. 8비트 컬러 이미지의 픽셀에 저장될 팔레트 인덱스 값을 원본 이미지의 픽셀 색상값으로 결정

<details>
  <summary>원본 이미지의 픽셀 색상값을 Octree에 삽입하는 과정(펼치기/접기)</summary><br>

* 이미지 출처: 자체 제작

![OctreeAddColor00](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_00.PNG)
- - - -
![OctreeAddColor01](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_01.PNG)
- - - -
![OctreeAddColor02](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_02.PNG)
- - - -
![OctreeAddColor03](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_03.PNG)
- - - -
![OctreeAddColor04](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_04.PNG)
- - - -
![OctreeAddColor05](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_05.PNG)
- - - -
![OctreeAddColor06](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_06.PNG)
- - - -
![OctreeAddColor07](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_07.PNG)
- - - -
![OctreeAddColor08](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_08.PNG)
- - - -
![OctreeAddColor09](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_09.PNG)

</details>

<details>
  <summary> Octree의 리프 노드가 아닌 노드들을 병합하는 과정(펼치기/접기)</summary><br>

* 이미지 출처: 자체 제작

![OctreeReduction00](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_00.PNG)
- - - -
![OctreeReduction01](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_01.PNG)
- - - -
![OctreeReduction02](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_02.PNG)
- - - -
![OctreeReduction03](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_03.PNG)
- - - -
![OctreeReduction04](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_04.PNG)
- - - -
![OctreeReduction05](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_05.PNG)
- - - -
![OctreeReduction06](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_06.PNG)

</details>

## 참고 자료

* 사용자 정의 캡션 참고 자료
  - https://github.com/grassator/win32-window-custom-titlebar
  - https://github.com/melak47/BorderlessWindow
* Octree Quantization 참고 자료
  - https://www.codeproject.com/Articles/109133/Octree-Color-Palette
  - https://www.codeguru.com/multimedia/better-gifs-with-octrees/
