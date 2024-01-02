#!/bin/bash

#g++ lapsim_lite.cpp -lSDL2 -o lapsim_lite
#g++ lapsim_lite.cpp -lncurses -o lapsim_lite
#export XDG_RUNTIME_DIR=/tmp
mkdir bin_linux
g++ lapsim_lite.cpp -o ./bin_linux/lapsim_lite -std=c++2a -O3
#./lapsim_lite

#################
# Windows build #
#################

mkdir bin_windows
x86_64-w64-mingw32-g++ -Wl,--stack,10000000 lapsim_lite.cpp -o ./bin_windows/lapsim_lite.exe -std=c++2a -O3

# Paths to the found DLLs
#DLL_PATH_1="/System/Volumes/Data/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/lib/libstdc++-6.dll"
#DLL_PATH_2="/System/Volumes/Data/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/lib/libgcc_s_seh-1.dll"
#/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-i686/i686-w64-mingw32/bin/libwinpthread-1.dll
#/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/bin/libwinpthread-1.dll
DLL_PATH_1="/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/lib/libstdc++-6.dll"
DLL_PATH_2="/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/lib/libgcc_s_seh-1.dll"
DLL_PATH_3="/opt/homebrew/Cellar/mingw-w64/11.0.1/toolchain-x86_64/x86_64-w64-mingw32/bin/libwinpthread-1.dll"

if [ -f "$DLL_PATH_1" ] && [ -f "$DLL_PATH_2" ] && [ -f "$DLL_PATH_3" ]; then
  cp "$DLL_PATH_1" ./bin_windows
  cp "$DLL_PATH_2" ./bin_windows
  cp "$DLL_PATH_3" ./bin_windows
else
  echo "DLLs not found. Please provide correct paths."
fi
