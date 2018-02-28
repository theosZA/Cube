#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <irrlicht.h>

#include "Cube\Face.h"

#include "Cubie.h"

class CubeRenderer
{
public:
  CubeRenderer(irr::scene::ISceneManager&, irr::scene::ISceneNode* parent, irr::f32 totalSize, size_t cubeSize);

  void ApplyMove(Face, int quarterRotationsClockwise, int layers);

  // The speed is measured in quarter rotations per second.
  void AnimateMove(Face, int quarterRotationsClockwise, int layers, double speed);
  void UpdateAnimate(bool forceEnd = false);
  bool IsAnimating() const { return static_cast<bool>(currentAnimation); }

private:
  void RotateDegrees(Face, irr::f32 degrees, int layers);

  size_t cubeSize;
  irr::scene::ISceneNode* scaleNode;
  std::vector<std::unique_ptr<Cubie>> cubies;

  struct AnimatingMove
  {
    Face face;
    int quarterRotationsClockwise;
    int layers;
    double speed;
    std::chrono::time_point<std::chrono::steady_clock> timeEnd;
    std::chrono::time_point<std::chrono::steady_clock> timeLastUpdate;
  };
  std::unique_ptr<AnimatingMove> currentAnimation;
};