#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// An optimal solver for reducing from a domino state to a state solvable with only double turns.
class SolverDominoToDoubleTurns
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  SolverDominoToDoubleTurns(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will take the <F2, B2, L2, R2, U, D> subgroup to the <F2, B2, L2, R2, U2, D2> subgroup.
  // If a state where only double turns are needed can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will take the <F2, B2, L2, R2, U, D> subgroup to the <F2, B2, L2, R2, U2, D2> subgroup from the given scramble.
  // If a state where only double turns are needed can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

  // Returns the move sequence that will take the <F2, B2, L2, R2, U, D> subgroup to the <F2, B2, L2, R2, U2, D2> subgroup.
  // If a state where only double turns are needed can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const Cube3x3x3&) const;
  // Returns the move sequence that will take the <F2, B2, L2, R2, U, D> subgroup to the <F2, B2, L2, R2, U2, D2> subgroup from the given scramble.
  // If a state where only double turns are needed can't be reached within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const;

private:
  // Returns a key value that distinguishes based on whether each cubie can reach it's correct position with double turns.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};

