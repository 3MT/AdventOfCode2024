#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "Coord.h"

class Puzzle15
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static bool TryRobotMove(Coord start, Coord direction, std::vector<std::vector<char>>& map);
	static bool TryWideRobotMove(Coord start, Coord direction, std::vector<std::vector<char>>& map);
	static int64_t SumGPS(const std::vector<std::vector<char>>& map);
	static std::vector<std::vector<char>> ParseGrid(std::ifstream& file);
	static std::vector<std::vector<char>> ParseWideGrid(std::ifstream& file);
	static std::string ParseRobotCommands(std::ifstream& file);
};

