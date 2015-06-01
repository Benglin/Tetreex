//
//  main.cpp
//  Tetreex
//
//  Created by BEN GOH on 6/1/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <iostream>
#include "SDL2/SDL.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Video initialization failed!\n";
        return -1;
    }
    
    SDL_Window* pWindow = SDL_CreateWindow("Tetreex v0.1",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480, SDL_WINDOW_SHOWN);
    
    if (pWindow == nullptr) {
        std::cout << "Failed to create window!\n";
        return -2;
    }

    SDL_Surface* pSurface = SDL_GetWindowSurface(pWindow);
    SDL_FillRect(pSurface, nullptr, SDL_MapRGB(pSurface->format, 0xff, 0x80, 0x00));
    
    SDL_UpdateWindowSurface(pWindow);
    SDL_Delay(2000);
    
    SDL_DestroyWindow(pWindow);
    
    pSurface = nullptr;
    pWindow = nullptr;
    
    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
    
    return 0;
}
