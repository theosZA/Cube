#include <chrono>
#include <codecvt>
#include <locale>
#include <random>
#include <string>

#include <irrlicht.h>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include "CubeMoveSequenceAnimation.h"

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

std::unique_ptr<IrrlichtDevice> CreateRenderDevice()
{
  SIrrlichtCreationParameters createParameters;
  createParameters.DriverType = video::EDT_DIRECT3D9;
  createParameters.WindowSize = core::dimension2d<u32>(windowWidth, windowHeight);
  createParameters.AntiAlias = 4;
  return std::unique_ptr<IrrlichtDevice>{ createDeviceEx(createParameters) };
}

void Render(IrrlichtDevice& device)
{
  device.getVideoDriver()->beginScene(true, true, video::SColor(255, 100, 101, 140));
  device.getSceneManager()->drawAll();
  device.getGUIEnvironment()->drawAll();
  device.getVideoDriver()->endScene();
}

void DisplayMoveSequence(gui::IGUIStaticText& out, const std::vector<CubeMove> moves, bool showMoveCount = false)
{
  auto text = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(moves));
  if (showMoveCount)
    text += L" [" + std::to_wstring(moves.size()) + L"]";
  out.setText(text.c_str());
}

enum State
{
  Scrambling,
  Scrambled,
  Solving,
  Solved
};

int main()
{
  std::random_device randomSource;

  auto device = CreateRenderDevice();
  if (!device)
    return 1;
  device->setWindowCaption(L"Cube demo");

  auto sceneManager = device->getSceneManager();
  sceneManager->addCameraSceneNode(0, core::vector3df(-20, 20, 30), core::vector3df(0, 0, 0));
  CubeMoveSequenceAnimation cube(sceneManager, sceneManager->getRootSceneNode(), cubeRenderSize, cubeSize);

  auto scrambleStaticText = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 560, 22), true);
  auto solutionStaticText = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 26, 560, 38), true);

  State state = State::Solved;
  auto stateStart = std::chrono::high_resolution_clock::now();

  while (device->run())
  {
    Render(*device);

    switch (state)
    {
      case State::Scrambling:
      case State::Solving:
        cube.Update();
        break;

      case State::Scrambled:
        {
          auto now = std::chrono::high_resolution_clock::now();
          if (now - stateStart >= delayForSolve)
          {
            state = State::Solving;
            stateStart = now;
            auto solution = cube.SolveScramble(solveSpeed, 
              [&]
              {
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
          if (now - stateStart >= delayForSolve)
          {
            state = State::Scrambling;
            stateStart = now;
            auto scramble = cube.RandomScramble(randomSource(), 25, scrambleSpeed,
              [&]
              {
                state = State::Scrambled;
                stateStart = std::chrono::high_resolution_clock::now();
              });
            DisplayMoveSequence(*scrambleStaticText, scramble);
            DisplayMoveSequence(*solutionStaticText, std::vector<CubeMove>{});
          }
        }
        break;
    }
  }

  device->drop();
}