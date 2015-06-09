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
    
    auto pGame = new Game();
    auto pApplication = new Application();
    
    pApplication->Initialize(pGame);
    pApplication->Run();
    pApplication->Destroy();
    
    delete pApplication;
    delete pGame;
    
    return 0;
}
