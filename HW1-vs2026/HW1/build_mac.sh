#!/bin/zsh
# Build and run HW1 on macOS (Homebrew OpenCV). Run from anywhere: ./build_mac.sh
set -e
cd "$(dirname "$0")"

OPENCV="$(brew --prefix opencv)"

clang++ -std=c++17 -Wall -include opencv2/core/utils/logger.hpp HW1.cpp RunLengthCoding.cpp -o HW1_mac \
    -I"$OPENCV/include/opencv5" -L"$OPENCV/lib" \
    -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

./HW1_mac
