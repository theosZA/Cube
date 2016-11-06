#pragma once

#include <algorithm>
#include <array>
#include <vector>

#include "..\StickerPosition.h"
#include "..\Face.h"
#include "Cube3x3x3.h"

namespace CornerStructure {

// For a corner returns the sticker on one of the other faces of that cubie,
// going either clockwise or anti-clockwise as specified.
StickerPosition GetAdjacentCornerStickerPosition(StickerPosition, bool clockwise);

// Translate a corner sticker position into the three faces of that position
// in clockwise order starting from the specified sticker position.
std::array<Face, 3> StickerPositionToCorner(StickerPosition sticker);

// Returns true if the two positions specified are actually the same cubie.
bool AreStickersOnSameCubie(StickerPosition, StickerPosition);

// Returns true if the given position is actually the same cubie as one of the positions in the given sequence.
template <class StickerPositionIter>
bool AreStickersOnOneOfCubies(StickerPosition position, StickerPositionIter cubiesBegin, StickerPositionIter cubiesEnd)
{
  return std::find_if(cubiesBegin, cubiesEnd,
      [=](const StickerPosition& cubie)
      {
        return AreStickersOnSameCubie(position, cubie);
      }) != cubiesEnd;
}

// Returns the full cycle of corner cubie faces of a scrambled cube starting from the given corner.
std::vector<StickerPosition> GetCornerCycle(const Cube3x3x3& cube, StickerPosition startCorner);

} // CornerStructure