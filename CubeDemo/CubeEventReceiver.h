#pragma once

#include <vector>

#include <irrlicht.h>

#include "CubeRenderer.h"

#include "..\Cube\CubeMove.h"

class CubeEventReceiver : public irr::IEventReceiver
{
public:
  CubeEventReceiver(irr::IrrlichtDevice* device, CubeRenderer&, const std::vector<CubeMove>& moves);
  
  bool OnEvent(const irr::SEvent& event) override;

private:
  irr::IrrlichtDevice* device;
  CubeRenderer& cubeRenderer;
  std::vector<CubeMove> moves;
  size_t moveIndex = 0;
};