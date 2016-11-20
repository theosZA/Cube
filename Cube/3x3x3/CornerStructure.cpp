#include "CornerStructure.h"

#include <sstream>

namespace CornerStructure {

StickerPosition GetAdjacentCornerStickerPosition(StickerPosition position, bool clockwise)
{
  Face face = position.face;
  size_t index = position.index;
  switch (face)
  {
    case Face::Down:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Left, 8 };
          case 2: return StickerPosition{ Face::Front, 8 };
          case 6: return StickerPosition{ Face::Back, 8 };
          case 8: return StickerPosition{ Face::Right, 8 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Front, 6 };
          case 2: return StickerPosition{ Face::Right, 6 };
          case 6: return StickerPosition{ Face::Left, 6 };
          case 8: return StickerPosition{ Face::Back, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    case Face::Up:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Left, 0 };
          case 2: return StickerPosition{ Face::Back, 0 };
          case 6: return StickerPosition{ Face::Front, 0 };
          case 8: return StickerPosition{ Face::Right, 0 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Back, 2 };
          case 2: return StickerPosition{ Face::Right, 2 };
          case 6: return StickerPosition{ Face::Left, 2 };
          case 8: return StickerPosition{ Face::Front, 2 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    case Face::Front:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Left, 2 };
          case 2: return StickerPosition{ Face::Up, 8 };
          case 6: return StickerPosition{ Face::Down, 0 };
          case 8: return StickerPosition{ Face::Right, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Up, 6 };
          case 2: return StickerPosition{ Face::Right, 0 };
          case 6: return StickerPosition{ Face::Left, 8 };
          case 8: return StickerPosition{ Face::Down, 2 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    case Face::Right:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Front, 2 };
          case 2: return StickerPosition{ Face::Up, 2 };
          case 6: return StickerPosition{ Face::Down, 2 };
          case 8: return StickerPosition{ Face::Back, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Up, 8 };
          case 2: return StickerPosition{ Face::Back, 0 };
          case 6: return StickerPosition{ Face::Front, 8 };
          case 8: return StickerPosition{ Face::Down, 8 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    case Face::Back:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Right, 2 };
          case 2: return StickerPosition{ Face::Up, 0 };
          case 6: return StickerPosition{ Face::Down, 8 };
          case 8: return StickerPosition{ Face::Left, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Up, 2 };
          case 2: return StickerPosition{ Face::Left, 0 };
          case 6: return StickerPosition{ Face::Right, 8 };
          case 8: return StickerPosition{ Face::Down, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    case Face::Left:
      if (clockwise)
        switch (index)
        {
          case 0: return StickerPosition{ Face::Back, 2 };
          case 2: return StickerPosition{ Face::Up, 6 };
          case 6: return StickerPosition{ Face::Down, 6 };
          case 8: return StickerPosition{ Face::Front, 6 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }
      else // anti-clockwise
        switch (index)
        {
          case 0: return StickerPosition{ Face::Up, 0 };
          case 2: return StickerPosition{ Face::Front, 0 };
          case 6: return StickerPosition{ Face::Back, 8 };
          case 8: return StickerPosition{ Face::Down, 0 };
          default:
            throw std::domain_error("Invalid corner index " + std::to_string(index));
        }

    default:
      throw std::domain_error("Invalid face " + std::to_string(static_cast<int>(face)));
  }
}

std::array<Face, 3> StickerPositionToCorner(StickerPosition sticker)
{
  return std::array<Face, 3> {
    sticker.face,
    GetAdjacentCornerStickerPosition(sticker, true).face,
    GetAdjacentCornerStickerPosition(sticker, false).face
  };
}

StickerPosition CornerToStickerPosition(Face cornerFace, Face cornerFaceClockwise)
{
  const std::array<size_t, 4> cornerIndices{ 0, 2, 6, 8};
  for (auto index : cornerIndices)
  {
    StickerPosition stickerPosition{ cornerFace, index };
    if (GetAdjacentCornerStickerPosition(stickerPosition, true).face == cornerFaceClockwise)
      return stickerPosition;
  }
  std::ostringstream errorMessage;
  errorMessage << "Invalid corner specified by faces " << GetFaceChar(cornerFace) << " and " << GetFaceChar(cornerFaceClockwise);
  throw std::domain_error(errorMessage.str());
}

bool AreStickersOnSameCubie(StickerPosition pos1, StickerPosition pos2)
{
  return (pos1 == pos2) ||
         (GetAdjacentCornerStickerPosition(pos1, true) == pos2) ||
         (GetAdjacentCornerStickerPosition(pos1, false) == pos2);
}

std::vector<StickerPosition> GetCornerCycle(const Cube3x3x3& cube, StickerPosition startCorner)
{
  std::vector<StickerPosition> cycle;
  cycle.push_back(startCorner);
  do
  {
    cycle.push_back(cube[cycle.back()]);
  } while (cycle.back() != startCorner);
  cycle.pop_back();
  return cycle;
}

} // CornerStructure