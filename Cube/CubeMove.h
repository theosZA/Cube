#pragma once

#include <iosfwd>
#include <vector>

#include "Face.h"

struct CubeMove
{
  Face face;
  int quarterRotationsClockwise; // -1, 1 or 2 are normal for a cube

  bool operator==(const CubeMove& move) const;
};

std::ostream& operator<< (std::ostream&, const CubeMove&);

// Gives a move that would undo the specified move.
CubeMove InvertMove(CubeMove move);

// Gives a move sequence that would undo the specified move sequence.
std::vector<CubeMove> InvertMoveSequence(const std::vector<CubeMove>& moveSequence);

// Reads a space-separated list of moves in standard notation (e.g. "F U' R2")
// and returns the corresponding move sequence.
std::vector<CubeMove> ParseMoveSequence(const std::string& moves);

// Writes the move sequence as a comma-separated list of moves.
std::string MoveSequenceToText(const std::vector<CubeMove>& moves);

// Simplifies the move sequence by combining consecutive moves on the same face.
std::vector<CubeMove> SimplifyMoveSequence(const std::vector<CubeMove>& moves);

// Combines two move sequences into a simplified move sequence.
std::vector<CubeMove> operator+(const std::vector<CubeMove>&, const std::vector<CubeMove>&);