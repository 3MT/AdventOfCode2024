#include "Puzzle22.h"

#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <deque>
#include <set>

//https://adventofcode.com/2024/day/22
//there are a series of monkeys with an initial secret number, their numbers evolve over time with the following rules:

//In particular, each buyer's secret number evolves into the next secret number in the sequence via the following process:
//
//Calculate the result of multiplying the secret number by 64. Then, mix this result into the secret number.Finally, prune the secret number.
//Calculate the result of dividing the secret number by 32. Round the result down to the nearest integer.Then, mix this result into the secret number.Finally, prune the secret number.
//Calculate the result of multiplying the secret number by 2048. Then, mix this result into the secret number.Finally, prune the secret number.
//
//Each step of the above process involves mixing and pruning:
//
//To mix a value into the secret number, calculate the bitwise XOR of the given value and the secret number.Then, the secret number becomes the result of that operation. (If the secret number is 42 and you were to mix 15 into the secret number, the secret number would become 37.)
//To prune the secret number, calculate the value of the secret number modulo 16777216. Then, the secret number becomes the result of that operation. (If the secret number is 100000000 and you were to prune the secret number, the secret number would become 16113920.)

//after 2000 updates we are to calculate the sum of all the secret numbers.

std::string Puzzle22::SolvePuzzleA(const char* fileName)
{
    std::vector<int64_t> inputs = ParseFile(fileName);
    int64_t sum = 0;

    std::map<std::string, int64_t> diffTotals; //sum of the amount of bananas purchased at a certain diff sequence
    for (int64_t secret : inputs)
    {
        for (int i = 0; i < 2000; i++)
        {
            //Update the secret
            secret = IterateSecret(secret);
        }

        sum += secret;
    }
    
    std::string bestKey = "NONE!";
    int64_t bestTotal = 0;
    for (auto& keyValue : diffTotals)
    {
        if (keyValue.second > bestTotal)
        {
            bestTotal = keyValue.second;
            bestKey = keyValue.first;
        }
    }

    return std::to_string(sum);
}

//https://adventofcode.com/2024/day/22#part2
//same set up as part a but this time we are looking for the best time to buy bananas.
//the number of bananas we get is equal to the ones digit of the monkey we buy from.
//but we can only buy when a specific sequences of 4 differences occurs
//when the sequence we have chosen occurs for the first time on each monkey seller we immediately buy and cannot buy again.
//if we don't see the sequence for a buyer we simply buy nothing.
//the goal of part b is to calculate the best possible sequence and return how many bananas you get in that case.
std::string Puzzle22::SolvePuzzleB(const char* fileName)
{
    std::vector<int64_t> inputs = ParseFile(fileName);
    int64_t sum = 0;

    std::map<std::string, int64_t> diffTotals; //sum of the amount of bananas purchased at a certain diff sequence
    for (int64_t secret : inputs)
    {
        int64_t lastPrice = secret % 10;
        std::deque<int64_t> lastDiffs; //up to last 4 differences
        std::set<std::string> diffsChecked;
        for (int i = 0; i < 2000; i++)
        {
            //work out the diffs and check if we have hit a new 4 diff sequence, if so mark down the price and add it to the total.
            int64_t sellPrice = secret % 10;
            int64_t diff = sellPrice - lastPrice;
            lastPrice = sellPrice;
            lastDiffs.push_back(diff);
            if (lastDiffs.size() >= 4)
            {
                std::string key;
                for (auto iterator = lastDiffs.begin(); iterator < lastDiffs.end(); ++iterator)
                {
                    key.append(std::to_string(*iterator));
                    if (iterator != lastDiffs.end() - 1)
                    {
                        key.append(",");
                    }
                }
                lastDiffs.pop_front();
                if (!diffsChecked.contains(key))
                {
                    diffTotals[key] += sellPrice; //this is the first instance of this sequence for this secret, add it's sellprice to the sequence dict.
                    diffsChecked.insert(key);
                }
            }

            //Update the secret
            secret = IterateSecret(secret);
        }

        sum += secret;
    }

    std::string bestKey = "NONE!";
    int64_t bestTotal = 0;
    for (auto& keyValue : diffTotals)
    {
        if (keyValue.second > bestTotal)
        {
            bestTotal = keyValue.second;
            bestKey = keyValue.first;
        }
    }

#if DEBUG_OUTPUT
    std::cout << bestKey << "\n";
#endif

    return std::to_string(bestTotal);
}

std::vector<int64_t> Puzzle22::ParseFile(const char* fileName)
{
    std::vector<int64_t> numbers;
    std::string line;
    std::ifstream myfile(fileName);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            try
            {
                numbers.push_back(std::stoll(line));
            }
            catch (...)
            {
                std::cout << "Parse failed";
            }
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    return numbers;
}

int64_t Puzzle22::IterateSecret(int64_t secret)
{
    //Calculate the result of multiplying the secret number by 64. Then, mix this result into the secret number.Finally, prune the secret number.
    {
        int64_t other = secret * 64;
        secret = Mix(other, secret);
        secret = Prune(secret);
    }
    //Calculate the result of dividing the secret number by 32. Round the result down to the nearest integer.Then, mix this result into the secret number.Finally, prune the secret number.
    {
        int64_t other = secret / 32;
        secret = Mix(other, secret);
        secret = Prune(secret);
    }
    //Calculate the result of multiplying the secret number by 2048. Then, mix this result into the secret number.Finally, prune the secret number.
    {
        int64_t other = secret * 2048;
        secret = Mix(other, secret);
        secret = Prune(secret);
    }
    return secret;
}

bool Puzzle22::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle22/test.txt");
    std::string resultB = SolvePuzzleB("puzzle22/test2.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "37327623") return false;
    if (resultB != "23") return false;
    return true;
}

int64_t Puzzle22::IterateSecretNumber(int64_t input)
{
    return 0;
}

int64_t Puzzle22::Mix(int64_t input, int64_t input2)
{
    return input ^ input2;
}

int64_t Puzzle22::Prune(int64_t input)
{
    return input % 16777216;
}
