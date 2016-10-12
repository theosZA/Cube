#pragma once

#include <array>

#include <irrlicht.h>

#include "CubieFaceMesh.h"

#include "..\Cube\CubeMove.h"

class Cubie
{
public:
  Cubie(irr::scene::ISceneManager*, irr::scene::ISceneNode* parent, const irr::core::vector3df& position, const std::array<irr::video::SColor, 6>& faces);

  void ApplyMove(CubeMove);

private:
  void ApplyRotation(const irr::core::vector3df& rotation);

  std::array<CubieFaceMesh, 6> cubieFaces;
  irr::scene::ISceneNode* positionNode;
  irr::scene::ISceneNode* rotationNode;
  std::array<irr::scene::ISceneNode*, 6> faceSceneNodes;
};

