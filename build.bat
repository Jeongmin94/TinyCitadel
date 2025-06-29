@echo off
setlocal

REM =================================================================
REM 윈도우 빌드 환경 설정
REM =================================================================
REM vcpkg가 설치된 경로를 설정합니다. 사용자의 환경에 맞게 수정하세요.
REM 예: set VCPKG_ROOT=C:\dev\vcpkg
set VCPKG_ROOT=C:\dev\vcpkg

REM 빌드 타입을 설정합니다. (Debug 또는 Release)
set BUILD_TYPE=Debug

REM 실행 파일 이름을 설정합니다. (CMakeLists.txt의 project() 이름)
set EXECUTABLE_NAME=TinyCitadel.exe


REM =================================================================
REM 빌드 스크립트 시작
REM =================================================================
echo.
echo =================================
echo TinyCitadel Windows Build
echo =================================
echo.
echo Build Type: %BUILD_TYPE%
echo vcpkg Path: %VCPKG_ROOT%
echo.

REM 스크립트 파일이 있는 디렉토리로 이동
cd /d "%~dp0"

REM 빌드 디렉토리 생성
if not exist build (
    echo Creating build directory...
    mkdir build
)

REM 빌드 디렉토리로 이동
cd build
if errorlevel 1 (
    echo Failed to change directory to build.
    goto :fail
)

REM CMake 실행하여 Visual Studio 프로젝트 생성
REM vcpkg의 toolchain 파일을 사용하여 라이브러리(glfw3 등)를 자동으로 찾습니다.
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

REM CMake 실행 실패 시 종료
if errorlevel 1 (
    echo CMake configuration failed.
    goto :fail
)

REM 빌드 실행 (MSBuild 또는 cmake --build 사용)
echo.
echo Building the project...
cmake --build . --config %BUILD_TYPE%

REM 빌드 실패 시 종료
if errorlevel 1 (
    echo Build failed.
    goto :fail
)


REM =================================================================
REM 실행
REM =================================================================
echo.
echo Build successful. Running the application...
echo -------------------------------------------------
cd %BUILD_TYPE%
if exist %EXECUTABLE_NAME% (
    %EXECUTABLE_NAME%
) else (
    echo Executable '%EXECUTABLE_NAME%' not found in '%cd%'.
)
echo -------------------------------------------------
goto :end


:fail
echo.
echo !!!!!!! BUILD FAILED !!!!!!!
pause
exit /b 1

:end
echo.
echo Script finished.
pause
endlocal