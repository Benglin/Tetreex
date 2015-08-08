
#include "../inc/Tetreex.h"

using namespace Tetreex;

SplashScreen::SplashScreen(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas)
{
}

SplashScreen::~SplashScreen(void)
{
}

bool SplashScreen::UpdateFrame(double deltaTimeMs)
{
    return false; // No visual update required.
}
