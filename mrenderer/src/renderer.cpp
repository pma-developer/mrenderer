#include "renderer.h"
#include "mmath.h"

#include <ctime>
#include <cassert>
#include <iostream>

namespace mrenderer {
    void line(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
        bool steep = std::abs(bx - ax) < std::abs(by - ay);
        if (steep) {
            std::swap(ax, ay);
            std::swap(bx, by);
        }
        if (ax > bx) {
            std::swap(ax, bx);
            std::swap(ay, by);
        }
        const int dx = bx - ax;
        const int dy = std::abs(by - ay);
        const int sy = (by > ay) ? 1 : -1;

        int y = ay;
        int x = ax;
        int p = -dx;
        for (; x <= bx; ++x) {
            if (p > 0) {
                y += sy;
                p += 2 * dy - 2 * dx;
            }
            else {
                p += 2 * dy;
            }
            if (steep)
                framebuffer.set(y, x, color);
            else
                framebuffer.set(x, y, color);
        }
    }


    void runTest() {
        Vec2i tvec = { 1, 1 };
        Vec2i tvec2 = { 3, 3 };

        std::cout << std::format("vec1 {0}; vec2 {1}", tvec, tvec2) << std::endl;
        std::cout << to_string(tvec + tvec2) << std::endl;

        return;
        constexpr int width = 16;
        constexpr int height = 9;
        Image framebuffer = Image(width, height, 3);
        line(0, 0, width - 1, height - 3, framebuffer, colors::green);
        line(0, width - 1, width - 1, height - 3, framebuffer, colors::yellow);
        line(width - 1, 3, 0, height - 3, framebuffer, colors::green);
        line(width - 1, 3, 0, height - 3, framebuffer, colors::blue);

        line(0, 0, 3, height - 3, framebuffer, colors::white);
        line(0, height - 3, 3, 0, framebuffer, colors::white);

        //line(3, 1, 10, 10, framebuffer, colors::green);

        //std::srand(std::time({}));
        //for (int i = 0; i < 200; i++) {
        //    int ax = rand() % width, ay = rand() % height;
        //    int bx = rand() % width, by = rand() % height;
        //    line(ax, ay, bx, by, framebuffer, red);
        //}

        framebuffer.write_png("framebuffer.png", true);
    }
}