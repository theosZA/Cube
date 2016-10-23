#include "Solver2x2x2.h"

#include <map>
#include <utility>

// We only include F, U and R moves since B, D and L are equivalent in a 2x2x2 by symmetry.
const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 1 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Front, 2 }
};

Solver2x2x2::Solver2x2x2(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(maxMoves, 
         [](const Cube2x2x2& cube) { return cube.GetKey(); },
         possibleMoves,
         cacheFileName)
{}

std::vector<CubeMove> Solver2x2x2::Solve(Cube2x2x2 cube) const
{
  std::map<Face, Face> faceMapping;
  auto downFace = cube[StickerPosition{ Face::Down, 2 }].face;
  auto backFace = cube[StickerPosition{ Face::Back, 3 }].face;
  faceMapping[downFace] = Face::Down;
  faceMapping[GetOppositeFace(downFace)] = Face::Up;
  faceMapping[backFace] = Face::Back;
  faceMapping[GetOppositeFace(backFace)] = Face::Front;
  faceMapping[GetNextFaceClockwise(downFace, backFace)] = Face::Left;
  faceMapping[GetOppositeFace(GetNextFaceClockwise(downFace, backFace))] = Face::Right;
  cube.RemapFaces(faceMapping[Face::Front], faceMapping[Face::Up]);

  return solver.Solve(cube);
}

std::vector<CubeMove> Solver2x2x2::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube2x2x2 cube;
  cube += scramble;
  return Solve(cube);
}
