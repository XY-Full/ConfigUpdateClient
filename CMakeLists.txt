cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

project(ConfigUpdate)
set(CMAKE_CXX_STANDARD 17)

#find_library(PROTOBUF_LIB NAMES libprotobuf PATHS ${PROTOBUF_LIB_DIR})

# 添加头文件搜索路径
include_directories(
    ${CMAKE_SOURCE_DIR}/logic
    ${CMAKE_SOURCE_DIR}/protobuf/include
    ${CMAKE_SOURCE_DIR}/protobuf/proto_files
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/network
)

# 各子模块
add_subdirectory(network)
add_subdirectory(logic)
add_subdirectory(common)
add_subdirectory(protobuf)

# 可执行文件
add_executable(client main.cpp)

target_link_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/protobuf/lib)

set_target_properties(client PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)

target_link_libraries(clientsss
    PRIVATE network
            common
            logic
            proto
)