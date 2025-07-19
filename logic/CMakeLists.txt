
file(GLOB LOGIC_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(logic STATIC
    ${LOGIC_SRC}
)

target_include_directories(logic PUBLIC)
target_link_libraries(logic PUBLIC pthread common)