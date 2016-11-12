#pragma once

#include <random>
#include <vector>

#include "..\Cube\CubeMove.h"

// A random move scrambler for cubes.
class Scrambler 
{

public:
  // Initializes the random number generator of the scrambler with the given seed.
  Scrambler(int seed);

  // Returns a new random scramble.
  std::vector<CubeMove> CreateRandomScramble(size_t scrambleLength);

private:
  std::mt19937 generator;
};