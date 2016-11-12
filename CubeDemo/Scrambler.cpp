#include "Scrambler.h"

Scrambler::Scrambler(int seed)
: generator(seed)
{}

std::vector<CubeMove> Scrambler::CreateRandomScramble(size_t scrambleLength)
{
  std::uniform_int_distribution<> faceDistribution(0, 5);
  std::uniform_int_distribution<> rotationDistribution(0, 2);
  std::vector<CubeMove> scramble(scrambleLength);
  for (size_t i = 0; i < scrambleLength; ++i)
  {
    do
      scramble[i].face = static_cast<Face>(faceDistribution(generator));
    while (i > 0 && scramble[i].face == scramble[i - 1].face);

    scramble[i].quarterRotationsClockwise = rotationDistribution(generator);
    if (scramble[i].quarterRotationsClockwise == 0)
      scramble[i].quarterRotationsClockwise = -1;
  }

  return scramble;
}
