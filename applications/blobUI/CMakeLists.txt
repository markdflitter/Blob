cmake_minimum_required (VERSION 2.6)
project (blobUI)

FIND_PACKAGE ( Threads REQUIRED )
INCLUDE_DIRECTORIES (../../libraries/libBlob/include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

add_executable (blobUI.app src/main.m.cpp src/blobui.cpp src/field.cpp src/fieldThreadWorker.cpp src/redrawThreadWorker.cpp src/updateThreadWorker)
target_link_libraries(blobUI.app Qt5::Widgets)
TARGET_LINK_LIBRARIES ( blobUI.app ${CMAKE_THREAD_LIBS_INIT} )


