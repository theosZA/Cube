#pragma once

#include <utility>

#include "..\Face.h"
#include "..\2x2x2\Cube2x2x2.h"

// Represents the corner cubies of a 3x3x3.
class Corners3x3x3
{
public:
  void ApplyMove(Face, int quarterRotationsClockwise);
  std::pair<Face, size_t> Get(Face, size_t index) const;

private:
  Cube2x2x2 corners;
};