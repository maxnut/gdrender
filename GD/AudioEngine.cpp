#include "AudioEngine.h"

AudioEngine::~AudioEngine() {
    if (system) {
        system->close();
        system->release();
    }
}

std::shared_ptr<AudioEngine> AudioEngine::create()
{
    std::shared_ptr<AudioEngine> ptr(new AudioEngine);

    if (ptr->init())
        return ptr;

    return nullptr;
}

bool AudioEngine::init() {
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
        return false;
    }

    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        return false;
    }

    return true;
}

void AudioEngine::update() {
    if (system) {
        system->update();
    }
}

bool AudioEngine::loadAudio(const char* filePath) {
    if (!system) {
        return false;
    }

    FMOD_RESULT result = system->createStream(filePath, FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK) {
        return false;
    }

    return true;
}

void AudioEngine::play() {
    if (sound && !channel) {
        system->playSound(sound, nullptr, false, &channel);
    }
}

void AudioEngine::pause() {
    if (channel) {
        channel->setPaused(true);
    }
}

void AudioEngine::resume() {
    if (channel) {
        channel->setPaused(false);
    }
}

void AudioEngine::setPosition(float seconds) {
    if (channel) {
        unsigned int position = static_cast<unsigned int>(seconds * 1000);
        channel->setPosition(position, FMOD_TIMEUNIT_MS);
    }
}

void AudioEngine::setVolume(float volume) {
    if (channel) {
        channel->setVolume(volume);
    }
}