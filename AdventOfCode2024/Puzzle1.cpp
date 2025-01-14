#include "Puzzle1.h"
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

//Puzzle 1 Part 1
// https://adventofcode.com/2024/day/1
//Input: 2 lists of ints
//Output: the sum of the differences of each value in each list, starting with the lowest value (ie sorted small->large)

std::string Puzzle1::SolvePuzzleA(const char* fileName)
{
    std::vector<int64_t> leftValues;
    std::vector<int64_t> rightValues;

    //load file and populate vectors;
    ParseArrays(fileName, leftValues, rightValues);

    //sort both arrays so we are comparing smallest to smallest etc.
    std::sort(leftValues.begin(), leftValues.end());
    std::sort(rightValues.begin(), rightValues.end());

    //sum differences
    int64_t finalSum = 0;
    for (size_t i = 0; i < leftValues.size(); i++)
    {
        int64_t diff = leftValues[i] - rightValues[i];
        finalSum += abs(diff);
    }

    return std::to_string(finalSum);
}

//part 2 is to take the same input and use the left inputs as a key for the right inputs.
// https://adventofcode.com/2024/day/1#part2
//each left input receives a score, left value * number of times that number appears in the right input.
//then all values are summed.
std::string Puzzle1::SolvePuzzleB(const char* fileName)
{
    std::vector<int64_t> leftValues;
    std::vector<int64_t> rightValues;

    //load file and populate vectors;
    ParseArrays(fileName, leftValues, rightValues);

    int64_t sum = 0;
    std::sort(leftValues.begin(), leftValues.end());
    std::sort(rightValues.begin(), rightValues.end());

    while (leftValues.size() > 0)
    {
        //since each instance on the left is scored separately we can treat the formula as n * (left n count) * (right n count).
        int64_t value = leftValues.back();
        int64_t leftCount = 0;
        int64_t rightCount = 0;
        //accumulate all the end values
        while (leftValues.size() > 0 && leftValues.back() == value)
        {
            leftValues.pop_back();
            leftCount++;
        }
        //pop off until we reach elements that match our left value, then accumulate those.
        //any value greater than left value must necessarily not appear in our left column at all.
        while (rightValues.size() > 0 && rightValues.back() >= value)
        {
            if (rightValues.back() == value)
                rightCount++;
            rightValues.pop_back();
        }
        sum += value * leftCount * rightCount;
    }
    return std::to_string(sum);
}

bool Puzzle1::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle01/test.txt");
    std::string resultB = SolvePuzzleB("puzzle01/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "11") return false;
    if (resultB != "31") return false;
    return true;
}


void Puzzle1::ParseArrays(const char* fileName, std::vector<int64_t>& outLeftValues, std::vector<int64_t>& outRightValues)
{
    //format is x   y
    //where x and y are positive integers. 
    std::string line;
    std::ifstream myfile(fileName);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            int64_t leftInput, rightInput;
            if (ParseLine(line, leftInput, rightInput))
            {
                outLeftValues.push_back(leftInput);
                outRightValues.push_back(rightInput);
            }
            else
            {
                std::cout << "Bad parse on input: " << line;
            }
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
}

bool Puzzle1::ParseLine(const std::string& inputString, int64_t& out1, int64_t& out2)
{
    try
    {
        size_t firstSpace = inputString.find_first_of(' ');
        size_t lastSpace = inputString.find_last_of(' ');
        out1 = std::stoll(inputString.substr(0, firstSpace));
        out2 = std::stoll(inputString.substr(lastSpace, inputString.length()));
    }
    catch (...)
    {
        //parse failed, eof or bad input.
        out1 = 0;
        out2 = 0;
        return false;
    }
    return true;
}
