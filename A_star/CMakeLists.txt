# CMakeList.txt : CMake project for A_star, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (A_star "source/main.cpp" "headers/PathFinder.hpp" "source/Grid.cpp" "source/Node.cpp" "source/PathFinder.cpp")

target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-graphics.lib)
target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-system.lib)
target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-window.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET A_star PROPERTY CXX_STANDARD 20)
  set_property(TARGET A_star PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Add tests and install targets if needed.
