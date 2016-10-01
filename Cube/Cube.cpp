#include "Cube.h"

#include <sstream>

Cube& Cube::operator+=(const CubeMove& move)
{
  ApplyMove(move.face, move.quarterRotationsClockwise);
  return *this;
}

Cube& Cube::operator+=(const std::vector<CubeMove>& moves)
{
  for (const auto& move : moves)
    ApplyMove(move.face, move.quarterRotationsClockwise);
  return *this;
}

Cube& Cube::operator+=(const std::string& moves)
{
  return *this += ParseMoveSequence(moves);
}

StickerPosition Cube::operator[](StickerPosition position) const
{
  auto cubie = Get(position.face, position.index);
  return StickerPosition{ cubie.first, cubie.second };
}

StickerPosition Cube::Find(StickerPosition cubie) const
{
  auto size = GetSize();
  auto cubiesPerSide = size * size;
  for (size_t face = 0; face < 6; ++face)
    for (size_t index = 0; index < cubiesPerSide; ++index)
    {
      if ((*this)[StickerPosition{ static_cast<Face>(face), index }] == cubie)
        return StickerPosition{ static_cast<Face>(face), index };
    }
  std::ostringstream errorMessage;
  errorMessage << "Cubie for face " << GetFaceChar(static_cast<Face>(cubie.face)) << " position " << cubie.index << " not found";
  throw std::runtime_error(errorMessage.str());
}
