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

    // The game is actually 16x16 blocks in dimension. In SDL mode, these blocks (pixels) are
    // blown up to display on-screen, so the dimension stays as 16x16. In LED matrix mode
    // however, the board is 32x32, which is why we need to initialize the canvas as 32x32
    // in size. Each of the pixels on the game board gets shown as 2x2 pixels on the matrix.
    //
#ifdef USE_SDL_RENDERER
    auto pCanvas = new PixelBuffer(16, 16);
#else
    auto pCanvas = new PixelBuffer(32, 32);
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
