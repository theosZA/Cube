#pragma once

#include <memory>
#include <vector>

#include <irrlicht.h>

#include "Cubie.h"

#include "..\Cube\Face.h"

class CubeRenderer
{
public:
  CubeRenderer(irr::scene::ISceneManager*, irr::scene::ISceneNode* parent, irr::f32 totalSize, size_t cubeSize);

  void ApplyMove(Face, int quarterRotationsClockwise, int layers);

private:
  size_t cubeSize;
  irr::scene::ISceneNode* scaleNode;
  std::vector<std::unique_ptr<Cubie>> cubies;
};