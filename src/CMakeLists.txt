add_executable(a ransac.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(a ${OpenCV_LIBS})
