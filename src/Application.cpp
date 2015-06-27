
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
    while (mpInternalGame->CurrentState() != Game::State::Over)
    {
        ProcessInputEvents();
        mpInternalGame->UpdateFrame(); // Update frame till the game's over.
    }

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

                    case SDLK_LEFT:
                        mpInternalGame->HandleInput(Game::Input::Left);
                        break;

                    case SDLK_RIGHT:
                        mpInternalGame->HandleInput(Game::Input::Right);
                        break;

                    case SDLK_DOWN:
                        mpInternalGame->HandleInput(Game::Input::Down);
                        break;
                }

                break;
            }
        }
    }
}
