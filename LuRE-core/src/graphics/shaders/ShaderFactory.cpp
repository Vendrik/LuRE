#include "ShaderFactory.h"


namespace lumi {
	namespace graphics {
		namespace ShaderFactory {


			static const char* basic_vertex_shader =
				"# version 330 core\n"
				"layout(location = 0) in vec3 aPos; // the position variable has attribute position 0\n"
				"layout(location = 1) in vec2 aUvs;\n"
				"layout(location = 2) in vec3 aNormals;\n"
				"\n"
				"out vec3 FragPos; // specify a color output to the fragment shader\n"
				"out vec2 uvs;\n"
				"out vec3 normals;\n"
				"\n"
				"uniform mat4 pr_matrix;\n"
				"uniform mat4 vw_matrix;\n"
				"uniform mat4 ml_matrix;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
				"	FragPos = vec3(ml_matrix * vec4(aPos, 1.0)); // set the output variable to a dark-red color\n"
				"	uvs = aUvs;\n"
				"	normals = mat3(transpose(inverse(ml_matrix))) * aNormals;\n"
				"}\n";

			static const char* basic_fragment_shader =
				"# version 330 core\n"
				"out vec4 FragColor; // specify a color output to the fragment shader\n"
				"\n"
				"in vec3 FragPos; // the input variable from the vertex shader (same name and same type) \n"
				"in vec2 uvs;\n"
				"in vec3 normals;\n"
				"\n"
				"//uniform vec3 lightPos;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	//ambient\n"
				"	float ambientStr = 0.1;\n"
				"	vec3 ambient = ambientStr * vec3(1.0, 1.0, 1.0); //lightColor\n"
				"\n"
				"	//diffuse\n"
				"	vec3 norm = normalize(normals);\n"
				"	vec3 lightDir = normalize(vec3(0.0, 0.0, 25.0) - FragPos);\n"
				"	float diff = max(dot(norm, lightDir), 0.0);\n"
				"	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);\n"
				"\n"
				"	vec3 result = (ambient + diffuse) * vec3(0.4, 0.0, 0.0);\n"
				"	FragColor = vec4(norm, 1.0);\n"
				"}\n";

			static const char* default_vertex_shader =
				"# version 330 core\n"
				"\n"
				"layout (location = 0) in vec4 position;\n"
				"layout (location = 1) in vec2 uv;\n"
				"layout (location = 2) in float tid;\n"
				"layout (location = 3) in vec4 color;\n"
				"\n"
				"uniform mat4 pr_matrix;\n"
				"uniform mat4 vw_matrix = mat4(1.0);\n"
				"uniform mat4 ml_matrix = mat4(1.0);\n"
				"\n"
				"out DATA\n"
				"{\n"
				"	vec4 position;\n"
				"	vec2 uv;\n"
				"	float tid;\n"
				"	vec4 color;\n"
				"} vs_out;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_Position =  pr_matrix * vw_matrix * ml_matrix * position;\n"
				"	vs_out.position = ml_matrix * position;\n"
				"	vs_out.uv = uv; \n"
				"	vs_out.tid = tid; \n"
				"	vs_out.color = color;\n"
				"}\n";

			static const char* defaut_fragment_shader =
				"# version 330 core\n"
				"\n"
				"layout (location = 0) out vec4 color;\n"
				"\n"
				"uniform vec4 c;\n"
				"uniform vec2 light_pos;\n"
				"\n"
				"in DATA\n"
				"{\n"
				"	vec4 position;\n"
				"	vec2 uv;\n"
				"	float tid;\n"
				"	vec4 color;\n"
				"} fs_in;\n"
				"\n"
				"uniform sampler2D textures[32];\n"
				"\n"
				"void main()\n"
				"{\n"
				"	float intensity = 1.0 / length(fs_in.position.xy - light_pos);\n"
				"	//color = fs_in.color * intensity;\n"
				"	vec4 texColor = fs_in.color;\n"
				"\n"
				"	if (fs_in.tid > 0.0)\n"
				"	{\n"
				"		int tid = int(fs_in.tid - 0.5);\n"
				"		texColor = fs_in.color * texture(textures[tid], fs_in.uv);\n"
				"		//texColor = vec4(0,tid,1,1);\n"
				"	}\n"
				"	color = texColor;\n"
				"	//color = texColor * intensity;\n"
				"}\n";

			static const char* passthrough_vertex_shader =
				"#version 330 core\n"
				"layout (location = 0) in vec2 aPos;\n"
				"layout (location = 1) in vec2 aTexCoords;\n"
				"\n"
				"out vec2 TexCoords;\n"
				"\n"
				"void main()\n"
				"{\n"
				"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
				"    TexCoords = aTexCoords;\n"
				"}\n";

			static const char* passthrough_fragment_shader =
				"#version 330 core\n"
				"out vec4 FragColor;\n"
				"\n"
				"in vec2 TexCoords;\n"
				"\n"
				"uniform sampler2D screenTexture;\n"
				"\n"
				"void main()\n"
				"{\n"
				"    FragColor = texture(screenTexture, TexCoords);\n"
				"}\n";

			Shader * DefaultShader()
			{
				//return Shader::FromSource(default_vertex_shader, defaut_fragment_shader);
				return Shader::FromSource(basic_vertex_shader, basic_fragment_shader);
			}


			Shader * PassthroughShader()
			{
				return Shader::FromSource(passthrough_vertex_shader, passthrough_fragment_shader);
			}

		}
	}
}