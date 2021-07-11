#include "gepch.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.h"

namespace GooE {
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			GOOE_CORE_ERROR("{0}", infoLog.data());
			GOOE_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			GOOE_CORE_ERROR("{0}", infoLog.data());
			GOOE_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		rendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(rendererId, vertexShader);
		glAttachShader(rendererId, fragmentShader);

		// Link our program
		glLinkProgram(rendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(rendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(rendererId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave

			GOOE_CORE_ERROR("{0}", infoLog.data());
			GOOE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererId, vertexShader);
		glDetachShader(rendererId, fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(rendererId);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string name, const int value) {
		glUniform1i(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string name, const float value) {
		glUniform1f(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string name, const glm::vec3& value) {
		glUniform2f(GetUniformLocation(name.c_str()), value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string name, const glm::vec3& value) {
		glUniform3f(GetUniformLocation(name.c_str()), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string name, const glm::vec4& value) {
		glUniform4f(GetUniformLocation(name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string name, const glm::mat3& value) {
		glUniformMatrix3fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string name, const glm::mat4& value) {
		glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	const int OpenGLShader::GetUniformLocation(const std::string name) {
		if (uniformLocationsCache.find(name) != uniformLocationsCache.end())
			return uniformLocationsCache[name];

		GLint location = glGetUniformLocation(rendererId, name.c_str());
		uniformLocationsCache[name] = location;

		if (location == -1) GOOE_CORE_WARN("Uniform '{0}' doesn't exist!");

		return location;
	}
}