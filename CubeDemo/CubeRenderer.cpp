#include "CubeRenderer.h"

#include <stdexcept>

using namespace irr;

video::SColor FaceToColor(Face face)
{
  switch (face)
  {
    case Face::Down:  return video::SColor(255, 255, 255, 255);
    case Face::Up:    return video::SColor(255, 255, 255, 0);
    case Face::Front: return video::SColor(255, 0, 0, 255);
    case Face::Right: return video::SColor(255, 255, 0, 0);
    case Face::Back:  return video::SColor(255, 0, 255, 0);
    case Face::Left:  return video::SColor(255, 180, 90, 0);
    default:
      throw std::domain_error("Invalid face");
  }
}

CubeRenderer::CubeRenderer(const Cube2x2x2& cube, irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager)
{
  static const std::array<std::array<Face, 3>, 8> cubieFace
  {
    std::array<Face, 3>{ Face::Down, Face::Left,   Face::Front },
    std::array<Face, 3>{ Face::Down, Face::Front,  Face::Right },
    std::array<Face, 3>{ Face::Down, Face::Back,   Face::Left },
    std::array<Face, 3>{ Face::Down, Face::Right,  Face::Back },
    std::array<Face, 3>{ Face::Up,   Face::Left,   Face::Back },
    std::array<Face, 3>{ Face::Up,   Face::Back,   Face::Right },
    std::array<Face, 3>{ Face::Up,   Face::Front,  Face::Left },
    std::array<Face, 3>{ Face::Up,   Face::Right,  Face::Front }
  };

  static const std::array<std::array<size_t, 3>, 8> cubieIndex
  {
    std::array<size_t, 3>{ 0, 3, 2 },
    std::array<size_t, 3>{ 1, 3, 2 },
    std::array<size_t, 3>{ 2, 3, 2 },
    std::array<size_t, 3>{ 3, 3, 2 },
    std::array<size_t, 3>{ 0, 0, 1 },
    std::array<size_t, 3>{ 1, 0, 1 },
    std::array<size_t, 3>{ 2, 0, 1 },
    std::array<size_t, 3>{ 3, 0, 1 }
  };

  static const std::array<std::array<float, 3>, 8> cubieOffsets
  {
    std::array<float, 3>{ +5.1f, -5.1f, +5.1f },
    std::array<float, 3>{ -5.1f, -5.1f, +5.1f },
    std::array<float, 3>{ +5.1f, -5.1f, -5.1f },
    std::array<float, 3>{ -5.1f, -5.1f, -5.1f },
    std::array<float, 3>{ +5.1f, +5.1f, -5.1f },
    std::array<float, 3>{ -5.1f, +5.1f, -5.1f },
    std::array<float, 3>{ +5.1f, +5.1f, +5.1f },
    std::array<float, 3>{ -5.1f, +5.1f, +5.1f }
  };

  for (size_t i = 0; i < 8; ++i)
  {
    static int id = 1000;
    cubies[i] = new CubieSceneNode(parent, sceneManager, id++, core::vector3df(cubieOffsets[i][0], cubieOffsets[i][1], cubieOffsets[i][2]));
    for (size_t j = 0; j < 3; ++j)
      cubies[i]->SetFace(cubieFace[i][j], FaceToColor(cube[StickerPosition{ cubieFace[i][j], cubieIndex[i][j] }].face));
  }
}
