#cmake的最低版本号为3.6
cmake_minimum_required(VERSION 3.6)

#建立工程
project(Vtk)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#为工程添加源程序
add_executable(Vtk main.cpp)

#为工程添加静态链接库
target_link_libraries(Vtk ${VTK_LIBRARIES})