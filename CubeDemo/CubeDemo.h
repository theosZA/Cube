#pragma once

#include <memory>
#include <vector>

#include <irrlicht.h>

#include "CubeDemoStateMachine.h"
#include "CubeMoveSequenceAnimation.h"
#include "RenderDevice.h"
#include "Scrambler.h"
#include "..\Cube\CubeMove.h"

class CubeDemo
{
public:
  // A demo that will run with random scrambles seeded with the given value.
  CubeDemo(int randomSeed);
  // A demo that will run once with a fixed scramble.
  CubeDemo(const std::vector<CubeMove>& fixedScramble);

  // Runs the demo either with random scrambles or a fixed scramble as supplied in the constructor.
  void RunDemo();

private:
  std::unique_ptr<Scrambler> randomScrambler;
  std::vector<CubeMove> fixedScramble;
  int solvedCount;

  std::unique_ptr<RenderDevice> renderer;
  std::unique_ptr<CubeMoveSequenceAnimation> moveSequenceAnimator;
  std::unique_ptr<CubeDemoStateMachine> stateMachine;

  irr::gui::IGUIStaticText* scrambleStaticText;
  irr::gui::IGUIStaticText* solutionStaticText;

  void SetupScene();

  static void DisplayMoveSequence(irr::gui::IGUIStaticText& out, const std::vector<CubeMove>& moves, bool showMoveCount = false);
};