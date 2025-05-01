cmake_minimum_required(VERSION 3.18)
project(GameOfLife LANGUAGES CXX CUDA)

find_package(CUDA REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_50")

add_executable(GameOfLife main.cpp)

set_target_properties(GameOfLife PROPERTIES 
    CUDA_SEPARABLE_COMPILATION ON
)

target_link_libraries(GameOfLife 
    ${CUDA_LIBRARIES}
    sfml-graphics
    sfml-window
    sfml-system
)

target_include_directories(GameOfLife PRIVATE
    ${CUDA_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
)

install(TARGETS GameOfLife DESTINATION bin)