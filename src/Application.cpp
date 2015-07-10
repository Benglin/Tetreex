
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#ifdef USE_GPIO_INPUTS

const int Application::PinBufferSize = 3;
const int Application::DirBufferSize = 35;
const int Application::IoBufferSize  = 30;
const int Application::DirectionIn   = 0;
const int Application::DirectionOut  = 1;

const int Application::Gpio07 = 7;
const int Application::Gpio08 = 8;
const int Application::Gpio09 = 9;
const int Application::Gpio10 = 10;
const int Application::Gpio11 = 11;
const int Application::Gpio24 = 24;
const int Application::Gpio25 = 25;
const int Application::PinCount = 7;

#endif

Application::Application(Game* pGame) :
mpInternalGame(pGame)
{
    if (mpInternalGame == nullptr)
        throw new std::runtime_error("Invalid 'Game *' for 'Application'");

#ifdef USE_GPIO_INPUTS

    memset(&mPinStates[0], 0, sizeof(mPinStates));

#endif
}

bool Application::Initialize()
{
#ifdef USE_GPIO_INPUTS

    const int ports[PinCount] =
    {
        Gpio07, Gpio08, Gpio09, Gpio10, Gpio11, Gpio24, Gpio25
    };

    char message[128] = { 0 };
    for (int p = 0; p < PinCount; p++)
    {
        if (GpioExport(ports[p]) == -1)
        {
            snprintf(message, 128, "Could not enable GPIO pin: %d\n", ports[p]);
            std::cout << message;
            return false;
        }

        if (GpioDirection(ports[p], DirectionIn) == -1)
        {
            snprintf(message, 128, "Could not set direction for pin: %d\n", ports[p]);
            std::cout << message;
            return false;
        }

        snprintf(message, 128, "Successfully configured GPIO pin: %d\n", ports[p]);
        std::cout << message;
    }

#endif

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
#ifdef USE_GPIO_INPUTS

    const int ports[PinCount] =
    {
        Gpio07, Gpio08, Gpio09, Gpio10, Gpio11, Gpio24, Gpio25
    };

    char message[128] = { 0 };
    for (int p = 0; p < PinCount; p++)
    {
        if (GpioUnexport(ports[p]) == -1) {
            snprintf(message, 128, "Could not disable GPIO pin: %d\n", ports[p]);
        }
        else {
            snprintf(message, 128, "Successfully disabled GPIO pin: %d\n", ports[p]);
        }

        std::cout << message;
    }

#endif

    delete mpInternalGame;
    mpInternalGame = nullptr;
}

#ifndef USE_GPIO_INPUTS

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

#else

void Application::ProcessInputEvents()
{
    const int ports[PinCount] =
    {
        Gpio07, Gpio08, Gpio09, Gpio10, Gpio11, Gpio24, Gpio25
    };

    char message[128] = { 0 };
    for (int p = 0; p < PinCount; p++)
    {
        auto currentValue = GpioRead(ports[p]);
        if (currentValue == mPinStates[p])
            continue;

        snprintf(message, 128, "Pin %d value: %d\n", ports[p], currentValue);
        std::cout << message;

        mPinStates[p] = currentValue;
        if (currentValue == 0)
            continue; // Release key, no notification.

        switch (ports[p])
        {
            case Application::Gpio07: // Up button.
                mpInternalGame->HandleInput(Game::Input::Escape);
                break;
            case Application::Gpio08: // Down button.
                mpInternalGame->HandleInput(Game::Input::Down);
                break;
            case Application::Gpio09: // Right button.
                mpInternalGame->HandleInput(Game::Input::Right);
                break;
            case Application::Gpio10: // Blue button.
                mpInternalGame->HandleInput(Game::Input::Button0);
                break;
            case Application::Gpio11: // Left button.
                mpInternalGame->HandleInput(Game::Input::Left);
                break;
            case Application::Gpio24: // Yellow button.
                mpInternalGame->HandleInput(Game::Input::Button1);
                break;
            case Application::Gpio25: // Green button.
                mpInternalGame->HandleInput(Game::Input::Button2);
                break;
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

#endif
