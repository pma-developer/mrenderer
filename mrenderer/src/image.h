#pragma once

#include "vec.h"

#include <iostream>
#include <vector>
#include <filesystem>

namespace mrenderer {

	using Color = Vec<uint8_t, 4>;


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
