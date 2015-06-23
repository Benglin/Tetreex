//
//  Tetreex.cpp
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#ifdef USE_SDL_RENDERER

// =============================================================================
// PixelBuffer class definition.
// =============================================================================

const int PixelBuffer::OffsetShift = 5;
const int PixelBuffer::CellSize = 32;

PixelBuffer::PixelBuffer(int width, int height) :
mWidth(width),
mHeight(height),
mpRenderer(nullptr),
mpWindow(nullptr)
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Video/audio initialization failed");

    mpWindow = SDL_CreateWindow("Tetreex v0.1",
                                32, 32, 640, 480, SDL_WINDOW_SHOWN);

    if (mpWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    mpRenderer = SDL_CreateRenderer(mpWindow, -1, 0);
}

PixelBuffer::~PixelBuffer()
{
    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;
    mpRenderer = nullptr;

    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
    std::cout << "SDL framework shut down\n";
}

int PixelBuffer::width() const
{
    return this->mWidth;
}

int PixelBuffer::height() const
{
    return this->mHeight;
}

void PixelBuffer::SetPixel(int x, int y,
                      uint8_t red, uint8_t green, uint8_t blue)
{
    const int left = x << OffsetShift;
    const int top = y << OffsetShift;
    SDL_Rect rect = { left, top, CellSize, CellSize };

    SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
}

void PixelBuffer::Clear()
{
    Fill(0x00, 0x00, 0x00); // Black.
}

void PixelBuffer::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
    const int totalWidth = mWidth * (1 << OffsetShift);
    const int totalHeight = mHeight * (1 << OffsetShift);
    SDL_Rect rect = { 0, 0, totalWidth, totalHeight };

    SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
}

void PixelBuffer::Present() const
{
    SDL_RenderPresent(mpRenderer);
}

#endif

// =============================================================================
// AudioDevice class definition.
// =============================================================================

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
    mpBackgroundMusic = Mix_LoadMUS("audio/background.ogg");
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

    if (Mix_PlayingMusic() == 0)
    {
        // There is no music playing...
        Mix_PlayMusic(mpBackgroundMusic, -1); // Loop till it is stopped.
    }
    else
    {
        if (Mix_PausedMusic() == 1)
            Mix_ResumeMusic();
        else
            Mix_PausedMusic();
    }
}

// =============================================================================
// Game class definition.
// =============================================================================

Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas),
mFrameCount(0),
mCurrentState(State::None),
mpAudioDevice(nullptr)
{
    mpAudioDevice = new AudioDevice();
    mpAudioDevice->LoadMediaFiles();
    mpAudioDevice->PlayBackgroundMusic(true);
}

Game::~Game()
{
    delete mpAudioDevice;
    mpAudioDevice = nullptr;

    delete mpCanvas;
    mpCanvas = nullptr;
}

void Game::HandleInput(Game::Input input)
{
    switch (input)
    {
        case Game::Input::Escape:
            mCurrentState = Game::State::Over;
            break;
            
        default:
            break;
    }
}

void Game::UpdateFrame(void)
{
    // If this is the first frame, clear buffer...
    if (mFrameCount == 0)
        mpCanvas->Clear();

    const int x = mFrameCount % mpCanvas->width();
    const int y = mFrameCount / mpCanvas->height();
    mpCanvas->SetPixel(x, y, 0x40, 0x80, 0xff);

    mFrameCount = mFrameCount + 1;
    if (mFrameCount >= 1024)
        mCurrentState = Game::State::Over;

#ifdef USE_SDL_RENDERER

    auto pCanvas = ((PixelBuffer*) mpCanvas);
    pCanvas->Present(); // Only for SDL we need to present it.

#endif
}

Game::State Game::CurrentState(void) const
{
    return this->mCurrentState;
}

// =============================================================================
// Application class definition.
// =============================================================================

Application::Application(Game* pGame) :
mpInternalGame(pGame)
{
    if (mpInternalGame == nullptr)
        throw new std::runtime_error("Invalid 'Game *' for 'Application'");
}

bool Application::Initialize()
{
    return true;
}

int Application::Run()
{
    while (mpInternalGame->CurrentState() != Game::State::Over)
    {
        mpInternalGame->UpdateFrame(); // Update frame till the game's over.
        SDL_Delay(500);
        // std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    return 0;
}

void Application::Destroy()
{
    delete mpInternalGame;
    mpInternalGame = nullptr;
}
