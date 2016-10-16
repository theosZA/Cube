#include "Cube3x3x3.h"

Cube3x3x3::Cube3x3x3()
{}

void Cube3x3x3::RemapFaces(Face newFront, Face newUp)
{
  // TBD
}

void Cube3x3x3::ApplyMove(Face face, int quarterRotationsClockwise)
{
  corners.ApplyMove(face, quarterRotationsClockwise);
  edges.ApplyMove(face, quarterRotationsClockwise);
}

std::pair<Face, size_t> Cube3x3x3::Get(Face face, size_t index) const
{
  if (index == 4)
    return std::pair<Face, size_t>{ face, 4 };
  else if (index % 2 == 0)
    return corners.Get(face, index);
  else
    return edges.Get(face, index);
}
