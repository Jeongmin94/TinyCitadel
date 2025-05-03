### 기본 요구 사항
- Windows, Mac 모두 지원 가능해야 함

### 빌드 시스템
- CMake

### 그래픽스 라이브러리
- GLFW: 윈도우/컨텍스트 생성
- GLAD: OpenGL 함수 로더
- OpenGL 4.1 이하(4.2 이상은 맥에서 미지원)

### GUI
- ImGui
```
# 서브모듈 추가 (기본 브랜치)
git submodule add https://github.com/ocornut/imgui.git vendor/imgui

# (선택 사항) docking 브랜치를 사용하고 싶다면
# cd vendor/imgui
# git checkout docking
# cd ../..
# git add vendor/imgui # 변경된 브랜치 정보 커밋

# 서브모듈 초기화 및 업데이트 (다른 사람이 저장소를 클론한 후 실행)
# git clone --recurse-submodules https://github.com/Jeongmin94/CitadelPhysicsEngine2D.git # 처음 클론할 때
# 또는
# git submodule update --init --recursive # 이미 클론한 경우
```