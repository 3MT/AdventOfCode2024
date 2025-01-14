#include "Puzzle20.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <assert.h>
#include "Coord.h"

char c_Start = 'S';
char c_End = 'E';
char c_Floor = '.';
char c_Wall = '#';

//https://adventofcode.com/2024/day/20
//we are supplied with a simple maze to path through
//it's a track to race through, but with a twist, we can cheat
//once per path we can noclip through 2 tiles, ignoring obstacles
//the goal of part a is to count how many cheats would save us 100 steps
std::string Puzzle20::SolvePuzzleA(const char* fileName)
{
    std::vector<std::vector<char>> map;    
    std::fstream file(fileName);
    size_t width = 0;
    size_t height = 0;
    Coord start;
    Coord goal;

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
                assert(width == line.size());
            }
            for (size_t x = 0; x < width; x++)
            {
                if (line[x] == c_Start)
                {
                    start = Coord(x, height);
                    line[x] = c_Floor;
                }
                if (line[x] == c_End)
                {
                    goal = Coord(x, height);
                    line[x] = c_Floor;
                }
            }

            height++;
            std::vector<char> charLine(line.begin(), line.end());
            map.push_back(charLine);
        }
    }

    //flood fill distance
    std::queue<Coord> front;
    std::map<Coord,int64_t> distances;
    front.push(start);
    distances[start] = 0;
    while (front.size() > 0)
    {
        Coord currentNode = front.front();
        front.pop();
        int64_t nextDist = distances[currentNode] + 1;

        for (int dir = 0; dir < 4; dir++)
        {
            Coord neighbour = currentNode + Coord::CardinalDirections[dir];
            if (neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < width && neighbour.y < height)
            {
                bool seen = distances.contains(neighbour);
                if (!seen || distances[neighbour] > nextDist)
                {
                    distances[neighbour] = nextDist;

                    if (map[neighbour.y][neighbour.x] == c_Floor)
                    {
                        front.push(neighbour);
                    }
                }
            }
        }
    }


    std::map<int64_t, int64_t> timeSaves;
    int64_t sub100Count = 0;
    for (int64_t x = 0; x < width; x++)
    {
        for (int64_t y = 0; y < height; y++)
        {
            if (map[y][x] == c_Wall)
            {
                Coord pos(x, y);
                for (int dir = 0; dir < 4; dir++)
                {
                    Coord neighbour = Coord(x,y) + Coord::CardinalDirections[dir];
                    if (neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < width && neighbour.y < height)
                    {
                        if (map[neighbour.y][neighbour.x] != c_Floor)
                            continue;
                    }
                    if (distances.contains(neighbour))
                    {
                        int64_t diff = distances[neighbour] - distances[pos] - 1; //-1 because we still have to count the regular move.
                        if (diff > 0)
                        {
                            //std::cout << "Time save: " << distances[neighbour] - distances[pos] << "\n";
                            if (diff >= 100)
                            {
                                sub100Count++;
                            }
                            timeSaves[diff]++;
                        }
                    }
                }
            }
        }
    }

    return std::to_string(sub100Count);
}

//https://adventofcode.com/2024/day/20#part2
//as part a, but now a cheat can noclip through up to 20 tiles in one go
std::string Puzzle20::SolvePuzzleB(const char* fileName)
{
    std::vector<std::vector<char>> map;
    std::fstream file(fileName);
    size_t width = 0;
    size_t height = 0;
    Coord start;
    Coord goal;

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
                assert(width == line.size());
            }
            for (size_t x = 0; x < width; x++)
            {
                if (line[x] == c_Start)
                {
                    start = Coord(x, height);
                    line[x] = c_Floor;
                }
                if (line[x] == c_End)
                {
                    goal = Coord(x, height);
                    line[x] = c_Floor;
                }
            }

            height++;
            std::vector<char> charLine(line.begin(), line.end());
            map.push_back(charLine);
        }
    }

    //flood fill distance
    std::queue<Coord> front;
    std::map<Coord, int64_t> distances;
    front.push(start);
    distances[start] = 0;
    while (front.size() > 0)
    {
        Coord currentNode = front.front();
        front.pop();
        int64_t nextDist = distances[currentNode] + 1;

        for (int64_t dir = 0; dir < 4; dir++)
        {
            Coord neighbour = currentNode + Coord::CardinalDirections[dir];
            if (neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < width && neighbour.y < height)
            {
                bool seen = distances.contains(neighbour);
                if (!seen || distances[neighbour] > nextDist)
                {
                    distances[neighbour] = nextDist;

                    if (map[neighbour.y][neighbour.x] == c_Floor)
                    {
                        front.push(neighbour);
                    }
                }
            }
        }
    }


    int64_t circleRadius = 21; //distance we can 'cheat' in a circle (manhattan distance)
    std::vector<Coord> circleOffsets;
    for (int x = -circleRadius; x <= circleRadius; x++)
    {
        for (int y = -circleRadius; y <= circleRadius; y++)
        {
            if (abs(x) + abs(y) < circleRadius)
            {
                circleOffsets.push_back(Coord(x, y));
            }
        }
    }

    //the logic looks scary but basically we are just finding all paths that 'jump' to a part of the track that's closer to the goal than here, and store the time saved
    std::map<int64_t, int64_t> timeSaves;
    int64_t sub100Count = 0;
    for (size_t x = 1; x < width - 1; x++)
    {
        for (size_t y = 1; y < height - 1; y++)
        {
            if (map[y][x] == c_Floor)
            {
                Coord pos(x, y);

                for (const Coord& offset : circleOffsets)
                {
                    Coord neighbour = Coord(x, y) + offset;
                    if (neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < width && neighbour.y < height)
                    {
                        if (map[neighbour.y][neighbour.x] != c_Floor)
                            continue;
                    }
                    if (distances.contains(neighbour) && distances.contains(pos))
                    {
                        int64_t manhattanDistance = abs(offset.x) + abs(offset.y);
                        int64_t diff = distances[neighbour] - distances[pos] - manhattanDistance;
                        if (diff > 0)
                        {
                            //std::cout << "Time save: " << distances[neighbour] - distances[pos] << "\n";
                            if (diff >= 100)
                            {
                                sub100Count++;
                            }
                            timeSaves[diff]++;
                        }
                    }
                }
            }
        }
    }

    return std::to_string(sub100Count);
}

bool Puzzle20::RunTests()
{
    //no tests for this one, was easier to just work with the final maze.
    return true;
}
