
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
mpContent(nullptr),
mpActiveTetromino(nullptr),
mDistribution(0, ((int)Tetromino::Type::Max - 1))
{
    ResetContents();
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

void Board::StartNewGame(void)
{
    ResetContents();
}

bool Board::IsTopMostRowNonEmpty(void) const
{
    auto pReadPointer = PixelAt(0, 0);
    for (int x = 0; x < mWidth; ++x) {
        if (0x00000000 != pReadPointer[x])
            return true; // Found a non-empty cell.
    }

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
            auto pixelOnMold = mold.mMasks[my][mx];
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

void Board::SetColor(int x, int y, unsigned int color, bool permanent)
{
    color = permanent ? MAKEPERMANENT(color) : MAKETEMPORARY(color);

    auto pPixel = PixelAt(x, y);
    if (pPixel != nullptr)
        *pPixel = color;
}

bool Board::AdvanceTetromino(bool& compactDidTakePlace)
{
    compactDidTakePlace = false;
    if (mpActiveTetromino == nullptr)
        return false; // Nothing to advance.

    if (mpActiveTetromino->CanMove(Tetromino::Direction::Down))
        mpActiveTetromino->Move(Tetromino::Direction::Down);
    else
    {
        // Cannot move further.
        FuseActiveTetromino(compactDidTakePlace);
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

bool Board::RotateTetromino(Tetromino::Rotation rotation)
{
    if (mpActiveTetromino != nullptr)
        return mpActiveTetromino->Rotate(rotation);

    return false; // Nothing changes internally.
}

int gtype = 0;

void Board::GenerateTetromino(void)
{
    if (mpActiveTetromino != nullptr) {
        std::cout << "Invalid call to Board::GenerateTetromino";
        return;
    }

    if (gtype == 7)
        gtype = 0;

    auto type = ((Tetromino::Type) gtype++);
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

            // A pixel on board is corresponding to two pixels on display.
            auto left = ((x + w) << 1);
            auto top = ((y + h) << 1);

            mpCanvas->SetPixel(left + 0, top + 0, red, green, blue);
            mpCanvas->SetPixel(left + 1, top + 0, red, green, blue);
            mpCanvas->SetPixel(left + 0, top + 1, red, green, blue);
            mpCanvas->SetPixel(left + 1, top + 1, red, green, blue);
        }
    }
}

void Board::ResetContents(void)
{
    auto elements = mWidth * mHeight;

    if (mpContent == nullptr)
        mpContent = new unsigned int[elements];

    memset(mpContent, 0, elements * sizeof(unsigned int));
    mpCanvas->Clear(); // Clear display to empty screen.
}

void Board::FuseActiveTetromino(bool& compactDidTakePlace)
{
    if (mpActiveTetromino != nullptr) {
        mpActiveTetromino->FuseOnBoard();
        delete mpActiveTetromino;
        mpActiveTetromino = nullptr;
    }

    compactDidTakePlace = CompactContent();
}

bool Board::CompactContent(void)
{
    auto didCompactLines = false;
    auto currRow = mHeight - 1;
    auto nextRow = currRow - 1;

    while (currRow > 0)
    {
        if (!IsRowFilled(currRow))
        {
            currRow--;
            nextRow--;
            continue;
        }

        // Current row is filled, start moving next rows in.
        while (nextRow >= 0)
        {
            MoveRowContentDownward(nextRow);
            nextRow = nextRow - 1;
        }

        didCompactLines = true;
        nextRow = currRow - 1; // Reset next row cursor.
    }

    if (didCompactLines) // Refresh only at least a line was compacted.
        this->RefreshRegion(0, 0, mWidth, mHeight);

    return didCompactLines;
}

void Board::MoveRowContentDownward(int row)
{
    auto pReadPointer = PixelAt(0, row);
    auto pWritePointer = PixelAt(0, row + 1);
    auto bytes = mWidth * sizeof(unsigned int);
    memcpy(pWritePointer, pReadPointer, bytes);
}

bool Board::IsRowFilled(int row) const
{
    auto pReadPointer = PixelAt(0, row);
    if (pReadPointer == nullptr)
        return false;

    for (int x = 0; x < mWidth; ++x) {
        if (0x00000000 == pReadPointer[x])
            return false; // Found an empty cell.
    }

    return true; // All cells are filled.
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
