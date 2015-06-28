
#include <iostream>
#include "../inc/Tetreex.h"

using namespace Tetreex;

#define ISPERMANENT(c)      ((c & 0xff000000) == 0xff000000)
#define MAKETEMPORARY(c)    (c & 0x00ffffff)
#define MAKEPERMANENT(c)    ((c & 0x00ffffff) | 0xff000000)

Board::Board(int width, int height, rgb_matrix::Canvas* pCanvas) :
mWidth(width),
mHeight(height),
mpCanvas(pCanvas),
mpActiveTetromino(nullptr),
mDistribution(0, ((int)Tetromino::Type::Max - 1))
{
    auto elements = mWidth * mHeight;
    mpContent = new unsigned int[elements];
    memset(mpContent, 0, elements * sizeof(unsigned int));
}

Board::~Board()
{
    delete mpActiveTetromino;
    mpActiveTetromino = nullptr;

    delete[] mpContent;
    mpContent = nullptr;
}

int Board::Width(void) const
{
    return mWidth;
}

int Board::Height(void) const
{
    return mHeight;
}

bool Board::IsGameOver(void) const
{
    return false;
}

bool Board::IsPlacementPossible(int x, int y, const Mold& mold) const
{
    for (int my = 0, by = y; my < mold.mBoundingSize; my++, by++)
    {
        // Skip the entire row if it is beyond the board.
        if (by < 0 || (by >= mHeight))
            continue;

        for (int mx = 0, bx = x; mx < mold.mBoundingSize; mx++, bx++)
        {
            auto pixelOnMold = mold.mBits[my][mx];
            if (pixelOnMold == 0x00000000)
                continue; // The pixel is empty so it doesn't matter.

            auto p = PixelAt(bx, by); // Get pixel on board...
            if (p == nullptr)
                continue; // Coordinates out-of-bound.

            // When it gets here, it means the mold has a colored pixel. If the
            // corresponding pixel on the board is not empty, that means the mold
            // cannot be placed here.
            //
            auto pixelOnBoard = *p;
            if (!ISPERMANENT(pixelOnBoard)) // Just a temporary pixel.
                continue;

            if (pixelOnBoard != 0x00000000)
                return false;
        }
    }

    return true;
}

bool Board::HasActiveTetromino(void) const
{
    return mpActiveTetromino != nullptr;
}

void Board::FuseActiveTetromino(void)
{
    if (mpActiveTetromino != nullptr) {
        mpActiveTetromino->FuseOnBoard();
        delete mpActiveTetromino;
        mpActiveTetromino = nullptr;
    }
}

void Board::SetColor(int x, int y, unsigned int color, bool permanent)
{
    color = permanent ? MAKEPERMANENT(color) : MAKETEMPORARY(color);

    auto pPixel = PixelAt(x, y);
    if (pPixel != nullptr)
        *pPixel = color;
}

bool Board::AdvanceTetromino(void)
{
    if (mpActiveTetromino == nullptr)
        return false; // Nothing to advance.

    if (mpActiveTetromino->CanMove(Tetromino::Direction::Down))
        mpActiveTetromino->Move(Tetromino::Direction::Down);
    else
    {
        // Cannot move further.
        this->FuseActiveTetromino();
    }

    // If there's no more active tetromino, then it means it's
    // been fused to the board. In this case the method returns
    // false, indicating that it is time for a new tetromino.
    //
    return mpActiveTetromino != nullptr;
}

bool Board::MoveTetromino(Tetromino::Direction direction)
{
    if (mpActiveTetromino != nullptr)
        return mpActiveTetromino->Move(direction);

    return false; // Nothing changes internally.
}

void Board::GenerateTetromino(void)
{
    if (mpActiveTetromino != nullptr) {
        std::cout << "Invalid call to Board::GenerateTetromino";
        return;
    }

    auto type = ((Tetromino::Type) mDistribution(mGenerator));
    mpActiveTetromino = new Tetromino(this, type);
    mpActiveTetromino->Move(Tetromino::Direction::Down);
}

void Board::RefreshRegion(int x, int y, int width, int height) const
{
    if (x >= mWidth || (y >= mHeight))
        return; // Out of bound.

    if (x < 0) {
        width = width + x;
        x = 0;
    }

    if (y < 0) {
        height = height + y;
        y = 0;
    }

    if (x + width > mWidth)
        width = mWidth - x;
    if (y + height > mHeight)
        height = mHeight - y;

    for (auto h = 0; h < height; h++)
    {
        auto pPixel = PixelAtUnsafe(x, y + h);
        for (auto w = 0; w < width; w++)
        {
            auto color = *(pPixel + w);

            auto red   = ((color & 0x00ff0000) >> 16);
            auto green = ((color & 0x0000ff00) >>  8);
            auto blue  = ((color & 0x000000ff) >>  0);
            mpCanvas->SetPixel(x + w, y + h, red, green, blue);
        }
    }
}

unsigned int* Board::PixelAt(int x, int y) const
{
    if (x < 0 || (x >= mWidth))
        return nullptr;
    if (y < 0 || (y >= mHeight))
        return nullptr;

    return PixelAtUnsafe(x, y);
}

unsigned int* Board::PixelAtUnsafe(int x, int y) const
{
    return &(mpContent[(y * mWidth) + x]);
}
