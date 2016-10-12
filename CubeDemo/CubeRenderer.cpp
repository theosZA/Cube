#include "CubeRenderer.h"

#include <stdexcept>

#include "..\Cube\Face.h"

using namespace irr;

const auto cNone = video::SColor(255, 0, 0, 0);
const auto cDown = video::SColor(255, 255, 255, 255);
const auto cUp = video::SColor(255, 255, 255, 0);
const auto cFront = video::SColor(255, 0, 0, 255);
const auto cRight = video::SColor(255, 255, 0, 0);
const auto cBack = video::SColor(255, 0, 255, 0);
const auto cLeft = video::SColor(255, 255, 127, 0);

CubeRenderer::CubeRenderer(scene::ISceneManager* manager, scene::ISceneNode* parent)
: cubies
  {
    Cubie(manager, parent, core::vector3df{ +5.1f, -5.1f, +5.1f }, std::array<irr::video::SColor, 6>{ cDown, cNone, cFront, cNone, cNone, cLeft }),
    Cubie(manager, parent, core::vector3df{ -5.1f, -5.1f, +5.1f }, std::array<irr::video::SColor, 6>{ cDown, cNone, cFront, cRight, cNone, cNone }),
    Cubie(manager, parent, core::vector3df{ +5.1f, -5.1f, -5.1f }, std::array<irr::video::SColor, 6>{ cDown, cNone, cNone, cNone, cBack, cLeft }),
    Cubie(manager, parent, core::vector3df{ -5.1f, -5.1f, -5.1f }, std::array<irr::video::SColor, 6>{ cDown, cNone, cNone, cRight, cBack, cNone }),
    Cubie(manager, parent, core::vector3df{ +5.1f, +5.1f, -5.1f }, std::array<irr::video::SColor, 6>{ cNone, cUp, cNone, cNone, cBack, cLeft }),
    Cubie(manager, parent, core::vector3df{ -5.1f, +5.1f, -5.1f }, std::array<irr::video::SColor, 6>{ cNone, cUp, cNone, cRight, cBack, cNone }),
    Cubie(manager, parent, core::vector3df{ +5.1f, +5.1f, +5.1f }, std::array<irr::video::SColor, 6>{ cNone, cUp, cFront, cNone, cNone, cLeft }),
    Cubie(manager, parent, core::vector3df{ -5.1f, +5.1f, +5.1f }, std::array<irr::video::SColor, 6>{ cNone, cUp, cFront, cRight, cNone, cNone })
  }
{}

void CubeRenderer::ApplyMove(CubeMove move)
{
  for (auto& cubie : cubies)
    cubie.ApplyMove(move);
}
