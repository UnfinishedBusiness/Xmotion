
cmake_minimum_required(VERSION 2.8)

PROJECT(Visualizer)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(Visualizer MACOSX_BUNDLE Visualizer)

if(VTK_LIBRARIES)
  target_link_libraries(Visualizer ${VTK_LIBRARIES})
else()
  target_link_libraries(Visualizer vtkHybrid vtkWidgets)
endif()
