#pragma once

#include <vector>
#include <string>

class Puzzle4
{
public:
	class PuzzleGrid
	{
	public:
		PuzzleGrid()
		{
			height = 0;
			width = 0;
		}

		char GetChar(int64_t x, int64_t y) const
		{
			if (x < 0 || x >= width || y < 0 || y >= width)
			{
				return '?';
			}
			else
			{
				return tiles[y][x];
			}
		}

		void ParseGrid(const char* fileName);
		int64_t GetWidth() const { return width; }
		int64_t GetHeight() const { return height; }

	private:
		std::vector<std::vector<char>> tiles;
		int64_t height;
		int64_t width;
	};

	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static bool SearchForString(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString, int64_t xDir, int64_t yDir);
	static bool SearchForCrossString(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString);
	static bool SearchForCrossStringDiagonal(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString);
};

