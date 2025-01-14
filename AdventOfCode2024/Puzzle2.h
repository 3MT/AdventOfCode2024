#pragma once

#include <string>
#include <vector>

class Puzzle2
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);

	static bool RunTests();

private:
	static std::vector<std::vector<int64_t>> ParseFile(const char* fileName);
	static bool ValidIncrements(const std::vector<int64_t>& input, int64_t min, int64_t max);
	static bool ValidDecrements(const std::vector<int64_t>& input, int64_t min, int64_t max);
	static bool ValidIncrementsB(std::vector<int64_t>::iterator begin, std::vector<int64_t>::iterator end, int64_t min, int64_t max);
	static bool ValidDecrementsB(std::vector<int64_t>::iterator begin, std::vector<int64_t>::iterator end, int64_t min, int64_t max);
};

