#pragma once

#include <cstdint>
#include <vector>
#include <string>

class Puzzle9
{
public:	
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::vector<int64_t> ParseFile(const char* fileName);
	static std::vector<int64_t> PackData(std::vector<int64_t>& input);
	static std::vector<int64_t> NoFragmentPacking(std::vector<int64_t>& input);
	static void VisualiseData(const std::vector<int64_t>& input);
	static int64_t CalculateChecksum(const std::vector<int64_t>& input);
	static int64_t CountFileLength(const std::vector<int64_t>& input, int64_t startIndex, int64_t endIndex, int64_t fileID);
	static int64_t CountFileLengthReverse(const std::vector<int64_t>& input, int64_t startIndex, int64_t fileID);
};

