#include "SolverDominoReduction.h"

#include <algorithm>
#include <array>

#include "CornerStructure.h"

static const std::array<StickerPosition, 4> equatorEdgeCubies
{
  StickerPosition{ Face::Front, 3 },
  StickerPosition{ Face::Front, 5 },
  StickerPosition{ Face::Back, 3 },
  StickerPosition{ Face::Back, 5 }
};

static const std::array<StickerPosition, 12> edgeCubies
{
  StickerPosition{ Face::Up, 1 },
  StickerPosition{ Face::Up, 3 },
  StickerPosition{ Face::Up, 5 },
  StickerPosition{ Face::Up, 7 },
  StickerPosition{ Face::Down, 1 },
  StickerPosition{ Face::Down, 3 },
  StickerPosition{ Face::Down, 5 },
  StickerPosition{ Face::Down, 7 },
  StickerPosition{ Face::Front, 3 },
  StickerPosition{ Face::Front, 5 },
  StickerPosition{ Face::Back, 3 },
  StickerPosition{ Face::Back, 5 }
};

static const std::array<StickerPosition, 8> cornerCubies
{
  StickerPosition{ Face::Up, 0 },
  StickerPosition{ Face::Up, 2 },
  StickerPosition{ Face::Up, 6 },
  StickerPosition{ Face::Up, 8 },
  StickerPosition{ Face::Down, 0 },
  StickerPosition{ Face::Down, 2 },
  StickerPosition{ Face::Down, 6 },
  StickerPosition{ Face::Down, 8 }
};

// Because of edge orientation we can't do quarter turns on the F and B faces.
static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 2 },
  CubeMove{ Face::Left, 1 },
  CubeMove{ Face::Left, -1 },
  CubeMove{ Face::Left, 2 },
  CubeMove{ Face::Down, 1 },
  CubeMove{ Face::Down, -1 },
  CubeMove{ Face::Down, 2 },
  CubeMove{ Face::Back, 2 },
};

SolverDominoReduction::SolverDominoReduction(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(maxMoves,
          [](const Cube3x3x3& cube)
          {
            return SolverDominoReduction::GetKeyValue(cube);
          },
          possibleMoves,
          cacheFileName)
{}

std::vector<CubeMove> SolverDominoReduction::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> SolverDominoReduction::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t SolverDominoReduction::GetRequiredMoveCount(const Cube3x3x3& cube) const
{
  return solver.GetRequiredMoveCount(cube);
}

std::uint32_t SolverDominoReduction::GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return GetRequiredMoveCount(cube);
}

std::uint32_t SolverDominoReduction::GetKeyValue(const Cube3x3x3& cube)
{
  // 12 edges, each:
  //  1 bit = contains an E slice edge
  // 8 corners, each:
  //  2 bits = orientation
  std::uint32_t key = 0;
  for (const auto& edge : edgeCubies)
  {
    key <<= 1;
    auto cubie = cube[edge];
    if (std::find(equatorEdgeCubies.begin(), equatorEdgeCubies.end(), cubie) != equatorEdgeCubies.end())
    {
      key |= 1;
    }
  }
  for (const auto& corner : cornerCubies)
  {
    key <<= 2;
    auto cubie = CornerStructure::StickerPositionToCorner(cube[corner]);
    if (cubie[1] == Face::Up || cubie[1] == Face::Down)
    {
      key |= 1;
    }
    else if (cubie[2] == Face::Up || cubie[2] == Face::Down)
    {
      key |= 2;
    }
  }
  return key;
}
