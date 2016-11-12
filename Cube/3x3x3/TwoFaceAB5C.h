#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// A solver that extends an 2x2x3+EO to AB5C on a 3x3x3 cube. That is given all but the U and R faces are
// solved but the U and R edges are oriented on those faces, one corner and all other edges are solved
// leaving all but 5 corners (AB5) solved.
class TwoFaceAB5C
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file (prefix plus corner name), and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  TwoFaceAB5C(std::uint32_t maxMoves, const std::string& cacheFileNamePrefix = "");

  // Returns the move sequence that will take a 2x2x3+EO state to an AB5C state. The specified corner
  // is the one that is solved. If the state can't be reached within the number of moves specified in the
  // constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&, Face cornerFace, Face cornerFaceClockwise) const;
  // Returns the move sequence that will take a 2x2x3+EO state to an AB5C state from the given scramble.
  // The specified corner is the one that is solved. If the state can't be reached within the number of
  // moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble, Face cornerFace, Face cornerFaceClockwise) const;

private:
  // Returns a key value that distinguishes based on the specified corner cubie and the edge cubies in the
  // U and R faces. Cubies not on the U and R faces are ignored since our moves can't change those cubies.
  static std::uint32_t GetKeyValue(const Cube3x3x3&, Face cornerFace, size_t cornerIndex);

  // Returns a unique name for the corner (regardless of faces chosen).
  static std::string GetCornerName(Face cornerFace, Face cornerFaceClockwise);

  std::map<std::string, CubeStateSolver<Cube3x3x3>> solversByCorner;
};

