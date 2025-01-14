#include "Puzzle10.h"

#include <assert.h>
#include <fstream>
#include <iostream>>
#include <iterator>
#include <set>
#include <string>
#include <unordered_map>

//https://adventofcode.com/2024/day/10
//Given a 2d heightmap (integers 0 to 9) we are to find all trailheads
//a trailhead is any low point (0), and trails are paths from the trailhead to any peak (9)
//the 'score' of a trailhead is the number of peaks reachable from the trail by making moves
//that only increment height by 1. You cannot go steeper than 1, or go horizontal/down for a trail to count.
//the task is to sum all scores of all trailheads.
std::string Puzzle10::SolvePuzzleA(const char* fileName)
{
	std::vector<std::vector<int64_t>> heights;
    std::vector<Path> unresolvedPaths;

    heights = ParseGrid(fileName);

    for (int x = 0; x < heights[0].size(); x++)
    {
        for (int y = 0; y < heights.size(); y++)
        {
            if (heights[y][x] == 0)
            {
                Path newPath;
                newPath.currentHeight = 0;
                newPath.steps.push_back(Coord(x, y));
                unresolvedPaths.push_back(newPath);
            }
        }
    }

    std::vector<Path> resolvedPaths;
    int iterationCount = 0;
    while (unresolvedPaths.size() > 0)
    {
        iterationCount++;
        std::vector<Path> newPaths;
        Path currentPath = unresolvedPaths.back();
        unresolvedPaths.pop_back();
        if (currentPath.currentHeight == 9)
        {
            resolvedPaths.push_back(currentPath);
        }
        else
        {
            currentPath.ContinuePath(heights, unresolvedPaths);
        }
    }   

    std::unordered_map<Coord, std::set<Coord>, CoordHash> uniqueStartEndPaths;
    for (int i = resolvedPaths.size() - 1; i >= 0; i--)
    {
        Path& path = resolvedPaths[i];
        uniqueStartEndPaths[path.steps[0]].insert(path.steps[9]);
    }

    int sum = 0;
    for (auto& unique : uniqueStartEndPaths)
    {
        sum += unique.second.size();
    }

	return std::to_string(sum);
}

//https://adventofcode.com/2024/day/10#part2
//part B is the same as part A but we are counting distinct paths to 9 instead of number of 9s reachable. (rating)
//so if there are 2 ways to get from a trailhead to a peak, that trailhead has a 'rating' of 2 but a score of 1.
//the puzzle for part b is to sum the ratings rather than scores.
std::string Puzzle10::SolvePuzzleB(const char* fileName)
{
    std::vector<std::vector<int64_t>> heights;
    std::vector<Path> unresolvedPaths;

    heights = ParseGrid(fileName);

    for (int x = 0; x < heights[0].size(); x++)
    {
        for (int y = 0; y < heights.size(); y++)
        {
            if (heights[y][x] == 0)
            {
                Path newPath;
                newPath.currentHeight = 0;
                newPath.steps.push_back(Coord(x, y));
                unresolvedPaths.push_back(newPath);
            }
        }
    }

    std::vector<Path> resolvedPaths;
    int iterationCount = 0;
    while (unresolvedPaths.size() > 0)
    {
        iterationCount++;
        std::vector<Path> newPaths;
        Path currentPath = unresolvedPaths.back();
        unresolvedPaths.pop_back();
        if (currentPath.currentHeight == 9)
        {
            resolvedPaths.push_back(currentPath);
        }
        else
        {
            currentPath.ContinuePath(heights, unresolvedPaths);
        }
    }

    return std::to_string(resolvedPaths.size());
}

bool Puzzle10::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle10/test.txt");
    std::string resultB = SolvePuzzleB("puzzle10/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "36") return false;
    if (resultB != "81") return false;
    return true;
}

std::vector<std::vector<int64_t>> Puzzle10::ParseGrid(const char* fileName)
{
    std::vector<std::vector<int64_t>> grid;
    int64_t width = 0;
    int64_t height = 0;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            if (width == 0)
            {
                width = line.size();
            }
            else
            {
                //grid width mustn't change.
                assert(width == line.size());
            }

            height++;
            std::vector<int64_t> intLine(line.begin(), line.end());
            for (int64_t i = 0; i < intLine.size(); i++)
            {
                intLine[i] = intLine[i] - '0'; //char to int.
            }
            grid.push_back(intLine);
        }
    }

    return grid;
}

bool Puzzle10::Path::ContinuePath(const std::vector<std::vector<int64_t>>& map, std::vector<Path>& outNewPaths)
{
    Coord& currentPosition = *(steps.end() - 1);

    Coord up(currentPosition.x, currentPosition.y - 1);
    Coord down(currentPosition.x, currentPosition.y + 1);
    Coord left(currentPosition.x - 1, currentPosition.y);
    Coord right(currentPosition.x + 1, currentPosition.y);
    if (CheckPosition(map, up, currentHeight + 1))
    {
        Path copyPath = *this;
        copyPath.currentHeight++;
        copyPath.steps.push_back(up);
        outNewPaths.push_back(copyPath);
    }
    if (CheckPosition(map, down, currentHeight + 1))
    {
        Path copyPath = *this;
        copyPath.currentHeight++;
        copyPath.steps.push_back(down);
        outNewPaths.push_back(copyPath);
    }
    if (CheckPosition(map, left, currentHeight + 1))
    {
        Path copyPath = *this;
        copyPath.currentHeight++;
        copyPath.steps.push_back(left);
        outNewPaths.push_back(copyPath);
    }
    if (CheckPosition(map, right, currentHeight + 1))
    {
        Path copyPath = *this;
        copyPath.currentHeight++;
        copyPath.steps.push_back(right);
        outNewPaths.push_back(copyPath);
    }

    return outNewPaths.size() > 0;
}

bool Puzzle10::Path::CheckPosition(const std::vector<std::vector<int64_t>>& map, const Coord& position, int64_t heightCheck)
{
    if (position.x >= 0 && position.x < map[0].size() && position.y >= 0 && position.y < map.size())
    {
        return map[position.y][position.x] == heightCheck;
    }
    return false;
}