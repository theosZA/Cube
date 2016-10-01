#include "CubieSceneNode.h"

using namespace irr;

CubieSceneNode::CubieSceneNode(scene::ISceneNode* parent, scene::ISceneManager* sceneManager, s32 id, core::vector3df offset)
: ISceneNode(parent, sceneManager, id)
{
  material.Wireframe = false;
  material.Lighting = false;
  material.AntiAliasing = video::EAAM_FULL_BASIC;

  vertices[0] = video::S3DVertex(0 + offset.X, 0 + offset.Y, 0 + offset.Z, -1, -1, -1, video::SColor(255, 32, 32, 32), 0, 0);
  vertices[1] = video::S3DVertex(10 + offset.X, 0 + offset.Y, 0 + offset.Z, 1, -1, -1, video::SColor(255, 32, 32, 32), 1, 0);
  vertices[2] = video::S3DVertex(10 + offset.X, 10 + offset.Y, 0 + offset.Z, 1, 1, -1, video::SColor(255, 32, 32, 32), 1, 1);
  vertices[3] = video::S3DVertex(0 + offset.X, 10 + offset.Y, 0 + offset.Z, -1, 1, -1, video::SColor(255, 32, 32, 32), 0, 1);
  vertices[4] = video::S3DVertex(0 + offset.X, 0 + offset.Y, 10 + offset.Z, -1, -1, 1, video::SColor(255, 32, 32, 32), 1, 1);
  vertices[5] = video::S3DVertex(10 + offset.X, 0 + offset.Y, 10 + offset.Z, 1, -1, 1, video::SColor(255, 32, 32, 32), 0, 1);
  vertices[6] = video::S3DVertex(10 + offset.X, 10 + offset.Y, 10 + offset.Z, 1, 1, 1, video::SColor(255, 32, 32, 32), 0, 0);
  vertices[7] = video::S3DVertex(0 + offset.X, 10 + offset.Y, 10 + offset.Z, -1, 1, 1, video::SColor(255, 32, 32, 32), 1, 0);

  boundingBox.reset(vertices[0].Pos);
  for (size_t i = 1; i < 8; ++i)
    boundingBox.addInternalPoint(vertices[i].Pos);

  for (size_t face = 0; face < 6; ++face)
    faces[face] = video::SColor(255, 0, 0, 0);
}

void CubieSceneNode::SetFace(Face face, video::SColor color)
{
  faces[static_cast<size_t>(face)] = color;
}

void CubieSceneNode::OnRegisterSceneNode()
{
  if (IsVisible)
    SceneManager->registerNodeForRendering(this);

  ISceneNode::OnRegisterSceneNode();
}

void CubieSceneNode::render()
{
  u16 indices[] = 
  { 
    4, 5, 1, 0, // down
    2, 3, 7, 6, // up
    4, 5, 6, 7, // front
    3, 0, 4, 7, // right
    3, 2, 1, 0, // back
    1, 2, 6, 5  // left
  };
  auto driver = SceneManager->getVideoDriver();
  driver->setMaterial(material);
  driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
  for (size_t face = 0; face < 6; ++face)
  {
    for (size_t i = 0; i < 8; ++i)
      vertices[i].Color = faces[face];
    driver->drawVertexPrimitiveList(&vertices[0], 8, &indices[face * 4], 1, video::EVT_STANDARD, scene::EPT_QUADS, video::EIT_16BIT);
  }
}
