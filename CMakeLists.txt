cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -ggdb -fopenmp -std=c++17")

list(APPEND CMAKE_PREFIX_PATH "/home/cll/zoe/opencv-4.x")
find_package(OpenCV REQUIRED)

include_directories(/home/cll/zoe/opencv-4.x/include/opencv4)
include_directories(./dependencies/indicators/include)
include_directories(./dependencies/tinyxml2)
include_directories(./dependencies/tinyobjloader)
include_directories(./src)

set(SRC_FILE
    dependencies/tinyxml2/tinyxml2.cpp
    src/common/utils.cpp
    src/common/AABB.cpp
    src/common/BVH.cpp
    src/common/Light.cpp
    src/common/Camera.cpp
    src/objects/ModelLoader.cpp
    src/objects/Material.cpp
    src/objects/Object.cpp
    src/objects/Sphere.cpp
    src/objects/Triangle.cpp
    src/Scene.cpp
    src/Renderer.cpp
)

add_library(rayTracing SHARED ${SRC_FILE})

# add_executable(testUtils tests/basic/testUtils.cpp)
# add_dependencies(testUtils rayTracing)
# target_link_libraries(testUtils ${OpenCV_LIBS} rayTracing)

# add_executable(testBunny tests/testBunny.cpp)
# add_dependencies(testBunny rayTracing)
# target_link_libraries(testBunny ${OpenCV_LIBS} rayTracing)

# add_executable(testRender tests/testRender.cpp)
# add_dependencies(testRender rayTracing)
# target_link_libraries(testRender ${OpenCV_LIBS} rayTracing)

# add_executable(testBVHRender tests/testBVHRender.cpp)
# add_dependencies(testBVHRender rayTracing)
# target_link_libraries(testBVHRender ${OpenCV_LIBS} rayTracing)

add_executable(testCornellBox tests/testCornellBox.cpp)
add_dependencies(testCornellBox rayTracing)
target_link_libraries(testCornellBox ${OpenCV_LIBS} rayTracing)

add_executable(testDebug tests/testDebug.cpp)
add_dependencies(testDebug rayTracing)
target_link_libraries(testDebug ${OpenCV_LIBS} rayTracing)

add_executable(testBVHCornellBox tests/testBVHCornellBox.cpp)
add_dependencies(testBVHCornellBox rayTracing)
target_link_libraries(testBVHCornellBox ${OpenCV_LIBS} rayTracing)

add_executable(testCornell tests/scenes/testCornell.cpp)
add_dependencies(testCornell rayTracing)
target_link_libraries(testCornell ${OpenCV_LIBS} rayTracing)

add_executable(testStairscase tests/scenes/testStairscase.cpp)
add_dependencies(testStairscase rayTracing)
target_link_libraries(testStairscase ${OpenCV_LIBS} rayTracing)

add_executable(testVeach tests/scenes/testVeach.cpp)
add_dependencies(testVeach rayTracing)
target_link_libraries(testVeach ${OpenCV_LIBS} rayTracing)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)