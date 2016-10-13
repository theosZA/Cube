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

#include "CubeRenderer.h"
#include "..\Cube\Solver2x2x2.h"

using namespace irr;
using namespace std::chrono_literals;

const double scrambleSpeed = 10.0;
const double solveSpeed = 3.0;
const auto delayForSolve = 1500ms;
const size_t cubeSize = 2;
const size_t scrambleLength = 25;
const f32 cubeRenderSize = 25.0f;
const u32 windowWidth = 900;
const u32 windowHeight = 900;

void Render(video::IVideoDriver& driver, scene::ISceneManager& sceneManager, gui::IGUIEnvironment& guiEnvironment)
{
  driver.beginScene(true, true, video::SColor(255, 100, 101, 140));
  sceneManager.drawAll();
  guiEnvironment.drawAll();
  driver.endScene();
}

std::vector<CubeMove> CreateRandomScramble(size_t length)
{
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> faceDistribution(0, 5);
  std::uniform_int_distribution<> rotationDistribution(0, 2);
  std::vector<CubeMove> scramble(length);
  for (size_t i = 0; i < length; ++i)
  {
    do
      scramble[i].face = static_cast<Face>(faceDistribution(generator));
    while (i > 0 && scramble[i].face == scramble[i - 1].face);

    scramble[i].quarterRotationsClockwise = rotationDistribution(generator);
    if (scramble[i].quarterRotationsClockwise == 0)
      scramble[i].quarterRotationsClockwise = -1;
  }
  return scramble;
}

std::vector<CubeMove> SolveScramble(const std::vector<CubeMove>& scramble)
{
  Solver2x2x2 solver(20, "solution.2x2");
  Cube2x2x2 cube;
  cube += scramble;
  return solver.Solve(cube);
}

int main()
{
  SIrrlichtCreationParameters createParameters;
  createParameters.DriverType = video::EDT_OPENGL;
  createParameters.WindowSize = core::dimension2d<u32>(windowWidth, windowHeight);
  createParameters.AntiAlias = 4;
  auto device = createDeviceEx(createParameters);
  if (!device)
    return 1;

  device->setWindowCaption(L"Cube demo");
  auto driver = device->getVideoDriver();
  auto sceneManager = device->getSceneManager();
  auto guiEnvironment = device->getGUIEnvironment();
  CubeRenderer cubeRenderer(sceneManager, sceneManager->getRootSceneNode(), cubeRenderSize, cubeSize);

  sceneManager->addCameraSceneNode(0, core::vector3df(-20, 20, 30), core::vector3df(0, 0, 0));
  Render(*driver, *sceneManager, *guiEnvironment);

  auto scramble = CreateRandomScramble(scrambleLength);
  auto scrambleIter = scramble.begin();
  auto scrambleStaticText = guiEnvironment->addStaticText(L"", core::rect<s32>(10, 10, 360, 22), true);

  bool solved = false;
  std::chrono::time_point<std::chrono::steady_clock> solveStartTime;

  std::vector<CubeMove> solution;
  auto solutionIter = solution.begin();
  auto solutionStaticText = guiEnvironment->addStaticText(L"", core::rect<s32>(10, 26, 360, 38), true);

  while (device->run())
  {
    if (cubeRenderer.IsAnimating())
      cubeRenderer.UpdateAnimate();
    else
    {
      if (scrambleIter != scramble.end())
      {
        auto move = *scrambleIter;
        cubeRenderer.AnimateMove(move.face, move.quarterRotationsClockwise, 1, scrambleSpeed);
        decltype(scramble) scrambleSoFar(scramble.begin(), scrambleIter);
        scrambleStaticText->setText(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(scrambleSoFar)).c_str());
        ++scrambleIter;
        if (scrambleIter == scramble.end())
          solveStartTime = std::chrono::high_resolution_clock::now() + delayForSolve;
      }
      else if (!solved)
      {
        if (std::chrono::high_resolution_clock::now() >= solveStartTime)
        {
          solution = SolveScramble(scramble);
          solutionIter = solution.begin();
          solved = true;
        }
      } 
      else if (solutionIter != solution.end())
      {
        auto move = *solutionIter;
        cubeRenderer.AnimateMove(move.face, move.quarterRotationsClockwise, 1, solveSpeed);
        decltype(solution) solutionSoFar(solution.begin(), solutionIter);
        solutionStaticText->setText(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(solutionSoFar)).c_str());
        ++solutionIter;
      }
    }
    Render(*driver, *sceneManager, *guiEnvironment);
  }

  device->drop();
}