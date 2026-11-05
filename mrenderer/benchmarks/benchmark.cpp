#include "renderer.h"
#include "image.h"

#include <benchmark/benchmark.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void naive_line(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    for (int x = ax; x <= bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + (by - ay) * t);
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
    }
}


static void BM_naive_line(benchmark::State& state) {
    srand(time(0));
    const int w = 1920;
    const int h = 1080;
    Image fb = Image(w, h, 3);

    for (auto _ : state) {
        int x0 = rand() % w;
        int x1 = rand() % w;
        int y0 = rand() % h;
        int y1 = rand() % h;

        naive_line(x0, y0, x1, y1, fb, colors::white);
    }
}

static void BM_line(benchmark::State& state) {
    srand(time(0));
    const int w = 1920;
    const int h = 1080;
    Image fb = Image(w, h, 3);

    for (auto _ : state) {
        int x0 = rand() % w;
        int x1 = rand() % w;
        int y0 = rand() % h;
        int y1 = rand() % h;
        
        line(x0, y0, x1, y1, fb, colors::white);
    }
}

BENCHMARK(BM_line);
BENCHMARK(BM_naive_line);