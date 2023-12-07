cmake_minimum_required(VERSION 3.26)
project(dex)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.c" "include/*.h")

add_executable(dex ${SRCS})

target_include_directories(dex PUBLIC include)
#if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#    target_compile_definitions(dex PUBLIC DEEX_DEBUG)
#else ()
#    target_compile_definitions(dex PUBLIC DEEX_RELEASE)
#endif ()

IF (WIN32)
    set_target_properties(dex PROPERTIES WIN32_EXECUTABLE TRUE)
ELSE()

ENDIF()


#SDL
#if unix
message("SDL Build start--xdxd-------------------")
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
target_link_libraries(dex ${SDL2_LIBRARIES})
target_include_directories(dex PUBLIC ${SDL2_INCLUDE_DIRS})
message("SDL Build end--xdxd-------------------")

#MSDF-atlas-gen
message("msdf start--xdxd-------------------")
#find_package(Threads)
set(MSDF_ATLAS_USE_VCPKG OFF)
set(MSDF_ATLAS_USE_SKIA OFF)
set(MSDFGEN_USE_SKIA OFF)
set(MSDFGEN_DISABLE_SVG ON)
add_subdirectory(dependencies/msdf-atlas-gen)
target_link_libraries(dex msdf-atlas-gen::msdf-atlas-gen)
target_include_directories(dex PUBLIC dependencies/msdf-atlas-gen)
message("msdf end--xdxd-------------------")



#GLM
target_include_directories(dex PUBLIC dependencies/glm)

#stb
target_include_directories(dex PUBLIC dependencies/stb)


# Add a post-build command to copy dlls to build dir
#add_custom_command(TARGET dex POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${PROJECT_SOURCE_DIR}/${SDL_PATH}/bin/SDL2.dll"
#        "${PROJECT_BINARY_DIR}"
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${PROJECT_SOURCE_DIR}/${SDL_PATH}/bin/SDL2_ttf.dll"
#        "${PROJECT_BINARY_DIR}"
#        COMMENT "Copying dlls to build directory"
#)
