#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

#include <iostream>

class AudioEngine {
private:
    FMOD::System* system;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
public:
    bool isPlaying;
public:
    
    ~AudioEngine();

    static std::shared_ptr<AudioEngine> create();

    bool init();
    void update();
    bool loadAudio(const char* filePath);
    void play();
    void pause();
    void resume();
    void setVolume(float volume);
    void setPosition(float seconds);
};