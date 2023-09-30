#pragma once

#include "Replay.h"

class ReplayPlayer
{
  public:
	Replay replay;
	bool hasLoaded = false;

    size_t actionIndex = 0;

  public:
	void load(std::string path);

    void update();
};