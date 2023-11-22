#include "QuadSprite.h"

std::shared_ptr<QuadSprite> QuadSprite::create()
{
	std::shared_ptr<QuadSprite> ptr = std::make_shared<QuadSprite>();

	if (ptr->init())
		return ptr;

	return nullptr;
}

void QuadSprite::draw()
{
	glBindVertexArray(VAO);
	glUseProgram(material->getProgram());

	for (auto& texture : material->getTextures())
	{
		glActiveTexture(GL_TEXTURE0 + texture->textureSlot);
		glBindTexture(GL_TEXTURE_2D, texture->getTexture());
	}

	material->setMat4("model", getTransform());
    material->setVec2("tex_res", {1, 1});
	material->setBool("blending", false);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));

	glBindVertexArray(0);
}

void QuadSprite::setColor(glm::vec4 color)
{
	for(auto&vert : vertices)
		vert.color = color;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool QuadSprite::init()
{
	material = Material::create("shaders\\vert.glsl", "shaders\\frag.glsl");

	material->setInt("base_texture", 0);

	vertices[0].position = glm::vec2(-1, -1);
	vertices[1].position = glm::vec2(1, -1);
	vertices[2].position = glm::vec2(1, 1);
	vertices[3].position = glm::vec2(-1, 1);
	vertices[0].texCoord = glm::vec2(0, 0);
	vertices[1].texCoord = glm::vec2(1, 0);
	vertices[2].texCoord = glm::vec2(1, 1);
	vertices[3].texCoord = glm::vec2(0, 1);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}