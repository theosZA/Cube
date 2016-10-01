#pragma once

#include <iosfwd>

#include "Face.h"

struct StickerPosition
{
  Face face;
  size_t index; // 0 1 2
                // 3 4 5
                // 6 7 8
                // or for 2x2x2
                // 0 1
                // 2 3

  inline bool operator==(const StickerPosition& rhs) const
  {
      return face == rhs.face && index == rhs.index;
  }

  inline bool operator!=(const StickerPosition& rhs) const
  {
    return !(*this == rhs);
  }
};

std::ostream& operator<<(std::ostream&, const StickerPosition&);
