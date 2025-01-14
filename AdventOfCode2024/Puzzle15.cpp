#include "Puzzle15.h"

#include <assert.h>
#include <fstream>>
#include <iostream>
#include <sstream>
#include <set>

const char c_WALL = '#';
const char c_CRATE = 'O';
const char c_BIGCRATEL = '[';
const char c_BIGCRATER = ']';
const char c_ROBOT = '@';
const char c_BLANK = '.';

const char c_LEFT =  '<';
const char c_RIGHT = '>';
const char c_UP =    '^';
const char c_DOWN =  'v';

//https://adventofcode.com/2024/day/15
//we are given a setup for a sokoban-like setting
//a robot pushes crates around (see above const chars for the types of tiles)
//we are also given a series of inputs in arrow form (<,v,>,^)
//we are to have the robot perform the set of rules under sokoban rules (the robot CAN push multiple boxes at once however)
//then calculate the 'gps' coordinates of each crate once the robot is done.
//the gps coords are calculated by it's y position multiplied by 100 plus it's x position.
//we are to return the sum of all the crates gps coords.
std::string Puzzle15::SolvePuzzleA(const char* fileName)
{
    std::ifstream myFile(fileName);
    std::vector<std::vector<char>> tiles = ParseGrid(myFile);
    std::string commands = ParseRobotCommands(myFile);
    Coord robotPosition;
    for (size_t x = 0; x < tiles[0].size(); x++)
    {
        for (size_t y = 0; y < tiles.size(); y++)
        {
            if (tiles[y][x] == c_ROBOT)
            {
                robotPosition.x = x;
                robotPosition.y = y;
                tiles[y][x] = c_BLANK;
            }
        }
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        switch (commands[i])
        {
            case c_LEFT:
            {
                Coord left(-1, 0);
                if (TryRobotMove(robotPosition, left, tiles))
                {
                    robotPosition = robotPosition + left;
                }
                break;
            }
            case c_RIGHT:
            {
                Coord right(1, 0);
                if (TryRobotMove(robotPosition, right, tiles))
                {
                    robotPosition = robotPosition + right;
                }
                break;
            }
            case c_DOWN:
            {
                Coord down(0, 1);
                if (TryRobotMove(robotPosition, down, tiles))
                {
                    robotPosition = robotPosition + down;
                }
                break;
            }
            case c_UP:
            {
                Coord up(0, -1);
                if (TryRobotMove(robotPosition, up, tiles))
                {
                    robotPosition = robotPosition + up;
                }
                break;
            }
            default:
            {
                //no op
                break;
            }
        }
    }

    return std::to_string(SumGPS(tiles));
}

//https://adventofcode.com/2024/day/15#part2
//As part 1, however we need to double the width of the map, so each # is actually ##
//the crates are also larger, they are 2 tiles across now, which means 1 crate can push 2 crates and this can continue indefinitely.
//given this new setup we need to calculate the final gps coordinates for each crate again and sum them for our result.
std::string Puzzle15::SolvePuzzleB(const char* fileName)
{
    std::ifstream myFile(fileName);
    std::vector<std::vector<char>> tiles = ParseWideGrid(myFile);
    std::string commands = ParseRobotCommands(myFile);
    Coord robotPosition;
    for (size_t x = 0; x < tiles[0].size(); x++)
    {
        for (size_t y = 0; y < tiles.size(); y++)
        {
            if (tiles[y][x] == c_ROBOT)
            {
                robotPosition.x = x;
                robotPosition.y = y;
                tiles[y][x] = c_BLANK;
            }
        }
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        switch (commands[i])
        {
        case c_LEFT:
        {
            Coord left(-1, 0);
            if (TryWideRobotMove(robotPosition, left, tiles))
            {
                robotPosition = robotPosition + left;
            }
            break;
        }
        case c_RIGHT:
        {
            Coord right(1, 0);
            if (TryWideRobotMove(robotPosition, right, tiles))
            {
                robotPosition = robotPosition + right;
            }
            break;
        }
        case c_DOWN:
        {
            Coord down(0, 1);
            if (TryWideRobotMove(robotPosition, down, tiles))
            {
                robotPosition = robotPosition + down;
            }
            break;
        }
        case c_UP:
        {
            Coord up(0, -1);
            if (TryWideRobotMove(robotPosition, up, tiles))
            {
                robotPosition = robotPosition + up;
            }
            break;
        }
        default:
        {
            //no op
            break;
        }
        }

    }
#if DEBUG_OUTPUT
    {
        //debug print
        std::stringstream buffer;
        buffer << '\n';
        for (int y = 0; y < tiles.size(); y++)
        {
            for (int x = 0; x < tiles[0].size(); x++)
            {
                if (robotPosition == Coord(x, y))
                    buffer << '@';
                else
                    buffer << tiles[y][x];
            }
            buffer << '\n';
        }
        system("cls"); //clear console
        std::cout << buffer.str();
    }
#endif

    return std::to_string(SumGPS(tiles));
}

bool Puzzle15::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle15/test.txt");
    std::string resultB = SolvePuzzleB("puzzle15/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "10092") return false;
    if (resultB != "9021") return false;
    return true;
}

bool Puzzle15::TryRobotMove(Coord start, Coord direction, std::vector<std::vector<char>>& map)
{
    Coord move = start + direction;
    if (move.x < 0 || move.y < 0 || move.x >= map[0].size() || move.y >= map.size())
    {
        return false;
    }

    Coord startCrate = move;
    Coord endCrate;
    bool didPush = false;

    while (map[move.y][move.x] == c_CRATE)
    {
        move = move + direction;
        endCrate = move;
        didPush = true;
    }

    if (map[move.y][move.x] == c_BLANK)
    {
        if (didPush)
        {
            //push the crate stack forward.
            map[startCrate.y][startCrate.x] = c_BLANK;
            map[endCrate.y][endCrate.x] = c_CRATE;
        }
        return true;
    }

    if (map[move.y][move.x] == c_WALL)
        return false;
}

bool Puzzle15::TryWideRobotMove(Coord start, Coord direction, std::vector<std::vector<char>>& map)
{
    Coord move = start + direction;
    if (move.x < 0 || move.y < 0 || move.x >= map[0].size() || move.y >= map.size())
    {
        return false;
    }

    char moveTile = map[move.y][move.x];

    if (moveTile == c_BIGCRATEL || moveTile == c_BIGCRATER)
    {
        std::set<Coord> pushedCrates;
        std::set<Coord> front;
        front.insert(move);
        if (moveTile == c_BIGCRATEL)
        {
            front.insert(move + Coord(1, 0));
        }
        else
        {
            front.insert(move + Coord(-1, 0));
        }

        bool canPush = true;
        while (front.size() > 0)
        {
            Coord currentCratePart = *front.begin();
            front.erase(currentCratePart);
            if (pushedCrates.contains(currentCratePart))
            {
                continue;
            }
            else
            {
                Coord nextCrate = currentCratePart + direction;
                char nextTile = map[nextCrate.y][nextCrate.x];
                if (nextTile == c_WALL)
                {
                    //one of our crates is blocked call the whole thing off.
                    canPush = false;
                    break;
                }
                else if (nextTile == c_BIGCRATEL)
                {
                    front.insert(nextCrate + Coord(1, 0));
                    front.insert(nextCrate);
                }
                else if (nextTile == c_BIGCRATER)
                {
                    front.insert(nextCrate + Coord(-1, 0));
                    front.insert(nextCrate);
                }
                pushedCrates.insert(currentCratePart);
            }
        }
        if (canPush)
        {
            //kinda dirty double buffer here, could probably do it in place in a more efficient manner if this was a bottleneck but I don't think it will be.
            std::vector<std::vector<char>> newMap = map;
            for (const Coord& crate : pushedCrates)
            {
                newMap[crate.y][crate.x] = c_BLANK;
            }
            for (const Coord& crate : pushedCrates)
            {
                newMap[crate.y + direction.y][crate.x + direction.x] = map[crate.y][crate.x];
            }
            map = newMap;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    if (moveTile == c_BLANK)
    {
        return true;
    }

    if (moveTile == c_WALL)
        return false;
}


int64_t Puzzle15::SumGPS(const std::vector<std::vector<char>>& map)
{
    int64_t sum = 0;
    for (size_t x = 0; x < map[0].size(); x++)
    {
        for (size_t y = 0; y < map.size(); y++)
        {
            if (map[y][x] == c_CRATE)
            {
                sum += (y * 100) + x;
            }
            if (map[y][x] == c_BIGCRATEL)
            {
                sum += (y * 100) + x;
            }
        }
    }
    return sum;
}

std::vector<std::vector<char>> Puzzle15::ParseGrid(std::ifstream& file)
{
    std::vector<std::vector<char>> tiles;
    size_t width = 0;
    size_t height = 0;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            if (width == 0)
            {
                width = line.size();
            }
            else
            {
                if (width != line.size())
                {
                    //done parsing grid
                    return tiles;
                }
            }

            height++;
            std::vector<char> charLine(line.begin(), line.end());
            tiles.push_back(charLine);
        }
    }
}

std::vector<std::vector<char>> Puzzle15::ParseWideGrid(std::ifstream& file)
{
    std::vector<std::vector<char>> tiles;
    int width = 0;
    int height = 0;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            if (width == 0)
            {
                width = line.size();
            }
            else
            {
                if (width != line.size())
                {
                    //done parsing grid
                    return tiles;
                }
            }

            height++;
            std::vector<char> charLine;
            for (char character : line)
            {
                switch (character)
                {
                case c_CRATE:
                    charLine.push_back(c_BIGCRATEL);
                    charLine.push_back(c_BIGCRATER);
                    break;
                case c_BLANK:
                    charLine.push_back(c_BLANK);
                    charLine.push_back(c_BLANK);
                    break;
                case c_ROBOT:
                    charLine.push_back(c_ROBOT);
                    charLine.push_back(c_BLANK);
                    break;
                case c_WALL:
                    charLine.push_back(c_WALL);
                    charLine.push_back(c_WALL);
                    break;
                }
            }
            tiles.push_back(charLine);
        }
    }
}

std::string Puzzle15::ParseRobotCommands(std::ifstream& file)
{
    std::string output;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            output.append(line);
        }
    }
    return output;
}