
add_library (mrenderer_lib "image.cpp" "image.h" "renderer.cpp" "renderer.h"  "obj_parser.cpp" "obj_parser.h" "model.cpp" "model.h" "mmath.cpp" "mmath.h")
target_include_directories(mrenderer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mrenderer_lib PUBLIC cxx_std_20)