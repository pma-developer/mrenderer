
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "image.h"
#include <filesystem>

Image::Image(int w, int h, uint8_t channels) : w(w), h(h), channels(channels), data(w*h*channels, 0) { }

int Image::write_png(std::filesystem::path path) const {
	return stbi_write_png(path.string().c_str(), w, h, channels, data.data(), 0);
}

void Image::set(int x, int y, const Color& c) {
	if (x < 0 || y < 0 || y >= h || x >= w) return;
	size_t offset = (x + static_cast<size_t>(y) * w) * channels;
	memcpy(data.data() + offset, c.rgba, channels);
}

Color Image::get(int x, int y) const {
	size_t offset = (x + static_cast<size_t>(y) * w) * channels;
	Color res = { 0 };
	for (uint8_t ch = 0; ch < channels; ++ch) {
		res[ch] = data[offset + ch];
	}
	return res;
} 


int Image::width() const { return w; }
int Image::height() const { return h; }