#include "Puzzle8.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

const char c_BlankTile = '.';

//https://adventofcode.com/2024/day/8
//we are given a 2d board of transmitters, the transmitters generate
//antinodes with other transmitters of the same type (they are labelled as chars)
//each antinode is positioned opposite the other transmitter, e.g.
//...a.a...
//would generate antinodes as such:
//.X.a.a.X.
//we need to count the number of antipoles within the boards dimensions. (which can stack.)
std::string Puzzle8::SolvePuzzleA(const char* fileName)
{
    PuzzleBoard board;
    board.ParseBoard(fileName);
    board.CalculateAntiPolesA();
#if DEBUG_OUTPUT
    board.PrintAntiPoles();
#endif
    return std::to_string(board.CountAntiPoles());
}

//https://adventofcode.com/2024/day/8#part2
//as part a, but antinodes are generated in lines, so for the example:
//...a.a...........
//antinodes would generate as such
//.X.a.a.X.X.X.X.X.
//again, antinodes stack.
std::string Puzzle8::SolvePuzzleB(const char* fileName)
{
    PuzzleBoard board;
    board.ParseBoard(fileName);
    board.CalculateAntiPolesB();
#if DEBUG_OUTPUT
    board.PrintAntiPoles();
#endif
    return std::to_string(board.CountAntiPoles());
}

bool Puzzle8::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle08/test.txt");
    std::string resultB = SolvePuzzleB("puzzle08/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "14") return false;
    if (resultB != "34") return false;
    return true;
}

void Puzzle8::PuzzleBoard::ParseBoard(const char* fileName)
{
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
                assert(width == line.size());
            }
            for (size_t x = 0; x < line.size(); x++)
            {
                if (line[x] != c_BlankTile)
                {
                    stations[line[x]].push_back(Coord(x,height));
                }
            }

            std::vector<bool> linepoles;
            linepoles.resize(line.size(), false);
            antipoles.push_back(linepoles);
            height++;
        }
    }
}

void Puzzle8::PuzzleBoard::CalculateAntiPolesA()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            antipoles[y][x] = false;
        }
    }

    for (const auto& iterator : stations)
    {
        const std::vector<Coord>& stationList = iterator.second;
        //stations grouped by char type
        for (size_t sourceStationIndex = 0; sourceStationIndex < stationList.size(); sourceStationIndex++)
        {
            for (size_t otherStationIndex = 0; otherStationIndex < stationList.size(); otherStationIndex++)
            {
                if (sourceStationIndex == otherStationIndex)
                    continue;

                Coord diff = stationList[sourceStationIndex] - stationList[otherStationIndex];
                Coord antiPole = stationList[sourceStationIndex] + diff;
                if (antiPole.x >= 0 && antiPole.x < width && antiPole.y >= 0 && antiPole.y < height)
                {
                    antipoles[antiPole.y][antiPole.x] = true;
                }
            }
        }
    }
}

void Puzzle8::PuzzleBoard::CalculateAntiPolesB()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            antipoles[y][x] = false;
        }
    }

    for (const auto& iterator : stations)
    {
        const std::vector<Coord>& stationList = iterator.second;
        //stations grouped by char type
        for (size_t sourceStationIndex = 0; sourceStationIndex < stationList.size(); sourceStationIndex++)
        {
            for (size_t otherStationIndex = 0; otherStationIndex < stationList.size(); otherStationIndex++)
            {
                if (sourceStationIndex == otherStationIndex)
                    continue;

                Coord diff = stationList[sourceStationIndex] - stationList[otherStationIndex];
                bool forwardDone = false;
                bool backwardDone = false;
                int iteration = 1;
                while (!forwardDone || !backwardDone)
                {
                    Coord forwardAntiPole = stationList[sourceStationIndex] + (diff * iteration);
                    if (!forwardDone && forwardAntiPole.x >= 0 && forwardAntiPole.x < width && forwardAntiPole.y >= 0 && forwardAntiPole.y < height)
                    {
                        antipoles[forwardAntiPole.y][forwardAntiPole.x] = true;
                    }
                    else
                    {
                        forwardDone = true;
                    }
                    Coord backwardAntiPole = stationList[sourceStationIndex] - (diff * iteration);
                    if (!backwardDone && backwardAntiPole.x >= 0 && backwardAntiPole.x < width && backwardAntiPole.y >= 0 && backwardAntiPole.y < height)
                    {
                        antipoles[backwardAntiPole.y][backwardAntiPole.x] = true;
                    }
                    else
                    {
                        backwardDone = true;
                    }

                    iteration++;
                }
            }
        }
    }
}

void Puzzle8::PuzzleBoard::PrintAntiPoles()
{
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < height; x++)
        {
            std::cout << (antipoles[y][x] ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

size_t Puzzle8::PuzzleBoard::CountAntiPoles() const
{
    int sum = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (antipoles[y][x])
                sum++;
        }
    }
    return sum;
}
