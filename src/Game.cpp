
#include "../inc/Tetreex.h"

using namespace Tetreex;


Game::Game(rgb_matrix::Canvas* pCanvas) :
mpCanvas(pCanvas),
mFrameCount(0),
mCurrentState(State::None),
mpBoard(nullptr),
mpAudioDevice(nullptr)
{
    mpAudioDevice = new AudioDevice();
    mpAudioDevice->LoadMediaFiles();
    mpAudioDevice->PlayBackgroundMusic(true);

    mpBoard = new Board(32, 32, mpCanvas);

    auto pTetromino = new Tetromino(mpBoard);
    mpBoard->AddTetromino(pTetromino);
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
            
        case Game::Input::Bottom:
            mpBoard->MoveTetromino(Tetromino::Direction::Down);
            break;

        default:
            break;
    }
}

void Game::UpdateFrame(void)
{
#ifdef USE_SDL_RENDERER
    
    auto pCanvas = ((PixelBuffer*) mpCanvas);
    pCanvas->Present(); // Only for SDL we need to present it.
    
#endif
}

Game::State Game::CurrentState(void) const
{
    return this->mCurrentState;
}
