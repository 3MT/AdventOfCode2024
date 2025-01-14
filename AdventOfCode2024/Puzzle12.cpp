#include "Puzzle12.h"

#include <assert.h>
#include <fstream>
#include <iostream>

//https://adventofcode.com/2024/day/12
//given a set of regions in a 2D rect (identified by unique chars) we are to work out the 'cost' of fencing the area.
//the cost is found by multiplying area by perimeter.
//then we are to sum all of these costs.
std::string Puzzle12::SolvePuzzleA(const char* fileName)
{
    std::vector<std::string> board = ParseBoard(fileName);
    std::vector<Region> regions = DetermineRegions(board);
    int sum = 0;

    for (const Region& region : regions)
    {
        sum += region.GetArea() * region.GetPerimeter();
    }

	return std::to_string(sum);
}

//https://adventofcode.com/2024/day/12#part2
//as part A, but the cost is now area multiplied by number of sides
//a side is any straight edge on the area covered, regardless of length.
std::string Puzzle12::SolvePuzzleB(const char* fileName)
{
    std::vector<std::string> board = ParseBoard(fileName);
    std::vector<Region> regions = DetermineRegions(board);
    int sum = 0;

    for (const Region& region : regions)
    {
        sum += region.GetArea() * region.GetSides();
    }

    return std::to_string(sum);
}

bool Puzzle12::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle12/test.txt");
    std::string resultB = SolvePuzzleB("puzzle12/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "1930") return false;
    if (resultB != "1206") return false;
    return true;
}

std::vector<Puzzle12::Region> Puzzle12::DetermineRegions(const std::vector<std::string>& board)
{
    std::vector<Region> regions;
    std::set<Coord> seenTiles;
    for (int x = 0; x < board[0].size(); x++)
    {
        for (int y = 0; y < board.size(); y++)
        {
            Coord position = Coord(x, y);
            if (seenTiles.contains(position))
            {
                continue;
            }

            Region newRegion;
            newRegion.type = board[y][x];
            newRegion.tiles = FloodFill(position, board);
            seenTiles.insert(newRegion.tiles.begin(), newRegion.tiles.end());
            regions.push_back(newRegion);
        }
    }
    return regions;
}

std::set<Coord> Puzzle12::FloodFill(Coord start, const std::vector<std::string>& board)
{
    std::set<Coord> seenTiles;
    std::set<Coord> regionTiles;
    std::set<Coord> searchFront;
    char type = board[start.y][start.x];

    regionTiles.insert(start);
    seenTiles.insert(start);
    searchFront.insert(start + Coord(0,1));
    searchFront.insert(start + Coord(0,-1));
    searchFront.insert(start + Coord(1,0));
    searchFront.insert(start + Coord(-1,0));

    while (searchFront.size() > 0)
    {
        Coord src = *searchFront.begin();
        searchFront.erase(src);
        if (seenTiles.contains(src))
        {
            continue;
        }
        seenTiles.insert(src);

        if (TileMatches(type, src, board))
        {
            regionTiles.insert(src);

            Coord up = src + Coord(0, -1);
            Coord down = src + Coord(0, 1);
            Coord left = src + Coord(-1, 0);
            Coord right = src + Coord(1, 0);

            if (!seenTiles.contains(up))
                searchFront.insert(up);
            if (!seenTiles.contains(down))
                searchFront.insert(down);
            if (!seenTiles.contains(left))
                searchFront.insert(left);
            if (!seenTiles.contains(right))
                searchFront.insert(right);
        }
    }
    return regionTiles;
}

bool Puzzle12::TileMatches(char type, Coord position, const std::vector<std::string>& board)
{
    if (position.x >= 0 && position.x < board[0].size() && position.y >= 0 && position.y < board.size())
    {
        return board[position.y][position.x] == type;
    }
    return false;
}


std::vector<std::string> Puzzle12::ParseBoard(const char* fileName)
{
    std::vector<std::string> board;
    std::ifstream myFile(fileName);
    int width = 0;
    int height = 0;
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
            board.push_back(line);
        }
    }

    return board;
}

int Puzzle12::Region::GetSides() const
{
    int sum = 0;
    for (const Coord& coord : tiles)
    {
        sum += GetVertexCount(coord);
    }
    return sum;
}

int Puzzle12::Region::GetVertexCount(Coord src) const
{
    int count = 0;

    Coord up = src + Coord(0, -1);
    Coord down = src + Coord(0, 1);
    Coord left = src + Coord(-1, 0);
    Coord right = src + Coord(1, 0);
    Coord upleft = src + Coord(-1, -1);
    Coord upright = src + Coord(1, -1);
    Coord downleft = src + Coord(-1, 1);
    Coord downright = src + Coord(1, 1);

    //inner corners
    if (tiles.contains(up) && tiles.contains(left) && !tiles.contains(upleft))
    {
        count++;
    }
    if (tiles.contains(up) && tiles.contains(right) && !tiles.contains(upright))
    {
        count++;
    }
    if (tiles.contains(down) && tiles.contains(left) && !tiles.contains(downleft))
    {
        count++;
    }
    if (tiles.contains(down) && tiles.contains(right) && !tiles.contains(downright))
    {
        count++;
    }

    //outer corners
    if (!tiles.contains(up) && !tiles.contains(left))
    {
        count++;
    }
    if (!tiles.contains(up) && !tiles.contains(right))
    {
        count++;
    }
    if (!tiles.contains(down) && !tiles.contains(left))
    {
        count++;
    }
    if (!tiles.contains(down) && !tiles.contains(right))
    {
        count++;
    }
    return count;
}

int Puzzle12::Region::GetPerimeter() const
{
    int perimeter = 0;
    for (const Coord& coord : tiles)
    {
        Coord up = coord + Coord(0, -1);
        Coord down = coord + Coord(0, 1);
        Coord left = coord + Coord(-1, 0);
        Coord right = coord + Coord(1, 0);
        if (!tiles.contains(up))
            perimeter++;
        if (!tiles.contains(down))
            perimeter++;
        if (!tiles.contains(left))
            perimeter++;
        if (!tiles.contains(right))
            perimeter++;
    }
    return perimeter;
}
