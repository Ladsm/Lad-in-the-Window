cmake_minimum_required(VERSION 3.10)
project(LITW VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES 
    src/main.cpp
    src/userinput.cpp
    src/Window.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/include/windows
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()
