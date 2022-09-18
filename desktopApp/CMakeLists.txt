cmake_minimum_required(VERSION 3.16)

project(BoardStream VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

qt_add_executable(appBoardStream
    src/main.cpp
    src/camerathread.cpp
    src/streamhandler.cpp
    src/whiteboardimageprovider.cpp
    include/camerathread.h
    include/streamhandler.h
    include/whiteboardimageprovider.h
)

target_include_directories(appBoardStream PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(appBoardStream PRIVATE "/opt/homebrew/include")
target_include_directories(appBoardStream PRIVATE "/opt/homebrew/include/opencv4")

target_link_directories(appBoardStream PRIVATE "/opt/homebrew/lib")


qt_add_qml_module(appBoardStream
    URI BoardStream
    VERSION 1.0
    QML_FILES main.qml 
)

set_target_properties(appBoardStream PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER boardstream.mihirpatil.me
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
)

target_link_libraries(appBoardStream
    PRIVATE Qt6::Quick
    PRIVATE opencv_videoio
    PRIVATE opencv_core
    PRIVATE opencv_aruco
    PRIVATE opencv_imgproc
    PRIVATE opencv_imgcodecs
)

install(TARGETS appBoardStream
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
