#include "Puzzle6.h"
#include <assert.h>
#include <fstream>
#include <iostream>

const char c_GuardStart = '^';
const char c_Wall = '#';
const char c_Floor = '.';

//https://adventofcode.com/2024/day/6
//input is a grid with a guard marked, the guard starts moving upwards initially. when the guard hits a wall she turns right
//the goal is to count the unique number of tiles visited by the guard before she leaves the marked grid
std::string Puzzle6::SolvePuzzleA(const char* fileName)
{
    GuardedRoom room;
    room.ParseGrid(fileName);
    while (room.CanTick())
    {
        room.TickGrid();
    }
    return std::to_string(room.SumVisitedTiles());
}

//https://adventofcode.com/2024/day/6#part2
//the same rules as part a but the goal is to get the guard stuck in a loop forever
//we need to find each tile we could turn into an obstacle cause the guard to get stuck in a loop.
std::string Puzzle6::SolvePuzzleB(const char* fileName)
{
    GuardedRoom room;
    room.ParseGrid(fileName);
    int64_t loopPositions = 0;
    int64_t count = 0;
    for (int64_t x = 0; x < room.GetWidth(); x++)
    {
        for (int64_t y = 0; y < room.GetHeight(); y++)
        {
            count += 1;
            if (room.ValidObstaclePos(x,y))
            {
                //this is kind of brute forcey, I can think of one culling step we can do which is to take the results from part a and only try tiles that the guard touched.
                //however that only culls about 30-50% of tiles, I'm not sure if there's something I'm missing here but I think you have to iterate it to figure out if you're stuck in a loop so it's gonna take time no matter what
                //not sure if it's actually a halting problem, maybe there's something clever you can do?

                //update: looked into other solutions and didn't really see anything better than brute force using the culling method I mentioned before
                //at best people optimised by multithreading which is a good idea as this is trivially threadable but you're still brute forcing, it's not that clever.
                //some people used floyd's tortoise and hare algorithm to detect cycles which probably saves some memory over what I'm doing here and I didn't know about it beforehand, so I guess that's a funfact.
                //in release mode my code spits out an answer in ~1s which, looking at other answers, is pretty solid for linear execution.

                room.SetTile(x, y, GuardedRoom::TileType::BLOCKED);
                room.Reset();
                while (room.CanTick())
                {
                    room.TickGrid();
                }
                if (room.GuardStuck())
                {
                    loopPositions++;
                    room.Reset();
                }
                room.SetTile(x, y, GuardedRoom::TileType::CLEAR);
            }
            else
            {
                continue;
            }
        }
    }

    //result is counting the number of positions that generate a loop.
    return std::to_string(loopPositions);
}

bool Puzzle6::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle06/test.txt");
    std::string resultB = SolvePuzzleB("puzzle06/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "41") return false;
    if (resultB != "6") return false;
    return true;
}

void Puzzle6::GuardedRoom::TickGrid()
{
    bool aheadClear = false;
    int64_t newX = -1;
    int64_t newY = -1;
    while (!aheadClear)
    {
        newX = guardX;
        newY = guardY;
        switch (guardDirection)
        {
        case Direction::UP:
            newY -= 1;
            break;
        case Direction::DOWN:
            newY += 1;
            break;
        case Direction::LEFT:
            newX -= 1;
            break;
        case Direction::RIGHT:
            newX += 1;
            break;
        }
        if (newX < 0 || newX >= width || newY < 0 || newY >= height)
        {
            aheadClear = true; //leaving map
        }
        else
        {
            if (tiles[newY][newX] != TileType::BLOCKED)
            {
                aheadClear = true;
            }
            else
            {
                //rotate 90 deg
                switch (guardDirection)
                {
                case Direction::UP:
                    guardDirection = RIGHT;
                    break;
                case Direction::DOWN:
                    guardDirection = LEFT;
                    break;
                case Direction::LEFT:
                    guardDirection = UP;
                    break;
                case Direction::RIGHT:
                    guardDirection = DOWN;
                    break;
                }
            }
        }
    }
    guardX = newX;
    guardY = newY;

    if (newX < 0 || newX >= width || newY < 0 || newY >= height)
    {
        guardOnMap = false;
    }
    else
    {
        if ((visitedTiles[guardY][guardX] & guardDirection) != 0) //check for loop
            guardInLoop = true;

        visitedTiles[guardY][guardX] = (Direction)(visitedTiles[guardY][guardX] | guardDirection);
    }
}

void Puzzle6::GuardedRoom::ParseGrid(const char* fileName)
{
    //input format is a rect filled with . # and ^, . is blank, # is obstacle and ^ is the guards initial position, moving upwards.
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
            std::vector<TileType> tileLine;
            for (size_t i = 0; i < line.size(); i++)
            {
                switch (line[i])
                {
                    case c_Floor:
                    {
                        //clear pos
                        tileLine.push_back(TileType::CLEAR);
                        break;
                    }
                    case c_GuardStart:
                    {
                        //guard pos
                        tileLine.push_back(TileType::CLEAR);
                        initialGuardX = i;
                        initialGuardY = tiles.size();
                        guardOnMap = true;
                        break;
                    }
                    case c_Wall:
                    {
                        tileLine.push_back(TileType::BLOCKED);
                        break;
                    }
                    default:
                    {
                        assert(false); //bad input
                        break;
                    }
                }
            }
            tiles.push_back(tileLine);
        }
    }

    Reset();
}

int64_t Puzzle6::GuardedRoom::SumVisitedTiles() const
{
    int64_t sum = 0;
    for (const std::vector<Direction>& line : visitedTiles)
    {
        for (Direction tile : line)
        {
            sum += (tile == Direction::NONE) ? 0 : 1;
        }
    }
    return sum;
}

void Puzzle6::GuardedRoom::Reset()
{
    visitedTiles.clear();
    visitedTiles.reserve(height);
    for (int64_t y = 0; y < height; y++)
    {
        std::vector<Direction> newTiles;
        newTiles.reserve(width);
        for (int64_t x = 0; x < width; x++)
        {
            newTiles.push_back(Direction::NONE);
        }
        visitedTiles.push_back(newTiles);
    }
    guardX = initialGuardX;
    guardY = initialGuardY;
    guardDirection = UP;
    guardOnMap = true;
    guardInLoop = false;
}

//debug visualisation
void Puzzle6::GuardedRoom::PrintGrid(int64_t highlightX, int64_t highlightY) const
{
    int64_t x = 0;
    int64_t y = 0;
    for (const std::vector<TileType>& line : tiles)
    {
        x = 0;
        for (TileType tile : line)
        {
            if (highlightX == x && highlightY == y)
            {
                std::cout << "O";
            }
            else if (x == guardX && y == guardY)
            {
                switch (guardDirection)
                {
                case Direction::UP:
                    std::cout << "^";
                    break;
                case Direction::DOWN:
                    std::cout << "v";
                    break;
                case Direction::LEFT:
                    std::cout << "<";
                    break;
                case Direction::RIGHT:
                    std::cout << ">";
                    break;
                }
            }
            else
            {
                switch (tile)
                {
                case TileType::BLOCKED:
                    std::cout << "#";
                    break;
                case TileType::CLEAR:
                    std::cout << ".";
                    break;
                case TileType::VISITED:
                    std::cout << "X";
                    break;
                }
            }
            x++;
        }
        std::cout << "\n";
        y++;
    }
    std::cout << "\n";
}

