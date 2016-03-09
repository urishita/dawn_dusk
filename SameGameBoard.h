#pragma once

class CSameGameBoard
{
public:
  /*  Default Constructor */
  CSameGameBoard(void);
  /*  Destructor */
  ~CSameGameBoard(void);
  /*  Function to randomly setup the board */
  void SetupBoard(void);
  /*  Get the color at a particular location */
  COLORREF GetBoardSpace(int row, int col);
  /*  Accessor functions to get board size information */
  int GetWidth(void) const { return m_nWidth; }
  int GetHeight(void) const { return m_nHeight; }
  int GetColumns(void) const { return m_nColumns; }
  int GetRows(void) const { return m_nRows; }
  /*  Function to delete the board and free memory */
  void DeleteBoard(void);
   /*  Is the game over? */
  bool IsGameOver(void) const;
  /*  Get the number of blocks remaining */
  int GetRemainingCount(void) const { return m_nRemaining; }
  /*  Function to delete all adjacent blocks */
  int DeleteBlocks(int row, int col);
private:
  /*  Function to create the board and allocate memory */
  void CreateBoard(void);
   /*  Direction enumeration for deleting blocks */
  enum Direction
  {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
  };
  /*  Recursive helper function for deleting blocks */
  int DeleteNeighborBlocks(int row, int col, int color,
                           Direction direction);
  /*  Function to compact the board after blocks are eliminated */
  void CompactBoard(void);
  /*  2D array pointer */
  int** m_arrBoard;
  /*  List of colors, 0 is background and 1-3 are piece colors */
  COLORREF m_arrColors[4];
  /*  Board size information */
  int m_nColumns;
  int m_nRows;
  int m_nHeight;
  int m_nWidth;
  /*  Number of blocks remaining */
  int m_nRemaining;
};