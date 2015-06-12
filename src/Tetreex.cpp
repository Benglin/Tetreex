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
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Video initialization failed");

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
// Game class definition.
// =============================================================================

Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas),
mFrameCount(0),
mCurrentState(State::None)
{
}

Game::~Game()
{
    delete mpCanvas;
    mpCanvas = nullptr;
}

void Game::UpdateFrame(void)
{
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
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    
    return 0;
}

void Application::Destroy()
{
    delete mpInternalGame;
    mpInternalGame = nullptr;
}
