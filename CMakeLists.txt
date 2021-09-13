cmake_minimum_required(VERSION 3.10)

project(sim_eco)

find_package(CUDA REQUIRED)
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

include_directories("core/include")
file(GLOB SOURCES "core/src/*.cpp")

include_directories("cu/include")
file(GLOB CUDA_SOURCES "cuda/src/*")

cuda_add_executable(${PROJECT_NAME} ${SOURCES} ${CUDA_SOURCES})
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX ${OpenCV_LIBS})
