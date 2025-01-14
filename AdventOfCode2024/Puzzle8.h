#pragma once
#include <map>
#include <vector>

#include "Coord.h"

class Puzzle8
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	class PuzzleBoard
	{
	public:
		PuzzleBoard()
		{
			width = 0;
			height = 0;
		}
		void ParseBoard(const char* fileName);
		void CalculateAntiPolesA();
		void CalculateAntiPolesB();
		void PrintAntiPoles();
		size_t CountAntiPoles() const;

	private:
		int width;
		int height;
		std::map<char, std::vector<Coord>> stations;
		std::vector<std::vector<bool>> antipoles;
	};
};

