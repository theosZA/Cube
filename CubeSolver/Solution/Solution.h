#pragma once

#include <iosfwd>
#include <vector>

#include "Cube\Face.h"

#include "SolutionStep.h"

class Solution
{
  public:
    Solution() : length(-1) {}

    // Adds a new step to the end of the solution.
    Solution& operator+=(SolutionStep);

    // Adds a new step to the end of the solution and returns the new solution.
    Solution operator+(SolutionStep) const;

    // Returns the number of moves in the solution.
    int Length() const;

    // Returns the move sequence making up this solution. Moves on the inverse solve are inverted and added to the
    // end of the forward solve. Consequently this only makes sense to use on a solution whose skeleton is complete.
    std::vector<CubeMove> GetMoves() const;

    // Rotates the cube, applying the solution and steps to a different set of faces.
    // All moves using the old faces are replaced by the same move on the new faces, and similarly
    // for all other corresponding faces.
    // Neither the old face pair nor the new face pairs can consist of opposite faces since that
    // wouldn't uniquely identify a rotation.
    Solution Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const;

    // Returns the solution such that the normal scramble and solve are swapped with the inverted scramble and solve, i.e. what was
    // originally considered the inverse solve is now the normal solve.
    Solution Invert() const;

    // Combine a scramble with this solution to add moves of the inverse solve as pre-moves to the scramble.
    // Returns the pre-moves plus the scramble plus the forward solve.
    std::vector<CubeMove> CombineScrambleAndSolution(const std::vector<CubeMove>& scramble) const;

    // Writes the full solution details to the given stream.
    friend std::ostream& operator<<(std::ostream& out, const Solution& solution);

  private:
    // Returns the index of the last (non-empty) step of the skeleton.
    // If there is no step completing the skeleton then -1 is returned.
    std::vector<SolutionStep>::size_type GetLastSkeletonStep() const;

    std::vector<SolutionStep> steps;
    mutable int length; // -1 if not calculated yet
};