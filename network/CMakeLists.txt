
file(GLOB NETWORK_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Protobuf 路径
set(PROTOBUF_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/protobuf/include)
set(PROTOBUF_LIB_DIR     ${CMAKE_SOURCE_DIR}/protobuf/lib)
link_directories(${PROTOBUF_LIB_DIR})
include_directories(${PROTOBUF_INCLUDE_DIR})

add_library(network STATIC
    ${NETWORK_SRC}
)

set_target_properties(network PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)

target_include_directories(network PUBLIC)
target_link_libraries(network PUBLIC common libprotobuf)