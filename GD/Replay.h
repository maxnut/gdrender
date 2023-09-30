#pragma once

#include <iostream>
#include <vector>

struct FrameCapture
{
	bool player2;
	uint32_t frame;
	double yAccel;
	float px, py, rot;
};

class Replay
{
  public:
	float fps = 0.f;
	std::vector<FrameCapture> frames;

  public:
	bool load(std::string name);
};