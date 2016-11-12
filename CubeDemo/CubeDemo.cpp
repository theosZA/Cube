#include "CubeDemo.h"

#include <chrono>
#include <codecvt>
#include <stdexcept>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;
using namespace std::chrono_literals;

const double scrambleSpeed = 10.0;
const double solveSpeed = 3.0;
const auto delayForSolve = 1500ms;
const size_t cubeSize = 3;
const size_t scrambleLength = 25;
const f32 cubeRenderSize = 25.0f;
const u32 windowWidth = 900;
const u32 windowHeight = 900;

CubeDemo::CubeDemo(int randomSeed)
: randomScrambler(new Scrambler(randomSeed)),
  solvedCount(0)
{}

CubeDemo::CubeDemo(const std::vector<CubeMove>& fixedScramble)
: fixedScramble(fixedScramble),
  solvedCount(0)
{}

CubeDemo::~CubeDemo()
{
  device->drop();
}

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

  while (device->run())
  {
    Render(*device);

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
  device = CreateRenderDevice();
  if (!device)
    throw std::runtime_error("Failed to create render device");
  device->setWindowCaption(L"Cube demo");

  auto sceneManager = device->getSceneManager();
  sceneManager->addCameraSceneNode(0, core::vector3df(-20, 20, 30), core::vector3df(0, 0, 0));
  moveSequenceAnimator.reset(new CubeMoveSequenceAnimation(sceneManager, sceneManager->getRootSceneNode(), cubeRenderSize, cubeSize));

  scrambleStaticText = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 560, 22), true);
  solutionStaticText = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 26, 560, 38), true);
}

std::unique_ptr<IrrlichtDevice> CubeDemo::CreateRenderDevice()
{
  SIrrlichtCreationParameters createParameters;
  createParameters.DriverType = video::EDT_DIRECT3D9;
  createParameters.WindowSize = core::dimension2d<u32>(windowWidth, windowHeight);
  createParameters.AntiAlias = 4;
  return std::unique_ptr<IrrlichtDevice>{ createDeviceEx(createParameters) };
}

void CubeDemo::Render(IrrlichtDevice& device)
{
  device.getVideoDriver()->beginScene(true, true, video::SColor(255, 100, 101, 140));
  device.getSceneManager()->drawAll();
  device.getGUIEnvironment()->drawAll();
  device.getVideoDriver()->endScene();
}

void CubeDemo::DisplayMoveSequence(gui::IGUIStaticText& out, const std::vector<CubeMove> moves, bool showMoveCount)
{
  auto text = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(moves));
  if (showMoveCount)
    text += L" [" + std::to_wstring(moves.size()) + L"]";
  out.setText(text.c_str());
}

