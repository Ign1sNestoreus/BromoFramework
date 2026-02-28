cmake_minimum_required(VERSION 3.20)
project(bromo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

# Находим компилятор glslc
find_program(GLSLC glslc REQUIRED)

# Пути к шейдерам
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/phong.vert.spv
    COMMAND ${GLSLC} ${SHADER_SRC_DIR}/phong.vert -o ${SHADER_OUTPUT_DIR}/phong.vert.spv
    DEPENDS ${SHADER_SRC_DIR}/phong.vert
)
add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/phong.frag.spv
    COMMAND ${GLSLC} ${SHADER_SRC_DIR}/phong.frag -o ${SHADER_OUTPUT_DIR}/phong.frag.spv
    DEPENDS ${SHADER_SRC_DIR}/phong.frag
)

# Компиляция вершинного шейдера
add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/shader.vert.spv
    COMMAND ${GLSLC} ${SHADER_SRC_DIR}/shader.vert -o ${SHADER_OUTPUT_DIR}/shader.vert.spv
    DEPENDS ${SHADER_SRC_DIR}/shader.vert
    COMMENT "Compiling vertex shader"
)

# Компиляция фрагментного шейдера
add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/shader.frag.spv
    COMMAND ${GLSLC} ${SHADER_SRC_DIR}/shader.frag -o ${SHADER_OUTPUT_DIR}/shader.frag.spv
    DEPENDS ${SHADER_SRC_DIR}/shader.frag
    COMMENT "Compiling fragment shader"
)

# Цели для привязки зависимостей
add_custom_target(compile_shaders DEPENDS
    ${SHADER_OUTPUT_DIR}/shader.vert.spv
    ${SHADER_OUTPUT_DIR}/shader.frag.spv
    ${SHADER_OUTPUT_DIR}/phong.vert.spv
    ${SHADER_OUTPUT_DIR}/phong.frag.spv
)

# Автоматически находим все .cc файлы в src/
file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

# Привязываем цель компиляции шейдеров к исполняемому файлу
add_dependencies(${PROJECT_NAME} compile_shaders)

target_link_libraries(${PROJECT_NAME} glfw glm Vulkan::Vulkan)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_BINARY_DIR}/shaders/")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()
