#pragma once
#include "image.h"

namespace mrenderer {
	void Line(int ax, int ay, int bx, int by, Image& framebuffer, const Color& color);
	void RunTest();
}