#include "Puzzle11.h"

#include <assert.h>>
#include <fstream>>
#include <iostream>

//https://adventofcode.com/2024/day/11
//given an input sequences of stones with numbers of them, we need to determine their state after several (25) blinks.
//each time we blink, the following rules apply:
    //If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
    //If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
    //If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
std::string Puzzle11::SolvePuzzleA(const char* fileName)
{
    std::vector<uint64_t> stones;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            try
            {
                size_t index = 0;
                while (index < std::string::npos)
                {
                    index = 0;
                    stones.push_back(std::stoll(line, &index));
                    index++;

                    //parsing line done
                    if (index >= line.length())
                        break;

                    //consume used input
                    line = line.substr(index);
                }
            }
            catch (...)
            {
                std::cout << "Parse failed";
            }
        }
    }

    for (int64_t i = 0; i < 25; i++)
    {
        stones = NaiveIterateStones(stones);
    }

    //experiment: see how many int types there are
    // I'm wondering if there is a simple mathematical solution rather than needing to calculate each stone type.
    // if the number of stone cycles is low then perhaps it's modelable?
    // 
    // update: looking at the non test input it seems to cap at 3811 unique stone numbers (at least for my puzzle input)
    // given how many variants there are and how each one may behave differently I'm moderately convinced there isn't a simple way to convert this into a straightforward
    // mathematical formula, there might be something more clever than the map solution I found for part b but I'd wager my solution is at least Pretty Good(tm)
    // there are definitely cycles that occur but there are so many different cycles that modeling them becomes cumbersome pretty quickly and I have a feeling it won't realistically do better than
    // the map approach.
    // 
    // update3: looked into what people were doing online and seems like my approach is the intended answer and my code benchmarks pretty well compared to others so I'm giving myself a gold star.
    // some people supposedly used memoisation but i don't think that helps at all? the expensive operation is dealing with the volume of stones, not the actual operation we're doing (where memoisation would be useful)
    // that said I haven't seen a problem yet that made me think of memoisation so maybe I should try and give it a crack for one of the later problems.
    // 
    //std::set<uint64_t> uniqueInts;
    //for (uint64_t stone : stones)
    //{
    //    uniqueInts.insert(stone);
    //}

	return std::to_string(stones.size());
}

//https://adventofcode.com/2024/day/11#part2
//as part 1, but with 75 blinks, scaling up the exponential growth to an insane degree meaning we need a more efficient approach.
std::string Puzzle11::SolvePuzzleB(const char* fileName)
{
    std::unordered_map<uint64_t, uint64_t> stones; //left is stone number, right is current number
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            try
            {
                size_t index = 0;
                while (index < std::string::npos)
                {
                    index = 0;
                    int64_t number = std::stoll(line, &index);
                    stones[number]++;
                    index++;

                    //parsing line done
                    if (index >= line.length())
                        break;

                    //consume used input
                    line = line.substr(index);
                }
            }
            catch (...)
            {
                std::cout << "Parse failed";
            }
        }
    }

    for (int64_t i = 0; i < 75; i++)
    {
        stones = IterateMapStones(stones);
    }
    uint64_t sum = 0;
    for (auto& keyvalue : stones)
    {
        sum += keyvalue.second;
    }
    return std::to_string(sum);
}

bool Puzzle11::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle11/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "55312") return false;
    //no test for part B, when I played around with implementation I did set the depth to 25 to ensure it matched the results for A however.
    return true;
}



//this performs much much better but I wonder if there is just a mathematical formula we can use? (see update2 earlier for results on this thinkies)
std::unordered_map<uint64_t, uint64_t> Puzzle11::IterateMapStones(const std::unordered_map<uint64_t, uint64_t>& inputStones)
{
    std::unordered_map<uint64_t, uint64_t> stones; //left is stone number, right is current number
    for (auto& keyvalue : inputStones)
    {
        uint64_t stoneNumber = keyvalue.first;
        uint64_t stoneCount = keyvalue.second;
        //* If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
        if (stoneNumber == 0)
        {
            stones[1] += stoneCount;
            continue;
        }
        else
        {
            //* If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
            
            //may be accuracy issues for huge numbers.
            int64_t digits = (int64_t)(std::log10l((long double)stoneNumber)) + 1;
            if (digits % 2 == 0)
            {
                uint64_t left = stoneNumber / (std::pow(10, digits / 2));
                uint64_t right = stoneNumber - (left * (std::pow(10, digits / 2)));
                stones[left] += stoneCount;
                stones[right] += stoneCount;
                continue;
            }
        }
        //* If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
        stones[stoneNumber * 2024] += stoneCount;

    }

    return stones;
}

std::vector<uint64_t> Puzzle11::NaiveIterateStones(const std::vector<uint64_t>& inputStones)
{
    std::vector<uint64_t> stones;
    stones.reserve(inputStones.size() * 2); //a bit aggressive but guaranteed to be enough space
    for (uint64_t stone : inputStones)
    {
        //* If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
        if (stone == 0)
        {
            stones.push_back(1);
            continue;
        }
        else
        { 
            //* If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
            
            //may be accuracy issues for huge numbers.
            int64_t digits = (int64_t)(std::log10l((long double)stone)) + 1;
            if (digits % 2 == 0)
            {
                uint64_t left = stone / (std::pow(10, digits / 2));
                uint64_t right = stone - (left * (std::pow(10, digits / 2)));
                stones.push_back(left);
                stones.push_back(right);
                continue;
            }
        }
        //* If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
        stones.push_back(stone * 2024);
    }
    return stones;
}
