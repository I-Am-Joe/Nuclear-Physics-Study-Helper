cmake_minimum_required(VERSION 3.20)
project(matlab_engine_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(MATLAB_ROOT "C:/Program Files/MATLAB/R2024b")  

add_executable(matlab_engine_demo main.cpp)

target_include_directories(matlab_engine_demo PRIVATE
    "${MATLAB_ROOT}/extern/include"
)

target_link_directories(matlab_engine_demo PRIVATE
    "${MATLAB_ROOT}/extern/lib/win64/microsoft"
)

target_link_libraries(matlab_engine_demo PRIVATE
    MatlabEngine
    MatlabDataArray
)