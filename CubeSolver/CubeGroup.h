#pragma once

#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <stdexcept>

#include "Cube\3x3x3\CornerStructure.h"

// A descriptive element describing the general (non-mathematical) group that the cube belongs to.
// This can be used to determine what solver steps can be applied to it. For groups that have
// rotations, reflections, etc., there is exactly one canonical group.
enum CubeGroup
{
  Solved,
  Scrambled,
  Block2x2x2,     // block positioned at BDL
  Block2x2x3,     // block position at BDL-FDL, i.e. with U and R faces free
  Block2x2x3_EO,  // block positioned at BDL-FDL and edges oriented with respect to F and B faces
  F2L_FSlot_EO,   // F2L-1, free slot positioned at FR-FDR, U face free, and edges oriented with respect to F and B faces.
  F2L_BSlot_EO,   // F2L-1, free slot positioned at BR-BDR, U face free, and edges oriented with respect to F and B faces.
  AB5C_twisted,   // all 5 corners twisted in place
  AB5C_2cycles,   // 4 of the 5 corners form two distinct 2-cycles while the 5th corner is twisted in place
  AB5C_3cycle,    // 3 of the 5 corners form a (twisted or non-twisted) 3-cycle while the other 2 corners are twisted in place
  AB5C_5cycle,    // all 5 corners form a 5-cycle
  AB4C_twisted,   // all 4 corners twisted in place
  AB4C_3cycle,    // 3 of the 4 corners form a twisted 3-cycle while the 4th corner is twisted in place
  AB4C_2cycles,   // the corners form two distinct 2-cycles
  AB3C_twisted,   // all 3 corners twisted in place
  AB3C_3cycle,    // all 3 corners form a 3-cycle
  AB2C_twisted    // both corners twisted in place
};

// Returns a reasonable lower bound on the number of moves that would be required to solve most cubes in the given group.
// These estimates are the number of moves that 99% of solves from that position require.
inline int LowerBoundMovesRequired(CubeGroup cubeGroup)
{
  switch (cubeGroup)
  {
    case Solved:        return 0;

    // 1 corner cycle insertion
    case AB3C_3cycle:   return 4;

    // 2 corner cycle insertions
    case AB2C_twisted:  return 4;
    case AB3C_twisted:  return 6;
    case AB4C_2cycles:  return 5;
    case AB4C_3cycle:   return 6;
    case AB5C_5cycle:   return 6;

    // 3 corner cycle insertions
    case AB4C_twisted:  return 8;
    case AB5C_3cycle:   return 8;
    case AB5C_2cycles:  return 8;

    // 4 corner cycle insertions
    case AB5C_twisted:  return 11;

    // Petrus block building
    case F2L_FSlot_EO:  
    case F2L_BSlot_EO:  return 10;
    case Block2x2x3_EO: return 13;
    case Block2x2x3:    return 16;
    case Block2x2x2:    return 19;
    case Scrambled:     return 21;

    default:
      throw std::domain_error("Unexpected cube group " + std::to_string(static_cast<int>(cubeGroup)));
  }
}

// Returns the expected number of moves to solve a cube from the given group.
inline double MeanMovesRequiredForLinearSolve(CubeGroup cubeGroup)
{
  switch (cubeGroup)
  {
    case Solved:        return 0.0;

      // 1 corner cycle insertion
    case AB3C_3cycle:   return 5.9;

      // 2 corner cycle insertions
    case AB2C_twisted:  return 7.8;
    case AB3C_twisted:  return 9.4;
    case AB4C_2cycles:  return 8.8;
    case AB4C_3cycle:   return 9.8;
    case AB5C_5cycle:   return 10.4;

      // 3 corner cycle insertions
    case AB4C_twisted:  return 12.1;
    case AB5C_3cycle:   return 12.9;
    case AB5C_2cycles:  return 12.1;

      // 4 corner cycle insertions
    case AB5C_twisted:  return 15.0;

      // Petrus block building
    case F2L_FSlot_EO:
    case F2L_BSlot_EO:  return 16.7;
    case Block2x2x3_EO: return 21.1;
    case Block2x2x3:    return 25.5;
    case Block2x2x2:    return 30.3;
    case Scrambled:     return 34.7;

    default:
      throw std::domain_error("Unexpected cube group " + std::to_string(static_cast<int>(cubeGroup)));
  }
}
