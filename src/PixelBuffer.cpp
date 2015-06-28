
#include <stdio.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#define MAKECOLOR(r, g, b)  (((r & 0x000000ff) << 16) | \
                             ((g & 0x000000ff) << 8) |  \
                             (b & 0x000000ff) |         \
                             0xff000000)

#ifdef USE_SDL_RENDERER

const int PixelBuffer::ScaleFactor = 16;

PixelBuffer::PixelBuffer(int width, int height) :
mWidth(width),
mHeight(height),
mpInternalBuffer(nullptr),
mpTexture(nullptr),
mpRenderer(nullptr),
mpWindow(nullptr)
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Video/audio initialization failed");

    mpWindow = SDL_CreateWindow("Tetreex v0.1", 32, 32,
        width * ScaleFactor, height * ScaleFactor, SDL_WINDOW_SHOWN);

    if (mpWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    mpRenderer = SDL_CreateRenderer(mpWindow, -1, 0);
    mpTexture = SDL_CreateTexture(mpRenderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);

    SDL_RenderSetScale(mpRenderer, ((float)ScaleFactor), ((float)ScaleFactor));

    mpInternalBuffer = new unsigned int[mWidth * mHeight];
    memset(mpInternalBuffer, 0x00, mWidth * mHeight * sizeof(unsigned int));
}

PixelBuffer::~PixelBuffer()
{
    SDL_DestroyTexture(mpTexture);
    SDL_DestroyRenderer(mpRenderer);
    SDL_DestroyWindow(mpWindow);

    mpWindow = nullptr;
    mpTexture = nullptr;
    mpRenderer = nullptr;

    delete [] mpInternalBuffer;
    mpInternalBuffer = nullptr;

    std::cout << "Shutting down SDL framework...\n";
    SDL_Quit();
    std::cout << "SDL framework shut down\n";
}

int PixelBuffer::width() const
{
    return this->mWidth;
}

int PixelBuffer::height() const
{
    return this->mHeight;
}

void PixelBuffer::SetPixel(int x, int y,
                           uint8_t red, uint8_t green, uint8_t blue)
{
    const auto color = MAKECOLOR(red, green, blue);
    mpInternalBuffer[(y * mWidth) + x] = color;
}

void PixelBuffer::Clear()
{
    Fill(0x00, 0x00, 0x00); // Black.
}

void PixelBuffer::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
    const auto color = MAKECOLOR(red, green, blue);
    auto writePointer = &mpInternalBuffer[0];

    auto pixels = mWidth * mHeight;
    for (int i = 0; i < pixels; i++) {
        *writePointer++ = color;
    }
}

void PixelBuffer::Present() const
{
    SDL_UpdateTexture(mpTexture, nullptr, mpInternalBuffer,
        mWidth * sizeof(unsigned int));

    SDL_RenderClear(mpRenderer);
    SDL_RenderCopy(mpRenderer, mpTexture, nullptr, nullptr);
    SDL_RenderPresent(mpRenderer);
}

#endif

