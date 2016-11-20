#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// A solver that extends an 2x2x3+EO to AB3C on a 3x3x3 cube. That is given all but the U and R faces are
// solved but the U and R edges are oriented on those faces, 3 corners and all other edges are solved
// leaving all but 3 corners (AB5) solved.
class TwoFaceAB3C
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file (with the name being the prefix plus the 3 corners
  // to be solved), and only if that isn't found will the solver generate the moves from scratch and then
  // save to the cache file.
  TwoFaceAB3C(std::uint32_t maxMoves, const std::string& cacheFileNamePrefix = "");

  struct Corner
  {
    Face cornerFace;
    Face cornerFaceClockwise;

    bool operator<(const Corner&) const;
  };

  // Returns the move sequence that will take a 2x2x3+EO state to an AB3C state from the given scramble.
  // The specified corners are the ones that are solved. If the state can't be reached within the number of
  // moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble, const std::array<Corner, 3>& corners) const;

  // Returns true if the given scramble can be solved to an AB3C state with the specified corners solved
  // using only U and R moves.
  bool CanSolve(const std::vector<CubeMove>& scramble, const std::array<Corner, 3>& corners) const;

private:
  // Returns a key value that distinguishes based on the specified 3 corner cubies and the edge cubies in the
  // U and R faces. Cubies not on the U and R faces are ignored since our moves can't change those cubies.
  static std::uint32_t GetKeyValue(const Cube3x3x3&, const std::array<Corner, 3>& corners);
  // Returns 15 bits that distinguishes based on the specified 3 corner cubies in the U and R faces.
  static std::uint32_t GetCornersKeyValue(const Cube3x3x3&, std::array<Corner, 3> corners);
  // Returns 16 bits that distinguishes based on the edge cubies in the U and R faces.
  static std::uint32_t GetEdgesKeyValue(const Cube3x3x3&);

  // Returns a unique name for the 3 corners (regardless of faces chosen for the corner or the order of the corners).
  static std::string GetCornersName(std::array<Corner, 3> corners);
  // Returns a unique name for the corner (regardless of faces chosen for that cubie).
  static std::string GetCornerName(const Corner& corner);

  // Returns a unique name for the 3 corners with a rotation corresponding to the swapping of U and R faces.
  static std::string GetCornersNameSwapFaces(std::array<Corner, 3> corners);
  // Returns the corner corresponding to a rotation swapping of U and R faces on the given corner.
  static Corner SwapFacesOnCorner(const Corner& corner);

  std::map<std::string, CubeStateSolver<Cube3x3x3>> solversByCorner;
};

#pragma once
