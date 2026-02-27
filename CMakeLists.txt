cmake_minimum_required(VERSION 3.20)
project(bromo)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc src/window.cc)

target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)
