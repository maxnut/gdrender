#include "Material.h"

#include "Common.h"

int Material::id;

std::unordered_map<std::string, GLuint> Material::cachedShaders;

GLuint Material::getShader(const std::string& path, int type)
{
	if (Material::cachedShaders.contains(path))
		return Material::cachedShaders[path];

	Material::cachedShaders.insert({path, Common::parseShader(path, type)});

	return Material::cachedShaders[path];
}

std::shared_ptr<Material> Material::create(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::shared_ptr<Material> ptr = std::make_shared<Material>();
	if (ptr->init(vertexPath, fragmentPath))
		return ptr;

	return nullptr;
}

bool Material::init(const std::string& vertexPath, const std::string& fragmentPath)
{
	this->vertexID = Material::getShader(vertexPath, GL_VERTEX_SHADER);
	this->fragmentID = Material::getShader(fragmentPath, GL_FRAGMENT_SHADER);

	this->programID = glCreateProgram();
	glAttachShader(this->programID, this->vertexID);
	glAttachShader(this->programID, this->fragmentID);

	glBindAttribLocation(this->programID, 0, "vertex_position");
	glBindAttribLocation(this->programID, 1, "vertex_color");
	glBindAttribLocation(this->programID, 2, "vertex_texCoord");

	glLinkProgram(this->programID);

	unsigned int uniformBlockMatrices = glGetUniformBlockIndex(this->programID, "Camera");
	glUniformBlockBinding(this->programID, uniformBlockMatrices, 0);

	id++;

	return true;
}

void Material::addTexture(std::shared_ptr<Texture> tex, uint32_t slot)
{
	this->textures.push_back(tex);
	tex->textureSlot = slot;
}

GLuint Material::getUniform(const std::string& name)
{
	if (this->cachedUniforms.contains(name))
		return this->cachedUniforms[name];

	GLuint uniform = glGetUniformLocation(programID, name.c_str());

	this->cachedUniforms.insert({name, uniform});
	return uniform;
}