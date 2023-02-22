add_executable (A_star "source/main.cpp" "headers/PathFinder.hpp" "source/Grid.cpp" "source/Node.cpp" "source/PathFinder.cpp")

target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-graphics.lib)
target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-window.lib)
target_link_libraries(A_star ${CMAKE_SOURCE_DIR}/libraries/sfml-system.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_custom_command(TARGET A_star POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bin/openal32.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-audio-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-audio-d-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-graphics-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-graphics-d-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-network-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-network-d-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-system-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-system-d-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-window-2.dll
        ${CMAKE_SOURCE_DIR}/bin/sfml-window-d-2.dll
        $<TARGET_FILE_DIR:A_star>
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET A_star PROPERTY CXX_STANDARD 20)
  set_property(TARGET A_star PROPERTY CXX_STANDARD_REQUIRED ON)
endif()