#pragma once

#include <string>
#include <utility>
#include <vector>

#include "CubeMove.h"
#include "Face.h"
#include "StickerPosition.h"

// Defines an interface for cube-based puzzles.
class Cube
{
public:
  virtual ~Cube() {}

  // Applies a standard move to this cube.
  Cube& operator+=(const CubeMove& move);
  // Applies a standard move sequence.
  Cube& operator+=(const std::vector<CubeMove>& moves);
  // Applies the move sequence provided in standard notation with whitespace to separate moves.
  Cube& operator+=(const std::string& moves);

  // Looking at the specified position, which face of which cubie is in that position?
  StickerPosition operator[](StickerPosition) const;
  // Finds the position of the cubie that should go in a specified position.
  StickerPosition Find(StickerPosition) const;

protected:
  // Returns the number of cubies along each side.
  virtual size_t GetSize() const = 0;

  // Rotates the specified face n steps clockwise, e.g. F' is done by ApplyMove(Face::Front, -1).
  virtual void ApplyMove(Face, int quarterRotationsClockwise) = 0;

  // Returns the specific face of the cubie in the specified position.
  virtual std::pair<Face, size_t> Get(Face, size_t index) const = 0;
};
