#pragma once

#include <cstdint>
#include <set>

#include "Coord.h"

class Puzzle18
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static void RenderBoard(const std::set<Coord>& coords, int64_t width, int64_t height);
};

