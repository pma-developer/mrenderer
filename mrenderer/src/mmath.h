#pragma once
#include <format>

namespace mrenderer {
	template <typename T, size_t L> struct Vec {
		T data[L];
	};
	template <typename T> struct Vec<T, 3> {
		union {
			struct {
				T x, y, z;
			};
			T data[3];
		};
	};
	template <typename T> struct Vec<T, 2> {
		union {
			struct {
				T x, y;
			};
			T data[2];
		};
	};

	template <typename T, size_t L>
	Vec<T, L> operator+(const Vec<T, L>& lhs, const Vec<T, L>& rhs) {
		Vec<T, L> result;
		for (size_t i = 0; i < L; ++i) {
			result.data[i] = lhs.data[i] + rhs.data[i];
		}
		return result;
	}

	template <typename T, size_t L>
	struct std::formatter<Vec<T, L>> : std::formatter<T> {

		template <typename FormatContext>
		FormatContext::iterator format(const mrenderer::Vec<T, L>& vec, FormatContext& ctx) const {
			auto out = ctx.out();
			out = std::format_to(out, "[");

			for (size_t i = 0; i < L; ++i) {
				ctx.advance_to(out);
				out = std::formatter<T>::format(vec.data[i], ctx);
				if (i < L - 1) {
					out = std::format_to(out, ", ");
				}
			}
			return std::format_to(out, "]");
		}
	};

	template <typename T, size_t L>
	std::string to_string(const Vec<T, L>& vec) {
		return std::format("{}", vec);
	};

	using Vec3f = Vec<float, 3>;
	using Vec2f = Vec<float, 2>;
	using Vec3i = Vec<std::int32_t, 3>;
	using Vec2i = Vec<std::int32_t, 2>;
}