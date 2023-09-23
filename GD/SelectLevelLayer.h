#pragma once

#include "Layer.h"
#include "Level.h"

class SelectLevelLayer : public Layer
{
private:
	std::vector<GameLevel> levels;

	char searchStr[50];
	char searchStrPersistent[50];

	int page = 1;
public:
	static std::shared_ptr<SelectLevelLayer> create();

	bool init();

	void searchLevels(std::string searchQuery, int page);

	virtual void draw() override;
};