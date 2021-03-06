#pragma once

#include <memory>
#include <vector>

#include <irrlicht.h>

#include "Cube\Scrambler\Scrambler.h"
#include "Cube\CubeMove.h"

#include "CubeDemoStateMachine.h"
#include "CubeMoveSequenceAnimation.h"
#include "RenderDevice.h"

class CubeDemo
{
public:
  // A demo that will run with random scrambles seeded with the given value.
  CubeDemo(int randomSeed);
  // A demo that will run a sequence of fixed scrambles.
  CubeDemo(const std::vector<std::vector<CubeMove>>& fixedScrambles);

  // Runs the demo either with random scrambles or a fixed scramble as supplied in the constructor.
  void RunDemo();

private:
  std::unique_ptr<Scrambler> randomScrambler;
  std::vector<std::vector<CubeMove>> fixedScrambles;
  size_t solvedCount;

  std::unique_ptr<RenderDevice> renderer;
  std::unique_ptr<CubeMoveSequenceAnimation> moveSequenceAnimator;
  std::unique_ptr<CubeDemoStateMachine> stateMachine;

  irr::gui::IGUIStaticText* scrambleStaticText;
  irr::gui::IGUIStaticText* solutionStaticText;

  void SetupScene();

  static void DisplayMoveSequence(irr::gui::IGUIStaticText& out, const std::vector<CubeMove>& moves, bool showMoveCount = false);
};