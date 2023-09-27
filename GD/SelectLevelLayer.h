#pragma once

#include "Layer.h"
#include "Level.h"

#include <array>

class SelectLevelLayer : public Layer
{
private:
	std::vector<GameLevel> levels;

	std::array<char, 50> searchStr;
	std::array<char, 50> searchStrPersistent;

	int page = 1;
public:
	static std::shared_ptr<SelectLevelLayer> create();

	bool init();

	void searchLevels(std::string searchQuery, int page);

	virtual void draw() override;
};