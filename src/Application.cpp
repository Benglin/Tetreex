
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../inc/Tetreex.h"

using namespace Tetreex;

const int Application::PinBufferSize = 3;
const int Application::DirBufferSize = 35;
const int Application::IoBufferSize  = 30;
const int Application::DirectionIn   = 0;
const int Application::DirectionOut  = 1;

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
        mpInternalGame->RenderFrame();
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

                    case SDLK_a:
                        mpInternalGame->HandleInput(Game::Input::Button0);
                        break;

                    case SDLK_s:
                        mpInternalGame->HandleInput(Game::Input::Button1);
                        break;

                    case SDLK_d:
                        mpInternalGame->HandleInput(Game::Input::Button2);
                        break;
                }

                break;
            }
        }
    }
}

int Application::GpioExport(int pin)
{
    auto fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open export for writing!\n");
        return(-1);
    }

    char buffer[PinBufferSize];
    auto bytes_written = snprintf(buffer, PinBufferSize, "%d", pin);
    write(fd, buffer, bytes_written);

    close(fd);
    return(0);
}

int Application::GpioUnexport(int pin)
{
    auto fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open unexport for writing!\n");
        return(-1);
    }

    char buffer[PinBufferSize];
    auto bytes_written = snprintf(buffer, PinBufferSize, "%d", pin);
    write(fd, buffer, bytes_written);

    close(fd);
    return(0);
}

int Application::GpioDirection(int pin, int direction)
{
    char path[DirBufferSize];
    snprintf(path, DirBufferSize, "/sys/class/gpio/gpio%d/direction", pin);
    auto fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio direction for writing!\n");
        return(-1);
    }

    auto isInput = direction == DirectionIn;
    static const char s_directions_str[]  = "in\0out";
    if (-1 == write(fd, &s_directions_str[isInput ? 0 : 3], isInput ? 2 : 3)) {
        fprintf(stderr, "Failed to set direction!\n");
        return(-1);
    }

    close(fd);
    return(0);
}

int Application::GpioRead(int pin)
{
    char path[IoBufferSize];
    snprintf(path, IoBufferSize, "/sys/class/gpio/gpio%d/value", pin);
    auto fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return(-1);
    }

    char value_str[3];
    if (-1 == read(fd, value_str, 3)) {
        fprintf(stderr, "Failed to read value!\n");
        return(-1);
    }

    close(fd);
    return(atoi(value_str));
}
