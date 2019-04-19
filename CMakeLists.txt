cmake_minimum_required(VERSION 3.14)
project(CS4204Practical2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
add_executable(CS4204Practical2 main.c conc_queue.c Farm.c Stage.c)
#target_link_libraries(CS4204Practical2 threads::Threads)
