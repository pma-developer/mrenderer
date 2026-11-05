
add_subdirectory ("src")
add_executable (mrenderer_app "app.cpp" "app.h" "benchmarks/benchmark.cpp")
target_include_directories(mrenderer_app PRIVATE "src")
target_link_libraries(mrenderer_app PRIVATE mrenderer_lib)

set_property(TARGET mrenderer_app PROPERTY CXX_STANDARD 20)