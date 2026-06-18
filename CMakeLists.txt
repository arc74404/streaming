cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(streaming)

# -------- SERVER PART -------- #

file(GLOB SERVER_SRC "src/sources_server/*.cpp")

set(SERVER_NAME server)

add_executable(${SERVER_NAME})

target_sources(${SERVER_NAME} PRIVATE ${SERVER_SRC})

# -------- ----------- -------- #

# add_executable(client)
