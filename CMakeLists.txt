cmake_minimum_required(VERSION 3.11)
project(test VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(eyaml ./src/yaml.cpp)
add_executable(ftxeu ./src/todoTest.cpp)
add_executable(testftx ./src/test.cpp)
target_include_directories(ftxeu PRIVATE $ENV{CPLUS_INCLUDE_PATH})
target_include_directories(testftx PRIVATE $ENV{CPLUS_INCLUDE_PATH} PRIVATE ./include/)
target_include_directories(eyaml PRIVATE $ENV{CPLUS_INCLUDE_PATH} PRIVATE ./include/)
include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()
target_link_libraries(eyaml PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(testftx PUBLIC yaml-cpp::yaml-cpp) # The library or executable that require yaml-cpp library
target_link_libraries(ftxeu 
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-screen.lib"
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-dom.lib"
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-component.lib"
  )
target_link_libraries(testftx PUBLIC yaml-cpp::yaml-cpp) # The library or executable that require yaml-cpp library
target_link_libraries(testftx 
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-screen.lib"
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-dom.lib"
  PRIVATE "$ENV{C_LIBRARY_PATH}\\ftxui-component.lib"
  )

