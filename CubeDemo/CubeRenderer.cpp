#include "CubeRenderer.h"

#include <array>

using namespace irr;

const auto cNone = video::SColor(255, 0, 0, 0);
const auto cDown = video::SColor(255, 255, 255, 255);
const auto cUp = video::SColor(255, 255, 255, 0);
const auto cFront = video::SColor(255, 0, 0, 255);
const auto cRight = video::SColor(255, 255, 0, 0);
const auto cBack = video::SColor(255, 0, 255, 0);
const auto cLeft = video::SColor(255, 255, 127, 0);

const f32 padding = 0.02f;

CubeRenderer::CubeRenderer(scene::ISceneManager& manager, scene::ISceneNode* parent, f32 totalSize, size_t cubeSize)
: cubeSize(cubeSize)
{
  auto scale = totalSize / cubeSize * (1 - padding);
  scaleNode = manager.addEmptySceneNode(parent);
  scaleNode->setScale(core::vector3df { scale, scale, scale });

  for (size_t x = 0; x < cubeSize; ++x)
    for (size_t y = 0; y < cubeSize; ++y)
      for (size_t z = 0; z < cubeSize; ++z)
        if (x == 0 || x == cubeSize - 1 || y == 0 || y == cubeSize - 1 || z == 0 || z == cubeSize - 1)
        {
          std::array<video::SColor, 6> cubieFaceColors;
          cubieFaceColors[static_cast<size_t>(Face::Down)] = (y == 0) ? cDown : cNone;
          cubieFaceColors[static_cast<size_t>(Face::Up)] = (y == cubeSize - 1) ? cUp : cNone;
          cubieFaceColors[static_cast<size_t>(Face::Front)] = (z == 0) ? cFront : cNone;
          cubieFaceColors[static_cast<size_t>(Face::Right)] = (x == cubeSize - 1) ? cRight : cNone;
          cubieFaceColors[static_cast<size_t>(Face::Back)] = (z == cubeSize - 1) ? cBack : cNone;
          cubieFaceColors[static_cast<size_t>(Face::Left)] = (x == 0) ? cLeft : cNone;
          core::vector3df position 
          {
            (cubeSize - 1) / 2.0f - x,
            y - (cubeSize - 1) / 2.0f,
            (cubeSize - 1) / 2.0f - z,
          };
          cubies.emplace_back(new Cubie{ manager, scaleNode, position / (1 - padding), cubieFaceColors });
        }
}

void CubeRenderer::ApplyMove(Face face, int quarterRotationsClockwise, int layers)
{
  RotateDegrees(face, 90.0f * quarterRotationsClockwise, layers);
}

void CubeRenderer::AnimateMove(Face face, int quarterRotationsClockwise, int layers, double speed)
{
  if (currentAnimation)
    UpdateAnimate(true);

  double totalNanoseconds = abs(quarterRotationsClockwise) * 1e9 / speed;

  auto start = std::chrono::high_resolution_clock::now();
  auto end = start + std::chrono::nanoseconds(static_cast<int>(totalNanoseconds));
  currentAnimation.reset(new AnimatingMove{ face, quarterRotationsClockwise, layers, speed, end, start });
}

void CubeRenderer::UpdateAnimate(bool forceEnd)
{
  if (currentAnimation)
  {
    double speedInDegreesPerNanosecond = currentAnimation->speed * 90.0 / 1e9;
    if (currentAnimation->quarterRotationsClockwise < 0)
      speedInDegreesPerNanosecond = -speedInDegreesPerNanosecond;
    auto now = std::chrono::high_resolution_clock::now();
    if (now >= currentAnimation->timeEnd || forceEnd)
    {
      auto elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentAnimation->timeEnd - currentAnimation->timeLastUpdate).count();
      RotateDegrees(currentAnimation->face, static_cast<f32>(speedInDegreesPerNanosecond * elapsedNanoseconds), currentAnimation->layers);
      currentAnimation.reset();
    }
    else
    {
      auto elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now - currentAnimation->timeLastUpdate).count();
      RotateDegrees(currentAnimation->face, static_cast<f32>(speedInDegreesPerNanosecond * elapsedNanoseconds), currentAnimation->layers);
      currentAnimation->timeLastUpdate = now;
    }
  }
}

void CubeRenderer::RotateDegrees(Face face, f32 degrees, int layers)
{
  for (auto& cubie : cubies)
    cubie->ApplyMove(face, degrees, (cubeSize - layers) - (cubeSize - 1) / 2.0f);
}
