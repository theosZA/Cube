#include "SolverEdgesBackSlot.h"

#include <algorithm>
#include <array>

#include "..\StickerPosition.h"

// We only have 5 more edges to solve, but the 2x2x1 block already made on the R face will
// temporarily be disturbed, so we need to ensure those 3 cubies are also returned to the
// correct position. This gives us a total of 7 edges and 1 corner to track.
static const std::array<StickerPosition, 7> edgeCubies
{
  StickerPosition{ Face::Up, 1 },
  StickerPosition{ Face::Up, 3 },
  StickerPosition{ Face::Up, 5 },
  StickerPosition{ Face::Up, 7 },
  StickerPosition{ Face::Front, 5 },
  StickerPosition{ Face::Back, 3 },
  StickerPosition{ Face::Down, 5 }
};

static const StickerPosition cornerCubie{ Face::Right, 6 };

// If the BR edge is solved, then B face moves would also be possible, but this would greatly
// complicate the algorithm, and can't be handled by the current solver logic.
static const std::vector<CubeMove> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
};

SolverEdgesBackSlot::SolverEdgesBackSlot(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(
    maxMoves,
    // Get key
    [](const Cube3x3x3& cube)
    {
      return SolverEdgesBackSlot::GetKeyValue(cube);
    },
    possibleMoves,
    cacheFileName)
{}

std::vector<CubeMove> SolverEdgesBackSlot::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> SolverEdgesBackSlot::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t SolverEdgesBackSlot::GetKeyValue(const Cube3x3x3& cube)
{
  // 7 edges, each:
  //  3 bits = position (0-6 corresponding to position in array)
  // 1 corner:
  //  3 bits = face
  //  4 bits = index
  std::uint32_t key = 0;
  for (const auto& cubie : edgeCubies)
  {
    key <<= 3;
    auto position = cube.Find(cubie);
    auto arrayIndex = std::distance(edgeCubies.begin(), std::find(edgeCubies.begin(), edgeCubies.end(), position));
    key += static_cast<std::uint32_t>(arrayIndex);
  }
  key <<= 7;
  auto position = cube.Find(cornerCubie);
  key += (static_cast<std::uint32_t>(position.face) << 4) + static_cast<std::uint32_t>(position.index);
  return key;
}
