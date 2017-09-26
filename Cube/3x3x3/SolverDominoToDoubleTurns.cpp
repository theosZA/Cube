#include "SolverDominoToDoubleTurns.h"

#include <algorithm>
#include <array>

static const std::array<StickerPosition, 4> mEdgeCubies
{
  StickerPosition{ Face::Up, 1 },
  StickerPosition{ Face::Up, 7 },
  StickerPosition{ Face::Down, 1 },
  StickerPosition{ Face::Down, 7 },
};

static const std::array<StickerPosition, 4> sEdgeCubies
{
  StickerPosition{ Face::Up, 3 },
  StickerPosition{ Face::Up, 5 },
  StickerPosition{ Face::Down, 3 },
  StickerPosition{ Face::Down, 5 },
};

static const std::array<StickerPosition, 4> orbit1CornerCubies
{
  StickerPosition{ Face::Up, 0 },
  StickerPosition{ Face::Up, 8 },
  StickerPosition{ Face::Down, 0 },
  StickerPosition{ Face::Down, 8 },
};

static const std::array<StickerPosition, 4> orbit2CornerCubies
{
  StickerPosition{ Face::Up, 2 },
  StickerPosition{ Face::Up, 6 },
  StickerPosition{ Face::Down, 2 },
  StickerPosition{ Face::Down, 6 },
};

// Because of the domino state we can't do quarter turns on the F, B, L and R faces.
static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 2 },
  CubeMove{ Face::Left, 2 },
  CubeMove{ Face::Down, 1 },
  CubeMove{ Face::Down, -1 },
  CubeMove{ Face::Down, 2 },
  CubeMove{ Face::Back, 2 },
};

SolverDominoToDoubleTurns::SolverDominoToDoubleTurns(std::uint32_t maxMoves, const std::string& cacheFileName)
  : solver(maxMoves,
            [](const Cube3x3x3& cube)
            {
              return SolverDominoToDoubleTurns::GetKeyValue(cube);
            },
            possibleMoves,
            cacheFileName)
{}

std::vector<CubeMove> SolverDominoToDoubleTurns::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> SolverDominoToDoubleTurns::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t SolverDominoToDoubleTurns::GetRequiredMoveCount(const Cube3x3x3& cube) const
{
  return solver.GetRequiredMoveCount(cube);
}

std::uint32_t SolverDominoToDoubleTurns::GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return GetRequiredMoveCount(cube);
}

std::uint32_t SolverDominoToDoubleTurns::GetKeyValue(const Cube3x3x3& cube)
{
  // 8 edges, each:
  //  1 bit = in correct slice
  // 8 corners, each:
  //  1 bit = in correct orbit
  std::uint32_t key = 0;
  for (const auto& edge : mEdgeCubies)
  {
    key <<= 1;
    auto cubie = cube[edge];
    if (std::find(mEdgeCubies.begin(), mEdgeCubies.end(), cubie) != mEdgeCubies.end())
    {
      key |= 1;
    }
  }
  for (const auto& edge : sEdgeCubies)
  {
    key <<= 1;
    auto cubie = cube[edge];
    if (std::find(sEdgeCubies.begin(), sEdgeCubies.end(), cubie) != sEdgeCubies.end())
    {
      key |= 1;
    }
  }
  for (const auto& corner : orbit1CornerCubies)
  {
    key <<= 1;
    auto cubie = cube[corner];
    if (std::find(orbit1CornerCubies.begin(), orbit1CornerCubies.end(), cubie) != orbit1CornerCubies.end())
    {
      key |= 1;
    }
  }
  for (const auto& corner : orbit2CornerCubies)
  {
    key <<= 1;
    auto cubie = cube[corner];
    if (std::find(orbit2CornerCubies.begin(), orbit2CornerCubies.end(), cubie) != orbit2CornerCubies.end())
    {
      key |= 1;
    }
  }
  return key;
}
