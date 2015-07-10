
#include <stdio.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

const int AudioDevice::ChunkSize = 2048;

AudioDevice::AudioDevice() :
mpBackgroundMusic(nullptr)
{
#ifndef USE_SDL_RENDERER

    // If we are not set up to use SDL renderer, the SDL framework would not
    // have been initialized because PixelBuffer wouldn't have been invoked.
    // If that's the case, initialize audio subsystem here...
    //
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Audio initialization failed");

#endif

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, AudioDevice::ChunkSize) < 0)
    {
        std::cout << "SDL_mixer Error: %s\n" << Mix_GetError();
        throw std::runtime_error("SDL_mixer initialization failed");
    }
}

AudioDevice::~AudioDevice()
{
    if (mpBackgroundMusic != nullptr) {

        if (Mix_PlayingMusic() != 0)
            Mix_HaltMusic(); // Stop music if it is playing.

        Mix_FreeMusic(mpBackgroundMusic);
        mpBackgroundMusic = nullptr;
    }

    Mix_Quit();
}

bool AudioDevice::LoadMediaFiles()
{
    mpBackgroundMusic = Mix_LoadMUS("/home/pi/projects/Tetreex/audio/background.ogg");
    if (mpBackgroundMusic == nullptr)
    {
        std::cout << "Audio loading failed: background.ogg\n";
        return false;
    }

    return true;
}

void AudioDevice::PlayBackgroundMusic(bool play) const
{
    if (mpBackgroundMusic == nullptr)
        return;

    if (play && (Mix_PlayingMusic() == 0))
    {
        // There is no music playing...
        Mix_PlayMusic(mpBackgroundMusic, -1); // Loop till it is stopped.
    }
    else
    {
        if (Mix_PausedMusic() == 1) // Music is paused...
        {
            if (play)
                Mix_ResumeMusic();
        }
        else // Music is playing...
        {
            if (play == false)
                Mix_PauseMusic();
        }
    }
}
