#pragma once

#include <array>

#include <irrlicht.h>

#include "..\Cube\Face.h"

class CubieSceneNode : public irr::scene::ISceneNode
{
  public:
    CubieSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager*, irr::s32 id, irr::core::vector3df offset);

    void SetFace(Face, irr::video::SColor);

    void OnRegisterSceneNode() override;
    void render() override;
    const irr::core::aabbox3d<irr::f32>& getBoundingBox() const override { return boundingBox; }
    irr::u32 getMaterialCount() const override { return 1; }
    irr::video::SMaterial& getMaterial(irr::u32 i) { return material; }

  private:
    std::array<irr::video::SColor, 6> faces;

    irr::core::aabbox3d<irr::f32> boundingBox;
    irr::video::S3DVertex vertices[8];
    irr::video::SMaterial material;
};