
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

        this->Draw();
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
}

void Tetromino::Draw(void) const
{
    for (auto y = 0; y < 4; y++)
    {
        auto ty = mY + y; // Target x-coordinate.
        if (ty < 0 || (ty >= mpBoard->Height()))
            continue;

        auto pRowPointer = mpBoard->ContentAt(0, y);
        for (auto x = 0; x < 4; x++)
        {
            auto tx = mX + x;
            if (tx >= 0 && (x < mpBoard->Width()))
            {
                auto flags = mMoldData.mBits[y][x];
                auto color = flags & mMoldData.mColor;
                *(pRowPointer + tx) = color;
            }
        }
    }
}
