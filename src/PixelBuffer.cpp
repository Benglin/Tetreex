
#include <stdio.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#ifdef USE_SDL_RENDERER

const int PixelBuffer::OffsetShift = 4;
const int PixelBuffer::CellSize = 16;

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

    auto windowWidth = width * CellSize;
    auto windowHeight = height * CellSize;
    mpWindow = SDL_CreateWindow("Tetreex v0.1", 32, 32,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if (mpWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    mpRenderer = SDL_CreateRenderer(mpWindow, -1, 0);
    mpTexture = SDL_CreateTexture(mpRenderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

    mpInternalBuffer = new unsigned int[windowWidth * windowHeight];
    memset(mpInternalBuffer, 0x00, windowWidth * windowHeight * sizeof(unsigned int));
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
    const int left = x << OffsetShift;
    const int top = y << OffsetShift;

    const auto color = 0xff000000 |
        ((red & 0x000000ff) |
         ((green & 0x000000ff) << 8) |
         ((blue & 0x000000ff) << 16));

    const auto pitch = mWidth * CellSize;
    auto row = &mpInternalBuffer[(top * mWidth) + left];
    
    for (int l = 0; l < CellSize; l++, row += pitch) {
        auto writePointer = row;
        for (int t = 0; t < CellSize; t++) {
            *writePointer++ = color;
        }
    }

    // SDL_Rect rect = { left, top, CellSize, CellSize };
    // SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    // SDL_RenderFillRect(mpRenderer, &rect);
}

void PixelBuffer::Clear()
{
    Fill(0x00, 0x00, 0x00); // Black.
}

void PixelBuffer::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
    /*
    const int totalWidth = mWidth * (1 << OffsetShift);
    const int totalHeight = mHeight * (1 << OffsetShift);
    SDL_Rect rect = { 0, 0, totalWidth, totalHeight };
    
    SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
    */
}

void PixelBuffer::Present() const
{
    SDL_UpdateTexture(mpTexture, nullptr, mpInternalBuffer,
                      mWidth * CellSize * sizeof(unsigned int));

    SDL_RenderClear(mpRenderer);
    SDL_RenderCopy(mpRenderer, mpTexture, nullptr, nullptr);
    SDL_RenderPresent(mpRenderer);
}

#endif

