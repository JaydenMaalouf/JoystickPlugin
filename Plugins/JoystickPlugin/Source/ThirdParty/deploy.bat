@echo off

echo "copy dlls to plugin binary/win64"
copy Release\*.dll ..\..\..\Binaries\Win64\

echo "copy to Project binary/win64"
copy Release\*.dll ..\..\..\..\..\Binaries\Win64\
