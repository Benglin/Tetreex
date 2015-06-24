
#include <stdio.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#ifdef USE_SDL_RENDERER

const int PixelBuffer::OffsetShift = 5;
const int PixelBuffer::CellSize = 32;

PixelBuffer::PixelBuffer(int width, int height) :
mWidth(width),
mHeight(height),
mpRenderer(nullptr),
mpWindow(nullptr)
{
    // insert code here...
    std::cout << "Initializing SDL framework...\n";
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Video/audio initialization failed");
    
    mpWindow = SDL_CreateWindow("Tetreex v0.1",
                                32, 32, 640, 480, SDL_WINDOW_SHOWN);
    
    if (mpWindow == nullptr)
        throw std::runtime_error("Failed to create window");
    
    mpRenderer = SDL_CreateRenderer(mpWindow, -1, 0);
}

PixelBuffer::~PixelBuffer()
{
    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;
    mpRenderer = nullptr;
    
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
    SDL_Rect rect = { left, top, CellSize, CellSize };
    
    SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
}

void PixelBuffer::Clear()
{
    Fill(0x00, 0x00, 0x00); // Black.
}

void PixelBuffer::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
    const int totalWidth = mWidth * (1 << OffsetShift);
    const int totalHeight = mHeight * (1 << OffsetShift);
    SDL_Rect rect = { 0, 0, totalWidth, totalHeight };
    
    SDL_SetRenderDrawColor(mpRenderer, red, green, blue, 0xff);
    SDL_RenderFillRect(mpRenderer, &rect);
}

void PixelBuffer::Present() const
{
    SDL_RenderPresent(mpRenderer);
}

#endif

