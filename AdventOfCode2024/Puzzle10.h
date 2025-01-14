#pragma once

#include <cstdint>
#include <vector>

#include "Coord.h"

class Puzzle10
{
public:
	struct Path
	{
	public :
		std::vector<Coord> steps;
		int64_t currentHeight = 0;
		bool ContinuePath(const std::vector<std::vector<int64_t>>& map, std::vector<Path>& outNewPaths);
		bool CheckPosition(const std::vector<std::vector<int64_t>>& map, const Coord& position, int64_t heightCheck);
	};
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::vector<std::vector<int64_t>> ParseGrid(const char* fileName);
};

