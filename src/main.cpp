//
//  main.cpp
//  Tetreex
//
//  Created by BEN GOH on 6/1/15.
//  Copyright (c) 2015 BEN GOH. All rights reserved.
//

#include "../inc/Tetreex.h"

using namespace Tetreex;

int main(int argc, const char * argv[]) {

#ifdef USE_SDL_RENDERER

    auto pCanvas = new PixelBuffer(32, 32);

#else

    rgb_matrix::GPIO io;
    if (!io.Init())
        return 1;

    auto rows = 32;
    auto chain = 1;
    auto pCanvas = new rgb_matrix::RGBMatrix(&io, rows, chain);

#endif

    auto pGame = new Game(pCanvas);
    auto pApplication = new Application(pGame);

    pApplication->Initialize();
    pApplication->Run();
    pApplication->Destroy();

    // Both pGame will be deleted by Application::Destroy().
    // Likewise, pCanvas will be deleted by Game destructor.
    delete pApplication;
    return 0;
}
