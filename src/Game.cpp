
#include <unistd.h>
#include "../inc/Tetreex.h"

using namespace Tetreex;

const double Game::MinFrameTime = 40.0;         // 40ms per frame = 25 fps.
const double Game::DropInterval = 400.0;        // Starting drop time of 400ms.
const double Game::IntervalChangeAmount = 20.0; // Amount of ms to speed up.
const double Game::MinDropInterval = 100.0;     // Lowest drop interval.

Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas),
mVisualInvalidated(false),
mAccumTimeDiff(0.0),
mDropInterval(Game::DropInterval),
mCurrentState(State::SplashScreen),
mpBoard(nullptr),
mpAudioDevice(nullptr)
{
    mpAudioDevice = new AudioDevice();
    mpAudioDevice->LoadMediaFiles();
    mpBoard = new Board(16, 16, mpCanvas);

    mpSplashScreen = new SplashScreen(mpCanvas);
}

Game::~Game()
{
    delete mpSplashScreen;
    mpSplashScreen = nullptr;

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
            mCurrentState = Game::State::Terminated;
            break;

        case Game::Input::Left:
            if (mCurrentState == State::InProgress)
                mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Left);
            break;

        case Game::Input::Right:
            if (mCurrentState == State::InProgress)
                mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Right);
            break;

        case Game::Input::Down:
            if (mCurrentState == State::InProgress)
                mVisualInvalidated = mpBoard->MoveTetromino(Tetromino::Direction::Down);
            break;

        case Game::Input::Button0:
            // Pressing button 0 will restart the game if it's currently over.
            if (mCurrentState == State::SplashScreen) {
                mpBoard->StartNewGame();
                mpAudioDevice->PlayBackgroundMusic(true);
                mCurrentState = State::InProgress;
                mAccumTimeDiff = 0.0;
                mDropInterval = DropInterval; // Reset drop interval.
            }
            break;

        case Game::Input::Button1:
            if (mCurrentState == State::InProgress)
                mVisualInvalidated = mpBoard->RotateTetromino(Tetromino::Rotation::CounterClockWise);
            break;

        case Game::Input::Button2:
            if (mCurrentState == State::InProgress)
                mVisualInvalidated = mpBoard->RotateTetromino(Tetromino::Rotation::ClockWise);
            break;

        default:
            break;
    }
}

void Game::UpdateFrame(double deltaTimeMs)
{
    if (mCurrentState == State::SplashScreen) {
        mVisualInvalidated = mpSplashScreen->UpdateFrame(deltaTimeMs);
        return;
    }

    if (mCurrentState != State::InProgress)
        return; // Game is not running, no frame update.

    mAccumTimeDiff = mAccumTimeDiff + deltaTimeMs;
    if (mAccumTimeDiff < mDropInterval)
        return; // Too soon, come back later.

    // Making sure that we don't overshoot by a lot.
    mAccumTimeDiff = mAccumTimeDiff - mDropInterval;
    if (mAccumTimeDiff > mDropInterval)
        mAccumTimeDiff = mDropInterval;

    auto compactDidTakePlace = false;
    if (!mpBoard->AdvanceTetromino(compactDidTakePlace))
    {
        if (compactDidTakePlace)
        {
            mDropInterval -= IntervalChangeAmount; // Speed up after each clear.
            if (mDropInterval < MinDropInterval)
                mDropInterval = MinDropInterval;
        }

        if (mpBoard->IsTopMostRowNonEmpty()) {
            mCurrentState = State::SplashScreen;
            mpCanvas->Clear(); // Clear screen since it is over now.
            mpAudioDevice->PlayBackgroundMusic(false); // Pause music playback.
        }
        else
            mpBoard->GenerateTetromino();
    }

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
