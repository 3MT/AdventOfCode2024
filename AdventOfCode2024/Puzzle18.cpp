#include "Puzzle18.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

int64_t c_Unreachable = INT64_MAX;
//https://adventofcode.com/2024/day/18
//we are on a rectangular board and must navigate from the top left to the bottom right, our input is a series of coordinates that become impassible over time.
//for part 1, 1024 of those coordinates are applied, corrupting their tiles and making them impassible. We are then required to count the shortest number of steps required to reach the ending.
std::string Puzzle18::SolvePuzzleA(const char* fileName)
{
    std::vector<Coord> corruptions;

	std::ifstream myFile(fileName);

	//parse.
	if (myFile.is_open())
	{
		std::string line;
		while (getline(myFile, line))
		{
			try
			{
				//format "x,y" x and y are positive ints
				size_t index = 0;
				int64_t left = stoll(line, &index);
				index++; //','
				line = line.substr(index);
				int64_t right = stoll(line);
				corruptions.push_back(Coord(left, right));
			}
			catch (...) //catch failed stoll
			{
				std::cout << "Parse failed";
			}
		}
	}

	int64_t width = 71;
	int64_t height = 71;
	int64_t corruptionCount = 1024;
	if (corruptions.size() < 50)
	{
		//test case
		width = 7;
		height = 7;
		corruptionCount = 12;
	}

	//corrupt the map
	std::set<Coord> corruptedTiles;
	corruptedTiles.insert(corruptions.begin(), corruptions.begin() + corruptionCount);

	//flood fill distances.
	std::vector<std::vector<int64_t>> distances;
	for (int y = 0; y < height; y++)
	{
		std::vector<int64_t> line;
		line.resize(width, c_Unreachable);
		distances.push_back(line);
	}
	
	//flood fill distances.
	//question asks for final position but I'm suspicious part B will require a flood fill so doing that instead ;)
	std::set<Coord> front;
	front.insert(Coord(0, 0));
	distances[0][0] = 0;
	while (front.size() != 0)
	{
		Coord node = *front.begin();
		front.erase(node);
		int64_t distance = distances[node.y][node.x];

		for (int i = 0; i < 4; i++)
		{
			Coord neighbour = node + Coord::CardinalDirections[i];
			if (neighbour.x < 0 || neighbour.y < 0 || neighbour.x >= width || neighbour.y >= height)
			{
				//oob
				continue;
			}
			if (corruptedTiles.contains(neighbour))
			{
				//blocked.
				continue;
			}
			if (distance + 1 < distances[neighbour.y][neighbour.x])
			{
				distances[neighbour.y][neighbour.x] = distance + 1;
				front.insert(neighbour);
			}
		}
	}

	//goal position
	Coord goal(height - 1, width - 1);
    return std::to_string(distances[goal.y][goal.x]);
}

//similar to part A, but we need to determine the first coordinate to be corrupted that will prevent us from reaching the end.
std::string Puzzle18::SolvePuzzleB(const char* fileName)
{
	std::vector<Coord> corruptions;

	std::ifstream myFile(fileName);

	//parse.
	if (myFile.is_open())
	{
		std::string line;
		while (getline(myFile, line))
		{
			try
			{
				//format "x,y" x and y are positive ints
				size_t index = 0;
				int64_t left = stoll(line, &index);
				index++; //','
				line = line.substr(index);
				int64_t right = stoll(line);
				corruptions.push_back(Coord(left, right));
			}
			catch (...) //catch failed stoll
			{
				std::cout << "Parse failed";
			}
		}
	}

	int64_t width = 71;
	int64_t height = 71;
	int64_t corruptionCount = corruptions.size();
	if (corruptions.size() < 50)
	{
		//test case
		width = 7;
		height = 7;
	}

	int64_t finalDistance = c_Unreachable;
	Coord goal(height - 1, width - 1);
	//we are doing a binary search here, so we set an upper and lower bound, then check in between them.
	//if our chosen iteration is pathable, we are too low, if it isn't pathable, it's too high.
	//if lowerbound ever = upperbound we have narrowed it down to the correct answer.
	int64_t lowBound = 0;
	int64_t upperBound = corruptions.size();
	while (upperBound != lowBound)
	{
		corruptionCount = (upperBound + lowBound) / 2;
		//corrupt the map
		std::set<Coord> corruptedTiles;
		corruptedTiles.insert(corruptions.begin(), corruptions.begin() + corruptionCount);

		//flood fill distances.
		std::vector<std::vector<int64_t>> distances;
		for (int y = 0; y < height; y++)
		{
			std::vector<int64_t> line;
			line.resize(width, c_Unreachable);
			distances.push_back(line);
		}

		//flood fill distances.
		//question asks for final position but I'm suspicious part B will require a flood fill so doing that instead ;)
		//update: pranked, they do not need a flood fill :'(
		//i'm gonna commit to floodfill though because I think binary search will be more fun to implement than doing A* for the 10000000000000000000000000000000000000th time in my career.
		std::set<Coord> front;
		front.insert(Coord(0, 0));
		distances[0][0] = 0;
		while (front.size() != 0)
		{
			Coord node = *front.begin();
			front.erase(node);
			int64_t distance = distances[node.y][node.x];

			for (int i = 0; i < 4; i++)
			{
				Coord neighbour = node + Coord::CardinalDirections[i];
				if (neighbour.x < 0 || neighbour.y < 0 || neighbour.x >= width || neighbour.y >= height)
				{
					//oob
					continue;
				}
				if (corruptedTiles.contains(neighbour))
				{
					//blocked.
					continue;
				}
				if (distance + 1 < distances[neighbour.y][neighbour.x])
				{
					distances[neighbour.y][neighbour.x] = distance + 1;
					front.insert(neighbour);
				}
			}
		}

		//goal position
		finalDistance = distances[goal.y][goal.x];

		//binary search
		if (finalDistance == c_Unreachable)
		{
			upperBound = corruptionCount;
		}
		else
		{
			lowBound = corruptionCount;
		}
		if (upperBound - 1 == lowBound)
		{
			//truncated rounding edge case, upperbound == lowerbound or upperbound -1  == lowerbound both indicate we are on target.
			upperBound = lowBound;
		}
	}
	return std::to_string(corruptions[corruptionCount].x) + "," + std::to_string(corruptions[corruptionCount].y);
}

void Puzzle18::RenderBoard(const std::set<Coord>& coords, int64_t width, int64_t height)
{

	std::stringstream out;
	out << "\n";
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			bool found = coords.contains(Coord(x, y));
			out << (found ? "#" : ".");
		}
		out << "\n";
	}

	system("cls");
	std::cout << out.str();
}

bool Puzzle18::RunTests()
{
	//test case answers provided by question
	std::string resultA = SolvePuzzleA("puzzle18/test.txt");
	std::string resultB = SolvePuzzleB("puzzle18/test.txt");
#if DEBUG_OUTPUT
	std::cout << "A: " << resultA << "\n";
	std::cout << "B: " << resultB << "\n";
#endif
	if (resultA != "22") return false;
	if (resultB != "6,1") return false;
	return true;
}