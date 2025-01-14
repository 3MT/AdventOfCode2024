#include "Puzzle2.h"

#include <assert.h>
#include <fstream>
#include <iostream>

int64_t c_MinChange = 1;
int64_t c_MaxChange = 3;

// Puzzle 2 https://adventofcode.com/2024/day/2
//for puzzle A, each line contains a list of positive integers, a list is 'safe' if it continually increases or decreases by between 1 to 3.
//we count number of safe lists and output that.
std::string Puzzle2::SolvePuzzleA(const char* fileName)
{
    std::vector<std::vector<int64_t>> lines = ParseFile(fileName);
    int64_t safeSum = 0;
    for (const std::vector<int64_t>& line : lines)
    {
        if (ValidIncrements(line, c_MinChange, c_MaxChange))
        {
            safeSum++;
        }
        else if (ValidDecrements(line, c_MinChange, c_MaxChange))
        {
            safeSum++;
        }
    }
    return std::to_string(safeSum);
}

//https://adventofcode.com/2024/day/2#part2
//puzzle b is the same as puzzle 1, but one 'incorrect' value is allowed, a value that doesn't match the increment/decrement pattern is 'incorrect', any value can be incorrect and must be inferred from the entire list
std::string  Puzzle2::SolvePuzzleB(const char* fileName)
{
    std::vector<std::vector<int64_t>> lines = ParseFile(fileName);
    int64_t safeSum = 0;
    for (std::vector<int64_t>& line : lines)
    {
        if (line.size() <= 1) //check for unusual but valid cases to simplify iterator code below.
        {
            safeSum++;
            continue;
        }
        if (ValidIncrementsB(line.begin(), line.end(), c_MinChange, c_MaxChange))
        {
            safeSum++;
        }
        else if (ValidDecrementsB(line.begin(), line.end(), c_MinChange, c_MaxChange))
        {
            safeSum++;
        }
        else
        {
            //this solution is kinda cursed but it was the only thing i could come up with that doesn't brute force the solution by simply trying every possible list that is missing one element.
            //the validincrementsB() function mostly works but isn't robust to the first element being the incorrect element (false negative), reversing the vector is a dirty but effective option to fix this issue.
            //I'm sure there is a better solution than checking the list forward and backwards, but I'm a bit sick of this question (this is my 4th(?) attempted solution to part B...)
            //and this is at least better than making array.size() copies of the array and checking all of them.

            //update: i looked into other solutions cause this question really put me through the wringer compared to most other questions and literally every other solution I found just did the brute force array copy method in one way or another lol
            //for our case I guess best case perf is you find it first try so you iterate once, worse case you iterate 2x2 times (not gonna count std::reverse as an iteration cause I could optimise it out but I'm being lazy lol)
            //for brute force best case they find it first try, worst case they iterate N arrays (twice? you might be able to combine the dec/inc checks into one iteration I guess)
            //given the input this means their worst case is N = 8x2 so I guess I'm winning even on the test data
            //thinking about it the puzzle setters chose n = 8 as the max so they likely expected most people to brute force I guess? (going higher would make brute forcing prohibitive i think)
            //so I guess I'll give myself a gold star for at least trying to avoid it lol

            std::reverse(line.begin(), line.end()); //could use reverse iterators to avoid this iteration but I can't do it without making another set of validincrementsB() functions so I'm gonna take the easy road today.
            if (ValidIncrementsB(line.begin(), line.end(), c_MinChange, c_MaxChange))
            {
                safeSum++;
            }
            else if (ValidDecrementsB(line.begin(), line.end(), c_MinChange, c_MaxChange))
            {
                safeSum++;
            }
            std::reverse(line.begin(), line.end()); //unreverse the elements
        }

    }
    return std::to_string(safeSum);
}

bool Puzzle2::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle02/test.txt");
    std::string resultB = SolvePuzzleB("puzzle02/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "2") return false;
    if (resultB != "4") return false;
    return true;
}

//iterate list and return true if all elements are increasing by a value between min and max
bool Puzzle2::ValidIncrements(const std::vector<int64_t>& input, int64_t min, int64_t max)
{
    int64_t lastNum = input[0];

    for (size_t i = 1; i < input.size(); i++)
    {
        int64_t diff = input[i] - lastNum;
        if (diff >= min && diff <= max)
        {
            //valid.
        }
        else
        {
            return false;
        }
        lastNum = input[i];
    }
    return true;
}

//iterate list and return true if all elements are decreasing by a value between min and max
bool Puzzle2::ValidDecrements(const std::vector<int64_t>& input, int64_t min, int64_t max)
{
    int64_t lastNum = input[0];

    for (size_t i = 1; i < input.size(); i++)
    {
        int64_t diff = lastNum - input[i];
        if (diff >= min && diff <= max)
        {
            //valid.
        }
        else
        {
            return false;
        }
        lastNum = input[i];
    }
    return true;
}

//iterate list and return true if all elements are increasing by a value between min and max, if it encounters a value that doesn't fit this pattern it will ignore one instance and continue
bool Puzzle2::ValidIncrementsB(std::vector<int64_t>::iterator begin, std::vector<int64_t>::iterator end, int64_t min, int64_t max)
{
    int64_t lastNum = *begin;
    begin++;

    bool foundError = false;
    for (; begin != end; ++begin) 
    {
        int64_t diff = *begin - lastNum;

        if (diff >= min && diff <= max)
        {
            //valid.
        }
        else
        {
            if (!foundError)
            {
                foundError = true;
                continue;
            }
            return false;
        }
        lastNum = *begin;
    }
    return true;
}

//iterate list and return true if all elements are decreasing by a value between min and max, if it encounters a value that doesn't fit this pattern it will ignore one instance and continue
bool Puzzle2::ValidDecrementsB(std::vector<int64_t>::iterator begin, std::vector<int64_t>::iterator end, int64_t min, int64_t max)
{
    int64_t lastNum = *begin;
    begin++;

    bool foundError = false;
    for (; begin != end; ++begin)
    {
        int64_t diff = lastNum - *begin;

        if (diff >= min && diff <= max)
        {
            //valid.
        }
        else
        {
            if (!foundError)
            {
                foundError = true;
                continue;
            }
            return false;
        }
        lastNum = *begin;
    }
    return true;
}


std::vector<std::vector<int64_t>> Puzzle2::ParseFile(const char* fileName)
{
    //format: x y z
    //list of positive space separated integers which form a list of 'temperatures'
    std::vector<std::vector<int64_t>> output;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            std::vector<int64_t> lineOutput;
            while (line.size() > 0)
            {
                try
                {
                    size_t firstSpace = line.find_first_of(' ');
                    int64_t value = 0;
                    value = std::stoll(line.substr(0, firstSpace));
                    lineOutput.push_back(value);

                    if (firstSpace == std::string::npos) //we have validated the last number (no more values)
                    {
                        break;
                    }
                    //cull the part we parsed from the string
                    line = line.substr(firstSpace + 1);
                }
                catch (...)
                {
                    //parse failed.
                    std::cout << "Parse Failed";
                    break;
                }
            }
            output.push_back(lineOutput);
        }
        myFile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    return output;
}
