cmake_minimum_required(VERSION 3.15)

# set the project name
project(DDZCJ_3players)

set(SRC_LIST DataStructure.cpp MinMax.cpp MoveGener.cpp Main.cpp)


# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 设置 Release 构建时启用静态链接
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()