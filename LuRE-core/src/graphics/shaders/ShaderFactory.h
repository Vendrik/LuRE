#pragma once

#include "Shader.h"

namespace lumi {
	namespace graphics {
		namespace ShaderFactory {
			Shader* DefaultShader();
			Shader * PassthroughShader();
		}


	}
}