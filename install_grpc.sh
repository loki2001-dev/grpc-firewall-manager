#!/bin/bash

echo "=====> Installing required packages for gRPC..."
sudo apt update
sudo apt install -y build-essential
sudo apt install -y autoconf automake libtool curl make g++ unzip
sudo apt install -y pkg-config
sudo apt install -y libssl-dev
sudo apt install -y zlib1g-dev
sudo apt install -y libsystemd-dev

echo "=====> Building gRPC..."
GRPC_DIR="$(pwd)/3rdparty/grpc"

if [ ! -d "$GRPC_DIR" ]; then
  echo "Error: gRPC source directory not found at $GRPC_DIR"
  exit 1
fi

cd "$GRPC_DIR"

echo "=====> Checking gRPC source structure..."
ls -la ./

# Check if CMakeLists.txt exists in root
if [ ! -f "CMakeLists.txt" ]; then
  echo "Error: CMakeLists.txt not found in grpc root directory"
  exit 1
fi

echo "=====> Found CMakeLists.txt in root directory"

# Initialize and update submodules
echo "=====> Initializing git submodules..."
git submodule update --init --recursive

rm -rf build
mkdir -p build
cd build

echo "=====> Configuring gRPC build with CMake..."
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DgRPC_INSTALL=ON \
  -DgRPC_BUILD_TESTS=OFF \
  -DgRPC_ABSL_PROVIDER=module \
  -DgRPC_CARES_PROVIDER=module \
  -DgRPC_PROTOBUF_PROVIDER=module \
  -DgRPC_RE2_PROVIDER=module \
  -DgRPC_SSL_PROVIDER=package \
  -DgRPC_ZLIB_PROVIDER=package \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

echo "=====> Compiling gRPC with CMake..."
cmake --build . --config Release --parallel $(nproc)

echo "=====> Installing gRPC with CMake..."
sudo cmake --build . --target install

echo "=====> Updating library cache..."
sudo ldconfig

echo "=====> Verifying gRPC installation..."
# Check if grpc_cpp_plugin exists and is executable
if command -v grpc_cpp_plugin >/dev/null 2>&1; then
  echo "✓ grpc_cpp_plugin found: $(which grpc_cpp_plugin)"
fi

# Check if gRPC headers are installed
if [ -d "/usr/local/include/grpc" ] || [ -d "/usr/local/include/grpcpp" ]; then
  echo "✓ gRPC headers found in /usr/local/include/"
fi

# Check if gRPC libraries are installed
if ls /usr/local/lib/libgrpc*.so* >/dev/null 2>&1 || ls /usr/local/lib/libgrpc*.a >/dev/null 2>&1; then
  echo "✓ gRPC libraries found in /usr/local/lib/"
  echo "  Installed libraries:"
  ls -la /usr/local/lib/libgrpc* 2>/dev/null | head -5
fi

# Overall verification
if command -v grpc_cpp_plugin >/dev/null 2>&1 && [ -d "/usr/local/include/grpcpp" ]; then
  echo "gRPC installation and verification successful!"
fi

echo "- gRPC installed to: /usr/local"
echo "- Headers location: /usr/local/include/grpc*"
echo "- Libraries location: /usr/local/lib"
echo "- Tools location: /usr/local/bin (grpc_cpp_plugin, etc.)"
echo "=====> Installation complete!"