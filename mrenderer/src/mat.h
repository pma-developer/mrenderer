#include "vec.h"

#include <array>
#include <cmath>

namespace mrenderer {
	struct Mat4 {
		std::array<Vec4f, 4> data;

		Mat4 operator*(const Mat4& rhs) {
			Mat4 result;

			for (size_t y = 0; y < 4; ++y) {
				for (size_t x = 0; x < 4; ++x) {
					for (size_t k = 0; k < 4; ++k) {
						result[y][x] += data[y][k] * rhs[k][x];
					}
				}
			}

			return result;
		}

		const Vec4f& operator[](size_t i) const { return data[i]; }
		Vec4f& operator[](size_t i) { return data[i]; }

		static constexpr Mat4 Identity() {
			return { std::array<Vec4f, 4>{
				Vec4f{ 1, 0, 0, 0 },
				Vec4f{ 0, 1, 0, 0 },
				Vec4f{ 0, 0, 1, 0 },
				Vec4f{ 0, 0, 0, 1 }
			} };
		}

		static constexpr Mat4 RotX(float angle) {
			float s = std::sin(angle);
			float c = std::cos(angle);
			return { std::array<Vec4f, 4>{
				Vec4f{ 1,  0,  0, 0 },
				Vec4f{ 0,  c, -s, 0 },
				Vec4f{ 0,  s,  c, 0 },
				Vec4f{ 0,  0,  0, 1 }
			} };
		}

		static constexpr Mat4 RotY(float angle) {
			float s = std::sin(angle);
			float c = std::cos(angle);
			return { std::array<Vec4f, 4>{
				Vec4f{ 1,  0,  0, 0 },
				Vec4f{ 0,  c, -s, 0 },
				Vec4f{ 0,  s,  c, 0 },
				Vec4f{ 0,  0,  0, 1 }
			} };
		}
		static constexpr Mat4 RotZ(float angle) {
			float s = std::sin(angle);
			float c = std::cos(angle);
			return { std::array<Vec4f, 4>{
				Vec4f{ s, c, 0, 0 },
				Vec4f{ c, -s, 0, 0 },
				Vec4f{ 0, 0, 1, 0 },
				Vec4f{ 0, 0, 0, 1 }
			} };
		}
	};

	template<typename Mat>
	Mat Transpose(const Mat& mat) {
		Mat result;
		for (size_t y = 0; y < mat.data.size(); ++y) {
			for (size_t x = 0; x < mat.data.size(); ++x) {
				result[y][x] = mat[x][y];
			}
		}
		return result;
	}
}