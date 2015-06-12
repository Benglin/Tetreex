//
//  Tetreex.h
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include "SDL2/SDL.h"

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

        SDL_Renderer* mpRenderer;
        Game* mpInternalGame;
        SDL_Window* mpWindow;
    };
}


#endif
