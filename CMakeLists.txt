cmake_minimum_required(VERSION 3.4)

find_package(Threads REQUIRED)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER arm-linux-g++)
set(CMAKE_C_COMPILER arm-linux-gcc)

add_compile_options(
	-std=gnu++11
	-Wall
	-Werror
	-O2
)

set(LIB_NAME adc)
add_library(${LIB_NAME} STATIC src/adc.cpp)
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)