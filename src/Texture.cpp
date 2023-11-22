#include "Texture.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::cachedTextures;

std::shared_ptr<Texture> Texture::create(const std::string& path)
{
	if (cachedTextures.contains(path))
		return cachedTextures[path];

	std::shared_ptr<Texture> ptr = std::make_shared<Texture>();

	if (ptr->init(path))
	{
		cachedTextures.insert({path, ptr});
		return ptr;
	}

	return nullptr;
}

bool Texture::init(const std::string& path)
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		int format = nrChannels == 3 ? GL_RGB : GL_RGBA;

		resolution = {width, height};

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		stbi_image_free(data);
		return false;
	}

	stbi_image_free(data);

	return true;
}