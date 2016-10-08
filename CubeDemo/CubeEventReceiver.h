#pragma once

#include <irrlicht.h>

#include "CubeRenderer.h"

class CubeEventReceiver : public irr::IEventReceiver
{
public:
  CubeEventReceiver(irr::IrrlichtDevice* device, Cube2x2x2&, CubeRenderer&, const std::vector<CubeMove>& moves);
  
  bool OnEvent(const irr::SEvent& event) override;

private:
  irr::IrrlichtDevice* device;
  Cube2x2x2& cube;
  CubeRenderer& cubeRenderer;
  std::vector<CubeMove> moves;
  size_t moveIndex = 0;
};