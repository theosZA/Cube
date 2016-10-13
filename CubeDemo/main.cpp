#include <codecvt>
#include <locale>
#include <string>

#include <irrlicht.h>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;

#include "CubeEventReceiver.h"
#include "CubeRenderer.h"
#include "..\Cube\Solver2x2x2.h"

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

  std::string moves = "D'  R2  B'  F  R  F  U2  F2  R'  U'  B2  L2  U2  R2  F'  D  B'  D  U2  L2  R'  F  L'  F'  R'";

  Cube2x2x2 cube;
  cube += moves;

  auto label = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(moves);
  guiEnvironment->addStaticText(label.c_str(), core::rect<s32>(10, 10, 360, 22), true);

  CubeRenderer cubeRenderer(sceneManager, sceneManager->getRootSceneNode(), 25.0f, 2);
  for (const auto& move : ParseMoveSequence(moves))
    cubeRenderer.ApplyMove(move.face, move.quarterRotationsClockwise, 1);

  sceneManager->addCameraSceneNode(0, core::vector3df(-20, 20, 30), core::vector3df(0, 0, 0));
  {
    driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
    sceneManager->drawAll();
    guiEnvironment->drawAll();
    driver->endScene();
  }

  Solver2x2x2 solver(20, "solution.2x2");
  auto solution = solver.Solve(cube);
  size_t step = 0;
  CubeEventReceiver cubeEventReceiver(device, cubeRenderer, solution);
  device->setEventReceiver(&cubeEventReceiver);

  while (device->run())
  {
    cubeRenderer.UpdateAnimate();
    driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
    sceneManager->drawAll();
    guiEnvironment->drawAll();
    driver->endScene();
  }

  device->drop();
}