#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

#include "Cube\Scrambler\Scrambler.h"
#include "CubeSolver\CubeGroup.h"
#include "CubeSolver\Solver\Solver3x3x3.h"

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

std::vector<std::vector<CubeMove>> GetScrambles(Scrambler& scrambler, std::map<std::string, std::string>& commandLineParameters, int defaultRandomScrambles = 10)
{
  std::vector<std::vector<CubeMove>> scrambles;
  auto scrambleFileName = commandLineParameters["scrambles"];
  if (scrambleFileName.empty())
  {
    std::random_device randomSource;
    Scrambler scrambler(randomSource());
    for (int i = 0; i < defaultRandomScrambles; ++i)
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
  return scrambles;
}

std::unique_ptr<std::ostream> GetOutputStream(std::map<std::string, std::string>& commandLineParameters)
{
  auto outputFileName = commandLineParameters["output"];
  if (outputFileName.empty())
  {
    return std::make_unique<std::ostream>(std::cout.rdbuf());
  }
  else
  {
    return std::make_unique<std::ofstream>(outputFileName);
  }
}

enum SolveStrategy
{
  AStar,
  Exhaustive,
  LinearBest
};

SolveStrategy GetSolveStrategy(std::map<std::string, std::string>& commandLineParameters)
{
  auto solveStrategyName = commandLineParameters["strategy"];
  const std::map<std::string, SolveStrategy> strategies
  {
    std::pair<std::string, SolveStrategy>{ "", SolveStrategy::AStar },
    std::pair<std::string, SolveStrategy>{ "AStar", SolveStrategy::AStar },
    std::pair<std::string, SolveStrategy>{ "Exhaustive", SolveStrategy::Exhaustive },
    std::pair<std::string, SolveStrategy>{ "LinearBest", SolveStrategy::LinearBest }
  };
  auto findIter = strategies.find(solveStrategyName);
  if (findIter == strategies.end())
  {
    throw std::runtime_error("Unknown strategy: " + solveStrategyName);
  }
  return findIter->second;
}

Solution Solve(const std::vector<CubeMove>& scramble, const Solver3x3x3& solver, SolveStrategy solveStrategy)
{
  switch (solveStrategy)
  {
    case AStar:
      return solver.Solve(scramble);

    case Exhaustive:
      return solver.ExhaustiveSolveToState(scramble).solutionSoFar;

    case LinearBest:
    {
      auto solutionSoFar = PartialSolution{ Solution{}, CubeGroup::Scrambled };
      const std::array<std::set<CubeGroup>, 3> targetStatesPerStep
      {
        std::set<CubeGroup>{ CubeGroup::Block2x2x2 },
        std::set<CubeGroup>{ CubeGroup::Block2x2x3_EO },
        std::set<CubeGroup>{ CubeGroup::AB5C_twisted, CubeGroup::AB5C_2cycles, CubeGroup::AB5C_3cycle, CubeGroup::AB5C_5cycle, CubeGroup::AB4C_twisted, CubeGroup::AB4C_3cycle, CubeGroup::AB4C_2cycles, CubeGroup::AB3C_twisted, CubeGroup::AB3C_3cycle, CubeGroup::AB2C_twisted },
      };
      for (const auto& targetStates : targetStatesPerStep)
      {
        solutionSoFar = solver.ExhaustiveSolveToState(scramble, solutionSoFar, targetStates);
      }
      auto finalSolution = solver.LinearBestSolveToState(scramble, solutionSoFar);
      return finalSolution->solutionSoFar;
    }

    default:
      throw std::domain_error("Unhandled solve strategy " + std::to_string(static_cast<int>(solveStrategy)));
  }
}

int main(int argc, char* argv[])
{
  try
  {
    auto commandLineParameters = ReadCommandLineParameters(argc, argv);
    
    std::random_device randomSource;
    Scrambler scrambler(randomSource());
    auto scrambles = GetScrambles(scrambler, commandLineParameters);

    auto outputStream = GetOutputStream(commandLineParameters);
    auto solveStrategy = GetSolveStrategy(commandLineParameters);

    Solver3x3x3 solver;
    for (const auto& scramble : scrambles)
    {
      *outputStream << MoveSequenceToText(scramble) << "\n\n";
      
      auto solution = Solve(scramble, solver, solveStrategy);
      *outputStream << solution << "\n\n===============================================================================\n\n";
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}