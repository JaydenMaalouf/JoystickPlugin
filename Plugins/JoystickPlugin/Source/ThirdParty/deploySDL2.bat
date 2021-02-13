@echo off

echo "copy dlls to plugin binary/win64"
copy SDL2\lib\*.dll ..\..\Binaries\Win64\

echo "copy to Project binary/win64"
copy SDL2\lib\*.dll ..\..\..\..\Binaries\Win64\
