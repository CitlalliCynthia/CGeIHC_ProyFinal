#include "Sound.h"
#include <fmod_errors.h>

SoundSystemClass::SoundSystemClass() : m_pSystem(nullptr) {
    if (FMOD::System_Create(&m_pSystem) != FMOD_OK) {
        std::cerr << "Error: No se pudo crear el sistema FMOD.\n";
        return;
    }

    int driverCount = 0;
    m_pSystem->getNumDrivers(&driverCount);
    if (driverCount == 0) {
        std::cerr << "Error: No hay drivers de audio disponibles.\n";
        return;
    }

    if (m_pSystem->init(36, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) {
        std::cerr << "Error: No se pudo inicializar FMOD.\n";
    }
}

SoundSystemClass::~SoundSystemClass() {
    if (m_pSystem) {
        m_pSystem->release();
    }
}

void SoundSystemClass::createSound(SoundClass* pSound, const char* pFile) {
    if (m_pSystem->createSound(pFile, FMOD_DEFAULT, nullptr, pSound) != FMOD_OK) {
        std::cerr << "Error al cargar el sonido: " << pFile << std::endl;
    }
}

void SoundSystemClass::playSound(SoundClass pSound, ChannelClass* pChannel, float volume, bool bLoop) {
    if (!pSound) return;

    if (!bLoop) {
        pSound->setMode(FMOD_LOOP_OFF);
    }
    else {
        pSound->setMode(FMOD_LOOP_NORMAL);
        pSound->setLoopCount(-1);
    }

    // Reproducir el sonido y asignar el canal
    m_pSystem->playSound(pSound, nullptr, false, pChannel);

    // Llamar a update inmediatamente después de la reproducción
    m_pSystem->update();

    // Ajustar el volumen del canal al valor deseado
    if (*pChannel) {
        (*pChannel)->setVolume(volume);
    }
}

void SoundSystemClass::releaseSound(SoundClass pSound) {
    if (pSound) {
        pSound->release();
    }
}

void SoundSystemClass::update() {
    if (m_pSystem) {
        m_pSystem->update();
    }
}
