#!/bin/bash
set -e
RED='\033[0;31m'
GREEN='\033[0;32m'
MAG='\033[0;36m'
NC='\033[0m'

echo -e "${MAG}[INFO]${NC} OPENCV FOR C++ SETUP STARTS...";

sudo apt install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

echo -e "${MAG}[INFO]${NC} DEPENDENCIES INSTALLED ${GREEN}PROPERLY${NC}...";
echo -e "${RED}[WARNING]\n[WARNING]\n[WARNING]${NC} "
echo -e "${MAG}[INFO]${NC} IF YOU DON'T HAVE OPENCV FOR C++,"
echo -e "${MAG}[INFO]${NC} PLEASE FOLLOW THE INSTRUCTIONS ON THE OFFICIAL SITE"
echo -e "${MAG}[INFO]${NC} https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html"
echo -e "${MAG}[INFO]${NC} OR TYPE"
echo -e "${MAG}[INFO]${GREEN} mkdir tmp && cd tmp"
echo -e "${MAG}[INFO]${GREEN} git clone https://github.com/opencv/opencv.git"
echo -e "${MAG}[INFO]${GREEN} cd opencv"
echo -e "${MAG}[INFO]${GREEN} mkdir release && cd release"
echo -e "${MAG}[INFO]${GREEN} cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local .."
echo -e "${MAG}[INFO]${GREEN} make"
echo -e "${MAG}[INFO]${GREEN} sudo make install"
echo -e "${MAG}[INFO]${GREEN} cd ../../../"
echo -e "${MAG}[INFO]${GREEN} rm -rf tmp/"
echo -e "${RED}[WARNING]\n[WARNING]\n[WARNING]${NC} "
