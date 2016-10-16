#pragma once

#include <utility>
#include <cstdint>

#include "..\Face.h"

// Represents the edge cubies of a 3x3x3.
class Edges3x3x3
{
public:
  Edges3x3x3();

  void ApplyMove(Face, int quarterRotationsClockwise);
  std::pair<Face, size_t> Get(Face, size_t index) const;

private:
  // Returns the cubie at the specified position.
  std::uint64_t GetCubie(std::uint32_t positionIndex) const;
  // Returns the orientation of the cubie at the specified position. False is unflipped with respect to (F, B, U, D, L2, R2).
  bool GetOrientation(std::uint32_t positionIndex) const;

  // Sets the cubie at the specified position.
  void SetCubie(std::uint32_t positionIndex, std::uint64_t cubie);
  // Sets the orientation of the cubie in the specified position. False is unflipped with respect to (F, B, U, D, L2, R2).
  void SetOrientation(std::uint32_t positionIndex, bool orientation);

  std::uint64_t state;
};