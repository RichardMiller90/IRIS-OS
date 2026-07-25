cmake_minimum_required(VERSION 3.21)
project(MobileOS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    WaylandCompositor
    WaylandClient
    OpenGL
)

# Compositor (System UI)
qt_add_executable(mobile-compositor
    compositor/main.cpp
    compositor/compositor.cpp
    compositor/compositor.h
    compositor/appwindow.cpp
    compositor/appwindow.h
    compositor/gesturehandler.cpp
    compositor/gesturehandler.h
    compositor/virtualkeyboard.cpp
    compositor/virtualkeyboard.h
    resources.qrc
)

target_link_libraries(mobile-compositor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::WaylandCompositor
    Qt6::OpenGL
)

target_include_directories(mobile-compositor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# App framework library
qt_add_library(mobileapp SHARED
    shared/mobileapplication.cpp
    shared/mobileapplication.h
)

target_link_libraries(mobileapp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::WaylandClient
)

target_include_directories(mobileapp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
