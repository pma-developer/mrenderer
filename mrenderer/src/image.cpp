
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "image.h"
#include <filesystem>

namespace mrenderer {

	Image::Image(int w, int h, uint8_t channels) : w(w), h(h), channels(channels), data(w* h* channels, 0) {}

	int Image::WritePng(const std::filesystem::path& out_path, bool flip) {
		stbi_flip_vertically_on_write(flip);
		return stbi_write_png(out_path.string().c_str(), w, h, channels, data.data(), 0);
	}

	void Image::set(int x, int y, const Color c) {
		if (x < 0 || y < 0 || y >= h || x >= w) return;
		size_t offset = (x + static_cast<size_t>(y) * w) * channels;
		memcpy(data.data() + offset, c.data, channels);
	}

	Color Image::get(int x, int y) const {
		size_t offset = (x + static_cast<size_t>(y) * w) * channels;
		Color res = { 0 };
		for (uint8_t ch = 0; ch < channels; ++ch) {
			res.data[ch] = data[offset + ch];
		}
		return res;
	}

	void Image::FlipY() {
		assert(w > 0 && h > 0 && channels > 0);
		const int row_bytes = w * channels;

		for (int byte = 0; byte < row_bytes * (h / 2); ++byte) {
			int row = byte / row_bytes;
			int col = byte % row_bytes;

			int flipped = (h - row - 1) * row_bytes + col;
			std::swap(data[byte], data[flipped]);
		}
	}

	int Image::width() const { return w; }
	int Image::height() const { return h; }
	uint8_t Image::get_channels() const { return channels; }
}