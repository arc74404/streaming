cmake_minimum_required(VERSION 3.12)

set(SERVER_NAME server)
set(CLIENT_NAME client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(streaming)

include(FetchContent)

# stb_image

FetchContent_Declare(
    stb
    URL https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(stb)

# ---------

# boost

find_package(Boost REQUIRED)

# -----

# -------- SERVER PART -------- #

file(GLOB_RECURSE  SERVER_SRC "src/sources_server/*.cpp")

add_executable(${SERVER_NAME})

target_sources(${SERVER_NAME} PRIVATE ${SERVER_SRC})

# -------- ----------- -------- #

# -------- CLIENT PART -------- #
file(GLOB_RECURSE CLIENT_COMMON_SRC "src/sources_client/io/*.cpp" "src/sources_client/main.cpp")

if (LINUX) 
	file(GLOB_RECURSE CLIENT_OS_SRC "src/sources_client/image_data/capture/linux/*.cpp")	
endif()

if (WIN32) 
	file(GLOB_RECURSE CLIENT_OS_SRC "src/sources_client/image_data/capture/windows/*.cpp")	
endif()

add_executable(${CLIENT_NAME})
target_sources(${CLIENT_NAME} PRIVATE ${CLIENT_COMMON_SRC} ${CLIENT_OS_SRC})

if (LINUX)
	find_package(PkgConfig REQUIRED)
    pkg_check_modules(PipeWire REQUIRED IMPORTED_TARGET libpipewire-0.3)
    pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)
    
    target_link_libraries(${CLIENT_NAME} PRIVATE 
        PkgConfig::PipeWire
        ${SYSTEMD_LIBRARIES}
    )
    target_include_directories(${CLIENT_NAME} PRIVATE ${SYSTEMD_INCLUDE_DIRS}) 
endif()

# -------- ----------- -------- #

target_link_libraries(${SERVER_NAME} PRIVATE Boost::headers)
target_link_libraries(${CLIENT_NAME} PRIVATE Boost::headers)