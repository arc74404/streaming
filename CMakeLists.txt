cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
project(streaming)

file(GLOB_RECURSE SOURCES "sources/*.cpp")

if(NOT WIN32)
    list(FILTER SOURCES EXCLUDE REGEX "sources/image_data/capture/windows/.*")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        d3d11.lib 
        dxgi.lib
    )
endif()