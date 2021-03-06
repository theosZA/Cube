#pragma once

#include "CubeStateSolver.h"

#include <fstream>

template<class CubeType>
CubeStateSolver<CubeType>::CubeStateSolver(std::uint32_t maxMoves, const std::function<std::uint32_t(const CubeType&)> getKey,
                                           const std::vector<CubeMove>& possibleMoves, const std::string& cacheFileName)
: graph(getKey,
  [=](const CubeType& cube)
  {
    std::vector<std::pair<CubeMove, CubeType>> adjacentStates;
    adjacentStates.reserve(possibleMoves.size());
    for (const auto& move : possibleMoves)
    {
      CubeType newCube = cube;
      newCube += move;
      adjacentStates.push_back(std::pair<CubeMove, CubeType>{ move, newCube});
    }
    return adjacentStates;
  })
{
  if (!ReadCacheFile(cacheFileName))
  {
    graph.Build(CubeType{}, maxMoves);
    WriteCacheFile(cacheFileName);
  }
}

template<class CubeType>
std::vector<CubeMove> CubeStateSolver<CubeType>::Solve(const CubeType& cube) const
{
  return graph.FindShortestPathToRoot(cube);
}

template<class CubeType>
std::vector<CubeMove> CubeStateSolver<CubeType>::Solve(const std::vector<CubeMove>& scramble) const
{
  CubeType cube;
  cube += scramble;
  return Solve(cube);
}

template<class CubeType>
std::uint32_t CubeStateSolver<CubeType>::GetRequiredMoveCount(const CubeType& cube) const
{
  return graph.GetShortestPathLength(cube);
}

template<class CubeType>
std::uint32_t CubeStateSolver<CubeType>::GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const
{
  CubeType cube;
  cube += scramble;
  return GetRequiredMoveCount(cube);
}

template<class CubeType>
bool CubeStateSolver<CubeType>::CanSolve(const CubeType& cube) const
{
  return graph.ContainsVertex(cube);
}

template<class CubeType>
void CubeStateSolver<CubeType>::WriteCacheFile(const std::string& cacheFileName) const
{
  if (!cacheFileName.empty())
    graph.WriteToStream(std::ofstream(cacheFileName, std::ios::binary));
}

template<class CubeType>
bool CubeStateSolver<CubeType>::ReadCacheFile(const std::string& cacheFileName)
{
  return !cacheFileName.empty() && graph.ReadFromStream(std::ifstream(cacheFileName, std::ios::binary));
}
