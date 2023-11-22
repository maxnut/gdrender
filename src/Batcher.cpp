#include "Batcher.h"
#include "Common.h"
#include "GameObject.h"

std::shared_ptr<Batcher> Batcher::create(std::string texture, bool blend)
{
	std::shared_ptr<Batcher> ptr = std::make_shared<Batcher>();

	if (ptr->init(texture, blend))
		return ptr;

	return nullptr;
}

void Batcher::draw()
{
	if (needSend)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, minRange * sizeof(Vertex), (maxRange - minRange + 1) * sizeof(Vertex), &vertices[minRange]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		needSend = false;

		minRange = std::numeric_limits<size_t>::max();
		maxRange = 0;
	}

	if (blending)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(VAO);
	glUseProgram(material->getProgram());

	for (auto& texture : material->getTextures())
	{
		glActiveTexture(GL_TEXTURE0 + texture->textureSlot);
		glBindTexture(GL_TEXTURE_2D, texture->getTexture());
	}

	material->setVec2("tex_res", texRes);
	material->setBool("blending", blending);
	material->setMat4("model", getTransform());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Batcher::sendVertex(Vertex& vert, size_t index)
{
	vertices[index] = vert;
	needSend = true;

	minRange = std::min(minRange, index);
	maxRange = std::max(maxRange, index);
}

void Batcher::updateSprite(Sprite* sp)
{
	if (sp->currentBatcher != this)
		return;

	size_t atIndex = sp->atlasIndex * 4;
	for (int i = 0; i < 4; i++)
		vertices[atIndex + i] = sp->vertices[i];

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, atIndex * sizeof(Vertex), 4 * sizeof(Vertex), sp->vertices);
}

bool Batcher::init(std::string texture, bool blend)
{
	this->blending = blend;
	material = Material::create("shaders\\vert.glsl", "shaders\\frag.glsl");

	auto tex = Texture::create(texture);
	texRes = tex->resolution;
	material->setInt("base_texture", 0);
	material->addTexture(tex, 0);

	vertices.clear();
	indices.clear();

	glBlendEquation(GL_FUNC_ADD);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

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

void Batcher::addSprite(Sprite* sp)
{
	if (sp->currentBatcher != nullptr)
		return;

	sprites.insert({sp->uniqueID, sp});

	batcherDirty = true;

	sp->currentBatcher = this;
}

void Batcher::removeSprite(Sprite* sp)
{
	if (batcherDirty)
		return;

	sp->setOpacity(0);

	sprites.unordered_erase(sp->uniqueID);

	sp->currentBatcher = nullptr;
}

void Batcher::generateVertices()
{
	vertices.clear();
	indices.clear();

	auto data = sprites.values_container();

	std::sort(data.begin(), data.end(), [](const auto& x, const auto& y) {
		if (!x.second->parent || !y.second->parent)
			return false;

		return x.second->parent->zOrder < y.second->parent->zOrder;
	});

	size_t i = 0;
	size_t triangleIndex = 0;

	for (auto& pair : data)
	{
		Sprite* sprite = pair.second;
		sprite->atlasIndex = i;
		vertices.push_back(sprite->vertices[0]);
		vertices.push_back(sprite->vertices[1]);
		vertices.push_back(sprite->vertices[2]);
		vertices.push_back(sprite->vertices[3]);

		indices.push_back(static_cast<uint32_t>(triangleIndex));
		indices.push_back(static_cast<uint32_t>(triangleIndex + 1));
		indices.push_back(static_cast<uint32_t>(triangleIndex + 2));

		indices.push_back(static_cast<uint32_t>(triangleIndex + 2));
		indices.push_back(static_cast<uint32_t>(triangleIndex + 3));
		indices.push_back(static_cast<uint32_t>(triangleIndex));

		triangleIndex += 4;
		i++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	needSend = false;

	minRange = std::numeric_limits<size_t>::max();
	maxRange = 0;

	batcherDirty = false;
}
