#pragma once

#include "Coord.h"
#include <map>
#include <vector>
#include <set>

class Puzzle16
{
public:
	struct PathNode
	{
	public:
		Coord position;
		int64_t distance = INT64_MAX;
	};

	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::vector<std::vector<char>> ParseGrid(std::ifstream& file);

	static int64_t ScorePath(const std::vector<PathNode>& path, const Coord& end, bool requireEnding = true);
	static void BuildPaths(std::vector<std::vector<PathNode>>& paths, std::map<Coord, PathNode>& nodes, PathNode& node, int path, int index, const Coord& start, const Coord& end);
	static bool BuildGoodPaths(std::vector<std::vector<Puzzle16::PathNode>>& outPaths, std::vector<Puzzle16::PathNode> previousNodes, Coord& node, const Coord& end, int64_t& bestScore, int64_t runningScore, const std::vector<std::vector<char>>& map, std::map<Coord, int64_t>& bestScores);

	static void PrintCosts(std::map<Coord, PathNode> nodes, const std::vector<std::vector<char>>& map);
};

