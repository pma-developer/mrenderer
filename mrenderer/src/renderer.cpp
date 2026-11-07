#include "renderer.h"
#include "mmath.h"
#include "model.h"

#include <ctime>
#include <cassert>
#include <iostream>

namespace mrenderer {
    void Line(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color) {
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
    void Line(Vec2i p0, Vec2i p1, Image& framebuffer, const Color& color) {
        Line(p0.x, p0.y, p1.x, p1.y, framebuffer, color);
    }
    void Line3dOrtho(Vec3f p0, Vec3f p1, Image& framebuffer, const Color& color) {
        p0.x = (p0.x + 1) * framebuffer.width() / 2;
        p0.y = (p0.y + 1) * framebuffer.height() / 2;

        p1.x = (p1.x + 1) * framebuffer.width() / 2;
        p1.y = (p1.y + 1) * framebuffer.height() / 2;

        Vec2i proj_p0_i = Vec2i{ (int)std::round(p0.x), (int)std::round(p0.y) };
        Vec2i proj_p1_i = Vec2i{ (int)std::round(p1.x), (int)std::round(p1.y) };
        Line(proj_p0_i, proj_p1_i, framebuffer, color);
    };
    void Line3d(Vec3f p0, Vec3f p1, Image& framebuffer, const Color& color) {
        Vec2f proj_p0 = Vec2f{ p0.x / p0.z, p0.y / p0.z };
        Vec2f proj_p1 = Vec2f{ p1.x / p1.z, p1.y / p1.z };

        proj_p0.x = (proj_p0.x + 1) * framebuffer.width() / 2;
        proj_p0.y = (proj_p0.y + 1) * framebuffer.height() / 2;

        proj_p1.x = (proj_p1.x + 1) * framebuffer.width() / 2;
        proj_p1.y = (proj_p1.y + 1) * framebuffer.height() / 2;

        Vec2i proj_p0_i = Vec2i{ (int)std::round(proj_p0.x), (int)std::round(proj_p0.y) };
        Vec2i proj_p1_i = Vec2i{ (int)std::round(proj_p1.x), (int)std::round(proj_p1.y) };
        Line(proj_p0_i, proj_p1_i, framebuffer, color);
    };

    void DrawWireframe(const Model& model, Image& framebuffer) {
        for(Vec3i trig : model.trigs) {
            Vec3f v0 = model.vertices[trig.data[0]];
            Vec3f v1 = model.vertices[trig.data[1]];
            Vec3f v2 = model.vertices[trig.data[2]];

            Line3dOrtho(v0, v1, framebuffer, colors::white);
            Line3dOrtho(v1, v2, framebuffer, colors::white);
            Line3dOrtho(v2, v0, framebuffer, colors::white);
        }
    }

    void RunTest() {
        constexpr int kWidth = 1280;
        constexpr int kHeight = 720;
        Image framebuffer = Image(kWidth, kHeight, 3);
        Model m = Model("assets/diablo3_pose/diablo3_pose.obj");
        DrawWireframe(m, framebuffer);

        framebuffer.WritePng("framebuffer.png", true);
    }
}