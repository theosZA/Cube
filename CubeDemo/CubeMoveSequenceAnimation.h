#pragma once

#include <functional>
#include <memory>
#include <random>
#include <vector>

#include <irrlicht.h>

#include "Cube\CubeMove.h"

#include "CubeRenderer.h"

// The CubeMoveSequenceAnimation renders a cube as a scene node, updating
// frame by frame to play through a move sequence.
class CubeMoveSequenceAnimation
{
public:
  CubeMoveSequenceAnimation(irr::scene::ISceneManager&, irr::scene::ISceneNode* parent, irr::f32 totalSize, size_t cubeSize);

  // Start an animation of the given move sequence. When the animation is completed the callback will be invoked.
  void Start(std::vector<CubeMove> moves, double quarterRotationsPerSecond, std::function<void()> onComplete);
  // Update the animation for the next rendered frame based on the current time.
  void Update();
  // Stop the animation by skipping to the end. All the moves will be played out instantly (no animation).
  void JumpToEnd();
  // Returns true if we are still in the middle of animating a started move sequence.
  bool IsAnimating() const;

private:
  void StartMove(size_t index);

  size_t cubeSize;
  CubeRenderer cube;

  double quarterRotationsPerSecond;
  std::vector<CubeMove> moves;
  size_t currentMove;
  std::function<void()> onComplete;
};

