@echo off
cd build
cmake --build .
set PATH=F:\Qt\6.10.2\mingw_64\bin;%PATH%
start sharesphere_qt.exe

