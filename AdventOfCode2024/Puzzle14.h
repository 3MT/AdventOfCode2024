#pragma once
#include <cstdint>
#include <string>

#include "Coord.h"

class Puzzle14
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	class Robot
	{
	public:
		Coord position;
		Coord velocity; //might be better to rename it to vector

		void Update(int64_t worldWidth, int64_t worldHeight);
	};

	static std::vector<Robot> ParseFile(const char* fileName);
	static void DrawBoard(const std::vector<Robot>& robots, int64_t width, int64_t height);
	static int64_t CalculateSafetyFactor(const std::vector<Robot>& robots, int64_t width, int64_t height);
	static int64_t CalculateWeirdness(const std::vector<Robot>& robots, int64_t width, int64_t height);
};

