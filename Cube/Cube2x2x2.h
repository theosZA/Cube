#pragma once

#include <cstdint>

#include "Cube.h"

class Cube2x2x2 : public Cube
{
public:
  // Constructor starts from solved state with each face corresponding to their expected face.
  Cube2x2x2();
  // Constructor from a unique key as returned by GetKey().
  Cube2x2x2(int key);

  // Returns a unique key specifying the position.
  int GetKey() const;

  void RemapFaces(Face newFront, Face newUp) override;

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

  // Sets the cubie at the specified position to have the given faces and have the required orientation.
  void SetCubieFaces(std::uint32_t positionIndex, const std::array<Face, 3>& newFaces);

  std::uint32_t state;
};