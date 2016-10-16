#pragma once

#include "..\Cube.h"

class Cube3x3x3 : public Cube
{
public:
  // Constructor starts from solved state with each face corresponding to their expected face.
  Cube3x3x3();

  void RemapFaces(Face newFront, Face newUp) override;

private:
  size_t GetSize() const override { return 3; }
  void ApplyMove(Face, int quarterRotationsClockwise) override;
  std::pair<Face, size_t> Get(Face, size_t index) const override;
};