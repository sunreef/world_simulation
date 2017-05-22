
cmake_minimum_required(VERSION 3.6)

project(World_Simulation)

if(WIN32)
	set(OpenCV_DIR C:/Users/Victor/Documents/opencv/build)
endif(WIN32)

find_package(OpenCV REQUIRED)

FIND_PACKAGE( OpenMP QUIET)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB Core_Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB Core_Headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

file(GLOB Resources_Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/*.cpp")
file(GLOB Resources_Headers "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/*.h")

file(GLOB Utils_Sources "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp")
file(GLOB Utils_Headers "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.h")

source_group(Core FILES ${Core_Sources} ${Core_Headers})
source_group(Resources FILES ${Resources_Sources} ${Resources_Headers})
source_group(Utils FILES ${Utils_Sources} ${Utils_Headers})

add_executable(World_Simulation ${Core_Sources} ${Core_Headers} ${Resources_Sources} ${Resources_Headers} ${Utils_Sources} ${Utils_Headers})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(World_Simulation ${OpenCV_LIBS})
