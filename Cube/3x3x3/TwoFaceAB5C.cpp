#include "TwoFaceAB5C.h"

#include <algorithm>
#include <array>
#include <iterator>

#include "CornerStructure.h"
#include "..\StickerPosition.h"

TwoFaceAB5C::TwoFaceAB5C(std::uint32_t maxMoves, const std::string& cacheFileNamePrefix)
{
  const std::array<StickerPosition, 6> cornerStickers {
    StickerPosition{ Face::Right, 0 },
    StickerPosition{ Face::Right, 2 },
    StickerPosition{ Face::Right, 6 },
    StickerPosition{ Face::Right, 8 },
    StickerPosition{ Face::Up, 0 },
    StickerPosition{ Face::Up, 6 }
  };
  auto dotPos = cacheFileNamePrefix.rfind('.');
  auto prefix = cacheFileNamePrefix.substr(0, dotPos - 1);
  auto extension = cacheFileNamePrefix.substr(dotPos);
  for (const auto& cornerSticker : cornerStickers)
  {
    auto corner = CornerStructure::StickerPositionToCorner(cornerSticker);
    solversByCorner.emplace(GetCornerName(corner[0], corner[1]), 
      CubeStateSolver<Cube3x3x3>(maxMoves,
      // Get key
      [cornerSticker](const Cube3x3x3& cube)
      {
        return TwoFaceAB5C::GetKeyValue(cube, cornerSticker.face, cornerSticker.index);
      },
      std::vector<CubeMove> {
        CubeMove{ Face::Right, 1 },
        CubeMove{ Face::Right, -1 },
        CubeMove{ Face::Right, 2 },
        CubeMove{ Face::Up, 1 },
        CubeMove{ Face::Up, -1 },
        CubeMove{ Face::Up, 2 }
      },
      prefix + GetCornerName(corner[0], corner[1]) + extension));
  }
}

std::vector<CubeMove> TwoFaceAB5C::Solve(const Cube3x3x3& cube, Face cornerFace, Face cornerFaceClockwise) const
{
  return solversByCorner.find(GetCornerName(cornerFace, cornerFaceClockwise))->second.Solve(cube);
}

std::vector<CubeMove> TwoFaceAB5C::Solve(const std::vector<CubeMove>& scramble, Face cornerFace, Face cornerFaceClockwise) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube, cornerFace, cornerFaceClockwise);
}

std::uint32_t TwoFaceAB5C::GetKeyValue(const Cube3x3x3& cube, Face cornerFace, size_t cornerIndex)
{
  // Corner:
  // 3 bits = face
  // 4 bits = index
  // 7 edges, each:
  //  3 bits = 1 of 7 positions

  auto cornerPosition = cube.Find(StickerPosition{ cornerFace, cornerIndex });
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

std::string TwoFaceAB5C::GetCornerName(Face cornerFace, Face cornerFaceClockwise)
{
  std::array<Face, 3> faces { cornerFace, cornerFaceClockwise, GetNextFaceClockwise(cornerFace, cornerFaceClockwise) };
  auto firstFaceIter = std::min_element(faces.begin(), faces.end());
  std::rotate(faces.begin(), firstFaceIter, faces.end());
  std::string name;
  std::transform(faces.begin(), faces.end(), std::back_inserter(name), &GetFaceChar);
  return name;
}
