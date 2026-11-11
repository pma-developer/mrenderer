#pragma once
#include "vec.h"

#include <vector>
#include <filesystem>

namespace mrenderer {
	struct Model {
		std::vector<Vec3f> vertices;
		std::vector<Vec3i> trigs;

		Model(std::vector<Vec3f> vertices, std::vector<Vec3i> trigs) : vertices(vertices), trigs(trigs) {};
		Model(const std::filesystem::path& load_path);

		void LoadFromObj(const std::filesystem::path& input_path);
		void SortTrigsByZ();
	};
}