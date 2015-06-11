//
//  Tetreex.h
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include "SDL2/SDL.h"

#define __APPLE__ 1

#ifndef Tetreex_Tetreex_h
#define Tetreex_Tetreex_h

namespace Tetreex
{
    class Game
    {
    };

    class Application
    {
    public:

        Application(Game* pGame);
        bool Initialize();
        int  Run();
        void Destroy();

    private:

#ifdef __APPLE__

        SDL_Window* mpWindow; // Only desktop has concept of windows.

#endif

        SDL_Renderer* mpRenderer;
        Game* mpInternalGame;
    };
}


#endif
