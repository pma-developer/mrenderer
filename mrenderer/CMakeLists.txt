
add_subdirectory ("src")
add_executable (mrenderer_app "app.cpp" "app.h")

target_link_libraries(mrenderer_app PRIVATE mrenderer_lib)