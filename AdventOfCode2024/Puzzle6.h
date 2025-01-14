#pragma once
#include <vector>
#include <string>

#include <iostream>

class Puzzle6
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	class GuardedRoom
	{
	public:
		enum TileType
		{
			CLEAR,
			VISITED,
			BLOCKED,
		};

		enum Direction
		{
			NONE = 0,
			LEFT = 1 << 0,
			RIGHT = 1 << 1,
			UP = 1 << 2,
			DOWN = 1 << 3
		};

		GuardedRoom()
		{
			guardX = 0;
			guardY = 0;
			initialGuardX = 0;
			initialGuardY = 0;
			height = 0;
			width = 0;
			guardDirection = Direction::NONE;
			guardOnMap = false;
			guardInLoop = false;
		}
		int64_t GetWidth() const { return width; }
		int64_t GetHeight() const { return height; }
		void TickGrid();
		void ParseGrid(const char* fileName);
		void PrintGrid(int64_t x, int64_t y) const;
		bool CanTick() const { return guardOnMap && !guardInLoop; };
		bool GuardStuck() const { return guardInLoop; };
		int64_t SumVisitedTiles() const;
		void Reset();

		void SetTile(int64_t x, int64_t y, TileType newType) { tiles[y][x] = newType; }

		bool ValidObstaclePos(int64_t x, int64_t y) const { return tiles[y][x] == TileType::CLEAR && (x != initialGuardX || y != initialGuardY); }
	private:
		std::vector<std::vector<TileType>> tiles;
		std::vector<std::vector<Direction>> visitedTiles;
		int64_t height;
		int64_t width;
		int64_t guardX;
		int64_t guardY;
		int64_t initialGuardX;
		int64_t initialGuardY;
		Direction guardDirection;
		bool guardOnMap;
		bool guardInLoop;
	};
};