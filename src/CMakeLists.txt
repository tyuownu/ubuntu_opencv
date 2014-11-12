add_executable(a ransac.cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
find_package(OpenCV REQUIRED)
target_link_libraries(a ${OpenCV_LIBS})
