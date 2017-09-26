#include "SolverEO.h"

#include <algorithm>
#include <array>

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

static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 1 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Front, 2 },
  CubeMove{ Face::Left, 1 },
  CubeMove{ Face::Left, -1 },
  CubeMove{ Face::Left, 2 },
  CubeMove{ Face::Down, 1 },
  CubeMove{ Face::Down, -1 },
  CubeMove{ Face::Down, 2 },
  CubeMove{ Face::Back, 2 },
  CubeMove{ Face::Back, -1 },
  CubeMove{ Face::Back, 1 }
};

SolverEO::SolverEO(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(maxMoves,
          [](const Cube3x3x3& cube)
          {
            return SolverEO::GetKeyValue(cube);
          },
          possibleMoves,
          cacheFileName)
{}

std::vector<CubeMove> SolverEO::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> SolverEO::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t SolverEO::GetRequiredMoveCount(const Cube3x3x3& cube) const
{
  return solver.GetRequiredMoveCount(cube);
}

std::uint32_t SolverEO::GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return GetRequiredMoveCount(cube);
}

std::uint32_t SolverEO::GetKeyValue(const Cube3x3x3& cube)
{
  // 12 edges, each:
  //  1 bit = oriented correctly with respect to F/B
  std::uint32_t key = 0;
  for (const auto& edge : edgeCubies)
  {
    key <<= 1;
    auto cubie = cube[edge];
    if (std::find(edgeCubies.begin(), edgeCubies.end(), cubie) != edgeCubies.end())
    {
      key |= 1;
    }
  }
  return key;
}
