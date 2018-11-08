cmake_minimum_required(VERSION 2.6)
project(GuardsAndLevers)

# Define sources and executable
set(EXECUTABLE_NAME "GuardsAndLevers")
set(EXECUTABLE_OUTPUT_PATH "bin/${CMAKE_BUILD_TYPE}")
include_directories(src)
include_directories(lib)
add_executable(
  ${EXECUTABLE_NAME}

  src/main.cpp
  src/Inputs.cpp
  src/Inputs.hpp
  src/Level.cpp
  src/Level_drawWorld.cpp
  src/Level.hpp
  src/LevelLogic/LevelLogic.cpp
  src/LevelLogic/LevelLogic_guardAi.cpp
  src/LevelLogic/LevelLogic.hpp
  src/WorldState.cpp
  src/WorldState.hpp
  src/ActionState.cpp
  src/ActionState.hpp
  src/AnimationState.cpp
  src/AnimationState.hpp

  lib/imgui/imstb_truetype.h
  lib/imgui/imgui-SFML.h
  lib/imgui/imgui_draw.cpp
  lib/imgui/imconfig.h
  lib/imgui/imgui_internal.h
  lib/imgui/imgui-SFML.cpp
  lib/imgui/imgui.cpp
  lib/imgui/imgui_demo.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imstb_textedit.h
  lib/imgui/imgui.h
  lib/imgui/imstb_rectpack.h
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Detect and add SFML
find_package(SFML 2.4 REQUIRED system window graphics network audio)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

# Detect and add OpenGL
find_package(OpenGL)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})

# Set C++ standard to C++17
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD_REQUIRED true)
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 17)

if(MSVC)
  target_compile_options(${EXECUTABLE_NAME} PRIVATE "/W4")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wall")
  target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wextra")
  target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wno-unused-parameter")
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
include(CPack)
