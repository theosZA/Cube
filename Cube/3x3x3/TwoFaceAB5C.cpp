#include "TwoFaceAB5C.h"

#include <algorithm>
#include <array>

#include "..\StickerPosition.h"

TwoFaceAB5C::TwoFaceAB5C(std::uint32_t maxMoves, const std::string& cacheFileName)
: solver(
    maxMoves,
    // Get key
    [](const Cube3x3x3& cube)
    {
      return TwoFaceAB5C::GetKeyValue(cube);
    },
    std::vector<CubeMove>
    {
      CubeMove{ Face::Right, 1 },
      CubeMove{ Face::Right, -1 },
      CubeMove{ Face::Right, 2 },
      CubeMove{ Face::Up, 1 },
      CubeMove{ Face::Up, -1 },
      CubeMove{ Face::Up, 2 }
    },
    cacheFileName)
{}

std::vector<CubeMove> TwoFaceAB5C::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> TwoFaceAB5C::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::uint32_t TwoFaceAB5C::GetKeyValue(const Cube3x3x3& cube)
{
  // Corner:
  // 3 bits = face
  // 4 bits = index
  // 7 edges, each:
  //  3 bits = 1 of 7 positions

  auto cornerPosition = cube.Find(StickerPosition{ Face::Right, 8 });
  std::uint32_t key = static_cast<std::uint32_t>(cornerPosition.face);
  key <<= 4;
  key += static_cast<std::uint32_t>(cornerPosition.index);

  static const std::array<StickerPosition, 7> edges
  {
    StickerPosition{ Face::Up, 1 },
    StickerPosition{ Face::Up, 3 },
    StickerPosition{ Face::Up, 5 },
    StickerPosition{ Face::Up, 7 },
    StickerPosition{ Face::Front, 5 },
    StickerPosition{ Face::Back, 3 },
    StickerPosition{ Face::Down, 5 },
  };
  for (const auto& edgePosition : edges)
  {
    key <<= 3;
    auto edge = cube[edgePosition];
    auto edgeIter = std::find(edges.begin(), edges.end(), edge);
    key += static_cast<std::uint32_t>(std::distance(edges.begin(), edgeIter));
  }
  return key;
}
