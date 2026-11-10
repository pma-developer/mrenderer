#pragma once
#include <iostream>
#include <vector>
#include <filesystem>

namespace mrenderer {
	struct Color {
		uint8_t rgba[4];

		uint8_t& r() { return rgba[0]; }
		uint8_t& g() { return rgba[1]; }
		uint8_t& b() { return rgba[2]; }
		uint8_t& a() { return rgba[3]; }

		uint8_t& operator[](const int i) { return rgba[i]; }
		const uint8_t& operator[](const int i) const { return rgba[i]; }
	};

	namespace colors {
		inline constexpr Color red{ 255, 0, 0, 255 };
		inline constexpr Color green{ 0, 255, 0, 255 };
		inline constexpr Color blue{ 0, 0, 255, 255 };
		inline constexpr Color yellow{ 255, 255, 0, 255 };
		inline constexpr Color black{ 0, 0, 0, 0 };
		inline constexpr Color white{ 255, 255, 255, 255 };
	}

	class Image {
	public:
		Image() = default;
		Image(int w, int h, uint8_t channels);
		int WritePng(const std::filesystem::path& path, bool flip);
		void set(int x, int y, const Color c);
		Color get(int x, int y) const;
		void FlipY();
		int width() const;
		int height() const;
		uint8_t get_channels() const;
	private:
		int w = 0, h = 0;
		uint8_t channels = 0;
		std::vector<uint8_t> data;
	};
}
