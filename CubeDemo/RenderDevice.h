#pragma once

#include <cstdint>
#include <memory>

#include <irrlicht.h>

// Wrapper class for the Irrlicht renderer device.
class RenderDevice
{
public:
  RenderDevice(std::uint32_t windowWidth, std::uint32_t windowHeight, const char* windowCaption);

  // Creates a camera at the given camera co-ordinates aiming at the given target co-ordinates.
  void AddCamera(float cameraX, float cameraY, float cameraZ, float targetX, float targetY, float targetZ);

  // Runs the device, e.g. handling user input. Returns false if no longer running.
  bool Run();
  // Renders the current scene to the device.
  void RenderScene();

  // Returns the underlying scene manager so that you can add content to the scene.
  irr::scene::ISceneManager& GetSceneManager();

  // Creates an empty static text control and returns the underlying GUI element.
  irr::gui::IGUIStaticText& CreateStaticText(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2);

private:
  std::unique_ptr<irr::IrrlichtDevice> device;
};