#include "CubieFaceMesh.h"

using namespace irr;

CubieFaceMesh::CubieFaceMesh(video::SColor color)
{
  vertices[0] = video::S3DVertex(-0.5f, -0.5f, 0, -1, -1, -1, color, 0, 0);
  vertices[1] = video::S3DVertex(0.5f, -0.5f, 0, 1, -1, -1, color, 1, 0);
  vertices[2] = video::S3DVertex(0.5f, 0.5f, 0, 1, 1, -1, color, 1, 1);
  vertices[3] = video::S3DVertex(-0.5f, 0.5f, 0, -1, 1, -1, color, 0, 1);

  static const std::array<u16, 6> indices { 0, 1, 2, 2, 3, 0 };
  meshBuffer.append(vertices.data(), static_cast<u32>(vertices.size()), indices.data(), static_cast<u32>(indices.size()));
  mesh.addMeshBuffer(&meshBuffer);
}
