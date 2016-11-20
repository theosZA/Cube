#include "TwoFaceAB3C.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "CornerStructure.h"
#include "..\StickerPosition.h"

bool TwoFaceAB3C::Corner::operator<(const TwoFaceAB3C::Corner& comp) const
{
  return cornerFace < comp.cornerFace ||
        (cornerFace == comp.cornerFace && cornerFaceClockwise < comp.cornerFaceClockwise);
}

TwoFaceAB3C::TwoFaceAB3C(std::uint32_t maxMoves, const std::string& cacheFileNamePrefix)
{
  const std::array<Corner, 6> corners{
    Corner{ Face::Up, Face::Front },
    Corner{ Face::Up, Face::Left },
    Corner{ Face::Up, Face::Back },
    Corner{ Face::Up, Face::Right },
    Corner{ Face::Right, Face::Back },
    Corner{ Face::Right, Face::Down }
  };
  auto dotPos = cacheFileNamePrefix.rfind('.');
  auto prefix = cacheFileNamePrefix.substr(0, dotPos);
  auto extension = cacheFileNamePrefix.substr(dotPos);
  for (size_t i = 0; i < corners.size(); ++i)
    for (size_t j = i + 1; j < corners.size(); ++j)
      for (size_t k = j + 1; k < corners.size(); ++k)
      {
        std::array<Corner, 3> currentCorners{
          corners[i],
          corners[j],
          corners[k]
        };
        if (solversByCorner.find(GetCornersNameSwapFaces(currentCorners)) == solversByCorner.end())
        {
          solversByCorner.emplace(GetCornersName(currentCorners),
            CubeStateSolver<Cube3x3x3>(maxMoves,
              // Get key
              [currentCorners](const Cube3x3x3& cube)
              {
                return TwoFaceAB3C::GetKeyValue(cube, currentCorners);
              },
              std::vector<CubeMove> {
                CubeMove{ Face::Right, 1 },
                CubeMove{ Face::Right, -1 },
                CubeMove{ Face::Right, 2 },
                CubeMove{ Face::Up, 1 },
                CubeMove{ Face::Up, -1 },
                CubeMove{ Face::Up, 2 }
              },
              prefix + "-" + GetCornersName(currentCorners) + extension));
        }
      }
}

std::vector<CubeMove> TwoFaceAB3C::Solve(const Cube3x3x3& cube, const std::array<Corner, 3>& corners) const
{
  auto solverIter = solversByCorner.find(GetCornersName(corners));
  if (solverIter != solversByCorner.end())
  {
    return solverIter->second.Solve(cube);
  }
  solverIter = solversByCorner.find(GetCornersNameSwapFaces(corners));
  return Rotate(solverIter->second.Solve(cube), std::pair<Face, Face>{ Face::Up, Face::Right }, std::pair<Face, Face>{ Face::Right, Face::Up });
}

std::vector<CubeMove> TwoFaceAB3C::Solve(const std::vector<CubeMove>& scramble, const std::array<Corner, 3>& corners) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube, corners);
}

bool TwoFaceAB3C::CanSolve(const std::vector<CubeMove>& scramble, const std::array<Corner, 3>& corners) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return solversByCorner.find(GetCornersName(corners))->second.CanSolve(cube);
}

std::uint32_t TwoFaceAB3C::GetKeyValue(const Cube3x3x3& cube, const std::array<Corner, 3>& corners)
{
  return (GetCornersKeyValue(cube, corners) << 16) + GetEdgesKeyValue(cube);
}

std::uint32_t TwoFaceAB3C::GetCornersKeyValue(const Cube3x3x3& cube, std::array<Corner, 3> corners)
{
  // 3 corners, each:
  //  3 bits: 1 of 6 faces
  //  2 bits: 1 of 4 possible corner positions on that face

  std::sort(corners.begin(), corners.end());

  std::uint32_t key = 0;
  for (size_t i = 0; i < 3; ++i)
  {
    auto cubie = CornerStructure::CornerToStickerPosition(corners[i].cornerFace, corners[i].cornerFaceClockwise);
    auto cornerPosition = cube.Find(cubie);
    key <<= 3;
    key += static_cast<std::uint32_t>(cornerPosition.face);
    key <<= 2;
    // 0 -> 0, 2 -> 1, 6 -> 2, 8 -> 3
    key += (static_cast<std::uint32_t>(cornerPosition.index) + 1) / 3;
  }
  return key;
}

std::uint32_t TwoFaceAB3C::GetEdgesKeyValue(const Cube3x3x3& cube)
{
  // 5 edges, each:
  //  3 bits: 1 of 7 positions
  // 2 edges, totalling:
  //  1 bit: 1 of 2 remaining permutations

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
  std::uint32_t key = 0;

  for (size_t i = 0; i < 5; ++i)
  {
    key <<= 3;
    auto edge = cube[edges[i]];
    auto edgeIter = std::find(edges.begin(), edges.end(), edge);
    key += static_cast<std::uint32_t>(std::distance(edges.begin(), edgeIter));
  }

  key <<= 1;
  if (cube[edges[5]] < cube[edges[6]])
    key += 1;

  return key;
}

std::string TwoFaceAB3C::GetCornersName(std::array<Corner, 3> corners)
{
  // Convert corners to a canonical representation - this is one with the U or D face first.
  for (auto& corner : corners)
    if (corner.cornerFace != Face::Up && corner.cornerFace != Face::Down)
    {
      auto face3 = GetNextFaceClockwise(corner.cornerFace, corner.cornerFaceClockwise);
      if (corner.cornerFaceClockwise == Face::Up || corner.cornerFaceClockwise == Face::Down)
      {
        corner.cornerFace = corner.cornerFaceClockwise;
        corner.cornerFaceClockwise = face3;
      }
      else
      {
        corner.cornerFaceClockwise = corner.cornerFace;
        corner.cornerFace = face3;
      }
    }
  std::sort(corners.begin(), corners.end());

  std::ostringstream cornersName;
  cornersName << GetCornerName(corners[0]) << '-' << GetCornerName(corners[1]) << '-' << GetCornerName(corners[2]);
  return cornersName.str();
}

std::string TwoFaceAB3C::GetCornerName(const Corner& corner)
{
  std::array<Face, 3> faces{ corner.cornerFace, corner.cornerFaceClockwise, GetNextFaceClockwise(corner.cornerFace, corner.cornerFaceClockwise) };
  auto firstFaceIter = std::min_element(faces.begin(), faces.end());
  std::rotate(faces.begin(), firstFaceIter, faces.end());
  std::string name;
  std::transform(faces.begin(), faces.end(), std::back_inserter(name), &GetFaceChar);
  return name;
}

std::string TwoFaceAB3C::GetCornersNameSwapFaces(std::array<Corner, 3> corners)
{
  return GetCornersName(std::array<Corner, 3>{
    SwapFacesOnCorner(corners[0]),
    SwapFacesOnCorner(corners[1]),
    SwapFacesOnCorner(corners[2])
  });
}

TwoFaceAB3C::Corner TwoFaceAB3C::SwapFacesOnCorner(const Corner& corner)
{
  static const std::array<Face, 6> newFaces{
    Face::Left,
    Face::Right,
    Face::Back,
    Face::Up,
    Face::Front,
    Face::Down
  };
  return Corner{
    newFaces[static_cast<size_t>(corner.cornerFace)],
    newFaces[static_cast<size_t>(corner.cornerFaceClockwise)]
  };
}
