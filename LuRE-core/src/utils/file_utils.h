#pragma once

#include <string>
#include <vector>

namespace lumi {

	struct rawModel {
		float* vertices;
		size_t verticesCount;

		float* uvs;
		size_t uvsCout;

		float* normals;
		size_t normalsCount;

		unsigned int* indices;
		size_t indicesCount;
	};


	struct loadedModel {
		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> uvs;

		std::vector<unsigned int> indices;

	};

	std::string read_file(const char* filepath);

	std::vector<std::string> split(const std::string &s, char delim);

	rawModel loadOBJ(const char* filepath);

}