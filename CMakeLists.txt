cmake_minimum_required(VERSION 3.5)

project(sim_eco LANGUAGES CUDA CXX)

# Cuda
find_package(CUDA REQUIRED)
CUDA_SELECT_NVCC_ARCH_FLAGS(ARCH_FLAGS "Auto")
set(CUDA_ARCH "${ARCH_FLAGS}" CACHE STRING "Value of the NVCC -arch option." FORCE)
message(STATUS "CUDA_ARCH: \"${CUDA_ARCH}\"")
SET(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} ${CUDA_ARCH}")
#set_property(TARGET CUDA_ARCHITECTURES PROPERTY ${CUDA_ARCH})

# OpenMP
find_package(OpenMP REQUIRED)

# OpenCV
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

# Includes
include_directories(${PROJECT_NAME} "core/include")
include_directories(${PROJECT_NAME} "cuda/include")

# Sources
file(GLOB SOURCE "core/src/*.cpp")
file(GLOB CUDA_FILES "cuda/src/*.cu" "cuda/src/*.cpp")

cuda_add_executable(${PROJECT_NAME} ${SOURCE} ${CUDA_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -O3)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX ${CUDA_LIBRARIES} ${OpenCV_LIBS})
