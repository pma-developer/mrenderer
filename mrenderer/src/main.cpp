#include "main.h"
#include "image.h"

#include <ctime>

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
    constexpr int width = 512;
    constexpr int height = 512;
    Image framebuffer = Image(width, height, 3);
    Color red = { 255, 0, 0, 255 };

    std::srand(std::time({}));
    for (int i = 0; i < 200; i++) {
        int ax = rand() % width, ay = rand() % height;
        int bx = rand() % width, by = rand() % height;
        line(ax, ay, bx, by, framebuffer, red);
    }

    framebuffer.write_png("framebuffer.png");
    return 0;
}
