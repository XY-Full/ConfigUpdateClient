
file(GLOB LOGIC_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(logic STATIC
    ${LOGIC_SRC}
)

set_target_properties(logic PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
)

target_include_directories(logic PUBLIC)
target_link_libraries(logic PUBLIC common)