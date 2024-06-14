cmake_minimum_required(VERSION 3.6.0)

project("MyNDKApp")
set(OpenCV_STATIC ON)
set(OpenCV_DIR E:/opencv4_6/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)
find_library( log-lib log )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
add_library( native-lib SHARED main.cpp img_transfer/imgProcess.cpp)

target_link_libraries( native-lib ${log-lib} ${OpenCV_LIBS})



