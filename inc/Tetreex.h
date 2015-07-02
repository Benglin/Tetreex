//
//  Tetreex.h
//  Tetreex
//
//  Created by BEN GOH on 6/8/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include <stdexcept>
#include <random>
#include "SDL2/SDL.h"

#ifdef __APPLE__
#include "SDL2_mixer/SDL_mixer.h"
#else
#include "SDL_mixer.h"
#endif

#include "../matrix/include/led-matrix.h"

#ifndef Tetreex_Tetreex_h
#define Tetreex_Tetreex_h

#define USE_GPIO_INPUTS

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

        void Rotate(bool clockwise);

    public:

        int mBoundingSize;
        unsigned int mColor; // 0xAaBbGgRr

        int mMargins[4];
        unsigned int mMasks[4][4];

    private:

        void Rotate2x2(bool clockwise);
        void Rotate3x3(bool clockwise);
        void Rotate4x4(bool clockwise);
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

        Tetromino(Board* pBoard, Tetromino::Type type);

        bool CanMove(Direction direction) const;
        bool CanRotate(Rotation rotation) const;

        bool Move(Direction direction);
        bool Rotate(Rotation rotation);
        void FuseOnBoard(void) const;

    private:

        static const Mold Molds[];

        void Initialize(void);
        void Clear(void) const;
        void Draw(unsigned int color, bool permanent) const;

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
        bool IsPlacementPossible(int x, int y, const Mold& mold) const;
        bool HasActiveTetromino(void) const;
        void FuseActiveTetromino(void);
        void SetColor(int x, int y, unsigned int color, bool permanent);

        bool AdvanceTetromino(void);
        bool MoveTetromino(Tetromino::Direction direction);
        bool RotateTetromino(Tetromino::Rotation rotation);
        void GenerateTetromino(void);
        void RefreshRegion(int x, int y, int width, int height) const;
        
    private:

        unsigned int* PixelAt(int x, int y) const;
        unsigned int* PixelAtUnsafe(int x, int y) const;

        int mWidth, mHeight;
        unsigned int* mpContent;
        std::default_random_engine mGenerator;
        std::uniform_int_distribution<int> mDistribution;
        
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
        void RenderFrame(void);
        State CurrentState(void) const;

    private:

        static const double MinFrameTime;
        static const double DropInterval;

        double mPrevDropTime;
        double mDropInterval;
        bool mVisualInvalidated;

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

#ifdef USE_GPIO_INPUTS

        int GpioExport(int pin);
        int GpioUnexport(int pin);
        int GpioDirection(int pin, int direction);
        int GpioRead(int pin);

        static const int PinBufferSize;
        static const int DirBufferSize;
        static const int IoBufferSize;
        static const int DirectionIn;
        static const int DirectionOut;

        // GPIO for inputs
        static const int Gpio07;
        static const int Gpio08;
        static const int Gpio09;
        static const int Gpio10;
        static const int Gpio11;
        static const int Gpio24;
        static const int Gpio25;
        static const int PinCount;

        int mPinStates[7];
#endif

        Game* mpInternalGame;
    };
}


#endif
