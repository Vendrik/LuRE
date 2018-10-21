#include "file_utils.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "..\maths\maths.h"

#include "Log.h"

namespace lumi {


	std::string read_file(const char* filepath)
	{
		std::ifstream inputFile;
		std::stringstream sstream;

		inputFile.open(filepath);
		if (!inputFile.good()) {

			LUMI_ERROR("Could not open the current file: %s", filepath);
			inputFile.close();
			return std::string("");
		}

		sstream << inputFile.rdbuf();

		inputFile.close();
		return sstream.str();

	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> tokens;

		while (std::getline(ss, item, delim)) {
			tokens.push_back(item);
		}
		return tokens;
	}

	rawModel loadOBJ(const char* filepath)
	{
		rawModel result = {nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0};


		std::vector<maths::vec3> vertices;
		std::vector<maths::vec2> uvs;
		std::vector<maths::vec3> normals;
		std::vector<unsigned int> indices;

		std::ifstream inputFile;


		inputFile.open(filepath);
		if (!inputFile.good()) {

			LUMI_ERROR("Could not open the current file: %s", filepath);
			inputFile.close();
			return result;
		}

		std::string line;
		bool toAlloc = true;
		while (std::getline(inputFile, line))
		{

			std::vector<std::string> tokens = split(line, ' ');

			if (tokens[0].compare("v") == 0) {
				vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

			}
			else if (tokens[0].compare("vt") == 0) {
				uvs.emplace_back(std::stof(tokens[1]), 1.0f - std::stof(tokens[2]));

			}
			else if (tokens[0].compare("vn") == 0) {
				normals.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

			}
			else if (tokens[0].compare("f") == 0) {

				if (toAlloc) {
					result.vertices = (float*)malloc(sizeof(maths::vec3) * vertices.size());
					result.uvs = (float*)malloc(sizeof(maths::vec2) * vertices.size());
					result.normals = (float*)malloc(sizeof(maths::vec3) * vertices.size());
					toAlloc = false;
				}

				for (int i = 1; i < 4; i++) {
					std::vector<std::string> vertex = split(tokens[i], '/');

					int currentVertexPointer = std::stoi(vertex[0]) - 1;
					indices.emplace_back(currentVertexPointer);

					int currentUvs = std::stoi(vertex[1]) - 1;
					memcpy_s(result.uvs + (currentVertexPointer * 2), sizeof(maths::vec2), &uvs[currentUvs], sizeof(maths::vec2));

					int currentNormal = std::stoi(vertex[2]) - 1;
					memcpy_s(result.normals + (currentVertexPointer * 3), sizeof(maths::vec3), &normals[currentNormal], sizeof(maths::vec3));

				}
			}
		}

		inputFile.close();

		memcpy_s(result.vertices, sizeof(float) * 3 * vertices.size(), &vertices[0], sizeof(float) * 3 * vertices.size());

		result.indices = (unsigned int*)malloc(sizeof(unsigned int) * indices.size());
		memcpy_s(result.indices, sizeof(unsigned int) * indices.size(), &indices[0], sizeof(unsigned int) * indices.size());

		result.indicesCount = indices.size();
		result.uvsCout = vertices.size() * 2;
		result.normalsCount = vertices.size() * 3;
		result.verticesCount = vertices.size() * 3;

		return result;

	}



}