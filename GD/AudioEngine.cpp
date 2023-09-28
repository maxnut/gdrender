#include "AudioEngine.h"
#include "raudio.h"

AudioEngine::~AudioEngine() {
    if (isPlaying) {
        PauseMusicStream(music);
    }

    if (IsMusicReady(music)) {
        UnloadMusicStream(music);
    }
}

std::optional<AudioEngine> AudioEngine::create()
{
    AudioEngine ret;
    return ret.init() ? std::make_optional<AudioEngine>(ret) : std::nullopt;
}

bool AudioEngine::init() {
    if (IsAudioDeviceReady()) return true;

    InitAudioDevice();

    return IsAudioDeviceReady();
}

void AudioEngine::update() {
    if (isPlaying) {
        UpdateMusicStream(music);
    }
}

bool AudioEngine::loadAudio(const char* filePath) {
    if (IsMusicReady(music))
    {
        UnloadMusicStream(music);
    }
    music = LoadMusicStream(filePath);
    return IsMusicReady(music);
}

void AudioEngine::play() {
    if (!IsMusicStreamPlaying(music) && IsMusicReady(music)) {
        PlayMusicStream(music);
        isPlaying = true;
    }
}

void AudioEngine::pause() {
    if (IsMusicStreamPlaying(music)) {
        PauseMusicStream(music);
        isPlaying = false;
    }
}

void AudioEngine::resume() {
    if (!IsMusicStreamPlaying(music)) {
        ResumeMusicStream(music);
        isPlaying = true;
    }
}

void AudioEngine::setPosition(float seconds) {
    SeekMusicStream(music, seconds);
}

void AudioEngine::setVolume(float volume) {
    SetMusicVolume(music, volume);
}