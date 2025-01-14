#include "Puzzle9.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

//https://adventofcode.com/2024/day/9
//given an input of integers, we are to generate a hard drive filled with data.
//the input is alternating between the length of a file and the length of empty space to the next file, e.g.
//13242 means there is a 1 byte file, 3 empty bytes, 2 byte file, 4 empty bytes and a 2 byte file (0...11....22)
//files are also indexed from left to right (this is inferred by their position)
//the goal of puzzle A is to pack the data by filling each leftmost empty byte with the rightmost file byte.
//so the previous example 13242 expands to 0...11....22 and would compress to 02211.......
std::string Puzzle9::SolvePuzzleA(const char* fileName)
{
    std::vector<int64_t> unpackedData = ParseFile(fileName);

#ifdef DEBUG_OUTPUT
    VisualiseData(unpackedData);
#endif
    std::vector<int64_t> packedData = PackData(unpackedData);
#ifdef DEBUG_OUTPUT
    VisualiseData(packedData);
#endif
    return std::to_string(CalculateChecksum(packedData));
}

//https://adventofcode.com/2024/day/9#part2
//part b is as part a but when packing we only want to move entire files.
//each file only attempts to move once.
std::string Puzzle9::SolvePuzzleB(const char* fileName)
{
    std::vector<int64_t> unpackedData = ParseFile(fileName);

#ifdef DEBUG_OUTPUT
    VisualiseData(unpackedData);
#endif
    std::vector<int64_t> packedData = NoFragmentPacking(unpackedData);
#ifdef DEBUG_OUTPUT
    VisualiseData(packedData);
#endif

    return std::to_string(CalculateChecksum(packedData));
}

bool Puzzle9::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle09/test.txt");
    std::string resultB = SolvePuzzleB("puzzle09/test.txt");

    //had a hard to track down bug that was due to the final copy being off by one
    //they were sourced by using inputs other people were stuck on/posted online as good tests
    //we actually passed 3/4 initially, test4 revealed the edge case where when we were packing
    //and we reached the last number we can move and it happened to move exactly next to itself
    //the algo would fail, adjusting some limits in the NoFragmentPacking func fixed this.
    std::string resultB2 = SolvePuzzleB("puzzle09/test2.txt");
    std::string resultB3 = SolvePuzzleB("puzzle09/test3.txt");
    std::string resultB4 = SolvePuzzleB("puzzle09/test4.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
    std::cout << "B2: " << resultB2 << "\n";
    std::cout << "B3: " << resultB3 << "\n";
    std::cout << "B4: " << resultB4 << "\n";
#endif
    if (resultA != "1928") return false;
    if (resultB != "2858") return false;
    if (resultB2 != "169") return false;
    if (resultB3 != "5768") return false;
    if (resultB4 != "2184") return false;
    return true;
}

std::vector<int64_t> Puzzle9::ParseFile(const char* fileName)
{
    std::ifstream myFile(fileName);
    std::vector<int64_t> unpackedData; //each entry is a "block", integer is the ID of the "file" in that blocks, -1 is empty
    bool isEmptyData = false;
    int64_t fileId = 0;
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            //only expect 1 line of input for this question
            for (char digit : line)
            {
                int64_t count = (int64_t)(digit - '0');
                assert(count >= 0 && count < 10);
                for (int64_t i = 0; i < count; i++)
                {
                    unpackedData.push_back(isEmptyData ? -1 : fileId);
                }
                if (!isEmptyData)
                    fileId++;
                isEmptyData = !isEmptyData;
            }
        }
        return unpackedData;
    }
}

void Puzzle9::VisualiseData(const std::vector<int64_t>& input)
{
    //debugvis
    std::cout << "\n";
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] == -1)
            std::cout << ".";
        else
            std::cout << input[i] % 10; //mod 10 so they are correctly visually szied for larger values
    }
    std::cout << "\n";
}

std::vector<int64_t> Puzzle9::PackData(std::vector<int64_t>& input)
{
    std::vector<int64_t> output;
    int64_t forwardIterator = 0;
    int64_t backwardIterator = input.size() - 1;
    int64_t padding = 0;
    while (forwardIterator <= backwardIterator)
    {
        if (input[forwardIterator] != -1)
        {
            output.push_back(input[forwardIterator]);
            forwardIterator++;
        }
        else
        {
            if (input[backwardIterator] != -1)
            {
                output.push_back(input[backwardIterator]);
                forwardIterator++;
                backwardIterator--;
            }
            else
            {
                backwardIterator--;
            }
            padding++;
        }
    }
    for (int64_t i = 0; i < padding; i++)
    {
        output.push_back(-1); //leftover free blocks
    }
    return output;
}

//counts the length of the file starting at startindex
//file ends when the file id changes or we hit end index
int64_t Puzzle9::CountFileLength(const std::vector<int64_t>& input, int64_t startIndex, int64_t endIndex, int64_t fileID)
{
    int64_t count = 0;
    while (input[startIndex + count] == fileID)
    {
        count++;
        if (startIndex + count >= endIndex)
            break;
    }
    return count;
}

//similar to CountFileLength() but reversed
int64_t Puzzle9::CountFileLengthReverse(const std::vector<int64_t>& input, int64_t startIndex, int64_t fileID)
{
    int64_t count = 0;
    while (input[startIndex - count] == fileID)
    {
        count++;
        if ((startIndex - count) < 0)
            break;
    }
    return count;
}

//packing under part B ruleset (no fragmenting files allowed)s
std::vector<int64_t> Puzzle9::NoFragmentPacking(std::vector<int64_t>& input)
{
    std::vector<int64_t> output = input;
    int64_t backwardIterator = output.size() - 1;

    while (backwardIterator >= 0)
    {
        int64_t writeFileID = output[backwardIterator];
        if (writeFileID != -1)
        {
            //find file we want to write off the back of the disk
            int64_t size = CountFileLengthReverse(output, backwardIterator, writeFileID);
            backwardIterator -= size;

            //try and find a place to put this file
            int64_t forwardIterator = 0;
            while (forwardIterator <= backwardIterator)
            {
                //check file
                int64_t fileID = output[forwardIterator];
                int64_t readSize = CountFileLength(output, forwardIterator, backwardIterator + 1, fileID);
                if (fileID == -1 && readSize >= size) //memory is blank and big enough for the file we want
                {
                    //write the file
                    for (int64_t i = 0; i < size; i++)
                    {
                        assert(output[forwardIterator + i] == -1);
                        output[forwardIterator + i] = writeFileID;
                        output[backwardIterator + i + 1] = -1; //backward iterator is off by one because we set it to the block before itself earlier
                    }
                    break;
                }
                else
                {
                    forwardIterator += readSize;
                }
            }
        }
        else
        {
            backwardIterator--;
        }
    }

    

    return output;
}


int64_t Puzzle9::CalculateChecksum(const std::vector<int64_t>&input)
{
    int64_t sum = 0;
    for (int64_t index = 0; index < input.size(); index++)
    {
        if (input[index] != -1)
        {
            sum += (int64_t)input[index] * (int64_t)index;
        }
    }
    return sum;
}

