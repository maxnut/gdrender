#pragma once

#include "raudio.h"
#include <optional>
#include <filesystem>

class AudioEngine
{
private:
	Music music = {0};

public:
	bool isPlaying = false;

	~AudioEngine();

	static std::optional<AudioEngine> create();

	bool init();
	void update();
	bool loadAudio(const std::filesystem::path& filePath);
	void play();
	void pause();
	void resume();
	void setVolume(float volume);
	void setPosition(float seconds);
};
