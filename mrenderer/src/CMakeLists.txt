
add_library (mrenderer_lib "image.cpp" "image.h" "renderer.cpp" "renderer.h"    "model.cpp" "model.h" "vec.cpp" "vec.h"  "mat.cpp" "mat.h")
target_include_directories(mrenderer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mrenderer_lib PUBLIC cxx_std_20)