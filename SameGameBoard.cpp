#include "StdAfx.h"
#include "SameGameBoard.h"

CSameGameBoard::CSameGameBoard(void)
: m_arrBoard(NULL),
  m_nColumns(15), m_nRows(15),
  m_nHeight(35),  m_nWidth(35),m_nRemaining(0)
{
  m_arrColors[0] = RGB(  0,  0,  0);
  m_arrColors[1] = RGB(255,  0,  0);
  m_arrColors[2] = RGB(255,255, 64);
  m_arrColors[3] = RGB(  0,  0,255);
}

CSameGameBoard::~CSameGameBoard(void)
{
  //  Simply delete the board
  DeleteBoard();
}

void CSameGameBoard::SetupBoard(void)
{
  //  Create the board if needed
  if(m_arrBoard == NULL)
    CreateBoard();
  //  Randomly set each square to a color
  for(int row = 0; row < m_nRows; row++)
    for(int col = 0; col < m_nColumns; col++)
      m_arrBoard[row][col] = (rand() % 3) + 1;
  m_nRemaining = m_nRows * m_nColumns;
}

COLORREF CSameGameBoard::GetBoardSpace(int row, int col)
{
  //  Check the bounds of the array
  if(row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
    return m_arrColors[0];
  return m_arrColors[m_arrBoard[row][col]];
}

void CSameGameBoard::DeleteBoard(void)
{
  //  Don't delete a NULL board
  if(m_arrBoard != NULL)
  {
    for(int row = 0; row < m_nRows; row++)
    {
      if(m_arrBoard[row] != NULL)
      {
        //  Delete each row first
        delete [] m_arrBoard[row];
        m_arrBoard[row] = NULL;
      }
    }
    //  Finally delete the array of rows
    delete [] m_arrBoard;
    m_arrBoard = NULL;
  }
}

void CSameGameBoard::CreateBoard(void)
{
  //  If there is already a board, delete it
  if(m_arrBoard != NULL)
    DeleteBoard();
  //  Create the array of rows
  m_arrBoard = new int*[m_nRows];
  //  Create each row
  for(int row = 0; row < m_nRows; row++)
  {
    m_arrBoard[row] = new int[m_nColumns];
    //  Set each square to be empty
    for(int col = 0; col < m_nColumns; col++)
      m_arrBoard[row][col] = 0;
  }
}
int CSameGameBoard::DeleteBlocks(int row, int col)
{
  //  Make sure that the row and column are valid
  if(row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
    return -1;
  //  Can't delete background blocks
  int nColor = m_arrBoard[row][col];
  if(nColor == 0)
    return -1;
  //	First check if there are any of the adjacent sides
  //  with the same color
  int nCount = -1;
  if((row - 1 >= 0 && m_arrBoard[row - 1][col] == nColor) ||
     (row + 1 < m_nRows && m_arrBoard[row + 1][col] == nColor) ||
     (col - 1 >= 0 && m_arrBoard[row][col - 1] == nColor) ||
     (col + 1 < m_nColumns && m_arrBoard[row][col + 1] == nColor))
  {
    //  Then call the recursive function to eliminate all 
    //  other touching blocks with same color
    m_arrBoard[row][col] = 0;
    nCount = 1;
    //  Recursive call for up
    nCount +=
      DeleteNeighborBlocks(row - 1, col, nColor, DIRECTION_DOWN);
    //  Recursive call for down
    nCount +=
      DeleteNeighborBlocks(row + 1,col, nColor, DIRECTION_UP);
    //  Recursive call for left
    nCount +=
      DeleteNeighborBlocks(row, col - 1, nColor, DIRECTION_RIGHT);
    //  Recursive call for right
    nCount +=
      DeleteNeighborBlocks(row, col + 1, nColor, DIRECTION_LEFT);
    //  Finally compact the board
    CompactBoard();
    //  Remove the count from the number remaining
    m_nRemaining -= nCount;
  }
  //  Return the total number of pieces deleted
  return nCount;
}
int CSameGameBoard::DeleteNeighborBlocks(int row, int col, int color,
                                         Direction direction)
{
  //  Check if it is on the board
  if(row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
    return 0;
  //  Check if it has the same color
  if(m_arrBoard[row][col] != color)
    return 0;
  int nCount = 1;
  m_arrBoard[row][col] = 0;
  //  If we weren't told to not go back up, check up
  if(direction != DIRECTION_UP)
    nCount +=
      DeleteNeighborBlocks(row - 1, col, color, DIRECTION_DOWN);
  //  If we weren't told to not go back down, check down
  if(direction != DIRECTION_DOWN)
    nCount +=
      DeleteNeighborBlocks(row + 1, col, color, DIRECTION_UP);
  //  If we weren't told to not go back left, check left
  if(direction != DIRECTION_LEFT)
    nCount +=
      DeleteNeighborBlocks(row, col - 1, color, DIRECTION_RIGHT);
  //  If we weren't told to not go back right, check right
  if(direction != DIRECTION_RIGHT)
    nCount +=
      DeleteNeighborBlocks(row, col + 1, color, DIRECTION_LEFT);
  //  Return the total number of pieces deleted
  return nCount;
}
void CSameGameBoard::CompactBoard(void)
{
  //  First move everything down
  for(int col = 0; col < m_nColumns; col++)
  {
    int nNextEmptyRow = m_nRows - 1;
    int nNextOccupiedRow = nNextEmptyRow;
    while(nNextOccupiedRow >= 0 && nNextEmptyRow >= 0)
    {
      //  First find the next empty row
      while(nNextEmptyRow >= 0 &&
            m_arrBoard[nNextEmptyRow][col] != 0)
        nNextEmptyRow--;
      if(nNextEmptyRow >= 0)
      {
        //  Then find the next occupied row from the next empty row
        nNextOccupiedRow = nNextEmptyRow - 1;
        while(nNextOccupiedRow >= 0 &&
              m_arrBoard[nNextOccupiedRow][col] == 0)
          nNextOccupiedRow--;
        if(nNextOccupiedRow >= 0)
        {
          //  Now move the block from occupied to empty
          m_arrBoard[nNextEmptyRow][col] =
            m_arrBoard[nNextOccupiedRow][col];
          m_arrBoard[nNextOccupiedRow][col] = 0;
        }
      }
    }
  }
  //  Then move everything from right to left
  int nNextEmptyCol = 0;
  int nNextOccupiedCol = nNextEmptyCol;
  while(nNextEmptyCol < m_nColumns && nNextOccupiedCol < m_nColumns)
  {
    //  First find the next empty column
    while(nNextEmptyCol < m_nColumns &&
          m_arrBoard[m_nRows - 1][nNextEmptyCol] != 0)
      nNextEmptyCol++;
    if(nNextEmptyCol < m_nColumns)
    {
      //  Then find the next column with something in it
      nNextOccupiedCol = nNextEmptyCol + 1;
      while(nNextOccupiedCol < m_nColumns &&
            m_arrBoard[m_nRows - 1][nNextOccupiedCol] == 0)
        nNextOccupiedCol++;
      if(nNextOccupiedCol < m_nColumns)
      {
        //  Move entire column to the left
        for(int row = 0; row < m_nRows; row++)
        {
          m_arrBoard[row][nNextEmptyCol] =
            m_arrBoard[row][nNextOccupiedCol];
          m_arrBoard[row][nNextOccupiedCol] = 0;
        }
      }
    }
  }
}
bool CSameGameBoard::IsGameOver(void) const
{
  //  Go column by column, left to right
  for(int col = 0; col < m_nColumns; col++)
  {
    //  Row by row, bottom to top
    for(int row = m_nRows - 1; row >= 0; row--)
    {
      int nColor = m_arrBoard[row][col];
      //  Once we hit background, this column is done
      if(nColor == 0)
        break;
      else
      {
        //  Check above and right
        if(row - 1 >= 0 &&
           m_arrBoard[row - 1][col] == nColor)
          return false;
        else if(col + 1 < m_nColumns &&
                m_arrBoard[row][col + 1] == nColor)
          return false;
      }
    }
  }
  //  No two found adjacent
  return true;
}