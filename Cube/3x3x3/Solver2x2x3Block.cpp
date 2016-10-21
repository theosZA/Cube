#include "Solver2x2x3Block.h"

#include <array>

#include "..\StickerPosition.h"

static const std::array<StickerPosition, 3> extensionCubies
{
  StickerPosition{ Face::Front, 3 },
  StickerPosition{ Face::Front, 6 },
  StickerPosition{ Face::Front, 7 }
};

static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 2 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Front, 1 }
};

Solver2x2x3Block::Solver2x2x3Block(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(
    maxMoves,
    // Get key
    [](const Cube3x3x3& cube)
    {
      return Solver2x2x3Block::GetKeyValue(cube);
    },
    possibleMoves,
    cacheFileName)
{}

std::vector<CubeMove> Solver2x2x3Block::Solve(const Cube3x3x3& cube)
{
  return solver.Solve(cube);
}

std::vector<CubeMove> Solver2x2x3Block::Solve(const std::vector<CubeMove>& scramble)
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t Solver2x2x3Block::GetKeyValue(const Cube3x3x3& cube)
{
  // 2 edges + corner, each:
  //  3 bits = face
  //  4 bits = index
  std::uint32_t key = 0;
  for (const auto& cubie : extensionCubies)
  {
    key <<= 7;
    auto position = cube.Find(cubie);
    key += (static_cast<std::uint32_t>(position.face) << 4) + static_cast<std::uint32_t>(position.index);
  }
  return key;
}
