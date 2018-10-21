#pragma once

#include <vector>

#include "Model.h"

namespace lumi {
	namespace graphics {

		class Renderer 
		{
		public:
			Renderer();
			virtual ~Renderer();

			virtual void render(std::vector<Model>& models) = 0;

		};

	}
}