#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <fmod.hpp>
#include <iostream>

typedef FMOD::Sound* SoundClass;
typedef FMOD::Channel* ChannelClass;

class SoundSystemClass {
private:
    FMOD::System* m_pSystem;

public:
    SoundSystemClass();
    ~SoundSystemClass();

    void createSound(SoundClass* pSound, const char* pFile);
    void playSound(SoundClass pSound, ChannelClass* pChannel, float volume = 1.0f, bool bLoop = false);
    void releaseSound(SoundClass pSound);
    void update();
};

#endif // SOUNDSYSTEM_H

