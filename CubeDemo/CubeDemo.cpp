#include "CubeDemo.h"

#include <cstdint>
#include <chrono>
#include <codecvt>

using namespace std::chrono_literals;

const double scrambleSpeed = 10.0;
const double solveSpeed = 3.0;
const auto delayForSolve = 1500ms;
const size_t cubeSize = 3;
const size_t scrambleLength = 25;
const float cubeRenderSize = 25.0f;
const std::uint32_t windowWidth = 900;
const std::uint32_t windowHeight = 900;

CubeDemo::CubeDemo(int randomSeed)
: randomScrambler(new Scrambler(randomSeed)),
  solvedCount(0)
{}

CubeDemo::CubeDemo(const std::vector<CubeMove>& fixedScramble)
: fixedScramble(fixedScramble),
  solvedCount(0)
{}

void CubeDemo::RunDemo()
{
  SetupScene();

  enum State
  {
    Scrambling,
    Scrambled,
    Solving,
    Solved
  };
  State state = State::Solved;
  auto stateStart = std::chrono::high_resolution_clock::now();

  while (renderer->Run())
  {
    renderer->RenderScene();

    switch (state)
    {
      case State::Scrambling:
      case State::Solving:
        moveSequenceAnimator->Update();
        break;

      case State::Scrambled:
      {
        auto now = std::chrono::high_resolution_clock::now();
        if (now - stateStart >= delayForSolve)
        {
          state = State::Solving;
          stateStart = now;
          auto solution = moveSequenceAnimator->SolveScramble(solveSpeed,
              [&]
              {
                ++solvedCount;
                state = State::Solved;
                stateStart = std::chrono::high_resolution_clock::now();
              });
          DisplayMoveSequence(*solutionStaticText, solution, true);
        }
      }
      break;

      case State::Solved:
      {
        auto now = std::chrono::high_resolution_clock::now();
        if (now - stateStart >= delayForSolve && (randomScrambler || solvedCount == 0))
        {
          state = State::Scrambling;
          stateStart = now;

          std::vector<CubeMove> scramble = randomScrambler ? randomScrambler->CreateRandomScramble(25) : fixedScramble;
          DisplayMoveSequence(*scrambleStaticText, scramble);
          DisplayMoveSequence(*solutionStaticText, std::vector<CubeMove>{});
          moveSequenceAnimator->Start(scramble, scrambleSpeed,
              [&]
              {
                state = State::Scrambled;
                stateStart = std::chrono::high_resolution_clock::now();
              });
        }
      }
      break;
    }
  }
}

void CubeDemo::SetupScene()
{
  renderer.reset(new RenderDevice(windowWidth, windowWidth, "Cube demo"));
  renderer->AddCamera(-20, 20, 30, 0, 0, 0);
  auto& sceneManager = renderer->GetSceneManager();
  moveSequenceAnimator.reset(new CubeMoveSequenceAnimation(sceneManager, sceneManager.getRootSceneNode(), cubeRenderSize, cubeSize));

  scrambleStaticText = &renderer->CreateStaticText(10, 10, 560, 22);
  solutionStaticText = &renderer->CreateStaticText(10, 26, 560, 38);
}

void CubeDemo::DisplayMoveSequence(irr::gui::IGUIStaticText& out, const std::vector<CubeMove>& moves, bool showMoveCount)
{
  auto text = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(moves));
  if (showMoveCount)
    text += L" [" + std::to_wstring(moves.size()) + L"]";
  out.setText(text.c_str());
}

