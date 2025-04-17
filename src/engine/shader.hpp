#pragma once

#include <glm/glm.hpp>
#include <string>

enum ShaderType {
	PROGRAM = 0,
    VERTEX,
    FRAGMENT
};

class Shader {
	public:
		Shader(std::string vertexPath, std::string fragmentPath);

		void use();

		void setInt(const char* name, int value);
		void setFloat(const char* name, float value);
		void setVec2(const char* name, glm::vec2 value);
		void setVec3(const char* name, glm::vec3 value);
		void setVec4(const char* name, glm::vec4 value);
		void setMat4(const char* name, glm::mat4 value);

	private:
		void checkCompileErrors(unsigned int shader, enum ShaderType type);

		unsigned int m_ID;
};