# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mrenderer")

add_subdirectory ("mrenderer")


option(BUILD_BENCHMARKS "Build benchmarks" OFF)
if(BUILD_BENCHMARKS)
	include(FetchContent)

	set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
	set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
	set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
	FetchContent_Declare(
	  benchmark
	  GIT_REPOSITORY https://github.com/google/benchmark
	  GIT_TAG        v1.10.0
	)
	FetchContent_MakeAvailable(benchmark)
	add_subdirectory ("mrenderer/benchmarks")
endif()


target_include_directories(mrenderer_lib PRIVATE "third_party/")
target_include_directories(mrenderer_app PRIVATE "third_party/")

add_custom_target(assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:mrenderer_app>/assets"
)

add_dependencies(mrenderer_app assets)
install(TARGETS mrenderer_app DESTINATION "out/installed")