#pragma once

#include <cstdint>

#include "Cube.h"

class Cube2x2x2 : public Cube
{
public:
  Cube2x2x2();

private:
  size_t GetSize() const override;
  void ApplyMove(Face, int quarterRotationsClockwise) override;
  std::pair<Face, size_t> Get(Face, size_t index) const override;

  // Returns the cubie at the specified position.
  std::uint32_t GetCubie(std::uint32_t positionIndex) const;
  // Returns the orientation of the cubie at the specified position. (0 = none, 1 = clockwise, 2 = anti-clockwise)
  std::uint32_t GetOrientation(std::uint32_t positionIndex) const;

  // Sets the cubie at the specified position.
  void SetCubie(std::uint32_t positionIndex, std::uint32_t cubie);
  // Sets the orientation of the cubie in the specified position. (0 = none, 1 = clockwise, 2 = anti-clockwise)
  void SetOrientation(std::uint32_t positionIndex, std::uint32_t orientation);

  std::uint32_t state;
};