
#include "../inc/Tetreex.h"
#include <iostream>

using namespace Tetreex;

SplashScreen::SplashScreen(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas)
{
    std::cout << "Initializing splash screen...";

    //Initialize PNG loading
    int imageFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imageFlags) & imageFlags)) {
        std::cout << "Failed to initialize SDL image";
        return;
    }

#ifdef __APPLE__
    auto splashScreenPath = "images/splash-screen.png";
#else
    auto splashScreenPath = "/home/pi/projects/Tetreex/images/splash-screen.png";
#endif

    mpSurface = IMG_Load(splashScreenPath);
    if (mpSurface == nullptr) {
        std::cout << "Failed to load splash screen image";
        return;
    }
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

    return false; // No visual update required.
}
