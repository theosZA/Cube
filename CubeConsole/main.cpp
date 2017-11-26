#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>

#include "..\Cube\3x3x3\SolverStep_2x2x2Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3EO.h"
#include "..\Cube\3x3x3\SolverStep_EO.h"
#include "..\Cube\3x3x3\SolverStep_DominoReduction.h"
#include "..\Cube\3x3x3\SolverStep_DominoToDoubleTurns.h"
#include "..\Cube\Scrambler\Scrambler.h"

std::vector<PartialSolution> GetAllPartialSolutions(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution)
{
  // Rotate to canonical orientation.
  auto rotatedScramble = Rotate(scramble, partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);
  auto rotatedSolutionSoFar = partialSolution.solutionSoFar.Rotate(partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);

  // Get solutions.
  auto rotatedSolutions = solverStep.Solve(rotatedScramble, rotatedSolutionSoFar);
  auto inverseRotatedSolutions = solverStep.Solve(InvertMoveSequence(rotatedScramble), rotatedSolutionSoFar.Invert());
  for (auto& inverseRotatedSolution : inverseRotatedSolutions)
  {
    rotatedSolutions.push_back(inverseRotatedSolution.Invert());
  }

  // Rotate back to original orientation.
  std::vector<PartialSolution> solutions;
  solutions.reserve(rotatedSolutions.size());
  std::transform(rotatedSolutions.begin(), rotatedSolutions.end(), std::back_inserter(solutions),
      [&](const PartialSolution& rotatedSolution)
      {
        return rotatedSolution.Rotate(partialSolution.rotationNewFaces, partialSolution.rotationOldFaces);
      });
  return solutions;
}

template <class Random>
PartialSolution SolveRandom(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution, Random& generator)
{
  auto solutions = GetAllPartialSolutions(solverStep, scramble, partialSolution);
  auto randomIndex = std::uniform_int_distribution<size_t>(0, solutions.size() - 1)(generator);
  return solutions[randomIndex];
}

PartialSolution SolveBest(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution)
{
  auto solutions = GetAllPartialSolutions(solverStep, scramble, partialSolution);
  return *std::min_element(solutions.begin(), solutions.end(),
      [](const PartialSolution& a, const PartialSolution& b)
      {
        return a.solutionSoFar.Length() < b.solutionSoFar.Length();
      });
}

std::map<std::string, std::string> ReadCommandLineParameters(int argc, char* argv[])
{
  std::map<std::string, std::string> commandLineParameters;
  for (int i = 1; i < argc; ++i)
  {
    std::string parameter = argv[i];
    auto equalsPos = parameter.find('=');
    if (equalsPos != std::string::npos)
    {
      auto key = parameter.substr(0, equalsPos);
      auto value = parameter.substr(equalsPos + 1);
      commandLineParameters[key] = value;
    }
  }
  return commandLineParameters;
}

int main(int argc, char* argv[])
{
  auto commandLineParameters = ReadCommandLineParameters(argc, argv);

  std::vector<std::vector<CubeMove>> scrambles;
  auto scrambleFileName = commandLineParameters["scrambles"];
  if (scrambleFileName.empty())
  {
    std::random_device randomSource;
    Scrambler scrambler(randomSource());
    for (int i = 0; i < 10; ++i)
    {
      scrambles.push_back(scrambler.CreateRandomScramble(25));
    }
  }
  else
  {
    std::ifstream scrambleFile(scrambleFileName);
    std::string line;
    while (std::getline(scrambleFile, line))
    { // Skip empty lines or comment lines.
      if (!line.empty() && (line.size() < 2 || line[0] != '/' || line[1] != '/'))
      {
        // Skip to the first move on the line.
        auto firstMovePos = line.find_first_of("UDFRBL");
        // Ignore whitespace at the end of the line.
        auto endPos = line.find_last_not_of(" \t\r\n");
        if (firstMovePos != std::string::npos)
        {
          scrambles.push_back(ParseMoveSequence(line.substr(firstMovePos, endPos + 1 - firstMovePos)));
        }
      }
    }
  }
  
  std::ostream* out;
  auto outputFileName = commandLineParameters["output"];
  if (outputFileName.empty())
  {
    out = &std::cout;
  }
  else
  {
    out = new std::ofstream(outputFileName);
  }

  SolverStep_2x2x2Block solver2x2x2("block2x2x2.3x3");

  for (const auto& scramble : scrambles)
  {
    *out << "Scramble: " << MoveSequenceToText(scramble) << "\n\n";

    auto partialSolutions_2x2x2 = GetAllPartialSolutions(solver2x2x2, scramble, PartialSolution{ Solution{}, CubeGroup::Scrambled });
    std::sort(partialSolutions_2x2x2.begin(), partialSolutions_2x2x2.end(),
              [](const PartialSolution& a, const PartialSolution& b)
              {
                return a.solutionSoFar.Length() < b.solutionSoFar.Length();
              });
    
    for (auto partialSolution : partialSolutions_2x2x2)
    {
      *out << partialSolution.solutionSoFar.Length() << ' ' << partialSolution.solutionSoFar.GetStepsDescription();
    }
    *out << "============================================================\n";
  }

  if (!outputFileName.empty())
  {
    delete out;
  }
}