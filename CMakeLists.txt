cmake_minimum_required(VERSION 3.0.0)
project(Capstone_2D_shooting_game VERSION 0.1.0)

include(CTest)
enable_testing()

# For used in Windows only
# Set the directory to SDL2 library
set(SDL2_DIR "E:/opt/local/i686-w64-mingw32/lib/cmake/SDL2")
# Set compilier flags for Windows system
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,-subsystem,windows")

# Find installed SDL2 library
find_package(SDL2 REQUIRED)
# Add SDL2 include directory to include path
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(shooting2D src/main.cpp src/renderer.cpp)
# Link SDL2 library
target_link_libraries(shooting2D ${SDL2_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
