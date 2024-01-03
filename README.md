# GDIPGifTest

MFC와 GDI+로 제작된 GIF 이미지 분해/제작 소프트웨어입니다.

## 프로젝트 구조

* GDIPGifTestDlg.h
  - 캡션 탭 다이얼로그(자식 비정형 다이얼로그)들을 관리하는 프로그램의 메인 다이얼로그입니다.
* TXYC_CustomCaption.h
  - 캡션 탭 다이얼로그 관리 기능을 제공하는 클래스입니다.
  - 메인 다이얼로그는 CDialog 클래스와 이 클래스를 다중상속 받습니다.
* TXYC_CaptionControl.h
  - 메인 다이얼로그와 캡션 탭 다이얼로그 간의 통신에 사용할 메시지 번호들입니다.
* TXYC_GDIP.h
  - GDI+ 기능들을 제공하는 클래스입니다.
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
  - 작업 스레드가 사용할 데이터 구조체가 선언되어 있습니다.
  - 사용할 벡터 자료구조의 자료형이 선언되어 있습니다.
  - GifAsmDlg 다이얼로그와 작업 진행 상태 확인 다이얼로그에서만 사용됩니다.
* TXYC_CustomProgress.h
  - GDI+ 출력을 사용하는 사용자 정의 프로그레스 컨트롤 윈도우입니다.
  - 작업 진행 상태 확인 다이얼로그에서만 사용됩니다.
* RectPositionFunc.h
  - 사각형 영역의 종횡비를 유지하면서 다른 사각형 영역의 정중앙에 위치시키는 함수들입니다.
* OctreeQuantizer.h
  - [Octree Quantization 알고리즘](#octree-quantization-알고리즘) 기능을 제공하는 클래스입니다.
  - GifAsmDlg 다이얼로그에서만 사용됩니다.
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

## 기능 소개

### 캡션 탭 논클라이언트 영역

#### 캡션 탭 논클라이언트 영역의 특징

* 캡션 영역까지 확장된 메인 다이얼로그의 클라이언트 영역 일부 공간을 논클라이언트 영역으로 지정해서 사용합니다.
* 메인 다이얼로그에서 추가한 자식 다이얼로그 개수만큼의 캡션 탭이 생성됩니다.
* 메인 다이얼로그의 크기를 변경하면 캡션 탭 다이얼로그의 크기가 메인 다이얼로그의 크기에 맞게 변경됩니다.
* 캡션 탭 기능을 제외한 나머지 기능들은 아래와 같이 일반적인 윈도우 논클라이언트 영역의 기능과 동일합니다.
  - 논클라이언트 영역을 드래그해서 윈도우 이동
  - 윈도우 테두리를 드래그해서 윈도우 크기 조정
  - 윈도우를 흔들어서 나머지 윈도우들 모두 최소화
  - 최소화/최대화/닫기 버튼 동작
  - Aero Snap(윈도우 키 + 방향 키) 등등...

#### 캡션 탭 다이얼로그의 특징

* 캡션 탭 다이얼로그는 캡션 영역이 없고 WS_CHILD 속성을 가진 메인 다이얼로그의 자식 비정형 다이얼로그입니다.
* 아래와 같은 방법들을 통해 다른 캡션 탭 다이얼로그로 이동할 수 있습니다.
  - 캡션 탭을 마우스로 클릭
  - Ctrl + Tab 또는 Ctrl + Shift + Tab 키 조합의 입력

![CaptionTabMouseDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/caption_tab_mouse_demo.gif)

![CaptionTabKeyDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/caption_tab_key_demo.gif)

### GIF 이미지 분해

![GifDasmDlg](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/GifDasmDlg_img.png)

#### 읽어들인 GIF 이미지 분해

* GIF 이미지를 읽어들이면 프레임 개수만큼의 프레임 섬네일 표시용 리스트 박스 항목이 만들어집니다.
* GIF 이미지를 읽어들이는 방법들은 아래와 같습니다.
  - GIF 이미지를 드래그 & 드롭
  - 'GIF 이미지 찾기' 버튼 클릭

![LoadGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/load_gif_demo.gif)

#### GIF 이미지 재생

* '재생' 버튼을 클릭해서 분해된 GIF 이미지의 프레임들을 재생시키거나 중지시킬 수 있습니다.

![PlayGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/play_gif_demo.gif)

#### GIF 이미지의 프레임 저장

* 프레임 저장 스플릿 버튼을 통해 프레임 저장 모드를 변경할 수 있으며, 사용할 수 있는 프레임 저장 모드는 아래와 같습니다.
  - 현재 선택된 프레임만 PNG 이미지로 저장
  - 모든 프레임들을 PNG 이미지로 저장

![SaveGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/save_gif_demo.gif)

* 분해된 GIF 이미지의 모든 프레임들을 저장하면 분해된 GIF 이미지의 모든 프레임들이 저장된 폴더가 생성됩니다.
  - 생성된 폴더의 이름은 읽어들인 GIF 이미지의 파일 이름과 동일합니다.

![GifSaveAllFrameResult](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/gif_save_all_frame_result.png)

### GIF 이미지 제작

![GifAsmDlg](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/GifAsmDlg_img.png)

#### 이미지 읽기

* 이미지를 읽어들이면 원본 이미지와 원본 이미지 기반의 프레임 이미지가 저장된 리스트 박스 항목이 만들어집니다.
  - 프레임 이미지는 기준 프레임과 동일한 크기로 생성됩니다.
* GIF 이미지 제작에 사용할 이미지를 읽어들이는 방법들은 아래와 같습니다.
  - 이미지 파일을 드래그 & 드롭
  - '프레임 이미지 추가' 버튼 클릭

![LoadImageDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/load_image_demo.gif)

#### 기준 프레임 설정

* 제일 먼저 읽어들인 이미지가 기준 프레임으로 설정됩니다.
* 'GIF 기준 프레임으로 설정' 버튼을 클릭해서 현재 선택된 이미지를 기준 프레임으로 설정할 수 있습니다.
* 기준 프레임과 화면비가 다른 이미지를 읽어들였다면 읽어들인 이미지의 프레임 이미지는 해당 이미지의 화면비를 유지하기 위해 레터 박스가 추가된 상태로 만들어집니다.
* 기준 프레임이 설정될 때마다 기준 프레임 크기 에디트 컨트롤에 설정된 기준 프레임의 크기가 표시됩니다.

![ResetBaseFrameDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/reset_base_frame_demo.gif)

#### 이미지 재생과 프레임 지연 시간 설정

* '재생' 버튼을 클릭해서 GIF 이미지 제작에 사용할 이미지들을 재생시키거나 중지시킬 수 있으며, 현재 설정된 프레임 지연 시간이 재생 지연 시간입니다.
* 프레임 지연 시간 에디트 컨트롤과 연결된 스핀 컨트롤로 프레임 지연 시간을 변경할 수 있으며 스핀 컨트롤의 조작 방법은 아래와 같습니다.
  - 스핀 컨트롤 버튼을 마우스로 클릭
  - 프레임 지연 시간 에디트 컨트롤에 포커스가 있는 상태로 위아래 방향 키를 입력
  - 프레임 지연 시간 에디트 컨트롤에 포커스가 있는 상태로 마우스 휠을 회전
* 프레임 지연 시간이 변경될 때마다 변경된 프레임 지연 시간의 실제 프레임 지연 시간이 자동으로 표시됩니다.

![PlayFrameDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/play_frame_demo.gif)

#### 선택된 프레임의 위치 이동

* '선택된 프레임의 이동 거리' 텍스트 아래에 있는 스핀 컨트롤을 통해 현재 선택된 프레임을 앞 위치나 뒤 위치로 이동시킬 수 있습니다.
* '선택된 프레임의 이동 거리' 텍스트 아래에 있는 스플릿 버튼을 통해 현재 선택된 프레임의 1회 이동 거리를 설정할 수 있습니다.

![ShiftFrameDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/shift_frame_demo.gif)

#### 프레임 삭제

* 프레임 삭제 스플릿 버튼을 통해 읽어들인 이미지와 해당 이미지의 프레임 이미지를 삭제할 수 있으며, 사용할 수 있는 프레임 삭제 모드는 아래와 같습니다.
  - 선택된 프레임 삭제
  - 모든 프레임 삭제
* 남은 프레임 이미지가 없다면 기준 프레임이 없음을 나타내도록 기준 프레임의 폭과 높이가 0으로 설정됩니다.

![RemoveFrameDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/remove_frame_demo.gif)

#### 8비트 컬러로 변환된 프레임 이미지 미리 보기

* 현재 선택된 프레임 이미지의 8비트 컬러 변환 이미지를 미리 보기 다이얼로그에 출력해서 보여줍니다.
* 프레임 이미지를 8비트 컬러 변환 이미지로 만드는 데에 [Octree Quantization 알고리즘](#octree-quantization-알고리즘)이 사용됩니다.
* 미리 보기 다이얼로그는 크기를 자유롭게 조절할 수 있습니다.

![ConvertPreviewDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/convert_preview_demo.gif)

#### GIF 이미지 제작

* 'GIF로 저장' 버튼을 클릭해서 리스트 박스에 있는 모든 프레임 이미지들로 GIF 이미지를 만들 수 있습니다.
* GIF 이미지 제작에 사용할 프레임 이미지의 8비트 컬러 변환 이미지를 만드는 데에 [Octree Quantization 알고리즘](#octree-quantization-알고리즘)이 사용됩니다.

![CreateGifDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/create_gif_demo.gif)

#### 스레드 작업과 스레드 작업 중단

* 스레드를 통해 실행되는 작업들은 아래와 같습니다.
  - 이미지 읽기
  - 기준 프레임 변경
  - 8비트 컬러로 변환된 프레임 미리 보기
  - GIF 이미지 제작
* 스레드가 실행되면 작업 진행 상태를 보여주는 다이얼로그가 만들어지며, 스레드의 작업 한 개가 완료될 때마다 작업 상태 표시 다이얼로그의 프로그레스 바 진행도가 증가합니다.
* 스레드가 실행되는 도중에는 작업표시줄 아이콘 메뉴의 '창 닫기' 명령으로 메인 다이얼로그가 닫히지 않습니다.

![WindowClosePreventionDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/window_close_prevention_demo.gif)

* 작업 상태 표시 다이얼로그의 '작업 중단' 버튼을 클릭하면 스레드가 진행하던 작업을 강제로 종료시킬 수 있습니다.
  - 이미지 읽기 작업은 스레드가 강제로 종료되면 프레임 이미지 생성을 완료한 이미지까지만 읽어들입니다.
  - 이미지 읽기 작업을 제외한 나머지 스레드 작업들은 스레드가 강제로 종료되면 완료된 작업을 마무리하지 않고 모두 버립니다.

![LoadAbortDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/load_abort_demo.gif)

![ResetBaseAbortDemo](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/reset_base_abort_demo.gif)

#### Octree Quantization 알고리즘

* 노드 한 개가 최대 8개의 자식 노드를 가질 수 있는 트리 자료구조인 Octree를 사용해서 이미지의 핵심 색상들을 추출해내는 알고리즘입니다.
> Octree Quantization 알고리즘의 동작 과정
> 1. 원본 이미지의 모든 픽셀 색상값을 Octree에 삽입
> 2. 병합 우선 순위가 높은 순서대로 리프 노드가 아닌 노드들을 리프 노드로 병합해서 Octree의 리프 노드 개수를 256개로 감소시킴
> 3. 리프 노드에 저장된 색상값들을 8비트 컬러 이미지의 팔레트에 삽입
> 4. 8비트 컬러 이미지의 픽셀에 저장될 팔레트 인덱스 값을 원본 이미지의 픽셀 색상값으로 결정
* Octree에 원본 이미지의 픽셀 색상값을 삽입하는 과정을 이미지로 표현하면 아래와 같습니다.(설명에 사용된 이미지들은 모두 자체적으로 제작했습니다.)

![OctreeAddColor00](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_00.PNG)

![OctreeAddColor01](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_01.PNG)

![OctreeAddColor02](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_02.PNG)

![OctreeAddColor03](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_03.PNG)

![OctreeAddColor04](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_04.PNG)

![OctreeAddColor05](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_05.PNG)

![OctreeAddColor06](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_06.PNG)

![OctreeAddColor07](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_07.PNG)

![OctreeAddColor08](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_08.PNG)

![OctreeAddColor09](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_add_color_09.PNG)

* Octree의 리프 노드가 아닌 노드들을 병합하는 과정을 이미지로 표현하면 아래와 같습니다.(설명에 사용된 이미지들은 모두 자체적으로 제작했습니다.)

![OctreeReduction00](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_00.PNG)

![OctreeReduction01](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_01.PNG)

![OctreeReduction02](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_02.PNG)

![OctreeReduction03](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_03.PNG)

![OctreeReduction04](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_04.PNG)

![OctreeReduction05](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_05.PNG)

![OctreeReduction06](https://raw.githubusercontent.com/toxync/GDIPGifTest/master/demo_images/octree_reduction_06.PNG)
