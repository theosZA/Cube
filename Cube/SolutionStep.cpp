#include "SolutionStep.h"

#include <algorithm>
#include <sstream>

SolutionStep::SolutionStep(std::string description, std::vector<CubeMove> stepMoves, bool isSkeletonComplete)
: description(std::move(description)),
  isSkeletonComplete(isSkeletonComplete),
  stepMoves(std::move(stepMoves)),
  isStepMoveOnInverseSolve(std::vector<bool>(this->stepMoves.size(), false)),
  insertionIndex(-1)
{}

SolutionStep::SolutionStep(std::string description, std::vector<CubeMove> stepMoves, std::vector<bool> isStepMoveOnInverseSolve, bool isSkeletonComplete)
: description(std::move(description)),
  isSkeletonComplete(isSkeletonComplete),
  stepMoves(std::move(stepMoves)),
  isStepMoveOnInverseSolve(std::move(isStepMoveOnInverseSolve)),
  insertionIndex(-1)
{}

SolutionStep::SolutionStep(std::string description, size_t insertionIndex, std::vector<CubeMove> stepMoves)
: description(std::move(description)),
  isSkeletonComplete(true),
  stepMoves(std::move(stepMoves)),
  isStepMoveOnInverseSolve(std::vector<bool>(this->stepMoves.size(), false)),
  insertionIndex(insertionIndex)
{}

bool SolutionStep::IsEmpty() const
{
  return stepMoves.empty();
}

bool SolutionStep::IsSkeletonComplete() const
{
  return isSkeletonComplete;
}

const std::string& SolutionStep::GetDescription() const
{
  return description;
}

std::string SolutionStep::GetMoveSequence(const std::vector<CubeMove>& forwardSolution) const
{
  if (stepMoves.empty())
  {
    return std::string{};
  }

  std::ostringstream moveSequenceText;

  if (insertionIndex != -1)
  { // Insertion step - indicate where the insertion happens.
    std::vector<CubeMove> movesBeforeInsertion{ forwardSolution.begin(), forwardSolution.begin() + insertionIndex };
    std::vector<CubeMove> movesAfterInsertion{ forwardSolution.begin() + insertionIndex, forwardSolution.end() };
    moveSequenceText << MoveSequenceToText(movesBeforeInsertion) << " * " << MoveSequenceToText(movesAfterInsertion)
      << "\n  Insert at * = ";
  }

  for (std::vector<CubeMove>::size_type i = 0; i < stepMoves.size(); ++i)
  {
    if (i != 0)
    {
      moveSequenceText << ' ';
    }
    if (isStepMoveOnInverseSolve[i] && (i == 0 || !isStepMoveOnInverseSolve[i - 1]))
    { // Start of sequence of moves on the inverse solve.
      moveSequenceText << '(';
    }
    moveSequenceText << stepMoves[i];
    if (isStepMoveOnInverseSolve[i] && (i + 1 >= stepMoves.size() || !isStepMoveOnInverseSolve[i + 1]))
    { // End of sequence of moves on the inverse solve.
      moveSequenceText << ')';
    }
  }

  return moveSequenceText.str();
}

void SolutionStep::AccumulateSolution(std::vector<CubeMove>& forwardSolution, std::vector<CubeMove>& inverseSolution) const
{
  if (insertionIndex != -1)
  { // Insert move into solution.
    if (!inverseSolution.empty())
    {
      forwardSolution += InvertMoveSequence(inverseSolution);
      forwardSolution = SimplifyMoveSequence(forwardSolution);
      inverseSolution.clear();
    }
    std::vector<CubeMove> newSolution;
    newSolution.reserve(forwardSolution.size() + stepMoves.size());
    std::copy(forwardSolution.begin(), forwardSolution.begin() + insertionIndex, std::back_inserter(newSolution));
    std::copy(stepMoves.begin(), stepMoves.end(), std::back_inserter(newSolution));
    std::copy(forwardSolution.begin() + insertionIndex, forwardSolution.end(), std::back_inserter(newSolution));
    forwardSolution = SimplifyMoveSequence(newSolution);
    return;
  }

  bool forwardSolutionChanged = false;
  bool inverseSolutionChanged = false;
  for (std::vector<bool>::size_type i = 0; i < isStepMoveOnInverseSolve.size(); ++i)
  {
    if (isStepMoveOnInverseSolve[i])
    {
      inverseSolution.push_back(stepMoves[i]);
      inverseSolutionChanged = true;
    }
    else
    {
      forwardSolution.push_back(stepMoves[i]);
      forwardSolutionChanged = true;
    }
  }
  if (forwardSolutionChanged)
  {
    forwardSolution = SimplifyMoveSequence(forwardSolution);
  }
  if (inverseSolutionChanged)
  {
    inverseSolution = SimplifyMoveSequence(inverseSolution);
  }
}

SolutionStep SolutionStep::Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const
{
  return SolutionStep{ *this, oldFaces, newFaces };
}

SolutionStep SolutionStep::Invert() const
{
  std::vector<bool> isStepMoveOnForwardSolve(isStepMoveOnInverseSolve.size());
  for (std::vector<bool>::size_type i = 0; i < isStepMoveOnInverseSolve.size(); ++i)
  {
    isStepMoveOnForwardSolve[i] = !isStepMoveOnInverseSolve[i];
  }
  return SolutionStep{ description, stepMoves, isStepMoveOnForwardSolve, isSkeletonComplete };
}

SolutionStep::SolutionStep(const SolutionStep& source, std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces)
: description(source.description),
  isSkeletonComplete(source.isSkeletonComplete),
  stepMoves(::Rotate(source.stepMoves, oldFaces, newFaces)),
  isStepMoveOnInverseSolve(source.isStepMoveOnInverseSolve),
  insertionIndex(source.insertionIndex)
{}
