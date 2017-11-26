#include "Scrambler.h"

Scrambler::Scrambler(int seed)
: generator(seed)
{}

std::vector<CubeMove> Scrambler::CreateRandomScramble(size_t scrambleLength)
{
  std::uniform_int_distribution<> faceDistribution(0, 5);
  std::uniform_int_distribution<> rotationDistribution(0, 2);
  std::vector<CubeMove> scramble;
  while (scramble.size() < scrambleLength)
  {
    auto face = static_cast<Face>(faceDistribution(generator));
    auto rotations = rotationDistribution(generator);
    if (rotations == 0)
      rotations = -1;
    scramble.push_back(CubeMove{ face, rotations });
    scramble = SimplifyMoveSequence(scramble);
  }

  return scramble;
}
