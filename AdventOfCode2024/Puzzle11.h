#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>

class Puzzle11
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::vector<uint64_t> NaiveIterateStones(const std::vector<uint64_t>& inputStones);
	static std::unordered_map<uint64_t, uint64_t> IterateMapStones(const std::unordered_map<uint64_t, uint64_t>& inputStones);
};

