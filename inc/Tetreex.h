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

        void Move(Direction direction);
        void Rotate(Rotation rotation);
        void FuseOnBoard(void) const;

    private:

        static const Mold Molds[];

        void Initialize(void);
        void Clear(void) const;
        void Draw(void) const;

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
        unsigned int* ContentAt(int x, int y) const;

        bool AdvanceTetromino(void);
        void AddTetromino(Tetromino* pTetromino);
        void RefreshRegion(int x, int y, int width, int height) const;
        
    private:
        int mWidth, mHeight;
        unsigned int* mpContent;
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
            Top,
            Right,
            Bottom,
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
        
        int mFrameCount;

        rgb_matrix::Canvas* mpCanvas;
        State mCurrentState;
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
