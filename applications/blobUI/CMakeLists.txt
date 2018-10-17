cmake_minimum_required (VERSION 2.6)
project (blobUI)
cmake_policy (SET CMP0015 NEW)

include_directories (../../libraries/libBlob/include)
link_directories (../../../bld/libraries/libBlob/release)
link_directories (../../../bld/libraries/libBlob/debug)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)

find_package (Qt5Widgets CONFIG REQUIRED)
find_package (Threads REQUIRED)

add_executable (blobUI.app "")
target_sources (blobUI.app
	PRIVATE
		"src/main.m.cpp"
		"src/blobui.cpp"
		"src/field.cpp"
		"src/fieldThreadWorker.cpp"
		"src/redrawThreadWorker.cpp"
		"src/updateThreadWorker.cpp")

target_link_libraries (blobUI.app Qt5::Widgets)
target_link_libraries (blobUI.app ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (blobUI.app optimized libBlob.a debug libBlobd.a)

