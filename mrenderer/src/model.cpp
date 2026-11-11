#include "model.h"

#include <fstream>
#include <ranges>
#include <iostream>

namespace mrenderer {

	Model::Model(const std::filesystem::path& load_path) {
		LoadFromObj(load_path);
	}

	void Model::LoadFromObj(const std::filesystem::path& input_path) {

		std::ifstream fs;
		fs.exceptions(std::ifstream::badbit);
		fs.open(input_path);

		vertices = std::vector<Vec3f>();
		trigs = std::vector<Vec3i>();

		std::string line;
		while (std::getline(fs, line)) {

			std::istringstream line_ss(line);
			std::string fst;
			line_ss >> fst;
			if (fst == "v") {
				Vec3f v;
				if (!(line_ss >> v.x >> v.y >> v.z)) break;

				vertices.push_back(v);
			}
			else if (fst == "f") {
				std::string face_str;
				Vec3i trig;
				for (size_t i = 0; line_ss >> face_str; ++i) {
					// .obj file indices start from 1, so '- 1' is required
					trig.data[i] = std::stoi(face_str) - 1;
				}
				trigs.push_back(trig);
			}
		}
	}

	void Model::SortTrigsByZ() {
		auto minZ = [&](const Vec3i& t) {
			return std::min(std::min(vertices[t.data[0]].z, vertices[t.data[1]].z), vertices[t.data[2]].z);
		};


		std::sort(trigs.begin(), trigs.end(), [&](const Vec3i& trig1, const Vec3i& trig2) {
			return minZ(trig1) < minZ(trig2);
		});
	}
}