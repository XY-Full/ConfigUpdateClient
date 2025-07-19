cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=17 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_PREFIX_PATH="./protobuf/lib/cmake" -B build -S .
cmake --build build --config Release
