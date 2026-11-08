#include "renderer.h"
#include "mmath.h"
#include "model.h"

#include <ctime>
#include <cassert>
#include <iostream>
#include <array>
#include <span>
#include <ranges>

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

    void DrawTriangleScanline(std::array<Vec2i, 3> verts, Image& framebuffer, const Color& color) {
        std::sort(verts.begin(), verts.end(), [](Vec2i a, Vec2i b) {
            return a.y < b.y;
        });
        Vec2i v0 = verts[0];
        Vec2i v1 = verts[1];
        Vec2i v2 = verts[2];

        if (v0.y == v2.y) {
            auto [minIt, maxIt] = std::minmax_element(verts.begin(), verts.end(), [](Vec2i a, Vec2i b) {
                return a.x < a.y;
            });
            Line(minIt->x, v0.y, maxIt->x, v0.y, framebuffer, color);
            return;
        }

        float slope01 = v1.y == v0.y ? 0 : (v1.x - v0.x) / (float)(v1.y - v0.y);
        float slope12 = v2.y == v1.y ? 0 : (v2.x - v1.x) / (float)(v2.y - v1.y);

        for (int y = v0.y; y <= v2.y; ++y) {
            float x0 = (y - v0.y) * (v2.x - v0.x) / (float)(v2.y - v0.y) + v0.x;
            float x1 = y < v1.y
                ? (y - v0.y) * slope01 + v0.x
                : (y - v1.y) * slope12 + v1.x;

            Line(std::round(x0), y, std::round(x1), y, framebuffer, color);
        }
    }

    void DrawAABB(std::span<Vec2i> verts, Image& framebuffer, const Color& color) {
        auto [start, end] = CalculateAABB(verts);
        Vec2i start_r = Vec2i{ end.x, start.y };
        Vec2i end_l = Vec2i{ start.x, end.y };

        DrawTriangleScanline({ start, start_r, end_l }, framebuffer, color);
        DrawTriangleScanline({ start_r, end, end_l }, framebuffer, color);
    }

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
        constexpr int kWidth = 20;
        constexpr int kHeight = 20;

        Vec2i v0{ 9, 1 };
        Vec2i v1{ 13, 10 };
        Vec2i v2{ 1, 16 };

        Image framebuffer = Image(kWidth, kHeight, 3);

        auto trig = std::array<Vec2i, 3>{ v0, v1, v2 };
        //Line(v0, v1, framebuffer, colors::white);
        //Line(v1, v2, framebuffer, colors::white);
        //Line(v2, v0, framebuffer, colors::white);
        DrawAABB(trig, framebuffer, colors::white);
        DrawTriangleScanline(trig, framebuffer, colors::red);

        //Model m = Model("assets/diablo3_pose/diablo3_pose.obj");
        //DrawWireframe(m, framebuffer);

        framebuffer.WritePng("framebuffer.png", true);
    }
}