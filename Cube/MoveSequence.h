#pragma once

#include <vector>

#include "Cube.h"

class MoveSequence
{
public:
  MoveSequence(Cube3x3x3 cube) : cube(cube) {}

  void PushMove(CubeMove);
  void PopMove();
  void Clear();

  const Cube3x3x3& GetCube() const { return cube; }

  template<class CubeMoveOutIter>
  void CopyMoves(CubeMoveOutIter out)
  {
    for (auto move : moves)
      out = possibleMoves[move];
  }

  // Sets the move sequence to the first in the class-defined order of move sequences of the specific length.
  void GoToFirst(size_t length);
  // Sets the move sequence to the next in the class-defined order of move sequences of the same length.
  // Returns false if this was the last such move sequence.
  bool GoToNext();

private:
  // Adds a move to the move sequence where the move is specified by an index into the array of possible moves (0-17).
  void PushMove(size_t moveIndex);

  Cube3x3x3 cube;
  std::vector<size_t> moves; // index into the array of possible moves (0-17)

  static const std::array<CubeMove, 18> MoveSequence::possibleMoves;
};

