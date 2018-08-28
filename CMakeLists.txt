cmake_minimum_required(VERSION 2.6)
project(Teleshooter)

# Define sources and executable
set(EXECUTABLE_NAME "GuardsAndLevers")
set(EXECUTABLE_OUTPUT_PATH "bin/${CMAKE_BUILD_TYPE}")
add_executable(
  ${EXECUTABLE_NAME}

  src/main.cpp
  src/Inputs.cpp
  src/Inputs.hpp
  src/Level.cpp
  src/Level.hpp
  src/drawWorld.cpp
  src/WorldState.cpp
  src/WorldState.hpp
)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 REQUIRED system window graphics network audio)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

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
