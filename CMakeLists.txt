cmake_minimum_required(VERSION 3.10)
project(LITW VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_SOURCES 
    src/userinput.cpp 
    src/Window.cpp 
    src/widgets/ShellWidget.cpp 
)
add_library(LITW_lib STATIC ${LIB_SOURCES})
set_target_properties(LITW_lib PROPERTIES OUTPUT_NAME "LITW")
target_include_directories(LITW_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(LITWDemo "Demo/MainDemo.cpp")
target_link_libraries(LITWDemo PRIVATE LITW_lib)
if(WIN32)
    target_compile_options(LITW_lib PRIVATE /utf-8)
    target_link_libraries(LITW_lib PUBLIC User32.lib Gdi32.lib Shell32.lib Advapi32.lib)
    set_target_properties(LITWDemo PROPERTIES WIN32_EXECUTABLE FALSE)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(LITW_lib PUBLIC Threads::Threads)
endif()