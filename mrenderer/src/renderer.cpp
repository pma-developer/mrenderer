#include "renderer.h"

#include <ctime>
#include <cassert>
#include <iostream>



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
