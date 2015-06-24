
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

unsigned int* Board::ContentAt(int x, int y) const
{
    return &(mpContent[(y * mWidth) + x]);
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
}
