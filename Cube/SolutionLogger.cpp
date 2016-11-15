#include "SolutionLogger.h"

SolutionLogger::SolutionLogger(const std::string& fileName)
: log(fileName, std::ios::trunc),
  count(0)
{}

void SolutionLogger::LogScramble(const std::vector<CubeMove>& scramble)
{
  ++count;
  log << "Scramble " << count << ": " << MoveSequenceToText(scramble) << "\n\n";
  log.flush();
}

void SolutionLogger::LogSolution(const Solution& solution)
{
  for (const auto& step : solution.steps)
  {
    if (step.skeletonPreceedingMoves.empty() && step.skeletonSucceedingMoves.empty())
      log << step.description << ": " << MoveSequenceToText(step.moves) << '\n';
    else
      log << step.description << ": Skeleton = " << MoveSequenceToText(step.skeletonPreceedingMoves) << " * " << MoveSequenceToText(step.skeletonSucceedingMoves)
          << "\n  Insert at * = " << MoveSequenceToText(step.moves) << '\n';
  }

  log << "\nFinal solution (" << solution.moves.size() << " moves): " << MoveSequenceToText(solution.moves)
      << "\n\n-----------------------------------------------\n\n";
  log.flush();
}
