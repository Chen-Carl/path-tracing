cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++17")

list(APPEND CMAKE_PREFIX_PATH "/home/cll/zoe/opencv-4.x")
find_package(OpenCV REQUIRED)

include_directories(/home/cll/zoe/opencv-4.x/include/opencv4)
include_directories(./src)

set(SRC_FILE
    src/common/utils.cpp
    src/common/AABB.cpp
    src/common/BVH.cpp
    src/objects/Object.cpp
    src/objects/Sphere.cpp
    src/objects/Triangle.cpp
    src/objects/MeshTriangles.cpp
    src/Light.cpp
    src/Scene.cpp
    src/Renderer.cpp
)

add_library(rayTracing SHARED ${SRC_FILE})

add_executable(testUtils tests/testUtils.cpp)
add_dependencies(testUtils rayTracing)
target_link_libraries(testUtils ${OpenCV_LIBS} rayTracing)

add_executable(testObject tests/testObject.cpp)
add_dependencies(testObject rayTracing)
target_link_libraries(testObject ${OpenCV_LIBS} rayTracing)

add_executable(testBVH tests/testBVH.cpp)
add_dependencies(testBVH rayTracing)
target_link_libraries(testBVH ${OpenCV_LIBS} rayTracing)

add_executable(testBunny tests/testBunny.cpp)
add_dependencies(testBunny rayTracing)
target_link_libraries(testBunny ${OpenCV_LIBS} rayTracing)

add_executable(testTrace tests/testTrace.cpp)
add_dependencies(testTrace rayTracing)
target_link_libraries(testTrace ${OpenCV_LIBS} rayTracing)

add_executable(testCastRay tests/testCastRay.cpp)
add_dependencies(testCastRay rayTracing)
target_link_libraries(testCastRay ${OpenCV_LIBS} rayTracing)

add_executable(testRender tests/testRender.cpp)
add_dependencies(testRender rayTracing)
target_link_libraries(testRender ${OpenCV_LIBS} rayTracing)

add_executable(testBVHRender tests/testBVHRender.cpp)
add_dependencies(testBVHRender rayTracing)
target_link_libraries(testBVHRender ${OpenCV_LIBS} rayTracing)

add_executable(testTriangle tests/testTriangle.cpp)
add_dependencies(testTriangle rayTracing)
target_link_libraries(testTriangle ${OpenCV_LIBS} rayTracing)

add_executable(testAABB tests/testAABB.cpp)
add_dependencies(testAABB rayTracing)
target_link_libraries(testAABB ${OpenCV_LIBS} rayTracing)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)