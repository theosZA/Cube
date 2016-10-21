#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Solver.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// A solver that extends a 2x2x2 block to a 2x2x3 block on a 3x3x3 cube.
class Solver2x2x3Block : public Solver
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  Solver2x2x3Block(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will extend a BLD 2x2x2 block into a BLDF 2x2x3 block. If the block can't
  // be created within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&);
  // Returns the move sequence that will extend a BLD 2x2x2 block into a BLDF 2x2x3 block from the given
  // scramble. If the block can't be created within the number of moves specified in the constructor then
  // an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) override;

private:
  // Returns a key value that distinguishes based on the cubies in the UFRD 2x2x3 block, but excludes
  // the cubies in the FUR 2x2x2 block since our moves can't change those cubies.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};

