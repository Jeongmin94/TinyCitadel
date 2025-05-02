#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR" || exit # 스크립트 디렉토리로 이동 실패 시 종료

# 빌드 디렉토리 생성 (이미 있다면 무시됨)
mkdir -p build

# 빌드 디렉토리로 이동
cd build || exit # 이동 실패 시 종료

# CMake 실행 (CMakeLists.txt 변경 감지 시 자동 재구성)
# Release 빌드 타입을 명시적으로 지정할 수 있습니다. (예: 최적화 활성화)
# cmake .. -DCMAKE_BUILD_TYPE=Release
cmake ..

# 빌드 실행 (-j 옵션으로 병렬 빌드 가능, 예: make -j4)
make

# 빌드 성공 시 실행 파일 실행
if [ $? -eq 0 ]; then
    echo "Build successful. Running the application..."
    ./CitadelPhysicsEngine2D # CMakeLists.txt의 project() 이름과 일치해야 함
else
    echo "Build failed."
fi