
#include "../inc/Tetreex.h"
#include <iostream>

using namespace Tetreex;

const double SplashScreen::ScrollIntervalMs = 40.0;

SplashScreen::SplashScreen(rgb_matrix::Canvas* pCanvas) :
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

    auto pRowPointer = ((unsigned char *) mpSurface->pixels);

    for (auto y = 0; y < 32; y++, pRowPointer += mpSurface->pitch)
    {
        auto p = pRowPointer;
        for (auto x = 0; x < 32; x++, p += 4)
        {
            mpCanvas->SetPixel(x, y, p[2], p[1], p[0]);
        }
    }

    return true;
}
