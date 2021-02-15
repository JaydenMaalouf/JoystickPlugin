@echo off

echo "CMake not may be older than 3.15"
rem if not exist "C:\Program Files (x86)\CMake\" goto INSTALL_CMAKE
echo "Test for CMAKE"
if not exist "C:\Program Files\CMake" (	
	echo "not found cmake.exe"
	goto INSTALL_CMAKE	
) else (	
	SET CMAKE_EXE="C:\Program Files\CMake\bin\cmake.exe"
	echo "found cmake.exe"	
)

echo "Visual Studio 2019"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat x64"

SET SDL_DIR=SDL2_DEV
SET BUILD_DIR=build-SDL2

mkdir %SDL_DIR%\Lib

mkdir %BUILD_DIR%

cd %BUILD_DIR%

%CMAKE_EXE% -G "Visual Studio 16 2019" -A x64 -DSDL_STATIC=ON -DSDL_SHARED=ON -DLIB_C=ON-DFORCE_STATIC_VCRT=ON -DEPIC_EXTENSIONS=OFF --config Release ../%SDL_DIR%

"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" sdl2.sln /t:SDL2 /p:Configuration="Release"
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" sdl2.sln /t:SDL2main /p:Configuration="Release"
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" sdl2.sln /t:SDL2-static /p:Configuration="Release"
rem "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" sdl2.sln /t:INSTALL /p:Configuration="Release"

echo "copy libs/inc"
copy ..\%SDL_DIR%\include\*.* ..\SDL2\include\
copy include\*.* ..\SDL2\include\
copy Release\*.* ..\SDL2\Lib\

echo "copy dlls to plugin binary/win64"
copy Release\*.dll ..\..\..\Binaries\Win64\

echo "copy to Project binary/win64"
copy Release\*.dll ..\..\..\..\..\Binaries\Win64\

cd ..

echo "SUCCESS"
goto ENDE

:INSTALL_MERCURIAL
cls
echo "Please install Mercurial x64"
pause

:INSTALL_CMAKE
cls
echo "Please install cmake > 3.6 x64"
pause

:ENDE
echo "finish."
pause
