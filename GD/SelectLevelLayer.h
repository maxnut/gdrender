#pragma once

#include "Layer.h"

class SelectLevelLayer : public Layer
{
public:
	static std::shared_ptr<SelectLevelLayer> create();

	bool init();

	virtual void draw() override;
};