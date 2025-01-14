#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include "Coord.h"

class Puzzle21
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();


private:
	static std::vector<std::string> SplitMoveset(const std::string& input);
	static std::string GetBestDirPath(Coord start, Coord end);
	static std::string GetBestNumpadPath(Coord start, Coord end);
	static int64_t DetermineMovelength(const std::string& keyPresses, int64_t depth, int64_t maxDepth, std::map<char, Coord> dirKeys, std::vector<std::map<std::string, int64_t>>& memoisationTable);
	static int64_t ScoreMoveset(int64_t movesetLength, const std::string& keyCombo);
};

