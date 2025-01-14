#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <map>

class Puzzle19
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static void ParseFile(const char* fileName, std::vector<std::set<std::string>>& outPatterns, std::vector<std::string>& outDesigns);
	static bool IsDesignPossible(const std::string& design, const std::vector<std::set<std::string>>& patterns);
	static int64_t CountPossibleDesigns(const std::string& design, std::map<std::string, int64_t>& scoreCache, const std::vector<std::set<std::string>>& patterns);
};

