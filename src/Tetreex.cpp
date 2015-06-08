//
//  Tetreex.cpp
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "Tetreex.h"

using namespace Tetreex;

Application::Application() :
mpWindow(nullptr),
mpSurface(nullptr)
{
    
}

bool Application::InitializeCore()
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Video initialization failed!\n";
        return false;
    }
    
    mpWindow = SDL_CreateWindow("Tetreex v0.1",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                640, 480, SDL_WINDOW_SHOWN);
    
    if (mpWindow == nullptr) {
        std::cout << "Failed to create window!\n";
        return false;
    }
    
    return true;
}

int Application::RunCore()
{
    mpSurface = SDL_GetWindowSurface(mpWindow);
    SDL_FillRect(mpSurface, nullptr, SDL_MapRGB(mpSurface->format, 0xff, 0x80, 0x00));
    
    SDL_Rect rect = { 10, 10, 128, 64 };
    SDL_FillRect(mpSurface, &rect, SDL_MapRGB(mpSurface->format, 0x00, 0x80, 0xff));
    
    SDL_UpdateWindowSurface(mpWindow);
    SDL_Delay(2000);
    
    return 0;
}

void Application::DestroyCore()
{
    SDL_DestroyWindow(mpWindow);
    
    mpSurface = nullptr;
    mpWindow = nullptr;
    
    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
    
}
