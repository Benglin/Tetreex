//
//  Tetreex.cpp
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#ifdef USE_SDL_RENDERER

// =============================================================================
// PixelBuffer class definition.
// =============================================================================

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
    
}

void PixelBuffer::Clear()
{
}

void PixelBuffer::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
}

void PixelBuffer::Present() const
{
    SDL_SetRenderDrawColor(mpRenderer, 0x80, 0xff, 0x40, 0xff);
    SDL_RenderClear(mpRenderer);
    
    SDL_Rect rect = { 10, 10, 128, 64 };
    SDL_SetRenderDrawColor(mpRenderer, 0x40, 0x80, 0xff, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
    
    SDL_RenderPresent(mpRenderer);
    SDL_Delay(2000);
}

#endif

// =============================================================================
// Game class definition.
// =============================================================================

Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas)
{
}

Game::~Game()
{
    delete mpCanvas;
    mpCanvas = nullptr;
}

void Game::UpdateFrame(void)
{
#ifdef USE_SDL_RENDERER
    
    auto pCanvas = ((PixelBuffer*) mpCanvas);
    pCanvas->Present(); // Only for SDL we need to present it.

#endif
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
    mpInternalGame->UpdateFrame();
    return 0;
}

void Application::Destroy()
{
    delete mpInternalGame;
    mpInternalGame = nullptr;
}
