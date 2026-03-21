cmake_minimum_required(VERSION 3.10)
project(LITW VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_SOURCES
    src/userinput.cpp
    src/Window.cpp
    src/widgets/ShellWidget.cpp
)
add_library(${PROJECT_NAME}_lib STATIC ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_executable(LITW "Demo/main.cpp")
target_link_libraries(LITW PRIVATE ${PROJECT_NAME}_lib)
if(WIN32)
    target_compile_options(${PROJECT_NAME}_lib PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC User32.lib Gdi32.lib Shell32.lib Advapi32.lib)
    set_target_properties(LITW PROPERTIES WIN32_EXECUTABLE FALSE)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC Threads::Threads)
endif()
set_target_properties(LITW PROPERTIES OUTPUT_NAME "LITW")
