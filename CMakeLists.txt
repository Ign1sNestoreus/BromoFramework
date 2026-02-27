cmake_minimum_required(VERSION 3.20)
project(bromo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем режим сборки
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

# Автоматически находим все .cc файлы в src/
file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)

# Включаем слои валидации в Debug-сборке
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG) # можно не использовать
endif()
