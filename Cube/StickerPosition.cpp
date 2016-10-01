#include "StickerPosition.h"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const StickerPosition& position)
{
  out << position.face << '[' << position.index << ']';
  return out;
}