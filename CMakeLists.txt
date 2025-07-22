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

# 设置exe图标
if (WIN32)
    set(WIN_RESOURCES icon/icon.rc)
endif()

# 可执行文件
add_executable(client main.cpp ${WIN_RESOURCES})

# 设置可执行文件名称
set_target_properties(client PROPERTIES OUTPUT_NAME "client")

# 编译完后执行拷贝命令
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:client>"
        "${CMAKE_SOURCE_DIR}/更新服务端配置.exe"
)

target_link_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/protobuf/lib)

set_target_properties(client PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)

target_link_libraries(client
    PRIVATE network
            common
            logic
            proto
)