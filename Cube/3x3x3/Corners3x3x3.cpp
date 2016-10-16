#include "Corners3x3x3.h"

#include "..\CubeMove.h"
#include "..\StickerPosition.h"

const size_t cornerIndexToCubeIndexMapping[] { 0, 2, 6, 8 };
const size_t cubeIndexToCornerIndexMapping[] { 0, 255, 1, 255, 255, 255, 2, 255, 3 };

void Corners3x3x3::ApplyMove(Face face, int quarterRotationsClockwise)
{
  corners += CubeMove{ face, quarterRotationsClockwise };
}

std::pair<Face, size_t> Corners3x3x3::Get(Face face, size_t index) const
{
  auto result = corners[StickerPosition{ face, cubeIndexToCornerIndexMapping[index] }];
  return std::pair<Face, size_t>{ result.face, cornerIndexToCubeIndexMapping[result.index] };
}
