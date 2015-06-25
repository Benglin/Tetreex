
#include "../inc/Tetreex.h"

using namespace Tetreex;


Application::Application(Game* pGame) :
mpInternalGame(pGame)
{
    if (mpInternalGame == nullptr)
        throw new std::runtime_error("Invalid 'Game *' for 'Application'");
}

bool Application::Initialize()
{
    return true;
}

int Application::Run()
{
    /*
    while (mpInternalGame->CurrentState() != Game::State::Over)
    {
        ProcessInputEvents();
        mpInternalGame->UpdateFrame(); // Update frame till the game's over.
        SDL_Delay(250);
    }
    */

    mpInternalGame->UpdateFrame();
    SDL_Delay(2000);

    return 0;
}

void Application::Destroy()
{
    delete mpInternalGame;
    mpInternalGame = nullptr;
}

void Application::ProcessInputEvents()
{
    SDL_Event event = { 0 };
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mpInternalGame->HandleInput(Game::Input::Escape);
                break;
                
            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        mpInternalGame->HandleInput(Game::Input::Escape);
                        break;
                }
                
                break;
            }
        }
    }
}
