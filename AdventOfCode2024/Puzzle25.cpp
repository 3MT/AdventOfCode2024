#include "Puzzle25.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

//https://adventofcode.com/2024/day/25
//we are given a list of keys and locks in the format:
//#####
//.####
//.####
//.####
//.#.#.
//.#...
//.....
//
//.....
//#....
//#....
//#...#
//#.#.#
//#.###
//#####
//hashes represent positions the key/lock exists and . represents empty space.
//the definition of a key fitting a lock is if no position in the key overlaps with the lock
//keys are indicated by the bottom row being entirely #s, locks are the oppposite.
//our task is to count how many unique pairs of keys and locks fit together.
std::string Puzzle25::SolvePuzzleA(const char* fileName)
{
    std::fstream myFile(fileName);
    std::vector<int64_t> keys;
    std::vector<int64_t> locks;
    if (myFile.is_open())
    {
        int64_t subCount = 0;
        int64_t currentParse = 0;
        bool parsingKey = false;
        bool parsingLock = false;
        std::string line;
        while (getline(myFile, line))
        {
            if (line.size() > 0)
            {
                if (!parsingKey && !parsingLock)
                {
                    if (line._Equal("#####"))
                    {
                        parsingLock = true;
                        subCount = 0;
                        currentParse = 0;
                    }
                    if (line._Equal("....."))
                    {
                        parsingKey = true;
                        subCount = 0;
                        currentParse = 0;
                    }
                    continue;
                }
                if ((parsingKey || parsingLock) && subCount < 5)
                {
                    for (int64_t i = 0; i < 5; i++)
                    {
                        if (line[i] == '#')
                        {
                            currentParse += 1ll << (subCount * 5 + i);
                        }
                    }
                }
                if (subCount == 5)
                {
                    if (parsingKey)
                        keys.push_back(currentParse);
                    else
                        locks.push_back(currentParse);
                }
                subCount++;
            }
            else
            {
                parsingKey = false;
                parsingLock = false;
            }
        }
    }

    int64_t matchingSum = 0;
    for (size_t i = 0; i < keys.size(); i++)
    {
        for (size_t j = 0; j < locks.size(); j++)
        {
            if ((keys[i] & locks[j]) == 0ll) //binary and to check for overlaps.
            {
                matchingSum++;
            }
        }
    }

    return std::to_string(matchingSum);
}

//There is no part 2! We did it!
std::string Puzzle25::SolvePuzzleB(const char* fileName)
{
    return "Merry Christmas!";
}

bool Puzzle25::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle25/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "3") return false;
    return true;
}
