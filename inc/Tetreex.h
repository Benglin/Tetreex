//
//  Tetreex.h
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdexcept>
#include "SDL2/SDL.h"

#ifdef __APPLE__
#include "SDL2_mixer/SDL_mixer.h"
#else
#include "SDL_mixer.h"
#endif

#include "../matrix/include/graphics.h"

#ifndef Tetreex_Tetreex_h
#define Tetreex_Tetreex_h

namespace Tetreex
{
    class Board; // Forward declaration.

#ifdef USE_SDL_RENDERER

    class PixelBuffer : public rgb_matrix::Canvas
    {
    public:

        PixelBuffer(int width, int height);
        virtual ~PixelBuffer(void);

        virtual int width(void) const;
        virtual int height(void) const;

        virtual void SetPixel(int x, int y,
                              uint8_t red, uint8_t green, uint8_t blue);

        virtual void Clear(void); // Clear screen to be all black.
        virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

        void Present(void) const;

    private:

        static const int ScaleFactor;

        int mWidth, mHeight;
        unsigned int* mpInternalBuffer;
        SDL_Texture* mpTexture;
        SDL_Renderer* mpRenderer;
        SDL_Window* mpWindow;
    };

#endif

    class AudioDevice
    {
    public:
        AudioDevice(void);
        ~AudioDevice(void);

        bool LoadMediaFiles(void);
        void PlayBackgroundMusic(bool play) const;

    private:

        static const int ChunkSize;

        Mix_Music* mpBackgroundMusic;
    };
    
    struct Mold
    {
    public:
        
        int mBoundingSize;
        unsigned int mColor; // 0xAaBbGgRr
        unsigned int mBits[4][4];
    };

    class Tetromino
    {
    public:

        enum class Type : unsigned int
        {
            I, O, T, J, L, S, Z, Max
        };

        enum class Rotation : unsigned int
        {
            ClockWise, CounterClockWise
        };

        enum class Direction : unsigned int
        {
            Left, Right, Down
        };

        Tetromino(Board* pBoard);

        bool CanMove(Direction direction) const;
        bool CanRotate(Rotation rotation) const;

        bool Move(Direction direction);
        bool Rotate(Rotation rotation);
        void FuseOnBoard(void) const;

    private:

        static const Mold Molds[];

        void Initialize(void);
        void Clear(void) const;
        void Draw(unsigned int color) const;

        int mX, mY, mWidth, mHeight;
        Tetromino::Type mType;
        Mold mMoldData;
        Board* mpBoard;
    };

    class Board
    {
    public:
        Board(int width, int height, rgb_matrix::Canvas* pCanvas);
        ~Board();

        int Width(void) const;
        int Height(void) const;
        bool IsGameOver(void) const;
        bool HasActiveTetromino(void) const;
        void FuseActiveTetromino(void);
        void SetColor(int x, int y, unsigned int color);

        bool AdvanceTetromino(void);
        void MoveTetromino(Tetromino::Direction direction);
        void AddTetromino(Tetromino* pTetromino);
        void RefreshRegion(int x, int y, int width, int height) const;
        
    private:

        unsigned int* PixelAt(int x, int y) const;
        unsigned int* PixelAtUnsafe(int x, int y) const;

        int mWidth, mHeight;
        unsigned int* mpContent;
        
        Tetromino* mpActiveTetromino;
        rgb_matrix::Canvas* mpCanvas;
    };

    class Game
    {
    public:

        enum class State : unsigned int
        {
            None = 0,
            Over = 1
        };
        
        enum class Input : unsigned int
        {
            None = 0,
            Left,
            Up,
            Right,
            Down,
            Button0,
            Button1,
            Button2,
            Escape
        };

        Game(rgb_matrix::Canvas* pCanvas);
        ~Game(void);

        void HandleInput(Game::Input input);
        void UpdateFrame(void);
        State CurrentState(void) const;

    private:

        static const double MinFrameTime;
        static const double DropInterval;

        double mPrevDropTime;
        double mDropInterval;

        rgb_matrix::Canvas* mpCanvas;
        State mCurrentState;
        Board* mpBoard;
        AudioDevice* mpAudioDevice;
    };

    class Application
    {
    public:

        Application(Game* pGame);
        bool Initialize(void);
        int  Run(void);
        void Destroy(void);

    private:

        void ProcessInputEvents(void);

        Game* mpInternalGame;
    };
}


#endif
