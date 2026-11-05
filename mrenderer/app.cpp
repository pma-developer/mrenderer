#include "image.h"
#include "renderer.h"

#include <ctime>
#include <cassert>
#include <iostream>

int main()
{
    constexpr int width = 16;
    constexpr int height = 9;
    Image framebuffer = Image(width, height, 3);
    line(0, 0, width - 1, height - 3, framebuffer, colors::green);
    b_line2(0, width - 1, width - 1, height - 3, framebuffer, colors::yellow);
    line(width - 1, 3, 0, height - 3, framebuffer, colors::green);
    b_line2(width - 1, 3, 0, height - 3, framebuffer, colors::blue);

    b_line2(0, 0, 3, height - 3, framebuffer, colors::white);
    b_line2(0, height - 3, 3, 0, framebuffer, colors::white);

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