#include "Cube3x3x3.h"

Cube3x3x3::Cube3x3x3()
{
  // TBD
}

void Cube3x3x3::RemapFaces(Face newFront, Face newUp)
{
  // TBD
}

void Cube3x3x3::ApplyMove(Face, int quarterRotationsClockwise)
{
  // TBD
}

std::pair<Face, size_t> Cube3x3x3::Get(Face face, size_t index) const
{
  return std::pair<Face, size_t>{ face, index };
}
