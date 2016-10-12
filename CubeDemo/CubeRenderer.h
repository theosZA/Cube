#pragma once

#include <array>

#include <irrlicht.h>

#include "Cubie.h"

#include "..\Cube\CubeMove.h"

class CubeRenderer
{
public:
  CubeRenderer(irr::scene::ISceneManager*, irr::scene::ISceneNode* parent);

  void ApplyMove(CubeMove);

private:
  std::array<Cubie, 8> cubies;
};