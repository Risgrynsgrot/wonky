
emcmake cmake -S . -B ./webbuild -DCMAKE_TOOLCHAIN_FILE=./emsdk/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use
