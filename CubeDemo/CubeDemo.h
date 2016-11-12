#pragma once

#include <memory>
#include <vector>

#include <irrlicht.h>

#include "CubeMoveSequenceAnimation.h"
#include "Scrambler.h"
#include "..\Cube\CubeMove.h"

class CubeDemo
{
public:
  // A demo that will run with random scrambles seeded with the given value.
  CubeDemo(int randomSeed);
  // A demo that will run once with a fixed scramble.
  CubeDemo(const std::vector<CubeMove>& fixedScramble);

  ~CubeDemo();

  // Runs the demo either with random scrambles or a fixed scramble as supplied in the constructor.
  void RunDemo();

private:
  std::unique_ptr<Scrambler> randomScrambler;
  std::vector<CubeMove> fixedScramble;
  int solvedCount;

  std::unique_ptr<irr::IrrlichtDevice> device;
  std::unique_ptr<CubeMoveSequenceAnimation> moveSequenceAnimator;

  irr::gui::IGUIStaticText* scrambleStaticText;
  irr::gui::IGUIStaticText* solutionStaticText;

  void SetupScene();

  static std::unique_ptr<irr::IrrlichtDevice> CreateRenderDevice();
  static void Render(irr::IrrlichtDevice& device);
  static void DisplayMoveSequence(irr::gui::IGUIStaticText& out, const std::vector<CubeMove> moves, bool showMoveCount = false);
};