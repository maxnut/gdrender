#include "ReplayPlayer.h"
#include "GameLayer.h"

#include "Application.h"

void ReplayPlayer::load(std::string path)
{
	hasLoaded = replay.load(path);
	actionIndex = 0;

	if (!hasLoaded)
		return;

	Application::instance->framerate = replay.fps;
	Application::instance->lockDelta = true;
}

void ReplayPlayer::update()
{
	if (!hasLoaded || actionIndex + 1 >= replay.frames.size())
		return;

	std::shared_ptr<Sprite> player = GameLayer::instance->playerSpriteTemp;
	FrameCapture* frame = &replay.frames[actionIndex];
	if (frame->player2 && actionIndex + 1 < replay.frames.size())
		frame = &replay.frames[++actionIndex];

	player->setPosition(frame->px, frame->py - 90);
	player->setRotation(-frame->rot);
	player->updateVerticesPosition();
	actionIndex++;
}