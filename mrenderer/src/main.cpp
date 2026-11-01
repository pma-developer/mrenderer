#include "main.h"
#include "image.h"

#include <ctime>
#include <cassert>

void b_line0(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
    int dx = bx - ax;
    int dy = by - ay;
    assert(bx >= ax && by >= ay && dy <= dx);
    float m = dy / (float)dx;
    float b = (bx * ay - ax * by) / (float)dx;
    int y = ay;
    for (int x = ax; x <= bx; ++x) {
        float d1d2 = 2 * m * (x + 1) - 2 * y + 2 * b - 1;
        framebuffer.set(x, y, color);
        if (d1d2 > 0) {
            y++;
        }
    }
}

void b_line1(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
    int dx = bx - ax;
    int dy = by - ay;
    assert(bx >= ax && by >= ay && dy <= dx);
    int c = 2 * (bx * ay - ax * by) - dx;
    int y = ay;
    for (int x = ax; x <= bx; ++x) {
        int p = 2 * dy * (x + 1) - 2 * y * dx;
        framebuffer.set(x, y, color);
        if (p > -c) {
            y++;
        }
    }
}

void b_line2(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
    const int dx = bx - ax;
    const int dy = by - ay;
    assert(bx >= ax && by >= ay && dy <= dx);
    int y = ay;
    int x = ax;
    const int c = 2 * (bx * y - x * by) - dx;
    int p = 2 * (dy * x - dx * y) + c;
    for (; x <= bx; ++x) {
        if (p > 0) {
            y++;
            p += 2 * dy - 2 * dx;
        }
        else {
            p += 2 * dy;
        }
        framebuffer.set(x, y, color);
    }
}



void line(int ax, int ay, int bx, int by, Image& framebuffer, Color color) {
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


int main()
{
    constexpr int width = 16;
    constexpr int height = 9;
    Image framebuffer = Image(width, height, 3);
    line(0, 0, width - 1, height - 3, framebuffer, colors::green);
    b_line2(0, 0, width - 1, height - 3, framebuffer, colors::yellow);

    //line(3, 1, 10, 10, framebuffer, colors::green);

    //std::srand(std::time({}));
    //for (int i = 0; i < 200; i++) {
    //    int ax = rand() % width, ay = rand() % height;
    //    int bx = rand() % width, by = rand() % height;
    //    line(ax, ay, bx, by, framebuffer, red);
    //}

    framebuffer.write_png("framebuffer.png", true);
    return 0;
}
