#pragma once

#include <string>
#include <vector>

#include "Cube\CubeMove.h"

// A step in a cube solution.
class SolutionStep
{
  public:
    // Simple constructor for a solution step. Each move is considered a step on the normal (forward) solve.
    SolutionStep(std::string description, std::vector<CubeMove> stepMoves, bool isSkeletonComplete = false);

    // Complex constructor for a solution step. For each move you must specify whether it is on the normal solve (false) or inverse solve (true).
    // The size of the 2 vectors must be identical. For performance reasons this is not verified here and is undefined behaviour if not adhered to.
    SolutionStep(std::string description, std::vector<CubeMove> stepMoves, std::vector<bool> isStepMoveOnInverseSolve, bool isSkeletonComplete = false);

    // Constructor for an insertion solution step.
    SolutionStep(std::string description, size_t insertionIndex, std::vector<CubeMove> stepMoves);

    // Returns true if there are no moves in this step.
    bool IsEmpty() const;

    // Returns true if this is the last step in building the solve's skeleton.
    bool IsSkeletonComplete() const;

    // Returns text to describe the step.
    const std::string& GetDescription() const;

    // Returns text specifying the move sequence including brackets around moves made on the inverse solve. The partial (forward) solution up to
    // this point is required in case this is an insertion step and we need to indicate where the insertion happens.
    std::string GetMoveSequence(const std::vector<CubeMove>& forwardSolution) const;

    // Adds this step's moves of the forward solve to the end of the forward solution, and the moves of the inverse solve to the beginning of
    // the inverse solution.
    void AccumulateSolution(std::vector<CubeMove>& forwardSolution, std::vector<CubeMove>& inverseSolution) const;

    // Rotates the cube, applying the solution step to a different set of faces.
    // All moves using the old faces are replaced by the same move on the new faces, and similarly for all other corresponding faces.
    // Neither the old face pair nor the new face pairs can consist of opposite faces since that wouldn't uniquely identify a rotation.
    SolutionStep Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const;

    // Returns the solution step such that the normal scramble and solve are swapped with the inverted scramble and solve, i.e. what was
    // originally considered the inverse solve is now the normal solve.
    // Inverting an insertion step will thrown an exception.
    SolutionStep Invert() const;

  private:
    // Creates a copy of the provided solution step, but rotated such that the old faces are replaced by the new faces.
    SolutionStep(const SolutionStep&, std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces);

    std::string description;
    bool isSkeletonComplete;

    std::vector<CubeMove> stepMoves;
    std::vector<bool> isStepMoveOnInverseSolve;

    size_t insertionIndex;
};