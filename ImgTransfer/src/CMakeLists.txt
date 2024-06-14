find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(img_transfer STATIC imgProcess.cpp)
target_include_directories(img_transfer PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(img_transfer ${OpenCV_LIBS})

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(ImgTransfer
        main.cpp
        filesInfoGet.cpp
        )
target_link_libraries(ImgTransfer img_transfer)
target_link_libraries(ImgTransfer ${OpenCV_LIBS})
target_include_directories(ImgTransfer PUBLIC ${PROJECT_SOURCE_DIR}/inc)

