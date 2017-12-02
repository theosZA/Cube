#include "SolverF2LMinusBackSlot.h"

#include <array>

#include "..\StickerPosition.h"

static const std::array<StickerPosition, 3> lastBlockCubies
{
  StickerPosition{ Face::Right, 3 },
  StickerPosition{ Face::Right, 6 },
  StickerPosition{ Face::Right, 7 }
};

static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
};

SolverF2LMinusBackSlot::SolverF2LMinusBackSlot(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(
    maxMoves,
    // Get key
    [](const Cube3x3x3& cube)
    {
      return SolverF2LMinusBackSlot::GetKeyValue(cube);
    },
    possibleMoves,
    cacheFileName)
{}

std::vector<CubeMove> SolverF2LMinusBackSlot::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> SolverF2LMinusBackSlot::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t SolverF2LMinusBackSlot::GetKeyValue(const Cube3x3x3& cube)
{
  // 2 edges + corner, each:
  //  3 bits = face
  //  4 bits = index
  std::uint32_t key = 0;
  for (const auto& cubie : lastBlockCubies)
  {
    key <<= 7;
    auto position = cube.Find(cubie);
    key += (static_cast<std::uint32_t>(position.face) << 4) + static_cast<std::uint32_t>(position.index);
  }
  return key;
}
