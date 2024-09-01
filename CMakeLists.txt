cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# ---- Project ----

project(wonky LANGUAGES C)
set(CMAKE_C_STANDARD 11) # Requires C11 standard

if (EMSCRIPTEN)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s FORCE_FILESYSTEM=1 --embed-file ../assets@")
	set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/raylib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/json-c)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/notcurses)
target_include_directories(notcurses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/notcurses/include/)

file(GLOB enet_sources ${CMAKE_CURRENT_SOURCE_DIR}/deps/enet/*.c)
add_library(enet STATIC ${enet_sources})
target_include_directories(enet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/enet/include/)

add_library(luajit STATIC IMPORTED)
target_include_directories(luajit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/deps/luajit/src/)
set_target_properties(luajit PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/luajit/src/libluajit.a)

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

# link to your dependencies' targets here
target_link_libraries(${PROJECT_NAME} json-c)
#target_link_libraries(${PROJECT_NAME} tmx)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} luajit)
target_link_libraries(${PROJECT_NAME} enet)
target_link_libraries(${PROJECT_NAME} notcurses)

# --- copy assets so exe can find the /assets folder ----
set(link_src "${CMAKE_SOURCE_DIR}/assets")
if(UNIX AND NOT APPLE) 
	set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
else() 
	set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets") 
endif() 
add_custom_command(
	TARGET ${PROJECT_NAME} 
	POST_BUILD COMMAND ${CMAKE_COMMAND} 
	-E copy ${link_src} ${link_dst} 
	DEPENDS ${link_dst} 
	COMMENT "copy ${link_src} => ${link_dst}" 
	)
