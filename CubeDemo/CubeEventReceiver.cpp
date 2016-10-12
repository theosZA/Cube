#include "CubeEventReceiver.h"

#include <codecvt>
#include <locale>
#include <string>

#include "..\Cube\Cube.h"

using namespace irr;

CubeEventReceiver::CubeEventReceiver(irr::IrrlichtDevice* device, CubeRenderer& cubeRenderer, const std::vector<CubeMove>& moves)
: device(device),
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
        cubeRenderer.ApplyMove(moves[moveIndex++]);
        decltype(moves) movesSoFar(moves.begin(), moves.begin() + moveIndex);
        auto label = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(MoveSequenceToText(movesSoFar));
        device->getGUIEnvironment()->addStaticText(label.c_str(), core::rect<s32>(10, 26, 360, 36), true);
      }
      return true;
    }
  }
  return false;
}
