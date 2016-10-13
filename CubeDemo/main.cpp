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

using namespace irr;

#include "CubeRenderer.h"
#include "..\Cube\Solver2x2x2.h"

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

int main()
{
  SIrrlichtCreationParameters createParameters;
  createParameters.DriverType = video::EDT_OPENGL;
  createParameters.WindowSize = core::dimension2d<u32>(640, 480);
  createParameters.AntiAlias = 4;
  auto device = createDeviceEx(createParameters);
  if (!device)
    return 1;

  device->setWindowCaption(L"Cube demo");
  auto driver = device->getVideoDriver();
  auto sceneManager = device->getSceneManager();
  auto guiEnvironment = device->getGUIEnvironment();
  CubeRenderer cubeRenderer(sceneManager, sceneManager->getRootSceneNode(), 25.0f, 2);

  sceneManager->addCameraSceneNode(0, core::vector3df(-20, 20, 30), core::vector3df(0, 0, 0));
  Render(*driver, *sceneManager, *guiEnvironment);

  auto scramble = CreateRandomScramble(25);
  auto scrambleIter = scramble.begin();
  auto scrambleStaticText = guiEnvironment->addStaticText(L"", core::rect<s32>(10, 10, 360, 22), true);

  Solver2x2x2 solver(20, "solution.2x2");
  Cube2x2x2 cube;
  cube += scramble;
  auto solution = solver.Solve(cube);
  auto solutionIter = solution.begin();
  auto solutionStaticText = guiEnvironment->addStaticText(L"", core::rect<s32>(10, 26, 360, 38), true);
  std::chrono::time_point<std::chrono::steady_clock> solveStartTime;

  while (device->run())
  {
    if (cubeRenderer.IsAnimating())
      cubeRenderer.UpdateAnimate();
    else
    {
      if (scrambleIter != scramble.end())
      {
        auto move = *scrambleIter;
        cubeRenderer.AnimateMove(move.face, move.quarterRotationsClockwise, 1, 10.0);
        decltype(scramble) scrambleSoFar(scramble.begin(), scrambleIter);
        scrambleStaticText->setText(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(scrambleSoFar)).c_str());
        ++scrambleIter;
        if (scrambleIter == scramble.end())
          solveStartTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1500);
      }
      else if (solutionIter != solution.end() && std::chrono::high_resolution_clock::now() >= solveStartTime)
      {
        auto move = *solutionIter;
        cubeRenderer.AnimateMove(move.face, move.quarterRotationsClockwise, 1, 3.0);
        decltype(solution) solutionSoFar(solution.begin(), solutionIter);
        solutionStaticText->setText(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(solutionSoFar)).c_str());
        ++solutionIter;
      }
    }
    Render(*driver, *sceneManager, *guiEnvironment);
  }

  device->drop();
}