#!/bin/bash

echo "=====> Installing required packages for Protobuf..."
sudo apt update
sudo apt install -y build-essential
sudo apt install -y cmake
sudo apt install -y git
sudo apt install -y autoconf automake libtool curl make g++ unzip
sudo apt install -y pkg-config

echo "=====> Building Protobuf..."
PROTOBUF_DIR="$(pwd)/3rdparty/protobuf"

if [ ! -d "$PROTOBUF_DIR" ]; then
  echo "Error: Protobuf source directory not found at $PROTOBUF_DIR"
  exit 1
fi

cd "$PROTOBUF_DIR"

echo "=====> Checking Protobuf source structure..."
ls -la ./

# Check if CMakeLists.txt exists in root
if [ ! -f "CMakeLists.txt" ]; then
  echo "Error: CMakeLists.txt not found in protobuf root directory"
  exit 1
fi

echo "=====> Found CMakeLists.txt in root directory"

# Clean any previous build
if [ -d "build" ]; then
  echo "=====> Cleaning previous build directory..."
  rm -rf build
fi

mkdir build
cd build

echo "=====> Configuring Protobuf build with CMake..."
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -Dprotobuf_BUILD_TESTS=OFF \
  -Dprotobuf_BUILD_SHARED_LIBS=ON \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

# Check if cmake configuration was successful
if [ $? -ne 0 ]; then
  echo "Error: CMake configuration failed"
  exit 1
fi

echo "=====> Compiling Protobuf with CMake..."
cmake --build . --config Release --parallel $(nproc)

echo "=====> Installing Protobuf with CMake..."
sudo cmake --build . --target install

echo "=====> Updating library cache..."
sudo ldconfig

echo "=====> Verifying Protobuf installation..."
protoc --version

if [ $? -eq 0 ]; then
  echo "Protobuf installation and verification successful!"
fi

echo "- Protobuf installed to: /usr/local"
echo "- Headers location: /usr/local/include/google/protobuf"
echo "- Libraries location: /usr/local/lib"

echo "=====> Installation complete!"