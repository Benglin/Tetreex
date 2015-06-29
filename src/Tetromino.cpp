
#include "../inc/Tetreex.h"

using namespace Tetreex;

const Mold Tetromino::Molds[] =
{
    { // I
        4,
        0xff00F5EA,

        { 0, 1, 0, 2 },

        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // O
        2,
        0xffFFE620,

        { 0, 0, 0, 0 },

        {
            { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // T
        3,
        0xff787AFF,

        { 0, 0, 0, 1 },

        {
            { 0x00000000, 0xffffffff, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // J
        3,
        0xff2094FA,

        { 0, 1, 0, 0 },

        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0x00000000, 0x00000000, 0xffffffff, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // L
        3,
        0xffFF9500,

        { 0, 1, 0, 0 },

        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0xffffffff, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // S
        3,
        0xff04DE71,

        { 0, 0, 0, 1 },

        {
            { 0x00000000, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },

    { // Z
        3,
        0xffFA114F,

        { 0, 0, 0, 1 },

        {
            { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },
            { 0x00000000, 0xffffffff, 0xffffffff, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        }
    },
};

void Mold::Rotate(bool clockwise)
{
    switch (this->mBoundingSize)
    {
        case 2: Rotate2x2(clockwise); break;
        case 3: Rotate3x3(clockwise); break;
        case 4: Rotate4x4(clockwise); break;
    }
}

void Mold::Rotate2x2(bool clockwise)
{
    // Does nothing for 2 x 2 case.
}

void Mold::Rotate3x3(bool clockwise)
{
    if (clockwise)
    {
        auto t = mMasks[0][0];
        mMasks[0][0] = mMasks[2][0];
        mMasks[2][0] = mMasks[2][2];
        mMasks[2][2] = mMasks[0][2];
        mMasks[0][2] = t;

        t = mMasks[0][1];
        mMasks[0][1] = mMasks[1][0];
        mMasks[1][0] = mMasks[2][1];
        mMasks[2][1] = mMasks[1][2];
        mMasks[1][2] = t;
    }
    else
    {
        auto t = mMasks[0][0];
        mMasks[0][0] = mMasks[0][2];
        mMasks[0][2] = mMasks[2][2];
        mMasks[2][2] = mMasks[2][0];
        mMasks[2][0] = t;

        t = mMasks[0][1];
        mMasks[0][1] = mMasks[1][2];
        mMasks[1][2] = mMasks[2][1];
        mMasks[2][1] = mMasks[1][0];
        mMasks[1][0] = t;
    }
}

void Mold::Rotate4x4(bool clockwise)
{
    if (clockwise)
    {
        auto t = mMasks[0][0];
        mMasks[0][0] = mMasks[3][0];
        mMasks[3][0] = mMasks[3][3];
        mMasks[3][3] = mMasks[0][3];
        mMasks[0][3] = t;

        t = mMasks[0][1];
        mMasks[0][1] = mMasks[2][0];
        mMasks[2][0] = mMasks[3][2];
        mMasks[3][2] = mMasks[1][3];
        mMasks[1][3] = t;

        t = mMasks[0][2];
        mMasks[0][2] = mMasks[1][0];
        mMasks[1][0] = mMasks[3][1];
        mMasks[3][1] = mMasks[2][3];
        mMasks[2][3] = t;

        t = mMasks[1][1];
        mMasks[1][1] = mMasks[2][1];
        mMasks[2][1] = mMasks[2][2];
        mMasks[2][2] = mMasks[1][2];
        mMasks[1][2] = t;
    }
    else
    {
        auto t = mMasks[0][0];
        mMasks[0][0] = mMasks[0][3];
        mMasks[0][3] = mMasks[3][3];
        mMasks[3][3] = mMasks[3][0];
        mMasks[3][0] = t;

        t = mMasks[0][1];
        mMasks[0][1] = mMasks[1][3];
        mMasks[1][3] = mMasks[3][2];
        mMasks[3][2] = mMasks[2][0];
        mMasks[2][0] = t;

        t = mMasks[0][2];
        mMasks[0][2] = mMasks[2][3];
        mMasks[2][3] = mMasks[3][1];
        mMasks[3][1] = mMasks[1][0];
        mMasks[1][0] = t;

        t = mMasks[1][1];
        mMasks[1][1] = mMasks[1][2];
        mMasks[1][2] = mMasks[2][2];
        mMasks[2][2] = mMasks[2][1];
        mMasks[2][1] = t;
    }
}

Tetromino::Tetromino(Board* pBoard, Tetromino::Type type) :
mX(0), mY(0), mWidth(0), mHeight(0),
mpBoard(pBoard), mType(type)
{
    Initialize();
}

bool Tetromino::CanMove(Direction direction) const
{
    auto x = mX, y = mY; // New x and y coordinates.

    switch (direction)
    {
        case Direction::Left:
            x = x - 1;
            break;

        case Direction::Right:
            x = x + 1;
            break;

        case Direction::Down:
            y = y + 1;
            break;

        default:
            break;
    }

    // First validate both the left and right margins, ensuring they do not go out-of-bound.
    //
    if (x + mMoldData.mMargins[0] < 0)
        return false;
    if ((x + mMoldData.mBoundingSize - mMoldData.mMargins[2]) > mpBoard->Width())
        return false;

    // Ensure the block doesn't go beyond the lower bound of the board.
    if ((y + mMoldData.mBoundingSize - mMoldData.mMargins[3]) > mpBoard->Height())
        return false;

    // Check to see if placement is possible.
    return mpBoard->IsPlacementPossible(x, y, mMoldData);
}

bool Tetromino::CanRotate(Rotation rotation) const
{
    Mold temporary; // Make a duplicate copy for rotation...
    memcpy(&temporary, &mMoldData, sizeof(Mold));
    temporary.Rotate(rotation == Tetromino::Rotation::ClockWise);

    auto x = mX, y = mY;
    if (x + temporary.mMargins[0] < 0)
        x = 0;
    else if (x + temporary.mBoundingSize - temporary.mMargins[2] > mpBoard->Width())
        x = mpBoard->Width() - (temporary.mBoundingSize - temporary.mMargins[2]);

    // Ensure the block doesn't go beyond the lower bound of the board.
    if ((y + temporary.mBoundingSize - temporary.mMargins[3]) > mpBoard->Height())
        return false;

    // Check to see if placement is possible.
    return mpBoard->IsPlacementPossible(x, y, temporary);
}

bool Tetromino::Move(Direction direction)
{
    if (!CanMove(direction))
        return false;

    auto oldX = mX, oldY = mY;
    auto widthToRefresh = mMoldData.mBoundingSize;
    auto heightToRefresh = mMoldData.mBoundingSize;

    this->Clear(); // Clear the existing region.

    switch (direction)
    {
        case Direction::Left:
            mX = mX - 1;
            widthToRefresh++;
            break;
        case Direction::Right:
            mX = mX + 1;
            widthToRefresh++;
            break;
        case Direction::Down:
            mY = mY + 1;
            heightToRefresh++;
            break;
    }

    this->Draw(mMoldData.mColor, false); // Render at new location.

    // Flush the contents of board to canvas.
    mpBoard->RefreshRegion(oldX < mX ? oldX : mX,
                           oldY < mY ? oldY : mY,
                           widthToRefresh,
                           heightToRefresh);

    return true;
}

bool Tetromino::Rotate(Rotation rotation)
{
    if (!CanRotate(rotation))
        return false;

    auto oldX = mX, oldY = mY;
    auto widthToRefresh = mMoldData.mBoundingSize;
    auto heightToRefresh = mMoldData.mBoundingSize;

    this->Clear(); // Clear the existing region.

    mMoldData.Rotate(rotation == Tetromino::Rotation::ClockWise);

    // Ensure the rotation results in tetromino being pushed inward if it goes beyond.
    if (mX + mMoldData.mMargins[0] < 0) {
        mX = 0;
        widthToRefresh += mX - oldX;
    }
    else if (mX + mMoldData.mBoundingSize - mMoldData.mMargins[2] > mpBoard->Width()) {
        mX = mpBoard->Width() - (mMoldData.mBoundingSize - mMoldData.mMargins[2]);
        widthToRefresh += oldX - mX;
    }

    this->Draw(mMoldData.mColor, false); // Render at new location.

    // Flush the contents of board to canvas.
    mpBoard->RefreshRegion(oldX < mX ? oldX : mX,
                           oldY < mY ? oldY : mY,
                           widthToRefresh,
                           heightToRefresh);

    return true;
}

void Tetromino::FuseOnBoard(void) const
{
    Draw(mMoldData.mColor, true); // Draw with permanent color.
}

void Tetromino::Initialize(void)
{
    auto pMold = &(Tetromino::Molds[(int)mType]);
    memcpy(&mMoldData, pMold, sizeof(Mold));

    // In order for a new tetromino to be hidden from view, its vertical offset needs
    // to be just before the first row of pixels. Here the total height is taken, and
    // then subtracting the bottom margin will be the actual height of the tetromino.
    // Moving that many pixels vertically above the board will hide the tetromino just
    // right.
    //
    mY = -(mMoldData.mBoundingSize - mMoldData.mMargins[3]);
    mX = ((mpBoard->Width() - mMoldData.mBoundingSize) / 2);
}

void Tetromino::Clear(void) const
{
    Draw(0x00000000, false);
}

void Tetromino::Draw(unsigned int color, bool permanent) const
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
                auto flags = mMoldData.mMasks[y][x];
                if (flags != 0x00000000)
                {
                    // Draw only if it's not a blank.
                    color = flags & color;
                    mpBoard->SetColor(tx, ty, color, permanent);
                }
            }
        }
    }
}
