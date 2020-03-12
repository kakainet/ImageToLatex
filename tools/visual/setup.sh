#!/bin/bash
set -e
RED='\033[0;31m'
GREEN='\033[0;32m'
MAG='\033[0;36m'
NC='\033[0m'

echo -e "${MAG}[INFO]${NC} VISUAL TOOL SETUP STARTS...";

# remove old files
rm -rf build external

# boost
sudo apt install libboost-all-dev

#opencv
bash ./setup_cv.sh

# threadpool
mkdir external
# shellcheck disable=SC2164
cd external
git clone https://github.com/progschj/ThreadPool.git
mv ThreadPool threadpool

# googletest
git clone https://github.com/google/googletest.git
cd ..

echo -e "${MAG}[INFO]${NC} SETUP FINISHED ${GREEN}PROPERLY${NC}...";
echo -e "${MAG}[INFO]${NC} NOW BUILD A PROJECT"
echo -e "${MAG}[INFO]${NC} GO TO /tools/visual/"
echo -e "${MAG}[INFO]${NC} mkdir build && cd build && cmake .. && make"
