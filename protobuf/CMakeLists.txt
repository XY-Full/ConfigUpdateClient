file(GLOB PROTO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/proto_files/*.cc")

add_library(proto ${PROTO_SRCS})

# Protobuf 路径
set(PROTOBUF_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/protobuf/include)
set(PROTOBUF_LIB_DIR     ${CMAKE_SOURCE_DIR}/protobuf/lib)
link_directories(${PROTOBUF_LIB_DIR})
include_directories(${PROTOBUF_INCLUDE_DIR})

target_include_directories(proto
                            PUBLIC 
                            ${CMAKE_SOURCE_DIR}/protobuf/include
                            ${CMAKE_SOURCE_DIR}/protobuf/proto_files
                            )

set_target_properties(proto PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)
                            
target_include_directories(proto PUBLIC libprotobuf)