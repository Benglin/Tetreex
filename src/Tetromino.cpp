
#include "../inc/Tetreex.h"

using namespace Tetreex;

Tetromino::Tetromino(Board* pBoard) :
mColor(255, 128, 0)
{
    
}

bool Tetromino::CanMove(Direction direction) const
{
    return true;
}

bool Tetromino::CanRotate(Rotation rotation) const
{
    return true;
}

void Tetromino::Move(Direction direction)
{
    
}

void Tetromino::Rotate(Rotation rotation)
{
    
}
