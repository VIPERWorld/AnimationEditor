cmake_minimum_required(VERSION 3.5)
project(AnimationEditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})

set(SOURCE_FILES
        src/main.cpp
        src/AEPDocumentService.cpp
        src/AnimationEditor.cpp
        src/JSONDocumentService.cpp
        src/XMLDocumentService.cpp)
add_executable(AnimationEditor ${SOURCE_FILES})
qt5_use_modules(AnimationEditor Widgets)