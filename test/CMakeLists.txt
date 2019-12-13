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

include_directories(../include)
set(SOURCE ../src/adc.cpp test.cpp)
add_executable(testbench ${SOURCE})
target_link_libraries(testbench ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS testbench DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)