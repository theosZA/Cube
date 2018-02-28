#pragma once

#include <array>

#include <irrlicht.h>

#include "Cube\CubeMove.h"

#include "CubieFaceMesh.h"

class Cubie
{
public:
  Cubie(irr::scene::ISceneManager&, irr::scene::ISceneNode* parent, const irr::core::vector3df& position, const std::array<irr::video::SColor, 6>& faces);

  void ApplyMove(Face, irr::f32 degreesClockwise, irr::f32 minDistanceFromCentre);

private:
  void ApplyRotation(const irr::core::vector3df& rotation);

  std::array<CubieFaceMesh, 6> cubieFaces;
  irr::scene::ISceneNode* positionNode;
  irr::scene::ISceneNode* rotationNode;
  std::array<irr::scene::ISceneNode*, 6> faceSceneNodes;
};

