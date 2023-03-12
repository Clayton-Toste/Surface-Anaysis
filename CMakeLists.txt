cmake_minimum_required(VERSION 3.0.0)
project(surface_analysis VERSION 1.0.0)
set (CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

file(GLOB SOURCE src/*.cpp)
add_executable(surface_analysis ${SOURCE})

target_link_libraries(surface_analysis ${wxWidgets_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH bin)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set_target_properties(surface_analysis PROPERTIES SUFFIX ".exe")
include(CPack)
