
cmake_minimum_required(VERSION 3.7)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(World_Simulation)

set(OpenCV_DIR C:/Users/Victor/Documents/opencv/build)
find_package(OpenCV REQUIRED)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB Headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

source_group(Source_Files FILES Sources)
source_group(Header_Files FILES Headers)


add_executable(World_Simulation ${Sources} ${Headers})
target_link_libraries(World_Simulation ${OpenCV_LIBS})
