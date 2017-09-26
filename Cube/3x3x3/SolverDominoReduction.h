#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// An optimal solver for reducing to a domino (3x3x2) on an edge-oriented 3x3x3 cube.
class SolverDominoReduction
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  SolverDominoReduction(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will reduce to a domino, the <F2, B2, L2, R2, U, D> subgroup.
  // If a domino can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will reduce to a domino, the <F2, B2, L2, R2, U, D> subgroup from the given scramble.
  // If a domino can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

  // Returns the number of moves that will reduce to a domino, the <F2, B2, L2, R2, U, D> subgroup.
  // If a domino can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const Cube3x3x3&) const;
  // Returns the number of moves that will reduce to a domino, the <F2, B2, L2, R2, U, D> subgroup from the given scramble.
  // If a domino can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const;

private:
  // Returns a key value that distinguishes based on position of E slive edges and orientation of corners.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};

