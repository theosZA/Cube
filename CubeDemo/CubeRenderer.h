#pragma once

#include <array>

#include <irrlicht.h>

#include "..\Cube\Cube2x2x2.h"

#include "CubieSceneNode.h"

class CubeRenderer
{
public:
  CubeRenderer(const Cube2x2x2& cube, irr::scene::ISceneNode* parent, irr::scene::ISceneManager*);

  void SetCube(const Cube2x2x2& cube);

private:
  std::array<CubieSceneNode*, 8> cubies;
};