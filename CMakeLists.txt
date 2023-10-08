cmake_minimum_required(VERSION 3.25)
project(sigle_sign C)

set(CMAKE_C_STANDARD 11)
set(INC_DIR /usr/local/Cellar/projectlib/include)
set(LINK_DIR /usr/local/Cellar/projectlib/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ssl crypto pbc gmp curl)

add_executable(sigle_sign main.c Global.c)

add_executable(server server.c)
add_executable(customer customer.c)
add_executable(user user.c Global.c)
add_executable(customer2 customer2.c)
add_executable(W W.c Global.c)
add_executable(IS IS.c Global.c)
add_executable(main2 main2.c Global.c)
add_executable(main3 main3.c Global.c)