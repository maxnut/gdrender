#pragma once

#include "raudio.h"
#include <optional>
class AudioEngine {
private:
    Music music = { 0 };

public:
    bool isPlaying = false;

    ~AudioEngine();

    static std::optional<AudioEngine> create();

    bool init();
    void update();
    bool loadAudio(const char* filePath);
    void play();
    void pause();
    void resume();
    void setVolume(float volume);
    void setPosition(float seconds);
};