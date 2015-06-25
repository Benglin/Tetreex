
#include "../inc/Tetreex.h"

using namespace Tetreex;

const Mold Tetromino::Molds[] =
{
    {
        4,
        0xff0080ff,

        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    {
        3,
        0xff00ff80,
        
        {
            { 0x00000000, 0xffffffff, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },
};

Tetromino::Tetromino(Board* pBoard) :
mX(0), mY(0), mWidth(0), mHeight(0),
mpBoard(pBoard)
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
    if (CanMove(direction))
    {
        this->Clear();

        switch (direction)
        {
            case Direction::Left:
                mX = mX - 1;
                break;
            case Direction::Right:
                mX = mX + 1;
                break;
            case Direction::Down:
                mY = mY + 1;
                break;
        }

        this->Draw(mMoldData.mColor);
    }
}

void Tetromino::Rotate(Rotation rotation)
{
}

void Tetromino::FuseOnBoard(void) const
{
}

void Tetromino::Initialize(void)
{
    mType = Type::I;
    
    auto pMold = &(Tetromino::Molds[(int)mType]);
    memcpy(&mMoldData, pMold, sizeof(Mold));
}

void Tetromino::Clear(void) const
{
    Draw(0x00000000);
}

void Tetromino::Draw(unsigned int color) const
{
    auto boardWidth   = mpBoard->Width();
    auto boardHeight  = mpBoard->Height();
    auto boundingSize = mMoldData.mBoundingSize;

    for (auto y = 0; y < boundingSize; y++)
    {
        auto ty = mY + y; // Target y-coordinate.
        if (ty < 0 || (ty >= boardHeight))
            continue;

        for (auto x = 0; x < boundingSize; x++)
        {
            auto tx = mX + x; // Target x-coordinate.
            if (tx >= 0 && (x < boardWidth))
            {
                auto flags = mMoldData.mBits[y][x];
                if (flags != 0x00000000)
                {
                    // Draw only if it's not a blank.
                    color = flags & color;
                    mpBoard->SetColor(tx, ty, color);
                }
            }
        }
    }
}
