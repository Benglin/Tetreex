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

Application::Application(Game* pGame) :
mpRenderer(nullptr),
mpWindow(nullptr),
mpInternalGame(pGame)
{
}

bool Application::Initialize()
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Video initialization failed!\n";
        return false;
    }

    mpWindow = SDL_CreateWindow("Tetreex v0.1",
                                32, 32, 640, 480, SDL_WINDOW_SHOWN);
    
    if (mpWindow == nullptr) {
        std::cout << "Failed to create window!\n";
        return false;
    }
    
    mpRenderer = SDL_CreateRenderer(mpWindow, -1, 0);
    return true;
}

int Application::Run()
{
    SDL_SetRenderDrawColor(mpRenderer, 0x80, 0xff, 0x40, 0xff);
    SDL_RenderClear(mpRenderer);

    SDL_Rect rect = { 10, 10, 128, 64 };
    SDL_SetRenderDrawColor(mpRenderer, 0x40, 0x80, 0xff, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);

    SDL_RenderPresent(mpRenderer);
    SDL_Delay(2000);
    return 0;
}

void Application::Destroy()
{
    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;
    mpRenderer = nullptr;

    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
}
