#pragma once

#include <functional>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

namespace GraphAlgorithms
{

template <class Node, class Score>
Node AStarSearchForClosestTargetNode(const Node& startNode,
                                     std::function<bool(const Node&)> isTargetNode,
                                     std::function<Score(const Node&)> getCostSoFar,
                                     std::function<Score(const Node&)> getEstimatedCostToTarget,
                                     std::function<std::vector<Node>(const Node&)> getSuccessorNodes)
{
  auto priorityFunction = [=](const Node& a, const Node& b)
  {
    return getCostSoFar(a) + getEstimatedCostToTarget(a) > getCostSoFar(b) + getEstimatedCostToTarget(b);
  };
  std::priority_queue<Node, std::vector<Node>, decltype(priorityFunction)> nodes(priorityFunction);
  nodes.push(startNode);

  while (!nodes.empty() && !isTargetNode(nodes.top()))
  {
    auto currentNode = nodes.top();
    nodes.pop();

    auto successorNodes = getSuccessorNodes(currentNode);
    for (const auto& successorNode : successorNodes)
    {
      nodes.push(successorNode);
    }
  }

  if (nodes.empty())
  {
    throw std::runtime_error("Failed to find target node");
  }

  return nodes.top();
}

template <class Node, class Score>
std::optional<Node> GreedySearchForClosestTargetNode(const Node& startNode,
  std::function<bool(const Node&)> isTargetNode,
  std::function<Score(const Node&)> getCostSoFar,
  std::function<Score(const Node&)> getEstimatedCostToTarget,
  std::function<std::vector<Node>(const Node&)> getSuccessorNodes)
{
  Node currentNode = startNode;
  while (true)
  {
    std::optional<Node> bestNode;
    auto successorNodes = getSuccessorNodes(currentNode);
    for (const auto& successorNode : successorNodes)
    {
      if (!bestNode || 
          getCostSoFar(successorNode) + getEstimatedCostToTarget(successorNode) < getCostSoFar(*bestNode) + getEstimatedCostToTarget(*bestNode) ||
          (!isTargetNode(*bestNode) && isTargetNode(successorNode)))
      {
        bestNode = successorNode;
      }
    }
    if (!bestNode || isTargetNode(*bestNode))
    {
      return bestNode;
    }
    currentNode = *bestNode;
  }
}

};