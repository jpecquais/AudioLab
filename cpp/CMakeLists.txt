cmake_minimum_required(VERSION 3.14)

project(AUDIOLAB VERSION 0.0.1)

add_library(AudioLab STATIC
  Filters/filterdesign.cpp
  Filters/onePoleSwitcher.cpp
)


set_target_properties(AudioLab PROPERTIES PUBLIC
  CXX_STANDARD 17 
  CXX_STANDARD_REQUIRED
  )


target_include_directories(AudioLab PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Source
  PRIVATE
  "Filters"
  )
