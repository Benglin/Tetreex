
#include "../inc/Tetreex.h"

using namespace Tetreex;

Board::Board(int width, int height, rgb_matrix::Canvas* pCanvas) :
mWidth(width),
mHeight(height),
mpCanvas(pCanvas)
{
    auto elements = mWidth * mHeight;
    mpContent = new unsigned int[elements];
    memset(mpContent, 0, elements * sizeof(unsigned int));
}

Board::~Board()
{
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

bool Board::HasActiveTetromino(void) const
{
    return false;
}

void Board::SetColor(int x, int y, unsigned int color)
{
    auto pPixel = PixelAt(x, y);
    if (pPixel != nullptr)
        *pPixel = color;
}

bool Board::AdvanceTetromino(void)
{
    return true; // Advancement successful.
}

void Board::AddTetromino(Tetromino* pTetromino)
{
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

            auto red   = ((color & 0x000000ff) >>  0);
            auto green = ((color & 0x0000ff00) >>  8);
            auto blue  = ((color & 0x00ff0000) >> 16);
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