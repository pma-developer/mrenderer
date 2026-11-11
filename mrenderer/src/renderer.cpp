#include "renderer.h"
#include "vec.h"
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
    void DrawTriangle(Vec2i v0, Vec2i v1, Vec2i v2, Color v0_c, Color v1_c, Color v2_c, Image& framebuffer) {
        Vec2i q1 = v1 - v0;
        Vec2i q2 = v2 - v0;

        int total_area = Cross(q1, q2); // signed double area
        //if (total_area < 2) return;     // discard backfaces, triangles covering less than 1 px
        auto [start, end] = CalculateAABB<int,2>(std::array{ v0, v1, v2 });

        for (int y = start.y; y <= end.y; ++y) {
            for (int x = start.x; x <= end.x; ++x) {
                Vec2i p = Vec2i{ x, y } - v0;
                float gamma = Cross(q1, p) / (float)total_area;
                float beta = Cross(p, q2) / (float)total_area;
                float alpha = 1 - beta - gamma;
                bool inside = alpha >= 0 && beta >= 0 && gamma >= 0;
                Color color = alpha * v0_c + beta * v1_c + gamma * v2_c;

                if (inside) {
                    
                    framebuffer.set(x, y, color);
                    //if (alpha >= 0.1f && beta >= 0.1f && gamma >= 0.1f) {
                    //    framebuffer.set(x, y, colors::black);
                    //}
                    //float dist = Length(Vec3f{ alpha, beta, gamma } - Vec3f{ 0.3f, 0.3f, 0.3f });
                    //if (dist <= 0.6f) {
                    //    framebuffer.set(x, y, colors::black);
                    //}
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

    void DrawModel(Model& model, Image& framebuffer) {
        model.SortTrigsByZ();
        for (Vec3i trig : model.trigs) {
            Vec3f v0 = model.vertices[trig.data[0]];
            Vec3f v1 = model.vertices[trig.data[1]];
            Vec3f v2 = model.vertices[trig.data[2]];

            Vec2i v0_p = ProjectOrtho(v0, framebuffer.width(), framebuffer.height());
            Vec2i v1_p = ProjectOrtho(v1, framebuffer.width(), framebuffer.height());
            Vec2i v2_p = ProjectOrtho(v2, framebuffer.width(), framebuffer.height());

            Color c {
                std::rand() % 255,
                std::rand() % 255,
                std::rand() % 255,
                255
            };

            //DrawTriangle(v0_p, v1_p, v2_p, colors::red, colors::green, colors::blue, framebuffer);
            DrawTriangle(v0_p, v1_p, v2_p, c, c, c, framebuffer);
        }
    }

    void RunTest() {
        constexpr int kWidth = 1280;
        constexpr int kHeight = 720;
        //constexpr int kWidth = 320;
        //constexpr int kHeight = 180;

        Vec2i v0{ 10, 1 };
        Vec2i v1{ kWidth - 13, kHeight - 10 };
        Vec2i v2{ 2, kHeight - 16 };

        Image framebuffer = Image(kWidth, kHeight, 3);

        auto trig = std::array<Vec2i, 3>{ v0, v1, v2 };

        //DrawAABB(trig, framebuffer, colors::white);
        //DrawTriangleScanline(trig, framebuffer, colors::red);

        //DrawTriangle(v0, v1, v2, colors::red, colors::green, colors::blue, framebuffer);
        Model m = Model("assets/diablo3_pose/diablo3_pose.obj");
        DrawModel(m, framebuffer);
        //DrawWireframe(m, framebuffer);
        
        framebuffer.WritePng("framebuffer.png", true);
    }
}