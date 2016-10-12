#pragma once

#include <array>

#include <irrlicht.h>

class CubieFaceMesh
{
public:
  CubieFaceMesh(irr::video::SColor color);

  irr::scene::SMesh& GetMesh() { return mesh; }
  const irr::scene::SMesh& GetMesh() const { return mesh; }

private:
  std::array<irr::video::S3DVertex, 4> vertices;
  irr::scene::SMeshBuffer meshBuffer;
  irr::scene::SMesh mesh;
};