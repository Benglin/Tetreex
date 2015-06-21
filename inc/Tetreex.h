//
//  Tetreex.h
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdexcept>
#include "SDL2/SDL.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "../matrix/include/canvas.h"

#ifndef Tetreex_Tetreex_h
#define Tetreex_Tetreex_h

namespace Tetreex
{
#ifdef USE_SDL_RENDERER

    class PixelBuffer : public rgb_matrix::Canvas
    {
    public:

        PixelBuffer(int width, int height);
        virtual ~PixelBuffer();

        virtual int width() const;
        virtual int height() const;

        virtual void SetPixel(int x, int y,
                              uint8_t red, uint8_t green, uint8_t blue);

        virtual void Clear(); // Clear screen to be all black.
        virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

        void Present() const;

    private:
        
        static const int OffsetShift;
        static const int CellSize;

        int mWidth, mHeight;
        SDL_Renderer* mpRenderer;
        SDL_Window* mpWindow;
    };

#endif

    class AudioDevice
    {
    public:
        AudioDevice();
        ~AudioDevice();

        bool LoadMediaFiles();
        void PlayBackgroundMusic(bool play) const;

    private:

        static const int ChunkSize;

        Mix_Music* mpBackgroundMusic;
    };

    class Game
    {
    public:
        
        enum class State : unsigned int
        {
            None = 0,
            Over = 1
        };

        Game(rgb_matrix::Canvas* pCanvas);
        ~Game();

        void UpdateFrame(void);
        State CurrentState(void) const;

    private:
        
        int mFrameCount;

        rgb_matrix::Canvas* mpCanvas;
        State mCurrentState;
        AudioDevice* mpAudioDevice;
    };

    class Application
    {
    public:

        Application(Game* pGame);
        bool Initialize();
        int  Run();
        void Destroy();

    private:

        Game* mpInternalGame;
    };
}


#endif
