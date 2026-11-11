#include "renderer.h"
#include "vec.h"
#include "model.h"

#include <ctime>
#include <cassert>
#include <iostream>
#include <array>
#include <span>
#include <ranges>
#include <algorithm>

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
    Vec2i ProjectOrtho(Vec3f p, int w, int h) {
        return Vec2i{
            (int)((p.x + 1) * w / 2),
            (int)((p.y + 1) * h / 2)
        };
    }

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
    void DrawTriangle(Vec3f v0_f, Vec3f v1_f, Vec3f v2_f, Color v0_c, Color v1_c, Color v2_c, Image& framebuffer, Image& z_buffer) {

        Vec2i v0 = ProjectOrtho(v0_f, framebuffer.width(), framebuffer.height());
        Vec2i v1 = ProjectOrtho(v1_f, framebuffer.width(), framebuffer.height());
        Vec2i v2 = ProjectOrtho(v2_f, framebuffer.width(), framebuffer.height());

        Vec2i e1 = v2 - v0; // edge opposite to v1
        Vec2i e2 = v1 - v0; // edge opposite to v2

        int total_area = Cross(e2, e1); // signed double area
        if (total_area < 2) return;     // discard backfaces, triangles covering less than 1 px
        auto [start, end] = CalculateAABB<int,2>(std::array{ v0, v1, v2 });

        for (int y = start.y; y <= end.y; ++y) {
            for (int x = start.x; x <= end.x; ++x) {
                Vec2i p = Vec2i{ x, y } - v0;

                int w1 = Cross(p, e1);
                int w2 = Cross(e2, p);
                int w0 = total_area - w1 - w2;

                bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
                if (!inside) continue;

                double gamma = w2 / (double)total_area;
                double beta = w1 / (double)total_area;
                double alpha = 1 - beta - gamma;

                double z_val = alpha * v0_f.z + beta * v1_f.z + gamma * v2_f.z;
                uint8_t z_channel = (z_val + 1) * 255 / 2;
                if (z_channel > z_buffer.get(x, y).data[0]) {
                    Color color = alpha * v0_c + beta * v1_c + gamma * v2_c;

                    framebuffer.set(x, y, color);
                    z_buffer.set(x, y, Color { z_channel, 0, 0, 0});
                }
            }
        }
    }

    void DrawAABB(std::span<const Vec2i> verts, Image& framebuffer, const Color& color) {
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

            Vec2i v0_p = ProjectOrtho(v0, framebuffer.width(), framebuffer.height());
            Vec2i v1_p = ProjectOrtho(v1, framebuffer.width(), framebuffer.height());
            Vec2i v2_p = ProjectOrtho(v2, framebuffer.width(), framebuffer.height());


            Line(v0_p, v1_p, framebuffer, colors::white);
            Line(v1_p, v2_p, framebuffer, colors::white);
            Line(v2_p, v0_p, framebuffer, colors::white);
        }
    }

    void DrawModel(Model& model, Image& framebuffer, Image& z_buffer) {
        for (Vec3i trig : model.trigs) {
            Vec3f v0 = model.vertices[trig.data[0]];
            Vec3f v1 = model.vertices[trig.data[1]];
            Vec3f v2 = model.vertices[trig.data[2]];

            Color c {
                std::rand() % 255,
                std::rand() % 255,
                std::rand() % 255,
                255
            };

            DrawTriangle(v0, v1, v2, c, c, c, framebuffer, z_buffer);
        }
    }

    void DrawDiabloModel() {
        constexpr int kWidth = 1280;
        constexpr int kHeight = 720;

        Image framebuffer = Image(kWidth, kHeight, 3);
        Image z_buffer = Image(kWidth, kHeight, 1);

        Model m = Model("assets/diablo3_pose/diablo3_pose.obj");
        DrawModel(m, framebuffer, z_buffer);

        framebuffer.WritePng("framebuffer.png", true);
        z_buffer.WritePng("z_buffer.png", true);
    }

    void DrawDebugTriangle() {
        constexpr int kWidth = 20;
        constexpr int kHeight = 20;

        Vec2i v0{ 10, 1 };
        Vec2i v1{ 13, 10 };
        Vec2i v2{ 2, 16 };

        Image framebuffer = Image(kWidth, kHeight, 3);
        Image z_buffer = Image(kWidth, kHeight, 1);

        auto trig = std::array<Vec2i, 3>{ v0, v1, v2 };
        auto trig3d = std::array<Vec3f, 3>();
        std::transform(trig.begin(), trig.end(), trig3d.begin(), [](Vec2i& v) {
            return Vec3f {
                (float)2*v.x/kWidth - 1,
                (float)2*v.y/kHeight - 1,
                1};
            }
        );

        DrawTriangle(trig3d[0], trig3d[1], trig3d[2], colors::red, colors::green, colors::blue, framebuffer, z_buffer);

        framebuffer.WritePng("framebuffer.png", true);
        z_buffer.WritePng("z_buffer.png", true);
    }

    void RunTest() {
        //DrawDebugTriangle();
        DrawDiabloModel();
    }
}