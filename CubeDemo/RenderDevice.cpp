#include "RenderDevice.h"

#include <codecvt>
#include <stdexcept>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;

RenderDevice::RenderDevice(std::uint32_t windowWidth, std::uint32_t windowHeight, const char* windowCaption)
{
  SIrrlichtCreationParameters createParameters;
  createParameters.DriverType = video::EDT_DIRECT3D9;
  createParameters.WindowSize = core::dimension2d<u32>(windowWidth, windowHeight);
  createParameters.AntiAlias = 4;

  device.reset(createDeviceEx(createParameters));
  if (!device)
    throw std::runtime_error("Failed to create render device");

  auto windowCaptionUtf16 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(windowCaption);
  device->setWindowCaption(windowCaptionUtf16.c_str());
}

void RenderDevice::AddCamera(float cameraX, float cameraY, float cameraZ, float targetX, float targetY, float targetZ)
{
  core::vector3df camera(cameraX, cameraY, cameraZ);
  core::vector3df target(targetX, targetY, targetZ);
  device->getSceneManager()->addCameraSceneNode(0, camera, target);
}

bool RenderDevice::Run()
{
  return device->run();
}

void RenderDevice::RenderScene()
{
  device->getVideoDriver()->beginScene(true, true, video::SColor(255, 100, 101, 140));
  device->getSceneManager()->drawAll();
  device->getGUIEnvironment()->drawAll();
  device->getVideoDriver()->endScene();
}

scene::ISceneManager& RenderDevice::GetSceneManager()
{
  auto sceneManager = device->getSceneManager();
  if (!sceneManager)
    throw std::runtime_error("No scene manager for render device");
  return *sceneManager;
}

gui::IGUIStaticText& RenderDevice::CreateStaticText(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2)
{
  auto staticText = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(x1, y1, x2, y2), true);
  if (!staticText)
    throw std::runtime_error("Failed to create static text control");
  return *staticText;
}
