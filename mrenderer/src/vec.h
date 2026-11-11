#pragma once
#include <format>
#include <limits>
#include <span>
#include <tuple>

namespace mrenderer {
	template <typename T, size_t L> struct Vec {
		T data[L];
	};
	template <typename T> struct Vec<T, 3> {
		union {
			struct { T x, y, z; };
			struct { T x, y, z; };
			T data[3];
		};

		static constexpr Vec<T, 3> Zero() { return { 0, 0, 0 }; };
		static constexpr Vec<T, 3> One()  { return { 1, 1, 1 }; };
	};
	template <typename T> struct Vec<T, 2> {
		union {
			struct { T x, y; };
			T data[2];
		};

		static constexpr Vec<T, 2> Zero() { return { 0, 0}; };
		static constexpr Vec<T, 2> One() { return { 1, 1}; };
	};

	template <typename T, size_t L>
	Vec<T, L> operator*(const Vec<T, L>& lhs, float k) {
		Vec<T, L> result;
		for (size_t i = 0; i < L; ++i) {
			result.data[i] = lhs.data[i] * k;
		}
		return result;
	}

	template <typename T, size_t L>
	Vec<T, L> operator*(float k, const Vec<T, L>& rhs) {
		return rhs * k;
	}

	template <typename T, size_t L>
	Vec<T, L> operator+(const Vec<T, L>& lhs, const Vec<T, L>& rhs) {
		Vec<T, L> result;
		for (size_t i = 0; i < L; ++i) {
			result.data[i] = lhs.data[i] + rhs.data[i];
		}
		return result;
	}

	template <typename T, size_t L>
	Vec<T, L>& operator+=(Vec<T, L>& lhs, const Vec<T, L>& rhs) {
		lhs = lhs + rhs;
		return lhs;
	}

	template <typename T, size_t L>
	Vec<T, L> operator-(const Vec<T, L>& lhs, const Vec<T, L>& rhs) {
		Vec<T, L> result;
		for (size_t i = 0; i < L; ++i) {
			result.data[i] = lhs.data[i] - rhs.data[i];
		}
		return result;
	}

	template <typename T, size_t L>
	Vec<T, L>& operator-=(Vec<T, L>& lhs, const Vec<T, L>& rhs) {
		lhs = lhs - rhs;
		return lhs;
	}


	template <typename T, size_t L>
	T Dot(const Vec<T, L>& v1, const Vec<T, L>& v2) {
		T res = 0;
		for (size_t i = 0; i < L; ++i) {
			res += v1.data[i] * v2.data[i];
		}
		return res;
	}

	template <typename T, size_t L>
	double Length(const Vec<T, L>& vec) {
		return sqrt(Dot(vec, vec));
	}

	template <typename T, size_t L>
	std::tuple<Vec<T, L>, Vec<T, L>> CalculateAABB(std::span<const Vec<T, L>> vecs) {
		Vec<T, L> min = vecs[0], max = vecs[0];
		
		for(auto vec : vecs) {
			for (size_t i = 0; i < L; ++i) {
				if (min.data[i] > vec.data[i]) min.data[i] = vec.data[i];
				if (max.data[i] < vec.data[i]) max.data[i] = vec.data[i];
			}
		}

		return std::tuple{min, max};
	}

	template <typename T>
	constexpr Vec<T, 3> Cross(Vec<T, 3> p, Vec<T, 3> q) {
		return Vec<T, 3> {
			p.y * q.z - p.z * q.y,
			p.z * q.x - p.x * q.z,
			p.x * q.y - p.y * q.x
		};
	}

	template <typename T>
	constexpr T Cross(Vec<T, 2> p, Vec<T, 2> q) {
		return p.x * q.y - p.y * q.x;
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
	}


	using Vec3f = Vec<float, 3>;
	using Vec2f = Vec<float, 2>;
	using Vec3i = Vec<std::int32_t, 3>;
	using Vec2i = Vec<std::int32_t, 2>;
}