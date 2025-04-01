cmake_minimum_required(VERSION 3.20)
project(sleep_soundly LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SRC_CPP "src/*.cpp")
file(GLOB SRC_RC "src/*.rc")

add_executable(sleep_soundly WIN32 ${SRC_CPP} ${SRC_RC})
target_link_libraries(sleep_soundly PRIVATE winmm)
if(SRC_RC)
	enable_language(RC)
endif()
