#include "Puzzle4.h"

#include <assert.h>
#include <fstream>>
#include <iostream>

//https://adventofcode.com/2024/day/4
//2d grid input, simple word search, find XMAS going diagonal, horizontal and vertical (reverse also works)
std::string Puzzle4::SolvePuzzleA(const char* fileName)
{
    PuzzleGrid grid;
    grid.ParseGrid(fileName);
    int64_t xmasCount = 0;
    for (int64_t x = 0; x < grid.GetWidth(); x++)
    {
        for (int64_t y = 0; y < grid.GetHeight(); y++)
        {
            if (grid.GetChar(x, y) != 'X')
                continue; //mostly doing this to make debugging less tedious lol

            if (SearchForString(grid, x, y, "XMAS", 0, -1)) xmasCount++; //up
            if (SearchForString(grid, x, y, "XMAS", 1, -1)) xmasCount++; //up right
            if (SearchForString(grid, x, y, "XMAS", 1, 0)) xmasCount++; //right
            if (SearchForString(grid, x, y, "XMAS", 1, 1)) xmasCount++; //down right
            if (SearchForString(grid, x, y, "XMAS", 0, 1)) xmasCount++; //down
            if (SearchForString(grid, x, y, "XMAS", -1, 1)) xmasCount++; //down left
            if (SearchForString(grid, x, y, "XMAS", -1, 0)) xmasCount++; //left
            if (SearchForString(grid, x, y, "XMAS", -1, -1)) xmasCount++; //up left
        }
    }
	return std::to_string(xmasCount);
}

//https://adventofcode.com/2024/day/4#part2
//twist on word search, we're looking for "MAS" in an X shape (horizontal vertical reverse diagonal etc etc.)
//examples of valid X-MASes
// M*M  *M* 
// *A*  MAS
// S*S  *S*
std::string Puzzle4::SolvePuzzleB(const char* fileName)
{
    PuzzleGrid grid;
    grid.ParseGrid(fileName);
    int64_t xmasCount = 0;
    for (int64_t x = 1; x < grid.GetWidth()-1; x++) //can't possibly form an 'X-MAS' on the top bottom right and left column/rows so skip them.
    {
        for (int64_t y = 1; y < grid.GetHeight()-1; y++)
        {
            if (grid.GetChar(x, y) != 'A')
                continue; //mostly doing this to make debugging less tedious lol

            if (SearchForCrossStringDiagonal(grid, x, y, "MAS")) xmasCount++;
        }
    }
    return std::to_string(xmasCount);
}

bool Puzzle4::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle04/test.txt");
    std::string resultB = SolvePuzzleB("puzzle04/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "18") return false;
    if (resultB != "9") return false;
    return true;
}

bool Puzzle4::SearchForString(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString, int64_t xDir, int64_t yDir)
{
    for (int64_t i = 0; i < (int64_t)searchString.size(); i++)
    {
        if (grid.GetChar(x + (xDir * i), y + (yDir * i)) != searchString[i])
            return false;
    }
    return true;
}

//you're not actually meant to make this one woops.
bool Puzzle4::SearchForCrossString(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString)
{
    int64_t count = 0;
    count += SearchForString(grid, x, y+1, searchString, 0, -1); //up
    count += SearchForString(grid, x, y-1, searchString, 0, 1); //down
    count += SearchForString(grid, x-1, y, searchString, 1, 0); //right
    count += SearchForString(grid, x+1, y, searchString, -1, 0); //left

#ifdef DEBUG_OUTPUT
    if (count == 2)
    {
        std::cout << ' '                        << grid.GetChar(x + 0, y - 1) << ' '                        << "\n";
        std::cout << grid.GetChar(x - 1, y + 0) << grid.GetChar(x + 0, y + 0) << grid.GetChar(x + 1, y + 0) << "\n";
        std::cout << ' '                        << grid.GetChar(x + 0, y + 1) << ' '                        << "\n";
        std::cout << x << "," << y << "\n\n";
    }
#endif
    return (count == 2);
}

bool Puzzle4::SearchForCrossStringDiagonal(const PuzzleGrid& grid, int64_t x, int64_t y, const std::string& searchString)
{
    int64_t count = 0;
    count += SearchForString(grid, x+1, y+1, searchString, -1, -1); //up left
    count += SearchForString(grid, x-1, y-1, searchString, 1, 1); //down right
    count += SearchForString(grid, x-1, y+1, searchString, 1, -1); //up right
    count += SearchForString(grid, x+1, y-1, searchString, -1, 1); //down left

#ifdef DEBUG_OUTPUT
    if (count == 2)
    {
        std::cout << grid.GetChar(x - 1, y - 1) << ' '                        << grid.GetChar(x + 1, y - 1) << "\n";
        std::cout << ' '                        << grid.GetChar(x + 0, y + 0) << ' '                        << "\n";
        std::cout << grid.GetChar(x - 1, y + 1) << ' '                        << grid.GetChar(x + 1, y + 1) << "\n";
        std::cout << x << "," << y << "\n\n";
    }
#endif
    return (count == 2);
}

void Puzzle4::PuzzleGrid::ParseGrid(const char* fileName)
{
    //input format is a rectangular jumble of the characters in XMAS
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
            std::vector<char> charLine(line.begin(), line.end());
            tiles.push_back(charLine);
        }
    }
}
