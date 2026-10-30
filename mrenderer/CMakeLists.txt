# CMakeList.txt : CMake project for mrenderer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mrenderer "src/main.cpp" "src/main.h" "src/image.cpp" "src/image.h")
target_include_directories(mrenderer PRIVATE "deps/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mrenderer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
