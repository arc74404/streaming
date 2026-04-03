cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
project(streaming)

file(GLOB_RECURSE SOURCES "sources/*.cpp")

if(NOT WIN32)
    list(FILTER SOURCES EXCLUDE REGEX "sources/image_data/capture/windows/.*")
endif()

if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PipeWire REQUIRED IMPORTED_TARGET libpipewire-0.3)
endif()

if(NOT LINUX)
    list(FILTER SOURCES EXCLUDE REGEX "sources/image_data/capture/linux/.*")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        d3d11.lib 
        dxgi.lib
    )
endif()
if(LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::PipeWire)
endif()
