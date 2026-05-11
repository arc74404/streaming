set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 23)
project(streaming)

# ====== FetchContent ======
include(FetchContent)

# stb_image_write
FetchContent_Declare(
    stb
    URL https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(stb)

# ====== Сбор исходников ======
file(GLOB_RECURSE SOURCES "sources/*.cpp")

if(NOT WIN32)
    list(FILTER SOURCES EXCLUDE REGEX "sources/image_data/capture/windows/.*")
endif()
if(NOT LINUX)
    list(FILTER SOURCES EXCLUDE REGEX "sources/image_data/capture/linux/.*")
endif()

# ====== Исполняемый файл ======
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем stb (автоматически скачанный)
target_include_directories(${PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR})

# ====== Windows ======
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib dxgi.lib)
endif()

# ====== Linux ======
if(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fsanitize=address -lc++abi")
    
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PipeWire REQUIRED IMPORTED_TARGET libpipewire-0.3)
    pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)
    
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        PkgConfig::PipeWire
        ${SYSTEMD_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${SYSTEMD_INCLUDE_DIRS})
endif()