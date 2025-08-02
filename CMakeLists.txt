CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(grpc_firewall-manager)

# OPTION (Standard)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs -Wno-attributes -Wreturn-type")

# AVOID conflicts
SET(protobuf_BUILD_TESTS OFF CACHE BOOL "Build protobuf tests" FORCE)
SET(gRPC_BUILD_TESTS OFF CACHE BOOL "Build gRPC tests" FORCE)
SET(RE2_BUILD_TESTING OFF CACHE BOOL "Build RE2 tests" FORCE)

# AVOID conflicts
SET(gRPC_PROTOBUF_PROVIDER "module" CACHE STRING "Provider of protobuf library")
SET(gRPC_ZLIB_PROVIDER "module" CACHE STRING "Provider of zlib library")
SET(gRPC_CARES_PROVIDER "module" CACHE STRING "Provider of c-ares library")
SET(gRPC_SSL_PROVIDER "module" CACHE STRING "Provider of ssl library")
SET(gRPC_RE2_PROVIDER "module" CACHE STRING "Provider of re2 library")
SET(gRPC_ABSL_PROVIDER "module" CACHE STRING "Provider of absl library")

# OPTION (3rdparty)
SET(BUILD_SHARED_LIBS ON)

# ADD 3rdparty
ADD_SUBDIRECTORY(3rdparty/spdlog)
ADD_SUBDIRECTORY(3rdparty/grpc)

# FIND ALL .proto
FILE(GLOB PROTO_FILES "proto/*.proto")

# INITIALIZE generated files
SET(PROTO_SRCS)
SET(PROTO_HDRS)
SET(GRPC_SRCS)
SET(GRPC_HDRS)

# FIND (protoc and grpc_cpp_plugin)
SET(PROTOC_EXECUTABLE $<TARGET_FILE:protoc>)
SET(GRPC_CPP_PLUGIN $<TARGET_FILE:grpc_cpp_plugin>)

# GENERATE C++ files for each proto file
FOREACH(PROTO_FILE ${PROTO_FILES})
    GET_FILENAME_COMPONENT(PROTO_NAME ${PROTO_FILE} NAME_WE)

    # Set output paths
    SET(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    SET(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h")
    SET(GRPC_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
    SET(GRPC_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")

    # Generate protobuf files
    ADD_CUSTOM_COMMAND(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND ${PROTOC_EXECUTABLE}
            ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto
            ${PROTO_FILE}
            DEPENDS ${PROTO_FILE} protoc
            COMMENT "Generating protobuf files for ${PROTO_FILE}"
            VERBATIM
    )

    # Generate gRPC files
    ADD_CUSTOM_COMMAND(
            OUTPUT ${GRPC_SRC} ${GRPC_HDR}
            COMMAND ${PROTOC_EXECUTABLE}
            ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
            --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto
            ${PROTO_FILE}
            DEPENDS ${PROTO_FILE} protoc grpc_cpp_plugin
            COMMENT "Generating gRPC files for ${PROTO_FILE}"
            VERBATIM
    )

    LIST(APPEND PROTO_SRCS ${PROTO_SRC})
    LIST(APPEND PROTO_HDRS ${PROTO_HDR})
    LIST(APPEND GRPC_SRCS ${GRPC_SRC})
    LIST(APPEND GRPC_HDRS ${GRPC_HDR})
ENDFOREACH()

# INCLUDE
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(firewall_server
        src/main_server.cpp
        ${PROTO_SRCS}
        ${GRPC_SRCS}
)

# LINk
TARGET_LINK_LIBRARIES(firewall_server PRIVATE
        libprotobuf
        grpc++
)

ADD_EXECUTABLE(firewall_client
        src/main_client.cpp
        ${PROTO_SRCS}
        ${GRPC_SRCS}
)

# LINk
TARGET_LINK_LIBRARIES(firewall_client PRIVATE
        libprotobuf
        grpc++
)