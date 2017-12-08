#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Cube\CubeMove.h"
#include "Cube\3x3x3\Cube3x3x3.h"

// A solver that fixes edge orientation on the U and R faces. Cubies not on the U and R faces are
// left unchanged.
class TwoFaceEO
{
public:
  // Initializes the solver with the move sequence required for all possible U/R edge orientations.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  TwoFaceEO(const std::string& cacheFileName = "", const std::string& cacheFileName2x2x2 = "");

  // Returns the move sequence that will orient the U and R edges.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will orient the U and R edges from the given scramble.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

private:
  // Generates solutions for all edge orientation cases.
  void GenerateSolutions(std::uint8_t maxMoves, const std::string& cacheFileName2x2x2);

  // Writes the solutions to a cache file.
  void WriteCacheFile(const std::string& cacheFileName) const;
  // Reads the solutions from a cache file written by WriteCacheFile().
  // Returns true only if the file was found and the set of solutions was populated.
  bool ReadCacheFile(const std::string& cacheFileName);

  // Returns a key value that distinguishes only based on the edge orientation of the U and R faces.
  static std::uint8_t GetKeyValue(const Cube3x3x3&);
  // Returns true if all cubies are in position except the U and R faces.
  static bool IsTwoFaceCase(const Cube3x3x3&);

  std::unordered_map<std::uint8_t, std::vector<CubeMove>> solutions;
};

