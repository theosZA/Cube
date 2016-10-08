#include "CubeEventReceiver.h"

#include <codecvt>
#include <locale>
#include <string>

#include "..\Cube\Cube.h"

using namespace irr;

CubeEventReceiver::CubeEventReceiver(irr::IrrlichtDevice* device, Cube2x2x2& cube, CubeRenderer& cubeRenderer, const std::vector<CubeMove>& moves)
: device(device),
  cube(cube),
  cubeRenderer(cubeRenderer),
  moves(moves)
{}

bool CubeEventReceiver::OnEvent(const irr::SEvent& event)
{
  if (event.EventType == EET_KEY_INPUT_EVENT)
  {
    if (event.KeyInput.Char == L' ')
    {
      if (moveIndex < moves.size())
      {
        cube += moves[moveIndex++];
        cubeRenderer.SetCube(cube);
        decltype(moves) movesSoFar(moves.begin(), moves.begin() + moveIndex);
        auto label = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(movesSoFar));
        device->getGUIEnvironment()->addStaticText(label.c_str(), core::rect<s32>(10, 26, 260, 36), true);
      }
      return true;
    }
  }
  return false;
}