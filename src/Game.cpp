
#include <sys/time.h>
#include "../inc/Tetreex.h"

using namespace Tetreex;

const double Game::MinFrameTime = 40.0;  // 40ms per frame = 25 fps.
const double Game::DropInterval = 500.0; // Starting drop time of 500ms.

Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas),
mVisualInvalidated(false),
mPrevDropTime(0),
mDropInterval(Game::DropInterval),
mCurrentState(State::None),
mpBoard(nullptr),
mpAudioDevice(nullptr)
{
    mpAudioDevice = new AudioDevice();
    mpAudioDevice->LoadMediaFiles();
    mpAudioDevice->PlayBackgroundMusic(true);

    mpBoard = new Board(16, 16, mpCanvas);
    mpBoard->GenerateTetromino();
}

Game::~Game()
{
    delete mpBoard;
    mpBoard = nullptr;

    delete mpAudioDevice;
    mpAudioDevice = nullptr;

    delete mpCanvas;
    mpCanvas = nullptr;
}

void Game::HandleInput(Game::Input input)
{
    switch (input)
    {
        case Game::Input::Escape:
            mCurrentState = Game::State::Over;
            break;

        case Game::Input::Left:
            mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Left);
            break;

        case Game::Input::Right:
            mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Right);
            break;

        case Game::Input::Down:
            mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Down);
            break;

        case Game::Input::Button0:
            break;

        case Game::Input::Button1:
            mVisualInvalidated = mpBoard->RotateTetromino(Tetromino::Rotation::CounterClockWise);
            break;

        case Game::Input::Button2:
            mVisualInvalidated = mpBoard->RotateTetromino(Tetromino::Rotation::ClockWise);
            break;
    }
}

void Game::UpdateFrame(void)
{
    timeval tv;
    gettimeofday(&tv, nullptr);
    double currentTime = ((tv.tv_sec * 1000.0) + (tv.tv_usec * (1.0 / 1000.0)));
    const double difference = currentTime - mPrevDropTime;

    if (difference < mDropInterval)
        return; // Too soon, come back later.

    // Making sure that we don't overshoot by a lot.
    double overshot = difference - mDropInterval;
    if (overshot > mDropInterval)
        overshot = mDropInterval;

    mPrevDropTime = currentTime - overshot;
    if (!mpBoard->AdvanceTetromino())
        mpBoard->GenerateTetromino();

    mVisualInvalidated = true;
}

void Game::RenderFrame(void)
{
    if (!mVisualInvalidated)
        return;

#ifdef USE_SDL_RENDERER

    auto pCanvas = ((PixelBuffer*) mpCanvas);
    pCanvas->Present(); // Only for SDL we need to present it.

#endif

    mVisualInvalidated = false;
}

Game::State Game::CurrentState(void) const
{
    return this->mCurrentState;
}
