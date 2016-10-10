#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Cube2x2x2.h"
#include "CubeMove.h"

// An optimal solver for 2x2x2 cubes.
class Solver2x2x2
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  Solver2x2x2(size_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will solve the given cube. If the cube can't be solved within the number
  // of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(Cube2x2x2);

private:
  // Writes the set of positions to a cache file.
  void WriteCacheFile(const std::string& cacheFileName);
  // Reads the set of positions from a cache file written by WriteCacheFile().
  // Returns true only if the file was found and the set of positions was populated.
  bool ReadCacheFile(const std::string& cacheFileName);

  // Returns the number of moves required to solve the given cube or -1 if no solution is known.
  int SafeGetNumMovesToSolve(const Cube2x2x2&);
  // Returns the number of moves required to solve the given cube. Throws an exception if the position is not found.
  int GetNumMovesToSolve(const Cube2x2x2&);
  // Returns the best move for the given cube. Throws an exception if no move could be found.
  CubeMove FindNextMove(const Cube2x2x2&);

  std::unordered_map<int, int> movesToSolve;  // number of moves to solve from a given position identified by a key value
};