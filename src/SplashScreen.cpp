
#include "../inc/Tetreex.h"
#include <iostream>

using namespace Tetreex;

const int SplashScreen::DisplayWidth = 32;
const double SplashScreen::ScrollIntervalMs = 32.0;

SplashScreen::SplashScreen(rgb_matrix::Canvas* pCanvas) :
mCurrentWindow(-DisplayWidth),
mAccumTimeDiff(0.0),
mpCanvas(pCanvas)
{
    std::cout << "Initializing splash screen...\n";

    //Initialize PNG loading
    int imageFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imageFlags) & imageFlags)) {
        std::cout << "Failed to initialize SDL image\n";
        return;
    }

#ifdef __APPLE__
    auto splashScreenPath = "images/splash-screen.png";
#else
    auto splashScreenPath = "/home/pi/projects/Tetreex/images/splash-screen.png";
#endif

    mpSurface = IMG_Load(splashScreenPath);
    if (mpSurface == nullptr) {
        std::cout << "Failed to load splash screen image\n";
        return;
    }

    std::cout << "Splash screen successfully initialized\n";
}

SplashScreen::~SplashScreen(void)
{
    if (mpSurface != nullptr) {
        SDL_FreeSurface(mpSurface);
        mpSurface = nullptr;
    }
}

void SplashScreen::ResetScrollPosition(void)
{
    mAccumTimeDiff = 0.0;
    mCurrentWindow = -DisplayWidth;
}

bool SplashScreen::UpdateFrame(double deltaTimeMs)
{
    if (mpSurface == nullptr)
        return false; // There was a problem with loading.

    mAccumTimeDiff = mAccumTimeDiff + deltaTimeMs;
    if (mAccumTimeDiff < ScrollIntervalMs)
        return false; // No visual update required.

    mAccumTimeDiff = mAccumTimeDiff - ScrollIntervalMs;
    if (mAccumTimeDiff > ScrollIntervalMs)
        mAccumTimeDiff = ScrollIntervalMs;

    // Move visible window by 1 pixel.
    mCurrentWindow = mCurrentWindow + 1;
    if (mCurrentWindow >= mpSurface->w)
        mCurrentWindow = -DisplayWidth;

    // Calculate number of visible columns.
    auto startX = 0;
    auto visibleColumns = DisplayWidth;
    if (mCurrentWindow < 0)
    {
        startX = -mCurrentWindow;
        visibleColumns = DisplayWidth + mCurrentWindow;
        visibleColumns = visibleColumns < 0 ? 0 : visibleColumns;
    }
    else
    {
        startX = 0;
        visibleColumns = mpSurface->w - mCurrentWindow;
        visibleColumns = visibleColumns > DisplayWidth ?
            DisplayWidth : visibleColumns;
    }

    auto pRowPointer = ((unsigned char *) mpSurface->pixels);
    auto offset = mCurrentWindow < 0 ? 0 : mCurrentWindow;
    pRowPointer = pRowPointer + offset * 4;

    for (auto y = 0; y < DisplayWidth; y++, pRowPointer += mpSurface->pitch)
    {
        auto p = pRowPointer;
        for (auto x = 0; x < visibleColumns; x++, p += 4)
        {
#ifdef __APPLE__
            mpCanvas->SetPixel(x + startX, y, p[2], p[1], p[0]);
#else
            mpCanvas->SetPixel(x + startX, y, p[0], p[1], p[2]);
#endif
        }
    }

    return true;
}
