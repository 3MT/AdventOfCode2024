#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Puzzle22
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static int64_t IterateSecretNumber(int64_t input);
	static int64_t IterateSecret(int64_t secret);
	static int64_t Mix(int64_t input, int64_t input2);
	static int64_t Prune(int64_t input);
	static std::vector<int64_t> ParseFile(const char* fileName);
};

