#include <iostream>
#include <vector>
#include <filesystem>

struct Color {
	uint8_t rgba[4];

	uint8_t& r() { return rgba[0]; }
	uint8_t& g() { return rgba[1]; }
	uint8_t& b() { return rgba[2]; }
	uint8_t& a() { return rgba[3]; }

	uint8_t& operator[](const int i) { return rgba[i]; }
	const uint8_t& operator[](const int i) const { return rgba[i]; }
};

class Image {
public:
	Image() = default;
	Image(int w, int h, uint8_t channels);
	int write_png(std::filesystem::path) const;
	void set(int x, int y, const Color& c);
	Color get(int x, int y) const;
	int width() const;
	int height() const;
private:
	int w = 0, h = 0;
	uint8_t channels = 0;
	std::vector<uint8_t> data;
};
