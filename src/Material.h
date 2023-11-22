#pragma once

#include "Texture.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_map>

class Material
{
  private:
	static std::unordered_map<std::string, GLuint> cachedShaders;

	std::unordered_map<std::string, GLuint> cachedUniforms;

	GLuint vertexID, fragmentID, programID;

	std::vector<std::shared_ptr<Texture>> textures;

  public:

	static int id;

  public:
	static GLuint getShader(const std::string&, int type);

	static std::shared_ptr<Material> create(const std::string& vertexPath, const std::string& fragmentPath);
	bool init(const std::string& vertexPath, const std::string& fragmentPath);

	void addTexture(std::shared_ptr<Texture> tex, uint32_t slot);

	std::vector<std::shared_ptr<Texture>>& getTextures()
	{
		return textures;
	}

	GLuint getProgram()
	{
		return programID;
	}

	GLuint getUniform(const std::string& name);

	// completely stolen from learnopengl

	void setBool(const std::string& name, bool value)
	{
		glUseProgram(this->getProgram());
		glUniform1i(getUniform(name), (int)value);
	}

	void setInt(const std::string& name, int value)
	{
		glUseProgram(this->getProgram());
		glUniform1i(getUniform(name), value);
	}

	void setFloat(const std::string& name, float value)
	{
		glUseProgram(this->getProgram());
		glUniform1f(getUniform(name), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value)
	{
		glUseProgram(this->getProgram());
		glUniform2fv(getUniform(name), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y)
	{
		glUseProgram(this->getProgram());
		glUniform2f(getUniform(name), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value)
	{
		glUseProgram(this->getProgram());
		glUniform3fv(getUniform(name), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z)
	{
		glUseProgram(this->getProgram());
		glUniform3f(getUniform(name), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value)
	{
		glUseProgram(this->getProgram());
		glUniform4fv(getUniform(name), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUseProgram(this->getProgram());
		glUniform4f(getUniform(name), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat)
	{
		glUseProgram(this->getProgram());
		glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& mat)
	{
		glUseProgram(this->getProgram());
		glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat)
	{
		glUseProgram(this->getProgram());
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
	}

	bool getBool(const std::string& name)
	{
		glUseProgram(this->getProgram());
		GLint value;
		glGetUniformiv(getProgram(), getUniform(name), &value);
		return value != 0;
	}

	int getInt(const std::string& name)
	{
		glUseProgram(this->getProgram());
		GLint value;
		glGetUniformiv(getProgram(), getUniform(name), &value);
		return value;
	}

	float getFloat(const std::string& name)
	{
		glUseProgram(this->getProgram());
		GLfloat value;
		glGetUniformfv(getProgram(), getUniform(name), &value);
		return value;
	}

	glm::vec2 getVec2(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::vec2 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0]);
		return value;
	}

	glm::vec3 getVec3(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::vec3 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0]);
		return value;
	}

	glm::vec4 getVec4(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::vec4 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0]);
		return value;
	}

	glm::mat2 getMat2(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::mat2 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0][0]);
		return value;
	}

	glm::mat3 getMat3(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::mat3 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0][0]);
		return value;
	}

	glm::mat4 getMat4(const std::string& name)
	{
		glUseProgram(this->getProgram());
		glm::mat4 value;
		glGetUniformfv(getProgram(), getUniform(name), &value[0][0]);
		return value;
	}
};