#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// An optimal solver for a 2x2x2 block on a 3x3x3 cube.
class Solver2x2x2Block
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  Solver2x2x2Block(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will create the FUR 2x2x2 block for the given cube. If the block can't
  // be created within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will create the FUR 2x2x2 block from the given scramble. If the block
  // can't be created within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

  // Returns the move sequence that will create the 2x2x2 block around the given corner from the given scramble.
  // If the block can't be created within the number of moves specified in the constructor then an exception
  // is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble, Face cornerFace, Face cornerFaceClockwise) const;

private:
  // Returns a key value that distinguishes based on the cubies in the FUR block only.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};
