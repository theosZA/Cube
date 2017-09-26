#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// An optimal solver for edge orientation on a 3x3x3 cube.
class SolverEO
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  SolverEO(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will orient all edges with respect to the <F2, B2, U, D, L, R> subgroup.
  // If the edges can't be oriented within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will orient all edges with respect to the <F2, B2, U, D, L, R> subgroup from the given scramble.
  // If the edges can't be oriented within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

  // Returns the number of moves that will orient all edges with respect to the <F2, B2, U, D, L, R> subgroup.
  // If the edges can't be oriented within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const Cube3x3x3&) const;
  // Returns the number of moves that will orient all edges with respect to the <F2, B2, U, D, L, R> subgroup from the given scramble.
  // If the edges can't be oriented within the number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const;

private:
  // Returns a key value that distinguishes based on the orientation of the edges only.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};
