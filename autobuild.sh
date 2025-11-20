mkdir -p bin
mkdir -p build
cd build
cmake ..
make -j$(nproc)