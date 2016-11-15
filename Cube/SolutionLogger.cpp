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

void SolutionLogger::LogStep(const std::string& stepDescription)
{
  log << stepDescription << '\n';
}

void SolutionLogger::LogSolution(const std::vector<CubeMove>& solution)
{
  log << "Final solution (" << solution.size() << " moves): " << MoveSequenceToText(solution)
      << "\n\n-----------------------------------------------\n\n";
  log.flush();
}
