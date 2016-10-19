#include "catch.hpp"

#include "Graph/RootedGraph.h"

int GetKey(int v)
{ 
  return 10000 + v; 
}

int GetModuloKey(int v)
{
  return v % 10;
}

auto GetNoAdjacentVertices(int)
{
  return std::vector<std::pair<int, int>>{}; 
}

const int odd = 77;
const int even = 93;
const int half = 127;
auto GetThreeAdjacentVertices(int v)
{
  return std::vector<std::pair<int, int>>
  {
    std::pair<int, int>{ even, 2 * v },
    std::pair<int, int>{ odd,  2 * v + 1 },
    std::pair<int, int>{ half, v / 2 }
  };
}

TEST_CASE("Rooted graph - Ensure we can't find a path on an empty graph", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetNoAdjacentVertices);
  CHECK_THROWS(graph.FindShortestPathToRoot(0));
}

TEST_CASE("Rooted graph - Ensure we can find an empty path from root to root", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetNoAdjacentVertices);
  const int root = 17;
  graph.Build(root, 0);
  CHECK(graph.FindShortestPathToRoot(root).empty());
}

TEST_CASE("Rooted graph - Ensure we can find an empty path from root to root in a populated graph", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK(graph.FindShortestPathToRoot(root).empty());
}

TEST_CASE("Rooted graph - Ensure we can find an empty path from root to root in a populated graph under key equivalency", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetModuloKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK(graph.FindShortestPathToRoot(27).empty());
}

TEST_CASE("Rooted graph - Ensure we can find all adjacent vertices as a length 1 path", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK(graph.FindShortestPathToRoot(8) == std::vector<int>{ odd });
  CHECK(graph.FindShortestPathToRoot(34) == std::vector<int>{ half });
  CHECK(graph.FindShortestPathToRoot(35) == std::vector<int>{ half });
}

TEST_CASE("Rooted graph - Ensure we can find a length 3 path", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK(graph.FindShortestPathToRoot(32) == (std::vector<int>{ half, half, odd }));
}

TEST_CASE("Rooted graph - Ensure we can find a maximum length path", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK(graph.FindShortestPathToRoot(266) == (std::vector<int>{ half, half, half, half, half, odd }));
}

TEST_CASE("Rooted graph - Ensure we can't find a path longer than maximum length", "[RootedGraph]")
{
  RootedGraph<int, int, int> graph(GetKey, GetThreeAdjacentVertices);
  const int root = 17;
  graph.Build(root, 6);
  CHECK_THROWS(graph.FindShortestPathToRoot(532));
}
