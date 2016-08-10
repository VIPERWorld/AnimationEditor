cmake_minimum_required(VERSION 3.5)
project(AnimationEditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Xml_INCLUDES})

set(SOURCE_FILES
        AnimationEditor/main.cpp
        AnimationEditor/AnimationEditor.cpp
        AnimationEditor/Animation.cpp
        AnimationEditor/Panels/AnimationsPanel.cpp
        AnimationEditor/Panels/AnimationsPanel.h AnimationEditor/Panels/AnimationFramesPanel.cpp AnimationEditor/Panels/AnimationFramesPanel.h AnimationEditor/Panels/InputBoxesPanel.cpp AnimationEditor/Panels/InputBoxesPanel.h AnimationEditor/Panels/AnimationEditorPanel.cpp AnimationEditor/Panels/AnimationEditorPanel.h)

add_executable(AnimationEditor ${SOURCE_FILES})
qt5_use_modules(AnimationEditor Widgets)
qt5_use_modules(AnimationEditor Xml)