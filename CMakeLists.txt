cmake_minimum_required(VERSION 3.10)
project(LITW VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_SOURCES
    src/userinput.cpp
    src/Window.cpp
    src/widgets/ShellWidget.cpp
)
add_library(LITW STATIC ${LIB_SOURCES})
add_library(LITW::LITW ALIAS LITW)
target_include_directories(LITW
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if(WIN32)
    target_compile_options(LITW PRIVATE /utf-8)
    target_link_libraries(LITW PUBLIC user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(LITW PUBLIC Threads::Threads)
endif()
add_executable(LITWDemo Demo/MainDemo.cpp)
target_link_libraries(LITWDemo PRIVATE LITW)
include(GNUInstallDirs)
install(TARGETS LITW
    EXPORT LITWTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT LITWTargets
    FILE LITWTargets.cmake
    NAMESPACE LITW::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LITW
)
install(FILES cmake/LITWConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LITW
)