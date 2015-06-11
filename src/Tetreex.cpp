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
mpSurface(nullptr),
mpInternalGame(pGame)
{
#ifdef __APPLE__
    
    mpWindow = nullptr;
    
#endif
}

bool Application::Initialize()
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Video initialization failed!\n";
        return false;
    }

#ifdef __APPLE__

    mpWindow = SDL_CreateWindow("Tetreex v0.1",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                640, 480, SDL_WINDOW_SHOWN);

    if (mpWindow == nullptr) {
        std::cout << "Failed to create window!\n";
        return false;
    }

    mpSurface = SDL_GetWindowSurface(mpWindow);

#else

    
    const auto pVideoInfo = SDL_GetVideoInfo();
    
    auto systemX = pVideoInfo->current_w ;
    auto systemY = pVideoInfo->current_h ;
    auto bpp = pVideoInfo->vfmt->BitsPerPixel ;
    
    //Set up screen
    mpSurface = SDL_SetVideoMode( systemX, systemY, bpp, SDL_SWSURFACE );
    if (mpSurface == nullptr)
    {
        std::count << "SDL_SetVideoMode failed\n";
        return 0;
    }
    
#endif

    return true;
}

int Application::Run()
{
    SDL_FillRect(mpSurface, nullptr, SDL_MapRGB(mpSurface->format, 0xff, 0x80, 0x00));

    SDL_Rect rect = { 10, 10, 128, 64 };
    SDL_FillRect(mpSurface, &rect, SDL_MapRGB(mpSurface->format, 0x00, 0x80, 0xff));

#ifdef __APPLE__

    SDL_UpdateWindowSurface(mpWindow);
    
#else
    
    SDL_Flip(mpSurface);

#endif

    SDL_Delay(2000);
    return 0;
}

void Application::Destroy()
{
#ifdef __APPLE__

    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;

#endif
    
    mpSurface = nullptr;
    
    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
}
