#pragma once

#include <set>
#include <vector>
#include <string>
#include "Coord.h"

class Puzzle12
{

public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	class Region
	{
	public:
		char type;
		std::set<Coord> tiles;

		int GetArea() const { return tiles.size(); }
		int GetSides() const;
		int GetPerimeter() const;
		int GetVertexCount(Coord src) const;
	};
	static std::vector<Region> DetermineRegions(const std::vector<std::string>& board);
	static std::set<Coord> FloodFill(Coord start, const std::vector<std::string>& board);
	static bool TileMatches(char type, Coord position, const std::vector<std::string>& board);
	static std::vector<std::string> ParseBoard(const char* fileName);
};

