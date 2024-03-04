cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# ---- Project ----

project(wonky LANGUAGES C)
set(CMAKE_C_STANDARD 11) # Requires C11 standard

# ---- Clang-tidy ----
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
#set(CLANG_TIDY_CMD_LINE "${CLANG_TIDY_EXE}" "-checks=*")
#
macro(target_enable_clang_tidy TARGET)
  set_target_properties(${TARGET} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_CMD_LINE}")
  #set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_CMD_LINE}")
endmacro()

# ---- Fetch CPM ----

set(CPM_DOWNLOAD_VERSION 0.38.6)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}")
  file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

# ---- Add dependencies via CPM ----
# See https://github.com/TheLartians/CPM.cmake for details and examples

CPMAddPackage("gh:raysan5/raylib#5.0")
CPMAddPackage("gh:json-c/json-c#json-c-0.17-20230812")
#CPMAddPackage("gh:baylej/tmx#tmx_1.2.0")

CPMAddPackage(
  NAME enet
  VERSION 1.3.17
  GIT_TAG 4f8e9bdc4ce6d1f61a6274b0e557065a38190952
  GITHUB_REPOSITORY lsalzman/enet
  DOWNLOAD_ONLY True
)

if(enet_ADDED)
  file(GLOB enet_sources ${enet_SOURCE_DIR}/*.c)
  add_library(enet STATIC ${enet_sources})
  target_include_directories(enet PUBLIC ${enet_SOURCE_DIR}/include/)
endif()

# ---- Create executable ----
# add your source files here
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(${PROJECT_NAME} ${headers} ${sources})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_enable_clang_tidy(${PROJECT_NAME})

# link to your dependencies' targets here
target_link_libraries(${PROJECT_NAME} json-c)
#target_link_libraries(${PROJECT_NAME} tmx)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} enet)

# --- Create Tests ---
#file(GLOB_RECURSE testsources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
#remove(sources "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp")
#add_executable(tests ${testsources} ${headers} ${sources})
#set_target_properties(tests PROPERTIES CXX_STANDARD 20)
#target_link_libraries(tests Catch2::Catch2WithMain)
#target_link_libraries(tests raylib)
#target_link_libraries(tests EnTT)
#target_link_libraries(tests enet)
#target_link_libraries(tests bitsery)
#enable_testing()
#add_test(tests tests)

# setup your target's properties
#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

#set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_CLANG_TIDY
#  clang-tidy;
#  -header-filter=.;
#  -checks=*;
#  -warnings-as-errors=*;)

#set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2")
