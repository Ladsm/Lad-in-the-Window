cmake_minimum_required(VERSION 3.10)
project(LITW VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES 
    src/main.cpp
    src/userinput.cpp
    src/Window.cpp
    src/widgets/ShellWidget.cpp
)
if(WIN32)
    list(APPEND SOURCES "windowsRC/LITW.rc")
endif()
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/include/windows
)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/windowsRC")
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
        target_link_libraries(LITW PRIVATE 
        User32.lib
        Gdi32.lib
        Shell32.lib
        Advapi32.lib
    )
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()
