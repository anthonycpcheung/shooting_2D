cmake_minimum_required(VERSION 3.7.0)
project(Capstone_2D_shooting_game VERSION 0.1.0)

# Set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17")

# Find installed SDL2 libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Add SDL2 include directory to include path
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} src)

add_executable(shooting2D src/main.cpp src/controller.cpp src/renderer.cpp src/game.cpp src/sprite.cpp src/fighter.cpp src/bullet.cpp)
# Link SDL2 library
target_link_libraries(shooting2D ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
