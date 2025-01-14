#pragma once

#include <cstdint>
#include "Coord.h"

class Puzzle13
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	class ClawMachine
	{
	public:
		Coord buttonA; //costs 1
		Coord buttonB; //costs 3
		Coord target;

		ClawMachine()
		{
		};

		int64_t FindWinSolver();//search for a win with the 2 buttons, if no win returns -1
		int64_t FindWinNaive();//search for a win with the 2 buttons, if no win returns -1
	};
};

