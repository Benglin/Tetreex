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
    
    class IApplication
    {
    public:
        bool Initialize(Game* pGame)
        {
            mpInternalGame = pGame;
            return InitializeCore();
        }
        
        int Run()
        {
            return RunCore();
        }
        
        void Destroy()
        {
            DestroyCore();
        }
        
    protected:
        virtual bool InitializeCore() = 0;
        virtual int  RunCore() = 0;
        virtual void DestroyCore() = 0;
        
        Game* GetInternalGame() const
        {
            return this->mpInternalGame;
        }
        
    private:
        Game* mpInternalGame;
    };
    
#ifdef __APPLE__
    
    class Application : public IApplication
    {
    public:
        Application();
        
    protected:
        bool InitializeCore();
        int  RunCore();
        void DestroyCore();
        
    private:
        SDL_Window* mpWindow;
        SDL_Surface* mpSurface;
    };
    
#else
    
    class Application : public IApplication
    {
    public:
        Application();
        
    protected:
        bool InitializeCore();
        int  RunCore();
        void DestroyCore();
        
    private:
    };

#endif
    
    
}


#endif
