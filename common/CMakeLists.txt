# 自动收集当前目录所有 .cpp 文件
file(GLOB COMMON_CPP_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

# 生成静态库
add_library(common STATIC
    ${COMMON_CPP_FILES}
)

set_target_properties(common PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)

# 添加头文件搜索路径
target_include_directories(common
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/logic
    ${CMAKE_SOURCE_DIR}/busd
    ${CMAKE_SOURCE_DIR}/protobuf/include
    ${CMAKE_SOURCE_DIR}/protobuf/proto_files
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/third/nlohmann-json
)

target_include_directories(common PUBLIC)