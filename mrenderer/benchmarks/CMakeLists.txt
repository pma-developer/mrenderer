add_executable(mrenderer_benchmarks "benchmark.cpp")

target_link_libraries(mrenderer_benchmarks
    PRIVATE
        mrenderer_lib
        benchmark::benchmark
        benchmark::benchmark_main
)